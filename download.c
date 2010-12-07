#include <stdio.h>
// ====================
#include "URLParser.h"



int main( int argc, char** argv )
{
    // possible invalid usage
    if( argc < 2 )
    {
        printf( "dowload ftp client - wrong usage \n" );
        printf( "usage: ftp://[<user>:<password>@]<host>/<url-path> \n\n" );
        
        return -1;
    }
        
}