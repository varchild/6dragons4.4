#include "h/mud.h"

/* Run command taken from DOTD codebase - Samson 2-25-99 */
/* Added argument to let players specify how far to run.
 * Fixed an infinite loop bug where somehow a closed door would cause problems.
 * Added Overland support to the command. Samson 4-4-01
 */
/* Supressed display of rooms/terrain until you stop to prevent buffer overflows - Samson 4-16-01 */
void do_run(CHAR_DATA *ch, char *argument)
{
  char                    arg[MAX_INPUT_LENGTH];
  ROOM_INDEX_DATA        *from_room;
  EXIT_DATA              *pexit;
  int                     amount = 0, x;

  bool                    limited = FALSE;

  argument = one_argument(argument, arg);

  if(IS_NPC(ch) && !xIS_SET(ch->act, ACT_BEASTMELD))
    return;

  if(arg[0] == '\0')
  {
    send_to_char("Syntax Run <dir> <number>\r\n", ch);
    return;
  }

  if(ch->position != POS_STANDING && ch->position != POS_MOUNTED)
  {
    send_to_char("You are not in the correct position for that.\r\n", ch);
    return;
  }

  if(argument)
  {
    if(is_number(argument))
    {
      limited = TRUE;
      amount = atoi(argument);
    }
  }
  from_room = ch->in_room;

  if(!amount)
  {
    limited = TRUE;
    amount = 50;
  }

  if(limited)
  {
    for(x = 1; x <= amount; x++)
    {
      if((pexit = find_door(ch, arg, TRUE)) != NULL)
      {

        if(ch->move < 1)
        {
          send_to_char("You are too exhausted to run anymore.\r\n", ch);
          ch->move = 0;
          break;
        }
        if(move_char(ch, pexit, 0, TRUE) == rSTOP)
          break;

        if(ch->position != POS_STANDING && ch->position != POS_MOUNTED)
        {
          send_to_char("Your run has been interrupted!\r\n", ch);
          break;
        }
      }
    }
  }
  else
  {
    while((pexit = find_door(ch, arg, TRUE)) != NULL)
    {
      if(ch->move < 1)
      {
        send_to_char("You are too exhausted to run anymore.\r\n", ch);
        ch->move = 0;
        break;
      }
      if(move_char(ch, pexit, 0, TRUE) == rSTOP)
        break;

      if(ch->position != POS_STANDING && ch->position != POS_MOUNTED)
      {
        send_to_char("Your run has been interrupted!\r\n", ch);
        break;
      }
    }
  }

  {
    if(ch->in_room == from_room)
    {
      send_to_char("You try to run but don't get anywhere.\r\n", ch);
      act(AT_ACTION, "$n tries to run but doesn't get anywhere.", ch, NULL, NULL, TO_ROOM);
      return;
    }
  }
  if(xIS_SET(ch->act, PLR_ENHANCED) || (xIS_SET(ch->act, ACT_BEASTMELD) && xIS_SET(ch->master->act, PLR_ENHANCED)))
    send_to_char("!!SOUND(sound/run.wav)\r\n", ch);

  send_to_char("You slow down after your run.\r\n", ch);
  act(AT_ACTION, "$n slows down after $s run.", ch, NULL, NULL, TO_ROOM);
  return;
}
