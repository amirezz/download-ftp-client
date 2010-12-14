#include <stdio.h>
#include <string.h>
// =====================
#include "URLParser.h"
#include "TCPHandler.h"
#include "FTPHandler.h"
#include "ByteBuffer.h"



int main( int argc, char** argv )
{
    // possible invalid usage
    if( argc < 2 )
    {
        printf( "dowload ftp client - wrong usage \n" );
        printf( "usage: download ftp://[<user>:<password>@]<host>/<url-path> \n\n" );
        
        return -1;
    }
    
    
    // parses the ftp url and fetches the required fields
    URLData* p_urldata = ParseFTPURL( argv[ 1 ] );
            
    char* pa_hostname = getHostName( p_urldata );
    char* pa_path     = getUrlPath( p_urldata );
    char* pa_user     = getUser( p_urldata );
    char* pa_password = getPassword( p_urldata );
    
    
    // parses the url path and retrieves the file name
    ByteBuffer* p_buffer = CreateByteBuffer();
    AddBytes( ( unsigned char* )pa_path, strlen( pa_path ) + 1, p_buffer );
    
    char* p_nexttoken = strtok( ( char* ) getBuffer( p_buffer ), "/" );
    char* p_filename  = p_nexttoken;
    
    while( p_nexttoken = strtok( NULL, "/" ) )
        p_filename = p_nexttoken;
    
    
    // prints the connection's details
    printf( "Host Name: %s\n", pa_hostname );
    printf( "Url Path: %s\n", pa_path );
    printf( "File Name: %s\n", p_filename );
    printf( "User: %s\n", pa_user );
    printf( "Password: " );
    
    char* pa_tmp = pa_password;
    while( *pa_tmp ) {
        
        printf( "*" );
        ++pa_tmp;
    }
    printf( "\n\n" );
    
    
    // tries to open a new tcp based connection to the ftp server
    TCPHandler* p_cmd = NewTCPHandler( pa_hostname, 21 );
    
    // failed to open a connection to the ftp server
    if( NULL == p_cmd )
        return -1;
    
    
    // prints the remote server's details
    printf( "download ftp client - connection successfully established \n" );
    printf( "Remote Server Name: %s \n", getRemoteName( p_cmd ) );
    printf( "Remote Server IP: %s \n", getRemoteIP( p_cmd ) );
    printf( "Remote Server Port: %d \n\n", getRemotePort( p_cmd ) );
    
    
    // opens a new FTP communication channel
    FTPHandler* p_ftph = OpenFTPChannel( p_cmd, pa_user, pa_password );
    
    if( NULL == p_ftph ) { // ftp authentication failed
        printf( "download ftp client - failed ftp authentication \n\n" );
        ReleaseTCPHandler( p_cmd );
    }
    
    
    // and activates the passive mode
    int result = PassiveMode( p_ftph );
    
    if( -1 == result ) { // failed to request the passive mode
        printf( "download ftp client - failed to request the passive mode \n\n" );
        CloseFTPChannel( p_ftph );
        
        return -1;
    }
    
    
    // requests the remote file size
    ByteBuffer* p_request = CreateByteBuffer();
    AddBytes( ( unsigned char* )"size ",5, p_request );
    AddBytes( ( unsigned char* )pa_path, strlen( pa_path ), p_request );
    AddBytes( ( unsigned char* )"\r\n",2, p_request );
    WriteCmdToFTP( p_ftph, ( char* )getBuffer( p_request ) ); // writes the SIZE request to the remote FTP server
    
    char answer[ 500 ];
    int filesize = 0;
    result = ReadCmdFromFTP( p_ftph, answer ); // reads the answer
    
    if( -1 == result ) {
        printf( "download ftp client - failed to retrive the remote file size \n\n" );
        
        return -1;
    }
    else sscanf( answer, "%*s %d", &filesize );
    
    printf( "download ftp client - the download will now beggin (press any key!)\n" );
    printf( "Remote File Name: %s \n", p_filename );
    printf( "Remote File Size: %d \n\n", filesize );
    getchar();

    
    // changes the file to binary
    MakeEmpty( p_request );
    AddBytes( ( unsigned char* )"type I\r\n",8, p_request );
    WriteCmdToFTP( p_ftph, ( char* )getBuffer( p_request ) ); // writes the TYPE request to the remote FTP server
    
    
    // requests the download of the file
    MakeEmpty( p_request );
    AddBytes( ( unsigned char* )"retr ",5, p_request );
    AddBytes( ( unsigned char* )pa_path, strlen( pa_path ), p_request );
    AddBytes( ( unsigned char* )"\r\n",2, p_request );
    WriteCmdToFTP( p_ftph, ( char* )getBuffer( p_request ) ); // writes the RETR request to the remote FTP server
    
    
    // and downloads the file
    char filebuffer[ 1024 ];
    FILE* p_file = fopen( p_filename, "w" );
    while( filesize > 0 )
    {
        result = ReadDataFromFTP( p_ftph, filebuffer, filesize > 1024 ? 1024 : filesize );
        
        if( result > 0 ) {
            filesize -= result;
            fwrite( ( void* )filebuffer, 1, result , p_file );
            
        }
    }
    fclose( p_file ); // the file was successfuly read
    

    // closes the connection
    CloseFTPChannel( p_ftph );
    
    return 0;
}