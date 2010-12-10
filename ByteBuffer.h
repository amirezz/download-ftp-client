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
 \version   xx.xx
 \date      dd-mm-yyyy - hh:mm GMT
 
 dynamic array of bytes data structure
*/

#ifndef __BYTEBUFFER_H__
#define __BYTEBUFFER_H__



typedef struct ByteBuffer ByteBuffer;

ByteBuffer* CreateByteBuffer( void );
void ReleaseByteBuffer( ByteBuffer* p_buffer );

unsigned int getLength( ByteBuffer* p_buffer );
unsigned int getUsedSpace( ByteBuffer* p_buffer );
unsigned int getFreeSpace( ByteBuffer* p_buffer );
void* getBuffer( ByteBuffer* p_buffer );
unsigned char getNthByte( ByteBuffer* p_buffer, unsigned int n );

void AddByte( unsigned char byte, ByteBuffer* p_bufer );
void AddBytes( unsigned char* pa_bytes, unsigned int len, ByteBuffer* p_buffer );
void MakeEmpty( ByteBuffer* p_bufer );



#endif /* __BYTEBUFFER_H__ */