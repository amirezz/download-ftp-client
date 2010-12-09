/* ------------------------------------------------------------- */
/*  This file is a portion of the Download FTP Client software.  */
/*  It is distributed under the MIT License, which is available  */
/*  in the root of this distribution and at the following URL:   */
/*                                                               */
/*  http://www.opensource.org/licenses/mit-license.php           */
/*                                                               */
/*  Copyright (c) 2010 Fernando Moreira                          */
/* ------------------------------------------------------------- */

/* ===================================================================================================
    File:           URLParser.c
    Description:    contains a set of functions and data structures that aid the parsing of a FTP url
    Authors:        Fernando Moreira ( f.pinto.moreira@gmail.com );
    Version:        0.1
    Last Update:    08-12-2010 - 22:53 GMT
   =================================================================================================== */


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
// ====================
#include "URLParser.h"



struct URLData
{
    char* pa_user;      // user string
    char* pa_password;  // password string
    char* pa_hostname;  // name of the remote host
    char* pa_urlpath;   // host's directory path
};


// ReleaseURLData
void ReleaseURLData( URLData* p_urldata )
{
    // invalid pointer?
    if( !p_urldata )
        return;
    
    if( p_urldata->pa_user )
        free( p_urldata->pa_user );
    
    if( p_urldata->pa_password )
        free( p_urldata->pa_password );
    
    if( p_urldata->pa_hostname )
        free( p_urldata->pa_hostname );
    
    if( p_urldata->pa_urlpath )
        free( p_urldata->pa_urlpath );
    
    if( p_urldata )
        free( p_urldata );
}

// getUser
char* getUser( URLData* p_urldata )
{
    if( !p_urldata )
        return "";
    
    else return p_urldata->pa_user;
}

// getPassword
char* getPassword( URLData* p_urldata )
{
    if( !p_urldata )
        return "";
    
    else return p_urldata->pa_password;
}

// getPlainUrl
char* getHostName( URLData* p_urldata )
{
    if( !p_urldata )
        return "";
    
    else return p_urldata->pa_hostname;
}

// getUrlPath
char* getUrlPath( URLData* p_urlpath )
{
    if( !p_urlpath )
        return "";
    
    else return p_urlpath->pa_urlpath;
}


// ParseFTPURL
URLData* ParseFTPURL( char* pa_url )
{
    // invalid pointer?
    if( !pa_url )
        return NULL;
    
    /*
     url parsing.
     parses an url that adopts this url syntax: ftp://[<user>:<password>@]<host>/<url-path>
    */
    
    URLData* p_newurl = ( URLData* )malloc( sizeof( URLData ) );
    memset( ( void* )p_newurl, 0, sizeof( URLData ) );
    
    char name[ 20 + 1 ];     // user name buffer
    char password[ 20 + 1 ]; // user password buffer
    char host[ 20 + 1 ];     // remote host name buffer
    char path[ 100 +1 ];     // remote host directory path buffer
    
    int result = sscanf( pa_url, "ftp://%20[^:]:%20[^@]@%20[^/]/%s", name, password, host, path );
    if( 4 == result ){
        p_newurl->pa_user = ( char* )malloc( sizeof( char ) * ( strlen( name ) + 1 ) );
        strcpy( p_newurl->pa_user, name );
        
        p_newurl->pa_password = ( char* )malloc( sizeof( char ) * ( strlen( password ) + 1 ) );
        strcpy( p_newurl->pa_password, password );
        
        p_newurl->pa_hostname = ( char* )malloc( sizeof( char ) * ( strlen( host ) + 1 ) );
        strcpy( p_newurl->pa_hostname, host );
        
        p_newurl->pa_urlpath = ( char* )malloc( sizeof( char ) * ( strlen( path ) + 1 ) );
        strcpy( p_newurl->pa_urlpath, path );
        
        return p_newurl;
    }
    
    else
    {
        result = sscanf( pa_url, "ftp://%20[^/]/%s", host, path );
        if( 2 == result ){
            
            p_newurl->pa_user     = "";
            p_newurl->pa_password = "";
            
            p_newurl->pa_hostname = ( char* )malloc( sizeof( char ) * ( strlen( host ) + 1 ) );
            strcpy( p_newurl->pa_hostname, host );
            
            p_newurl->pa_urlpath = ( char* )malloc( sizeof( char ) * ( strlen( path ) + 1 ) );
            strcpy( p_newurl->pa_urlpath, path );
            
            return p_newurl;
        }
        
        else {
            free( p_newurl );
            return NULL;
        }

    }
}