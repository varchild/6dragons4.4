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
 *---------------------------------------------------------------------------*
 *                            Transfer data module                           *
 *****************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include "h/mud.h"
#include "h/files.h"
#include "h/key.h"

char                   *distime(time_t updated);

#define TRANSFER_FILE SYSTEM_DIR "transfer.dat"
#define TRANSFERAT 1024
typedef struct transfer_data
{
   short byte;
   short kilo;
   short mega;
   short giga;
} TRANSFER_DATA;

TRANSFER_DATA *in_transfer;
TRANSFER_DATA *out_transfer;
TRANSFER_DATA *mccp_out_transfer;
TRANSFER_DATA *mccp_saved_transfer;
TRANSFER_DATA *i3_in_transfer;
TRANSFER_DATA *i3_out_transfer;
TRANSFER_DATA *total_in_transfer;
TRANSFER_DATA *total_out_transfer;
time_t transfer_reset_time;
short tsaveupdate;

void save_transfer( void )
{
   FILE *fp;

   if( !( fp = fopen( TRANSFER_FILE, "w" ) ) )
   {
      bug( "%s: couldn't open %s for writing", __FUNCTION__, TRANSFER_FILE );
      perror( TRANSFER_FILE );
      return;
   }
   fprintf( fp, "In        %d %d %d %d\n", in_transfer->giga, in_transfer->mega, in_transfer->kilo, in_transfer->byte );
   fprintf( fp, "Out       %d %d %d %d\n", out_transfer->giga, out_transfer->mega, out_transfer->kilo, out_transfer->byte );
   fprintf( fp, "MCCPOut   %d %d %d %d\n", mccp_out_transfer->giga, mccp_out_transfer->mega, mccp_out_transfer->kilo,
            mccp_out_transfer->byte );
   fprintf( fp, "MCCPSaved %d %d %d %d\n", mccp_saved_transfer->giga, mccp_saved_transfer->mega, mccp_saved_transfer->kilo,
            mccp_saved_transfer->byte );
   fprintf( fp, "I3In     %d %d %d %d\n", i3_in_transfer->giga, i3_in_transfer->mega, i3_in_transfer->kilo,
            i3_in_transfer->byte );
   fprintf( fp, "I3Out    %d %d %d %d\n", i3_out_transfer->giga, i3_out_transfer->mega, i3_out_transfer->kilo,
            i3_out_transfer->byte );
   fprintf( fp, "TReset    %ld\n", transfer_reset_time );
   fprintf( fp, "End\n" );
   fclose( fp );
   fp = NULL;
}

/* Update the info in transfer input and output */
/* I think its a good idea to know the bandwidth and how much we are saving because of mccp :) */
void update_bytes( TRANSFER_DATA * transfer )
{
   if( !transfer )
      return;

   while( transfer->byte >= TRANSFERAT )
   {
      transfer->byte -= TRANSFERAT;
      transfer->kilo += 1;
   }
   while( transfer->kilo >= TRANSFERAT )
   {
      transfer->kilo -= TRANSFERAT;
      transfer->mega += 1;
   }
   while( transfer->mega >= TRANSFERAT )
   {
      transfer->mega -= TRANSFERAT;
      transfer->giga += 1;
   }
   if( --tsaveupdate <= 0 )
   {
      save_transfer(  );
      tsaveupdate = 50;
   }
}

/*
 * types:
 *    1 = in_transfer
 *    2 = out_transfer
 *    3 = mccp_out_transfer
 *    4 = mccp_saved_transfer
 *    5 = i3_in_transfer
 *    6 = i3_out_transfer
 */
void update_transfer( int type, int size )
{
   /*
    * If size is less then or equal to 0 whats the point 
    */
   if( size <= 0 )
      return;

   /*
    * If not a valid type return 
    */
   if( type < 1 || type > 6 )
      return;

   if( type == 1 )
   {
      if( in_transfer )
      {
         in_transfer->byte += size;
         update_bytes( in_transfer );
      }
   }
   else if( type == 2 )
   {
      if( out_transfer )
      {
         out_transfer->byte += size;
         update_bytes( out_transfer );
      }
   }
   else if( type == 3 )
   {
      if( mccp_out_transfer )
      {
         mccp_out_transfer->byte += size;
         update_bytes( mccp_out_transfer );
      }
   }
   else if( type == 4 )
   {
      if( mccp_saved_transfer )
      {
         mccp_saved_transfer->byte += size;
         update_bytes( mccp_saved_transfer );
      }
   }
   else if( type == 5 )
   {
      if( i3_in_transfer )
      {
         i3_in_transfer->byte += size;
         update_bytes( i3_in_transfer );
      }
   }
   else if( type == 6 )
   {
      if( i3_out_transfer )
      {
         i3_out_transfer->byte += size;
         update_bytes( i3_out_transfer );
      }
   }

   /*
    * Get a nice updated full count all the time 
    */
   if( type == 1 || type == 5 )
   {
      if( total_in_transfer )
      {
         total_in_transfer->giga = ( in_transfer->giga + i3_in_transfer->giga );
         total_in_transfer->mega = ( in_transfer->mega + i3_in_transfer->mega );
         total_in_transfer->kilo = ( in_transfer->kilo + i3_in_transfer->kilo );
         total_in_transfer->byte = ( in_transfer->byte + i3_in_transfer->byte );
         update_bytes( total_in_transfer );
      }
   }
   else if( type == 2 || type == 3 || type == 6 )
   {
      if( total_out_transfer )
      {
         total_out_transfer->giga = ( out_transfer->giga + mccp_out_transfer->giga + i3_out_transfer->giga );
         total_out_transfer->mega = ( out_transfer->mega + mccp_out_transfer->mega + i3_out_transfer->mega );
         total_out_transfer->kilo = ( out_transfer->kilo + mccp_out_transfer->kilo + i3_out_transfer->kilo );
         total_out_transfer->byte = ( out_transfer->byte + mccp_out_transfer->byte + i3_out_transfer->byte );
         update_bytes( total_out_transfer );
      }
   }
}

void clear_bytes( TRANSFER_DATA * transfer )
{
   if( !transfer )
      return;
   transfer->byte = 0;
   transfer->kilo = 0;
   transfer->mega = 0;
   transfer->giga = 0;
}

/* Clear both transfers */
void clear_transfer( void )
{
   if( in_transfer )
      clear_bytes( in_transfer );
   if( out_transfer )
      clear_bytes( out_transfer );
   if( mccp_out_transfer )
      clear_bytes( mccp_out_transfer );
   if( mccp_saved_transfer )
      clear_bytes( mccp_saved_transfer );
   if( i3_in_transfer )
      clear_bytes( i3_in_transfer );
   if( i3_out_transfer )
      clear_bytes( i3_out_transfer );
   if( total_in_transfer )
      clear_bytes( total_in_transfer );
   if( total_out_transfer )
      clear_bytes( total_out_transfer );
}

/* Create the transfer data */
void create_transfer( void )
{
   if( !in_transfer )
      CREATE( in_transfer, TRANSFER_DATA, 1 );
   if( !out_transfer )
      CREATE( out_transfer, TRANSFER_DATA, 1 );
   if( !mccp_out_transfer )
      CREATE( mccp_out_transfer, TRANSFER_DATA, 1 );
   if( !mccp_saved_transfer )
      CREATE( mccp_saved_transfer, TRANSFER_DATA, 1 );
   if( !i3_in_transfer )
      CREATE( i3_in_transfer, TRANSFER_DATA, 1 );
   if( !i3_out_transfer )
      CREATE( i3_out_transfer, TRANSFER_DATA, 1 );
   if( !total_in_transfer )
      CREATE( total_in_transfer, TRANSFER_DATA, 1 );
   if( !total_out_transfer )
      CREATE( total_out_transfer, TRANSFER_DATA, 1 );
   clear_transfer(  );
   tsaveupdate = 50;
   transfer_reset_time = current_time;
}

/* Free the transfer data */
void free_transfer( void )
{
   clear_transfer(  );
   if( in_transfer )
      DISPOSE( in_transfer );
   if( out_transfer )
      DISPOSE( out_transfer );
   if( mccp_out_transfer )
      DISPOSE( mccp_out_transfer );
   if( mccp_saved_transfer )
      DISPOSE( mccp_saved_transfer );
   if( i3_in_transfer )
      DISPOSE( i3_in_transfer );
   if( i3_out_transfer )
      DISPOSE( i3_out_transfer );
   if( total_in_transfer )
      DISPOSE( total_in_transfer );
   if( total_out_transfer )
      DISPOSE( total_out_transfer );
}

void show_transfer( CHAR_DATA * ch, TRANSFER_DATA * transfer )
{
   if( !transfer )
      return;
   if( transfer->giga > 0 )
      ch_printf( ch, "%dGB", transfer->giga );
   if( transfer->mega > 0 )
      ch_printf( ch, "%s%dMB", ( transfer->giga > 0 ) ? " " : "", transfer->mega );
   if( transfer->kilo > 0 )
      ch_printf( ch, "%s%dKB", ( transfer->giga > 0 || transfer->mega > 0 ) ? " " : "", transfer->kilo );
   if( transfer->byte > 0 )
      ch_printf( ch, "%s%dB", ( transfer->giga > 0 || transfer->mega > 0 || transfer->kilo > 0 ) ? " " : "",
                 transfer->byte );
}

void do_check_transfer( CHAR_DATA *ch, char *argument )
{
   /*
    * Display the transfer data 
    */
   if( !argument || argument[0] == '\0' )
   {
      ch_printf( ch, "&wTransfer data since &C%s&D\r\n", distime( transfer_reset_time ) );
      send_to_char( "&wReceived           :  &C", ch );
      show_transfer( ch, in_transfer );
      send_to_char( "\r\n&wUncompressed sent  :  &C", ch );
      show_transfer( ch, out_transfer );
      send_to_char( "\r\n&wCompressed sent    :  &C", ch );
      show_transfer( ch, mccp_out_transfer );
      send_to_char( "\r\n&wSaved by MCCP      :  &C", ch );
      show_transfer( ch, mccp_saved_transfer );
      send_to_char( "\r\n&wI3 data received  :  &C", ch );
      show_transfer( ch, i3_in_transfer );
      send_to_char( "\r\n&wI3 data sent      :  &C", ch );
      show_transfer( ch, i3_out_transfer );
      send_to_char( "\r\n&wTotal Received     :  &C", ch );
      show_transfer( ch, total_in_transfer );
      send_to_char( "\r\n&wTotal Sent         :  &C", ch );
      show_transfer( ch, total_out_transfer );
      send_to_char( "&D\r\n", ch );
      return;
   }

   /*
    * Just so we can start it over 
    */
   if( !str_cmp( argument, "clear" ) )
   {
      clear_transfer(  );
      transfer_reset_time = current_time;
      save_transfer(  );
      send_to_char( "The transfer table has been cleared.\r\n", ch );
      return;
   }

   send_to_char( "Usage: transfer <clear>\r\n", ch );
}

void fread_transfer( FILE * fp )
{
   const char *word;
   bool fMatch;

   for( ;; )
   {
      word = feof( fp ) ? "End" : fread_word( fp );
      fMatch = FALSE;

      if( word[0] == EOF )
         word = "End";

      switch ( UPPER( word[0] ) )
      {
         case '*':
            fMatch = TRUE;
            fread_to_eol( fp );
            break;

         case 'E':
            if( !str_cmp( word, "End" ) )
               return;
            break;

         case 'I':
            if( !str_cmp( word, "In" ) )
            {
               in_transfer->giga = fread_number( fp );
               in_transfer->mega = fread_number( fp );
               in_transfer->kilo = fread_number( fp );
               in_transfer->byte = fread_number( fp );
               fMatch = TRUE;
               break;
            }
            if( !str_cmp( word, "I3In" ) )
            {
               i3_in_transfer->giga = fread_number( fp );
               i3_in_transfer->mega = fread_number( fp );
               i3_in_transfer->kilo = fread_number( fp );
               i3_in_transfer->byte = fread_number( fp );
               fMatch = TRUE;
               break;
            }
            if( !str_cmp( word, "I3Out" ) )
            {
               i3_out_transfer->giga = fread_number( fp );
               i3_out_transfer->mega = fread_number( fp );
               i3_out_transfer->kilo = fread_number( fp );
               i3_out_transfer->byte = fread_number( fp );
               fMatch = TRUE;
               break;
            }
            break;

         case 'M':
            if( !str_cmp( word, "MCCPOut" ) )
            {
               mccp_out_transfer->giga = fread_number( fp );
               mccp_out_transfer->mega = fread_number( fp );
               mccp_out_transfer->kilo = fread_number( fp );
               mccp_out_transfer->byte = fread_number( fp );
               fMatch = TRUE;
               break;
            }
            if( !str_cmp( word, "MCCPSaved" ) )
            {
               mccp_saved_transfer->giga = fread_number( fp );
               mccp_saved_transfer->mega = fread_number( fp );
               mccp_saved_transfer->kilo = fread_number( fp );
               mccp_saved_transfer->byte = fread_number( fp );
               fMatch = TRUE;
               break;
            }
            break;

         case 'O':
            if( !str_cmp( word, "Out" ) )
            {
               out_transfer->giga = fread_number( fp );
               out_transfer->mega = fread_number( fp );
               out_transfer->kilo = fread_number( fp );
               out_transfer->byte = fread_number( fp );
               fMatch = TRUE;
               break;
            }
            break;

         case 'T':
            KEY( "TReset", transfer_reset_time, fread_time( fp ) );
            break;
      }

      if( !fMatch )
      {
         bug( "%s: no match: %s", __FUNCTION__, word );
         fread_to_eol( fp );
      }
   }
}

void load_transfer( void )
{
   FILE *fp;

   if( !( fp = fopen( TRANSFER_FILE, "r" ) ) )
   {
      log_printf( "%s: couldn't open %s for reading. Using Defaults.", __FUNCTION__, TRANSFER_FILE );
      perror( TRANSFER_FILE );
      return;
   }
   fread_transfer( fp );
   fclose( fp );
   fp = NULL;
}
