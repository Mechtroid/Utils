/*
 * AString.h
 *
 *  Created on: Dec 15, 2014
 *      Author: Engineeer
 */

#ifndef ASTRING_H_
#define ASTRING_H_
#include "AS1.h"
#include <string.h>


#define STREAMENTER "exit\r\n"
#define STREAMEXIT "$$$"
//Asynchronous Serial Helper
byte AS_SendChar(unsigned char);
void AS_SendString(unsigned char*, long);
void AS_SendDataString(unsigned char*);
void AS_SendReplyPage(unsigned char*, unsigned char*);
void AS_printf(unsigned char*, ...);
#endif /* ASTRING_H_ */
