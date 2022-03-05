
#include <string.h>
#include "h/mud.h"
#include "h/ships.h"
#include "h/files.h"

bool                    in_hash_table(const char *str);

/*
Just starting the idea for ship sailing code.  Doesn't have to be too fancy, but able to get elaborate enough to foster things such as
random sea encounters, and ship trading system of goods/services.
Possibly consider adding carpenter tradeskill to build these ships, and a few other things,
or just add to inventor crafts.

*/

short check_ship1(CHAR_DATA *ch)
{
  int                     shipone_low, shipone_hi;

  shipone_low = 56300;
  shipone_hi = 56305;
  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56300:
        occupy += 1;
        break;
      case 56301:
        occupy += 1;
        break;
      case 56302:
        occupy += 1;
        break;
      case 56303:
        occupy += 1;
        break;
      case 56304:
        occupy += 1;
        break;
      case 56305:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship ONE returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship ONE returning a FALSE value.");
  }
  return occupy;
}

short check_ship2(CHAR_DATA *ch)
{
  int                     shiptwo_low, shiptwo_hi;

  shiptwo_low = 56306;
  shiptwo_hi = 56311;
  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56306:
        occupy += 1;
        break;
      case 56307:
        occupy += 1;
        break;
      case 56308:
        occupy += 1;
        break;
      case 56309:
        occupy += 1;
        break;
      case 56310:
        occupy += 1;
        break;
      case 56311:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship TWO returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship TWO returning a FALSE value.");
  }
  return occupy;

}

short check_ship3(CHAR_DATA *ch)
{
  int                     shipthree_low, shipthree_hi;

  shipthree_low = 56312;
  shipthree_hi = 56317;
  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56312:
        occupy += 1;
        break;
      case 56313:
        occupy += 1;
        break;
      case 56314:
        occupy += 1;
        break;
      case 56315:
        occupy += 1;
        break;
      case 56316:
        occupy += 1;
        break;
      case 56317:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship THREE returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship THREE returning a FALSE value.");
  }
  return occupy;

}

short check_ship4(CHAR_DATA *ch)
{
  int                     shipfour_low, shipfour_hi;

  shipfour_low = 56318;
  shipfour_hi = 56323;

  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56318:
        occupy += 1;
        break;
      case 56319:
        occupy += 1;
        break;
      case 56320:
        occupy += 1;
        break;
      case 56321:
        occupy += 1;
        break;
      case 56322:
        occupy += 1;
        break;
      case 56323:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship FOUR returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship FOUR returning a FALSE value.");
  }
  return occupy;

}

short check_ship5(CHAR_DATA *ch)
{
  int                     shipfive_low, shipfive_hi;

  shipfive_low = 56324;
  shipfive_hi = 56329;

  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56324:
        occupy += 1;
        break;
      case 56325:
        occupy += 1;
        break;
      case 56326:
        occupy += 1;
        break;
      case 56327:
        occupy += 1;
        break;
      case 56328:
        occupy += 1;
        break;
      case 56329:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship FIVE returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship FIVE returning a FALSE value.");
  }
  return occupy;

}

short check_ship6(CHAR_DATA *ch)
{
  int                     shipsix_low, shipsix_hi;

  shipsix_low = 56330;
  shipsix_hi = 56335;

  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56330:
        occupy += 1;
        break;
      case 56331:
        occupy += 1;
        break;
      case 56332:
        occupy += 1;
        break;
      case 56333:
        occupy += 1;
        break;
      case 56334:
        occupy += 1;
        break;
      case 56335:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship SIX returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship SIX returning a FALSE value.");
  }
  return occupy;

}

short check_ship7(CHAR_DATA *ch)
{
  int                     shipseven_low, shipseven_hi;

  shipseven_low = 56336;
  shipseven_hi = 56341;

  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56336:
        occupy += 1;
        break;
      case 56337:
        occupy += 1;
        break;
      case 56338:
        occupy += 1;
        break;
      case 56339:
        occupy += 1;
        break;
      case 56340:
        occupy += 1;
        break;
      case 56341:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship SEVEN returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship SEVEN returning a FALSE value.");
  }
  return occupy;

}

short check_ship8(CHAR_DATA *ch)
{
  int                     shipeight_low, shipeight_hi;

  shipeight_low = 56342;
  shipeight_hi = 56347;

  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56342:
        occupy += 1;
        break;
      case 56343:
        occupy += 1;
        break;
      case 56344:
        occupy += 1;
        break;
      case 56345:
        occupy += 1;
        break;
      case 56346:
        occupy += 1;
        break;
      case 56347:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship EIGHT returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship EIGHT returning a FALSE value.");
  }
  return occupy;

}

short check_ship9(CHAR_DATA *ch)
{
  int                     shipnine_low, shipnine_hi;

  shipnine_low = 56348;
  shipnine_hi = 56353;

  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56348:
        occupy += 1;
        break;
      case 56349:
        occupy += 1;
        break;
      case 56350:
        occupy += 1;
        break;
      case 56351:
        occupy += 1;
        break;
      case 56352:
        occupy += 1;
        break;
      case 56353:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship NINE returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship NINE returning a FALSE value.");
  }
  return occupy;

}

short check_ship10(CHAR_DATA *ch)
{
  int                     shipten_low, shipten_hi;

  shipten_hi = 56359;
  shipten_low = 56354;

  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56354:
        occupy += 1;
        break;
      case 56355:
        occupy += 1;
        break;
      case 56356:
        occupy += 1;
        break;
      case 56357:
        occupy += 1;
        break;
      case 56358:
        occupy += 1;
        break;
      case 56359:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship TEN returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship TEN returning a FALSE value.");
  }
  return occupy;

}

short check_ship11(CHAR_DATA *ch)
{
  int                     shipone_low, shipone_hi;

  shipone_low = 56360;
  shipone_hi = 56365;
  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56360:
        occupy += 1;
        break;
      case 56361:
        occupy += 1;
        break;
      case 56362:
        occupy += 1;
        break;
      case 56363:
        occupy += 1;
        break;
      case 56364:
        occupy += 1;
        break;
      case 56365:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship ELEVEN returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship ELEVEN returning a FALSE value.");
  }
  return occupy;
}

short check_ship12(CHAR_DATA *ch)
{
  int                     shiptwo_low, shiptwo_hi;

  shiptwo_low = 56366;
  shiptwo_hi = 56371;
  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56366:
        occupy += 1;
        break;
      case 56367:
        occupy += 1;
        break;
      case 56368:
        occupy += 1;
        break;
      case 56369:
        occupy += 1;
        break;
      case 56370:
        occupy += 1;
        break;
      case 56371:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship TWELVE returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship TWELVE returning a FALSE value.");
  }
  return occupy;

}

short check_ship13(CHAR_DATA *ch)
{
  int                     shipthree_low, shipthree_hi;

  shipthree_low = 56372;
  shipthree_hi = 56377;
  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56372:
        occupy += 1;
        break;
      case 56373:
        occupy += 1;
        break;
      case 56374:
        occupy += 1;
        break;
      case 56375:
        occupy += 1;
        break;
      case 56376:
        occupy += 1;
        break;
      case 56377:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship THIRTEEN returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship THIRTEEN returning a FALSE value.");
  }
  return occupy;

}

short check_ship14(CHAR_DATA *ch)
{
  int                     shipfour_low, shipfour_hi;

  shipfour_low = 56378;
  shipfour_hi = 56383;

  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56378:
        occupy += 1;
        break;
      case 56379:
        occupy += 1;
        break;
      case 56380:
        occupy += 1;
        break;
      case 56381:
        occupy += 1;
        break;
      case 56382:
        occupy += 1;
        break;
      case 56383:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship FOURTEEN returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship FOURTEEN returning a FALSE value.");
  }
  return occupy;

}

short check_ship15(CHAR_DATA *ch)
{
  int                     shipfive_low, shipfive_hi;

  shipfive_low = 56384;
  shipfive_hi = 56389;

  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56384:
        occupy += 1;
        break;
      case 56385:
        occupy += 1;
        break;
      case 56386:
        occupy += 1;
        break;
      case 56387:
        occupy += 1;
        break;
      case 56388:
        occupy += 1;
        break;
      case 56389:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship FIFTEEN returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship FIFTEEN returning a FALSE value.");
  }
  return occupy;

}

short check_ship16(CHAR_DATA *ch)
{
  int                     shipsix_low, shipsix_hi;

  shipsix_low = 56390;
  shipsix_hi = 56395;

  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56390:
        occupy += 1;
        break;
      case 56391:
        occupy += 1;
        break;
      case 56392:
        occupy += 1;
        break;
      case 56393:
        occupy += 1;
        break;
      case 56394:
        occupy += 1;
        break;
      case 56395:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship SIXTEEN returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship SIXTEEN returning a FALSE value.");
  }
  return occupy;

}

short check_ship17(CHAR_DATA *ch)
{
  int                     shipseven_low, shipseven_hi;

  shipseven_low = 56396;
  shipseven_hi = 56401;

  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56396:
        occupy += 1;
        break;
      case 56397:
        occupy += 1;
        break;
      case 56398:
        occupy += 1;
        break;
      case 56399:
        occupy += 1;
        break;
      case 56400:
        occupy += 1;
        break;
      case 56401:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship SEVENTEEN returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship SEVENTEEN returning a FALSE value.");
  }
  return occupy;

}

short check_ship18(CHAR_DATA *ch)
{
  int                     shipeight_low, shipeight_hi;

  shipeight_low = 56402;
  shipeight_hi = 56407;

  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56402:
        occupy += 1;
        break;
      case 56403:
        occupy += 1;
        break;
      case 56404:
        occupy += 1;
        break;
      case 56405:
        occupy += 1;
        break;
      case 56406:
        occupy += 1;
        break;
      case 56407:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship EIGHTEEN returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship EIGHTEEN returning a FALSE value.");
  }
  return occupy;

}

short check_ship19(CHAR_DATA *ch)
{
  int                     shipnine_low, shipnine_hi;

  shipnine_low = 56408;
  shipnine_hi = 56413;

  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56408:
        occupy += 1;
        break;
      case 56409:
        occupy += 1;
        break;
      case 56410:
        occupy += 1;
        break;
      case 56411:
        occupy += 1;
        break;
      case 56412:
        occupy += 1;
        break;
      case 56413:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship NINETEEN returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship NINETEEN returning a FALSE value.");
  }
  return occupy;

}

short check_ship20(CHAR_DATA *ch)
{
  int                     shipten_low, shipten_hi;

  shipten_hi = 56419;
  shipten_low = 56414;

  short                   occupy = 0;

  for(ch = first_char; ch; ch = ch->next)
  {
    switch (ch->in_room->vnum)
    {
      case 56414:
        occupy += 1;
        break;
      case 56415:
        occupy += 1;
        break;
      case 56416:
        occupy += 1;
        break;
      case 56417:
        occupy += 1;
        break;
      case 56418:
        occupy += 1;
        break;
      case 56419:
        occupy += 1;
        break;
    }

  }
  if(occupy > 0)
  {
    log_printf("check ship TWENTY returning a TRUE value.");
  }
  if(occupy == 0)
  {
    log_printf("check ship TWENTY returning a FALSE value.");
  }
  return occupy;

}

/*
I'm gonna use xIS_SET( ch->act, ACT_SHIPWRIGHT )
TO SET MOBS TO SHIPWRIGHT FLAG, AND HAVE THEM SELL DEED OBJECTS OF THE SHIPS FOR PLAYERS.
*/
CHAR_DATA              *find_shipwright(CHAR_DATA *ch)
{
  CHAR_DATA              *shipwright = NULL;

  for(shipwright = ch->in_room->first_person; shipwright; shipwright = shipwright->next_in_room)
    if(IS_NPC(shipwright) && xIS_SET(shipwright->act, ACT_SHIPWRIGHT))
      break;

  return shipwright;
}

void do_hull_repair(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA              *shipwright;
  int                     cost = 0;
  char                    buf[MSL];

  if(!(shipwright = find_shipwright(ch)))
  {
    send_to_char("You're not in at a shipwright!\r\n", ch);
    return;
  }

  if(IS_NPC(ch))
    return;

  if(ch->pcdata->hull == 100)
  {
    snprintf(buf, MSL, "say %s, your ship hull looks like it is at 100 percent already.", ch->name);
    interpret(shipwright, buf);
    return;
  }
  cost = 100 - ch->pcdata->hull;
  if((GET_MONEY(ch, CURR_SILVER) < cost))
  {
    snprintf(buf, MSL, "say %s, you don't have enough silver to get your hull repaired you need %d silver.", ch->name, cost);
    interpret(shipwright, buf);
    return;
  }

  GET_MONEY(ch, CURR_SILVER) -= cost;
  act(AT_CYAN, "The shipwright repairs $n's ship hull.", ch, NULL, NULL, TO_ROOM);
  act(AT_CYAN, "The shipwright repairs your ship's hull.", ch, NULL, NULL, TO_CHAR);
  ch->pcdata->hull = 100;
  save_char_obj(ch);
  return;
}


void do_deal( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA              *shipwright;
    char                    buf[MSL];
    OBJ_DATA               *obj;
    int                     cost = 0;

    // Rent for one day = 500 gold - maybe use obj->timer
    // Purchase ship = 10,000 gold

    if ( IS_NPC( ch ) )
        return;

    if ( !( shipwright = find_shipwright( ch ) ) ) {
        send_to_char( "You're not in at a shipwright!\r\n", ch );
        return;
    }

    if ( !str_cmp( argument, "rent" ) ) {
        cost = 500;
    }
    else
        cost = 10000;

    if ( GET_MONEY( ch, CURR_GOLD ) < cost ) {
        ch_printf( ch, "You haven't got that much gold.\r\n" );
        return;
    }

    snprintf( buf, MSL, "say %s, you have purchased a good ship.", ch->name );
    interpret( shipwright, buf );

/* Setup a very high price to purchase a ship deed then add the object itself with a owner flag */

    obj = create_object( get_obj_index( OBJ_VNUM_DEED ), 0 );
    obj->level = ch->level;

    snprintf( buf, MSL, "%s's ship deed", ch->name );
    STRFREE( obj->short_descr );
    obj->short_descr = STRALLOC( buf );
    STRFREE( obj->name );
    obj->name = STRALLOC( buf );
    STRFREE( obj->description );
    obj->description = STRALLOC( "A deed to some sort of property has been left here." );
    /*
     * To avoid freeing a null pointer
     */
    if ( obj->owner == NULL ) {
        obj->owner = STRALLOC( ch->name );
    }
    else {
        STRFREE( obj->owner );
        obj->owner = STRALLOC( ch->name );
    }
    if ( !str_cmp( argument, "rent" ) ) {
        obj->timer = 100;
    }

    GET_MONEY( ch, CURR_GOLD ) -= cost;
    act( AT_CYAN, "The shipwright gives $n the deed.", ch, NULL, NULL, TO_ROOM );
    act( AT_CYAN, "The shipwright gives you the deed.", ch, NULL, NULL, TO_CHAR );
    obj = obj_to_char( obj, ch );
    ch->pcdata->hull = 100;
    save_char_obj( ch );
    return;
}



void do_deed(CHAR_DATA *ch, char *argument)
{
  OBJ_DATA               *obj;
  CHAR_DATA              *victim;
  char                    arg[MIL];
  char                    buf[MIL];

  argument = one_argument(argument, arg);

  if(arg[0] == '\0')
  {
    send_to_char("Sign the deed over to who?\r\n", ch);
    return;
  }
  if((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They aren't here.\r\n", ch);
    return;
  }

  if(IS_NPC(ch) || IS_NPC(victim))
    return;

  for(obj = ch->first_carrying; obj; obj = obj->next_content)
  {
    if(obj->pIndexData->vnum == OBJ_VNUM_DEED && !str_cmp(obj->owner, ch->name))
      break;
  }
  if(!obj)
  {
    send_to_char("You do not have a deed issued in your name.\r\n", ch);
    return;
  }

  /*
   * To avoid freeing a null pointer
   */
  if(obj->owner == NULL)
  {
    obj->owner = STRALLOC(victim->name);
  }
  else
  {
    STRFREE(obj->owner);
    obj->owner = STRALLOC(victim->name);
  }

  snprintf(buf, MIL, "%s's ship deed", victim->name);
  STRFREE(obj->short_descr);
  obj->short_descr = STRALLOC(buf);

  act(AT_CYAN, "$n signs over the deed to $N.", ch, NULL, victim, TO_ROOM);
  act(AT_CYAN, "You sign over the deed to $N.", ch, NULL, victim, TO_CHAR);
  victim->pcdata->hull = 100;
  save_char_obj(victim);
  return;
}

void do_depart(CHAR_DATA *ch, char *argument)
{
  if(IS_NPC(ch))
  {
    return;
  }

  if(ch->pcdata->ship == 0)
  {
    send_to_char("You are not on a ship to depart.\r\n", ch);
    return;
  }

  if(!str_cmp(argument, "abandon"))
  {
    if(ch->position == POS_FIGHTING)
    {
      CHAR_DATA              *victim;

      victim = who_fighting(ch);
      stop_fighting(ch, TRUE);
      stop_fighting(victim, TRUE);
      stop_hating(victim);
    }
    send_to_char("You throw caution to the wind, and abandon ship jumping off the gang plank!\r\n", ch);
    act(AT_CYAN, "$n runs up to the gang plank and jumps off into the water!", ch, NULL, NULL, TO_ROOM);
    if(xIS_SET(ch->act, PLR_COMMUNICATION))
      send_to_char("!!SOUND(sound/walktheplank.wav)\r\n", ch);

    if(ch->pcdata->watervnum == 0)
      ch->pcdata->watervnum = 43088;

    char_from_room(ch);
    char_to_room(ch, get_room_index(ch->pcdata->watervnum));
    ch->pcdata->ship = 0;
    ch->pcdata->watervnum = 0;
    ch->pcdata->boatvnum = 0;
    ch->pcdata->direction = 0;
    xREMOVE_BIT(ch->act, PLR_BOAT);
    interpret(ch, (char *)"look");
    return;
  }

// Check to see in view water
  if(ch->in_room->vnum != ch->pcdata->watervnum)
  {
    char_from_room(ch);
    char_to_room(ch, get_room_index(ch->pcdata->watervnum));
  }
  send_to_char
    ("You salute the Officer of the Day first and say, 'I request permission  to  leave\r\nthe  ship,  sir/ma’am.'  After receiving  permission, you then\r\nface and salute the ensign and depart!\r\n",
     ch);
  act(AT_CYAN, "$n salutes the Officer of the Day requesting permission to leave the ship.  $n then salutes the ensign and walks off the ship.", ch, NULL, NULL, TO_ROOM);
  ch->pcdata->ship = 0;
  ch->pcdata->watervnum = 0;
  ch->pcdata->boatvnum = 0;
  ch->pcdata->direction = 0;
  xREMOVE_BIT(ch->act, PLR_BOAT);
  interpret(ch, (char *)"look");
  return;
}

void do_aboard(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA              *gch;

  if(IS_NPC(ch))
  {
    return;
  }

// Check to see if player has the deed to a ship
  OBJ_DATA               *deed;

  for(deed = ch->first_carrying; deed; deed = deed->next_content)
  {
    if(deed->pIndexData->vnum == OBJ_VNUM_DEED && !str_cmp(deed->owner, ch->name))
      break;
  }
  if(!deed)
  {
    send_to_char("You do not have a deed issued in your name.\r\n", ch);
    return;
  }

  act(AT_CYAN,
      "$n suddenly comes to attention and faces the flag at the ship stern while saluting smartly.\r\n$n then faces the Officer of the Day and salutes while saying,\r\nRequest permission to come aboard.",
      ch, NULL, NULL, TO_ROOM);

// Is there a dock?
  if(ch->in_room->sector_type == SECT_DOCK)
  {
    ch->pcdata->watervnum = ch->in_room->vnum;
  }
  else
  {
    send_to_char("There is no ship here to go aboard your not even at the dock.\r\n", ch);
    return;
  }

  if(ch->pcdata->hull == 0)
  {
    send_to_char("Your ship is wrecked, first you will have to use the hull command to get it repaired.\r\n", ch);
    return;
  }

// Is there other players following owner of boat?
  ch_printf(ch,
            "You suddenly come to attention and face the %s flag at the stern of the ship while you salute smartly.\r\nYou then face the Officer of the Day and salute while saying Request\r\npermission to come aboard",
            ch->pcdata->htown_name);

  // Set the dock location
  if(ch->pcdata->watervnum == 0)
    ch->pcdata->watervnum = ch->in_room->vnum;

  if(check_ship1(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(FIRST_DECK_ROOM));
          gch->pcdata->ship = 1;
        }
      }
    }
    char_from_room(ch);
    char_to_room(ch, get_room_index(FIRST_DECK_ROOM));
    ch->pcdata->ship = 1;
  }
  else if(check_ship2(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(SECOND_DECK_ROOM));
          gch->pcdata->ship = 2;
        }
      }
    }

    char_from_room(ch);
    char_to_room(ch, get_room_index(SECOND_DECK_ROOM));
    ch->pcdata->ship = 2;
  }
  else if(check_ship3(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(THIRD_DECK_ROOM));
          gch->pcdata->ship = 3;
        }
      }
    }

    char_from_room(ch);
    char_to_room(ch, get_room_index(THIRD_DECK_ROOM));
    ch->pcdata->ship = 3;
  }
  else if(check_ship4(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(FOURTH_DECK_ROOM));
          gch->pcdata->ship = 4;
        }
      }
    }

    char_from_room(ch);
    char_to_room(ch, get_room_index(FOURTH_DECK_ROOM));
    ch->pcdata->ship = 4;
  }
  else if(check_ship5(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(FIFTH_DECK_ROOM));
          gch->pcdata->ship = 5;
        }
      }
    }

    char_from_room(ch);
    char_to_room(ch, get_room_index(FIFTH_DECK_ROOM));
    ch->pcdata->ship = 5;
  }
  else if(check_ship6(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(SIXTH_DECK_ROOM));
          gch->pcdata->ship = 6;
        }
      }
    }

    char_from_room(ch);
    char_to_room(ch, get_room_index(SIXTH_DECK_ROOM));
    ch->pcdata->ship = 6;
  }
  else if(check_ship7(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(SEVENTH_DECK_ROOM));
          gch->pcdata->ship = 7;
        }
      }
    }

    char_from_room(ch);
    char_to_room(ch, get_room_index(SEVENTH_DECK_ROOM));
    ch->pcdata->ship = 7;
  }
  else if(check_ship8(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(EIGHTH_DECK_ROOM));
          gch->pcdata->ship = 8;
        }
      }
    }

    char_from_room(ch);
    char_to_room(ch, get_room_index(EIGHTH_DECK_ROOM));
    ch->pcdata->ship = 8;
  }
  else if(check_ship9(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(NINETH_DECK_ROOM));
          gch->pcdata->ship = 9;
        }
      }
    }

    char_from_room(ch);
    char_to_room(ch, get_room_index(NINETH_DECK_ROOM));
    ch->pcdata->ship = 9;
  }
  else if(check_ship10(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(TENTH_DECK_ROOM));
          gch->pcdata->ship = 10;
        }
      }
    }

    char_from_room(ch);
    char_to_room(ch, get_room_index(TENTH_DECK_ROOM));
    ch->pcdata->ship = 10;
  }
  else if(check_ship11(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(ELEVEN_DECK_ROOM));
          gch->pcdata->ship = 11;
        }
      }
    }
    char_from_room(ch);
    char_to_room(ch, get_room_index(ELEVEN_DECK_ROOM));
    ch->pcdata->ship = 11;
  }
  else if(check_ship12(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(TWELVE_DECK_ROOM));
          gch->pcdata->ship = 12;
        }
      }
    }

    char_from_room(ch);
    char_to_room(ch, get_room_index(TWELVE_DECK_ROOM));
    ch->pcdata->ship = 12;
  }
  else if(check_ship13(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(THIRTEEN_DECK_ROOM));
          gch->pcdata->ship = 13;
        }
      }
    }

    char_from_room(ch);
    char_to_room(ch, get_room_index(THIRTEEN_DECK_ROOM));
    ch->pcdata->ship = 13;
  }
  else if(check_ship14(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(FOURTEEN_DECK_ROOM));
          gch->pcdata->ship = 14;
        }
      }
    }

    char_from_room(ch);
    char_to_room(ch, get_room_index(FOURTEEN_DECK_ROOM));
    ch->pcdata->ship = 14;
  }
  else if(check_ship15(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(FIFTEEN_DECK_ROOM));
          gch->pcdata->ship = 15;
        }
      }
    }

    char_from_room(ch);
    char_to_room(ch, get_room_index(FIFTEEN_DECK_ROOM));
    ch->pcdata->ship = 15;
  }
  else if(check_ship16(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(SIXTEEN_DECK_ROOM));
          gch->pcdata->ship = 16;
        }
      }
    }

    char_from_room(ch);
    char_to_room(ch, get_room_index(SIXTEEN_DECK_ROOM));
    ch->pcdata->ship = 16;
  }
  else if(check_ship17(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(SEVENTEEN_DECK_ROOM));
          gch->pcdata->ship = 17;
        }
      }
    }

    char_from_room(ch);
    char_to_room(ch, get_room_index(SEVENTEEN_DECK_ROOM));
    ch->pcdata->ship = 17;
  }
  else if(check_ship18(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(EIGHTEEN_DECK_ROOM));
          gch->pcdata->ship = 18;
        }
      }
    }

    char_from_room(ch);
    char_to_room(ch, get_room_index(EIGHTEEN_DECK_ROOM));
    ch->pcdata->ship = 18;
  }
  else if(check_ship19(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(NINETEEN_DECK_ROOM));
          gch->pcdata->ship = 19;
        }
      }
    }

    char_from_room(ch);
    char_to_room(ch, get_room_index(NINETEEN_DECK_ROOM));
    ch->pcdata->ship = 19;
  }
  else if(check_ship20(ch) == 0)
  {
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends in same room to ship.
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          char_from_room(gch);
          char_to_room(gch, get_room_index(TWENTY_DECK_ROOM));
          gch->pcdata->ship = 20;
        }
      }
    }

    char_from_room(ch);
    char_to_room(ch, get_room_index(TWENTY_DECK_ROOM));
    ch->pcdata->ship = 20;
  }
  else
  {
    log_string("Error: All starting ship deck rooms are currently in use.");
    log_printf("Error: Bad ship deck placement reached on %s. ", ch->name);
    char_from_room(ch);
    char_to_room(ch, get_room_index(FIRST_DECK_ROOM));
  }
  interpret(ch, (char *)"look");
  ch->pcdata->boatvnum = ch->in_room->vnum;

}

short inpilothouse(CHAR_DATA *ch)
{
  if(ch->pcdata->ship == 1)
  {
    if(ch->in_room->vnum == 56303)
      return TRUE;
  }
  else if(ch->pcdata->ship == 2)
  {
    if(ch->in_room->vnum == 56309)
      return TRUE;
  }
  else if(ch->pcdata->ship == 3)
  {
    if(ch->in_room->vnum == 56316)
      return TRUE;
  }
  else if(ch->pcdata->ship == 4)
  {
    if(ch->in_room->vnum == 56322)
      return TRUE;
  }
  else if(ch->pcdata->ship == 5)
  {
    if(ch->in_room->vnum == 56329)
      return TRUE;
  }
  else if(ch->pcdata->ship == 6)
  {
    if(ch->in_room->vnum == 56335)
      return TRUE;
  }
  else if(ch->pcdata->ship == 7)
  {
    if(ch->in_room->vnum == 56341)
      return TRUE;
  }
  else if(ch->pcdata->ship == 8)
  {
    if(ch->in_room->vnum == 56347)
      return TRUE;
  }
  else if(ch->pcdata->ship == 9)
  {
    if(ch->in_room->vnum == 56353)
      return TRUE;
  }
  else if(ch->pcdata->ship == 10)
  {
    if(ch->in_room->vnum == 56359)
      return TRUE;
  }
  else if(ch->pcdata->ship == 11)
  {
    if(ch->in_room->vnum == 56365)
      return TRUE;
  }
  else if(ch->pcdata->ship == 12)
  {
    if(ch->in_room->vnum == 56371)
      return TRUE;
  }
  else if(ch->pcdata->ship == 13)
  {
    if(ch->in_room->vnum == 56377)
      return TRUE;
  }
  else if(ch->pcdata->ship == 14)
  {
    if(ch->in_room->vnum == 56383)
      return TRUE;
  }
  else if(ch->pcdata->ship == 15)
  {
    if(ch->in_room->vnum == 56389)
      return TRUE;
  }
  else if(ch->pcdata->ship == 16)
  {
    if(ch->in_room->vnum == 56395)
      return TRUE;
  }
  else if(ch->pcdata->ship == 17)
  {
    if(ch->in_room->vnum == 56401)
      return TRUE;
  }
  else if(ch->pcdata->ship == 18)
  {
    if(ch->in_room->vnum == 56407)
      return TRUE;
  }
  else if(ch->pcdata->ship == 19)
  {
    if(ch->in_room->vnum == 56413)
      return TRUE;
  }
  else if(ch->pcdata->ship == 20)
  {
    if(ch->in_room->vnum == 56419)
      return TRUE;
  }
  return FALSE;
}

void do_pilot(CHAR_DATA *ch, char *argument)
{
// Enter the Captain's cabin where the ship can be steered only owner can use this command
// Check to see if player has the deed to a ship
  OBJ_DATA               *deed;

  for(deed = ch->first_carrying; deed; deed = deed->next_content)
  {
    if(deed->pIndexData->vnum == OBJ_VNUM_DEED && deed->owner && !str_cmp(deed->owner, ch->name))
      break;
  }
  if(!deed)
  {
    send_to_char("You do not have a deed issued in your name.\r\n", ch);
    return;
  }

  if(str_cmp(argument, "enter") && str_cmp(argument, "leave") && str_cmp(argument, "setsail") && str_cmp(argument, "dropanchor"))
  {
    send_to_char("Syntax Pilot choice\r\n", ch);
    send_to_char("Choice being: enter, setsail, dropanchor or leave\r\n", ch);
    return;
  }

  if(ch->pcdata->ship == 0)
  {
    bug("Do_Pilot %s trying to use pilot command with ship set to zero!\r\n", ch->name);
    return;
  }

  if(!str_cmp(argument, "setsail"))
  {
    if(!inpilothouse(ch))
    {
      send_to_char("You must be in the pilot house to issue that order.\r\n", ch);
      return;
    }
    // Check for owner of ship to issue command
    if(!IS_SET(ch->in_room->room_flags, ROOM_ANCHOR))
    {
      send_to_char("You are already set for sailing.\r\n", ch);
      return;
    }
    act(AT_CYAN, "You bellow, all hands prepare to set sail!", ch, NULL, NULL, TO_CHAR);
    act(AT_CYAN, "Several deck hands spring into action raising the anchor and sails.", ch, NULL, NULL, TO_CHAR);
    act(AT_CYAN, "$n bellows, all hands prepare to set sail!", ch, NULL, NULL, TO_ROOM);
    act(AT_CYAN, "Several deck hands spring into action raising the anchor and sails.", ch, NULL, NULL, TO_ROOM);
    REMOVE_BIT(ch->in_room->room_flags, ROOM_ANCHOR);
  }

  if(!str_cmp(argument, "dropanchor"))
  {
    if(!inpilothouse(ch))
    {
      send_to_char("You must be in the pilot house to issue that order.\r\n", ch);
      return;
    }

    // Check for owner of ship to issue command
    if(IS_SET(ch->in_room->room_flags, ROOM_ANCHOR))
    {
      send_to_char("You are already dropped anchor.\r\n", ch);
      return;
    }
    act(AT_CYAN, "You bellow, all hands prepare to drop anchor!", ch, NULL, NULL, TO_CHAR);
    act(AT_CYAN, "Several deck hands spring into action lowering the anchor and sails.", ch, NULL, NULL, TO_CHAR);
    act(AT_CYAN, "$n bellows, all hands prepare to drop anchor!", ch, NULL, NULL, TO_ROOM);
    act(AT_CYAN, "Several deck hands spring into action lowering the anchor and sails.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->in_room->room_flags, ROOM_ANCHOR);
  }

  if(!str_cmp(argument, "enter"))
  {
    ch->pcdata->boatvnum = ch->in_room->vnum;

// Check for which ship the player is on to know what pilot house to send too
    switch (ch->pcdata->ship)
    {
      case 1:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56303));
        break;
      case 2:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56309));
        break;
      case 3:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56316));
        break;
      case 4:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56322));
        break;
      case 5:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56329));
        break;
      case 6:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56335));
        break;
      case 7:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56341));
        break;
      case 8:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56347));
        break;
      case 9:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56353));
        break;
      case 10:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56359));
        break;
      case 11:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56365));
        break;
      case 12:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56371));
        break;
      case 13:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56377));
        break;
      case 14:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56383));
        break;
      case 15:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56389));
        break;
      case 16:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56395));
        break;
      case 17:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56401));
        break;
      case 18:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56407));
        break;
      case 19:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56413));
        break;
      case 20:
        char_from_room(ch);
        char_to_room(ch, get_room_index(56419));
        break;
    }
    interpret(ch, (char *)"look");

  }

  if(!str_cmp(argument, "leave"))
  {
    char_from_room(ch);
    char_to_room(ch, get_room_index(ch->pcdata->boatvnum));
    interpret(ch, (char *)"look");
  }

  return;

}

void do_sail(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA              *gch;

  if(IS_NPC(ch))
    return;

  // Put some check here so they cannot sail if sea_monster attack until dealt with
  if(ch->pcdata->cansail == 2)
  {
    send_to_char("You cannot sail until the ship invader is destroyed.\r\n", ch);
    return;
  }

  if(ch->pcdata->direction == 0)
  {
    send_to_char("You must first steer the proper course for the ship.\r\n", ch);
    return;
  }

  if(ch->in_room->vnum != ch->pcdata->watervnum)
  {
    send_to_char("Sir you will have to use the water view before you can sail.\r\n", ch);
    return;
  }

  if(ch->pcdata->direction == 1)
  {
    move_char(ch, get_exit(ch->in_room, DIR_NORTH), 0, FALSE);
    ch->pcdata->watervnum = ch->in_room->vnum;
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends to same water location
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          gch->pcdata->watervnum = ch->in_room->vnum;
        }
      }
    }
    return;
  }
  else if(ch->pcdata->direction == 2)
  {
    move_char(ch, get_exit(ch->in_room, DIR_EAST), 0, FALSE);
    ch->pcdata->watervnum = ch->in_room->vnum;
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends to same water location
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          gch->pcdata->watervnum = ch->in_room->vnum;
        }
      }
    }
    return;
  }
  else if(ch->pcdata->direction == 3)
  {
    move_char(ch, get_exit(ch->in_room, DIR_WEST), 0, FALSE);
    ch->pcdata->watervnum = ch->in_room->vnum;
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends to same water location
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          gch->pcdata->watervnum = ch->in_room->vnum;
        }
      }
    }
    return;
  }
  else if(ch->pcdata->direction == 4)
  {
    move_char(ch, get_exit(ch->in_room, DIR_SOUTH), 0, FALSE);
    ch->pcdata->watervnum = ch->in_room->vnum;
    if(IS_GROUPED(ch))
    {
      // add this loop to take grouped friends to same water location
      for(gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
      {
        if(is_same_group(gch, ch) && !IS_NPC(gch) && gch != ch)
        {
          gch->pcdata->watervnum = ch->in_room->vnum;
        }
      }
    }
    return;
  }
  return;
}

// Change steer command to only plot ship course, so that sail command will send them in that direction.
void do_steer(CHAR_DATA *ch, char *argument)
{
// 1 = north, 2 = east, 3 = west, and 4 = south

// Check for owner of ship
  OBJ_DATA               *deed;

  for(deed = ch->first_carrying; deed; deed = deed->next_content)
  {
    if(deed->pIndexData->vnum == OBJ_VNUM_DEED && !str_cmp(deed->owner, ch->name))
      break;
  }
  if(!deed)
  {
    send_to_char("You do not have a deed issued in your name.\r\n", ch);
    return;
  }

  if(ch->pcdata->ship == 0)
  {
    bug("Do_steer %s trying to use steer command with ship set to zero!\r\n", ch->name);
    return;
  }

  // Check for owner of ship to issue command
  if(IS_SET(ch->in_room->room_flags, ROOM_ANCHOR))
  {
    send_to_char("You have to pilot setsail before you can steer.\r\n", ch);
    return;
  }

  if(!xIS_SET(ch->act, PLR_BOAT))
  {
    xSET_BIT(ch->act, PLR_BOAT);  // Use to make sure only steer on
    // water
  }
  if(ch->pcdata->watervnum == 0)
  {
    bug("Do_steer %s trying to use steer command with watervnum set to zero!\r\n", ch->name);
    return;
  }
  if(ch->in_room->vnum != ch->pcdata->watervnum)
  {
    send_to_char("You must use the view water command before you can steer the ship any direction.\r\n", ch);
    return;
  }

  if(str_cmp(argument, "north") && str_cmp(argument, "n") && str_cmp(argument, "east")
     && str_cmp(argument, "e") && str_cmp(argument, "west") && str_cmp(argument, "w") && str_cmp(argument, "south") && str_cmp(argument, "s"))
  {
    send_to_char("Proper syntax steer direction\r\n", ch);
    return;
  }

  act(AT_CYAN, "You bark out an order, to trim the sails and turn the rudder!", ch, NULL, NULL, TO_CHAR);

  if(!str_cmp(argument, "north") || !str_cmp(argument, "n"))
  {
    act(AT_CYAN, "The exec officer yells, 'Steering to the north, Captain!'", ch, NULL, NULL, TO_CHAR);
    act(AT_CYAN, "The exec officer hollars at the crew, 'Steer to the north, double quick!'", ch, NULL, NULL, TO_CHAR);
    act(AT_CYAN, "The ship slowly comes about to the north.", ch, NULL, NULL, TO_CHAR);
    ch->pcdata->direction = 1;
  }
  if(!str_cmp(argument, "east") || !str_cmp(argument, "e"))
  {
    act(AT_CYAN, "The exec officer yells, 'Steering to the east, Captain!'", ch, NULL, NULL, TO_CHAR);
    act(AT_CYAN, "The exec officer hollars at the crew, 'Steer to the east, double quick!'", ch, NULL, NULL, TO_CHAR);
    act(AT_CYAN, "The ship slowly comes about to the east.", ch, NULL, NULL, TO_CHAR);
    ch->pcdata->direction = 2;
  }
  if(!str_cmp(argument, "west") || !str_cmp(argument, "w"))
  {
    act(AT_CYAN, "The exec officer yells, 'Steering to the west, Captain!'", ch, NULL, NULL, TO_CHAR);
    act(AT_CYAN, "The exec officer hollars at the crew, 'Steer to the west, double quick!'", ch, NULL, NULL, TO_CHAR);
    act(AT_CYAN, "The ship slowly comes about to the west.", ch, NULL, NULL, TO_CHAR);
    ch->pcdata->direction = 3;
  }
  if(!str_cmp(argument, "south") || !str_cmp(argument, "s"))
  {
    act(AT_CYAN, "The exec officer yells, 'Steering to the south, Captain!'", ch, NULL, NULL, TO_CHAR);
    act(AT_CYAN, "The exec officer hollars at the crew, 'Steer to the south, double quick!'", ch, NULL, NULL, TO_CHAR);
    act(AT_CYAN, "The ship slowly comes about to the south.", ch, NULL, NULL, TO_CHAR);
    ch->pcdata->direction = 4;
  }

//  Move the ship.
}

/* While at helm of ship change prompt to
Wind amount direction, Hull Durability percentage, Sail Direction
*/

void do_view(CHAR_DATA *ch, char *argument)
{
  if(IS_NPC(ch))
    return;

  if(ch->pcdata->cansail == 2)
    ch->pcdata->cansail = 0;

// Check to see if player has the deed to a ship
  OBJ_DATA               *deed;

  for(deed = ch->first_carrying; deed; deed = deed->next_content)
  {
    if(deed->pIndexData->vnum == OBJ_VNUM_DEED && !str_cmp(deed->owner, ch->name))
      break;
  }
  if(!deed)
  {
    send_to_char("You do not have a deed issued in your name.\r\n", ch);
    return;
  }

  if(ch->pcdata->boatvnum == 0)
  {
    bug("Do_View %s trying to use view command with boatvnum set to zero!\r\n", ch->name);
    return;
  }
  if(ch->pcdata->watervnum == 0)
  {
    bug("Do_View %s trying to use view command with watervnum set to zero!\r\n", ch->name);
    return;
  }
  if(str_cmp(argument, "boat") && str_cmp(argument, "water"))
  {
    send_to_char("Syntax View choice\r\n", ch);
    send_to_char("Choice being: boat or water\r\n", ch);
    return;
  }

// use ch->pcdata->boatvnum to set where the player is in the boat
// use ch->pcdata->watervnum to set where the boat is in the ocean
  if(!str_cmp(argument, "boat"))
  {
    if(xIS_SET(ch->act, PLR_BOAT))
      xREMOVE_BIT(ch->act, PLR_BOAT);
    char_from_room(ch);
    char_to_room(ch, get_room_index(ch->pcdata->boatvnum));
    interpret(ch, (char *)"look");
  }
  if(!str_cmp(argument, "water"))
  {
    if(!xIS_SET(ch->act, PLR_BOAT))
      xSET_BIT(ch->act, PLR_BOAT);
    char_from_room(ch);
    char_to_room(ch, get_room_index(ch->pcdata->watervnum));
    interpret(ch, (char *)"look");
  }
  return;
}
