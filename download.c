#include <stdio.h>
// =====================
#include "URLParser.h"
#include "TCPHandler.h"



int main( int argc, char** argv )
{
    // possible invalid usage
    /*
    if( argc < 2 )
    {
        printf( "dowload ftp client - wrong usage \n" );
        printf( "usage: download ftp://[<user>:<password>@]<host>/<url-path> \n\n" );
        
        return -1;
    }*/
    
    // parses the ftp url and fetches the required fields
    //URLData* p_urldata = ParseFTPURL( argv[ 1 ] );
    URLData* p_urldata = ParseFTPURL( "necg.fe.up.pt" );

    
    char* pa_plainurl = getPlainUrl( p_urldata );
    char* pa_user     = getUser( p_urldata );
    char* pa_password = getPassword( p_urldata );
    
    printf( "Host Name: %s\n", pa_plainurl );
    printf( "User: %s\n", pa_user );
    printf( "Password: " );
    
    char* pa_tmp = pa_password;
    while( *pa_tmp ) {
        
        printf( "*" );
        ++pa_tmp;
    }
    printf( "\n\n" );
    
    // tries to open a new tcp connection
    TCPHandler* p_cmd = NewTCPHandler( pa_plainurl, 21 );
    
    // failed to open a connection to the ftp server
    if( NULL == p_cmd )
        return -1;
    
    printf( "download ftp client - connection successfully established \n" );
    printf( "Remote Server Name: %s \n", getRemoteName( p_cmd ) );
    printf( "Remote Server IP: %s \n", getRemoteIP( p_cmd ) );
    printf( "Remote Server Port: %d \n\n", getRemotePort( p_cmd ) );
    
    ReleaseTCPHandler( p_cmd );
    
    return 0;
}