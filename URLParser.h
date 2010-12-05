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
 \file      URLParser.h
 \author    Fernando Moreira ( f.pinto.moreira@gmail.com );
 \version   xx.xx
 \date      dd-mm-yyyy - hh:mm GMT
 
 contains a set of functions and data structures that aid the parsing of a FTP url.
 
*/

#ifndef __URL_PARSER_H__
#define __URL_PARSER_H__



typedef struct URLData URLData;

/*! releases an \e URLData data structure created by the \e ParseFTPURL function
 
 \param     [in]   p_urldata   pointer to \e URLData structure that should be released
 \return    ---
 \remarks   if \e NULL = \e p_urldata the function does nothing
 \remarks   if \e p_urldata is invalid the function may develop umpredictable behaviour
 \attention not tested
*/
void ReleaseURLData( URLData* p_urldata );

/*!
 *  returns a null ended string representing the user field of an ftp url
 *  return \e NULL if no user is declared in the url
 */
char* getUser( URLData* p_urldata );

/*!
 *  returns a null ended string representing the password field of an ftp url
 *  return \e NULL if no password is declared in the url
 */
char* getPassword( URLData* p_urldata );

//! returns a null ended string representing the plain ftp url: user and password fiels are striped out
char* getPlainUrl( URLData* p_urldata );


/*! parses a ftp url getting the user, password and plain url if any
 
 \param     [in]   pa_url   pointer to a null-ended string contaning the ftp url
 \return    pointer to an \e URLData data structure
 \remarks   if \e NULL = \e pa_url the function does nothing
 \remarks   if \e pa_url is invalid the function may develop unpredictable behaviour
 \attention not tested
 */
URLData* ParseFTPURL( char* pa_url );



#endif /* __URL_PARSER_H__ */