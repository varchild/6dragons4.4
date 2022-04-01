/*
This could be a disaster or a welcomed change to combat here.  Trying to get it setup so players have a reason to use some of these lower 
level skills in combat and thought maybe if they could use them in combinations that it would help.

Vladaar
*/
#include <string.h>
#include "h/mud.h"

void do_coalesce(CHAR_DATA *ch, char *argument)
{
  char                    arg1[MIL], arg2[MIL], arg3[MIL], arg4[MIL];
  short                   sn;
  SKILLTYPE              *skill;
  char                    buf[MSL];

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  argument = one_argument(argument, arg3);
  argument = one_argument(argument, arg4);

  if(IS_NPC(ch))
    return;

  if(ch->fighting && can_use_skill(ch, number_percent(), gsn_coalesce))
  {

    if(!arg1 || arg1[0] == '\0' && ch->fighting)
    {
      if(ch->mana < 50)
      {
        send_to_char("You don't have enough mana to keep that exhausting attack up.\r\n", ch);
        return;
      }
      short                   meh = number_range(1, 100);

      // perform the attacks
      learn_from_success(ch, gsn_coalesce);
      if(ch->pcdata->coal1 != NULL && ch->pcdata->coal2 != NULL)
      {
        if(meh > 50 && meh < 100)
        {
          ch_printf(ch, "You recite a coalesce of magic spells hitting first with %s and then missing your timing!\r\n", ch->pcdata->coal1);
          snprintf(buf, MSL, "cast %s", ch->pcdata->coal1);
          interpret(ch, buf);
          WAIT_STATE(ch, 5);
          ch->mana -= 25;
          if(ch->mana < 0)
            ch->mana = 0;
          return;
        }
        if(ch->pcdata->coal3 != NULL)
        {
          if(meh > 30 && meh < 51)
          {
            ch_printf(ch, "You recite a coalesce of magic spells hitting first with %s, then following with %s but mispronouncing the final one!\r\n", ch->pcdata->coal1, ch->pcdata->coal2);
            snprintf(buf, MSL, "cast %s", ch->pcdata->coal1);
            interpret(ch, buf);
            snprintf(buf, MSL, "cast %s", ch->pcdata->coal2);
            interpret(ch, buf);
            WAIT_STATE(ch, 10);
            ch->mana -= 30;
            if(ch->mana < 0)
              ch->mana = 0;
            return;
          }
        }
        else
        {
          if(meh > 30 && meh < 51)
          {
            ch_printf(ch, "You recite a coalesce of magic spells hitting first with %s, then following with %s!\r\n", ch->pcdata->coal1, ch->pcdata->coal2);
            snprintf(buf, MSL, "cast %s", ch->pcdata->coal1);
            interpret(ch, buf);
            snprintf(buf, MSL, "cast %s", ch->pcdata->coal2);
            interpret(ch, buf);
            WAIT_STATE(ch, 10);
            ch->mana -= 30;
            if(ch->mana < 0)
              ch->mana = 0;
            return;
          }

        }
        if(meh < 31 && ch->pcdata->coal3 != NULL)
        {
          ch_printf(ch,
                    "You recite a coalesce of magic spells hitting first with %s, then following with %s and finishing it off with %s!\r\n",
                    ch->pcdata->coal1, ch->pcdata->coal2, ch->pcdata->coal3);
          snprintf(buf, MSL, "cast %s", ch->pcdata->coal1);
          interpret(ch, buf);
          snprintf(buf, MSL, "cast %s", ch->pcdata->coal2);
          interpret(ch, buf);
          snprintf(buf, MSL, "cast %s", ch->pcdata->coal3);
          interpret(ch, buf);
          WAIT_STATE(ch, 15);
          ch->mana -= 35;
          if(ch->mana < 0)
            ch->mana = 0;
          return;
        }
      }
    }
  }
  if(ch->fighting)
  {
    ch_printf(ch, "You attempt to recite a coalesce of magic spells, but mess up the timing.\r\n");
    WAIT_STATE(ch, 7);
    learn_from_failure(ch, gsn_coalesce);
    return;
  }

  if(!ch->fighting)
  {
    if(!arg2 || arg2[0] == '\0')
    {
      send_to_char("Syntax: coalesce spell1 spell2 save\r\n", ch);
      send_to_char("Syntax: coalesce spell1 spell2 spell3 save\r\n", ch);
      send_to_char("        coalesce\r\n", ch);
      if(ch->pcdata->coal1 != NULL && ch->pcdata->coal2 != NULL && ch->pcdata->coal3 == NULL)
        ch_printf(ch, "\r\nCurrently set to %s, %s\r\n", ch->pcdata->coal1, ch->pcdata->coal2);
      if(ch->pcdata->coal1 != NULL && ch->pcdata->coal2 != NULL && ch->pcdata->coal3 != NULL)
        ch_printf(ch, "\r\nCurrently set to %s, %s, %s\r\n", ch->pcdata->coal1, ch->pcdata->coal2, ch->pcdata->coal3);
      return;
    }

    if(!str_cmp(arg3, "save"))
    {
      /*
       * sn = skill_lookup( arg1 ); skill = skill_table[sn];
       * 
       * if ( sn < 1 || skill->name == NULL ) { ch_printf( ch, "%s is not a spell you can 
       * use, try your coalesce again.\r\n", arg1 ); return; }
       * 
       * if ( str_cmp( arg1, skill_table[sn]->name ) ) { ch_printf( ch, "%s is not a
       * spell you can use, try your coalesce again.\r\n", arg1 ); return; } sn =
       * skill_lookup( arg2 ); skill = skill_table[sn];
       * 
       * if ( sn < 1 || skill->name == NULL ) { ch_printf( ch, "%s is not a spell you can 
       * use, try your coalesce again.\r\n", arg2 ); return; }
       * 
       * if ( str_cmp( arg2, skill_table[sn]->name ) ) { ch_printf( ch, "%s is not a
       * spell you can use, try your coalesce again.\r\n", arg2 ); return; }
       * 
       * if ( !str_cmp( arg1, arg2 ) ) { send_to_char( "Your coalesce spell1 choice
       * cannot be the same as spell2 choice.\r\n", ch ); return; } 
       */
      if(ch->pcdata->coal1)
        STRFREE(ch->pcdata->coal1);
      ch->pcdata->coal1 = STRALLOC(arg1);
      if(ch->pcdata->coal2)
        STRFREE(ch->pcdata->coal2);
      ch->pcdata->coal2 = STRALLOC(arg2);
      if(ch->pcdata->coal3)
        STRFREE(ch->pcdata->coal3);

      send_to_char("Your new coalesce spell sequence has been saved.\r\n", ch);
      return;

    }
    else
    {
      if(str_cmp(arg4, "save") && arg1[0] != '\0')
      {
        send_to_char("Syntax: coalesce spell1 spell2 save\r\n", ch);
        send_to_char("Syntax: coalesce spell1 spell2 spell3 save\r\n", ch);
        send_to_char("        coalesce\r\n", ch);
        if(ch->pcdata->coal1 != NULL && ch->pcdata->coal2 != NULL && ch->pcdata->coal3 == NULL)
          ch_printf(ch, "\r\nCurrently set to %s, %s\r\n", ch->pcdata->coal1, ch->pcdata->coal2);
        if(ch->pcdata->coal1 != NULL && ch->pcdata->coal2 != NULL && ch->pcdata->coal3 != NULL)
          ch_printf(ch, "\r\nCurrently set to %s, %s, %s\r\n", ch->pcdata->coal1, ch->pcdata->coal2, ch->pcdata->coal3);
        return;
      }

      if(!str_cmp(arg4, "save"))
      {
/*
                sn = skill_lookup( arg1 );
                skill = skill_table[sn];

                if ( sn < 1 || skill->name == NULL ) {
                    ch_printf( ch, "%s is not a spell you can use, try your coalesce again.\r\n",
                               arg1 );
                    return;
                }

                if ( str_cmp( arg1, skill_table[sn]->name ) ) {
                    ch_printf( ch, "%s is not a spell you can use, try your coalesce again.\r\n",
                               arg1 );
                    return;
                }

                sn = skill_lookup( arg2 );
                skill = skill_table[sn];

                if ( sn < 1 || skill->name == NULL ) {
                    ch_printf( ch, "%s is not a spell you can use, try your coalesce again.\r\n",
                               arg2 );
                    return;
                }

                if ( str_cmp( arg2, skill_table[sn]->name ) ) {
                    ch_printf( ch, "%s is not a spell you can use, try your coalesce again.\r\n",
                               arg2 );
                    return;
                }

                sn = skill_lookup( arg3 );
                skill = skill_table[sn];
                if ( arg3 ) {
                    if ( sn < 1 || skill->name == NULL ) {
                        ch_printf( ch, "%s is not a spell you can use, try your coalesce again.\r\n",
                                   arg3 );
                        return;
                    }

                    if ( str_cmp( arg3, skill_table[sn]->name ) ) {
                        ch_printf( ch, "%s is not a spell you can use, try your coalesce again.\r\n",
                                   arg3 );
                        return;
                    }
                }
*/
        if(!str_cmp(arg1, arg2))
        {
          send_to_char("Your coalesce spell1 choice cannot be the same as spell2 choice.\r\n", ch);
          return;
        }
      }
      if(arg3)
      {
        if(!str_cmp(arg1, arg3))
        {
          send_to_char("Your coalesce spell1 choice cannot be the same as spell3 choice.\r\n", ch);
          return;
        }
        if(!str_cmp(arg2, arg3))
        {
          send_to_char("Your coalesce spell2 choice cannot be the same as spell3 choice.\r\n", ch);
          return;
        }
      }
      if(ch->pcdata->coal1)
        STRFREE(ch->pcdata->coal1);
      ch->pcdata->coal1 = STRALLOC(arg1);
      if(ch->pcdata->coal2)
        STRFREE(ch->pcdata->coal2);
      ch->pcdata->coal2 = STRALLOC(arg2);
      if(arg3)
      {
        if(ch->pcdata->coal3)
          STRFREE(ch->pcdata->coal3);
        ch->pcdata->coal3 = STRALLOC(arg3);
      }
      send_to_char("Your new coalesce attack sequence has been saved.\r\n", ch);
      return;
    }
  }
}
