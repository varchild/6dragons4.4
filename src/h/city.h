/***************************************************************************
 * - Chronicles Copyright 2001, 2002 by Brad Ensley (Orion Elder)          *
 * - SMAUG 1.4  Copyright 1994, 1995, 1996, 1998 by Derek Snider           *
 * - Merc  2.1  Copyright 1992, 1993 by Michael Chastain, Michael Quan,    *
 *   and Mitchell Tse.                                                     *
 * - DikuMud    Copyright 1990, 1991 by Sebastian Hammer, Michael Seifert, *
 *   Hans-Henrik Stærfeldt, Tom Madsen, and Katja Nyboe.                   *
 * - Win32 port by Nick Gammon                                             *
 ***************************************************************************
 * - Main MUD header                                                       *
 ***************************************************************************/

typedef struct city_data CITY_DATA;

typedef struct rollcall_data ROLLCALL_DATA;
void                    remove_rollcall(CITY_DATA * city, char *name);
void                    add_rollcall(CITY_DATA * city, char *name, int Class, int level, int kills, int deaths);
void                    update_rollcall(CHAR_DATA *ch);

void                    update_cities(void);

typedef struct siege_data SIEGE_DATA;
extern SIEGE_DATA      *first_siege;
extern SIEGE_DATA      *last_siege;

struct siege_data
{
  SIEGE_DATA             *next;
  SIEGE_DATA             *prev;
  CITY_DATA              *acity;  /* Attacking City */
  CITY_DATA              *dcity;  /* Defending City */
  time_t                  siege_started;  /* Time the siege started */
};

struct rollcall_data
{
  ROLLCALL_DATA          *next;
  ROLLCALL_DATA          *prev;
  char                   *name;
  time_t                  joined;
  time_t                  lastupdated;
  int                     Class;
  int                     level;
  int                     kills;
  int                     deaths;
};

struct city_data
{
  CITY_DATA              *next;
  CITY_DATA              *prev;
  ROLLCALL_DATA          *first_citizen;
  ROLLCALL_DATA          *last_citizen;
  char                   *filename;
  char                   *name;
  char                   *description;
  char                   *bank;
  char                   *duke;
  char                   *baron;
  char                   *captain;
  char                   *sheriff;
  char                   *knight;
  char                   *allegiance;
  char                   *attacker; /* Who is attacking the city? */
  char                   *attacker2;  /* Who else is attacking the city? */
  int                     defense;
  int                     offense;
  short                   ocatapult;
  short                   return_ocatapult;
  short                   dcatapult;
  short                   return_dcatapult;
  short                   ballista;
  short                   return_ballista;
  short                   osoldiers;
  short                   return_osoldiers;
  short                   oguards;
  short                   return_oguards;

  short                   dsoldiers;
  short                   return_dsoldiers;
  short                   dguards;
  short                   return_dguards;

  short                   oarchers;
  short                   return_oarchers;
  short                   darchers;
  short                   return_darchers;
  short                   owarriors;
  short                   return_owarriors;
  short                   dwarriors;
  short                   return_dwarriors;
  short                   arrows;
  short                   return_arrows;
  short                   platform;
  short                   return_platform;
  short                   ram;
  short                   return_ram;
  short                   pitch;
  short                   return_pitch;
  int                     wins;
  int                     loses;
  int                     recall;
  int                     siege1_room;  /* Vnums for city stuff */
  int                     siege2_room;
  int                     siege3_room;
  int                     defend1_room;
  int                     defend2_room;
  int                     defend3_room;
  int                     soldier_mob;
  int                     archer_mob;
  int                     guard_mob;
  int                     warrior_mob;
  int                     catapult_obj;
  int                     ballista_obj;
  int                     pitch_obj;
  int                     platform_obj;
  int                     ram_obj;
  int                     arrow_obj;
  int                     min_house;
  int                     max_house;
  int                     onlinechars;
  short                   hardened; // crafted catapult or ballista
};

/* Externals Rule */

extern CITY_DATA       *first_city;
extern CITY_DATA       *last_city;

#define CT      CITY_DATA

/* city.c */

CT                     *get_city args((const char *name));
void load_cities        args((void));
void save_city          args((CITY_DATA * city));

#define IS_CITY(ch) ( !IS_NPC( (ch) ) && (ch)->pcdata->city )
#define IS_DUKE(ch) ( IS_CITY( (ch) ) && VLD_STR( (ch)->pcdata->city->duke ) && !str_cmp( (ch)->pcdata->city->duke, (ch)->name ) )
#define IS_BARON(ch) ( IS_CITY( (ch) ) && VLD_STR( (ch)->pcdata->city->baron ) && !str_cmp( (ch)->pcdata->city->baron, (ch)->name ) )
#define IS_CAPTAIN(ch) ( IS_CITY( (ch) ) && VLD_STR( (ch)->pcdata->city->captain ) && !str_cmp( (ch)->pcdata->city->captain, (ch)->name ) )
#define IS_SHERIFF(ch) ( IS_CITY( (ch) ) && VLD_STR( (ch)->pcdata->city->sheriff ) && !str_cmp( (ch)->pcdata->city->sheriff, (ch)->name ) )
#define IS_KNIGHT(ch) ( IS_CITY( (ch) ) && VLD_STR( (ch)->pcdata->city->knight ) && !str_cmp( (ch)->pcdata->city->knight, (ch)->name ) )
#define IS_OFFICIAL(ch) ( IS_CITY( (ch) ) && ( IS_DUKE( (ch) ) || IS_BARON( (ch) ) || IS_CAPTAIN( (ch) ) || IS_SHERIFF( (ch) ) || IS_KNIGHT( (ch) ) ) )
