/*****************************************************************************
 * DikuMUD (C) 1990, 1991 by:                                                *
 *   Sebastian Hammer, Michael Seifert, Hans Henrik Staefeldt, Tom Madsen,   *
 *   and Katja Nyboe.                                                        *
 *---------------------------------------------------------------------------*
 * MERC 2.1 (C) 1992, 1993 by:                                               *
 *   Michael Chastain, Michael Quan, and Mitchell Tse.                       *
 *---------------------------------------------------------------------------*
 * SMAUG 1.4 (C) 1994, 1995, 1996, 1998 by: Derek Snider.                    *
 *   Team: Thoric, Altrag, Blodkai, Narn, Haus, Scryn, Rennard, Swordbearer, *
 *         gorog, Grishnakh, Nivek, Tricops, and Fireblade.                  *
 *---------------------------------------------------------------------------*
 * SMAUG 1.7 FUSS by: Samson and others of the SMAUG community.              *
 *                    Their contributions are greatly appreciated.           *
 *---------------------------------------------------------------------------*
 * LoP (C) 2006 - 2013 by: the LoP team.                                     *
 *****************************************************************************
 * Advanced string hashing functions (c)1996 D.S.D. Software, written by     *
 * Derek Snider for use in SMAUG.                                            *
 *                                                                           *
 * These functions keep track of how many "links" are pointing to the	     *
 * memory allocated, and will free the memory if all the links are removed.  *
 * Make absolutely sure you do not mix use of strdup and free with these     *
 * functions, or nasty stuff will happen!                                    *
 * Most occurances of strdup/str_dup should be replaced with str_alloc, and  *
 * any free/DISPOSE used on the same pointer should be replaced with	     *
 * str_free.  If a function uses strdup for temporary use... it is best if   *
 * it is left as is.  Just don't get usage mixed up between conventions.     *
 * The hashstr_data size is 8 bytes of overhead.  Don't be concerned about   *
 * this as you still save lots of space on duplicate strings. -Thoric        *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "h/mud.h"

#define STR_HASH_SIZE 1024


struct __attribute__((packed)) hashstr_data
/* struct hashstr_data */
{
  struct hashstr_data    *next; /* next hash element */
  unsigned short int      links;  /* number of links to this string */
  unsigned short int      length; /* length of string */
};

struct hashstr_data    *string_hash[STR_HASH_SIZE];

/*
 * Check hash table for existing occurance of string.
 * If found, increase link count, and return pointer,
 * otherwise add new string to hash table, and return pointer.
 */
char                   *str_alloc(const char *str, const char *filename, int line)
{
  register int            len, hash, psize;
  register struct hashstr_data *ptr;

  if(!str || str[0] == '\0')
  {
    //  bug( "%s: %s@%d trying to allocate an empty/null string", __FUNCTION__, filename, line );
    return NULL;
  }
  len = strlen(str);
  psize = sizeof(struct hashstr_data);
  hash = len % STR_HASH_SIZE;
  for(ptr = string_hash[hash]; ptr; ptr = ptr->next)
  {
    if(len == ptr->length && !strcmp(str, (char *)ptr + psize))
    {
      if((ptr->links + 1) > 65500)
        continue;
      else
        ++ptr->links;
      return (char *)ptr + psize;
    }
  }
  ptr = (struct hashstr_data *)malloc(len + psize + 1);
  ptr->links = 1;
  ptr->length = len;
  if(len)
    strcpy((char *)ptr + psize, str);
  else
    strcpy((char *)ptr + psize, "");
  ptr->next = string_hash[hash];
  string_hash[hash] = ptr;
  return (char *)ptr + psize;
}

/*
 * Used to make a quick copy of a string pointer that is known to be already
 * in the hash table.  Function increments the link count and returns the
 * same pointer passed.
 */
char                   *quick_link(const char *str, const char *filename, int line)
{
  register struct hashstr_data *ptr;

  if(!str || str[0] == '\0')
  {
   //   bug( "%s: %s@%d trying to allocate an empty/null string", __FUNCTION__, filename, line );
    return NULL;
  }
  ptr = (struct hashstr_data *)(str - sizeof(struct hashstr_data));
  if(!ptr || ptr->links == 0)
  {
    fprintf(stderr, "%s: %s@%d called bad pointer\n", __FUNCTION__, filename, line);
    return NULL;
  }
  if((ptr->links + 1) > 65500)
    return str_alloc(str, filename, line);
  else
    ++ptr->links;
  return (char *)str;
}

/*
 * Used to remove a link to a string in the hash table.
 * If all existing links are removed, the string is removed from the
 * hash table and disposed of.
 * returns how many links are left, or -1 if an error occurred.
 */
int str_free(const char *str, const char *filename, int line)
{
  register int            len, hash;
  register struct hashstr_data *ptr, *ptr2, *ptr2_next;

  if(!str || str[0] == '\0')
  {
   //   bug( "%s: %s@%d trying to free an empty/null string", __FUNCTION__, filename, line );
    return -1;
  }
  len = strlen(str);
  hash = len % STR_HASH_SIZE;
  ptr = (struct hashstr_data *)(str - sizeof(struct hashstr_data));
  if(!ptr || ptr->links == 0)
  { 
    fprintf(stderr, "%s: %s@%d calling bad pointer\n", __FUNCTION__, filename, line);
    return -1;
  }
  if(--ptr->links == 0)
  {
    if(string_hash[hash] == ptr)
    {
      string_hash[hash] = ptr->next;
      free(ptr);
      return 0;
    }
    for(ptr2 = string_hash[hash]; ptr2; ptr2 = ptr2_next)
    {
      ptr2_next = ptr2->next;
      if(ptr2_next == ptr)
      {
        ptr2->next = ptr->next;
        free(ptr);
        return 0;
      }
    }
    fprintf(stderr, "%s: %s@%d calling pointer thats not found for string: %s\n", __FUNCTION__, filename, line, str);
    return -1;
  }
  return ptr->links;
}

bool hash_dump(int hash)
{
  struct hashstr_data    *ptr;
  char                   *str;
  int                     c, psize;

  if(hash > STR_HASH_SIZE || hash < 0)
  {
    fprintf(stderr, "%s: invalid hash size\n", __FUNCTION__);
    return true;
  }
  psize = sizeof(struct hashstr_data);
  for(c = 0, ptr = string_hash[hash]; ptr; ptr = ptr->next, c++)
  {
    str = (char *)(((long)ptr) + psize);
    fprintf(stderr, "Len: %4d Lnks: %5d Str: %s\r\n", ptr->length, ptr->links, strip_cr(str));
  }

  if(!mud_down || (mud_down && c > 0))
    fprintf(stderr, "Total strings in hash %d: %d\r\n", hash, c);

  if(c > 0)
    return true;
  else
    return false;
}

char                   *check_hash(const char *str)
{
  static char             buf[1024];
  int                     len, hash, psize, p = 0, c;
  struct hashstr_data    *ptr, *fnd;

  buf[0] = '\0';
  len = strlen(str);
  psize = sizeof(struct hashstr_data);
  hash = len % STR_HASH_SIZE;
  for(fnd = NULL, ptr = string_hash[hash], c = 0; ptr; ptr = ptr->next, c++)
    if(len == ptr->length && !strcmp(str, (char *)ptr + psize))
    {
      fnd = ptr;
      p = c + 1;
    }
  if(fnd)
    snprintf(buf, sizeof(buf), "Hash info on string: %s\r\nLinks: %d  Position: %d/%d  Hash: %d  Length: %d\r\n", str, fnd->links, p, c, hash, fnd->length);
  else
    snprintf(buf, sizeof(buf), "%s not found.\r\n", str);
  return buf;
}

char                   *hash_stats(void)
{
  static char             buf[1024];
  struct hashstr_data    *ptr;
  int                     x, c, total = 0, totlinks = 0, unique = 0, bytesused = 0, wouldhave = 0, hilink = 0, saved = 0;
  int                     hashsize = sizeof(struct hashstr_data);

  for(x = 0; x < STR_HASH_SIZE; x++)
  {
    for(c = 0, ptr = string_hash[x]; ptr; ptr = ptr->next, c++)
    {
      total++;
      if(ptr->links == 1)
        unique++;
      if(ptr->links > hilink)
        hilink = ptr->links;
      totlinks += ptr->links;
      bytesused += (ptr->length + 1 + hashsize);
      /*
       * Wouldhave is done in what str_dup would use instead of the hash if bytes saved shows -
       * your wasteing memory because of to many unique links
       */
      wouldhave += (ptr->links * (ptr->length + 1));
    }
  }
  saved = (wouldhave - bytesused);
  snprintf(buf, sizeof(buf),
           "Total Strings: %8d  Bytes Used  : %8d\r\n"
           "Total Links  : %8d  Bytes %s: %8d\r\n" "Unique Links : %8d\r\n" "Hi Link Count: %8d\r\n", total, bytesused, totlinks, saved >= 0 ? "Saved " : "Wasted", abs(saved), unique, hilink);
  return buf;
}

void show_high_hash(int top)
{
  struct hashstr_data    *ptr;
  int                     x, psize;
  char                   *str;

  psize = sizeof(struct hashstr_data);
  for(x = 0; x < STR_HASH_SIZE; x++)
  {
    for(ptr = string_hash[x]; ptr; ptr = ptr->next)
    {
      if(ptr->links >= top)
      {
        str = (char *)(((long)ptr) + psize);
        fprintf(stderr, "Links: %5d  String: >%s<\r\n", ptr->links, strip_cr(str));
      }
    }
  }
}

bool in_hash_table(const char *str)
{
  register int            len, hash, psize;
  register struct hashstr_data *ptr;

  len = strlen(str);
  psize = sizeof(struct hashstr_data);
  hash = len % STR_HASH_SIZE;
  for(ptr = string_hash[hash]; ptr; ptr = ptr->next)
    if(len == ptr->length && str == ((char *)ptr + psize))
      return true;
  return false;
}


#ifdef MEMWATCH

void do_memwatch(CHAR_DATA *ch, char *argument)
{
  char                    arg[MSL], arg2[MSL];
  short                   value = 0;

  set_pager_color(AT_CYAN, ch);

  if(argument[0] == '\0')
  {  
    pager_printf(ch, "Syntax: memwatch < option > <#>\r\n" );
    pager_printf(ch, "Options being: grab, drop, nomansland, statistics, freebuffer,\r\n");
    pager_printf(ch, "               autocheck, calccheck, dumpcheck, mark, unmark\r\n" );
  }

  argument = one_argument(argument, arg);
  argument = one_argument(argument, arg2);
  smash_tilde(argument);

    if(!is_number(arg2))
    {
      send_to_char("# must be numeric.\r\n", ch);
      return;
    }

    value = atoi(arg2);

    if(value < 0 || value > 1000 )
    {
      send_to_char("Number must be betwen 0 and 1000.\r\n", ch);
      return;
    }

  if(!str_cmp(arg, "grab"))
  {  
    if ( arg2[0] == '\0' )
    {
    send_to_char("memwatch grab how many kilobytes?\r\n", ch );
    return;
    }
    mwGrab(value);
    ch_printf(ch, "You just grabbed %d kilobytes of memory for stress testing.\r\n", value);
    return;
  }  
  if(!str_cmp(arg, "drop"))
  {
    if ( arg2[0] == '\0' )
    {
    send_to_char("memwatch drop how many kilobytes?\r\n", ch );
    return;
    }
    mwDrop(value);
    ch_printf(ch, "You just dropped %d kilobytes of memory from stress testing.\r\n", value);
    return;

  }



}

#endif
