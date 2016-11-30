/*
 *  This file is part of mrim-prpl.
 *
 *  mrim-prpl is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  mrim-prpl is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with mrim-prpl.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "mrim.h"
#include "cl.h"
#include "message.h"

/* Plugin loading and unloading */

static gboolean plugin_load(PurplePlugin *plugin) {
	return TRUE;
}

static gboolean plugin_unload(PurplePlugin *plugin) {
	g_free(moods);
	g_free(mrim_user_agent);
	return TRUE;
}

static void plugin_destroy(PurplePlugin *plugin) {

}


typedef void (*GcFunc)(PurpleConnection *from,
					   PurpleConnection *to,
					   gpointer userdata);

typedef struct {
	GcFunc fn;
	PurpleConnection *from;
	gpointer userdata;
} GcFuncData;


void discover_status(PurpleConnection *from, PurpleConnection *to, gpointer userdata) {
	const char *from_username = from->account->username;
	const char *to_username = to->account->username;

	purple_debug_info("steem-prpl", "%s checks status of %s\n", from_username, to_username);
	purple_prpl_got_user_status(from->account, to_username, STEEM_STATUS_ONLINE, NULL);
}

static void call_if_steemprpl(gpointer data, gpointer userdata) {
	PurpleConnection *gc = (PurpleConnection *)(data);
	GcFuncData *gcfdata = (GcFuncData *)userdata;

	if (!strcmp(gc->account->protocol_id, MRIM_PRPL_ID))	// TODO: MRIM_PRPL_ID -> STEEM_PRPL_ID
		gcfdata->fn(gcfdata->from, gc, gcfdata->userdata);
}

static void foreach_steemprpl_gc(GcFunc fn, PurpleConnection *from, gpointer userdata) {
	GcFuncData gcfdata = { fn, from, userdata };
	g_list_foreach(purple_connections_get_all(), call_if_steemprpl, &gcfdata);
}

static void steem_login(PurpleAccount *account) {
	purple_debug_info("steem-prpl", "[%s]\n", __func__);
	g_return_if_fail(account != NULL);
	PurpleConnection *gc = purple_account_get_connection(account);
	purple_connection_set_state(gc, PURPLE_CONNECTED);

	foreach_steemprpl_gc(discover_status, gc, NULL);
}

static void mrim_close(PurpleConnection *gc) {
}

void mrim_add_ack_cb(MrimData *mrim, guint32 seq, void (*func)(MrimData *, gpointer, MrimPackage *), gpointer data) {
	MrimAck *ack = g_new(MrimAck, 1);
	ack->seq = seq;
	ack->func = func;
	ack->data = data;
	g_hash_table_insert(mrim->acks, GUINT_TO_POINTER(seq), ack);
}

GList *mrim_prpl_actions(PurplePlugin *plugin, gpointer context) {
	purple_debug_info("mrim-prpl", "[%s]\n", __func__);
	GList *actions = NULL;
	return actions;
}

/* Chat config */
static GList *mrim_chat_info(PurpleConnection *gc)
{
	purple_debug_info("mrim-prpl", "[%s]\n", __func__);
	GList *list = NULL;
	struct proto_chat_entry *pce;

	pce = g_new0(struct proto_chat_entry, 1);
	pce->label = _("_Room:");
	pce->identifier = "room";
	pce->required = TRUE;
	list = g_list_append(list, pce);

	return list; /* config options GList */
}

GHashTable *mrim_chat_info_defaults(PurpleConnection *gc, const char *chat_name)
{
	purple_debug_info("mrim-prpl", "[%s] chat_name %s\n", __func__, chat_name);

	GHashTable *defaults;
	defaults = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, g_free);
	g_hash_table_insert(defaults, "room", g_strdup(chat_name));
	return defaults;
}

/* Some other functions */

static gboolean mrim_offline_message(const PurpleBuddy *buddy) {
	return TRUE;
}

static const char *mrim_list_icon(PurpleAccount *account, PurpleBuddy *buddy) {
	return "steem";
}

/* Purple plugin structures */

static PurplePluginProtocolInfo prpl_info = { //OPT_PROTO_CHAT_TOPIC
	OPT_PROTO_MAIL_CHECK, /* options */
	NULL,			/* user_splits */
	NULL,			/* protocol_options */
	{			/* icon_spec */
		"png,gif,jpg",			/* format */
		0,				/* min_width */
		0,				/* min_height */
		128,				/* max_width */
		128,				/* max_height */
		10240,				/* max_filesize */
		PURPLE_ICON_SCALE_DISPLAY	/* scale_rules */
	},
	mrim_list_icon,		/* list_icon */
	NULL,					/* list_emblem */
	mrim_status_text,	/* status_text */
	mrim_tooltip_text,	/* tooltip_text */
//	mrim_status_types,	/* status_types */
	steem_status_types,
	mrim_user_actions,	/* user_actions */
	mrim_chat_info,		/* chat_info */
	mrim_chat_info_defaults,/* chat_info_defaults */
	steem_login,		/* login */
	mrim_close,		/* close */
	mrim_send_im,		/* send_im */
	NULL,			/* set_info */
	mrim_send_typing,	/* send_typing */
	mrim_get_info,		/* get_info */
	mrim_set_status,	/* set_status */
	NULL,			/* set_idle */
	NULL,			/* change_password */
	mrim_add_buddy,		/* add_buddy */
	NULL,			/* add_buddies */
	mrim_remove_buddy,	/* remove_buddy */
	NULL,			/* remove_buddies */
	NULL,			/* add_permit */
	NULL,			/* add_deny */
	NULL,			/* rem_permit */
	NULL,			/* rem_deny */
	NULL,			/* set_permit_deny */
	mrim_chat_join,	/* chat_join */
	NULL,			/* reject_chat */
	NULL,			/* get_chat_name */
	mrim_chat_invite,/* chat_invite */
	mrim_chat_leave,/* chat_leave */
	NULL,			/* whisper */
	mrim_chat_send,	/* chat_send */
	NULL,			/* keep_alive */
	NULL,			/* register_user */
	NULL,			/* get_cb_info */
	NULL,			/* get_cb_away */
	mrim_alias_buddy,	/* alias_buddy */
	mrim_move_buddy,	/* move_buddy */
	mrim_rename_group,	/* rename_group */
	mrim_free_buddy,	/* free_buddy */
	NULL,			/* convo_closed */
	mrim_normalize,		/* normalize */
	NULL,			/* set_buddy_icon */
	mrim_remove_group,	/* remove_group */
	NULL,			/* get_cb_real_name */
	NULL,			/* set_chat_topic */
	NULL,			/* find_blist_chat */
	NULL,			/* roomlist_get_list */
	NULL,			/* roomlist_cancel */
	NULL,			/* roomlist_expand_category */
	NULL,
	NULL,
	NULL,
	mrim_offline_message,	/* mrim_offline_message */
	NULL,			/* whiteboard_prpl_ops */
	NULL,			/* send_raw */
	NULL,			/* roomlist_room_serialize */
	NULL,			/* unregister_user */
	mrim_send_attention,	/* send_attention */
	NULL,			/* get_attention_types */
	sizeof(PurplePluginProtocolInfo),/* struct_size */
	NULL,					/* get_account_text_table */
	NULL,					/* initiate_media */
	NULL,					/* get_media_caps */
	mrim_get_moods,				/* get_moods */
	NULL,					/* set_public_alias */
	NULL					/* get_public_alias */
	#if PURPLE_MAJOR_VERSION >= 2 && PURPLE_MINOR_VERSION >= 8
	,NULL,					/* add_buddy_with_invite */
	NULL					/* add_buddies_with_invite */
	#endif
};

static PurplePluginInfo plugin_info = {
	PURPLE_PLUGIN_MAGIC,
	PURPLE_MAJOR_VERSION,
	PURPLE_MINOR_VERSION,
	PURPLE_PLUGIN_PROTOCOL,
	NULL,
	0,
	NULL,
	PURPLE_PRIORITY_DEFAULT,
	MRIM_PRPL_ID,
	"Steem",
	PROJECT_VERSION,
	"Steem",
	"Steem",
	"@noisy - Krzysztof Szumny <noisy.pl@gmail.com>",
	"https://github.com/noisy/steem-purple",
	plugin_load,		/* plugin_load */
	plugin_unload,		/* plugin_unload */
	plugin_destroy,		/* plugin_destroy */
	NULL,
	&prpl_info,		/* extra_info */
	NULL,
	mrim_prpl_actions,	/* plugin_actions */
	NULL,
	NULL,
	NULL,
	NULL
};

static void init_plugin(PurplePlugin *plugin) {
	purple_debug_info("mrim-prpl", "Starting up Steem\n");
	mrim_plugin = plugin;
}

PURPLE_INIT_PLUGIN(mrim_prpl, init_plugin, plugin_info)
