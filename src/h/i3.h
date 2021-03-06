/****************************************************************************
 *                   ^     +----- |  / ^     ^ |     | +-\                  *
 *                  / \    |      | /  |\   /| |     | |  \                 *
 *                 /   \   +---   |<   | \ / | |     | |  |                 *
 *                /-----\  |      | \  |  v  | |     | |  /                 *
 *               /       \ |      |  \ |     | +-----+ +-/                  *
 ****************************************************************************
 * AFKMud Copyright 1997-2006 by Roger Libiez (Samson),                     *
 * Levi Beckerson (Whir), Michael Ward (Tarl), Erik Wolfe (Dwip),           *
 * Cameron Carroll (Cam), Cyberfox, Karangi, Rathian, Raine, and Adjani.    *
 * All Rights Reserved.                                                     *
 * Registered with the United States Copyright Office. TX 5-877-286         *
 *                                                                          *
 * External contributions from Xorith, Quixadhal, Zarius, and many others.  *
 *                                                                          *
 * Original SMAUG 1.4a written by Thoric (Derek Snider) with Altrag,        *
 * Blodkai, Haus, Narn, Scryn, Swordbearer, Tricops, Gorog, Rennard,        *
 * Grishnakh, Fireblade, and Nivek.                                         *
 *                                                                          *
 * Original MERC 2.1 code by Hatchet, Furey, and Kahn.                      *
 *                                                                          *
 * Original DikuMUD code by: Hans Staerfeldt, Katja Nyboe, Tom Madsen,      *
 * Michael Seifert, and Sebastian Hammer.                                   *
 ****************************************************************************
 *                       Intermud-3 Network Module                          *
 ****************************************************************************/

/*
 * Copyright (c) 2000 Fatal Dimensions
 *
 * See the file "LICENSE" or information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 */

/* Ported to Smaug 1.4a by Samson of Alsherok.
 * Consolidated for cross-codebase compatibility by Samson of Alsherok.
 * Modifications and enhancements to the code
 * Copyright (c)2001-2003 Roger Libiez ( Samson )
 * Registered with the United States Copyright Office
 * TX 5-562-404
 */

#ifndef __I3_H__
#define __I3_H__

/* Number of messages to store in the channel history */
#define MAX_I3HISTORY 50
#define MAX_I3TELLHISTORY 50

/* Locations of the configuration files */
/* Remcon: Ask and ye shall receive. */
#define I3_DIR           "i3/"
#ifdef I3_WILEY
#define PLAYER_DIR       "players/"
#endif
#define HTTP_PLACEHOLDER        "http://6dragons.tk/"
#define MAXPLAYERS_PLACEHOLDER  0
#define NUMLOGINS_PLACEHOLDER   0
#define RFC1123FMT              "%a, %d %b %Y %H:%M:%S %Z"
#define CODETYPE                "DikuMUD"
#define CODEBASE                "Chronicles1.2a"
#define CODEVERSION             "1.9q1"

#define I3_CONFIG_FILE   I3_DIR "i3.config"
#define I3_CHANNEL_FILE  I3_DIR "i3.channels"
#define I3_PASSWORD_FILE I3_DIR "i3.password"
#define I3_UCACHE_FILE   I3_DIR "i3.ucache"
#define I3_MUDLIST_FILE  I3_DIR "i3.mudlist"
#define I3_CHANLIST_FILE I3_DIR "i3.chanlist"
#define I3_BAN_FILE      I3_DIR "i3.bans"
#define I3_COLOR_FILE    I3_DIR "i3.color"
#define I3_HELP_FILE     I3_DIR "i3.help"
#define I3_CMD_FILE      I3_DIR "i3.commands"
#define I3_ROUTER_FILE   I3_DIR "i3.routers"

/* Make sure you've examined the i3cfg.h file and set your information accordingly!
 * If you don't, things below this point will fail - Samson 7-15-03 
 */
#include "i3cfg.h"

typedef enum
{
  I3PERM_NOTSET, I3PERM_NONE, I3PERM_MORT, I3PERM_IMM, I3PERM_ADMIN, I3PERM_IMP
} permissions;

/* Flag macros */
#define I3IS_SET(flag, bit)	((flag) & (bit))
#define I3SET_BIT(var, bit)	((var) |= (bit))
#define I3REMOVE_BIT(var, bit)((var) &= ~(bit))

/* Channel flags, only one so far, but you never know when more might be useful */
#define I3CHAN_LOG      (1 <<  0)

/* Player flags */
#define I3_TELL         (1 <<  0)
#define I3_DENYTELL     (1 <<  1)
#define I3_BEEP         (1 <<  2)
#define I3_DENYBEEP     (1 <<  3)
#define I3_INVIS        (1 <<  4)
#define I3_PRIVACY      (1 <<  5)
#define I3_DENYFINGER   (1 <<  6)
#define I3_AFK          (1 <<  7)
#define I3_COLORFLAG    (1 <<  8)
#define I3_PERMOVERRIDE (1 <<  9)

/* You should not need to edit anything below this line if I've done this all correctly. */

/* The current revision of the I3 code */
#define I3DRIVER "I3 Driver 2.40b"

/* IPS: Inbound packet size.
 * OPS: Outbound packet size.
 * You would be well advised not to mess with these things or Bad Things(tm) will happen to you.
 */
#define IPS 131072
#define OPS 65536

#define I3PERM(ch)            (CH_I3DATA((ch))->i3perm)
#define I3FLAG(ch)            (CH_I3DATA((ch))->i3flags)
#define FIRST_I3IGNORE(ch)    (CH_I3DATA((ch))->i3first_ignore)
#define LAST_I3IGNORE(ch)     (CH_I3DATA((ch))->i3last_ignore)
#define I3LISTEN(ch)          (CH_I3DATA((ch))->i3_listen)
#define I3DENY(ch)            (CH_I3DATA((ch))->i3_denied)
#define I3REPLY(ch)           (CH_I3DATA((ch))->i3_replyname)
#define I3TELLHISTORY(ch,x)   (CH_I3DATA((ch))->i3_tellhistory[(x)])
#define I3INVIS(ch)           ( I3IS_SET( I3FLAG((ch)), I3_INVIS ) )
#define CH_I3AFK(ch)          ( I3IS_SET( I3FLAG((ch)), I3_AFK ) )
#define I3ISINVIS(ch)         ( I3INVIS((ch)) )

#define LGST 4096 /* Large String */
#define SMST 1024 /* Small String */

/* Macro taken from DOTD codebase. Fcloses a file, then nulls its pointer for safety. */
#define I3FCLOSE(fp)  do { fclose((fp)); (fp)=NULL; } while(0)

/*
 * Memory allocation macros.
 */
#define I3CREATE(result, type, number)                            \
do                                                                \
{                                                                 \
    if (!((result) = (type *) calloc ((number), sizeof(type))))   \
    {                                                             \
	i3_log( "Malloc failure @ %s:%d\n", __FILE__, __LINE__ );    \
	abort();                                                    \
    }                                                             \
} while(0)

#define I3DISPOSE(point) \
do                       \
{                        \
   if((point))           \
   {                     \
	free((void *)(point));     \
	(point) = NULL;    \
   }                     \
} while(0)

#define I3STRALLOC strdup
#define I3STRFREE I3DISPOSE

/* double-linked list handling macros -Thoric ( From the Smaug codebase ) */
/* Updated by Scion 8/6/1999 */
#define I3LINK(link, first, last, next, prev)   \
do                                              \
{                                               \
   if ( !(first) )                              \
   {                                            \
      (first) = (link);                         \
      (last) = (link);                          \
   }                                            \
   else                                         \
      (last)->next = (link);                    \
   (link)->next = NULL;                         \
   if ((first) == (link))                       \
      (link)->prev = NULL;                      \
   else                                         \
      (link)->prev = (last);                    \
   (last) = (link);                             \
} while(0)

#define I3INSERT(link, insert, first, next, prev)     \
do                                                    \
{                                                     \
   (link)->prev = (insert)->prev;                     \
   if ( !(insert)->prev )                             \
      (first) = (link);                               \
   else                                               \
      (insert)->prev->next = (link);                  \
   (insert)->prev = (link);                           \
   (link)->next = (insert);                           \
} while(0)

#define I3UNLINK(link, first, last, next, prev) \
do                                              \
{                                               \
   if ( !(link)->prev )                         \
   {                                            \
      (first) = (link)->next;                   \
	if((first))                               \
	   (first)->prev = NULL;                  \
   }                                            \
   else                                         \
   {                                            \
      (link)->prev->next = (link)->next;        \
   }                                            \
   if( !(link)->next )                          \
   {                                            \
      (last) = (link)->prev;                    \
	if((last))                                \
	   (last)->next = NULL;                   \
   }                                            \
   else                                         \
   {                                            \
      (link)->next->prev = (link)->prev;        \
   }                                            \
} while(0)

/* Internal structures */
typedef struct I3_channel I3_CHANNEL;
typedef struct I3_mud   I3_MUD;
typedef struct I3_header I3_HEADER;
typedef struct I3_ignore I3_IGNORE;
typedef struct I3_ban   I3_BAN;
typedef struct ucache_data UCACHE_DATA;
typedef struct i3_chardata I3_CHARDATA;
typedef struct router_data ROUTER_DATA;
typedef struct i3_color_table I3_COLOR; /* The Color config */
typedef struct i3_command_table I3_CMD_DATA;  /* Command table */
typedef struct i3_help_table I3_HELP_DATA;  /* Help table */
typedef struct i3_cmd_alias I3_ALIAS; /* Big, bad, bloated command alias thing */

typedef void            I3_FUN(CHAR_DATA *ch, const char *argument);

#define I3_CMD( name ) void (name)( CHAR_DATA *ch, const char *argument )

extern int              I3_socket;

extern I3_MUD          *first_mud;
extern I3_MUD          *this_i3mud;
extern char            *I3_ROUTER_NAME;

/* Oh yeah, baby, that raunchy looking Merc structure just got the facelift of the century.
 * Thanks to Thoric and friends for the slick idea.
 */
struct i3_cmd_alias
{
  I3_ALIAS               *next;
  I3_ALIAS               *prev;
  char                   *name;
};

struct i3_command_table
{
  I3_CMD_DATA            *next;
  I3_CMD_DATA            *prev;
  I3_ALIAS               *first_alias;
  I3_ALIAS               *last_alias;
  I3_FUN                 *function;
  char                   *name;
  int                     level;
  bool                    connected;
};

struct i3_help_table
{
  I3_HELP_DATA           *next;
  I3_HELP_DATA           *prev;
  char                   *name;
  char                   *text;
  int                     level;
};

struct i3_color_table
{
  I3_COLOR               *next;
  I3_COLOR               *prev;
  char                   *name; /* the name of the color */
  char                   *mudtag; /* What the mud uses for the raw tag */
  char                   *i3tag;  /* This client's internal code that represents the
                                   * mudtag to the network */
  char                   *i3fish; /* The Pinkfish code for this color - bleh at having to 
                                   * do this twice! */
};

struct router_data
{
  ROUTER_DATA            *next;
  ROUTER_DATA            *prev;
  char                   *name;
  char                   *ip;
  int                     port;
  int                     reconattempts;
};

struct ucache_data
{
  UCACHE_DATA            *next;
  UCACHE_DATA            *prev;
  char                   *name;
  int                     gender;
  time_t                  time;
};

struct I3_ignore
{
  I3_IGNORE              *next;
  I3_IGNORE              *prev;
  char                   *name;
};

struct I3_ban
{
  I3_BAN                 *next;
  I3_BAN                 *prev;
  char                   *name;
};

struct i3_chardata
{
  I3_IGNORE              *i3first_ignore; /* List of people to ignore stuff from - Samson 
                                           * 2-7-01 */
  I3_IGNORE              *i3last_ignore;
  char                   *i3_replyname; /* Target for reply - Samson 1-23-01 */
  char                   *i3_listen;  /* The I3 channels someone is listening to - Samson 
                                       * 1-30-01 */
  char                   *i3_denied;  /* The I3 channels someone is forbidden to use -
                                       * Samson 6-16-03 */
  char                   *i3_tellhistory[MAX_I3TELLHISTORY];  /* History of received
                                                               * i3tells - Samson 1-21-04 
                                                               */
  int                     i3flags;  /* Flag settings such as invis, tell on/off, beep
                                     * on/off, etc. - Samson 6-30-03 */
  int                     i3perm; /* Your permission setting. None, All, Imm, Admin, Imp
                                   * - Samson 6-25-03 */
};

struct I3_header
{
  char                    originator_mudname[SMST];
  char                    originator_username[SMST];
  char                    target_mudname[SMST];
  char                    target_username[SMST];
};

struct I3_channel
{
  I3_CHANNEL             *next;
  I3_CHANNEL             *prev;
  char                   *local_name;
  char                   *host_mud;
  char                   *I3_name;
  char                   *layout_m;
  char                   *layout_e;
  char                   *history[MAX_I3HISTORY];
  int                     status;
  int                     i3perm;
  long                    flags;
};

struct I3_mud
{
  I3_MUD                 *next;
  I3_MUD                 *prev;

  /*
   * Stuff for the first mapping set 
   */
  int                     status;
  char                   *name;
  char                   *ipaddress;
  char                   *mudlib;
  char                   *base_mudlib;
  char                   *driver;
  char                   *mud_type;
  char                   *open_status;
  char                   *admin_email;
  char                   *telnet;
  char                   *web_wrong;  /* This tag shows up in the wrong location on
                                       * several implementations, including previous
                                       * AFKMud versions */
  int                     player_port;
  int                     imud_tcp_port;
  int                     imud_udp_port;

  bool                    tell;
  bool                    beep;
  bool                    emoteto;
  bool                    who;
  bool                    finger;
  bool                    locate;
  bool                    channel;
  bool                    news;
  bool                    mail;
  bool                    file;
  bool                    auth;
  bool                    ucache;

  int                     smtp;
  int                     ftp;
  int                     nntp;
  int                     http;
  int                     pop3;
  int                     rcp;
  int                     amrcp;

  /*
   * Stuff for the second mapping set - can be added to as indicated by i3_log messages for missing keys 
   */
  char                   *banner;
  char                   *web;
  char                   *time;
  char                   *daemon;
  int                     jeamland;

  /*
   * only used for this mud 
   */
  char                   *routerName;
  bool                    autoconnect;
  int                     password;
  int                     mudlist_id;
  int                     chanlist_id;
  int                     minlevel;
  int                     immlevel;
  int                     adminlevel;
  int                     implevel;
};

/* External hooks */
bool                    i3_is_connected(void);
size_t                  i3_strlcpy(char *dst, const char *src, size_t siz);
size_t                  i3_strlcat(char *dst, const char *src, size_t siz);

const char             *i3_one_argument(const char *argument, char *arg_first);
void                    i3_loop(void);
bool                    i3_load_char(CHAR_DATA *ch, FILE * fp, const char *word);
void                    i3_save_char(CHAR_DATA *ch, FILE * fp);
void                    i3_free_chardata(CHAR_DATA *ch);
void                    i3_free_data(bool complete);
void                    i3_init_char(CHAR_DATA *ch);
bool                    i3_command_hook(CHAR_DATA *ch, const char *command, const char *argument);

void                    i3_startup(bool forced, int mudport, bool isconnected);
void                    i3_shutdown(int delay);

void                    i3_load_mudlist(void);
void                    i3_load_chanlist(void);
void                    i3_save_mudlist(void);
void                    i3_save_chanlist(void);
void                    i3_save_history(void);

void                    i3_listen_channel(CHAR_DATA *ch, const char *argument);
bool                    i3_has_name(char *list, const char *name);
void                    i3_npc_chat(const char *chan_name, const char *actor, const char *message);
#endif
