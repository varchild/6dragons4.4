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
/* commented out due to being a forward delcaration error
extern const struct carpenter_table lumber_type[];
extern const struct smith_ores_type smith_ores_table[];
extern const struct smith_items_type tier_one[];
extern const struct baker_table fruit_types[];
extern const struct baker_table food_one[];
extern const struct baker_table drink_one[];
extern const struct hunter_table hunted_one[];
extern const struct tanner_table tanned_one[];
extern const struct jeweler_table jewelry_one[];
extern const struct unearthed_table unearthed_one[];
extern const struct draconic_table craft_one[];
*/

typedef struct ReactivefailQQ REACTIVE_FAIL_DATA;

extern REACTIVE_FAIL_DATA *first_qq;
extern REACTIVE_FAIL_DATA *last_qq;

#define CRAFTQS_FILE "craftqs.dat"

struct ReactivefailQQ /* hahah i'm hilarious - Volk */
{
  REACTIVE_FAIL_DATA     *next;
  REACTIVE_FAIL_DATA     *prev;
  const char             *question; /* The question itself! */
  const char             *answer1;  /* The answer! */
  const char             *answer2;  /* Might have other answers.. */
  const char             *answer3;  /* as above */
  int                     trade;  /* What trade it applies to */
  int                     number; /* Makes it easier to find */
};

struct smith_items_type
{
  const char             *name;
  int                     level;
  int                     cost;
  int                     weight;
  int                     wear_flags;
  int                     item_type;
  int                     base_v0;
  int                     base_v1;
  int                     base_v2;
  int                     base_v3;
  int                     base_v4;
  int                     base_v5;
  int                     base_v6;
};

struct jeweler_table
{
  const char             *name;
  int                     level;
  int                     cost;
  int                     weight;
  int                     wear_flags;
  int                     item_type;
  int                     base_v0;
  int                     base_v1;
  int                     base_v2;
  int                     base_v3;
  int                     base_v4;
  int                     base_v5;
  int                     base_v6;
};

struct draconic_table
{
  const char             *name;
  const char             *short_descr;
  int                     level;
  int                     cost;
  int                     weight;
  int                     wear_flags;
  int                     item_type;
  int                     base_v0;
  int                     base_v1;
  int                     base_v2;
  int                     base_v3;
  int                     base_v4;
  int                     base_v5;
  int                     base_v6;
};

struct baker_table
{
  const char             *name;
  int                     level;
  int                     cost;
  int                     weight;
  int                     wear_flags;
  int                     item_type;
  int                     base_v0;
  int                     base_v1;
  int                     base_v2;
  int                     base_v3;
  int                     base_v4;
  int                     base_v5;
  int                     base_v6;
};

struct tanner_table
{
  const char             *name;
  const char             *short_descr;
  int                     level;
  int                     cost;
  int                     weight;
  int                     wear_flags;
  int                     item_type;
  int                     base_v0;
  int                     base_v1;
  int                     base_v2;
  int                     base_v3;
  int                     base_v4;
  int                     base_v5;
  int                     base_v6;
};

struct hunter_table
{
  const char             *name;
  const char             *short_descr;
  const char             *long_descr;
  int                     tlevel;
  short                   race;
  short                   level;
};

struct smith_ores_type
{
  const char             *name;
  int                     cost_stone;
  int                     armor_mod;
  int                     workability;
  const char             *color;
};

struct unearthed_table
{
  const char             *name;
  int                     cost_stone;
  int                     armor_mod;
  int                     workability;
  const char             *color;
  int                     level;
};

const struct smith_ores_type smith_ores_table[] = {
  {"bronze", 10, 10, 25, "reddish-gold"}, /* 1 */
  {"silver", 120, 8, 18, "bright"}, /* 4 */
  {"gold", 225, 5, 16, "gleaming"}, /* 5 */
  {"iron", 25, 15, 22, "bright-gray"},  /* 2 */
  {"steel", 50, 25, 20, "silvery-gray"},  /* 3 */
  {"titanium", 300, 30, 25, "bright-light-green"},  /* 6 */
  {NULL, 0}
};

const struct unearthed_table unearthed_one[] = {
  {"titanium", 10, 10, 25, "bright-light-green", 1},  /* 1 */
  {"vibranium", 120, 8, 18, "bright-silvery-gray", 5},  /* 4 */
  {"adamantium", 225, 5, 16, "gleaming-silvery-gray", 10},  /* 5 */
  {"carbonadium", 25, 15, 22, "dull-dark-gray", 15},  /* 2 */
  {"meteor", 300, 25, 20, "red-green-swirling", 20},  /* 3 */
  {NULL, 0}
};

const struct baker_table fruit_types[] = {
  {"strawberries", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 3, 0, 0},  /* 1 */
  {"oranges", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 1, 0, 0}, /* 2 */
  {"blueberries", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 4, 0, 0}, /* 3 */
  {"raspberries", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 6, 0, 0}, /* 4 */
  {"watermelons", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 9, 0, 0}, /* 5 */
  {"peaches", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 1, 0, 0}, /* 6 */
  {"pears", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 9, 0, 0}, /* 7 */

  {"corn", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 14, 0, 0}, /* 1 */
  {"lettuce", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 11, 0, 0},  /* 2 */
  {"carrots", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 1, 0, 0}, /* 5 */
  {"onions", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 5, 0, 0},  /* 6 */
  {"radishes", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 3, 0, 0},  /* 7 */
  {"pickles", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 11, 0, 0},  /* 8 */

  {"cinnamon", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 1, 0, 0},  /* 1 */
  {"sugar-cane", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 5, 0, 0},  /* 2 */
  {"garlic", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 5, 0, 0},  /* 3 */
  {"wheat", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 1, 0, 0}, /* 4 */
  {"fig", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 11, 0, 0},  /* 5 */
  {"ginger", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 1, 0, 0},  /* 6 */
  {"pepper", 0, 1, 2, ITEM_TAKE, ITEM_RAW, 0, 0, 0, 0, 13, 0, 0}, /* 8 */
  {NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const struct jeweler_table jewelry_one[] = {
  {"earring", 1, 10, 2, ITEM_TAKE | ITEM_WEAR_EARS, ITEM_ARMOR, 0, 0, 0, 0, 0, 0, 0}, /* 1 
                                                                                       */
  {"ring", 1, 1, 2, ITEM_TAKE | ITEM_WEAR_FINGER, ITEM_ARMOR, 12, 0, 0, 6, 6, 100, 5},
  {"necklace", 1, 0, 2, ITEM_TAKE | ITEM_WEAR_NECK, ITEM_ARMOR, 12, 0, 0, 6, 6, 30, 5},
  {"sash", 1, 1, 2, ITEM_TAKE | ITEM_WEAR_WAIST, ITEM_ARMOR, 12, 0, 0, 6, 2, 0, 0},
  {"gloves", 1, 1, 2, ITEM_TAKE | ITEM_WEAR_HANDS, ITEM_ARMOR, 12, 0, 0, 5, 14, 30, 5}, /* 5 
                                                                                         */
  {"anklet", 1, 1, 2, ITEM_TAKE | ITEM_WEAR_ANKLE, ITEM_ARMOR, 0, 10979, 10982, 6, 6, 30, 15},
  {"bracelet", 1, 1, 2, ITEM_TAKE | ITEM_WEAR_WRIST, ITEM_ARMOR, 10978, 0, 10982, 6, 6, 30, 15},
  {"pauldrons", 1, 1, 2, ITEM_TAKE | ITEM_WEAR_SHOULDERS, ITEM_ARMOR, 0, 10981, 10983, 6, 6, 30, 15},
  {"leggings", 5, 1, 2, ITEM_TAKE | ITEM_WEAR_LEGS, ITEM_ARMOR, 10980, 0, 10983, 6, 6, 30, 15},
  {"dagger", 5, 1, 0, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 2000, 0, 0, 6, 2, 30, 10}, /* 10 
                                                                                       */
  {"sandals", 5, 3, 2, ITEM_TAKE | ITEM_WEAR_FEET, ITEM_ARMOR, 500, 500, 0, 0, 0, 0, 0},
  {"sabre", 5, 1, 2, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 12, 0, 0, 1, 1, 0, 0},
  {"coat", 5, 1, 2, ITEM_TAKE | ITEM_WEAR_BODY, ITEM_ARMOR, 0, 0, 0, 0, 0, 0, 888},
  {"cape", 5, 1, 2, ITEM_TAKE | ITEM_WEAR_ABOUT, ITEM_ARMOR, 12, 0, 0, 1, 1, 0, 0},
  {"crown", 5, 1, 2, ITEM_TAKE | ITEM_WEAR_HEAD, ITEM_ARMOR, 0, 0, 0, 0, 0, 0, 888},

  {"battle-axe", 10, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 2, 0, 3, 9, 0, 0},
  {"back-scale", 10, 1, 1, ITEM_TAKE | ITEM_WEAR_BACK, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"fieldplate-suit", 10, 1, 1, ITEM_TAKE | ITEM_WEAR_BODY, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"plated-boots", 10, 1, 8, ITEM_TAKE | ITEM_WEAR_FEET, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"plated-gauntlets", 10, 1, 2, ITEM_TAKE | ITEM_WEAR_HANDS, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"mace", 10, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 2, 0, 4, 5, 0, 0},
  {"round-shield", 10, 1, 8, ITEM_TAKE | ITEM_WEAR_SHIELD, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"scale-mask", 10, 1, 8, ITEM_TAKE | ITEM_WEAR_FACE, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
// 23
  {"tanto-blade", 15, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 2, 0, 2, 2, 0, 0},
  {"scale-girdle", 15, 1, 1, ITEM_TAKE | ITEM_WEAR_WAIST, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"fullplate-suit", 15, 1, 1, ITEM_TAKE | ITEM_WEAR_BODY, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"ornate-ring", 15, 1, 8, ITEM_TAKE | ITEM_WEAR_FINGER, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"ear-stud", 15, 1, 2, ITEM_TAKE | ITEM_WEAR_EARS, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"sceptre", 15, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 2, 0, 4, 5, 0, 0},
  {"fullplate-visor", 15, 1, 8, ITEM_TAKE | ITEM_WEAR_EYES, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"fullplate-helmet", 15, 1, 8, ITEM_TAKE | ITEM_WEAR_HEAD, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
// 31
  {NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const struct baker_table food_one[] = {
  {"snack", 0, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0}, /* 1 */
  {"dessert", 0, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0}, /* 2 */
  {"pie", 0, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0}, /* 3 */
  {"cake", 0, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0},  /* 4 */
  {"salad", 0, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0}, /* 5 */
  {"entree", 0, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0},  /* 6 */
  {"loaf", 0, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0},  /* 7 */
  {"muffin", 0, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0},  /* 8 */

  {"soup", 5, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0},  /* 1 */
  {"stuffing", 5, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0},  /* 2 */
  {"cream", 5, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0}, /* 3 */
  {"roast", 5, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0}, /* 4 */
  {"stew", 5, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0},  /* 5 */
  {"tart", 5, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0},  /* 6 */
  {"pastry", 5, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0},  /* 7 */
  {"pudding", 5, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0}, /* 8 */

  {"crisp", 15, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0},  /* 1 */
  {"patties", 15, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0},  /* 2 */
  {"skewers", 15, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0},  /* 3 */
  {"crumble", 15, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0},  /* 4 */
  {"spread", 15, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0}, /* 5 */
  {"strudel", 15, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0},  /* 6 */
  {"casserole", 15, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0},  /* 7 */
  {"sandwich", 15, 1, 2, ITEM_TAKE, ITEM_FOOD, 0, 0, 0, 0, 0, 0}, /* 8 */
  {NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const struct baker_table drink_one[] = {
  {"wooden-decanter", 0, 1, 1, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0},  /* 1 */
  {"rabbitskin-flask", 0, 1, 2, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0}, /* 2 */
  {"wooden-goblet", 0, 1, 2, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0},  /* 3 */
  {"wooden-tankard", 0, 1, 2, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0}, /* 4 */
  {"wooden-chalice", 0, 1, 2, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0}, /* 5 */
  {"wooden-vessell", 0, 1, 2, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0}, /* 6 */
  {"wooden-mug", 0, 1, 2, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0}, /* 7 */
  {"wooden-pitcher", 0, 1, 3, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0}, /* 8 */

  {"bronze-decanter", 5, 1, 2, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0},  /* 1 */
  {"stagskin-flask", 5, 1, 1, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0}, /* 2 */
  {"bronze-goblet", 5, 1, 2, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0},  /* 3 */
  {"bronze-tankard", 5, 1, 2, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0}, /* 4 */
  {"bronze-chalice", 5, 1, 2, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0}, /* 5 */
  {"bronze-vessell", 5, 1, 2, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0}, /* 6 */
  {"bronze-mug", 5, 1, 2, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0}, /* 7 */
  {"bronze-pitcher", 5, 1, 5, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0}, /* 8 */

  {"iron-decanter", 15, 1, 2, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0}, /* 1 */
  {"deerskin-flask", 15, 1, 1, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0},  /* 2 */
  {"iron-goblet", 15, 1, 2, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0}, /* 3 */
  {"iron-tankard", 15, 1, 2, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0},  /* 4 */
  {"iron-chalice", 15, 1, 2, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0},  /* 5 */
  {"iron-vessell", 15, 1, 2, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0},  /* 6 */
  {"iron-mug", 15, 1, 2, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0},  /* 7 */
  {"iron-pitcher", 15, 1, 5, ITEM_TAKE, ITEM_DRINK_CON, 0, 0, 0, 0, 0, 0},  /* 8 */
  {NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const struct smith_items_type tier_one[] = {
  {"dagger", 0, 1, 2, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 1, 0, 2, 2, 0, 0},
  {"plate-torque", 0, 1, 3, ITEM_TAKE | ITEM_WEAR_NECK, ITEM_ARMOR, 0, 1, 0, 0, 0, 0, 0},
  {"mask", 0, 1, 2, ITEM_TAKE | ITEM_WEAR_FACE, ITEM_ARMOR, 0, 1, 0, 0, 0, 0, 0},
  {"girdle", 0, 1, 2, ITEM_TAKE | ITEM_WEAR_WAIST, ITEM_ARMOR, 0, 1, 0, 0, 0, 0, 0},
  {"scale-tunic", 0, 1, 8, ITEM_TAKE | ITEM_WEAR_BODY, ITEM_ARMOR, 0, 2, 0, 0, 0, 0, 0},
  {"scale-wrist", 0, 1, 3, ITEM_TAKE | ITEM_WEAR_WRIST, ITEM_ARMOR, 0, 2, 0, 0, 0, 0, 0},
  {"scale-helm", 0, 1, 3, ITEM_TAKE | ITEM_WEAR_HEAD, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"buckler-shield", 0, 1, 2, ITEM_TAKE | ITEM_WEAR_SHIELD, ITEM_ARMOR, 0, 2, 0, 0, 0, 0, 0},
// 8
  {"longsword", 5, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 2, 0, 1, 1, 0, 0},
  {"finger-ring", 5, 1, 1, ITEM_TAKE | ITEM_WEAR_FINGER, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"ear-hoop", 5, 1, 1, ITEM_TAKE | ITEM_WEAR_EARS, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"scale-armlets", 5, 1, 8, ITEM_TAKE | ITEM_WEAR_ARMS, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"scale-visor", 5, 1, 2, ITEM_TAKE | ITEM_WEAR_EYES, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"shortsword", 5, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 2, 0, 1, 1, 0, 0},
  {"plate-leggings", 5, 1, 8, ITEM_TAKE | ITEM_WEAR_LEGS, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"scale-anklet", 5, 1, 8, ITEM_TAKE | ITEM_WEAR_ANKLE, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
// 16
  {"battle-axe", 10, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 2, 0, 3, 9, 0, 0},
  {"back-scale", 10, 1, 1, ITEM_TAKE | ITEM_WEAR_BACK, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"fieldplate-suit", 10, 1, 1, ITEM_TAKE | ITEM_WEAR_BODY, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"plated-boots", 10, 1, 8, ITEM_TAKE | ITEM_WEAR_FEET, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"plated-gauntlets", 10, 1, 2, ITEM_TAKE | ITEM_WEAR_HANDS, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"mace", 10, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 2, 0, 4, 5, 0, 0},
  {"round-shield", 10, 1, 8, ITEM_TAKE | ITEM_WEAR_SHIELD, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"scale-mask", 10, 1, 8, ITEM_TAKE | ITEM_WEAR_FACE, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
// 24
  {"tanto-blade", 15, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 2, 0, 2, 2, 0, 0},
  {"scale-girdle", 15, 1, 1, ITEM_TAKE | ITEM_WEAR_WAIST, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"fullplate-suit", 15, 1, 1, ITEM_TAKE | ITEM_WEAR_BODY, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"ornate-ring", 15, 1, 8, ITEM_TAKE | ITEM_WEAR_FINGER, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"ear-stud", 15, 1, 2, ITEM_TAKE | ITEM_WEAR_EARS, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"shuriken", 15, 1, 8, ITEM_TAKE | ITEM_HOLD, ITEM_PROJECTILE, 0, 2, 0, 1, 1, 0, 0},
  {"fullplate-visor", 15, 1, 8, ITEM_TAKE | ITEM_WEAR_EYES, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"fullplate-helmet", 15, 1, 8, ITEM_TAKE | ITEM_WEAR_HEAD, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
// 32
  {"wakizashi-sword", 20, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 2, 0, 1, 1, 0, 0},
  {"tower-shield", 20, 1, 1, ITEM_TAKE | ITEM_WEAR_SHIELD, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"fullplate-mask", 20, 1, 1, ITEM_TAKE | ITEM_WEAR_FACE, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"fullplate-armlets", 20, 1, 8, ITEM_TAKE | ITEM_WEAR_ARMS, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"plate-anklets", 20, 1, 2, ITEM_TAKE | ITEM_WEAR_ANKLE, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"katana-sword", 20, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 2, 0, 1, 1, 0, 0},
  {"plate-girdle", 20, 1, 8, ITEM_TAKE | ITEM_WEAR_WAIST, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"back-plate", 20, 1, 8, ITEM_TAKE | ITEM_WEAR_BACK, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},

  {NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const struct draconic_table craft_one[] = {
  {"talons", "a pair of talons", 1, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 2, 0, 1, 14, 0, 0},
  {"saddle-bags", "a set of saddle-bags", 1, 1, 1, ITEM_TAKE | ITEM_WEAR_BACK, ITEM_CONTAINER, 500, 3, 0, 0, 0, 0, 0},
  {"barding", "a set of barding", 1, 1, 1, ITEM_TAKE | ITEM_WEAR_ABOUT, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"wing-tip-left", "a left wing-tip", 1, 1, 8, ITEM_TAKE | ITEM_WEAR_L_WING, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"wing-tip-right", "a right wing-tip", 1, 1, 2, ITEM_TAKE | ITEM_WEAR_R_WING, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"wing-mesh-left", "a left wing-mesh", 5, 1, 8, ITEM_TAKE | ITEM_WEAR_L_WING, ITEM_ARMOR, 0, 2, 0, 1, 1, 0, 0},
  {"wing-mesh-right", "a right wing-mesh", 5, 1, 8, ITEM_TAKE | ITEM_WEAR_R_WING, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"treasure-pile", "a pile of treasure", 5, 1, 8, ITEM_TAKE, ITEM_TREASURE, 0, 0, 3, 0, 0, 0, 0},
  {"belt", "a rider-free belt", 5, 1, 8, ITEM_TAKE | ITEM_WEAR_WAIST, ITEM_ARMOR, 0, 2, 0, 1, 1, 0, 0},
  {"draco-mask", "a draco-mask", 10, 1, 1, ITEM_TAKE | ITEM_WEAR_FACE, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"plated-gauntlets", "a set of plated-gauntlets", 10, 1, 1, ITEM_TAKE | ITEM_WEAR_HANDS, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"crystal-light", "a crystal-light", 10, 1, 8, ITEM_TAKE, ITEM_LIGHT, 0, 0, -1, 0, 0, 0, 0},
  {"plated-backplate", "a plated-backplate", 10, 1, 2, ITEM_TAKE | ITEM_WEAR_BACK, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"draco-anklet", "a draco-anklet", 10, 1, 8, ITEM_TAKE | ITEM_WEAR_ANKLE, ITEM_ARMOR, 0, 2, 0, 1, 1, 0, 0},
  {"draco-bracer", "a draco-bracer", 15, 1, 8, ITEM_TAKE | ITEM_WEAR_ARMS, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"draco-visor", "a draco-visor", 15, 1, 8, ITEM_TAKE | ITEM_WEAR_EYES, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"draco-helmet", "a draco-helmet", 15, 1, 8, ITEM_TAKE | ITEM_WEAR_HEAD, ITEM_ARMOR, 0, 2, 0, 1, 1, 0, 0},
  {"treasure-mound", "a mound of treasure", 15, 1, 1, ITEM_TAKE, ITEM_TREASURE, 0, 0, 4, 0, 0, 0, 0},
  {"pocket-dimension", "a pocket-dimension", 20, 1, 1, ITEM_TAKE | ITEM_WEAR_WAIST, ITEM_CONTAINER, 4000, 3, 0, 0, 0, 0, 0},
  {"full-breastplate", "a full-breastplate", 20, 1, 8, ITEM_TAKE | ITEM_WEAR_BODY, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

};

const struct tanner_table tanned_one[] = {
  {"fur-torque", "a fur torque", 0, 1, 3, ITEM_TAKE | ITEM_WEAR_NECK, ITEM_ARMOR, 0, 1, 0, 0, 0,
   0, 0},
  {"fur-scarf", "a fur scarf", 0, 1, 2, ITEM_TAKE | ITEM_WEAR_FACE, ITEM_ARMOR, 0, 1, 0, 0, 0, 0,
   0},
  {"fur-girdle", "a fur girdle", 0, 1, 2, ITEM_TAKE | ITEM_WEAR_WAIST, ITEM_ARMOR, 0, 1, 0, 0, 0,
   0, 0},
  {"fur-tunic", "a fur tunic", 0, 1, 8, ITEM_TAKE | ITEM_WEAR_BODY, ITEM_ARMOR, 0, 2, 0, 0, 0, 0,
   0},
  {"fur-wrist", "a fur wrist band", 0, 1, 3, ITEM_TAKE | ITEM_WEAR_WRIST, ITEM_ARMOR, 0, 2, 0, 0,
   0, 0,
   0},
  {"fur-cap", "a fur cap", 0, 1, 3, ITEM_TAKE | ITEM_WEAR_HEAD, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"fur-boots", "a pair of fur boots", 0, 1, 2, ITEM_TAKE | ITEM_WEAR_FEET, ITEM_ARMOR, 0, 3, 0,
   0, 0,
   0, 0},
  {"fur-gloves", "a pair of fur gloves", 0, 1, 2, ITEM_TAKE | ITEM_WEAR_HANDS, ITEM_ARMOR, 0, 3,
   0, 0,
   0, 0, 0},
  // 8
  {"finger-band", "a finger band", 5, 1, 1, ITEM_TAKE | ITEM_WEAR_FINGER, ITEM_ARMOR, 0, 3, 0, 0,
   0, 0, 0},
  {"ear-band", "a ear band", 5, 1, 1, ITEM_TAKE | ITEM_WEAR_EARS, ITEM_ARMOR, 0, 3, 0, 0, 0, 0,
   0},
  {"leather-armlets", "a pair of leather armlets", 5, 1, 8, ITEM_TAKE | ITEM_WEAR_ARMS,
   ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"leather-visor", "a leather visor", 5, 1, 2, ITEM_TAKE | ITEM_WEAR_EYES, ITEM_ARMOR, 0, 3, 0,
   0, 0, 0, 0},
  {"fur-leggings", "a pair of leather leggings", 5, 1, 8, ITEM_TAKE | ITEM_WEAR_LEGS, ITEM_ARMOR,
   0, 3, 0, 0, 0, 0, 0},
  {"leather-anklet", "a leather anklet", 5, 1, 8, ITEM_TAKE | ITEM_WEAR_ANKLE, ITEM_ARMOR, 0, 3,
   0, 0, 0, 0, 0},
  {"sheath", "a sheath", 5, 1, 2, ITEM_TAKE | ITEM_WEAR_SHEATH, ITEM_SHEATH, 0, 3, 0, 0, 0, 0, 0},
  {"leather-cloak", "a leather cloak", 5, 1, 2, ITEM_TAKE | ITEM_WEAR_ABOUT, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  // 16
  {"back-leather", "a leather back plate", 10, 1, 1, ITEM_TAKE | ITEM_WEAR_BACK, ITEM_ARMOR, 0, 3,
   0, 0, 0, 0, 0},
  {"leather-suit", "a leather suit of armor", 10, 1, 1, ITEM_TAKE | ITEM_WEAR_BODY, ITEM_ARMOR, 0,
   3, 0, 0, 0, 0, 0},
  {"leather-boots", "a pair of leather boots", 10, 1, 8, ITEM_TAKE | ITEM_WEAR_FEET, ITEM_ARMOR,
   0, 3, 0, 0, 0, 0, 0},
  {"leather-gauntlets", "a pair of leather gauntlets", 10, 1, 2, ITEM_TAKE | ITEM_WEAR_HANDS,
   ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"leather-mask", "a leather mask", 10, 1, 8, ITEM_TAKE | ITEM_WEAR_FACE, ITEM_ARMOR, 0, 3, 0, 0,
   0, 0, 0},
  {"leather-backpack", "a leather backpack", 10, 1, 2, ITEM_TAKE | ITEM_WEAR_BACK, ITEM_CONTAINER,
   500, 3, 0, 12, 0, 0, 0},
  {"leather-leggings", "a pair of leather leggings", 10, 1, 8, ITEM_TAKE | ITEM_WEAR_LEGS,
   ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"leather-girdle", "a leather girdle", 10, 1, 8, ITEM_TAKE | ITEM_WEAR_WAIST, ITEM_ARMOR, 0, 3,
   0, 0, 0, 0, 0},
// 24
  {"leather-bracer", "a leather bracer", 15, 1, 1, ITEM_TAKE | ITEM_WEAR_WRIST, ITEM_ARMOR, 0, 3,
   0, 0, 0, 0, 0},
  {"leather-cap", "a leather cap", 15, 1, 1, ITEM_TAKE | ITEM_WEAR_HEAD, ITEM_ARMOR, 0, 3, 0, 0,
   0, 0, 0},
  {"studded-armlets", "a pair of studded leather armlets", 15, 1, 8, ITEM_TAKE | ITEM_WEAR_ARMS,
   ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"studded-gloves", "a pair of studded leather gloves", 15, 1, 2, ITEM_TAKE | ITEM_WEAR_HANDS,
   ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"studded-visor", "a studded leather visor", 15, 1, 8, ITEM_TAKE | ITEM_WEAR_EYES, ITEM_ARMOR,
   0, 3, 0, 0, 0, 0, 0},
  {"studded-leggings", "a pair of studded leather leggings", 15, 1, 8, ITEM_TAKE | ITEM_WEAR_LEGS,
   ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"studded-anklet", "a studded leather anklet", 15, 1, 8, ITEM_TAKE | ITEM_WEAR_ANKLE,
   ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"studded-collar", "a studded leather collar", 15, 1, 8, ITEM_TAKE | ITEM_WEAR_NECK, ITEM_ARMOR,
   0, 3, 0, 0, 0, 0, 0},
// 32
  {"hardened-backplate", "a hardened leather backplate", 20, 1, 1, ITEM_TAKE | ITEM_WEAR_BACK, ITEM_ARMOR, 0, 3, 0, 12, 0, 0, 0},
  {"hardened-suit", "a hardened leather suit", 20, 1, 1, ITEM_TAKE | ITEM_WEAR_BODY, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"hardened-boots", "a pair of hardened leather boots", 20, 1, 8, ITEM_TAKE | ITEM_WEAR_FEET, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"hardened-gauntlets", "a pair of hardened leather gauntlets", 20, 1, 2, ITEM_TAKE | ITEM_WEAR_HANDS, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"hardened-mask", "a hardened leather mask", 20, 1, 8, ITEM_TAKE | ITEM_WEAR_FACE, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"hardened-backpack", "a hardened leather backpack", 20, 1, 8, ITEM_TAKE | ITEM_WEAR_BACK, ITEM_CONTAINER, 800, 3, 0, 12, 0, 0, 0},
  {"hardened-leggings", "a pair of hardened leather leggings", 20, 1, 8, ITEM_TAKE | ITEM_WEAR_LEGS, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"hardened-cap", "a hardened leather cap", 20, 1, 8, ITEM_TAKE | ITEM_WEAR_HEAD, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"leather-whip", "a leather whip", 10, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 0, 0, 5, 3, 0, 0},
  {"hardened-anklets", "a pair of hardened leather anklets", 15, 1, 8, ITEM_TAKE | ITEM_WEAR_ANKLE, ITEM_ARMOR, 0, 3, 0, 0, 0, 0, 0},
  {"hand-claw", "a pair of shuko hand claws", 20, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 0, 0, 1, 14, 0, 0},
  {"drums-set", "a set of drums", 15, 1, 1, ITEM_TAKE | ITEM_HOLD, ITEM_INSTRUMENT, 0, 0, 0, 0, 0, 0, 0},
  {"quiver", "a quiver", 15, 1, 1, ITEM_TAKE | ITEM_WEAR_BACK, ITEM_QUIVER, 50, 0, 0, 0, 0, 0, 0},
// 40
  {NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const struct hunter_table hunted_one[] = {
  {"rabbit", "a small rabbit", "A small rabbit is here.", 0, 83, 1},  /* 1 */
  {"pheasant", "a small pheasant", "A small pheasant is here.", 0, 83, 1},  /* 2 */
  {"dove", "a small dove", "A small dove is here.", 0, 83, 1},  /* 3 */
  {"goose", "a small goose", "A small goose is here.", 0, 83, 1}, /* 4 */
  {"duck", "a small duck", "A small duck is here.", 0, 83, 1},  /* 5 */
  {"squirrel", "a small squirrel", "A small squirrel is here.", 0, 83, 1},  /* 6 */
  {"badger", "a small badger", "A small badger is here.", 0, 83, 1},  /* 7 */
  {"raccoon", "a small raccoon", "A small raccoon is here.", 0, 83, 1}, /* 8 */
  {"deer", "a white tailed deer", "A white tailed deer is here.", 5, 83, 1},  /* 1 */
  {"turkey", "a turkey", "A turkey is here.", 5, 83, 1},  /* 2 */
  {"boar", "a wild boar", "A wild boar is here.", 5, 83, 1},  /* 3 */
  {"antelope", "a antelope", "A antelope is here.", 5, 83, 1},  /* 4 */
  {"coyote", "a coyote", "A coyote is here.", 5, 83, 1},  /* 5 */
  {"bobcat", "a bobcat", "A bobcat is here.", 5, 83, 1},  /* 6 */
  {"wolf", "a wolf", "A wolf is here.", 5, 83, 1},  /* 7 */
  {"leopard", "a leopard", "A leopard is here.", 5, 83, 1}, /* 8 */
  {"bear", "a bear", "A bear is here.", 15, 83, 1}, /* 1 */
  {"moose", "a moose", "A moose is here.", 15, 83, 1},  /* 2 */
  {"lion", "a lion", "A lion is here.", 15, 83, 1}, /* 3 */
  {"bugbear", "a bugbear", "A bugbear is here.", 15, 83, 1},  /* 4 */
  {NULL, NULL, NULL, 0, 0, 0}
};

struct carpenter_table
{
  const char             *name;
  int                     level;
  int                     cost;
  int                     weight;
  int                     wear_flags;
  int                     item_type;
  int                     base_v0;
  int                     base_v1;
  int                     base_v2;
  int                     base_v3;
  int                     base_v4;
  int                     base_v5;
  int                     base_v6;
};

struct lumber_type
{
  const char             *name;
  int                     cost_stone;
  int                     armor_mod;
  int                     workability;
  const char             *color;
};

const struct lumber_type lumber_table[] = {
  {"fir", 10, 10, 25, "light-brown"}, /* 1 */
  {"cedar", 120, 8, 18, "pinkish-brown"}, /* 4 */
  {"pine", 225, 5, 16, "reddish-brown"},  /* 5 */
  {"walnut", 25, 15, 22, "dark-brown"}, /* 2 */
  {"oak", 50, 25, 20, "slightly-brown"},  /* 3 */
  {"hickory", 300, 30, 25, "brown"},  /* 6 */
  {NULL, 0}
};

/* 
arrows, bows, catapults, ballista, staves, wands, club, crates, wagon, buckler shield, kite shield, tower shield, chests, crossbow, chair, couch, bed, table, torch, shovel, pick axe, 1h axe, 2h axe, row boat, galley ship, lance, spear, mace. flail. 

Note: catapults, bows, arrows, ballista produced by carpenter has mlumber durability in sieges. 
pick axe less likely to break, furniture having 1/2 dragon healing bonuses for resting or 
sleeping on 
*/

const struct carpenter_table lumber_one[] = {
  {"arrow", 1, 10, 2, ITEM_TAKE | ITEM_HOLD, ITEM_PROJECTILE, 12, 0, 0, 1, 0, 0, 0},  // 1 
                                                                                      // 
  // 
  {"bolt", 1, 10, 2, ITEM_TAKE | ITEM_HOLD, ITEM_PROJECTILE, 12, 0, 0, 0, 0, 0, 0},
  {"staff", 1, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 12, 2, 0, 4, 11, 0, 0},
  {"bow", 5, 1, 8, ITEM_TAKE | ITEM_MISSILE_WIELD, ITEM_MISSILE_WEAPON, 12, 2, 0, 4, 6, 0, 0},
  {"crossbow", 10, 1, 8, ITEM_TAKE | ITEM_MISSILE_WIELD, ITEM_MISSILE_WEAPON, 12, 2, 0, 4, 6, 0, 0},  // 5 
                                                                                                      // 
  // 
  {"hardened-defense", 15, 1, 8, ITEM_TAKE, ITEM_SABOTAGE, 0, 3, 0, 0, 0, 0, 0},
  {"club", 5, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 12, 2, 0, 4, 5, 0, 0},
  {"crate", 5, 1, 8, ITEM_TAKE, ITEM_CONTAINER, 12, 0, 0, 0, 0, 0, 0},  // 10 
  {"wagon", 10, 1, 8, ITEM_TAKE, ITEM_CONTAINER, 12, 0, 0, 0, 0, 0, 0}, // code to follow 
                                                                        // 
  // player around
  // as they move 
  {"buckler-shield", 1, 1, 8, ITEM_TAKE | ITEM_WEAR_SHIELD, ITEM_ARMOR, 0, 2, 0, 4, 5, 0, 0},
  {"kite-shield", 5, 1, 8, ITEM_TAKE | ITEM_WEAR_SHIELD, ITEM_ARMOR, 0, 2, 0, 4, 5, 0, 0},
  {"tower-shield", 10, 1, 8, ITEM_TAKE | ITEM_WEAR_SHIELD, ITEM_ARMOR, 0, 2, 0, 4, 5, 0, 0},
  {"chest", 10, 1, 8, ITEM_TAKE | ITEM_HOLD, ITEM_CONTAINER, 0, 0, 0, 0, 0, 0, 0},  // 15 
                                                                                    // 
  // 
  {"chair", 15, 1, 8, ITEM_TAKE, ITEM_FURNITURE, 4, 5000, 10, 150, 5, 0, 0},
  {"couch", 15, 1, 8, ITEM_TAKE, ITEM_FURNITURE, 4, 5000, 129, 150, 5, 0, 0},
  {"bed", 15, 1, 8, ITEM_TAKE, ITEM_FURNITURE, 4, 5000, 129, 150, 5, 0, 0},
  {"table", 15, 1, 8, ITEM_TAKE, ITEM_FURNITURE, 4, 5000, 10, 150, 5, 0, 0},
  {"torch", 1, 1, 8, ITEM_TAKE | ITEM_HOLD, ITEM_LIGHT, 0, 2, 0, 4, 5, 0, 0}, // 20 
  {"shovel", 1, 1, 8, ITEM_TAKE | ITEM_HOLD, ITEM_SHOVEL, 0, 2, 0, 4, 5, 0, 0},
  {"pick-axe", 5, 1, 8, ITEM_TAKE | ITEM_HOLD, ITEM_TOOL, 0, 2, 0, 4, 5, 0, 0},
  {"axe", 1, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 2, 0, 4, 9, 0, 0},
  {"battle-axe", 5, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 2, 0, 4, 8, 0, 0},
  {"row-boat", 5, 1, 8, ITEM_TAKE, ITEM_BOAT, 0, 2, 0, 4, 5, 0, 0}, // 25 
  {"ship-mast", 10, 1, 8, ITEM_TAKE, ITEM_PIECE, 0, 41029, 41030, 4, 5, 0, 0},
  {"ship-hull", 10, 1, 8, ITEM_TAKE, ITEM_PIECE, 41028, 2, 41030, 4, 5, 0, 0},
  {"ship-aft", 10, 1, 8, ITEM_TAKE, ITEM_PIECE, 0, 41032, 41034, 4, 5, 0, 0},
  {"ship-stern", 10, 1, 8, ITEM_TAKE, ITEM_PIECE, 41031, 2, 41034, 4, 5, 0, 0},
  {"lance", 15, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 2, 0, 4, 12, 0, 0}, // 30 
  {"spear", 10, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 2, 0, 4, 10, 0, 0},
  {"mace", 5, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 2, 0, 4, 5, 0, 0},
  {"flail", 5, 1, 8, ITEM_TAKE | ITEM_WIELD, ITEM_WEAPON, 0, 2, 0, 4, 13, 0, 0},  // 33 
  {"violin", 15, 1, 1, ITEM_TAKE | ITEM_HOLD, ITEM_INSTRUMENT, 0, 0, 0, 0, 0, 0, 0},
  {NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
