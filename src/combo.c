/*
This could be a disaster or a welcomed change to combat here.  Trying to get it setup so players have a reason to use some of these lower 
level skills in combat and thought maybe if they could use them in combinations that it would help.

08-17-16 happy to report it was welcomed change by players. 

Vladaar
*/
#include <string.h>
#include "h/mud.h"

void do_combo(CHAR_DATA *ch, char *argument)
{
  char                    arg1[MIL], arg2[MIL], arg3[MIL], arg4[MIL];
  short                   sn;
  SKILLTYPE              *skill;

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  argument = one_argument(argument, arg3);
  argument = one_argument(argument, arg4);

  if(IS_NPC(ch))
    return;

  if(ch->fighting && can_use_skill(ch, number_percent(), gsn_combo))
  {

    if(!arg1 || arg1[0] == '\0' && ch->fighting)
    {
      if(ch->move < 10)
      {
        send_to_char("You don't have enough move points to keep that exhausting attack up.\r\n", ch);
        return;
      }
      short                   meh = number_range(1, 100);

      // perform the attacks
      learn_from_success(ch, gsn_combo);
      if(ch->pcdata->combo1 != NULL && ch->pcdata->combo2 != NULL)
      {
        if(meh > 50 && meh < 100)
        {
          ch_printf(ch, "You perform a combo atack hitting first with %s and then missing your timing!\r\n", ch->pcdata->combo1);
          interpret(ch, ch->pcdata->combo1);
          WAIT_STATE(ch, 5);
          ch->move -= 10;
          return;
        }
        if(ch->pcdata->combo3 != NULL)
        {
          if(meh > 30 && meh < 51)
          {
            ch_printf(ch, "You perform a combo atack hitting first with %s, then following with %s but fumbling your final move!\r\n", ch->pcdata->combo1, ch->pcdata->combo2);
            interpret(ch, ch->pcdata->combo1);
            interpret(ch, ch->pcdata->combo2);
            WAIT_STATE(ch, 10);
            ch->move -= 10;
            return;
          }
        }
        else
        {
          if(meh > 30 && meh < 51)
          {
            ch_printf(ch, "You perform a combo atack hitting first with %s, then following with %s!\r\n", ch->pcdata->combo1, ch->pcdata->combo2);
            interpret(ch, ch->pcdata->combo1);
            interpret(ch, ch->pcdata->combo2);
            WAIT_STATE(ch, 10);
            ch->move -= 10;
            return;
          }

        }
        if(meh < 31 && ch->pcdata->combo3 != NULL)
        {
          ch_printf(ch, "You perform a combo atack hitting first with %s, then following with %s and finishing it off with %s!\r\n", ch->pcdata->combo1, ch->pcdata->combo2, ch->pcdata->combo3);
          interpret(ch, ch->pcdata->combo1);
          interpret(ch, ch->pcdata->combo2);
          interpret(ch, ch->pcdata->combo3);
          WAIT_STATE(ch, 15);
          ch->move -= 10;
          return;
        }
      }
    }
  }
  if(ch->fighting)
  {
    ch_printf(ch, "You attempt a combo attack, but mess up the timing.\r\n");
    WAIT_STATE(ch, 7);
    learn_from_failure(ch, gsn_combo);
    return;
  }

  if(!ch->fighting)
  {
    if(!arg2 || arg2[0] == '\0')
    {
      send_to_char("Syntax: combo attack1 attack2 save\r\n", ch);
      send_to_char("Syntax: combo attack1 attack2 attack3 save\r\n", ch);
      send_to_char("        combo\r\n", ch);
      if(ch->pcdata->combo1 != NULL && ch->pcdata->combo2 != NULL && ch->pcdata->combo3 == NULL)
        ch_printf(ch, "\r\nCurrently set to %s, %s\r\n", ch->pcdata->combo1, ch->pcdata->combo2);
      if(ch->pcdata->combo1 != NULL && ch->pcdata->combo2 != NULL && ch->pcdata->combo3 != NULL)
        ch_printf(ch, "\r\nCurrently set to %s, %s, %s\r\n", ch->pcdata->combo1, ch->pcdata->combo2, ch->pcdata->combo3);
      return;
    }

    if(!str_cmp(arg3, "save"))
    {
/*
            sn = skill_lookup( arg1 );
            skill = skill_table[sn];

            if ( sn < 1 || skill->name == NULL ) {
                ch_printf( ch, "%s is not a skill you can use, try your combo again.\r\n", arg1 );
                return;
            }

            if ( str_cmp( arg1, skill_table[sn]->name ) ) {
                ch_printf( ch, "%s is not a skill you can use, try your combo again.\r\n", arg1 );
                return;
            }
            sn = skill_lookup( arg2 );
            skill = skill_table[sn];

            if ( sn < 1 || skill->name == NULL ) {
                ch_printf( ch, "%s is not a skill you can use, try your combo again.\r\n", arg2 );
                return;
            }

            if ( str_cmp( arg2, skill_table[sn]->name ) ) {
                ch_printf( ch, "%s is not a skill you can use, try your combo again.\r\n", arg2 );
                return;
            }
*/
      if(!str_cmp(arg1, arg2))
      {
        send_to_char("Your combo attack1 choice cannot be the same as attack2 choice.\r\n", ch);
        return;
      }
      if(ch->pcdata->combo1)
        STRFREE(ch->pcdata->combo1);
      ch->pcdata->combo1 = STRALLOC(arg1);
      if(ch->pcdata->combo2)
        STRFREE(ch->pcdata->combo2);
      ch->pcdata->combo2 = STRALLOC(arg2);
      if(ch->pcdata->combo3)
        STRFREE(ch->pcdata->combo3);
      send_to_char("Your new combo attack sequence has been saved.\r\n", ch);
      return;

    }
    else
    {
      if(str_cmp(arg4, "save") && arg1[0] != '\0')
      {
        send_to_char("Syntax: combo attack1 attack2 save\r\n", ch);
        send_to_char("Syntax: combo attack1 attack2 attack3 save\r\n", ch);
        send_to_char("        combo\r\n", ch);
        if(ch->pcdata->combo1 != NULL && ch->pcdata->combo2 != NULL && ch->pcdata->combo3 == NULL)
          ch_printf(ch, "\r\nCurrently set to %s, %s\r\n", ch->pcdata->combo1, ch->pcdata->combo2);
        if(ch->pcdata->combo1 != NULL && ch->pcdata->combo2 != NULL && ch->pcdata->combo3 != NULL)
          ch_printf(ch, "\r\nCurrently set to %s, %s, %s\r\n", ch->pcdata->combo1, ch->pcdata->combo2, ch->pcdata->combo3);
        return;
      }

      if(!str_cmp(arg4, "save"))
      {
/*
                sn = skill_lookup( arg1 );
                skill = skill_table[sn];

                if ( sn < 1 || skill->name == NULL ) {
                    ch_printf( ch, "%s is not a skill you can use, try your combo again.\r\n",
                               arg1 );
                    return;
                }

                if ( str_cmp( arg1, skill_table[sn]->name ) ) {
                    ch_printf( ch, "%s is not a skill you can use, try your combo again.\r\n",
                               arg1 );
                    return;
                }

                sn = skill_lookup( arg2 );
                skill = skill_table[sn];

                if ( sn < 1 || skill->name == NULL ) {
                    ch_printf( ch, "%s is not a skill you can use, try your combo again.\r\n",
                               arg2 );
                    return;
                }

                if ( str_cmp( arg2, skill_table[sn]->name ) ) {
                    ch_printf( ch, "%s is not a skill you can use, try your combo again.\r\n",
                               arg2 );
                    return;
                }

                sn = skill_lookup( arg3 );
                skill = skill_table[sn];
                if ( arg3 ) {
                    if ( sn < 1 || skill->name == NULL ) {
                        ch_printf( ch, "%s is not a skill you can use, try your combo again.\r\n",
                                   arg3 );
                        return;
                    }

                    if ( str_cmp( arg3, skill_table[sn]->name ) ) {
                        ch_printf( ch, "%s is not a skill you can use, try your combo again.\r\n",
                                   arg3 );
                        return;
                    }
                }
*/
        if(!str_cmp(arg1, arg2))
        {
          send_to_char("Your combo attack1 choice cannot be the same as attack2 choice.\r\n", ch);
          return;
        }
      }
      if(arg3)
      {
        if(!str_cmp(arg1, arg3))
        {
          send_to_char("Your combo attack1 choice cannot be the same as attack3 choice.\r\n", ch);
          return;
        }
        if(!str_cmp(arg2, arg3))
        {
          send_to_char("Your combo attack2 choice cannot be the same as attack3 choice.\r\n", ch);
          return;
        }
      }
      if(ch->pcdata->combo1)
        STRFREE(ch->pcdata->combo1);
      ch->pcdata->combo1 = STRALLOC(arg1);
      if(ch->pcdata->combo2)
        STRFREE(ch->pcdata->combo2);
      ch->pcdata->combo2 = STRALLOC(arg2);
      if(arg3)
      {
        if(ch->pcdata->combo3)
          STRFREE(ch->pcdata->combo3);
        ch->pcdata->combo3 = STRALLOC(arg3);
      }
      send_to_char("Your new combo attack sequence has been saved.\r\n", ch);
      return;
    }
  }
}
