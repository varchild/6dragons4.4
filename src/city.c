 /*
  * CITY POLITICS CODE START - Vladaar 
  */
#include <string.h>

#include "h/mud.h"
#include "h/files.h"
#include "h/city.h"
#include "h/key.h"
#include <time.h>

CITY_DATA              *first_city;
CITY_DATA              *last_city;

SIEGE_DATA             *first_siege;
SIEGE_DATA             *last_siege;

void                    start_siege(SIEGE_DATA * siege, CITY_DATA * ocity, CITY_DATA * dcity);
void                    update_city_def_off(CITY_DATA * city);

bool                    valid_pfile(const char *filename);
void                    fix_city_order(CITY_DATA * city);
void                    fread_city(CITY_DATA * city, FILE * fp);
extern bool             load_city_file(const char *cityfile);
void                    write_city_list(void);
void                    end_siege(SIEGE_DATA * siege);
extern int              get_npc_class(char *Class);
bool                    in_hash_table(const char *str);

int                     count_attackers(SIEGE_DATA * siege);
int                     count_defenders(SIEGE_DATA * siege);

/* When removing objects if no one online for the defending or attacking city return that to the city */
void returntocity(OBJ_DATA *obj)
{
  SIEGE_DATA             *siege;
  CITY_DATA              *acity, *dcity;

  if(!obj || !obj->city || !obj->siege)
    return;

  siege = obj->siege;
  acity = siege->acity;
  dcity = siege->dcity;

  /*
   * The items are always in the defense city rooms 
   */

  if(obj->in_room->vnum == dcity->defend1_room)
  {
    if(dcity->onlinechars > 0)
      return;
    if(obj->pIndexData->vnum == dcity->catapult_obj)
      dcity->dcatapult++;

    if(obj->pIndexData->vnum == dcity->pitch_obj)
      dcity->pitch++;

    if(obj->pIndexData->vnum == dcity->arrow_obj)
      dcity->arrows++;
  }
  if(obj->in_room->vnum == dcity->defend2_room)
  {
    if(dcity->onlinechars > 0)
      return;
    if(obj->pIndexData->vnum == dcity->catapult_obj)
      dcity->dcatapult++;

    if(obj->pIndexData->vnum == dcity->pitch_obj)
      dcity->pitch++;

    if(obj->pIndexData->vnum == dcity->arrow_obj)
      dcity->arrows++;

  }
  if(obj->in_room->vnum == dcity->defend3_room)
  {
    if(dcity->onlinechars > 0)
      return;

    if(obj->pIndexData->vnum == dcity->catapult_obj)
      dcity->dcatapult++;

    if(obj->pIndexData->vnum == dcity->pitch_obj)
      dcity->pitch++;

    if(obj->pIndexData->vnum == dcity->arrow_obj)
      dcity->arrows++;

  }

// catapults ballista ram platform 
  if(obj->in_room->vnum == dcity->siege1_room)
  {
    if(acity->onlinechars > 0)
      return;
    if(obj->pIndexData->vnum == acity->catapult_obj)
      acity->ocatapult++;
    if(obj->pIndexData->vnum == acity->ballista_obj)
      acity->ballista++;
    if(obj->pIndexData->vnum == acity->ram_obj)
      acity->ram++;
    if(obj->pIndexData->vnum == acity->platform_obj)
      acity->platform++;

  }
  if(obj->in_room->vnum == dcity->siege2_room)
  {
    if(acity->onlinechars > 0)
      return;
    if(obj->pIndexData->vnum == acity->catapult_obj)
      acity->ocatapult++;
    if(obj->pIndexData->vnum == acity->ballista_obj)
      acity->ballista++;
    if(obj->pIndexData->vnum == acity->ram_obj)
      acity->ram++;
    if(obj->pIndexData->vnum == acity->platform_obj)
      acity->platform++;

  }
  if(obj->in_room->vnum == dcity->siege3_room)
  {
    if(acity->onlinechars > 0)
      return;
    if(obj->pIndexData->vnum == acity->catapult_obj)
      acity->ocatapult++;
    if(obj->pIndexData->vnum == acity->ballista_obj)
      acity->ballista++;
    if(obj->pIndexData->vnum == acity->ram_obj)
      acity->ram++;
    if(obj->pIndexData->vnum == acity->platform_obj)
      acity->platform++;

  }
}

void bug_all_city_info(void)
{
  CITY_DATA              *city;

  for(city = first_city; city; city = city->next)
  {
    bug("%s: Def(%d) Off(%d) Cat(%d)(%d) Bal(%d) Sol(%d) Gua(%d) Arc(%d)(%d) War(%d)(%d) Arr(%d) Pla(%d) Ram(%d) Pit(%d)", city->name, city->defense, city->offense, city->ocatapult, city->dcatapult,
        city->ballista, city->dguards, city->osoldiers, city->oarchers, city->darchers, city->owarriors, city->dwarriors, city->arrows, city->platform, city->ram, city->pitch);
  }
}

/* Update the cities onlinechars */
void update_cities(void)
{
  CITY_DATA              *city;
  CHAR_DATA              *wch;

  /*
   * Set to none 
   */
  for(city = first_city; city; city = city->next)
    city->onlinechars = 0;

  /*
   * Recount them all 
   */
  for(wch = first_char; wch; wch = wch->next)
  {
    if(IS_NPC(wch) || !wch || !IS_CITY(wch) || IS_IMMORTAL(wch))
      continue;
    city = get_city(wch->pcdata->city_name);
    if(city)
      city->onlinechars++;
  }
}

/* Is the city attacking or being attacked by anyone */
bool is_in_siege(CITY_DATA * city)
{
  SIEGE_DATA             *siege;

  for(siege = first_siege; siege; siege = siege->next)
  {
    if(siege->acity == city)
      return TRUE;
    if(siege->dcity == city)
      return TRUE;
  }

  return FALSE;
}

/* Is the attacker attacking the defender */
bool is_sieging(CITY_DATA * attacker, CITY_DATA * defender)
{
  SIEGE_DATA             *siege;

  for(siege = first_siege; siege; siege = siege->next)
  {
    if(siege->acity == attacker && siege->dcity == defender)
      return TRUE;
  }

  return FALSE;
}

SIEGE_DATA             *add_siege(CITY_DATA * attacker, CITY_DATA * defender)
{
  SIEGE_DATA             *siege;

  if(!attacker || !defender)
    return NULL;

  CREATE(siege, SIEGE_DATA, 1);
  if(!siege)
    return NULL;
  siege->acity = attacker;
  siege->dcity = defender;
  siege->siege_started = current_time;
  LINK(siege, first_siege, last_siege, next, prev);
  return siege;
}

void insert_rollcall(CITY_DATA * city, ROLLCALL_DATA * rollcall)
{
  ROLLCALL_DATA          *uroll;
  int                     daydiff, odaydiff;

  for(uroll = city->first_citizen; uroll; uroll = uroll->next)
  {
    odaydiff = (current_time - uroll->lastupdated) / 86400;
    daydiff = (current_time - rollcall->lastupdated) / 86400;

    if(odaydiff < 3 && uroll->level > rollcall->level)
      continue;
    /*
     * So if level is the same or they havent logged in for 3 days just insert here
     */
    INSERT(rollcall, uroll, city->first_citizen, next, prev);
    return;
  }
  LINK(rollcall, city->first_citizen, city->last_citizen, next, prev);
}

CITY_DATA              *get_city(const char *name)
{
  CITY_DATA              *city;

  if(!VLD_STR(name))
    return NULL;

  for(city = first_city; city; city = city->next)
  {
    if(!str_cmp(name, city->name))
      return city;
  }
  return NULL;
}

void proc_chan(CHAR_DATA *ch, char *argument)
{
  char                    bufx[MIL];
  DESCRIPTOR_DATA        *d;
  CITY_DATA              *city;

  city = get_city(ch->pcdata->city_name);
  if(!city)
    return;

  snprintf(bufx, MIL, "&GA town crier hurriedly reads off a new proclamation shouting Hear ye, Hear ye: %s&D\r\n", argument);
  for(d = first_descriptor; d; d = d->next)
  {
    if(d->connected != CON_PLAYING || !d->character || !d->character->pcdata)
      continue;
    if(!IS_IMMORTAL(d->character))
    {
      if(!VLD_STR(d->character->pcdata->city_name))
        continue;
      if(str_cmp(d->character->pcdata->city_name, city->name))
        continue;
    }
    send_to_char(bufx, d->character);
  }
}

void write_city_list(void)
{
  CITY_DATA              *tcity;
  FILE                   *fpout;
  char                    filename[256];

  snprintf(filename, 256, "%s%s", CITY_DIR, CITY_LIST);
  fpout = FileOpen(filename, "w");
  if(!fpout)
  {
    bug("FATAL: cannot open %s for writing!\r\n", filename);
    perror(filename);
    return;
  }
  for(tcity = first_city; tcity; tcity = tcity->next)
    fprintf(fpout, "%s\n", tcity->filename);
  fprintf(fpout, "$\n");
  FileClose(fpout);
  return;
}

void fwrite_citizenlist(FILE * fp, CITY_DATA * city)
{
  ROLLCALL_DATA          *rollcall;

  for(rollcall = city->first_citizen; rollcall; rollcall = rollcall->next)
  {
    if(!valid_pfile(rollcall->name))  /* Just don't save if not a valid * * * * * * * * * 
                                       * * * * pfile 
                                       */
      continue;

    fprintf(fp, "%s", "#ROLLCALL\n");
    fprintf(fp, "Name      %s~\n", rollcall->name);
    fprintf(fp, "Joined    %ld\n", (time_t) rollcall->joined);
    fprintf(fp, "Updated    %ld\n", (time_t) rollcall->lastupdated);
    fprintf(fp, "Class     %s~\n", npc_class[rollcall->Class]);
    fprintf(fp, "Level     %d\n", rollcall->level);
    fprintf(fp, "Kills     %d\n", rollcall->kills);
    fprintf(fp, "Deaths    %d\n", rollcall->deaths);
    fprintf(fp, "%s", "End\n\n");
  }
}

/* Save a city's data to its data file */
void save_city(CITY_DATA * city)
{
  FILE                   *fp;
  char                    filename[256];

  if(!city)
  {
    bug("%s: null city pointer!", __FUNCTION__);
    return;
  }
  if(!VLD_STR(city->filename))
  {
    bug("%s: %s has no filename", __FUNCTION__, city->name);
    return;
  }

  snprintf(filename, 256, "%s%s", CITY_DIR, city->filename);
  if(!(fp = FileOpen(filename, "w")))
  {
    bug("%s: can't open %s for writing.", __FUNCTION__, filename);
    perror(filename);
    return;
  }

  fprintf(fp, "#CITY\n");
  if(VLD_STR(city->name))
    fprintf(fp, "Name          %s~\n", city->name);
  if(VLD_STR(city->filename))
    fprintf(fp, "Filename      %s~\n", city->filename);
  if(VLD_STR(city->description))
    fprintf(fp, "Description   %s~\n", strip_cr(city->description));
  if(VLD_STR(city->bank))
    fprintf(fp, "Bank          %s~\n", city->bank);
  if(VLD_STR(city->duke))
    fprintf(fp, "Duke          %s~\n", city->duke);
  if(VLD_STR(city->baron))
    fprintf(fp, "Baron         %s~\n", city->baron);
  if(VLD_STR(city->captain))
    fprintf(fp, "Captain       %s~\n", city->captain);
  if(VLD_STR(city->sheriff))
    fprintf(fp, "Sheriff       %s~\n", city->sheriff);
  if(VLD_STR(city->knight))
    fprintf(fp, "Knight        %s~\n", city->knight);
  if(VLD_STR(city->allegiance))
    fprintf(fp, "Allegiance    %s~\n", city->allegiance);
  fprintf(fp, "Defense          %d\n", city->defense);
  fprintf(fp, "Offense          %d\n", city->offense);
  fprintf(fp, "OffenseCatapult  %d\n", city->ocatapult);
  fprintf(fp, "DefenseCatapult  %d\n", city->dcatapult);
  fprintf(fp, "Ballista         %d\n", city->ballista);
  fprintf(fp, "OffenseSoldiers  %d\n", city->osoldiers);
  fprintf(fp, "DefenseGuards    %d\n", city->dguards);
  fprintf(fp, "DefenseArchers   %d\n", city->darchers);
  fprintf(fp, "OffenseArchers   %d\n", city->oarchers);
  fprintf(fp, "DefenseWarriors  %d\n", city->dwarriors);
  fprintf(fp, "OffenseWarriors  %d\n", city->owarriors);
  fprintf(fp, "Arrows           %d\n", city->arrows);
  fprintf(fp, "Platform         %d\n", city->platform);
  fprintf(fp, "Ram              %d\n", city->ram);
  fprintf(fp, "Pitch            %d\n", city->pitch);
  fprintf(fp, "Wins             %d\n", city->wins);
  fprintf(fp, "Loses            %d\n", city->loses);
  fprintf(fp, "Recall           %d\n", city->recall);
  fprintf(fp, "Siege1           %d\n", city->siege1_room);
  fprintf(fp, "Siege2           %d\n", city->siege2_room);
  fprintf(fp, "Siege3           %d\n", city->siege3_room);
  fprintf(fp, "Defend1          %d\n", city->defend1_room);
  fprintf(fp, "Defend2          %d\n", city->defend2_room);
  fprintf(fp, "Defend3          %d\n", city->defend3_room);
  fprintf(fp, "MSoldier         %d\n", city->soldier_mob);
  fprintf(fp, "MArcher          %d\n", city->archer_mob);
  fprintf(fp, "MGuard           %d\n", city->guard_mob);
  fprintf(fp, "MWarrior         %d\n", city->warrior_mob);
  fprintf(fp, "OCatapult        %d\n", city->catapult_obj);
  fprintf(fp, "OBallista        %d\n", city->ballista_obj);
  fprintf(fp, "OPitch           %d\n", city->pitch_obj);
  fprintf(fp, "OPlatform        %d\n", city->platform_obj);
  fprintf(fp, "ORam             %d\n", city->ram_obj);
  fprintf(fp, "OArrow           %d\n", city->arrow_obj);
  fprintf(fp, "MinHouse         %d\n", city->min_house);
  fprintf(fp, "MaxHouse         %d\n", city->max_house);
  fprintf(fp, "Hardened         %d\n", city->hardened);
  fprintf(fp, "End\n\n");
  fwrite_citizenlist(fp, city);
  fprintf(fp, "#END\n");
  FileClose(fp);
}

/* Read in actual city data. */
void fread_city(CITY_DATA * city, FILE * fp)
{
  const char             *word;
  int                     defaultcity = 0;
  bool                    fMatch;

  for(;;)
  {
    word = feof(fp) ? "End" : fread_word(fp);
    fMatch = FALSE;

    switch (UPPER(word[0]))
    {
      case '*':
        fMatch = TRUE;
        fread_to_eol(fp);
        break;

      case 'A':
        KEY("Allegiance", city->allegiance, fread_string(fp));
        KEY("Arrows", city->arrows, fread_number(fp));
        break;

      case 'B':
        KEY("Ballista", city->ballista, fread_number(fp));
        KEY("Bank", city->bank, fread_string(fp));
        KEY("Baron", city->baron, fread_string(fp));
        break;

      case 'C':
        KEY("Captain", city->captain, fread_string(fp));
        break;

      case 'D':
        KEY("DefenseCatapult", city->dcatapult, fread_number(fp));
        KEY("Defense", city->defense, fread_number(fp));
        KEY("Description", city->description, fread_string(fp));
        KEY("Duke", city->duke, fread_string(fp));
        KEY("DefenseArchers", city->darchers, fread_number(fp));
        KEY("DefenseWarriors", city->dwarriors, fread_number(fp));
        KEY("DefenseGuards", city->dguards, fread_number(fp));
        KEY("Defend1", city->defend1_room, fread_number(fp));
        KEY("Defend2", city->defend2_room, fread_number(fp));
        KEY("Defend3", city->defend3_room, fread_number(fp));
        break;

      case 'E':
        if(!str_cmp(word, "End"))
        {
          return;
        }
        break;

      case 'F':
        KEY("Filename", city->filename, fread_string(fp));
        break;

      case 'H':
        KEY("Hardened", city->hardened, fread_number(fp));
        break;

      case 'K':
        KEY("Knight", city->knight, fread_string(fp));
        break;

      case 'L':
        KEY("Loses", city->loses, fread_number(fp));
        break;

      case 'M':
        KEY("MinHouse", city->min_house, fread_number(fp));
        KEY("MaxHouse", city->max_house, fread_number(fp));
        KEY("MSoldier", city->soldier_mob, fread_number(fp));
        KEY("MArcher", city->archer_mob, fread_number(fp));
        KEY("MGuard", city->guard_mob, fread_number(fp));
        KEY("MWarrior", city->warrior_mob, fread_number(fp));
        break;

      case 'N':
        KEY("Name", city->name, fread_string(fp));
        break;

      case 'O':
        KEY("OffenseCatapult", city->ocatapult, fread_number(fp));
        KEY("Offense", city->offense, fread_number(fp));
        KEY("OffenseArchers", city->oarchers, fread_number(fp));
        KEY("OffenseWarriors", city->owarriors, fread_number(fp));
        KEY("OffenseSoldiers", city->osoldiers, fread_number(fp));
        KEY("OCatapult", city->catapult_obj, fread_number(fp));
        KEY("OBallista", city->ballista_obj, fread_number(fp));
        KEY("OPitch", city->pitch_obj, fread_number(fp));
        KEY("OPlatform", city->platform_obj, fread_number(fp));
        KEY("ORam", city->ram_obj, fread_number(fp));
        KEY("OArrow", city->arrow_obj, fread_number(fp));
        break;

      case 'P':
        KEY("Pitch", city->pitch, fread_number(fp));
        KEY("Platform", city->platform, fread_number(fp));
        break;

      case 'R':
        KEY("Ram", city->ram, fread_number(fp));
        KEY("Recall", city->recall, fread_number(fp));
        break;

      case 'S':
        KEY("Siege1", city->siege1_room, fread_number(fp));
        KEY("Siege2", city->siege2_room, fread_number(fp));
        KEY("Siege3", city->siege3_room, fread_number(fp));
        KEY("Sheriff", city->sheriff, fread_string(fp));
        break;

      case 'W':
        KEY("Wins", city->wins, fread_number(fp));
        break;
    }

    if(!fMatch)
    {
      bug("Fread_city: no match: %s", word);
      fread_to_eol(fp);
    }
  }
}

void                    fread_citizenlist(CITY_DATA * city, FILE * fp);

/* Load a city file */
bool load_city_file(const char *cityfile)
{
  char                    filename[256];
  CITY_DATA              *city;
  FILE                   *fp;
  bool                    found;

  CREATE(city, CITY_DATA, 1);
  found = FALSE;
  city->siege1_room = 0;  /* Vnums for city stuff */
  city->siege2_room = 0;
  city->siege3_room = 0;
  city->defend1_room = 0;
  city->defend2_room = 0;
  city->defend3_room = 0;
  city->soldier_mob = 0;
  city->archer_mob = 0;
  city->guard_mob = 0;
  city->warrior_mob = 0;
  city->catapult_obj = 0;
  city->ballista_obj = 0;
  city->pitch_obj = 0;
  city->platform_obj = 0;
  city->ram_obj = 0;
  city->arrow_obj = 0;
  city->min_house = 0;
  city->max_house = 0;
  city->onlinechars = 0;
  city->defense = 0;
  city->offense = 0;
  city->ocatapult = 0;
  city->dcatapult = 0;
  city->ballista = 0;
  city->dguards = 0;
  city->osoldiers = 0;
  city->oarchers = 0;
  city->darchers = 0;
  city->owarriors = 0;
  city->dwarriors = 0;
  city->arrows = 0;
  city->platform = 0;
  city->ram = 0;
  city->pitch = 0;
  city->hardened = 0;
  snprintf(filename, 256, "%s%s", CITY_DIR, cityfile);
  if((fp = FileOpen(filename, "r")) != NULL)
  {
    found = TRUE;
    for(;;)
    {
      char                    letter;
      char                   *word;

      letter = fread_letter(fp);
      if(letter == '*')
      {
        fread_to_eol(fp);
        continue;
      }
      if(letter != '#')
      {
        bug("%s", "Load_city_file: # not found.");
        break;
      }
      word = fread_word(fp);
      if(!str_cmp(word, "CITY"))
        fread_city(city, fp);
      else if(!str_cmp(word, "ROLLCALL"))
        fread_citizenlist(city, fp);
      else if(!str_cmp(word, "END"))
        break;
      else
      {
        bug("%s: bad section: %s.", __FUNCTION__, word);
        break;
      }
    }
    FileClose(fp);
  }
  if(found)
    LINK(city, first_city, last_city, next, prev);
  else
    DISPOSE(city);
  return found;
}

/* Load in all the city files. */
void load_cities()
{
  FILE                   *fpList;
  const char             *filename;
  char                    citylist[256];

  first_city = NULL;
  last_city = NULL;
  log_string("Loading cities...");
  snprintf(citylist, 256, "%s%s", CITY_DIR, CITY_LIST);
  if((fpList = FileOpen(citylist, "r")) == NULL)
  {
    bug("load_cities: can't open %s for reading.", citylist);
    perror(citylist);
    exit(1);
  }
  for(;;)
  {
    filename = feof(fpList) ? "$" : fread_word(fpList);

    if(!filename || filename[0] == '\0' || filename[0] == '$')
      break;

    log_string(filename);

    if(!load_city_file(filename))
      bug("Cannot load city file: %s", filename);
  }
  FileClose(fpList);
  log_string(" Done cities ");
}

void do_revoke(CHAR_DATA *ch, char *argument)
{
  char                    arg[MIL];
  CHAR_DATA              *victim;
  CITY_DATA              *city;

  if(IS_NPC(ch) || !ch->pcdata->city)
  {
    send_to_char("Huh?\r\n", ch);
    return;
  }

  city = ch->pcdata->city;

  if(!str_cmp(ch->name, city->duke) || !str_cmp(ch->name, city->baron));
  else
  {
    send_to_char("Huh?\r\n", ch);
    return;
  }

  argument = one_argument(argument, arg);
  if(arg[0] == '\0')
  {
    send_to_char("Revoke whom from your city?\r\n", ch);
    return;
  }

  if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("That player is not here.\r\n", ch);
    return;
  }
  if(IS_NPC(victim))
  {
    send_to_char("Not on NPC's.\r\n", ch);
    return;
  }
  if(victim->pcdata->city != ch->pcdata->city)
  {
    send_to_char("This player does not belong to your city!\r\n", ch);
    return;
  }
  victim->pcdata->city = NULL;
  if(VLD_STR(victim->pcdata->city_name))
    STRFREE(victim->pcdata->city_name);
  act(AT_MAGIC, "You revoke $N from $t", ch, city->name, victim, TO_CHAR);
  act(AT_MAGIC, "$n revokes $N from $t", ch, city->name, victim, TO_ROOM);
  act(AT_MAGIC, "$n revokes you from $t", ch, city->name, victim, TO_VICT);
  remove_rollcall(city, victim->name);
  save_char_obj(victim);
  save_city(city);
}

void do_checksieges(CHAR_DATA *ch, char *argument)
{
  SIEGE_DATA             *siege, *siege_next;
  int                     defense, offense;
  bool                    endall = FALSE, foundsiege = FALSE;

  if(!VLD_STR(argument))
  {
    send_to_char("Usage: checksieges status\r\n", ch);
    send_to_char("checksieges endall\r\n", ch);
    return;
  }

  if(VLD_STR(argument) && !str_cmp(argument, "endall"))
    endall = TRUE;

  if(!str_cmp(argument, "endall") || !str_cmp(argument, "status"))
  {
    for(siege = first_siege; siege; siege = siege_next)
    {
      siege_next = siege->next;

      defense = count_defenders(siege);
      offense = count_attackers(siege);

      foundsiege = TRUE;

      if(!endall)
      {
        ch_printf(ch, "%s(%d) is attacking %s(%d).\r\n", siege->acity->name, offense, siege->dcity->name, defense);
        continue;
      }

      end_siege(siege);
      UNLINK(siege, first_siege, last_siege, next, prev);
      siege->acity = NULL;
      siege->dcity = NULL;
      DISPOSE(siege);
    }

    if(!foundsiege)
      send_to_char("No sieges currently going on.\r\n", ch);
  }
}

void do_setcity(CHAR_DATA *ch, char *argument)
{
  SIEGE_DATA             *siege, *siege_next;
  CITY_DATA              *city;
  char                    arg1[MIL], arg2[MIL], buf[MSL];

  set_char_color(AT_PLAIN, ch);
  if(IS_NPC(ch))
  {
    send_to_char("Huh?\r\n", ch);
    return;
  }
  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  if(!VLD_STR(arg1))
  {
    send_to_char("Usage: setcity <city> <field> <player>\r\n", ch);
    send_to_char("\r\nField being one of:\r\n", ch);
    send_to_char(" duke baron captain sheriff knight\r\n", ch);
    send_to_char(" desc endsiege reset recall\r\n", ch);
    send_to_char(" name filename allegiance\r\n", ch);
    send_to_char(" siege1 siege2 siege3 defend1 defend2 defend3\r\n", ch);
    send_to_char(" soldier archer warrior guard catapult ballista pitch\r\n", ch);
    send_to_char(" platform ram arrow minhouse maxhouse settomax\r\n", ch);
    return;
  }

  city = get_city(arg1);
  if(!city)
  {
    send_to_char("No such city.\r\n", ch);
    return;
  }
  if(!str_cmp(arg2, "duke"))
  {
    if(VLD_STR(city->duke))
      STRFREE(city->duke);
    if(VLD_STR(argument))
      city->duke = STRALLOC(argument);
  }
  else if(!str_cmp(arg2, "baron"))
  {
    if(VLD_STR(city->baron))
      STRFREE(city->baron);
    if(VLD_STR(argument))
      city->baron = STRALLOC(argument);
  }
  else if(!str_cmp(arg2, "endsiege"))
  {
    bool                    ended = FALSE;

    for(siege = first_siege; siege; siege = siege_next)
    {
      siege_next = siege->next;

      if(siege->dcity != city)
        continue;

      ended = TRUE;
      sprintf(buf, "&C%s siege against %s has been ended early by %s!", siege->acity->name, siege->dcity->name, ch->name);
      announce(buf);
      end_siege(siege);
      UNLINK(siege, first_siege, last_siege, next, prev);
      siege->acity = NULL;
      siege->dcity = NULL;
      DISPOSE(siege);
    }
    if(!ended)
      ch_printf(ch, "%s isn't currently being sieged by any city.\r\n", city->name);
    return;
  }
  else if(!str_cmp(arg2, "bank"))
  {
    if(VLD_STR(city->bank))
      STRFREE(city->bank);
    if(VLD_STR(argument))
      city->bank = STRALLOC(argument);
  }
  else if(!str_cmp(arg2, "captain"))
  {
    if(VLD_STR(city->captain))
      STRFREE(city->captain);
    if(VLD_STR(argument))
      city->captain = STRALLOC(argument);
  }
  else if(!str_cmp(arg2, "sheriff"))
  {
    if(VLD_STR(city->sheriff))
      STRFREE(city->sheriff);
    if(VLD_STR(argument))
      city->sheriff = STRALLOC(argument);
  }
  else if(!str_cmp(arg2, "knight"))
  {
    if(VLD_STR(city->knight))
      STRFREE(city->knight);
    if(VLD_STR(argument))
      city->knight = STRALLOC(argument);
  }
  else if(!str_cmp(arg2, "allegiance"))
  {
    if(VLD_STR(city->allegiance))
      STRFREE(city->allegiance);
    if(VLD_STR(argument))
      city->allegiance = STRALLOC(argument);
  }
  else if(!str_cmp(arg2, "recall"))
  {
    city->recall = atoi(argument);
    send_to_char("City recall set.\r\n", ch);
  }
  else if(!str_cmp(arg2, "siege1"))
  {
    city->siege1_room = atoi(argument);
    send_to_char("City siege1 set.\r\n", ch);
  }
  else if(!str_cmp(arg2, "siege2"))
  {
    city->siege2_room = atoi(argument);
    send_to_char("City siege2 set.\r\n", ch);
  }
  else if(!str_cmp(arg2, "siege3"))
  {
    city->siege3_room = atoi(argument);
    send_to_char("City siege3 set.\r\n", ch);
  }
  else if(!str_cmp(arg2, "defend1"))
  {
    city->defend1_room = atoi(argument);
    send_to_char("City defend1 set.\r\n", ch);
  }
  else if(!str_cmp(arg2, "defend2"))
  {
    city->defend2_room = atoi(argument);
    send_to_char("City defend2 set.\r\n", ch);
  }
  else if(!str_cmp(arg2, "defend3"))
  {
    city->defend3_room = atoi(argument);
    send_to_char("City defend3 set.\r\n", ch);
  }
  else if(!str_cmp(arg2, "soldier"))
  {
    city->soldier_mob = atoi(argument);
    send_to_char("City soldier set.\r\n", ch);
  }
  else if(!str_cmp(arg2, "archer"))
  {
    city->archer_mob = atoi(argument);
    send_to_char("City archer set.\r\n", ch);
  }
  else if(!str_cmp(arg2, "warrior"))
  {
    city->warrior_mob = atoi(argument);
    send_to_char("City warrior set.\r\n", ch);
  }
  else if(!str_cmp(arg2, "guard"))
  {
    city->guard_mob = atoi(argument);
    send_to_char("City guard set.\r\n", ch);
  }

  else if(!str_cmp(arg2, "minhouse"))
  {
    city->min_house = atoi(argument);
    send_to_char("City minhouse set.\r\n", ch);
  }
  else if(!str_cmp(arg2, "maxhouse"))
  {
    city->max_house = atoi(argument);
    send_to_char("City maxhouse set.\r\n", ch);
  }
  else if(!str_cmp(arg2, "arrow"))
  {
    city->arrow_obj = atoi(argument);
    send_to_char("City arrow set.\r\n", ch);
  }
  else if(!str_cmp(arg2, "catapult"))
  {
    city->catapult_obj = atoi(argument);
    send_to_char("City catapult set.\r\n", ch);
  }
  else if(!str_cmp(arg2, "ballista"))
  {
    city->ballista_obj = atoi(argument);
    send_to_char("City ballista set.\r\n", ch);
  }
  else if(!str_cmp(arg2, "pitch"))
  {
    city->pitch_obj = atoi(argument);
    send_to_char("City pitch set.\r\n", ch);
  }
  else if(!str_cmp(arg2, "platform"))
  {
    city->platform_obj = atoi(argument);
    send_to_char("City platform set.\r\n", ch);
  }
  else if(!str_cmp(arg2, "ram"))
  {
    city->ram_obj = atoi(argument);
    send_to_char("City ram set.\r\n", ch);
  }
  else if(!str_cmp(arg2, "settomax"))
  {
    city->ballista = 6;
    city->ram = 2;
    city->platform = 6;
    city->ocatapult = 6;
    city->osoldiers = 50;
    city->oarchers = 50;
    city->owarriors = 50;
    city->pitch = 10;
    city->arrows = 50;
    city->dcatapult = 10;
    city->dguards = 50;
    city->darchers = 50;
    city->dwarriors = 50;
    update_city_def_off(city);
    save_city(city);
  }
  else if(!str_cmp(arg2, "reset"))
  {
    city->ballista = 0;
    city->ram = 0;
    city->platform = 0;
    city->ocatapult = 0;
    city->osoldiers = 0;
    city->oarchers = 0;
    city->owarriors = 0;
    city->pitch = 0;
    city->arrows = 0;
    city->dcatapult = 0;
    city->dguards = 0;
    city->darchers = 0;
    city->dwarriors = 0;
    city->defense = 0;
    city->offense = 0;
    city->hardened = 0;
    save_city(city);
  }
  else if(get_trust(ch) < LEVEL_IMMORTAL)
  {
    do_setcity(ch, (char *)"");
    return;
  }
  else if(!str_cmp(arg2, "name"))
  {
    if(VLD_STR(city->name))
      STRFREE(city->name);
    if(VLD_STR(argument))
      city->name = STRALLOC(argument);
  }
  else if(!str_cmp(arg2, "filename"))
  {
    if(VLD_STR(city->filename))
      STRFREE(city->filename);
    if(VLD_STR(argument))
      city->filename = STRALLOC(argument);
    write_city_list();
  }
  else if(!str_cmp(arg2, "desc"))
  {
    if(VLD_STR(city->description))
      STRFREE(city->description);
    if(VLD_STR(argument))
      city->description = STRALLOC(argument);
  }
  else if(get_trust(ch) < LEVEL_AJ_CPL)
  {
    do_setcity(ch, (char *)"");
    return;
  }
  else
  {
    do_setcity(ch, (char *)"");
    return;
  }
  send_to_char("Done.\r\n", ch);
  save_city(city);
}

void do_showcity(CHAR_DATA *ch, char *argument)
{
  CITY_DATA              *city;

  set_char_color(AT_PLAIN, ch);
  if(IS_NPC(ch))
  {
    send_to_char("Huh?\r\n", ch);
    return;
  }
  if(!VLD_STR(argument))
  {
    send_to_char("Usage: showcity <city>\r\n", ch);
    return;
  }

  city = get_city(argument);
  if(!city)
  {
    send_to_char("No such city.\r\n", ch);
    return;
  }
  ch_printf(ch, "\r\n&wCity:     &W%s\r\n", city->name ? city->name : "Not Set");
  ch_printf(ch, "&wFilename:     &W%s\r\n", city->filename ? city->filename : "Not Set");
  ch_printf(ch, "&wAllegiance:   &W%s\r\n", city->allegiance ? city->allegiance : "Not Set");
  ch_printf(ch, "&wDuke:         &W%s\r\n", city->duke ? city->duke : "Not Set");
  ch_printf(ch, "&wBaron:        &W%s\r\n", city->baron ? city->baron : "Not Set");
  ch_printf(ch, "&wCaptain:      &W%s\r\n", city->captain ? city->captain : "Not Set");
  ch_printf(ch, "&wSheriff:      &W%s\r\n", city->sheriff ? city->sheriff : "Not Set");
  ch_printf(ch, "&wKnight:       &W%s\r\n", city->knight ? city->knight : "Not Set");
  ch_printf(ch, "&wDefense:      &W%d\r\n", city->defense);
  ch_printf(ch, "&wOffense:      &W%d\r\n", city->offense);
  ch_printf(ch, "&wWins:         &W%d\r\n", city->wins);
  ch_printf(ch, "&wLoses:        &W%d\r\n", city->loses);
  ch_printf(ch, "&wRecall:       &W%d\r\n", city->recall);
  ch_printf(ch, "&wSiege1:       &W%d\r\n", city->siege1_room);
  ch_printf(ch, "&wSiege2:       &W%d\r\n", city->siege2_room);
  ch_printf(ch, "&wSiege3:       &W%d\r\n", city->siege3_room);
  ch_printf(ch, "&wDefend1:      &W%d\r\n", city->defend1_room);
  ch_printf(ch, "&wDefend2:      &W%d\r\n", city->defend2_room);
  ch_printf(ch, "&wDefend3:      &W%d\r\n", city->defend3_room);
  ch_printf(ch, "&wSoldier:      &W%d\r\n", city->soldier_mob);
  ch_printf(ch, "&wWarrior:      &W%d\r\n", city->warrior_mob);
  ch_printf(ch, "&wGuard:        &W%d\r\n", city->guard_mob);
  ch_printf(ch, "&wArcher:       &W%d\r\n", city->archer_mob);
  ch_printf(ch, " &wOSoldiers: &W%d", city->osoldiers);
  ch_printf(ch, " &wDGuards: &W%d", city->dguards);
  ch_printf(ch, " &wDWarriors: &W%d", city->dwarriors);
  ch_printf(ch, " &wOWarriors: &W%d\r\n", city->owarriors);
  ch_printf(ch, " &wDArchers: &W%d", city->darchers);
  ch_printf(ch, " &wOArchers: &W%d\r\n", city->oarchers);
  ch_printf(ch, "  &wCatapult:     &W%d\r\n", city->catapult_obj);
  ch_printf(ch, " &wDCatapult: &W%d", city->dcatapult);
  ch_printf(ch, " &wOCatapult: &W%d\r\n", city->ocatapult);
  ch_printf(ch, " &wBallista:     &W%d\r\n", city->ballista_obj);
  ch_printf(ch, "   &wBallistas: &W%d\r\n", city->ballista);
  ch_printf(ch, "&wPitch:        &W%d\r\n", city->pitch_obj);
  ch_printf(ch, "   &wPitches:   &W%d\r\n", city->pitch);
  ch_printf(ch, "&wPlatform:     &W%d\r\n", city->platform_obj);
  ch_printf(ch, "   &wPlatforms: &W%d\r\n", city->platform);
  ch_printf(ch, "&wRam:          &W%d\r\n", city->ram_obj);
  ch_printf(ch, "   &wRams:      &W%d\r\n", city->ram);
  ch_printf(ch, "&wArrow:        &W%d\r\n", city->arrow_obj);
  ch_printf(ch, "&wArrows:       &W%d\r\n", city->arrows);
  ch_printf(ch, "&wHardened:     &W%d\r\n", city->hardened);
  ch_printf(ch, "&WMinHouse:     &W%d\r\n", city->min_house);
  ch_printf(ch, "&WMaxHouse:     &W%d\r\n", city->max_house);
  ch_printf(ch, "&wDescription:\r\n&W%s\r\n", city->description ? city->description : "Not Set");
}

void do_makecity(CHAR_DATA *ch, char *argument)
{
  char                    filename[256];
  CITY_DATA              *city;
  bool                    found;

  set_char_color(AT_IMMORT, ch);
  if(!VLD_STR(argument))
  {
    send_to_char("Usage: makecity <city name>\r\n", ch);
    return;
  }
  found = FALSE;
  snprintf(filename, 256, "%s%s", CITY_DIR, strlower(argument));
  CREATE(city, CITY_DATA, 1);
  LINK(city, first_city, last_city, next, prev);
  city->name = STRALLOC(argument);
  send_to_char("city now made.\r\n", ch);
}

// Support for removing invalid players - Aurin
bool valid_player       args((char *name));

void do_city(CHAR_DATA *ch, char *argument)
{
  CITY_DATA              *city;
  bool                    changed = FALSE;

  set_char_color(AT_CYAN, ch);
  if(!first_city)
  {
    send_to_char("There are no cities currently formed.\r\n", ch);
    return;
  }
  if(!VLD_STR(argument))
  {
    send_to_char("\r\n&WCurrent Cities in the &RCLASSIC 6DRAGONS&W realms\r\n", ch);
    send_to_char("&c----------------------------------------------------------------------------\r\n", ch);
    send_to_char("\r\n&cCity                     Allegiance           Duke\r\n", ch);
    for(city = first_city; city; city = city->next)
      ch_printf(ch, "&C%-24s %-20s %-20s\r\n", city->name ? city->name : "Unknown City", city->allegiance ? city->allegiance : "No Allegiance", city->duke ? city->duke : "No Duke");
    send_to_char("\r\n&cUse '&Wcity &c<&Wname of city&c>' for more detailed information.\r\n", ch);
    return;
  }

  city = get_city(argument);
  if(!city)
  {
    send_to_char("&cNo such city exists...\r\n", ch);
    return;
  }

  if(city->duke)
  {
    if(!valid_player(city->duke))
    {
      STRFREE(city->duke);
      changed = TRUE;
      ch_printf(ch, "Duke removed for being an invalid player. Please use the command again.\r\n");
    }
  }
  if(city->baron)
  {
    if(!valid_player(city->baron))
    {
      STRFREE(city->baron);
      changed = TRUE;
      ch_printf(ch, "Baron removed for being an invalid player. Please use the command again.\r\n");
    }
  }
  if(city->captain)
  {
    if(!valid_player(city->captain))
    {
      STRFREE(city->captain);
      changed = TRUE;
      ch_printf(ch, "Captain removed for being an invalid player. Please use the command again.\r\n");
    }
  }
  if(city->sheriff)
  {
    if(!valid_player(city->sheriff))
    {
      STRFREE(city->sheriff);
      changed = TRUE;
      ch_printf(ch, "Sheriff removed for being an invalid player. Please use the command again.\r\n");
    }
  }
  if(city->knight)
  {
    if(!valid_player(city->knight))
    {
      STRFREE(city->knight);
      changed = TRUE;
      ch_printf(ch, "Knight removed for being an invalid player. Please use the command again.\r\n");
    }
  }

  if(changed)
    save_city(city);

  ch_printf(ch, "&c\r\n%s\r\n", city->name);
  ch_printf(ch, "&cDuke:       &w%s\r\n", city->duke ? city->duke : "No One");
  ch_printf(ch, "&cBaron:      &w%s\r\n", city->baron ? city->baron : "No One");
  ch_printf(ch, "&cCaptain:    &w%s\r\n", city->captain ? city->captain : "No One");
  ch_printf(ch, "&cSheriff:    &w%s\r\n", city->sheriff ? city->sheriff : "No One");
  ch_printf(ch, "&cKnight:     &w%s\r\n", city->knight ? city->knight : "No One");
  ch_printf(ch, "&cAllegiance  &w%s\r\n", city->allegiance ? city->allegiance : "No One");
  ch_printf(ch, "&cDefense:    &w%d\r\n", city->defense);
  ch_printf(ch, "&cOffense:    &w%d\r\n", city->offense);
  ch_printf(ch, "&cWins:       &w%d    &cLoses:      &w%d\r\n", city->wins, city->loses);
  ch_printf(ch, "&cDescription:\r\n&w%s\r\n", city->description ? city->description : "No Description");
}

/* Updating Dragon Lair quest to be a little more snazzy and automating it to not require a staff member to assign them the lair.  
This makes it easier for players especially if staff are not around or are busy.  In addition to adding this snippet you will have 
to make sure the min vnum and max vnum is set to what area you want for lairs.  Then use the rat command to set all free lairs to Not Assigned.  Vladaar */

void do_mplair(CHAR_DATA *ch, char *argument)
{
  ROOM_INDEX_DATA        *location;
  CHAR_DATA              *victim;
  AREA_DATA              *tarea;
  char                    arg[MIL];
  char                    Newfilename[MSL];
  char                    ownname[MSL];
  int                     min, max, vnum;
  OBJ_DATA               *obj;

  if(!IS_NPC(ch) || IS_AFFECTED(ch, AFF_CHARM))
  {
    error(ch);
    return;
  }

  argument = one_argument(argument, arg);

  if(arg[0] == '\0')
  {
    progbug(ch, "Mplair - Invalid argument [%s]", ch->name);
    return;
  }

  if(!(victim = get_char_room(ch, arg)))
  {
    if(!(victim = get_char_world(ch, arg)))
      progbug(ch, "Mplair [%d]- victim [%s] does not exist", ch->pIndexData ? ch->pIndexData->vnum : 0, arg);
    return;
  }

  if(IS_NPC(victim))
  {
    send_to_char("Not on NPC's.\r\n", victim);
    return;
  }

  if(victim->pcdata->lair > 0)
  {
    return;
  }

  for(obj = victim->first_carrying; obj; obj = obj->next_content)
  {
    if(obj->pIndexData->vnum == 40022)
      break;
  }

  if(!obj)
  {
    return;
  }

  obj_from_char(obj);
  extract_obj(obj);

  /*
   * need to add pause here and so they can't move Then some cool cinematic affects of
   * breaching into mountain cavern to dig out lair 
   */

  act(AT_YELLOW, "A power suddenly forms within you!", ch, NULL, victim, TO_VICT);
  interpret(ch, (char *)"mppause 5");
  act(AT_WHITE, "You breech the void between time and space, and disappear.\r\n", victim, NULL, NULL, TO_CHAR);
  interpret(ch, (char *)"mppause 5");
  act(AT_ORANGE, "You reappear inside the caverns and burrow and tear through the stone digging out your lair!\r\n", victim, NULL, NULL, TO_CHAR);

  if(victim->race != RACE_DRAGON)
  {
    send_to_char("You must be a dragon to have a lair!\r\n", ch);
    return;
  }
  // 2000 vnum area should be enough for dragon lairs
  min = 65000;
  max = 67000;

  // search for an open room to assign new house
  for(vnum = min; vnum <= max; vnum++)
  {
    if(!(location = get_room_index(vnum)))
      continue;

    if(!str_cmp(location->owner, "Not Assigned"))
    {
      victim->pcdata->lair = vnum;
      break;
    }
  }

  /*
   * Add check to see if we need to report that Lairs available ran out.  
   */

  if(victim->pcdata->lair < 65000 || !victim->pcdata->lair)
  {
    send_to_char
      ("The Staff of Classic 6Dragons sincerely apologize but currently all available lairs are taken.\r\nPlease report this to a staff member immediately and you will be issued a lair once this is remedied.\r\n",
       ch);
    return;
  }

  // send player to new home
  location = get_room_index(victim->pcdata->lair);
  char_from_room(victim);
  char_to_room(victim, location);

  // Set the room owner to the player name
  if(VLD_STR(location->owner))
    STRFREE(location->owner);
  location->owner = STRALLOC(victim->name);

  if(VLD_STR(location->name))
    STRFREE(location->name);
  snprintf(ownname, sizeof(ownname), "%s's Lair", location->owner);
  location->name = STRALLOC(ownname);

  do_look(victim, (char *)"auto");

  // save the area
  tarea = victim->in_room->area;
  snprintf(Newfilename, MSL, "%s%s", AREA_DIR, victim->in_room->area->filename);
  fold_area(tarea, Newfilename, FALSE);

}

void do_mpcity(CHAR_DATA *ch, char *argument)
{
  CITY_DATA              *city;
  ROOM_INDEX_DATA        *location;
  CHAR_DATA              *victim;
  AREA_DATA              *tarea;
  char                    arg[MIL];
  char                    arg1[MIL];
  char                    Newfilename[MSL];
  char                    ownname[MSL];
  int                     min, max, vnum;

  if(!IS_NPC(ch) || IS_AFFECTED(ch, AFF_CHARM))
  {
    error(ch);
    return;
  }

  argument = one_argument(argument, arg);
  argument = one_argument(argument, arg1);
  if(arg[0] == '\0')
  {
    progbug(ch, "Mpcity - Invalid argument [%s]", ch->name);
    return;
  }

  if(!(victim = get_char_room(ch, arg)))
  {
    if(!(victim = get_char_world(ch, arg)))
      progbug(ch, "Mpcity [%d]- victim [%s] does not exist", ch->pIndexData ? ch->pIndexData->vnum : 0, arg);
    return;
  }

  if(IS_NPC(victim))
  {
    send_to_char("Not on NPC's.\r\n", victim);
    return;
  }

  city = get_city(arg1);
  if(!city)
  {
    send_to_char("No such city.\r\n", victim);
    return;
  }

  if(VLD_STR(victim->pcdata->city_name))
    STRFREE(victim->pcdata->city_name);
  victim->pcdata->city_name = QUICKLINK(city->name);
  victim->pcdata->city = city;
  add_rollcall(city, victim->name, victim->Class, victim->level, victim->pcdata->mkills, victim->pcdata->mdeaths);
  save_char_obj(victim);
  act(AT_WHITE, "$n awards $N citizenship in $t.", ch, city->name, victim, TO_NOTVICT);
  act(AT_WHITE, "$n inducts you citizenship in $t.", ch, city->name, victim, TO_VICT);

  if(victim->race != RACE_DRAGON)
  {
    // check what homeland they are in
    if(victim->pcdata->city->min_house <= 0 || victim->pcdata->city->max_house <= 0 || victim->pcdata->city->max_house <= victim->pcdata->city->min_house)
    {
      send_to_char("Sorry, your city doesn't have any housing.\r\n", ch);
      return;
    }

    min = victim->pcdata->city->min_house;
    max = victim->pcdata->city->max_house;
    // search for an open room to assign new house
    for(vnum = min; vnum <= max; vnum++)
    {
      if(!(location = get_room_index(vnum)))
        continue;

      if(!str_cmp(location->owner, "Not Assigned"))
      {
        victim->pcdata->lair = vnum;
        break;
      }
    }

    // send player to new home
    location = get_room_index(victim->pcdata->lair);
    char_from_room(victim);
    char_to_room(victim, location);

    // Set the room owner to the player name
    if(VLD_STR(location->owner))
      STRFREE(location->owner);
    location->owner = STRALLOC(victim->name);

    if(VLD_STR(location->name))
      STRFREE(location->name);
    snprintf(ownname, sizeof(ownname), "%s's House", location->owner);
    location->name = STRALLOC(ownname);

    do_look(victim, (char *)"auto");

    // save the area
    tarea = victim->in_room->area;
    snprintf(Newfilename, MSL, "%s%s", AREA_DIR, victim->in_room->area->filename);
    fold_area(tarea, Newfilename, FALSE);
  }
}

/* Update the city's offense and defense */
/* Should have it adjust after new things are bought and after a siege */
void update_city_def_off(CITY_DATA * city)
{
  if(!city)
    return;

  /*
   * Set them to basic 
   */
  city->defense = 0;
  city->offense = 0;

  city->pitch += city->return_pitch;
  city->defense += (10 * city->pitch);
  city->return_pitch = 0;

  city->arrows += city->return_arrows;
  city->defense += (1 * city->arrows);
  city->return_arrows = 0;

  city->dcatapult += city->return_dcatapult;
  city->defense += (5 * city->dcatapult);
  city->return_dcatapult = 0;

  city->dguards += city->return_dguards;
  city->defense += (2 * city->dguards);
  city->return_dguards = 0;

  city->darchers += city->return_darchers;
  city->defense += (2 * city->darchers);
  city->return_darchers = 0;

  city->dwarriors += city->return_dwarriors;
  city->defense += (2 * city->dwarriors);
  city->return_dwarriors = 0;

  city->ballista += city->return_ballista;
  city->offense += (10 * city->ballista);
  city->return_ballista = 0;

  city->ram += city->return_ram;
  city->offense += (25 * city->ram);
  city->return_ram = 0;

  city->ocatapult += city->return_ocatapult;
  city->offense += (10 * city->ocatapult);
  city->return_ocatapult = 0;

  city->osoldiers += city->return_osoldiers;
  city->offense += (2 * city->osoldiers);
  city->return_osoldiers = 0;

  city->oarchers += city->return_oarchers;
  city->offense += (2 * city->oarchers);
  city->return_oarchers = 0;

  city->owarriors += city->return_owarriors;
  city->offense += (2 * city->owarriors);
  city->return_owarriors = 0;

  city->platform += city->return_platform;
  city->offense += (15 * city->platform);
  city->return_platform = 0;
}

void do_fortify(CHAR_DATA *ch, char *argument)
{
  CITY_DATA              *city;
  BANK_DATA              *bank;
  char                    arg1[MIL];
  int                     count = 1;  /* Default to 1 */

  if(IS_NPC(ch) || !ch->pcdata->city)
  {
    send_to_char("Huh?\r\n", ch);
    return;
  }

  city = get_city(ch->pcdata->city_name);
  if(!city)
  {
    send_to_char("No such city.\r\n", ch);
    return;
  }

  bank = find_bank(city->bank);

  if(!bank)
  {
    send_to_char("There is no account for that city!\r\n", ch);
    return;
  }

  argument = one_argument(argument, arg1);
  if(!VLD_STR(arg1))
  {
    send_to_char("Usage: fortify <field>  <amount>\r\n", ch);
    send_to_char("\r\nField being one of:\r\n", ch);
    send_to_char(" pitch arrows catapults guards warriors archers\r\n", ch);
    send_to_char(" costs - lists costs to fortify\r\n", ch);
    return;
  }

  if(VLD_STR(argument) && is_number(argument))
    count = UMAX(1, atoi(argument));

  if(!str_cmp(arg1, "costs"))
  {
    ch_printf(ch, "Pitch costs 50 silver. You have %d of them.\r\n", city->pitch);
    ch_printf(ch, "Arrows costs 10 silver. You have %d of them.\r\n", city->arrows);
    ch_printf(ch, "Catapults costs 50 silver. You have %d of them.\r\n", city->dcatapult);
    ch_printf(ch, "Guards costs 10 silver. You have %d of them.\r\n", city->dguards);
    ch_printf(ch, "Archers costs 10 silver. You have %d of them.\r\n", city->darchers);
    ch_printf(ch, "Warriors costs 10 silver. You have %d of them.\r\n", city->dwarriors);
    ch_printf(ch, "You have %d silver in %s city bank.\r\n", bank->silver, city->name);
    send_to_char("Usage: fortify <field>  <amount>\r\n", ch);
    return;
  }

  if(is_in_siege(city))
  {
    send_to_char("You cannot fortify during a siege\r\n", ch);
    return;
  }

  if(!str_cmp(arg1, "pitch"))
  {
    if((city->pitch + count) > 10)
    {
      ch_printf(ch, "Your city has %d cauldrons of pitch, the most you can have is 10.\r\n", city->pitch);
      return;
    }
    if(bank->silver < (50 * count))
    {
      ch_printf(ch, "Your city bank account cannot afford %d of them.\r\n", count);
      return;
    }
    bank->silver -= (50 * count);
    ch_printf(ch, "You fortify your city with %d cauldron of burning pitch.\r\n", count);
    city->pitch += count;
  }
  else if(!str_cmp(arg1, "arrows"))
  {
    if((city->arrows + count) > 50)
    {
      send_to_char("You cannot have more then 50 dozen arrows.\r\n", ch);
      return;
    }
    if(bank->silver < (10 * count))
    {
      send_to_char("Your city bank account cannot afford that.\r\n", ch);
      return;
    }
    bank->silver -= (10 * count);
    ch_printf(ch, "You fortify your city with %d bundles of arrows.\r\n", count);
    city->arrows += count;
  }
  else if(!str_cmp(arg1, "catapults"))
  {
    if((city->dcatapult + count) > 10)
    {
      send_to_char("You cannot have more then 10 catapults.\r\n", ch);
      return;
    }
    if(bank->silver < (50 * count))
    {
      send_to_char("Your city bank account cannot afford that.\r\n", ch);
      return;
    }
    bank->silver -= (50 * count);
    ch_printf(ch, "You fortify your city with %d catapults.\r\n", count);
    city->dcatapult += count;
  }
  else if(!str_cmp(arg1, "guards"))
  {
    if((city->dguards + count) > 50)
    {
      send_to_char("You cannot have more then 50 guards.\r\n", ch);
      return;
    }
    if(bank->silver < (10 * count))
    {
      send_to_char("Your city bank account cannot afford that.\r\n", ch);
      return;
    }
    bank->silver -= (10 * count);
    ch_printf(ch, "You fortify your city with %d guards.\r\n", count);
    city->dguards += count;
  }
  else if(!str_cmp(arg1, "archers"))
  {
    if((city->darchers + count) > 50)
    {
      send_to_char("You cannot have more then 50 archers.\r\n", ch);
      return;
    }
    if(bank->silver < (10 * count))
    {
      send_to_char("Your city bank account cannot afford that.\r\n", ch);
      return;
    }
    bank->silver -= (10 * count);
    ch_printf(ch, "You fortify your city with %d archers.\r\n", count);
    city->darchers += count;
  }
  else if(!str_cmp(arg1, "warriors"))
  {
    if((city->dwarriors + count) > 50)
    {
      send_to_char("You cannot have more then 50 warriors.\r\n", ch);
      return;
    }
    if(bank->silver < (10 * count))
    {
      send_to_char("Your city bank account cannot afford that.\r\n", ch);
      return;
    }
    bank->silver -= (10 * count);
    ch_printf(ch, "You fortify your city with %d warriors.\r\n", count);
    city->dwarriors += count;
  }
  else
  {
    do_fortify(ch, (char *)"");
    return;
  }

  update_city_def_off(city);
  save_city(city);
}

void do_siege(CHAR_DATA *ch, char *argument)
{
  CITY_DATA              *city;
  BANK_DATA              *bank;
  char                    arg1[MIL];
  int                     count = 1, cost = 0, siegetype = 0;

  if(IS_NPC(ch) || !ch->pcdata->city)
  {
    send_to_char("Huh?\r\n", ch);
    return;
  }

  city = get_city(ch->pcdata->city_name);
  if(!city)
  {
    send_to_char("No such city.\r\n", ch);
    return;
  }

  bank = find_bank(city->bank);
  if(!bank)
  {
    send_to_char("There is no account for that city!\r\n", ch);
    return;
  }

  argument = one_argument(argument, arg1);
  if(!VLD_STR(arg1))
  {
    send_to_char("Usage: siege <field> <amount>\r\n", ch);
    send_to_char("\r\nField being one of:\r\n", ch);
    send_to_char(" catapults ballista ram platform soldiers archers warriors platform\r\n", ch);
    send_to_char(" costs - displays the costs for siege items\r\n", ch);
    return;
  }

  if(VLD_STR(argument) && is_number(argument))
    count = UMAX(1, atoi(argument));

  if(!str_cmp(arg1, "costs"))
  {
    ch_printf(ch, "Ballista costs 50 silver. You have %d of them.\r\n", city->ballista);
    ch_printf(ch, "Rams cost 40 silver. You have %d of them.\r\n", city->ram);
    ch_printf(ch, "Platforms costs 50 silver. You have %d of them.\r\n", city->platform);
    ch_printf(ch, "Catapults costs 50 silver. You have %d of them.\r\n", city->ocatapult);
    ch_printf(ch, "Soldiers costs 10 silver. You have %d of them.\r\n", city->osoldiers);
    ch_printf(ch, "Archers costs 10 silver. You have %d of them.\r\n", city->oarchers);
    ch_printf(ch, "Warriors costs 10 silver. You have %d of them.\r\n", city->owarriors);
    ch_printf(ch, "You have %d silver in %s city bank.\r\n", bank->silver, city->name);
    send_to_char("Usage: siege <field> <amount>\r\n", ch);
    return;
  }

  if(is_in_siege(city))
  {
    send_to_char("You cannot add to your siege during a siege\r\n", ch);
    return;
  }

  if(!str_cmp(arg1, "ballista"))
  {
    if((city->ballista + count) > 6)
    {
      send_to_char("You can't have more then 6 ballistas.\r\n", ch);
      return;
    }
    ch_printf(ch, "You add to your city siege ability with %d ballista.\r\n", count);
    siegetype = 1;
    cost = (50 * count);
  }
  else if(!str_cmp(arg1, "ram"))
  {
    if((city->ram + count) > 2)
    {
      send_to_char("You can't have more then 2 rams.\r\n", ch);
      return;
    }
    ch_printf(ch, "You add to your city siege ability with %d ram.\r\n", count);
    siegetype = 2;
    cost = (40 * count);
  }
  else if(!str_cmp(arg1, "catapults"))
  {
    if((city->ocatapult + count) > 6)
    {
      send_to_char("You can't have more then 6 catapults.\r\n", ch);
      return;
    }
    siegetype = 3;
    ch_printf(ch, "You add to your city siege ability with %d catapults.\r\n", count);
    cost = (50 * count);
  }
  else if(!str_cmp(arg1, "soldiers"))
  {
    if((city->osoldiers + count) > 50)
    {
      send_to_char("You can't have more then 50 soldiers.\r\n", ch);
      return;
    }
    siegetype = 4;
    ch_printf(ch, "You add to your city siege ability with %d soldiers.\r\n", count);
    cost = (10 * count);
  }
  else if(!str_cmp(arg1, "archers"))
  {
    if((city->oarchers + count) > 50)
    {
      send_to_char("You can't have more then 50 archerss.\r\n", ch);
      return;
    }
    ch_printf(ch, "You add to your city siege ability with %d archers.\r\n", count);
    siegetype = 5;
    cost = (10 * count);
  }
  else if(!str_cmp(arg1, "warriors"))
  {
    if((city->owarriors + count) > 50)
    {
      send_to_char("You can't have more then 50 warriors.\r\n", ch);
      return;
    }
    siegetype = 6;
    ch_printf(ch, "You add to your city siege ability with %d warriors.\r\n", count);
    cost = (10 * count);
  }
  else if(!str_cmp(arg1, "platform"))
  {
    if((city->platform + count) > 6)
    {
      send_to_char("You can't have more then 6 wooden platforms.\r\n", ch);
      return;
    }
    siegetype = 7;
    ch_printf(ch, "You add to your city siege ability with %d wooden platform.\r\n", count);
    cost = (50 * count);
  }
  else
  {
    do_siege(ch, (char *)"");
    return;
  }

  if(bank->silver < cost)
  {
    ch_printf(ch, "Your city bank account can't afford %d %s.\r\n", count, arg1);
    return;
  }
  bank->silver -= cost;

  if(siegetype == 1)
    city->ballista += count;
  else if(siegetype == 2)
    city->ram += count;
  else if(siegetype == 3)
    city->ocatapult += count;
  else if(siegetype == 4)
    city->osoldiers += count;
  else if(siegetype == 5)
    city->oarchers += count;
  else if(siegetype == 6)
    city->owarriors += count;
  else if(siegetype == 7)
    city->platform += count;

  update_city_def_off(city);
  save_city(city);
}

// In addition to do_contribute, all citizens should have to contribute once a day via tax
void do_tax(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA              *victim;
  char                    arg1[MIL];
  char                    arg2[MIL];
  CITY_DATA              *city;
  BANK_DATA              *bank;
  int                     amount;
  int                     currtime = time(0);

  if(IS_NPC(ch) || !ch->pcdata->city)
  {
    send_to_char("Huh?\r\n", ch);
    return;
  }

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  city = get_city(ch->pcdata->city_name);
  if(!city)
  {
    send_to_char("No such city.\r\n", ch);
    return;
  }

  bank = find_bank(city->bank);

  if((city->duke && !str_cmp(ch->name, city->duke))
     || (city->baron && !str_cmp(ch->name, city->baron)) || (city->captain && !str_cmp(ch->name, city->captain)) || (city->sheriff && !str_cmp(ch->name, city->sheriff)));
  else
  {
    send_to_char("Huh?\r\n", ch);
    return;
  }

  if(!bank)
  {
    send_to_char("There is no account for that city!\r\n", ch);
    return;
  }

  if(!(victim = get_char_world(ch, arg1)))
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(IS_NPC(victim))
  {
    send_to_char("They are not a citizen.\r\n", ch);
    return;
  }

  if(!victim->pcdata->city)
  {
    send_to_char("They are not a citizen.\r\n", ch);
    return;
  }

  if(!VLD_STR(arg1))
  {
    send_to_char("Usage: tax <player>\r\n", ch);
    return;
  }

  if(str_cmp(ch->pcdata->city_name, victim->pcdata->city_name))
  {
    send_to_char("They are not a citizen of your city.\r\n", ch);
    return;
  }

  amount = 1;

  if((currtime - victim->pcdata->warn) < 3600)
  {
    send_to_char("Please wait at least one mud hour before attempting to collect taxes again.\r\n", ch);
    return;
  }

  if(amount > GET_MONEY(victim, CURR_SILVER))
  {
    victim->pcdata->warn = currtime;
    send_to_char("They don't have enough silver to pay their taxes.\r\n", ch);
    send_to_char("You don't have enough silver to pay your taxes.\r\nYou have one mud hour to come up with 1 silver piece for your taxes.", victim);
    return;
  }

  bank->silver += amount;
  GET_MONEY(victim, CURR_SILVER) -= amount;
  ch_printf(ch, "\r\n&cYou collect %d silver from %s to add to the city bank.\r\n", amount, victim->name);
  ch_printf(victim, "\r\n&cYou pay %d silver for your taxes to the city bank.\r\n", amount);
  save_char_obj(victim);
  save_bank();

// tax should be 1 silver daily, on a 3 strike system.  
// If you don't have 1 silver after 3 strikes, lose citizenship.
}

void do_contribute(CHAR_DATA *ch, char *argument)
{
  char                    arg1[MIL];
  CITY_DATA              *city;
  BANK_DATA              *bank;
  int                     amount;

  if(IS_NPC(ch) || !ch->pcdata->city)
  {
    send_to_char("Huh?\r\n", ch);
    return;
  }

  argument = one_argument(argument, arg1);
  city = get_city(ch->pcdata->city_name);
  if(!city)
  {
    send_to_char("No such city.\r\n", ch);
    return;
  }

  bank = find_bank(city->bank);

  if(!bank)
  {
    send_to_char("There is no account for that city!\r\n", ch);
    return;
  }

  if(arg1[0] == '\0')
  {
    send_to_char("Usage: contribute <amount>\r\n", ch);
    return;
  }

  amount = atoi(arg1);

  if(amount > GET_MONEY(ch, CURR_SILVER))
  {
    send_to_char("You don't have enough silver to contribute that much.\r\n", ch);
    return;
  }

  bank->silver += amount;
  GET_MONEY(ch, CURR_SILVER) -= amount;
  ch_printf(ch, "\r\n&cYou contribute %d silver to the city bank.\r\n", amount);
  save_char_obj(ch);
  save_bank();
}

void do_setenemy(CHAR_DATA *ch, char *argument)
{
  char                    arg1[MIL];
  char                    arg2[MIL];
  CITY_DATA              *city;

  if(IS_NPC(ch) || !ch->pcdata->city)
  {
    send_to_char("Huh?\r\n", ch);
    return;
  }
  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  if(!VLD_STR(arg1))
  {
    send_to_char("Usage: setenemy <field> <player>\r\n", ch);
    send_to_char("\r\nField being one of:\r\n", ch);
    send_to_char(" race class player city clan\r\n", ch);
    return;
  }
  city = get_city(ch->pcdata->city_name);
  if(!city)
  {
    send_to_char("No such city.\r\n", ch);
    return;
  }

  if(!str_cmp(arg1, "list"))
  {
  }
}

/* Done so we can just loop through and save lots of code */
int get_city_room(CITY_DATA * city, short type)
{
  int                     uroom = 0;

  if(!city)
    return uroom;

  if(type == 1)
    uroom = city->defend1_room;
  if(type == 2)
    uroom = city->defend2_room;
  if(type == 3)
    uroom = city->defend3_room;
  if(type == 4)
    uroom = city->siege1_room;
  if(type == 5)
    uroom = city->siege2_room;
  if(type == 6)
    uroom = city->siege3_room;

  return uroom;
}

/* Count up all the defenders */
int count_defenders(SIEGE_DATA * siege)
{
  CITY_DATA              *city;
  ROOM_INDEX_DATA        *location;
  OBJ_DATA               *obj;
  CHAR_DATA              *rch;
  int                     defenders = 0, oncount, roomnum;

  if(!siege)
    return 0;

  /*
   * Battles are always in defending city rooms 
   */
  city = siege->dcity;
  if(!city)
    return 0;

  for(oncount = 1; oncount <= 3; oncount++)
  {
    roomnum = get_city_room(city, oncount);
    location = get_room_index(roomnum);

    if(!location)
      bug("%s: couldn't find room %d.", __FUNCTION__, roomnum);
    else
    {
      for(obj = location->first_content; obj; obj = obj->next_content)
      {
        if(!obj->siege || !obj->city)
          continue;
        if(obj->item_type != ITEM_SABOTAGE)
          continue;
        if(obj->city != city || obj->siege != siege)
          continue;
        if(obj->pIndexData->vnum == city->pitch_obj)
        defenders += (obj->count * 10);
        if(obj->pIndexData->vnum == city->arrow_obj)
        defenders += (obj->count * 1);
        if(obj->pIndexData->vnum == city->catapult_obj)
        defenders += (obj->count * 5);
      }

      for(rch = location->first_person; rch; rch = rch->next_in_room)
      {
        if(!IS_NPC(rch))
          continue;
        if(!rch->city || !rch->siege)
          continue;
        if(rch->pIndexData->vnum != city->guard_mob && rch->pIndexData->vnum != city->archer_mob && rch->pIndexData->vnum != city->warrior_mob)
          continue;
        if(rch->city != city || rch->siege != siege)
          continue;
        defenders += 2;
      }
    }
  }

  return defenders;
}

/* Count up the attackers */
int count_attackers(SIEGE_DATA * siege)
{
  CITY_DATA              *city, *acity;
  ROOM_INDEX_DATA        *location;
  OBJ_DATA               *obj;
  CHAR_DATA              *rch;
  int                     attackers = 0, oncount, roomnum;

  if(!siege)
    return 0;
  city = siege->dcity;
  acity = siege->acity;
  if(!city || !acity)
    return 0;

  for(oncount = 4; oncount <= 6; oncount++)
  {
    roomnum = get_city_room(city, oncount);
    location = get_room_index(roomnum);

    if(!location)
      bug("%s: couldn't find room %d.", __FUNCTION__, roomnum);
    else
    {
      for(obj = location->first_content; obj; obj = obj->next_content)
      {
        if(!obj->city || !obj->siege)
          continue;
        if(obj->item_type != ITEM_SABOTAGE)
          continue;
        if(obj->city != acity || obj->siege != siege)
          continue;
        /*
         * Make the stuff worth what it is worth for the offensive value 
         */
        if(obj->pIndexData->vnum == acity->ballista_obj)
          attackers += (obj->count * 10);
        if(obj->pIndexData->vnum == acity->ram_obj)
          attackers += (obj->count * 25);
        if(obj->pIndexData->vnum == acity->catapult_obj)
          attackers += (obj->count * 10);
        if(obj->pIndexData->vnum == acity->platform_obj)
          attackers += (obj->count * 15);
      }

      for(rch = location->first_person; rch; rch = rch->next_in_room)
      {
        if(!IS_NPC(rch))
          continue;
        if(!rch->city || !rch->siege)
          continue;
        if(rch->pIndexData->vnum != acity->soldier_mob && rch->pIndexData->vnum != acity->archer_mob && rch->pIndexData->vnum != acity->warrior_mob)
          continue;
        if(rch->city != acity || rch->siege != siege)
          continue;

        /*
         * Since soldiers are 2 off the offense lets list it for the offensive
         * value 
         */
        attackers += 2;
      }
    }
  }

  return attackers;
}

/* Put what should happen when the siege ends in here */
void end_siege(SIEGE_DATA * siege)
{
  CITY_DATA              *city = NULL, *acity = NULL;
  OBJ_DATA               *obj, *obj_next;
  CHAR_DATA              *rch, *rch_next, *wch;
  ROOM_INDEX_DATA        *location = NULL;
  char                    buf[MSL];
  int                     attackers = 0, defenders = 0, oncount, roomnum, ovnum, track;

  if(!siege)
    return;

  city = siege->dcity;
  acity = siege->acity;
  if(!city || !acity)
    return;

  /*
   * Attackers and Defenders found for this siege in the city being attacked (Count
   * before removing of course) 
   */
  attackers = count_attackers(siege);
  defenders = count_defenders(siege);

  /*
   * Now lets remove all we should and give back what we remove 
   */
  for(oncount = 1; oncount <= 6; oncount++)
  {
    roomnum = get_city_room(city, oncount);
    location = get_room_index(roomnum);

    if(!location)
    {
      bug("%s: No location for %d.", __FUNCTION__, roomnum);
      continue;
    }

    REMOVE_BIT(location->room_flags, ROOM_ARENA);

    for(obj = location->first_content; obj; obj = obj_next)
    {
      obj_next = obj->next_content;

      if(!obj->whichcity || !obj->siege || obj->siege != siege)
        continue;

      /*
       * Put back the counts of whats left 
       */
      ovnum = obj->pIndexData->vnum;
      if(obj->whichcity == city->name)
      {
        if(ovnum == city->catapult_obj)
          city->dcatapult += obj->count;
        if(ovnum == city->pitch_obj)
          city->pitch += obj->count;
        if(ovnum == city->arrow_obj)
          city->arrows += obj->count;
      }
      else if(obj->whichcity == acity->name)
      {
        if(ovnum == acity->catapult_obj)
          acity->ocatapult += obj->count;
        if(ovnum == acity->ballista_obj)
          acity->ballista += obj->count;
        if(ovnum == acity->platform_obj)
          acity->platform += obj->count;
        if(ovnum == acity->ram_obj)
          acity->ram += obj->count;
      }
      else
        continue;

      obj_from_room(obj);
      extract_obj(obj);
    }

    track = 0;
    for(rch = location->first_person; rch; rch = rch_next)
    {
      rch_next = rch->next_in_room;

      if(!IS_NPC(rch))
        continue;

      if(!rch->city || !rch->siege || rch->siege != siege)
        continue;

      ovnum = rch->pIndexData->vnum;
      if(rch->city == city)
      {
        if(ovnum == city->archer_mob)
          city->darchers++;
        if(ovnum == city->guard_mob)
          city->dguards++;
        if(ovnum == city->warrior_mob)
          city->dwarriors++;
      }
      else if(rch->city == acity)
      {
        if(ovnum == acity->archer_mob)
          acity->oarchers++;
        if(ovnum == acity->soldier_mob)
          acity->osoldiers++;
        if(ovnum == acity->warrior_mob)
          acity->owarriors++;
      }
      else
        continue;

      extract_char(rch, TRUE);
    }
  }

  /*
   * Update the stuff for both cities 
   */
  update_city_def_off(acity);
  update_city_def_off(city);

  if(attackers > defenders)
  {
    sprintf(buf, "&C%s has defeated %s and stormed their city walls!", acity->name, city->name);
    announce(buf);
    acity->wins += 1;
    city->loses += 1;

    // declare attackers winners
    for(wch = first_char; wch; wch = wch->next)
    {
      if(IS_NPC(wch) || !IS_CITY(wch) || IS_IMMORTAL(wch))
        continue;
      if(VLD_STR(wch->pcdata->city_name) && !str_cmp(wch->pcdata->city_name, acity->name))
      {
        send_to_char("&cYou gain &W10&c Glory for winning the siege!\r\n", wch);
        wch->quest_curr += 10;
      }
    }
  }
  else
  {
    sprintf(buf, "&C%s has defeated %s and protected their city walls!", city->name, acity->name);
    announce(buf);
    city->wins += 1;
    acity->loses += 1;

    for(wch = first_char; wch; wch = wch->next)
    {
      if(IS_NPC(wch) || !IS_CITY(wch) || IS_IMMORTAL(wch))
        continue;
      if(VLD_STR(wch->pcdata->city_name) && !str_cmp(wch->pcdata->city_name, city->name))
      {
        send_to_char("&cYou gain &W10&c Glory for winning the siege!\r\n", wch);
        wch->quest_curr += 10;
      }
    }
  }

  save_city(city);
  save_city(acity);
}

bool should_end_siege(SIEGE_DATA * siege)
{
  if(count_defenders(siege) <= 0)
    return TRUE;
  else if(count_attackers(siege) <= 0)
    return TRUE;

  return FALSE;
}

time_t                  last_siege_check = 0;

void handle_sieges(void)
{
  SIEGE_DATA             *siege, *siege_next;
  char                    buf[MSL];
  int                     defense, offense;

  if(!last_siege_check)
    last_siege_check = (current_time - 1);

  if(last_siege_check >= current_time)
    return;

  for(siege = first_siege; siege; siege = siege_next)
  {
    siege_next = siege->next;

    if(should_end_siege(siege))
    {
      end_siege(siege);
      UNLINK(siege, first_siege, last_siege, next, prev);
      siege->acity = NULL;
      siege->dcity = NULL;
      DISPOSE(siege);
      continue;
    }

    offense = (count_attackers(siege) + siege->acity->offense);
    defense = (count_defenders(siege) + siege->dcity->defense);

// This function needs to be altered for full war so it has one announcement if multiple cities

    if(((siege->siege_started + 1800) - current_time) == 1200)
    {
      snprintf(buf, sizeof(buf), "%s", "&CWith 20 minutes left, ");
      if(offense > defense)
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "%s is showing signs of falling to %s!", siege->dcity->name, siege->acity->name);
      else
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "%s is showing signs of holding off %s!", siege->dcity->name, siege->acity->name);
      announce(buf);
    }
    else if(((siege->siege_started + 1800) - current_time) == 600)
    {
      snprintf(buf, sizeof(buf), "%s", "&CWith 10 minutes left, ");
      if(offense > defense)
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "%s is showing signs of falling to %s!", siege->dcity->name, siege->acity->name);
      else
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "%s is showing signs of holding off %s!", siege->dcity->name, siege->acity->name);
      announce(buf);
    }
    else if(((siege->siege_started + 1800) - current_time) == 180)
    {
      snprintf(buf, sizeof(buf), "%s", "&CWith 3 minutes left, ");
      if(offense > defense)
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "%s is showing signs of falling to %s!", siege->dcity->name, siege->acity->name);
      else
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "%s is showing signs of holding off %s!", siege->dcity->name, siege->acity->name);
      announce(buf);
    }

    if(siege->siege_started != 0 && current_time > (siege->siege_started + 1800))
    {
      end_siege(siege);
      UNLINK(siege, first_siege, last_siege, next, prev);
      siege->acity = NULL;
      siege->dcity = NULL;
      DISPOSE(siege);
    }
  }

  last_siege_check = current_time;
}

bool can_siege(CITY_DATA * city, char *name)
{
  if(city)
    return TRUE;
  return FALSE;
}

void start_siege(SIEGE_DATA * siege, CITY_DATA * ocity, CITY_DATA * dcity)
{
  char                    long_buf[MSL], name_buf[MSL], short_buf[MSL];
  SIEGE_DATA             *csiege;
  CITY_DATA              *city;
  ROOM_INDEX_DATA        *location;
  OBJ_INDEX_DATA         *oindex;
  OBJ_DATA               *obj;
  CHAR_DATA              *mob;
  int                     oncount, roomnum, count, vnum, created, citycount, defcount = 0, offcount = 0;
  bool                    usealldef = FALSE, usealloff = FALSE;

  long_buf[0] = '\0';

  /*
   * Count all the cities and subtract 1 ( since one is the one defending 
   */
  citycount = 0;
  for(city = first_city; city; city = city->next)
    citycount++;  /* Count the cities */
  citycount--;  /* Take off one */

  /*
   * Lets see if they are already being used in the max sieges 
   */
  for(csiege = first_siege; csiege; csiege = csiege->next)
  {
    if(csiege->dcity == dcity)
      defcount++;
    if(csiege->acity == ocity)
      offcount++;
  }
  if(defcount == citycount)
    usealldef = TRUE;
  if(offcount == citycount)
    usealloff = TRUE;

  for(oncount = 1; oncount <= 6; oncount++)
  {
    roomnum = get_city_room(dcity, oncount);
    location = get_room_index(roomnum);
    if(!location)
    {
      bug("%s: location %d is NULL.", __FUNCTION__, roomnum);
      continue;
    }

    SET_BIT(location->room_flags, ROOM_ARENA);

    /*
     * 3 and 5 have archers 
     */
    if(oncount == 3 || oncount == 5)
    {
      if(oncount == 3)
      {
        count = dcity->darchers;
        if ( count < 0 )
           count = number_range(1, 4);  // so don't have sieges with nothing to do
        if(count > 0)
        {
          if(!usealldef)
            count = (count / citycount);
          dcity->darchers -= count;
        }
        vnum = dcity->archer_mob;
      }
      else
      {
        count = ocity->oarchers;
       	if ( count < 0 )
       	   count = number_range(1, 4);  // so don't have	sieges with nothing to do

        if(count > 0)
        {
          if(!usealloff)
            count = (count / citycount);
          ocity->oarchers -= count;
        }
        vnum = ocity->archer_mob;
      }

      if(count > 0)
      {
        for(created = 0; created < count; created++)
        {
          mob = create_mobile(get_mob_index(vnum));

          mob->color = 1;
          mob->siege = siege;
          if(oncount == 3)
            mob->city = dcity;
          else
            mob->city = ocity;
/* Ok so trying to add city names here to the mobs so confused players don't attack their own city defenses - Vladaar */
          sprintf(long_buf, "A %s archer is standing here.", strlower(mob->city->name));
          STRFREE(mob->long_descr);
          mob->long_descr = STRALLOC(long_buf);
          char_to_room(mob, location);
        }
      }
    }

    /*
     * 2, 3, 4, 6 have soldiers 
     */
    if(oncount == 2 || oncount == 1 || oncount == 4 || oncount == 6)
    {
      if(oncount == 2)
      {
        count = dcity->dguards;
       	if ( count < 0 )
       	   count = number_range(1, 4);  // so don't have	sieges with nothing to do

        if(count > 0)
        {
          if(!usealldef)
            count = (count / citycount);
          dcity->dguards -= count;
        }
        vnum = dcity->guard_mob;
      }
      else if(oncount == 1)
      {
        count = dcity->dwarriors;
       	if ( count < 0 )
       	   count = number_range(1, 4);  // so don't have	sieges with nothing to do

        if(count > 0)
        {
          if(!usealldef)
            count = (count / citycount);
          dcity->dwarriors -= count;
        }
        vnum = dcity->warrior_mob;
      }
      else if(oncount == 4)
      {
       	if ( count < 0 )
       	   count = number_range(1, 4);  // so don't have	sieges with nothing to do

        count = ocity->owarriors;
        if(count > 0)
        {
          if(!usealloff)
            count = (count / citycount);
          ocity->owarriors -= count;
        }
        vnum = ocity->warrior_mob;
      }
      else
      {
        count = ocity->osoldiers;
       	if ( count < 0 )
       	   count = number_range(1, 4);  // so don't have	sieges with nothing to do

        if(count > 0)
        {
          if(!usealloff)
            count = (count / citycount);
          ocity->osoldiers -= count;
        }
        vnum = ocity->soldier_mob;
      }

      if(count > 0)
      {
        for(created = 0; created < count; created++)
        {
          mob = create_mobile(get_mob_index(vnum));
          mob->color = 1;
          mob->siege = siege;

          if(oncount == 2 || oncount == 1)
            mob->city = dcity;
          else
            mob->city = ocity;
/* Ok so trying to add city names here to the mobs so confused players don't attack their own city defenses - Vladaar */
          if(mob->pIndexData->vnum == 41038)
          {
            sprintf(long_buf, "A %s soldier is standing here.", strlower(mob->city->name));
            sprintf(name_buf, "soldier %s", strlower(mob->city->name));
            sprintf(short_buf, "a %s soldier", strlower(mob->city->name));
          }
          else if(mob->pIndexData->vnum == 41091)
          {
            sprintf(long_buf, "A %s warrior is standing here.", strlower(mob->city->name));
            sprintf(name_buf, "warrior %s", strlower(mob->city->name));
            sprintf(short_buf, "a %s warrior", strlower(mob->city->name));
          }
          else if(mob->pIndexData->vnum == 41092)
          {
            sprintf(long_buf, "A %s guard is standing here.", strlower(mob->city->name));
            sprintf(name_buf, "guard %s", strlower(mob->city->name));
            sprintf(short_buf, "a %s guard", strlower(mob->city->name));
          }
          STRFREE(mob->long_descr);
          mob->long_descr = STRALLOC(long_buf);
          STRFREE(mob->name);
          mob->name = STRALLOC(name_buf);
          STRFREE(mob->short_descr);
          mob->short_descr = STRALLOC(short_buf);
          char_to_room(mob, location);
        }
      }
    }

    /*
     * All of them have at least one object 
     */
    vnum = 0;
    if(oncount == 1)
    {
      count = dcity->dcatapult;
      if(count > 0)
      {
        if(!usealldef)
          count = (count / citycount);
        dcity->dcatapult -= count;
      }
      vnum = dcity->catapult_obj;
    }
    else if(oncount == 2)
    {
      count = dcity->pitch;
      if(count > 0)
      {
        if(!usealldef)
          count = (count / citycount);
        dcity->pitch -= count;
      }
      vnum = dcity->pitch_obj;
    }
    else if(oncount == 3)
    {
      count = dcity->arrows;
      if(count > 0)
      {
        if(!usealldef)
          count = (count / citycount);
        dcity->arrows -= count;
      }
      vnum = dcity->arrow_obj;
    }
    else if(oncount == 4)
    {
      count = ocity->ocatapult;
      if(count > 0)
      {
        if(!usealloff)
          count = (count / citycount);
        ocity->ocatapult -= count;
      }
      vnum = ocity->catapult_obj;
    }
    else if(oncount == 5)
    {
      count = ocity->ballista;
      if(count > 0)
      {
        if(!usealloff)
          count = (count / citycount);
        ocity->ballista -= count;
      }
      vnum = ocity->ballista_obj;
    }
    else if(oncount == 6)
    {
      count = ocity->platform;
      if(count > 0)
      {
        if(!usealloff)
          count = (count / citycount);
        ocity->platform -= count;
      }
      vnum = ocity->platform_obj;
    }

    obj = NULL;
    if(count > 0)
    {
      if(!(oindex = get_obj_index(vnum)))
        bug("%s: couldn't find object index for %d.", __FUNCTION__, vnum);
      else if(!(obj = create_object(oindex, 1)))
        bug("%s: Couldn't create catapults for %d.", __FUNCTION__, vnum);
      else
      {
        obj->color = 1;
        obj->count = count;
        obj->pIndexData->count += (count - 1);
        numobjsloaded += (count - 1);
        obj->siege = siege;
        if(oncount == 1 || oncount == 2 || oncount == 3)
        {
          obj->city = dcity;
        }
        else
        {
          obj->city = ocity;
        }

        obj_to_room(obj, location);
        if(!str_cmp(dcity->name, "Paleon City"))
        {
          if(obj->whichcity != NULL)
            STRFREE(obj->whichcity);
          obj->whichcity = STRALLOC(obj->city->name);
        }
        else if(!str_cmp(dcity->name, "Dakar City"))
        {
          if(obj->whichcity != NULL)
            STRFREE(obj->whichcity);
          obj->whichcity = STRALLOC(obj->city->name);
        }
        else if(!str_cmp(dcity->name, "Forbidden City"))
        {
          if(obj->whichcity != NULL)
            STRFREE(obj->whichcity);
          obj->whichcity = STRALLOC(obj->city->name);
        }
        if(!str_cmp(ocity->name, "Paleon City"))
        {
          if(obj->whichcity != NULL)
            STRFREE(obj->whichcity);
          obj->whichcity = STRALLOC(obj->city->name);
        }
        else if(!str_cmp(ocity->name, "Dakar City"))
        {
          if(obj->whichcity != NULL)
            STRFREE(obj->whichcity);
          obj->whichcity = STRALLOC(obj->city->name);
        }
        else if(!str_cmp(ocity->name, "Forbidden City"))
        {
          if(obj->whichcity != NULL)
            STRFREE(obj->whichcity);
          obj->whichcity = STRALLOC(obj->city->name);
        }

      }
    }

    /*
     * 5 has the rams also 
     */
    if(oncount == 5)
    {
      count = ocity->ram;
      if(count > 0)
      {
        if(!usealloff)
          count = (count / citycount);
        ocity->ram -= count;
      }

      if(count > 0)
      {
        obj = create_object(get_obj_index(ocity->ram_obj), 1);
        if(!obj)
          bug("%s: Couldn't create rams.", __FUNCTION__);
        else
        {
          obj->color = 1;
          obj->count = count;
          obj->pIndexData->count += (count - 1);
          numobjsloaded += (count - 1);
          obj->siege = siege;
          obj->city = ocity;
          if(!str_cmp(ocity->name, "Paleon City"))
          {
            if(obj->whichcity != NULL)
              STRFREE(obj->whichcity);
            obj->whichcity = STRALLOC(obj->city->name);
          }
          else if(!str_cmp(ocity->name, "Dakar City"))
          {
            if(obj->whichcity != NULL)
              STRFREE(obj->whichcity);
            obj->whichcity = STRALLOC(obj->city->name);
          }
          else if(!str_cmp(ocity->name, "Forbidden City"))
          {
            if(obj->whichcity != NULL)
              STRFREE(obj->whichcity);
            obj->whichcity = STRALLOC(obj->city->name);
          }

          obj_to_room(obj, location);
          if(!obj->city)
            obj->city = dcity;

        }
      }
    }
  }

  /*
   * Update the stuff for both cities 
   */
  update_city_def_off(ocity);
  update_city_def_off(dcity);
}

void do_laysiege(CHAR_DATA *ch, char *argument)
{
  SIEGE_DATA             *siege = NULL;
  CITY_DATA              *ocity, *dcity;
  char                    arg1[MIL], buf[MSL];

  if(IS_NPC(ch))
  {
    send_to_char("Huh ?\r\n", ch);
    return;
  }

  if(!IS_IMMORTAL(ch) && ch->pcdata->next_siege > current_time)
  {
    int                     utime, umins = 0, uhours = 0;

    utime = (ch->pcdata->next_siege - current_time);
    while(utime > 3600)
    {
      utime -= 3600;
      uhours++;
    }
    while(utime > 60)
    {
      utime -= 60;
      umins++;
    }

    send_to_char("You can't laysiege on anything currently, try back in", ch);
    if(uhours > 0)
      ch_printf(ch, " %d hour%s", uhours, uhours == 1 ? "" : "s");
    if(umins > 0)
      ch_printf(ch, " %d minute%s", umins, umins == 1 ? "" : "s");
    if(utime > 0)
      ch_printf(ch, " %d second%s", utime, utime == 1 ? "" : "s");
    send_to_char("\r\n", ch);
    return;
  }

  argument = one_argument(argument, arg1);

  if(!VLD_STR(arg1))
  {
    send_to_char("Usage: laysiege <city>\r\n", ch);
    if(IS_IMMORTAL(ch))
    {
      send_to_char("Option Fullwar\r\n", ch);
    }
    return;
  }

  if(get_timer(ch, TIMER_RECENTFIGHT) > 0 && !IS_IMMORTAL(ch))
  {
    set_char_color(AT_RED, ch);
    send_to_char("Your adrenaline is pumping too hard right now!\r\n", ch);
    return;
  }

  /*
   * Lets allow a way to start a full out war 
   */
  if(IS_IMMORTAL(ch) && !str_cmp(arg1, "fullwar"))
  {
    for(ocity = first_city; ocity; ocity = ocity->next)
    {
      for(dcity = first_city; dcity; dcity = dcity->next)
      {
        if(ocity == dcity)
          continue;
        if(is_sieging(ocity, dcity))
          continue;
        siege = add_siege(ocity, dcity);
        if(!siege)
        {
          ch_printf(ch, "Failed to create a siege for %s attacking %s.\r\n", ocity->name, dcity->name);
          continue;
        }

        sprintf(buf, "&C%s has started a siege against %s!", ocity->name, dcity->name);
        announce(buf);
        start_siege(siege, ocity, dcity);
      }
    }
    return;
  }

  if(IS_IMMORTAL(ch) && VLD_STR(argument))
    ocity = get_city(argument);
  else
    ocity = get_city(ch->pcdata->city_name);
  dcity = get_city(arg1);

  if(!ocity)
  {
    if(!IS_IMMORTAL(ch) || !VLD_STR(argument))
      send_to_char("You aren't even a citizen of a city.\r\n", ch);
    else
      ch_printf(ch, "No city called %s.\r\n", argument);
    return;
  }

  if(!dcity)
  {
    send_to_char("No such city.\r\n", ch);
    return;
  }

  if(!IS_IMMORTAL(ch) && !can_siege(ocity, ch->name))
  {
    send_to_char("Huh?\r\n", ch);
    return;
  }

  if(ocity == dcity)
  {
    ch_printf(ch, "Why would %s want to laysiege against itself?\r\n", ocity->name);
    return;
  }

  if(!IS_IMMORTAL(ch) && ch->in_room->vnum != dcity->siege1_room)
  {
    send_to_char("You are not at the siege location to laysiege on that city.\r\n", ch);
    return;
  }

  if(is_sieging(ocity, dcity))
  {
    ch_printf(ch, "%s is already attacking %s.\r\n", ocity->name, dcity->name);
    return;
  }

  siege = add_siege(ocity, dcity);
  if(!siege)
  {
    send_to_char("Failed to create a new siege to use\r\n", ch);
    return;
  }

  sprintf(buf, "&C%s has started a siege against %s!", ocity->name, dcity->name);
  announce(buf);
  ch->pcdata->next_siege = (current_time + 7200);

  start_siege(siege, ocity, dcity);
}

void do_proclaim(CHAR_DATA *ch, char *argument)
{
  CITY_DATA              *city;

  if(IS_NPC(ch))
  {
    send_to_char("Huh ?\r\n", ch);
    return;
  }

  if(!VLD_STR(argument))
  {
    send_to_char("Usage: proclaim <message>\r\n", ch);
    return;
  }

  city = get_city(ch->pcdata->city_name);

  if(!city)
  {
    send_to_char("You aren't even a citizen of a city.\r\n", ch);
    return;
  }

  proc_chan(ch, argument);
}

/* If the ch is way higher than the soldier reset it */
void set_soldier(CHAR_DATA *soldier, CHAR_DATA *ch, bool archer)
{
  if(!soldier || !ch)
    return;

  if(!IS_NPC(soldier) || !soldier->city || !soldier->siege)
    return;

  /*
   * It is already higher than where we would set it so leave it alone 
   */
  if(soldier->level > ch->level)
    return;

  if(!archer)
    soldier->level = ((soldier->city->onlinechars > 0) ? ch->level - 5 : (ch->level));
  else
    soldier->level = ((soldier->city->onlinechars > 0) ? (ch->level - 5) : ch->level);
  soldier->max_hit = set_hp(soldier->level);
  soldier->hit = soldier->max_hit;
  soldier->armor = set_armor_class(soldier->level);
  soldier->hitroll = set_hitroll(soldier->level);
  soldier->damroll = set_damroll(soldier->level);
  soldier->numattacks = set_num_attacks(soldier->level);
  soldier->hitplus = set_hp(soldier->level);
  soldier->soldierset = TRUE;
}

void do_sabotage(CHAR_DATA *ch, char *argument)
{
  CITY_DATA              *city = NULL, *ucity;
  OBJ_DATA               *obj;
  CHAR_DATA              *soldier = NULL;
  int                     ovnum;
  short                   chance = number_range(1, 10);
  short                   rand;

  if(!ch || IS_NPC(ch))
    return;

  for(obj = ch->in_room->first_content; obj; obj = obj->next_content)
  {
    if(obj->item_type == ITEM_SABOTAGE)
      break;
  }

  if(!obj || obj->item_type != ITEM_SABOTAGE)
  {
    send_to_char("Nothing here to sabotage.\r\n", ch);
    return;
  }

  if(!VLD_STR(ch->pcdata->city_name) || !(ucity = get_city(ch->pcdata->city_name)))
  {
    send_to_char("You can't sabotage anything since you don't belong to a city.\r\n", ch);
    return;
  }

  separate_obj(obj);
  ovnum = obj->pIndexData->vnum;

  city = get_city(obj->whichcity);

  if(!city)
  {
    bug("%s: city is NULL for object vnum %d.", __FUNCTION__, ovnum);
    return;
  }

  /*
   * Good idea to not let them sabotage their own city's stuff 
   */
  if(city == ucity)
  {
    send_to_char("Why would you want to damage the stuff from your city?\r\n", ch);
    return;
  }

  WAIT_STATE(ch, 20);
  if(city->hardened > 0)
  {
    chance = 1;
    city->hardened -= 1;
    save_city(city);
  }
  for(soldier = ch->in_room->first_person; soldier; soldier = soldier->next_in_room)
  {
    if(!IS_NPC(soldier))
      continue;

    if(!soldier->city || !soldier->siege)
      continue;

    if(soldier->pIndexData->vnum == city->soldier_mob)
    {
      set_soldier(soldier, ch, FALSE);
      break;
    }
    if(soldier->pIndexData->vnum == city->warrior_mob)
    {
      set_soldier(soldier, ch, FALSE);
      break;
    }
    if(soldier->pIndexData->vnum == city->guard_mob)
    {
      set_soldier(soldier, ch, FALSE);
      break;
    }
    if(soldier->pIndexData->vnum == city->archer_mob)
    {
      set_soldier(soldier, ch, TRUE);
      break;
    }
  }

  if(chance < 5)
  {
    ch_printf(ch, "&cYou attempt to sabotage %s, but fail to complete your task.\r\n", obj->short_descr);
    if(soldier)
    {
      interpret(soldier, (char *)"mpecho");
      if(soldier->pIndexData->vnum == city->archer_mob)
        interpret(soldier, (char *)"mpecho &cA shout is taken up as an archer observes the sabotage attempt!&D");
      else
        interpret(soldier, (char *)"mpecho &cA shout is taken up as a soldier observes the sabotage attempt!&D");
      interpret(soldier, (char *)"mpecho");
      if(!soldier->fighting)
        set_fighting(soldier, ch);
    }
    return;
  }
  ch_printf(ch, "&cYou attempt to sabotage %s.\r\nYou successfully destroyed %s.\r\n", obj->short_descr, obj->short_descr);
  if(soldier)
  {
    interpret(soldier, (char *)"mpecho");
    if(soldier->pIndexData->vnum == city->archer_mob)
      interpret(soldier, (char *)"mpecho &cA shout is taken up as an archer observes the sabotage act!&D");
    else
      interpret(soldier, (char *)"mpecho &cA shout is taken up as a soldier observes the sabotage act!&D");
    interpret(soldier, (char *)"mpecho");
    if(!soldier->fighting)
      set_fighting(soldier, ch);
  }

  rand = number_range(1, 10);
  if(rand > 6)
  {
    send_to_char("&WYou gain one glory for sabotaging the enemy!\r\n", ch);
    ch->quest_curr += 1;
  }
  else
    gain_exp(ch, ch->level * 1500);

  returntocity(obj);
  extract_obj(obj);
  update_city_def_off(city);
  save_city(city);
}

// City Rosters?

void add_rollcall(CITY_DATA * city, char *name, int Class, int level, int kills, int deaths)
{
  ROLLCALL_DATA          *rollcall;

  CREATE(rollcall, ROLLCALL_DATA, 1);
  rollcall->name = STRALLOC(name);
  rollcall->Class = Class;
  rollcall->level = level;
  rollcall->kills = kills;
  rollcall->deaths = deaths;
  rollcall->joined = current_time;
  rollcall->lastupdated = current_time;
  insert_rollcall(city, rollcall);
}

void remove_rollcall(CITY_DATA * city, char *name)
{
  ROLLCALL_DATA          *rollcall, *roll_next;

  if(!city || !name || name[0] == '\0')
    return;

  if(VLD_STR(city->duke) && !str_cmp(city->duke, name))
    STRFREE(city->duke);
  if(VLD_STR(city->baron) && !str_cmp(city->baron, name))
    STRFREE(city->baron);
  if(VLD_STR(city->captain) && !str_cmp(city->captain, name))
    STRFREE(city->captain);
  if(VLD_STR(city->sheriff) && !str_cmp(city->sheriff, name))
    STRFREE(city->sheriff);
  if(VLD_STR(city->knight) && !str_cmp(city->knight, name))
    STRFREE(city->knight);

  for(rollcall = city->first_citizen; rollcall; rollcall = roll_next)
  {
    roll_next = rollcall->next;

    if(!str_cmp(name, rollcall->name))
    {
      STRFREE(rollcall->name);
      UNLINK(rollcall, city->first_citizen, city->last_citizen, next, prev);
      DISPOSE(rollcall);
    }
  }

  fix_city_order(city);
}

/* Remove this one from any and all roll calls */
void remove_from_rollcalls(char *name)
{
  CITY_DATA              *city;

  if(!VLD_STR(name))
    return;

  for(city = first_city; city; city = city->next)
  {
    remove_rollcall(city, name);
    save_city(city);
  }
}

void update_rollcall(CHAR_DATA *ch)
{
  CITY_DATA              *city;
  ROLLCALL_DATA          *rollcall;

  if(!ch || !ch->pcdata || !(city = ch->pcdata->city))
    return;

  for(rollcall = ch->pcdata->city->first_citizen; rollcall; rollcall = rollcall->next)
  {
    if(!str_cmp(ch->name, rollcall->name))
    {
      rollcall->level = ch->level;
      rollcall->kills = ch->pcdata->mkills;
      rollcall->deaths = ch->pcdata->mdeaths;
      rollcall->lastupdated = current_time;
      UNLINK(rollcall, city->first_citizen, city->last_citizen, next, prev);
      insert_rollcall(city, rollcall);
      save_city(ch->pcdata->city);
      return;
    }
  }

  /*
   * If we make it here, assume they haven't been added previously 
   */
  add_rollcall(ch->pcdata->city, ch->name, ch->Class, ch->level, ch->pcdata->mkills, ch->pcdata->mdeaths);
  save_city(ch->pcdata->city);
}

/* For use during city removal and memory cleanup */
void remove_all_rollcalls(CITY_DATA * city)
{
  ROLLCALL_DATA          *rollcall, *rollcall_next;

  for(rollcall = city->first_citizen; rollcall; rollcall = rollcall_next)
  {
    rollcall_next = rollcall->next;

    STRFREE(rollcall->name);
    UNLINK(rollcall, city->first_citizen, city->last_citizen, next, prev);
    DISPOSE(rollcall);
  }
}

void do_rollcall(CHAR_DATA *ch, char *argument)
{
  CITY_DATA              *city;
  ROLLCALL_DATA          *rollcall;
  char                    arg[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];
  int                     total = 0;

  if(IS_NPC(ch))
  {
    send_to_char("NPCs can't use this command.\r\n", ch);
    return;
  }

  if(!argument || argument[0] == '\0')
  {
    send_to_char("Usage: rollcall <cityname>\r\n", ch);
    if(IS_IMMORTAL(ch) || IS_DUKE(ch) || IS_BARON(ch))
      send_to_char("Usage: rollcall <cityname> remove <name>\r\n", ch);
    return;
  }

  argument = one_argument(argument, arg);
  if(!(city = get_city(arg)))
  {
    ch_printf(ch, "No such city known as %s\r\n", arg);
    return;
  }

  if(!argument || argument[0] == '\0')
  {
    ch_printf(ch, "Citizenship rollcall for the %s\r\n\r\n", city->name);
    ch_printf(ch, "%-15.15s  %-15.15s %-6.6s %-6.6s %-6.6s %s\r\n", "Name", "Class", "Level", "Kills", "Deaths", "Joined on");
    send_to_char("-------------------------------------------------------------------------------------\r\n", ch);
    for(rollcall = city->first_citizen; rollcall; rollcall = rollcall->next)
    {
      if(rollcall->level > 100)
        continue;

      ch_printf(ch, "%-15.15s  %-15.15s %-6d %-6d %-6d %s", rollcall->name, capitalize(npc_class[rollcall->Class]), rollcall->level, rollcall->kills, rollcall->deaths, ctime(&rollcall->joined));
      total++;
    }
    ch_printf(ch, "\r\nThere are %d citizen%s in %s\r\n", total, total == 1 ? "" : "s", city->name);
    return;
  }

  if(IS_IMMORTAL(ch) || (city->duke && !str_cmp(ch->name, city->duke)) || (city->baron && !str_cmp(ch->name, city->baron)))
  {
    argument = one_argument(argument, arg2);
    if(!str_cmp(arg2, "remove"))
    {
      if(!argument || argument[0] == '\0')
      {
        send_to_char("Remove who from the rollcall?\r\n", ch);
        return;
      }
      remove_rollcall(city, argument);
      save_city(city);
      ch_printf(ch, "%s has been removed from the rollcall for %s\r\n", argument, city->name);
      return;
    }
  }
  do_rollcall(ch, (char *)"");
}

void fread_citizenlist(CITY_DATA * city, FILE * fp)
{
  ROLLCALL_DATA          *rollcall;
  const char             *word;
  bool                    fMatch;

  CREATE(rollcall, ROLLCALL_DATA, 1);
  rollcall->lastupdated = current_time;
  for(;;)
  {
    word = feof(fp) ? "End" : fread_word(fp);
    fMatch = FALSE;

    switch (UPPER(word[0]))
    {
      case '*':
        fMatch = TRUE;
        fread_to_eol(fp);
        break;

      case 'C':
        if(!str_cmp(word, "Class"))
        {
          char                   *temp = fread_string(fp);
          int                     Class = get_npc_class(temp);

          if(Class < 0 || Class >= MAX_NPC_CLASS)
          {
            bug("%s: Invalid class in city rollcall", __FUNCTION__);
            Class = get_npc_class((char *)"warrior");
          }
          STRFREE(temp);
          rollcall->Class = Class;
          fMatch = TRUE;
          break;
        }
        break;

      case 'D':
        KEY("Deaths", rollcall->deaths, fread_number(fp));
        break;

      case 'E':
        if(!str_cmp(word, "End"))
        {
          LINK(rollcall, city->first_citizen, city->last_citizen, next, prev);
          return;
        }
        break;

      case 'J':
        KEY("Joined", rollcall->joined, fread_number(fp));
        break;

      case 'K':
        KEY("Kills", rollcall->kills, fread_number(fp));
        break;

      case 'L':
        KEY("Level", rollcall->level, fread_number(fp));
        break;

      case 'N':
        KEY("Name", rollcall->name, fread_string(fp));
        break;

      case 'U':
        KEY("Updated", rollcall->lastupdated, fread_number(fp));
        break;

    }
    if(!fMatch)
      bug("%s: no match: %s", __FUNCTION__, word);

  }
}

bool has_city_position(CITY_DATA * city, const char *name)
{
  if(!city || !VLD_STR(name))
    return FALSE;

  if(VLD_STR(city->duke))
    if(!str_cmp(city->duke, name))
      return TRUE;
  if(VLD_STR(city->baron))
    if(!str_cmp(city->baron, name))
      return TRUE;
  if(VLD_STR(city->captain))
    if(!str_cmp(city->captain, name))
      return TRUE;
  if(VLD_STR(city->sheriff))
    if(!str_cmp(city->sheriff, name))
      return TRUE;
  if(VLD_STR(city->knight))
    if(!str_cmp(city->knight, name))
      return TRUE;
  return FALSE;
}

/* This is used to update the city positions if one becomes empty */
void fix_city_order(CITY_DATA * city)
{
  char                    buf[MSL];
  bool                    savecity = FALSE;

  if(!city)
    return;

  /*
   * First thing we need to do is make sure no repeats 
   */
  /*
   * Ok we have a duke, lets make sure they aren't anything else 
   */
  if(VLD_STR(city->duke))
  {
    if(VLD_STR(city->baron))
      if(!str_cmp(city->duke, city->baron))
      {
        savecity = TRUE;
        STRFREE(city->baron);
      }
    if(VLD_STR(city->captain))
      if(!str_cmp(city->duke, city->captain))
      {
        savecity = TRUE;
        STRFREE(city->captain);
      }
    if(VLD_STR(city->sheriff))
      if(!str_cmp(city->duke, city->sheriff))
      {
        savecity = TRUE;
        STRFREE(city->sheriff);
      }
    if(VLD_STR(city->knight))
      if(!str_cmp(city->duke, city->knight))
      {
        savecity = TRUE;
        STRFREE(city->knight);
      }
  }

  /*
   * Ok we have a baron, lets make sure they aren't anything else 
   */
  if(VLD_STR(city->baron))
  {
    if(VLD_STR(city->captain))
      if(!str_cmp(city->baron, city->captain))
      {
        savecity = TRUE;
        STRFREE(city->captain);
      }
    if(VLD_STR(city->sheriff))
      if(!str_cmp(city->baron, city->sheriff))
      {
        savecity = TRUE;
        STRFREE(city->sheriff);
      }
    if(VLD_STR(city->knight))
      if(!str_cmp(city->baron, city->knight))
      {
        savecity = TRUE;
        STRFREE(city->knight);
      }
  }

  /*
   * Ok we have a captain, lets make sure they aren't anything else 
   */
  if(VLD_STR(city->captain))
  {
    if(VLD_STR(city->sheriff))
      if(!str_cmp(city->captain, city->sheriff))
      {
        savecity = TRUE;
        STRFREE(city->sheriff);
      }
    if(VLD_STR(city->knight))
      if(!str_cmp(city->captain, city->knight))
      {
        savecity = TRUE;
        STRFREE(city->knight);
      }
  }

  /*
   * Ok we have a sheriff, lets make sure they aren't anything else 
   */
  if(VLD_STR(city->sheriff))
  {
    if(VLD_STR(city->knight))
      if(!str_cmp(city->sheriff, city->knight))
      {
        savecity = TRUE;
        STRFREE(city->knight);
      }
  }

  if(!VLD_STR(city->duke))
  {
    STRFREE(city->duke);
    if(VLD_STR(city->baron))
    {
      city->duke = STRALLOC(city->baron);
      STRFREE(city->baron);
      savecity = TRUE;
      snprintf(buf, MIL, "&W[&RAnnouncement&W]&C %s has been awarded the position of Duke for %s!", city->duke, city->name);
      echo_to_all(AT_RED, buf, ECHOTAR_ALL);
    }
    else if(VLD_STR(city->captain))
    {
      city->duke = STRALLOC(city->captain);
      STRFREE(city->captain);
      savecity = TRUE;
      snprintf(buf, MIL, "&W[&RAnnouncement&W]&C %s has been awarded the position of Duke for %s!", city->duke, city->name);
      echo_to_all(AT_RED, buf, ECHOTAR_ALL);
    }
    else if(VLD_STR(city->sheriff))
    {
      city->duke = STRALLOC(city->sheriff);
      STRFREE(city->sheriff);
      savecity = TRUE;
      snprintf(buf, MIL, "&W[&RAnnouncement&W]&C %s has been awarded the position of Duke for %s!", city->duke, city->name);
      echo_to_all(AT_RED, buf, ECHOTAR_ALL);
    }
    else if(VLD_STR(city->knight))
    {
      city->duke = STRALLOC(city->knight);
      STRFREE(city->knight);
      savecity = TRUE;
      snprintf(buf, MIL, "&W[&RAnnouncement&W]&C %s has been awarded the position of Duke for %s!", city->duke, city->name);
      echo_to_all(AT_RED, buf, ECHOTAR_ALL);
    }
  }

  if(!VLD_STR(city->baron))
  {
    STRFREE(city->baron);
    if(VLD_STR(city->captain))
    {
      city->baron = STRALLOC(city->captain);
      STRFREE(city->captain);
      savecity = TRUE;
      snprintf(buf, MIL, "&W[&RAnnouncement&W]&C %s has been awarded the position of Baron for %s!", city->baron, city->name);
      echo_to_all(AT_RED, buf, ECHOTAR_ALL);
    }
    else if(VLD_STR(city->sheriff))
    {
      city->baron = STRALLOC(city->sheriff);
      STRFREE(city->sheriff);
      savecity = TRUE;
      snprintf(buf, MIL, "&W[&RAnnouncement&W]&C %s has been awarded the position of Baron for %s!", city->baron, city->name);
      echo_to_all(AT_RED, buf, ECHOTAR_ALL);
    }
    else if(VLD_STR(city->knight))
    {
      city->baron = STRALLOC(city->knight);
      STRFREE(city->knight);
      savecity = TRUE;
      snprintf(buf, MIL, "&W[&RAnnouncement&W]&C %s has been awarded the position of Baron for %s!", city->baron, city->name);
      echo_to_all(AT_RED, buf, ECHOTAR_ALL);
    }
  }

  if(!VLD_STR(city->captain))
  {
    STRFREE(city->captain);
    if(VLD_STR(city->sheriff))
    {
      city->captain = STRALLOC(city->sheriff);
      STRFREE(city->sheriff);
      savecity = TRUE;
      snprintf(buf, MIL, "&W[&RAnnouncement&W]&C %s has been awarded the position of Captain for %s!", city->captain, city->name);
      echo_to_all(AT_RED, buf, ECHOTAR_ALL);
    }
    else if(VLD_STR(city->knight))
    {
      city->captain = STRALLOC(city->knight);
      STRFREE(city->knight);
      savecity = TRUE;
      snprintf(buf, MIL, "&W[&RAnnouncement&W]&C %s has been awarded the position of Captain for %s!", city->captain, city->name);
      echo_to_all(AT_RED, buf, ECHOTAR_ALL);
    }
  }

  if(!VLD_STR(city->sheriff))
  {
    STRFREE(city->sheriff);
    if(VLD_STR(city->knight))
    {
      city->sheriff = STRALLOC(city->knight);
      STRFREE(city->knight);
      savecity = TRUE;
      snprintf(buf, MIL, "&W[&RAnnouncement&W]&C %s has been awarded the position of Sheriff for %s!", city->sheriff, city->name);
      echo_to_all(AT_RED, buf, ECHOTAR_ALL);
    }
  }

  if(savecity)
    save_city(city);
}
