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
 \file      FTPHandler.h
 \author    Fernando Moreira ( f.pinto.moreira@gmail.com );
 \version   0.1
 \date      14-12-2010 - 11:09 GMT
 
 wraps a tcp based connection to a ftp server.
 provides an interface for basic io ops so far.
*/

#ifndef __FTP_HANDLER__
#define __FTP_HANDLER__



#include "TCPHandler.h"


typedef struct FTPHandler FTPHandler;

FTPHandler* OpenFTPChannel( TCPHandler* p_cmd, char* pa_user, char* pa_pass );
void CloseFTPChannel( FTPHandler* p_ftph );

int PassiveMode( FTPHandler* p_ftph );
int WriteCmdToFTP( FTPHandler* p_ftph, char* pa_cmd );
int ReadCmdFromFTP( FTPHandler* p_ftph, char* pa_buffer );
int WriteDataToFTP( FTPHandler* p_ftph, void* p_data, unsigned int len );
int ReadDataFromFTP( FTPHandler* p_ftph, void* p_buffer, unsigned int len );



#endif /* __FTP_HANDLER__ */