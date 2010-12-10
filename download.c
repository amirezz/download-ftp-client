#include <stdio.h>
#include <string.h>
// =====================
#include "URLParser.h"
#include "TCPHandler.h"
#include "FTPHandler.h"



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
    URLData* p_urldata = ParseFTPURL( "ftp://anonymous:-anonymous@ftp.fe.up.pt/pub/Deec/jbarbosa/tese.pdf" );
    
    char* pa_hostname = getHostName( p_urldata );
    char* pa_path     = getUrlPath( p_urldata );
    char* pa_user     = getUser( p_urldata );
    char* pa_password = getPassword( p_urldata );
    
    printf( "Host Name: %s\n", pa_hostname );
    printf( "Url Path: %s\n", pa_path );
    printf( "User: %s\n", pa_user );
    printf( "Password: " );
    
    char* pa_tmp = pa_password;
    while( *pa_tmp ) {
        
        printf( "*" );
        ++pa_tmp;
    }
    printf( "\n\n" );
    
    // tries to open a new ftp cmd connection
    TCPHandler* p_cmd = NewTCPHandler( pa_hostname, 21 );
    
    // failed to open a connection to the ftp server
    if( NULL == p_cmd )
        return -1;
    
    printf( "download ftp client - connection successfully established \n" );
    printf( "Remote Server Name: %s \n", getRemoteName( p_cmd ) );
    printf( "Remote Server IP: %s \n", getRemoteIP( p_cmd ) );
    printf( "Remote Server Port: %d \n\n", getRemotePort( p_cmd ) );
    
    // opens a new FTP channel
    FTPHandler* p_ftph = OpenFTPChannel( p_cmd, pa_user, pa_password );
    
    if( NULL == p_ftph ) { // ftp authentication failed
        printf( "download ftp client - failed ftp authentication \n" );
        ReleaseTCPHandler( p_cmd );
    }
    
    // and activates the passive mode
    if( -1 == PassiveMode( p_ftph ) ) {
        printf( "download ftp client - failed to request the passive mode \n" );
        CloseFTPChannel( p_ftph );
        
        return -1;
    }
    
    else {
        CloseFTPChannel( p_ftph );
    }

    
    return 0;
}