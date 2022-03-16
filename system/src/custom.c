/***************************************************************************** * SKILLS MODULE * 
****************************************************************************/

#include "h/mud.h"
#include "h/damage.h"
#include "h/polymorph.h"

int                     heal_rate, heal_xp = 0;

void                    remove_all_equipment(CHAR_DATA *ch);

void do_infrasion(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;

  if(IS_AFFECTED(ch, AFF_INFRARED))
  {
    send_to_char("You are already affected by infrasion.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_infrasion))
  {
    if(xIS_SET(ch->act, PLR_BATTLE) || (xIS_SET(ch->act, ACT_BEASTMELD) && xIS_SET(ch->master->act, PLR_BATTLE)))
      send_to_char("!!SOUND(sound/infravision.wav)\r\n", ch);
    af.type = gsn_infrasion;
    af.duration = ch->level;
    af.level = ch->level;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = meb(AFF_INFRARED);
    affect_join(ch, &af);
    act(AT_GREEN, "Your eyes slowly begin to see the heat signatures of your surroundings.", ch, NULL, NULL, TO_CHAR);

    learn_from_success(ch, gsn_infrasion);
  }
  else
    act(AT_GREEN, "You try to use your infrasion, but get distracted.", ch, NULL, NULL, TO_CHAR);

  learn_from_failure(ch, gsn_infrasion);

}

void do_mistform(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;

  if(IS_AFFECTED(ch, AFF_PASS_DOOR))
  {
    send_to_char("You are already in mistform.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_mistform))
  {

        short bonus = 0;
        if ( calculate_age(ch) > 119 && calculate_age(ch) < 130 )
        {
        bonus = 25;
        }
        if ( calculate_age(ch) > 129 && calculate_age(ch) < 140 )
        {
        bonus = 50;
        }
        if ( calculate_age(ch) > 139 )
        {
        bonus = 100;
        }

    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/mistform.wav)\r\n", ch);
    af.type = gsn_mistform;
    af.duration = ch->level+bonus;
    af.level = ch->level;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = meb(AFF_PASS_DOOR);
    affect_join(ch, &af);
    act(AT_BLOOD, "Using your blood power, you assume a mist-like form.", ch, NULL, NULL, TO_CHAR);
    act(AT_BLOOD, "$n suddenly disolves into a fine mist.", ch, NULL, NULL, TO_ROOM);

    learn_from_success(ch, gsn_mistform);
  }
  else
    act(AT_BLOOD, "You try to use your blood power, but fail to assume a mist-like form.", ch, NULL, NULL, TO_CHAR);

  learn_from_failure(ch, gsn_mistform);

}

void do_hypnotize(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;
  short                   chance;
  CHAR_DATA              *victim;
  CHAR_DATA              *rch;

  if((victim = get_char_room(ch, target_name)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(!IS_NPC(victim))
  {
    send_to_char("This power only works on dumb beasts.\r\n", ch);
    return;
  }

  if(is_safe(ch, victim, TRUE))
    return;

  if(IS_AFFECTED(victim, AFF_HYPNOTIZE))
  {
    send_to_char("They are already hypnotized.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_hypnotize))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/hypnotize.wav)\r\n", ch);

    learn_from_success(ch, gsn_hypnotize);
    if(victim->level > ch->level)
    {
      chance = number_chance(1, 4);
      if(chance > 2)
      {
        af.type = gsn_hypnotize;
        af.duration = 4;
        af.level = ch->level;
        af.location = APPLY_NONE;
        af.modifier = 0;
        af.bitvector = meb(AFF_HYPNOTIZE);
        affect_join(victim, &af);

        act(AT_CYAN, "You stare into the eyes of your victims.\r\n&rTheir eyes fill with blood!", ch, NULL, NULL, TO_CHAR);
        act(AT_BLOOD, "Everyone's eyes suddenly fill with blood!", ch, NULL, victim, TO_ROOM);
        for(rch = ch->in_room->first_person; rch; rch = rch->next_in_room)
        {
          if(is_same_group(ch, rch))
            stop_fighting(rch, TRUE);
        }
        stop_fighting(victim, TRUE);
        stop_hunting(victim);
        stop_hating(victim);
        ch->hate_level = 0;
        return;
      }
      else
        return;
    }

    if(victim->level <= ch->level)
    {
      af.type = gsn_hypnotize;
      af.duration = 5;
      af.level = ch->level;
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.bitvector = meb(AFF_HYPNOTIZE);
      affect_join(victim, &af);
      act(AT_CYAN, "You stare into the eyes of your victims.\r\n&rTheir eyes fill with blood!", ch, NULL, NULL, TO_CHAR);
      act(AT_BLOOD, "Everyone's eyes suddenly fill with blood!", ch, NULL, victim, TO_ROOM);

      for(rch = ch->in_room->first_person; rch; rch = rch->next_in_room)
      {
        if(is_same_group(ch, rch))
          stop_fighting(rch, TRUE);
      }
      stop_fighting(victim, TRUE);
      stop_hunting(victim);
      stop_hating(victim);
      ch->hate_level = 0;
      return;
    }
  }
  else
    learn_from_failure(ch, gsn_hypnotize);
  return;
}

void do_willow_skin(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;

  if(IS_AFFECTED(ch, AFF_WILLOW_SKIN))
  {
    send_to_char("You are already using willow skin.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_willow_skin))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/willow.wav)\r\n", ch);

    af.type = gsn_willow_skin;
    af.location = APPLY_AC;
    af.bitvector = meb(AFF_WILLOW_SKIN);
    af.modifier = -1 * ch->level;
    af.duration = ch->level * 4;
    af.level = ch->level;
    affect_to_char(ch, &af);
    learn_from_success(ch, gsn_willow_skin);
    act(AT_ORANGE, "Your skin hardens to the density of willow trees!\r\n", ch, NULL, NULL, TO_CHAR);
    act(AT_ORANGE, "$n's skin takes on a willow tree appearance!\r\n", ch, NULL, NULL, TO_ROOM);
    return;
  }
  else
    act(AT_CYAN, "You try to take on willow skin, but get distracted!\r\n", ch, NULL, NULL, TO_CHAR);
  learn_from_failure(ch, gsn_willow_skin);
  return;
}

void do_valtors_song(CHAR_DATA *ch, char *argument)
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

  if(IS_AFFECTED(victim, AFF_VALTORS_SONG))
  {
    send_to_char("You already have valtors song affecting you.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_valtors_song))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/valtors.wav)\r\n", ch);

    af.type = gsn_valtors_song;
    af.location = APPLY_DEX;
    xCLEAR_BITS(af.bitvector);
    af.modifier = (ch->level / 20) + 1;
    af.duration = ch->level * 4;
    af.level = ch->level;
    affect_to_char(victim, &af);

    af.type = gsn_valtors_song;
    af.location = APPLY_AC;
    af.bitvector = meb(AFF_VALTORS_SONG);
    af.modifier = -1 * ch->level;
    af.duration = ch->level * 4;
    af.level = ch->level;
    affect_to_char(victim, &af);

    act(AT_CYAN, "You begin an ancient battle song!\r\n", ch, NULL, victim, TO_CHAR);
    if(ch != victim)
      act(AT_CYAN, "$n sings a ancient battle song causing defensive aura surrounds you!\r\n", ch, NULL, victim, TO_VICT);
    learn_from_success(ch, gsn_valtors_song);
  }
  else
    act(AT_CYAN, "You begin to ancient battle song, but get distracted.\r\n", ch, NULL, victim, TO_CHAR);
  learn_from_failure(ch, gsn_valtors_song);
  return;

}

void do_talkup(CHAR_DATA *ch, char *argument)
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

  if(IS_AFFECTED(victim, AFF_TALKUP))
  {
    send_to_char("You already have talkup affecting you.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_talkup))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/talkup.wav)\r\n", ch);

    af.type = gsn_talkup;
    af.location = APPLY_CHA;
    xCLEAR_BITS(af.bitvector);
    af.modifier = (ch->level / 20) + 1;
    af.duration = ch->level * 4;
    af.level = ch->level;
    affect_to_char(victim, &af);

    af.type = gsn_talkup;
    af.location = APPLY_STR;
    af.bitvector = meb(AFF_TALKUP);
    af.modifier = (ch->level / 20) + 1;
    af.duration = ch->level * 4;
    af.level = ch->level;
    affect_to_char(victim, &af);

    act(AT_CYAN, "You begin to brag about deeds untold!\r\n", ch, NULL, victim, TO_CHAR);
    act(AT_CYAN, "A false sense of pride swells up in you with the deeds others think you performed!\r\n", ch, NULL, victim, TO_VICT);
    learn_from_success(ch, gsn_talkup);
  }
  else
    act(AT_CYAN, "You begin to brag, but get distracted.\r\n", ch, NULL, victim, TO_CHAR);
  learn_from_failure(ch, gsn_talkup);
  return;

}

void do_creeping_doom(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA              *vch;
  CHAR_DATA              *vch_next;
  bool                    ch_died;

  ch_died = FALSE;

  if(IS_SET(ch->in_room->room_flags, ROOM_SAFE))
  {
    send_to_char_color("&BThe staff of 6D prevents you from attacking here.\r\n", ch);
    return;
  }

  if(!ch->in_room->first_person)
  {
    send_to_char("There's no one else here!\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_creeping_doom))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/creepingdoom.wav)\r\n", ch);
    act(AT_ORANGE, "You call forth a massive swarming creeping doom of insects!", ch, NULL, NULL, TO_CHAR);
    act(AT_ORANGE, "$n calls forth a massive swarming creeping doom of insects!", ch, NULL, NULL, TO_ROOM);

    for(vch = ch->in_room->first_person; vch; vch = vch_next)
    {
      vch_next = vch->next_in_room;

      if(IS_NPC(vch) && (vch->pIndexData->vnum == MOB_VNUM_SOLDIERS || vch->pIndexData->vnum == MOB_VNUM_ARCHERS))
        continue;

      // Bug fix here, was striking grouped members. -Taon
      if(is_same_group(vch, ch))
        continue;

      if(!IS_NPC(vch) && xIS_SET(vch->act, PLR_WIZINVIS) && vch->pcdata->wizinvis >= LEVEL_IMMORTAL)
        continue;

      if(IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch))
      {
        if(char_died(ch))
        {
          ch_died = TRUE;
          break;
        }

        global_retcode = damage(ch, vch, ludicrous, gsn_creeping_doom);
      }
      learn_from_success(ch, gsn_creeping_doom);
    }
  }
  else
    act(AT_ORANGE, "You try to call a creeping doom swarm, but get distracted!", ch, NULL, NULL, TO_CHAR);
  learn_from_failure(ch, gsn_creeping_doom);
  return;
}

void do_swarm(CHAR_DATA *ch, char *argument)
{
  char                    arg[MIL];
  CHAR_DATA              *victim;

  argument = one_argument(argument, arg);

  if(arg[0] == '\0')
  {
    if(ch->fighting)
      victim = who_fighting(ch);
    else
    {
      send_to_char("&OSend an insect swarm on who?\r\n", ch);
      return;
    }
  }
  else if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_swarm) || IS_NPC(ch))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/swarm.wav)\r\n", ch);
    act(AT_ORANGE, "You summon a swarm of insects against $N!", ch, NULL, victim, TO_CHAR);
    act(AT_ORANGE, "$n summon's a swarm of insects upon $N!", ch, NULL, victim, TO_ROOM);
    global_retcode = damage(ch, victim, ludicrous, gsn_swarm);
    learn_from_success(ch, gsn_swarm);
    return;
  }
  else
    act(AT_ORANGE, "You try to summon a swarm of insects, but get distracted!", ch, NULL, NULL, TO_CHAR);
  global_retcode = damage(ch, victim, 0, gsn_swarm);

  learn_from_failure(ch, gsn_swarm);
  return;

}

// A few shaman skills I'd rather not make, but the class needs to be more shaman-like
void do_hex(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;
  char                    arg[MIL];
  CHAR_DATA              *victim;

  argument = one_argument(argument, arg);

  if(arg[0] == '\0')
  {
    if(ch->fighting)
      victim = who_fighting(ch);
    else
    {
      send_to_char("Put the old evil eye hex on who?\r\n", ch);
      return;
    }
  }
  else if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_hex) || IS_NPC(ch))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/hex.wav)\r\n", ch);
    act(AT_BLOOD, "You focus on putting the old evil eye hex on $N!", ch, NULL, victim, TO_CHAR);
    act(AT_BLOOD, "$n puts an evil eye hex on $N!", ch, NULL, victim, TO_ROOM);
    global_retcode = damage(ch, victim, mediumhigh, gsn_hex);
    affect_strip(victim, gsn_hex);
    af.type = gsn_hex;
    af.location = APPLY_AC;
    xCLEAR_BITS(af.bitvector);
    af.modifier = 20;
    af.duration = ch->level;
    af.level = ch->level;
    affect_to_char(victim, &af);
    learn_from_success(ch, gsn_hex);
  }
  else
  {
    act(AT_CYAN, "You try to use the old evil eye hex, but get distracted!", ch, NULL, NULL, TO_CHAR);
    global_retcode = damage(ch, victim, 0, gsn_hex);
    learn_from_failure(ch, gsn_hex);
  }
  return;
}

void do_frost(CHAR_DATA *ch, char *argument)
{
  char                    arg[MIL];
  CHAR_DATA              *victim;

  argument = one_argument(argument, arg);

  if(arg[0] == '\0')
  {
    if(ch->fighting)
      victim = who_fighting(ch);
    else
    {
      send_to_char("Frost who?\r\n", ch);
      return;
    }
  }
  else if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_frost) || IS_NPC(ch))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/frost.wav)\r\n", ch);
    act(AT_LBLUE, "You cause a frosting effect on $N!", ch, NULL, victim, TO_CHAR);
    act(AT_LBLUE, "$n causes a frosting effect on $N!", ch, NULL, victim, TO_ROOM);
    global_retcode = damage(ch, victim, mediumhigh, gsn_frost);
    learn_from_success(ch, gsn_frost);
  }
  else
  {
    act(AT_LBLUE, "You try to cause a frosting effect, but get distracted!", ch, NULL, NULL, TO_CHAR);
    global_retcode = damage(ch, victim, 0, gsn_frost);
    learn_from_failure(ch, gsn_frost);
  }
  return;
}

void do_freeze(CHAR_DATA *ch, char *argument)
{
  char                    arg[MIL];
  CHAR_DATA              *victim;

  argument = one_argument(argument, arg);

  if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(ch == victim || arg[0] == '\0')
  {
    send_to_char("&CFreeze who?\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_freeze))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/frost.wav)\r\n", ch);
    act(AT_LBLUE, "You cause a freezing effect on $N!", ch, NULL, victim, TO_CHAR);
    act(AT_LBLUE, "$n causes a freezing effect on $N!", ch, NULL, victim, TO_ROOM);
    global_retcode = damage(ch, victim, high, gsn_freeze);
    learn_from_success(ch, gsn_freeze);
  }
  else
    act(AT_LBLUE, "You try to cause a freezing effect, but get distracted!", ch, NULL, NULL, TO_CHAR);
  global_retcode = damage(ch, victim, 0, gsn_freeze);

  learn_from_failure(ch, gsn_freeze);
  return;

}

void do_chill(CHAR_DATA *ch, char *argument)
{
  char                    arg[MIL];
  CHAR_DATA              *victim;

  argument = one_argument(argument, arg);

  if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(ch == victim || arg[0] == '\0')
  {
    send_to_char("&CChill who?\r\n", ch);
    return;
  }
  if(can_use_skill(ch, number_percent(), gsn_chill))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/chill.wav)\r\n", ch);
    act(AT_LBLUE, "You send a chilling blast at $N!", ch, NULL, victim, TO_CHAR);
    act(AT_LBLUE, "$n sends a chilling blast at $N!", ch, NULL, victim, TO_ROOM);
    global_retcode = damage(ch, victim, medium, gsn_chill);
    learn_from_success(ch, gsn_chill);
  }
  else
    act(AT_LBLUE, "You send a chilling blast, but miss $N!", ch, NULL, victim, TO_CHAR);
  global_retcode = damage(ch, victim, 0, gsn_chill);

  learn_from_failure(ch, gsn_chill);
  return;

}

void do_acid_spray(CHAR_DATA *ch, char *argument)
{
  char                    arg[MIL];
  CHAR_DATA              *victim;

  argument = one_argument(argument, arg);

  if(arg[0] == '\0')
  {
    if(ch->fighting)
      victim = who_fighting(ch);
    else
    {
      send_to_char("Acid spray who?\r\n", ch);
      return;
    }
  }
  else if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_acid_spray) || IS_NPC(ch))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/acidspray.wav)\r\n", ch);
    act(AT_GREEN, "You open your mouth and spray acid at $N!", ch, NULL, victim, TO_CHAR);
    act(AT_GREEN, "$n opens $s mouth and sprays acid at $N!", ch, NULL, victim, TO_ROOM);
    global_retcode = damage(ch, victim, ludicrous, gsn_acid_spray);
    learn_from_success(ch, gsn_acid_spray);
    return;
  }
  else
    act(AT_GREEN, "You open your mouth spraying acid, but miss $N!", ch, NULL, victim, TO_CHAR);
  global_retcode = damage(ch, victim, 0, gsn_acid_spray);

  learn_from_failure(ch, gsn_acid_spray);
  return;

}

void do_critic(CHAR_DATA *ch, char *argument)
{
  char                    arg[MIL];
  CHAR_DATA              *victim;

  argument = one_argument(argument, arg);
  OBJ_DATA               *obj;

  for(obj = ch->first_carrying; obj; obj = obj->next_content)
  {
    if(obj->item_type == ITEM_INSTRUMENT)
    {
      break;
    }
  }

  if(!obj)
  {
    send_to_char("You do not have an instrument handy.\r\n", ch);
    return;
  }

  if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(ch == victim)
  {
    send_to_char("Whose a critic?\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_critic))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/critic.wav)\r\n", ch);
    act(AT_CYAN, "You visciously swing your $p and wrap it around $N's head!", ch, obj, victim, TO_CHAR);
    act(AT_CYAN, "$n visciously swings $p and wraps it around $N's head!", ch, obj, victim, TO_ROOM);
    global_retcode = damage(ch, victim, ludicrous, gsn_critic);
    separate_obj(obj);
    obj_from_char(obj);
    extract_obj(obj);
    learn_from_success(ch, gsn_critic);
  }
  else
    act(AT_CYAN, "You visciously swing your $p but miss $N's head!", ch, NULL, victim, TO_CHAR);
  global_retcode = damage(ch, victim, 0, gsn_critic);

  learn_from_failure(ch, gsn_critic);
  return;

}

void do_quick_blade(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;

  if(IS_AFFECTED(ch, AFF_QUICK_BLADE))
  {
    send_to_char("You are already using quick blade.\r\n", ch);
    return;
  }

  if(ch->move < ch->max_move / 10)
  {
    send_to_char("You don't have enough move left for quick blade.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_quick_blade))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/speed.wav)\r\n", ch);

    af.type = gsn_quick_blade;
    af.location = APPLY_AFFECT;
    af.bitvector = meb(AFF_QUICK_BLADE);
    af.modifier = 0;
    af.duration = 5;
    af.level = ch->level;
    affect_to_char(ch, &af);
    ch->move = number_range(get_curr_con(ch) * 2, get_curr_con(ch) * 4);
    act(AT_CYAN, "You put all your move into your speed for an all out assault!\r\n", ch, NULL, NULL, TO_CHAR);
    act(AT_CYAN, "$n begins to move with quick blade speed!\r\n", ch, NULL, NULL, TO_ROOM);
    learn_from_success(ch, gsn_quick_blade);
    return;
  }
  else
    act(AT_CYAN, "You try to put your move into your speed, but fail to concentrate enough.\r\n", ch, NULL, NULL, TO_CHAR);
  learn_from_failure(ch, gsn_quick_blade);
  return;

}

// toggle for vampires using speed assault
void do_blinding_speed(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;

  if(IS_AFFECTED(ch, AFF_BLINDING_SPEED))
  {
    send_to_char("You are already using blinding speed.\r\n", ch);
    return;
  }

  if(ch->blood < ch->max_blood / 10)
  {
    send_to_char("You don't have enough blood power for blinding speed.\r\n", ch);
    return;
  }
  if(ch->move < ch->max_move / 10)
  {
    send_to_char("You don't have enough movement endurance for blinding speed.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_blinding_speed))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/speed.wav)\r\n", ch);

    af.type = gsn_blinding_speed;
    af.location = APPLY_AFFECT;
    af.bitvector = meb(AFF_BLINDING_SPEED);
    af.modifier = 0;
    af.duration = 5;
    af.level = ch->level;
    affect_to_char(ch, &af);
    ch->blood = 3;
    ch->move = ch->level;
    act(AT_RED, "You put all your blood power into your speed for an all out assault!\r\n", ch, NULL, NULL, TO_CHAR);
    act(AT_CYAN, "$n beings to move in a complete blur of speed!\r\n", ch, NULL, NULL, TO_ROOM);
    learn_from_success(ch, gsn_blinding_speed);
    return;
  }
  else
    act(AT_CYAN, "You try to put your blood power into your speed, but fail to concentrate enough.\r\n", ch, NULL, NULL, TO_CHAR);
  learn_from_failure(ch, gsn_blinding_speed);
  return;

}

void do_morris_dance(CHAR_DATA *ch, char *argument)
{
  char                    arg[MIL];

  argument = one_argument(argument, arg);

  if(can_use_skill(ch, number_percent(), gsn_morris_dance))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/morris.wav)\r\n", ch);
    if(ch->martialarts == 0)
    {
      ch->martialarts = 3;
      act(AT_CYAN, "You begin the intricate movements of the morris dance!\r\n", ch, NULL, NULL, TO_CHAR);
      act(AT_CYAN, "$n's hips move from side to side as $s beings the moves of the morris dance!\r\n", ch, NULL, NULL, TO_ROOM);
      learn_from_success(ch, gsn_morris_dance);
    }
    else
    {
      ch->martialarts = 0;
      act(AT_CYAN, "You stop the intricate movements of the morris dance!\r\n", ch, NULL, NULL, TO_CHAR);
      learn_from_success(ch, gsn_morris_dance);
    }
  }
  else
    act(AT_CYAN, "You start to perform the morris dance, but misstep.\r\n", ch, NULL, NULL, TO_CHAR);
  learn_from_failure(ch, gsn_morris_dance);
  return;

}

void do_juggle(CHAR_DATA *ch, char *argument)
{
  char                    arg[MIL];
  short                   chance = number_range(1, 100);
  CHAR_DATA              *victim;

  argument = one_argument(argument, arg);
  OBJ_DATA               *obj;

  if(!str_cmp(arg, "collect"))
  {
    if(!IN_WILDERNESS(ch))
    {
      send_to_char("You have to be in the wilderness to find suitable clubs to juggle.\r\n", ch);
      return;
    }
    if(can_use_skill(ch, number_percent(), gsn_juggle))
    {
      if(xIS_SET(ch->act, PLR_BATTLE))
        send_to_char("!!SOUND(sound/collect.wav)\r\n", ch);
      act(AT_CYAN, "You find a well balanced club for juggling.", ch, NULL, NULL, TO_CHAR);
      act(AT_CYAN, "$n finds a well balanced club for juggling.", ch, NULL, NULL, TO_ROOM);
      obj = create_object(get_obj_index(41030), 0);
      obj_to_char(obj, ch);
      obj->value[1] = 1;
      obj->value[2] = 4;
      obj->value[4] = 5;
      learn_from_success(ch, gsn_juggle);
    }
    else
      act(AT_CYAN, "You try to find a well balanced club for juggling, but see nothing suitable.", ch, NULL, NULL, TO_CHAR);
    act(AT_CYAN, "$n looks frustrated trying to find a well balanced club for juggling.", ch, NULL, NULL, TO_ROOM);

    learn_from_failure(ch, gsn_juggle);
    return;
  }

  short                   count = 0;

  for(obj = ch->first_carrying; obj; obj = obj->next_content)
  {
    if(obj->pIndexData->vnum == 41030)
    {
      count += 1;
    }
    if(count > 2)
      break;
  }

  if(!obj || count < 3)
  {
    send_to_char("You do not have enough clubs to juggle with.\r\n", ch);
    return;
  }

  if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(ch == victim)
  {
    send_to_char("Juggle at who?\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_juggle))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/juggle.wav)\r\n", ch);
    act(AT_CYAN, "You begin juggling and launch your clubs at $N!", ch, NULL, victim, TO_CHAR);
    act(AT_CYAN, "$n begins juggling and launches $s clubs at $N", ch, NULL, victim, TO_ROOM);
    global_retcode = damage(ch, victim, mediumhigh, gsn_juggle);
    separate_obj(obj);
    obj_from_char(obj);
    extract_obj(obj);
    learn_from_success(ch, gsn_juggle);
  }
  else
    act(AT_CYAN, "You begin juggling, but lose concentration and drop them!", ch, NULL, NULL, TO_CHAR);
  act(AT_CYAN, "$n begins juggling but loses concentration dropping them.", ch, NULL, NULL, TO_ROOM);
  global_retcode = damage(ch, victim, 0, gsn_juggle);

  learn_from_failure(ch, gsn_juggle);
  return;
}

void do_story(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;
  short                   chance;
  CHAR_DATA              *victim;
  CHAR_DATA              *rch;
  int                     sn;
  char                    arg[MIL];

  argument = one_argument(argument, arg);

  if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(!IS_NPC(victim))
  {
    send_to_char("This story telling only works on npcs.\r\n", ch);
    return;
  }

  if(is_safe(ch, victim, TRUE))
    return;

  if(IS_AFFECTED(victim, AFF_ENTHRALLED))
  {
    send_to_char("They are already enthralled.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_story))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/story.wav)\r\n", ch);

    if(victim->level > ch->level)
    {
      chance = number_chance(1, 4);
      if(chance > 2)
      {
        af.type = gsn_story;
        af.duration = 4;
        af.level = ch->level;
        af.location = APPLY_NONE;
        af.modifier = 0;
        af.bitvector = meb(AFF_ENTHRALLED);
        affect_join(victim, &af);
        act(AT_CYAN, "You begin to spin a fascinating yarn about a story you just made up on the spur of the moment.", ch, NULL, NULL, TO_CHAR);
        act(AT_CYAN, "$n's fascinating story beings to entrall $N!", ch, NULL, victim, TO_ROOM);
        for(rch = ch->in_room->first_person; rch; rch = rch->next_in_room)
        {
          if(is_same_group(ch, rch))
            stop_fighting(rch, TRUE);
        }
        stop_fighting(victim, TRUE);
        stop_hunting(victim);
        stop_hating(victim);
        ch->hate_level = 0;
        return;
      }
      else
        return;
    }
    if(victim->level <= ch->level)
    {
      af.type = gsn_story;
      af.duration = 5;
      af.level = ch->level;
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.bitvector = meb(AFF_ENTHRALLED);
      affect_join(victim, &af);
      act(AT_CYAN, "You begin to spin a fascinating yarn about a story you just made up on the spur of the moment.", ch, NULL, NULL, TO_CHAR);
      act(AT_CYAN, "$n's fascinating story beings to entrall $N!", ch, NULL, victim, TO_ROOM);

      for(rch = ch->in_room->first_person; rch; rch = rch->next_in_room)
      {
        if(is_same_group(ch, rch))
          stop_fighting(rch, TRUE);
      }
      stop_fighting(victim, TRUE);
      stop_hunting(victim);
      stop_hating(victim);
      ch->hate_level = 0;
      return;
    }
    learn_from_success(ch, gsn_story);
  }
  else
    learn_from_failure(ch, gsn_story);
  return;
}

void do_angelic_being(CHAR_DATA *ch, char *argument)
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

  if(IS_AFFECTED(victim, AFF_ANGELIC_BEING))
  {
    send_to_char("You are already using angelic being.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_angelic_being))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/angelic.wav)\r\n", ch);

    af.type = gsn_angelic_being;
    af.location = APPLY_HITROLL;
    xCLEAR_BITS(af.bitvector);
    af.modifier = (ch->level / 12) + 1;
    af.duration = ch->level * 4;
    af.level = ch->level;
    affect_to_char(victim, &af);

    af.type = gsn_angelic_being;
    af.location = APPLY_DAMROLL;
    xCLEAR_BITS(af.bitvector);
    af.modifier = (ch->level / 12) + 1;
    af.duration = ch->level * 4;
    af.level = ch->level;
    affect_to_char(victim, &af);

    af.type = gsn_angelic_being;
    af.location = APPLY_STR;
    af.bitvector = meb(AFF_ANGELIC_BEING);
    af.modifier = (ch->level / 20) + 1;
    af.duration = ch->level * 4;
    af.level = ch->level;
    affect_to_char(victim, &af);

    act(AT_BLUE, "You call upon your angelic being!\r\n", ch, NULL, victim, TO_CHAR);
    act(AT_BLUE, "A surge of angelic fervor washes over you!\r\n", ch, NULL, victim, TO_VICT);
    learn_from_success(ch, gsn_angelic_being);
  }
  else
    act(AT_BLUE, "You call upon your angelic being, but get distracted.\r\n", ch, NULL, victim, TO_CHAR);
  learn_from_failure(ch, gsn_angelic_being);
  return;
}

void do_hawk_spirit(CHAR_DATA *ch, char *argument)
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

  if(IS_AFFECTED(victim, AFF_HAWK_SPIRIT))
  {
    send_to_char("You already have the spirit of the hawk within you.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_hawk_spirit))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/hawkspirit.wav)\r\n", ch);

    af.type = gsn_hawk_spirit;
    af.location = APPLY_HITROLL;
    af.modifier = (ch->level / 8) + 1;
    af.duration = ch->level * 5;
    af.bitvector = meb(AFF_HAWK_SPIRIT);
    af.level = ch->level;
    affect_to_char(victim, &af);
    act(AT_BLUE, "You call on the spirit of the hawk!\r\n", ch, NULL, victim, TO_CHAR);
    act(AT_BLUE, "Your eyes narrow as the spirit of the hawk fills you!\r\n", ch, NULL, victim, TO_VICT);

    learn_from_success(ch, gsn_hawk_spirit);
  }
  else
    act(AT_BLUE, "You try to call on the spirit of the hawk, but get distracted.\r\n", ch, NULL, victim, TO_CHAR);
  learn_from_failure(ch, gsn_hawk_spirit);
  return;
}

void do_confidence(CHAR_DATA *ch, char *argument)
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

  if(can_use_skill(ch, number_percent(), gsn_confidence))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/confidence.wav)\r\n", ch);

    affect_strip(victim, gsn_confidence);
    af.type = gsn_confidence;
    af.location = APPLY_HITROLL;
    af.modifier = (ch->level / 8) + 1;
    af.duration = ch->level * 5;
    xCLEAR_BITS(af.bitvector);
    af.level = ch->level;
    affect_to_char(victim, &af);
    act(AT_BLUE, "You speak the words of the ancient adventurers...\r\n", ch, NULL, victim, TO_CHAR);
    act(AT_BLUE, "A sudden confidence begins to well up inside you!\r\n", ch, NULL, victim, TO_VICT);
    learn_from_success(ch, gsn_confidence);
  }
  else
    act(AT_BLUE, "You speak the words, but are distracted.\r\n", ch, NULL, victim, TO_CHAR);
  learn_from_failure(ch, gsn_confidence);
  return;
}

void do_ward_of_ethereal(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;

  if(IS_AFFECTED(ch, AFF_ETHEREAL))
  {
    send_to_char("You are already protected by the ethereal ward.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_ward_of_ethereal))
  {
    af.type = gsn_ward_of_ethereal;
    af.duration = ch->level + 25;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = meb(AFF_ETHEREAL);
    af.level = ch->level;
    ch->ward_dam = (300 + ch->level * 20);
    affect_to_char(ch, &af);
    act(AT_PLAIN, "You force an ethereal ward outward around you.", ch, NULL, NULL, TO_CHAR);
    act(AT_PLAIN, "$n's ethereal ward shimmers as it expands around them.", ch, NULL, NULL, TO_NOTVICT);
    learn_from_success(ch, gsn_ward_of_ethereal);
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/etherealward.wav)\r\n", ch);

  }
  else
    act(AT_PLAIN, "You fail to control your ethereal ward and it disappates.", ch, NULL, NULL, TO_CHAR);
  learn_from_failure(ch, gsn_ward_of_ethereal);
}

void do_maul(CHAR_DATA *ch, char *argument)
{
  short                   nomore;

  nomore = 10;
  CHAR_DATA              *victim;
  char                    arg[MIL];

  argument = one_argument(argument, arg);

  if(arg[0] == '\0')
  {
    if(ch->fighting)
      victim = who_fighting(ch);
    else
    {
      send_to_char("Maul attack who?\r\n", ch);
      return;
    }
  }
  else if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(ch->Class != CLASS_BEAR)
  {
    send_to_char("You have to be a bear to maul someone.\r\n", ch);
    return;
  }

  if(ch->move < nomore)
  {
    send_to_char("You do not have enough move to do that.\r\n", ch);
    return;
  }

  if(ch == victim)
  {
    send_to_char("Suicide is a mortal sin.\r\n", ch);
    return;
  }
  send_to_char("\r\n\r\n&OYou take off in blurring speed!!\r\n", ch);
  if(can_use_skill(ch, number_percent(), gsn_maul))
  {
    WAIT_STATE(ch, skill_table[gsn_maul]->beats);
    act(AT_GREEN, "You deliver a mauling attack with your claws at $N!", ch, NULL, victim, TO_CHAR);
    act(AT_GREEN, "Your are knocked down from the speed and power of $n's mauling attack!", ch, NULL, victim, TO_VICT);
    learn_from_success(ch, gsn_maul);
    ch->move = (ch->move - nomore);
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/maul.wav)\r\n", ch);

    global_retcode = damage(ch, victim, extrahigh, gsn_maul);

  }
  else
  {
    learn_from_failure(ch, gsn_maul);
    global_retcode = damage(ch, victim, 0, gsn_maul);
    send_to_char("&cYour maul attack misses the target.\r\n", ch);
  }
}

void do_ferocious_strike(CHAR_DATA *ch, char *argument)
{
  short                   nomore;

  nomore = 10;
  CHAR_DATA              *victim;
  char                    arg[MIL];

  argument = one_argument(argument, arg);

  if(arg[0] == '\0')
  {
    if(ch->fighting)
      victim = who_fighting(ch);
    else
    {
      send_to_char("Send a ferocious strike at who?\r\n", ch);
      return;
    }
  }
  else if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }
  if(ch->move < nomore)
  {
    send_to_char("You do not have enough move to do that.\r\n", ch);
    return;
  }

  if(ch == victim)
  {
    send_to_char("Suicide is a mortal sin.\r\n", ch);
    return;
  }

  if(ch->mount)
  {
    send_to_char("You cannot deliver a ferocious strike while mounted.\r\n", ch);
    return;
  }
  send_to_char("\r\n\r\n&OYou attack with an incredible rate of speed!\r\n", ch);
  if(can_use_skill(ch, number_percent(), gsn_ferocious_strike))
  {
    WAIT_STATE(ch, skill_table[gsn_ferocious_strike]->beats);
    act(AT_GREEN, "You deliver a ferocious strike to $N!", ch, NULL, victim, TO_CHAR);
    act(AT_GREEN, "Your vision blurs from $n's ferocious strike!", ch, NULL, victim, TO_VICT);
    learn_from_success(ch, gsn_ferocious_strike);
    ch->move = (ch->move - nomore);
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/strike.wav)\r\n", ch);

    global_retcode = damage(ch, victim, ludicrous, gsn_ferocious_strike);

  }
  else
  {
    learn_from_failure(ch, gsn_ferocious_strike);
    global_retcode = damage(ch, victim, 0, gsn_ferocious_strike);
    send_to_char("&cYour ferocious strike just misses it's target.\r\n", ch);
  }
}

// Start of Shadow Knight skills
void do_knee(CHAR_DATA *ch, char *argument)
{
  short                   nomore;

  nomore = 10;
  CHAR_DATA              *victim;
  char                    arg[MIL];

  argument = one_argument(argument, arg);

  if(arg[0] == '\0')
  {
    if(ch->fighting)
      victim = who_fighting(ch);
    else
    {
      send_to_char("Send a high knee strike at who?\r\n", ch);
      return;
    }
  }
  else if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(ch->move < nomore)
  {
    send_to_char("You do not have enough move to do that.\r\n", ch);
    return;
  }

  if(ch == victim)
  {
    send_to_char("Suicide is a mortal sin.\r\n", ch);
    return;
  }

  if(ch->mount)
  {
    send_to_char("You cannot deliver a high knee while mounted.\r\n", ch);
    return;
  }

  send_to_char("\r\n\r\n&OYou leap up high in the air!\r\n", ch);
  if(can_use_skill(ch, number_percent(), gsn_knee))
  {
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/knee.wav)\r\n", ch);

    WAIT_STATE(ch, skill_table[gsn_knee]->beats);
    act(AT_GREEN, "$n leaps into the air!!", ch, NULL, victim, TO_VICT);
    learn_from_success(ch, gsn_knee);
    ch->move = (ch->move - nomore);
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/knee.wav)\r\n", ch);

    if(ch->Class != CLASS_MONK)
      global_retcode = damage(ch, victim, medium, gsn_knee);
    else
      global_retcode = damage(ch, victim, mediumhigh, gsn_knee);

  }
  else
  {
    learn_from_failure(ch, gsn_knee);
    global_retcode = damage(ch, victim, 0, gsn_knee);
    send_to_char("&cYour high knee strike just misses it's target.\r\n", ch);
    WAIT_STATE(ch, skill_table[gsn_knee]->beats);

  }
}

void do_chilled_touch(CHAR_DATA *ch, char *argument)
{
  short                   nomore;

  nomore = 10;
  CHAR_DATA              *victim;
  char                    arg[MIL];

  argument = one_argument(argument, arg);

  if(arg[0] == '\0')
  {
    if(ch->fighting)
      victim = who_fighting(ch);
    else
    {
      send_to_char("Use a chilled touch at who?\r\n", ch);
      return;
    }
  }
  else if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(ch->move < nomore)
  {
    send_to_char("You do not have enough move to do that.\r\n", ch);
    return;
  }

  if(ch == victim)
  {
    send_to_char("Suicide is a mortal sin.\r\n", ch);
    return;
  }

  send_to_char("\r\n\r\n&WYour hands turn white from cold!\r\n", ch);
  if(can_use_skill(ch, number_percent(), gsn_chilled_touch))
  {
    WAIT_STATE(ch, skill_table[gsn_chilled_touch]->beats);
    act(AT_CYAN, "Your chilled touch BURNS $N's flesh!", ch, NULL, victim, TO_CHAR);
    act(AT_CYAN, "Your skin literally burns from $n's chilled touch!", ch, NULL, victim, TO_VICT);
    learn_from_success(ch, gsn_chilled_touch);
    ch->move = (ch->move - nomore);
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/chilled.wav)\r\n", ch);
    global_retcode = damage(ch, victim, mediumhigh, gsn_chilled_touch);

  }
  else
  {
    learn_from_failure(ch, gsn_chilled_touch);
    global_retcode = damage(ch, victim, 0, gsn_chilled_touch);
    send_to_char("&cYour chilled touch just misses it's target.\r\n", ch);
  }
}

void do_frost_touch(CHAR_DATA *ch, char *argument)
{
  short                   nomore;

  nomore = 10;
  CHAR_DATA              *victim;
  char                    arg[MIL];

  argument = one_argument(argument, arg);

  if(arg[0] == '\0')
  {
    if(ch->fighting)
      victim = who_fighting(ch);
    else
    {
      send_to_char("Use a frost touch at who?\r\n", ch);
      return;
    }
  }
  else if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(ch->move < nomore)
  {
    send_to_char("You do not have enough move to do that.\r\n", ch);
    return;
  }

  if(ch == victim)
  {
    send_to_char("Suicide is a mortal sin.\r\n", ch);
    return;
  }
  send_to_char("\r\n\r\n&WYour hands turn white from cold!\r\n", ch);
  if(can_use_skill(ch, number_percent(), gsn_frost_touch))
  {
    WAIT_STATE(ch, skill_table[gsn_frost_touch]->beats);
    act(AT_CYAN, "Your frost touch BURNS $N's flesh!", ch, NULL, victim, TO_CHAR);
    act(AT_CYAN, "Your skin literally burns from $n's frost touch!", ch, NULL, victim, TO_VICT);
    learn_from_success(ch, gsn_frost_touch);
    ch->move = (ch->move - nomore);
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/frost.wav)\r\n", ch);
    global_retcode = damage(ch, victim, high, gsn_frost_touch);

  }
  else
  {
    learn_from_failure(ch, gsn_frost_touch);
    global_retcode = damage(ch, victim, 0, gsn_frost_touch);
    send_to_char("&cYour frost touch just misses it's target.\r\n", ch);
  }
}

void do_icy_touch(CHAR_DATA *ch, char *argument)
{
  short                   nomore;

  nomore = 10;
  CHAR_DATA              *victim;
  char                    arg[MIL];

  argument = one_argument(argument, arg);

  if(arg[0] == '\0')
  {
    if(ch->fighting)
      victim = who_fighting(ch);
    else
    {
      send_to_char("Use a icy touch at who?\r\n", ch);
      return;
    }
  }
  else if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(ch->move < nomore)
  {
    send_to_char("You do not have enough move to do that.\r\n", ch);
    return;
  }

  if(ch == victim)
  {
    send_to_char("Suicide is a mortal sin.\r\n", ch);
    return;
  }
  send_to_char("\r\n\r\n&WYour hands turn white from cold!\r\n", ch);
  if(can_use_skill(ch, number_percent(), gsn_icy_touch))
  {
    WAIT_STATE(ch, skill_table[gsn_icy_touch]->beats);
    act(AT_CYAN, "Your icy touch BURNS $N's flesh!", ch, NULL, victim, TO_CHAR);
    act(AT_CYAN, "Your skin literally burns from $n's icy touch!", ch, NULL, victim, TO_VICT);

    learn_from_success(ch, gsn_icy_touch);
    ch->move = (ch->move - nomore);
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/ice.wav)\r\n", ch);
    global_retcode = damage(ch, victim, extrahigh, gsn_icy_touch);

  }
  else
  {
    learn_from_failure(ch, gsn_icy_touch);
    global_retcode = damage(ch, victim, 0, gsn_icy_touch);
    send_to_char("&cYour icy touch just misses it's target.\r\n", ch);
  }
}

void do_tame(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA              *victim;

  if(xIS_SET(ch->act, PLR_BOUGHT_PET))
  {
    send_to_char("You canot gain affinity with more then one pet at a time.\r\n", ch);
    return;
  }

  if(!IN_WILDERNESS(ch))
  {
    send_to_char("You may only tame beasts from the wilderness.\r\n", ch);
    return;
  }

  if(ch->fighting)
    victim = who_fighting(ch);
  else
  {
    send_to_char("You can't tame a animal your not fighting.\r\n", ch);
    return;
  }

  if(victim->hit > victim->max_hit / 2)
  {
    send_to_char("They are not weakened enough to try to tame yet.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_tame))
  {
    WAIT_STATE(ch, skill_table[gsn_tame]->beats);
    add_follower(victim, ch);
    xSET_BIT(ch->act, PLR_BOUGHT_PET);
    xSET_BIT(victim->act, ACT_BEASTPET);
    xSET_BIT(victim->act, ACT_PET);
    xSET_BIT(victim->affected_by, AFF_CHARM);
    xSET_BIT(victim->affected_by, AFF_INFRARED);
    xSET_BIT(victim->attacks, ATCK_BITE);
    xSET_BIT(victim->defenses, DFND_DODGE);
    xSET_BIT(victim->xflags, PART_GUTS);
    xREMOVE_BIT(victim->act, ACT_AGGRESSIVE);
    xREMOVE_BIT(victim->act, ACT_WILD_AGGR);
    ch->pcdata->charmies++;
    ch->pcdata->pet = victim;
    ch->pcdata->petlevel = victim->level;
    ch->pcdata->petaffection = 10;
    ch->pcdata->pethungry = 950;
    ch->pcdata->petthirsty = 950;
    victim->hit = ch->max_hit + get_curr_con(ch) * 3;
    victim->max_hit = victim->hit + get_curr_con(ch) * 3;
    victim->move = ch->max_move;
    victim->max_move = victim->move;
    xSET_BIT(victim->affected_by, AFF_TRUESIGHT);
    stop_fighting(victim, TRUE);
    stop_hunting(victim);
    stop_hating(victim);
    victim->race = RACE_ANIMAL;
    ch->hate_level = 0;
    ch = victim->master;

    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/whip.wav)\r\n", ch);

    act(AT_BLUE, "\r\nYou have weakened $N to the point, $E can be tamed.\r\n", ch, NULL, victim, TO_CHAR);
    act(AT_BLUE, "$n has weakened $N to the point $E can be tamed.\r\n", ch, NULL, victim, TO_ROOM);
    learn_from_success(ch, gsn_tame);
    return;
  }
  else
  {
    act(AT_BLUE, "\r\nYou try to tame a ferral $N, but fail.\r\n", ch, NULL, victim, TO_CHAR);
    act(AT_BLUE, "$n tries to tame a ferral $N, but fails.\r\n", ch, NULL, victim, TO_ROOM);
    learn_from_failure(ch, gsn_tame);
  }
  return;
}

void do_mist_acid(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA              *vch;
  CHAR_DATA              *vch_next;
  bool                    ch_died;

  ch_died = FALSE;

  if(IS_SET(ch->in_room->room_flags, ROOM_SAFE))
  {
    return;
  }

  if(xIS_SET(ch->act, PLR_BATTLE))
    send_to_char("!!SOUND(sound/mistacid.wav)\r\n", ch);

  act(AT_GREEN, "You raise your arms and cause a green mist to fill the area!", ch, NULL, NULL, TO_CHAR);
  act(AT_GREEN, "$n raises $s arms and causes a green mist to fill the area!", ch, NULL, NULL, TO_ROOM);

  if(can_use_skill(ch, number_percent(), gsn_mist_acid) || IS_NPC(ch))
  {
    for(vch = ch->in_room->first_person; vch; vch = vch_next)
    {
      vch_next = vch->next_in_room;

      if(IS_NPC(vch) && (vch->pIndexData->vnum == MOB_VNUM_SOLDIERS || vch->pIndexData->vnum == MOB_VNUM_ARCHERS))
        continue;

      // Bug fix here, was striking grouped members. -Taon
      if(is_same_group(vch, ch))
        continue;

      if(!IS_NPC(vch) && xIS_SET(vch->act, PLR_WIZINVIS) && vch->pcdata->wizinvis >= LEVEL_IMMORTAL)
        continue;

      if(IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch))
      {
        if(char_died(ch))
        {
          ch_died = TRUE;
          break;
        }

        global_retcode = damage(ch, vch, high, gsn_mist_acid);
      }
    }

    learn_from_success(ch, gsn_mist_acid);
    return;
    if(!vch->fighting)
      global_retcode = damage(ch, vch, 0, gsn_mist_acid);
  }
  else
    learn_from_failure(ch, gsn_mist_acid);
  return;
}

void do_wolf_spirit(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;
  CHAR_DATA              *mob;
  bool                    found;

  found = FALSE;
  for(mob = first_char; mob; mob = mob->next)
  {
    if(mob)
    {
      if(IS_NPC(mob) && mob->in_room && ch->in_room)
      {
        if(mob->master)
        {
          if(!str_cmp(ch->name, mob->master->name) && (mob->in_room == ch->in_room))
          {
            found = TRUE;
            break;
          }
        }
      }
    }
  }

  if(can_use_skill(ch, number_percent(), gsn_wolf_spirit))
  {
    affect_strip(ch, gsn_wolf_spirit);
    af.type = gsn_wolf_spirit;
    af.duration = ch->level + 200;
    af.location = APPLY_MOVE;
    af.modifier = get_curr_dex(ch) * 200;
    af.level = ch->level;
    xCLEAR_BITS(af.bitvector);
    affect_to_char(ch, &af);

    if(found == TRUE)
    {
      affect_strip(mob, gsn_wolf_spirit);
      af.type = gsn_wolf_spirit;
      af.duration = ch->level + 200;
      af.location = APPLY_MOVE;
      af.modifier = get_curr_dex(ch) * 200;
      af.level = ch->level;
      xCLEAR_BITS(af.bitvector);
      affect_to_char(mob, &af);
    }

    learn_from_success(ch, gsn_wolf_spirit);
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/wolfspirit.wav)\r\n", ch);
    act(AT_BLUE, "You call on the spirit of the wolf!\r\n", ch, NULL, NULL, TO_CHAR);
    if(mob)
      act(AT_BLUE, "Your eyes narrow as the spirit of the wolf fills you!\r\n", ch, NULL, mob, TO_VICT);
    return;
  }
  else
  {
    act(AT_PLAIN, "Your attempt using a wolf spirit fails to fully work.", ch, NULL, NULL, TO_CHAR);
    learn_from_failure(ch, gsn_wolf_spirit);
  }
  return;
}

void do_bull_spirit(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;
  CHAR_DATA              *mob;
  bool                    found;

  found = FALSE;
  for(mob = first_char; mob; mob = mob->next)
  {
    if(mob)
    {
      if(IS_NPC(mob) && mob->in_room && ch->in_room)
      {
        if(mob->master)
        {
          if(!str_cmp(ch->name, mob->master->name) && (mob->in_room == ch->in_room))
          {
            found = TRUE;
            break;
          }
        }
      }
    }
  }

  if(can_use_skill(ch, number_percent(), gsn_bull_spirit))
  {
    affect_strip(ch, gsn_bull_spirit);
    af.type = gsn_bull_spirit;
    af.duration = ch->level + 200;
    af.location = APPLY_CON;
    af.modifier = 1 + ch->level / 17;
    af.level = ch->level;
    xCLEAR_BITS(af.bitvector);
    affect_to_char(ch, &af);

    if(xIS_SET(ch->act, PLR_BATTLE) || (xIS_SET(ch->act, ACT_BEASTMELD) && xIS_SET(ch->master->act, PLR_BATTLE)))
      send_to_char("!!SOUND(sound/bull.wav)\r\n", ch);

    if(found == TRUE)
    {
      affect_strip(mob, gsn_bull_spirit);
      af.type = gsn_bull_spirit;
      af.duration = ch->level + 200;
      af.location = APPLY_CON;
      af.modifier = 1 + ch->level / 17;
      af.level = ch->level;
      xCLEAR_BITS(af.bitvector);
      affect_to_char(mob, &af);
    }

    learn_from_success(ch, gsn_bull_spirit);
    act(AT_PLAIN, "Your bull spirit fills your being.", ch, NULL, NULL, TO_CHAR);
    return;
  }
  else
  {
    act(AT_PLAIN, "Your attempt using a bull spirit fails to fully work.", ch, NULL, NULL, TO_CHAR);
    learn_from_failure(ch, gsn_bull_spirit);
  }
  return;
}

void do_bear_spirit(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;
  CHAR_DATA              *mob;
  bool                    found;

  found = FALSE;
  for(mob = first_char; mob; mob = mob->next)
  {
    if(mob)
    {
      if(IS_NPC(mob) && mob->in_room && ch->in_room)
      {
        if(mob->master)
        {
          if(!str_cmp(ch->name, mob->master->name) && (mob->in_room == ch->in_room))
          {
            found = TRUE;
            break;
          }
        }
      }
    }
  }

  if(can_use_skill(ch, number_percent(), gsn_bear_spirit))
  {
    affect_strip(ch, gsn_bear_spirit);
    af.type = gsn_bear_spirit;
    af.duration = ch->level + 200;
    af.location = APPLY_STR;
    af.modifier = 1 + ch->level / 17;
    af.level = ch->level;
    xCLEAR_BITS(af.bitvector);
    affect_to_char(ch, &af);

    if(xIS_SET(ch->act, PLR_BATTLE) || (xIS_SET(ch->act, ACT_BEASTMELD) && xIS_SET(ch->master->act, PLR_BATTLE)))
      send_to_char("!!SOUND(sound/bear.wav)\r\n", ch);

    if(found == TRUE)
    {
      affect_strip(mob, gsn_bear_spirit);
      af.type = gsn_bear_spirit;
      af.duration = ch->level + 200;
      af.location = APPLY_STR;
      af.modifier = 1 + ch->level / 17;
      af.level = ch->level;
      xCLEAR_BITS(af.bitvector);
      affect_to_char(mob, &af);
    }

    learn_from_success(ch, gsn_bear_spirit);
    act(AT_PLAIN, "Your bear spirit fills your being.", ch, NULL, NULL, TO_CHAR);
    return;
  }
  else
  {
    act(AT_PLAIN, "Your attempt using a bear spirit fails to fully work.", ch, NULL, NULL, TO_CHAR);
    learn_from_failure(ch, gsn_bear_spirit);
  }
  return;

}

void do_shroud_spirit(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;
  CHAR_DATA              *mob;
  bool                    found;

  found = FALSE;
  for(mob = first_char; mob; mob = mob->next)
  {
    if(mob)
    {
      if(IS_NPC(mob) && mob->in_room && ch->in_room)
      {
        if(mob->master)
        {
          if(!str_cmp(ch->name, mob->master->name) && (mob->in_room == ch->in_room))
          {
            found = TRUE;
            break;
          }
        }
      }
    }
  }

  if(can_use_skill(ch, number_percent(), gsn_shroud_spirit))
  {
    if(xIS_SET(ch->act, PLR_BATTLE) || (xIS_SET(ch->act, ACT_BEASTMELD) && xIS_SET(ch->master->act, PLR_BATTLE)))
      send_to_char("!!SOUND(sound/shroud.wav)\r\n", ch);

    affect_strip(ch, gsn_shroud_spirit);
    xREMOVE_BIT(ch->affected_by, AFF_HIDE);
    af.type = gsn_shroud_spirit;
    af.duration = ch->level + 200;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.level = ch->level;
    af.bitvector = meb(AFF_HIDE);
    affect_to_char(ch, &af);

    if(found == TRUE)
    {
      affect_strip(mob, gsn_shroud_spirit);
      xREMOVE_BIT(mob->affected_by, AFF_HIDE);
      af.type = gsn_shroud_spirit;
      af.duration = ch->level + 200;
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.level = ch->level;
      af.bitvector = meb(AFF_HIDE);
      affect_to_char(mob, &af);
    }

    learn_from_success(ch, gsn_shroud_spirit);
    act(AT_PLAIN, "Your shroud spirit sends everyone into a hidden position.", ch, NULL, NULL, TO_CHAR);
    return;
  }
  else
  {
    act(AT_PLAIN, "Your attempt at a shroud spirit fails to fully work.", ch, NULL, NULL, TO_CHAR);
    learn_from_failure(ch, gsn_shroud_spirit);
  }

}

/*
void do_hog_affinity( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA            *paf;
    int                     sn = 1;
    MOB_INDEX_DATA         *temp;
    CHAR_DATA              *mob;
    AFFECT_DATA             af;
    char                    buf[MSL];
    char                   *name;
    bool                    found;

    if ( IS_NPC( ch ) ) {
        return;
    }

    if ( ( temp = get_mob_index( MOB_VNUM_LSKELLIE ) ) == NULL ) {
        bug( "Skill_Hog_affinity: Hog vnum %d doesn't exist.", MOB_VNUM_LSKELLIE );
        return;
    }

    found = FALSE;
    for ( mob = first_char; mob; mob = mob->next ) {
        if ( IS_NPC( mob ) && mob->in_room && ch == mob->master ) {
            found = TRUE;
            break;
        }
    }

    if ( xIS_SET( ch->act, PLR_BOUGHT_PET ) && found == TRUE ) {
        send_to_char( "You already have a pet, dismiss it first.\r\n", ch );
        return;
    }

    if ( can_use_skill( ch, number_percent(  ), gsn_hog_affinity ) ) {
        WAIT_STATE( ch, skill_table[gsn_hog_affinity]->beats );
        mob = create_mobile( temp );
        char_to_room( mob, ch->in_room );
        mob->race = 82;
        mob->level = ch->level;
        mob->hit = ch->hit;
        mob->max_hit = mob->hit;
        if ( ch->level < 20 ) {
            mob->move = ch->level * 100;
        }
        else if ( ch->level > 19 && ch->level < 50 ) {
            mob->move = ch->level * 150;
        }
        else {
            mob->move = ch->level * 200;
        }
        mob->max_move = mob->move;
        mob->perm_str = 20;
        mob->sex = 2;
        mob->armor = set_armor_class( mob->level );
        mob->hitroll = set_hitroll( mob->level );
        mob->damroll = set_damroll( mob->level );
        mob->numattacks = set_num_attacks( mob->level );
        mob->hitplus = set_hp( mob->level );
        mob->alignment = ch->alignment;
        mudstrlcpy( buf, "feral hog", MSL );
        STRFREE( mob->name );
        mob->name = STRALLOC( buf );
        if ( VLD_STR( mob->short_descr ) )
            STRFREE( mob->short_descr );
        mob->short_descr = STRALLOC( "a feral hog" );
        snprintf( buf, MSL, "A feral hog is rooting around here.\r\n" );
        if ( VLD_STR( mob->description ) )
            STRFREE( mob->long_descr );
        mob->long_descr = STRALLOC( buf );
        mob->color = 1;
        act( AT_BLUE, "\r\nYou rely on your hog affinity to call yourself a feral hog.\r\n", ch,
             NULL, NULL, TO_CHAR );
        act( AT_BLUE, "$n makes some grunting sounds, and a feral hog comes to $s side.\r\n", ch,
             NULL, NULL, TO_ROOM );
        add_follower( mob, ch );
        xSET_BIT( ch->act, PLR_BOUGHT_PET );
        xSET_BIT( mob->act, ACT_PET );
        xSET_BIT( mob->affected_by, AFF_CHARM );
        xSET_BIT( mob->attacks, ATCK_BITE );
        xSET_BIT( mob->defenses, DFND_DODGE );
        xSET_BIT( mob->xflags, PART_GUTS );
        learn_from_success( ch, gsn_hog_affinity );
        ch->pcdata->charmies++;
        ch->pcdata->pet = mob;
        xSET_BIT( mob->affected_by, AFF_TRUESIGHT );
        return;
    }
    else {
        act( AT_CYAN, "You make some grunting noises.\r\n", ch, NULL, NULL, TO_CHAR );
        act( AT_CYAN, "$n makes some grunting noises.\r\n", ch, NULL, NULL, TO_ROOM );
        send_to_char( "You lose your concentration and fail to call out to a feral hog.\r\n", ch );
        learn_from_failure( ch, gsn_hog_affinity );
        return;
    }
    return;
}
*/
void do_steed(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA            *paf;
  int                     sn = 1;
  MOB_INDEX_DATA         *temp;
  CHAR_DATA              *mob;
  AFFECT_DATA             af;
  char                    buf[MSL];
  OBJ_DATA               *obj;
  char                   *name;
  bool                    found;

  if(IS_NPC(ch))
  {
    return;
  }

  if((temp = get_mob_index(MOB_VNUM_LSKELLIE)) == NULL)
  {
    bug("Skill_Steed: Steed vnum %d doesn't exist.", MOB_VNUM_LSKELLIE);
    return;
  }

  found = FALSE;
  for(mob = first_char; mob; mob = mob->next)
  {
    if(IS_NPC(mob) && mob->in_room && ch == mob->master)
    {
      found = TRUE;
      break;
    }
  }

  if(xIS_SET(ch->act, PLR_BOUGHT_PET) && found == TRUE)
  {
    send_to_char("You already have a pet, dismiss it first.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_steed))
  {
    WAIT_STATE(ch, skill_table[gsn_steed]->beats);
    mob = create_mobile(temp);
    char_to_room(mob, ch->in_room);
    mob->race = 82;
    mob->level = ch->level;
    mob->hit = set_hp(mob->level);
    mob->max_hit = set_hp(mob->level);
    mob->hit = mob->hit * 4;
    mob->max_hit = mob->max_hit * 4;
    if(ch->level < 20)
    {
      mob->move = ch->level * 100;
    }
    else if(ch->level > 19 && ch->level < 50)
    {
      mob->move = ch->level * 150;
    }
    else
    {
      mob->move = ch->level * 200;
    }
    mob->max_move = mob->move;
    mob->perm_str = 20;
    mob->sex = 2;
    mob->armor = set_armor_class(mob->level);
    mob->hitroll = set_hitroll(mob->level);
    mob->damroll = set_damroll(mob->level);
    mob->numattacks = set_num_attacks(mob->level);
    mob->hitplus = set_hp(mob->level);
    mob->alignment = ch->alignment;
    mudstrlcpy(buf, "warhorse war horse", MSL);
    STRFREE(mob->name);
    mob->name = STRALLOC(buf);
    if(VLD_STR(mob->short_descr))
      STRFREE(mob->short_descr);
    if(ch->level < 20)
    {
      mob->short_descr = STRALLOC("a light warhorse");
      snprintf(buf, MSL, "A light warhorse stands here.\r\n");
    }
    else if(ch->level > 19 && ch->level < 50)
    {
      mob->short_descr = STRALLOC("a medium warhorse");
      snprintf(buf, MSL, "A medium warhorse stands here.\r\n");
    }
    else
    {
      mob->short_descr = STRALLOC("a heavy warhorse");
      snprintf(buf, MSL, "A heavy warhorse stands here.\r\n");
    }
    if(VLD_STR(mob->description))
      STRFREE(mob->long_descr);
    mob->long_descr = STRALLOC(buf);
    mob->color = 1;
    act(AT_BLUE, "\r\nYou call out to your warhorse steed.\r\n", ch, NULL, NULL, TO_CHAR);
    act(AT_BLUE, "$n utters some arcane words, and a warhorse steed comes to $s side.\r\n", ch, NULL, NULL, TO_ROOM);
    add_follower(mob, ch);
    xSET_BIT(ch->act, PLR_BOUGHT_PET);
    xSET_BIT(mob->act, ACT_PET);
    xSET_BIT(mob->affected_by, AFF_CHARM);
    xSET_BIT(mob->act, ACT_MOUNTABLE);
    xSET_BIT(mob->attacks, ATCK_BITE);
    xSET_BIT(mob->defenses, DFND_DODGE);
    xSET_BIT(mob->xflags, PART_GUTS);
    learn_from_success(ch, gsn_steed);
    ch->pcdata->charmies++;
    ch->pcdata->pet = mob;
    xSET_BIT(mob->affected_by, AFF_TRUESIGHT);
    obj = create_object(get_obj_index(1281), 0);
    obj->level = mob->level;
    if(mob->level > 19)
    {
      // affect 1
      CREATE(paf, AFFECT_DATA, 1);

      paf->type = sn;
      paf->duration = -1;
      paf->location = APPLY_NONE;
      paf->modifier = 0;
      paf->bitvector = meb(AFF_AQUA_BREATH);
      LINK(paf, obj->first_affect, obj->last_affect, next, prev);
    }
    if(mob->level > 49)
    {
      // affect 2
      CREATE(paf, AFFECT_DATA, 1);

      paf->type = sn;
      paf->duration = -1;
      paf->location = APPLY_NONE;
      paf->modifier = 0;
      paf->bitvector = meb(AFF_PROTECT);
      LINK(paf, obj->first_affect, obj->last_affect, next, prev);
    }
    obj_to_char(obj, mob);
    equip_char(mob, obj, WEAR_WAIST);

    return;
  }
  else
  {
    act(AT_CYAN, "You utter some arcane words.\r\n", ch, NULL, NULL, TO_CHAR);
    act(AT_CYAN, "$n utters some arcane words.\r\n", ch, NULL, NULL, TO_ROOM);
    send_to_char("You speak an incorrect phrase and fail to call to your steed.\r\n", ch);
    learn_from_failure(ch, gsn_steed);
    return;
  }
  return;
}

void do_mend(CHAR_DATA *ch, char *argument)
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

  heal_rate = (ch->level * 4);

  if(ch->level > 40)
    heal_rate = ch->level * 5;

  if(ch->level > 69 && ch->level < 80)
    heal_rate = ch->level * 6;

  if(ch->level > 79)
    heal_rate = ch->level * 8;

  heal_rate += number_range(get_curr_wis(ch), (get_curr_wis(ch) * 5));

  if(heal_rate > 1500 && ch->level > 70)
    heal_rate = 1500;
  else if(heal_rate > 1000 && ch->level < 71)
    heal_rate = 1000;

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

  WAIT_STATE(ch, skill_table[gsn_mend]->beats);
  if(!ch->fighting)
    WAIT_STATE(ch, 5);
  if(can_use_skill(ch, number_percent(), gsn_mend))
  {
    ch->mana -= 30;
    if(!ch->fighting)
      ch->mana -= 20;

    if(ch->fighting)
    {
      if(victim == ch)
        ch_printf(ch, "&rYou focus your power on mending your wounds.\r\n");
      else
        ch_printf(ch, "&rYou focus your power on mending the wounds of %s.\r\n", victim->name);
    }
    if(!ch->fighting)
    {
      if(victim == ch)
        ch_printf(ch, "&rYou take your time and focus your power on mending your wounds.\r\n");
      else
        ch_printf(ch, "&rYou take your time and focus your power on mending the wounds of %s.\r\n", victim->name);
    }

    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/mend.wav)\r\n", ch);
    if(ch->fighting)
    {
      if(victim != ch)
        ch_printf(victim, "&r%s begins to mend your wounds.\r\n", ch->name);
      act(AT_BLOOD, "$n concentrates as $N's wounds begin to mend.", ch, NULL, victim, TO_ROOM);
    }
    else
    {
      if(victim != ch)
        ch_printf(victim, "&r%s takes the proper time and begins to mend your wounds.\r\n", ch->name);
      act(AT_BLOOD, "$n takes the proper time and concentrates as $N's wounds being to mend.", ch, NULL, victim, TO_ROOM);
    }
    victim->hit += heal_rate;
    learn_from_success(ch, gsn_mend);
  }
  else
  {
    send_to_char("You attempt to focus your power, but get distracted.\r\n", ch);
    learn_from_failure(ch, gsn_mend);
  }
  return;
}

void do_combat_heal(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA              *victim;
  short                   nomore;

  if(ch->level < 10)
    nomore = 5;
  else if(ch->level > 9)
    nomore = 10;

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

  // Prevent ch from healing mob they're fighting. -Taon
  if(IS_NPC(victim) && who_fighting(ch) == victim)
  {
    send_to_char("You cant bring yourself to heal your foe.\r\n", ch);
    return;
  }

  if(ch->mana < 10)
  {
    send_to_char("You don't have enough mana to do that.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_combat_heal))
  {
    WAIT_STATE(ch, 12);
    if(victim->hit >= victim->max_hit)
    {
      send_to_char("There is no wound to combat heal.\r\n", ch);
      return;
    }

    if(victim->hit < victim->max_hit)
    {
      if(xIS_SET(ch->act, PLR_BATTLE))
        send_to_char("!!SOUND(sound/combatheal.wav)\r\n", ch);
      ch->mana -= nomore;
      act(AT_CYAN, "You combat heal the wound.", ch, NULL, victim, TO_CHAR);
      if(victim == ch)
      {
        act(AT_CYAN, "You combat heal your wounds.", ch, NULL, victim, TO_CHAR);
      }
      else
      {
        act(AT_CYAN, "$n begins to combat heal your wounds.", ch, NULL, victim, TO_VICT);
      }
      act(AT_CYAN, "$n combat heals $N's wounds.", ch, NULL, victim, TO_NOTVICT);
      victim->hit += 200;
      if(victim->hit > victim->max_hit)
        victim->hit = victim->max_hit;
      learn_from_success(ch, gsn_combat_heal);
      return;
    }
  }
  else
  {
    act(AT_CYAN, "You attempt to combat heal wounds, but get distracted.", ch, NULL, victim, TO_CHAR);
    learn_from_failure(ch, gsn_combat_heal);
    WAIT_STATE(ch, 12);
    return;
  }

}

void do_sylvan_touch(CHAR_DATA *ch, char *argument)
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

  heal_rate = (ch->level * 4);

  if(ch->level > 40)
    heal_rate = ch->level * 5;

  if(ch->level > 69 && ch->level < 80)
    heal_rate = ch->level * 6;

  if(ch->level > 79)
    heal_rate = ch->level * 8;

  heal_rate += number_range(get_curr_wis(ch), (get_curr_wis(ch) * 5));

  if(heal_rate > 1500 && ch->level > 70)
    heal_rate = 1500;
  else if(heal_rate > 1000 && ch->level < 71)
    heal_rate = 1000;

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

  WAIT_STATE(ch, skill_table[gsn_sylvan_touch]->beats);
  if(!ch->fighting)
    WAIT_STATE(ch, 5);
  if(can_use_skill(ch, number_percent(), gsn_sylvan_touch))
  {
    ch->mana -= 30;
    if(!ch->fighting)
      ch->mana -= 20;

    if(ch->fighting)
    {
      if(victim == ch)
        ch_printf(ch, "&GYou summon the healing energies of sylvan touch.\r\n");
      else
        ch_printf(ch, "&GYou summon the healing energies of sylvan touch to wash over %s.\r\n", victim->name);
    }
    if(!ch->fighting)
    {
      if(victim == ch)
        ch_printf(ch, "&GYou take your time and summon the healing energies of sylvan touch.\r\n");
      else
        ch_printf(ch, "&GYou take your time and summon the healing energies of sylvan touch to wash over %s.\r\n", victim->name);
    }

    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/sylvan.wav)\r\n", ch);
    if(ch->fighting)
    {
      if(victim != ch)
        ch_printf(victim, "&G%s summons the healing energies of sylvan touch to wash over you.\r\n", ch->name);
      act(AT_GREEN, "$n summons the healing energies of sylvan touch to wash over $N.", ch, NULL, victim, TO_ROOM);
    }
    else
    {
      if(victim != ch)
        ch_printf(victim, "&G%s takes the proper time and summons the healing energies to wash over you.\r\n", ch->name);
      act(AT_GREEN, "$n takes the proper time and summons the healing energies of sylvan touch.", ch, NULL, victim, TO_ROOM);
    }
    victim->hit += heal_rate;
    learn_from_success(ch, gsn_sylvan_touch);
  }
  else
  {
    send_to_char("You attempt to summon the healing energies of sylvan touch but get distracted.\r\n", ch);
    learn_from_failure(ch, gsn_sylvan_touch);
  }
  return;
}

void do_effloresce(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA              *victim;
  short                   nomore;

  if(ch->level < 10)
    nomore = 5;
  else if(ch->level > 9)
    nomore = 10;

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

  // Prevent ch from healing mob they're fighting. -Taon
  if(IS_NPC(victim) && who_fighting(ch) == victim)
  {
    send_to_char("You cant bring yourself to heal your foe.\r\n", ch);
    return;
  }

  if(ch->mana < 10)
  {
    send_to_char("You don't have enough mana to do that.\r\n", ch);
    return;
  }
  if(can_use_skill(ch, number_percent(), gsn_effloresce))
  {
    WAIT_STATE(ch, skill_table[gsn_effloresce]->beats);
    if(victim->hit >= victim->max_hit)
    {
      send_to_char("There is no wound to heal.\r\n", ch);
      return;
    }
    gain_exp(ch, heal_xp);
    if(victim->hit < victim->max_hit)
    {
      if(xIS_SET(ch->act, PLR_BATTLE))
        send_to_char("!!SOUND(sound/effloresce.wav)\r\n", ch);
      ch->mana -= nomore;
      act(AT_GREEN, "You summon healing energies from the plant life.", ch, NULL, victim, TO_CHAR);
      if(victim != ch)
      {
        act(AT_GREEN, "$n sends healing energies into you.", ch, NULL, victim, TO_VICT);
      }
      act(AT_GREEN, "$n summons healing energies.", ch, NULL, victim, TO_NOTVICT);
      victim->hit += (100 + (get_curr_wis(ch) * 3));
      if(victim->hit > victim->max_hit)
        victim->hit = victim->max_hit;
      learn_from_success(ch, gsn_effloresce);
      return;
    }
  }
  else
  {
    act(AT_CYAN, "You attempt to summon healing energies, but get distracted.", ch, NULL, victim, TO_CHAR);
    learn_from_failure(ch, gsn_effloresce);
    WAIT_STATE(ch, skill_table[gsn_effloresce]->beats);
    return;
  }
}

void do_tend(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA              *victim;
  short                   nomore = 3;

  if(ch->mana < 5)
  {
    send_to_char("You don't have enough mana to do that.\r\n", ch);
    return;
  }
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

  if(victim->hit >= victim->max_hit)
  {
    send_to_char("There is no wound to tend.\r\n", ch);
    return;
  }

  // Lets stop the healing of mobs whom the character is fighting. -Taon
  if(IS_NPC(victim) && who_fighting(ch) == victim)
  {
    send_to_char("You can't bring yourself to tend the wounds of your foe!\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_tend))
  {
    WAIT_STATE(ch, skill_table[gsn_tend]->beats);

    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/cure.wav)\r\n", ch);

    act(AT_MAGIC, "$N's tended wounds begin healing.", ch, NULL, victim, TO_NOTVICT);
    if(victim != ch)
    {
      act(AT_CYAN, "You focus on tending $N's wounds.", ch, NULL, victim, TO_CHAR);
      act(AT_CYAN, "Your wounds now tended to begin to heal.", ch, NULL, victim, TO_VICT);
    }
    else
      act(AT_CYAN, "Your wounds now tended to begin to heal.", ch, NULL, victim, TO_CHAR);
    if(victim->level < 10)
    {
      victim->hit += number_range(3, get_curr_wis(ch));
    }
    if(victim->level > 9 && victim->level < 20)
    {
      victim->hit += number_range(get_curr_wis(ch), get_curr_wis(ch) * 2);
    }
    if(victim->level > 19)
    {
      victim->hit += number_range(get_curr_wis(ch) * 2, get_curr_wis(ch) * 5);
    }

    ch->mana -= nomore;
    learn_from_success(ch, gsn_tend);
    return;
  }
  else
  {
    act(AT_CYAN, "You attempt to tend the wounds, but fail to concentrate properly.", ch, NULL, victim, TO_CHAR);
    learn_from_failure(ch, gsn_tend);
    WAIT_STATE(ch, skill_table[gsn_tend]->beats);
    return;
  }
}

void do_poultice(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA              *victim;
  short                   nomore;

  if(ch->level < 10)
    nomore = 2;
  else if(ch->level > 9)
    nomore = 5;

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

  // Prevent ch from healing mob they're fighting. -Taon
  if(IS_NPC(victim) && who_fighting(ch) == victim)
  {
    send_to_char("You cant bring yourself to heal your foe.\r\n", ch);
    return;
  }

  if(ch->mana < 10)
  {
    send_to_char("You don't have enough mana to do that.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_poultice))
  {
    WAIT_STATE(ch, skill_table[gsn_poultice]->beats);

    if(victim->hit >= victim->max_hit)
    {
      send_to_char("There is no wound to poultice.\r\n", ch);
      return;
    }

    if(victim->hit < victim->max_hit)
    {
      if(xIS_SET(ch->act, PLR_BATTLE))
        send_to_char("!!SOUND(sound/cure.wav)\r\n", ch);
      ch->mana -= nomore;
      act(AT_CYAN, "You gather the herbs necessary and create a poultice.", ch, NULL, victim, TO_CHAR);
      if(victim == ch)
      {
        act(AT_CYAN, "You apply the poultice to your wounds.", ch, NULL, victim, TO_CHAR);
      }
      else
      {
        act(AT_CYAN, "$n applies a poultice to your wounds.", ch, NULL, victim, TO_VICT);
      }
      act(AT_CYAN, "$n applies a poultice to $N's wounds.", ch, NULL, victim, TO_NOTVICT);
      if(victim->level < 10)
      {
        victim->hit += number_range(3, get_curr_wis(ch) + 10);
      }
      if(victim->level > 9 && victim->level < 20)
      {
        victim->hit += number_range((get_curr_wis(ch) + 10), get_curr_wis(ch) * 3);
      }
      if(victim->level > 19)
      {
        victim->hit += number_range(get_curr_wis(ch) * 2, get_curr_wis(ch) * 5);
      }

      if(victim->hit >= victim->max_hit)
      {
        victim->hit = victim->max_hit;
      }
      learn_from_success(ch, gsn_poultice);
      return;
    }
  }
  else
  {
    act(AT_CYAN, "You gather the herbs necessary and create a poultice, but don't mix it properly.", ch, NULL, victim, TO_CHAR);
    learn_from_failure(ch, gsn_poultice);
    WAIT_STATE(ch, skill_table[gsn_poultice]->beats);
    return;
  }

}

void do_butterfly_kick(CHAR_DATA *ch, char *argument)
{
  short                   nomore;

  nomore = 10;
  CHAR_DATA              *victim;
  char                    arg[MIL];

  argument = one_argument(argument, arg);

  if(arg[0] == '\0')
  {
    if(ch->fighting)
      victim = who_fighting(ch);
    else
    {
      send_to_char("Send a butterfly kick at who?\r\n", ch);
      return;
    }
  }

  else if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(ch->move < nomore)
  {
    send_to_char("You do not have enough move to do that.\r\n", ch);
    return;
  }

  if(ch == victim)
  {
    send_to_char("Suicide is a mortal sin.\r\n", ch);
    return;
  }

  if(ch->mount)
  {
    send_to_char("You cannot deliver a butterfly kick while mounted.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_butterfly_kick) || IS_NPC(ch))
  {
    WAIT_STATE(ch, skill_table[gsn_butterfly_kick]->beats);
    act(AT_GREEN, "You deliver a butterfly kick to $N!", ch, NULL, victim, TO_CHAR);

    act(AT_GREEN, "Your vision blurs from the ferocity of $n's butterfly kick!", ch, NULL, victim, TO_VICT);
    learn_from_success(ch, gsn_butterfly_kick);
    ch->move = (ch->move - nomore);
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/butterflykick.wav)\r\n", ch);

    global_retcode = damage(ch, victim, extrahigh, gsn_butterfly_kick);

  }

  else
  {
    learn_from_failure(ch, gsn_butterfly_kick);
    global_retcode = damage(ch, victim, 0, gsn_butterfly_kick);
    send_to_char("&cYour butterfly kick just misses it's target.\r\n", ch);
  }
}

void animal_change(CHAR_DATA *ch, bool tohuman)
{ /* tohuman=TRUE if going to human, * =FALSE if going to animal */
  short                   backup[MAX_SKILL];
  bool                    dshowbackup[MAX_SKILL];
  int                     sn = 0;
  short                   ability = 0;
  AFFECT_DATA             af;
  bool                    fForm = FALSE;
  MORPH_DATA             *morph;
  char                    buf[MSL];

  if(IS_NPC(ch) || ch->Class == CLASS_BEAST )
  {
    return;
  }

  if(ch->position == POS_SLEEPING)
  {
    send_to_char("You can't do animal_change if you are sleeping!", ch);
    return;
  }

  for(sn = 0; sn < MAX_SKILL; sn++)
  {
    dshowbackup[sn] = FALSE;
    backup[sn] = 0;
  }

  if(tohuman)
  {
    send_to_char("\r\nYou remove all your gear as your shape is about to vastly change.\r\n", ch);
    remove_all_equipment(ch);

    ch->pcdata->tmprace = ch->race;
    ch->pcdata->tmpclass = ch->Class;
    ch->race = RACE_ANIMAL;
    short                   num = 0;

    if(ch->pcdata->choice == 1)
      num = 1001;
    else if(ch->pcdata->choice == 2)
      num = 1004;
    else if(ch->pcdata->choice == 3)
      num = 1002;
    else if(ch->pcdata->choice == 4)
      num = 1003;
    else if(ch->pcdata->choice == 5)
      num = 1006;
    else if(ch->pcdata->choice == 6)
      num = 1007;
    else if(ch->pcdata->choice == 7)
      num = 1013;

    morph = get_morph_vnum(num);

    if(!morph)
    {
      send_to_char("No morph data stopped here.\r\n", ch);
      return;
    }

    if(ch->pcdata->choice == 1)
    {
      ch->Class = CLASS_WOLF;
      if(xIS_SET(ch->act, PLR_BATTLE))
        send_to_char("!!SOUND(sound/wolf.wav)\r\n", ch);
      morph->vnum = 1001;
    }
    else if(ch->pcdata->choice == 2)
    {
      ch->Class = CLASS_OWL;
      if(xIS_SET(ch->act, PLR_BATTLE))
        send_to_char("!!SOUND(sound/owl.wav)\r\n", ch);
      morph->vnum = 1004;
    }
    else if(ch->pcdata->choice == 3)
    {
      ch->Class = CLASS_FISH;
      if(xIS_SET(ch->act, PLR_BATTLE))
        send_to_char("!!SOUND(sound/fish.wav)\r\n", ch);
      morph->vnum = 1002;
    }
    else if(ch->pcdata->choice == 4)
    {
      ch->Class = CLASS_BEAR;
      if(xIS_SET(ch->act, PLR_BATTLE))
        send_to_char("!!SOUND(sound/bear.wav)\r\n", ch);
      morph->vnum = 1003;
    }
    else if(ch->pcdata->choice == 5)
    {
      ch->Class = CLASS_GRYPHON;
      if(xIS_SET(ch->act, PLR_BATTLE))
        send_to_char("!!SOUND(sound/gryphon.wav)\r\n", ch);
      morph->vnum = 1006;
    }
    else if(ch->pcdata->choice == 6)
    {
      ch->Class = CLASS_TREANT;
      if(xIS_SET(ch->act, PLR_BATTLE))
        send_to_char("!!SOUND(sound/treant.wav)\r\n", ch);
      morph->vnum = 1007;
    }
    else if(ch->pcdata->choice == 7)
    {
      ch->Class = CLASS_BAT;
      if(xIS_SET(ch->act, PLR_BATTLE))
        send_to_char("!!SOUND(sound/batform.wav)\r\n", ch);
      morph->vnum = 1013;
    }

    if(ch->race == RACE_ANIMAL && ch->secondclass != -1)
    {
      ch->pcdata->tmpsecond = ch->secondclass;
      ch->secondclass = -1;
    }
    if(ch->race == RACE_ANIMAL && ch->thirdclass != -1)
    {
      ch->pcdata->tmpthird = ch->thirdclass;
      ch->thirdclass = -1;
    }

    fForm = TRUE;

    if(fForm)
    {
      if(ch->pcdata->choice == 7)
      {
        af.type = gsn_bat_form;
      }
      else
      {
        af.type = gsn_animal_form;
      }
      af.duration = -1;
      af.level = ch->level;
      af.location = APPLY_AFFECT;
      af.modifier = 0;
      af.bitvector = meb(AFF_ANIMALFORM);
      affect_to_char(ch, &af);

      snprintf(buf, MSL, "%d", morph->vnum);
      do_imm_morph(ch, buf);
    }

    if(ch->morph == NULL || ch->morph->morph == NULL)
    {
      fForm = FALSE;
      return;
    }

    /*
     * Ok this is a simple way of handling it, we toss through the sns and set
     * everything up as we go 
     */
    for(sn = 0; sn < MAX_SKILL; sn++)
    {
      /*
       * Toss into backup 
       */
      if(ch->pcdata->learned[sn] > 0)
      {
        backup[sn] = ch->pcdata->learned[sn];
        dshowbackup[sn] = ch->pcdata->dshowlearned[sn];
      }

      /*
       * Toss on animal skills 
       */
      if(ch->pcdata->dlearned[sn] > 0)
      {
        ch->pcdata->learned[sn] = ch->pcdata->dlearned[sn];
        ch->pcdata->dlearned[sn] = 0;
        ch->pcdata->dshowlearned[sn] = ch->pcdata->dshowdlearned[sn];
      }

      /*
       * Toss backup into dlearned 
       */
      if(backup[sn] > 0)
      {
        ch->pcdata->dlearned[sn] = backup[sn];
        backup[sn] = 0;
        ch->pcdata->dshowdlearned[sn] = dshowbackup[sn];
      }
    }

    // Lastly reinstate human form % ..
    ability = skill_lookup("animal form");
    if(ability > 0)
      ch->pcdata->learned[ability] = ch->pcdata->dlearned[ability];
    ability = skill_lookup("common");

    if(ability > 0)
      ch->pcdata->learned[ability] = ch->pcdata->dlearned[ability];

    while(ch->first_affect)
      affect_remove(ch, ch->first_affect);
    ch->pcdata->tmparmor = ch->armor;
    ch->pcdata->tmpmax_hit = ch->max_hit;
    if(ch->Class == CLASS_BAT)
      ch->pcdata->tmpmax_mana = ch->max_blood;
    else
      ch->pcdata->tmpmax_mana = ch->max_mana;
    ch->pcdata->tmpmax_move = ch->max_move;

    ch->pcdata->tmpheight = ch->height;
    ch->pcdata->tmpweight = ch->weight;

    // Change this based on type of Morph
    if(ch->pcdata->choice == 1)
    {
      ch->max_hit += (ch->level * 2 + get_curr_con(ch));
      // ch->hit = ch->max_hit;
      ch->armor -= ch->level;
      ch->height = 60;
      ch->weight = 200;
    }
    else if(ch->pcdata->choice == 2)
    {
      ch->max_mana += (ch->level * 2 + get_curr_int(ch));
      // ch->mana = ch->max_mana;
      ch->armor -= ch->level;
      ch->height = 36;
      ch->weight = 40;
    }
    else if(ch->pcdata->choice == 3)
    {
      ch->max_move += (ch->level * 2 + get_curr_str(ch));
      // ch->move = ch->max_move;
      ch->armor -= ch->level;
      ch->height = 12;
      ch->weight = 20;
    }
    else if(ch->pcdata->choice == 4)
    {
      ch->max_hit += (ch->level * 5 + get_curr_con(ch) * 2);
      // ch->hit = ch->max_hit;
      ch->height = 90;
      ch->weight = 400;
      ch->armor -= ch->level;
    }
    else if(ch->pcdata->choice == 5)
    {
      ch->max_hit += (ch->level * 8 + get_curr_con(ch) * 2);
      // ch->hit = ch->max_hit;
      ch->height = 80;
      ch->weight = 300;
      ch->armor -= ch->level;
    }
    else if(ch->pcdata->choice == 6)
    {
      ch->max_hit += (ch->level * 10 + get_curr_con(ch) * 2);
      // ch->hit = ch->max_hit;
      ch->height = 144;
      ch->weight = 600;
      ch->armor -= ch->level;
    }
    else if(ch->pcdata->choice == 7)
    {
      ch->max_hit += (ch->level * 3 + get_curr_con(ch) * 2);
      // ch->hit = ch->max_hit;
      ch->height = 60;
      ch->weight = 50;
      ch->armor -= ch->level;
    }

    if(!IS_NPC(ch))
      update_aris(ch);

    save_char_obj(ch);

    if(ch->pcdata->choice == 7)
    {
      WAIT_STATE(ch, skill_table[gsn_bat_form]->beats);
      act(AT_BLOOD, "You call upon ancient magic and take on a animal form.", ch, NULL, NULL, TO_CHAR);
      act(AT_BLOOD, "$n suddenly shimmers and assumes a animal form.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You feel elation as your animal senses come in fully!", ch, NULL, NULL, TO_CHAR);
      learn_from_success(ch, gsn_bat_form);
    }
    else
    {
      WAIT_STATE(ch, skill_table[gsn_animal_form]->beats);
      act(AT_MAGIC, "You call upon ancient magic and take on a animal form.", ch, NULL, NULL, TO_CHAR);
      act(AT_MAGIC, "$n suddenly shimmers and assumes a animal form.", ch, NULL, NULL, TO_ROOM);
      act(AT_CYAN, "You feel elation as your animal senses come in fully!", ch, NULL, NULL, TO_CHAR);
      learn_from_success(ch, gsn_animal_form);
    }
    send_to_char("\r\nNow in your new form, you once again don your gear.\r\n", ch);
    interpret(ch, (char *)"wear all");
    save_char_obj(ch);
    return;
  }
  else
  { /* going to normal */
    if(xIS_SET(ch->act, PLR_BATTLE))
      send_to_char("!!SOUND(sound/humanform.wav)\r\n", ch);

    send_to_char("\r\nYou remove all your gear as your shape is about to vastly change.\r\n", ch);
    remove_all_equipment(ch);

    send_to_char("You release the ancient magic, and change into your normal form.\r\n", ch);
    if(ch->pcdata->choice == 7)
      act(AT_BLOOD, "$n suddenly shimmers and assumes their normal form!", ch, NULL, NULL, TO_ROOM);
    else
      act(AT_MAGIC, "$n suddenly shimmers and assumes their normal form!", ch, NULL, NULL, TO_ROOM);
    act(AT_CYAN, "You feel disoriented as you get used to your normal form!", ch, NULL, NULL, TO_CHAR);
    ch->pcdata->choice = 0;
    while(ch->first_affect)
      affect_remove(ch, ch->first_affect);
    do_unmorph_char(ch);
    ch->armor = ch->pcdata->tmparmor;
    ch->max_hit = ch->pcdata->tmpmax_hit;
    // ch->hit = ch->max_hit;
    if(ch->pcdata->tmpclass == 15)
      ch->max_blood = ch->pcdata->tmpmax_mana;
    else
      ch->max_mana = ch->pcdata->tmpmax_mana;
    // ch->mana = ch->max_mana;
    ch->max_move = ch->pcdata->tmpmax_move;
    // ch->move = ch->max_move;
    ch->height = ch->pcdata->tmpheight;
    ch->weight = ch->pcdata->tmpweight;
    ch->race = ch->pcdata->tmprace;
    ch->Class = ch->pcdata->tmpclass;
    if(ch->pcdata->tmpsecond != -1)
    {
      ch->secondclass = ch->pcdata->tmpsecond;
      ch->pcdata->tmpsecond = -1;
    }
    if(ch->pcdata->tmpthird != -1)
    {
      ch->thirdclass = ch->pcdata->tmpthird;
      ch->pcdata->tmpthird = -1;
    }

    if(ch->race != RACE_ANIMAL)
    {
      ch->pcdata->tmpmax_hit = 0;
      ch->pcdata->tmpmax_mana = 0;
      ch->pcdata->tmpmax_move = 0;
      ch->pcdata->tmpheight = 0;
      ch->pcdata->tmpweight = 0;
      /*
       * ch->pcdata->tmprace = 0; ch->pcdata->tmpclass = 0; 
       */
      ch->pcdata->tmparmor = 0;
    }
    if(ch->pcdata->tmplevel > 100)
    {
      ch->pcdata->tmpmax_hit = 30000;
      ch->pcdata->tmpmax_mana = 30000;
      ch->pcdata->tmpmax_move = 30000;
    }

    /*
     * Ok this is a simple way of handling it, we toss through the sns and set
     * everything up as we go 
     */
    for(sn = 0; sn < MAX_SKILL; sn++)
    {
      /*
       * Toss into backup 
       */
      if(ch->pcdata->learned[sn] > 0)
      {
        backup[sn] = ch->pcdata->learned[sn];
        dshowbackup[sn] = ch->pcdata->dshowlearned[sn];
      }

      /*
       * Toss on human skills 
       */
      if(ch->pcdata->dlearned[sn] > 0)
      {
        ch->pcdata->learned[sn] = ch->pcdata->dlearned[sn];
        ch->pcdata->dlearned[sn] = 0;
        ch->pcdata->dshowlearned[sn] = ch->pcdata->dshowdlearned[sn];
        ch->pcdata->dshowdlearned[sn] = FALSE;
      }

      /*
       * Toss backup into dlearned 
       */
      if(backup[sn] > 0)
      {
        ch->pcdata->dlearned[sn] = backup[sn];
        backup[sn] = 0;
        ch->pcdata->dshowdlearned[sn] = dshowbackup[sn];
      }
    }

    /*
     * Lastly reinstate human form % .. 
     */
    ability = skill_lookup("animal form");
    if(ability > 0)
      ch->pcdata->learned[ability] = ch->pcdata->dlearned[ability];
    ability = skill_lookup("common");

    if(ability > 0)
      ch->pcdata->learned[ability] = ch->pcdata->dlearned[ability];
// forgot to let them wear all going back to human form?
    interpret(ch, (char *)"wear all");

    if(!IS_NPC(ch))
      update_aris(ch);
    save_char_obj(ch);
  }
}

void do_bat_form(CHAR_DATA *ch, char *argument)
{
  short                   nomore = 10;
  AFFECT_DATA             af;

  // Temp fix for players switching from animal to human form to remove curse and
  // poison. -Taon
  if(IS_AFFECTED(ch, AFF_CURSE) || IS_AFFECTED(ch, AFF_POISON))
  {
    send_to_char("You can't change form when cursed or affected by poison.", ch);
    return;
  }

  if(IS_NPC(ch))
    return;

  if(ch->race == RACE_ANIMAL)
  {
    affect_strip(ch, gsn_bat_form);
    xREMOVE_BIT(ch->affected_by, AFF_ANIMALFORM);
    animal_change(ch, FALSE);
    return;
  }
// Set to bat form
  ch->pcdata->choice = 7;

  if(ch->blood < nomore)
  {
    send_to_char("You do not have enough blood power to do that.\r\n", ch);
    return;
  }
  if(can_use_skill(ch, number_percent(), gsn_bat_form))
  {
    ch->blood = (ch->blood - nomore);
    animal_change(ch, TRUE);
    return;
  }
  else
  {
    learn_from_failure(ch, gsn_bat_form);
    send_to_char("&rYou fail to summon the ancient magic.\r\n", ch);
    return;
  }

}

void do_animal_form(CHAR_DATA *ch, char *argument)
{
  short                   nomore = 100;
  AFFECT_DATA             af;

  // Temp fix for players switching from animal to human form to remove curse and
  // poison. -Taon
  if(IS_AFFECTED(ch, AFF_CURSE) || IS_AFFECTED(ch, AFF_POISON))
  {
    send_to_char("You can't change form when cursed or affected by poison.", ch);
    return;
  }

  if(IS_NPC(ch))
    return;

  if(ch->race == RACE_ANIMAL)
  {
    affect_strip(ch, gsn_animal_form);
    xREMOVE_BIT(ch->affected_by, AFF_ANIMALFORM);
    animal_change(ch, FALSE);
    return;
  }

  if(!VLD_STR(argument))
  {
    send_to_char("Syntax: animal type\r\n", ch);
    if(ch->level < 20)
      send_to_char("Available types: wolf\r\n", ch);
    if(ch->level < 30 && ch->level > 19)
      send_to_char("Available types: wolf owl\r\n", ch);
    if(ch->level < 40 && ch->level > 29)
      send_to_char("Available types: wolf owl fish\r\n", ch);
    if(ch->level < 60 && ch->level > 39)
      send_to_char("Available types: wolf owl fish bear\r\n", ch);
    if(ch->level < 80 && ch->level > 59)
      send_to_char("Available types: wolf owl fish bear gryphon\r\n", ch);
    if(ch->level > 79)
      send_to_char("Available types: wolf owl fish bear gryphon treant\r\n", ch);
    return;
  }

  if(!str_cmp(argument, "wolf") && ch->level > 9)
  {
    ch->pcdata->choice = 1;
  }
  if(!str_cmp(argument, "wolf") && ch->level < 10)
  {
    send_to_char("You are not high enough level to do that.\r\n", ch);
    return;
  }
  if(!str_cmp(argument, "owl") && ch->level > 19)
  {
    ch->pcdata->choice = 2;
    // set it to wolf
  }
  if(!str_cmp(argument, "owl") && ch->level < 20)
  {
    send_to_char("You are not high enough level to do that.\r\n", ch);
    return;
  }
  if(!str_cmp(argument, "fish") && ch->level > 29)
  {
    ch->pcdata->choice = 3;
    // set it to wolf
  }
  if(!str_cmp(argument, "fish") && ch->level < 30)
  {
    send_to_char("You are not high enough level to do that.\r\n", ch);
    return;
  }

  if(!str_cmp(argument, "bear") && ch->level > 39)
  {
    ch->pcdata->choice = 4;
    // set it to wolf
  }
  if(!str_cmp(argument, "bear") && ch->level < 40)
  {
    send_to_char("You are not high enough level to do that.\r\n", ch);
    return;
  }

  if(!str_cmp(argument, "gryphon") && ch->level > 59)
  {
    ch->pcdata->choice = 5;
    // set it to wolf
  }
  if(!str_cmp(argument, "gryphon") && ch->level < 60)
  {
    send_to_char("You are not high enough level to do that.\r\n", ch);
    return;
  }

  if(!str_cmp(argument, "treant") && ch->level > 79)
  {
    ch->pcdata->choice = 6;
    // set it to wolf
  }
  if(!str_cmp(argument, "treant") && ch->level < 80)
  {
    send_to_char("You are not high enough level to do that.\r\n", ch);
    return;
  }

  if(str_cmp(argument, "treant") && str_cmp(argument, "gryphon") && str_cmp(argument, "bear") && str_cmp(argument, "fish") && str_cmp(argument, "owl") && str_cmp(argument, "wolf"))
  {
    send_to_char("Syntax: animal type\r\n", ch);
    if(ch->level < 20)
      send_to_char("Available types: wolf\r\n", ch);
    if(ch->level < 30 && ch->level > 19)
      send_to_char("Available types: wolf owl\r\n", ch);
    if(ch->level < 40 && ch->level > 29)
      send_to_char("Available types: wolf owl fish\r\n", ch);
    if(ch->level < 60 && ch->level > 39)
      send_to_char("Available types: wolf owl fish bear\r\n", ch);
    if(ch->level < 80 && ch->level > 59)
      send_to_char("Available types: wolf owl fish bear gryphon\r\n", ch);
    if(ch->level > 79)
      send_to_char("Available types: wolf owl fish bear gryphon treant\r\n", ch);

    return;
  }

  if(ch->mana < nomore)
  {
    send_to_char("You do not have enough mana to do that.\r\n", ch);
    return;
  }
  if(can_use_skill(ch, number_percent(), gsn_animal_form))
  {
    ch->mana = (ch->mana - nomore);
    animal_change(ch, TRUE);
    return;
  }
  else
  {
    learn_from_failure(ch, gsn_animal_form);
    send_to_char("&cYou fail to summon the ancient magic.\r\n", ch);
    return;
  }
}

void do_stalk_prey(CHAR_DATA *ch, char *argument)
{
  MOB_INDEX_DATA         *temp;
  CHAR_DATA              *mob;
  CHAR_DATA              *prey;
  bool                    found;

  found = FALSE;
  for(mob = first_char; mob; mob = mob->next)
  {
    if(mob)
    {
      if(IS_NPC(mob) && mob->in_room && ch->in_room)
      {
        if(mob->master)
        {
          if(!str_cmp(ch->name, mob->master->name) && (mob->in_room == ch->in_room))
          {
            found = TRUE;
            break;
          }
        }
      }
    }
  }

  if(found == FALSE && !xIS_SET(ch->act, ACT_BEASTMELD))
  {
    send_to_char("Your pet is not here, to stalk any prey.\r\n", ch);
    return;
  }

  if(!IS_OUTSIDE(ch) || ch->in_room->sector_type == SECT_INSIDE || ch->in_room->sector_type == SECT_ROAD)
  {
    act(AT_CYAN, "$n tries to find suitable prey to stalk, but nothing is near.", mob, NULL, NULL, TO_ROOM);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_stalk_prey))
  {
    WAIT_STATE(ch, skill_table[gsn_stalk_prey]->beats);
    learn_from_success(ch, gsn_stalk_prey);
    int                     vnum;

    vnum = number_range(1, 4);
    if(vnum == 1)
      vnum = 4410;
    else if(vnum == 2)
      vnum = 1510;
    else if(vnum == 3)
      vnum = 16710;
    else if(vnum == 4)
      vnum = 13009;

    temp = get_mob_index(vnum);
    if(xIS_SET(ch->act, ACT_BEASTMELD))
    {
      ch->master->pcdata->pethungry = 700;
      if(ch->master->pcdata->petaffection < 25)
        ch->master->pcdata->petaffection += 5;
    }
    else
    {
      ch->pcdata->pethungry = 700;
      if(ch->pcdata->petaffection < 25)
        ch->pcdata->petaffection += 5;
    }
    if(xIS_SET(ch->act, PLR_BATTLE) || (xIS_SET(ch->act, ACT_BEASTMELD) && xIS_SET(ch->master->act, PLR_BATTLE)))
      send_to_char("!!SOUND(sound/create.wav)\r\n", ch);
    prey = create_mobile(temp);
    prey->level = 1;
    char_to_room(prey, ch->in_room);
    if(xIS_SET(ch->act, ACT_BEASTMELD))
      act(AT_CYAN, "You successfully stalks down $N and slay it!", ch, NULL, prey, TO_CHAR);
    if(!xIS_SET(ch->act, ACT_BEASTMELD))
      act(AT_CYAN, "$n successfully stalks down $N and slay's it!", mob, NULL, prey, TO_ROOM);
    raw_kill(ch, prey);
    return;
  }
  else
  {
    if(!xIS_SET(ch->act, ACT_BEASTMELD))
      act(AT_CYAN, "$n tries to stalk a small animal, but fails to get near.", mob, NULL, NULL, TO_ROOM);
    learn_from_failure(ch, gsn_stalk_prey);
    return;
  }

}

void do_infectious_claws(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;
  CHAR_DATA              *mob;
  bool                    found;

  found = FALSE;

  for(mob = first_char; mob; mob = mob->next)
  {
    if(mob)
    {
      if(IS_NPC(mob) && mob->in_room && ch->in_room)
      {
        if(mob->master)
        {
          if(!str_cmp(ch->name, mob->master->name) && (mob->in_room == ch->in_room))
          {
            found = TRUE;
            break;
          }
        }
      }
    }
  }
  if(found == FALSE && !xIS_SET(ch->act, ACT_BEASTMELD))
  {
    send_to_char("Your pet is not here.\r\n", ch);
    return;
  }
  if(xIS_SET(ch->act, ACT_BEASTMELD))
  {
    mob = ch;
  }

  if(can_use_skill(ch, number_percent(), gsn_infectious_claws))
  {
    WAIT_STATE(ch, skill_table[gsn_infectious_claws]->beats);
    learn_from_success(ch, gsn_infectious_claws);
    affect_strip(mob, gsn_infectious_claws);

    if(xIS_SET(ch->act, PLR_BATTLE) || (xIS_SET(ch->act, ACT_BEASTMELD) && xIS_SET(ch->master->act, PLR_BATTLE)))
      send_to_char("!!SOUND(sound/create.wav)\r\n", ch);
    act(AT_CYAN, "$n opens a poison gland to $s claws.", mob, NULL, NULL, TO_ROOM);
    if(xIS_SET(ch->act, ACT_BEASTMELD))
      act(AT_CYAN, "You open a poison gland in your claws.", mob, NULL, NULL, TO_CHAR);
    xSET_BIT(mob->attacks, ATCK_POISON);
    af.type = gsn_infectious_claws;
    af.duration = ch->level + 50;
    af.location = APPLY_NONE;
    af.level = ch->level;
    af.bitvector = meb(AFF_INFECTIOUS_CLAWS);
    af.modifier = 0;
    affect_to_char(mob, &af);
    return;
  }
  else
  {
    act(AT_CYAN, "$n attempts to open a poison gland to $s claws, but fails.", mob, NULL, NULL, TO_ROOM);
    learn_from_failure(ch, gsn_infectious_claws);
    return;
  }

}

void do_hone_claws(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;
  CHAR_DATA              *mob;
  bool                    found;

  found = FALSE;

  for(mob = first_char; mob; mob = mob->next)
  {
    if(mob)
    {
      if(IS_NPC(mob) && mob->in_room && ch->in_room)
      {
        if(mob->master)
        {
          if(!str_cmp(ch->name, mob->master->name) && (mob->in_room == ch->in_room))
          {
            found = TRUE;
            break;
          }
        }
      }
    }
  }
  if(found == FALSE && !xIS_SET(ch->act, ACT_BEASTMELD))
  {
    send_to_char("Your pet is not here, to sharpen it's claws.\r\n", ch);
    return;
  }

  if(xIS_SET(ch->act, ACT_BEASTMELD))
    mob = ch;

  if(!IS_OUTSIDE(ch) || ch->in_room->sector_type == SECT_INSIDE || ch->in_room->sector_type == SECT_ROAD)
  {
    act(AT_CYAN, "$n tries to find something suitable to sharpen it's claws on, but nothing is near.", mob, NULL, NULL, TO_ROOM);
    return;
  }

  if(mob->fighting)
  {
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_hone_claws))
  {
    WAIT_STATE(ch, skill_table[gsn_hone_claws]->beats);
    learn_from_success(ch, gsn_hone_claws);
    affect_strip(mob, gsn_hone_claws);

    if(xIS_SET(ch->act, PLR_BATTLE) || (xIS_SET(ch->act, ACT_BEASTMELD) && xIS_SET(ch->master->act, PLR_BATTLE)))
      send_to_char("!!SOUND(sound/hone.wav)\r\n", ch);
    act(AT_CYAN, "$n finds a small tree and begins to hone the sharpenss of it's claws.", mob, NULL, NULL, TO_ROOM);
    if(xIS_SET(mob->act, ACT_BEASTMELD))
      act(AT_CYAN, "You find a small tree and begin to hone the sharpenss of your claws.", mob, NULL, NULL, TO_CHAR);

    af.type = gsn_hone_claws;
    af.duration = ch->level + 50;
    af.location = APPLY_DAMROLL;
    af.level = ch->level;
    xCLEAR_BITS(af.bitvector);
    af.modifier = 5;
    affect_to_char(mob, &af);
    return;
  }
  else
  {
    act(AT_CYAN, "$n tries to find suitable place to hone the sharpness of it's claws, but nothing is near.", mob, NULL, NULL, TO_ROOM);
    learn_from_failure(ch, gsn_hone_claws);
    return;
  }

}

void do_net(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA             af;
  CHAR_DATA              *victim;
  char                    arg[MIL];
  OBJ_DATA               *obj;

  argument = one_argument(argument, arg);

  if(arg[0] == '\0')
  {
    if(ch->fighting)
      victim = who_fighting(ch);
    else
    {
      send_to_char("Try to throw a net on who?\r\n", ch);
      return;
    }
  }
  else if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(IS_AFFECTED(victim, AFF_SLOW))
  {
    send_to_char("They are already slowed down.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_net))
  {
    WAIT_STATE(ch, skill_table[gsn_net]->beats);
    learn_from_success(ch, gsn_net);

    af.bitvector = meb(AFF_SLOW);
    af.type = gsn_net;
    af.duration = (int)((ch->level - get_curr_int(victim)) * 2);
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.level = ch->level;
    affect_to_char(victim, &af);
    act(AT_CYAN, "You manage to throw a net around $N, slowing them down.", ch, NULL, victim, TO_CHAR);
    act(AT_CYAN, "$n throws a net around $N, slowing down $S movements.", ch, NULL, victim, TO_ROOM);
    global_retcode = damage(ch, victim, 1, gsn_net);
    obj = create_object(get_obj_index(41033), 0);
    obj_to_room(obj, victim->in_room);
    make_scraps(obj);
  }
  else
  {
    act(AT_CYAN, "You try to throw a net around $N, but the dodge to the side.", ch, NULL, victim, TO_CHAR);
    learn_from_failure(ch, gsn_net);
    return;
  }

}

void do_find_water(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA              *mob;
  bool                    found, wfound;
  OBJ_DATA               *water;

  found = FALSE;
  for(mob = first_char; mob; mob = mob->next)
  {
    if(mob)
    {
      if(IS_NPC(mob) && mob->in_room && ch->in_room)
      {
        if(mob->master)
        {
          if(!str_cmp(ch->name, mob->master->name) && (mob->in_room == ch->in_room))
          {
            found = TRUE;
            break;
          }
        }
      }
    }
  }
  if(found == FALSE && !xIS_SET(ch->act, ACT_BEASTMELD))
  {
    send_to_char("Your pet is not here, to find any water.\r\n", ch);
    return;
  }

  if(!IS_OUTSIDE(ch) || ch->in_room->sector_type == SECT_INSIDE || ch->in_room->sector_type == SECT_ROAD)
  {
    act(AT_CYAN, "$n tries to find suitable drinking water, but nothing is near.", mob, NULL, NULL, TO_ROOM);
    return;
  }

  wfound = FALSE;

  for(water = ch->in_room->first_content; water; water = water->next_content)
  {
    if(water->item_type == ITEM_FOUNTAIN)
    {
      wfound = TRUE;
      break;
    }
  }

  if(wfound)
  {
    send_to_char("There is already a spring of water here.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_find_water))
  {
    WAIT_STATE(ch, skill_table[gsn_find_water]->beats);
    learn_from_success(ch, gsn_find_water);
    if(xIS_SET(ch->act, PLR_BATTLE) || (xIS_SET(ch->act, ACT_BEASTMELD) && xIS_SET(ch->master->act, PLR_BATTLE)))
      send_to_char("!!SOUND(sound/lapwater.wav)\r\n", ch);
    water = create_object(get_obj_index(41027), 0);
    if(xIS_SET(ch->act, ACT_BEASTMELD))
    {
      ch->master->pcdata->petthirsty = 700;
      if(ch->master->pcdata->petaffection < 25)
        ch->master->pcdata->petaffection += 5;
      act(AT_CYAN, "You are able to locate $p and drink until your content.", ch, water, NULL, TO_CHAR);
    }
    else
    {
      ch->pcdata->petthirsty = 700;
      if(ch->pcdata->petaffection < 25)
        ch->pcdata->petaffection += 5;
    }
    if(!xIS_SET(ch->act, ACT_BEASTMELD))
      act(AT_CYAN, "$n is able to locate $p and drinks until content.", mob, water, NULL, TO_ROOM);
    obj_to_room(water, ch->in_room);
    return;
  }
  else
  {
    if(!xIS_SET(ch->act, ACT_BEASTMELD))
      act(AT_CYAN, "$n tries to find suitable drinking water, but nothing is near.", mob, NULL, NULL, TO_ROOM);
    learn_from_failure(ch, gsn_find_water);
    return;
  }
}

void do_join(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA              *mob;
  bool                    found;

// Must be in beast meld to use?  brings body to pet.
  if(!xIS_SET(ch->act, ACT_BEASTMELD))
  {
    send_to_char("You must be in beast meld to use join.\r\n", ch);
    return;
  }

  if(ch->mana < 50)
  {
    send_to_char("You don't have enough mana to do that.\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_join))
  {
    WAIT_STATE(ch, skill_table[gsn_join]->beats);
    learn_from_success(ch, gsn_join);

    act(AT_DGREEN, "$n's body begins to glow and disappears in a firery light!", ch->master, NULL, NULL, TO_ROOM);
    char_from_room(ch->master);
    char_to_room(ch->master, ch->in_room);
    ch->mana = 0;
    ch->master->mana = 0;
    ch_printf(ch, "&GYour body travels spiritually to join with you!\r\n");
    act(AT_DGREEN, "$n's body travels spiritually to join with you!", ch, NULL, NULL, TO_ROOM);
    if(xIS_SET(ch->act, PLR_BATTLE) || (xIS_SET(ch->act, ACT_BEASTMELD) && xIS_SET(ch->master->act, PLR_BATTLE)))
      send_to_char("!!SOUND(sound/join.wav)\r\n", ch);

    do_look(ch, (char *)"auto");
  }
  else
  {
    act(AT_CYAN, "\r\nYou begin to call for your body to join with you, but get distracted.\r\n", ch, NULL, NULL, TO_CHAR);
    learn_from_failure(ch, gsn_join);
    return;
  }

}

void do_beast_meld(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA              *mob;
  bool                    found;
  AFFECT_DATA             af;

  set_char_color(AT_IMMORT, ch);

  if(!ch->desc)
    return;

  if(ch->desc->original)
  {
    ch_printf(ch, "You release your beast meld.\r\n");
    if(xIS_SET(ch->act, PLR_BATTLE) || (xIS_SET(ch->act, ACT_BEASTMELD) && xIS_SET(ch->master->act, PLR_BATTLE)))
      send_to_char("!!SOUND(sound/beastmeld.wav)\r\n", ch);
    interpret(ch->master, (char *)"stand");
    interpret(ch->master, (char *)"visible");
    ch->desc->character = ch->desc->original;
    ch->desc->original = NULL;
    ch->desc->character->desc = ch->desc;
    ch->desc->character->switched = NULL;
    ch->desc = NULL;
    xREMOVE_BIT(ch->act, ACT_BEASTMELD);
    affect_strip(ch->master, gsn_beast_meld);
    xREMOVE_BIT(ch->master->affected_by, AFF_BEASTMELD);
    return;
  }

  found = FALSE;
  for(mob = first_char; mob; mob = mob->next)
  {
    if(IS_NPC(mob) && ch == mob->master)
    {
      found = TRUE;
      break;
    }
  }

  if(found == FALSE && !xIS_SET(ch->act, PLR_BOUGHT_PET))
  {
    send_to_char("You don't have a pet.\r\n", ch);
    return;
  }

  if(mob->desc)
  {
    send_to_char("Character pet meld in use.\r\n", ch);
    return;
  }

  if(ch->switched)
  {
    send_to_char("You can't switch into a player that is beast melded!\r\n", ch);
    return;
  }
  if(!IS_NPC(ch) && xIS_SET(ch->act, PLR_FREEZE))
  {
    send_to_char("You shouldn't switch into a player that is frozen!\r\n", ch);
    return;
  }

  if(can_use_skill(ch, number_percent(), gsn_beast_meld))
  {
    if(xIS_SET(ch->act, PLR_BATTLE) || (xIS_SET(ch->act, ACT_BEASTMELD) && xIS_SET(ch->master->act, PLR_BATTLE)))
      send_to_char("!!SOUND(sound/beastmeld.wav)\r\n", ch);

    WAIT_STATE(ch, skill_table[gsn_beast_meld]->beats);
    learn_from_success(ch, gsn_beast_meld);
    af.type = gsn_hide;
    af.duration = -1;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = meb(AFF_HIDE);
    af.level = ch->level;
    affect_to_char(ch, &af);

    af.type = gsn_beast_meld;
    af.duration = -1;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = meb(AFF_BEASTMELD);
    af.level = ch->level;
    affect_to_char(ch, &af);
    ch->desc->character = mob;
    ch->desc->original = ch;
    mob->desc = ch->desc;
    ch->desc = NULL;
    ch->switched = mob;

    if(IS_BLIND(ch))
      xSET_BIT(mob->act, PLR_BLIND);
    send_to_char("You begin to beast meld.\r\n", mob);
    xSET_BIT(mob->act, ACT_BEASTMELD);
    act(AT_CYAN, "$n lays down and $s eyes go blank while $s melds into the surroundings.\r\n", ch, NULL, NULL, TO_ROOM);
    interpret(ch, (char *)"rest");
    save_char_obj(ch);
    return;
  }
  else
  {
    act(AT_CYAN, "\r\nYou begin to beast meld, but lose your concentration.\r\n", ch, NULL, NULL, TO_CHAR);
    learn_from_failure(ch, gsn_beast_meld);
    return;
  }
}
