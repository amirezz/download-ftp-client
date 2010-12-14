/* ------------------------------------------------------------- */
/*  This file is a portion of the Download FTP Client software.  */
/*  It is distributed under the MIT License, which is available  */
/*  in the root of this distribution and at the following URL:   */
/*                                                               */
/*  http://www.opensource.org/licenses/mit-license.php           */
/*                                                               */
/*  Copyright (c) 2010 Fernando Moreira                          */
/* ------------------------------------------------------------- */

/* ===============================================================================
    File:           ByteBuffer.c
    Description:    implements the functions and data structure from ByteBuffer.h
    Authors:        Fernando Moreira ( f.pinto.moreira@gmail.com );
    Version:        0.2
    Last Update:    13-12-2010 - 20:20 GMT
   =============================================================================== */



#include <stdlib.h>
#include <string.h>
// =====================
#include "ByteBuffer.h"



/*------------*/
/* ByteBuffer */
/*------------*/

struct ByteBuffer
{
	unsigned int length;		// full length of the buffer
	unsigned int freespace;		// free space inside the buffer
	unsigned char* p_buffer;	// the buffer...
};


// CreateByteBuffer
ByteBuffer* CreateByteBuffer( void )
{
	ByteBuffer* p_newbuffer = ( ByteBuffer* ) malloc( sizeof( ByteBuffer ) );

	if( p_newbuffer ) {
		p_newbuffer->length			= 1;
		p_newbuffer->freespace		= 1;
		p_newbuffer->p_buffer		= ( unsigned char* ) malloc( sizeof( unsigned char ) );
		p_newbuffer->p_buffer[ 0 ]	= '\0';

		return p_newbuffer;
	}

	else return NULL;
}

// ReleaseByteBuffer
void ReleaseByteBuffer( ByteBuffer* p_buffer )
{
    // valid pointer?
	if( !p_buffer )
        return;

	if( p_buffer->p_buffer )
		free( p_buffer->p_buffer );

	free( p_buffer );
}

// getLength
unsigned int getLength( ByteBuffer* p_buffer )
{
	if( !p_buffer )
        return 0;

	else return
        p_buffer->length;
}

// getUsedSpace
unsigned int getUsedSpace( ByteBuffer* p_buffer )
{
	if( !p_buffer )
        return 0;

	else return
        p_buffer->length - p_buffer->freespace;
}

// getFreeSpace
unsigned int getFreeSpace( ByteBuffer* p_buffer )
{
	if( !p_buffer )
        return 0;

	else return
        p_buffer->freespace;
}

// getBuffer
void* getBuffer( ByteBuffer* p_buffer )
{
	if( !p_buffer )
        return NULL;

    else return
        ( void* )p_buffer->p_buffer;
}

// getNthByte
unsigned char getNthByte( ByteBuffer* p_buffer, unsigned int n )
{
    if( !p_buffer )
        return '\0';

    else if( n > p_buffer->length )
        return '\0';

    else return
        p_buffer->p_buffer[ n ];
}

// AddByte
void AddByte( unsigned char byte, ByteBuffer* p_buffer )
{
	if( !p_buffer )
        return;

	// is there any space available to add a new byte?
	if( getFreeSpace( p_buffer ) > 0 )
	{
		p_buffer->p_buffer[ p_buffer->length - p_buffer->freespace ] = byte;
		--p_buffer->freespace;

		return;
	}

	// nop...
	else
	{
		unsigned char* p_newbuffer = ( unsigned char* )malloc( sizeof( unsigned char ) * p_buffer->length * 2 );	// opens space for a new buffer with twice the length of the previous one
		memset( ( void* )p_newbuffer, 0, sizeof( unsigned char ) * p_buffer->length * 2 );							// clears the new buffer
		memcpy( ( void* )p_newbuffer, ( void* )( p_buffer->p_buffer ), p_buffer->length );		   					// copies the older content
		free( p_buffer->p_buffer );														   	   						// releases the previous buffer
		p_buffer->p_buffer = p_newbuffer;													   						// and saves the new one

		p_buffer->length	*= 2;												// updates the length of the new buffer
		p_buffer->freespace	= p_buffer->length / 2;
		p_buffer->p_buffer[ p_buffer->length - p_buffer->freespace ] = byte;	// saves the byte
		--p_buffer->freespace;													// one less byte available

		return;
	}
}

// AddBytes
void AddBytes( unsigned char* pa_bytes, unsigned int len, ByteBuffer* p_buffer )
{
    // invalid pointers?
    if( !pa_bytes || !p_buffer )
        return;
    
    while( len ){
        AddByte( *pa_bytes, p_buffer );
        ++pa_bytes;
        --len;
    }
}

// MakeEmpty
void MakeEmpty( ByteBuffer* p_buffer )
{
		if( !p_buffer )
			return;

	p_buffer->freespace = p_buffer->length;
	memset( ( void* )( p_buffer->p_buffer ), 0, sizeof( unsigned char ) * p_buffer->length );
}
