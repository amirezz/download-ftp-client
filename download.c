#include <stdio.h>
#include <string.h>
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
    URLData* p_urldata = ParseFTPURL( "ftp://anonymous:ei06120@ftp.fe.up.pt/pub/Deec/jbarbosa/tese.pdf" );

    
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
    
    char buffer[1000];
    
    int len = ReadFromRemote( p_cmd, ( void* )buffer, 100 );
    WriteToRemote( p_cmd, ( void* )"user anonymous\n\r", strlen( "user anonymous\n\r" ) );
    len = ReadFromRemote( p_cmd, ( void* )buffer, 100 );
    WriteToRemote( p_cmd, ( void* )"pass -ei06120\n\r", strlen( "pass -ei06120\n\r" ) );
    len = ReadFromRemote( p_cmd, ( void* )buffer, 1000 );
    WriteToRemote( p_cmd, ( void* )"pasv\n\r", 6 );
    len = ReadFromRemote( p_cmd, ( void* )buffer, 100 );
    
    
    //ReleaseTCPHandler( p_data ); // releases the data connection
    ReleaseTCPHandler( p_cmd );  // releases the comand connection
    
    return 0;
}