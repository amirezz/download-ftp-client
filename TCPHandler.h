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
 \file      TCPHandler.h
 \author    Fernando Moreira ( f.pinto.moreira@gmail.com );
 \version   0.1
 \date      08-12-2010 - 22:53 GMT
 
 wraps a tcp based network connection. client side only.
*/

#ifndef __TCP_HANDLER_H__
#define __TCP_HANDLER_H__



typedef struct TCPHandler TCPHandler;


TCPHandler* NewTCPHandler( char* pa_plaiurl, unsigned int remote_port );
void ReleaseTCPHandler( TCPHandler* p_tcph );

char* getRemoteName( TCPHandler* p_tcph );
char* getRemoteIP( TCPHandler* p_tcph );
unsigned int getRemotePort( TCPHandler* p_tcph );

int ReadFromRemote( TCPHandler* p_tcph, void* p_buffer, unsigned int len );
int WriteToRemote( TCPHandler* p_tcph, void* p_data, unsigned int len );



#endif /* __TCP_HANDLER_H__ */