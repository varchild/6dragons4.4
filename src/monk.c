/****************************************************************************
*Status: Incomplete.  Est: 95%                                              *
*                                                                           *
*Author: Dustan Gunn "Taon"                                                 *
*Author's Email: Itztaon@aol.com                                            *
*Code Information: The home of most of the monk class skills.               *
*Other: This was wrote for use on 6dragons mud. This isn't the final        *
*header, simply temporary until I complete it.                              *
*                                                                           * 
*****************************************************************************
*                        MONK CLASS MODULE                                  *
****************************************************************************/

#include "h/mud.h"
#include "h/polymorph.h"

void do_grapple(CHAR_DATA *ch, char *argument)
{
  if(IS_NPC(ch))
    return;

  if(ch->Class != CLASS_MONK)
    return;

  if(ch->martialarts == 0)
  {
    ch->martialarts = 1;
    if(ch->fighting)
    {
      WAIT_STATE(ch, skill_table[gsn_grapple]->beats);
      act(AT_PLAIN, "You move closer and switch your martial arts to grappling techniques.", ch, NULL, NULL, TO_CHAR);
      return;
    }
    act(AT_PLAIN, "You switch your martial arts to grappling techniques.", ch, NULL, NULL, TO_CHAR);
    return;
  }
  if(ch->martialarts == 1)
  {
    ch->martialarts = 0;
    if(ch->fighting)
    {
      WAIT_STATE(ch, skill_table[gsn_grapple]->beats);
      act(AT_PLAIN, "You back away some distance and switch your grappling techniques to martial arts.", ch, NULL, NULL, TO_CHAR);
      return;
    }
    act(AT_PLAIN, "You switch your grappling techniques to martial arts.", ch, NULL, NULL, TO_CHAR);
    return;
  }

}

void do_courage(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;
  CHAR_DATA              *victim;
  char                    arg[MIL];

  argument = one_argument(argument, arg);

  if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(arg[0] == '\0')
  {
    victim = ch;
  }

  if(IS_NPC(ch) && IS_AFFECTED(ch, AFF_CHARM))
  {
    send_to_char("You can't concentrate enough for that.\r\n", ch);
    return;
  }

  if(IS_AFFECTED(victim, AFF_COURAGE))
  {
    send_to_char("You already called on the courage of your ancestors.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_courage))
  {
    af.type = gsn_courage;
    af.duration = ch->level * 2;
    af.level = ch->level;
    af.location = APPLY_HITROLL;
    af.modifier = 1 + ch->level / 20;
    xCLEAR_BITS(af.bitvector);
    affect_to_char(victim, &af);

    af.type = gsn_courage;
    af.duration = ch->level * 2;
    af.level = ch->level;
    af.location = APPLY_DAMROLL;
    af.modifier = 1 + ch->level / 20;
    xCLEAR_BITS(af.bitvector);
    affect_to_char(victim, &af);

    af.type = gsn_courage;
    af.duration = ch->level * 2;
    af.level = ch->level;
    af.location = APPLY_STR;
    af.modifier = (get_curr_wis(ch) / 4); // -5 to 8 with max
    af.bitvector = meb(AFF_COURAGE);
    affect_to_char(victim, &af);

    learn_from_success(ch, gsn_courage);
    act(AT_PLAIN, "You call on the courage of your ancestors!", ch, NULL, NULL, TO_CHAR);
    act(AT_PLAIN, "A steadiness comes over $N as courage washes over them.", ch, NULL, victim, TO_ROOM);
    return;
  }
  else
    learn_from_failure(ch, gsn_courage);
  send_to_char("You failed to call upon courage of your ancestors.\r\n", ch);

}


void do_inner_strength(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;

  if(IS_NPC(ch) && IS_AFFECTED(ch, AFF_CHARM))
  {
    send_to_char("You can't concentrate enough for that.\r\n", ch);
    return;
  }

  if(IS_AFFECTED(ch, AFF_INNER_STRENGTH))
  {
    send_to_char("You already called on your inner strength.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_inner_strength))
  {

    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/innerstrength.wav)\r\n", ch);

    af.type = gsn_inner_strength;
    af.duration = ch->level * 2;
    af.level = ch->level;
    af.location = APPLY_HITROLL;
    af.modifier = 1 + ch->level / 20;
    xCLEAR_BITS(af.bitvector);
    affect_to_char(ch, &af);

    af.type = gsn_inner_strength;
    af.duration = ch->level * 2;
    af.level = ch->level;
    af.location = APPLY_DAMROLL;
    af.modifier = 1 + ch->level / 20;
    xCLEAR_BITS(af.bitvector);
    affect_to_char(ch, &af);

    af.type = gsn_inner_strength;
    af.duration = ch->level * 2;
    af.level = ch->level;
    af.location = APPLY_STR;
    af.modifier = (get_curr_wis(ch) / 4); // -5 to 8 with max 
    af.bitvector = meb(AFF_INNER_STRENGTH);
    affect_to_char(ch, &af);

    learn_from_success(ch, gsn_inner_strength);
    act(AT_PLAIN, "Fierce determination swells within you as you call on your inner strength!", ch, NULL, NULL, TO_CHAR);
    act(AT_PLAIN, "A look of fierce determination comes over $n.", ch, NULL, NULL, TO_NOTVICT);
    return;
  }
  else
    learn_from_failure(ch, gsn_inner_strength);
  send_to_char("You failed to call upon your inner strength.\r\n", ch);
}

//Was having issues with the smaug_spell version of it, so I just
//wrote the skill. Simple enough. -Taon
//Status: Completed skill
void do_keen_eye(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;

  if(IS_NPC(ch) && !xIS_SET(ch->act, ACT_BEASTMELD))
    return;
  if(IS_AFFECTED(ch, AFF_CHARM) && !xIS_SET(ch->act, ACT_BEASTMELD))
    return;

  if(!xIS_SET(ch->act, ACT_BEASTMELD))
  {
    if(ch->pcdata->learned[gsn_keen_eye] <= 0)
    {
      send_to_char("You wouldn't know where to start!\r\n", ch);
      return;
    }
  }
  if(ch->position != POS_STANDING)
  {
    send_to_char("You must be standing in order to use this.\r\n", ch);
    return;
  }
  if(ch->move < 25)
  {
    send_to_char("You dont have enough energy.\r\n", ch);
    return;
  }
  if(IS_AFFECTED(ch, AFF_KEEN_EYE))
  {
    send_to_char("Your eyes already see unseen things.\r\n", ch);
    return;
  }

  WAIT_STATE(ch, skill_table[gsn_keen_eye]->beats);
  if(can_use_skill(ch, number_percent(), gsn_keen_eye))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/keen.wav)\r\n", ch);

    af.type = gsn_keen_eye;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.duration = ch->level * 5;
    af.bitvector = meb(AFF_DETECT_HIDDEN);
    af.level = ch->level;
    affect_to_char(ch, &af);

    af.type = gsn_keen_eye;
    af.location = APPLY_HITROLL;
    af.modifier = get_curr_str(ch) / 5;
    af.duration = ch->level * 5;
    af.bitvector = meb(AFF_KEEN_EYE);
    af.level = ch->level;
    affect_to_char(ch, &af);
    send_to_char("&cYour keen eyes take in things others would not notice.\r\n", ch);
    ch->move -= 25;
    learn_from_success(ch, gsn_keen_eye);
    return;
  }
  else
    learn_from_failure(ch, gsn_keen_eye);
  send_to_char("&cYour vision blurs, as you attempt to use your keen eyes.\r\n", ch);
  return;
}

//Status: complete. -Taon
//Recently converted so that Dragons can use it as Iron Scales as-well. -Taon
void do_iron_skin(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;
  short                   chance;

  if(IS_NPC(ch))
    return;
  if(IS_AFFECTED(ch, AFF_CHARM))
  {
    send_to_char("But you're charmed...\r\n", ch);
    return;
  }
  if(IS_AFFECTED(ch, AFF_IRON_SKIN))
  {
    send_to_char("You're already as tough as iron.\r\n", ch);
    return;
  }
  if(ch->position == POS_FIGHTING)
  {
    send_to_char("You can't perform such a task while in combat.", ch);
    return;
  }
  if(ch->move < 40 - get_curr_dex(ch))
  {
    send_to_char("You're too tired to do such a thing.\r\n", ch);
    return;
  }

  chance = (get_curr_dex(ch) / 2) + ch->pcdata->learned[gsn_iron_skin] + number_range(1, 30);

  if(ch->pcdata->learned[gsn_iron_skin] > 0)
  {
    ch->move -= 40 - get_curr_dex(ch);

    if(chance >= number_range(50, 100))
    {
      if(ch->race != RACE_DRAGON)
        send_to_char("Your skin turns as hard as iron.\r\n", ch);
      else
        send_to_char("Your scales turns as hard as iron.\r\n", ch);

      af.type = gsn_iron_skin;

      if(ch->level < 30)
        af.duration = (ch->level * 3) + (get_curr_wis(ch) * 5);
      else
        af.duration = (ch->level * 4) + (get_curr_wis(ch) * 6);

      af.level = ch->level;
      af.location = APPLY_AC;

      if(ch->level < 25)
        af.modifier = -25;
      else if(ch->level < 50)
        af.modifier = -35;
      else if(ch->level < 75)
        af.modifier = -45;
      else
        af.modifier = -55;

      af.bitvector = meb(AFF_IRON_SKIN);
      affect_to_char(ch, &af);

      af.type = gsn_iron_skin;

      if(ch->level < 30)
        af.duration = (ch->level * 3) + (get_curr_wis(ch) * 5);
      else
        af.duration = (ch->level * 4) + (get_curr_wis(ch) * 6);

      af.level = ch->level;
      af.location = APPLY_DEX;

      if(ch->pcdata->learned[gsn_iron_skin] < 30)
        af.modifier = -4;
      else if(ch->pcdata->learned[gsn_iron_skin] < 50)
        af.modifier = -3;
      else if(ch->pcdata->learned[gsn_iron_skin] < 85)
        af.modifier = -2;
      else
        af.modifier = -1;

      af.bitvector = meb(AFF_IRON_SKIN);
      affect_to_char(ch, &af);

      af.type = gsn_iron_skin;

      af.location = APPLY_DAMROLL;

      if(ch->level < 50)
        af.modifier = 3;
      else
        af.modifier = 5;

      if(ch->level < 30)
        af.duration = (ch->level * 3) + (get_curr_wis(ch) * 5);
      else
        af.duration = (ch->level * 4) + (get_curr_wis(ch) * 6);

      af.level = ch->level;
      af.bitvector = meb(AFF_IRON_SKIN);
      affect_to_char(ch, &af);

      learn_from_success(ch, gsn_iron_skin);

      return;
    }
    else
    {
      send_to_char("You've failed to turn yourself as hard as iron.\r\n", ch);

      learn_from_failure(ch, gsn_iron_skin);

      return;
    }
  }
  else
  {
    send_to_char("You wouldn't know where to start.\r\n", ch);
    return;
  }
}

//Status: Just started.... -Taon
//Notes: Will be used to help monks fend off hunger. -Taon
void do_sustain_self(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;

  if(IS_NPC(ch))
    return;
  if(IS_AFFECTED(ch, AFF_CHARM))
    return;
  if(ch->move < 50 - get_curr_dex(ch))
  {
    send_to_char("You dont have enough energy to accomplish such a feat.", ch);
    return;
  }

  if(ch->pcdata->learned[gsn_sustain_self] > 0)
  {
    ch->move -= 50 - get_curr_dex(ch);

    if(!IS_AFFECTED(ch, AFF_SUSTAIN_SELF))
    {
      send_to_char("You urge your body to fight off hunger.\r\n", ch);
      learn_from_success(ch, gsn_sustain_self);
      af.type = gsn_sustain_self;
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.duration = -1;
      af.bitvector = meb(AFF_SUSTAIN_SELF);
      af.level = ch->level;
      affect_to_char(ch, &af);
      return;
    }
    else
    {
      send_to_char("Your body will no longer fight off hunger.\r\n", ch);
      xREMOVE_BIT(ch->affected_by, AFF_SUSTAIN_SELF);
      return;
    }
  }
  else
  {
    send_to_char("You wouldn't have a clue on how to do such a thing.\r\n", ch);
    return;
  }
}

//New daze skill for monks complete. -Taon
//Msg: Also used by shadowknights.

void do_daze(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA              *victim;
  AFFECT_DATA             af;
  short                   chance;

  chance = number_range(1, 100);
  if(IS_NPC(ch))
    return;
  if(IS_AFFECTED(ch, AFF_CHARM))
    return;

  if(ch->pcdata->learned[gsn_daze] <= 0)
  {
    send_to_char("You dont know this skill.\r\n", ch);
    return;
  }
  if(!ch->fighting)
  {
    send_to_char("But you're not fighting anyone!\r\n", ch);
    return;
  }
  if(ch->move < 35 - get_curr_dex(ch))
  {
    send_to_char("You don't have enough energy.\r\n", ch);
    return;
  }

  victim = who_fighting(ch);

  if(!IS_NPC(ch) && IS_AFFECTED(victim, AFF_SLOW))
  {
    send_to_char("They're already slow enough.\r\n", ch);
    return;
  }

  if(!IS_NPC(victim) && (get_curr_dex(victim) - get_curr_dex(ch) >= 2) && chance < 95)
  {
    act(AT_WHITE, "You try a daze technique on $N, but $E deftly sidesteps it.", ch, NULL, victim, TO_CHAR);
    WAIT_STATE(ch, skill_table[gsn_daze]->beats);
    return;
  }

  if(IS_NPC(ch) && (victim->level - ch->level > 9) && chance < 95)
  {
    act(AT_WHITE, "You try a daze technique on $N, but $E is too powerful for such a technique.", ch, NULL, victim, TO_CHAR);
    WAIT_STATE(ch, skill_table[gsn_daze]->beats);
    return;
  }
  if(!IS_NPC(ch) && victim->level - ch->level > 4 && chance < 95)
  {
    act(AT_WHITE, "You try a daze technique on $N, but $E is too powerful for such a technique.", ch, NULL, victim, TO_CHAR);
    WAIT_STATE(ch, skill_table[gsn_daze]->beats);
    return;
  }

  if(chance < 15)
  {
    act(AT_WHITE, "You try a daze technique on $N, but $E sees it coming.", ch, NULL, victim, TO_CHAR);
    WAIT_STATE(ch, skill_table[gsn_daze]->beats);
    return;
  }

  if(can_use_skill(ch, (number_percent()), gsn_daze))
  {
    WAIT_STATE(ch, 2 * (PULSE_VIOLENCE / 2));
    WAIT_STATE(victim, 3 * (PULSE_VIOLENCE / 2));
    af.type = gsn_daze;
    af.location = APPLY_STR;

    if(get_curr_str(ch) < 12)
      af.modifier = -1;
    else if(get_curr_str(ch) < 16)
      af.modifier = -2;
    else if(get_curr_str(ch) < 20)
      af.modifier = -3;
    else
      af.modifier = -4;

    af.level = ch->level;
    af.duration = ch->level / 2;
    af.bitvector = meb(AFF_SLOW);
    affect_to_char(victim, &af);
    learn_from_success(ch, gsn_daze);
    ch->move -= 35 - get_curr_dex(ch);
    ch_printf(ch, "&cYou charge into %s leaving them dazed.\r\n", victim->name);
    ch_printf(victim, "&c%s charges into you, leaving you dazed.\r\n", ch->name);
  }
  else
  {
    WAIT_STATE(ch, skill_table[gsn_daze]->beats);
    send_to_char("You fail to properly daze your opponet!\r\n", ch);
    learn_from_failure(ch, gsn_daze);
  }
  return;
}

//Status: Shieldblock Code installed... -Taon

//This is complete though I intend on giving a good
//chance of causing damage to the shield when this is
//invoked. 

//Status: Installed. -Taon

bool check_phase(CHAR_DATA *ch, CHAR_DATA *victim)
{
  int                     dice;

  if(IS_NPC(victim))
    return FALSE;
  if(!IS_AWAKE(victim))
    return FALSE;
  if(victim->move < 2 || victim->mana < 2)
    return FALSE;
  if(number_chance(1, 3) == 1)
    return FALSE;

  if(!IS_AFFECTED(victim, AFF_PHASE))
  {
    return FALSE;
  }

  if(can_use_skill(victim, number_percent(), gsn_phase))
  {
    dice = (int)(LEARNED(victim, gsn_phase) / sysdata.phase_mod);

    if(get_curr_dex(victim) >= get_curr_dex(ch) + 5)
      dice += get_curr_dex(victim) - get_curr_dex(ch) + 5;
    else
      dice += get_curr_dex(victim) - get_curr_dex(ch);

    if((IS_AFFECTED(victim, AFF_BLINDNESS) && !IS_AFFECTED(victim, AFF_NOSIGHT)) || victim->position == POS_SITTING || victim->position == POS_RESTING)
      dice /= 2;

    if(!chance(victim, dice + victim->level - ch->level))
    {
      learn_from_failure(victim, gsn_phase);
      return FALSE;
    }

    act(AT_DGREEN, "Your body phases, absorbing $n's attack.", ch, NULL, victim, TO_VICT);
    act(AT_CYAN, "$N's body phases, absorbing your attack.", ch, NULL, victim, TO_CHAR);
    victim->move -= 2;
    victim->mana -= 2;
    learn_from_success(victim, gsn_phase);

    if(number_chance(1, 5) > 4)
      return TRUE;
  }
  else
    return FALSE;
}

//Status: Completed skill, also used by priests/angels. -Taon
void do_minor_restore(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA              *victim;
  char                    arg[MIL];
  int                     heal_rate, heal_xp = 0;

  if(IS_NPC(ch))
    return;

  if(ch->mana < 30)
  {
    send_to_char("You don't have enough mana to accomplish this task.\r\n", ch);
    return;
  }

  if(IS_AFFECTED(ch, AFF_CHARM))
  {
    send_to_char("You can't concentrate on such a task right now.\r\n", ch);
    return;
  }

  argument = one_argument(argument, arg);

  if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(arg[0] == '\0')
  {
    victim = ch;
  }

  if(victim == who_fighting(ch))
  {
    send_to_char("But you're fighting them....\r\n", ch);
    return;
  }
  if(victim->hit >= victim->max_hit)
  {
    send_to_char("This one is already at max health.\r\n", ch);
    return;
  }

  heal_rate = (ch->level * 5);

  if(ch->level > 39 && ch->level < 70)
    heal_rate = ch->level * 6;

  if(ch->level > 69 && ch->level < 80)
    heal_rate = ch->level * 8;

  if(ch->level > 79)
    heal_rate = ch->level * 10;

  heal_rate += number_range(get_curr_wis(ch), (get_curr_wis(ch) * 5));

  if(ch->Class == CLASS_ANGEL)
  { // bonus or negative based on alignment
        short bonus = 0;
        if ( calculate_age(ch) > 19 && calculate_age(ch) < 30 )
        {
        bonus = 5;
        }
        if ( calculate_age(ch) > 29 && calculate_age(ch) < 40 )
        {
        bonus = 10;
        }
        if ( calculate_age(ch) > 39 )
        {
        bonus = 15;
        }
       heal_rate += bonus;
  }
    

  if(!ch->fighting)
    heal_rate *= 2;

  gain_exp(ch, heal_xp);

  if(heal_rate > victim->max_hit)
    heal_rate = victim->max_hit;

  if(victim->hit > victim->max_hit)
  {
    if(victim->hit - victim->max_hit > 1000)
      victim->hit = victim->max_hit + 1000; // capped this bonus
  }

  WAIT_STATE(ch, skill_table[gsn_minor_restore]->beats);
  if(!ch->fighting)
    WAIT_STATE(ch, 5);
  if(can_use_skill(ch, number_percent(), gsn_minor_restore))
  {
    ch->mana -= 30;
    if(!ch->fighting)
      ch->mana -= 20;

    if(ch->fighting)
    {
      if(victim == ch)
        ch_printf(ch, "&cYou begin to perform a minor restore.\r\n");
      else
        ch_printf(ch, "&cYou perform a minor restore on %s\r\n", victim->name);
    }
    if(!ch->fighting)
    {
      if(victim == ch)
        ch_printf(ch, "&cYou take your time and perform a minor restore.\r\n");
      else
        ch_printf(ch, "&cYou take your time and perform a minor restore on %s\r\n", victim->name);
    }

    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/minor.wav)\r\n", ch);
    if(ch->fighting)
    {
      if(victim != ch)
        ch_printf(victim, "%s performs a minor restore, filling you with life.\r\n", ch->name);
      act(AT_MAGIC, "$n performs a minor restore on $N.", ch, NULL, victim, TO_ROOM);
    }
    else
    {
      if(victim != ch)
        ch_printf(victim, "%s taking the proper time performs a minor restore, filling you with life.\r\n", ch->name);
      act(AT_MAGIC, "$n takes the proper time and performs a minor restore on $N.", ch, NULL, victim, TO_ROOM);
    }
    victim->hit += heal_rate;
    learn_from_success(ch, gsn_minor_restore);
  }
  else
  {
    send_to_char("You've failed to properly perform a minor restore.\r\n", ch);
    learn_from_failure(ch, gsn_minor_restore);
  }
  return;
}

//Status: Completed skill. -Taon
void do_untangle(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA              *victim;
  char                    arg[MIL];

  argument = one_argument(argument, arg);

  if(IS_NPC(ch))
    return;

  if(arg[0] == '\0')
  {
    send_to_char("Untangle who???\r\n", ch);
    return;
  }
  if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("Your target is nowhere to be found.\r\n", ch);
    return;
  }

  if(!IS_AFFECTED(victim, AFF_SNARE) && !IS_AFFECTED(victim, AFF_TANGLE))
  {
    send_to_char("But they're not ensnared or tangled!\r\n", ch);
    return;
  }

  if(victim == ch)
  {
    send_to_char("You can't untangle yourself.\r\n", ch);
    return;
  }
  if(ch->move < 20)
  {
    send_to_char("You don't have enough energy to do such a thing.\r\n", ch);
    return;
  }

  WAIT_STATE(ch, skill_table[gsn_untangle]->beats);
  if(can_use_skill(ch, number_percent(), gsn_untangle))
  {
    if(IS_NPC(victim))
      ch_printf(ch, "You untangle %s\r\n", victim->short_descr);
    else
      ch_printf(ch, "You untangle %s\r\n", victim->name);

    ch_printf(victim, "You're untangled by %s\r\n ", ch->name);
    affect_strip(victim, gsn_ensnare);
    xREMOVE_BIT(victim->affected_by, AFF_SNARE);
    affect_strip(victim, gsn_vines);
    xREMOVE_BIT(victim->affected_by, AFF_TANGLE);
    ch->move -= 20;
    learn_from_success(ch, gsn_untangle);
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/tangle.wav)\r\n", ch);

  }
  else
  {
    send_to_char("You've failed to untangle your target!\r\n", ch);
    learn_from_failure(ch, gsn_untangle);
  }
  return;
}

/* Idea behind this is a monk or assassin can ensnare their target,
   leaving them unable to leave that room, until snare wears off. -Taon
   STATUS: 90% complete, will complete tomorrow night.
*/

void do_ensnare(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;
  CHAR_DATA              *victim;
  char                    arg[MIL];
  int                     chance;

  argument = one_argument(argument, arg);

  if(IS_NPC(ch))
    return;

  if(ch->pcdata->learned[gsn_ensnare] < 0)
  {
    send_to_char("You wouldn't know where to begin!\r\n", ch);
    return;
  }
  if(arg[0] == '\0')
  {
    send_to_char("You must provide a target to ensnare.\r\n", ch);
    return;
  }
  if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("Your target is nowhere to be found!\r\n", ch);
    return;
  }
  if(victim == ch)
  {
    send_to_char("Ensnare yourself, eh?", ch);
    return;
  }
  if(ch->move < 25)
  {
    send_to_char("You dont have enough movement!\r\n", ch);
    return;
  }
  if(victim->mount)
  {
    send_to_char("You can't snare them while they're mounted.\r\n", ch);
    return;
  }

  chance = number_chance(1, 25) + (ch->pcdata->learned[gsn_ensnare] / 2) + (ch->level - victim->level) + ((get_curr_dex(ch) - get_curr_dex(victim)) * 2);

  if(victim->position == POS_SITTING || victim->position == POS_RESTING || !IS_AWAKE(ch))
    chance += 20;
  if(IS_IMMORTAL(ch))
    chance = 101;

  if(chance > 100 || number_chance(1, 10) == 2)
  {

    af.type = gsn_ensnare;
    af.location = APPLY_NONE;
    af.modifier = 0;

    if(ch->level < 20)
      af.duration = 30 - get_curr_str(victim);
    else if(ch->level < 40)
      af.duration = 40 - get_curr_str(victim);
    else if(ch->level < 60)
      af.duration = 50 - get_curr_str(victim);
    else if(ch->level < 80)
      af.duration = 60 - get_curr_str(victim);
    else
      af.duration = 70 - get_curr_str(victim);

    af.bitvector = meb(AFF_SNARE);
    af.level = ch->level;
    affect_to_char(victim, &af);

    if(IS_NPC(victim))
      ch_printf(ch, "You've ensnared %s.\r\n", victim->short_descr);
    else
      ch_printf(ch, "You've ensnared %s.\r\n", victim->name);

    ch->move -= 25;
    ch_printf(victim, "You've been ensnared by %s.\r\n", ch->name);
    learn_from_success(ch, gsn_ensnare);
  }
  else
  {
    send_to_char("You've failed to snare your target.\r\n", ch);
    learn_from_failure(ch, gsn_ensnare);
  }
  return;
}

/* Status: Completed skill. -Taon */
void do_recoil(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;
  short                   chance;

  if(IS_NPC(ch))
    return;
  if(IS_AFFECTED(ch, AFF_CHARM))
  {
    send_to_char("But you're charmed.\r\n", ch);
    return;
  }
  if(ch->pcdata->learned[gsn_recoil] <= 0)
  {
    send_to_char("You wouldn't know where to start.\r\n", ch);
    return;
  }
  if(ch->move < 35 - get_curr_dex(ch))
  {
    send_to_char("You don't have enough energy to do such a task.\r\n", ch);
    return;
  }
  if(IS_AFFECTED(ch, AFF_RECOIL))
  {
    send_to_char("You're already recoiled.\r\n", ch);
    return;
  }

  chance = get_curr_dex(ch) + get_curr_str(ch) + (ch->pcdata->learned[gsn_recoil] / 2);

  if(IS_IMMORTAL(ch))
    chance = 101;

  if(chance > number_chance(60, 100))
  {
    WAIT_STATE(ch, skill_table[gsn_recoil]->beats);
    af.type = gsn_recoil;
    af.location = APPLY_DAMROLL;
    af.duration = ch->level * (get_curr_wis(ch) / 4);
    af.modifier = get_curr_str(ch) / 10;
    af.level = ch->level;
    af.bitvector = meb(AFF_RECOIL);
    affect_to_char(ch, &af);

    af.type = gsn_recoil;
    af.location = APPLY_STR;
    af.duration = ch->level * (get_curr_wis(ch) / 4);

    if(ch->level < 50)
      af.modifier = 2;
    else if(ch->level < 75)
      af.modifier = 3;
    else if(ch->level < 90)
      af.modifier = 4;
    else
      af.modifier = 5;

    af.level = ch->level;
    af.bitvector = meb(AFF_RECOIL);
    affect_to_char(ch, &af);
    send_to_char("You recoil your posture for maximum strength.\r\n", ch);
    ch->move -= 35 - get_curr_dex(ch);
    learn_from_success(ch, gsn_recoil);
    return;
  }
  else
  {
    WAIT_STATE(ch, skill_table[gsn_recoil]->beats);
    send_to_char("You failed to properly recoil your body.\r\n", ch);
    learn_from_failure(ch, gsn_recoil);
    return;
  }
}

//Autoskill for monks. -Taon
//Note: Also given to several other classes.
bool check_displacement(CHAR_DATA *ch, CHAR_DATA *victim)
{
  int                     chance;

  if(IS_NPC(victim))
    return FALSE;

  if(!IS_AWAKE(victim))
    return FALSE;

  if(number_chance(1, 7) < 4)
    return FALSE;

  if(victim->pcdata->learned[gsn_displacement] <= 0)
    return FALSE;

  chance = (victim->pcdata->learned[gsn_displacement] / sysdata.displacement_mod) + get_curr_dex(victim) + (victim->level - ch->level) + (get_curr_lck(victim) / 3);

  if(IS_AFFECTED(victim, AFF_SLOW))
    chance -= 10;

  if(chance > number_chance(20, 60))
  {
    act(AT_DGREEN, "$n hits your displaced image.", ch, NULL, victim, TO_VICT);
    act(AT_CYAN, "You hit $N's displaced image.", ch, NULL, victim, TO_CHAR);

    if(number_chance(0, 5) > 4)
      learn_from_success(victim, gsn_displacement);

    return TRUE;
  }

  learn_from_failure(victim, gsn_displacement);
  return FALSE;
}

void do_disguise(CHAR_DATA *ch, char *argument)
{
  MORPH_DATA             *morph;
  AFFECT_DATA             af;
  char                    arg[MSL];
  short                   percent;
  bool                    fForm = FALSE;

  CHAR_DATA              *victim = NULL;

  if(!ch)
  {
    bug("%s", "do_disguise: NULL ch!");
    return;
  }

  if(IS_NPC(ch))
    return;

  if(ch->fighting)
  {
    send_to_char("Not while you're still fighting!\r\n", ch);
    return;
  }

  if(IS_AFFECTED(ch, AFF_DISGUISE))
  {
    send_to_char("You are already disguised!\r\n", ch);
    return;
  }

  if(global_retcode == rCHAR_DIED || global_retcode == rBOTH_DIED || char_died(ch))
    return;

  argument = one_argument(argument, arg);

  if(!arg || arg[0] == '\0')
  {
    int                     mcnt = 0;

    send_to_char("&CSyntax: disguise < key word >\r\n", ch);
    send_to_char("Syntax: undisguise\r\n\r\n", ch);
    /*
     * Search Morph CLASS for Matching Forms 
     */
    send_to_pager("\r\n&CThe Following Disguises are Available:\r\n&W-[ &GKey Words&W ]---[&G Form Description &W]--  [&G Minimum Level &W]--&w\r\n", ch);
    if(!morph_start)
    {
      send_to_pager("&R (None Available) &w\r\n", ch);
      return;
    }
    for(morph = morph_start; morph; morph = morph->next)
    {

      if(!str_cmp(morph->short_desc, "a black bear") ||
         !str_cmp(morph->short_desc, "a gray wolf") ||
         !str_cmp(morph->short_desc, "a vampire bat") || !str_cmp(morph->short_desc, "a gryphon") || !str_cmp(morph->short_desc, "an owl") || !str_cmp(morph->short_desc, "a bass fish"))
        continue;
      /*
       * Check Level 
       */
      if(morph->level > ch->level)
        continue;
      /*
       * Check all classes 
       */
      if(morph->Class != 0 && !IS_SET(morph->Class, ch->Class))

        continue;
      /*
       * Check race 
       */
      if(morph->race != 0 && !IS_SET(morph->race, ch->race))
        continue;
      mcnt++;
      pager_printf_color(ch, "&C%-15.15s &W:&w%-30.30s &C%-3d&w\r\n", morph->key_words, morph->short_desc, morph->level);
    }

    if(mcnt == 0)
      send_to_pager("&R (None Available)&w\r\n", ch);

    send_to_char("&W-------------------------------------------------------&w\r\n", ch);
  }
  else
  {
    char                    buf[MSL];

    /*
     * Check for Valid Form Name and Transform
     */
    if(!morph_start)
    {
      send_to_pager("&R(No Forms Available)&w\r\n", ch);
      return;
    }

    for(morph = morph_start; morph; morph = morph->next)
    {
      /*
       * Check Level 
       */
      if(morph->level > ch->level)
        continue;
      /*
       * Check all classes 
       */
      if(morph->Class != 0 && !IS_SET(morph->Class, ch->Class))
        continue;
      /*
       * Check race 
       */
      if(morph->race != 0 && !IS_SET(morph->race, ch->race))
        continue;
      if(!nifty_is_name(arg, morph->key_words))
        continue;
      fForm = TRUE;
      break;
    }

    if(!fForm)
    {
      send_to_char("&YNo such alternative form for you to disguise into.&w\r\n", ch);
      return;
    }
    if(morph->level > ch->level)
    {
      send_to_char("&cYou are not high enough level to disguise yourself that well.&w\r\n", ch);
      return;
    }
    percent = LEARNED(ch, gsn_disguise);
    WAIT_STATE(ch, skill_table[gsn_disguise]->beats);

    if(!chance(ch, percent) || !fForm)
    {
      send_to_char("&cYou fail to assume the disguise.&w\r\n", ch);
      learn_from_failure(ch, gsn_disguise);
      return;
    }

    if(fForm)
    {
      snprintf(buf, MSL, "%d", morph->vnum);
      do_imm_morph(ch, buf);
    }

    if(ch->morph == NULL || ch->morph->morph == NULL)
    {
      fForm = FALSE;
      return;
    }
    af.type = gsn_disguise;
    af.duration = ch->level + 20;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.level = ch->level;
    af.bitvector = meb(AFF_DISGUISE);
    affect_to_char(ch, &af);
    learn_from_success(ch, gsn_disguise);
    act(AT_CYAN, "You carefully don a disguise to fool others.", ch, NULL, victim, TO_CHAR);
    act(AT_CYAN, "You see $n carefully don a disguise.", ch, NULL, NULL, TO_ROOM);
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/disguise.wav)\r\n", ch);

  }
}

void do_undisguise(CHAR_DATA *ch, char *argument)
{
  if((skill_table[gsn_disguise]->skill_level[ch->Class] == -1 && !IS_IMMORTAL(ch)) && (LEARNED(ch, gsn_disguise) < 1))
  {
    error(ch);
    return;
  }

  if(ch->morph == NULL || ch->morph->morph == NULL)
  {
    send_to_char("Why would you need to undisguise when you are not disguised?\r\n", ch);
    return;
  }

  act(AT_LBLUE, "You quickly remove your disguise.", ch, NULL, NULL, TO_CHAR);
  act(AT_LBLUE, "$n removes an elaborate disguise.", ch, NULL, NULL, TO_NOTVICT);

  /*
   * Strip off ShapeShift Affects
   */
  affect_strip(ch, gsn_disguise);
  xREMOVE_BIT(ch->affected_by, AFF_DISGUISE);
  do_unmorph_char(ch);
  return;
}
