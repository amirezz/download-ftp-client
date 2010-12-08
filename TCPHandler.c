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
    File:           TCPHandler.c
    Description:    implements the functions and data structure from TCPHandler.h
    Authors:        Fernando Moreira ( f.pinto.moreira@gmail.com );
    Version:        xx.xx
    Last Update:    dd-mm-yyyy - hh:mm GMT
   =============================================================================== */


#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
// =====================
#include "TCPHandler.h"



struct TCPHandler
{
    char* pa_remotename;
    char* pa_remoteip;
    int remoteport;
    int socketfd;
};


// NewTCPHandler
TCPHandler* NewTCPHandler( char* pa_plainurl, unsigned int port )
{
    TCPHandler* p_tcph = ( TCPHandler* )malloc( sizeof( TCPHandler ) );
    memset( ( void* )p_tcph, 0, sizeof( TCPHandler ) );
    
    // fectches the remote server info
    struct hostent* p_remoteinfo = gethostbyname( pa_plainurl );
    
    // invalid host name?
    if( NULL == p_remoteinfo )
    {
        printf( "download ftp client - invalid host \n\n" );
        ReleaseTCPHandler( p_tcph );
        
        return NULL;
    }
    
    // saves the remote server's name
    p_tcph->pa_remotename = ( char* )malloc( sizeof( char ) * ( strlen( p_remoteinfo->h_name ) + 1 ) );
    memcpy( p_tcph->pa_remotename, p_remoteinfo->h_name, sizeof( char ) * ( strlen( p_remoteinfo->h_name ) + 1 ) );
    
    // saves the remote server's ip
    p_tcph->pa_remoteip = ( char* )malloc( sizeof( char ) * ( strlen( inet_ntoa( *( ( struct in_addr* )( p_remoteinfo->h_addr ) ) ) ) + 1 ) );
    
    memcpy( p_tcph->pa_remoteip,
            inet_ntoa( *( ( struct in_addr* )( p_remoteinfo->h_addr ) ) ),
            sizeof( char ) * ( strlen( inet_ntoa( *( ( struct in_addr* )( p_remoteinfo->h_addr ) ) ) ) + 1 ) );
    
    // saves the remote server's port
    p_tcph->remoteport = port;
    
    
    // creates a new socket
    int fd = socket( AF_INET, SOCK_STREAM, 0 );
    
    if( -1 == fd )
    {
        printf("download ftp client - failed to create a new socket \n\n");
        ReleaseTCPHandler( p_tcph );
        
        return NULL;
    }
    else p_tcph->socketfd = fd;
    
    // declares the remote server info
    struct sockaddr_in remote_info;
    memset( ( void* )&remote_info, 0, sizeof( struct sockaddr_in ) );
    
    remote_info.sin_family      = AF_INET;
	remote_info.sin_addr.s_addr = inet_addr(p_tcph->pa_remoteip );
	remote_info.sin_port        = htons( p_tcph->remoteport );
    
    // open a tcp connection to the remote server
    int result = connect( p_tcph->socketfd, ( struct sockaddr* )&remote_info, sizeof( struct sockaddr_in ) );
    
    if( -1 == result )
    {
        printf( "download ftp client - failed to connect to the remote server \n\n" );
        ReleaseTCPHandler( p_tcph );
        
        return NULL;
    }
    
    else return p_tcph; // the connection was successfuly established
}

// ReleaseTCPHandler
void ReleaseTCPHandler( TCPHandler* p_tcph )
{
    // valid pointer?
    if( !p_tcph )
        return;
    
    if( p_tcph->pa_remotename )
        free( p_tcph->pa_remotename );
    
    if( p_tcph->pa_remoteip )
        free( p_tcph->pa_remoteip );
    
    if( p_tcph->socketfd > 0 )
        close( p_tcph->socketfd );
    
    free( p_tcph );
}

// getRemoteName
char* getRemoteName( TCPHandler* p_tcph )
{
    // invalid pointer
    if( !p_tcph )
        return "";
    
    if( p_tcph->pa_remotename )
        return p_tcph->pa_remotename;
    
    else return "";
}

// getRemoteIP
char* getRemoteIP( TCPHandler* p_tcph )
{
    // invalid pointer
    if( !p_tcph )
        return "";
    
    if( p_tcph->pa_remoteip )
        return p_tcph->pa_remoteip;
    
    else return "";
}

// getRemotePort
unsigned int getRemotePort( TCPHandler* p_tcph )
{
    if( !p_tcph )
        return 0;
    
    else return p_tcph->remoteport;
}