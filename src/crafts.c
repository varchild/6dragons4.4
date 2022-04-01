/***************************************************************************
                      _____  __      __  __      __
                     /  _  \/  \    /  \/  \    /  \
                    /  /_\  \   \/\/   /\   \/\/   /
                   /    |    \        /  \        /
                   \____|__  /\__/\  /    \__/\  /
                           \/      \/          \/

    As the Wheel Weaves based on ROM 2.4. Original code by Dalsor.
    See changes.log for a list of changes from the original ROM code.
    Credits for code created by other authors have been left
 	intact at the head of each function.

    Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,
    Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.

    Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael
    Chastain, Michael Quan, and Mitchell Tse.

    In order to use any part of this Merc Diku Mud, you must comply with
    both the original Diku license in 'license.doc' as well the Merc
    license in 'license.txt'.  In particular, you may not remove either of
    these copyright notices.

    Much time and thought has gone into this software and you are
    benefitting.  We hope that you share your changes too.  What goes
    around, comes around.

	ROM 2.4 is copyright 1993-1998 Russ Taylor
	ROM has been brought to you by the ROM consortium
	    Russ Taylor (rtaylor@hypercube.org)
	    Gabrielle Taylor (gtaylor@hypercube.org)
	    Brian Moore (zump@rom.org)
	By using this code, you have agreed to follow the terms of the
	ROM license, in the file Rom24/doc/rom.license
 ***************************************************************************/

#include <string.h>
#include "h/mud.h"
#include "h/crafts.h"
#include "h/files.h"
#include "h/key.h"
#include "h/clans.h"

#define SMITH_PRODUCT    41002
#define MAX_GEMS 15
#define MAX_JEWELRY 31
#define MAX_MILL_ITEMS 32
#define MAX_SMITH_ITEMS  40
#define MAX_FOOD 20
#define MAX_DRINK 20
#define MAX_FRUIT 20
#define MAX_TANNED 45
#define MAX_CRAFTED 20
#define MAX_HUNTED 20
#define MAX_JEWELRY 31
short                   set_min_armor(int level);
short                   set_max_armor(int level);

#define MAX_ORE_ITEMS   7
int clan_lookup         args((const char *name));
REACTIVE_FAIL_DATA     *first_qq;
REACTIVE_FAIL_DATA     *last_qq;

int                     usetier;

int drink_lookup(const char *name)
{
  int                     item;

  for(item = 0; item < MAX_DRINK; item++)
  {
    if(!str_cmp(name, drink_one[item].name))
    {
      usetier = 1;
      return item;
    }
  }
  usetier = -1;
  return -1;
}

int get_armor_ac_mod(int level, int tlevel)
{
  if(tlevel >= 20)
  {
    if(level > 19)
      return 3;
    if(level > 4)
      return 4;
    return 5;
  }
  if(tlevel >= 15)
  {
    if(level > 19)
      return 1;
    if(level > 4)
      return 2;
    return 3;
  }
  if(tlevel > 5)
  {
    if(level > 19)
      return -5;
    if(level > 4)
      return -3;
    return -1;
  }
  if(level > 19)
    return -10;
  if(level > 4)
    return -4;

  return -1;
}

int tier_lookup(const char *name)
{
  int                     item;

  for(item = 0; item < MAX_SMITH_ITEMS; item++)
  {
    if(!str_cmp(name, tier_one[item].name))
    {
      usetier = 1;
      return item;
    }
  }
  usetier = -1;
  return -1;
}

int gem_lookup(const char *name)
{
  int                     item;

  for(item = 0; item < MAX_JEWELRY; item++)
  {
    if(!str_cmp(name, jewelry_one[item].name))
    {
      usetier = 1;
      return item;
    }
  }
  usetier = -1;
  return -1;
}

int mill_lookup(const char *name)
{
  int                     item;

  for(item = 0; item < MAX_MILL_ITEMS; item++)
  {
    if(!str_cmp(name, lumber_one[item].name))
    {
      usetier = 1;
      return item;
    }
  }
  usetier = -1;
  return -1;
}

short weapon_bonus(int level, int olevel)
{
  short                   qball = 0;

  if(level < 5)
    qball = 3;
  else if(level < 10 && level > 4)
    qball = 4;
  else if(level < 15 && level > 9)
    qball = 5;
  else if(level < 20 && level > 14)
    qball = 6;
  else if(level >= 20)
    qball = 7;
  else
    qball = 3;
  if(olevel < 20 && qball > 4)
    qball -= 2;
  else if(olevel > 19 && olevel < 49)
    qball += 2;
  else if(olevel > 50 && olevel < 100)
    qball += 3;
  else if(olevel == 100)
    qball += 4;
  else
    qball += 2;
  return qball;
}

short set_tier_min(int level, int tier)
{
  if(tier == 1)
  {
    if(level <= 4)
      return 3;
    if(level <= 9)
      return 5;
    if(level <= 19)
      return 7;
    if(level <= 25)
      return 12;
    if(level <= 34)
      return 17;
    if(level <= 44)
      return 22;
    if(level <= 54)
      return 27;
    if(level <= 64)
      return 32;
    if(level <= 74)
      return 37;
    if(level <= 84)
      return 42;
    if(level <= 99)
      return 47;
    return 52;
  }
  if(tier == 2)
  {
    if(level <= 4)
      return 4;
    if(level <= 9)
      return 6;
    if(level <= 19)
      return 8;
    if(level <= 25)
      return 13;
    if(level <= 34)
      return 18;
    if(level <= 44)
      return 23;
    if(level <= 54)
      return 28;
    if(level <= 64)
      return 33;
    if(level <= 74)
      return 38;
    if(level <= 84)
      return 43;
    if(level <= 99)
      return 48;
    return 53;
  }
  /*
   * If you get here its tier 3 
   */
  if(level <= 4)
    return 6;
  if(level <= 9)
    return 8;
  if(level <= 19)
    return 10;
  if(level <= 25)
    return 15;
  if(level <= 34)
    return 20;
  if(level <= 44)
    return 25;
  if(level <= 54)
    return 30;
  if(level <= 64)
    return 35;
  if(level <= 74)
    return 40;
  if(level <= 84)
    return 45;
  if(level <= 99)
    return 50;
  return 55;
}

short set_tier_max(int level, int tier)
{
  if(tier == 1)
  {
    if(level <= 4)
      return 7;
    if(level <= 9)
      return 12;
    if(level <= 19)
      return 17;
    if(level <= 24)
      return 22;
    if(level <= 34)
      return 32;
    if(level <= 39)
      return 37;
    if(level <= 44)
      return 42;
    if(level <= 49)
      return 47;
    if(level <= 54)
      return 52;
    if(level <= 60)
      return 62;
    if(level <= 70)
      return 72;
    if(level <= 80)
      return 82;
    if(level <= 90)
      return 92;
    if(level <= 99)
      return 102;
    return 100;
  }
  if(tier == 2)
  {
    if(level <= 4)
      return 8;
    if(level <= 9)
      return 13;
    if(level <= 19)
      return 18;
    if(level <= 24)
      return 23;
    if(level <= 34)
      return 33;
    if(level <= 39)
      return 38;
    if(level <= 44)
      return 43;
    if(level <= 49)
      return 48;
    if(level <= 54)
      return 53;
    if(level <= 60)
      return 63;
    if(level <= 70)
      return 73;
    if(level <= 80)
      return 83;
    if(level <= 90)
      return 93;
    if(level <= 99)
      return 103;
    return 103;
  }
  /*
   * If here its tier 3 
   */
  if(level <= 4)
    return 10;
  if(level <= 9)
    return 15;
  if(level <= 19)
    return 20;
  if(level <= 24)
    return 25;
  if(level <= 34)
    return 35;
  if(level <= 39)
    return 40;
  if(level <= 44)
    return 45;
  if(level <= 49)
    return 50;
  if(level <= 54)
    return 55;
  if(level <= 60)
    return 65;
  if(level <= 70)
    return 75;
  if(level <= 80)
    return 85;
  if(level <= 90)
    return 95;
  if(level <= 99)
    return 105;
  return 105;
}

void learn_from_craft(CHAR_DATA *ch, int sn)
{
  int                     adept = 0, gain = 0;
  char                    buf[MSL];

  if(IS_NPC(ch) || ch->pcdata->learned[sn] <= 0)
    return;

  adept = 95;

  if(adept < 1)
    return;

  if(ch->pcdata->tradelevel >= 20)
    return;

  if(sn == gsn_forge || sn == gsn_tan || sn == gsn_bake || sn == gsn_mix || sn == gsn_jewelry || sn == gsn_mill || sn == gsn_produce)
  {
    if(ch->pcdata->tradelevel <= 19)
      gain = number_chance(300, 700);
    if(ch->pcdata->tradelevel <= 15)
      gain = number_chance(125, 400);
    if(ch->pcdata->tradelevel <= 10)
      gain = number_chance(50, 80);
    if(ch->pcdata->tradelevel <= 5)
      gain = number_chance(25, 50);
    if(ch->pcdata->tradelevel <= 2)
      gain = number_chance(3, 10);
  }

  if(sn == gsn_mine || sn == gsn_hunt || sn == gsn_gather || sn == gsn_fell || sn == gsn_unearth)
  {
    gain = number_chance(1, 5);
  }

  if(happyhouron || sysdata.happy)
  {
    gain *= 2;
  }
  if(!happyhouron && ch->pcdata->getsdoubleexp)
  {
    gain *= 3;
  }

  if(xIS_SET(ch->act, PLR_R54))
    gain *= 4;

  if(IS_AFFECTED(ch, AFF_BOOST))
    gain *= 3.0;

  save_char_obj(ch);
  ch->pcdata->craftpoints += gain;
  ch_printf(ch, "&WYou receive %d crafting point%s.\r\n", gain, gain == 1 ? "" : "s");

  if(ch->pcdata->craftpoints >= craft_level(ch, ch->pcdata->tradelevel + 1))
  {
    snprintf(buf, MSL, "%s has just achieved crafting level %d!\r\n", ch->name, ++ch->pcdata->tradelevel);
    announce(buf);

    ch->pcdata->craftpoints = (ch->pcdata->craftpoints - craft_level(ch, (ch->pcdata->tradelevel)));
  }

}

/* modifying do_mine to include gemstones for jewelers - Patriot */

void send_mine_syntax(CHAR_DATA *ch)
{
  send_to_char("&CThe Listing for Mining&c\r\n", ch);
  if(IS_BLIND(ch))
  {
    send_to_char("&cType Mine and the name of the material you want to mine.\r\n", ch);
  }
  else
  {
    send_to_char("&cSyntax: Mine <&Cmaterial&c>\r\n", ch);
  }

  if(ch->pcdata->tradeclass == 20)
  {
    send_to_char(" bronze", ch);

    if(ch->pcdata->tradelevel >= 3)
      send_to_char(" silver", ch);
    if(ch->pcdata->tradelevel >= 5)
      send_to_char(" gold", ch);
    if(ch->pcdata->tradelevel >= 8)
      send_to_char(" iron", ch);
    if(ch->pcdata->tradelevel >= 10)
      send_to_char(" steel", ch);
    if(ch->pcdata->tradelevel >= 15)
      send_to_char(" titanium", ch);
  }
  else if(ch->pcdata->tradeclass == 24)
  {
    send_to_char(" bronze", ch);
    if(ch->pcdata->tradelevel >= 3)
      send_to_char(" silver", ch);
    if(ch->pcdata->tradelevel >= 5)
      send_to_char(" gold", ch);
    send_to_char(" amethyst", ch);
    if(ch->pcdata->tradelevel >= 3)
      send_to_char(" pearl", ch);
    if(ch->pcdata->tradelevel >= 5)
      send_to_char(" emerald", ch);
    if(ch->pcdata->tradelevel >= 8)
      send_to_char(" ruby", ch);
    if(ch->pcdata->tradelevel >= 10)
      send_to_char(" sapphire", ch);
    if(ch->pcdata->tradelevel >= 15)
      send_to_char(" diamond", ch);
  }
  send_to_char("\r\n", ch);

}

void send_unearth_syntax(CHAR_DATA *ch)
{
  send_to_char("&CThe Listing for Unearthed&c\r\n", ch);
  if(IS_BLIND(ch))
  {
    send_to_char("&cType Unearth and the name of the material you want to unearth.\r\n", ch);
  }
  else
  {
    send_to_char("&cSyntax: Unearth <&Cmaterial&c>\r\n", ch);
  }

  if(ch->pcdata->tradeclass == 26)
  {
    send_to_char(" titanium", ch);

    if(ch->pcdata->tradelevel >= 5)
      send_to_char(" vibranium", ch);
    if(ch->pcdata->tradelevel >= 10)
      send_to_char(" adamantium", ch);
    if(ch->pcdata->tradelevel >= 15)
      send_to_char(" carbonadium ", ch);
    if(ch->pcdata->tradelevel >= 20)
      send_to_char(" meteor", ch);
  }
  send_to_char("\r\n", ch);

}

// A digging skill while in dragon form
void do_unearth(CHAR_DATA *ch, char *argument)
{
  OBJ_DATA               *obj;
  char                    arg1[MIL], name[MSL], shortdesc[MSL], longdesc[MSL];
  int                     color, value, cost, knows;
  short                   chance, level, slevel;

  argument = one_argument(argument, arg1);

  if(IS_NPC(ch))
    return;

  if(!VLD_STR(arg1) || !str_cmp(arg1, "list"))
  {
    send_unearth_syntax(ch);
    return;
  }

  if(ch->position != POS_STANDING)
  {
    send_to_char("You should be standing to attempt this.\r\n", ch);
  }

  switch (ch->in_room->sector_type)
  {
    case SECT_OCEAN:
    case SECT_ARCTIC:
    case SECT_BEACH:
    case SECT_FOG:
    case SECT_SKY:
    case SECT_CLOUD:
    case SECT_SNOW:
    case SECT_ORE:
    case SECT_QUICKSAND:
    case SECT_DEEPMUD:
    case SECT_PORTALSTONE:
    case SECT_LAKE:
    case SECT_CAMPSITE:
    case SECT_DOCK:
    case SECT_RIVER:
    case SECT_WATERFALL:
    case SECT_CROSSROAD:
    case SECT_VROAD:
    case SECT_HROAD:
    case SECT_ROAD:
    case SECT_CITY:
    case SECT_SWAMP:
    case SECT_LAVA:
    case SECT_OCEANFLOOR:
    case SECT_AREA_ENT:
    case SECT_AIR:
    case SECT_UNDERGROUND:
    case SECT_UNDERWATER:
    case SECT_INSIDE:
      send_to_char("You cannot gather here.\r\n", ch);
      return;

    default:
      break;
  }

  if(ch->move < 6)
  {
    send_to_char("You don't have enough move points to unearth.\r\n", ch);
    return;
  }
  else
  {
    ch->move -= 5;
  }

  // Add a further check so if not a jeweler can't mine jewels
  if(ch->pcdata->tradeclass != 26)
  {
    send_to_char("You are not a dragonic tradeclass.\r\n", ch);
    return;
  }

  if(ch->race != RACE_DRAGON)
  {
    send_to_char("Huh?\r\n", ch);
    return;
  }

  if(ch->pcdata->tradeclass == 26 && ch->race == RACE_DRAGON)
  {
    if((ch->pcdata->tradelevel < 5) && (!str_cmp(arg1, "vibranium") || !str_cmp(arg1, "adamantium") || !str_cmp(arg1, "carbonadium") || !str_cmp(arg1, "meteor")))
    {
      send_to_char("You don't have the skill to unearth that yet.", ch);
      return;
    }
    if((ch->pcdata->tradelevel < 10) && (!str_cmp(arg1, "adamantium") || !str_cmp(arg1, "carbonadium") || !str_cmp(arg1, "meteor")))
    {
      send_to_char("You don't have the skill to unearth that yet.", ch);
      return;
    }
    if((ch->pcdata->tradelevel < 15) && (!str_cmp(arg1, "carbonadium") || !str_cmp(arg1, "meteor")))
    {
      send_to_char("You don't have the skill to unearth that yet.", ch);
      return;
    }
    if((ch->pcdata->tradelevel < 20) && (!str_cmp(arg1, "meteor")))
    {
      send_to_char("You don't have the skill to unearth that yet.", ch);
      return;
    }
  }

  if(!str_cmp(arg1, "titanium"))
  {
    knows = 15;
    level = 0;
  }
  else if(!str_cmp(arg1, "vibranium"))
  {
    knows = 40;
    level = 5;
  }
  else if(!str_cmp(arg1, "adamantium"))
  {
    knows = 55;
    level = 10;
  }
  else if(!str_cmp(arg1, "carbonadium"))
  {
    knows = 65;
    level = 15;
  }
  else if(!str_cmp(arg1, "meteor"))
  {
    knows = 85;
    level = 20;
  }

  OBJ_DATA               *bag;

  for(bag = ch->first_carrying; bag; bag = bag->next_content)
  {
    if(bag->item_type == ITEM_RESOURCE_BAG && bag->value[0] == 125) // either
      // change
      // that to
      // accept
// craft1-20 or change vnum of bag
      break;
  }

  if(!bag)
  {
    send_to_char("You do not have a dragonic saddle to hold anything with.\r\n", ch);
    return;
  }

  if(bag->craft1 >= 1000 && !str_cmp(arg1, "titanium"))
  {
    send_to_char("You cannot hold any more titanium in your dragonic saddle.\r\n", ch);
    return;
  }
  else if(bag->craft2 >= 1000 && !str_cmp(arg1, "vibranium"))
  {
    send_to_char("You cannot hold any more vibranium in your dragonic saddle.\r\n", ch);
    return;
  }
  else if(bag->craft3 >= 1000 && !str_cmp(arg1, "adamantium"))
  {
    send_to_char("You cannot hold any more adamantium in your dragonic saddle.\r\n", ch);
    return;
  }
  else if(bag->craft4 >= 1000 && !str_cmp(arg1, "carbonadium"))
  {
    send_to_char("You cannot hold any more carbonadium in your dragonic saddle.\r\n", ch);
    return;
  }
  else if(bag->craft5 >= 1000 && !str_cmp(arg1, "meteor"))
  {
    send_to_char("You cannot hold any more meteor in your dragonic saddle.\r\n", ch);
    return;
  }

  if(ch->move < 5)
  {
    send_to_char("You don't have enough energy to keep unearthing.\r\n", ch);
    return;
  }

  if(!str_cmp(arg1, "titanium") || !str_cmp(arg1, "vibranium") || !str_cmp(arg1, "adamantium") || !str_cmp(arg1, "carbonadium")); /* Does 
                                                                                                                                   * nothing
                                                                                                                                   * * and
                                                                                                                                   * keeps *
                                                                                                                                   * going */
  else if(!str_cmp(arg1, "meteor"))
    chance = number_chance(1, 5); /* More chance of failing on this */
  else
  { /* If none of those then give a * message */

    send_unearth_syntax(ch);
    return;
  }

  WAIT_STATE(ch, skill_table[gsn_unearth]->beats);

  if(!can_use_skill(ch, number_percent(), gsn_unearth))
  {
    learn_from_failure(ch, gsn_unearth);
    send_to_char("You stop unearthing as you hit a massive boulder.\r\n", ch);
    return;
  }

  ch->move -= 1;

  chance = number_chance(1, 10);

  short                   amount = 0, extra = 0;

  amount = 1;
  extra = number_range(1, 4);
  if(ch->pcdata->tradelevel > 9 && extra == 2)
  {
    amount += 1;
  }
  if(ch->pcdata->tradelevel > 14 && extra == 2)
  {
    amount += 1;
  }

  if(chance < 3)
  {
    if(chance <= 1)
    {
      act(AT_ORANGE, "$n's unearthing is not skillful enough to make progress.\r\n", ch, NULL, NULL, TO_ROOM);
      send_to_char("&OYour unearthing is not skillful enough to make progress.\r\n", ch);
    }
    else if(chance >= 2)
    {
      act(AT_CYAN, "$n begins to labor in earnest unearthing away at piles of dirt and rock.\r\n", ch, NULL, NULL, TO_ROOM);
      send_to_char("&cYou begin to labor in earnest unearthing away at piles of dirt and rock.\r\n", ch);
      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/dig.wav)\r\n", ch);

    }
    return;
  }

  if(ch->pcdata->learned[gsn_unearth] < knows || ch->pcdata->tradelevel < level)
  {
    send_to_char("You begin to unearth dirt and rock with your massive claws.\r\n", ch);
    pager_printf(ch, "&GYour unearthing pays off as you successfully find a metal!\r\n");
    act(AT_GREEN, "$n's unearthing pays off as $e successfully finds a metal.", ch, NULL, NULL, TO_ROOM);
    pager_printf(ch, "&GYour lack of unearthing knowledge shows as you have gouged the metal beyond use!\r\n");
    act(AT_GREEN, "$n's looks away in disgust as $e realizes the metal is ruined.", ch, NULL, NULL, TO_ROOM);
    learn_from_failure(ch, gsn_unearth);
    return;
  }

  if(!str_cmp(arg1, "titanium"))
  {
    bag->craft1 += amount;
  }
  else if(!str_cmp(arg1, "vibranium"))
  {
    bag->craft2 += amount;
  }
  else if(!str_cmp(arg1, "adamantium"))
  {
    bag->craft3 += amount;
  }
  else if(!str_cmp(arg1, "carbonadium"))
  {
    bag->craft4 += amount;
  }
  else if(!str_cmp(arg1, "meteor"))
  {
    bag->craft5 += amount;
  }

  send_to_char("You begin to unearth a metal.\r\n", ch);
  ch_printf(ch, "&GYour unearthing pays off as you successfully dig up a %s metal!\r\n", arg1);

  act(AT_GREEN, "$n's unearthing suddenly digs up a metal.", ch, NULL, NULL, TO_ROOM);
  if(amount > 2)
  {
    send_to_char("You notice your able to save more metal with your more precise unearthing of the metals as you dig deep with your mighty talons.\r\n", ch);
  }
  else
    send_to_char("You carefully dig up the metal chunks and place it in your dragonic bag.\r\n", ch);
  if(xIS_SET(ch->act, PLR_CRAFTS))
    send_to_char("!!SOUND(sound/dig.wav)\r\n", ch);

  learn_from_craft(ch, gsn_unearth);

}

void send_craft_syntax(CHAR_DATA *ch)
{
  if(!ch)
    return;

  if(IS_BLIND(ch))
  {
    send_to_char("Type produce metal type into item level\r\n", ch);
    send_to_char("Type produce list for a list of items you can produce or produce list all to see everything.\r\n", ch);
    if(ch->pcdata->tradelevel > 9)
    {
      send_to_char("Type produce Id number repair\r\n", ch);
      send_to_char("eg. produce 3232 repair\r\n", ch);
    }
    if(ch->pcdata->tradelevel > 14)
    {
      send_to_char("Type produce id number rename new name to change the objects name or produce id number rekey new key to change what you type to get the object.\r\n", ch);
    }
    send_to_char("Note There are 4 steps, keep doing the produce command until the last step.\r\n", ch);
  }
  else
  {
    send_to_char("&cSyntax: produce <&Cmetal&c> into <&Citem&c> <&Clevel&c>\r\n", ch);
    send_to_char("Syntax: produce list or produce list all\r\n", ch);
    if(ch->pcdata->tradelevel > 9)
    {
      send_to_char("Syntax: produce id# repair\r\n", ch);
      send_to_char("eg. produce 3232 repair\r\n", ch);
    }

    if(ch->pcdata->tradelevel > 14)
    {
      send_to_char("Syntax: produce id# rename '&Cnew name&c' or produce id# rekey '&Cnew key&c'\r\n", ch);
    }
    send_to_char("Note: There are &C4 steps&c, keep doing the produce command until the last step.\r\n", ch);
  }
}

// Actual craft has to be in draogn lord form
void do_produce(CHAR_DATA *ch, char *argument)
{
  short                   number;
  OBJ_DATA               *item;
  char                    arg1[MIL], buf[MSL], arg2[MIL], arg3[MIL], arg4[MIL];
  char                    name_buf[MSL], short_buf[MSL], long_buf[MSL], extra_buf[MSL];
  const char             *adj;
  int                     i = 0, x = 0, sn = 1, usetan, usehunt;
  bool                    itm = FALSE;
  short                   chance;
  AFFECT_DATA            *paf;
  struct skill_type      *skill = NULL;

  chance = number_range(1, 10);

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  argument = one_argument(argument, arg3);
  argument = one_argument(argument, arg4);

  name_buf[0] = '\0';
  short_buf[0] = '\0';
  long_buf[0] = '\0';
  extra_buf[0] = '\0';

  if(IS_NPC(ch))
    return;

  if(ch->race == RACE_DRAGON)
  {
    send_to_char("You can't possibly do precision product work in your massive dragon body!\r\n", ch);
    return;
  }

  if(arg1[0] == '\0')
  {
    send_craft_syntax(ch);
    return;
  }

  if(ch->move < 6)
  {
    send_to_char("You don't have enough move points to perform that.\r\n", ch);
    return;
  }
  else
  {
    ch->move -= 2;
  }

  if(ch->position != POS_STANDING)
  {
    send_to_char("You should be standing to attempt this.\r\n", ch);
  }

// Just adding objID to work for repair code section 

  if(!str_cmp(arg2, "repair"))
  {
    OBJ_DATA               *repair;

    for(repair = ch->first_carrying; repair; repair = repair->next_content)
    {
      if(atoi(arg1) == repair->guid && (repair->item_type == ITEM_ARMOR || repair->item_type == ITEM_WEAPON))
        break;
    }

    if(!repair)
    {
      send_to_char("You do not have any repairable equipment to repair.\r\n", ch);
      return;
    }

    if(ch->pcdata->tradelevel < 10)
    {
      send_to_char("You are not skilled enough of a producer to repair items yet.\r\n", ch);
      return;
    }

    if(repair->item_type == ITEM_WEAPON && repair->value[0] == 12)
    {
      send_to_char("It is already honed to perfection.\r\n", ch);
      return;
    }
    if(repair->item_type == ITEM_ARMOR && repair->value[0] == repair->value[1])
    {
      send_to_char("It is already forged to perfection.\r\n", ch);
      return;
    }

    act(AT_CYAN, "$n hammers $p back to shape.", ch, repair, NULL, TO_ROOM);
    act(AT_CYAN, "You hammer $p back to shape.", ch, repair, NULL, TO_CHAR);
    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/hammer.wav)\r\n", ch);

    switch (repair->item_type)
    {
      case ITEM_ARMOR:
        repair->value[0] = repair->value[1];
        break;
      case ITEM_WEAPON:
        repair->value[0] = INIT_WEAPON_CONDITION;
        break;
    }
    return;
  }

// End of repair code section 

  if(!str_cmp(arg2, "rename") && ch->pcdata->tradelevel > 14 && !xIS_SET(ch->act, PLR_NORENAME))
  {
    OBJ_DATA               *item;

    for(item = ch->first_carrying; item; item = item->next_content)
    {
      if(atoi(arg1) == item->guid && item->pIndexData->vnum == SMITH_PRODUCT)
        break;
    }
    if(!item)
    {
      send_to_char("You do not have any produced items to rename or rekey.\r\n", ch);
      return;
    }

    if(!arg3 || arg3[0] == '\0')
    {
      send_to_char("You must specify the new keywords, ie produce id number rename 'new name' or rename 'ring toering'.\r\n", ch);
      return;
    }
    ch_printf(ch, "&cChanging item's description from &C%s&c to&C %s&c..\r\n", item->short_descr, arg3);
    STRFREE(item->short_descr);
    item->short_descr = STRALLOC(arg3);
    sprintf(long_buf, "%s has been left here along the ground.", item->short_descr);
    STRFREE(item->description);
    item->description = STRALLOC(long_buf);
    send_to_char("&RDone!&c Please note, the KEYWORDS used to drop/equip/etc remain the same.\r\n", ch);
    save_char_obj(ch);
    return;
  }
  else if(!str_cmp(arg2, "rename") && (ch->pcdata->tradelevel < 15 || xIS_SET(ch->act, PLR_NORENAME)))
  {
    send_to_char("You must specify the new keywords, ie rename id number 'new name' or rename 'ring toering'.\r\n", ch);
    return;
  }

  if(!str_cmp(arg2, "rekey") && ch->pcdata->tradelevel > 14 && !xIS_SET(ch->act, PLR_NORENAME))
  {
    OBJ_DATA               *item;

    for(item = ch->first_carrying; item; item = item->next_content)
    {
      if(atoi(arg1) == item->guid && item->pIndexData->vnum == SMITH_PRODUCT)
        break;
    }
    if(!item)
    {
      send_to_char("You do not have any produced items to rename or rekey.\r\n", ch);
      return;
    }

    ch_printf(ch, "&cChanging item's keywords from &C%s&c to &C%s&c..\r\n", item->name, arg3);
    STRFREE(item->name);
    item->name = STRALLOC(arg3);
    send_to_char("&RDone!&c Please note, the DESCRIPTION when looking at the item remains the same.\r\n", ch);
    save_char_obj(ch);
    return;
  }
  else if(!str_cmp(arg2, "rekey") && (ch->pcdata->tradelevel < 15 || xIS_SET(ch->act, PLR_NORENAME)))
  {
    send_to_char("You must specify the new keywords, ie 'produce rekey whatever new name' or 'produce rekey sword toothpick'.\r\n", ch);
    return;
  }

  if(!str_cmp(arg1, "list"))
  {
    int                     y, count = 0;
    char                    wearloc[MSL];
    short                   value;

    send_to_char("\t\t\t&CThe Production Listing\r\n\r\n", ch);

    if(!str_cmp(arg2, "all"))
    {
      for(y = 0; y < MAX_CRAFTED; y++)
      {
        count++;
        ch_printf(ch, "&c%-15s  %-3d ", craft_one[y].name ? craft_one[y].name : "null name", craft_one[y].level);

        if(count == 3)
        {
          count = 0;
          send_to_char("\r\n", ch);
        }
      }
      if(count != 0)
        send_to_char("\r\n", ch);
      send_craft_syntax(ch);
      return;

    }
    for(y = 0; y < MAX_CRAFTED; y++)
    {
      if(ch->pcdata->tradelevel < craft_one[y].level)
        continue;
      count++;
      ch_printf(ch, "&c%-15s      ", craft_one[y].name ? craft_one[y].name : "null name");
      if(count == 3)
      {
        count = 0;
        send_to_char("\r\n", ch);
      }
    }
    if(count != 0)
      send_to_char("\r\n", ch);
    send_to_char("\r\n", ch);
    send_craft_syntax(ch);
    return;
  }

  OBJ_DATA               *bag, *obj;

  for(bag = ch->first_carrying; bag; bag = bag->next_content)
  {
    if(bag->item_type == ITEM_RESOURCE_BAG)
      break;
  }

  if(!bag)
  {
    send_to_char("You do not have a dragonic's saddle bag to produce with.\r\n", ch);
    return;
  }

  if((bag->craft1 < 1 && !str_cmp(arg1, "titanium")) ||
     (bag->craft2 < 1 && !str_cmp(arg1, "vibranium")) ||
     (bag->craft3 < 1 && !str_cmp(arg1, "adamantium")) || (bag->craft4 < 1 && !str_cmp(arg1, "carbonadium")) || (bag->craft5 < 1 && !str_cmp(arg1, "meteor")))
  {

    send_to_char("You would need to unearth more metals for your dragonic saddle bag first.\r\n", ch);
    return;
  }

  if(!str_cmp(arg1, "titanium"))
  {
    bag->craft1 -= 1;
  }
  else if(!str_cmp(arg1, "vibranium"))
  {
    bag->craft2 -= 1;
  }
  else if(!str_cmp(arg1, "adamantium"))
  {
    bag->craft3 -= 1;
  }
  else if(!str_cmp(arg1, "carbonadium"))
  {
    bag->craft4 -= 1;
  }
  else if(!str_cmp(arg1, "meteor"))
  {
    bag->craft5 -= 1;
  }
  else
  {
    send_to_char("That's not a proper metal type.\r\n", ch);
    return;
  }

  if(arg2[0] == '\0' || arg3[0] == '\0')
  {
    send_craft_syntax(ch);
    return;

  }
  if(str_cmp(arg2, "into"))
  {
    send_craft_syntax(ch);
    return;
  }

  if(!IS_SET(ch->in_room->room_flags, ROOM_TRADESKILLS))
  {
    send_to_char("You must be in a tradeskills building to do this.\r\n", ch);
    return;
  }

  {

    for(i = 0; i < MAX_CRAFTED; i++)
    {
      if(!str_cmp(arg3, craft_one[i].name))
      {
        usetan = i;
        itm = TRUE;
        break;
      }
    }

    if(!itm || ch->pcdata->tradelevel < craft_one[i].level)
    {
      send_to_char("This is not a valid produce type.\r\n", ch);
      return;
    }

    bool                    wrong = FALSE;

    for(x = 0; x < 6; x++)
    {
      if(!str_cmp(arg1, unearthed_one[x].name))
      {
        wrong = TRUE;
        break;
      }
    }

    if(!wrong || ch->pcdata->tradelevel < unearthed_one[x].level)
    {
      send_to_char("This is not a valid metal type.\r\n", ch);
      return;
    }

    if(ch->pcdata->learned[gsn_cone] == 0)
    {
      send_to_char("First you need to practice cone skill to chemically alter the metals so they are malleable.\r\n", ch);
      return;
    }

    SKILLTYPE              *skill = get_skilltype(gsn_produce);

    if(ch->pcdata->tradelevel <= 1)
      adj = "poorly";
    else if(ch->pcdata->tradelevel <= 5)
      adj = "simply";
    else if(ch->pcdata->tradelevel <= 8)
      adj = "properly";
    else if(ch->pcdata->tradelevel <= 10)
      adj = "well";
    else if(ch->pcdata->tradelevel <= 15)
      adj = "finely";
    else if(ch->pcdata->tradelevel <= 19)
      adj = "masterfully";
    else
      adj = "legendary";

    WAIT_STATE(ch, 15);
    short                   failure = 0;

    if(ch->pcdata->tradelevel < 5)
      failure = 14;
    else if(ch->pcdata->tradelevel < 10 && ch->pcdata->tradelevel > 4)
      failure = 10;
    else if(ch->pcdata->tradelevel < 15 && ch->pcdata->tradelevel > 9)
      failure = 8;
    else if(ch->pcdata->tradelevel < 20 && ch->pcdata->tradelevel > 14)
      failure = 5;
    else if(ch->pcdata->tradelevel == 20)
      failure = 2;

    if(bag->value[6] == 0)
    {
      if(number_percent() <= failure)
      {

        act(AT_CYAN, "$n prepares the metal by blasting it with your cone attack, but frown as $e realizes the metal got damaged.", ch, NULL, NULL, TO_ROOM);
        act(AT_CYAN, "You prepare the metal by blasting it with your cone attack, but you realize the metal got damaged.", ch, NULL, NULL, TO_CHAR);
        if(xIS_SET(ch->act, PLR_CRAFTS))
          send_to_char("!!SOUND(sound/cone.wav)\r\n", ch);

        learn_from_failure(ch, gsn_produce);
        return;
      }

      act(AT_CYAN, "$n prepares the metal by blasting it with your cone attack.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You prepare the metal by blasting it with your cone attack.", ch, NULL, NULL, TO_CHAR);
      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/cone.wav)\r\n", ch);

      bag->value[6] = 1;
      obj = create_object(get_obj_index(OBJ_VNUM_ORE), 0);
      if(obj->name)
        STRFREE(obj->name);
      if(obj->short_descr)
        STRFREE(obj->short_descr);
      if(obj->description)
        STRFREE(obj->description);
      obj->name = STRALLOC("metal");
      obj->description = STRALLOC("A prepared metal has been left here.");
      obj->short_descr = STRALLOC("a prepared metal");
      obj_to_char(obj, ch);
      return;
    }

    if(bag->value[6] == 1)
    {
      if(!can_use_skill(ch, number_percent(), gsn_produce) || number_percent() <= failure)
      {
        act(AT_CYAN, "$n hammers away at the metal.", ch, NULL, NULL, TO_ROOM);
        act(AT_CYAN, "You hammer away at the metal.", ch, NULL, NULL, TO_CHAR);
        if(xIS_SET(ch->act, PLR_CRAFTS))
          send_to_char("!!SOUND(sound/hammer.wav)\r\n", ch);

        learn_from_failure(ch, gsn_produce);
        return;
      }

      act(AT_CYAN, "$n places the now malleable metal on the anvil and begins to shape it with the hammer.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You place the now malleable metal on the anvil and begin to shape it with your hammer.", ch, NULL, NULL, TO_CHAR);
      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/hammer.wav)\r\n", ch);

      bag->value[6] = 2;
      for(obj = ch->first_carrying; obj; obj = obj->next_content)
      {
        if(obj->item_type == ITEM_RAW)
          break;
      }

      if(!obj)
      {
        send_to_char("You do not have your metal.\r\n", ch);
        bag->value[6] = 0;
        return;
      }

      if(VLD_STR(obj->short_descr))
        STRFREE(obj->short_descr);
      obj->short_descr = STRALLOC("a shaped metal");
      return;
    }

    if(bag->value[6] == 2)
    {
      act(AT_CYAN, "$n begins to assemble the pieces to the shaped metal.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You begin to assemble the pieces to the shaped metal.", ch, NULL, NULL, TO_CHAR);
      if(number_range(1, 4) > 2)
        learn_from_craft(ch, gsn_produce);  // add a chance of gaining exp

      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/assemble.wav)\r\n", ch);

      bag->value[6] = 3;
      for(obj = ch->first_carrying; obj; obj = obj->next_content)
      {
        if(obj->item_type == ITEM_RAW)
          break;
      }

      if(!obj)
      {
        send_to_char("You do not have your metal.\r\n", ch);
        bag->value[6] = 0;
        return;
      }

      if(VLD_STR(obj->short_descr))
        STRFREE(obj->short_descr);
      obj->short_descr = STRALLOC("a nearly assembled piece");
      return;
    }

    act(AT_CYAN, "$n sands away the rough edges of the assembled piece into a final product.", ch, NULL, NULL, TO_ROOM);
    act(AT_CYAN, "You sand away the rought edges of the assembled piece into a final product.", ch, NULL, NULL, TO_CHAR);
    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/grind2.wav)\r\n", ch);

    for(obj = ch->first_carrying; obj; obj = obj->next_content)
    {
      if(obj->item_type == ITEM_RAW)
        break;
    }

    if(!obj)
    {
      send_to_char("You do not have your metal.\r\n", ch);
      bag->value[6] = 0;
      return;
    }
    separate_obj(obj);
    obj_from_char(obj);
    extract_obj(obj);
    bag->value[6] = 0;
    learn_from_craft(ch, gsn_produce);
    item = create_object(get_obj_index(SMITH_PRODUCT), 1);
    if(arg4[0] == '\0')
    {
      item->level = ch->level;
    }
    else
      item->level = atoi(arg4);

    bool                    animal = FALSE;

    for(x = 0; x < 6; x++)
    {
      if(!str_cmp(arg1, unearthed_one[x].name))
      {
        animal = TRUE;
        break;
      }
    }

    if(!animal || ch->pcdata->tradelevel < craft_one[x].level)
    {
      send_to_char("This is not a valid item type.\r\n", ch);
      return;
    }

    sprintf(name_buf, "%s", craft_one[i].name);
    sprintf(short_buf, "%s, %s produced from %s", craft_one[i].short_descr, adj, unearthed_one[x].name);
    sprintf(long_buf, "Here lies %s, %s produced from %s.", craft_one[i].short_descr, adj, unearthed_one[x].name);
    item->item_type = craft_one[i].item_type;
    item->wear_flags += craft_one[i].wear_flags;
    item->weight = (craft_one[i].weight);
    item->cost = craft_one[i].weight;
    item->value[0] = set_min_armor(item->level);
    item->value[1] = set_max_armor(item->level);
    item->pIndexData->value[0] = set_min_armor(item->level);
    item->pIndexData->value[1] = set_max_armor(item->level);

    if(ch->carry_number + get_obj_number(item) > can_carry_n(ch))
    {
      send_to_char("You can't carry that many items, and drop the produced item.\r\n", ch);
      separate_obj(item);
      // obj_from_char( item );
      extract_obj(item);
      return;
    }

    if((ch->carry_weight + get_obj_weight(item, FALSE)) > can_carry_w(ch))
    {
      send_to_char("You can't carry that much weight, and drop the produced item.\r\n", ch);
      separate_obj(item);
      extract_obj(item);
      return;
    }

    /*
     * Random Affect applier - Code to make crafted items worth it
     */
    if(ch->pcdata->tradelevel > 4 && item->item_type != ITEM_TREASURE)
    {
      // Random affect of armor, str, con, dex, int, wis, cha, lck, hitroll,
      // damroll
      short                   imbue;

      if(item->item_type == ITEM_ARMOR)
      {
        imbue = number_range(1, 8);
      }
      else if(item->item_type == ITEM_WEAPON)
      {
        imbue = number_range(1, 8);
      }
      else
      {
        imbue = number_range(1, 8);
      }

      short                   bonus = 0;

      if(ch->pcdata->tradelevel < 10)
      {
        bonus = 1;
      }
      else if(ch->pcdata->tradelevel > 9)
      {
        bonus = 2;
      }
      if(item->level < 49)
      {
        bonus += 1;
      }
      else if(item->level > 50 && item->level < 90)
      {
        bonus += 2;
      }
      else if(item->level > 89)
      {
        bonus += 3;
      }
      if(item->item_type == ITEM_ARMOR && item->level > 60)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_HITROLL;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }

      if(imbue == 1)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_AC;
        paf->modifier = -5;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }
      else if(imbue == 2)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_STR;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }
      else if(imbue == 3)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_CON;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }
      else if(imbue == 4)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_DEX;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }
      else if(imbue == 5)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_INT;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }
      else if(imbue == 6)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_WIS;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }
      else if(imbue == 7)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_CHA;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }
      else if(imbue == 8)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_LCK;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }
      if(item->level > 70)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_DAMROLL;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }

    }

    if(item->item_type == ITEM_WEAPON)
    {
      short                   qball = 0;

      qball = weapon_bonus(ch->pcdata->tradelevel, item->level);

      CREATE(paf, AFFECT_DATA, 1);

      paf->type = sn;
      paf->duration = -1;
      paf->location = APPLY_DAMROLL;
      paf->modifier = qball;
      xCLEAR_BITS(paf->bitvector);
      LINK(paf, item->first_affect, item->last_affect, next, prev);

      CREATE(paf, AFFECT_DATA, 1);

      paf->type = sn;
      paf->duration = -1;
      paf->location = APPLY_HITROLL;
      paf->modifier = qball;
      xCLEAR_BITS(paf->bitvector);
      LINK(paf, item->first_affect, item->last_affect, next, prev);
    }

    if(ch->pcdata->tradelevel < 5)
    {
      item->value[0] = item->value[0] - 1;
      item->value[1] = item->value[1] - 1;
      GET_VALUE(item, type) = CURR_COPPER;
      item->cost = 25;
    }

    if(ch->pcdata->tradelevel < 15 && ch->pcdata->tradelevel >= 5)
    {
      CREATE(paf, AFFECT_DATA, 1);

      paf->type = sn;
      paf->duration = -1;
      paf->location = APPLY_HIT;
      paf->modifier = 1;
      xCLEAR_BITS(paf->bitvector);
      LINK(paf, item->first_affect, item->last_affect, next, prev);
      GET_VALUE(item, type) = CURR_SILVER;
      item->cost = 10;
    }

    if(ch->pcdata->tradelevel < 20 && ch->pcdata->tradelevel >= 15)
    {
      CREATE(paf, AFFECT_DATA, 1);

      paf->type = sn;
      paf->duration = -1;
      paf->location = APPLY_HIT;
      if(item->level >= 90)
        paf->modifier = 15;
      if(item->level >= 70 && item->level < 90)
        paf->modifier = 8;
      if(item->level >= 50 && item->level < 70)
        paf->modifier = 5;
      if(item->level >= 30 && item->level < 50)
        paf->modifier = 3;
      if(item->level < 30)
        paf->modifier = 2;
      xCLEAR_BITS(paf->bitvector);
      LINK(paf, item->first_affect, item->last_affect, next, prev);
      GET_VALUE(item, type) = CURR_SILVER;
      item->cost = 20;
    }

    if(ch->pcdata->tradelevel >= 20)
    {
      CREATE(paf, AFFECT_DATA, 1);

      paf->type = sn;
      paf->duration = -1;
      paf->location = APPLY_HIT;
      if(item->level >= 90)
        paf->modifier = 20;
      if(item->level >= 70 && item->level < 90)
        paf->modifier = 10;
      if(item->level >= 50 && item->level < 70)
        paf->modifier = 8;
      if(item->level >= 30 && item->level < 50)
        paf->modifier = 5;
      if(item->level < 30)
        paf->modifier = 3;
      xCLEAR_BITS(paf->bitvector);
      LINK(paf, item->first_affect, item->last_affect, next, prev);
      GET_VALUE(item, type) = CURR_GOLD;
      item->cost = 10;
    }

    int                     tier;

    if(craft_one[i].level >= 15)
      tier = 3;
    else if(craft_one[i].level > 5 && craft_one[i].level < 15)
      tier = 2;
    else
      tier = 1;

    if(item->item_type == ITEM_ARMOR)
    {
      item->size = 77;
      item->layers = 128;
      item->value[4] = 777;
      if(number_percent() > 80)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_AC;
        paf->modifier = number_range(-10, 10);
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }

      if(ch->pcdata->tradelevel < 5)
      { // first tier crafted armor
        item->value[3] = 100;

      }
      else if(ch->pcdata->tradelevel > 4 && ch->pcdata->tradelevel < 10)
      { // second 
        // 
        // 
        // tier 
        // crafted 
        // armor
        item->value[3] = 200;
      }
      else if(ch->pcdata->tradelevel > 9 && ch->pcdata->tradelevel < 20)
      { // third 
        // 
        // 
        // tier 
        // crafted 
        // armor
        item->value[3] = 300;
      }
      else if(ch->pcdata->tradelevel >= 20)
      { // fourth tier crafted armor
        item->value[3] = 400;
      }
    }

    if(item->item_type == ITEM_CONTAINER)
    {
      if(ch->pcdata->tradelevel < 5)
      {
        item->value[0] = 300;
        item->value[1] = 1;
        item->value[2] = 0;
        item->value[3] = 12;
      }
      else if(ch->pcdata->tradelevel > 4 && ch->pcdata->tradelevel < 10)
      { // second 
        // 
        // 
        // tier 
        // crafted 
        // armor
        item->value[0] = 400;
        item->value[1] = 1;
        item->value[2] = 0;
        item->value[3] = 12;
      }
      else if(ch->pcdata->tradelevel > 9 && ch->pcdata->tradelevel < 20)
      { // third 
        // 
        // 
        // tier 
        // crafted 
        // armor
        item->value[0] = 800;
        item->value[1] = 1;
        item->value[2] = 0;
        item->value[3] = 12;
      }
      else if(ch->pcdata->tradelevel >= 20)
      { // fourth tier crafted armor
        item->value[0] = 4000;
        item->value[1] = 1;
        item->value[2] = 0;
        item->value[3] = 12;
      }

    }

    if(item->item_type == ITEM_WEAR_L_WING || ITEM_WEAR_R_WING)
    {
      if(!str_cmp(arg1, "carbondadium"))
        item->layers = 64;
      if(!str_cmp(arg1, "meteor"))
        item->layers = 128;
    }

    if(item->item_type == ITEM_TREASURE)
    {
      item->value[2] = craft_one[i].base_v2;
    }

    if(item->item_type == ITEM_WEAPON)
    {
      send_to_char("Its a newly produced weapon!\r\n", ch);
      item->value[0] = 12;
      item->value[1] = set_tier_min(item->level, tier);
      item->value[2] = set_tier_max(item->level, tier);
      item->value[3] = craft_one[i].base_v3;
      item->value[4] = craft_one[i].base_v4;
      if(ch->pcdata->tradelevel < 5)
      {
        item->value[6] = 200;
      }
      else if(ch->pcdata->tradelevel > 4 && ch->pcdata->tradelevel < 10)
      {
        item->value[6] = 300;
      }
      else if(ch->pcdata->tradelevel > 9 && ch->pcdata->tradelevel < 15)
      {
        item->value[6] = 400;
      }
      else if(ch->pcdata->tradelevel > 14 && ch->pcdata->tradelevel < 20)
      {
        item->value[6] = 500;
      }
      else if(ch->pcdata->tradelevel >= 20)
      {
        item->value[6] = 600;
      }
    }
    sprintf(extra_buf,
            "\r\n&CThis produced item bears the seal of %s, the %s draconic.\r\n",
            ch->name,
            ch->pcdata->tradelevel <= 5 ? "apprentice" : ch->pcdata->tradelevel <= 10 ? "journeyman" : ch->pcdata->tradelevel <= 19 ? "expert" : ch->pcdata->tradelevel >= 20 ? "master" : "reknowned");
    if(VLD_STR(item->name))
      STRFREE(item->name);
    item->name = STRALLOC(name_buf);
    if(VLD_STR(item->short_descr))
      STRFREE(item->short_descr);
    item->short_descr = STRALLOC(short_buf);
    if(VLD_STR(item->description))
      STRFREE(item->description);
    item->description = STRALLOC(long_buf);

    EXTRA_DESCR_DATA       *ed;

    CREATE(ed, EXTRA_DESCR_DATA, 1);

    LINK(ed, item->first_extradesc, item->last_extradesc, next, prev);
    ed->keyword = STRALLOC(item->name);
    ed->description = STRALLOC(extra_buf);
    item->color = 1;
    obj_to_char(item, ch);

    if(item->item_type == ITEM_QUIVER)
      item->value[1] = 0;

    if( item->item_type == ITEM_LIGHT)
      item->value[2] = -1;


    if(item->item_type == ITEM_WEAR_HEAD || item->item_type == ITEM_WEAR_FINGER || item->item_type == ITEM_WEAR_WRIST || item->item_type == ITEM_WIELD || item->item_type == ITEM_SHEATH)
    {
      item->layers = 0;
    }

    if(item->item_type == ITEM_ARMOR && item->wear_loc == WEAR_FACE)
      item->layers = 0;

    CLAN_DATA              *clan;

    if(IS_CLANNED(ch))
    {
      clan = ch->pcdata->clan;
      ch->pcdata->clanpoints += 1;
      clan->totalpoints += 1;
      ch_printf(ch, "\r\n&G%s clan has gained a status point from your craftsmanship, now totaling %d clan status points!\r\n", clan->name, clan->totalpoints);
      save_char_obj(ch);
      save_clan(clan);
    }

    return;
  }
  tail_chain();

}

void do_mine(CHAR_DATA *ch, char *argument)
{
  OBJ_DATA               *obj;
  char                    arg1[MIL], name[MSL], shortdesc[MSL], longdesc[MSL];
  int                     color, value, cost, knows;
  short                   chance, level, slevel;

  argument = one_argument(argument, arg1);

  if(IS_NPC(ch))
    return;

  if(!VLD_STR(arg1) || !str_cmp(arg1, "list"))
  {
    send_mine_syntax(ch);
    return;
  }

  if(ch->position != POS_STANDING)
  {
    send_to_char("You should be standing to attempt this.\r\n", ch);
  }

  if(ch->in_room->sector_type != SECT_ORE)
  {
    send_to_char("You cannot mine here.\r\n", ch);
    return;
  }

  if((obj = get_eq_char(ch, WEAR_HOLD)) == NULL)
  {
    send_to_char("You are not holding a pick tool.\r\n", ch);
    return;
  }

  if(obj->item_type != ITEM_TOOL)
  {
    send_to_char("You must be holding a pick tool.\r\n", ch);
    return;
  }

  if(ch->move < 1)
  {
    send_to_char("You don't have enough move points to perform that.\r\n", ch);
    return;
  }
  else
  {
    ch->move -= 1;
  }

  chance = number_chance(1, 500);

  if(ch->pcdata->learned[gsn_mine] == 95)
  {
    chance = number_chance(1, 1000);
  }

  if(chance == 3 && obj->pIndexData->vnum != 41002 && !IS_AFFECTED(ch, AFF_BOOST))
  {
    send_to_char("Your pick axe suddenly snapped in two!\r\n", ch);
    make_scraps(obj);
      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/snap.wav)\r\n", ch);
    return;
  }

  // Add a further check so if not a jeweler can't mine jewels
  if(ch->pcdata->tradeclass != 20 && ch->pcdata->tradeclass != 24)
  {
    send_to_char("You are not a jeweler or miner tradeclass.\r\n", ch);
    return;
  }

  if(ch->pcdata->tradeclass == 20)
  {
    if((ch->pcdata->tradelevel < 3) && (!str_cmp(arg1, "silver") || !str_cmp(arg1, "gold") || !str_cmp(arg1, "iron") || !str_cmp(arg1, "steel") || !str_cmp(arg1, "titanium")))
    {
      send_to_char("You don't have the skill to mine that yet.", ch);
      return;
    }
    if((ch->pcdata->tradelevel < 5) && (!str_cmp(arg1, "gold") || !str_cmp(arg1, "iron") || !str_cmp(arg1, "steel") || !str_cmp(arg1, "titanium")))
    {
      send_to_char("You don't have the skill to mine that yet.", ch);
      return;
    }
    if((ch->pcdata->tradelevel < 8) && (!str_cmp(arg1, "iron") || !str_cmp(arg1, "steel") || !str_cmp(arg1, "titanium")))
    {
      send_to_char("You don't have the skill to mine that yet.", ch);
      return;
    }
    if((ch->pcdata->tradelevel < 10) && (!str_cmp(arg1, "steel") || !str_cmp(arg1, "titanium")))
    {
      send_to_char("You don't have the skill to mine that yet.", ch);
      return;
    }
    if((ch->pcdata->tradelevel < 15) && (!str_cmp(arg1, "titanium")))
    {
      send_to_char("You don't have the skill to mine that yet.", ch);
      return;
    }
  }
  // Add a further check so if not a jeweler can't mine jewels
  if((ch->pcdata->tradeclass == 20)
     && (!str_cmp(arg1, "amethyst") || !str_cmp(arg1, "pearl") || !str_cmp(arg1, "emerald") || !str_cmp(arg1, "ruby") || !str_cmp(arg1, "sapphire") || !str_cmp(arg1, "diamond")))
  {
    send_to_char("You are not a jeweler tradeclass.\r\n", ch);
    return;
  }

  if((ch->pcdata->tradeclass == 24) && (!str_cmp(arg1, "iron") || !str_cmp(arg1, "steel") || !str_cmp(arg1, "titanium")))
  {
    send_to_char("You don't have the skill to mine that yet.", ch);
    return;
  }
  if((ch->pcdata->tradelevel < 10) && (!str_cmp(arg1, "steel") || !str_cmp(arg1, "titanium")))
  {
    send_to_char("You don't have the skill to mine that yet.", ch);
    return;
  }

  if((ch->pcdata->tradelevel < 3)
     && (!str_cmp(arg1, "silver") || !str_cmp(arg1, "gold") || !str_cmp(arg1, "iron")
         || !str_cmp(arg1, "steel")
         || !str_cmp(arg1, "titanium") || !str_cmp(arg1, "pearl") || !str_cmp(arg1, "emerald") || !str_cmp(arg1, "ruby") || !str_cmp(arg1, "sapphire") || !str_cmp(arg1, "diamond")))
  {
    send_to_char("You don't have the skill to mine that yet.", ch);
    return;
  }
  if((ch->pcdata->tradelevel < 5)
     && (!str_cmp(arg1, "gold") || !str_cmp(arg1, "iron") || !str_cmp(arg1, "steel")
         || !str_cmp(arg1, "titanium") || !str_cmp(arg1, "emerald") || !str_cmp(arg1, "ruby") || !str_cmp(arg1, "sapphire") || !str_cmp(arg1, "diamond")))
  {
    send_to_char("You don't have the skill to mine that yet.", ch);
    return;
  }
  if((ch->pcdata->tradelevel < 8)
     && (!str_cmp(arg1, "iron") || !str_cmp(arg1, "steel") || !str_cmp(arg1, "titanium") || !str_cmp(arg1, "ruby") || !str_cmp(arg1, "sapphire") || !str_cmp(arg1, "diamond")))
  {
    send_to_char("You don't have the skill to mine that yet.", ch);
    return;
  }
  if((ch->pcdata->tradelevel < 10) && (!str_cmp(arg1, "steel") || !str_cmp(arg1, "titanium") || !str_cmp(arg1, "sapphire") || !str_cmp(arg1, "diamond")))
  {
    send_to_char("You don't have the skill to mine that yet.", ch);
    return;
  }
  if((ch->pcdata->tradelevel < 15) && (!str_cmp(arg1, "titanium") || !str_cmp(arg1, "diamond")))
  {
    send_to_char("You don't have the skill to mine that yet.", ch);
    return;
  }

  if(!str_cmp(arg1, "bronze") || !str_cmp(arg1, "amethyst"))
  {
    knows = 15;
    if(ch->pcdata->tradeclass == 20)
    {
      level = 0;
    }
    else
    {
      slevel = 0;
    }
  }
  else if(!str_cmp(arg1, "silver") || !str_cmp(arg1, "pearl"))
  {
    knows = 40;
    if(ch->pcdata->tradeclass == 20)
    {
      level = 3;
    }
    else
    {
      slevel = 3;
    }
  }
  else if(!str_cmp(arg1, "gold") || !str_cmp(arg1, "emerald"))
  {
    knows = 55;
    if(ch->pcdata->tradeclass == 20)
    {
      level = 5;
    }
    else
    {
      slevel = 5;
    }
  }
  else if(!str_cmp(arg1, "iron") || !str_cmp(arg1, "ruby"))
  {
    knows = 65;
    if(ch->pcdata->tradeclass == 20)
    {
      level = 8;
    }
    else
    {
      slevel = 8;
    }
  }
  else if(!str_cmp(arg1, "steel") || !str_cmp(arg1, "sapphire"))
  {
    knows = 85;
    if(ch->pcdata->tradeclass == 20)
    {
      level = 10;
    }
    else
    {
      slevel = 10;
    }

  }
  else if(!str_cmp(arg1, "titanium") || !str_cmp(arg1, "diamond"))
  {
    knows = 95;
    if(ch->pcdata->tradeclass == 20)
    {
      level = 15;
    }
    else
    {
      slevel = 15;
    }

  }

  OBJ_DATA               *bag;

  for(bag = ch->first_carrying; bag; bag = bag->next_content)
  {
    if(bag->item_type == ITEM_RESOURCE_BAG && bag->value[0] != 500) // either
      // change
      // that to
      // accept
      // craft1-20 or change vnum of bag
      break;
  }

  if(!bag)
  {
    send_to_char("You do not have a miner's bag to hold anything with.\r\n", ch);
    return;
  }

  if(bag->craft1 >= 1000 && !str_cmp(arg1, "bronze"))
  {
    send_to_char("You cannot hold any more bronze ore in your miner's bag.\r\n", ch);
    return;
  }
  else if(bag->craft2 >= 1000 && !str_cmp(arg1, "silver"))
  {
    send_to_char("You cannot hold any more silver ore in your miner's bag.\r\n", ch);
    return;
  }
  else if(bag->craft3 >= 1000 && !str_cmp(arg1, "gold"))
  {
    send_to_char("You cannot hold any more gold ore in your miner's bag.\r\n", ch);
    return;
  }
  else if(bag->craft4 >= 1000 && !str_cmp(arg1, "iron"))
  {
    send_to_char("You cannot hold any more iron ore in your miner's bag.\r\n", ch);
    return;
  }
  else if(bag->craft5 >= 1000 && !str_cmp(arg1, "steel"))
  {
    send_to_char("You cannot hold any more steel ore in your miner's bag.\r\n", ch);
    return;
  }
  else if(bag->craft6 >= 1000 && !str_cmp(arg1, "titanium"))
  {
    send_to_char("You cannot hold any more titanium ore in your miner's bag.\r\n", ch);
    return;
  }
  else if(bag->craft7 >= 1000 && !str_cmp(arg1, "amethyst"))
  {
    send_to_char("You cannot hold any more amethyst gems in your miner's bag.\r\n", ch);
    return;
  }
  else if(bag->craft8 >= 1000 && !str_cmp(arg1, "pearl"))
  {
    send_to_char("You cannot hold any more pearl gems in your miner's bag.\r\n", ch);
    return;
  }
  else if(bag->craft9 >= 1000 && !str_cmp(arg1, "emerald"))
  {
    send_to_char("You cannot hold any more emerald gems in your miner's bag.\r\n", ch);
    return;
  }
  else if(bag->craft10 >= 1000 && !str_cmp(arg1, "ruby"))
  {
    send_to_char("You cannot hold any more ruby gems in your miner's bag.\r\n", ch);
    return;
  }
  else if(bag->craft11 >= 1000 && !str_cmp(arg1, "sapphire"))
  {
    send_to_char("You cannot hold any more sapphire gems in your miner's bag.\r\n", ch);
    return;
  }
  else if(bag->craft12 >= 1000 && !str_cmp(arg1, "diamond"))
  {
    send_to_char("You cannot hold any more diamond gems in your miner's bag.\r\n", ch);
    return;
  }

  if(ch->move < 5)
  {
    send_to_char("You don't have enough energy to keep mining.\r\n", ch);
    return;
  }

  if(!str_cmp(arg1, "gold") || !str_cmp(arg1, "silver") || !str_cmp(arg1, "bronze") || !str_cmp(arg1, "iron") || !str_cmp(arg1, "steel") || !str_cmp(arg1, "amethyst") || !str_cmp(arg1, "pearl") || !str_cmp(arg1, "emerald") || !str_cmp(arg1, "ruby") || !str_cmp(arg1, "sapphire"));  /* Does 
                                                                                                                                                                                                                                                                                           * nothing 
                                                                                                                                                                                                                                                                                           * and 
                                                                                                                                                                                                                                                                                           * keeps 
                                                                                                                                                                                                                                                                                           * going 
                                                                                                                                                                                                                                                                                           */
  else if(!str_cmp(arg1, "titanium") || !str_cmp(arg1, "diamond"))
    chance = number_chance(1, 5); /* More chance of failing on this */
  else
  { /* If none of those then give a message */

    send_mine_syntax(ch);
    return;
  }

  WAIT_STATE(ch, skill_table[gsn_mine]->beats);

  if(!can_use_skill(ch, number_percent(), gsn_mine))
  {
    learn_from_failure(ch, gsn_mine);
    send_to_char("You cannot find anything of worth from your mining.\r\n", ch);
    return;
  }
  if(xIS_SET(ch->act, PLR_CRAFTS))
    send_to_char("!!SOUND(sound/tinktinktink.wav)\r\n", ch);

  ch->move -= 1;

  chance = number_chance(1, 10);

  if(chance < 3)
  {
    if(chance <= 1)
    {
      act(AT_ORANGE, "$n's mining only digs up a pile of dirt.\r\n", ch, NULL, NULL, TO_ROOM);
      send_to_char("&OYour mining only digs up a pile of dirt.\r\n", ch);
    }
    else if(chance >= 2)
    {
      act(AT_CYAN, "$n begins to labor in earnest chipping away at the rock with $s pick axe.\r\n", ch, NULL, NULL, TO_ROOM);
      send_to_char("&cYou begin to labor in earnest chipping away at the rock.\r\n", ch);
    }
    return;
  }

  if(ch->pcdata->tradeclass == 24)
  {
    if(ch->pcdata->learned[gsn_mine] < knows || ch->pcdata->tradelevel < slevel)
    {
      if(!str_cmp(arg1, "amethyst") || !str_cmp(arg1, "pearl") || !str_cmp(arg1, "emerald") || !str_cmp(arg1, "ruby") || !str_cmp(arg1, "sapphire") || !str_cmp(arg1, "diamond"))
      {
        send_to_char("You begin to mine.\r\n", ch);
        pager_printf(ch, "&GYour mining pays off as you successfully unearthed a gem!\r\n");
        act(AT_GREEN, "$n's mining suddenly unearths a gem.", ch, NULL, NULL, TO_ROOM);
        pager_printf(ch, "&GYour lack of mining skills has ruined the gem with a split in it!\r\n");
        act(AT_GREEN, "$n's looks away in disgust as $e realizes the gem is ruined.", ch, NULL, NULL, TO_ROOM);
        learn_from_failure(ch, gsn_mine);
        return;
      }
    }
  }
  if(ch->pcdata->tradeclass == 20)
  {
    if(ch->pcdata->learned[gsn_mine] < knows || ch->pcdata->tradelevel < slevel)
    {
      if(!str_cmp(arg1, "bronze") || !str_cmp(arg1, "silver") || !str_cmp(arg1, "gold") || !str_cmp(arg1, "iron") || !str_cmp(arg1, "steel") || !str_cmp(arg1, "titanium"))
      {
        send_to_char("You begin to mine for ore.\r\n", ch);
        pager_printf(ch, "&GYour mining pays off as you successfully unearthed some ore!\r\n");
        act(AT_GREEN, "$n's mining suddenly unearths some ore.", ch, NULL, NULL, TO_ROOM);
        pager_printf(ch, "&GYour lack of mining skills has ruined the ore with a split in the ore!\r\n");
        act(AT_GREEN, "$n's looks away in disgust as $e realizes the ore is ruined.", ch, NULL, NULL, TO_ROOM);
        learn_from_failure(ch, gsn_mine);
        return;
      }
    }
  }
  short                   amount = 0, extra = 0;

  amount = 1;
  extra = number_range(1, 4);
  if(ch->pcdata->tradelevel > 9 && extra == 2)
  {
    amount += 1;
  }
  if(ch->pcdata->tradelevel > 14 && extra == 2)
  {
    amount += 1;
  }

  if(!str_cmp(arg1, "bronze"))
  {
    bag->craft1 += amount;
  }
  else if(!str_cmp(arg1, "silver"))
  {
    bag->craft2 += amount;
  }
  else if(!str_cmp(arg1, "gold"))
  {
    bag->craft3 += amount;
  }
  else if(!str_cmp(arg1, "iron"))
  {
    bag->craft4 += amount;
  }
  else if(!str_cmp(arg1, "steel"))
  {
    bag->craft5 += amount;
  }
  else if(!str_cmp(arg1, "titanium"))
  {
    bag->craft6 += amount;
  }
  else if(!str_cmp(arg1, "amethyst"))
  {
    bag->craft7 += amount;
  }
  else if(!str_cmp(arg1, "pearl"))
  {
    bag->craft8 += amount;
  }
  else if(!str_cmp(arg1, "emerald"))
  {
    bag->craft9 += amount;
  }
  else if(!str_cmp(arg1, "ruby"))
  {
    bag->craft10 += amount;
  }
  else if(!str_cmp(arg1, "sapphire"))
  {
    bag->craft11 += amount;
  }
  else if(!str_cmp(arg1, "diamond"))
  {
    bag->craft12 += amount;
  }
  send_to_char("You begin to mine.\r\n", ch);
  if(!str_cmp(arg1, "amethyst") || !str_cmp(arg1, "pearl") || !str_cmp(arg1, "emerald") || !str_cmp(arg1, "ruby") || !str_cmp(arg1, "sapphire") || !str_cmp(arg1, "diamond"))
  {
    ch_printf(ch, "&GYour mining pays off as you successfully unearthed %d %s gem!\r\n", amount, arg1);
  }
  else
  {
    ch_printf(ch, "&GYour mining pays off as you successfully unearthed %d %s ore!\r\n", amount, arg1);
  }
  act(AT_GREEN, "$n's mining suddenly unearths something.", ch, NULL, NULL, TO_ROOM);
  learn_from_craft(ch, gsn_mine);
}

void send_jewelry_syntax(CHAR_DATA *ch)
{
  if(!ch)
    return;
  send_to_char("\r\n", ch);
  if(IS_BLIND(ch))
  {
    send_to_char("Type jewelry list to see what styles are available.\r\n", ch);
    send_to_char("Type jewelry metal gem style level\r\n", ch);

    if(ch->pcdata->tradelevel > 9)
    {
      send_to_char("Type jewelry Id number repair\r\n", ch);
      send_to_char("eg. jewelry 3232 repair\r\n", ch);
    }
    if(ch->pcdata->tradelevel > 14)
    {
      send_to_char("Syntax: jewelry id number rename '&Cnew name&c' or jewelry id number rekey '&Cnew key&c'\r\n", ch);
    }

    send_to_char("Note There are 4 steps, keep doing the jewelry command until the last step.\r\n", ch);
  }
  else
  {
    send_to_char("&cSyntax: jewelry list - displays styles\r\n", ch);
    send_to_char("&cSyntax: jewelry <&Cmetal&c> <&Cgem&c> <&Cstyle&c> <&Clevel&c>\r\n", ch);
    if(ch->pcdata->tradelevel > 9)
    {
      send_to_char("Syntax: jewelry Id# repair\r\n", ch);
      send_to_char("eg. jewelry 3232 repair\r\n", ch);
    }
    if(ch->pcdata->tradelevel > 14)
    {
      send_to_char("Syntax: jewelry id# rename '&Cnew name&c' or jewelry id# rekey '&Cnew key&c'\r\n", ch);

    }
    send_to_char("Note: There are &C4 steps&c, keep doing the jewelry command until the last step.\r\n", ch);
  }
  return;
}

void do_jewelry(CHAR_DATA *ch, char *argument)
{
  short                   number;
  int                     item_type, sn = 1;
  AFFECT_DATA            *paf;
  char                    arg1[MIL];
  char                    arg2[MIL];
  char                    arg3[MIL];
  char                    arg4[MIL];
  bool                    itm = FALSE;
  int                     i = 0;
  char                    name_buf[MSL];
  char                    short_buf[MSL];
  char                    long_buf[MSL];
  char                    extra_buf[MSL];
  const char             *adj;
  OBJ_DATA               *obj, *sobj;

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  argument = one_argument(argument, arg3);
  argument = one_argument(argument, arg4);

  if(IS_NPC(ch))
    return;

  if(!arg1 || arg1[0] == '\0')
  {
    send_jewelry_syntax(ch);
    return;
  }

  if(!str_cmp(arg1, "list"))
  {
    send_to_char("&CThe Listing for Jewelry&c\r\n\r\n", ch);
    if(!str_cmp(arg2, "all"))
    {
      for(int y = 0; y < MAX_JEWELRY; y++)
      {
        ch_printf(ch, "&c%-18s    ", jewelry_one[y].name ? jewelry_one[y].name : "null name");
        if((y + 1) % 4 == 0)
          send_to_char("\r\n", ch);
      }

    }
    else
    {
      for(int y = 0; y < MAX_JEWELRY; y++)
      {
        if(ch->pcdata->tradelevel < jewelry_one[y].level)
          continue;
        ch_printf(ch, "&c%-15s      ", jewelry_one[y].name ? jewelry_one[y].name : "null name");
        if((y + 1) % 4 == 0)
          send_to_char("\r\n", ch);
      }
    }
    send_to_char("\r\n", ch);
    send_jewelry_syntax(ch);
    return;
  }

  if(ch->position != POS_STANDING)
  {
    send_to_char("You should be standing to attempt this.\r\n", ch);
  }

  if(ch->move < 1)
  {
    send_to_char("You don't have enough move points to perform that.\r\n", ch);
    return;
  }
  else
  {
    ch->move -= 1;
  }

// Just adding objID to work for repair code section 

  if(!str_cmp(arg2, "repair"))
  {
    OBJ_DATA               *repair;

    for(repair = ch->first_carrying; repair; repair = repair->next_content)
    {
      if(atoi(arg1) == repair->guid && (repair->item_type == ITEM_ARMOR || repair->item_type == ITEM_WEAPON))
        break;
    }

    if(!repair)
    {
      send_to_char("You do not have any repairable equipment to repair.\r\n", ch);
      return;
    }

    if(ch->pcdata->tradelevel < 10)
    {
      send_to_char("You are not skilled enough of a craftsman to repair items yet.\r\n", ch);
      return;
    }
    if(repair->item_type == ITEM_WEAPON && repair->value[0] == 12)
    {
      send_to_char("It is already honed to perfection.\r\n", ch);
      return;
    }
    if(repair->item_type == ITEM_ARMOR && repair->value[0] == repair->value[1])
    {
      send_to_char("It is already forged to perfection.\r\n", ch);
      return;
    }

    act(AT_CYAN, "$n hammers $p back to shape.", ch, repair, NULL, TO_ROOM);
    act(AT_CYAN, "You hammer $p back to shape.", ch, repair, NULL, TO_CHAR);
    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/hammer.wav)\r\n", ch);

    switch (repair->item_type)
    {
      case ITEM_ARMOR:
        repair->value[0] = repair->value[1];
        break;
      case ITEM_WEAPON:
        repair->value[0] = INIT_WEAPON_CONDITION;
        break;
    }
    return;
  }

// End of repair code section 

  if(!str_cmp(arg2, "rename") && ch->pcdata->tradelevel > 14 && !xIS_SET(ch->act, PLR_NORENAME))
  {
    OBJ_DATA               *item;

    for(item = ch->first_carrying; item; item = item->next_content)
    {
      if(atoi(arg1) == item->guid && item->pIndexData->vnum == SMITH_PRODUCT)
        break;
    }
    if(!item)
    {
      send_to_char("You do not have any jewelry items to rename or rekey.\r\n", ch);
      return;
    }

    if(!arg3 || arg3[0] == '\0')
    {
      send_to_char("You must specify the new keywords, ie jewelry id number rename 'new name' or rename 'ring toering'.\r\n", ch);
      return;
    }
    ch_printf(ch, "&cChanging item's description from &C%s&c to&C %s&c..\r\n", item->short_descr, arg3);

    STRFREE(item->short_descr);
    item->short_descr = STRALLOC(arg3);
    sprintf(long_buf, "%s has been left here along the ground.", item->short_descr);
    STRFREE(item->description);
    item->description = STRALLOC(long_buf);
    send_to_char("&RDone!&c Please note, the KEYWORDS used to drop/equip/etc remain the same.\r\n", ch);
    save_char_obj(ch);
    return;
  }
  else if(!str_cmp(arg2, "rename") && (ch->pcdata->tradelevel < 15 || xIS_SET(ch->act, PLR_NORENAME)))
  {
    send_to_char("You must specify the new keywords, ie rename id number 'new name' or rename 'ring toering'.\r\n", ch);
    return;
  }
  if(!str_cmp(arg2, "rekey") && ch->pcdata->tradelevel > 14 && !xIS_SET(ch->act, PLR_NORENAME))
  {
    OBJ_DATA               *item;

    for(item = ch->first_carrying; item; item = item->next_content)
    {
      if(atoi(arg1) == item->guid && item->pIndexData->vnum == SMITH_PRODUCT)
        break;
    }
    if(!item)
    {
      send_to_char("You do not have any jewelry items to rename or rekey.\r\n", ch);
      return;
    }
    ch_printf(ch, "&cChanging item's keywords from &C%s&c to &C%s&c..\r\n", item->name, arg3);
    STRFREE(item->name);
    item->name = STRALLOC(arg3);
    send_to_char("&RDone!&c Please note, the DESCRIPTION when looking at the item remains the same.\r\n", ch);
    save_char_obj(ch);
    return;
  }
  else if(!str_cmp(arg2, "rekey") && (ch->pcdata->tradelevel < 15 || xIS_SET(ch->act, PLR_NORENAME)))
  {
    send_to_char("You must specify the new keywords, ie 'jewelry rekey whatever new name' or 'jewelry rekey sword toothpick'.\r\n", ch);
    return;
  }

  OBJ_DATA               *bag;

  for(bag = ch->first_carrying; bag; bag = bag->next_content)
  {
    if(bag->item_type == ITEM_RESOURCE_BAG && bag->value[0] != 500)
      break;
  }

  if(!bag)
  {
    send_to_char("You do not have an miner's bag to jewelry with.\r\n", ch);
    return;
  }

  if(!str_cmp(arg1, "gold") && bag->craft3 < 1)
  {
    send_to_char("You don't have enough gold ore for that.\r\n", ch);
    return;
  }
  else if(!str_cmp(arg1, "gold") && bag->craft3 > 0)
  {
    bag->craft3 -= 1;
  }
  if(!str_cmp(arg1, "silver") && bag->craft2 < 1)
  {
    send_to_char("You don't have enough silver ore for that.\r\n", ch);
    return;
  }
  else if(!str_cmp(arg1, "silver") && bag->craft2 > 0)
  {
    bag->craft2 -= 1;
  }
  if(!str_cmp(arg1, "bronze") && bag->craft1 < 1)
  {
    send_to_char("You don't have enough bronze ore for that.\r\n", ch);
    return;
  }
  else if(!str_cmp(arg1, "bronze") && bag->craft1 > 0)
  {
    bag->craft1 -= 1;
  }
  if(!str_cmp(arg1, "iron") && bag->craft4 < 1)
  {
    send_to_char("You don't have enough iron ore for that.\r\n", ch);
    return;
  }
  else if(!str_cmp(arg1, "iron") && bag->craft4 > 0)
  {
    bag->craft4 -= 1;
  }
  if(!str_cmp(arg1, "steel") && bag->craft5 < 1)
  {
    send_to_char("You don't have enough steel ore for that.\r\n", ch);
    return;
  }
  else if(!str_cmp(arg1, "steel") && bag->craft5 > 0)
  {
    bag->craft5 -= 1;
  }
  if(!str_cmp(arg1, "titanium") && bag->craft6 < 1)
  {
    send_to_char("You don't have enough titanium ore for that.\r\n", ch);
    return;
  }
  else if(!str_cmp(arg1, "titanium") && bag->craft6 > 0)
  {
    bag->craft6 -= 1;
  }
  if(!str_cmp(arg2, "amethyst") && bag->craft7 < 1)
  {
    send_to_char("You don't have enough amethysts for that.\r\n", ch);
    return;
  }
  else if(!str_cmp(arg2, "amethyst") && bag->craft7 > 0)
  {
    bag->craft7 -= 1;
  }
  if(!str_cmp(arg2, "pearl") && bag->craft8 < 1)
  {
    send_to_char("You don't have enough pearls for that.\r\n", ch);
    return;
  }
  else if(!str_cmp(arg2, "pearl") && bag->craft8 > 0)
  {
    bag->craft8 -= 1;
  }
  if(!str_cmp(arg2, "emerald") && bag->craft9 < 1)
  {
    send_to_char("You don't have enough emeralds for that.\r\n", ch);
    return;
  }
  else if(!str_cmp(arg2, "emerald") && bag->craft9 > 0)
  {
    bag->craft9 -= 1;
  }
  if(!str_cmp(arg2, "ruby") && bag->craft10 < 1)
  {
    send_to_char("You don't have enough rubys for that.\r\n", ch);
    return;
  }
  else if(!str_cmp(arg2, "ruby") && bag->craft10 > 0)
  {
    bag->craft10 -= 1;
  }
  if(!str_cmp(arg2, "sapphire") && bag->craft11 < 1)
  {
    send_to_char("You don't have enough sapphires for that.\r\n", ch);
    return;
  }
  else if(!str_cmp(arg2, "sapphire") && bag->craft11 > 0)
  {
    bag->craft11 -= 1;
  }

  if(!str_cmp(arg2, "diamond") && bag->craft12 < 1)
  {
    send_to_char("You don't have enough diamonds for that.\r\n", ch);
    return;
  }
  else if(!str_cmp(arg2, "diamond") && bag->craft12 > 0)
  {
    bag->craft12 -= 1;
  }

  if(!IS_SET(ch->in_room->room_flags, ROOM_TRADESKILLS))
  {
    send_to_char("You must be in a tradeskills building to do this.\r\n", ch);
    return;
  }

  // This is the nitty gritty of checking tier output
  i = gem_lookup(arg3);
  if(i < 0 || ch->pcdata->tradelevel < jewelry_one[i].level)
  {
    send_to_char("That isn't a something you can create.\r\n", ch);
    return;
  }

  SKILLTYPE              *skill = get_skilltype(gsn_jewelry);

  if(ch->pcdata->tradelevel <= 1)
    adj = "poorly";
  else if(ch->pcdata->tradelevel <= 5)
    adj = "simply";
  else if(ch->pcdata->tradelevel <= 8)
    adj = "properly";
  else if(ch->pcdata->tradelevel <= 10)
    adj = "well";
  else if(ch->pcdata->tradelevel <= 15)
    adj = "finely";
  else if(ch->pcdata->tradelevel <= 19)
    adj = "masterfully";
  else
    adj = "legendary";

  WAIT_STATE(ch, 15);

  short                   failure = 0;

  if(ch->pcdata->tradelevel < 5)
    failure = 14;
  else if(ch->pcdata->tradelevel < 10 && ch->pcdata->tradelevel > 4)
    failure = 10;
  else if(ch->pcdata->tradelevel < 15 && ch->pcdata->tradelevel > 9)
    failure = 8;
  else if(ch->pcdata->tradelevel < 20 && ch->pcdata->tradelevel > 14)
    failure = 5;
  else if(ch->pcdata->tradelevel == 20)
    failure = 2;

// Ok here is the nitty gritty jewelry steps to make final product
  if(bag->value[1] < 1)
  {
    if(!can_use_skill(ch, number_percent(), gsn_jewelry) || number_percent() <= failure)
    {
      act(AT_CYAN, "$n begins to melt the metal ore into the proper style.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You begin to melt the metal ore into the proper style.", ch, NULL, NULL, TO_CHAR);
      send_to_char("\r\nYou realize you measured wrong and have to start over.\r\n", ch);
      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/bellowpump.wav)\r\n", ch);

      learn_from_failure(ch, gsn_jewelry);
      return;
    }

    bag->value[1] = 1;  // first step completed
    act(AT_CYAN, "$n begins to melt the metal ore into the proper style.", ch, NULL, NULL, TO_ROOM);
    act(AT_CYAN, "You begin to melt the metal ore into the proper style.", ch, NULL, NULL, TO_CHAR);
    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/bellowpump.wav)\r\n", ch);

    obj = create_object(get_obj_index(OBJ_VNUM_ORE), 0);
    if(obj->name)
      STRFREE(obj->name);
    if(obj->short_descr)
      STRFREE(obj->short_descr);
    if(obj->description)
      STRFREE(obj->description);
    obj->name = STRALLOC("item");
    obj->description = STRALLOC("A partially assembled item has been left here.");
    obj->short_descr = STRALLOC("a partially assembled item");
    obj_to_char(obj, ch);
    return;
  }

  if(bag->value[1] == 1)
  {
    for(obj = ch->first_carrying; obj; obj = obj->next_content)
    {
      if(obj->item_type == ITEM_RAW)
        break;
    }
    if(!obj)
    {
      send_to_char("You do not have the item in your inventory.\r\n", ch);
      bag->value[1] = 0;
      return;
    }

    if(number_percent() <= failure)
    {
      act(AT_CYAN, "$n takes the gem and begins to cut it to the appropriate size.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You take the gem and begin to cut it to the appropriate size.", ch, NULL, NULL, TO_CHAR);
      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/tinktinktink.wav)\r\n", ch);

      send_to_char("\r\nYour inexperience shows as cracked your gem rendering it useless.\r\n", ch);
      separate_obj(obj);
      obj_from_char(obj);
      extract_obj(obj);
      bag->value[1] = 0;
      WAIT_STATE(ch, 15);
      learn_from_failure(ch, gsn_jewelry);
      return;
    }
    act(AT_CYAN, "$n takes the gem and begins to cut it to the appropriate size.", ch, NULL, NULL, TO_ROOM);
    act(AT_CYAN, "You take the gem and begin to cut it to the appropriate size.", ch, NULL, NULL, TO_CHAR);
    if(number_range(1, 4) > 2)
      learn_from_craft(ch, gsn_jewelry);  // add a chance of gaining exp

    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/tinktinktink.wav)\r\n", ch);

    // second step
    bag->value[1] = 2;
    if(obj->short_descr)
      STRFREE(obj->short_descr);
    obj->short_descr = STRALLOC("a halfway assembled item");
    WAIT_STATE(ch, 15);
    return;
  }

  if(bag->value[1] == 2)
  {
    for(obj = ch->first_carrying; obj; obj = obj->next_content)
    {
      if(obj->item_type == ITEM_RAW)
        break;
    }
    if(!obj)
    {
      send_to_char("You do not have the item in your inventory.\r\n", ch);
      bag->value[1] = 0;
      return;
    }

    if(!can_use_skill(ch, number_percent(), gsn_jewelry) || number_percent() <= failure)
    {
      act(AT_CYAN, "$n carefully attempts to set the gem into the style.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You carefully attempt to set the gem into the style.", ch, NULL, NULL, TO_CHAR);

      send_to_char("\r\nYou curse as you suddenly lose your grip and the gem and style fall into the smelter.\r\n", ch);
      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/break1.wav)\r\n", ch);

      WAIT_STATE(ch, 15);
      learn_from_failure(ch, gsn_jewelry);
      return;
    }
    // third step
    act(AT_CYAN, "$n carefully attempts to set the gem into the style.", ch, NULL, NULL, TO_ROOM);
    act(AT_CYAN, "You carefully attempt to set the gem into the style.", ch, NULL, NULL, TO_CHAR);
    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/setting.wav)\r\n", ch);

    bag->value[1] = 3;
    if(obj->short_descr)
      STRFREE(obj->short_descr);
    obj->short_descr = STRALLOC("a nearly assembled item");
    WAIT_STATE(ch, 15);
    return;
  }

  if(bag->value[1] == 3)
  {
    for(obj = ch->first_carrying; obj; obj = obj->next_content)
    {
      if(obj->item_type == ITEM_RAW)
        break;
    }
    if(!obj)
    {
      bag->value[1] = 0;
      send_to_char("You do not have the item in your inventory.\r\n", ch);
      return;
    }

    if(number_percent() <= failure)
    {
      act(AT_CYAN, "$n assembles the final pieces and begins to polish it.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You assemble the final pieces and begin to polish it.", ch, NULL, NULL, TO_CHAR);
      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/assemble.wav)\r\n", ch);

      send_to_char("\r\nYou sigh as you realize it has a imperfection that will require extra work.\r\n", ch);
      WAIT_STATE(ch, 15);
      learn_from_failure(ch, gsn_jewelry);
      return;
    }

    // fourth step
    act(AT_CYAN, "$n assembles the final pieces and begins to polish it.", ch, NULL, NULL, TO_ROOM);
    act(AT_CYAN, "You assemble the final pieces and begin to polish it.", ch, NULL, NULL, TO_CHAR);
    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/assemble.wav)\r\n", ch);

    bag->value[1] = 0;
    learn_from_craft(ch, gsn_jewelry);
    separate_obj(obj);
    obj_from_char(obj);
    extract_obj(obj);

    if(!str_cmp(arg3, "pauldrons"))
    {
      sobj = create_object(get_obj_index(SMITH_PRODUCT), 0);
      if(sobj->name)
        STRFREE(sobj->name);
      if(sobj->short_descr)
        STRFREE(sobj->short_descr);
      if(sobj->description)
        STRFREE(sobj->description);
      sprintf(name_buf, "%s", jewelry_one[i].name);
      sprintf(short_buf, "a %s %s %s", arg1, arg2, jewelry_one[i].name);
      sprintf(long_buf, "A %s %s %s has been left here.", arg1, arg2, jewelry_one[i].name);
      sobj->name = STRALLOC(name_buf);
      sobj->short_descr = STRALLOC(short_buf);
      sobj->description = STRALLOC(long_buf);

      sobj->item_type = jewelry_one[i].item_type;
      SET_BIT(sobj->wear_flags, ITEM_TAKE);
      SET_BIT(sobj->wear_flags, ITEM_WEAR_SHOULDERS); // figure this out
      if(arg4[0] == '\0')
        sobj->level = ch->level;
      else
        sobj->level = atoi(arg4);
      sobj->weight = (jewelry_one[i].weight);
      sobj->cost = jewelry_one[i].cost;
      sobj->value[0] = jewelry_one[i].base_v0;
      sobj->value[1] = jewelry_one[i].base_v1;
      sobj->value[2] = jewelry_one[i].base_v2;
      sobj->value[3] = jewelry_one[i].base_v3;
      sobj->value[4] = jewelry_one[i].base_v4;
      sobj->value[5] = jewelry_one[i].base_v5;
      sobj->value[6] = jewelry_one[i].base_v6;
      sobj->pIndexData->value[0] = jewelry_one[i].base_v0;
      sobj->pIndexData->value[1] = jewelry_one[i].base_v1;
      sobj->pIndexData->value[2] = jewelry_one[i].base_v2;
      sobj->pIndexData->value[3] = jewelry_one[i].base_v3;
      sobj->pIndexData->value[4] = jewelry_one[i].base_v4;
      sobj->pIndexData->value[5] = jewelry_one[i].base_v5;
      sobj->pIndexData->value[6] = jewelry_one[i].base_v6;
      send_to_char("Its a newly manufactured piece of armor!\r\n", ch);

      short                   imbue = 0;

      imbue = number_range(1, 8);

      short                   bonus = 0;

      if(ch->pcdata->tradelevel < 10)
      {
        bonus = 1;
      }
      else if(ch->pcdata->tradelevel > 9)
      {
        bonus = 2;
      }
      if(sobj->level < 49)
      {
        bonus += 1;
      }
      else if(sobj->level > 50 && sobj->level < 90)
      {
        bonus += 2;
      }
      else if(sobj->level > 89)
      {
        bonus += 3;
      }
      if(sobj->level > 60)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_HITROLL;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, sobj->first_affect, sobj->last_affect, next, prev);
      }
      if(imbue == 1)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_AC;
        paf->modifier = -5;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, sobj->first_affect, sobj->last_affect, next, prev);
      }
      else if(imbue == 2)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_STR;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, sobj->first_affect, sobj->last_affect, next, prev);
      }
      else if(imbue == 3)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_CON;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, sobj->first_affect, sobj->last_affect, next, prev);
      }
      else if(imbue == 4)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_DEX;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, sobj->first_affect, sobj->last_affect, next, prev);
      }
      else if(imbue == 5)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_INT;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, sobj->first_affect, sobj->last_affect, next, prev);
      }
      else if(imbue == 6)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_WIS;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, sobj->first_affect, sobj->last_affect, next, prev);
      }
      else if(imbue == 7)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_CHA;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, sobj->first_affect, sobj->last_affect, next, prev);
      }
      else if(imbue == 8)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_LCK;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, sobj->first_affect, sobj->last_affect, next, prev);
      }
      if(sobj->level > 70)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_DAMROLL;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, sobj->first_affect, sobj->last_affect, next, prev);
      }

      sobj->value[0] = set_min_armor(sobj->level);
      sobj->value[1] = set_max_armor(sobj->level);
      sobj->value[4] = 777;
      if(ch->pcdata->tradelevel < 5)
      { // first tier crafted armor
        sobj->value[3] = 100;
      }
      else if(ch->pcdata->tradelevel > 4 && ch->pcdata->tradelevel < 10)
      { // second 
        // 
        // 
        // tier 
        // crafted 
        // armor
        sobj->value[3] = 200;
      }
      else if(ch->pcdata->tradelevel > 9 && ch->pcdata->tradelevel < 20)
      { // third 
        // 
        // 
        // tier 
        // crafted 
        // armor
        sobj->value[3] = 300;
      }
      else if(ch->pcdata->tradelevel >= 20)
      { // fourth tier crafted armor
        sobj->value[3] = 400;
      }
      sprintf(extra_buf,
              "\r\n&CThis manufactured item bears the seal of %s, the %s jeweler.\r\n",
              ch->name,
              ch->pcdata->tradelevel <=
              5 ? "apprentice" : ch->pcdata->tradelevel <= 10 ? "journeyman" : ch->pcdata->tradelevel <= 19 ? "expert" : ch->pcdata->tradelevel == 20 ? "master" : "reknowned");

      EXTRA_DESCR_DATA       *ed;

      CREATE(ed, EXTRA_DESCR_DATA, 1);

      LINK(ed, sobj->first_extradesc, sobj->last_extradesc, next, prev);
      ed->keyword = STRALLOC(sobj->name);
      ed->description = STRALLOC(extra_buf);
      sobj->value[0] = set_min_armor(sobj->level);
      sobj->value[1] = set_max_armor(sobj->level);

      obj_to_char(sobj, ch);
      return;
    }

    obj = create_object(get_obj_index(SMITH_PRODUCT), 0);

    if(obj->name)
      STRFREE(obj->name);
    if(obj->short_descr)
      STRFREE(obj->short_descr);
    if(obj->description)
      STRFREE(obj->description);
    sprintf(name_buf, "%s", jewelry_one[i].name);
    sprintf(short_buf, "a %s %s %s", arg1, arg2, jewelry_one[i].name);
    sprintf(long_buf, "A %s %s %s has been left here.", arg1, arg2, jewelry_one[i].name);

    obj->item_type = jewelry_one[i].item_type;
    obj->wear_flags += jewelry_one[i].wear_flags;
    if(arg4[0] == '\0')
      obj->level = ch->level;
    else
      obj->level = atoi(arg4);

    obj->weight = (jewelry_one[i].weight);
    obj->cost = jewelry_one[i].cost;
    obj->value[0] = jewelry_one[i].base_v0;
    obj->value[1] = jewelry_one[i].base_v1;
    obj->value[2] = jewelry_one[i].base_v2;
    obj->value[3] = jewelry_one[i].base_v3;
    obj->value[4] = jewelry_one[i].base_v4;
    obj->value[5] = jewelry_one[i].base_v5;
    obj->value[6] = jewelry_one[i].base_v6;
    obj->pIndexData->value[0] = jewelry_one[i].base_v0;
    obj->pIndexData->value[1] = jewelry_one[i].base_v1;
    obj->pIndexData->value[2] = jewelry_one[i].base_v2;
    obj->pIndexData->value[3] = jewelry_one[i].base_v3;
    obj->pIndexData->value[4] = jewelry_one[i].base_v4;
    obj->pIndexData->value[5] = jewelry_one[i].base_v5;
    obj->pIndexData->value[6] = jewelry_one[i].base_v6;

    if(obj->item_type == ITEM_ARMOR)
    {
      send_to_char("Its a newly manufactured piece of armor!\r\n", ch);
      obj->layers = 128;
      /*
       * Random Affect applier - Code to make crafted objs worth it
       */
      if(ch->pcdata->tradelevel > 4)
      {
        // Random affect of armor, str, con, dex, int, wis, cha, lck, hitroll,
        // damroll
        short                   imbue;

        imbue = number_range(1, 8);

        short                   bonus = 0;

        if(ch->pcdata->tradelevel < 10)
        {
          bonus = 1;
        }
        else if(ch->pcdata->tradelevel > 9)
        {
          bonus = 2;
        }
        if(obj->level < 49)
        {
          bonus += 1;
        }
        else if(obj->level > 50 && obj->level < 90)
        {
          bonus += 2;
        }
        else if(obj->level > 89)
        {
          bonus += 3;
        }
        if(obj->level > 60)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_HITROLL;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, obj->first_affect, obj->last_affect, next, prev);
        }
        if(imbue == 1)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_AC;
          paf->modifier = -5;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, obj->first_affect, obj->last_affect, next, prev);
        }
        else if(imbue == 2)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_STR;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, obj->first_affect, obj->last_affect, next, prev);
        }
        else if(imbue == 3)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_CON;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, obj->first_affect, obj->last_affect, next, prev);
        }
        else if(imbue == 4)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_DEX;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, obj->first_affect, obj->last_affect, next, prev);
        }
        else if(imbue == 5)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_INT;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, obj->first_affect, obj->last_affect, next, prev);
        }
        else if(imbue == 6)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_WIS;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, obj->first_affect, obj->last_affect, next, prev);
        }
        else if(imbue == 7)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_CHA;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, obj->first_affect, obj->last_affect, next, prev);
        }
        else if(imbue == 8)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_LCK;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, obj->first_affect, obj->last_affect, next, prev);
        }
        if(obj->level > 70)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_DAMROLL;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, obj->first_affect, obj->last_affect, next, prev);
        }
      }

      obj->value[0] = set_min_armor(obj->level);
      obj->value[1] = set_max_armor(obj->level);
      obj->value[4] = 777;
      if(ch->pcdata->tradelevel < 5)
      { // first tier crafted armor
        obj->value[3] = 100;
      }
      else if(ch->pcdata->tradelevel > 4 && ch->pcdata->tradelevel < 10)
      { // second 
        // 
        // 
        // tier 
        // crafted 
        // armor
        obj->value[3] = 200;
      }
      else if(ch->pcdata->tradelevel > 9 && ch->pcdata->tradelevel < 20)
      { // third 
        // 
        // 
        // 
        // crafted 
        // armor
        obj->value[3] = 300;
      }
      else if(ch->pcdata->tradelevel >= 20)
      { // fourth tier crafted armor
        obj->value[3] = 400;
      }

    }
    else if(obj->item_type == ITEM_WEAPON)
    {
      /*
       * Random Affect applier - Code to make crafted objs worth it
       */
      if(ch->pcdata->tradelevel > 4)
      {
        // Random affect of armor, str, con, dex, int, wis, cha, lck, hitroll,
        // damroll
        short                   imbue;

        imbue = number_range(1, 8);

        if(imbue == 1)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_AC;
          paf->modifier = -5;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, obj->first_affect, obj->last_affect, next, prev);
        }
        else if(imbue == 2)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_STR;
          paf->modifier = 1;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, obj->first_affect, obj->last_affect, next, prev);
        }
        else if(imbue == 3)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_CON;
          paf->modifier = 1;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, obj->first_affect, obj->last_affect, next, prev);
        }
        else if(imbue == 4)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_DEX;
          paf->modifier = 1;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, obj->first_affect, obj->last_affect, next, prev);
        }
        else if(imbue == 5)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_INT;
          paf->modifier = 1;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, obj->first_affect, obj->last_affect, next, prev);
        }
        else if(imbue == 6)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_WIS;
          paf->modifier = 1;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, obj->first_affect, obj->last_affect, next, prev);
        }
        else if(imbue == 7)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_CHA;
          paf->modifier = 1;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, obj->first_affect, obj->last_affect, next, prev);
        }
        else if(imbue == 8)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_LCK;
          paf->modifier = 1;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, obj->first_affect, obj->last_affect, next, prev);
        }

        short                   qball = 0;

        qball = weapon_bonus(ch->pcdata->tradelevel, obj->level);

        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_HITROLL;
        paf->modifier = qball;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, obj->first_affect, obj->last_affect, next, prev);

        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_DAMROLL;
        paf->modifier = qball;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, obj->first_affect, obj->last_affect, next, prev);
      }
      send_to_char("Its a newly manufactured weapon!\r\n", ch);
      obj->value[0] = set_min_chart(obj->level);
      obj->value[1] = set_min_chart(obj->level);
      obj->value[2] = set_max_chart(obj->level);
      obj->pIndexData->value[0] = set_min_chart(obj->level);
      obj->pIndexData->value[1] = set_max_chart(obj->level);
      obj->pIndexData->value[2] = set_min_chart(obj->level);
    }

    obj_to_char(obj, ch);

    if(ch->pcdata->tradelevel >= 20)
    {
      GET_VALUE(obj, type) = CURR_GOLD;
      obj->cost = 30;
    }
    else if(ch->pcdata->tradelevel >= 15)
    {
      GET_VALUE(obj, type) = CURR_GOLD;
      obj->cost = 25;
    }
    else if(ch->pcdata->tradelevel >= 10)
    {
      GET_VALUE(obj, type) = CURR_GOLD;
      obj->cost = 15;
    }
    else if(ch->pcdata->tradelevel >= 5)
    {
      GET_VALUE(obj, type) = CURR_SILVER;
      obj->cost = 50;
    }
    else if(ch->pcdata->tradelevel >= 1)
    {
      GET_VALUE(obj, type) = CURR_COPPER;
      obj->cost = 25;
    }

    item_type = jewelry_one[i].item_type;

    if(obj->item_type == ITEM_WEAPON)
    {
      if(ch->pcdata->tradelevel < 5)
      {
        obj->value[6] = 200;
      }
      else if(ch->pcdata->tradelevel > 4 && ch->pcdata->tradelevel < 10)
      {
        obj->value[6] = 300;
      }
      else if(ch->pcdata->tradelevel > 9 && ch->pcdata->tradelevel < 15)
      {
        obj->value[6] = 400;
      }
      else if(ch->pcdata->tradelevel > 14 && ch->pcdata->tradelevel < 20)
      {
        obj->value[6] = 500;
      }
      else if(ch->pcdata->tradelevel >= 20)
      {
        obj->value[6] = 600;
      }
      if(obj->value[4] == 0 || obj->value[4] == 4 || obj->value[4] == 8)
      {
        short                   bonus;

        bonus = obj->level / 5;
        if(bonus < 1)
        {
          bonus = 1;
        }
        obj->value[1] = set_min_chart(obj->level) + bonus * 3 + set_min_chart(obj->level / 2);
        obj->value[2] = set_max_chart(obj->level) + bonus + (set_max_chart(obj->level) / 2);
        obj->pIndexData->value[1] = set_min_chart(obj->level) + bonus * 3 + set_min_chart(obj->level / 2);
        obj->pIndexData->value[2] = set_max_chart(obj->level) + bonus + (set_max_chart(obj->level) / 2);
        obj->weight = 15;
      }

      if(ch->pcdata->tradelevel <= 5)
      {
        obj->value[0] = 6;
        obj->value[1] -= 1;
        obj->value[2] -= 1;
        GET_VALUE(obj, type) = CURR_COPPER;
        obj->cost = 25;
      }

      if(ch->pcdata->tradelevel > 5)
      {
        int                     modifier;

        if(ch->pcdata->tradelevel >= 15)
        {
          if(obj->level >= 90)
            modifier = 15;
          else if(obj->level >= 70)
            modifier = 8;
          else if(obj->level >= 50)
            modifier = 5;
          else if(obj->level >= 30)
            modifier = 3;
          else
            modifier = 2;
        }
        else if(ch->pcdata->tradelevel >= 20)
        {
          if(obj->level >= 90)
            modifier = 12;
          else if(obj->level >= 70)
            modifier = 10;
          else if(obj->level >= 50)
            modifier = 8;
          else if(obj->level >= 30)
            modifier = 6;
          else
            modifier = 4;
        }
        else
          modifier = 1;

        obj->value[0] = 8;

        /*
         * Chance to make it better or worser
         */
        if(number_percent() > 50)
          modifier += number_range(1, 5);
        else if(number_percent() < 50)
          modifier -= number_range(1, 5);
        if(modifier == 0)
          modifier = 1;
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_HIT;
        paf->modifier = modifier;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, obj->first_affect, obj->last_affect, next, prev);
      }
      if(ch->pcdata->tradelevel >= 15)
      {
        int                     modifier;

        if(ch->pcdata->tradelevel >= 20)
        {
          if(obj->level >= 90)
            modifier = 12;
          else if(obj->level >= 70)
            modifier = 10;
          else if(obj->level >= 50)
            modifier = 8;
          else if(obj->level >= 30)
            modifier = 6;
          else
            modifier = 4;
        }
        else
        {
          if(obj->level >= 90)
            modifier = 10;
          else if(obj->level >= 70)
            modifier = 8;
          else if(obj->level >= 50)
            modifier = 6;
          else if(obj->level >= 30)
            modifier = 4;
          else
            modifier = 3;
        }
        obj->value[0] = 10;
        obj->value[1] += 1;
        obj->value[2] += 1;

        /*
         * Chance to make it better or worser
         */
        if(number_percent() > 50)
          modifier += number_range(1, 5);
        else if(number_percent() < 50)
          modifier -= number_range(1, 5);
        if(modifier == 0)
          modifier = 1;
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_MOVE;
        paf->modifier = modifier;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, obj->first_affect, obj->last_affect, next, prev);

      }
      if(ch->pcdata->tradelevel >= 20)
      {
        GET_VALUE(obj, type) = CURR_GOLD;
        obj->cost = 25;
      }
    }
    else if(obj->item_type == ITEM_ARMOR)
    {
      CREATE(paf, AFFECT_DATA, 1);

      paf->type = sn;
      paf->duration = -1;
      paf->location = APPLY_HIT;

      if(ch->pcdata->tradelevel >= 20)
      {
        if(obj->level >= 90)
          paf->modifier = 20;
        else if(obj->level >= 70)
          paf->modifier = 10;
        else if(obj->level >= 50)
          paf->modifier = 8;
        else if(obj->level >= 30)
          paf->modifier = 5;
        else
          paf->modifier = 3;
        GET_VALUE(obj, type) = CURR_GOLD;
        obj->cost = 25;
      }
      else if(ch->pcdata->tradelevel >= 15)
      {
        if(obj->level >= 90)
          paf->modifier = 15;
        else if(obj->level >= 70)
          paf->modifier = 8;
        else if(obj->level >= 50)
          paf->modifier = 5;
        else if(obj->level >= 30)
          paf->modifier = 3;
        else
          paf->modifier = 2;
        GET_VALUE(obj, type) = CURR_SILVER;
        obj->cost = 25;
      }
      else if(ch->pcdata->tradelevel > 5)
      {
        paf->modifier = 2;
        GET_VALUE(obj, type) = CURR_SILVER;
        obj->cost = 10;
      }
      else
      {
        GET_VALUE(obj, type) = CURR_COPPER;
        obj->cost = 25;
        paf->modifier = 1;
      }

      /*
       * Chance to make it better or worser
       */
      if(number_percent() > 50)
        paf->modifier += number_range(1, 5);
      else if(number_percent() < 50)
        paf->modifier -= number_range(1, 5);
      if(paf->modifier == 0)
        paf->modifier = 1;

      xCLEAR_BITS(paf->bitvector);
      LINK(paf, obj->first_affect, obj->last_affect, next, prev);

      if(number_percent() > 80)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_AC;
        paf->modifier = number_range(-10, 10);
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, obj->first_affect, obj->last_affect, next, prev);
      }
      obj->value[1] += get_armor_ac_mod(obj->level, ch->pcdata->tradelevel);
      obj->value[0] = obj->value[1];
    }

    if(IS_CLANNED(ch))
    {
      CLAN_DATA              *clan;

      clan = ch->pcdata->clan;
      ch->pcdata->clanpoints += 1;
      clan->totalpoints += 1;
      ch_printf(ch, "\r\n&G%s clan has gained a status point from your craftsmanship, now totaling %d clan status points!\r\n", clan->name, clan->totalpoints);
      save_char_obj(ch);
      save_clan(clan);
    }

    sprintf(extra_buf,
            "\r\n&CThis manufactured item bears the seal of %s, the %s jeweler.\r\n",
            ch->name,
            ch->pcdata->tradelevel <= 5 ? "apprentice" : ch->pcdata->tradelevel <= 10 ? "journeyman" : ch->pcdata->tradelevel <= 19 ? "expert" : ch->pcdata->tradelevel >= 20 ? "master" : "reknowned");

    obj->name = STRALLOC(name_buf);
    obj->short_descr = STRALLOC(short_buf);
    obj->description = STRALLOC(long_buf);

    EXTRA_DESCR_DATA       *ed;

    CREATE(ed, EXTRA_DESCR_DATA, 1);

    LINK(ed, obj->first_extradesc, obj->last_extradesc, next, prev);
    ed->keyword = STRALLOC(obj->name);
    ed->description = STRALLOC(extra_buf);
    if(obj->item_type == ITEM_ARMOR)
    {
      obj->value[0] = set_min_armor(obj->level);
      obj->value[1] = set_max_armor(obj->level);
    }
    int                     tier;

    if(jewelry_one[i].level >= 15)
      tier = 3;
    else if(jewelry_one[i].level > 5 && jewelry_one[i].level < 15)
      tier = 2;
    else
      tier = 1;
    return;
  }
  return;
}

void send_forge_syntax(CHAR_DATA *ch)
{
  if(!ch)
    return;
  send_to_char("\r\n", ch);

  if(IS_BLIND(ch))
  {
    send_to_char("Type forge ore into item level\r\n", ch);
    send_to_char("Type forge list for a list of items you can forge or forge list all to see everything.\r\n", ch);
    send_to_char("Type forge fire to light the forge\r\n", ch);
    if(ch->pcdata->tradelevel > 9)
    {
      send_to_char("Type forge id number repair\r\n", ch);
      send_to_char("eg. forge 3223 repair\r\n", ch);

    }
    if(ch->pcdata->tradelevel > 14)
    {
      send_to_char("Type forge id number rename new name to change the objects name or forge id number rekey new key to change what you type to get the object.\r\n", ch);
    }
    send_to_char("Ore possibilities include bronze, silver, gold, iron, steel, titanium\r\n", ch);
    send_to_char("Note There are 4 steps, keep doing the forge command until the last step.\r\n", ch);

  }
  else
  {
    send_to_char("&cSyntax: forge <&Cmaterial&c> into <&Citem&c> <&Clevel&c>\r\n", ch);
    send_to_char("Syntax: forge list or forge list all\r\n", ch);
    send_to_char("Syntax: forge fire\r\n", ch);
    if(ch->pcdata->tradelevel > 9)
    {
      send_to_char("Syntax: forge id# repair\r\n", ch);
      send_to_char("eg. forge 3232 repair\r\n", ch);

    }

    if(ch->pcdata->tradelevel > 14)
    {

      send_to_char("Syntax: forge id rename <&Cnew name&c> forge id rekey <<&Cnew name&c>\r\n", ch);
    }
    send_to_char("Materials being: bronze, silver, gold, iron, steel, titanium\r\n", ch);
    send_to_char("Note: There are &C4 steps&c, keep doing the forge command until the last step.\r\n", ch);
  }
}

void do_forge(CHAR_DATA *ch, char *argument)
{
  short                   number;
  OBJ_DATA               *item, *obj;
  AFFECT_DATA            *paf;
  struct skill_type      *skill = NULL;
  char                    arg1[MIL], arg2[MIL], arg3[MIL], arg4[MIL];
  char                    name_buf[MSL], short_buf[MSL], long_buf[MSL], extra_buf[MSL];
  const char             *adj;
  int                     i = 0, x = 0, difficulty = 0, output = 0, sn = 1;
  short                   mnum = 0;
  bool                    itm = FALSE, hasore = FALSE, found = FALSE;

  name_buf[0] = '\0';
  short_buf[0] = '\0';
  long_buf[0] = '\0';
  extra_buf[0] = '\0';

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  argument = one_argument(argument, arg3);
  argument = one_argument(argument, arg4);

  if(IS_NPC(ch) || ch->desc == NULL)
    return;

  if(!arg1 || arg1[0] == '\0')
  {
    send_forge_syntax(ch);
    return;
  }

  if(ch->move < 1)
  {
    send_to_char("You	don't have enough move points to perform that.\r\n", ch);
    return;
  }
  else
  {
    ch->move -= 1;
  }

  if(ch->position != POS_STANDING)
  {
    send_to_char("You should be standing to attempt this.\r\n", ch);
  }

// Just adding objID to work for repair code section 

  if(!str_cmp(arg2, "repair"))
  {
    OBJ_DATA               *repair;

    for(repair = ch->first_carrying; repair; repair = repair->next_content)
    {
      if(atoi(arg1) == repair->guid && (repair->item_type == ITEM_ARMOR || repair->item_type == ITEM_WEAPON))
        break;
    }

    if(!repair)
    {
      send_to_char("You do not have any repairable equipment to repair.\r\n", ch);
      return;
    }

    if(ch->pcdata->tradelevel < 10)
    {
      send_to_char("You are not skilled enough of a craftsman to repair items yet.\r\n", ch);
      return;
    }

    if(repair->item_type == ITEM_WEAPON && repair->value[0] == 12)
    {
      send_to_char("It is already honed to perfection.\r\n", ch);
      return;
    }
    if(repair->item_type == ITEM_ARMOR && repair->value[0] == repair->value[1])
    {
      send_to_char("It is already forged to perfection.\r\n", ch);
      return;
    }

    act(AT_CYAN, "$n hammers $p back to shape.", ch, repair, NULL, TO_ROOM);
    act(AT_CYAN, "You hammer $p back to shape.", ch, repair, NULL, TO_CHAR);
    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/hammer.wav)\r\n", ch);

    switch (repair->item_type)
    {
      case ITEM_ARMOR:
        repair->value[0] = repair->value[1];
        break;
      case ITEM_WEAPON:
        repair->value[0] = INIT_WEAPON_CONDITION;
        break;
    }
    return;
  }

// End of repair code section 

  if(!str_cmp(arg2, "rename") && ch->pcdata->tradelevel > 14 && !xIS_SET(ch->act, PLR_NORENAME))
  {
    OBJ_DATA               *item;

    for(item = ch->first_carrying; item; item = item->next_content)
    {
      if(atoi(arg1) == item->guid && item->pIndexData->vnum == SMITH_PRODUCT)
        break;
    }
    if(!item)
    {
      send_to_char("You do not have any forged items to rename or rekey.\r\n", ch);
      return;
    }

    if(!arg3 || arg3[0] == '\0')
    {
      send_to_char("You must specify the new keywords, ie forge id number rename 'new name' or rename 'ring toering'.\r\n", ch);
      return;
    }
    ch_printf(ch, "&cChanging item's description from &C%s&c to&C %s&c..\r\n", item->short_descr, arg3);
    STRFREE(item->short_descr);
    item->short_descr = STRALLOC(arg3);
    sprintf(long_buf, "%s has been left here along the ground.", item->short_descr);
    STRFREE(item->description);
    item->description = STRALLOC(long_buf);
    send_to_char("&RDone!&c Please note, the KEYWORDS used to drop/equip/etc remain the same.\r\n", ch);
    save_char_obj(ch);
    return;
  }
  else if(!str_cmp(arg2, "rename") && (ch->pcdata->tradelevel < 15 || xIS_SET(ch->act, PLR_NORENAME)))
  {
    send_to_char("You must specify the new keywords, ie rename id number 'new name' or rename 'ring toering'.\r\n", ch);
    return;
  }

  if(!str_cmp(arg2, "rekey") && ch->pcdata->tradelevel > 14 && !xIS_SET(ch->act, PLR_NORENAME))
  {
    OBJ_DATA               *item;

    for(item = ch->first_carrying; item; item = item->next_content)
    {
      if(atoi(arg1) == item->guid && item->pIndexData->vnum == SMITH_PRODUCT)
        break;
    }
    if(!item)
    {
      send_to_char("You do not have any forged items to rename or rekey.\r\n", ch);
      return;
    }

    ch_printf(ch, "&cChanging item's keywords from &C%s&c to &C%s&c..\r\n", item->name, arg3);
    STRFREE(item->name);
    item->name = STRALLOC(arg3);
    send_to_char("&RDone!&c Please note, the DESCRIPTION when looking at the item remains the same.\r\n", ch);
    save_char_obj(ch);
    return;
  }
  else if(!str_cmp(arg2, "rekey") && (ch->pcdata->tradelevel < 15 || xIS_SET(ch->act, PLR_NORENAME)))
  {
    send_to_char("You must specify the new keywords, ie 'forge rekey whatever new name' or 'forge rekey sword toothpick'.\r\n", ch);
    return;
  }

  if(!str_cmp(arg1, "list"))
  {
    int                     y, count = 0;
    char                    wearloc[MSL];
    short                   value;

    send_to_char("\t\t\t&CThe Forge Listing\r\n\r\n", ch);

    if(!str_cmp(arg2, "all"))
    {
      for(y = 0; y < MAX_SMITH_ITEMS; y++)
      {
        if(arg2 && !is_number(arg2))
          value = get_wflag(arg2);
        if(value < 0 || value > 31)
          value = 0;

        if(value == 0)
        {
          if(!IS_BLIND(ch))
          {
            if(((!str_cmp(arg2, "0") && tier_one[y].level == 0)
                || (!str_cmp(arg2, "1") && tier_one[y].level == 1)
                || (!str_cmp(arg2, "2") && tier_one[y].level == 2)
                || (!str_cmp(arg2, "3") && tier_one[y].level == 3)
                || (!str_cmp(arg2, "4") && tier_one[y].level == 4)
                || (!str_cmp(arg2, "5") && tier_one[y].level == 5)
                || (!str_cmp(arg2, "6") && tier_one[y].level == 6)
                || (!str_cmp(arg2, "7") && tier_one[y].level == 7)
                || (!str_cmp(arg2, "8") && tier_one[y].level == 8)
                || (!str_cmp(arg2, "9") && tier_one[y].level == 9)
                || (!str_cmp(arg2, "10") && tier_one[y].level == 10)
                || (!str_cmp(arg2, "11") && tier_one[y].level == 11)
                || (!str_cmp(arg2, "12") && tier_one[y].level == 12)
                || (!str_cmp(arg2, "13") && tier_one[y].level == 13)
                || (!str_cmp(arg2, "14") && tier_one[y].level == 14)
                || (!str_cmp(arg2, "15") && tier_one[y].level == 15)
                || (!str_cmp(arg2, "16") && tier_one[y].level == 16)
                || (!str_cmp(arg2, "17") && tier_one[y].level == 17)
                || (!str_cmp(arg2, "18") && tier_one[y].level == 18)
                || (!str_cmp(arg2, "19") && tier_one[y].level == 19)
                || (!str_cmp(arg2, "20") && tier_one[y].level == 20))
               || (!str_cmp(arg1, "list") && str_cmp(arg2, "0")
                   && str_cmp(arg2, "1") && str_cmp(arg2, "2")
                   && str_cmp(arg2, "3") && str_cmp(arg2, "4")
                   && str_cmp(arg2, "5") && str_cmp(arg2, "6")
                   && str_cmp(arg2, "7") && str_cmp(arg2, "8")
                   && str_cmp(arg2, "9") && str_cmp(arg2, "10")
                   && str_cmp(arg2, "11") && str_cmp(arg2, "12")
                   && str_cmp(arg2, "13") && str_cmp(arg2, "14")
                   && str_cmp(arg2, "15") && str_cmp(arg2, "16") && str_cmp(arg2, "17") && str_cmp(arg2, "18") && str_cmp(arg2, "19") && str_cmp(arg2, "20")))
            {
              count++;
              snprintf(wearloc, sizeof(wearloc), "&c(&w%s&c)%s(%d)", flag_string(tier_one[y].wear_flags - 1, w_flags), tier_one[y].name ? tier_one[y].name : "null name", tier_one[y].level);
              ch_printf(ch, "&c%-32s ", wearloc);
            }
          }
          else
          {
            if(((!str_cmp(arg2, "0") && tier_one[y].level == 0)
                || (!str_cmp(arg2, "1") && tier_one[y].level == 1)
                || (!str_cmp(arg2, "2") && tier_one[y].level == 2)
                || (!str_cmp(arg2, "3") && tier_one[y].level == 3)
                || (!str_cmp(arg2, "4") && tier_one[y].level == 4)
                || (!str_cmp(arg2, "5") && tier_one[y].level == 5)
                || (!str_cmp(arg2, "6") && tier_one[y].level == 6)
                || (!str_cmp(arg2, "7") && tier_one[y].level == 7)
                || (!str_cmp(arg2, "8") && tier_one[y].level == 8)
                || (!str_cmp(arg2, "9") && tier_one[y].level == 9)
                || (!str_cmp(arg2, "10") && tier_one[y].level == 10)
                || (!str_cmp(arg2, "11") && tier_one[y].level == 11)
                || (!str_cmp(arg2, "12") && tier_one[y].level == 12)
                || (!str_cmp(arg2, "13") && tier_one[y].level == 13)
                || (!str_cmp(arg2, "14") && tier_one[y].level == 14)
                || (!str_cmp(arg2, "15") && tier_one[y].level == 15)
                || (!str_cmp(arg2, "16") && tier_one[y].level == 16)
                || (!str_cmp(arg2, "17") && tier_one[y].level == 17)
                || (!str_cmp(arg2, "18") && tier_one[y].level == 18)
                || (!str_cmp(arg2, "19") && tier_one[y].level == 19)
                || (!str_cmp(arg2, "20") && tier_one[y].level == 20))
               || (!str_cmp(arg1, "list") && str_cmp(arg2, "0")
                   && str_cmp(arg2, "1") && str_cmp(arg2, "2")
                   && str_cmp(arg2, "3") && str_cmp(arg2, "4")
                   && str_cmp(arg2, "5") && str_cmp(arg2, "6")
                   && str_cmp(arg2, "7") && str_cmp(arg2, "8")
                   && str_cmp(arg2, "9") && str_cmp(arg2, "10")
                   && str_cmp(arg2, "11") && str_cmp(arg2, "12")
                   && str_cmp(arg2, "13") && str_cmp(arg2, "14")
                   && str_cmp(arg2, "15") && str_cmp(arg2, "16") && str_cmp(arg2, "17") && str_cmp(arg2, "18") && str_cmp(arg2, "19") && str_cmp(arg2, "20")))
            {
              count++;
              ch_printf(ch, "%2d &c%-28s", tier_one[y].level, tier_one[y].name ? tier_one[y].name : "null name");
            }
          }
        }
        else
        {
          if(!IS_BLIND(ch))
          {
            if((1 << value) != (tier_one[y].wear_flags - 1))
              continue;
            count++;
            snprintf(wearloc, sizeof(wearloc), "&c(&w%s&c)%s(%d)", flag_string(tier_one[y].wear_flags - 1, w_flags), tier_one[y].name ? tier_one[y].name : "null name", tier_one[y].level);
            ch_printf(ch, "&c%-32s ", wearloc);
          }
          else
          {
            if((1 << value) != (tier_one[y].wear_flags - 1))
              continue;
            count++;
            ch_printf(ch, "%2d &c%-28s", tier_one[y].level, tier_one[y].name ? tier_one[y].name : "null name");
          }
        }
        if(count == 3)
        {
          send_to_char("\r\n", ch);
          count = 0;
        }
      }

      if(count != 3)
      {
        send_to_char("\r\r\n\n", ch);
      }
      send_forge_syntax(ch);
      return;
    }

    for(y = 0; y < MAX_SMITH_ITEMS; y++)
    {
      if(ch->pcdata->tradelevel < tier_one[y].level)
        continue;
      count++;
      ch_printf(ch, "&c%-15s      ", tier_one[y].name ? tier_one[y].name : "null name");
      if(count == 3)
      {
        count = 0;
        send_to_char("\r\n", ch);
      }
    }
    if(count != 0)
      send_to_char("\r\n", ch);
    send_forge_syntax(ch);
    return;
  }

  if(!str_cmp(arg1, "fire"))
  {
    OBJ_DATA               *stove;
    bool                    found;

    found = FALSE;

    for(stove = ch->in_room->first_content; stove; stove = stove->next_content)
    {
      if(stove->item_type == ITEM_FORGE)
      {
        found = TRUE;
        break;
      }
    }

    if(!found)
    {
      send_to_char("There must be a forge to fire it.\r\n", ch);
      return;
    }

    if(stove->value[0] == 1)
    {
      send_to_char("There is no need to fire the forge, it is already.\r\n", ch);
      return;
    }

    OBJ_DATA               *coal;

    for(coal = ch->first_carrying; coal; coal = coal->next_content)
    {
      if(coal->item_type == ITEM_COAL)
        break;
    }
    if(!coal)
    {
      send_to_char("You do not have any coal to fire the forge.\r\n", ch);
      return;
    }
    separate_obj(coal);
    obj_from_char(coal);

    act(AT_CYAN, "$n fires up the forge lighting the coal within it.", ch, NULL, NULL, TO_ROOM);
    act(AT_CYAN, "You fire the forge lighting the coal within it.", ch, NULL, NULL, TO_CHAR);
    act(AT_YELLOW, "A flame flickers within the forge.", ch, NULL, NULL, TO_ROOM);
    act(AT_YELLOW, "A flame flickers within the forge.", ch, NULL, NULL, TO_CHAR);
    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/forgefire.wav)\r\n", ch);

    extract_obj(coal);
    stove->value[0] = 1;
    return;
  }

  OBJ_DATA               *bag;

  for(bag = ch->first_carrying; bag; bag = bag->next_content)
  {
    if(bag->item_type == ITEM_RESOURCE_BAG && bag->value[0] != 500)
      break;
  }

  if(!bag)
  {
    send_to_char("You do not have a miner's bag to forge with.\r\n", ch);
    return;
  }

  if(arg2[0] == '\0' || arg3[0] == '\0')
  {
    send_forge_syntax(ch);
    return;
  }

  if(str_cmp(arg2, "into"))
  {
    send_forge_syntax(ch);
    return;
  }

  if(!IS_SET(ch->in_room->room_flags, ROOM_TRADESKILLS))
  {
    send_to_char("You must be in a tradeskills building to do this.\r\n", ch);
    return;
  }

  {
    OBJ_DATA               *stove;
    bool                    found;

    found = FALSE;

    for(stove = ch->in_room->first_content; stove; stove = stove->next_content)
    {
      if(stove->item_type == ITEM_FORGE)
      {
        found = TRUE;
        break;
      }
    }

    if(!found)
    {
      send_to_char("There must be a forge in the room in order to do that.\r\n", ch);
      return;
    }

    {

      // This is the nitty gritty of checking tier output
      i = tier_lookup(arg3);
      if(i < 0 || ch->pcdata->tradelevel < tier_one[i].level)
      {
        send_to_char("That isn't a something you can create.\r\n", ch);
        return;
      }

      if(str_cmp(arg1, "bronze") && str_cmp(arg1, "silver") && str_cmp(arg1, "gold") && str_cmp(arg1, "iron") && str_cmp(arg1, "steel") && str_cmp(arg1, "titanium"))
      {
        send_to_char("You can only use bronze, silver, gold, iron, steel, titanium.\r\n", ch);
        return;
      }

      if(!str_cmp(arg1, "bronze") && bag->craft1 < 1)
      {
        send_to_char("You don't have any bronze ore in your miner's bag.\r\n", ch);
        return;
      }
      else if(!str_cmp(arg1, "silver") && bag->craft2 < 1)
      {
        send_to_char("You don't have any silver ore in your miner's bag.\r\n", ch);
        return;
      }
      else if(!str_cmp(arg1, "gold") && bag->craft3 < 1)
      {
        send_to_char("You don't have any gold ore in your miner's bag.\r\n", ch);
        return;
      }
      else if(!str_cmp(arg1, "iron") && bag->craft4 < 1)
      {
        send_to_char("You don't have any iron ore in your miner's bag.\r\n", ch);
        return;
      }
      else if(!str_cmp(arg1, "steel") && bag->craft5 < 1)
      {
        send_to_char("You don't have any steel ore in your miner's bag.\r\n", ch);
        return;
      }
      else if(!str_cmp(arg1, "titanium") && bag->craft6 < 1)
      {
        send_to_char("You don't have any titanium ore in your miner's bag.\r\n", ch);
        return;
      }

      if(!str_cmp(arg1, "bronze"))
      {
        bag->craft1 -= 1;
        x = 1;
      }
      else if(!str_cmp(arg1, "silver"))
      {
        bag->craft2 -= 1;
        x = 2;
      }
      else if(!str_cmp(arg1, "gold"))
      {
        bag->craft3 -= 1;
        x = 3;
      }
      else if(!str_cmp(arg1, "iron"))
      {
        bag->craft4 -= 1;
        x = 4;
      }
      else if(!str_cmp(arg1, "steel"))
      {
        bag->craft5 -= 1;
        x = 5;
      }
      else if(!str_cmp(arg1, "titanium"))
      {
        bag->craft6 -= 1;
        x = 6;
      }
      else
      {
        send_to_char("That's not a proper ore.\r\n", ch);
        return;
      }

      SKILLTYPE              *skill = get_skilltype(gsn_forge);

      WAIT_STATE(ch, 15);

      if(stove->value[0] != 1)
      {
        send_to_char("You have to fire the forge first.\r\n", ch);
        send_to_char("Syntax: forge fire\r\n", ch);
        return;
      }

      short                   failure = 0;

      if(ch->pcdata->tradelevel < 5)
        failure = 14;
      else if(ch->pcdata->tradelevel < 10 && ch->pcdata->tradelevel > 4)
        failure = 10;
      else if(ch->pcdata->tradelevel < 15 && ch->pcdata->tradelevel > 9)
        failure = 8;
      else if(ch->pcdata->tradelevel < 20 && ch->pcdata->tradelevel > 14)
        failure = 5;
      else if(ch->pcdata->tradelevel == 20)
        failure = 2;

      if(bag->value[6] == 0)
      {
        if(!can_use_skill(ch, number_percent(), gsn_forge) || number_percent() <= failure)
        {
          act(AT_CYAN, "$n prepares the ore by placing it in the furnace, but the material is left too long and melts.", ch, NULL, NULL, TO_ROOM);
          act(AT_CYAN, "You prepare the ore by placing it in the furnace, but the material is left too long and melts.", ch, NULL, NULL, TO_CHAR);
          if(xIS_SET(ch->act, PLR_CRAFTS))
            send_to_char("!!SOUND(sound/sizzle.wav)\r\n", ch);

          learn_from_failure(ch, gsn_forge);
          return;
        }
        bag->value[6] = 1;
        act(AT_CYAN, "$n prepares the ore by placing it in the forge and letting it get white-hot before pulling it out.", ch, NULL, NULL, TO_ROOM);
        act(AT_CYAN, "You prepare the ore by placing it in the forge and letting it get white-hot before pulling it out.", ch, NULL, NULL, TO_CHAR);
        if(xIS_SET(ch->act, PLR_CRAFTS))
          send_to_char("!!SOUND(sound/sizzle.wav)\r\n", ch);

        WAIT_STATE(ch, 15);
        obj = create_object(get_obj_index(OBJ_VNUM_ORE), 0);
        if(obj->name)
          STRFREE(obj->name);
        if(obj->short_descr)
          STRFREE(obj->short_descr);
        if(obj->description)
          STRFREE(obj->description);
        obj->name = STRALLOC(arg1);
        obj->description = STRALLOC("A white-hot ore has been left here.");
        obj->short_descr = STRALLOC("a white-hot ore");
        obj_to_char(obj, ch);
        return;
      }

      if(bag->value[6] == 1)
      {
        if(number_percent() <= failure)
        {
          act(AT_CYAN, "$n places the heated ore on the anvil and begins to shape it with the hammer, but gets distracted and distorts the material beyond use.", ch, NULL, NULL, TO_ROOM);
          act(AT_CYAN, "You place the heated ore on the anvil and begin to shape it with your hammer, but get distracted and distort the material beyond use.", ch, NULL, NULL, TO_CHAR);
          if(xIS_SET(ch->act, PLR_CRAFTS))
            send_to_char("!!SOUND(sound/hammer.wav)\r\n", ch);
          for(obj = ch->first_carrying; obj; obj = obj->next_content)
          {
            if(obj->item_type == ITEM_RAW)
              break;
          }

          if(!obj)
          {
            send_to_char("You do not have your ore piece.\r\n", ch);
            bag->value[6] = 0;
            return;
          }

          separate_obj(obj);
          obj_from_char(obj);
          extract_obj(obj);

          if(!str_cmp(arg1, "bronze"))
          {
            bag->craft1 -= 1;
          }
          else if(!str_cmp(arg1, "silver"))
          {
            bag->craft2 -= 1;
          }
          else if(!str_cmp(arg1, "gold"))
          {
            bag->craft3 -= 1;
          }
          else if(!str_cmp(arg1, "iron"))
          {
            bag->craft4 -= 1;
          }
          else if(!str_cmp(arg1, "steel"))
          {
            bag->craft5 -= 1;
          }
          else if(!str_cmp(arg1, "titanium"))
          {
            bag->craft6 -= 1;
          }
          learn_from_failure(ch, gsn_forge);
          return;
        }

        act(AT_CYAN, "$n places the heated ore on the anvil and begins to shape it with the hammer.", ch, NULL, NULL, TO_ROOM);
        act(AT_CYAN, "You place the heated ore on the anvil and begin to shape it with your hammer.", ch, NULL, NULL, TO_CHAR);
        if(xIS_SET(ch->act, PLR_CRAFTS))
          send_to_char("!!SOUND(sound/hammer.wav)\r\n", ch);
        WAIT_STATE(ch, 15);

        OBJ_DATA               *obj;

        for(obj = ch->first_carrying; obj; obj = obj->next_content)
        {
          if(obj->item_type == ITEM_RAW)
            break;
        }

        if(!obj)
        {
          send_to_char("You do not have your ore piece.\r\n", ch);
          bag->value[6] = 0;
          return;
        }
        bag->value[6] = 2;
        if(obj->short_descr)
          STRFREE(obj->short_descr);
        obj->short_descr = STRALLOC("&Yformed hot ore");
        return;
      }
      /*
       * They should only get these and spend alot of time on the hammery part the rest
       * should be fairly straight foward 
       */
      if(bag->value[6] == 2)
      {
        if(!can_use_skill(ch, number_percent(), gsn_forge) || number_percent() <= failure)
        {
          act(AT_CYAN, "$n hammers away at the material.", ch, NULL, NULL, TO_ROOM);
          act(AT_CYAN, "You hammer away at the material.", ch, NULL, NULL, TO_CHAR);
          if(xIS_SET(ch->act, PLR_CRAFTS))
            send_to_char("!!SOUND(sound/hammer.wav)\r\n", ch);
          learn_from_failure(ch, gsn_forge);
          return;
        }

        OBJ_DATA               *obj;

        for(obj = ch->first_carrying; obj; obj = obj->next_content)
        {
          if(obj->item_type == ITEM_RAW)
            break;
        }

        if(!obj)
        {
          send_to_char("You do not have your ore piece.\r\n", ch);
          bag->value[6] = 0;
          return;
        }

        if(number_percent() <= failure)
        {
          act(AT_CYAN, "As $n starts to hammer the material, $s realizes it has cooled off and needs to be reheated.", ch, NULL, NULL, TO_ROOM);
          act(AT_CYAN, "As you start to hammer the material, you realize it has cooled off and needs to be reheated.", ch, NULL, NULL, TO_CHAR);
          if(xIS_SET(ch->act, PLR_CRAFTS))
            send_to_char("!!SOUND(sound/hammer.wav)\r\n", ch);
          learn_from_failure(ch, gsn_forge);
          bag->value[6] = 1;
          if(obj->short_descr)
            STRFREE(obj->short_descr);
          obj->short_descr = STRALLOC("&cformed cooled ore");
          return;
        }

        act(AT_CYAN, "$n places the cooling material in a barrel of water to cool it off.", ch, NULL, NULL, TO_ROOM);
        act(AT_CYAN, "You place the cooling material in a barrel of water to cool it off.", ch, NULL, NULL, TO_CHAR);
        if(number_range(1, 4) > 2)
          learn_from_craft(ch, gsn_forge);  // add a chance of gaining exp

        WAIT_STATE(ch, 15);
        if(xIS_SET(ch->act, PLR_CRAFTS))
          send_to_char("!!SOUND(sound/sizzling.wav)\r\n", ch);
        bag->value[6] = 3;
        if(obj->short_descr)
          STRFREE(obj->short_descr);
        obj->short_descr = STRALLOC("&wthe roughly finished product");
        return;
      }

      if(!can_use_skill(ch, number_percent(), gsn_forge) || number_percent() <= failure)
      {
        OBJ_DATA               *obj;

        for(obj = ch->first_carrying; obj; obj = obj->next_content)
        {
          if(obj->item_type == ITEM_RAW)
            break;
        }

        if(!obj)
        {
          send_to_char("You do not have your ore piece.\r\n", ch);
          bag->value[6] = 0;
          return;
        }

        act(AT_CYAN, "You spin the grinding wheel to smooth the rough edges, but get distracted and distort the material beyond use.", ch, NULL, NULL, TO_CHAR);
        act(AT_CYAN, "$n spins the grinding wheel to smooth the rough edges, but gets distracted and distorts the material beyond use.", ch, NULL, NULL, TO_ROOM);
        if(xIS_SET(ch->act, PLR_CRAFTS))
          send_to_char("!!SOUND(sound/grinding.wav)\r\n", ch);

        // separate_obj( obj );
        obj_from_char(obj);
        extract_obj(obj);
        bag->value[6] = 0;
        if(!str_cmp(arg1, "bronze"))
        {
          bag->craft1 -= 1;
        }
        else if(!str_cmp(arg1, "silver"))
        {
          bag->craft2 -= 1;
        }
        else if(!str_cmp(arg1, "gold"))
        {
          bag->craft3 -= 1;
        }
        else if(!str_cmp(arg1, "iron"))
        {
          bag->craft4 -= 1;
        }
        else if(!str_cmp(arg1, "steel"))
        {
          bag->craft5 -= 1;
        }
        else if(!str_cmp(arg1, "titanium"))
        {
          bag->craft6 -= 1;
        }
        learn_from_failure(ch, gsn_forge);
        return;
      }

      act(AT_CYAN, "$n spins the grinding wheel to smooth the rough edges of $s work.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You spin the grinding wheel to smooth the rough edges of your work.", ch, NULL, NULL, TO_CHAR);
      WAIT_STATE(ch, 15);
      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/grinding.wav)\r\n", ch);

      for(obj = ch->first_carrying; obj; obj = obj->next_content)
      {
        if(obj->item_type == ITEM_RAW)
          break;
      }

      if(!obj)
      {
        send_to_char("You do not have your ore piece.\r\n", ch);
        bag->value[6] = 0;
        return;
      }

      bag->value[6] = 0;
      learn_from_craft(ch, gsn_forge);

      item = create_object(get_obj_index(SMITH_PRODUCT), 1);
      if(!str_cmp(arg1, "bronze"))
        item->color = 1;
      else if(!str_cmp(arg1, "silver"))
        item->color = 5;
      else if(!str_cmp(arg1, "gold"))
        item->color = 14;
      else if(!str_cmp(arg1, "iron"))
        item->color = 13;
      else if(!str_cmp(arg1, "steel"))
        item->color = 13;
      else if(!str_cmp(arg1, "titanium"))
        item->color = 9;

      if(arg4[0] == '\0')
        item->level = ch->level;
      else
        item->level = atoi(arg4);

      {
        int                     tier, item_type, wearflags, weight, cost;
        const char             *name;

        if(tier_one[i].level >= 15)
          tier = 3;
        else if(tier_one[i].level > 5 && tier_one[i].level < 15)
          tier = 2;
        else
          tier = 1;

        name = tier_one[i].name;
        item_type = tier_one[i].item_type;
        wearflags = tier_one[i].wear_flags;
        weight = tier_one[i].weight;
        cost = tier_one[i].weight;
        mnum = 1;

        if(ch->pcdata->tradelevel <= 1)
          adj = "poorly";
        else if(ch->pcdata->tradelevel <= 5)
          adj = "simply";
        else if(ch->pcdata->tradelevel <= 8)
          adj = "properly";
        else if(ch->pcdata->tradelevel <= 10)
          adj = "well";
        else if(ch->pcdata->tradelevel <= 15)
          adj = "finely";
        else if(ch->pcdata->tradelevel <= 19)
          adj = "masterfully";
        else
          adj = "legendary";

        for(x = 0; x < MAX_ORE_ITEMS - 1; x++)
        {
          if(!str_cmp(arg1, smith_ores_table[x].name))
          {
            hasore = TRUE;
            break;
          }
        }

        sprintf(name_buf, "%s", name);
        sprintf(short_buf, "a %s, %s forged from %s %s ore", name, adj, smith_ores_table[x].color, smith_ores_table[x].name);
        sprintf(long_buf, "Here lies a %s, %s forged from %s %s ore.", name, adj, smith_ores_table[x].color, smith_ores_table[x].name);
        // separate_obj( obj );
        obj_from_char(obj);
        extract_obj(obj);

        if(item->name)
          STRFREE(item->name);
        if(item->short_descr)
          STRFREE(item->short_descr);
        if(item->description)
          STRFREE(item->description);

        item->name = STRALLOC(name_buf);
        item->short_descr = STRALLOC(short_buf);
        item->description = STRALLOC(long_buf);

        item->item_type = item_type;
        item->wear_flags += wearflags;
        item->weight = weight;
        item->cost = cost;
        if(item->item_type == ITEM_ARMOR)
        {
          send_to_char("Its a newly forged piece of armor!\r\n", ch);
          item->value[0] = set_min_armor(item->level);
          item->value[1] = set_max_armor(item->level);
          item->value[4] = 777;
          if(str_cmp(arg1, "finger-ring") || str_cmp(arg1, "ear-hoop"))
          {
            item->layers = 128;

          }
          if(ch->pcdata->tradelevel < 5)
          { // first tier crafted armor
            item->value[3] = 100;
          }
          else if(ch->pcdata->tradelevel > 4 && ch->pcdata->tradelevel < 10)
          { // second 
            // 
            // 
            // 
            // 
            // 
            // 
            // 
            // 
            // 
            // 
            // tier 
            // crafted 
            // armor
            item->value[3] = 200;
          }
          else if(ch->pcdata->tradelevel > 9 && ch->pcdata->tradelevel < 20)
          { // third 
            // 
            // 
            // 
            // 
            // 
            // 
            // 
            // 
            // 
            // 
            // tier 
            // crafted 
            // armor
            item->value[3] = 300;
          }
          else if(ch->pcdata->tradelevel >= 20)
          { // fourth tier crafted
            // armor
            item->value[3] = 400;
          }
        }
        if(item->item_type == ITEM_WEAPON)
        {
          send_to_char("Its a newly forged weapon!\r\n", ch);
          item->value[0] = 12;
          item->value[1] = set_tier_min(item->level, tier);
          item->value[2] = set_tier_max(item->level, tier);
          item->value[3] = tier_one[i].base_v3;
          item->value[4] = tier_one[i].base_v4;
          if(ch->pcdata->tradelevel < 5)
          {
            item->value[6] = 200;
          }
          else if(ch->pcdata->tradelevel > 4 && ch->pcdata->tradelevel < 10)
          {
            item->value[6] = 300;
          }
          else if(ch->pcdata->tradelevel > 9 && ch->pcdata->tradelevel < 15)
          {
            item->value[6] = 400;
          }
          else if(ch->pcdata->tradelevel > 14 && ch->pcdata->tradelevel < 20)
          {
            item->value[6] = 500;
          }
          else if(ch->pcdata->tradelevel >= 20)
          {
            item->value[6] = 600;
          }

        }
      }

      sprintf(extra_buf,
              "\r\n&CThis crafted item bears the seal of %s, the %s blacksmith.\r\n",
              ch->name,
              ch->pcdata->tradelevel <= 5 ? "apprentice" : ch->pcdata->tradelevel <=
              10 ? "journeyman" : ch->pcdata->tradelevel <= 19 ? "expert" : ch->pcdata->tradelevel >= 20 ? "master" : "reknowned");

      EXTRA_DESCR_DATA       *ed;

      CREATE(ed, EXTRA_DESCR_DATA, 1);

      LINK(ed, item->first_extradesc, item->last_extradesc, next, prev);
      ed->keyword = STRALLOC(item->name);
      ed->description = STRALLOC(extra_buf);

      /*
       * Random Affect applier - Code to make crafted items worth it
       */
      if(ch->pcdata->tradelevel > 4)
      {
        // Random affect of armor, str, con, dex, int, wis, cha, lck, hitroll,
        // damroll
        short                   imbue;

        if(item->item_type == ITEM_ARMOR)
        {
          imbue = number_range(1, 8);
        }
        else if(item->item_type == ITEM_WEAPON)
        {
          imbue = number_range(1, 8);
        }
        else
        {
          imbue = number_range(1, 8);
        }

        short                   bonus = 0;

        if(ch->pcdata->tradelevel < 10)
        {
          bonus = 1;
        }
        else if(ch->pcdata->tradelevel > 9)
        {
          bonus = 2;
        }
        if(item->level < 49)
        {
          bonus += 1;
        }
        else if(item->level > 50 && item->level < 90)
        {
          bonus += 2;
        }
        else if(item->level > 89)
        {
          bonus += 3;
        }

        if(item->item_type == ITEM_ARMOR && item->level > 60)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_HITROLL;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        if(imbue == 1)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_AC;
          paf->modifier = -5;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        else if(imbue == 2)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_STR;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        else if(imbue == 3)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_CON;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        else if(imbue == 4)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_DEX;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        else if(imbue == 5)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_INT;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        else if(imbue == 6)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_WIS;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        else if(imbue == 7)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_CHA;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        else if(imbue == 8)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_LCK;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        if(item->level > 70)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_DAMROLL;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
      }
      if(item->item_type == ITEM_WEAPON)
      {
        short                   qball = 0;

        qball = weapon_bonus(ch->pcdata->tradelevel, item->level);

        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_HITROLL;
        paf->modifier = qball;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);

        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_DAMROLL;
        paf->modifier = qball;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);

        if(item->value[4] == 0 || item->value[4] == 4 || item->value[4] == 8)
        {
          short                   bonus;

          bonus = item->level / 5;
          if(bonus < 1)
          {
            bonus = 1;
          }
          item->value[1] = set_min_chart(item->level) + bonus * 3 + set_min_chart(item->level / 2);
          item->value[2] = set_max_chart(item->level) + bonus + (set_max_chart(item->level) / 2);
          item->pIndexData->value[1] = set_min_chart(item->level) + bonus * 3 + set_min_chart(item->level / 2);
          item->pIndexData->value[2] = set_max_chart(item->level) + bonus + (set_max_chart(item->level) / 2);
          item->weight = 15;
        }

        if(ch->pcdata->tradelevel <= 5)
        {
          item->value[0] = 6;
          item->value[1] -= 1;
          item->value[2] -= 1;
          GET_VALUE(item, type) = CURR_COPPER;
          item->cost = 25;
        }

        if(ch->pcdata->tradelevel > 5)
        {
          int                     modifier;

          if(ch->pcdata->tradelevel >= 15)
          {
            if(item->level >= 90)
              modifier = 15;
            else if(item->level >= 70)
              modifier = 8;
            else if(item->level >= 50)
              modifier = 5;
            else if(item->level >= 30)
              modifier = 3;
            else
              modifier = 2;
          }
          else if(ch->pcdata->tradelevel >= 20)
          {
            if(item->level >= 90)
              modifier = 12;
            else if(item->level >= 70)
              modifier = 10;
            else if(item->level >= 50)
              modifier = 8;
            else if(item->level >= 30)
              modifier = 6;
            else
              modifier = 4;
          }
          else
            modifier = 1;

          item->value[0] = 8;

          /*
           * Chance to make it better or worser 
           */
          if(number_percent() > 50)
            modifier += number_range(1, 5);
          else if(number_percent() < 50)
            modifier -= number_range(1, 5);
          if(modifier == 0)
            modifier = 1;

          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_HIT;
          paf->modifier = modifier;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
          GET_VALUE(item, type) = CURR_SILVER;
          item->cost = 10;
        }
        if(ch->pcdata->tradelevel >= 15)
        {
          int                     modifier;

          if(ch->pcdata->tradelevel >= 20)
          {
            if(item->level >= 90)
              modifier = 12;
            else if(item->level >= 70)
              modifier = 10;
            else if(item->level >= 50)
              modifier = 8;
            else if(item->level >= 30)
              modifier = 6;
            else
              modifier = 4;
          }
          else
          {
            if(item->level >= 90)
              modifier = 10;
            else if(item->level >= 70)
              modifier = 8;
            else if(item->level >= 50)
              modifier = 6;
            else if(item->level >= 30)
              modifier = 4;
            else
              modifier = 3;
          }
          item->value[0] = 10;
          item->value[1] += 1;
          item->value[2] += 1;

          /*
           * Chance to make it better or worser 
           */
          if(number_percent() > 50)
            modifier += number_range(1, 5);
          else if(number_percent() < 50)
            modifier -= number_range(1, 5);
          if(modifier == 0)
            modifier = 1;

          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_MOVE;
          paf->modifier = modifier;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
          GET_VALUE(item, type) = CURR_SILVER;
          item->cost = 25;
        }
        if(ch->pcdata->tradelevel >= 20)
        {
          GET_VALUE(item, type) = CURR_GOLD;
          item->cost = 25;
        }
      }
      else if(item->item_type == ITEM_ARMOR)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_HIT;

        if(ch->pcdata->tradelevel >= 20)
        {
          if(item->level >= 90)
            paf->modifier = 20;
          else if(item->level >= 70)
            paf->modifier = 10;
          else if(item->level >= 50)
            paf->modifier = 8;
          else if(item->level >= 30)
            paf->modifier = 5;
          else
            paf->modifier = 3;
          GET_VALUE(item, type) = CURR_GOLD;
          item->cost = 25;
        }
        else if(ch->pcdata->tradelevel >= 15)
        {
          if(item->level >= 90)
            paf->modifier = 15;
          else if(item->level >= 70)
            paf->modifier = 8;
          else if(item->level >= 50)
            paf->modifier = 5;
          else if(item->level >= 30)
            paf->modifier = 3;
          else
            paf->modifier = 2;
          GET_VALUE(item, type) = CURR_SILVER;
          item->cost = 25;
        }
        else if(ch->pcdata->tradelevel > 5)
        {
          paf->modifier = 2;
          GET_VALUE(item, type) = CURR_SILVER;
          item->cost = 10;
        }
        else
        {
          GET_VALUE(item, type) = CURR_COPPER;
          item->cost = 25;
          paf->modifier = 1;
        }

        /*
         * Chance to make it better or worser 
         */
        if(number_percent() > 50)
          paf->modifier += number_range(1, 5);
        else if(number_percent() < 50)
          paf->modifier -= number_range(1, 5);
        if(paf->modifier == 0)
          paf->modifier = 1;

        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);

        if(number_percent() > 80)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_AC;
          paf->modifier = number_range(-10, 10);
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        item->value[1] += get_armor_ac_mod(item->level, ch->pcdata->tradelevel);
        item->value[0] = item->value[1];
      }

      {
        int                     ichange = 0;

        if(!str_cmp(arg1, "silver"))
          ichange = 3;
        else if(!str_cmp(arg1, "gold"))
          ichange = 5;
        else if(!str_cmp(arg1, "iron"))
          ichange = 8;
        else if(!str_cmp(arg1, "steel"))
          ichange = 10;
        else if(!str_cmp(arg1, "titanium"))
          ichange = 12;

        if(item->item_type == ITEM_WEAPON)
        {
          item->value[0] += ichange;
          item->value[2] += ichange;
        }
        if(item->item_type == ITEM_ARMOR)
        {
          item->value[0] += ichange;
          item->value[1] += ichange;
        }
      }

      if(ch->carry_number + get_obj_number(item) > can_carry_n(ch))
      {
        send_to_char("You can't carry that many items, and drop the forged ore into the furnace.\r\n", ch);
        separate_obj(item);
        // obj_from_char( item );
        extract_obj(item);
        return;
      }

      if((ch->carry_weight + get_obj_weight(item, FALSE)) > can_carry_w(ch))
      {
        send_to_char("You can't carry that much weight, and drop the forged ore into the furnace.\r\n", ch);
        separate_obj(item);
        // obj_from_char( item );
        extract_obj(item);
        return;
      }
      if(item)
      {
        obj_to_char(item, ch);
      }
      {
        short                   extinguish = number_chance(1, 8);

        if(extinguish == 8 && !IS_AFFECTED(ch, AFF_BOOST))
        {
          send_to_char("\r\n&wThe forge burns the last of the coal and the flame is extinguished.\r\n", ch);
          stove->value[0] = 0;
        }
      }

      if(item->item_type == ITEM_WEAR_HEAD || item->item_type == ITEM_WEAR_FINGER || item->item_type == ITEM_WEAR_WRIST || item->item_type == ITEM_WIELD)
      {
        item->pIndexData->layers = 0;
        item->layers = 0;
      }

      if(item->item_type == ITEM_ARMOR && item->wear_loc == WEAR_FACE)
        item->layers = 0;

      if(IS_CLANNED(ch))
      {
        CLAN_DATA              *clan;

        clan = ch->pcdata->clan;
        ch->pcdata->clanpoints += 1;
        clan->totalpoints += 1;
        ch_printf(ch, "\r\n&G%s clan has gained a status point from your craftsmanship, now totaling %d clan status points!\r\n", clan->name, clan->totalpoints);
        save_char_obj(ch);
        save_clan(clan);
      }
      return;
    }
    tail_chain();
  }
}

void send_tan_syntax(CHAR_DATA *ch)
{
  if(!ch)
    return;
  if(IS_BLIND(ch))
  {
    send_to_char("Type tan hide into item level\r\n", ch);
    send_to_char("Type tan list for a list of items you can tan or tan list all to see everything.\r\n", ch);
    if(ch->pcdata->tradelevel > 9)
    {
      send_to_char("Type tan Id number repair\r\n", ch);
      send_to_char("eg. tan 3232 repair\r\n", ch);
    }

    if(ch->pcdata->tradelevel > 14)
    {
      send_to_char("Type tan id number rename new name to change the objects name or tan id number rekey new key to change what you type to get the object.\r\n", ch);
    }
    send_to_char("Note There are 4 steps, keep doing the tan command until the last step.\r\n", ch);
  }
  else
  {
    send_to_char("&cSyntax: tan <&Chide&c> into <&Citem&c> <&Clevel&c>\r\n", ch);
    send_to_char("Syntax: tan list or tan list all\r\n", ch);
    if(ch->pcdata->tradelevel > 9)
    {
      send_to_char("Syntax: tan id# repair\r\n", ch);
      send_to_char("eg. tan 3232 repair\r\n", ch);
    }

    if(ch->pcdata->tradelevel > 14)
    {
      send_to_char("Syntax: tan id# rename '&Cnew name&c' or tan id# rekey '&Cnew key&c'\r\n", ch);
    }
    send_to_char("Note: There are &C4 steps&c, keep doing the tan command until the last step.\r\n", ch);
  }
}

void do_tan(CHAR_DATA *ch, char *argument)
{
  short                   number;
  OBJ_DATA               *item;
  char                    arg1[MIL], buf[MSL], arg2[MIL], arg3[MIL], arg4[MIL];
  char                    name_buf[MSL], short_buf[MSL], long_buf[MSL], extra_buf[MSL];
  const char             *adj;
  int                     i = 0, x = 0, sn = 1, usetan, usehunt;
  bool                    itm = FALSE;
  short                   chance;
  AFFECT_DATA            *paf;
  struct skill_type      *skill = NULL;

  chance = number_range(1, 10);

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  argument = one_argument(argument, arg3);
  argument = one_argument(argument, arg4);

  name_buf[0] = '\0';
  short_buf[0] = '\0';
  long_buf[0] = '\0';
  extra_buf[0] = '\0';

  if(IS_NPC(ch))
    return;

  if(arg1[0] == '\0')
  {
    send_tan_syntax(ch);
    return;
  }

  if(ch->move < 1)
  {
    send_to_char("You don't have enough move points to perform that.\r\n", ch);
    return;
  }
  else
  {
    ch->move -= 1;
  }

  if(ch->position != POS_STANDING)
  {
    send_to_char("You should be standing to attempt this.\r\n", ch);
  }

// Just adding objID to work for repair code section 

  if(!str_cmp(arg2, "repair"))
  {
    OBJ_DATA               *repair;

    for(repair = ch->first_carrying; repair; repair = repair->next_content)
    {
      if(atoi(arg1) == repair->guid && (repair->item_type == ITEM_ARMOR || repair->item_type == ITEM_WEAPON))
        break;
    }

    if(!repair)
    {
      send_to_char("You do not have any repairable equipment to repair.\r\n", ch);
      return;
    }

    if(ch->pcdata->tradelevel < 10)
    {
      send_to_char("You are not skilled enough of a craftsman to repair items yet.\r\n", ch);
      return;
    }

    if(repair->item_type == ITEM_WEAPON && repair->value[0] == 12)
    {
      send_to_char("It is already honed to perfection.\r\n", ch);
      return;
    }
    if(repair->item_type == ITEM_ARMOR && repair->value[0] == repair->value[1])
    {
      send_to_char("It is already forged to perfection.\r\n", ch);
      return;
    }

    act(AT_CYAN, "$n hammers $p back to shape.", ch, repair, NULL, TO_ROOM);
    act(AT_CYAN, "You hammer $p back to shape.", ch, repair, NULL, TO_CHAR);
    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/hammer.wav)\r\n", ch);

    switch (repair->item_type)
    {
      case ITEM_ARMOR:
        repair->value[0] = repair->value[1];
        break;
      case ITEM_WEAPON:
        repair->value[0] = INIT_WEAPON_CONDITION;
        break;
    }
    return;
  }

// End of repair code section 

  if(!str_cmp(arg2, "rename") && ch->pcdata->tradelevel > 14 && !xIS_SET(ch->act, PLR_NORENAME))
  {
    OBJ_DATA               *item;

    for(item = ch->first_carrying; item; item = item->next_content)
    {
      if(atoi(arg1) == item->guid && item->pIndexData->vnum == SMITH_PRODUCT)
        break;
    }
    if(!item)
    {
      send_to_char("You do not have any tanned items to rename or rekey.\r\n", ch);
      return;
    }

    if(!arg3 || arg3[0] == '\0')
    {
      send_to_char("You must specify the new keywords, ie tan id number rename 'new name' or rename 'ring toering'.\r\n", ch);
      return;
    }
    ch_printf(ch, "&cChanging item's description from &C%s&c to&C %s&c..\r\n", item->short_descr, arg3);
    STRFREE(item->short_descr);
    item->short_descr = STRALLOC(arg3);
    sprintf(long_buf, "%s has been left here along the ground.", item->short_descr);
    STRFREE(item->description);
    item->description = STRALLOC(long_buf);
    send_to_char("&RDone!&c Please note, the KEYWORDS used to drop/equip/etc remain the same.\r\n", ch);
    save_char_obj(ch);
    return;
  }
  else if(!str_cmp(arg2, "rename") && (ch->pcdata->tradelevel < 15 || xIS_SET(ch->act, PLR_NORENAME)))
  {
    send_to_char("You must specify the new keywords, ie rename id number 'new name' or rename 'ring toering'.\r\n", ch);
    return;
  }

  if(!str_cmp(arg2, "rekey") && ch->pcdata->tradelevel > 14 && !xIS_SET(ch->act, PLR_NORENAME))
  {
    OBJ_DATA               *item;

    for(item = ch->first_carrying; item; item = item->next_content)
    {
      if(atoi(arg1) == item->guid && item->pIndexData->vnum == SMITH_PRODUCT)
        break;
    }
    if(!item)
    {
      send_to_char("You do not have any tanned items to rename or rekey.\r\n", ch);
      return;
    }

    ch_printf(ch, "&cChanging item's keywords from &C%s&c to &C%s&c..\r\n", item->name, arg3);
    STRFREE(item->name);
    item->name = STRALLOC(arg3);
    send_to_char("&RDone!&c Please note, the DESCRIPTION when looking at the item remains the same.\r\n", ch);
    save_char_obj(ch);
    return;
  }
  else if(!str_cmp(arg2, "rekey") && (ch->pcdata->tradelevel < 15 || xIS_SET(ch->act, PLR_NORENAME)))
  {
    send_to_char("You must specify the new keywords, ie 'tan rekey whatever new name' or 'tan rekey sword toothpick'.\r\n", ch);
    return;
  }

  if(!str_cmp(arg1, "list"))
  {
    int                     y, count = 0;
    char                    wearloc[MSL];
    short                   value;

    send_to_char("\t\t\t&CThe Tannery Listing\r\n\r\n", ch);

    if(!str_cmp(arg2, "all"))
    {
      for(y = 0; y < MAX_TANNED; y++)
      {
        count++;
        ch_printf(ch, "&c%-15s  %-3d ", tanned_one[y].name ? tanned_one[y].name : "null name", tanned_one[y].level);

        if(count == 3)
        {
          count = 0;
          send_to_char("\r\n", ch);
        }
      }
      if(count != 0)
        send_to_char("\r\n", ch);
      send_tan_syntax(ch);
      return;

    }
    for(y = 0; y < MAX_TANNED; y++)
    {
      if(ch->pcdata->tradelevel < tanned_one[y].level)
        continue;
      count++;
      ch_printf(ch, "&c%-15s      ", tanned_one[y].name ? tanned_one[y].name : "null name");
      if(count == 3)
      {
        count = 0;
        send_to_char("\r\n", ch);
      }
    }
    if(count != 0)
      send_to_char("\r\n", ch);
    send_to_char("\r\n", ch);
    send_tan_syntax(ch);
    return;
  }

  OBJ_DATA               *bag, *obj;

  for(bag = ch->first_carrying; bag; bag = bag->next_content)
  {
    if(bag->item_type == ITEM_HUNTERS_BAG)
      break;
  }

  if(!bag)
  {
    send_to_char("You do not have a hunter's bag to tan with.\r\n", ch);
    return;
  }

  if((bag->craft1 < 1 && !str_cmp(arg1, "rabbit")) ||
     (bag->craft2 < 1 && !str_cmp(arg1, "goose")) ||
     (bag->craft3 < 1 && !str_cmp(arg1, "badger")) ||
     (bag->craft4 < 1 && !str_cmp(arg1, "turkey")) ||
     (bag->craft5 < 1 && !str_cmp(arg1, "coyote")) ||
     (bag->craft6 < 1 && !str_cmp(arg1, "leopard")) ||
     (bag->craft7 < 1 && !str_cmp(arg1, "lion")) ||
     (bag->craft8 < 1 && !str_cmp(arg1, "pheasant")) ||
     (bag->craft9 < 1 && !str_cmp(arg1, "duck")) ||
     (bag->craft10 < 1 && !str_cmp(arg1, "raccoon")) ||
     (bag->craft11 < 1 && !str_cmp(arg1, "boar")) ||
     (bag->craft12 < 1 && !str_cmp(arg1, "bobcat")) ||
     (bag->craft13 < 1 && !str_cmp(arg1, "bear")) ||
     (bag->craft14 < 1 && !str_cmp(arg1, "dove")) ||
     (bag->craft15 < 1 && !str_cmp(arg1, "squirrel")) ||
     (bag->craft16 < 1 && !str_cmp(arg1, "deer")) ||
     (bag->craft17 < 1 && !str_cmp(arg1, "antelope")) ||
     (bag->craft18 < 1 && !str_cmp(arg1, "wolf")) || (bag->craft19 < 1 && !str_cmp(arg1, "moose")) || (bag->craft20 < 1 && !str_cmp(arg1, "bugbear")))
  {

    send_to_char("You would need to hunt more kills for your hunter's bag first.\r\n", ch);
    return;
  }

  if(!str_cmp(arg1, "rabbit"))
  {
    bag->craft1 -= 1;
  }
  else if(!str_cmp(arg1, "goose"))
  {
    bag->craft2 -= 1;
  }
  else if(!str_cmp(arg1, "badger"))
  {
    bag->craft3 -= 1;
  }
  else if(!str_cmp(arg1, "turkey"))
  {
    bag->craft4 -= 1;
  }
  else if(!str_cmp(arg1, "coyote"))
  {
    bag->craft5 -= 1;
  }
  else if(!str_cmp(arg1, "leopard"))
  {
    bag->craft6 -= 1;
  }
  else if(!str_cmp(arg1, "lion"))
  {
    bag->craft7 -= 1;
  }
  else if(!str_cmp(arg1, "pheasant"))
  {
    bag->craft8 -= 1;
  }
  else if(!str_cmp(arg1, "duck"))
  {
    bag->craft9 -= 1;
  }
  else if(!str_cmp(arg1, "raccoon"))
  {
    bag->craft10 -= 1;
  }
  else if(!str_cmp(arg1, "boar"))
  {
    bag->craft11 -= 1;
  }
  else if(!str_cmp(arg1, "bobcat"))
  {
    bag->craft12 -= 1;
  }
  else if(!str_cmp(arg1, "bear"))
  {
    bag->craft13 -= 1;
  }
  else if(!str_cmp(arg1, "dove"))
  {
    bag->craft14 -= 1;
  }
  else if(!str_cmp(arg1, "squirrel"))
  {
    bag->craft15 -= 1;
  }
  else if(!str_cmp(arg1, "deer"))
  {
    bag->craft16 -= 1;
  }
  else if(!str_cmp(arg1, "antelope"))
  {
    bag->craft17 -= 1;
  }
  else if(!str_cmp(arg1, "wolf"))
  {
    bag->craft18 -= 1;
  }
  else if(!str_cmp(arg1, "moose"))
  {
    bag->craft19 -= 1;
  }
  else if(!str_cmp(arg1, "bugbear"))
  {
    bag->craft20 -= 1;
  }
  else
  {
    send_to_char("That's not a proper hide.\r\n", ch);
    return;
  }

  if(arg2[0] == '\0' || arg3[0] == '\0')
  {
    send_tan_syntax(ch);
    return;

  }
  if(str_cmp(arg2, "into"))
  {
    send_tan_syntax(ch);
    return;
  }

  if(!IS_SET(ch->in_room->room_flags, ROOM_TRADESKILLS))
  {
    send_to_char("You must be in a tradeskills building to do this.\r\n", ch);
    return;
  }

  {

    for(i = 0; i < MAX_TANNED; i++)
    {
      if(!str_cmp(arg3, tanned_one[i].name))
      {
        usetan = i;
        itm = TRUE;
        break;
      }
    }

    if(!itm || ch->pcdata->tradelevel < tanned_one[i].level)
    {
      send_to_char("This is not a valid item type.\r\n", ch);
      return;
    }

    bool                    wrong = FALSE;

    for(x = 0; x < MAX_HUNTED; x++)
    {
      if(!str_cmp(arg1, hunted_one[x].name))
      {
        wrong = TRUE;
        break;
      }
    }

    if(!wrong || ch->pcdata->tradelevel < hunted_one[x].level)
    {
      send_to_char("This is not a valid item type.\r\n", ch);
      return;
    }

    SKILLTYPE              *skill = get_skilltype(gsn_tan);

    if(ch->pcdata->tradelevel <= 1)
      adj = "poorly";
    else if(ch->pcdata->tradelevel <= 5)
      adj = "simply";
    else if(ch->pcdata->tradelevel <= 8)
      adj = "properly";
    else if(ch->pcdata->tradelevel <= 10)
      adj = "well";
    else if(ch->pcdata->tradelevel <= 15)
      adj = "finely";
    else if(ch->pcdata->tradelevel <= 19)
      adj = "masterfully";
    else
      adj = "legendary";

    WAIT_STATE(ch, 15);

    short                   failure = 0;

    if(ch->pcdata->tradelevel < 5)
      failure = 14;
    else if(ch->pcdata->tradelevel < 10 && ch->pcdata->tradelevel > 4)
      failure = 10;
    else if(ch->pcdata->tradelevel < 15 && ch->pcdata->tradelevel > 9)
      failure = 8;
    else if(ch->pcdata->tradelevel < 20 && ch->pcdata->tradelevel > 14)
      failure = 5;
    else if(ch->pcdata->tradelevel == 20)
      failure = 2;

    if(bag->value[6] == 0)
    {
      if(number_percent() <= failure)
      {
        act(AT_CYAN, "$n prepares the hide by placing it in a lime water mixture filled basin, frowns as $e realizes the hide has already started rotting.", ch, NULL, NULL, TO_ROOM);
        act(AT_CYAN, "You prepare the hide by placing it in a lime water mixture filled basin to wash off any blood, but you realize the hide has already started rotting.", ch, NULL, NULL, TO_CHAR);
        if(xIS_SET(ch->act, PLR_CRAFTS))
          send_to_char("!!SOUND(sound/liquid.wav)\r\n", ch);

        learn_from_failure(ch, gsn_tan);
        return;
      }

      act(AT_CYAN, "$n prepares the hide by placing it in the lime water mixture filled basin.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You prepare the hide by placing it in the lime water mixture filled basin, and begin scrubbing off any blood.", ch, NULL, NULL, TO_CHAR);
      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/liquid.wav)\r\n", ch);

      bag->value[6] = 1;
      obj = create_object(get_obj_index(OBJ_VNUM_ORE), 0);
      if(obj->name)
        STRFREE(obj->name);
      if(obj->short_descr)
        STRFREE(obj->short_descr);
      if(obj->description)
        STRFREE(obj->description);
      obj->name = STRALLOC("hide");
      obj->description = STRALLOC("A prepared hide has been left here.");
      obj->short_descr = STRALLOC("a prepared hide");
      obj_to_char(obj, ch);
      return;
    }

    if(bag->value[6] == 1)
    {
      if(!can_use_skill(ch, number_percent(), gsn_tan) || number_percent() <= failure)
      {
        act(AT_CYAN, "$n scrubs away at the hide.", ch, NULL, NULL, TO_ROOM);
        act(AT_CYAN, "You scrub away at the hide.", ch, NULL, NULL, TO_CHAR);
        if(xIS_SET(ch->act, PLR_CRAFTS))
          send_to_char("!!SOUND(sound/grind2.wav)\r\n", ch);

        learn_from_failure(ch, gsn_tan);
        return;
      }

      act(AT_CYAN, "$n places nutgals tannic acid on the hide to preserve it.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You place nutgals tannic acid on the hide to preserve it.", ch, NULL, NULL, TO_CHAR);
      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/sizzle.wav)\r\n", ch);

      bag->value[6] = 2;
      for(obj = ch->first_carrying; obj; obj = obj->next_content)
      {
        if(obj->item_type == ITEM_RAW)
          break;
      }

      if(!obj)
      {
        send_to_char("You do not have your hide.\r\n", ch);
        bag->value[6] = 0;
        return;
      }

      if(VLD_STR(obj->short_descr))
        STRFREE(obj->short_descr);
      obj->short_descr = STRALLOC("a preserved hide");
      return;
    }

    if(bag->value[6] == 2)
    {
      act(AT_CYAN, "$n treats the hide with linseed oil, and cuts it into shapes.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You treat the hide with linseed oil, and cut it into the desired shape.", ch, NULL, NULL, TO_CHAR);
      if(number_range(1, 4) > 2)
        learn_from_craft(ch, gsn_tan);  // add a chance of gaining exp

      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/scissor.wav)\r\n", ch);

      bag->value[6] = 3;
      for(obj = ch->first_carrying; obj; obj = obj->next_content)
      {
        if(obj->item_type == ITEM_RAW)
          break;
      }

      if(!obj)
      {
        send_to_char("You do not have your hide.\r\n", ch);
        bag->value[6] = 0;
        return;
      }

      if(VLD_STR(obj->short_descr))
        STRFREE(obj->short_descr);
      obj->short_descr = STRALLOC("a pattern of hide shapes");
      return;
    }

    act(AT_CYAN, "$n sews the hide pattern shapes into a tanned final product.", ch, NULL, NULL, TO_ROOM);
    act(AT_CYAN, "You sew the hide pattern shapes into a tanned final product.", ch, NULL, NULL, TO_CHAR);
    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/assemble.wav)\r\n", ch);

    for(obj = ch->first_carrying; obj; obj = obj->next_content)
    {
      if(obj->item_type == ITEM_RAW)
        break;
    }

    if(!obj)
    {
      send_to_char("You do not have your hide.\r\n", ch);
      bag->value[6] = 0;
      return;
    }
    separate_obj(obj);
    obj_from_char(obj);
    extract_obj(obj);
    bag->value[6] = 0;
    learn_from_craft(ch, gsn_tan);
    item = create_object(get_obj_index(SMITH_PRODUCT), 1);
    if(arg4[0] == '\0')
    {
      item->level = ch->level;
    }
    else
      item->level = atoi(arg4);

    bool                    animal = FALSE;

    for(x = 0; x < MAX_HUNTED; x++)
    {
      if(!str_cmp(arg1, hunted_one[x].name))
      {
        animal = TRUE;
        break;
      }
    }

    if(!animal || ch->pcdata->tradelevel < hunted_one[x].level)
    {
      send_to_char("This is not a valid item type.\r\n", ch);
      return;
    }

    sprintf(name_buf, "%s", tanned_one[i].name);
    sprintf(short_buf, "%s, %s tanned from %s", tanned_one[i].short_descr, adj, hunted_one[x].name);
    sprintf(long_buf, "Here lies %s, %s tanned from %s.", tanned_one[i].short_descr, adj, hunted_one[x].name);
    item->item_type = tanned_one[i].item_type;
    item->wear_flags += tanned_one[i].wear_flags;
    item->weight = (tanned_one[i].weight);
    item->cost = tanned_one[i].weight;
    item->value[0] = set_min_armor(item->level);
    item->value[1] = set_max_armor(item->level);
    item->pIndexData->value[0] = set_min_armor(item->level);
    item->pIndexData->value[1] = set_max_armor(item->level);

    if(ch->carry_number + get_obj_number(item) > can_carry_n(ch))
    {
      send_to_char("You can't carry that many items, and drop the tanned hide into the tannic acid.\r\n", ch);
      separate_obj(item);
      // obj_from_char( item );
      extract_obj(item);
      return;
    }

    if((ch->carry_weight + get_obj_weight(item, FALSE)) > can_carry_w(ch))
    {
      send_to_char("You can't carry that much weight, and drop the tanned hide into the tannic acid.\r\n", ch);
      separate_obj(item);
      extract_obj(item);
      return;
    }

    /*
     * Random Affect applier - Code to make crafted items worth it
     */
    if(ch->pcdata->tradelevel > 4)
    {
      // Random affect of armor, str, con, dex, int, wis, cha, lck, hitroll,
      // damroll
      short                   imbue;

      if(item->item_type == ITEM_ARMOR)
      {
        imbue = number_range(1, 8);
      }
      else if(item->item_type == ITEM_WEAPON)
      {
        imbue = number_range(1, 8);
      }
      else
      {
        imbue = number_range(1, 8);
      }

      short                   bonus = 0;

      if(ch->pcdata->tradelevel < 10)
      {
        bonus = 1;
      }
      else if(ch->pcdata->tradelevel > 9)
      {
        bonus = 2;
      }
      if(item->level < 49)
      {
        bonus += 1;
      }
      else if(item->level > 50 && item->level < 90)
      {
        bonus += 2;
      }
      else if(item->level > 89)
      {
        bonus += 3;
      }
      if(item->item_type == ITEM_ARMOR && item->level > 60)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_HITROLL;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }

      if(imbue == 1)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_AC;
        paf->modifier = -5;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }
      else if(imbue == 2)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_STR;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }
      else if(imbue == 3)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_CON;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }
      else if(imbue == 4)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_DEX;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }
      else if(imbue == 5)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_INT;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }
      else if(imbue == 6)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_WIS;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }
      else if(imbue == 7)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_CHA;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }
      else if(imbue == 8)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_LCK;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }
      if(item->level > 70)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_DAMROLL;
        paf->modifier = bonus;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }

    }

    if(item->item_type == ITEM_WEAPON)
    {
      short                   qball = 0;

      qball = weapon_bonus(ch->pcdata->tradelevel, item->level);

      CREATE(paf, AFFECT_DATA, 1);

      paf->type = sn;
      paf->duration = -1;
      paf->location = APPLY_DAMROLL;
      paf->modifier = qball;
      xCLEAR_BITS(paf->bitvector);
      LINK(paf, item->first_affect, item->last_affect, next, prev);

      CREATE(paf, AFFECT_DATA, 1);

      paf->type = sn;
      paf->duration = -1;
      paf->location = APPLY_HITROLL;
      paf->modifier = qball;
      xCLEAR_BITS(paf->bitvector);
      LINK(paf, item->first_affect, item->last_affect, next, prev);
    }

    if(ch->pcdata->tradelevel < 5)
    {
      item->value[0] = item->value[0] - 1;
      item->value[1] = item->value[1] - 1;
      GET_VALUE(item, type) = CURR_COPPER;
      item->cost = 25;
    }

    if(ch->pcdata->tradelevel < 15 && ch->pcdata->tradelevel >= 5)
    {
      CREATE(paf, AFFECT_DATA, 1);

      paf->type = sn;
      paf->duration = -1;
      paf->location = APPLY_HIT;
      paf->modifier = 1;
      xCLEAR_BITS(paf->bitvector);
      LINK(paf, item->first_affect, item->last_affect, next, prev);
      GET_VALUE(item, type) = CURR_SILVER;
      item->cost = 10;
    }

    if(ch->pcdata->tradelevel < 20 && ch->pcdata->tradelevel >= 15)
    {
      CREATE(paf, AFFECT_DATA, 1);

      paf->type = sn;
      paf->duration = -1;
      paf->location = APPLY_HIT;
      if(item->level >= 90)
        paf->modifier = 15;
      if(item->level >= 70 && item->level < 90)
        paf->modifier = 8;
      if(item->level >= 50 && item->level < 70)
        paf->modifier = 5;
      if(item->level >= 30 && item->level < 50)
        paf->modifier = 3;
      if(item->level < 30)
        paf->modifier = 2;
      xCLEAR_BITS(paf->bitvector);
      LINK(paf, item->first_affect, item->last_affect, next, prev);
      GET_VALUE(item, type) = CURR_SILVER;
      item->cost = 20;
    }

    if(ch->pcdata->tradelevel >= 20)
    {
      CREATE(paf, AFFECT_DATA, 1);

      paf->type = sn;
      paf->duration = -1;
      paf->location = APPLY_HIT;
      if(item->level >= 90)
        paf->modifier = 20;
      if(item->level >= 70 && item->level < 90)
        paf->modifier = 10;
      if(item->level >= 50 && item->level < 70)
        paf->modifier = 8;
      if(item->level >= 30 && item->level < 50)
        paf->modifier = 5;
      if(item->level < 30)
        paf->modifier = 3;
      xCLEAR_BITS(paf->bitvector);
      LINK(paf, item->first_affect, item->last_affect, next, prev);
      GET_VALUE(item, type) = CURR_GOLD;
      item->cost = 10;
    }

    int                     tier;

    if(tanned_one[i].level >= 15)
      tier = 3;
    else if(tanned_one[i].level > 5 && tanned_one[i].level < 15)
      tier = 2;
    else
      tier = 1;

    if(item->item_type == ITEM_ARMOR)
    {
      item->size = 77;
      item->layers = 128;
      item->value[4] = 777;
      if(number_percent() > 80)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_AC;
        paf->modifier = number_range(-10, 10);
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);
      }

      if(ch->pcdata->tradelevel < 5)
      { // first tier crafted armor
        item->value[3] = 100;

      }
      else if(ch->pcdata->tradelevel > 4 && ch->pcdata->tradelevel < 10)
      { // second 
        // 
        // 
        // tier 
        // crafted 
        // armor
        item->value[3] = 200;
      }
      else if(ch->pcdata->tradelevel > 9 && ch->pcdata->tradelevel < 20)
      { // third 
        // 
        // 
        // tier 
        // crafted 
        // armor
        item->value[3] = 300;
      }
      else if(ch->pcdata->tradelevel >= 20)
      { // fourth tier crafted armor
        item->value[3] = 400;
      }
    }

    if(item->item_type == ITEM_CONTAINER)
    {
      if(ch->pcdata->tradelevel < 5)
      {
        item->value[0] = 300;
        item->value[1] = 1;
        item->value[2] = 0;
        item->value[3] = 12;
      }
      else if(ch->pcdata->tradelevel > 4 && ch->pcdata->tradelevel < 10)
      { // second 
        // 
        // 
        // tier 
        // crafted 
        // armor
        item->value[0] = 400;
        item->value[1] = 1;
        item->value[2] = 0;
        item->value[3] = 12;
      }
      else if(ch->pcdata->tradelevel > 9 && ch->pcdata->tradelevel < 20)
      { // third 
        // 
        // 
        // tier 
        // crafted 
        // armor
        item->value[0] = 800;
        item->value[1] = 1;
        item->value[2] = 0;
        item->value[3] = 12;
      }
      else if(ch->pcdata->tradelevel >= 20)
      { // fourth tier crafted armor
        item->value[0] = 1000;
        item->value[1] = 1;
        item->value[2] = 0;
        item->value[3] = 12;
      }

    }
    if(item->item_type == ITEM_WEAPON)
    {
      send_to_char("Its a newly forged weapon!\r\n", ch);
      item->value[0] = 12;
      item->value[1] = set_tier_min(item->level, tier);
      item->value[2] = set_tier_max(item->level, tier);
      item->value[3] = tanned_one[i].base_v3;
      item->value[4] = tanned_one[i].base_v4;
      if(ch->pcdata->tradelevel < 5)
      {
        item->value[6] = 200;
      }
      else if(ch->pcdata->tradelevel > 4 && ch->pcdata->tradelevel < 10)
      {
        item->value[6] = 300;
      }
      else if(ch->pcdata->tradelevel > 9 && ch->pcdata->tradelevel < 15)
      {
        item->value[6] = 400;
      }
      else if(ch->pcdata->tradelevel > 14 && ch->pcdata->tradelevel < 20)
      {
        item->value[6] = 500;
      }
      else if(ch->pcdata->tradelevel >= 20)
      {
        item->value[6] = 600;
      }
    }
    sprintf(extra_buf,
            "\r\n&CThis tanned item bears the seal of %s, the %s tanner.\r\n",
            ch->name,
            ch->pcdata->tradelevel <= 5 ? "apprentice" : ch->pcdata->tradelevel <= 10 ? "journeyman" : ch->pcdata->tradelevel <= 19 ? "expert" : ch->pcdata->tradelevel >= 20 ? "master" : "reknowned");
    if(VLD_STR(item->name))
      STRFREE(item->name);
    item->name = STRALLOC(name_buf);
    if(VLD_STR(item->short_descr))
      STRFREE(item->short_descr);
    item->short_descr = STRALLOC(short_buf);
    if(VLD_STR(item->description))
      STRFREE(item->description);
    item->description = STRALLOC(long_buf);

    EXTRA_DESCR_DATA       *ed;

    CREATE(ed, EXTRA_DESCR_DATA, 1);

    LINK(ed, item->first_extradesc, item->last_extradesc, next, prev);
    ed->keyword = STRALLOC(item->name);
    ed->description = STRALLOC(extra_buf);
    item->color = 1;
    if(item->item_type == ITEM_QUIVER)
      item->value[1] = 0;

    if(item->item_type == ITEM_WEAR_HEAD || item->item_type == ITEM_WEAR_FINGER || item->item_type == ITEM_WEAR_WRIST || item->item_type == ITEM_WIELD || item->item_type == ITEM_SHEATH)
    {
      item->layers = 0;
    }

    if(item->item_type == ITEM_ARMOR && (item->wear_loc == WEAR_FACE || item->wear_loc == WEAR_NECK_1 || item->wear_loc == WEAR_NECK_2 || item->wear_loc == WEAR_BACK))
      item->layers = 0;

    obj_to_char(item, ch);

    CLAN_DATA              *clan;

    if(IS_CLANNED(ch))
    {
      clan = ch->pcdata->clan;
      ch->pcdata->clanpoints += 1;
      clan->totalpoints += 1;
      ch_printf(ch, "\r\n&G%s clan has gained a status point from your craftsmanship, now totaling %d clan status points!\r\n", clan->name, clan->totalpoints);
      save_char_obj(ch);
      save_clan(clan);
    }

    return;
  }
  tail_chain();
}

void do_hunt(CHAR_DATA *ch, char *argument)
{
  OBJ_DATA               *arrow, *bow, *slice, *skin;
  CHAR_DATA              *victim = NULL, *mob;
  MOB_INDEX_DATA         *prey;
  char                    buf[MSL], arg1[MIL], name_buf[MSL], short_buf[MSL], long_buf[MSL];
  int                     i = 0;
  short                   chance;
  bool                    iprey = FALSE, found = FALSE;

  chance = number_range(1, 10);

  argument = one_argument(argument, arg1);

  if(IS_NPC(ch))
    return;

  if(ch->position != POS_STANDING)
  {
    send_to_char("You should be standing to attempt this.\r\n", ch);
    return;
  }

  switch (ch->in_room->sector_type)
  {
    case SECT_OCEAN:
    case SECT_ARCTIC:
    case SECT_BEACH:
    case SECT_FOG:
    case SECT_SKY:
    case SECT_CLOUD:
    case SECT_SNOW:
    case SECT_ORE:
    case SECT_QUICKSAND:
    case SECT_DEEPMUD:
    case SECT_PORTALSTONE:
    case SECT_LAKE:
    case SECT_CAMPSITE:
    case SECT_DOCK:
    case SECT_RIVER:
    case SECT_WATERFALL:
    case SECT_CROSSROAD:
    case SECT_VROAD:
    case SECT_HROAD:
    case SECT_ROAD:
    case SECT_CITY:
    case SECT_LAVA:
    case SECT_OCEANFLOOR:
    case SECT_AREA_ENT:
    case SECT_AIR:
    case SECT_UNDERWATER:
    case SECT_INSIDE:
      send_to_char("You cannot hunt here.\r\n", ch);
      return;

    default:
      break;
  }

  if(IS_SET(ch->in_room->room_flags, ROOM_INDOORS))
  {
    send_to_char("You can't do that in here.\r\n", ch);
    return;
  }

  if(IS_SET(ch->in_room->area->flags, AFLAG_INDOORS))
  {
    send_to_char("You can't do that in here.\r\n", ch);
    return;
  }

  OBJ_DATA               *bag;

  for(bag = ch->first_carrying; bag; bag = bag->next_content)
  {
    if(bag->item_type == ITEM_HUNTERS_BAG)
      break;
  }

  if(!bag)
  {
    send_to_char("You do not have a hunter's bag to hold your kills with.\r\n", ch);
    return;
  }

  if(bag->craft1 >= 1000 && !str_cmp(arg1, "rabbit"))
  {
    send_to_char("You cannot hold any more rabbits in your hunter's bag.\r\n", ch);
    return;
  }
  else if(bag->craft2 >= 1000 && !str_cmp(arg1, "goose"))
  {
    send_to_char("You cannot hold any more geese in your hunter's bag.\r\n", ch);
    return;
  }
  else if(bag->craft3 >= 1000 && !str_cmp(arg1, "badger"))
  {
    send_to_char("You cannot hold any more badger in your hunter's bag.\r\n", ch);
    return;
  }
  else if(bag->craft4 >= 1000 && !str_cmp(arg1, "turkey"))
  {
    send_to_char("You cannot hold any more turkey in your hunter's bag.\r\n", ch);
    return;
  }
  else if(bag->craft5 >= 1000 && !str_cmp(arg1, "coyote"))
  {
    send_to_char("You cannot hold any more coyote in your hunter's bag.\r\n", ch);
    return;
  }
  else if(bag->craft6 >= 1000 && !str_cmp(arg1, "leopard"))
  {
    send_to_char("You cannot hold any more leopard in your hunter's bag.\r\n", ch);
    return;
  }
  else if(bag->craft7 >= 1000 && !str_cmp(arg1, "lion"))
  {
    send_to_char("You cannot hold any more lion in your hunter's bag.\r\n", ch);
    return;
  }
  else if(bag->craft8 >= 1000 && !str_cmp(arg1, "pheasant"))
  {
    send_to_char("You cannot hold any more pheasant in your hunter's bag.\r\n", ch);
    return;
  }
  else if(bag->craft9 >= 1000 && !str_cmp(arg1, "duck"))
  {
    send_to_char("You cannot hold any more duck in your hunter's bag.\r\n", ch);
    return;
  }
  else if(bag->craft10 >= 1000 && !str_cmp(arg1, "raccoon"))
  {
    send_to_char("You cannot hold any more raccoon in your hunter's bag.\r\n", ch);
    return;
  }
  else if(bag->craft11 >= 1000 && !str_cmp(arg1, "boar"))
  {
    send_to_char("You cannot hold any more boar in your hunter's bag.\r\n", ch);
    return;
  }
  else if(bag->craft12 >= 1000 && !str_cmp(arg1, "bobcat"))
  {
    send_to_char("You cannot hold any more wild bobcat in your hunter's bag.\r\n", ch);
    return;
  }
  else if(bag->craft13 >= 1000 && !str_cmp(arg1, "bear"))
  {
    send_to_char("You cannot hold any more wild bear in your hunter's bag.\r\n", ch);
    return;
  }
  else if(bag->craft14 >= 1000 && !str_cmp(arg1, "dove"))
  {
    send_to_char("You cannot hold any more dove in your hunter's bag.\r\n", ch);
    return;
  }
  else if(bag->craft15 >= 1000 && !str_cmp(arg1, "squirrel"))
  {
    send_to_char("You cannot hold any more squirrel in your hunter's bag.\r\n", ch);
    return;
  }
  else if(bag->craft16 >= 1000 && !str_cmp(arg1, "deer"))
  {
    send_to_char("You cannot hold any more deer in your hunter's bag.\r\n", ch);
    return;
  }
  else if(bag->craft17 >= 1000 && !str_cmp(arg1, "antelope"))
  {
    send_to_char("You cannot hold any more antelope in your hunter's bag.\r\n", ch);
    return;
  }
  else if(bag->craft18 >= 1000 && !str_cmp(arg1, "wolf"))
  {
    send_to_char("You cannot hold any more wolves in your hunter's bag.\r\n", ch);
    return;
  }
  else if(bag->craft19 >= 1000 && !str_cmp(arg1, "moose"))
  {
    send_to_char("You cannot hold any more moose in your hunter's bag.\r\n", ch);
    return;
  }
  else if(bag->craft20 >= 1000 && !str_cmp(arg1, "bugbear"))
  {
    send_to_char("You cannot hold any more bugbear in your hunter's bag.\r\n", ch);
    return;
  }

  if(ch->move < 1)
  {
    send_to_char("You	don't have enough move points to perform that.\r\n", ch);
    return;
  }
  else
  {
    ch->move -= 1;
  }

  if(arg1[0] == '\0')
  {
    if(IS_BLIND(ch))
    {
      send_to_char("Type Hunt prey\r\n", ch);
      send_to_char("Type Hunt list for a list of prey you can hunt.\r\n", ch);
    }
    else
    {
      send_to_char("&cSyntax: Hunt <&Cprey&c>\r\n", ch);
      send_to_char("Syntax: Hunt list\r\n", ch);
    }
    return;
  }

  if(!str_cmp(arg1, "list"))
  {
    int                     count = 0;

    send_to_char("\t\t\t&CThe Listing of Huntable Prey\r\n\r\n", ch);
    for(int y = 0; y < MAX_HUNTED; y++)
    {
      if(ch->pcdata->tradelevel < hunted_one[y].tlevel)
        continue;
      count++;
      ch_printf(ch, "&c%-15s      ", hunted_one[y].name ? hunted_one[y].name : "null name");
      if(count == 3)
      {
        count = 0;
        send_to_char("\r\n", ch);
      }
    }
    if(count != 0)
      send_to_char("\r\n", ch);
    send_to_char("\r\n&cSyntax: Hunt <&Cprey&c>\r\n", ch);
    send_to_char("Syntax: Hunt list\r\n", ch);
    return;
  }

  WAIT_STATE(ch, skill_table[gsn_hunt]->beats);

  if(!can_use_skill(ch, number_percent(), gsn_hunt))
  {
    learn_from_failure(ch, gsn_hunt);
    send_to_char("You stalk the area for your prey, but find nothing.\r\n", ch);
    return;
  }

  if(!(bow = get_eq_char(ch, WEAR_MISSILE_WIELD)))
  {
    send_to_char("But you are not wielding a missile weapon!!\r\n", ch);
    return;
  }

  if((arrow = get_eq_char(ch, WEAR_HOLD)) == NULL)
  {
    send_to_char("You are not holding a projectile!\r\n", ch);
    return;
  }

  if(arrow->item_type != ITEM_PROJECTILE)
  {
    send_to_char("You are not holding a projectile!\r\n", ch);
    return;
  }

  for(i = 0; i < MAX_HUNTED; i++)
  {
    if(!str_cmp(arg1, hunted_one[i].name))
    {
      iprey = TRUE;
      break;
    }
  }

  if(!iprey || ch->pcdata->tradelevel < hunted_one[i].tlevel)
  {
    send_to_char("This is not something you can hunt.\r\n", ch);
    return;
  }

  if(ch->pcdata->learned[gsn_hunt] <= 50 && chance <= 3)
  {
    send_to_char("You begin to stalk the area for your prey.\r\n", ch);
    send_to_char("A small game trail has tracks and droppings of the prey you are tracking.\r\n", ch);
    pager_printf(ch, "\r\n&GYour stalking pays off as you successfully found a %s!\r\n", arg1);
    act(AT_GREEN, "$n stalks up upon an animal.\r\n", ch, NULL, NULL, TO_ROOM);
    pager_printf(ch, "\r\n&GYour lack of hunting skills shows as the %s smells you and runs away!\r\n", arg1);

    act(AT_GREEN, "$n's looks away in disgust as $e realizes the animal ran off.\r\n", ch, NULL, NULL, TO_ROOM);
    learn_from_failure(ch, gsn_hunt);
    return;
  }

  short                   amount = 0, extra = 0;

  amount = 1;
  extra = number_range(1, 4);
  if(ch->pcdata->tradelevel > 9 && extra == 2)
  {
    amount += 1;
  }
  if(ch->pcdata->tradelevel > 14 && extra == 2)
  {
    amount += 1;
  }

  send_to_char("You begin to stalk the area for your prey.\r\n", ch);
  send_to_char("A small game trail has tracks and droppings of the prey you are tracking.\r\n", ch);
  ch_printf(ch, "\r\n&GYour stalking pays off as you successfully found a %s!\r\n", arg1);
  act(AT_GREEN, "$n stalks up upon an animal.\r\n", ch, NULL, NULL, TO_ROOM);

  pager_printf(ch, "\r\n&RAn adrenaline rush fills you from spotting a %s as you fumble for %s!\r\n", arg1, arrow->short_descr);
  pager_printf(ch, "\r\n&cYou ready %s and take careful aim.\r\n", arrow->short_descr);
  pager_printf(ch, "\r\n&GYou let loose %s striking %s in the heart!\r\n", arrow->short_descr, arg1);
  if(xIS_SET(ch->act, PLR_CRAFTS))
    send_to_char("!!SOUND(sound/mobhit.wav)\r\n", ch);

  act(AT_GREEN, "$n suddenly shoots at something.\r\n", ch, NULL, NULL, TO_ROOM);
  pager_printf(ch, "\r\n&cFollowing the trail of blood to your prey, you begin to gut the animal\r\nso the meat will not rot.  You cut out the meat, and separate the animal's hide.\r\n");
  if(amount > 1)
  {
    pager_printf(ch, "\r\nYou notice your precision shot allows you get extra material from the carcus as you carefully extract %s from the carcus.\r\n", arrow->short_descr);
  }
  else
    pager_printf(ch, "\r\nYou carefully extract %s from the carcus.\r\n", arrow->short_descr);

  if(!str_cmp(arg1, "rabbit"))
  {
    bag->craft1 += amount;
  }
  else if(!str_cmp(arg1, "goose"))
  {
    bag->craft2 += amount;
  }
  else if(!str_cmp(arg1, "badger"))
  {
    bag->craft3 += amount;
  }
  else if(!str_cmp(arg1, "turkey"))
  {
    bag->craft4 += amount;
  }
  else if(!str_cmp(arg1, "coyote"))
  {
    bag->craft5 += amount;
  }
  else if(!str_cmp(arg1, "leopard"))
  {
    bag->craft6 += amount;
  }
  else if(!str_cmp(arg1, "lion"))
  {
    bag->craft7 += amount;
  }
  else if(!str_cmp(arg1, "pheasant"))
  {
    bag->craft8 += amount;
  }
  else if(!str_cmp(arg1, "duck"))
  {
    bag->craft9 += amount;
  }
  else if(!str_cmp(arg1, "raccoon"))
  {
    bag->craft10 += amount;
  }
  else if(!str_cmp(arg1, "boar"))
  {
    bag->craft11 += amount;
  }
  else if(!str_cmp(arg1, "bobcat"))
  {
    bag->craft12 += amount;
  }
  else if(!str_cmp(arg1, "bear"))
  {
    bag->craft13 += amount;
  }
  else if(!str_cmp(arg1, "dove"))
  {
    bag->craft14 += amount;
  }
  else if(!str_cmp(arg1, "squirrel"))
  {
    bag->craft15 += amount;
  }
  else if(!str_cmp(arg1, "deer"))
  {
    bag->craft16 += amount;
  }
  else if(!str_cmp(arg1, "antelope"))
  {
    bag->craft17 += amount;
  }
  else if(!str_cmp(arg1, "wolf"))
  {
    bag->craft18 += amount;
  }
  else if(!str_cmp(arg1, "moose"))
  {
    bag->craft19 += amount;
  }
  else if(!str_cmp(arg1, "bugbear"))
  {
    bag->craft20 += amount;
  }

  learn_from_craft(ch, gsn_hunt);
}

void do_gather(CHAR_DATA *ch, char *argument)
{
  char                    name_buf[MSL];
  char                    short_buf[MSL];
  char                    long_buf[MSL];
  OBJ_DATA               *obj;
  char                    arg1[MIL];
  OBJ_DATA               *item;
  int                     i = 0;
  bool                    itm = FALSE, found = FALSE;
  short                   chance;

  chance = number_range(1, 10);

  argument = one_argument(argument, arg1);

  if(IS_NPC(ch))
    return;

  if(ch->position != POS_STANDING)
  {
    send_to_char("You should be standing to attempt this.\r\n", ch);
  }

  switch (ch->in_room->sector_type)
  {
    case SECT_OCEAN:
    case SECT_ARCTIC:
    case SECT_BEACH:
    case SECT_FOG:
    case SECT_SKY:
    case SECT_CLOUD:
    case SECT_SNOW:
    case SECT_ORE:
    case SECT_QUICKSAND:
    case SECT_DEEPMUD:
    case SECT_PORTALSTONE:
    case SECT_LAKE:
    case SECT_CAMPSITE:
    case SECT_DOCK:
    case SECT_RIVER:
    case SECT_WATERFALL:
    case SECT_CROSSROAD:
    case SECT_VROAD:
    case SECT_HROAD:
    case SECT_ROAD:
    case SECT_CITY:
    case SECT_SWAMP:
    case SECT_LAVA:
    case SECT_OCEANFLOOR:
    case SECT_AREA_ENT:
    case SECT_AIR:
    case SECT_UNDERWATER:
    case SECT_INSIDE:
      send_to_char("You cannot gather here.\r\n", ch);
      return;

    default:
      break;
  }

  if(IS_SET(ch->in_room->room_flags, ROOM_INDOORS))
  {
    send_to_char("You can't do that in here.\r\n", ch);
    return;
  }

  if(IS_SET(ch->in_room->area->flags, AFLAG_INDOORS))
  {
    send_to_char("You can't do that in here.\r\n", ch);
    return;
  }

  OBJ_DATA               *bag;

  for(bag = ch->first_carrying; bag; bag = bag->next_content)
  {
    if(bag->item_type == ITEM_GATHER_BAG)
      break;
  }

  if(!bag)
  {
    send_to_char("You do not have a baker's bag to hold your ingredients with.\r\n", ch);
    return;
  }

  if(bag->craft1 >= 1000 && !str_cmp(arg1, "strawberries"))
  {
    send_to_char("You cannot hold any more strawberries in your baker's bag.\r\n", ch);
    return;
  }
  else if(bag->craft2 >= 1000 && !str_cmp(arg1, "watermelons"))
  {
    send_to_char("You cannot hold any more watermelons in your baker's bag.\r\n", ch);
    return;
  }
  else if(bag->craft3 >= 1000 && !str_cmp(arg1, "lettuce"))
  {
    send_to_char("You cannot hold any more lettuce in your baker's bag.\r\n", ch);
    return;
  }
  else if(bag->craft4 >= 1000 && !str_cmp(arg1, "pickles"))
  {
    send_to_char("You cannot hold any more pickles in your baker's bag.\r\n", ch);
    return;
  }
  else if(bag->craft5 >= 1000 && !str_cmp(arg1, "wheat"))
  {
    send_to_char("You cannot hold any more wheat in your baker's bag.\r\n", ch);
    return;
  }
  else if(bag->craft6 >= 1000 && !str_cmp(arg1, "oranges"))
  {
    send_to_char("You cannot hold any more oranges in your baker's bag.\r\n", ch);
    return;
  }
  else if(bag->craft7 >= 1000 && !str_cmp(arg1, "peaches"))
  {
    send_to_char("You cannot hold any more peaches in your baker's bag.\r\n", ch);
    return;
  }
  else if(bag->craft8 >= 1000 && !str_cmp(arg1, "carrots"))
  {
    send_to_char("You cannot hold any more carrots in your baker's bag.\r\n", ch);
    return;
  }
  else if(bag->craft9 >= 1000 && !str_cmp(arg1, "cinnamon"))
  {
    send_to_char("You cannot hold any more cinnamon in your baker's bag.\r\n", ch);
    return;
  }
  else if(bag->craft10 >= 1000 && !str_cmp(arg1, "fig"))
  {
    send_to_char("You cannot hold any more fig in your baker's bag.\r\n", ch);
    return;
  }
  else if(bag->craft11 >= 1000 && !str_cmp(arg1, "blueberries"))
  {
    send_to_char("You cannot hold any more blueberries in your baker's bag.\r\n", ch);
    return;
  }
  else if(bag->craft12 >= 1000 && !str_cmp(arg1, "pears"))
  {
    send_to_char("You cannot hold any more pears in your baker's bag.\r\n", ch);
    return;
  }
  else if(bag->craft13 >= 1000 && !str_cmp(arg1, "onions"))
  {
    send_to_char("You cannot hold any more onions in your baker's bag.\r\n", ch);
    return;
  }
  else if(bag->craft14 >= 1000 && !str_cmp(arg1, "sugar-cane"))
  {
    send_to_char("You cannot hold any more sugar-cane in your baker's bag.\r\n", ch);
    return;
  }
  else if(bag->craft15 >= 1000 && !str_cmp(arg1, "ginger"))
  {
    send_to_char("You cannot hold any more ginger in your baker's bag.\r\n", ch);
    return;
  }
  else if(bag->craft16 >= 1000 && !str_cmp(arg1, "raspberries"))
  {
    send_to_char("You cannot hold any more raspberries in your baker's bag.\r\n", ch);
    return;
  }
  else if(bag->craft17 >= 1000 && !str_cmp(arg1, "corn"))
  {
    send_to_char("You cannot hold any more corn in your baker's bag.\r\n", ch);
    return;
  }
  else if(bag->craft18 >= 1000 && !str_cmp(arg1, "radishes"))
  {
    send_to_char("You cannot hold any more radishes in your baker's bag.\r\n", ch);
    return;
  }
  else if(bag->craft19 >= 1000 && !str_cmp(arg1, "garlic"))
  {
    send_to_char("You cannot hold any more garlic in your baker's bag.\r\n", ch);
    return;
  }
  else if(bag->craft20 >= 1000 && !str_cmp(arg1, "pepper"))
  {
    send_to_char("You cannot hold any more pepper in your baker's bag.\r\n", ch);
    return;
  }

  if(arg1[0] == '\0')
  {
    if(IS_BLIND(ch))
    {
      send_to_char("Type Gather ingredient\r\n", ch);
      send_to_char("Type Gather list for a list of ingredients you can gather.\r\n", ch);
    }
    else
    {
      send_to_char("&cSyntax: Gather <&Cingredient&c>\r\n", ch);
      send_to_char("Syntax: Gather list\r\n", ch);
    }
    return;
  }

  if(!str_cmp(arg1, "list"))
  {
    send_to_char("\t\t\t&CThe Listing of Gatherable Items\r\n\r\n", ch);
    for(int y = 0; y < MAX_FRUIT; y++)
    {
      ch_printf(ch, "&c%-15s      ", fruit_types[y].name ? fruit_types[y].name : "null name");
      if((y + 1) % 4 == 0)
        send_to_char("\r\n", ch);
    }
    send_to_char("\r\n&cSyntax: Gather <&Cingredient&c>\r\n", ch);
    send_to_char("Syntax: Gather list\r\n", ch);
    return;
  }

  if(ch->move < 1)
  {
    send_to_char("You don't have enough move points to perform that.\r\n", ch);
    return;
  }
  else
  {
    ch->move -= 1;
  }

  WAIT_STATE(ch, skill_table[gsn_gather]->beats);

  if(!can_use_skill(ch, number_percent(), gsn_gather))
  {
    learn_from_failure(ch, gsn_gather);
    send_to_char("You look around for things to gather, but find nothing.\r\n", ch);
    return;
  }

  if(ch->pcdata->learned[gsn_gather] <= 50 && chance <= 3)
  {
    send_to_char("You begin to search the area for ingredients to gather.\r\n", ch);
    pager_printf(ch, "\r\n&GYour gathering pays off as you gather your ingredient!\r\n");
    act(AT_GREEN, "$n reaches down and gathers something.\r\n", ch, NULL, NULL, TO_ROOM);
    pager_printf(ch, "\r\n&GYour lack of gathering skills shows as %s is rotten!\r\n", arg1);
    act(AT_GREEN, "$n's looks away in disgust as $e realizes the ingredient is rotten.\r\n", ch, NULL, NULL, TO_ROOM);
    learn_from_failure(ch, gsn_gather);
    return;
  }

  for(i = 0; i < MAX_FRUIT; i++)
  {
    if(!str_cmp(arg1, fruit_types[i].name))
    {
      itm = TRUE;
      break;
    }
  }

  // May need to comment out this check to get it to work.
  if(itm == FALSE)
  {
    send_to_char("This is not something you can gather.\r\n", ch);
    return;
  }
  short                   amount = 0, extra = 0;

  amount = 1;
  extra = number_range(1, 4);
  if(ch->pcdata->tradelevel > 9 && extra == 2)
  {
    amount += 1;
  }
  if(ch->pcdata->tradelevel > 14 && extra == 2)
  {
    amount += 1;
  }

  if(!str_cmp(arg1, "strawberries"))
  {
    bag->craft1 += amount;
  }
  else if(!str_cmp(arg1, "watermelons"))
  {
    bag->craft2 += amount;
  }
  else if(!str_cmp(arg1, "lettuce"))
  {
    bag->craft3 += amount;
  }
  else if(!str_cmp(arg1, "pickles"))
  {
    bag->craft4 += amount;
  }
  else if(!str_cmp(arg1, "wheat"))
  {
    bag->craft5 += amount;
  }
  else if(!str_cmp(arg1, "oranges"))
  {
    bag->craft6 += amount;
  }
  else if(!str_cmp(arg1, "peaches"))
  {
    bag->craft7 += amount;
  }
  else if(!str_cmp(arg1, "carrots"))
  {
    bag->craft8 += amount;
  }
  else if(!str_cmp(arg1, "cinnamon"))
  {
    bag->craft9 += amount;
  }
  else if(!str_cmp(arg1, "fig"))
  {
    bag->craft10 += amount;
  }
  else if(!str_cmp(arg1, "blueberries"))
  {
    bag->craft11 += amount;
  }
  else if(!str_cmp(arg1, "pears"))
  {
    bag->craft12 += amount;
  }
  else if(!str_cmp(arg1, "onions"))
  {
    bag->craft13 += amount;
  }
  else if(!str_cmp(arg1, "sugar-cane"))
  {
    bag->craft14 += amount;
  }
  else if(!str_cmp(arg1, "ginger"))
  {
    bag->craft15 += amount;
  }
  else if(!str_cmp(arg1, "raspberries"))
  {
    bag->craft16 += amount;
  }
  else if(!str_cmp(arg1, "corn"))
  {
    bag->craft17 += amount;
  }
  else if(!str_cmp(arg1, "radishes"))
  {
    bag->craft18 += amount;
  }
  else if(!str_cmp(arg1, "garlic"))
  {
    bag->craft19 += amount;
  }
  else if(!str_cmp(arg1, "pepper"))
  {
    bag->craft20 += amount;
  }

  if(xIS_SET(ch->act, PLR_CRAFTS))
    send_to_char("!!SOUND(sound/scissor.wav)\r\n", ch);

  send_to_char("You begin to search the area for ingredients to gather.\r\n", ch);
  ch_printf(ch, "\r\n&GYour gathering pays off as you gather %d %s!\r\n", amount, arg1);
  act(AT_GREEN, "$n reaches down and gathers something.\r\n", ch, NULL, NULL, TO_ROOM);
  learn_from_craft(ch, gsn_gather);
  WAIT_STATE(ch, skill_table[gsn_gather]->beats);
}

/* This is going to be challenging to come up with minor affects for so many ingriedients */

/* - maybe add resistances, immunity, suspectible? This is for mixing and baking */

short food_affect(const char *name)
{
  short                   item, affect = 0;

  for(item = 0; item < MAX_FOOD; item++)
  {
    if(!str_cmp(name, "strawberries"))
    {
      affect = 5;
    }
    else if(!str_cmp(name, "watermelons"))
    {
      affect = 6;
    }
    else if(!str_cmp(name, "lettuce"))
    {
      affect = 7;
    }
    else if(!str_cmp(name, "pickles"))
    {
      affect = 8;
    }
    else if(!str_cmp(name, "wheat"))
    {
      affect = 9;
    }
    else if(!str_cmp(name, "oranges"))
    {
      affect = 10;
    }
    else if(!str_cmp(name, "peaches"))
    {
      affect = 11;
    }
    else if(!str_cmp(name, "carrots"))
    {
      affect = 12;
    }
    else if(!str_cmp(name, "cinnamon"))
    {
      affect = 13;
    }
    else if(!str_cmp(name, "fig"))
    {
      affect = 14;
    }
    else if(!str_cmp(name, "blueberries"))
    {
      affect = 15;
    }
    else if(!str_cmp(name, "pears"))
    {
      affect = 16;
    }
    else if(!str_cmp(name, "onions"))
    {
      affect = 17;
    }
    else if(!str_cmp(name, "sugar-cane"))
    {
      affect = 18;
    }
    else if(!str_cmp(name, "ginger"))
    {
      affect = 19;
    }
    else if(!str_cmp(name, "raspberries"))
    {
      affect = 20;
    }
    else if(!str_cmp(name, "corn"))
    {
      affect = 21;
    }
    else if(!str_cmp(name, "radishes"))
    {
      affect = 22;
    }
    else if(!str_cmp(name, "garlic"))
    {
      affect = 23;
    }
    else if(!str_cmp(name, "pepper"))
    {
      affect = 24;
    }
  }
  return affect;
}

void send_bake_syntax(CHAR_DATA *ch)
{
  if(!ch)
    return;
  if(IS_BLIND(ch))
  {
    send_to_char("\r\nType bake ingredient into food level\r\n", ch);
    send_to_char("Type bake list to see what you can bake or bake list all to see everything.\r\n", ch);
    send_to_char("Type bake fire to light the oven.\r\n", ch);
    if(ch->pcdata->tradelevel > 4)
    {
      send_to_char("Type bake ID number heal or bake ID number antidote to upgrade your finished product into a salve.\r\n", ch);
    }
    if(ch->pcdata->tradelevel > 14)
    {
      send_to_char("Type bake id number rename new name to change the objects name or bake id number rekey new key to change what you type to get the object.\r\n", ch);
    }
    send_to_char("Note: There are 4 steps, keep doing the bake command until the last step.\r\n", ch);
  }
  else
  {
    send_to_char("\r\n&cSyntax: bake <&Cingredient&c> into <&Cfood&c> <&Clevel&c>\r\n", ch);
    send_to_char("&cSyntax: bake list or bake list all\r\n", ch);
    send_to_char("&cSyntax: bake fire\r\n", ch);
    if(ch->pcdata->tradelevel > 4)
    {
      send_to_char("&cSyntax: bake ID# heal or bake ID# antidote to upgrade your finished product into a salve.\r\n", ch);
    }
    if(ch->pcdata->tradelevel > 14)
    {
      send_to_char("&cSyntax: bake id # rename 'new name' or bake id # rekey 'new key' to change what you type to get the object.\r\n", ch);
    }
    send_to_char("&cNote: There are &C4 steps&c, keep doing the bake command until the last step.\r\n", ch);
  }
}

void do_bake(CHAR_DATA *ch, char *argument)
{
  short                   number;
  OBJ_DATA               *bakedfood;
  char                    arg1[MIL];
  char                    arg2[MIL];
  char                    arg3[MIL];
  char                    arg4[MIL];
  char                    name_buf[MSL];
  char                    short_buf[MSL];
  char                    long_buf[MSL];
  char                    extra_buf[MSL];
  const char             *adj;
  bool                    itm = FALSE;
  int                     i = 0, x = 0;
  struct skill_type      *skill = NULL;

  name_buf[0] = '\0';
  short_buf[0] = '\0';
  long_buf[0] = '\0';
  extra_buf[0] = '\0';

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  argument = one_argument(argument, arg3);
  argument = one_argument(argument, arg4);

  if(IS_NPC(ch) || ch->desc == NULL)
    return;

  if(arg1[0] == '\0')
  {
    send_bake_syntax(ch);
    return;
  }

  if(ch->position != POS_STANDING)
  {
    send_to_char("You should be standing to attempt this.\r\n", ch);
  }

  if(!str_cmp(arg1, "list"))
  {
    int                     y;
    int                     count = 0;

    send_to_char("\t\t\t&CThe Baked Food Listing\r\n\r\n", ch);

    if(!str_cmp(arg2, "all"))
    {
      for(y = 0; y < MAX_FOOD; y++)
      {
        count++;
        ch_printf(ch, "&c%-15s %-3d ", food_one[y].name ? food_one[y].name : "null name", food_one[y].level);

        if(count == 3)
        {
          count = 0;
          send_to_char("\r\n", ch);
        }
      }
      if(count != 0)
        send_to_char("\r\n", ch);
      send_bake_syntax(ch);
      return;
    }

    for(y = 0; y < MAX_FOOD; y++)
    {
      if(ch->pcdata->tradelevel < food_one[y].level)
        continue;
      count++;
      ch_printf(ch, "&c%-15s ", food_one[y].name ? food_one[y].name : "null name");
      if(count == 3)
      {
        count = 0;
        send_to_char("\r\n", ch);
      }
    }

    if(count != 0)
      send_to_char("\r\n", ch);
    send_bake_syntax(ch);
    return;
  }

  if(ch->move < 1)
  {
    send_to_char("You don't have enough move points to perform that.\r\n", ch);
    return;
  }
  else
  {
    ch->move -= 1;
  }

  if(!str_cmp(arg2, "rename") && ch->pcdata->tradelevel > 14 && !xIS_SET(ch->act, PLR_NORENAME))
  {
    OBJ_DATA               *item;

    for(item = ch->first_carrying; item; item = item->next_content)
    {
      if(atoi(arg1) == item->guid && item->pIndexData->vnum == 41004)
        break;
    }
    if(!item)
    {
      send_to_char("You do not have any baked items to rename or rekey.\r\n", ch);
      return;
    }

    if(!arg3 || arg3[0] == '\0')
    {
      send_to_char("You must specify the new keywords, ie bake id number rename 'new name' or rename 'ring toering'.\r\n", ch);
      return;
    }
    ch_printf(ch, "&cChanging item's description from &C%s&c to&C %s&c..\r\n", item->short_descr, arg3);
    STRFREE(item->short_descr);
    item->short_descr = STRALLOC(arg3);
    sprintf(long_buf, "%s has been left here along the ground.", item->short_descr);
    STRFREE(item->description);
    item->description = STRALLOC(long_buf);
    send_to_char("&RDone!&c Please note, the KEYWORDS used to drop/equip/etc remain the same.\r\n", ch);
    save_char_obj(ch);
    return;
  }
  else if(!str_cmp(arg2, "rename") && (ch->pcdata->tradelevel < 15 || xIS_SET(ch->act, PLR_NORENAME)))
  {
    send_to_char("You must specify the new keywords, ie rename id number 'new name' or rename 'ring toering'.\r\n", ch);
    return;
  }

  if(!str_cmp(arg2, "rekey") && ch->pcdata->tradelevel > 14 && !xIS_SET(ch->act, PLR_NORENAME))
  {
    OBJ_DATA               *item;

    for(item = ch->first_carrying; item; item = item->next_content)
    {
      if(atoi(arg1) == item->guid && item->pIndexData->vnum == 41004)
        break;
    }
    if(!item)
    {
      send_to_char("You do not have any baked items to rename or rekey.\r\n", ch);
      return;
    }

    ch_printf(ch, "&cChanging item's keywords from &C%s&c to &C%s&c..\r\n", item->name, arg3);
    STRFREE(item->name);
    item->name = STRALLOC(arg3);
    send_to_char("&RDone!&c Please note, the DESCRIPTION when looking at the item remains the same.\r\n", ch);
    save_char_obj(ch);
    return;
  }
  else if(!str_cmp(arg2, "rekey") && (ch->pcdata->tradelevel < 15 || xIS_SET(ch->act, PLR_NORENAME)))
  {
    send_to_char("You must specify the new keywords, ie 'bake rekey whatever new name' or 'bake rekey sword toothpick'.\r\n", ch);
    return;
  }

  if(!str_cmp(arg1, "fire"))
  {
    OBJ_DATA               *stove;
    bool                    found;

    found = FALSE;

    for(stove = ch->in_room->first_content; stove; stove = stove->next_content)
    {
      if(stove->item_type == ITEM_STOVE)
      {
        found = TRUE;
        break;
      }
    }

    if(!found)
    {
      send_to_char("There must be an oven to fire it.\r\n", ch);
      return;
    }

    if(stove->value[0] == 1)
    {
      send_to_char("There is no need to fire the oven, it is already.\r\n", ch);
      return;
    }

    OBJ_DATA               *coal;

    for(coal = ch->first_carrying; coal; coal = coal->next_content)
    {
      if(coal->item_type == ITEM_COAL)
        break;
    }

    if(!coal)
    {
      send_to_char("You do not have any coal to fire the oven.\r\n", ch);
      return;
    }

    separate_obj(coal);
    obj_from_char(coal);
    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/forgefire.wav)\r\n", ch);

    act(AT_CYAN, "$n fires up the oven lighting the coal within it.", ch, NULL, NULL, TO_ROOM);
    act(AT_CYAN, "You fire the oven lighting the coal within it.", ch, NULL, NULL, TO_CHAR);
    act(AT_YELLOW, "\r\nA flame flickers within the oven.", ch, NULL, NULL, TO_ROOM);
    act(AT_YELLOW, "\r\nA flame flickers within the oven.", ch, NULL, NULL, TO_CHAR);
    extract_obj(coal);
    stove->value[0] = 1;
    return;
  }

  OBJ_DATA               *bag, *obj;

  for(bag = ch->first_carrying; bag; bag = bag->next_content)
  {
    if(bag->item_type == ITEM_GATHER_BAG)
      break;
  }

  if(!bag)
  {
    send_to_char("You do not have a baker's bag to bake with.\r\n", ch);
    return;
  }

  if((bag->craft1 < 1 && !str_cmp(arg1, "strawberries")) ||
     (bag->craft2 < 1 && !str_cmp(arg1, "watermelons")) ||
     (bag->craft3 < 1 && !str_cmp(arg1, "lettuce")) ||
     (bag->craft4 < 1 && !str_cmp(arg1, "pickles")) ||
     (bag->craft5 < 1 && !str_cmp(arg1, "wheat")) ||
     (bag->craft6 < 1 && !str_cmp(arg1, "oranges")) ||
     (bag->craft7 < 1 && !str_cmp(arg1, "peaches")) ||
     (bag->craft8 < 1 && !str_cmp(arg1, "carrots")) ||
     (bag->craft9 < 1 && !str_cmp(arg1, "cinnamon")) ||
     (bag->craft10 < 1 && !str_cmp(arg1, "fig")) ||
     (bag->craft11 < 1 && !str_cmp(arg1, "blueberries")) ||
     (bag->craft12 < 1 && !str_cmp(arg1, "pears")) ||
     (bag->craft13 < 1 && !str_cmp(arg1, "onions")) ||
     (bag->craft14 < 1 && !str_cmp(arg1, "sugar-cane")) ||
     (bag->craft15 < 1 && !str_cmp(arg1, "ginger")) ||
     (bag->craft16 < 1 && !str_cmp(arg1, "raspberries")) ||
     (bag->craft17 < 1 && !str_cmp(arg1, "corn")) ||
     (bag->craft18 < 1 && !str_cmp(arg1, "radishes")) || (bag->craft19 < 1 && !str_cmp(arg1, "garlic")) || (bag->craft20 < 1 && !str_cmp(arg1, "pepper")))
  {
    send_to_char("You would need to gather more in your baker's bag first.\r\n", ch);
    return;
  }

  if(!IS_SET(ch->in_room->room_flags, ROOM_TRADESKILLS))
  {
    send_to_char("You must be in a tradeskills building to do this.\r\n", ch);
    return;
  }

  if(ch->pcdata->tradelevel > 4 && !str_cmp(arg2, "heal"))
  {
    OBJ_DATA               *finished;

    for(finished = ch->first_carrying; finished; finished = finished->next_content)
    {
      if(atoi(arg1) == finished->guid && finished->item_type == ITEM_FOOD && finished->pIndexData->vnum == 41004)
        break;
    }
    if(!finished)
    {
      send_to_char("You do not have any baked food to make a salve of.\r\n", ch);
      return;
    }
    finished->item_type = ITEM_SALVE;
    finished->value[0] = ch->pcdata->tradelevel * 5;  // spell level 
    finished->value[1] = ch->pcdata->tradelevel / 2;  // charges 
    finished->value[2] = ch->pcdata->tradelevel / 2;  // max charges 
    finished->value[3] = 12;  // affect delay 
    if(ch->pcdata->tradelevel > 14 && ch->pcdata->tradelevel < 20)
      finished->value[4] = 94;  // heal
    if(ch->pcdata->tradelevel < 15)
      finished->value[4] = 37;  // cure critical 
    if(ch->pcdata->tradelevel == 20)
    {
      finished->value[4] = 90;  // greater heal
      finished->value[1] = 3;
      finished->value[2] = 3;
    }
    finished->value[6] = -1;
// medicene 

// salve naming 

    sprintf(name_buf, "a healing salve");
    sprintf(short_buf, "a hand crafted healing salve");
    sprintf(long_buf, "Here lies a healing salve made from %s.", finished->short_descr);

    if(finished->name)
      STRFREE(finished->name);
    if(finished->short_descr)
      STRFREE(finished->short_descr);
    if(finished->description)
      STRFREE(finished->description);
    finished->name = STRALLOC(name_buf);
    finished->short_descr = STRALLOC(short_buf);
    finished->description = STRALLOC(long_buf);
    act(AT_CYAN, "$n grinds the final baked product into a healing herbed salve.", ch, NULL, NULL, TO_ROOM);
    act(AT_CYAN, "You grind the final baked product into a healing herbed salve.", ch, NULL, NULL, TO_CHAR);
    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/grinding.wav)\r\n", ch);
    return;
  }

  if(ch->pcdata->tradelevel > 4 && !str_cmp(arg2, "antidote"))
  {

    for(bakedfood = ch->first_carrying; bakedfood; bakedfood = bakedfood->next_content)
    {
      if(atoi(arg1) == bakedfood->guid && bakedfood->item_type == ITEM_FOOD && bakedfood->pIndexData->vnum == 41004)
        break;
    }

    if(!bakedfood)
    {
      send_to_char("You do not have any baked food to make a salve of.\r\n", ch);
      return;
    }

    bakedfood->item_type = ITEM_SALVE;
    bakedfood->value[0] = ch->pcdata->tradelevel * 5;
    bakedfood->value[1] = ch->pcdata->tradelevel / 2;
    bakedfood->value[2] = ch->pcdata->tradelevel / 2;
    bakedfood->value[3] = 12; // affect delay 
    bakedfood->value[4] = 36; // affect cure affliction 
    bakedfood->value[6] = -1;
// salve naming 

    sprintf(name_buf, "a antidotal salve");
    sprintf(short_buf, "a hand crafted antidotal salve");
    sprintf(long_buf, "Here lies a antidotal salve made from %s.", bakedfood->short_descr);

    if(bakedfood->name)
      STRFREE(bakedfood->name);
    if(bakedfood->short_descr)
      STRFREE(bakedfood->short_descr);
    if(bakedfood->description)
      STRFREE(bakedfood->description);
    bakedfood->name = STRALLOC(name_buf);
    bakedfood->short_descr = STRALLOC(short_buf);
    bakedfood->description = STRALLOC(long_buf);
    act(AT_CYAN, "$n grinds the final baked product into a antidote herbed salve.", ch, NULL, NULL, TO_ROOM);
    act(AT_CYAN, "You grind the final baked product into a antidote herbed salve.", ch, NULL, NULL, TO_CHAR);
    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/grinding.wav)\r\n", ch);
    return;
  }

  if(!str_cmp(arg1, "strawberries"))
  {
    bag->craft1 -= 1;
  }
  else if(!str_cmp(arg1, "watermelons"))
  {
    bag->craft2 -= 1;
  }
  else if(!str_cmp(arg1, "lettuce"))
  {
    bag->craft3 -= 1;
  }
  else if(!str_cmp(arg1, "pickles"))
  {
    bag->craft4 -= 1;
  }
  else if(!str_cmp(arg1, "wheat"))
  {
    bag->craft5 -= 1;
  }
  else if(!str_cmp(arg1, "oranges"))
  {
    bag->craft6 -= 1;
  }
  else if(!str_cmp(arg1, "peaches"))
  {
    bag->craft7 -= 1;
  }
  else if(!str_cmp(arg1, "carrots"))
  {
    bag->craft8 -= 1;
  }
  else if(!str_cmp(arg1, "cinnamon"))
  {
    bag->craft9 -= 1;
  }
  else if(!str_cmp(arg1, "fig"))
  {
    bag->craft10 -= 1;
  }
  else if(!str_cmp(arg1, "blueberries"))
  {
    bag->craft11 -= 1;
  }
  else if(!str_cmp(arg1, "pears"))
  {
    bag->craft12 -= 1;
  }
  else if(!str_cmp(arg1, "onions"))
  {
    bag->craft13 -= 1;
  }
  else if(!str_cmp(arg1, "sugar-cane"))
  {
    bag->craft14 -= 1;
  }
  else if(!str_cmp(arg1, "ginger"))
  {
    bag->craft15 -= 1;
  }
  else if(!str_cmp(arg1, "raspberries"))
  {
    bag->craft16 -= 1;
  }
  else if(!str_cmp(arg1, "corn"))
  {
    bag->craft17 -= 1;
  }
  else if(!str_cmp(arg1, "radishes"))
  {
    bag->craft18 -= 1;
  }
  else if(!str_cmp(arg1, "garlic"))
  {
    bag->craft19 -= 1;
  }
  else if(!str_cmp(arg1, "pepper"))
  {
    bag->craft20 -= 1;
  }
  else
  {
    send_to_char("That's not a proper ingredient.\r\n", ch);
    return;
  }

  if(arg2[0] == '\0' || arg3[0] == '\0')
  {
    send_bake_syntax(ch);
    return;

  }
  if(str_cmp(arg2, "into"))
  {
    send_bake_syntax(ch);
    return;
  }
  int                     perk = 0;

  {
    OBJ_DATA               *stove;
    bool                    found;

    found = FALSE;

    for(stove = ch->in_room->first_content; stove; stove = stove->next_content)
    {
      if(stove->item_type == ITEM_STOVE)
      {
        found = TRUE;
        break;
      }
    }

    if(!found)
    {
      send_to_char("There must be a oven in the room in order to do that.\r\n", ch);
      return;
    }

    if(stove->value[0] != 1)
    {
      send_to_char("&cYou have to fire the oven first.\r\n", ch);
      send_to_char("&cSyntax: bake fire\r\n", ch);
      return;
    }

// This is the nitty gritty of checking tier output 
    for(i = 0; i < MAX_FOOD; i++)
    {
      if(ch->pcdata->tradelevel < food_one[i].level)
        continue;
      if(!str_cmp(arg3, food_one[i].name))
      {
        itm = TRUE;
        break;
      }
    }

    if(itm == FALSE)
    {
      send_to_char("This is not a valid food type.\r\n", ch);
      return;
    }

    SKILLTYPE              *skill = get_skilltype(gsn_bake);

    if(ch->pcdata->tradelevel <= 1)
      adj = "poorly";
    else if(ch->pcdata->tradelevel <= 5)
      adj = "simply";
    else if(ch->pcdata->tradelevel <= 8)
      adj = "properly";
    else if(ch->pcdata->tradelevel <= 10)
      adj = "well";
    else if(ch->pcdata->tradelevel <= 15)
      adj = "finely";
    else if(ch->pcdata->tradelevel <= 19)
      adj = "masterfully";
    else
      adj = "legendary";

    WAIT_STATE(ch, 15);

    short                   failure = 0;

    if(ch->pcdata->tradelevel < 5)
      failure = 14;
    else if(ch->pcdata->tradelevel < 10 && ch->pcdata->tradelevel > 4)
      failure = 10;
    else if(ch->pcdata->tradelevel < 15 && ch->pcdata->tradelevel > 9)
      failure = 8;
    else if(ch->pcdata->tradelevel < 20 && ch->pcdata->tradelevel > 14)
      failure = 5;
    else if(ch->pcdata->tradelevel == 20)
      failure = 2;

    if(bag->value[6] == 0)
    {
      if(ch->pcdata->learned[gsn_bake] <= number_range(1, 5) || number_percent() <= failure)
      {
        act(AT_CYAN, "$n prepares the ingredient by placing it in the oven, but the ingredient is left too long and burns.", ch, NULL, NULL, TO_ROOM);
        act(AT_CYAN, "You prepare the ingredient by placing it in the oven, but the ingredient is left too long and burns.", ch, NULL, NULL, TO_CHAR);
        learn_from_failure(ch, gsn_bake);
        return;
      }

      act(AT_CYAN, "$n prepares the ingredient by placing it in the oven.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You prepare the ingredient by placing it in the oven.", ch, NULL, NULL, TO_CHAR);
      obj = create_object(get_obj_index(OBJ_VNUM_ORE), 0);
      if(obj->name)
        STRFREE(obj->name);
      if(obj->short_descr)
        STRFREE(obj->short_descr);
      if(obj->description)
        STRFREE(obj->description);
      obj->name = STRALLOC("dish");
      obj->description = STRALLOC("A cooked dish has been left here.");
      obj->short_descr = STRALLOC("a cooked dish");
      obj_to_char(obj, ch);
      bag->value[6] = 1;
      return;
    }

    if(bag->value[6] == 1)
    {
      if(ch->pcdata->learned[gsn_bake] <= number_range(1, 10) || number_percent() <= failure)
      {

        for(obj = ch->first_carrying; obj; obj = obj->next_content)
        {
          if(obj->item_type == ITEM_RAW)
            break;
        }

        if(!obj)
        {
          send_to_char("You do not have your dish.\r\n", ch);
          bag->value[6] = 0;
          return;
        }

        act(AT_CYAN, "$n places the cooked dish on the table and begins to add garnishments but gets distracted and ruins the dish by adding too much.", ch, NULL, NULL, TO_ROOM);
        act(AT_CYAN, "You place the cooked dish on the table and begin to add garnishments, but get distracted and ruin the dish by adding too much.", ch, NULL, NULL, TO_CHAR);
        separate_obj(obj);
        obj_from_char(obj);
        extract_obj(obj);
        bag->value[6] = 0;
        learn_from_failure(ch, gsn_bake);
        return;
      }

      act(AT_CYAN, "$n places the cooked dish on the table and begins to add garnishments.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You place the cooked dish on the table and begin to add garnishments.", ch, NULL, NULL, TO_CHAR);
      if(number_range(1, 4) > 2)
        learn_from_craft(ch, gsn_bake); // add a chance of gaining exp

      bag->value[6] += 1;
      for(obj = ch->first_carrying; obj; obj = obj->next_content)
      {
        if(obj->item_type == ITEM_RAW)
          break;
      }

      if(!obj)
      {
        send_to_char("You do not have your dish.\r\n", ch);
        bag->value[6] = 0;
        return;
      }

      obj->short_descr = STRALLOC("a prepped dish");
      return;
    }

    if(bag->value[6] == 2)
    {
      if(!can_use_skill(ch, number_percent(), gsn_bake) || number_percent() <= failure)
      {
        act(AT_CYAN, "$n tastes the dish to see if it is right, then keeps adding to it.", ch, NULL, NULL, TO_ROOM);
        act(AT_CYAN, "You taste the dish to see if it is right, then continue adding to it.", ch, NULL, NULL, TO_CHAR);
        if(xIS_SET(ch->act, PLR_CRAFTS))
          send_to_char("!!SOUND(sound/eat1.wav)\r\n", ch);

        learn_from_failure(ch, gsn_bake);
        return;
      }

      if(number_percent() <= failure)
      {
        act(AT_CYAN, "$n begins to stir the prepped dish, but gets distracted and ruins the dish by over stirring it.", ch, NULL, NULL, TO_ROOM);
        act(AT_CYAN, "You begin to stir the prepped dish, but get distracted and ruin the dish by over stirring it.", ch, NULL, NULL, TO_CHAR);
        for(obj = ch->first_carrying; obj; obj = obj->next_content)
        {
          if(obj->item_type == ITEM_RAW)
            break;
        }

        if(!obj)
        {
          send_to_char("You do not have your dish.\r\n", ch);
          bag->value[6] = 0;
          return;
        }

        separate_obj(obj);
        obj_from_char(obj);
        extract_obj(obj);
        bag->value[6] = 0;
        learn_from_failure(ch, gsn_bake);
        return;
      }
      act(AT_CYAN, "$n begins to stir the prepped dish.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You begin to stir the prepped dish.", ch, NULL, NULL, TO_CHAR);
      bag->value[6] += 1;
      for(obj = ch->first_carrying; obj; obj = obj->next_content)
      {
        if(obj->item_type == ITEM_RAW)
          break;
      }

      if(!obj)
      {
        send_to_char("You do not have your dish.\r\n", ch);
        bag->value[6] = 0;
        return;
      }

      obj->short_descr = STRALLOC("a prepared dish");
      return;
    }
    if(number_percent() <= failure)
    {
      act(AT_CYAN, "$n takes out some spices, but gets distracted and ruins the dish by adding too much.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You take out some spices, but get distracted and ruin the dish by adding too much.", ch, NULL, NULL, TO_CHAR);
      for(obj = ch->first_carrying; obj; obj = obj->next_content)
      {
        if(obj->item_type == ITEM_RAW)
          break;
      }

      if(!obj)
      {
        send_to_char("You do not have your dish.\r\n", ch);
        bag->value[6] = 0;
        return;
      }

      separate_obj(obj);
      obj_from_char(obj);
      extract_obj(obj);
      bag->value[6] = 0;
      learn_from_failure(ch, gsn_bake);
      return;
    }

    act(AT_CYAN, "$n takes out some spices, and adds the finishing touches.", ch, NULL, NULL, TO_ROOM);
    act(AT_CYAN, "You take out some spices, and add the finishing touches.", ch, NULL, NULL, TO_CHAR);
    bag->value[6] = 0;
    learn_from_craft(ch, gsn_bake);
    bakedfood = create_object(get_obj_index(OBJ_VNUM_CRAFTFOOD), 1);
    if(arg4[0] == '\0')
      bakedfood->level = ch->level;
    else
      bakedfood->level = atoi(arg4);

    found = FALSE;

    for(i = 0; i < MAX_FOOD; i++)
    {
      if(ch->pcdata->tradelevel < food_one[i].level)
        continue;
      if(!str_cmp(arg3, food_one[i].name))
      {
        found = TRUE;
        break;
      }
    }

    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/assemble.wav)\r\n", ch);

    if(!found)
    {
      send_to_char("Can't create it for some odd reason, inform a STAFF member.\r\n", ch);
      return;
    }

    sprintf(name_buf, "%s %s", food_one[i].name, STRALLOC(arg1));
    sprintf(short_buf, "a %s, %s cooked from %s", food_one[i].name, adj, STRALLOC(arg1));
    sprintf(long_buf, "Here lies a %s, %s cooked from %s.", food_one[i].name, adj, STRALLOC(arg1));
    bakedfood->item_type = food_one[i].item_type;
    bakedfood->wear_flags += food_one[i].wear_flags;
    bakedfood->weight = (food_one[i].weight);
    bakedfood->cost = food_one[i].weight;
    bakedfood->value[6] = 1;
    bakedfood->value[0] = 5;
    bakedfood->pIndexData->value[6] = 1;

    perk = food_affect(arg1);
    bakedfood->extra = perk;
    // From the number assigned the ingriedient a small effect will be in the food.

    if(ch->pcdata->tradelevel >= 5)
      bakedfood->value[6] = 2;
    if(ch->pcdata->tradelevel >= 10)
      bakedfood->value[6] = 3;

    if(ch->pcdata->tradelevel >= 20)
    {
      GET_VALUE(bakedfood, type) = CURR_GOLD;
      bakedfood->cost = 30;
    }
    else if(ch->pcdata->tradelevel >= 15)
    {
      GET_VALUE(bakedfood, type) = CURR_GOLD;
      bakedfood->cost = 25;
    }
    else if(ch->pcdata->tradelevel >= 10)
    {
      GET_VALUE(bakedfood, type) = CURR_GOLD;
      bakedfood->cost = 15;
    }
    else if(ch->pcdata->tradelevel >= 5)
    {
      GET_VALUE(bakedfood, type) = CURR_SILVER;
      bakedfood->cost = 50;
    }
    else
    {
      GET_VALUE(bakedfood, type) = CURR_COPPER;
      bakedfood->cost = 25;
    }

    sprintf(extra_buf,
            "\r\n&CThis crafted dish bears the seal of %s, the %s baker.\r\n",
            ch->name,
            ch->pcdata->tradelevel <= 5 ? "apprentice" : ch->pcdata->tradelevel <= 10 ? "journeyman" : ch->pcdata->tradelevel <= 19 ? "expert" : ch->pcdata->tradelevel == 20 ? "master" : "reknowned");

    if(bakedfood->name)
      STRFREE(bakedfood->name);
    if(bakedfood->short_descr)
      STRFREE(bakedfood->short_descr);
    if(bakedfood->description)
      STRFREE(bakedfood->description);

    bakedfood->name = STRALLOC(name_buf);
    bakedfood->short_descr = STRALLOC(short_buf);
    bakedfood->description = STRALLOC(long_buf);

    EXTRA_DESCR_DATA       *ed;

    CREATE(ed, EXTRA_DESCR_DATA, 1);

    LINK(ed, bakedfood->first_extradesc, bakedfood->last_extradesc, next, prev);
    ed->keyword = STRALLOC(bakedfood->name);
    ed->description = STRALLOC(extra_buf);

    if(ch->pcdata->tradelevel <= 1)
      bakedfood->value[1] = 1;
    else if(ch->pcdata->tradelevel <= 5)
      bakedfood->value[1] = 3;
    else if(ch->pcdata->tradelevel <= 8)
      bakedfood->value[1] = 5;
    else if(ch->pcdata->tradelevel <= 10)
      bakedfood->value[1] = 8;
    else if(ch->pcdata->tradelevel <= 15)
      bakedfood->value[1] = 10;
    else if(ch->pcdata->tradelevel <= 19)
      bakedfood->value[1] = 12;
    else
      bakedfood->value[1] = 15;
    for(obj = ch->first_carrying; obj; obj = obj->next_content)
    {
      if(obj->item_type == ITEM_RAW)
        break;
    }

    if(!obj)
    {
      send_to_char("You do not have your dish.\r\n", ch);
      bag->value[6] = 0;
      return;
    }

    bakedfood->color = obj->color;
    separate_obj(obj);
    obj_from_char(obj);
    extract_obj(obj);

    if(ch->carry_number + get_obj_number(bakedfood) > can_carry_n(ch))
    {
      send_to_char("You can't carry that many items, and drop the food on the floor.\r\n", ch);
      separate_obj(bakedfood);
// obj_from_char( bakedfood ); 
      extract_obj(bakedfood);
      return;
    }

    if(ch->carry_weight + (get_obj_weight(bakedfood, FALSE) * 1) + (1 > 1 ? 2 : 0) > can_carry_w(ch))
    {
      send_to_char("You can't carry that much weight, and drop the food on the floor.\r\n", ch);
      separate_obj(bakedfood);
// obj_from_char( bakedfood ); 
      extract_obj(bakedfood);
      return;
    }

    obj_to_char(bakedfood, ch);
    short                   extinguish;

    extinguish = number_chance(1, 8);
    if(extinguish == 8 && !IS_AFFECTED(ch, AFF_BOOST))
    {
      send_to_char("\r\n&wThe oven burns the last of the coal and the flame is extinguished.\r\n", ch);
      stove->value[0] = 0;
    }
    CLAN_DATA              *clan;

    if(IS_CLANNED(ch))
    {
      clan = ch->pcdata->clan;
      ch->pcdata->clanpoints += 1;
      clan->totalpoints += 1;
      ch_printf(ch, "\r\n&G%s clan has gained a status point from your craftsmanship, now totaling %d clan status points!\r\n", clan->name, clan->totalpoints);
      save_char_obj(ch);
      save_clan(clan);
    }
    return;
    tail_chain();
  }
}

void send_mix_syntax(CHAR_DATA *ch)
{
  if(!ch)
    return;

  send_to_char("\r\n", ch);
  if(IS_BLIND(ch))
  {
    send_to_char("Type mix ingredient into drink level\r\n", ch);
    send_to_char("Type mix list to see what you can mix or mix list all to see everything.\r\n", ch);
    if(ch->pcdata->tradelevel > 9)
    {
      send_to_char
        ("Type mix ID number affect, replacing the word affect with one of the following: str, int, wis, con, cha, lck, dex.\r\nThis will change your mixture into a herbed mixture with special affects.r\n",
         ch);
    }
    if(ch->pcdata->tradelevel > 14)
    {
      send_to_char("Type mix id number rename new name to change the objects name or mix id number rekey new key to change what you type to get the object.\r\n", ch);
    }
    send_to_char("Note: There are &C4 steps&c, keep doing the mix command until the last step.\r\n", ch);
  }
  else
  {
    send_to_char("&cSyntax: mix <&Cingredient&c> into <&Cdrink&c> <&Clevel&c>\r\n", ch);
    send_to_char("Syntax: mix list or mix list all\r\n", ch);
    if(ch->pcdata->tradelevel > 9)
    {
      send_to_char("&cSyntax: mix ID# <affect>\r\nAffect fields being: str, int, wis, con, cha, lck, dex\r\n", ch);
    }
    if(ch->pcdata->tradelevel > 14)
    {
      send_to_char("&cSyntax: mix id # rename 'new name' or mix id # rekey 'new key' to change what you type to get the object.\r\n", ch);
    }
    send_to_char("Note: There are &C4 steps&c, keep doing the mix command until the last step.\r\n", ch);
  }
}

void do_mix(CHAR_DATA *ch, char *argument)
{
  short                   number;
  OBJ_DATA               *mixeddrink;
  char                    arg1[MIL];
  char                    arg2[MIL];
  char                    arg3[MIL];
  char                    arg4[MIL];
  char                    name_buf[MSL];
  char                    short_buf[MSL];
  char                    long_buf[MSL];
  char                    extra_buf[MSL];
  const char             *adj;
  bool                    itm = FALSE;
  int                     i = 0, x = 0;

  name_buf[0] = '\0';
  short_buf[0] = '\0';
  long_buf[0] = '\0';
  extra_buf[0] = '\0';
  bool                    found;

  found = FALSE;

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  argument = one_argument(argument, arg3);
  argument = one_argument(argument, arg4);

  if(IS_NPC(ch) || ch->desc == NULL)
    return;

  if(arg1[0] == '\0')
  {
    send_mix_syntax(ch);
    return;
  }

  if(ch->move < 1)
  {
    send_to_char("You	don't have enough move points to perform that.\r\n", ch);
    return;
  }
  else
  {
    ch->move -= 1;
  }

  if(ch->position != POS_STANDING)
  {
    send_to_char("You should be standing to attempt this.\r\n", ch);
  }

  if(!str_cmp(arg2, "rename") && ch->pcdata->tradelevel > 14 && !xIS_SET(ch->act, PLR_NORENAME))
  {
    OBJ_DATA               *item;

    for(item = ch->first_carrying; item; item = item->next_content)
    {
      if(atoi(arg1) == item->guid && item->pIndexData->vnum == 41004)
        break;
    }
    if(!item)
    {
      send_to_char("You do not have any mixed items to rename or rekey.\r\n", ch);
      return;
    }

    if(!arg3 || arg3[0] == '\0')
    {
      send_to_char("You must specify the new keywords, ie mix id number rename 'new name' or rename 'ring toering'.\r\n", ch);
      return;
    }
    ch_printf(ch, "&cChanging item's description from &C%s&c to&C %s&c..\r\n", item->short_descr, arg3);
    STRFREE(item->short_descr);
    item->short_descr = STRALLOC(arg3);
    sprintf(long_buf, "%s has been left here along the ground.", item->short_descr);
    STRFREE(item->description);
    item->description = STRALLOC(long_buf);
    send_to_char("&RDone!&c Please note, the KEYWORDS used to drop/equip/etc remain the same.\r\n", ch);
    save_char_obj(ch);
    return;
  }
  else if(!str_cmp(arg2, "rename") && (ch->pcdata->tradelevel < 15 || xIS_SET(ch->act, PLR_NORENAME)))
  {
    send_to_char("You must specify the new keywords, ie rename id number 'new name' or rename 'ring toering'.\r\n", ch);
    return;
  }

  if(!str_cmp(arg2, "rekey") && ch->pcdata->tradelevel > 14 && !xIS_SET(ch->act, PLR_NORENAME))
  {
    OBJ_DATA               *item;

    for(item = ch->first_carrying; item; item = item->next_content)
    {
      if(atoi(arg1) == item->guid && item->pIndexData->vnum == 41004)
        break;
    }
    if(!item)
    {
      send_to_char("You do not have any mixed items to rename or rekey.\r\n", ch);
      return;
    }

    ch_printf(ch, "&cChanging item's keywords from &C%s&c to &C%s&c..\r\n", item->name, arg3);
    STRFREE(item->name);
    item->name = STRALLOC(arg3);
    send_to_char("&RDone!&c Please note, the DESCRIPTION when looking at the item remains the same.\r\n", ch);
    save_char_obj(ch);
    return;
  }
  else if(!str_cmp(arg2, "rekey") && (ch->pcdata->tradelevel < 15 || xIS_SET(ch->act, PLR_NORENAME)))
  {
    send_to_char("You must specify the new keywords, ie 'mix rekey whatever new name' or 'mix rekey sword toothpick'.\r\n", ch);
    return;
  }

  if(!str_cmp(arg1, "list"))
  {
    int                     count = 0, y;

    send_to_char("\t\t\t&CThe Mixed Drink Listing\r\n\r\n", ch);

    if(!str_cmp(arg2, "all"))
    {
      for(y = 0; y < MAX_DRINK; y++)
      {
        count++;
        ch_printf(ch, "&c%-15s   %-3d ", drink_one[y].name ? drink_one[y].name : "null name", drink_one[y].level);

        if(count == 3)
        {
          count = 0;
          send_to_char("\r\n", ch);
        }
      }
      if(count != 0)
        send_to_char("\r\n", ch);
      send_mix_syntax(ch);
      return;
    }

    for(y = 0; y < MAX_DRINK; y++)
    {
      if(ch->pcdata->tradelevel < drink_one[y].level)
        continue;
      count++;
      ch_printf(ch, "&c%-15s      ", drink_one[y].name ? drink_one[y].name : "null name");
      if(count == 3)
      {
        count = 0;
        send_to_char("\r\n", ch);
      }
    }
    if(count != 0)
      send_to_char("\r\n", ch);
    send_mix_syntax(ch);
    return;
  }

  OBJ_DATA               *bag, *obj;

  for(bag = ch->first_carrying; bag; bag = bag->next_content)
  {
    if(bag->item_type == ITEM_GATHER_BAG)
      break;
  }

  if(!bag)
  {
    send_to_char("You do not have a baker's bag to bake with.\r\n", ch);
    return;
  }

  if(!IS_SET(ch->in_room->room_flags, ROOM_TRADESKILLS))
  {
    send_to_char("You must be in a tradeskills building to do this.\r\n", ch);
    return;
  }
// Put herbed affect here

  // GENERIC STUFF THAT AFFECTS ALL IFCHECKS FOR herbs
  if(ch->pcdata->tradelevel > 9
     && (!str_cmp(arg2, "str") || !str_cmp(arg2, "int") || !str_cmp(arg2, "wis") || !str_cmp(arg2, "con") || !str_cmp(arg2, "cha") || !str_cmp(arg2, "lck") || !str_cmp(arg2, "dex")))
  {
    for(mixeddrink = ch->first_carrying; mixeddrink; mixeddrink = mixeddrink->next_content)
    {
      if(atoi(arg1) == mixeddrink->guid && mixeddrink->item_type == ITEM_DRINK_CON && mixeddrink->pIndexData->vnum == 41004)
        break;
    }

    if(!mixeddrink)
    {
      send_to_char("You do not have any mixed drink to make a herb of.\r\n", ch);
      return;
    }

    act(AT_CYAN, "$n grinds up herbs into the final mixed drink product.", ch, NULL, NULL, TO_ROOM);
    act(AT_CYAN, "You grind up herbs into final mixed drink product.", ch, NULL, NULL, TO_CHAR);
    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/grind2.wav)\r\n", ch);

    sprintf(name_buf, "a herbed mixture");
    sprintf(short_buf, "a hand crafted herbed mixture");
    sprintf(long_buf, "Here lies a herbed mixture made from %s.", mixeddrink->short_descr);

    if(mixeddrink->name)
      STRFREE(mixeddrink->name);
    if(mixeddrink->short_descr)
      STRFREE(mixeddrink->short_descr);
    if(mixeddrink->description)
      STRFREE(mixeddrink->description);
    mixeddrink->name = STRALLOC(name_buf);
    mixeddrink->short_descr = STRALLOC(short_buf);
    mixeddrink->description = STRALLOC(long_buf);

    mixeddrink->item_type = ITEM_POTION;
    mixeddrink->value[0] = ch->pcdata->tradelevel * 5;  // spell level
    mixeddrink->value[2] = -1;  // sn
    mixeddrink->value[3] = -1;  // sn
  }

  if(ch->pcdata->tradelevel > 9 && !str_cmp(arg2, "str"))
  {
    mixeddrink->value[1] = 109; // sn
    return;
  }
  else if(ch->pcdata->tradelevel > 9 && !str_cmp(arg2, "int"))
  {
    mixeddrink->value[1] = 54;  // sn
    return;
  }
  else if(ch->pcdata->tradelevel > 9 && !str_cmp(arg2, "wis"))
  {
    mixeddrink->value[1] = 150; // sn
    return;
  }
  else if(ch->pcdata->tradelevel > 9 && !str_cmp(arg2, "con"))
  {
    mixeddrink->value[1] = 182; // sn
    return;
  }
  else if(ch->pcdata->tradelevel > 9 && !str_cmp(arg2, "cha"))
  {
    mixeddrink->value[1] = 60;  // sn
    return;
  }
  else if(ch->pcdata->tradelevel > 9 && !str_cmp(arg2, "dex"))
  {
    mixeddrink->value[1] = 161; // sn
    return;
  }
  else if(ch->pcdata->tradelevel > 9 && !str_cmp(arg2, "lck"))
  {
    mixeddrink->value[1] = 86;  // sn
    return;
  }

  if((bag->craft1 < 1 && !str_cmp(arg1, "strawberries")) ||
     (bag->craft2 < 1 && !str_cmp(arg1, "watermelons")) ||
     (bag->craft3 < 1 && !str_cmp(arg1, "lettuce")) ||
     (bag->craft4 < 1 && !str_cmp(arg1, "pickles")) ||
     (bag->craft5 < 1 && !str_cmp(arg1, "wheat")) ||
     (bag->craft6 < 1 && !str_cmp(arg1, "oranges")) ||
     (bag->craft7 < 1 && !str_cmp(arg1, "peaches")) ||
     (bag->craft8 < 1 && !str_cmp(arg1, "carrots")) ||
     (bag->craft9 < 1 && !str_cmp(arg1, "cinnamon")) ||
     (bag->craft10 < 1 && !str_cmp(arg1, "fig")) ||
     (bag->craft11 < 1 && !str_cmp(arg1, "blueberries")) ||
     (bag->craft12 < 1 && !str_cmp(arg1, "pears")) ||
     (bag->craft13 < 1 && !str_cmp(arg1, "onions")) ||
     (bag->craft14 < 1 && !str_cmp(arg1, "sugar-cane")) ||
     (bag->craft15 < 1 && !str_cmp(arg1, "ginger")) ||
     (bag->craft16 < 1 && !str_cmp(arg1, "raspberries")) ||
     (bag->craft17 < 1 && !str_cmp(arg1, "corn")) ||
     (bag->craft18 < 1 && !str_cmp(arg1, "radishes")) || (bag->craft19 < 1 && !str_cmp(arg1, "garlic")) || (bag->craft20 < 1 && !str_cmp(arg1, "pepper")))
  {
    send_to_char("You would need to gather more in your baker's bag first.\r\n", ch);
    return;
  }

  if(!str_cmp(arg1, "strawberries"))
  {
    bag->craft1 -= 1;
  }
  else if(!str_cmp(arg1, "watermelons"))
  {
    bag->craft2 -= 1;
  }
  else if(!str_cmp(arg1, "corn"))
  {
    bag->craft17 -= 1;
  }
  else if(!str_cmp(arg1, "carrots"))
  {
    bag->craft8 -= 1;
  }
  else if(!str_cmp(arg1, "cinnamon"))
  {
    bag->craft9 -= 1;
  }
  else if(!str_cmp(arg1, "fig"))
  {
    bag->craft10 -= 1;
  }
  else if(!str_cmp(arg1, "oranges"))
  {
    bag->craft6 -= 1;
  }
  else if(!str_cmp(arg1, "peaches"))
  {
    bag->craft7 -= 1;
  }
  else if(!str_cmp(arg1, "lettuce"))
  {
    bag->craft3 -= 1;
  }
  else if(!str_cmp(arg1, "onions"))
  {
    bag->craft13 -= 1;
  }
  else if(!str_cmp(arg1, "sugar-cane"))
  {
    bag->craft14 -= 1;
  }
  else if(!str_cmp(arg1, "ginger"))
  {
    bag->craft15 -= 1;
  }
  else if(!str_cmp(arg1, "blueberries"))
  {
    bag->craft11 -= 1;
  }
  else if(!str_cmp(arg1, "pears"))
  {
    bag->craft12 -= 1;
  }
  else if(!str_cmp(arg1, "raspberries"))
  {
    bag->craft16 -= 1;
  }
  else if(!str_cmp(arg1, "radishes"))
  {
    bag->craft18 -= 1;
  }
  else if(!str_cmp(arg1, "garlic"))
  {
    bag->craft19 -= 1;
  }
  else if(!str_cmp(arg1, "pickles"))
  {
    bag->craft4 -= 1;
  }
  else if(!str_cmp(arg1, "wheat"))
  {
    bag->craft5 -= 1;
  }
  else if(!str_cmp(arg1, "pepper"))
  {
    bag->craft20 -= 1;
  }
  else
  {
    send_to_char("That's not a proper ingredient.\r\n", ch);
    return;
  }

  if(arg2[0] == '\0' || arg3[0] == '\0' || str_cmp(arg2, "into"))
  {
    send_mix_syntax(ch);
    return;
  }

  i = drink_lookup(arg3);
  if(i < 0 || ch->pcdata->tradelevel < drink_one[i].level)
  {
    send_to_char("That isn't a valid drink to mix.\r\n", ch);
    return;
  }

  if(ch->pcdata->tradelevel <= 1)
    adj = "poorly";
  else if(ch->pcdata->tradelevel <= 5)
    adj = "simply";
  else if(ch->pcdata->tradelevel <= 8)
    adj = "properly";
  else if(ch->pcdata->tradelevel <= 10)
    adj = "well";
  else if(ch->pcdata->tradelevel <= 15)
    adj = "finely";
  else if(ch->pcdata->tradelevel <= 19)
    adj = "masterfully";
  else
    adj = "legendary";

  WAIT_STATE(ch, 15);

  short                   failure = 0;

  if(ch->pcdata->tradelevel < 5)
    failure = 14;
  else if(ch->pcdata->tradelevel < 10 && ch->pcdata->tradelevel > 4)
    failure = 10;
  else if(ch->pcdata->tradelevel < 15 && ch->pcdata->tradelevel > 9)
    failure = 8;
  else if(ch->pcdata->tradelevel < 20 && ch->pcdata->tradelevel > 14)
    failure = 5;
  else if(ch->pcdata->tradelevel == 20)
    failure = 2;

  if(bag->value[6] == 0)
  {
    if(ch->pcdata->learned[gsn_mix] <= 5 || number_percent() <= failure)
    {
      act(AT_CYAN,
          "$n prepares the ingredient by placing it in the mixing bowl, and grinding it up.\r\nWhile grinding the ingredient $e notices that maggots are in it, and it is now ruined.",
          ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN,
          "You prepare the ingredient by placing it in the mixing bowl, and grinding it up.\r\nWhile grinding the ingredient you notice that maggots are in it, and it is now ruined.",
          ch, NULL, NULL, TO_CHAR);

      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/grind2.wav)\r\n", ch);

      learn_from_failure(ch, gsn_mix);
      return;
    }
    bag->value[6] = 1;
    act(AT_CYAN, "$n prepares the ingredient by placing it in the mixing bowl, and grinding it up.", ch, NULL, NULL, TO_ROOM);
    act(AT_CYAN, "You prepare the ingredient by placing it in the mixing bowl, and grinding it up.", ch, NULL, NULL, TO_CHAR);
    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/grind2.wav)\r\n", ch);

    obj = create_object(get_obj_index(OBJ_VNUM_ORE), 0);
    if(obj->name)
      STRFREE(obj->name);
    if(obj->short_descr)
      STRFREE(obj->short_descr);
    if(obj->description)
      STRFREE(obj->description);
    obj->name = STRALLOC("mix");
    obj->description = STRALLOC("A powdery mix has been left here.");
    obj->short_descr = STRALLOC("a powdery mix");
    obj_to_char(obj, ch);
    return;
  }

  if(bag->value[6] == 1)
  {
    if(ch->pcdata->learned[gsn_mix] <= 10 || number_percent() <= failure)
    {
      act(AT_CYAN, "$n places the mixing bowl on the table and begins adding honey to sweeten it.\r\n$n realizes the drink is ruined because $e added too much.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You place the mixing bowl on the table and begin adding honey to sweeten it.\r\nYou realize the drink is ruined because you added too much.", ch, NULL, NULL, TO_CHAR);
      for(obj = ch->first_carrying; obj; obj = obj->next_content)
      {
        if(obj->item_type == ITEM_RAW)
          break;
      }

      if(!obj)
      {
        send_to_char("You do not have your sweetened mix.\r\n", ch);
        bag->value[6] = 0;
        return;
      }

      separate_obj(obj);
      obj_from_char(obj);
      extract_obj(obj);
      bag->value[6] = 0;
      learn_from_failure(ch, gsn_mix);
      return;
    }

    act(AT_CYAN, "$n places the mixing bowl on the table and begins adding honey to sweeten it.", ch, NULL, NULL, TO_ROOM);
    act(AT_CYAN, "You place the mixing bowl on the table and begin adding honey to sweeten it.", ch, NULL, NULL, TO_CHAR);
    bag->value[6] += 1;
    for(obj = ch->first_carrying; obj; obj = obj->next_content)
    {
      if(obj->item_type == ITEM_RAW)
        break;
    }

    if(!obj)
    {
      send_to_char("You do not have your sweetened mix.\r\n", ch);
      bag->value[6] = 0;
      return;
    }

    obj->short_descr = STRALLOC("a sweetened mix");
    return;
  }

  if(bag->value[6] == 2)
  {
    if(ch->pcdata->learned[gsn_mix] <= 15 || number_percent() <= failure)
    {
      act(AT_CYAN, "$n begins to add fluids to the sweetened mix.\r\n$n realizes the drink is ruined because $e has added too much.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You begin to add fluids to the sweetened mix.\r\nYou realize the drink is ruined because you have added too much.", ch, NULL, NULL, TO_CHAR);
      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/liquid.wav)\r\n", ch);

      for(obj = ch->first_carrying; obj; obj = obj->next_content)
      {
        if(obj->item_type == ITEM_RAW)
          break;
      }

      if(!obj)
      {
        send_to_char("You do not have your powdery mix.\r\n", ch);
        bag->value[6] = 0;
        return;
      }

      separate_obj(obj);
      obj_from_char(obj);
      extract_obj(obj);
      bag->value[6] = 0;
      learn_from_failure(ch, gsn_mix);
      return;
    }

    act(AT_CYAN, "$n begins to add fluids to the sweetened mix.", ch, NULL, NULL, TO_ROOM);
    act(AT_CYAN, "You begin to add fluids to the sweetened mix.", ch, NULL, NULL, TO_CHAR);
    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/liquid.wav)\r\n", ch);

    bag->value[6] += 1;
    for(obj = ch->first_carrying; obj; obj = obj->next_content)
    {
      if(obj->item_type == ITEM_RAW)
        break;
    }

    if(!obj)
    {
      send_to_char("You do not have your watery mix.\r\n", ch);
      bag->value[6] = 0;
      return;
    }

    obj->short_descr = STRALLOC("a watery mix");
    return;
  }

  if(bag->value[6] == 3)
  {
    if(ch->pcdata->learned[gsn_mix] <= 15 || number_percent() <= failure)
    {
      act(AT_CYAN, "$n takes out some garnishments, and realizes that while adding the finishing touches $e has ruined it.\r\n", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You take out some garnishments, and realize that while adding the finishing touches you have ruined it.", ch, NULL, NULL, TO_CHAR);
      for(obj = ch->first_carrying; obj; obj = obj->next_content)
      {
        if(obj->item_type == ITEM_RAW)
          break;
      }

      if(!obj)
      {
        send_to_char("You do not have your watery mix.\r\n", ch);
        bag->value[6] = 0;
        return;
      }

      separate_obj(obj);
      obj_from_char(obj);
      extract_obj(obj);
      bag->value[6] = 0;
      learn_from_failure(ch, gsn_mix);
      return;
    }
    act(AT_CYAN, "$n takes out some garnishments, and adds the finishing touch.", ch, NULL, NULL, TO_ROOM);
    act(AT_CYAN, "You take out some garnishments, and add the finishing touch.", ch, NULL, NULL, TO_CHAR);
    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/assemble.wav)\r\n", ch);

    bag->value[6] += 1;
  }
  bag->value[6] = 0;
  learn_from_craft(ch, gsn_mix);
  mixeddrink = create_object(get_obj_index(OBJ_VNUM_CRAFTFOOD), 1);
  if(arg4[0] == '\0')
    mixeddrink->level = ch->level;
  else
    mixeddrink->level = atoi(arg4);

  sprintf(name_buf, "%s %s", STRALLOC(arg1), drink_one[i].name);
  sprintf(short_buf, "a %s, %s mixed from %s", drink_one[i].name, adj, STRALLOC(arg1));
  sprintf(long_buf, "Here lies a %s, %s mixed from %s.", drink_one[i].name, adj, STRALLOC(arg1));

  mixeddrink->item_type = drink_one[i].item_type;
  mixeddrink->wear_flags += drink_one[i].wear_flags;
  mixeddrink->weight = (drink_one[i].weight);
  mixeddrink->cost = drink_one[i].weight;
  mixeddrink->value[0] = 1;
  mixeddrink->value[1] = 1;
  mixeddrink->value[2] = 16;

  if(ch->pcdata->tradelevel < 6)
  {
    mixeddrink->value[6] = 1;
  }
  if(ch->pcdata->tradelevel < 11 && ch->pcdata->tradelevel > 5)
  {
    mixeddrink->value[6] = 2;
  }
  if(ch->pcdata->tradelevel > 10 && ch->pcdata->tradelevel < 20)
  {
    mixeddrink->value[6] = 3;
  }
  if(ch->pcdata->tradelevel > 19)
  {
    mixeddrink->value[6] = 3;
  }
  if(ch->pcdata->tradelevel < 5)
  {
    GET_VALUE(mixeddrink, type) = CURR_COPPER;
    mixeddrink->cost = 25;
  }
  else if(ch->pcdata->tradelevel >= 5 && ch->pcdata->tradelevel < 10)
  {
    GET_VALUE(mixeddrink, type) = CURR_SILVER;
    mixeddrink->cost = 50;
  }
  else if(ch->pcdata->tradelevel >= 10 && ch->pcdata->tradelevel < 15)
  {
    GET_VALUE(mixeddrink, type) = CURR_GOLD;
    mixeddrink->cost = 15;
  }
  else if(ch->pcdata->tradelevel >= 15)
  {
    GET_VALUE(mixeddrink, type) = CURR_GOLD;
    mixeddrink->cost = 25;
  }

  sprintf(extra_buf, "\r\n&CThis crafted drink bears the seal of %s, the %s baker.\r\n",
          ch->name,
          ch->pcdata->tradelevel <= 5 ? "apprentice" : ch->pcdata->tradelevel <= 10 ? "journeyman" : ch->pcdata->tradelevel <= 19 ? "expert" : ch->pcdata->tradelevel == 20 ? "master" : "reknowned");

  if(mixeddrink->name)
    STRFREE(mixeddrink->name);
  if(mixeddrink->short_descr)
    STRFREE(mixeddrink->short_descr);
  if(mixeddrink->description)
    STRFREE(mixeddrink->description);

  mixeddrink->name = STRALLOC(name_buf);
  mixeddrink->short_descr = STRALLOC(short_buf);
  mixeddrink->description = STRALLOC(long_buf);

  int                     perk = 0;

  perk = food_affect(arg1);
  mixeddrink->extra = perk;
  // From the number assigned the ingriedient a small effect will be in the food.

  EXTRA_DESCR_DATA       *ed;

  CREATE(ed, EXTRA_DESCR_DATA, 1);

  LINK(ed, mixeddrink->first_extradesc, mixeddrink->last_extradesc, next, prev);
  ed->keyword = STRALLOC(mixeddrink->name);
  ed->description = STRALLOC(extra_buf);
  for(obj = ch->first_carrying; obj; obj = obj->next_content)
  {
    if(obj->item_type == ITEM_RAW)
      break;
  }

  if(!obj)
  {
    send_to_char("You do not have your mixed drink.\r\n", ch);
    bag->value[6] = 0;
    return;
  }

  separate_obj(obj);
  obj_from_char(obj);
  extract_obj(obj);

  if(ch->carry_number + get_obj_number(mixeddrink) > can_carry_n(ch))
  {
    send_to_char("You can't carry that many items, and drop the food on the floor.\r\n", ch);
    separate_obj(mixeddrink);
    // obj_from_char( mixeddrink );
    extract_obj(mixeddrink);
    return;
  }

  if(ch->carry_weight + (get_obj_weight(mixeddrink, FALSE) * 1) + (1 > 1 ? 2 : 0) > can_carry_w(ch))
  {
    send_to_char("You can't carry that much weight, and drop the food on the floor.\r\n", ch);
    separate_obj(mixeddrink);
    // obj_from_char( mixeddrink );
    extract_obj(mixeddrink);
    return;
  }

  CLAN_DATA              *clan;

  if(IS_CLANNED(ch))
  {
    clan = ch->pcdata->clan;
    ch->pcdata->clanpoints += 1;
    clan->totalpoints += 1;
    ch_printf(ch, "\r\n&G%s clan has gained a status point from your craftsmanship, now totaling %d clan status points!\r\n", clan->name, clan->totalpoints);
    save_clan(clan);
  }

  obj_to_char(mixeddrink, ch);
  save_char_obj(ch);
}

/*
void do_rig( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA               *scrapobj;
    OBJ_DATA               *obj;

    if ( IS_NPC( ch ) )
        return;

    for ( scrapobj = ch->first_carrying; scrapobj; scrapobj = scrapobj->next_content ) {
        if ( scrapobj->rig != 0 && scrapobj->item_type == ITEM_SCRAPS )
            break;
    }

    if ( !scrapobj ) {
        send_to_char( "You cannot rig that.\r\n", ch );
        return;
    }

    if ( scrapobj->rig == 0 ) {
        send_to_char( "That is not a scrapped item that needs to be rigged.\r\n", ch );
        return;
    }
    WAIT_STATE( ch, skill_table[gsn_rig]->beats );

    if ( can_use_skill( ch, number_percent(  ), gsn_rig ) ) {

        obj = create_object( get_obj_index( scrapobj->rig ), ch->level );

        if ( obj->item_type == ITEM_ARMOR || obj->item_type == ITEM_WEAPON ) {
            obj->value[0] /= 2;
        }

        act( AT_ACTION, "$n rigs a $p to work with things that are laying around.", ch, scrapobj,
             NULL, TO_ROOM );
        act( AT_ACTION,
             "You grab a few things that are laying around and rig a $p together enough to work again.",
             ch, scrapobj, NULL, TO_CHAR );
        learn_from_success( ch, gsn_rig );
        obj_to_char( obj, ch );
        extract_obj( scrapobj );
    }
    else {
        learn_from_failure( ch, gsn_rig );
        act( AT_ACTION,
             "$n tries to rig a $p to work with things that are laying around, but fails.", ch,
             scrapobj, NULL, TO_ROOM );
        act( AT_ACTION,
             "You grab a few things that are laying around and try to rig a $p together, but fail.",
             ch, scrapobj, NULL, TO_CHAR );
    }
}
*/

void send_fell_syntax(CHAR_DATA *ch)
{
  send_to_char("&CThe Listing of Lumber for Felling&c\r\n", ch);
  if(IS_BLIND(ch))
  {
    send_to_char("&cType fell and the name of the lumber type you want to fell.\r\n", ch);
  }
  else
  {
    send_to_char("&cSyntax: Fell <&CLumber type&c>\r\n", ch);
  }
  send_to_char("Lumber: fir", ch);

  if(ch->pcdata->tradelevel >= 3)
    send_to_char(" cedar", ch);
  if(ch->pcdata->tradelevel >= 5)
    send_to_char(" pine", ch);
  if(ch->pcdata->tradelevel >= 8)
    send_to_char(" walnut", ch);
  if(ch->pcdata->tradelevel >= 10)
    send_to_char(" oak", ch);
  if(ch->pcdata->tradelevel >= 15)
    send_to_char(" hickory", ch);
  send_to_char("\r\n", ch);
}

void do_fell(CHAR_DATA *ch, char *argument)
{
  OBJ_DATA               *obj, *obj2;
  char                    arg1[MIL], name[MSL], shortdesc[MSL], longdesc[MSL];
  int                     color, value, cost, knows, level;
  short                   chance;
  bool                    wield = TRUE, dwield = TRUE, weapon = TRUE, weapon2 = TRUE;

  argument = one_argument(argument, arg1);

  if(IS_NPC(ch))
    return;

  if(!VLD_STR(arg1) || !str_cmp(arg1, "list"))
  {
    send_fell_syntax(ch);
    return;
  }

  if(ch->in_room->sector_type != SECT_FOREST && ch->in_room->sector_type != SECT_THICKFOREST)
  {
    send_to_char("You cannot saw lumber here.\r\n", ch);
    return;
  }

  if((obj = get_eq_char(ch, WEAR_WIELD)) == NULL)
  {
    wield = FALSE;
  }

  if((obj2 = get_eq_char(ch, WEAR_DUAL_WIELD)) == NULL)
  {
    dwield = FALSE;
  }

  if(obj)
  {
    if(obj->value[4] != 8 && obj->value[4] != 9)
    {
      weapon = FALSE;
    }
  }

  if(obj2)
  {
    if(obj2->value[4] != 8 && obj2->value[4] != 9)
    {
      weapon2 = FALSE;
    }
  }

  if(wield == FALSE)
  {
    send_to_char("You must be holding a axe tool.\r\n", ch);
    return;
  }

  if(weapon == FALSE && dwield == FALSE)
  {
    send_to_char("You must be holding a axe tool.\r\n", ch);
    return;
  }
  if(weapon == FALSE && weapon2 == FALSE)
  {
    send_to_char("You must be holding a axe tool.\r\n", ch);
    return;
  }

  chance = number_chance(1, 300);

  if(chance == 3 && obj->pIndexData->vnum != 41002 && !IS_AFFECTED(ch, AFF_BOOST))
  {
    send_to_char("Your axe handle suddenly snapped in two!\r\n", ch);
      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/snap.wav)\r\n", ch);
    make_scraps(obj);
    return;
  }

  if(ch->pcdata->tradelevel < 3 && (!str_cmp(arg1, "cedar") || !str_cmp(arg1, "pine") || !str_cmp(arg1, "walnut") || !str_cmp(arg1, "oak") || !str_cmp(arg1, "hickory")))
  {
    send_to_char("You don't have the skill to fell that tree yet.", ch);
    return;
  }
  if(ch->pcdata->tradelevel < 5 && (!str_cmp(arg1, "pine") || !str_cmp(arg1, "walnut") || !str_cmp(arg1, "oak") || !str_cmp(arg1, "hickory")))
  {
    send_to_char("You don't have the skill to fell that tree yet.", ch);
    return;
  }
  if(ch->pcdata->tradelevel < 8 && (!str_cmp(arg1, "walnut") || !str_cmp(arg1, "oak") || !str_cmp(arg1, "hickory")))
  {
    send_to_char("You don't have the skill to fell that yet.", ch);
    return;
  }
  if(ch->pcdata->tradelevel < 10 && (!str_cmp(arg1, "oak") || !str_cmp(arg1, "hickory")))
  {
    send_to_char("You don't have the skill to fell that yet.", ch);
    return;
  }
  if(ch->pcdata->tradelevel < 15 && (!str_cmp(arg1, "hickory")))
  {
    send_to_char("You don't have the skill to fell that yet.", ch);
    return;
  }

  if(!str_cmp(arg1, "fir"))
  {
    knows = 15;
    level = 0;
  }
  else if(!str_cmp(arg1, "cedar"))
  {
    knows = 40;
    level = 3;
  }
  else if(!str_cmp(arg1, "pine"))
  {
    knows = 55;
    level = 5;
  }
  else if(!str_cmp(arg1, "walnut"))
  {
    knows = 65;
    level = 8;
  }
  else if(!str_cmp(arg1, "oak"))
  {
    knows = 85;
    level = 10;
  }
  else if(!str_cmp(arg1, "hickory"))
  {
    knows = 95;
    level = 15;
  }

  OBJ_DATA               *bag;

  for(bag = ch->first_carrying; bag; bag = bag->next_content)
  {
    if(bag->item_type == ITEM_RESOURCE_BAG && bag->value[0] == 500) // either
      // change
      // that to
      // accept 
// craft1-20 or change vnum of bag 
      break;
  }

  if(!bag)
  {
    send_to_char("You do not have a lumber sled to hold anything with.\r\n", ch);
    return;
  }

  if(bag->craft1 >= 1000 && !str_cmp(arg1, "fir"))
  {
    send_to_char("You cannot hold any more fir in your lumber sled.\r\n", ch);
    return;
  }
  else if(bag->craft2 >= 1000 && !str_cmp(arg1, "cedar"))
  {
    send_to_char("You cannot hold any more cedar in your lumber sled.\r\n", ch);
    return;
  }
  else if(bag->craft3 >= 1000 && !str_cmp(arg1, "pine"))
  {
    send_to_char("You cannot hold any more pine in your lumber sled.\r\n", ch);
    return;
  }
  else if(bag->craft4 >= 1000 && !str_cmp(arg1, "walnut"))
  {
    send_to_char("You cannot hold any more walnut in your lumber sled.\r\n", ch);
    return;
  }
  else if(bag->craft5 >= 1000 && !str_cmp(arg1, "oak"))
  {
    send_to_char("You cannot hold any more oak in your lumber sled.\r\n", ch);
    return;
  }
  else if(bag->craft6 >= 1000 && !str_cmp(arg1, "hickory"))
  {
    send_to_char("You cannot hold any more hickory in your lumber sled.\r\n", ch);
    return;
  }

  if(ch->move < 5)
  {
    send_to_char("You don't have enough energy to keep felling trees.\r\n", ch);
    return;
  }

  if(!str_cmp(arg1, "fir") || !str_cmp(arg1, "cedar") || !str_cmp(arg1, "pine") || !str_cmp(arg1, "walnut") || !str_cmp(arg1, "oak"));  /* Does 
                                                                                                                                         * nothing
                                                                                                                                         * and
                                                                                                                                         * keeps
                                                                                                                                         * going */
  else if(!str_cmp(arg1, "hickory"))
    chance = number_chance(1, 5); /* More chance of failing on this */
  else
  { /* If none of those then give a * message */

    send_fell_syntax(ch);
    return;
  }

  WAIT_STATE(ch, skill_table[gsn_fell]->beats);

  if(!can_use_skill(ch, number_percent(), gsn_fell))
  {
    learn_from_failure(ch, gsn_fell);
    send_to_char("You stop chopping as you notice the wood is rotten.\r\n", ch);
    return;
  }

  ch->move -= 1;

  chance = number_chance(1, 10);

  short                   amount = 0, extra = 0;

  amount = 1;
  extra = number_range(1, 4);
  if(ch->pcdata->tradelevel > 9 && extra == 2)
  {
    amount += 1;
  }
  if(ch->pcdata->tradelevel > 14 && extra == 2)
  {
    amount += 1;
  }

  if(chance < 3)
  {
    if(chance <= 1)
    {
      act(AT_ORANGE, "$n's chopping is not skillful enough to make progress.\r\n", ch, NULL, NULL, TO_ROOM);
      send_to_char("&OYour chopping is not skillful enough to make progress.\r\n", ch);
    }
    else if(chance >= 2)
    {
      act(AT_CYAN, "$n begins to labor in earnest chopping away at the tree with $s axe.\r\n", ch, NULL, NULL, TO_ROOM);
      send_to_char("&cYou begin to labor in earnest chopping away at the tree.\r\n", ch);
      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/chopping.wav)\r\n", ch);

    }
    return;
  }

  if(ch->pcdata->learned[gsn_fell] < knows || ch->pcdata->tradelevel < level)
  {
    send_to_char("You begin to chop into the tree.\r\n", ch);
    pager_printf(ch, "&GYour chopping pays off as you successfully fell a tree!\r\n");
    act(AT_GREEN, "$n's chopping pays off as $e successfully fells a tree.", ch, NULL, NULL, TO_ROOM);
    pager_printf(ch, "&GYour lack of tree felling knowledge shows as you have hacked it beyond use!\r\n");
    act(AT_GREEN, "$n's looks away in disgust as $e realizes the lumber is ruined.", ch, NULL, NULL, TO_ROOM);
    learn_from_failure(ch, gsn_fell);
    return;
  }

  if(!str_cmp(arg1, "fir"))
  {
    bag->craft1 += amount;
  }
  else if(!str_cmp(arg1, "cedar"))
  {
    bag->craft2 += amount;
  }
  else if(!str_cmp(arg1, "pine"))
  {
    bag->craft3 += amount;
  }
  else if(!str_cmp(arg1, "walnut"))
  {
    bag->craft4 += amount;
  }
  else if(!str_cmp(arg1, "oak"))
  {
    bag->craft5 += amount;
  }
  else if(!str_cmp(arg1, "hickory"))
  {
    bag->craft6 += amount;
  }

  send_to_char("You begin to fell a tree.\r\n", ch);
  ch_printf(ch, "&GYour chopping pays off as you successfully fell a %s tree!\r\n", arg1);

  act(AT_GREEN, "$n's chopping suddenly fells a tree.", ch, NULL, NULL, TO_ROOM);
  if(amount > 2)
  {
    send_to_char("You notice your able to save more lumber with your more precise fell of the tree as you cut off the branches of the felled tree and place it on your sled.\r\n", ch);
  }
  else
    send_to_char("You carefully cut off the branches of the felled tree and place it on your sled.\r\n", ch);
  if(xIS_SET(ch->act, PLR_CRAFTS))
    send_to_char("!!SOUND(sound/treefell.wav)\r\n", ch);

  learn_from_craft(ch, gsn_fell);
}

void send_mill_syntax(CHAR_DATA *ch)
{
  if(!ch)
    return;
  send_to_char("\r\n", ch);

  if(IS_BLIND(ch))
  {
    send_to_char("Type mill lumber into item level\r\n", ch);
    send_to_char("Type mill list for a list of items you can mill or mill list all to see everything.\r\n", ch);
    if(ch->pcdata->tradelevel > 9)
    {
      send_to_char("Type mill fasten to fasten ship pieces together.\r\n", ch);
      send_to_char("Type mill id number repair\r\n", ch);
      send_to_char("eg. mill 3232 repair\r\n", ch);

    }
    if(ch->pcdata->tradelevel > 14)
    {
      send_to_char("Type mill id number rename new name to change the objects name or mill id number rekey new key to change what you type to get the object.\r\n", ch);
    }
    send_to_char("Lumber possibilities include fir, cedar, pine, walnut, oak, hickory\r\n", ch);
    send_to_char("Note There are 4 steps, keep doing the mill command until the last step.\r\n", ch);
  }
  else
  {
    send_to_char("&cSyntax: mill <&Cmaterial&c> into <&Citem&c> <&Clevel&c>\r\n", ch);
    send_to_char("Syntax: mill list or mill list all\r\n", ch);
    if(ch->pcdata->tradelevel > 9)
    {
      send_to_char("Syntax: mill fasten\r\n", ch);
      send_to_char("Syntax: mill id# repair\r\n", ch);
      send_to_char("eg. mill 3115 repair\r\n", ch);

    }

    if(ch->pcdata->tradelevel > 14)
    {
      send_to_char("&cSyntax: mill id# rename 'new name' or mill id# rekey 'new key' to change what you type to get the object.\r\n", ch);
    }
    send_to_char("Materials being: fir, cedar, pine, walnut, oak, hickory\r\n", ch);
    send_to_char("Note: There are &C4 steps&c, keep doing the mill command until the last step.\r\n", ch);
  }
}

void do_mill(CHAR_DATA *ch, char *argument)
{
  short                   number;
  OBJ_DATA               *item, *deed, *obj;
  AFFECT_DATA            *paf;
  struct skill_type      *skill = NULL;
  char                    arg1[MIL], arg2[MIL], arg3[MIL], arg4[MIL];
  char                    name_buf[MSL], buf[MSL], short_buf[MSL], long_buf[MSL], extra_buf[MSL];
  const char             *adj;
  int                     i = 0, x = 0, difficulty = 0, output = 0, sn = 1;
  short                   mnum = 0;
  bool                    itm = FALSE, haslumber = FALSE, found = FALSE;

  name_buf[0] = '\0';
  short_buf[0] = '\0';
  long_buf[0] = '\0';
  extra_buf[0] = '\0';

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  argument = one_argument(argument, arg3);
  argument = one_argument(argument, arg4);

  if(IS_NPC(ch) || ch->desc == NULL)
    return;

  if(!arg1 || arg1[0] == '\0')
  {
    send_mill_syntax(ch);
    return;
  }

  if(!str_cmp(arg1, "fasten"))
  {
    if(ch->in_room->sector_type != SECT_DOCK)
    {
      send_to_char("You must be down by the dock to fasten ship pieces together.\r\n", ch);
      return;
    }
    bool                    shipaft = 0;
    bool                    shipmast = 0;
    bool                    shipstern = 0;
    bool                    shiphull = 0;

    for(item = ch->first_carrying; item; item = item->next_content)
    {
      if(item->item_type == ITEM_PIECE && !str_cmp(item->name, "ship-aft"))
      {
        shipaft = 1;
        item->value[0] = 999;
        break;
      }
    }
    for(item = ch->first_carrying; item; item = item->next_content)
    {
      if(item->item_type == ITEM_PIECE && !str_cmp(item->name, "ship-mast"))
      {
        shipmast = 1;
        item->value[0] = 999;
        break;
      }
    }
    for(item = ch->first_carrying; item; item = item->next_content)
    {
      if(item->item_type == ITEM_PIECE && !str_cmp(item->name, "ship-stern"))
      {
        shipstern = 1;
        item->value[0] = 999;
        break;
      }
    }
    for(item = ch->first_carrying; item; item = item->next_content)
    {

      if(item->item_type == ITEM_PIECE && !str_cmp(item->name, "ship-hull"))
      {
        shiphull = 1;
        item->value[0] = 999;
        break;
      }

    }
    if(shiphull == 0 || shipstern == 0 || shipmast == 0 || shipaft == 0)
    {
      send_to_char("You do not have all the ship parts to fasten.\r\n", ch);
      return;
    }

    // generate steps water proofing, fastening.
    if(item->value[6] == 0)
    {
      short                   done = number_range(1, 10);

      act(AT_CYAN, "$n starts to paint the shellac on the ship pieces.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You begins to paint the shellac on the ship pieces.", ch, NULL, NULL, TO_CHAR);
      WAIT_STATE(ch, 12);
      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/painting.wav)\r\n", ch);

      if(done > 7)
      {
        item->value[6] = 7;
        learn_from_craft(ch, gsn_fell);
      }
      else
        return;
    }

    if(item->value[6] == 7)
    {
      short                   done = number_range(1, 10);

      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/hammer.wav)\r\n", ch);

      act(AT_CYAN, "$n begins fastening the ship pieces together.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You begin to fasten the ship pieces together.", ch, NULL, NULL, TO_CHAR);
      WAIT_STATE(ch, 12);

      if(done < 3)
      {
        item->value[6] = 8;
        learn_from_craft(ch, gsn_fell);
      }
      else
        return;

    }
    if(item->value[6] == 8)
    {

      act(AT_CYAN, "$n's ship is successfully completed, and a deed is issued.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "Your ship is successfully completed and a deed has been issued.", ch, NULL, NULL, TO_CHAR);
      // create the deed and give to player
      learn_from_craft(ch, gsn_fell);
      deed = create_object(get_obj_index(OBJ_VNUM_DEED), 1);
      deed->level = 1;

      snprintf(buf, MSL, "%s's ship deed", ch->name);
      STRFREE(deed->short_descr);
      deed->short_descr = STRALLOC(buf);
      STRFREE(deed->name);
      deed->name = STRALLOC(buf);
      STRFREE(deed->description);
      deed->description = STRALLOC("A deed to some sort of property has been left here.");
      /*
       * To avoid freeing a null pointer
       */
      if(deed->owner == NULL)
      {
        deed->owner = STRALLOC(ch->name);
      }
      else
      {
        STRFREE(deed->owner);
        deed->owner = STRALLOC(ch->name);
      }
    }
    deed = obj_to_char(deed, ch);
    for(item = ch->first_carrying; item; item = item->next_content)
    {
      if(item->item_type == ITEM_PIECE && item->value[0] == 999)
      {
        extract_obj(item);
      }
    }
    ch->pcdata->hull = 100;
    save_char_obj(ch);
    return;
  }

// Just adding objID to work for repair code section 

  if(!str_cmp(arg2, "repair"))
  {
    OBJ_DATA               *repair;

    for(repair = ch->first_carrying; repair; repair = repair->next_content)
    {
      if(atoi(arg1) == repair->guid && (repair->item_type == ITEM_ARMOR || repair->item_type == ITEM_WEAPON))
        break;
    }

    if(!repair)
    {
      send_to_char("You do not have any repairable equipment to repair.\r\n", ch);
      return;
    }

    if(ch->pcdata->tradelevel < 10)
    {
      send_to_char("You are not skilled enough of a craftsman to repair items yet.\r\n", ch);
      return;
    }
    if(repair->item_type == ITEM_WEAPON && repair->value[0] == 12)
    {
      send_to_char("It is already honed to perfection.\r\n", ch);
      return;
    }
    if(repair->item_type == ITEM_ARMOR && repair->value[0] == repair->value[1])
    {
      send_to_char("It is already forged to perfection.\r\n", ch);
      return;
    }

    act(AT_CYAN, "$n hammers $p back to shape.", ch, repair, NULL, TO_ROOM);
    act(AT_CYAN, "You hammer $p back to shape.", ch, repair, NULL, TO_CHAR);
    if(xIS_SET(ch->act, PLR_CRAFTS))
      send_to_char("!!SOUND(sound/hammer.wav)\r\n", ch);

    switch (repair->item_type)
    {
      case ITEM_ARMOR:
        repair->value[0] = repair->value[1];
        break;
      case ITEM_WEAPON:
        repair->value[0] = INIT_WEAPON_CONDITION;
        break;
    }
    return;
  }

// End of repair code section 

  if(!str_cmp(arg2, "rename") && ch->pcdata->tradelevel > 14 && !xIS_SET(ch->act, PLR_NORENAME))
  {
    OBJ_DATA               *item;

    for(item = ch->first_carrying; item; item = item->next_content)
    {
      if(atoi(arg1) == item->guid && item->pIndexData->vnum == SMITH_PRODUCT)
        break;
    }
    if(!item)
    {
      send_to_char("You do not have any milled items to rename or rekey.\r\n", ch);
      return;
    }

    if(!arg3 || arg3[0] == '\0')
    {
      send_to_char("You must specify the new keywords, ie mill id number rename 'new name' or rename 'ring toering'.\r\n", ch);
      return;
    }
    ch_printf(ch, "&cChanging item's description from &C%s&c to&C %s&c..\r\n", item->short_descr, arg3);
    STRFREE(item->short_descr);
    item->short_descr = STRALLOC(arg3);
    sprintf(long_buf, "%s has been left here along the ground.", item->short_descr);
    STRFREE(item->description);
    item->description = STRALLOC(long_buf);
    send_to_char("&RDone!&c Please note, the KEYWORDS used to drop/equip/etc remain the same.\r\n", ch);
    save_char_obj(ch);
    return;
  }
  else if(!str_cmp(arg2, "rename") && (ch->pcdata->tradelevel < 15 || xIS_SET(ch->act, PLR_NORENAME)))
  {
    send_to_char("You must specify the new keywords, ie rename id number 'new name' or rename 'ring toering'.\r\n", ch);
    return;
  }

  if(!str_cmp(arg2, "rekey") && ch->pcdata->tradelevel > 14 && !xIS_SET(ch->act, PLR_NORENAME))
  {
    OBJ_DATA               *item;

    for(item = ch->first_carrying; item; item = item->next_content)
    {
      if(atoi(arg1) == item->guid && item->pIndexData->vnum == 41002)
        break;
    }
    if(!item)
    {
      send_to_char("You do not have any milled items to rename or rekey.\r\n", ch);
      return;
    }

    ch_printf(ch, "&cChanging item's keywords from &C%s&c to &C%s&c..\r\n", item->name, arg3);
    STRFREE(item->name);
    item->name = STRALLOC(arg3);
    send_to_char("&RDone!&c Please note, the DESCRIPTION when looking at the item remains the same.\r\n", ch);
    save_char_obj(ch);
    return;
  }
  else if(!str_cmp(arg2, "rekey") && (ch->pcdata->tradelevel < 15 || xIS_SET(ch->act, PLR_NORENAME)))
  {
    send_to_char("You must specify the new keywords, ie 'mill rekey whatever new name' or 'mix rekey sword toothpick'.\r\n", ch);
    return;
  }

  if(!str_cmp(arg1, "list"))
  {
    int                     y, count = 0;
    char                    wearloc[MSL];
    short                   value;

    send_to_char("&CThe mill Listing\r\n\r\n", ch);

    if(!str_cmp(arg2, "all"))
    {
      for(y = 0; y < MAX_MILL_ITEMS; y++)
      {
/*
* We don't want the spaces so done seperate and then spaces handled where sent
*/
        if(arg2 && !is_number(arg2))
          value = get_wflag(arg2);
        if(value < 0 || value > 33)
          value = 0;

        if(value == 0)
        {
          if(!IS_BLIND(ch))
          {
            if(((!str_cmp(arg2, "0") && lumber_one[y].level == 0)
                || (!str_cmp(arg2, "1") && lumber_one[y].level == 1)
                || (!str_cmp(arg2, "2") && lumber_one[y].level == 2)
                || (!str_cmp(arg2, "3") && lumber_one[y].level == 3)
                || (!str_cmp(arg2, "4") && lumber_one[y].level == 4)
                || (!str_cmp(arg2, "5") && lumber_one[y].level == 5)
                || (!str_cmp(arg2, "6") && lumber_one[y].level == 6)
                || (!str_cmp(arg2, "7") && lumber_one[y].level == 7)
                || (!str_cmp(arg2, "8") && lumber_one[y].level == 8)
                || (!str_cmp(arg2, "9") && lumber_one[y].level == 9)
                || (!str_cmp(arg2, "10") && lumber_one[y].level == 10)
                || (!str_cmp(arg2, "11") && lumber_one[y].level == 11)
                || (!str_cmp(arg2, "12") && lumber_one[y].level == 12)
                || (!str_cmp(arg2, "13") && lumber_one[y].level == 13)
                || (!str_cmp(arg2, "14") && lumber_one[y].level == 14)
                || (!str_cmp(arg2, "15") && lumber_one[y].level == 15)
                || (!str_cmp(arg2, "16") && lumber_one[y].level == 16)
                || (!str_cmp(arg2, "17") && lumber_one[y].level == 17)
                || (!str_cmp(arg2, "18") && lumber_one[y].level == 18)
                || (!str_cmp(arg2, "19") && lumber_one[y].level == 19)
                || (!str_cmp(arg2, "20") && lumber_one[y].level == 20))
               || (!str_cmp(arg1, "list") && str_cmp(arg2, "0")
                   && str_cmp(arg2, "1") && str_cmp(arg2, "2")
                   && str_cmp(arg2, "3") && str_cmp(arg2, "4")
                   && str_cmp(arg2, "5") && str_cmp(arg2, "6")
                   && str_cmp(arg2, "7") && str_cmp(arg2, "8")
                   && str_cmp(arg2, "9") && str_cmp(arg2, "10")
                   && str_cmp(arg2, "11") && str_cmp(arg2, "12")
                   && str_cmp(arg2, "13") && str_cmp(arg2, "14")
                   && str_cmp(arg2, "15") && str_cmp(arg2, "16") && str_cmp(arg2, "17") && str_cmp(arg2, "18") && str_cmp(arg2, "19") && str_cmp(arg2, "20")))
            {
              count++;
              snprintf(wearloc, sizeof(wearloc), "&c(&w%s&c)%s(%d)", flag_string(lumber_one[y].wear_flags - 1, w_flags), lumber_one[y].name ? lumber_one[y].name : "null name", lumber_one[y].level);
              ch_printf(ch, "&c%-32s ", wearloc);
            }
          }
          else
          {
            if(((!str_cmp(arg2, "0") && lumber_one[y].level == 0)
                || (!str_cmp(arg2, "1") && lumber_one[y].level == 1)
                || (!str_cmp(arg2, "2") && lumber_one[y].level == 2)
                || (!str_cmp(arg2, "3") && lumber_one[y].level == 3)
                || (!str_cmp(arg2, "4") && lumber_one[y].level == 4)
                || (!str_cmp(arg2, "5") && lumber_one[y].level == 5)
                || (!str_cmp(arg2, "6") && lumber_one[y].level == 6)
                || (!str_cmp(arg2, "7") && lumber_one[y].level == 7)
                || (!str_cmp(arg2, "8") && lumber_one[y].level == 8)
                || (!str_cmp(arg2, "9") && lumber_one[y].level == 9)
                || (!str_cmp(arg2, "10") && lumber_one[y].level == 10)
                || (!str_cmp(arg2, "11") && lumber_one[y].level == 11)
                || (!str_cmp(arg2, "12") && lumber_one[y].level == 12)
                || (!str_cmp(arg2, "13") && lumber_one[y].level == 13)
                || (!str_cmp(arg2, "14") && lumber_one[y].level == 14)
                || (!str_cmp(arg2, "15") && lumber_one[y].level == 15)
                || (!str_cmp(arg2, "16") && lumber_one[y].level == 16)
                || (!str_cmp(arg2, "17") && lumber_one[y].level == 17)
                || (!str_cmp(arg2, "18") && lumber_one[y].level == 18)
                || (!str_cmp(arg2, "19") && lumber_one[y].level == 19)
                || (!str_cmp(arg2, "20") && lumber_one[y].level == 20))
               || (!str_cmp(arg1, "list") && str_cmp(arg2, "0")
                   && str_cmp(arg2, "1") && str_cmp(arg2, "2")
                   && str_cmp(arg2, "3") && str_cmp(arg2, "4")
                   && str_cmp(arg2, "5") && str_cmp(arg2, "6")
                   && str_cmp(arg2, "7") && str_cmp(arg2, "8")
                   && str_cmp(arg2, "9") && str_cmp(arg2, "10")
                   && str_cmp(arg2, "11") && str_cmp(arg2, "12")
                   && str_cmp(arg2, "13") && str_cmp(arg2, "14")
                   && str_cmp(arg2, "15") && str_cmp(arg2, "16") && str_cmp(arg2, "17") && str_cmp(arg2, "18") && str_cmp(arg2, "19") && str_cmp(arg2, "20")))
            {
              count++;
              ch_printf(ch, "%2d &c%-28s", lumber_one[y].level, lumber_one[y].name ? lumber_one[y].name : "null name");
            }
          }
        }
        else
        {
          if(!IS_BLIND(ch))
          {
            if((1 << value) != (lumber_one[y].wear_flags - 1))
              continue;
            count++;
            snprintf(wearloc, sizeof(wearloc), "&c(&w%s&c)%s(%d)", flag_string(lumber_one[y].wear_flags - 1, w_flags), lumber_one[y].name ? lumber_one[y].name : "null name", lumber_one[y].level);
            ch_printf(ch, "&c%-32s ", wearloc);
          }
          else
          {
            if((1 << value) != (lumber_one[y].wear_flags - 1))
              continue;
            count++;
            ch_printf(ch, "%2d &c%-28s", lumber_one[y].level, lumber_one[y].name ? lumber_one[y].name : "null name");
          }
        }
        if(count == 3)
        {
          send_to_char("\r\n", ch);
          count = 0;
        }
      }

      if(count != 3)
      {
        send_to_char("\r\n\r\n", ch);
      }
      send_mill_syntax(ch);
      return;
    }

    for(y = 0; y < MAX_MILL_ITEMS; y++)
    {
      if(ch->pcdata->tradelevel < lumber_one[y].level)
        continue;
      count++;
      ch_printf(ch, "&c%-15s ", lumber_one[y].name ? lumber_one[y].name : "null name");
      if(count == 3)
      {
        count = 0;
        send_to_char("\r\n", ch);
      }
    }
    if(count != 0)
      send_to_char("\r\n", ch);
    send_mill_syntax(ch);
    return;
  }

  OBJ_DATA               *bag;

  for(bag = ch->first_carrying; bag; bag = bag->next_content)
  {
    if(bag->item_type == ITEM_RESOURCE_BAG && bag->value[0] == 500)
      break;
  }

  if(!bag)
  {
    send_to_char("You do not have a lumber sled to mill with.\r\n", ch);
    return;
  }

  if(arg2[0] == '\0' || arg3[0] == '\0')
  {
    send_mill_syntax(ch);
    return;
  }

  if(str_cmp(arg2, "into"))
  {
    send_mill_syntax(ch);
    return;
  }

  if(!IS_SET(ch->in_room->room_flags, ROOM_TRADESKILLS))
  {
    send_to_char("You must be in a tradeskills building to do this.\r\n", ch);
    return;
  }

  {

    {

// This is the nitty gritty of checking tier output 
      i = mill_lookup(arg3);
      if(i < 0 || ch->pcdata->tradelevel < lumber_one[i].level)
      {
        send_to_char("That isn't a something you can create.\r\n", ch);
        return;
      }

      if(str_cmp(arg1, "fir") && str_cmp(arg1, "cedar") && str_cmp(arg1, "pine") && str_cmp(arg1, "walnut") && str_cmp(arg1, "oak") && str_cmp(arg1, "hickory"))
      {
        send_to_char("You can only use fir, cedar, pine, walnut, oak, or hickory.\r\n", ch);
        return;
      }

      short                   lumber = 1;

      if(!str_cmp(arg2, "ship-aft") || !str_cmp(arg2, "ship-hull") || !str_cmp(arg2, "ship-mast") || !str_cmp(arg2, "ship-stern"))
        lumber = 5;

      if(!str_cmp(arg1, "fir") && bag->craft1 < lumber)
      {
        send_to_char("You don't have enough fir lumber in your lumber sled.\r\n", ch);
        return;
      }
      else if(!str_cmp(arg1, "cedar") && bag->craft2 < lumber)
      {
        send_to_char("You don't have enough cedar lumber in your lumber sled.\r\n", ch);
        return;
      }
      else if(!str_cmp(arg1, "pine") && bag->craft3 < lumber)
      {
        send_to_char("You don't have enough pine lumber in your lumber sled.\r\n", ch);
        return;
      }
      else if(!str_cmp(arg1, "walnut") && bag->craft4 < lumber)
      {
        send_to_char("You don't have enough walnut lumber in your lumber sled.\r\n", ch);
        return;
      }
      else if(!str_cmp(arg1, "oak") && bag->craft5 < lumber)
      {
        send_to_char("You don't have enough oak lumber in your lumber sled.\r\n", ch);
        return;
      }
      else if(!str_cmp(arg1, "hickory") && bag->craft6 < lumber)
      {
        send_to_char("You don't have enough hickory lumber in your lumber sled.\r\n", ch);
        return;
      }

      if(!str_cmp(arg1, "fir"))
      {
        bag->craft1 -= lumber;
        x = 1;
      }
      else if(!str_cmp(arg1, "cedar"))
      {
        bag->craft2 -= lumber;
        x = 2;
      }
      else if(!str_cmp(arg1, "pine"))
      {
        bag->craft3 -= lumber;
        x = 3;
      }
      else if(!str_cmp(arg1, "walnut"))
      {
        bag->craft4 -= lumber;
        x = 4;
      }
      else if(!str_cmp(arg1, "oak"))
      {
        bag->craft5 -= lumber;
        x = 5;
      }
      else if(!str_cmp(arg1, "hickory"))
      {
        bag->craft6 -= lumber;
        x = 6;
      }

      SKILLTYPE              *skill = get_skilltype(gsn_mill);

      WAIT_STATE(ch, 15);

      short                   failure = 0;

      if(ch->pcdata->tradelevel < 5)
        failure = 14;
      else if(ch->pcdata->tradelevel < 10 && ch->pcdata->tradelevel > 4)
        failure = 10;
      else if(ch->pcdata->tradelevel < 15 && ch->pcdata->tradelevel > 9)
        failure = 8;
      else if(ch->pcdata->tradelevel < 20 && ch->pcdata->tradelevel > 14)
        failure = 5;
      else if(ch->pcdata->tradelevel == 20)
        failure = 2;

      if(bag->value[6] == 0)
      {
        if(ch->pcdata->learned[gsn_mill] <= number_range(1, 5) || number_percent() <= failure)
        {
          act(AT_CYAN, "$n prepares the lumber by measuring it, but fail to measure twice and got it wrong size lumber.", ch, NULL, NULL, TO_ROOM);
          act(AT_CYAN, "You prepare the lumber by measuring it, but fail to measure twice and got the wrong size lumber.", ch, NULL, NULL, TO_CHAR);
          if(xIS_SET(ch->act, PLR_CRAFTS))
            send_to_char("!!SOUND(sound/measure.wav)\r\n", ch);

          learn_from_failure(ch, gsn_mill);
          return;
        }
        bag->value[6] = 1;
        act(AT_CYAN, "$n prepares the lumber by measuring twice to ensure proper dimensions prior to cutting.", ch, NULL, NULL, TO_ROOM);
        act(AT_CYAN, "You prepare the lumber by measuring twice to ensure proper dimensions prior to cutting.", ch, NULL, NULL, TO_CHAR);
        if(xIS_SET(ch->act, PLR_CRAFTS))
          send_to_char("!!SOUND(sound/measure.wav)\r\n", ch);

        WAIT_STATE(ch, 15);
        obj = create_object(get_obj_index(OBJ_VNUM_ORE), 0);
        if(obj->name)
          STRFREE(obj->name);
        if(obj->short_descr)
          STRFREE(obj->short_descr);
        if(obj->description)
          STRFREE(obj->description);
        obj->name = STRALLOC(arg1);
        obj->description = STRALLOC("A rough lumber piece has been left here.");
        obj->short_descr = STRALLOC("a rough lumber piece");
        obj_to_char(obj, ch);
        learn_from_success(ch, gsn_mill);
        return;
      }

      if(bag->value[6] == 1)
      {
        if(ch->pcdata->learned[gsn_mill] <= number_range(1, 15) || number_percent() <= failure)
        {
          act(AT_CYAN, "$n places the lumber in the mill and begins to cut it to the proper measurements, but gets distracted and cuts too much.", ch, NULL, NULL, TO_ROOM);
          act(AT_CYAN, "You place the lumber in the mill and begin to cut it to the proper measurements, but get distracted and cut too much.", ch, NULL, NULL, TO_CHAR);
          if(xIS_SET(ch->act, PLR_CRAFTS))
            send_to_char("!!SOUND(sound/saw.wav)\r\n", ch);

          for(obj = ch->first_carrying; obj; obj = obj->next_content)
          {
            if(obj->item_type == ITEM_RAW)
              break;
          }

          if(!obj)
          {
            send_to_char("You do not have your lumber piece.\r\n", ch);
            bag->value[6] = 0;
            return;
          }

          separate_obj(obj);
          obj_from_char(obj);
          extract_obj(obj);

          if(!str_cmp(arg1, "fir"))
          {
            bag->craft1 -= lumber;
            if(bag->craft1 < 0)
              bag->craft1 = 0;
          }
          else if(!str_cmp(arg1, "cedar"))
          {
            bag->craft2 -= lumber;
            if(bag->craft2 < 0)
              bag->craft2 = 0;
          }
          else if(!str_cmp(arg1, "pine"))
          {
            bag->craft3 -= lumber;
            if(bag->craft3 < 0)
              bag->craft3 = 0;
          }
          else if(!str_cmp(arg1, "walnut"))
          {
            bag->craft4 -= lumber;
            if(bag->craft4 < 0)
              bag->craft4 = 0;
          }
          else if(!str_cmp(arg1, "oak"))
          {
            bag->craft5 -= lumber;
            if(bag->craft5 < 0)
              bag->craft5 = 0;
          }
          else if(!str_cmp(arg1, "hickory"))
          {
            bag->craft6 -= lumber;
            if(bag->craft6 < 0)
              bag->craft6 = 0;
          }
          learn_from_failure(ch, gsn_mill);
          return;
        }

        act(AT_CYAN, "$n places the lumber on the mill and cuts it to the proper size.", ch, NULL, NULL, TO_ROOM);
        act(AT_CYAN, "You place the lumber on the mill and cut it to the proper size.", ch, NULL, NULL, TO_CHAR);
        if(number_range(1, 4) > 2)
          learn_from_craft(ch, gsn_mill); // add a chance of gaining exp

        if(xIS_SET(ch->act, PLR_CRAFTS))
          send_to_char("!!SOUND(sound/saw.wav)\r\n", ch);

        WAIT_STATE(ch, 15);

        OBJ_DATA               *obj;

        for(obj = ch->first_carrying; obj; obj = obj->next_content)
        {
          if(obj->item_type == ITEM_RAW)
            break;
        }

        if(!obj)
        {
          send_to_char("You do not have your lumber piece.\r\n", ch);
          bag->value[6] = 0;
          return;
        }
        bag->value[6] = 2;
        if(obj->short_descr)
          STRFREE(obj->short_descr);
        obj->short_descr = STRALLOC("&Opiece of lumber");
        learn_from_success(ch, gsn_mill);
        return;
      }
/* 
* They should only get these and spend alot of time on the hammery part the rest 
* should be fairly straight foward 
*/
      if(bag->value[6] == 2)
      {
        if(!can_use_skill(ch, number_percent(), gsn_mill) || number_percent() <= failure)
        {
          act(AT_CYAN, "$n begins to carve away at the lumber, but gouges it beyond use", ch, NULL, NULL, TO_ROOM);
          act(AT_CYAN, "You start to carve the lumber into the shape needed but accidently gouge it beyond use.", ch, NULL, NULL, TO_CHAR);
          if(xIS_SET(ch->act, PLR_CRAFTS))
            send_to_char("!!SOUND(sound/carve.wav)\r\n", ch);

          learn_from_failure(ch, gsn_mill);
          return;
        }

        OBJ_DATA               *obj;

        for(obj = ch->first_carrying; obj; obj = obj->next_content)
        {
          if(obj->item_type == ITEM_RAW)
            break;
        }

        if(!obj)
        {
          send_to_char("You do not have your lumber piece.\r\n", ch);
          bag->value[6] = 0;
          return;
        }

        if(number_percent() <= failure)
        {
          act(AT_CYAN, "$n begins to carve away at the lumber, but gouges it beyond use", ch, NULL, NULL, TO_ROOM);
          act(AT_CYAN, "You start to carve the lumber into the shape needed but accidently gouge it beyond use.", ch, NULL, NULL, TO_CHAR);
          if(xIS_SET(ch->act, PLR_CRAFTS))
            send_to_char("!!SOUND(sound/carve.wav)\r\n", ch);

          learn_from_failure(ch, gsn_mill);
          bag->value[6] = 1;
          if(obj->short_descr)
            STRFREE(obj->short_descr);
          obj->short_descr = STRALLOC("&Oa carved piece of lumber");
          return;
        }

        act(AT_CYAN, "$n places the carved lumber in a vice.", ch, NULL, NULL, TO_ROOM);
        act(AT_CYAN, "You place the carved lumber in a vice.", ch, NULL, NULL, TO_CHAR);
        WAIT_STATE(ch, 15);

        if(xIS_SET(ch->act, PLR_CRAFTS))
          send_to_char("!!SOUND(sound/vice.wav)\r\n", ch);

        bag->value[6] = 3;
        if(obj->short_descr)
          STRFREE(obj->short_descr);
        obj->short_descr = STRALLOC("&wthe roughly finished product");
        learn_from_success(ch, gsn_mill);
        return;
      }

      if(!can_use_skill(ch, number_percent(), gsn_mill) || number_percent() <= failure)
      {
        OBJ_DATA               *obj;

        for(obj = ch->first_carrying; obj; obj = obj->next_content)
        {
          if(obj->item_type == ITEM_RAW)
            break;
        }

        if(!obj)
        {
          send_to_char("You do not have your lumber piece.\r\n", ch);
          bag->value[6] = 0;
          return;
        }

        act(AT_CYAN, "You begin to sand the carved lumber to smooth out the rough edges, but get distracted and take too much material off.", ch, NULL, NULL, TO_CHAR);
        act(AT_CYAN, "$n begins to sand the carved lumber to smooth out the rough edges, but gets distracted and takes too much material off.", ch, NULL, NULL, TO_ROOM);
        if(xIS_SET(ch->act, PLR_CRAFTS))
          send_to_char("!!SOUND(sound/grinding.wav)\r\n", ch);

        obj_from_char(obj);
        extract_obj(obj);
        bag->value[6] = 0;
        if(!str_cmp(arg1, "fir"))
        {
          bag->craft1 -= lumber;
        }
        else if(!str_cmp(arg1, "cedar"))
        {
          bag->craft2 -= lumber;
        }
        else if(!str_cmp(arg1, "pine"))
        {
          bag->craft3 -= lumber;
        }
        else if(!str_cmp(arg1, "walnut"))
        {
          bag->craft4 -= lumber;
        }
        else if(!str_cmp(arg1, "oak"))
        {
          bag->craft5 -= lumber;
        }
        else if(!str_cmp(arg1, "hickory"))
        {
          bag->craft6 -= lumber;
        }
        learn_from_failure(ch, gsn_mill);
        return;
      }
      act(AT_CYAN, "You begin to sand the carved lumber to smooth out the rough edges.", ch, NULL, NULL, TO_CHAR);
      act(AT_CYAN, "$n begins to sand the carved lumber to smooth out the rough edges.", ch, NULL, NULL, TO_ROOM);
      if(xIS_SET(ch->act, PLR_CRAFTS))
        send_to_char("!!SOUND(sound/grind2.wav)\r\n", ch);

      WAIT_STATE(ch, 15);

      for(obj = ch->first_carrying; obj; obj = obj->next_content)
      {
        if(obj->item_type == ITEM_RAW)
          break;
      }

      if(!obj)
      {
        send_to_char("You do not have your lumber piece.\r\n", ch);
        bag->value[6] = 0;
        return;
      }

      bag->value[6] = 0;
      learn_from_craft(ch, gsn_mill);

      item = create_object(get_obj_index(SMITH_PRODUCT), 1);
      if(!str_cmp(arg1, "fir"))
        item->color = 1;
      else if(!str_cmp(arg1, "cedar"))
        item->color = 5;
      else if(!str_cmp(arg1, "pine"))
        item->color = 14;
      else if(!str_cmp(arg1, "walnut"))
        item->color = 13;
      else if(!str_cmp(arg1, "oak"))
        item->color = 13;
      else if(!str_cmp(arg1, "hickory"))
        item->color = 9;

      if(arg4[0] == '\0')
        item->level = ch->level;
      else
        item->level = atoi(arg4);

      {
        int                     tier, item_type, wearflags, weight, cost;
        const char             *name;

        if(lumber_one[i].level >= 15)
          tier = 3;
        else if(lumber_one[i].level > 5 && lumber_one[i].level < 15)
          tier = 2;
        else
          tier = 1;

        name = lumber_one[i].name;
        item_type = lumber_one[i].item_type;
        wearflags = lumber_one[i].wear_flags;
        weight = lumber_one[i].weight;
        cost = lumber_one[i].weight;
        mnum = 1;

        if(ch->pcdata->tradelevel <= 1)
          adj = "poorly";
        else if(ch->pcdata->tradelevel <= 5)
          adj = "simply";
        else if(ch->pcdata->tradelevel <= 8)
          adj = "properly";
        else if(ch->pcdata->tradelevel <= 10)
          adj = "well";
        else if(ch->pcdata->tradelevel <= 15)
          adj = "finely";
        else if(ch->pcdata->tradelevel <= 19)
          adj = "masterfully";
        else
          adj = "legendary";

        for(x = 0; x < 7 - 1; x++)
        {
          if(!str_cmp(arg1, lumber_table[x].name))
          {
            haslumber = TRUE;
            break;
          }
        }

        sprintf(name_buf, "%s", name);
        sprintf(short_buf, "a %s, %s milled from %s %s lumber", name, adj, lumber_table[x].color, lumber_table[x].name);
        sprintf(long_buf, "Here lies a %s, %s milled from %s %s lumber.", name, adj, lumber_table[x].color, lumber_table[x].name);
// separate_obj( obj ); 
        // obj_from_char( obj );
        extract_obj(obj);

        if(item->name)
          STRFREE(item->name);
        if(item->short_descr)
          STRFREE(item->short_descr);
        if(item->description)
          STRFREE(item->description);

        item->name = STRALLOC(name_buf);
        item->short_descr = STRALLOC(short_buf);
        item->description = STRALLOC(long_buf);

        item->item_type = item_type;
        item->wear_flags += wearflags;
        item->weight = weight;
        item->cost = cost;
        if(item->item_type == ITEM_LIGHT)
        {
          send_to_char("\r\n&cIt's a newly milled torch!\r\n", ch);
        }
        else if(item->item_type == ITEM_SHOVEL)
        {
          send_to_char("\r\n&cIt's a newly milled shovel!\r\n", ch);
        }
        else if(item->item_type == ITEM_BOAT)
        {
          send_to_char("\r\n&cIt's a newly milled boat!\r\n", ch);
        }
        else if(item->item_type == ITEM_TOOL)
        {
          send_to_char("\r\n&cIt's a newly milled tool!\r\n", ch);
        }
        else if(item->item_type == ITEM_FURNITURE)
        {
          send_to_char("\r\n&cIt's a newly milled piece of furniture!\r\n", ch);
          item->value[0] = lumber_one[i].base_v0;
          item->value[1] = lumber_one[i].base_v1;
          item->value[2] = lumber_one[i].base_v2;
          item->value[3] = ch->pcdata->tradelevel * 10;
          item->value[4] = ch->pcdata->tradelevel * 10;
        }
        else if(item_type == ITEM_PIECE)
          send_to_char("\r\n&cIt's a newly milled part of a ship!\r\n", ch);
        else if(item->item_type == ITEM_ARMOR)
        {
          send_to_char("\r\n&cIts a newly milled shield!\r\n", ch);
          item->value[0] = set_min_armor(item->level);
          item->value[1] = set_max_armor(item->level);
          item->value[4] = 777;
          item->pIndexData->layers = 128;
          if(ch->pcdata->tradelevel < 5)
          { // first tier crafted armor 
            item->value[3] = 100;
          }
          else if(ch->pcdata->tradelevel > 4 && ch->pcdata->tradelevel < 10)
          { // second 
            // 
            // 
            // 
            // 
            // 
            // 
            item->value[3] = 200;
          }
          else if(ch->pcdata->tradelevel > 9 && ch->pcdata->tradelevel < 20)
          { // third 
            // 
            // 
            // 
            // 
            // 
            // 
            // 
            // 
            // 
            item->value[3] = 300;
          }
          else if(ch->pcdata->tradelevel == 20)
          { // fourth tier crafted 
// armor 
            item->value[3] = 400;
          }
        }

        else if(item->item_type == ITEM_MISSILE_WEAPON)
        {
          send_to_char("\r\n&cIts a newly milled missile weapon!\r\n", ch);
          item->value[0] = 12;
          item->value[1] = set_tier_min(item->level, tier);
          item->value[2] = set_tier_max(item->level, tier);
          item->value[4] = 6;
          item->value[3] = ch->pcdata->tradelevel;
          if(!str_cmp(arg3, "crossbow"))
          {
            item->value[6] = 0;
          }
          else
          {
            item->value[6] = 1;
          }
          if(!str_cmp(arg1, "fir"))
          {
            item->value[1] += 2;
            item->value[2] += 5;
          }
          else if(!str_cmp(arg1, "cedar"))
          {
            item->value[1] += 4;
            item->value[2] += 10;
          }
          else if(!str_cmp(arg1, "pine"))
          {
            item->value[1] += 5;
            item->value[2] += 12;
          }
          else if(!str_cmp(arg1, "walnut"))
          {
            item->value[1] += 6;
            item->value[2] += 15;
          }
          else if(!str_cmp(arg1, "oak"))
          {
            item->value[1] += 8;
            item->value[2] += 16;
          }
          else if(!str_cmp(arg1, "hickory"))
          {
            item->value[1] += 10;
            item->value[2] += 20;
          }

        }
        else if(item->item_type == ITEM_PROJECTILE)
        {
          send_to_char("\r\n&cIts a newly milled projectile!\r\n", ch);

          CREATE(paf, AFFECT_DATA, 2);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_HITROLL;
          paf->modifier = ch->pcdata->tradelevel / 4;
          if(paf->modifier < 1)
            paf->modifier = 1;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);

          if(!str_cmp(arg1, "fir"))
          {
            item->value[2] = item->level / 2;
            item->value[1] += 4;
            item->value[2] += 15;
          }
          else if(!str_cmp(arg1, "cedar"))
          {
            item->value[2] = item->level / 2;
            item->value[1] += 6;
            item->value[2] += 20;
          }
          else if(!str_cmp(arg1, "pine"))
          {
            item->value[2] = item->level / 2;
            item->value[1] += 8;
            item->value[2] += 22;

          }
          else if(!str_cmp(arg1, "walnut"))
          {
            item->value[2] = item->level / 2;
            item->value[1] += 10;
            item->value[2] += 25;
          }
          else if(!str_cmp(arg1, "oak"))
          {
            item->value[2] = item->level / 2;
            item->value[1] += 12;
            item->value[2] += 30;
          }
          else
          {
            item->value[2] = item->level / 2;
            item->value[1] += 15;
            item->value[2] += 35;
          }
          if(!str_cmp(item->name, "bolt"))
            item->value[4] = 0;
          else
            item->value[4] = 1;
        }

        else if(item->item_type == ITEM_SABOTAGE)
        {
          send_to_char("\r\n&cIts a newly milled city hardened defense!\r\n", ch);
        }
        else if(item->item_type == ITEM_CONTAINER)
        {
          send_to_char("\r\n&cIts a newly milled container!\r\n", ch);
          item->value[0] = ch->pcdata->tradelevel * 25;
          item->value[1] = 0;
          item->value[2] = 0;
          item->value[3] = 12;
          if(!str_cmp(arg1, "fir"))
          {
            item->value[0] += 50;
          }
          else if(!str_cmp(arg1, "cedar"))
          {
            item->value[0] += 100;
          }
          else if(!str_cmp(arg1, "pine"))
          {
            item->value[0] += 150;
          }
          else if(!str_cmp(arg1, "walnut"))
          {
            item->value[0] += 200;
          }
          else if(!str_cmp(arg1, "oak"))
          {
            item->value[0] += 250;
          }
          else if(!str_cmp(arg1, "hickory"))
          {
            item->value[0] += 300;
          }
        }
        else if(item->item_type == ITEM_QUIVER)
        {
          item->value[0] = 50;
        }
        else if(item->item_type == ITEM_WEAPON)
        {
          send_to_char("\r\n&cIts a newly milled weapon!\r\n", ch);
          item->value[0] = 12;
          item->value[1] = set_tier_min(item->level, tier);
          item->value[2] = set_tier_max(item->level, tier);
          item->value[3] = lumber_one[i].base_v3;
          item->value[4] = lumber_one[i].base_v4;
          if(ch->pcdata->tradelevel < 5)
          {
            item->value[6] = 200;
          }
          else if(ch->pcdata->tradelevel > 4 && ch->pcdata->tradelevel < 10)
          {
            item->value[6] = 300;
          }
          else if(ch->pcdata->tradelevel > 9 && ch->pcdata->tradelevel < 15)
          {
            item->value[6] = 400;
          }
          else if(ch->pcdata->tradelevel > 14 && ch->pcdata->tradelevel < 20)
          {
            item->value[6] = 500;
          }
          else if(ch->pcdata->tradelevel == 20)
          {
            item->value[6] = 600;
          }

        }
      }

      sprintf(extra_buf,
              "\r\n&CThis crafted item bears the seal of %s, the %s carpenter.\r\n",
              ch->name,
              ch->pcdata->tradelevel <= 5 ? "apprentice" : ch->pcdata->tradelevel <=
              10 ? "journeyman" : ch->pcdata->tradelevel <= 19 ? "expert" : ch->pcdata->tradelevel == 20 ? "master" : "reknowned");

      EXTRA_DESCR_DATA       *ed;

      CREATE(ed, EXTRA_DESCR_DATA, 1);

      LINK(ed, item->first_extradesc, item->last_extradesc, next, prev);
      ed->keyword = STRALLOC(item->name);
      ed->description = STRALLOC(extra_buf);

/* 
* Random Affect applier - Code to make crafted items worth it 
*/
      if(ch->pcdata->tradelevel > 4)
      {
// Random affect of armor, str, con, dex, int, wis, cha, lck, hitroll, 
// damroll 
        short                   imbue;

        if(item->item_type == ITEM_ARMOR)
        {
          imbue = number_range(1, 9);
        }
        else if(item->item_type == ITEM_WEAPON)
        {
          imbue = number_range(1, 8);
        }

        short                   bonus = 0;

        if(ch->pcdata->tradelevel < 10)
        {
          bonus = 1;
        }
        else if(ch->pcdata->tradelevel > 9)
        {
          bonus = 2;
        }
        if(item->level < 49)
        {
          bonus += 1;
        }
        else if(item->level > 50 && item->level < 90)
        {
          bonus += 2;
        }
        else if(item->level > 89)
        {
          bonus += 3;
        }

        if(item->item_type == ITEM_ARMOR)
        {
          CREATE(paf, AFFECT_DATA, 2);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_HITROLL;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        if(imbue == 1)
        {
          CREATE(paf, AFFECT_DATA, 2);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_AC;
          paf->modifier = -5;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        else if(imbue == 2)
        {
          CREATE(paf, AFFECT_DATA, 2);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_STR;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        else if(imbue == 3)
        {
          CREATE(paf, AFFECT_DATA, 2);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_CON;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        else if(imbue == 4)
        {
          CREATE(paf, AFFECT_DATA, 2);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_DEX;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        else if(imbue == 5)
        {
          CREATE(paf, AFFECT_DATA, 2);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_INT;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        else if(imbue == 6)
        {
          CREATE(paf, AFFECT_DATA, 2);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_WIS;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        else if(imbue == 7)
        {
          CREATE(paf, AFFECT_DATA, 2);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_CHA;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        else if(imbue == 8)
        {
          CREATE(paf, AFFECT_DATA, 2);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_LCK;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        else if(imbue == 9)
        {
          CREATE(paf, AFFECT_DATA, 2);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_DAMROLL;
          paf->modifier = bonus;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
      }
      if(item->item_type == ITEM_WEAPON)
      {
        short                   qball = 0;

        qball = weapon_bonus(ch->pcdata->tradelevel, item->level);

        CREATE(paf, AFFECT_DATA, 2);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_HITROLL;
        paf->modifier = qball;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);

        CREATE(paf, AFFECT_DATA, 2);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_DAMROLL;
        paf->modifier = qball;
        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);

        if(item->value[4] == 0 || item->value[4] == 4 || item->value[4] == 8)
        {
          short                   bonus;

          bonus = item->level / 5;
          if(bonus < 1)
          {
            bonus = 1;
          }
          item->value[1] = set_min_chart(item->level) + bonus * 3 + set_min_chart(item->level / 2);
          item->value[2] = set_max_chart(item->level) + bonus + (set_max_chart(item->level) / 2);
          item->pIndexData->value[1] = set_min_chart(item->level) + bonus * 3 + set_min_chart(item->level / 2);
          item->pIndexData->value[2] = set_max_chart(item->level) + bonus + (set_max_chart(item->level) / 2);
          item->weight = 15;
        }

        if(ch->pcdata->tradelevel <= 5)
        {
          item->value[0] = 6;
          item->value[1] -= 1;
          item->value[2] -= 1;
          GET_VALUE(item, type) = CURR_COPPER;
          item->cost = 25;
        }

        if(ch->pcdata->tradelevel > 5)
        {
          int                     modifier;

          if(ch->pcdata->tradelevel >= 15)
          {
            if(item->level >= 90)
              modifier = 15;
            else if(item->level >= 70)
              modifier = 8;
            else if(item->level >= 50)
              modifier = 5;
            else if(item->level >= 30)
              modifier = 3;
            else
              modifier = 2;
          }
          else if(ch->pcdata->tradelevel >= 20)
          {
            if(item->level >= 90)
              modifier = 12;
            else if(item->level >= 70)
              modifier = 10;
            else if(item->level >= 50)
              modifier = 8;
            else if(item->level >= 30)
              modifier = 6;
            else
              modifier = 4;
          }
          else
            modifier = 1;

          item->value[0] = 8;

/* 
* Chance to make it better or worser 
*/
          if(number_percent() > 50)
            modifier += number_range(1, 5);
          else if(number_percent() < 50)
            modifier -= number_range(1, 5);
          if(modifier == 0)
            modifier = 1;

          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_HIT;
          paf->modifier = modifier;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
          GET_VALUE(item, type) = CURR_BRONZE;
          item->cost = 10;
        }
        if(ch->pcdata->tradelevel >= 15)
        {
          int                     modifier;

          if(ch->pcdata->tradelevel >= 20)
          {
            if(item->level >= 90)
              modifier = 12;
            else if(item->level >= 70)
              modifier = 10;
            else if(item->level >= 50)
              modifier = 8;
            else if(item->level >= 30)
              modifier = 6;
            else
              modifier = 4;
          }
          else
          {
            if(item->level >= 90)
              modifier = 10;
            else if(item->level >= 70)
              modifier = 8;
            else if(item->level >= 50)
              modifier = 6;
            else if(item->level >= 30)
              modifier = 4;
            else
              modifier = 3;
          }
          item->value[0] = 10;
          item->value[1] += 1;
          item->value[2] += 1;

/* 
* Chance to make it better or worser 
*/
          if(number_percent() > 50)
            modifier += number_range(1, 5);
          else if(number_percent() < 50)
            modifier -= number_range(1, 5);
          if(modifier == 0)
            modifier = 1;

          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_MOVE;
          paf->modifier = modifier;
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
          GET_VALUE(item, type) = CURR_BRONZE;
          item->cost = 25;
        }
        if(ch->pcdata->tradelevel >= 20)
        {
          GET_VALUE(item, type) = CURR_SILVER;
          item->cost = 25;
        }
      }
      else if(item->item_type == ITEM_ARMOR)
      {
        CREATE(paf, AFFECT_DATA, 1);

        paf->type = sn;
        paf->duration = -1;
        paf->location = APPLY_HIT;

        if(ch->pcdata->tradelevel >= 20)
        {
          if(item->level >= 90)
            paf->modifier = 20;
          else if(item->level >= 70)
            paf->modifier = 10;
          else if(item->level >= 50)
            paf->modifier = 8;
          else if(item->level >= 30)
            paf->modifier = 5;
          else
            paf->modifier = 3;
          GET_VALUE(item, type) = CURR_SILVER;
          item->cost = 25;
        }
        else if(ch->pcdata->tradelevel >= 15)
        {
          if(item->level >= 90)
            paf->modifier = 15;
          else if(item->level >= 70)
            paf->modifier = 8;
          else if(item->level >= 50)
            paf->modifier = 5;
          else if(item->level >= 30)
            paf->modifier = 3;
          else
            paf->modifier = 2;
          GET_VALUE(item, type) = CURR_BRONZE;
          item->cost = 25;
        }
        else if(ch->pcdata->tradelevel > 5)
        {
          paf->modifier = 2;
          GET_VALUE(item, type) = CURR_BRONZE;
          item->cost = 10;
        }
        else
        {
          GET_VALUE(item, type) = CURR_COPPER;
          item->cost = 25;
          paf->modifier = 1;
        }

/* 
* Chance to make it better or worser 
*/
        if(number_percent() > 50)
          paf->modifier += number_range(1, 5);
        else if(number_percent() < 50)
          paf->modifier -= number_range(1, 5);
        if(paf->modifier == 0)
          paf->modifier = 1;

        xCLEAR_BITS(paf->bitvector);
        LINK(paf, item->first_affect, item->last_affect, next, prev);

        if(number_percent() > 90)
        {
          CREATE(paf, AFFECT_DATA, 1);

          paf->type = sn;
          paf->duration = -1;
          paf->location = APPLY_AC;
          paf->modifier = number_range(-10, 10);
          xCLEAR_BITS(paf->bitvector);
          LINK(paf, item->first_affect, item->last_affect, next, prev);
        }
        item->value[1] += get_armor_ac_mod(item->level, ch->pcdata->tradelevel);
        item->value[0] = item->value[1];
      }

      {
        int                     ichange = 0;

        if(!str_cmp(arg1, "cedar"))
          ichange = 3;
        else if(!str_cmp(arg1, "pine"))
          ichange = 5;
        else if(!str_cmp(arg1, "walnut"))
          ichange = 8;
        else if(!str_cmp(arg1, "oak"))
          ichange = 10;
        else if(!str_cmp(arg1, "hickory"))
          ichange = 12;

        if(item->item_type == ITEM_WEAPON)
        {
          item->value[0] += ichange;
          item->value[2] += ichange;
        }
        if(item->item_type == ITEM_ARMOR)
        {
          item->value[0] += ichange;
          item->value[1] += ichange;
        }
      }

      if(ch->carry_number + get_obj_number(item) > can_carry_n(ch))
      {
        send_to_char("You can't carry that many items, and drop the lumber into the mill.\r\n", ch);
        separate_obj(item);
        // obj_from_char( item );
        extract_obj(item);
        return;
      }

      if((ch->carry_weight + get_obj_weight(item, FALSE)) > can_carry_w(ch))
      {
        send_to_char("You can't carry that much weight, and drop the lumber into the mill.\r\n", ch);
        separate_obj(item);
        // obj_from_char( item );
        extract_obj(item);
        return;
      }
      if(item)
      {
        obj_to_char(item, ch);
      }

      GET_VALUE(item, type) = CURR_SILVER;

      if(IS_CLANNED(ch))
      {
        CLAN_DATA              *clan;

        clan = ch->pcdata->clan;
        ch->pcdata->clanpoints += 1;
        clan->totalpoints += 1;
        ch_printf(ch, "\r\n&G%s clan has gained a status point from your craftsmanship, now totaling %d clan status points!\r\n", clan->name, clan->totalpoints);
        save_char_obj(ch);
        save_clan(clan);
      }
      return;
    }
    tail_chain();
  }
}
