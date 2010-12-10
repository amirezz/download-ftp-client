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
    File:           FTPHandler.c
    Description:    implements the functions and data structure from TCPHandler.h
    Authors:        Fernando Moreira ( f.pinto.moreira@gmail.com );
    Version:        xx.xx
    Last Update:    dd-mm-yyyy - hh:mm GMT
   =============================================================================== */


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
// =====================
#include "FTPHandler.h"
#include "TCPHandler.h"
#include "ByteBuffer.h"



struct FTPHandler
{
    TCPHandler* p_cmd;
    TCPHandler* p_data;
};


// OpenFTPChannel
FTPHandler* OpenFTPChannel( TCPHandler* p_cmd, char* pa_user, char* pa_pass )
{
        if( !p_cmd || !pa_user || !pa_pass )
        return NULL;
    
    FTPHandler* p_ftph = ( FTPHandler* )malloc( sizeof( FTPHandler* ) );
    p_ftph->p_cmd      = p_cmd;
    p_ftph->p_data     = NULL;
    char answer[ 500 ];
    
    // reads the welcome message
    ReadCmdFromFTP( p_ftph, answer );
    printf( "server reply: %s\n", answer );
    
    // sends the 'user' command
    ByteBuffer* p_bb = CreateByteBuffer();
    AddBytes( ( unsigned char* )"user ", strlen( "user " ), p_bb );
    AddBytes( ( unsigned char* ) pa_user, strlen( pa_user ), p_bb );
    AddBytes( ( unsigned char* ) "\r\n", 2, p_bb );
    WriteToRemote( p_cmd, getBuffer( p_bb ), getUsedSpace( p_bb ) );
    
    // reads the answer
    ReadCmdFromFTP( p_ftph, answer );
    printf( "server reply: %s\n", answer );
    
    // sends the 'pass' cmd
    MakeEmpty( p_bb );
    AddBytes( ( unsigned char* )"pass ", strlen( "pass " ), p_bb );
    AddBytes( ( unsigned char* ) pa_pass, strlen( pa_pass ), p_bb );
    AddBytes( ( unsigned char* ) "\r\n", 2, p_bb );
    WriteToRemote( p_cmd, getBuffer( p_bb ), getUsedSpace( p_bb ) );
    
    // reads the answer
    ReadCmdFromFTP( p_ftph, answer );
    printf( "server reply: %s\n", answer );
    
    // the user was successfuly authenticated
    ReleaseByteBuffer( p_bb );
    
    return p_ftph;
}

// CloseFTPChannel
void CloseFTPChannel( FTPHandler* p_ftph )
{
    // invalid pointer?
    if( !p_ftph )
        return;
    
    if( p_ftph->p_data )
        ReleaseTCPHandler( p_ftph->p_data );
    
    if( p_ftph->p_cmd )
        ReleaseTCPHandler( p_ftph->p_cmd );
    
    free( p_ftph );
}

// PassiveMode
int PassiveMode( FTPHandler* p_ftph )
{
    // request a passive behaviour aka 'pasv'
    ByteBuffer* p_bb = CreateByteBuffer();
    AddBytes( ( unsigned char* )"pasv\r\n", strlen( "pasv\r\n" ), p_bb );
    WriteToRemote( p_ftph->p_cmd, getBuffer( p_bb ), getUsedSpace( p_bb ) );
    ReleaseByteBuffer( p_bb );
    
    // reads the welcome message
    char answer[ 500 ];
    ReadCmdFromFTP( p_ftph, answer );
    printf( "server reply: %s\n", answer );
    
    // checks the answer
    int port         = 0;
    ByteBuffer* p_ip = CreateByteBuffer();
    if( ( '2' == answer[ 0 ] ) &&
        ( '2' == answer[ 1 ] ) &&
        ( '7' == answer[ 2 ] ) )
    {
        // get the remote ip and port
        char buffer[ 60 ];
        char tokens[] = "Entering Passive Mode (,) ";
        strcpy( buffer, answer + 3 );
        
        // IPv4
        char* p_nextset = strtok( buffer, tokens );
        AddBytes( ( unsigned char* )p_nextset, strlen( p_nextset ), p_ip );
        AddByte( ( unsigned char )'.', p_ip );
        
        p_nextset = strtok( NULL, tokens );
        AddBytes( ( unsigned char* )p_nextset, strlen( p_nextset ), p_ip );
        AddByte( ( unsigned char )'.', p_ip );
        
        p_nextset = strtok( NULL, tokens );
        AddBytes( ( unsigned char* )p_nextset, strlen( p_nextset ), p_ip );
        AddByte( ( unsigned char )'.', p_ip );
        
        p_nextset = strtok( NULL, tokens );
        AddBytes( ( unsigned char* )p_nextset, strlen( p_nextset ), p_ip );
        AddBytes( ( unsigned char* )"", 1, p_ip );
        
        // port
        int dummy       = 0;
        char* p_portstr = strtok( NULL, tokens );
        sscanf( p_portstr, "%d", &dummy );
        
        port += dummy * 256;
        
        p_portstr = strtok( NULL, tokens );
        sscanf( p_portstr, "%d", &dummy );
        
        port += dummy;
    }
    
    else return -1;

    
    // open a new data connection
    TCPHandler* p_data = NewTCPHandler( ( char* )getBuffer( p_ip ), port );
    
    if( NULL == p_data ) { // data connection failed
        ReleaseByteBuffer( p_ip );
        
        return -1;
    }
    
    // data connection successfuly established
    else {
        ReleaseByteBuffer( p_ip );
        p_ftph->p_data = p_data;
        
        return 0;
    }

    
}

// WriteCmdToFTP
int WriteCmdToFTP( FTPHandler* p_ftph, char* pa_cmd )
{
    if( !p_ftph || !pa_cmd )
        return -1;
    
    int size = WriteToRemote( p_ftph->p_cmd, ( void* )pa_cmd, strlen( pa_cmd ) );
    
    return ( size > -1 ? size : 0 );
}

// ReadCmdFromFTP
int ReadCmdFromFTP( FTPHandler* p_ftph, char* pa_buffer )
{
    // invalid pointers?
    if( !p_ftph || !pa_buffer )
        return -1;

    ByteBuffer* p_answer    = CreateByteBuffer(); // reply buffer
    unsigned int read_bytes = 0;                  // number of bytes read in each pass
    char bytebuffer         = '\0';               // self explains
    do {
        read_bytes = 0;
        int size   = 0;
        while( !((( size = ReadFromRemote( p_ftph->p_cmd, ( void* )&bytebuffer, 1 )) < 0 ) &&
                 ( '\n' == bytebuffer )) ) {
            if( 1 == size ) {
                AddByte( ( unsigned char )bytebuffer, p_answer );
                ++read_bytes;
            }
        }
        
        usleep( 200000 ); // 200 milisseconds
    }
    while( read_bytes > 0 );
    
    strcpy( pa_buffer, ( char* )getBuffer( p_answer ) );
    
    unsigned int size = getUsedSpace( p_answer );
    ReleaseByteBuffer( p_answer );
    
    return size;
}

// WriteDataToFTP
int WriteDataToFTP( FTPHandler* p_ftph, void* p_data, unsigned int len )
{
    if( !p_ftph || !p_data )
        return -1;
    
    int size = WriteToRemote( p_ftph->p_data, p_data, len );
    
    return ( size > -1 ? size : 0 );
}

// ReadDataFromFTP
int ReadDataFromFTP( FTPHandler* p_ftph, void* p_buffer, unsigned int len )
{
    // invalid pointers?
    if( !p_ftph || !p_buffer )
        return -1;

    char* p_buffer_alias    = ( char* )p_buffer;
    char bytebuffer         = '\0'; // self explains
    unsigned int read_bytes = 0;    // total number of read bytes
    unsigned int accum      = 0;    // number of bytes read in each pass
    do {
        accum = 0;
        
        while( ( ReadFromRemote( p_ftph->p_cmd, ( void* )&bytebuffer, 1 ) > 0 ) &&
               ( len > 0 ) ) {
            
            *p_buffer_alias = bytebuffer;

            ++p_buffer_alias;
            ++read_bytes;
            --len;
        }
        
        usleep( 200000 ); // 200 milisseconds
    }
    while( accum > 0 );
    
    return read_bytes;
}