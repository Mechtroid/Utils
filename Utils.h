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
#include "ff.h"

#define IsUpper(c)	(((c)>='A')&&((c)<='Z'))
#define IsLower(c)	(((c)>='a')&&((c)<='z'))
#define IsDigit(c)	(((c)>='0')&&((c)<='9'))
#define IntAbs(c)   ((c<0) ? -c : c)
#define DBL_EPSILON (2.220446e-16)
#define DEFAULT_FRACTIONAL_PRECISION (5)
#define ROUND_MAGNITUDE (2)

unsigned char* fss(unsigned char* src, unsigned char split);
void strnCopy(uint8_t *dst, size_t dstSize, const unsigned char *src);
unsigned int strSplit(unsigned char*, size_t, unsigned char*, size_t, unsigned char*, unsigned char* );
int sToNum(long int*, unsigned char*);
int sToFloat(float*, unsigned char*);
int sToDouble(double*, unsigned char*);
void decomposeFloat(double , unsigned char* , uint16_t* , uint16_t* );
int strzMatch(unsigned char* string, unsigned char* substring);
int csprintf(unsigned char (*)(unsigned char), const unsigned char*, ...	);
int csprintf_va(unsigned char (*)(unsigned char), const unsigned char*, va_list	);

FRESULT scan_files(char*, bool);
#endif /* UTILS_H_ */
