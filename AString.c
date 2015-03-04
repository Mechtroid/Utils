/*
 * AString.c
 *
 *  Created on: Dec 16, 2014
 *      Author: Engineeer
 */
#include "AString.h"
#include "PE_Types.h"
#include "UTIL1.h"
#include "CDC1.h"
#include "Utils.h"
#include <string.h>
#include <stdarg.h>

bool streamMode = TRUE;
byte AS_SendChar(unsigned char cOut)
{
	CDC1_SendChar(cOut);
	if(AS1_SendChar(cOut)) //Failed with ERR_TXFULL
	{
		while(AS1_GetCharsInTxBuf() == AS1_OUT_BUF_SIZE);
		AS1_SendChar(cOut);
	}

}

/* AS_SendString(unsigned char* sOut, long count)
 * From the zero terminated string <sOut>, send <count> characters.
 * If <count> is 0, send characters until you hit a null char.
 * TODO: If <count> is negative, send <count> chars regardless of content.
 * 		(For sending straight data.)
 */
void AS_SendString(unsigned char* sOut, long count)
{
	CDC1_SendChar('>');
	int i = 0;

	while((i < count || count == 0) && sOut[i] != '\0')
	{
		AS_SendChar(sOut[i]);
		i++;
	}

	if(streamMode == FALSE && UTIL1_strncmp(sOut, (unsigned char*)STREAMENTER, 4) == 0)
	{
		streamMode = TRUE;
	}
	else if(streamMode == TRUE && UTIL1_strncmp(sOut, (unsigned char*)STREAMEXIT, 3) == 0)
	{
		streamMode = FALSE;
	}
}

unsigned char buf[1500];
size_t bufSize = sizeof(buf);
void AS_SendDataString(unsigned char* sData)
{
	unsigned char* commandBuffer = buf;
	size_t commandBufferSize = bufSize;
	size_t dataSize = strlen(sData);

	if(sData == buf) //clearing out our commandBuffer would mangle the data string, so start the command buffer at the END of the data string.
	{
		commandBuffer = buf + dataSize + 1;
		commandBufferSize = bufSize - dataSize - 1;
	}
	commandBuffer[0]='\0';

	UTIL1_strcat(commandBuffer, commandBufferSize, (unsigned char*)"write 0 ");
	UTIL1_strcatNum32u(commandBuffer, commandBufferSize, dataSize);
	UTIL1_strcat(commandBuffer, commandBufferSize, (unsigned char*)"\r\n");
	AS_SendString(commandBuffer, commandBufferSize);
	AS_SendString(sData, dataSize+1);
}

void AS_SendReplyPage(unsigned char* replyCode, unsigned char* replyContent)
{
	buf[0]='\0';
	int maxReplySize = 1440;
	UTIL1_strcat(buf, maxReplySize, (unsigned char*)"HTTP/1.1 ");
	UTIL1_strcat(buf, maxReplySize, replyCode);
	UTIL1_strcat(buf, maxReplySize, (unsigned char*)"\r\nContent-Type:text/json\r\nAccess-Control-Allow-Origin: *\r\nPragma:no-cache\r\n\r\n");
	UTIL1_strcat(buf, maxReplySize, replyContent);
	AS_SendDataString(buf);
}

//WARNING! Will only check the format string for sending the stream mode sentinels. (usually '$$$' and 'exit')
void AS_printf(unsigned char* fmt, ...)
{

	CDC1_SendChar('>');
	va_list arglist;

	va_start( arglist, fmt );
	int response = csprintf_va(&AS_SendChar, fmt, arglist);
	va_end( arglist );

	if(streamMode == FALSE && UTIL1_strncmp(fmt, (unsigned char*)STREAMENTER, 4) == 0)
	{
		streamMode = TRUE;
	}
	else if(streamMode == TRUE && UTIL1_strncmp(fmt, (unsigned char*)STREAMEXIT, 3) == 0)
	{
		streamMode = FALSE;
	}
}
