/*
 * Utils.h
 *
 *  Created on: Dec 12, 2014
 *      Author: Engineeer
 */

#ifndef UTILS_H_
#define UTILS_H_
#include <string.h>
#include <stdlib.h>
#include <STDarg.h>
#include "stddef.h"
#include "UTIL1.h"
#include "FAT1.h"

#define IsUpper(c)	(((c)>='A')&&((c)<='Z'))
#define IsLower(c)	(((c)>='a')&&((c)<='z'))
#define IsDigit(c)	(((c)>='0')&&((c)<='9'))

unsigned char* fss(unsigned char* src, unsigned char split);
void strnCopy(uint8_t *dst, size_t dstSize, const unsigned char *src);
unsigned int strSplit(unsigned char*, size_t, unsigned char*, size_t, unsigned char*, unsigned char* );
long int sToNum(long int*, unsigned char*);
int csprintf(unsigned char (*)(unsigned char), const unsigned char*, ...	);
int csprintf_va(unsigned char (*)(unsigned char), const unsigned char*, va_list	);
FRESULT scan_files(char*);
#endif /* UTILS_H_ */
