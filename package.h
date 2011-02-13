#ifndef PACKAGE_H
 #define PACKAGE_H
 #include "proto.h"
 #include "mrim.h"
 #include <stdarg.h>           /* для va_ макросов */

#define PACK_MAX_LEN 65536
#define CHUNK 16384

#ifdef WIN32
	#define RECV_FLAGS 0
#else
	#define RECV_FLAGS MSG_WAITALL
#endif

typedef struct
{
	mrim_packet_header_t *header;
	char *buf;// указатель на начало буфера
	char *cur;// указатель на ещё не считанные данные
//	char *end;// указатель на последний элемент буфера
	u_int len;// длина буффера
}package;

package *new_package(guint32 seq,guint32 type);
void add_ul(guint32 ul, package *pack);
void add_LPS(gchar *string, package *pack);
void add_raw(char *new_data, int len, package *pack);
void add_RTF(gchar *string, package *pack);
void add_base64(package *pack, gboolean gziped, gchar *fmt, ...);

gboolean send_package(package *pack, mrim_data *mrim);

package *read_package(mrim_data *mrim);
guint32 read_UL(package *pack);
gchar *read_rawLPS(package *pack);
gchar *read_LPS(package *pack);
gchar *read_UTF16LE(package *pack);
gchar *read_Z(package *pack);
void read_base64(package *pack, gboolean gziped, gchar *fmt, ...);

void free_package(package *pack);

void mrim_packet_dump(package *pack);
#endif
