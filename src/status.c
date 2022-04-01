/*
 *Code Information: I find this command quite useful, it reveals
 *important information which pertains to the player invoking it.
 *do_status was written by: Taon "Dustan Gunn", for use on 6Dragons
 *mud. This function is still currently under major construction. -Taon
 *Note: This isnt the final information header, just using it to
 *keep those with current shell access a little informed.
 */

#include "h/mud.h"

//I intend on cleaning up the format once finished. -Taon

void do_status(CHAR_DATA *ch, char *argument)
{

  if(IS_NPC(ch) && !xIS_SET(ch->act, ACT_BEASTPET))
    return;

  if(ch->position == POS_SLEEPING)
  {
    send_to_char("Can't check status while sleeping!\r\n", ch);
    return;
  }

  send_to_char("\r\n", ch);
  if(!IS_BLIND(ch))
  {
    send_to_char("\r\n&W-------------------&B[&D &i&W&uStatus&D&D&D &B]&D&W---------------------&D\r\n\r\n", ch);
  }
  else
  {
    send_to_char("\r\nStatus\r\n", ch);
  }
  send_to_char("&WHunger&D: ", ch);

  if(xIS_SET(ch->act, ACT_BEASTPET))
  {
    CHAR_DATA              *master = NULL;

    if(ch->master)
      master = ch->master;

    if(!master)
      return;

    if(master->pcdata->pethungry <= 99)
      send_to_char("&OYou're STARVING to death!!!&D\r\n", ch);
    else if(master->pcdata->pethungry <= 269 && master->pcdata->pethungry >= 100)
      send_to_char("&OYour getting very hungry!&D\r\n", ch);
    else if(master->pcdata->pethungry <= 279 && master->pcdata->pethungry >= 270)
      send_to_char("&OYou feel faint from hunger.&D\r\n", ch);
    else if(master->pcdata->pethungry <= 299 && master->pcdata->pethungry >= 280)
      send_to_char("&OYou're starting to get hungry.&D\r\n", ch);
    else if(master->pcdata->pethungry >= 300)
      send_to_char("&OYou're not hungry.&D\r\n", ch);
    // thirst
    send_to_char("&WThirst&D: ", ch);

    if(master->pcdata->petthirsty <= 99)
      send_to_char("&OYou're dying of thirst!&D\r\n", ch);
    else if(master->pcdata->petthirsty <= 269 && master->pcdata->petthirsty >= 100)
      send_to_char("&OYou really could use a sip of water.&D\r\n", ch);
    else if(master->pcdata->petthirsty <= 279 && master->pcdata->petthirsty >= 270)
      send_to_char("&OYour throat is parched.&D\r\n", ch);
    else if(master->pcdata->petthirsty <= 299 && master->pcdata->petthirsty >= 280)
      send_to_char("&OYou don't feel too thirsty.&D\r\n", ch);
    else if(master->pcdata->petthirsty >= 300)
      send_to_char("&OYou're not thirsty.&D\r\n", ch);

    return;
  }

  if(!IS_IMMORTAL(ch))
  {
    if(ch->pcdata->condition[COND_FULL] <= 0)
      send_to_char("&OYou're STARVING to death!!!&D\r\n", ch);
    else if(ch->pcdata->condition[COND_FULL] <= 10 && ch->pcdata->condition[COND_FULL] >= 1)
      send_to_char("&OYour getting very hungry!&D\r\n", ch);
    else if(ch->pcdata->condition[COND_FULL] <= 20 && ch->pcdata->condition[COND_FULL] >= 11)
      send_to_char("&OYou feel faint from hunger.&D\r\n", ch);
    else if(ch->pcdata->condition[COND_FULL] <= 30 && ch->pcdata->condition[COND_FULL] >= 21)
      send_to_char("&OYou're starting to get hungry.&D\r\n", ch);
    else if(ch->pcdata->condition[COND_FULL] >= 31)
      send_to_char("&OYou're not hungry.&D\r\n", ch);
  }
  else
    send_to_char("&OYou're always full!&D\r\n", ch);

  send_to_char("&WThirst&D: ", ch);

  if(!IS_IMMORTAL(ch))
  {
    if(ch->pcdata->condition[COND_THIRST] <= 0)
      send_to_char("&OYou're dying of thirst!&D\r\n", ch);
    else if(ch->pcdata->condition[COND_THIRST] <= 10 && ch->pcdata->condition[COND_THIRST] >= 1)
      send_to_char("&OYou really could use a sip of water.&D\r\n", ch);
    else if(ch->pcdata->condition[COND_THIRST] <= 20 && ch->pcdata->condition[COND_THIRST] >= 11)
      send_to_char("&OYour throat is parched.&D\r\n", ch);
    else if(ch->pcdata->condition[COND_THIRST] <= 30 && ch->pcdata->condition[COND_THIRST] >= 21)
      send_to_char("&OYou don't feel too thirsty.&D\r\n", ch);
    else if(ch->pcdata->condition[COND_THIRST] >= 31)
      send_to_char("&OYou're not thirsty.&D\r\n", ch);
  }
  else
    send_to_char("&OYou're never THIRSTY!&D\r\n", ch);

  send_to_char("&WEncumbrance&D: ", ch);
  ch_printf(ch, "&O%d/%d&D\r\n", ch->carry_weight, can_carry_w(ch));

  send_to_char("&WPosition&D: ", ch);

  switch (ch->position)
  {
    case POS_DEAD:
      send_to_char("&OYou're DEAD!!!&D\r\n", ch);
      break;
    case POS_INCAP:
      send_to_char("&OYou've been incapitated!&D\r\n", ch);
      break;
    case POS_STUNNED:
      send_to_char("&OYou're STUNNED!!!&D\r\n", ch);
      break;
    case POS_STANDING:
      send_to_char("&OYou're standing up.&D\r\n", ch);
      break;
    case POS_MEDITATING:
      send_to_char("&OYou're in a deep meditation.&D\r\n", ch);
      break;
    case POS_SITTING:
    case POS_RESTING:
      send_to_char("&OYou're sitting down.&D\r\n", ch);
      break;
    case POS_SLEEPING:
      send_to_char("&OYou're asleep.&D\r\n", ch);
      break;
    case POS_MOUNTED:
      send_to_char("&OYou're riding on a mount.&D\r\n", ch);
      break;
    case POS_MORTAL:
      send_to_char("&OYou're mortally wounded!&D\r\n", ch);
      break;
    case POS_EVASIVE:
      send_to_char("&OYou're taking evasive action.&D\r\n", ch);
      break;
    case POS_DEFENSIVE:
      send_to_char("&OYou're taking defensive action.&D\r\n", ch);
      break;
    case POS_AGGRESSIVE:
      send_to_char("&OYou're taking aggressive action.&D\r\n", ch);
      break;
    case POS_BERSERK:
      send_to_char("&OYou're &iBERSERKING*D!!&D\r\n", ch);
      break;
    case POS_FIGHTING:
      send_to_char("&OYou're fighting!&D\r\n", ch);
      break;
    default:
      send_to_char("&OError: Position out of bounds, for status.&D\r\n", ch);
      bug("Error: do_status doesn't reconize %s's current position.", ch->name);
      break;
  }

  send_to_char("&WMentalstate&D: ", ch);

  // Allow the fall throughs in this switch. -Taon
/* Whoops, think these are backwards! Were from -1 to -10, fixed 16/7/08 Volk */
  switch (ch->mental_state / 10)
  {
    case -10:
    case -9:
      send_to_pager("&OYou can barely keep your eyes open.&D\r\n", ch);
      break;
    case -8:
    case -7:
    case -6:
      send_to_pager("&OYou feel quite sedated.&D\r\n", ch);
      break;
    case -5:
    case -4:
    case -3:
      send_to_pager("&OYou could use a rest.&D\r\n", ch);
      break;
    case -2:
    case -1:
      send_to_pager("&OYou feel fine.&D\r\n", ch);
      break;
    case 0:
    case 1:
      send_to_pager("&OYou feel great.&D\r\n", ch);
      break;
    case 2:
    case 3:
      send_to_pager("&OYour mind is racing, preventing you from thinking straight.&D\r\n", ch);
      break;
    case 4:
    case 5:
    case 6:
      send_to_pager("&OYour mind is going 100 miles an hour.&D\r\n", ch);
      break;
    case 7:
    case 8:
      send_to_pager("&OYou have no idea what is real, and what is not.&D\r\n", ch);
      break;
    case 9:
    case 10:
      send_to_pager("&O&uYou are a Supreme Entity&D&D.\r\n", ch);
      break;
    default:
      send_to_pager("&OYou're completely messed up!&D\r\n", ch);
      break;
  }

  if(ch->quest_curr > 1)
    ch_printf(ch, "&WGlory Points&D: &O%d&D\r\n", ch->quest_curr);

  send_to_char("&W\r\nGeneral Information:&D\r\n", ch);

  if(ch->move <= ch->max_move / 5)
    send_to_char("&OYou need to rest, your body is tiring out.&D\r\n", ch);
  if(ch->mana <= ch->max_mana / 5)
    send_to_char("&OYou feel as if your magic is nearly drained.&D\r\n", ch);
  if(ch->hit <= ch->max_hit / 5)
    send_to_char("&OYou're hurt quite badly.&D\r\n", ch);

  if(IS_BLOODCLASS(ch) && (ch->blood <= ch->max_blood / 6))
    send_to_char("&OYour body begins to &uwithdraw&D&O from lack of blood.&D\r\n", ch);

  if(ch->pcdata->condition[COND_DRUNK] > 0)
  {
    if(ch->pcdata->condition[COND_DRUNK] < 3)
      send_to_char("&OYou've had a bit to drink.&D.&D\r\n", ch);
    else if(ch->pcdata->condition[COND_DRUNK] < 5)
      send_to_char("&OYou've been drinking.&D.&D\r\n", ch);
    else if(ch->pcdata->condition[COND_DRUNK] < 10)
      send_to_char("&OYou are starting to feel &udrunk&D.&D\r\n", ch);
    else if(ch->pcdata->condition[COND_DRUNK] >= 10)
      send_to_char("&OYou are &udrunk&D.&D\r\n", ch);
  }

  if(IS_VAMPIRE(ch))
  {
    if(IS_OUTSIDE(ch))
    {
      switch (time_info.sunlight)
      {
        case SUN_LIGHT:
          send_to_char("&YYou're burning to a crisp under the sun.&D\r\n", ch);
          break;
        case SUN_RISE:
          send_to_char("&YYou feel your skin begin to burn in the morning light.&D\r\n", ch);
          break;
        case SUN_SET:
          send_to_char("&YYou feel your skin burning under the setting sun.&D\r\n", ch);
          break;
      }
    }
  }

  if(IS_AFFECTED(ch, AFF_SHAPESHIFT))
    send_to_char("&OYour shape-shifted into another shape.&D\r\n", ch);
  if(IS_AFFECTED(ch, AFF_DISGUISE))
    send_to_char("&OYour in a disguise to hide your identiy.&D\r\n", ch);
  if(IS_AFFECTED(ch, AFF_ANOINT))
    send_to_char("&OYou've been recently anointed.&D\r\n", ch);
  if(IS_AFFECTED(ch, AFF_POISON))
    send_to_char("&OYou feel as if you've been poisoned.&D\r\n", ch);
  if(IS_AFFECTED(ch, AFF_FURY) || IS_AFFECTED(ch, AFF_BERSERK))
    send_to_char("&OYou're in a &ubloody rage&D&O.&D\r\n", ch);
  if(IS_AFFECTED(ch, AFF_FLYING) || IS_AFFECTED(ch, AFF_FLOATING))
    send_to_char("&OYour feet aren't touching the ground.&D\r\n", ch);
  if(IS_AFFECTED(ch, AFF_CHARM))
    send_to_char("&OYou're charmed.&D\r\n", ch);
  if(IS_AFFECTED(ch, AFF_BLINDNESS))
    send_to_char("&O&i&uA thick film covers your eyes preventing you from seeing.&D\r\n", ch);
  if(IS_AFFECTED(ch, AFF_HIDE))
    send_to_char("&OYou seem to be &ihiding&D&O from someone.&D\r\n", ch);
  if(IS_AFFECTED(ch, AFF_MAIM))
    send_to_char("&OYou've been badly &umaimed&D&O, causing you to bleed freely.&D\r\n", ch);
  if(IS_AFFECTED(ch, AFF_SILENCE))
    send_to_char("&OA magical force prevents you from speaking.&D\r\n", ch);
  if(IS_AFFECTED(ch, AFF_SURREAL_SPEED))
    send_to_char("&OYour body seems to be moving as fast as&i lightning.&D\r\n", ch);
  if(IS_AFFECTED(ch, AFF_SLOW))
    send_to_char("&O&iYour body is moving at a slow rate..&D\r\n", ch);
  if(IS_AFFECTED(ch, AFF_PARALYSIS) || ch->position == POS_STUNNED)
    send_to_char("&OYou're suffering from some sort of &i&uparalysis.&D\r\n", ch);
  if(IS_AFFECTED(ch, AFF_FEIGN))
    send_to_char("&OYou lay stiff on the ground, playing possum.&D\r\n", ch);
  if(IS_AFFECTED(ch, AFF_CURSE))
    send_to_char("&OYou feel as if you're cursed.&D\r\n", ch);
  if(IS_AFFECTED(ch, AFF_IRON_SKIN))
    send_to_char("&OYour skin is as solid as iron.\r\n&D", ch);
  if(IS_AFFECTED(ch, AFF_UNSEARING_SKIN))
    send_to_char("&OYour flesh bares resistance to fire.\r\n&D", ch);
  if(IS_AFFECTED(ch, AFF_ROOT))
    send_to_char("&OYour feet are &urooted&D&O into the ground.\r\n&D", ch);
  if(IS_AFFECTED(ch, AFF_SHIELD) || IS_AFFECTED(ch, AFF_WARD))
    send_to_char("&OYour surrounded by a magical shield.\r\n&D", ch);
  if(IS_AFFECTED(ch, AFF_INFRARED))
    send_to_char("&OYour eyes glow bright red.\r\n&D", ch);
  if(IS_AFFECTED(ch, AFF_DETECT_INVIS))
    send_to_char("&OYou can see invisible people and objects.\r\n&D", ch);
  if(IS_AFFECTED(ch, AFF_DETECT_HIDDEN))
    send_to_char("&OYou can see hidden people and objects.\r\n&D", ch);
  if(IS_AFFECTED(ch, AFF_BOOST))
    send_to_char("&OYou've been boosted in power.\r\n&D", ch);
  if(IS_AFFECTED(ch, AFF_SANCTUARY))
    send_to_char("&OYou're defenses are greatly improved.\r\n&D", ch);
  if(IS_AFFECTED(ch, AFF_REACTIVE))
    send_to_char("&OYour body heals itself during combat.\r\n&D", ch);
  if(IS_AFFECTED(ch, AFF_TRUESIGHT))
    send_to_char("&OYour vision is enhanced.\r\n&D", ch);
  if(IS_AFFECTED(ch, AFF_NOSIGHT))
    send_to_char("&OYou can see without your eyes.\r\n&D", ch);
  if(IS_AFFECTED(ch, AFF_ICESHIELD))
    send_to_char("&OYour surrounded by a shield of ice.\r\n&D", ch);
  if(IS_AFFECTED(ch, AFF_SHOCKSHIELD))
    send_to_char("&OYou're surrounded by a shield of energy.\r\n&D", ch);
  if(IS_AFFECTED(ch, AFF_FIRESHIELD))
    send_to_char("&OYou're surrounded by a shield of fire.\r\n&D", ch);
  if(IS_AFFECTED(ch, AFF_RECOIL))
    send_to_char("&OYou're positioned in a coiled up stance.\r\n&D", ch);
  if(IS_AFFECTED(ch, AFF_KEEN_EYE))
    send_to_char("&OYour eyes are sharply attuned to your surroundings.\r\n&D", ch);
  if(IS_AFFECTED(ch, AFF_KINETIC))
    send_to_char("&OA barrier of kinetic energy swirls around you.\r\n&D", ch);
  if(IS_AFFECTED(ch, AFF_SUSTAIN_SELF))
    send_to_char("&OYour body is fighting off hunger.\r\n&D", ch);

  if(ch->hate_level > 0)
  {
    if(ch->hate_level <= 5)
      send_to_char("&OYou're watching out for someone.&D\r\n", ch);
    else if(ch->hate_level < 20)
      send_to_char("&OYou get the feeling someone is after you.\r\n&D", ch);
    else if(ch->hate_level >= 20)
      send_to_char("&OYou're hated amongst your peers.&D\r\n", ch);
  }
  if(!IS_BLIND(ch))
    send_to_char("\r\n&W--------------------------------------------------&D\r\n\r\n", ch);
  return;
}
