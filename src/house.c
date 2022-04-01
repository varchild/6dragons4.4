/****************************************************************************
 * [S]imulated [M]edieval [A]dventure multi[U]ser [G]ame      |   \\._.//   *
 * -----------------------------------------------------------|   (0...0)   *
 * SMAUG 1.4 (C) 1994, 1995, 1996, 1998  by Derek Snider      |    ).:.(    *
 * -----------------------------------------------------------|    {o o}    *
 * SMAUG code team: Thoric, Altrag, Blodkai, Narn, Haus,      |   / ' ' \   *
 * Scryn, Rennard, Swordbearer, Gorog, Grishnakh, Nivek,      |~'~.VxvxV.~'~*
 * Tricops and Fireblade                                      |             *
 * ------------------------------------------------------------------------ *
 * Merc 2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael        *
 * Chastain, Michael Quan, and Mitchell Tse.                                *
 * Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          *
 * Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     *
 * ------------------------------------------------------------------------ *
 *                     Housing Module Source File                           *
 ****************************************************************************
 * Author : Senir                                                           *
 * E-Mail : oldgaijin@yahoo.com                                             *
 ****************************************************************************/

#include "h/mud.h"
bool                    in_hash_table(const char *str);

void do_invite(CHAR_DATA *ch, char *argument)
{
  char                    arg[MIL];
  CHAR_DATA              *victim;

  argument = one_argument(argument, arg);

  if((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(victim == ch)
  {
    send_to_char("Why are you trying to invite yourself?\r\n", ch);
    return;
  }

  if(IS_NPC(ch) || IS_NPC(victim))
    return;

  if(ch->pcdata->lair == 0)
    return;

  if(ch->race == RACE_DRAGON)
  {
    ch_printf(ch, "You invite %s to your lair.\r\n", victim->name);
    ch_printf(victim, "%s is wanting to invite you over to their %s lair.\r\n", ch->name, ch->pcdata->htown_name);
    ch_printf(victim, "Type reject to refuse the invitation, otherwise you can now walk to their lair.\r\n");
  }
  else
  {
    ch_printf(ch, "You invite %s to your house.\r\n", victim->name);
    ch_printf(victim, "%s is wanting to invite you over to their %s house.\r\n", ch->name, ch->pcdata->htown_name);
    ch_printf(victim, "Type reject to refuse the invitation, otherwise you can now walk to their house.\r\n");
  }
  victim->pcdata->invite = ch->pcdata->lair;
}

void do_reject(CHAR_DATA *ch, char *argument)
{
  char                    buf[MSL];

  if(IS_NPC(ch))
    return;

  if(ch->pcdata->invite > 0)
  {
    ch->pcdata->invite = 0;
    sprintf(buf, "&C%s has rejected an invite to a house.", ch->name);
    announce(buf);
    return;
  }
}

void do_house(CHAR_DATA *ch, char *argument)
{
  ROOM_INDEX_DATA        *location = NULL;

  switch (ch->substate)
  {
    default:
      break;

    case SUB_ROOM_DESC:
      location = (ROOM_INDEX_DATA *)ch->dest_buf;
      if(!location)
      {
        bug("%s", "redit: sub_room_desc: NULL ch->dest_buf");
        location = ch->in_room;
      }
      if(VLD_STR(location->description))
        STRFREE(location->description);
      location->description = copy_buffer(ch);
      stop_editing(ch);
      ch->substate = ch->tempnum;

      if(xIS_SET(ch->act, PLR_MUSIC))
        send_to_char("!!SOUND(sound/queen.wav)\r\n", ch);
      return;
  }

  if(IS_NPC(ch))
  {
    send_to_char_color("&CMob's don't have houses.\r\n", ch);
    return;
  }

  if(!ch->pcdata->lair)
  {
    send_to_char_color("&CYou don't have a house.\r\n", ch);
    return;
  }

  if(!(location = get_room_index(ch->pcdata->lair)))
  {
    send_to_char("Your house doesn't exist.\r\n", ch);
    return;
  }

  if(ch->in_room != location)
  {
    send_to_char_color("&CThat's not your house.\r\n", ch);
    return;
  }

  ch->tempnum = SUB_NONE;
  ch->substate = SUB_ROOM_DESC;
  ch->dest_buf = location;
  start_editing(ch, location->description);
}
