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
 \version   xx.xx
 \date      dd-mm-yyyy - hh:mm GMT
 
 <extended description>
*/

#ifndef __FTP_HANDLER__
#define __FTP_HANDLER__



#include "TCPHandler.h"


typedef struct FTPHandler FTPHandler;

FTPHandler* OpenFTPChannel( TCPHandler* p_cmd, char* pa_user, char* pa_password );
FTPHandler* CloseFTPChannel( FTPHandler* p_ftph );

int PassiveMode( FTPHandler* p_ftph );
int WriteToFTP( FTPHandler* p_ftph );
int ReadFromFTP( FTPHandler* p_ftph );



#endif /* __FTP_HANDLER__ */