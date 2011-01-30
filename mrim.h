#ifndef MRIM_H
 #define MRIM_H

#include "config.h"
#include <glib.h>
#include <time.h>

// i18n see
#define GETTEXT_PACKAGE "mrim-prpl"
#define LOCALEDIR "po"
#include <glib/gi18n-lib.h>


#ifdef WIN32
	#include <winsock2.h>
	#include <windows.h>
	#include <lm.h>
	//#include "dns_sd_proxy.h"
	// В виндовсах до Win2003 может отсутствовать.
	#ifndef MSG_WAITALL
		#define MSG_WAITALL    0x08
	#endif
	#define ENOTCONN 107 
#else 
	#include <sys/socket.h>
	#include <netinet/in.h> 
	#include <arpa/inet.h>
#endif

#ifdef ZLIB
	#include <zlib.h>
#endif

#ifdef DEBUG
#endif


// read/write
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h> // g_memmove
#include <unistd.h> // функции Read/Write

// libpurple
#include "account.h"
#include "accountopt.h"
#include "blist.h"
#include "buddyicon.h"
#include "cipher.h"
#include "cmds.h"
#include "conversation.h"
#include "connection.h"
#include "debug.h"
#include "dnsquery.h"
#include "dnssrv.h"
#include "debug.h"
#include "network.h"
#include "notify.h"
#include "plugin.h"
#include "privacy.h"
#include "proxy.h"
#include "prpl.h"
#include "request.h"
#include "status.h"
#include "util.h"
#include "version.h"

//mrim
#include "proto.h"
#include <sys/types.h>


static PurplePlugin *_mrim_plugin = NULL;

/* statuses (reference: libpurple/status.h) */
static struct status
{
	PurpleStatusPrimitive primative;
	guint32	mrim_status;
	const char*	id;
	const char*	name;
	gboolean user_settable;
	gboolean independent;
} mrim_statuses[] = {
	/*	primative,					mrim_status,				id,				name(i18n)			user_settable 		independent  			 */
	{	PURPLE_STATUS_INVISIBLE,	STATUS_FLAG_INVISIBLE,	"invisible",		N_("invisible"),		TRUE,				FALSE},	/* 0 */
	{	PURPLE_STATUS_OFFLINE,		STATUS_OFFLINE,			"offline",			N_("Offline"),			TRUE,				FALSE},	/* 1 */
	{	PURPLE_STATUS_AVAILABLE,	STATUS_ONLINE,			"online",			N_("Available"),		TRUE,				FALSE},	/* 2 */
	{	PURPLE_STATUS_AWAY,			STATUS_AWAY,			"away",				N_("Away"), 			TRUE,				FALSE}	/* 3 */
//	{	PURPLE_STATUS_UNSET,		STATUS_UNDETERMINATED,	"UNDETERMINATED",	"UNDETERMINATED",	TRUE,				FALSE}	/* 4 */
};
#define MRIM_STATUS_ID_MOBILE "mobile"
#define STATUSES_COUNT 4

#define MRIM_MAX_ERROR_COUNT 30

#define MRIM_PRPL_ID "prpl-ostin-mrim" // какой purple-id теперь?
#define MRIM "mrim"
// DISPLAY_VERSION in config.h
#define MRIM_MAIL_RU "mrim.mail.ru"
#define MRIM_MAIL_RU_PORT 2042
					// или 443
#define USER_AGENT_DESC "client=\"Pidgin\" version=" DISPLAY_VERSION
#define USER_AGENT "Mail.Ru Pidgin plugin by Ostin"
#define FREE(s) { if (s) g_free(s); s = NULL;}

#define FEATURES (FEATURE_FLAG_WAKEUP | FEATURE_FLAG_BASE_SMILES)

typedef struct
{
	PurpleGroup *gr;
	char *name;
	guint32 id;
	guint32 flag;
}mrim_group;

/*
typedef enum
{
	BUDDY,
	GROUP,
	PHONE, // phone only
	CHAT
}BUDDY_TYPE;
* */
typedef struct
{
	PurpleBuddy *buddy;
	gchar *addr;
	gchar *alias;
	gchar **phones;
	guint32 group_id;
	guint32 id;
	gboolean authorized;
	guint32 flags;
	guint32 s_flags;
	guint32 status;
	/*BUDDY_TYPE type;*/
	gchar *ips; // IP:PORT;IP:PORT;
}mrim_buddy;

typedef struct {
	PurpleConnection *gc;
	PurpleAccount *account;
	char *username;
	char *password;
	guint32 status;		  // status в формате Маил.Агента
	gchar *server;        // IP сервера
	int port;             // port на сервере
	int fd;               // дескриптор сокета
	guint32 seq;           // номер текущего пакета
	gpointer inp_package; // буфер для чтения
	int error_count;      // количество подряд идущих ошибок чтения пакетов

	guint keep_alive_handle;
	guint32 kap_count;
	PurpleUtilFetchUrlData *FetchUrlHandle;
	PurpleProxyConnectData *ProxyConnectHandle;

	// Почта
	guint32 mails;         // количество писем
	gchar *web_key;        // ключ веб-авторизации
	gchar *url;            // url
	// PQ
	GHashTable *pq;        // "очередь" сообщений (pending queue)
	GHashTable *mg;        // хеш-таблица групп
	GList *xfer_lists;     // список всех файлов для передачи

}mrim_data;

typedef struct
{
	char *from;
	mrim_data *mrim;
	guint32 seq;
}auth_data;


/******************************************
 *               PQ
 ******************************************/
typedef enum
{
	ADD_BUDDY,
	ADD_GROUP,
	RENAME_GROUP,
	REMOVE_BUDDY,
	REMOVE_GROUP,
	MOVE_BUDDY,
	MESSAGE,
    ANKETA_INFO,
    SMS,
    MODIFY_BUDDY,
    NEW_EMAIL,
    NEW_EMAILS,
    OPEN_URL,
    SEARCH,
    AVATAR
}PQ_TYPE;

typedef struct
{
	guint32 seq;
	guint32 kap_count;
	PQ_TYPE type; // тип действия. Какие данные хранятся в объединении
	union
	{
		struct
		{	
			PurpleBuddy *buddy;
			PurpleGroup *group;
			gboolean authorized;
			gboolean group_exsist;
		}add_buddy;
		struct
		{
			gchar *name;
		}add_group;
		struct
		{
			char *buddy_name;
			char *new_group;
		}move_buddy;
		struct
		{
			PurpleGroup *new_group;
		}rename_group;
		struct
		{
			gchar *group_name;
			guint32 group_id;
		}remove_group;
		struct
		{
			PurpleBuddy *buddy;
		}remove_buddy;
		struct
		{
			gchar *to;
			gchar *message;
			PurpleMessageFlags flags;
			// TODO rtf
		}message;
		struct
		{
			gchar *phone;
			gchar *message;
		}sms;
		struct
		{
			PurpleBuddy *buddy;
			mrim_buddy *mb;
		}modify_buddy;
		struct
		{
			gchar *from;
			gchar *subject;
		}new_email;
		struct
		{
			guint32 count;
		}new_emails;
		struct
		{
			gchar *url;
		}open_url;
		struct
		{
			gchar *username;
		}anketa_info;
		struct
		{

		}search;
	};
}mrim_pq;


/******************************************
 *               Actions
 ******************************************/
typedef enum
{
	MY_PROFILE=0,
	MY_AVATAR,
	MY_WORLD,
	MY_PHOTO,
	MY_VIDEO,
	MY_BLOG,
	MY_EMAIL,
	MY_MAIL_LISTS,
	MY_QUESTIONS,
    MRIM_WEB_CHAT,
    MY_TRAVEL,
    MY_HOROSCOPE,
    MY_GARAGE,
    MY_GOODS,
    MY_KIDS,
    MY_HEALTH,
    USER_SEARCH
}MRIM_LINKS;


static const gchar *links[]=
{
		// TODO Think of agentlang= suffix change.
		"http://win.mail.ru/cgi-bin/auth?Login=%s&agent=%%s&page=http://win.mail.ru/cgi-bin/userinfo?mra=1&lang=ru&ver=3686&agentlang=ru",
		"http://win.mail.ru/cgi-bin/auth?Login=%s&agent=%%s&page=http://foto.mail.ru/cgi-bin/avatars/lang=ru&ver=3686&agentlang=ru",
		"http://my.mail.ru/%s/%s", /* Мой мир */
		"http://foto.mail.ru/%s/%s", /* Фото */
		"http://video.mail.ru/%s/%s", /* Видео */
		"http://blogs.mail.ru/%s/%s" /* Блоги */
		"http:///otvet.mail.ru/%s/%s/" /* Вопросы и Ответы */
		"http://travel.mail.ru/account/details/" /* путешествия */

};

//char *mrim_status_to_prpl_status(guint32 status);
const char* mrim_status_to_prpl_status( guint32 status );
void set_user_status_by_mb(mrim_data *mrim, mrim_buddy *mb);

static gchar* mrim_message_offline_get_attr(const gchar* attr,void* input);
static time_t mrim_str_to_time(const gchar* str);
static void mrim_input_cb(gpointer data, gint source, PurpleInputCondition cond);
static void mrim_connect_cb(gpointer data, gint source, const gchar *error_message);
static void mrim_balancer_cb(PurpleUtilFetchUrlData *url_data, gpointer user_data, const gchar *url_text, gsize len, const gchar *error_message);
gboolean mrim_keep_alive(gpointer data);

void notify_emails(void *gc, gchar* webkey, guint32 count);

static void blist_search(PurpleConnection *gc, PurpleRequestFields *fields);

void clean_string(gchar *email);
gchar *clear_phone(gchar *phone);
gboolean is_valid_email(gchar *email);
gboolean is_valid_phone(gchar *phone);
#define is_valid_buddy_name(name) (is_valid_phone(name) || is_valid_email(name))

#if PURPLE_MAJOR_VERSION >= 2 && PURPLE_MINOR_VERSION <= 5
	void purple_connection_set_protocol_data(PurpleConnection *connection, void *proto_data);
	void *purple_connection_get_protocol_data(const PurpleConnection *connection);
	gpointer purple_buddy_get_protocol_data(const PurpleBuddy *buddy);
	void purple_buddy_set_protocol_data(PurpleBuddy *buddy, gpointer data);
#endif

#endif
