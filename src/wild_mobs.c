#include "h/mud.h"
short                   set_min_armor(int level);
short                   set_max_armor(int level);

// Geni wandering mob snippet
void found_prey         args((CHAR_DATA *ch, CHAR_DATA *victim));

void check_water_mobs(CHAR_DATA *ch)
{
  MOB_INDEX_DATA         *imob = NULL;
  CHAR_DATA              *mob = NULL;
  int                     vnum = -1;

  if((IS_NPC(ch) && !xIS_SET(ch->act, ACT_BEASTMELD)) || IS_IMMORTAL(ch) || IS_PUPPET(ch))
    return;

  if(IS_AFFECTED(ch, AFF_NOSCENT))
  {
    short                   chance;

    chance = number_chance(1, 8);

    if(chance < 6)
    {
      send_to_char("You see a creature in the distance, but it doesn't detect your presence.\r\n", ch);
      return;
    }
  }

  if(IS_AFFECTED(ch, AFF_VICTIM))
  {
    short                   chance = 0;

    chance = number_chance(1, 100);
    chance += lck_app[get_curr_lck(ch)].luck;
    if(chance > 55)
    {
      send_to_char("You force yourself to appear as a helpless victim.\r\n", ch);
      vnum = number_range(41030, 41034);
    }
  }
  else
  {
    short                   schance = 0;

    schance = number_chance(1, 100);
    schance -= lck_app[get_curr_lck(ch)].luck;
    if(schance > 85)
      vnum = number_range(41030, 41034);
  }

  if(vnum == -1)
    return;
  imob = get_mob_index(vnum);

  if(!imob)
  {
    snprintf(log_buf, MAX_STRING_LENGTH, "check_water_mobs: Missing mob for vnum %d", vnum);
    log_string(log_buf);
    return;
  }
  mob = create_mobile(imob);
  mob->timer = 1;
  mob->level = ch->level + 1;
  if(ch->race == RACE_DRAGON)
  {
    mob->level = ch->level + 3;
    mob->hit = set_hp(mob->level);
    mob->max_hit = set_hp(mob->level);
    mob->armor = set_armor_class(mob->level);
    mob->hitroll = set_hitroll(mob->level + 2);
    mob->damroll = set_damroll(mob->level + 2);
    mob->numattacks = set_num_attacks(mob->level + 2);
    mob->hitplus = set_hp(mob->level + 2);
  }
  else
  {
    mob->hit = set_hp(mob->level - 2);
    mob->max_hit = set_hp(mob->level - 2);
    mob->armor = set_armor_class(mob->level - 2);
    mob->hitroll = set_hitroll(mob->level - 2);
    mob->damroll = set_damroll(mob->level - 2);
    mob->numattacks = set_num_attacks(mob->level - 2);
    mob->hitplus = set_hp(mob->level - 2);
  }
if(xIS_SET(ch->act, PLR_BATTLE) || (xIS_SET(ch->act, ACT_BEASTMELD) && xIS_SET(ch->master->act, PLR_BATTLE)))
    send_to_char("!!SOUND(sound/crash.wav)\r\n", ch);

  start_hating(mob, ch);
  start_hunting(mob, ch);
  char_to_room(mob, ch->in_room);

  /*
   * Volk - quick message 
   */
  interpret(mob, (char *)"mpecho");
  interpret(mob, (char *)"mpecho &bA dark form suddenly swims toward you.&D");
  interpret(mob, (char *)"mpecho");
  if(IS_AFFECTED(ch, AFF_SNEAK) || IS_AFFECTED(ch, AFF_HIDE) || IS_AFFECTED(ch, AFF_INVISIBLE))
    interpret(mob, (char *)"emote swims from the east.");
  else
  {
    set_fighting(mob, ch);
    global_retcode = multi_hit(mob, ch, TYPE_UNDEFINED);
    do_bash(mob, (char *)"");
  }
}

/* This function does a random check, currently set to 6%,
 * to see if it should load a mobile
 * at the sector the PC just walked into.
 *
 * Rewritten by Geni to use tables and way too many mobs 
 */
void check_random_mobs(CHAR_DATA *ch)
{
  MOB_INDEX_DATA         *imob = NULL;
  CHAR_DATA              *mob = NULL;
  int                     vnum = -1;

  if((IS_NPC(ch) && !xIS_SET(ch->act, ACT_BEASTMELD)) || IS_IMMORTAL(ch) || IS_PUPPET(ch))
    return;

  if((ch->in_room->sector_type == SECT_HROAD) || (ch->in_room->sector_type == SECT_VROAD) || (ch->in_room->sector_type == SECT_CITY) || (ch->in_room->sector_type == SECT_AREA_ENT))
  {
    send_to_char("\r\nA squad of the king's army rides through ensuring wild mobs stay off the roads.\r\n", ch);
    return;
  }

  if(IS_AFFECTED(ch, AFF_NOSCENT))
  {
    short                   chance;

    chance = number_chance(1, 8);

    if(chance < 6)
    {
      send_to_char("You see a creature in the distance, but it doesn't detect your presence.\r\n", ch);
      return;
    }
  }

  if(ch->level <= 5)
    return;

  if(IS_AFFECTED(ch, AFF_VICTIM))
  {
    short                   chance = 0;

    chance = number_chance(1, 100);
    chance += lck_app[get_curr_lck(ch)].luck;
    if(chance > 55)
    {
      send_to_char("You force yourself to appear as a helpless victim.\r\n", ch);
      vnum = number_range(41000, 41020);
    }
  }
  else
  {
    short                   schance = 0;

    schance = number_chance(1, 100);
    schance -= lck_app[get_curr_lck(ch)].luck;
    if(schance > 85)
      vnum = number_range(41000, 41020);
  }

  if(vnum == -1)
    return;

  if(ch->in_room->sector_type == SECT_LAKE || ch->in_room->sector_type == SECT_RIVER)
    vnum = 41030;

  if(IN_RIFT(ch))
    vnum = number_range(6501, 6507);

  AFFECT_DATA             af;

  if(ch->Class == CLASS_BEAST && !IN_RIFT(ch))
  {
    vnum = number_range(41050, 41055);  // random pets
  }

  imob = get_mob_index(vnum);

  if(!imob)
  {
    snprintf(log_buf, MAX_STRING_LENGTH, "check_random_mobs: Missing mob for vnum %d", vnum);
    log_string(log_buf);
    return;
  }

  mob = create_mobile(imob);
  mob->timer = 1;
  mob->level = ch->level + 1;
  if(ch->Class == CLASS_BEAST)
  {
    mob->Class = 31;
    mob->secondclass = -1;
    mob->thirdclass = -1;
  }
  if(ch->race == RACE_DRAGON)
  {
    mob->level = ch->level + 3;
    mob->hit = set_hp(mob->level);
    mob->max_hit = set_hp(mob->level);
    mob->armor = set_armor_class(mob->level);
    mob->hitroll = set_hitroll(mob->level + 2);
    mob->damroll = set_damroll(mob->level + 2);
    mob->numattacks = set_num_attacks(mob->level + 2);
    mob->hitplus = set_hp(mob->level + 2);
  }
  else if(ch->race != RACE_DRAGON)
  {
    mob->hit = set_hp(mob->level - 2);
    mob->max_hit = set_hp(mob->level - 2);
    mob->armor = set_armor_class(mob->level - 2);
    if(ch->level > 45 && ch->level < 70)
    { // oddly people say hunting in
      // wilds is too easy during that
      // range.
      mob->hitroll = set_hitroll(mob->level);
      mob->damroll = set_damroll(mob->level);
    }
    else
    {
      mob->hitroll = set_hitroll(mob->level - 2);
      mob->damroll = set_damroll(mob->level - 2);
    }
    mob->numattacks = set_num_attacks(mob->level - 2);;
    mob->hitplus = set_hp(mob->level - 2);
  }
  if(IN_RIFT(ch))
  {
    af.type = gsn_sanctuary;
    af.duration = mob->level + 10;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = meb(AFF_SANCTUARY);
    af.level = mob->level;
    affect_to_char(mob, &af);

    mob->hitroll = (ch->hitroll * 2);
    mob->damroll = (ch->damroll * 2);
  }

  start_hating(mob, ch);
  start_hunting(mob, ch);
  char_to_room(mob, ch->in_room);

  if(IN_RIFT(ch))
  {
    OBJ_DATA               *obj;

    int                     onum;

    onum = number_range(6501, 6511);
    obj = create_object(get_obj_index(onum), 0);
    obj->level = ch->level;
    if(obj->item_type == ITEM_WEAPON)
    {
      obj->pIndexData->value[1] = set_min_chart(ch->level);
      obj->pIndexData->value[2] = set_max_chart(ch->level);
      obj->value[1] = set_min_chart(ch->level);
      obj->value[2] = set_max_chart(ch->level);
    }
    else if(obj->item_type == ITEM_ARMOR)
    {
      obj->pIndexData->value[0] = set_min_armor(ch->level);
      obj->pIndexData->value[1] = set_max_armor(ch->level);
      obj->value[0] = set_min_armor(ch->level);
      obj->value[1] = set_max_armor(ch->level);
    }
    obj = obj_to_char(obj, mob);
  }

  /*
   * Volk - quick message 
   */
  interpret(mob, (char *)"mpecho");
  interpret(mob, (char *)"mpecho &wA crashing sound is heard as you are suddenly ambushed!&D");
  interpret(mob, (char *)"mpecho");

  if(IS_AFFECTED(ch, AFF_SNEAK) || IS_AFFECTED(ch, AFF_HIDE) || IS_AFFECTED(ch, AFF_INVISIBLE))
  {
    interpret(mob, (char *)"emote arrives from the east.");
  }
  else
  {
    set_fighting(mob, ch);
    global_retcode = multi_hit(mob, ch, TYPE_UNDEFINED);
    do_bash(mob, (char *)"");
  }

if(xIS_SET(ch->act, PLR_BATTLE) || (xIS_SET(ch->act, ACT_BEASTMELD) && xIS_SET(ch->master->act, PLR_BATTLE)))
    send_to_char("!!SOUND(sound/crash.wav)\r\n", ch);
  /*
   * This should be long enough. If not, increase. Mob will be extracted when it expires.
   * * And trust me, this is a necessary measure unless you LIKE having your memory flooded
   * * by random overland mobs.
   */
  return;
}

/* Insert quick fix for wilderness sky area here.. Rather than go into this code too much
 * as it's a one use thing, i'll just do a quickie.. wild vnums 41k to 46k, wild_sky vnums
 * 51k to 56k */

void do_fixskywild(CHAR_DATA *ch, char *argument)
{
  int                     vnum, newvnum;
  int                     exitvnum;
  short                   dir;
  EXIT_DATA              *pexit;
  EXIT_DATA              *newexit;

  for(vnum = 60000; vnum < 62999; vnum++)
  {
    newexit = NULL;
    newvnum = vnum;
    newvnum += 2999;

/* Now we have 2 vnums, let's sort exits out from here */

/* Loop through exits */
    for(dir = 0; dir < 8; dir++)
    {
      if((pexit = get_exit(get_room_index(vnum), dir)) != NULL)
        /*
         * There IS an exit in * dir 
         */
        if(dir == DIR_EAST)
          break;
      if(dir == DIR_WEST)
        break;
      if(dir == DIR_NORTH)
        break;
      if(dir == DIR_SOUTH)
        break;

      {
/* Where is it going? */
        exitvnum = pexit->to_room->vnum;
        exitvnum += 2999;

        newexit = make_exit(get_room_index(newvnum), get_room_index(exitvnum), dir);
        newexit->key = -1;
        newexit->exit_info = 0;
      }
    }

/* Need to create one last exit to the actual sky room */
    newexit = make_exit(get_room_index(vnum), get_room_index(newvnum), DIR_UP);
    newexit->key = -1;
    newexit->exit_info = 0;
    newexit = make_exit(get_room_index(newvnum), get_room_index(vnum), DIR_DOWN);
    newexit->key = -1;
    newexit->exit_info = 0;
    newvnum = 0;
    exitvnum = 0;
  }
  return;

}

void check_sea_monsters(CHAR_DATA *ch)
{
// No tiny mobs here, talking giant squid tentacles, killer whale, giant sea turtle, pirates
  MOB_INDEX_DATA         *imob = NULL;
  CHAR_DATA              *mob = NULL;
  int                     vnum = -1;

  if(IS_NPC(ch))
    return;

  short                   chance = 0;

  chance = number_chance(1, 100);

  if(ch->pcdata->ship < 1)
    return;

  if(chance > 10)
    return;

  ROOM_INDEX_DATA        *lside, *rside;

  if(ch->pcdata->ship == 1)
  {
    rside = get_room_index(56300);
    lside = get_room_index(56301);
  }

  else if(ch->pcdata->ship == 2)
  {
    rside = get_room_index(56307);
    lside = get_room_index(56308);
  }
  else if(ch->pcdata->ship == 3)
  {
    rside = get_room_index(56312);
    lside = get_room_index(56313);
  }
  else if(ch->pcdata->ship == 4)
  {
    rside = get_room_index(56318);
    lside = get_room_index(56319);
  }
  else if(ch->pcdata->ship == 5)
  {
    rside = get_room_index(56324);
    lside = get_room_index(56326);
  }
  else if(ch->pcdata->ship == 6)
  {
    rside = get_room_index(56330);
    lside = get_room_index(56332);
  }
  else if(ch->pcdata->ship == 7)
  {
    rside = get_room_index(56336);
    lside = get_room_index(56338);
  }
  else if(ch->pcdata->ship == 8)
  {
    rside = get_room_index(56342);
    lside = get_room_index(56344);
  }
  else if(ch->pcdata->ship == 9)
  {
    rside = get_room_index(56348);
    lside = get_room_index(56350);
  }
  else if(ch->pcdata->ship == 10)
  {
    rside = get_room_index(56354);
    lside = get_room_index(56356);
  }
  else if(ch->pcdata->ship == 11)
  {
    rside = get_room_index(56360);
    lside = get_room_index(56362);
  }
  else if(ch->pcdata->ship == 12)
  {
    rside = get_room_index(56366);
    lside = get_room_index(56368);
  }
  else if(ch->pcdata->ship == 13)
  {
    rside = get_room_index(56372);
    lside = get_room_index(56374);
  }
  else if(ch->pcdata->ship == 14)
  {
    rside = get_room_index(56378);
    lside = get_room_index(56380);
  }
  else if(ch->pcdata->ship == 15)
  {
    rside = get_room_index(56384);
    lside = get_room_index(56386);
  }
  else if(ch->pcdata->ship == 16)
  {
    rside = get_room_index(56390);
    lside = get_room_index(56392);
  }
  else if(ch->pcdata->ship == 17)
  {
    rside = get_room_index(56396);
    lside = get_room_index(56398);
  }
  else if(ch->pcdata->ship == 18)
  {
    rside = get_room_index(56402);
    lside = get_room_index(56404);
  }
  else if(ch->pcdata->ship == 19)
  {
    rside = get_room_index(56408);
    lside = get_room_index(56410);
  }
  else if(ch->pcdata->ship == 20)
  {
    rside = get_room_index(56414);
    lside = get_room_index(56416);
  }

  if(chance < 11)
  {
    vnum = number_range(41046, 41049);
  }
  if(vnum == -1)
    return;
  imob = get_mob_index(vnum);

  if(!imob)
  {
    snprintf(log_buf, MAX_STRING_LENGTH, "check_sea_monsters: Missing mob for vnum %d", vnum);
    log_string(log_buf);
    return;
  }

  if(xIS_SET(ch->act, PLR_COMMUNICATION))
    send_to_char("!!SOUND(sound/seamonster.wav)\r\n", ch);

  mob = create_mobile(imob);
  mob->level = ch->level + 1;
  mob->timer = 100;
  mob->hit = set_hp(mob->level - 2);
  mob->max_hit = set_hp(mob->level - 2);
  mob->armor = set_armor_class(mob->level - 2);
  mob->hitroll = set_hitroll(mob->level - 2);
  mob->damroll = set_damroll(mob->level - 2);
  mob->numattacks = set_num_attacks(mob->level - 2);
  mob->hitplus = set_hp(mob->level - 2);
  // where to place the mobs? Gonna be dependant on which ship
  char_to_room(mob, lside);
  // bug( "check_sea_monster: placing %s in room %d", mob->name, mob->in_room->vnum );

  mob = create_mobile(imob);
  mob->level = ch->level + 1;
  mob->timer = 100;
  mob->hit = set_hp(mob->level - 2);
  mob->max_hit = set_hp(mob->level - 2);
  mob->armor = set_armor_class(mob->level - 2);
  mob->hitroll = set_hitroll(mob->level - 2);
  mob->damroll = set_damroll(mob->level - 2);
  mob->numattacks = set_num_attacks(mob->level - 2);
  mob->hitplus = set_hp(mob->level - 2);
  // where to place the mobs? Gonna be dependant on which ship
  char_to_room(mob, rside);
  // bug( "check_sea_monster: placing %s in room %d", mob->name, mob->in_room->vnum );
  if(mob->pIndexData->vnum == 41049)
  {
    interpret(mob, (char *)"mpasound &bAn invading ship has just boarded your ship!&D");
    send_to_char("&bAn invading ship has just boarded your ship!\r\n", ch);
  }
  else
  {
    interpret(mob, (char *)"mpasound &bA dark form suddenly swims toward your ship!&D");
    send_to_char("&bA dark form suddenly swims toward your ship!\r\n", ch);
  }
  if(ch->pcdata->cansail != 2)
    ch->pcdata->cansail = 2;
}
