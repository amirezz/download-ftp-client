/* ------------------------------------------------------------- */
/*  This file is a portion of the Download FTP Client software.  */
/*  It is distributed under the MIT License, which is available  */
/*  in the root of this distribution and at the following URL:   */
/*                                                               */
/*  http://www.opensource.org/licenses/mit-license.php           */
/*                                                               */
/*  Copyright (c) 2010 Fernando Moreira                          */
/* ------------------------------------------------------------- */

/*!
 \file      ByteBuffer.h
 \author    Fernando Moreira ( f.pinto.moreira@gmail.com );
 \version   0.1
 \date      13-12-2010 - 20:20 GMT
 
 dynamic array of bytes data structure
*/

#ifndef __BYTEBUFFER_H__
#define __BYTEBUFFER_H__



typedef struct ByteBuffer ByteBuffer;

//! creates a new \e ByteBuffer data structure
ByteBuffer* CreateByteBuffer( void );

//! deletes a \e ByteBuffer data structre
void ReleaseByteBuffer( ByteBuffer* p_buffer );

//! returns the total space allocated by a \e ByteBuffer data structure
unsigned int getLength( ByteBuffer* p_buffer );

//! returns the space used byte a \e ByteBuffer data structure
unsigned int getUsedSpace( ByteBuffer* p_buffer );

//! returns the space available inside a \e ByteBuffer data structure
unsigned int getFreeSpace( ByteBuffer* p_buffer );

//! returns the \e ByteBuffer's buffer
void* getBuffer( ByteBuffer* p_buffer );

//! returns the nth element of a \e ByteBuffer data structure (zero based)
unsigned char getNthByte( ByteBuffer* p_buffer, unsigned int n );

//! adds a single byte to the buffer
void AddByte( unsigned char byte, ByteBuffer* p_bufer );

//! adds an array of bytes with size \e len to the buffer
void AddBytes( unsigned char* pa_bytes, unsigned int len, ByteBuffer* p_buffer );

//! clears the buffer of a \e ByteBuffer data structure
void MakeEmpty( ByteBuffer* p_bufer );



#endif /* __BYTEBUFFER_H__ */