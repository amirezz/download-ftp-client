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
    Version:        xx.xx
    Last Update:    dd-mm-yyyy - hh:mm GMT
   =================================================================================================== */


#include <stdlib.h>
#include <string.h>
// ====================
#include "URLParser.h"



struct URLData
{
    char* pa_user;      // user string
    char* pa_password;  // password string
    char* pa_plainurl;  // url string with user and password fields stripped
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
    
    if( p_urldata->pa_plainurl )
        free( p_urldata->pa_plainurl );
    
    if( p_urldata )
        free( p_urldata );
}

// getUser
char* getUser( URLData* p_urldata )
{
    if( !p_urldata )
        return NULL;
    
    else return p_urldata->pa_user;
}

// getPassword
char* getPassword( URLData* p_urldata )
{
    if( !p_urldata )
        return NULL;
    
    else return p_urldata->pa_password;
}

// getPlainUrl
char* getPlainUrl( URLData* p_urldata )
{
    if( !p_urldata )
        return NULL;
    
    else return p_urldata->pa_plainurl;
}



// ParseFTPURL
URLData* ParseFTPURL( char* pa_url )
{
    // invalid pointer?
    if( !pa_url )
        return NULL;
    
    /*
     not yet implemented
     
     the url parsing is missing.
     user and password are left empty and
     the plain url matches the pa_url content
     */
    
    URLData* p_newurl       = malloc( sizeof( URLData ) );
    p_newurl->pa_user       = malloc( sizeof( char ) );
    p_newurl->pa_password   = malloc( sizeof( char ) );
    p_newurl->pa_plainurl   = malloc( sizeof( char ) * ( strlen( pa_url ) + 1 ) );
    
    p_newurl->pa_user[ 0 ]                     = '\0';
    p_newurl->pa_password[ 0 ]                 = '\0';
    p_newurl->pa_plainurl[ strlen( pa_url ) ]  = '\0';
    strcpy( p_newurl->pa_plainurl, pa_url );
    
    return p_newurl;
}