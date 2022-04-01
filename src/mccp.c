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
 *                         Client Compression Module                         *
 *****************************************************************************/

/*
 * mccp.c - support functions for mccp (the Mud Client Compression Protocol)
 *
 * see http://mccp.afkmud.com
 *
 * Copyright (c) 1999, Oliver Jowett <oliver@randomly.org>.
 *
 * This code may be freely distributed and used if this copyright notice is
 * retained intact.
 */

#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

/* Socket and TCP/IP stuff. */
#ifdef WIN32
#include <io.h>
#include <winsock2.h>
#undef EINTR
#undef EMFILE
#define EINTR WSAEINTR
#define EMFILE WSAEMFILE
#define EWOULDBLOCK WSAEWOULDBLOCK
#define MAXHOSTNAMELEN 32

#define  TELOPT_ECHO '\x01'
#define  NOP         '\xF1'
#define  GA          '\xF9'
#define  SB          '\xFA'
#define  WILL        '\xFB'
#define  WONT        '\xFC'
#define  DO          '\xFD'
#define  DONT        '\xFE'
#define  IAC         '\xFF'

void bailout( void );
void shutdown_checkpoint( void );
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <arpa/telnet.h>
#include <netdb.h>
#endif
#include "h/mud.h"
#include "h/mccp.h"

bool write_to_descriptor args((DESCRIPTOR_DATA *d, const char *txt, int length));


#if defined(__OpenBSD__) || defined(__FreeBSD__)
#define ENOSR 63
#endif

int mccpusers;
const unsigned char will_compress2_str[] = { IAC, WILL, TELOPT_COMPRESS2, '\0' };
const unsigned char start_compress2_str[] = { IAC, SB, TELOPT_COMPRESS2, IAC, SE, '\0' };

bool process_compressed( DESCRIPTOR_DATA * d )
{
   int iStart = 0, nBlock, nWrite, len;

   if( !d->mccp->out_compress )
      return TRUE;

   /*
    * Try to write out some data.. 
    */
   len = d->mccp->out_compress->next_out - d->mccp->out_compress_buf;

   if( len > 0 )
   {
      /*
       * we have some data to write 
       */
      for( iStart = 0; iStart < len; iStart += nWrite )
      {
         nBlock = UMIN( len - iStart, 4096 );
         if( ( nWrite = send( d->descriptor, d->mccp->out_compress_buf + iStart, nBlock, 0 ) ) < 0 )
         {
            if( errno == EAGAIN || errno == ENOSR )
               break;

            return FALSE;
         }

         if( !nWrite )
            break;
      }

      if( iStart )
      {
         /*
          * We wrote "iStart" bytes 
          */
         if( iStart < len )
            memmove( d->mccp->out_compress_buf, d->mccp->out_compress_buf + iStart, len - iStart );

         d->mccp->out_compress->next_out = d->mccp->out_compress_buf + len - iStart;
      }
   }
   return TRUE;
}

bool compressStart( DESCRIPTOR_DATA * d )
{
   z_stream *s;

   if( d->mccp->out_compress )
      return TRUE;

   CREATE( s, z_stream, 1 );
   CREATE( d->mccp->out_compress_buf, unsigned char, COMPRESS_BUF_SIZE );

   s->next_in = NULL;
   s->avail_in = 0;

   s->next_out = d->mccp->out_compress_buf;
   s->avail_out = COMPRESS_BUF_SIZE;

   s->zalloc = Z_NULL;
   s->zfree = Z_NULL;
   s->opaque = NULL;

   if( deflateInit( s, 9 ) != Z_OK )
   {
      DISPOSE( d->mccp->out_compress_buf );
      DISPOSE( s );
      return FALSE;
   }

   write_to_descriptor( d, ( const char * )start_compress2_str, 0 );

   d->can_compress = TRUE;
   d->mccp->out_compress = s;
   mccpusers++;
   return TRUE;
}

bool compressEnd( DESCRIPTOR_DATA * d )
{
   unsigned char dummy[1];

   if( !d || !d->mccp || !d->mccp->out_compress )
      return TRUE;

   d->mccp->out_compress->avail_in = 0;
   d->mccp->out_compress->next_in = dummy;

   if( deflate( d->mccp->out_compress, Z_FINISH ) != Z_STREAM_END )
      return FALSE;

   if( !process_compressed( d ) )   /* try to send any residual data */
      return FALSE;

   deflateEnd( d->mccp->out_compress );
   DISPOSE( d->mccp->out_compress_buf );
   DISPOSE( d->mccp->out_compress );
   mccpusers--;

   return TRUE;
}

void do_compress( CHAR_DATA *ch, char *argument )
{
   if( !ch->desc )
   {
      send_to_char( "What descriptor?!\n", ch );
      return;
   }

   if( !ch->desc->mccp->out_compress )
   {
      if( !compressStart( ch->desc ) )
         send_to_char( "&RCompression failed to start.\r\n", ch );
      else
         send_to_char( "&GOk, compression enabled.\r\n", ch );
   }
   else
   {
      compressEnd( ch->desc );
      ch->desc->can_compress = FALSE;
      send_to_char( "&ROk, compression disabled.\r\n", ch );
   }
}
