#include <string.h>

#include "h/mud.h"
#include <ctype.h>
#include <time.h>
#include "h/files.h"
#include "h/currency.h"

void                    free_bank_to_chars(BANK_DATA * bank);

CHAR_DATA              *find_banker(CHAR_DATA *ch)
{
  CHAR_DATA              *banker = NULL;

  for(banker = ch->in_room->first_person; banker; banker = banker->next_in_room)
    if(IS_NPC(banker) && xIS_SET(banker->act, ACT_BANKER))
      break;

  return banker;
}

/* Quick function for immortals/staff to list all bankers in the world */
void do_bankers(CHAR_DATA *ch, char *argument)
{

  CHAR_DATA              *mob;

  for(mob = first_char; mob; mob = mob->next)
  {
    if(IS_NPC(mob))
    {
      if(xIS_SET(mob->pIndexData->act, ACT_BANKER))
        ch_printf(ch, "&R[%d] Banker '%s', level %d : in room [%d] - %s..\n\r", mob->pIndexData->vnum, mob->short_descr, mob->level, mob->in_room->vnum, mob->in_room->area->name);
      else if(xIS_SET(mob->act, ACT_BANKER))
        ch_printf(ch, "&W[%d] Banker '%s', level %d : in room [%d] - %s..\n\r", mob->pIndexData->vnum, mob->short_descr, mob->level, mob->in_room->vnum, mob->in_room->area->name);
    }
  }

  send_to_char("&wMobs in &Wwhite &ware current, &Rred &ware proto.\n\r", ch);
  return;
}

void unlink_bank(BANK_DATA * bank)
{
  BANK_DATA              *tmp, *tmp_next;
  int                     hash;

  if(!bank)
  {
    bug("Unlink_bank: NULL bank");
    return;
  }

  if(bank->name[0] < 'a' || bank->name[0] > 'z')
    hash = 0;
  else
    hash = (bank->name[0] - 'a') + 1;

  if(bank == (tmp = bank_index[hash]))
  {
    bank_index[hash] = tmp->next;
    return;
  }
  for(; tmp; tmp = tmp_next)
  {
    tmp_next = tmp->next;
    if(bank == tmp_next)
    {
      tmp->next = tmp_next->next;
      return;
    }
  }
}

void free_bank(BANK_DATA * bank)
{
  if(bank->name)
    STRFREE(bank->name);
  if(bank->password)
    STRFREE(bank->password);
  DISPOSE(bank);
}


void free_banks(void)
{
  BANK_DATA              *bank, *bank_next;
  int                     hash;

  for(hash = 0; hash < 27; hash++)
  {
    for(bank = bank_index[hash]; bank; bank = bank_next)
    {
      if ( bank_next )
      {
      bank_next = bank->next;  // clean up memory crashes here
      unlink_bank(bank);
      free_bank(bank);
      }
    }
  }
}

void add_bank(BANK_DATA * bank)
{
  int                     hash, x;
  BANK_DATA              *tmp, *prev;

  if(!bank)
  {
    bug("Add_bank: NULL bank");
    return;
  }

  if(!bank->name)
  {
    bug("Add_bank: NULL bank->name");
    return;
  }

  if(!bank->password)
  {
    bug("Add_bank: NULL bank->password");
    return;
  }

  for(x = 0; bank->name[x] != '\0'; x++)
    bank->name[x] = LOWER(bank->name[x]);
  if(bank->name[0] < 'a' || bank->name[0] > 'z')
    hash = 0;
  else
    hash = (bank->name[0] - 'a') + 1;

  if((prev = tmp = bank_index[hash]) == NULL)
  {
    bank->next = bank_index[hash];
    bank_index[hash] = bank;
    return;
  }

  for(; tmp; tmp = tmp->next)
  {
    if((x = strcmp(bank->name, tmp->name)) == 0)
    {
      bug("Add_bank: trying to add duplicate name to bucket %d", hash);
      free_bank(bank);
      return;
    }
    else if(x < 0)
    {
      if(tmp == bank_index[hash])
      {
        bank->next = bank_index[hash];
        bank_index[hash] = bank;
        return;
      }
      prev->next = bank;
      bank->next = tmp;
      return;
    }
    prev = tmp;
  }
  /*
   * add to end
   */
  prev->next = bank;
  bank->next = NULL;
  return;
}

// Volk Jan-16: Quick command to merge two accounts
void do_merge(CHAR_DATA *ch, char *argument)
{
  BANK_DATA              *bank, *bank2;
  CHAR_DATA              *banker;
  char                    arg1[MIL], arg2[MIL], arg3[MIL], arg4[MIL];
  char                    buf[MSL];

  if(!(banker = find_banker(ch)))
  {
    send_to_char("You're not in a bank!\r\n", ch);
    return;
  }

  if(IS_NPC(ch))
  {
    snprintf(buf, MSL, "say Sorry, %s, we don't do business with mobs.", ch->name);
    interpret(banker, buf);
    return;
  }

  if(argument[0] == '\0')
  {
    send_to_char("Syntax: merge (name) (password) (name2) (password2)\r\n", ch);
    send_to_char("        using the name and password of the first account, then the second.\r\n", ch);
    send_to_char("The first account will be kept, with the funds from both accounts. The second will be deleted.\r\n", ch);
    interpret(banker, (char *)"say If you need help type &WHELP BANK&D.");
    return;
  }

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  argument = one_argument(argument, arg3);
  argument = one_argument(argument, arg4);

  bank = find_bank(arg1);
  bank2 = find_bank(arg3);

  if(!bank)
  {
    ch_printf(ch, "There is no account by the name of '%s'.\r\n", arg1);
    return;
  }

  if(!bank2)
  {
    ch_printf(ch, "There is no account by the name of '%s'.\r\n", arg3);
    return;
  }

  if(strcmp(sha256_crypt(arg2), bank->password))
  {
    ch_printf(ch, "Invalid password for account '%s'.\r\n", arg1);
    return;
  }

  if(strcmp(sha256_crypt(arg4), bank2->password))
  {
    ch_printf(ch, "Invalid password for account '%s'.\r\n", arg3);
    return;
  }

  ch_printf(ch, "Moving %d gold to account '%s' (previously had %d gold).\r\n", bank2->gold, arg1, bank->gold);
  bank->gold += bank2->gold;
  bank2->gold = 0;
  ch_printf(ch, "Moving %d silver to account '%s' (previously had %d silver).\r\n", bank2->silver, arg1, bank->silver);
  bank->silver += bank2->silver;
  bank2->silver = 0;
  ch_printf(ch, "Moving %d bronze to account '%s' (previously had %d bronze).\r\n", bank2->bronze, arg1, bank->bronze);
  bank->bronze += bank2->bronze;
  bank2->bronze = 0;
  ch_printf(ch, "Moving %d copper to account '%s' (previously had %d copper).\r\n", bank2->copper, arg1, bank->copper);
  bank->copper += bank2->copper;
  bank2->copper = 0;
  bank->lastused = current_time;

  ch_printf(ch, "Deleting... (%s)\r\n", bank2->name);
  free_bank_to_chars(bank2);
  unlink_bank(bank2);
  free_bank(bank2);
  save_bank();

  if(ch->pcdata && ch->pcdata->account)
    STRFREE(ch->pcdata->account);

  ch_printf(ch, "Account '%s' has successfully been deleted.\r\n", arg3);
  send_to_char("Please reselect your active account using the 'ACCOUNT' command.\r\n", ch);

  return;
}

void do_account(CHAR_DATA *ch, char *argument)
{
  BANK_DATA              *bank;
  CHAR_DATA              *banker;
  char                    arg1[MIL], arg2[MIL], arg3[MIL];
  int                     currtime = time(0);
  char                    buf[MSL], *pwdnew;

  if(!(banker = find_banker(ch)))
  {
    send_to_char("You're not in a bank!\r\n", ch);
    return;
  }

  if(IS_NPC(ch))
  {
    snprintf(buf, MSL, "say Sorry, %s, we don't do business with mobs.", ch->name);
    interpret(banker, buf);
    return;
  }

  if(argument[0] == '\0')
  {
    send_to_char("Syntax: account name password create/delete\r\n", ch);
    send_to_char("        account name password - to switch accounts\r\n", ch);
    interpret(banker, (char *)"say If you need help type &WHELP BANK&D.");
    return;
  }

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  argument = one_argument(argument, arg3);

  bank = find_bank(arg1); // account saved to pfile

  if(str_cmp(arg3, "create") && str_cmp(arg3, "delete"))
  {
    // add check here for correct password 
    if(!bank)
    {
      interpret(banker, (char *)"say There is no account with that name. Please use the correct name, or create a new account.");
      send_to_char("&wTo learn more, type &WHELP BANK&w.\r\n", ch);
      return;
    }
    if(strcmp(sha256_crypt(arg2), bank->password))
    {
      send_to_char("Invalid password.\r\n", ch);
      return;
    }

    if(ch->pcdata->account)
    {
      if(str_cmp(ch->pcdata->account, arg1))
      {
        ch_printf(ch, "You switch bank account from '%s' to '%s'.\r\n", ch->pcdata->account, arg1);
        STRFREE(ch->pcdata->account);
        ch->pcdata->account = STRALLOC(arg1);
      }
      else
        ch_printf(ch, "Account '%s' is already selected.\r\n", arg1);
    }
    else
    {
      ch_printf(ch, "Account '%s' has been selected.\r\n", arg1);
      ch->pcdata->account = STRALLOC(arg1);
    }
    return;
  }

  if(!str_cmp(arg3, "create") && (arg2))
  {

    if(strlen(arg1) < 4)
    {
      send_to_char("Account name must be at least 4 characters.\r\n", ch);
      return;
    }

    if(strlen(arg2) < 5)
    {
      send_to_char("Invalid Password.  Must be at least 5 characters in length.\r\n", ch);
      return;
    }
    if(arg2[0] == '!')
    {
      send_to_char("Password cannot begin with the '!' character.\r\n", ch);
      return;
    }

    if(bank)
    {
      send_to_char("There is already an account with that name!\r\n", ch);
      return;
    }

    if((currtime - ch->pcdata->lastaccountcreated) < 3600 && get_trust(ch) < LEVEL_AJ_LT)
    {
      send_to_char("Please wait at least one hour from previous creation time to make a new account.\r\n", ch);
      return;
    }

    pwdnew = sha256_crypt(arg2);
    CREATE(bank, BANK_DATA, 1);
    bank->lastused = current_time;
    bank->name = STRALLOC(arg1);
    bank->password = STRALLOC(pwdnew);
    bank->bronze = 0;
    bank->copper = 0;
    bank->gold = 0;
    bank->silver = 0;
    add_bank(bank);
    save_bank();
    ch->pcdata->lastaccountcreated = currtime;
    if(ch->pcdata->account)
      STRFREE(ch->pcdata->account);
    ch->pcdata->account = STRALLOC(arg1);
    save_char_obj(ch);
    saving_char = NULL;
    send_to_char("Your account has been added.\r\n", ch);
    return;
  }

  if(!str_cmp(arg3, "delete") && (arg2))
  {
    if(!bank)
    {
      send_to_char("There is no account with that name to delete!\r\n", ch);
      return;
    }
    if(strcmp(sha256_crypt(arg2), bank->password))
    {
      send_to_char("Invalid password.\r\n", ch);
      return;
    }

    GET_MONEY(ch, CURR_GOLD) += bank->gold;
    GET_MONEY(ch, CURR_SILVER) += bank->silver;
    GET_MONEY(ch, CURR_BRONZE) += bank->bronze;
    GET_MONEY(ch, CURR_COPPER) += bank->copper;
    ch_printf(ch, "Deleting... (%s)\r\n", bank->name);
    free_bank_to_chars(bank);
    unlink_bank(bank);
    free_bank(bank);
    save_bank();
    ch->pcdata->lastaccountcreated = 0;
    if(ch->pcdata->account)
      STRFREE(ch->pcdata->account);
    save_char_obj(ch);
    saving_char = NULL;
    send_to_char("Your account has successfully been deleted.\r\n", ch);
    return;
  }

}

void do_deposit(CHAR_DATA *ch, char *argument)
{
  BANK_DATA              *bank;
  CHAR_DATA              *banker;
  char                    arg1[MIL];
  char                    arg2[MIL];
  int                     amount = 0, currtime = time(0);
  int                     type = DEFAULT_CURR;
  char                    buf[MSL], *pwdnew;

  if(!(banker = find_banker(ch)))
  {
    send_to_char("You're not in a bank!\r\n", ch);
    return;
  }

  if(IS_NPC(ch))
  {
    snprintf(buf, MSL, "say Sorry, %s, we don't do business with mobs.", ch->name);
    interpret(banker, buf);
    return;
  }

  if(argument[0] == '\0')
  {
    send_to_char("Syntax: deposit amount currency\r\n", ch);
    interpret(banker, (char *)"say if you need help type &WHELP BANK&D.");
    return;
  }

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  bank = find_bank(ch->pcdata->account);  // account saved to pfile

  if(is_number(arg1))
  {
    amount = atoi(arg1);
  }

  if(!bank)
  {
    send_to_char("There is no account by that name, use the account command to create a new account.\r\n", ch);
    return;
  }
  if(!is_number(arg1))
  {
    send_to_char("You can only deposit coins.\r\n", ch);
    return;
  }
  else
  {
    amount = atoi(arg1);
  }
  if(arg2)
    type = get_currency_type(arg2);

  if(type == CURR_NONE)
  {
    send_to_char("You don't have any of that kind of coin.\r\n", ch);
    return;
  }
  if(amount <= 0)
  {

    send_to_char("You can't do that.\r\n", ch);
    return;
  }
  if(amount > GET_MONEY(ch, type))
  {
    send_to_char("You don't have that much.\r\n", ch);
    return;
  }

  if(type != CURR_BRONZE && type != CURR_COPPER && type != CURR_GOLD && type != CURR_SILVER)
  {
    send_to_char("No such currency.\r\n", ch);
    return;
  }

  GET_MONEY(ch, type) -= amount;
  ch_printf(ch, "You put %d %s coins in the bank.\r\n", amount, curr_types[type]);

  if(type == CURR_BRONZE)
  {
    bank->bronze += amount;
    ch_printf(ch, "This brings your account's bronze balance to %d.\r\n", bank->bronze);
  }
  else if(type == CURR_COPPER)
  {
    bank->copper += amount;
    ch_printf(ch, "This brings your account's copper balance to %d.\r\n", bank->copper);
  }
  else if(type == CURR_GOLD)
  {
    bank->gold += amount;
    ch_printf(ch, "This brings your account's gold balance to %d.\r\n", bank->gold);
  }
  else if(type == CURR_SILVER)
  {
    bank->silver += amount;
    ch_printf(ch, "This brings your account's silver balance to %d.\r\n", bank->silver);
  }
  save_char_obj(ch);
  bank->lastused = current_time;
  save_bank();
  return;
}

void do_withdraw(CHAR_DATA *ch, char *argument)
{
  BANK_DATA              *bank;
  CHAR_DATA              *banker;
  char                    arg1[MIL];
  char                    arg2[MIL];
  int                     amount = 0, currtime = time(0);
  int                     type = DEFAULT_CURR;
  char                    buf[MSL], *pwdnew;

  if(!(banker = find_banker(ch)))
  {
    send_to_char("You're not in a bank!\r\n", ch);
    return;
  }

  if(IS_NPC(ch))
  {
    snprintf(buf, MSL, "say Sorry, %s, we don't do business with mobs.", ch->name);
    interpret(banker, buf);
    return;
  }

  if(argument[0] == '\0')
  {
    send_to_char("Syntax: withrdaw amount currency\r\n", ch);
    interpret(banker, (char *)"say if you need help type &WHELP BANK&D.");
    return;
  }

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  bank = find_bank(ch->pcdata->account);  // account saved to pfile

  if(is_number(arg1))
  {
    amount = atoi(arg1);
  }

  if(!bank)
  {
    send_to_char("There is no account by that name, use the account command to create a new account.\r\n", ch);
    return;
  }
  if(!is_number(arg1))
  {
    send_to_char("You can only withdraw coins.\r\n", ch);
    return;
  }
  else
  {
    amount = atoi(arg1);
  }
  if(arg2)
    type = get_currency_type(arg2);
  if(type == CURR_NONE)
  {
    send_to_char("You don't have any of that kind of coin.\r\n", ch);
    return;
  }
  if(amount <= 0)
  {
    send_to_char("You can't do that.\r\n", ch);
    return;
  }
  if(type == CURR_BRONZE)
  {
    if(amount > bank->bronze)
    {
      ch_printf(ch, "You don't have that much %s in the bank.\r\n", curr_types[type]);
      return;
    }
    bank->bronze -= amount;
    ch_printf(ch, "This brings your account bronze balance to %d.\r\n", bank->bronze);
  }
  else if(type == CURR_COPPER)
  {
    if(amount > bank->copper)
    {
      ch_printf(ch, "You don't have that much %s in the bank.\r\n", curr_types[type]);
      return;
    }
    bank->copper -= amount;
    ch_printf(ch, "This brings your account copper balance to %d.\r\n", bank->copper);
  }
  else if(type == CURR_GOLD)
  {
    if(amount > bank->gold)
    {
      ch_printf(ch, "You don't have that much %s in the bank.\r\n", curr_types[type]);
      return;
    }
    bank->gold -= amount;
    ch_printf(ch, "This brings your account gold balance to %d.\r\n", bank->gold);
  }
  else if(type == CURR_SILVER)
  {
    if(amount > bank->silver)
    {
      ch_printf(ch, "You don't have that much %s in the bank.\r\n", curr_types[type]);
      return;
    }
    bank->silver -= amount;
    ch_printf(ch, "This brings your account silver balance to %d.\r\n", bank->silver);
  }
  else
  {
    send_to_char("No such currency.\r\n", ch);
    return;
  }
  GET_MONEY(ch, type) += amount;
  ch_printf(ch, "You took %d %s coins from the bank.\r\n", amount, curr_types[type]);
  bank->lastused = current_time;
  save_bank();
  save_char_obj(ch);
  return;
}

void free_bank_to_chars(BANK_DATA * bank)
{
  CHAR_DATA              *ch;

  if(!bank)
    return;

  for(ch = first_char; ch; ch = ch->next)
  {
    if(!ch || !ch->pcdata || !ch->pcdata->bank)
      continue;
    if(ch->pcdata->bank != bank)
      continue;
    ch_printf(ch, "%s account no longer exist.\r\n", bank->name);
    ch->pcdata->bank = NULL;
  }
}

void do_transfer(CHAR_DATA *ch, char *argument)
{
  BANK_DATA              *bank;
  BANK_DATA              *victim_bank;
  CHAR_DATA              *banker;
  char                    arg1[MIL], arg2[MIL], arg3[MIL];
  int                    amount = 0;
  int                     type = DEFAULT_CURR;
  char                    buf[MSL];
  if(!(banker = find_banker(ch)))
  {
    send_to_char("You're not in a bank!\r\n", ch);
    return;
  }

  if(IS_NPC(ch))
  {
    snprintf(buf, MSL, "say Sorry, %s, we don't do business with mobs.", ch->name);
    interpret(banker, buf);
    return;
  }

  if(argument[0] == '\0')
  {
    send_to_char("Syntax: transfer amount currency destination\r\n", ch);
    interpret(banker, (char *)"say if you need help type &WHELP BANK&D.");
    return;
  }

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  argument = one_argument(argument, arg3);
  bank = find_bank(ch->pcdata->account);  // account saved to pfile

  if(is_number(arg1))
  {
    amount = atoi(arg1);
  }

  if(!bank)
  {
    send_to_char("There is no account by that name, use the account command to create a new account.\r\n", ch);
    return;
  }

  if(!is_number(arg1))
  {
    send_to_char("You can only transfer coins.\r\n", ch);
    return;
  }
  else
  {
    amount = atoi(arg1);
  }
  if(arg1 && arg2)
    type = get_currency_type(arg2);

  if(type == CURR_NONE)
  {
    send_to_char("You don't have any of that kind of coin.\r\n", ch);
    return;
  }
  if(amount <= 0)
  {
    send_to_char("You can't do that.\r\n", ch);
    return;
  }
  victim_bank = find_bank(arg3);
  if(!victim_bank)
  {
    sprintf(buf, "%s There is no account by that name here.", ch->name);
    do_tell(banker, buf);
    return;
  }

  if(type == CURR_BRONZE)
  {
    if(amount > bank->bronze)
    {
      ch_printf(ch, "You don't have that much %s in the bank.\r\n", curr_types[type]);
      return;
    }

    bank->bronze -= amount;
    victim_bank->bronze += amount;
  }
  else if(type == CURR_COPPER)
  {
    if(amount > bank->copper)
    {
      ch_printf(ch, "You don't have that much %s in the bank.\r\n", curr_types[type]);
      return;
    }
    bank->copper -= amount;
    victim_bank->copper += amount;
  }
  else if(type == CURR_GOLD)
  {
    if(amount > bank->gold)
    {
      ch_printf(ch, "You don't have that much %s in the bank.\r\n", curr_types[type]);
      return;
    }
    bank->gold -= amount;
    victim_bank->gold += amount;
  }
  else if(type == CURR_SILVER)
  {
    if(amount > bank->silver)
    {
      ch_printf(ch, "You don't have that much %s in the bank.\r\n", curr_types[type]);
      return;
    }
    bank->silver -= amount;
    victim_bank->silver += amount;
  }
  ch_printf(ch, "You transfer %d %s coins from your account to theirs.\r\n", amount, curr_types[type]);
  bank->lastused = current_time;
  save_bank();
  return;

}
