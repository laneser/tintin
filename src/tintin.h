/******************************************************************************
*   TinTin++                                                                  *
*   Copyright (C) 2004 (See CREDITS file)                                     *
*                                                                             *
*   This program is protected under the GNU GPL (See COPYING)                 *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
*                                                                             *
*   This program is distributed in the hope that it will be useful,           *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
*   GNU General Public License for more details.                              *
*                                                                             *
*   You should have received a copy of the GNU General Public License         *
*   along with this program; if not, write to the Free Software               *
*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA *
*******************************************************************************/

/*********************************************************************/
/* file: tintin.h - the include file for tintin++                    */
/*                             TINTIN ++                             */
/*          (T)he K(I)cki(N) (T)ickin D(I)kumud Clie(N)t             */
/*                    modified by Bill Reiss 1993                    */
/*                     coded by peter unold 1992                     */
/*********************************************************************/

#include <stdio.h>
#include <zlib.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <ctype.h>
#include <stdarg.h>

/******************************************************************************
*   Autoconf patching by David Hedbor, neotron@lysator.liu.se                 *
*******************************************************************************/

#include "config.h"

#if defined(HAVE_STRING_H)
#include <string.h>
#elif defined(HAVE_STRINGS_H)
#include <strings.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_TIME_H
#include <time.h>
#endif

#ifdef TIME_WITH_SYS_TIME
#include <sys/time.h>
#endif

#ifdef SOCKS
#include <socks.h>
#endif

#ifndef BADSIG
#define BADSIG (RETSIGTYPE (*)(int))-1
#endif

#ifdef HAVE_NET_ERRNO_H
#include <net/errno.h>
#endif

#ifndef __TINTIN_H__
#define __TINTIN_H__

/*
	Typedefs
*/

typedef struct listnode LNODE;

typedef struct session *COMMAND (struct session *ses, const char *arg);
typedef struct session *CONFIG  (struct session *ses, char *arg, int index);
typedef struct session *CLASS   (struct session *ses, char *left, char *right);
typedef void CHAT (const char *arg);
typedef struct session *ARRAY   (struct session *ses, LNODE *list, const char *arg);
/*
	A bunch of constants
*/

#define TRUE                    1
#define FALSE                   0

#define SCREEN_WIDTH            80
#define SCREEN_HEIGHT           24

#define PRIORITY                0
#define ALPHA                   1
#define APPEND                  2

#define DEFAULT_OPEN            '{'
#define DEFAULT_CLOSE           '}'

#define HISTORY_FILE              ".tt_history"

#define DEFAULT_CAT_STR           "cat "
#define DEFAULT_COMPRESS_EXT      ".Z"
#define DEFAULT_UNCOMPRESS_STR    "uncompress -c "
#define DEFAULT_GZIP_EXT          ".gz"
#define DEFAULT_GUNZIP_STR        "gunzip -c "
#define DEFAULT_BZIP_EXT          ".bz2"
#define DEFAULT_BUNZIP_STR        "bunzip2 -c "

#define BUFFER_SIZE               10000
#define NUMBER_SIZE                 100

/*
	NOTE: get rid of the DEVELOPMENT warning in main() when you update this!
*/

#define VERSION_NUM "v1.94.0 (BETA)"

/*
	vt102 codes worth defining
*/

#define ESCAPE                         27

#define TELNET_PORT                    23

/*
	Index for lists used by tintin
*/

#define LIST_CONFIG                    0
#define LIST_ALIAS                     1
#define LIST_ACTION                    2
#define LIST_SUBSTITUTE                3
#define LIST_VARIABLE                  4
#define LIST_HIGHLIGHT                 5
#define LIST_ANTISUBSTITUTE            6
#define LIST_FUNCTION                  7
#define LIST_PATH                      8
#define LIST_PATHDIR                   9
#define LIST_TICKER                   10
#define LIST_MACRO                    11
#define LIST_PROMPT                   12
#define LIST_TAB                      13
#define LIST_CLASS                    14
#define LIST_MAX                      15

#define LIST_MATH                     LIST_MAX + 0
#define LIST_DELAY                    LIST_MAX + 1
#define LIST_HISTORY                  LIST_MAX + 2
#define LIST_ALL                      LIST_MAX + 3

#define CLASS_MAX                      5

#define ARRAY_MAX                      5

/*
	Various flags
*/

#define CHAT_NAME_CHANGE                 1
#define CHAT_REQUEST_CONNECTIONS         2
#define CHAT_CONNECTION_LIST             3
#define CHAT_TEXT_EVERYBODY              4
#define CHAT_TEXT_PERSONAL               5
#define CHAT_TEXT_GROUP                  6
#define CHAT_MESSAGE                     7
#define CHAT_DO_NOT_DISTURB              8

#define CHAT_SEND_ACTION                 9
#define CHAT_SEND_ALIAS                 10
#define CHAT_SEND_MACRO                 11
#define CHAT_SEND_VARIABLE              12
#define CHAT_SEND_EVENT                 13
#define CHAT_SEND_GAG                   14
#define CHAT_SEND_HIGHLIGHT             15
#define CHAT_SEND_LIST                  16
#define CHAT_SEND_ARRAY                 17
#define CHAT_SEND_BARITEM               18

#define CHAT_VERSION                    19
#define CHAT_FILE_START                 20
#define CHAT_FILE_DENY                  21
#define CHAT_FILE_BLOCK_REQUEST         22
#define CHAT_FILE_BLOCK                 23     
#define CHAT_FILE_END                   24
#define CHAT_FILE_CANCEL                25
#define CHAT_PING_REQUEST               26 
#define CHAT_PING_RESPONSE              27 
#define CHAT_PEEK_CONNECTIONS           28
#define CHAT_PEEK_LIST                  29
#define CHAT_SNOOP_START                30
#define CHAT_SNOOP_DATA                 31

#define CHAT_END_OF_COMMAND            255

#define CHAT_FLAG_PRIVATE             (1 <<  1)
#define CHAT_FLAG_REQUEST             (1 <<  2)
#define CHAT_FLAG_SERVE               (1 <<  3)
#define CHAT_FLAG_IGNORE              (1 <<  4)
#define CHAT_FLAG_FORWARD             (1 <<  5)
#define CHAT_FLAG_FORWARDBY           (1 <<  6)
#define CHAT_FLAG_FORWARDALL          (1 <<  7)
#define CHAT_FLAG_DND                 (1 <<  8)

#define SUB_NONE                      0
#define SUB_ARG                       (1 <<  0)
#define SUB_VAR                       (1 <<  1)
#define SUB_FUN                       (1 <<  2)
#define SUB_COL                       (1 <<  3)
#define SUB_ESC                       (1 <<  4)
#define SUB_CMD                       (1 <<  5)
#define SUB_ANC                       (1 <<  6)
#define SUB_SEC                       (1 <<  7)
#define SUB_EOL                       (1 <<  8)
#define SUB_FMT                       (1 <<  9)

#define SES_FLAG_NAWS                 (1 <<  0)
#define SES_FLAG_ECHOCOMMAND          (1 <<  1)
#define SES_FLAG_SNOOP                (1 <<  2)

#define SES_FLAG_MAPPING              (1 <<  4)
#define SES_FLAG_SPLIT                (1 <<  5)
#define SES_FLAG_SPEEDWALK            (1 <<  6)
#define SES_FLAG_RESETBUFFER          (1 <<  7)
#define SES_FLAG_READMUD              (1 <<  8)
#define SES_FLAG_WORDWRAP             (1 <<  9)
#define SES_FLAG_VERBATIM             (1 << 10)
#define SES_FLAG_CONNECTED            (1 << 11)
#define SES_FLAG_REPEATENTER          (1 << 12)
#define SES_FLAG_VERBOSE              (1 << 13)
#define SES_FLAG_QUIET                (1 << 14)

#define SES_FLAG_LOGPLAIN             (1 << 16)
#define SES_FLAG_LOGHTML              (1 << 17)
#define SES_FLAG_GAG                  (1 << 18)

#define SES_FLAG_USERCOMMAND          (1 << 20)
#define SES_FLAG_SCROLLLOCK           (1 << 21)
#define SES_FLAG_SCAN                 (1 << 22)
#define SES_FLAG_SCROLLSTOP           (1 << 23)
#define SES_FLAG_CONVERTMETA          (1 << 24)
#define SES_FLAG_PREPPED              (1 << 25)


#define SES_FLAG_BREAK                (1 << 28)

#define TELOPT_FLAG_INIT_SGA          (1 <<  0)
#define TELOPT_FLAG_SGA               (1 <<  1)
#define TELOPT_FLAG_EOR               (1 <<  2)
#define TELOPT_FLAG_ECHO              (1 <<  3)
#define TELOPT_FLAG_NAWS              (1 <<  4)
#define TELOPT_FLAG_PROMPT            (1 <<  5)
#define TELOPT_FLAG_IAC               (1 <<  6)
#define TELOPT_FLAG_DEBUG             (1 <<  7)

#define LIST_FLAG_IGNORE              (1 <<  0)
#define LIST_FLAG_MESSAGE             (1 <<  1)
#define LIST_FLAG_DEBUG               (1 <<  2)
#define LIST_FLAG_LOG                 (1 <<  3)
#define LIST_FLAG_DEFAULT             LIST_FLAG_MESSAGE

#define NODE_FLAG_CLASS               (1 <<  0)
#define NODE_FLAG_MAX                 (1 <<  1)

#define STR_HASH_FLAG_NOGREP          (1 <<  0)

#define CMD_FLAG_NONE                 (0 <<  0)
#define CMD_FLAG_SUB                  (1 <<  0)

#define MAX_STR_HASH                  5000

#define MAX_ROOM                      5000

/*
	Some macros to deal with double linked lists
*/

#define LINK(link, head, tail)                                   \
{                                                                \
     if ((head) == NULL)                                         \
     {                                                           \
          (head)                  = (link);                      \
     }                                                           \
     else                                                        \
     {                                                           \
          (tail)->next             = (link);                     \
     }                                                           \
     (link)->next                  = NULL;                       \
     (link)->prev                  = (tail);                     \
     (tail)                        = (link);                     \
}


#define INSERT_LEFT(link, rght, head)                            \
{                                                                \
     (link)->prev                  = (rght)->prev;               \
     (rght)->prev                  = (link);                     \
     (link)->next                  = (rght);                     \
                                                                 \
     if ((link)->prev)                                           \
     {                                                           \
          (link)->prev->next       = (link);                     \
     }                                                           \
     else                                                        \
     {                                                           \
          (head)                  = (link);                      \
     }                                                           \
}


#define INSERT_RIGHT(link, left, tail)                           \
{                                                                \
     (link)->next                  = (left)->next;               \
     (left)->next                  = (link);                     \
     (link)->prev                  = (left);                     \
                                                                 \
     if ((link)->next)                                           \
     {                                                           \
          (link)->next->prev       = (link);                     \
     }                                                           \
     else                                                        \
     {                                                           \
          (tail)                   = (link);                     \
     }                                                           \
}

#define UNLINK(link, head, tail)                                 \
{                                                                \
     if (((link)->prev == NULL && (link) != head)                \
     ||  ((link)->next == NULL && (link) != tail))               \
     {                                                           \
          tintin_printf2(NULL, "#UNLINK ERROR in file %s on line %d", __FILE__, __LINE__); \
          dump_stack();                                          \
     }                                                           \
     if ((link)->prev == NULL)                                   \
     {                                                           \
          (head)                  = (link)->next;                \
     }                                                           \
     else                                                        \
     {                                                           \
          (link)->prev->next       = (link)->next;               \
     }                                                           \
     if ((link)->next == NULL)                                   \
     {                                                           \
          (tail)                   = (link)->prev;               \
     }                                                           \
     else                                                        \
     {                                                           \
          (link)->next->prev       = (link)->prev;               \
     }                                                           \
     (link)->next = NULL;                                        \
     (link)->prev = NULL;                                        \
}

#define URANGE(a, b, c)           ((b) < (a) ? (a) : (b) > (c) ? (c) : (b))
#define UMAX(a, b)                ((a) > (b) ? (a) : (b))
#define UMIN(a, b)                ((a) < (b) ? (a) : (b))

#define IS_SPLIT(ses)             ((ses)->rows != (ses)->bot_row || (ses)->top_row != 1)

#define SCROLL(ses)               (((ses)->cur_row >= (ses)->top_row && (ses)->cur_row <= (ses)->bot_row) || (ses)->cur_row == (ses)->rows)

#define DO_COMMAND(command) struct session  *command (struct session *ses, const char *arg)
#define DO_CONFIG(config) struct session *config (struct session *ses, char *arg, int index)
#define DO_CLASS(class) struct session *class (struct session *ses, char *left, char *right)
#define DO_ARRAY(array) struct session *array (struct session *ses, LNODE *list, const char *arg)

#define DO_CHAT(chat) void chat (const char *arg)



/*
	Bit operations
*/

#define HAS_BIT(bitvector, bit)   ((bitvector)  & (bit))
#define SET_BIT(bitvector, bit)   ((bitvector) |= (bit))
#define DEL_BIT(bitvector, bit)   ((bitvector) &= (~(bit)))
#define TOG_BIT(bitvector, bit)   ((bitvector) ^= (bit))

/*
	Compatibility
*/

#define atoll(str) (strtoll(str, NULL, 10))


/************************ structures *********************/

struct listroot
{
	struct listnode   * f_node;
	struct listnode   * l_node;
	struct listnode   * update;
	int                 count;
	int                 flags;
};

struct listnode
{
	struct listnode   * next;
	struct listnode   * prev;
	struct listnode   * class;
	char              * left;
	char              * right;
	char              * pr;
	long long           data;
	short               flags;
};

struct session
{
	struct session        * next;
	struct session        * prev;
	struct room_data      * room_list[MAX_ROOM];
	int                     in_room;
	z_stream              * mccp;
	char                 ** buffer;
	char                  * name;
	FILE                  * logfile;
	FILE                  * logline;
	struct listroot       * list[LIST_ALL];
	struct listroot       * history;
	struct listnode       * class;
	int                     rows;
	int                     cols;
	int                     top_row;
	int                     bot_row;
	int                     cur_row;
	int                     sav_row;
	int                     cur_col;
	int                     sav_col;
	int                     map_size;
	int                     scroll_max;
	int                     scroll_row;
	int                     scroll_line;
	int                     scroll_base;
	int                     fgc;
	int                     bgc;
	int                     vtc;
	int                     socket;
	int                     telopts;
	long long               flags;
	char                  * host;
	char                  * port;
	long long               connect_retry;
	int                     connect_error;
	char                    more_output[BUFFER_SIZE];
	long long               check_output;
};

struct tintin_data
{
	struct session        * ses;
	struct session        * update;
	struct chat_data      * chat;
	char                  * mud_output_buf;
	int                     mud_output_max;
	int                     mud_output_len;
	unsigned char         * mccp_buf;
	int                     mccp_buf_max;
	Keymap                  keymap;
	long long               time;
	int                     str_hash_size;
	int                     history_size;
	int                     command_ref[26];
	char                    tintin_char;
	char                    verbatim_char;
	char                    repeat_char;
	char                    vars[10][BUFFER_SIZE];
	char                    cmds[10][BUFFER_SIZE];
};

struct chat_data
{
	struct chat_data      * next;
	struct chat_data      * prev;
	char                  * name;
	char                  * ip;
	char                  * version;
	char                  * download;
	char                  * reply;
	char                  * paste_buf;
	char                  * color;
	int                     port;
	int                     fd;
	time_t                  timeout;
	int                     flags;
	long long               paste_time;
	FILE                  * file_pt;
	char                  * file_name;
	long long               file_size;
	int                     file_block_cnt;
	int                     file_block_tot;
	long long               file_start_time;
};

struct command_type
{
	char                  * name;
	COMMAND               * command;
	int                     flags;
};

struct list_type
{
	char                  * name;
	char                  * name_multi;
	int                     mode;
	int                     args;
};


struct config_type
{
	char                  * name;
	char                  * msg_on;
	char                  * msg_off;
	CONFIG                * config;
};

struct color_type
{
	char                  * name;
	char                  * number;
	char                  * code;
};

struct class_type
{
	char                  * name;
	CLASS                 * class;
};

struct chat_type
{
	char                  * name;
	CHAT                  * chat;
};

struct array_type
{
	char                  * name;
	ARRAY                 * array;
};

struct str_hash_data
{
	struct str_hash_data    * next;
	struct str_hash_data    * prev;
	unsigned short            count;
     unsigned short            flags;
	unsigned short            lines;
	unsigned short            hash;
};

struct str_hash_index_data
{
	struct str_hash_data    * f_node;
	struct str_hash_data    * l_node;
};


struct room_data
{
	struct exit_data        * f_exit;
	struct exit_data        * l_exit;
	int                       vnum;
	short                     size;
	short                     flags;
	char                    * name;
	char                    * color;
};

struct exit_data
{
	struct exit_data        * next;
	struct exit_data        * prev;
	short                     vnum;
	char                    * name;
	char                    * cmd;
};

#endif


/*
	Some stuff we use globally
*/

extern int readline_echoing_p;

/*
	Function declarations
*/

#ifndef __INPUT_H__
#define __INPUT_H__

extern void commandloop(void);
extern char *readkeyboard(void);
extern void echo_command(struct session *ses, char *line, int newline);

#endif

#ifndef __ARRAY_H__
#define __ARRAY_H__

extern DO_COMMAND(do_list);

extern DO_ARRAY(array_ins);
extern DO_ARRAY(array_del);
extern DO_ARRAY(array_get);
extern DO_ARRAY(array_len);
extern DO_ARRAY(array_set);

#endif

#ifndef __CHAT_H__
#define __CHAT_H__

extern DO_COMMAND(do_chat);
extern DO_CHAT(chat_accept);
extern DO_CHAT(chat_call);
extern DO_CHAT(chat_cancelfile);
extern DO_CHAT(chat_color);
extern DO_CHAT(chat_decline);
extern DO_CHAT(chat_dnd);
extern DO_CHAT(chat_downloaddir);
extern DO_CHAT(chat_emote);
extern DO_CHAT(chat_filestat);
extern DO_CHAT(chat_forward);
extern DO_CHAT(chat_forwardall);
extern DO_CHAT(chat_ignore);
extern DO_CHAT(chat_initialize);
extern DO_CHAT(chat_info);
extern DO_CHAT(chat_ip);
extern DO_CHAT(chat_message);
extern DO_CHAT(chat_name);
extern DO_CHAT(chat_paste);
extern DO_CHAT(chat_peek);
extern DO_CHAT(chat_ping);
extern DO_CHAT(chat_private);
extern DO_CHAT(chat_public);
extern DO_CHAT(chat_reply);
extern DO_CHAT(chat_request);
extern DO_CHAT(chat_sendfile);
extern DO_CHAT(chat_transfer);
extern DO_CHAT(chat_serve);
extern DO_CHAT(chat_who);
extern DO_CHAT(chat_zap);

extern  int chat_new(int s);
extern void chat_call(const char *ip);
extern void close_chat(struct chat_data *buddy, int unlink);
extern void nonblock(int s);
extern void block(int s);
extern void process_chat_connections(fd_set *input_set, fd_set *exc_set);
extern void chat_forward_session(struct session *ses, char *linelog);
extern void chat_socket_printf(struct chat_data *buddy, const char *format, ...);
extern void chat_printf(const char *format, ...);
extern  int process_chat_input(struct chat_data *buddy);
extern void get_chat_commands(struct chat_data *buddy, char *buf, int len);
extern void chat_receive_text_everybody(struct chat_data *buddy, char *txt);
extern void chat_receive_text_personal(struct chat_data *buddy, char *txt);
extern void chat_receive_message(struct chat_data *buddy, char *txt);
extern void chat_receive_snoop_data(struct chat_data *buddy, char *txt);

extern void ping_response(struct chat_data *ch, char *time);

extern void request_response(struct chat_data *requester);
extern void parse_requested_connections(struct chat_data *buddy, char *txt);

extern void peek_response(struct chat_data *peeker);
extern void parse_peeked_connections(struct chat_data *buddy, char *txt);


extern void chat_receive_file(char *arg, struct chat_data *ch);
extern void send_block(struct chat_data *ch);
extern void receive_block(unsigned char *s, struct chat_data *ch);
extern void deny_file(struct chat_data *ch, char *arg);
extern void file_denied(struct chat_data *ch, char *txt);
extern void file_cleanup(struct chat_data *buddy);

extern int get_file_size(char *fpath);

extern void chat_puts(char *arg);
extern struct chat_data *find_buddy(const char *arg);
extern char *fix_file_name(char *name);



#endif
 
#ifndef __ECHO_H__
#define __ECHO_H__

extern void echo_on(struct session *ses);
extern void echo_off(struct session *ses);
extern void check_character_mode(struct session *ses);

#endif

#ifndef __CLASS_H__
#define __CLASS_H__

extern DO_COMMAND(do_class);
extern DO_COMMAND(do_unclass);
extern int count_class(struct session *ses, struct listnode *class);
extern DO_CLASS(class_open);
extern DO_CLASS(class_close);
extern DO_CLASS(class_read);
extern DO_CLASS(class_write);
extern DO_CLASS(class_kill);
extern void parse_class(struct session *ses, char *input, struct listnode *class);

#endif

#ifndef __MAPPER_H__
#define __MAPPER_H__

extern DO_COMMAND(do_redit);
extern int  create_room(struct session *ses, const char *arg);
extern void create_exit(struct session *ses, const char *arg);
extern int find_room(struct session *ses, char *arg);
extern int get_map_exit(struct exit_data *exit);
extern void create_map(struct session *ses, short room, short x, short y);
extern void follow_map(struct session *ses, const char *argument);
extern void build_map(short room, short x, short y);
extern char *draw_room(struct session *ses, struct room_data *room);

extern void search_path(short room, short size);
extern void shortest_path(struct session *ses, char *arg);

extern int find_coord(struct session *ses, char *arg);
extern void search_coord(int vnum, short x, short y, short z);

#endif

#ifndef __MATH_H__
#define __MATH_H__

extern DO_COMMAND(do_math);
extern DO_COMMAND(do_if);
extern long long get_number(struct session *ses, const char *str);
extern long long mathexp(struct session *ses, const char *str);
extern int mathexp_tokenize(struct session *ses, const char *str);
extern void mathexp_level(struct listnode *node);
extern void mathexp_compute(struct listnode *node);
extern long long tintoi(const char *str);
extern long long tincmp(const char *left, const char *right);
extern long long tindice(const char *left, const char *right);

#endif

#ifndef __PROMPT_H__
#define __PROMPT_H__

extern DO_COMMAND(do_split);
extern DO_COMMAND(do_unsplit);
extern void init_split(struct session *ses, int top, int bot);
extern void clean_screen(struct session *ses);
extern void dirty_screen(struct session *ses);
DO_COMMAND(do_prompt);
DO_COMMAND(do_unprompt);
extern void check_all_prompts(char *original, char *line, struct session *ses);
extern void do_one_prompt(struct session *ses, char *prompt, int row);

#endif


#ifndef __REGEXP_H__
#define __REGEXP_H__

extern int regexp(const char *exp, const char *str);
extern void substitute(struct session *ses, const char *string, char *result, int flags);
extern int check_one_action(const char *line, const char *original, const char *action, struct session *ses);
extern int action_regexp(const char *exp, const char *str);


#endif


#ifndef __CONFIG_H__
#define __CONFIG_H__

extern DO_COMMAND(do_configure);
extern DO_CONFIG(config_speedwalk);
extern DO_CONFIG(config_verbatim);
extern DO_CONFIG(config_repeatenter);
extern DO_CONFIG(config_echocommand);
extern DO_CONFIG(config_verbose);
extern DO_CONFIG(config_wordwrap);
extern DO_CONFIG(config_log);
extern DO_CONFIG(config_buffersize);
extern DO_CONFIG(config_scrolllock);
extern DO_CONFIG(config_historysize);
extern DO_CONFIG(config_connectretry);
extern DO_CONFIG(config_packetpatch);
extern DO_CONFIG(config_tintinchar);
extern DO_CONFIG(config_verbatimchar);
extern DO_CONFIG(config_repeatchar);
extern DO_CONFIG(config_debugtelnet);
extern DO_CONFIG(config_convertmeta);

#endif


#ifndef __MACRO_H__
#define __MACRO_H__

extern DO_COMMAND(do_macro);
extern DO_COMMAND(do_unmacro);
extern void macro_update(void);
#endif

#ifndef __STRHASH_H__
#define __STRHASH_H__

extern char *str_hash(char *str, int lines);
extern char *str_unhash(char *str);
extern unsigned short str_hash_lines(char *str);
extern unsigned short str_hash_grep(char *str, int write);
extern void reset_hash_table(void);

DO_COMMAND(do_hash);

#endif

#ifndef __BUFFER_H__
#define __BUFFER_H__

extern void init_buffer(struct session *ses, int size);
extern void add_line_buffer(struct session *ses, const char *line, int more_output);
extern DO_COMMAND(do_buffer);
extern DO_COMMAND(do_grep);
extern int show_buffer(struct session *ses);
extern void buffer_u(void);
extern void buffer_d(void);
extern void buffer_h(void);
extern void buffer_e(void);
extern void buffer_l(void);

#endif

#ifndef __DEBUG_H__
#define __DEBUG_H__

extern int push_call(char *f, ...);
extern void pop_call(void);
extern void dump_stack(void);

#endif

#ifndef __ACTION_H__
#define __ACTION_H__

extern struct session *do_action(struct session *ses, const char *arg);
extern struct session *do_unaction(struct session *ses, const char *arg);

extern void check_all_actions(const char *original, char *line, struct session *ses);

#endif

#ifndef __ALIAS_H__
#define __ALIAS_H__

extern struct session *do_alias(struct session *ses, const char *arg);
extern struct session *do_unalias(struct session *ses, const char *arg);
extern int check_all_aliases(char *original, char *line, struct session *ses);

#endif

#ifndef __ANTISUB_H__
#define __ANTISUB_H__

extern DO_COMMAND(do_antisubstitute);
extern DO_COMMAND(do_unantisubstitute);
extern int check_all_antisubstitutions(const char *original, char *line, struct session *ses);

#endif


#ifndef __FILES_H__
#define __FILES_H__

extern DO_COMMAND(do_read);
extern DO_COMMAND(do_readmap);
extern DO_COMMAND(do_write);
extern DO_COMMAND(do_writemap);
extern DO_COMMAND(do_session);
extern void prepare_for_write(int mode, struct listnode *node, char *result);
DO_COMMAND(do_textin);
DO_COMMAND(do_scan);

#endif 

#ifndef __FUNCTION_H__
#define __FUNCTION_H__

extern DO_COMMAND(do_function);
extern DO_COMMAND(do_unfunction);


#endif



#ifndef __HELP_H__
#define __HELP_H__

extern DO_COMMAND(do_help);

#endif

#ifndef __HIGHLIGHT_H__
#define __HIGHLIGHT_H__

extern DO_COMMAND(do_highlight);
extern int is_high_arg(const char *s);
extern DO_COMMAND(do_unhighlight);
extern void check_all_highlights(char *original, char *line, struct session *ses);
extern int get_highlight_codes(const char *htype, char *result);

#endif


#ifndef __LLIST_H__
#define __LLIST_H__

extern struct listroot *init_list(void);
extern void kill_list(struct session *ses, int index);
extern DO_COMMAND(do_killall);
extern struct listroot *copy_list(struct session *ses, struct listroot *sourcelist, int index);
extern void insertnode_list(struct session *ses, const char *ltext, const char *rtext, const char *prtext, int ondex);
extern void updatenode_list(struct session *ses, const char *ltext, const char *rtext, const char *prtext, int index);
extern void deletenode_list(struct session *ses, struct listnode *node, int index);
extern struct listnode *searchnode_list(struct listroot *listhead, const char *cptr);
extern struct listnode *searchnode_list_begin(struct listroot *listhead, const char *cptr, int mode);
extern void shownode_list(struct session *ses, struct listnode *node, int index);
extern void show_list(struct session *ses, struct listroot *listhead, int index);
extern int show_node_with_wild(struct session *ses, const char *cptr, int index);
extern struct listnode *search_node_with_wild(struct listroot *listhead, const char *cptr);
extern void addnode_list(struct listroot *listhead, const char *ltext, const char *rtext, const char *prtext);
extern int count_list(struct listroot *listhead);
extern DO_COMMAND(do_message);
extern DO_COMMAND(do_ignore);
extern DO_COMMAND(do_debug);
#endif


#ifndef __LOG_H__
#define __LOG_H__

extern void logit(struct session *ses, const char *txt, FILE *file);
extern DO_COMMAND(do_log);
extern DO_COMMAND(do_logline);
extern DO_COMMAND(do_writebuffer);
extern void write_html_header(FILE *fp);
extern void vt102_to_html(struct session *ses, const char *txt, char *out);
#endif


#ifndef __MAIN_H__
#define __MAIN_H__

extern struct session *gts;
extern struct tintin_data *gtd;

extern void trap_handler(int signal);
extern void winchhandler(int no_care);
extern int main(int argc, char **argv);
extern void init_tintin(void);

#endif

#ifndef __MISC_H__
#define __MISC_H__

extern DO_COMMAND(do_all);
extern DO_COMMAND(do_bell);
extern DO_COMMAND(do_boss);
extern DO_COMMAND(do_commands);
extern DO_COMMAND(do_cr);
extern DO_COMMAND(do_echo);
extern DO_COMMAND(do_end);
extern DO_COMMAND(do_forall);
extern DO_COMMAND(do_info);
extern DO_COMMAND(do_loop);
extern DO_COMMAND(do_nop);
extern DO_COMMAND(do_parse);
extern DO_COMMAND(do_return);
extern DO_COMMAND(do_send);
extern DO_COMMAND(do_showme);
extern DO_COMMAND(do_snoop);
extern DO_COMMAND(do_suspend);
extern DO_COMMAND(do_system);
extern DO_COMMAND(do_zap);

extern DO_COMMAND(do_gagline);
#endif


#ifndef __NET_H__
#define __NET_H__

extern int connect_mud(const char *host, const char *port, struct session *ses);
extern void write_line_mud(const char *line, struct session *ses);
extern void read_buffer_mud(struct session *ses);

#endif
#ifndef __TELOPT_H__
#define __TELOPT_H__

extern void translate_telopts(struct session *ses, unsigned char *src, int cplen);

extern int send_will_sga(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_do_eor(struct session *ses, int cplen, unsigned char *cpsrc);
extern int mark_prompt(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_will_ttype(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_will_tspeed(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_will_naws(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_wont_xdisploc(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_wont_new_environ(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_sb_naws(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_wont_lflow(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_sb_tspeed(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_sb_ttype(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_wont_status(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_dont_status(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_dont_sga(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_do_sga(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_wont_oldenviron(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_echo_on(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_echo_off(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_echo_will(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_ip(struct session *ses, int cplen, unsigned char *cpsrc);
extern int exec_zmp(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_do_mccp2(struct session *ses, int cplen, unsigned char *cpsrc);
extern int send_dont_mccp2(struct session *ses, int cplen, unsigned char *cpsrc);
extern int init_mccp(struct session *ses, int cplen, unsigned char *cpsrc);
extern void *zlib_alloc(void *opaque, unsigned int items, unsigned int size);
extern void zlib_free(void *opaque, void *address);

extern void init_telnet_session(struct session *ses);

#endif

#ifndef __PARSE_H__
#define __PARSE_H__

extern struct session *parse_input(char *input, struct session *ses);
extern int is_speedwalk_dirs(const char *cp);
extern void process_speedwalk(const char *cp, struct session *ses);
extern struct session *parse_tintin_command(const char *command, char *arg, struct session *ses);
extern const char *get_arg_all(const char *s, char *arg);
extern const char *get_arg_with_spaces(const char *s, char *arg);
extern const char *get_arg_in_braces(const char *s, char *arg, int flag);
extern const char *get_arg_stop_spaces(const char *s, char *arg);
extern const char *space_out(const char *s);
extern void write_mud(struct session *ses, const char *command);
extern void do_one_line(char *line, struct session *ses);

#endif

#ifndef __PATH_H__
#define __PATH_H__

extern DO_COMMAND(do_mark);
extern DO_COMMAND(do_map);
extern DO_COMMAND(do_savepath);
extern DO_COMMAND(do_loadpath);
extern DO_COMMAND(do_path);
extern DO_COMMAND(do_unpath);
extern DO_COMMAND(do_pathdir);
extern DO_COMMAND(do_walk);
extern void check_insert_path(const char *command, struct session *ses);

#endif

#ifndef __RL_H__
#define __RL_H__


extern void printline(struct session *ses, const char *str, int isaprompt);
extern void initrl(void);
extern void mainloop(void);
extern void readmud(struct session *ses);
extern void bait(void);
extern void process_mud_output(struct session *ses, char *linebuf, int prompt);
extern void show_message(struct session *ses, int index, const char *format, ...);
extern void tintin_header(struct session *ses, const char *format, ...);
extern void socket_printf(struct session *ses, size_t length, const char *format, ...);
extern void tintin_printf2(struct session *ses, const char *format, ...);
extern void tintin_printf(struct session *ses, const char *format, ...);
extern void tintin_puts2(const char *cptr, struct session *ses);
extern void tintin_puts(const char *cptr, struct session *ses);
extern void quitmsg(const char *m);
extern void myquitsig(int no_care);

#endif


#ifndef __RLHIST_H__
#define __RLHIST_H__

extern char *rlhist_expand(char *line);

#endif

#ifndef __RLTAB_H__
#define __RLTAB_H__

extern char *tab_complete(const char *str, int state);
extern DO_COMMAND(do_tab);
extern DO_COMMAND(do_untab);

#endif


#ifndef __VT102_H__
#define __VT102_H__

extern void save_pos(struct session *ses);
extern void restore_pos(struct session *ses);
extern void goto_rowcol(struct session *ses, int row, int col);
extern void erase_screen(struct session *ses);
extern void erase_toeol(void);
extern void erase_scroll_region(struct session *ses);
extern void reset(void);
extern void scroll_region(struct session *ses, int top, int bottom);
extern void reset_scroll_region(struct session *ses);
extern int skip_vt102_codes(const char *str);
extern int skip_vt102_codes_non_graph(const char *str);
extern void strip_vt102_codes(const char *str, char *buf);
extern void strip_vt102_codes_non_graph(const char *str, char *buf);
extern void strip_non_vt102_codes(const char *str, char *buf);
extern int strip_vt102_strlen(const char *str);
extern int interpret_vt102_codes(struct session *ses, const char *str, int real);

#endif

#ifndef __SCRSIZE_H__
#define __SCRSIZE_H__

extern void init_screen_size(struct session *ses);
extern int get_scroll_size(struct session *ses);

#endif

#ifndef __SESSION_H__
#define __SESSION_H__


extern struct session *session_command(const char *arg, struct session *ses);
extern void show_session(struct session *ses, struct session *ptr);
extern struct session *newactive_session(void);
extern struct session *activate_session(struct session *ses);
extern struct session *new_session(const char *name, const char *address, struct session *ses);
extern void connect_session(struct session *ses);
extern void cleanup_session(struct session *ses);


#endif

#ifndef __SUBSTITUTE_H__
#define __SUBSTITUTE_H__

extern DO_COMMAND(do_substitute);
extern DO_COMMAND(do_gag);
extern DO_COMMAND(do_unsubstitute);
extern DO_COMMAND(do_ungag);
extern void check_all_substitutions(char *original, char *line, struct session *ses);

#endif

#ifndef __TABLES_H__
#define __TABLES_H__


extern const struct command_type command_table[];
extern const struct list_type list_table[LIST_ALL];
extern const struct config_type config_table[];
extern const struct color_type color_table[];
extern const struct class_type class_table[CLASS_MAX];
extern const struct chat_type chat_table[];
extern const struct array_type array_table[ARRAY_MAX];

#endif

#ifndef __TEXT_H__
#define __TEXT_H__


int word_wrap(struct session *ses, const char *textin, char *textout, int scroll);

#endif

#ifndef __TICKS_H__
#define __TICKS_H__

extern DO_COMMAND(do_tick);
extern DO_COMMAND(do_untick);
extern DO_COMMAND(do_delay);
extern void tick_update(void);

#endif

#ifndef __UTILS_H__
#define __UTILS_H__

extern int is_abbrev(const char *s1, const char *s2);
extern int is_suffix(const char *s1, const char *s2);
extern int is_number(const char *str);
extern int hex_number(const char *str);
extern long long utime(void);
extern char *capitalize(const char *str);
extern void cat_sprintf(char *dest, const char *fmt, ...);
extern void syserr(const char *msg);

#endif

#ifndef __VARIABLES_H__
#define __VARIABLES_H__

extern DO_COMMAND(do_variable);
extern DO_COMMAND(do_internal_variable);
extern DO_COMMAND(do_unvariable);

extern DO_COMMAND(do_getlistlength);
extern DO_COMMAND(do_getitemnr);
extern DO_COMMAND(do_format);
extern DO_COMMAND(do_tolower);
extern DO_COMMAND(do_toupper);
extern DO_COMMAND(do_postpad);
extern DO_COMMAND(do_prepad);
extern DO_COMMAND(do_removestring);
extern DO_COMMAND(do_replacestring);

#endif
