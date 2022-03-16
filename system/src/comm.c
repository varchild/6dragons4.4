/***************************************************************************
 * - Chronicles Copyright 2001, 2002 by Brad Ensley (Orion Elder)          *
 * - SMAUG 1.4  Copyright 1994, 1995, 1996, 1998 by Derek Snider           *
 * - Merc  2.1  Copyright 1992, 1993 by Michael Chastain, Michael Quan,    *
 *   and Mitchell Tse.                                                     *
 * - DikuMud    Copyright 1990, 1991 by Sebastian Hammer, Michael Seifert, *
 *   Hans-Henrik Stærfeldt, Tom Madsen, and Katja Nyboe.                   *
 ***************************************************************************
 * - Low-level communication module                                        *
 ***************************************************************************/

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <zlib.h>
#include "h/mud.h"
#include "h/channels.h"
#include "h/auction.h"
#include "h/ban.h"
#include "h/copyover.h"
#include "h/files.h"
#include "h/polymorph.h"
#include "h/hometowns.h"
#include "h/city.h"
#include "h/new_auth.h"
#include "h/languages.h"
#include "h/clans.h"
#include "h/whois.h"
#include "h/mccp.h"

#ifdef I3
void                    free_i3_data(bool complete);
#endif
void game_loop( void );
extern const unsigned char echo_off_str[] = { IAC, WILL, TELOPT_ECHO, '\0' };
extern const unsigned char echo_on_str[] = { IAC, WONT, TELOPT_ECHO, '\0' };
extern const unsigned char will_mxp_str[] = { IAC, WILL, TELOPT_MXP, '\0' };
extern const unsigned char start_mxp_str[] = { IAC, SB, TELOPT_MXP, IAC, SE, '\0' };
extern const unsigned char do_mxp_str[] = { IAC, DO, TELOPT_MXP, '\0' };
extern const unsigned char dont_mxp_str[] = { IAC, DONT, TELOPT_MXP, '\0' };

#define TELOPT_COMPRESS 85
extern const unsigned char eor_on_str[] = { IAC, WILL, TELOPT_EOR, '\0' };
extern const unsigned char compress_on_str[] = { IAC, WILL, TELOPT_COMPRESS, '\0' };
extern const unsigned char compress2_on_str[] = { IAC, WILL, TELOPT_COMPRESS2, '\0' };
void update_transfer( int type, int size );

void                    wipe_area_resets(AREA_DATA *area);

const char *keep_alive_msg[] = {
   "You can tell by the way I walk, nah, nah, nah.\r\n",
   "Ah ah ah ah, staying alive.\r\n",
   "Keeping link alive.\r\n"
};

extern MPPAUSE_DATA    *first_mppause, *last_mppause;
void                    remove_all_rollcalls(CITY_DATA * city);
void                    free_all_scores(void);
void                    free_all_landmarks(void);
int                     arena_low_vnum;
int                     arena_high_vnum;
int                     init_socket(int mudport);
void                    free_hints(void);
void                    free_all_quest(void);
void                    free_all_locations(void);
void                    free_holidays(void);
void                    free_banks(void);
void                    freeIdeas(void);
void                    freeBugs(void);
void                    freeTypos(void);
void clear_news         args((bool sMatch, int nCount));
void                    free_all_races(void);
void                    free_all_classes(void);
void                    free_currencies(void);
void                    close_area(AREA_DATA *pArea);
extern BOARD_DATA      *first_board;
extern BOARD_DATA      *last_board;
bool                    cleanedupmemory = FALSE;
void                    free_bans(void);
void                    free_auction(AUCTION_DATA * auction);
void                    free_board(BOARD_DATA * board);
void                    free_mssp_info(void);
void                    free_help(HELP_DATA *pHelp);
void                    free_prog_actlists(void);
bool                    str_has_command(const char *buf, const char *cmd);
DESCRIPTOR_DATA        *d_next;
bool write_to_descriptor args((DESCRIPTOR_DATA *d, const char *txt, int length));

#ifdef WIN32
#include <io.h>
#undef EINTR
#undef EMFILE
#define EINTR WSAEINTR
#define EMFILE WSAEMFILE
#define EWOULDBLOCK WSAEWOULDBLOCK

void                    bailout(void);
void                    shutdown_checkpoint(void);
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <arpa/telnet.h>
#include <netdb.h>
//#define closesocket close
#endif

#ifdef I3
void                    i3_delete_info(void);
void                    free_i3_data(bool complete);
#endif

long long int           GUID;

void                    dispose_ban(BAN_DATA *ban, int type);
void                    close_all_areas(void);
void                    free_teleports(void);
void                    free_clans(void);
void                    free_morphs(void);
void                    free_councils(void);

const char              go_ahead_str[] = { '\xFF', '\xF9', '\0' };

void                    addname(char **list, const char *name);

void                    save_sysdata(SYSTEM_DATA sys);
short                   client_speed(short speed);

void                    handle_con_note_to(DESCRIPTOR_DATA *d, char *argument);
void                    handle_con_note_subject(DESCRIPTOR_DATA *d, char *argument);
void                    handle_con_note_expire(DESCRIPTOR_DATA *d, char *argument);

/* Global variables. */
DESCRIPTOR_DATA        *first_descriptor; /* First descriptor */
DESCRIPTOR_DATA        *last_descriptor;  /* Last descriptor */
int                     num_descriptors;
bool                    mud_down; /* Shutdown */
bool                    service_shut_down;  /* Shutdown by operator closing down service */
time_t                  boot_time;
HOUR_MIN_SEC            set_boot_time_struct;
HOUR_MIN_SEC           *set_boot_time;
struct tm              *new_boot_time;
struct tm               new_boot_struct;
char                    str_boot_time[MIL];
char                    lastplayercmd[MIL * 2];
time_t                  current_time; /* Time of this pulse */
int                     control;  /* Controlling descriptor */

#if defined(WIN32)
SOCKET                  newdesc;
#else
int                     newdesc;  /* New descriptor */
#endif
fd_set                  in_set; /* Set of desc's for reading */
fd_set                  out_set;  /* Set of desc's for writing */
fd_set                  exc_set;  /* Set of desc's with errors */
int                     maxdesc;
const char             *alarm_section = "(unknown)";
bool                    winter_freeze = FALSE;
extern char            *help_greeting;

/* OS-dependent local functions. */
bool read_from_descriptor args((DESCRIPTOR_DATA *d));

/* Other local functions (OS-independent). */
bool                    check_parse_name(char *name, bool newchar);
bool                    check_reconnect(DESCRIPTOR_DATA *d, char *name, bool fConn);
bool                    check_playing(DESCRIPTOR_DATA *d, char *name, bool kick);
int                     main(int argc, char **argv);
void                    genesis(DESCRIPTOR_DATA *d, char *argument);
bool                    flush_buffer(DESCRIPTOR_DATA *d, bool fPrompt);
void                    read_from_buffer(DESCRIPTOR_DATA *d);
void                    stop_idling(CHAR_DATA *ch);
void                    free_desc(DESCRIPTOR_DATA *d);
void                    display_prompt(DESCRIPTOR_DATA *d);
int                     make_color_sequence(const char *col, char *buf, DESCRIPTOR_DATA *d);
void                    set_pager_input(DESCRIPTOR_DATA *d, char *argument);
bool                    pager_output(DESCRIPTOR_DATA *d);
int                     port;

/* Scan a whole argument for a single word - return TRUE if found - Samson 7-24-00 */
/* Code by Orion Elder */
bool arg_cmp(char *haystack, char *needle)
{
  char                    argument[MSL];

  for(;;)
  {
    haystack = one_argument(haystack, argument);

    if(!argument || argument[0] == '\0')
      return FALSE;
    else if(!str_cmp(argument, needle))
      return TRUE;
    else
      continue;
  }
}

void cleanup_memory(void)
{
  int                     hash, sn, loopa;
  SKILLTYPE              *skill;
  SMAUG_AFF              *aff, *aff_next;
  HELP_DATA              *pHelp, *pHelp_next;
  CHAR_DATA              *character;
  OBJ_DATA               *object;
  AREA_DATA              *area, *area_next;
  DESCRIPTOR_DATA        *desc;
  struct act_prog_data   *apd, *apd_next;
  RESERVE_DATA           *resn, *resn_next;
  BOARD_DATA             *board, *board_next;
  CMDTYPE                *command, *cmd_next;
  MUD_CHANNEL            *channel, *channel_next;
  SOCIALTYPE             *social, *social_next;
  MORPH_DATA             *morph, *morph_next;
  LANG_DATA              *lang;
  LCNV_DATA              *lcnv;
  WIZINFO_DATA           *wiz, *wiz_next;
  HTOWN_DATA             *htown, *htown_next;
  CITY_DATA              *city, *city_next;
  AUTH_LIST              *auth, *nauth;

  if(cleanedupmemory == TRUE)
    return;
  cleanedupmemory = TRUE;

#ifdef I3
  fprintf(stdout, "%s", "I3 Data.\n");
  i3_free_data(TRUE);
#endif

  log_string("Local Channels.");
  for(channel = first_channel; channel; channel = channel_next)
  {
    channel_next = channel->next;
    UNLINK(channel, first_channel, last_channel, next, prev);
    for(loopa = 0; loopa < 20; loopa++)
    {
      if(VLD_STR(channel->history[loopa][0]))
        STRFREE(channel->history[loopa][0]);
      if(VLD_STR(channel->history[loopa][1]))
        STRFREE(channel->history[loopa][1]);
    }
    STRFREE(channel->name);
    DISPOSE(channel);
  }

  log_string("Commands.");
  for(hash = 0; hash < 126; hash++)
  {
    for(command = command_hash[hash]; command; command = cmd_next)
    {
      cmd_next = command->next;
      command->next = NULL;
      command->do_fun = NULL;
      free_command(command);
    }
  }

  log_string("Help files.");
  for(pHelp = first_help; pHelp; pHelp = pHelp_next)
  {
    pHelp_next = pHelp->next;
    free_help(pHelp);
  }

  log_string("Reserved name list.");
  for(resn = first_reserved; resn; resn = resn_next)
  {
    resn_next = resn->next;
    UNLINK(resn, first_reserved, last_reserved, next, prev);
    STRFREE(resn->name);
    DISPOSE(resn);
  }

  log_string("Socials.");
  for(hash = 0; hash < 27; hash++)
  {
    for(social = social_index[hash]; social; social = social_next)
    {
      social_next = social->next;
      free_social(social);
    }
  }

  /*
   * Whack supermob 
   */
  log_string("Whacking supermob.");
  if(supermob)
  {
    char_from_room(supermob);
    if(!supermob || !VLD_STR(supermob->name))
      bug("%s", "UnLinked: NULL - first_char");
    UNLINK(supermob, first_char, last_char, next, prev);
    free_char(supermob);
  }

  // MSSP Info
  fprintf(stdout, "%s", "MSSP Info.\n");
  free_mssp_info();

  log_string("Auctions");
  free_auction(auction);

  log_string("Freeing mppause data.");
  {
    MPPAUSE_DATA           *mppause, *mppause_next;

    for(mppause = first_mppause; mppause; mppause = mppause_next)
    {
      mppause_next = mppause->next;
      STRFREE(mppause->com_list);
      UNLINK(mppause, first_mppause, last_mppause, next, prev);
      DISPOSE(mppause);
    }
  }

  clean_obj_queue();
  log_string("Objects.");
  while((object = last_object) != NULL)
    extract_obj(object);
  clean_obj_queue();

  clean_char_queue();
  log_string("Characters.");
  while((character = last_char) != NULL)
    extract_char(character, TRUE);
  clean_char_queue();

  /*
   * Descriptors 
   */
  log_string("Descriptors.");
  for(desc = first_descriptor; desc; desc = d_next)
  {
    d_next = desc->next;
    UNLINK(desc, first_descriptor, last_descriptor, next, prev);
    free_desc(desc);
  }

  for(auth = first_auth_name; auth; auth = nauth)
  {
    nauth = auth->next;
    UNLINK(auth, first_auth_name, last_auth_name, next, prev);
    if(auth->authed_by)
      STRFREE(auth->authed_by);
    if(auth->change_by)
      STRFREE(auth->change_by);
    if(auth->denied_by)
      STRFREE(auth->denied_by);
    STRFREE(auth->name);
    DISPOSE(auth);
  }

  log_string("Wizinfo Data.");
  for(wiz = first_wizinfo; wiz; wiz = wiz_next)
  {
    wiz_next = wiz->next;
    UNLINK(wiz, first_wizinfo, last_wizinfo, next, prev);
    if(VLD_STR(wiz->name))
      STRFREE(wiz->name);
    if(VLD_STR(wiz->email))
      STRFREE(wiz->email);
    DISPOSE(wiz);
  }

  log_string("Classes.");
  free_all_classes();

  log_string("Races.");
  free_all_races();

  log_string("Currency.");
  free_currencies();

  log_string("Clans.");
  free_clans();

  log_string("HTowns.");
  for(htown = first_htown; htown; htown = htown_next)
  {
    htown_next = htown->next;
    if(VLD_STR(htown->description))
      STRFREE(htown->description);
    if(VLD_STR(htown->filename))
      STRFREE(htown->filename);
    if(VLD_STR(htown->general))
      STRFREE(htown->general);
    if(VLD_STR(htown->name))
      STRFREE(htown->name);
    if(VLD_STR(htown->nation))
      STRFREE(htown->nation);
    if(VLD_STR(htown->race))
      STRFREE(htown->race);
    if(VLD_STR(htown->ruler))
      STRFREE(htown->ruler);
    DISPOSE(htown);
  }

  log_string("Cities.");
  for(city = first_city; city; city = city_next)
  {
    city_next = city->next;
    if(VLD_STR(city->description))
      STRFREE(city->description);
    if(VLD_STR(city->bank))
      STRFREE(city->bank);
    if(VLD_STR(city->filename))
      STRFREE(city->filename);
    if(VLD_STR(city->name))
      STRFREE(city->name);
    if(VLD_STR(city->allegiance))
      STRFREE(city->allegiance);
    if(VLD_STR(city->duke))
      STRFREE(city->duke);
    if(VLD_STR(city->baron))
      STRFREE(city->baron);
    if(VLD_STR(city->captain))
      STRFREE(city->captain);
    if(VLD_STR(city->sheriff))
      STRFREE(city->sheriff);
    if(VLD_STR(city->knight))
      STRFREE(city->knight);
    remove_all_rollcalls(city);
    DISPOSE(city);
  }

  log_string("Councils.");
  free_councils();

  log_string("Morphs.");
  for(morph = morph_start; morph; morph = morph_next)
  {
    morph_next = morph->next;
    unmorph_all(morph);
    UNLINK(morph, morph_start, morph_end, next, prev);
    free_morph(morph);
  }

  log_string("Bans.");
  free_bans();

  /*
   * Boards 
   */
  log_string("Boards.");
  for(board = first_board; board; board = board_next)
  {
    board_next = board->next;
    free_board(board);
  }

  log_string("Area and Build data.");
  for(area = first_full_area; area; area = area_next)
  {
    area_next = area->next_area;
    {
      wipe_area_resets(area);
      close_area(area);
    }
  }

  log_string("Languages.");
  while(lang = last_lang)
  {
    while(lcnv = lang->last_precnv)
    {
      UNLINK(lcnv, lang->first_precnv, lang->last_precnv, next, prev);
      STRFREE(lcnv->old);
      STRFREE(lcnv->lnew);
      DISPOSE(lcnv);
    }
    while(lcnv = lang->last_cnv)
    {
      UNLINK(lcnv, lang->first_cnv, lang->last_cnv, next, prev);
      STRFREE(lcnv->old);
      STRFREE(lcnv->lnew);
      DISPOSE(lcnv);
    }
    if(VLD_STR(lang->name))
      STRFREE(lang->name);
    if(VLD_STR(lang->alphabet))
      STRFREE(lang->alphabet);
    UNLINK(lang, first_lang, last_lang, next, prev);
    DISPOSE(lang);
  }

  /*
   * System Data 
   */
  log_string("System data.");
  if(VLD_STR(sysdata.mud_name))
    STRFREE(sysdata.mud_name);
/*
  if(VLD_STR(sysdata.time_of_max))
    STRFREE(sysdata.time_of_max);
*/
  if(VLD_STR(sysdata.mud_name))
    STRFREE(sysdata.mud_name);
  if(VLD_STR(sysdata.http))
    STRFREE(sysdata.http);
  if(VLD_STR(sysdata.clan_overseer))
    STRFREE(sysdata.clan_overseer);
  if(VLD_STR(sysdata.clan_advisor))
    STRFREE(sysdata.clan_advisor);

  log_string("Mudprog act lists.");
  free_prog_actlists();

  /*
   * Skills && herbs 
   */
  for(hash = 0; hash < 2; hash++)
  {
    int                     usetop;

    if(hash == 1)
    {
      log_string("Skills.");
      usetop = top_sn;
    }
    else
    {
      log_string("Herbs.");
      usetop = top_herb;
    }
    for(sn = 0; sn <= usetop; sn++)
    {
      if(hash == 1)
        skill = skill_table[sn];
      else
        skill = herb_table[sn];
      if(!skill)
        continue;
      if(skill->affects)
      {
        for(aff = skill->affects; aff; aff = aff_next)
        {
          aff_next = aff->next;
          STRFREE(aff->duration);
          STRFREE(aff->modifier);
          DISPOSE(aff);
        }
      }
      if(VLD_STR(skill->name))
        STRFREE(skill->name);
      if(VLD_STR(skill->noun_damage))
        STRFREE(skill->noun_damage);
      if(VLD_STR(skill->msg_off))
        STRFREE(skill->msg_off);
      if(VLD_STR(skill->hit_char))
        STRFREE(skill->hit_char);
      if(VLD_STR(skill->hit_vict))
        STRFREE(skill->hit_vict);
      if(VLD_STR(skill->hit_room))
        STRFREE(skill->hit_room);
      if(VLD_STR(skill->hit_dest))
        STRFREE(skill->hit_dest);
      if(VLD_STR(skill->miss_char))
        STRFREE(skill->miss_char);
      if(VLD_STR(skill->miss_vict))
        STRFREE(skill->miss_vict);
      if(VLD_STR(skill->miss_room))
        STRFREE(skill->miss_room);
      if(VLD_STR(skill->die_char))
        STRFREE(skill->die_char);
      if(VLD_STR(skill->die_vict))
        STRFREE(skill->die_vict);
      if(VLD_STR(skill->die_room))
        STRFREE(skill->die_room);
      if(VLD_STR(skill->imm_char))
        STRFREE(skill->imm_char);
      if(VLD_STR(skill->imm_vict))
        STRFREE(skill->imm_vict);
      if(VLD_STR(skill->imm_room))
        STRFREE(skill->imm_room);
      if(VLD_STR(skill->dice))
        STRFREE(skill->dice);
      if(VLD_STR(skill->components))
        STRFREE(skill->components);
      if(VLD_STR(skill->teachers))
        STRFREE(skill->teachers);
      skill->spell_fun = NULL;
      skill->skill_fun = NULL;
      DISPOSE(skill);
    }
  }

  /*
   * Prog Act lists 
   */
  log_string("Mprog act list.");
  for(apd = mob_act_list; apd; apd = apd_next)
  {
    apd_next = apd->next;
    DISPOSE(apd);
  }

  /*
   * Some freaking globals 
   */
  log_string("Globals.");
  if(ranged_target_name)
    STRFREE(ranged_target_name);

  log_string("News.");
  clear_news(FALSE, 0);

  log_string("Ideas.");
  freeIdeas();

  log_string("Bugs.");
  freeBugs();

  log_string("Typos.");
  freeTypos();

  log_string("Quests.");
  free_all_quest();

  log_string("Locations.");
  free_all_locations();

  log_string("Holidays.");
  free_holidays();

  log_string("Banks.");
  free_banks();

  log_string("Landmarks.");
  free_all_landmarks();

  log_string("Hints.");
  free_hints();

   fprintf( stderr, "%s", "Transfer Data.\n" );
   free_transfer(  );

  log_string("Scores.");
  free_all_scores();

  log_string("Checking hash for leftovers.");
  {
    for(hash = 0; hash < 1024; hash++)
      hash_dump(hash);
  }

  log_string("Cleanup complete, exiting.");
} /* cleanup memory */

/*
 *Clean all memory on exit to help find leaks,
 *Yeah I know, one big ugly function -Druid
 *Added to AFKmud by Samson on 5-8-03
 *
 *Added to 6Dragons by Taon on 10-26-07
 */

#ifdef WIN32
int mainthread(int argc, char **argv)
#else
int main(int argc, char **argv)
#endif
{
  struct timeval          now_time;
  char                    hostn[128];
  bool                    fCopyOver = FALSE;
   mccpusers = 0;
#ifdef I3
  int                     i3socket = -1;
#endif
  /*
   * Memory debugging if needed. 
   */
#if defined(MALLOC_DEBUG)
  malloc_debug(2);
#endif

  DONT_UPPER = FALSE;
  num_descriptors = 0;
  first_descriptor = NULL;
  last_descriptor = NULL;
  sysdata.WAIT_FOR_AUTH = TRUE;
  sysdata.Advanced_Player = TRUE;

  /*
   * Init time. 
   */
  tzset();
  gettimeofday(&now_time, NULL);
  current_time = (time_t) now_time.tv_sec;
  boot_time = time(0);  /* <-- I think this is what you wanted */
  mudstrlcpy(str_boot_time, c_time(current_time, -1), MIL);

  /*
   * Get the port number. 
   */
  port = 4100;
  if(argc > 1)
  {
    if(!is_number(argv[1]))
    {
      fprintf(stderr, "Usage: %s [port #]\n", argv[0]);
      exit(1);
    }
    else if((port = atoi(argv[1])) <= 1024)
    {
      fprintf(stderr, "Port number must be above 1024.\n");
      exit(1);
    }
    if(argv[2] && argv[2][0])
    {
      fCopyOver = TRUE;
      control = atoi(argv[3]);
#ifdef I3
      i3socket = atoi(argv[4]);
#endif
    }
    else
      fCopyOver = FALSE;
  }
  /*
   * Run the game. 
   */
#ifdef WIN32
  {
    /*
     * Initialise Windows sockets library 
     */
    unsigned short          wVersionRequested = MAKEWORD(1, 1);
    WSADATA                 wsadata;
    int                     err;

    /*
     * Need to include library: wsock32.lib for Windows Sockets 
     */
    err = WSAStartup(wVersionRequested, &wsadata);
    if(err)
    {
      fprintf(stderr, "Error %i on WSAStartup\n", err);
      exit(1);
    }
    /*
     * standard termination signals 
     */
    signal(SIGINT, (void (__cdecl *) (int))bailout);
    signal(SIGTERM, (void (__cdecl *) (int))bailout);
  }
#endif /* WIN32 */

  log_string("Booting Database");
  boot_db(fCopyOver);

#ifdef I3
  /*
   * Initialize and connect to I3
   */
  i3_startup(FALSE, port, fCopyOver);

#endif

  /*
   * Autoboot time moved here, as it needs to be after the boot_db call. 
   */
  log_string("Initializing Autoboot");
  /*
   * Init boot time. 
   */
  set_boot_time = &set_boot_time_struct;
  set_boot_time->manual = 0;
  new_boot_time = update_time(localtime(&current_time));
  /*
   * Copies *new_boot_time to new_boot_struct, and then points new_boot_time to new_boot_struct again. -- Alty 
   */
  new_boot_struct = *new_boot_time;
  new_boot_time = &new_boot_struct;
  new_boot_time->tm_mday += sysdata.autoboot_period;
  if(new_boot_time->tm_hour > 12)
    new_boot_time->tm_mday += 5;
  new_boot_time->tm_sec = 0;
  new_boot_time->tm_min = sysdata.autoboot_minute;
  new_boot_time->tm_hour = sysdata.autoboot_hour;
  /*
   * Update new_boot_time (due to day increment) 
   */
  new_boot_time = update_time(new_boot_time);
  new_boot_struct = *new_boot_time;
  new_boot_time = &new_boot_struct;
  /*
   * Bug fix submitted by Gabe Yoder 
   */
  new_boot_time_t = mktime(new_boot_time);
  reboot_check(mktime(new_boot_time));
  /*
   * Set reboot time string for do_time 
   */
  get_reboot_string();
  log_string("Initializing socket");
  if(!fCopyOver)  /* We have already the port if * * copyover'ed */
    control = init_socket(port);

  /*
   * I don't know how well this will work on an unnamed machine as I don't have one handy, and the man pages are ever-so-helpful.. -- Alty 
   */
  if(gethostname(hostn, sizeof(hostn)) < 0)
  {
    perror("main: gethostname");
    mudstrlcpy(hostn, "unresolved", 128);
  }
  log_printf("%s ready at address %s on port %d.", sysdata.mud_name, hostn, port);

  if(fCopyOver)
  {
    log_string("Initiating copyover recovery.");
    copyover_recover();
  }

  game_loop();
  closesocket(control);

#ifdef I3
  i3_shutdown(FALSE);
#endif

#ifdef WIN32
  if(service_shut_down)
  {
    CHAR_DATA              *vch;

    /*
     * Save all characters before booting. 
     */
    for(vch = first_char; vch; vch = vch->next)
      if(!IS_NPC(vch))
      {
        shutdown_checkpoint();
        save_char_obj(vch);
      }
  }
  /*
   * Shut down Windows sockets 
   */

  WSACleanup(); /* clean up */
  kill_timer(); /* stop timer thread */
#endif

  /*
   * That's all, folks. 
   */
  log_string("Normal termination of game.");
  if(cleanedupmemory == FALSE)
  {
    log_string("Cleaning up Memory.");
    cleanup_memory();
  }

  exit(0);
}

bool remove_printf(char *fmt, ...)
{
  char                    buf[MSL * 2];
  va_list                 args;

  va_start(args, fmt);
  vsnprintf(buf, MSL * 2, fmt, args);
  va_end(args);
  if(remove(buf))
    return TRUE;
  else
    return FALSE;
}

bool exists_file(char *name)
{
  struct stat             fst;

  /*
   * Stands to reason that if there ain't a name to look at, it damn well don't exist! 
   */
  if(!name || !str_cmp(name, ""))
    return FALSE;
  if(stat(name, &fst) != -1)
    return TRUE;
  else
    return FALSE;
}

void open_mud_log(void)
{
  FILE                   *error_log;
  char                    buf[MIL], buf2[MIL];
  int                     logindex, logdel;

  for(logindex = 1000;; logindex++)
  {
    snprintf(buf, MIL, "log/%d.log", logindex);
    if(exists_file(buf))
      continue;
    else if(logindex > 1025)
    {
      for(logdel = 1000;; logdel++)
      {
        snprintf(buf2, MIL, "log/%d.log", logdel);
        if(logdel <= logindex)
        {
          if(exists_file(buf2))
          {
            if(!remove(buf2))
              continue;
            else if(errno != ENOENT)
              continue;
          }
          else
          {
            logindex = 999;
            break;
          }
        }
        else
        {
          logindex = 999;
          break;
        }
      }
    }
    else
      break;
  }
  if(!(error_log = FileOpen(buf, "a")))
  {
    fprintf(stderr, "Unable to append to %s.", buf);
    return;
  }
#if defined(WIN32)
  dup2(fileno(error_log), _fileno(stderr));
#else
  dup2(fileno(error_log), STDERR_FILENO);
#endif
  FileClose(error_log);
}

/* set up MXP */
void turn_on_mxp(DESCRIPTOR_DATA *d)
{
  d->mxp = TRUE;  /* turn it on now */
  write_to_buffer(d, (const char *)start_mxp_str, 0);
  write_to_buffer(d, MXPMODE(6), 0);  /* permanent secure mode */
  write_to_buffer(d, MXPTAG("!-- Set up MXP elements --"), 0);
  /*
   * Exit tag 
   */
  write_to_buffer(d, MXPTAG("!ELEMENT Ex '<send>' FLAG=RoomExit"), 0);
  /*
   * Room description tag 
   */
  write_to_buffer(d, MXPTAG("!ELEMENT rdesc '<p>' FLAG=RoomDesc"), 0);
  /*
   * Get an item tag (for things on the ground) 
   */
  write_to_buffer(d, MXPTAG
                  ("!ELEMENT Get \"<send href='"
                   "get &#39;&name;&#39;|"
                   "examine &#39;&name;&#39;|" "drink &#39;&name;&#39;" "' " "hint='RH mouse click to use this object|" "Get &desc;|" "Examine &desc;|" "Drink from &desc;" "'>\" ATT='name desc'"), 0);
  /*
   * Drop an item tag (for things in the inventory) 
   */
  write_to_buffer(d, MXPTAG
                  ("!ELEMENT Drop \"<send href='"
                   "drop &#39;&name;&#39;|"
                   "examine &#39;&name;&#39;|"
                   "look in &#39;&name;&#39;|"
                   "wear &#39;&name;&#39;|"
                   "eat &#39;&name;&#39;|"
                   "drink &#39;&name;&#39;"
                   "' " "hint='RH mouse click to use this object|" "Drop &desc;|" "Examine &desc;|" "Look inside &desc;|" "Wear &desc;|" "Eat &desc;|" "Drink &desc;" "'>\" ATT='name desc'"), 0);
  /*
   * Bid an item tag (for things in the auction) 
   */
  write_to_buffer(d, MXPTAG("!ELEMENT Bid \"<send href='bid &#39;&name;&#39;' " "hint='Bid for &desc;'>\" " "ATT='name desc'"), 0);
  /*
   * List an item tag (for things in a shop) 
   */
  write_to_buffer(d, MXPTAG("!ELEMENT List \"<send href='buy &#39;&name;&#39;' " "hint='Buy &desc;'>\" " "ATT='name desc'"), 0);
  /*
   * Player tag (for who lists, tells etc.) 
   */
  write_to_buffer(d, MXPTAG("!ELEMENT Player \"<send href='tell &#39;&name;&#39; ' " "hint='Send a message to &name;' prompt>\" " "ATT='name'"), 0);
} /* end of turn_on_mxp */

int init_socket(int mudport)
{
  struct sockaddr_in      sa;
  int                     x = 1;
  int                     fd;

  if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("Init_socket: socket");
    exit(1);
  }

  if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&x, sizeof(x)) < 0)
  {
    perror("Init_socket: SO_REUSEADDR");
    closesocket(fd);
    exit(1);
  }

#if defined(SO_DONTLINGER) && !defined(SYSV)
  {
    struct linger           ld;

    ld.l_onoff = 1;
    ld.l_linger = 1000;

    if(setsockopt(fd, SOL_SOCKET, SO_DONTLINGER, (const char *)&ld, sizeof(ld)) < 0)
    {
      perror("Init_socket: SO_DONTLINGER");
      closesocket(fd);
      exit(1);
    }
  }
#endif

  memset(&sa, '\0', sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(mudport);

  if(bind(fd, (struct sockaddr *)&sa, sizeof(sa)) == -1)
  {
    perror("Init_socket: bind");
    closesocket(fd);
    exit(1);
  }

  if(listen(fd, 50) < 0)
  {
    perror("Init_socket: listen");
    closesocket(fd);
    exit(1);
  }

  return fd;
}

/*
static void SegVio()
{
  CHAR_DATA *ch;

  log_string("SEGMENTATION VIOLATION");
  log_string(lastplayercmd);
  for(ch = first_char; ch; ch = ch->next)
    log_printf("%cPC: %-20s room: %d", IS_NPC(ch) ? 'N' : ' ', ch->name, ch->in_room->vnum);
  exit(0);
}
*/

/*
 * LAG alarm!        -Thoric
 */
/* LAG alarm! - Thoric */
void caught_alarm( int signum )
{
   bug( "%s: ALARM CLOCK!  In section %s", __FUNCTION__, alarm_section );
   echo_to_all( AT_IMMORT, "Alas, the hideous malevalent entity known only as 'Lag' rises once more!\r\n", ECHOTAR_ALL );
   if( newdesc )
   {
      FD_CLR( newdesc, &in_set );
      FD_CLR( newdesc, &out_set );
      FD_CLR( newdesc, &exc_set );
      log_string( "clearing newdesc" );
   }
}


bool check_bad_desc( int desc )
{
   if( FD_ISSET( desc, &exc_set ) )
   {
      FD_CLR( desc, &in_set );
      FD_CLR( desc, &out_set );
      log_string( "Bad FD caught and disposed." );
      return true;
   }
   return false;
}


void accept_new( void )
{
   static struct timeval null_time;
   DESCRIPTOR_DATA *d;

   FD_ZERO( &in_set );
   FD_ZERO( &out_set );
   FD_ZERO( &exc_set );

   FD_SET( control, &in_set );

   maxdesc = control;

   for( d = first_descriptor; d; d = d->next )
   {
      maxdesc = UMAX( maxdesc, d->descriptor );
      FD_SET( d->descriptor, &in_set );
      FD_SET( d->descriptor, &out_set );
      FD_SET( d->descriptor, &exc_set );
   }

   if( select( maxdesc + 1, &in_set, &out_set, &exc_set, &null_time ) < 0 )
   {
      perror( "accept_new: select: poll" );
      exit( 1 );
   }

   if( FD_ISSET( control, &exc_set ) )
   {
      bug( "Exception raise on controlling descriptor %d", control );
      FD_CLR( control, &in_set );
      FD_CLR( control, &out_set );
   }
   else if( FD_ISSET( control, &in_set ) )
      new_descriptor(  );
}

void game_loop( void )
{
   DESCRIPTOR_DATA *d;
   struct timeval last_time;
   char cmdline[MIL];

#ifndef WIN32
   signal( SIGPIPE, SIG_IGN );
   signal( SIGALRM, caught_alarm );
#endif

   fprintf( stderr,
            ".~`~.~`~.~`~.~`~.~`~.~`~.~`~.~`~.~`~.~`~.~`~[  Game Loop  ]~`~.~`~.~`~.~`~.~`~.~`~.~`~.~`~.~`~.~`~.~`~.\n" );

   gettimeofday( &last_time, NULL );
   current_time = ( time_t ) last_time.tv_sec + TIME_MODIFY;

   /*
    * Main loop 
    */
   while( !mud_down )
   {
      accept_new(  );

      /*
       * Kick out descriptors with raised exceptions
       * or have been idle, then check for input.
       */
      for( d = first_descriptor; d; d = d_next )
      {
         d_next = d->next;

         if( ++d->tempidle >= PULSE_PER_SECOND )
         {
            d->idle++;  /* make it so a descriptor can idle out */
            d->tempidle = 0;
         }
         if( d->character && ++d->character->temp_played >= PULSE_PER_SECOND )
         {
            if( d->idle <= 0 )   /* If they are idle they aren't actually playing */
               d->character->played++; /* Increase played time (goes by seconds) */
            d->character->temp_played = 0;
         }
         if( FD_ISSET( d->descriptor, &exc_set ) )
         {
            FD_CLR( d->descriptor, &in_set );
            FD_CLR( d->descriptor, &out_set );
            if( d->character && ( d->connected == CON_PLAYING || d->connected == CON_EDITING ) )
               save_char_obj( d->character );
            d->outtop = 0;
            close_socket( d, true );
            continue;
         }
         else if( ( !d->character && d->idle > 60 )   /* 1 minute */
                  || ( d->connected != CON_PLAYING && d->idle > 180 )   /* 3 minutes */
                  || ( d->idle > 3600 && get_trust( d->character ) < LEVEL_STAFF ) )   /* 1 hour */
         {
            write_to_descriptor( d, "Idle timeout... disconnecting.\r\n", 0 );
            d->outtop = 0;
            close_socket( d, true );
            continue;
         }
         else
         {
            d->fcommand = false;

            if( FD_ISSET( d->descriptor, &in_set ) )
            {
               d->idle = 0;
               d->tempidle = 0;
               if( d->character )
                  d->character->timer = 0;
               if( !read_from_descriptor( d ) )
               {
                  FD_CLR( d->descriptor, &out_set );
                  if( d->character && ( d->connected == CON_PLAYING || d->connected == CON_EDITING ) )
                     save_char_obj( d->character );
                  d->outtop = 0;
                  close_socket( d, false );
                  continue;
               }
            }

            if( d->character && d->character->wait > 0 )
            {
               --d->character->wait;
               continue;
            }

            read_from_buffer( d );
            if( d->incomm[0] && d->incomm[0] != '\0' )
            {
               d->fcommand = true;
               stop_idling( d->character );

               mudstrlcpy( cmdline, d->incomm, sizeof( cmdline ) );
               d->incomm[0] = '\0';

               if( d->character )
                  set_cur_char( d->character );

               if( d->pagepoint )
                  set_pager_input( d, cmdline );
               else
               {
                  switch ( d->connected )
                  {
                     default:
                        genesis(d, cmdline);
                        break;

                     case CON_PLAYING:
                        interpret( d->character, cmdline );
                        break;

                     case CON_EDITING:
                        edit_buffer( d->character, cmdline );
                        break;
                  }
               }
            }
            /*
             * later should add in a config for getting the message and time in mins to display it 
             */
            else if( d->character && d->character->pcdata && xIS_SET( d->character->act, PLR_KEEPALIVE )
                     && d->outtime < ( current_time - ( 30 * URANGE( 1, d->character->pcdata->kltime, 8 ) ) ) )
               write_to_descriptor( d, keep_alive_msg[number_range( 0, 2 )], 0 );
         }
         if( d == last_descriptor )
            break;
      }

#ifdef I3
      i3_loop(  );
#endif


      /*
       * Autonomous game motion. 
       */
      update_handler(  );

      /*
       * Output. 
       */
      for( d = first_descriptor; d; d = d_next )
      {
         d_next = d->next;

         if( ( d->fcommand || d->outtop > 0 ) && FD_ISSET( d->descriptor, &out_set ) )
         {
            if( d->pagepoint )
            {
               if( !pager_output( d ) )
               {
                  if( d->character && ( d->connected == CON_PLAYING || d->connected == CON_EDITING ) )
                     save_char_obj( d->character );
                  d->outtop = 0;
                  close_socket( d, false );
               }
            }
            else if( !flush_buffer( d, true ) )
            {
               if( d->character && ( d->connected == CON_PLAYING || d->connected == CON_EDITING ) )
                  save_char_obj( d->character );
               d->outtop = 0;
               close_socket( d, false );
            }
         }
         if( d == last_descriptor )
            break;
      }

      /*
       * Synchronize to a clock.
       * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
       * Careful here of signed versus unsigned arithmetic.
       */
      {
         struct timeval now_time;
         long secDelta;
         long usecDelta;

         gettimeofday( &now_time, NULL );
         usecDelta = ( ( int )last_time.tv_usec ) - ( ( int )now_time.tv_usec ) + 1000000 / PULSE_PER_SECOND;
         secDelta = ( ( int )last_time.tv_sec ) - ( ( int )now_time.tv_sec );
         while( usecDelta < 0 )
         {
            usecDelta += 1000000;
            secDelta -= 1;
         }

         while( usecDelta >= 1000000 )
         {
            usecDelta -= 1000000;
            secDelta += 1;
         }

         if( secDelta > 0 || ( secDelta == 0 && usecDelta > 0 ) )
         {
            struct timeval stall_time;

            stall_time.tv_usec = usecDelta;
            stall_time.tv_sec = secDelta;
#ifdef WIN32
            Sleep( ( stall_time.tv_sec * 1000L ) + ( stall_time.tv_usec / 1000L ) );
#else
            if( select( 0, NULL, NULL, NULL, &stall_time ) < 0 && errno != EINTR )
            {
               perror( "game_loop: select: stall" );
               exit( 1 );
            }
#endif
         }
      }

      gettimeofday( &last_time, NULL );
      current_time = ( time_t ) last_time.tv_sec;
      current_time += ( time_t ) TIME_MODIFY;
   }

   fflush( stderr ); /* make sure strerr is flushed */
}

void new_descriptor( void )
{
   DESCRIPTOR_DATA *dnew;
   char buf[MSL];
   char log_buf[MSL];
   struct sockaddr_in sock;
   struct hostent *from;
#ifndef WIN32
   socklen_t size;
#else
   unsigned int size;
   unsigned long arg = 1;
#endif
   int desc;

   size = sizeof( sock );
   if( check_bad_desc( control ) )
   {
      set_alarm( 0 );
      return;
   }

   set_alarm( 20 );
   alarm_section = ( char * )"new_descriptor::accept";
   if( ( desc = accept( control, ( struct sockaddr * )&sock, &size ) ) < 0 )
   {
      perror( "New_descriptor: accept" );
      bug( "%s: accept", __FUNCTION__ );
      set_alarm( 0 );
      return;
   }

   if( check_bad_desc( control ) )
   {
      set_alarm( 0 );
      return;
   }

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif

   set_alarm( 20 );
   alarm_section = ( char * )"new_descriptor: after accept";

#ifdef WIN32
   if( ioctlsocket( desc, FIONBIO, &arg ) == -1 )
#else
   if( fcntl( desc, F_SETFL, FNDELAY ) == -1 )
#endif
   {
      perror( "New_descriptor: fcntl: FNDELAY" );
      set_alarm( 0 );
      return;
   }
   if( check_bad_desc( control ) )
      return;

   CREATE( dnew, DESCRIPTOR_DATA, 1 );
   dnew->next = NULL;
   dnew->descriptor = desc;
   dnew->connected = CON_GET_NAME;
   dnew->outsize = 2000;
   dnew->idle = 0;
   dnew->tempidle = 0;
   dnew->lines = 0;
   dnew->scrlen = 24;
   dnew->port = ntohs( sock.sin_port );
   dnew->newstate = 0;
   dnew->prevcolor = 0x07;
   dnew->can_compress = false;
   dnew->speed = 32;
   CREATE( dnew->mccp, MCCP, 1 );
   CREATE( dnew->outbuf, char, dnew->outsize );

   mudstrlcpy( buf, inet_ntoa( sock.sin_addr ), sizeof( buf ) );
      dnew->host = STRALLOC( buf );

   log_printf_plus( LOG_COMM, LEVEL_STAFF, "Host: %s, Port: %d.", dnew->host, dnew->port );


   if( check_total_bans( dnew ) )
   {
      write_to_descriptor( dnew, "Your site has been banned from this Mud.\r\n", 0 );
      free_desc( dnew );
      set_alarm( 0 );
      return;
   }

   LINK( dnew, first_descriptor, last_descriptor, next, prev );

   /*
    * MCCP Compression 
    */
   write_to_buffer( dnew, ( const char * )will_compress2_str, 0 );

   /*
    * Send the greeting. 
    */
   if( !help_greeting || help_greeting[0] == '\0' )
      send_to_desc( "There is currently no greeting file to display.\r\n", dnew );
   else if( help_greeting[0] == '.' )
      send_to_desc( help_greeting + 1, dnew );
   else
      send_to_desc( help_greeting, dnew );


   if( ++num_descriptors > sysdata.maxplayers )
   {
      sysdata.maxplayers = num_descriptors;

      if( sysdata.maxplayers > sysdata.alltimemax )
      {
         sysdata.time_of_max = current_time;
         sysdata.alltimemax = sysdata.maxplayers;
         snprintf( buf, MSL, "Broke all-time maximum player record: %d", sysdata.alltimemax );
         announce( buf );
         save_sysdata(sysdata);
      }
   }

   set_alarm( 0 );
}

void free_desc(DESCRIPTOR_DATA *d)
{
  closesocket(d->descriptor);
  if(d->host)
    STRFREE(d->host);
  if(d->outbuf)
    DISPOSE(d->outbuf);
  if(d->pagebuf)
    DISPOSE(d->pagebuf);
  if(d->mccp)
   DISPOSE( d->mccp );
  DISPOSE(d);
}

void close_socket(DESCRIPTOR_DATA *dclose, bool force)
{
  CHAR_DATA              *ch;
  DESCRIPTOR_DATA        *d;
  bool                    DoNotUnlink = FALSE;
  AUTH_LIST              *old_auth;
  char                    buf[MSL];

  /*
   * flush outbuf 
   */
  if(!force && dclose->outtop > 0)
    flush_buffer(dclose, FALSE);
  /*
   * say bye to whoever's snooping this descriptor 
   */

  if(dclose->snoop_by)
    write_to_buffer(dclose->snoop_by, "Your victim has left the game.\r\n", 0);

  /*
   * stop snooping everyone else 
   */
  for(d = first_descriptor; d; d = d->next)
    if(d->snoop_by == dclose)
    {
      write_to_buffer(dclose, "You stop snooping your victim.\r\n", 0);
      d->snoop_by = NULL;
    }

  /*
   * Check for switched people who go link-dead. -- Altrag 
   */
  if(dclose->original)
  {
    if((ch = dclose->character) != NULL)
      do_return(ch, (char *)"");
    else
    {
      bug("Close_socket: dclose->original without character %s", (dclose->original->name ? dclose->original->name : "unknown"));
      dclose->character = dclose->original;
      dclose->original = NULL;
    }
  }
  ch = dclose->character;
  /*
   * sanity check :(
   */
  if(!dclose->prev && dclose != first_descriptor)
  {
    DESCRIPTOR_DATA        *dp, *dn;

    bug("Close_socket: %s desc:%p != first_desc:%p and desc->prev = NULL!", ch ? ch->name : d->host, dclose, first_descriptor);
    dp = NULL;
    for(d = first_descriptor; d; d = dn)
    {
      dn = d->next;
      if(d == dclose)
      {
        bug("Close_socket: %s desc:%p found, prev should be:%p, fixing.", ch ? ch->name : d->host, dclose, dp);
        dclose->prev = dp;
        break;
      }
      dp = d;
    }
    if(!dclose->prev)
    {
      bug("Close_socket: %s desc:%p could not be found!.", ch ? ch->name : dclose->host, dclose);
      DoNotUnlink = TRUE;
    }
  }
  if(!dclose->next && dclose != last_descriptor)
  {
    DESCRIPTOR_DATA        *dp, *dn;

    bug("Close_socket: %s desc:%p != last_desc:%p and desc->next = NULL!", ch ? ch->name : d->host, dclose, last_descriptor);
    dn = NULL;
    for(d = last_descriptor; d; d = dp)
    {
      dp = d->prev;
      if(d == dclose)
      {
        bug("Close_socket: %s desc:%p found, next should be:%p, fixing.", ch ? ch->name : d->host, dclose, dn);
        dclose->next = dn;
        break;
      }
      dn = d;
    }
    if(!dclose->next)
    {
      bug("Close_socket: %s desc:%p could not be found!.", ch ? ch->name : dclose->host, dclose);
      DoNotUnlink = TRUE;
    }
  }
  if(dclose->character)
  {
    snprintf(buf, MSL, "Closing link to %s.", ch->pcdata->filename);
    log_string_plus(buf, LOG_COMM, (short)UMAX((int)sysdata.log_level, (int)ch->level));
    /*
     * Link dead auth -- Rantic 
     */
    old_auth = get_auth_name(ch->name);
    if(old_auth != NULL && old_auth->state == AUTH_ONLINE)
    {
      old_auth->state = AUTH_LINK_DEAD;
      save_auth_list(  );
    }

    if(dclose->connected == CON_PLAYING || dclose->connected == CON_EDITING || dclose->connected == CON_DELETE || dclose->connected == CON_ROLL_STATS)
    {
      snprintf(buf, MSL, "Closing link to %s. (link-dead) ", ch->pcdata->filename);
      log_string_plus(buf, LOG_COMM, (short)UMAX((int)sysdata.log_level, (int)ch->level));
      {
        struct tm              *tme;
        time_t                  now;
        char                    day[50];

        now = time(0);
        tme = localtime(&now);
        strftime(day, 50, "%a %b %d %H:%M:%S %Y", tme);
        snprintf(buf, MSL, "%20s %24s has gone link-dead", ch->pcdata->filename, day);
        append_to_file(LAST_FILE, buf);
      }
            act( AT_ACTION, "$n has lost $s link.", ch, NULL, NULL, TO_CANSEE );
            ch->desc = NULL;

      if(IS_AFFECTED(ch, AFF_SHRINK))
      {
        xREMOVE_BIT(ch->affected_by, AFF_SHRINK);
        if(ch->height != race_table[ch->race]->height)
          ch->height = race_table[ch->race]->height;
        if(ch->weight != race_table[ch->race]->weight)
          ch->weight = race_table[ch->race]->weight;

      }

      if(IS_AFFECTED(ch, AFF_ENLARGE))
      {
        xREMOVE_BIT(ch->affected_by, AFF_ENLARGE);
        if(ch->height != race_table[ch->race]->height)
          ch->height = race_table[ch->race]->height;
        if(ch->weight != race_table[ch->race]->weight)
          ch->weight = race_table[ch->race]->weight;

      }
      /*
       * Strip the boost  
       */
      if(IS_AFFECTED(ch, AFF_BOOST))
      {
        if(IS_AFFECTED(ch, AFF_BOOST))
        {
          affect_strip(ch, gsn_boost);
          xREMOVE_BIT(ch->affected_by, AFF_BOOST);
        }
        if(IS_AFFECTED(ch, AFF_BOOST))
        {
          affect_strip(ch, gsn_boost);
          xREMOVE_BIT(ch->affected_by, AFF_BOOST);
        }
        if(IS_AFFECTED(ch, AFF_BOOST))
        {
          affect_strip(ch, gsn_boost);
          xREMOVE_BIT(ch->affected_by, AFF_BOOST);
        }

      }
    }
    else
    {
      /*
       * clear descriptor pointer to get rid of bug message in log 
       */
      dclose->character->desc = NULL;
      free_char(dclose->character);
    }
  }
  if(!DoNotUnlink)
  {
    /*
     * make sure loop doesn't get messed up 
     */
    if(d_next == dclose)
      d_next = d_next->next;
    UNLINK(dclose, first_descriptor, last_descriptor, next, prev);
  }

  if((int)dclose->descriptor == maxdesc)
    --maxdesc;
  free_desc(dclose);
  --num_descriptors;
  return;
}

bool read_from_descriptor(DESCRIPTOR_DATA *d)
{
  unsigned int            iStart;
  int                     iErr;

  /*
   * Hold horses if pending command already. 
   */
  if(d->incomm[0] != '\0')
    return TRUE;

  /*
   * Check for overflow. 
   */
  iStart = strlen(d->inbuf);
  if(iStart >= sizeof(d->inbuf) - 250)
  {
    log_printf("%s input overflow!", d->host);
    write_to_descriptor(d, "\r\n*** PUT A LID ON IT!!! ***\r\nYou cannot enter the same command more than 250 consecutive times!\r\n", 0);
    return FALSE;
  }

  for(;;)
  {
    int                     nRead;

    nRead = recv(d->descriptor, d->inbuf + iStart, sizeof(d->inbuf) - 10 - iStart, 0);
#ifdef WIN32
    iErr = WSAGetLastError();
#else
    iErr = errno;
#endif
    if(nRead > 0)
    {
      iStart += nRead;

      if(d->inbuf[iStart - 1] == '\r' || d->inbuf[iStart - 1] == '\n')
        break;

      /*
       * Reached limit so have to let it carry on 
       */
      if(iStart >= (sizeof(d->inbuf) - 10))
        break;
    }

    else if(nRead == 0)
    {
      char                    buf[MSL];

      snprintf(buf, sizeof(buf), "EOF encountered on read from host %s.", d->host);
      log_string_plus(buf, LOG_COMM, sysdata.log_level);
      return FALSE;
    }
    else if(iErr == EWOULDBLOCK)
      break;
    else
    {
      perror("Read_from_descriptor");
      return FALSE;
    }
  }

  d->inbuf[iStart] = '\0';
  return TRUE;
}




/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer( DESCRIPTOR_DATA * d )
{
   int i, j, k, iac = 0;

   /*
    * Hold horses if pending command already.
    */
   if( d->incomm[0] != '\0' )
      return;

// Input by Volk - command clearer
  if(str_has_command(d->inbuf, "clear"))
  {
    d->inbuf[0] = '\0';
    send_to_char("You've cleared all your pending commands.\r\n", d->character);
    return;
  }

   /*
    * Look for at least one new line.
    */
   for( i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r' && i < MAX_INBUF_SIZE; i++ )
   {
      if( d->inbuf[i] == '\0' )
         return;
   }

   /*
    * Canonical input processing.
    */
   for( i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
   {
      if( k >= 254 )
      {
         write_to_descriptor( d, "Line too long.\r\n", 0 );

         /*
          * skip the rest of the line 
          */
         /*
          * for ( ; d->inbuf[i] != '\0' || i>= MAX_INBUF_SIZE ; i++ )
          * {
          * if ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
          * break;
          * }
          */
         d->inbuf[i] = '\n';
         d->inbuf[i + 1] = '\0';
         break;
      }

      if( d->inbuf[i] == ( signed char )IAC )
         iac = 1;
      else if( iac == 1
               && ( d->inbuf[i] == ( signed char )DO || d->inbuf[i] == ( signed char )DONT
                    || d->inbuf[i] == ( signed char )WILL ) )
         iac = 2;
      else if( iac == 2 )
      {
         iac = 0;
         if( d->inbuf[i] == ( signed char )TELOPT_COMPRESS2 )
         {
            if( d->inbuf[i - 1] == ( signed char )DO )
               compressStart( d );
            else if( d->inbuf[i - 1] == ( signed char )DONT )
               compressEnd( d );
         }
      }
      else if( d->inbuf[i] == '\b' && k > 0 )
         --k;
      else if( isascii( d->inbuf[i] ) && isprint( d->inbuf[i] ) )
         d->incomm[k++] = d->inbuf[i];
   }

   /*
    * Finish off the line.
    */
   if( k == 0 )
      d->incomm[k++] = ' ';
   d->incomm[k] = '\0';

   /*
    * Deal with bozos with #repeat 1000 ...
    */
   if( k > 1 || d->incomm[0] == '!' )
   {
      if( d->incomm[0] != '!' && strcmp( d->incomm, d->inlast ) )
      {
         d->repeat = 0;
      }
      else
      {
         if( ++d->repeat >= 250 )
         {
/*		log_printf( "%s input spamming!", d->host );
*/
            write_to_descriptor( d,
                                 "\r\n*** PUT A LID ON IT!!! ***\r\nYou cannot enter the same command more than 250 consecutive times!\r\n",
                                 0 );
            mudstrlcpy( d->incomm, "quit", MAX_INPUT_LENGTH );
         }
      }
   }

   /*
    * Do '!' substitution.
    */
   if( d->incomm[0] == '!' )
      mudstrlcpy( d->incomm, d->inlast, MAX_INPUT_LENGTH );
   else
      mudstrlcpy( d->inlast, d->incomm, MAX_INPUT_LENGTH );

   /*
    * Shift the input buffer.
    */
   while( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
      i++;
   for( j = 0; ( d->inbuf[j] = d->inbuf[i + j] ) != '\0'; j++ )
      ;
   return;
}



/*
 * Low level output function.
 */
bool flush_buffer(DESCRIPTOR_DATA *d, bool fPrompt)
{
  char                    buf[MIL * 5];

  /*
   * If buffer has more than 4K inside, spit out .5K at a time   -Thoric
   */
  if(!mud_down && d->outtop > client_speed(d->speed))
  {
    memcpy(buf, d->outbuf, client_speed(d->speed));
    d->outtop -= client_speed(d->speed);
    memmove(d->outbuf, d->outbuf + client_speed(d->speed), d->outtop);
    if(d->snoop_by)
    {
      char                    snoopbuf[MIL];

      buf[client_speed(d->speed)] = '\0';
      if(d->character && d->character->name)
      {
        if((d->original && d->original->name))
          snprintf(snoopbuf, MIL, "%s (%s)", d->character->name, d->original->name);
        else
          snprintf(snoopbuf, MIL, "%s", d->character->name);
        write_to_buffer(d->snoop_by, snoopbuf, 0);
      }
      write_to_buffer(d->snoop_by, "% ", 2);
      write_to_buffer(d->snoop_by, buf, 0);
    }

    if(!write_to_descriptor(d, buf, client_speed(d->speed)))
    {
      d->outtop = 0;
      return FALSE;
    }
    return TRUE;
  }

  /*
   * Bust a prompt. 
   */
  if(fPrompt && !mud_down && d->connected == CON_PLAYING)
  {
    CHAR_DATA              *ch;

    ch = d->original ? d->original : d->character;
    if(xIS_SET(ch->act, PLR_BLANK))
      write_to_buffer(d, "\r\n", 2);

    /*
     * Stop possible color bleeding to players 
     */
    if(ch && !IS_NPC(ch) && xIS_SET(ch->act, PLR_ANSI))
    {
      write_to_buffer(d, ANSI_RESET, 4);
      d->prevcolor = 0x08;
    }
    if(xIS_SET(ch->act, PLR_PROMPT))
      display_prompt(d);
    if(xIS_SET(ch->act, PLR_TELNET_GA))
      write_to_buffer(d, go_ahead_str, 0);
  }

  /*
   * Short-circuit if nothing to write.
   */
  if(d->outtop == 0)
    return TRUE;

  /*
   * Snoop-o-rama.
   */
  if(d->snoop_by)
  {
    /*
     * without check, 'force mortal quit' while snooped caused crash, -h 
     */
    if(d->character && d->character->name)
    {
      /*
       * Show original snooped names. -- Altrag 
       */
      if(d->original && d->original->name)
        snprintf(buf, MIL * 5, "%s (%s)", d->character->name, d->original->name);
      else
        snprintf(buf, MIL * 5, "%s", d->character->name);
      write_to_buffer(d->snoop_by, buf, 0);
    }
    write_to_buffer(d->snoop_by, "% ", 2);
    write_to_buffer(d->snoop_by, d->outbuf, d->outtop);
  }

  /*
   * OS-dependent output.
   */
  if(!write_to_descriptor(d, d->outbuf, d->outtop))
  {
    d->outtop = 0;
    return FALSE;
  }
  else
  {
    d->outtop = 0;
    return TRUE;
  }
}

/*
 * MXP code by Nick Gammon.
 *
 * See: http://www.gammon.com.au
 * Queries:  http://www.gammon.com.au/forum/
 *
 * Count number of mxp tags need converting
 *    ie. < becomes &lt;
 *        > becomes &gt;
 *        & becomes &amp;
 */
int count_mxp_tags(const int bMXP, const char *txt, int length)
{
  char                    c;
  const char             *p;
  int                     count;
  int                     bInTag = FALSE;
  int                     bInEntity = FALSE;

  for(p = txt, count = 0; length > 0; p++, length--)
  {
    c = *p;

    if(bInTag)
    { /* in a tag, eg. <send> */
      if(!bMXP)
        count--;  /* not output if not MXP */
      if(c == MXP_ENDc)
        bInTag = FALSE;
    } /* end of being inside a tag */
    else if(bInEntity)
    { /* in a tag, eg. <send> */
      if(!bMXP)
        count--;  /* not output if not MXP */
      if(c == ';')
        bInEntity = FALSE;
    } /* end of being inside a tag */
    else
      switch (c)
      {

        case MXP_BEGc:
          bInTag = TRUE;
          if(!bMXP)
            count--;  /* not output if not MXP */
          break;

        case MXP_ENDc: /* shouldn't get this case */
          if(!bMXP)
            count--;  /* not output if not MXP */
          break;

        case MXP_AMPc:
          bInEntity = TRUE;
          if(!bMXP)
            count--;  /* not output if not MXP */
          break;

        default:
          if(bMXP)
          {
            switch (c)
            {
              case '<':  /* < becomes &lt; */
              case '>':  /* > becomes &gt; */
                count += 3;
                break;

              case '&':
                count += 4; /* & becomes &amp; */
                break;

              case '"':  /* " becomes &quot; */
                count += 5;
                break;
            } /* end of inner switch */
          } /* end of MXP enabled */
      } /* end of switch on character */

  } /* end of counting special characters */

  return count;
} /* end of count_mxp_tags */

void convert_mxp_tags(const int bMXP, char *dest, const char *src, int length)
{
  char                    c;
  const char             *ps;
  char                   *pd;
  int                     bInTag = FALSE;
  int                     bInEntity = FALSE;

  for(ps = src, pd = dest; length > 0; ps++, length--)
  {
    c = *ps;
    if(bInTag)
    { /* in a tag, eg. <send> */
      if(c == MXP_ENDc)
      {
        bInTag = FALSE;
        if(bMXP)
          *pd++ = '>';
      }
      else if(bMXP)
        *pd++ = c;  /* copy tag only in MXP mode */
    } /* end of being inside a tag */
    else if(bInEntity)
    { /* in a tag, eg. <send> */
      if(bMXP)
        *pd++ = c;  /* copy tag only in MXP mode */
      if(c == ';')
        bInEntity = FALSE;
    } /* end of being inside a tag */
    else
      switch (c)
      {
        case MXP_BEGc:
          bInTag = TRUE;
          if(bMXP)
            *pd++ = '<';
          break;

        case MXP_ENDc: /* shouldn't get this case */
          if(bMXP)
            *pd++ = '>';
          break;

        case MXP_AMPc:
          bInEntity = TRUE;
          if(bMXP)
            *pd++ = '&';
          break;

        default:
          if(bMXP)
          {
            switch (c)
            {
              case '<':
                memcpy(pd, "&lt;", 4);
                pd += 4;
                break;

              case '>':
                memcpy(pd, "&gt;", 4);
                pd += 4;
                break;

              case '&':
                memcpy(pd, "&amp;", 5);
                pd += 5;
                break;

              case '"':
                memcpy(pd, "&quot;", 6);
                pd += 6;
                break;

              default:
                *pd++ = c;
                break;  /* end of default */

            } /* end of inner switch */
          }
          else
            *pd++ = c;  /* not MXP - just copy character */
          break;

      } /* end of switch on character */

  } /* end of converting special characters */
} /* end of convert_mxp_tags */

/*
 * Append onto an output buffer.
 */
void write_to_buffer(DESCRIPTOR_DATA *d, const char *txt, unsigned int length)
{
  int                     origlength;

  if(!d)
  {
    bug("Write_to_buffer: NULL descriptor");
    return;
  }
  /*
   * Normally a bug... but can happen if loadup is used. 
   */
  if(!d->outbuf)
    return;
  /*
   * Find length in case caller didn't. 
   */
  if(length <= 0)
    length = strlen(txt);

  origlength = length;

  /*
   * work out how much we need to expand/contract it 
   */
  length += count_mxp_tags(d->mxp, txt, length);

  /*
   * Initial \r\n if needed. 
   */
  if(d->outtop == 0 && !d->fcommand)
  {
    d->outbuf[0] = '\n';
    d->outbuf[1] = '\r';
    d->outtop = 2;
  }
  /*
   * Expand the buffer as needed. 
   */
  while(d->outtop + length >= d->outsize)
  {
    if(d->outsize > 32000)
    {
      /*
       * empty buffer 
       */
      d->outtop = 0;
      if(!d->character)
        write_to_buffer(d, "\r\nBuffer overflowed and some of it was cleared.\r\n", 0);
      else
        send_to_char("\r\n&RBuffer overflowed and some of it was cleared.&D\r\n", d->character);
      return;
    }
    d->outsize *= 2;
    RECREATE(d->outbuf, char, d->outsize);
  }
  /*
   * Copy. 
   */
  // strncpy(d->outbuf + d->outtop, txt, length);
  convert_mxp_tags(d->mxp, d->outbuf + d->outtop, txt, origlength);
  d->outtop += length;
  d->outbuf[d->outtop] = '\0';
  return;
}

/*
 * Added block checking to prevent random booting of the descriptor. Thanks go
 * out to Rustry for his suggestions. -Orion
 */
bool write_to_descriptor_old(int desc, const char *txt, int length)
{
  int                     iStart = 0;
  int                     nWrite = 0;
  int                     nBlock = 0;
  int                     iErr = 0;

  if(length <= 0)
    length = strlen(txt);
  for(iStart = 0; iStart < length; iStart += nWrite)
  {
    nBlock = UMIN(length - iStart, 4096);
    nWrite = send(desc, txt + iStart, nBlock, 0);
    if(nWrite == -1)
    {
      iErr = errno;
      if(iErr == EWOULDBLOCK)
      {
        nWrite = 0;
        continue;
      }
      else
      {
        perror("Write_to_descriptor");
        return FALSE;
      }
    }
  }
  return TRUE;
}

/*
 * text.
 * Updated to run with the block checks by Orion... if it doesn't work, blame
 * him.;P -Orion
 */
/*
 * This is the MCCP version. Use write_to_descriptor_old to send non-compressed
 * text.
 * Updated to run with the block checks by Orion... if it doesn't work, blame
 * him.;P -Orion
 */
bool write_to_descriptor( DESCRIPTOR_DATA * d, const char *txt, int length )
{
   int iStart = 0, nWrite = 0, nBlock, iErr, len;

   if( length <= 0 )
      length = strlen( txt );

   if( d && d->mccp->out_compress )
   {
      size_t mccpsaved = length;

      /*
       * Won't send more then it has to so make sure we check if its under length 
       */
      if( mccpsaved > strlen( txt ) )
         mccpsaved = strlen( txt );

      d->mccp->out_compress->next_in = ( unsigned char * )txt;
      d->mccp->out_compress->avail_in = length;

      while( d->mccp->out_compress->avail_in )
      {
         d->mccp->out_compress->avail_out =
            COMPRESS_BUF_SIZE - ( d->mccp->out_compress->next_out - d->mccp->out_compress_buf );

         if( d->mccp->out_compress->avail_out )
         {
            int status = deflate( d->mccp->out_compress, Z_SYNC_FLUSH );

            if( status != Z_OK )
               return false;
         }

         len = d->mccp->out_compress->next_out - d->mccp->out_compress_buf;
         if( len > 0 )
         {
            for( iStart = 0; iStart < len; iStart += nWrite )
            {
               nBlock = UMIN( len - iStart, MSL );
               nWrite = send( d->descriptor, d->mccp->out_compress_buf + iStart, nBlock, 0 );

               if( nWrite > 0 )
               {
                  update_transfer( 3, nWrite );
                  mccpsaved -= nWrite;
               }

               if( nWrite == -1 )
               {
                  iErr = errno;
                  perror( "Write_to_descriptor" );
                  if( iErr == EWOULDBLOCK )
                  {
                     nWrite = 0;
                     continue;
                  }
                  else
                     return false;
               }

               if( !nWrite )
                  break;
            }

            if( !iStart )
               break;

            if( iStart < len )
               memmove( d->mccp->out_compress_buf, d->mccp->out_compress_buf + iStart, len - iStart );

            d->mccp->out_compress->next_out = d->mccp->out_compress_buf + len - iStart;
         }
      }
      d->outtime = current_time;
      if( mccpsaved > 0 )
         update_transfer( 4, mccpsaved );
      return true;
   }

   if( !write_to_descriptor_old( d->descriptor, txt, length ) )
      return false;
   if( d )
      d->outtime = current_time;
   return true;
}


bool is_reserved_name(char *name)
{
  RESERVE_DATA           *res;

  for(res = first_reserved; res; res = res->next)
    if((*res->name == '*' && !str_infix(res->name + 1, name)) || !str_cmp(res->name, name))
      return TRUE;
  return FALSE;
}

/*
 * Parse a name for acceptability.
 */
bool check_parse_name(char *name, bool newchar)
{
  CHAR_DATA              *vch;

  /*
   * Names checking should really only be done on new characters, otherwise
   * we could end up with people who can't access their characters.  Would
   * have also provided for that new area havoc mentioned below, while still
   * disallowing current area mobnames.  I personally think that if we can
   * have more than one mob with the same keyword, then may as well have
   * players too though, so I don't mind that removal.  -- Alty
   */

  if(is_reserved_name(name) && newchar)
    return FALSE;

  /*
   * Outdated stuff -- Alty
   */
/*     if(is_name(name, "all auto immortal self someone god supreme demigod dog guard cityguard cat cornholio spock hicaine hithoric death ass fuck shit piss crap quit"))
       return FALSE;*/

  /*
   * Length restrictions.
   */
  if(strlen(name) < 4 && str_cmp(name, "new") && str_cmp(name, "fya"))
    return FALSE;

  if(strlen(name) > 12)
    return FALSE;

  /*
   * Alphanumerics only.
   * Lock out IllIll twits.
   */
  {
    char                   *pc;
    bool                    fIll;

    fIll = TRUE;
    for(pc = name; *pc != '\0'; pc++)
    {
      if(!isalpha(*pc))
        return FALSE;
      if(LOWER(*pc) != 'i' && LOWER(*pc) != 'l')
        fIll = FALSE;
    }

    if(fIll)
      return FALSE;
  }

  /*
   * Illegal characters 
   */
  if(strstr(name, "..") || strstr(name, "/") || strstr(name, "\\"))
    return FALSE;

  /*
   * Mob names illegal for newbies now - Samson 7-24-00 
   */
  for(vch = first_char; vch; vch = vch->next)
  {
    if(IS_NPC(vch))
    {
      if(arg_cmp(vch->name, name) && newchar)
        return FALSE;
    }
  }

  return TRUE;
}

/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect(DESCRIPTOR_DATA *d, char *name, bool fConn)
{
  CHAR_DATA              *ch;

  for(ch = first_char; ch; ch = ch->next)
  {
    if(!IS_NPC(ch) && (!fConn || !ch->desc) && ch->pcdata->filename && !str_cmp(name, ch->pcdata->filename))
    {
      if(fConn && ch->switched)
      {
        write_to_buffer(d, "Already playing.\r\nName: ", 0);
        d->connected = CON_GET_NAME;
        if(d->character)
        {
          /*
           * clear descriptor pointer to get rid of bug message in log 
           */
          d->character->desc = NULL;
          free_char(d->character);
          d->character = NULL;
        }
        return BERR;
      }
      if(fConn == FALSE)
      {
        if(ch->pcdata->pwd)
          STRFREE(d->character->pcdata->pwd);
        d->character->pcdata->pwd = STRALLOC(ch->pcdata->pwd);
      }
      else
      {
        char                    buf[MSL];

        /*
         * clear descriptor pointer to get rid of bug message in log 
         */
        d->character->desc = NULL;
        free_char(d->character);
        d->character = ch;
        ch->desc = d;
        ch->timer = 0;
        if(IS_PUPPET(ch))
          REMOVE_BIT(ch->pcdata->flags, PCFLAG_PUPPET);
        send_to_char("Reconnecting.\r\n", ch);
        if(xIS_SET(ch->act, PLR_TUTORIAL))
          xREMOVE_BIT(ch->act, PLR_TUTORIAL);
        // Remove AFK flag from those logging in after linkdead! -Taon
        if(xSET_BIT(d->character->act, PLR_AFK))
        {
          send_to_char("You're no longer AFK!\r\n", ch);
          xREMOVE_BIT(d->character->act, PLR_AFK);
        }
        do_look(ch, (char *)"auto");
        act(AT_ACTION, "$n has reconnected.", ch, NULL, NULL, TO_CANSEE);
        snprintf(buf, MSL, "%s@%s reconnected.", ch->pcdata->filename, d->host);

        OBJ_DATA               *deed;

        for(deed = ch->first_carrying; deed; deed = deed->next_content)
        {
          if(deed->pIndexData->vnum == OBJ_VNUM_DEED && !str_cmp(deed->owner, ch->name))
            break;
        }

        if(ch->pcdata->ship > 0 && !deed)
        {
          ROOM_INDEX_DATA        *location = NULL;

          // Send them back to dock
          ch->pcdata->ship = 0;
          ch->pcdata->watervnum = 0;
          ch->pcdata->boatvnum = 0;
          ch->pcdata->direction = 0;
          location = get_room_index(ROOM_DOCK);
          char_to_room(ch, location);
        }

        log_string_plus(buf, LOG_COMM, (short)UMAX((int)sysdata.log_level, (int)ch->level));
        {
          struct tm              *tme;
          time_t                  now;
          char                    day[50];

          now = time(0);
          tme = localtime(&now);
          strftime(day, 50, "%a %b %d %H:%M:%S %Y", tme);
          snprintf(buf, MSL, "%20s %-24s has logged on.", ch->pcdata->filename, day);

          if(xIS_SET(ch->act, PLR_COMMUNICATION))
            send_to_char("!!SOUND(sound/player.wav)\r\n", ch);
             if ( ch->pcdata )
             ch->pcdata->onboard = 3;

          append_to_file(LAST_FILE, buf);
        }
        d->connected = CON_PLAYING;

        if(ch->pcdata && ch->pcdata->clan)
          update_roster(ch);
        if(ch->pcdata && ch->pcdata->city)
          update_rollcall(ch);
                check_auth_state( ch );                /* Link dead support -- Rantic */

      }
      return TRUE;
    }
  }

  return FALSE;
}

/*
 * Check if already playing.
 */
bool check_playing(DESCRIPTOR_DATA *d, char *name, bool kick)
{
  CHAR_DATA              *ch;
  char                    buf[MSL];

  DESCRIPTOR_DATA        *dold;
  int                     cstate;

  for(dold = first_descriptor; dold; dold = dold->next)
  {
    if(dold != d && (dold->character || dold->original) && !str_cmp(name, dold->original ? dold->original->pcdata->filename : dold->character->pcdata->filename))
    {
      cstate = dold->connected;
      ch = dold->original ? dold->original : dold->character;
      if(!ch->name || (cstate != CON_PLAYING && cstate != CON_EDITING && cstate != CON_DELETE && cstate != CON_ROLL_STATS))
      {
        write_to_buffer(d, "Already connected - try again.\r\n", 0);
        snprintf(buf, MSL, "%s already connected.", ch->pcdata->filename);
        log_string_plus(buf, LOG_COMM, sysdata.log_level);
        return BERR;
      }
      if(!kick)
        return TRUE;
      write_to_buffer(d, "Already playing... Kicking off old connection.\r\n", 0);
      write_to_buffer(dold, "Kicking off old connection... bye!\r\n", 0);
      close_socket(dold, FALSE);
      /*
       * clear descriptor pointer to get rid of bug message in log 
       */

      d->character->desc = NULL;
      free_char(d->character);
      d->character = ch;
      ch->desc = d;
      ch->timer = 0;
      if(ch->switched)
        do_return(ch->switched, (char *)"");
      ch->switched = NULL;
      if(IS_PUPPET(ch))
        REMOVE_BIT(ch->pcdata->flags, PCFLAG_PUPPET);
      if(xIS_SET(ch->act, PLR_TUTORIAL))
        xREMOVE_BIT(ch->act, PLR_TUTORIAL);
      send_to_char("Reconnecting.\r\n", ch);
      do_look(ch, (char *)"auto");
      act(AT_ACTION, "$n has reconnected, kicking off old link.", ch, NULL, NULL, TO_CANSEE);
      snprintf(buf, MSL, "%s@%s reconnected, kicking off old link.", ch->pcdata->filename, d->host);
      log_string_plus(buf, LOG_COMM, (short)UMAX((int)sysdata.log_level, (int)ch->level));
      d->connected = cstate;
      if(ch->pcdata && ch->pcdata->clan)
        update_roster(ch);
      return TRUE;
    }
  }

  return FALSE;
}

void stop_idling(CHAR_DATA *ch)
{
  ROOM_INDEX_DATA        *was_in_room;

  /*
   * if(!ch
   * ||   !ch->desc
   * ||    ch->desc->connected != CON_PLAYING
   * ||   !ch->was_in_room
   * ||    ch->in_room != get_room_index(ROOM_VNUM_LIMBO))
   * return;
   */

  if(!ch || !ch->desc || ch->desc->connected != CON_PLAYING || !IS_IDLE(ch))
    return;

  if(xIS_SET(ch->act, PLR_R64))
    ch->pcdata->recast = 0;

  ch->timer = 0;
  was_in_room = ch->was_in_room;
  char_from_room(ch);
  char_to_room(ch, was_in_room);
  ch->was_in_room = ch->in_room;
  /*
   * ch->was_in_room  = NULL;
   */
  REMOVE_BIT(ch->pcdata->flags, PCFLAG_IDLE);
  act(AT_ACTION, "$n has returned from the void.", ch, NULL, NULL, TO_ROOM);
  return;
}

/*
 * Function to strip off the "a" or "an" or "the" or "some" from an object's
 * short description for the purpose of using it in a sentence sent to
 * the owner of the object.  (Ie: an object with the short description
 * "a long dark blade" would return "long dark blade" for use in a sentence
 * like "Your long dark blade".  The object name isn't always appropriate
 * since it contains keywords that may not look proper.  -Thoric
 */
char                   *myobj(OBJ_DATA *obj)
{
  if(!str_prefix("a ", obj->short_descr))
    return obj->short_descr + 2;
  if(!str_prefix("an ", obj->short_descr))
    return obj->short_descr + 3;
  if(!str_prefix("the ", obj->short_descr))
    return obj->short_descr + 4;
  if(!str_prefix("some ", obj->short_descr))
    return obj->short_descr + 5;
  return obj->short_descr;
}

char                   *obj_short(OBJ_DATA *obj)
{
  static char             buf[MSL];

  if(obj->count > 1)
  {
    snprintf(buf, MSL, "%s (%d)", obj->short_descr, obj->count);
    return buf;
  }
  return obj->short_descr;
}

char                   *act_string(const char *format, CHAR_DATA *to, CHAR_DATA *ch, const void *arg1, const void *arg2, int flags)
{
  static const char      *const he_she[] = { "it", "he", "she" };
  static const char      *const him_her[] = { "it", "him", "her" };
  static const char      *const his_her[] = { "its", "his", "her" };
  static char             buf[MAX_STRING_LENGTH];
  char                    fname[MAX_INPUT_LENGTH];
  char                   *point = buf;
  char                    temp[MSL];
  const char             *str = format;
  const char             *i;
  bool                    should_upper = false;
  CHAR_DATA              *vch = (CHAR_DATA *)arg2;
  OBJ_DATA               *obj1 = (OBJ_DATA *)arg1;
  OBJ_DATA               *obj2 = (OBJ_DATA *)arg2;

  if(str[0] == '$')
    DONT_UPPER = false;
/*
  if ( obj1 ) {
     if ( obj1->name == NULL ){             
     bug( "Act: Object with no name set.  NULL name" );
     i = " <@@@> ";
     }
 }

  if ( obj2 ) {
     if	( obj2->name ==	NULL ){
     bug( "Act: Object with no name set.  NULL name" );
     i = " <@@@> ";
     }
 }
*/
  while(*str != '\0')
  {
    if(*str == '.' || *str == '?' || *str == '!')
      should_upper = true;
    else if(should_upper == true && !isspace(*str) && *str != '$')
      should_upper = false;

    if(*str != '$')
    {
      *point++ = *str++;
      continue;
    }
    ++str;
    if(!arg2 && *str >= 'A' && *str <= 'Z')
    {
      bug("Act: missing arg2 for code %c:", *str);
      bug("%s", format);
      i = " <@@@> ";
    }
    else
    {
      switch (*str)
      {
        default:
          bug("Act: bad code %c.", *str);
          i = " <@@@> ";
          break;

        case 'd':
          if(!arg2 || ((char *)arg2)[0] == '\0')
            i = "door";
          else
          {
            one_argument((char *)arg2, fname);
            i = fname;
          }
          break;

        case 'e':
          if(ch->sex > 2 || ch->sex < 0)
          {
            bug("act_string: player %s has sex set at %d!", ch->name, ch->sex);
            i = should_upper ? "It" : "it";
          }
          else
            i = should_upper ? !can_see(to, ch) ? "It" : capitalize(he_she[URANGE(0, ch->sex, 2)]) : !can_see(to, ch) ? "it" : he_she[URANGE(0, ch->sex, 2)];
          break;

        case 'E':
          if(vch->sex > 2 || vch->sex < 0)
          {
            bug("act_string: player %s has sex set at %d!", vch->name, vch->sex);
            i = should_upper ? "It" : "it";
          }
          else
            i = should_upper ? !can_see(to, vch) ? "It" : capitalize(he_she[URANGE(0, vch->sex, 2)]) : !can_see(to, vch) ? "it" : he_she[URANGE(0, vch->sex, 2)];
          break;

          // add case I
        case 'i':
          if(!can_see(to, vch))
            i = "Someone";
          else
          {
            snprintf(temp, sizeof(temp), "%s", vch->name);
            i = temp;
          }
          break;
        case 'I':
          if(!can_see(to, vch))
            i = "Someone";
          else
          {
            snprintf(temp, sizeof(temp), "%s", (to ? PERS(vch, to) : (vch)->short_descr));
            i = temp;
          }
          break;

        case 'm':
          if(ch->sex > 2 || ch->sex < 0)
          {
            bug("act_string: player %s has sex set at %d!", ch->name, ch->sex);
            i = should_upper ? "It" : "it";
          }
          else
            i = should_upper ? !can_see(to, ch) ? "It" : capitalize(him_her[URANGE(0, ch->sex, 2)]) : !can_see(to, ch) ? "it" : him_her[URANGE(0, ch->sex, 2)];
          break;

        case 'M':
          if(vch->sex > 2 || vch->sex < 0)
          {
            bug("act_string: player %s has sex set at %d!", vch->name, vch->sex);
            i = should_upper ? "It" : "it";
          }
          else
            i = should_upper ? !can_see(to, vch) ? "It" : capitalize(him_her[URANGE(0, vch->sex, 2)]) : !can_see(to, vch) ? "it" : him_her[URANGE(0, vch->sex, 2)];
          break;

        case 'n':
          if(!can_see(to, ch))
            i = "Someone";
          else
          {
            snprintf(temp, sizeof(temp), "%s", (to ? PERS(ch, to) : (ch)->name));
            i = temp;
          }
          break;

        case 'N':
          if(!can_see(to, vch))
            i = "Someone";
          else
          {
            snprintf(temp, sizeof(temp), "%s", (to ? PERS(vch, to) : (vch)->name));
            i = temp;
          }
          break;

        case 'p':
          if(!to || can_see_obj(to, obj1))
          {
            /*
             * Prevents act programs from triggering off note shorts 
             */
            if((!to || IS_NPC(to)) && (obj1->item_type == ITEM_PAPER))
              i = obj1->pIndexData->short_descr;
            else
              i = obj_short(obj1);
          }
          else
            i = "something";
          break;

        case 'P':
          if(!to || can_see_obj(to, obj2))
          {
            /*
             * Prevents act programs from triggering off note shorts 
             */
            if((!to || IS_NPC(to)) && (obj2->item_type == ITEM_PAPER))
              i = obj2->pIndexData->short_descr;
            else
              i = obj_short(obj2);
          }
          else
            i = "something";
          break;

        case 'q':
          i = (to == ch) ? "" : "s";
          break;
        case 'Q':
          i = (to == ch) ? "your" : his_her[URANGE(0, ch->sex, 2)];
          break;

        case 's':
          if(ch->sex > 2 || ch->sex < 0)
          {
            bug("act_string: player %s has sex set at %d!", ch->name, ch->sex);
            i = should_upper ? "It" : "it";
          }
          else
            i = should_upper ? !can_see(to, ch) ? "It" : capitalize(his_her[URANGE(0, ch->sex, 2)]) : !can_see(to, ch) ? "it" : his_her[URANGE(0, ch->sex, 2)];
          break;

        case 'S':
          if(vch->sex > 2 || vch->sex < 0)
          {
            bug("act_string: player %s has sex set at %d!", vch->name, vch->sex);
            i = should_upper ? "It" : "it";
          }
          else
            i = should_upper ? !can_see(to, vch) ? "It" : capitalize(his_her[URANGE(0, vch->sex, 2)]) : !can_see(to, vch) ? "it" : his_her[URANGE(0, vch->sex, 2)];
          break;

        case 't':
          if(arg1)
            i = (char *)arg1;
          else
          {
            bug("Act: Bad variable $t");
            i = " <@@@> ";
          }
          break;

        case 'T':
          if(arg2)
            i = (char *)arg2;
          else
          {
            bug("Act: Bad variable $T");
            i = " <@@@> ";
          }
          break;
      }
    }
    ++str;
    while((*point = *i) != '\0')
      ++point, ++i;
  }
  mudstrlcpy(point, "\r\n", MSL);
  if(!DONT_UPPER)
    buf[0] = UPPER(buf[0]);

  return buf;
}

#undef NAME

void act(short AType, const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type)
{
  char                   *txt;
  CHAR_DATA              *to;
  CHAR_DATA              *vch = (CHAR_DATA *)arg2;
  CHAR_DATA              *third = (CHAR_DATA *)arg1;

  /*
   * Discard null and zero-length messages.
   */
  if(!format || format[0] == '\0')
    return;

  if(!ch)
  {
    bug("Act: null ch. (%s)", format);
    return;
  }
  if(!ch->in_room)
    to = NULL;
  else if(type == TO_CHAR)
    to = ch;
  else if(type == TO_THIRD)
    to = third;
  else
    to = ch->in_room->first_person;

  /*
   * ACT_SECRETIVE handling
   */
  if(IS_NPC(ch) && xIS_SET(ch->act, ACT_SECRETIVE) && type != TO_CHAR)
    return;

  if(type == TO_VICT)
  {
    if(!vch)
    {
      bug("Act: null vch with TO_VICT.");
      bug("%s (%s)", ch->name, format);
      return;
    }

    if(!vch->in_room)
    {
      bug("%s -> %s (%s)", ch->name, vch->name, format);
      return;
    }
    to = vch;
  }

  /*
   * Anyone feel like telling me the point of looping through the whole
   * room when we're only sending to one char anyways..? -- Alty 
   */
  for(; to; to = (type == TO_CHAR || type == TO_VICT) ? NULL : (type == TO_THIRD) ? to->next : to->next_in_room)
  {
    if(!to)
    {
      bug("%s act: null to comm.c 2989", __FUNCTION__);
      return;
    }

    if((!to->desc && (IS_NPC(to) && !HAS_PROG(to->pIndexData, ACT_PROG))) || !IS_AWAKE(to))
      continue;

    if(type == TO_CHAR && to != ch)
      continue;
    if(type == TO_THIRD && (to != third || IS_NPC(to)))
      continue;
    if(type == TO_VICT && (to != vch || to == ch))
      continue;
    if(type == TO_ROOM && to == ch)
      continue;
    if(type == TO_NOTVICT && (to == ch || to == vch))
      continue;
    if(type == TO_CANSEE && (to == ch || (!IS_IMMORTAL(to) && !IS_NPC(ch) && (xIS_SET(ch->act, PLR_WIZINVIS) && (get_trust(to) < (ch->pcdata ? ch->pcdata->wizinvis : 0))))))
      continue;
    if(IS_IMMORTAL(to))
      txt = act_string(format, to, ch, arg1, arg2, STRING_IMM);
    else
      txt = act_string(format, to, ch, arg1, arg2, STRING_NONE);

/*  Puppet code - Volk  */
    if(IS_PUPPET(to) && to->redirect)
      ch_printf(to->redirect, "[%s] %s", to->name, txt);

    if(to->desc)
    {

      if(AType == AT_COLORIZE)
      {
        if(txt[0] == '&')
          send_to_char_color(txt, to);
        else
        {
          set_char_color(AT_MAGIC, to);
          send_to_char_color(txt, to);
        }
      }
      else
      {
        set_char_color(AType, to);
        send_to_char_color(txt, to);
      }

    }
    if(MOBtrigger)
    {
      /*
       * Note: use original string, not string with ANSI. -- Alty 
       */
      mprog_act_trigger(txt, to, ch, (OBJ_DATA *)arg1, (void *)arg2);
    }
  }
  MOBtrigger = TRUE;
  return;
}

/*
 * Edit here to change the default prompt. -Orion
 */
char                   *default_prompt(CHAR_DATA *ch)
{
  static char             buf[MSL];

  if(IS_BLIND(ch))
    return (char *)"%h";

  snprintf(buf, MSL, "&w%s&z::", !IS_NPC(ch) ? ch->name : ch->short_descr);
  mudstrlcat(buf, "&W[&Y%h&W/&Y%H&Chp ", MSL);

  if(IS_BLOODCLASS(ch))
    mudstrlcat(buf, "&R", MSL);
  else
    mudstrlcat(buf, "&B", MSL);
  mudstrlcat(buf, "%m&W/", MSL);
  if(IS_BLOODCLASS(ch))
    mudstrlcat(buf, "&R", MSL);
  else
    mudstrlcat(buf, "&B", MSL);
  mudstrlcat(buf, "%M&C", MSL);

  if(IS_BLOODCLASS(ch))
    mudstrlcat(buf, "bp", MSL);
  else
    mudstrlcat(buf, "m", MSL);

  mudstrlcat(buf, " &G%v&W/&G%V&Cmv&W] ", MSL);

  if(!IS_NPC(ch) && IS_SET(ch->pcdata->flags, PCFLAG_BUILDWALK))
    mudstrlcat(buf, "&C[&GBUILDWALK&C]&c", MSL);

  if(!IS_NPC(ch) && !IS_IMMORTAL(ch))
  {
    mudstrlcat(buf, "&W[", MSL);
    if(IS_THIRDCLASS(ch)) // Volk: Three classes!
      mudstrlcat(buf, "&P%d&W/&P%e&W/&P%f", MSL);
    else if(IS_SECONDCLASS(ch))
      mudstrlcat(buf, "&P%d&W/&P%e", MSL);
    else
      mudstrlcat(buf, "&P%d", MSL);
    mudstrlcat(buf, "&W]&pLP &w", MSL);
  }
  return buf;
}

/*
 * Edit here to change the default fprompt. -Orion
 */
char                   *default_fprompt(CHAR_DATA *ch)
{
  static char             buf[MSL];

  if(IS_BLIND(ch))
    return (char *)"%h";

  snprintf(buf, MSL, "&w%s&z::", !IS_NPC(ch) ? ch->name : ch->short_descr);
  mudstrlcat(buf, "&W[&Y%h&W/&Y%H&Chp ", MSL);
  if(IS_BLOODCLASS(ch))
    mudstrlcat(buf, "&R", MSL);
  else
    mudstrlcat(buf, "&B", MSL);
  mudstrlcat(buf, "%m&G&W/", MSL);
  if(IS_BLOODCLASS(ch))
    mudstrlcat(buf, "&R", MSL);
  else
    mudstrlcat(buf, "&B", MSL);
  mudstrlcat(buf, "%M&C", MSL);

  if(IS_BLOODCLASS(ch))
    mudstrlcat(buf, "bp", MSL);
  else
    mudstrlcat(buf, "m", MSL);

  mudstrlcat(buf, " &G%v&W/&G%V&Cmv&W]", MSL);

  if(!IS_NPC(ch) && !IS_IMMORTAL(ch))
  {
    mudstrlcat(buf, "&W[", MSL);
    if(IS_THIRDCLASS(ch)) // Volk: Three classes!
      mudstrlcat(buf, "&P%d&W/&P%e&W/&P%f", MSL);
    else if(IS_SECONDCLASS(ch))
      mudstrlcat(buf, "&P%d&W/&P%e", MSL);
    else
      mudstrlcat(buf, "&P%d", MSL);
    mudstrlcat(buf, "&W]&pLP &w", MSL);
  }
  if(!IS_NPC(ch))
  {
    if(ch->fighting != NULL && (IS_GROUPED(ch)))
    {
      mudstrlcat(buf, "&C[ &RMHL %y", MSL);
      mudstrlcat(buf, "&C]&D", MSL);
    }
    mudstrlcat(buf, "%l %C %c&D", MSL);
  }

  return buf;
}

int getcolor(char clr)
{
  static const char       colors[17] = "xrgObpcwzRGYBPCW";
  int                     r;

  for(r = 0; r < 16; r++)
    if(clr == colors[r])
      return r;
  return -1;
}

// Volk: Money! :P  Put in support for multiclass prompts and percentage prompts.
void display_prompt(DESCRIPTOR_DATA *d)
{
  CHAR_DATA              *ch = d->character;
  CHAR_DATA              *och = (d->original ? d->original : d->character);
  CHAR_DATA              *victim;
  bool                    ansi = (!IS_NPC(och) && xIS_SET(och->act, PLR_ANSI));
  const char             *prompt;
  char                    buf[MSL];
  char                   *pbuf = buf;
  unsigned int            stat;
  double                  pcheck;
  int                     percent;

  if(!ch)
  {
    bug("display_prompt: NULL ch");
    return;
  }

  if(!IS_NPC(ch) && ch->substate != SUB_NONE && ch->pcdata->subprompt && ch->pcdata->subprompt[0] != '\0')
    prompt = ch->pcdata->subprompt;
  else if(ch->fighting)
  {
    if(!ch->pcdata || !ch->pcdata->fprompt || !*ch->pcdata->fprompt)
      prompt = default_fprompt(ch);
    else
      prompt = ch->pcdata->fprompt;
  }
  else if(IS_NPC(ch) || !ch->pcdata->prompt || !*ch->pcdata->prompt)
    prompt = default_prompt(ch);
  else
    prompt = ch->pcdata->prompt;

  /*
   * reset MXP to default operation 
   */
  if(d->mxp)
  {
    strcpy(pbuf, ESC "[3z");
    pbuf += 4;
  }

  if(ansi)
  {
    mudstrlcpy(pbuf, ANSI_RESET, MSL);
    d->prevcolor = 0x08;
    pbuf += 4;
  }
  for(; *prompt; prompt++)
  {
    /*
     * '%' = prompt commands
     * Note: foreground changes will revert background to 0 (black)
     */
    if(*prompt != '%')
    {
      *(pbuf++) = *prompt;
      continue;
    }
    ++prompt;
    if(!*prompt)
      break;
    if(*prompt == *(prompt - 1))
    {
      *(pbuf++) = *prompt;
      continue;
    }

    switch (*(prompt - 1))
    {
      default:
        bug("Display_prompt: bad command char '%c'.", *(prompt - 1));
        break;
      case '%':
        *pbuf = '\0';
        stat = 0x80000000;
        switch (*prompt)
        {
          case '%':
            *pbuf++ = '%';
            *pbuf = '\0';
            break;
          case 'a':
            if(ch->level >= 10)
              stat = ch->alignment;
            else if(IS_GOOD(ch))
              mudstrlcpy(pbuf, "good", MSL);
            else if(IS_EVIL(ch))
              mudstrlcpy(pbuf, "evil", MSL);
            else
              mudstrlcpy(pbuf, "neutral", MSL);
            break;
          case 'A':
            if(xIS_SET(ch->act, PLR_AFK))
            {
              char                    afkbuf[MSL];

              mudstrlcpy(afkbuf, "AFK", MSL);
              mudstrlcat(pbuf, afkbuf, MSL);
              pbuf += strlen(afkbuf);
            }
            break;

          case 'C':  /* Tank */
            if(!ch->fighting || (victim = ch->fighting->who) == NULL)
              mudstrlcpy(pbuf, "N/A", MAX_STRING_LENGTH);
            else if(!victim->fighting || (victim = victim->fighting->who) == NULL)
              mudstrlcpy(pbuf, "N/A", MAX_STRING_LENGTH);
            else
            {
              if(victim->max_hit > 0)
                percent = (100 * victim->hit) / victim->max_hit;
              else
                percent = -1;
              if(percent >= 100)
                sprintf(pbuf, "&g<&cT:&w%s &cTC: &gPerfect Health", !IS_NPC(victim) ? victim->name : victim->short_descr);
              else if(percent >= 90)
                sprintf(pbuf, "&g<&cT:&w%s &cTC: &OSlightly Scratched", !IS_NPC(victim) ? victim->name : victim->short_descr);
              else if(percent >= 80)
                sprintf(pbuf, "&g<&cT:&w%s &cTC: &YFew Bruises", !IS_NPC(victim) ? victim->name : victim->short_descr);
              else if(percent >= 70)
                sprintf(pbuf, "&g<&cT:&w%s &cTC: &YSome Cuts", !IS_NPC(victim) ? victim->name : victim->short_descr);
              else if(percent >= 60)
                sprintf(pbuf, "&g<&cT:&w%s &cTC: &PSeveral Wounds", !IS_NPC(victim) ? victim->name : victim->short_descr);
              else if(percent >= 50)
                sprintf(pbuf, "&g<&cT:&w%s &cTC: &pNasty Wounds", !IS_NPC(victim) ? victim->name : victim->short_descr);
              else if(percent >= 40)
                sprintf(pbuf, "&g<&cT:&w%s &cTC: &pBleeding Freely", !IS_NPC(victim) ? victim->name : victim->short_descr);
              else if(percent >= 30)
                sprintf(pbuf, "&g<&cT:&w%s &cTC: &rCovered in Blood", !IS_NPC(victim) ? victim->name : victim->short_descr);
              else if(percent >= 20)
                sprintf(pbuf, "&g<&cT:&w%s &cTC: &rLeaking Guts", !IS_NPC(victim) ? victim->name : victim->short_descr);
              else if(percent >= 10)
                sprintf(pbuf, "&g<&cT:&w%s &cTC: &RAlmost Dead", !IS_NPC(victim) ? victim->name : victim->short_descr);
              else
                sprintf(pbuf, "&g<&cT:&w%s &cTC: &RDYING", !IS_NPC(victim) ? victim->name : victim->short_descr);
            }
            break;
          case 'c':
            if(!ch->fighting || (victim = ch->fighting->who) == NULL)
              mudstrlcpy(pbuf, "N/A", MAX_STRING_LENGTH);
            else
            {
              if(victim->max_hit > 0)
                percent = (100 * victim->hit) / victim->max_hit;
              else
                percent = -1;
              if(!IS_NPC(ch))
              {
                if(percent >= 100)
                  sprintf(pbuf, "&cE: &w%s &cEC: &gPerfect Health&g>", !IS_NPC(victim) ? victim->name : victim->short_descr);
                else if(percent >= 90)
                  sprintf(pbuf, "&cE: &w%s &cEC: &OSlightly Scratched&g>", !IS_NPC(victim) ? victim->name : victim->short_descr);
                else if(percent >= 80)
                  sprintf(pbuf, "&cE: &w%s &cEC: &YFew Bruises&g>", !IS_NPC(victim) ? victim->name : victim->short_descr);
                else if(percent >= 70)
                  sprintf(pbuf, "&cE: &w%s &cEC: &YSome Cuts&g>", !IS_NPC(victim) ? victim->name : victim->short_descr);
                else if(percent >= 60)
                  sprintf(pbuf, "&cE: &w%s &cEC: &PSeveral Wounds&g>", !IS_NPC(victim) ? victim->name : victim->short_descr);
                else if(percent >= 50)
                  sprintf(pbuf, "&cE: &w%s &cEC: &pNasty Wounds&g>", !IS_NPC(victim) ? victim->name : victim->short_descr);
                else if(percent >= 40)
                  sprintf(pbuf, "&cE: &w%s &cEC: &pBleeding Freely&g>", !IS_NPC(victim) ? victim->name : victim->short_descr);
                else if(percent >= 30)
                  sprintf(pbuf, "&cE: &w%s &cEC: &rCovered in Blood&g>", !IS_NPC(victim) ? victim->name : victim->short_descr);
                else if(percent >= 20)
                  sprintf(pbuf, "&cE: &w%s &cEC: &rLeaking Guts&g>", !IS_NPC(victim) ? victim->name : victim->short_descr);
                else if(percent >= 10)
                  sprintf(pbuf, "&cE: &w%s &cEC: &RAlmost Dead&g>", !IS_NPC(victim) ? victim->name : victim->short_descr);
                else
                  sprintf(pbuf, "&cE: &w%s &cEC: &RDYING&g>", !IS_NPC(victim) ? victim->name : victim->short_descr);
              }
              else
              {
                if(percent >= 100)
                  sprintf(pbuf, "&cE: &w%s &cEC: &gPerfect Health&g>", victim->short_descr);
                else if(percent >= 90)
                  sprintf(pbuf, "&cE: &w%s &cEC: &OSlightly Scratched&g>", victim->short_descr);
                else if(percent >= 80)
                  sprintf(pbuf, "&cE: &w%s &cEC: &YFew Bruises&g>", victim->short_descr);
                else if(percent >= 70)
                  sprintf(pbuf, "&cE: &w%s &cEC: &YSome Cuts&g>", victim->short_descr);
                else if(percent >= 60)
                  sprintf(pbuf, "&cE: &w%s &cEC: &PSeveral Wounds&g>", victim->short_descr);
                else if(percent >= 50)
                  sprintf(pbuf, "&cE: &w%s &cEC: &pNasty Wounds&g>", victim->short_descr);
                else if(percent >= 40)
                  sprintf(pbuf, "&cE: &w%s &cEC: &pBleeding Freely&g>", victim->short_descr);
                else if(percent >= 30)
                  sprintf(pbuf, "&cE: &w%s &cEC: &rCovered in Blood&g>", victim->short_descr);
                else if(percent >= 20)
                  sprintf(pbuf, "&cE: &w%s &cEC: &rLeaking Guts&g>", victim->short_descr);
                else if(percent >= 10)
                  sprintf(pbuf, "&cE: &w%s &cEC: &RAlmost Dead&g>", victim->short_descr);
                else
                  sprintf(pbuf, "&cE: &w%s &cEC: &RDYING&g>", victim->short_descr);
              }

            }
            break;
            /*
             * Volk: d/e/f are percentage to next level for that class. 
             */
          case 'd':
            if(IS_SECONDCLASS(ch))
            {
              pcheck = ch->firstexp;
              pcheck /= exp_class_level(ch, ch->firstlevel + 1, ch->Class);
              pcheck *= 100;
            }
            else
            {
              pcheck = ch->exp;
              pcheck /= exp_level(ch, ch->level + 1);
              pcheck *= 100;
            }
            stat = (int)pcheck;
            break;

          case 'e':
            if(IS_SECONDCLASS(ch))
            {
              pcheck = ch->secondexp;
              pcheck /= exp_class_level(ch, ch->secondlevel + 1, ch->secondclass);
              pcheck *= 100;
              stat = (int)pcheck;
            }
            break;

          case 'f':
            if(IS_THIRDCLASS(ch))
            {
              pcheck = ch->thirdexp;
              pcheck /= exp_class_level(ch, ch->thirdlevel + 1, ch->thirdclass);
              pcheck *= 100;
              stat = (int)pcheck;
            }
            break;

          case 'F':
            if(IS_IMMORTAL(och))
              snprintf(pbuf, MSL, "%s", flag_string(ch->in_room->room_flags, r_flags));
            break;
          case 'g':
            stat = GET_MONEY(ch, DEFAULT_CURR);
            break;
          case 'G':
            break;
          case 'h':
            stat = ch->hit;
            break;
          case 'H':
            stat = ch->max_hit;
            break;
          case 'i':
            if((!IS_NPC(ch) && xIS_SET(ch->act, PLR_WIZINVIS)) || (IS_NPC(ch) && xIS_SET(ch->act, ACT_MOBINVIS)))
              snprintf(pbuf, MSL, "(Invis %d) ", (IS_NPC(ch) ? ch->mobinvis : ch->pcdata->wizinvis));
            else if(IS_AFFECTED(ch, AFF_INVISIBLE))
              mudstrlcpy(pbuf, "(Invis) ", MSL);
            break;
          case 'I':
            stat = (IS_NPC(ch) ? (xIS_SET(ch->act, ACT_MOBINVIS) ? ch->mobinvis : 0) : (xIS_SET(ch->act, PLR_WIZINVIS) ? ch->pcdata->wizinvis : 0));
            break;
          case 'l':
            mudstrlcpy(pbuf, "\r\n", MAX_STRING_LENGTH);
            break;
          case 'm':
            if(IS_BLOODCLASS(ch))
              stat = ch->blood;
            else
              stat = ch->mana;
            break;
          case 'M':
            if(IS_BLOODCLASS(ch))
              stat = ch->max_blood;
            else
              stat = ch->max_mana;
            break;
          case 'n':
            if(!ch->fighting || (victim = ch->fighting->who) == NULL)
              mudstrlcpy(pbuf, "N/A", MSL);
            else
            {
              if(ch == victim)
                mudstrlcpy(pbuf, "You", MSL);
              else if(IS_NPC(victim))
                mudstrlcpy(pbuf, victim->short_descr, MSL);
              else
                mudstrlcpy(pbuf, victim->name, MSL);
              pbuf[0] = UPPER(pbuf[0]);
            }
            break;
          case 'N':  /* Tank */
            if(!ch->fighting || (victim = ch->fighting->who) == NULL)
              mudstrlcpy(pbuf, "N/A", MSL);
            else if(!victim->fighting || (victim = victim->fighting->who) == NULL)
              mudstrlcpy(pbuf, "N/A", MSL);
            else
            {
              if(ch == victim)
                mudstrlcpy(pbuf, "You", MSL);
              else if(IS_NPC(victim))
                mudstrlcpy(pbuf, victim->short_descr, MSL);
              else
                mudstrlcpy(pbuf, victim->name, MSL);
              pbuf[0] = UPPER(pbuf[0]);
            }
            break;
          case 'o':  /* display name of object on * * * * auction */
            if(auction->item)
              mudstrlcpy(pbuf, auction->item->name, MSL);
            break;
          case 'O':  /* display cost of object on * * * * auction */
            if(auction->item)
              stat = auction->bet;
            break;
          case 'q':
            if(!IS_NPC(ch) && ch->Class == CLASS_MONK)
            {
              stat = ch->pcdata->monkblood;
            }
            break;
          case 'Q':
            if(IS_NPC(ch))
              break;

          case 'r':
            if(IS_IMMORTAL(och))
              stat = ch->in_room->vnum;
            break;
          case 's':
            snprintf(pbuf, MSL, "%s%s%s", IS_AFFECTED(ch, AFF_INVISIBLE) ? "I" : "", IS_AFFECTED(ch, AFF_HIDE) ? "H" : "", IS_AFFECTED(ch, AFF_SNEAK) ? "S" : "");
            break;
          case 'S':
            if(ch->style == STYLE_BERSERK)
              mudstrlcpy(pbuf, "B", MSL);
            else if(ch->style == STYLE_AGGRESSIVE)
              mudstrlcpy(pbuf, "A", MSL);
            else if(ch->style == STYLE_DEFENSIVE)
              mudstrlcpy(pbuf, "D", MSL);
            else if(ch->style == STYLE_EVASIVE)
              mudstrlcpy(pbuf, "E", MSL);
            else
              mudstrlcpy(pbuf, "S", MSL);
            break;
          case 'T':
            if(time_info.hour < 5)
              mudstrlcpy(pbuf, "night", MSL);
            else if(time_info.hour < 6)
              mudstrlcpy(pbuf, "dawn", MSL);
            else if(time_info.hour < 19)
              mudstrlcpy(pbuf, "day", MSL);
            else if(time_info.hour < 21)
              mudstrlcpy(pbuf, "dusk", MSL);
            else
              mudstrlcpy(pbuf, "night", MSL);
            break;
          case 'u':
            if(IS_IMMORTAL(och))
              stat = num_descriptors;
            break;
          case 'U':
            if(IS_IMMORTAL(och))
              stat = sysdata.maxplayers;
            break;
          case 'v':
            stat = ch->move;
            break;
          case 'V':
            stat = ch->max_move;
            break;
          case 'x':
            stat = ch->exp;
            break;
          case 'X':
            stat = exp_level(ch, ch->level + 1) - ch->exp;
            break;
          case 'y':
            stat = ch->hate_level;
            break;
            /*
             * Allow them to use more then one line for prompt 
             */
          case 'z':
            mudstrlcpy(pbuf, "\r\n", MSL);
            break;
        }
        if(stat != 0x80000000)
          snprintf(pbuf, MSL, "%d", stat);
        pbuf += strlen(pbuf);
        break;
    }
  }
  *pbuf = '\0';
  send_to_char(buf, ch);
  return;
}

int make_color_sequence(const char *col, char *buf, DESCRIPTOR_DATA *d)
{
  int                     ln;
  const char             *ctype = col;
  unsigned char           cl;
  CHAR_DATA              *och;
  bool                    ansi;

  och = (d->original ? d->original : d->character);
  ansi = (!IS_NPC(och) && xIS_SET(och->act, PLR_ANSI));
  col++;
  if(!*col)
    ln = -1;
  else if(*ctype != '&' && *ctype != '^')
  {
    bug("Make_color_sequence: command '%c' not '&' or '^'.", *ctype);
    ln = -1;
  }
  else if(*col == *ctype)
  {
    buf[0] = *col;
    buf[1] = '\0';
    ln = 1;
  }
  else if(!ansi)
    ln = 0;
  else
  {
    cl = d->prevcolor;
    switch (*ctype)
    {
      default:
        bug("Make_color_sequence: bad command char '%c'.", *ctype);
        ln = -1;
        break;
      case '&':
        if(*col == '-')
        {
          buf[0] = '~';
          buf[1] = '\0';
          ln = 1;
          break;
        }
      case '^':
      {
        int                     newcol;

        if((newcol = getcolor(*col)) < 0)
        {
          ln = 0;
          break;
        }
        else if(*ctype == '&')
          cl = (cl & 0xF0) | newcol;
        else
          cl = (cl & 0x0F) | (newcol << 4);
      }
        if(cl == d->prevcolor)
        {
          ln = 0;
          break;
        }

        strcpy(buf, "\033[");
        if((cl & 0x88) != (d->prevcolor & 0x88))
        {
          if(cl == 0x07)
          {
            strcpy(buf, "\033[0;37"); // buf out of bounds?
          }
          else
          {
            if((cl & 0x08))
              strcat(buf, "1;");
            else
              strcat(buf, "0;");
            if((cl & 0x80))
              strcat(buf, "5;");
          }
          d->prevcolor = 0x07 | (cl & 0x88);
          ln = strlen(buf);
        }
        else
          ln = 2;
        if((cl & 0x07) != (d->prevcolor & 0x07))
        {
          snprintf(buf + ln, MSL - ln, "3%d;", cl & 0x07);
          ln += 3;
        }
        if((cl & 0x70) != (d->prevcolor & 0x70))
        {
          snprintf(buf + ln, MSL - ln, "4%d;", (cl & 0x70) >> 4);
          ln += 3;
        }
        if(buf[ln - 1] == ';')
          buf[ln - 1] = 'm';
        else
        {
          buf[ln++] = 'm';
          buf[ln] = '\0';
        }
        d->prevcolor = cl;
    }
  }
  if(ln <= 0)
    *buf = '\0';
  return ln;
}

void set_pager_input(DESCRIPTOR_DATA *d, char *argument)
{
  while(isspace(*argument))
    argument++;
  d->pagecmd = *argument;
  return;
}

bool pager_output(DESCRIPTOR_DATA *d)
{
  register char          *last;
  CHAR_DATA              *ch;
  int                     pclines;
  register int            lines;
  bool                    ret;

  if(!d || !d->pagepoint || d->pagecmd == -1)
    return TRUE;
  ch = d->original ? d->original : d->character;
  pclines = UMAX(ch->pcdata->pagerlen, 5) - 1;
  switch (LOWER(d->pagecmd))
  {
    default:
      lines = 0;
      break;
    case 'b':
      lines = -1 - (pclines * 2);
      break;
    case 'r':
      lines = -1 - pclines;
      break;
    case 'n':
      lines = 0;
      pclines = 0x7FFFFFFF; /* As many lines as possible */
      break;
    case 'q':
      d->pagetop = 0;
      d->pagepoint = NULL;
      flush_buffer(d, TRUE);
      DISPOSE(d->pagebuf);
      d->pagesize = MAX_STRING_LENGTH;
      return TRUE;
  }
  while(lines < 0 && --d->pagepoint >= d->pagebuf)
    if(*d->pagepoint == '\n')
      ++lines;
  if(d->pagepoint < d->pagebuf)
    d->pagepoint = d->pagebuf;
  if(*d->pagepoint == '\r' && *(++d->pagepoint) == '\n')
    ++d->pagepoint;
  for(lines = 0, last = d->pagepoint; lines < pclines; ++last)
    if(!*last)
      break;
    else if(*last == '\n')
      ++lines;
  if(*last == '\r')
    ++last;
  if(last != d->pagepoint)
  {
    if(!write_to_descriptor(d, d->pagepoint, (last - d->pagepoint)))
      return FALSE;
    d->pagepoint = last;
  }
  while(isspace(*last))
    ++last;
  if(!*last)
  {
    d->pagetop = 0;
    d->pagepoint = NULL;
    flush_buffer(d, TRUE);
    DISPOSE(d->pagebuf);
    d->pagesize = MAX_STRING_LENGTH;
    return TRUE;
  }
  d->pagecmd = -1;
  if(xIS_SET(ch->act, PLR_ANSI))
    if(write_to_descriptor(d, ANSI_LBLUE, 0) == FALSE)
      return FALSE;
  if((ret = write_to_descriptor(d, "(C)ontinue, (N)on-stop, (R)efresh, (B)ack, (Q)uit: [C] ", 0)) == FALSE)
    return FALSE;
  if(xIS_SET(ch->act, PLR_ANSI))
  {
    char                    buf[32];

    snprintf(buf, 32, "%s", color_str(d->pagecolor, ch));
    ret = write_to_descriptor(d, buf, 0);
  }
  return ret;
}

#ifdef WIN32

void                    shutdown_mud(char *reason);

void save_auth_list     args( ( void ) );


void bailout(void)
{
  echo_to_all(AT_IMMORT, "MUD shutting down by system operator NOW!!", ECHOTAR_ALL);
  shutdown_mud("MUD shutdown by system operator");
  log_string("MUD shutdown by system operator");
  Sleep(5000);  /* give "echo_to_all" time to display */
  mud_down = TRUE;  /* This will cause game_loop to exit */
  service_shut_down = TRUE; /* This will cause characters to be saved */
  fflush(stderr);
  return;
}

#endif

void do_speed(CHAR_DATA *ch, char *argument)
{
  short                   speed = atoi(argument);

  if(!ch->desc)
    return; // Don't send messages to people
  // who don't exist. duh.

  if(argument[0] == '\0')
  {
    ch_printf(ch, "Your present speed is a %d, which equates to %d bytes per second.\r\n", ch->desc->speed, client_speed(ch->desc->speed));
    return;
  }

  if(speed > 5 || speed < 0)
  {
    send_to_char("Speed is between 0 and 5.\r\n", ch);
    return;
  }
  ch->desc->speed = speed;
  ch_printf(ch, "The MUD will now send output to you at %d bytes per second.\r\n", client_speed(speed));
  if(client_speed(speed) > 2048)
    ch_printf(ch, "You should be aware %d is fast enough to lag you if you have a slow connection.\r\n", client_speed(speed));
  return;
}

short client_speed(short speed)
{
  switch (speed)
  {
    default:
      break;
    case 1:
      return 512;
    case 2:
      return 1024;
    case 3:
      return 2048;
    case 4:
      return 3584;
    case 5:
      return 5120;

  }
  return 2048;  // Better than a mere default
  // case.
}

/*  Volk was messing around with this one day - he wanted to create a
    command to create your 'command buffer', but as it's handled by
    waitstates in C and THEN next command processed (waiting in socket),
    there's no easy way to clear the commands. Will have a go!
    Credits to Ksilyan and Zeno for their help with this.  */

// Does 'buf' contain the single command 'cmd'?
// Defined as: \n or \r then whitespace
// then: cmd
// finally: whitespace then \n or \r
bool str_has_command(const char *buf, const char *cmd)
{
  int                     pos = 0;
  int                     state = 0;

  // States:
  // 0: newline (\n or \r) (on success --> 1)
  // 1: any whitespace (on success --> 3)
  // 2: whitespace until newline (on success --> success)
  // 3: cmd[0] (on success --> 4)
  // 4: cmd[1] (on success --> 5)
  // ...: ...
  // 3+n: cmd[n] (on success --> 2)

  char                    c;

  while((c = buf[pos++]) != '\0')
  {
    switch (state)
    {
        // find initial newline
      case 0:
        if(c == '\r' || c == '\n')
          state = 1;
        break;

        // munch whitespace
      case 1:
        if(isspace(c)); // munch
        else
        {
          state = 3;
          pos--;  // reconsider this character
        }
        break;

        // whitespace; stop at newline.
        // if we find a non-whitespace, go back to state 0
      case 2:
        if(!isspace(c))
        {
          state = 0;
        }
        if(c == '\r' || c == '\n')
          return TRUE;
        break;

        // at state 3, we need to find cmd[0]
        // at state 4, we need to find cmd[1]
        // etc.
      default:

        // is this not what we wanted? then go back to zero
        if(c != cmd[state - 3])
        {
          state = 0;
        }
        else
        {
          state++;

          // are we at the end of the cmd string?
          // if so, move on to final whitespace search.
          if(cmd[state - 3] == '\0')
          {
            state = 2;
          }
        }

        break;
    }
  }
  return FALSE;
}

/* source: EOD, by John Booth <???> */
void printf_to_char(CHAR_DATA *ch, const char *fmt, ...)
{
  char                    buf[MAX_STRING_LENGTH];
  va_list                 args;

  va_start(args, fmt);
  vsnprintf(buf, MSL, fmt, args);
  va_end(args);
  send_to_char(buf, ch);
}

bool can_use_path(CHAR_DATA *ch, const char *direct, const char *filename)
{
  struct stat             fst;
  char                    newfilename[1024];

  /*
   * Length restrictions 
   */
  if(!filename || filename[0] == '\0')
  {
    if(ch)
      send_to_char("Empty filename is not valid.\r\n", ch);
    return false;
  }
  if(strlen(filename) < 3)
  {
    if(ch)
      ch_printf(ch, "Filename (%s) isn't long enough.\r\n", filename);
    return false;
  }

  /*
   * Illegal characters 
   */
  if(strstr(filename, "..") || strstr(filename, "/") || strstr(filename, "\\"))
  {
    if(ch)
      send_to_char("A filename may not contain a '..', '/', or '\\' in it.\r\n", ch);
    return false;
  }

  /*
   * If that filename is already being used lets not allow it now to be on the safe
   * side 
   */
  snprintf(newfilename, sizeof(newfilename), "%s%s", direct, filename);
  if(stat(newfilename, &fst) != -1)
  {
    if(ch)
      ch_printf(ch, "The filename (%s) is already used.\r\n", filename);
    return false;
  }

  /*
   * If we got here assume its valid 
   */
  return true;
}
