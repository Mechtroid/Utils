/*
 * Utils.c
 *
 *  Created on: Dec 12, 2014
 *      Author: Engineeer
 */
#include "Utils.h"
#include <ctype.h>
#include <math.h>
#include "AString.h"
#include <stdio.h>



//================
//====STRINGS=====
//================


/*\
 * FSS: Fast String Split.  Takes a zero terminated string, <src>, and a split character, <split>.
 * Returns a pointer to the right side of the string past the split character, and <src> will now
 * point to the left side of the string. For a safer string split function that copies data, takes
 * split strings of more than one character, and leaves the original string unmodified, see strSplit.
 *
 * Note: Modifies the original string, advanced users can use fssUndo to restore the original string
 * after use. For more info, see comments directly after this function.
\*/
unsigned char* fss(unsigned char* src, unsigned char split)
{
	int splitLoc = 0; //location of the first instance of the split character.
	while(src[splitLoc] != split && src[splitLoc] != '\0')
	{
		splitLoc++;
	}

	if(src[splitLoc] == split) //found it, write null term, then return pointer after term
	{
		src[splitLoc] = '\0';
		return (src+splitLoc+1);
	}
	else	//didn't find it, return pointer to end of string.
	{
		return (src+splitLoc);
	}
}
/*\
 * fss takes a string and searches for the split character. Once fss finds it, the split character is
 * replaced with a null termination character, and a pointer to the character AFTER the split is
 * returned.  So if "Hello World!\0", the function will return a pointer to the 'W' in world, and the
 * string passed in will look like "Hello\0World!\0".  If the split char is NOT found, the string passed
 * in will be unmodified, and the function will just return a pointer to the '\0' at the end of the string.
\*/

/*\
 * FSP: Fast String Peel:  As fss above, except it returns a pointer to the part of the string before
 * the split character, instead of after.  The source pointer will be modified to point to the characters
 * after the split,
\*/
unsigned char* fsp(unsigned char** src, unsigned char split)
{
	//TODO: implement, dummy.
}

/*\
 * FSSUndo: Fast String Split UNDO: removes the null terminator inserted into a string by fss.
 * NOTE: Might fail if leftSide has been modified.
 * WARNING: Passing in bad pointers has the possibility of writing out of bounds to bad memory.
\*/
void fsUndo(unsigned char* leftSide, unsigned char* rightSide, unsigned char split)
{
	if(rightSide > leftSide && rightSide[0] != '\0')
	{
		if(rightSide-1)
		*(rightSide-1) = split;

	}
	else				//split char wasn't found
	{
		return;
	}
}



/* Copies a zero terminated string into another string and zero terminates it.
 * dstSize MUST account for the null character or truncation will occur.
 * (dstSize of 4 will only copy 3 characters into dst before null terminating.)
\*/
void strnCopy(uint8_t *dst, size_t dstSize, const unsigned char *src)
{
  while (dstSize > 1 && *src != '\0')
  {
    *dst++ = *src++;
    dstSize--;
  }
  if(dstSize > 0)
  {
	  *dst = '\0';
  }
}

/* Splits the string src into two halves at the first instance of the sentinel string/char.
 * The left and right sides of the src will be copied into the arguments left and right,
 * minus the sentinel value.
 * Returns the index of the first character after the sentinel if it is found, 0 otherwise.
 * (This is so the response easily maps into TRUE/FALSE)
 *
 * If either string argument is NULL, that string will not be modified.
 * If the left or right side of src is bigger than the size of left[] or right[], the result will be truncated.
 * Left[] and right[] will always be null terminated, but the remainder of the array will not be zeroed out.
 * If sentinel is not found, left[] will be filled with src, and the first character of right[] will be \0.
 */
unsigned int strSplit(unsigned char* left, size_t leftSize, unsigned char* right, size_t rightSize, unsigned char* src, unsigned char* sentinel)
{
	unsigned int uOffset = (unsigned int)(UTIL1_strFind(src, sentinel)+1);  //first element after the sentinel character.

	leftSize = (uOffset < leftSize && uOffset > 0) ? uOffset : leftSize; //don't copy over the sentinel or any characters after it.
	if(left != NULL)
	{
		strnCopy(left, leftSize, src);
	}

	rightSize = (uOffset == 0) ? 0 : rightSize; //if sentinel isn't found, just clear right.
	if(right != NULL)
	{
		strnCopy(right, rightSize, src+uOffset);
	}
	return uOffset;
}

/*\
 * STRing (Zero terminated) MATCH: Returns the location of a substring in a larger string, if found.
 * If the substring is not found, return -1.
 * NOTE: crudely case insensitive.
\*/
int strzMatch(unsigned char* string, unsigned char* substring)
{
	unsigned char *startPos = string, *currentPos, *currentSub;
	while(*startPos != '\0')
	{
		currentPos = startPos;
		currentSub = substring;
		while(*currentPos != '\0' && *currentSub != '\0')
		{
			int diff = ((int)toupper(*currentPos) - (int)toupper(*currentSub));
			if(diff == 0 )  //gotta be some sweet hexadecimal magic to make diff = 0 when it's 0, -32, or 32.
			{
				currentSub++;	//Match found, continue
				currentPos++;
			}
			else
			{
				int i = 5;
				break;
			}
		}
		if(*currentSub == '\0')
		{
			return startPos - string;
		}
		else
		{
			startPos++;
		}
	}
	return -1;
}

/* modify lNum to equal the base 10 number encoded in str, if possible.
 * If decoding fails, returns 0. Otherwise returns 1.
 * returns lNum.
 */
int sToNum(long int* lNum, unsigned char* str)
{
	if(*str == '\0' || isspace(*str))
	{
		return FALSE;
	}
	unsigned char** spTemp = &str;
	long int lTemp = strtol(str, (char**)spTemp, 0);
	if(**spTemp == '\0' || isspace(**spTemp))
	{
		if(lNum != NULL)
		{
			*lNum = lTemp;
			return TRUE;
		}
	}
	return FALSE;

}

int sToFloat(float* pFloat, unsigned char* str)
{
	if(*str == '\0' || isspace(*str) || pFloat == NULL)
	{
		return FALSE;
	}
	unsigned char** spTemp = &str;
	float dTemp = strtof(str, (char**)spTemp);
	if(**spTemp == '\0' || isspace(**spTemp))
	{
		*pFloat = dTemp;
		return TRUE;
	}
	return FALSE;
}

int sToDouble(double* pFloat, unsigned char* str)
{
	if(*str == '\0' || isspace(*str) || pFloat == NULL)
	{
		return FALSE;
	}
	unsigned char** spTemp = &str;
	double dTemp = strtod(str, (char**)spTemp);
	if(**spTemp == '\0' || isspace(**spTemp))
	{
		*pFloat = dTemp;
		return TRUE;
	}
	return FALSE;
}


//Typical usage: AS_printf("%c%02u.%03u", floatMinus, floatOnes, floatMillis);
void decomposeFloat(double dFloat, unsigned char* floatMinus, uint16_t* floatOnes, uint16_t* floatMillis)
{
	*floatMinus = ' ';
	if(dFloat < 0)
	{
		*floatMinus = '-';
		dFloat = -dFloat;
	}
	*floatOnes = (uint16_t) dFloat;
	*floatMillis = (uint16_t) ((dFloat - *floatOnes) * 1000);

}

typedef struct //simple char buffer for the cprintf function.
{
	unsigned char (*sendChar)(unsigned char);
	int idx, nchr;
	uint8_t buf[64];
} pbuff;

static void putc_bfrd 	(  				//Helper function for csprintf, below.
						pbuff* pb,
						char c
						)
{
	unsigned int bw;
	int i;

	i = pb->idx;	/* Buffer write index (-1:error) */
	if (i < 0) return;
						/* Write a character without conversion */
	pb->buf[i++] = (uint8_t)c;

	if (i >= (int)(sizeof pb->buf) - 3) {	/* Send buffered characters */
		bw = 0;
		while(bw < i) pb->sendChar(pb->buf[bw++]);
		i = (bw == (unsigned int)i) ? 0 : -1;
	}
	pb->idx = i;
	pb->nchr++;
}

/* CPRINTF: _C_haracter _S_treaming implementation of _PRINTF_
 * This takes a string with character formatting and streams it character by character using the function given.
 * Accepted format tokens: %k,c,s;b,o,d,u,x,X;f; !!!Note: ALWAYS CAST FLOATING POINT ARGUMENTS TO DOUBLE!!!
 * Token format: '%' ['-'|'0'] [<len>['.'<flen>]] ['l'|'L'] <type>
 * Shamelessly modified from FATfs's printf function.
 *
 * Format string types:
 * %k/K: skip  (for dynamically generated format strings using fixed argument lists)
 * %c/C: single character
 * %s/S: null-terminated string
 * %b/B: binary
 * %o/O: octal
 * %d/D: integer
 * %u/U: unsigned integer
 * %x:	 hexidecimal (lowercase)
 * %X:   Hexidecimal (Uppercase)
 * %f/F: Fractional precision, soft. (Will truncate trailing 0s after rounding).   (ENSURE ARGUMENT FOR THIS TOKEN IS CAST TO A DOUBLE, NOT A FLOAT)
 *
 * Token prefix format: '%' ['-'|'0'] [<len>[.<flen>]] ['l'/'L'] <token>
 * - 		Left justify
 * 0		Pad right with zeroes
 * <len>    Minimum Length/integer precision (strings/numbers)
 * .<flen>	Override default fractional length (0 = maximum precision, otherwise <flen>, default 5)
 * l/L		Read long (unsigned)int or full precision fractional number.  (WHICH IS A DOUBLE, NOT A FLOAT)
 *
 * !!!ALL ARGUMENTS FOR FRACTIONAL TOKENS WILL PRINT INCORRECTLY IF NOT CAST TO A DOUBLE!!!
 */
int csprintf(
			unsigned char (*sendChar)(unsigned char),	/*pointer to a function that takes a character and either prints it, sends it, or stores it.*/
			const unsigned char* fmt,					/*Formatted string to send*/
			...											/*Optional arguments*/
			)
{
	va_list arglist;
	va_start( arglist, fmt );
	int response = csprintf_va(sendChar, fmt, arglist );
	va_end( arglist );
}

int csprintf_va	(											/*returns number of characters written.*/
				unsigned char (*sendChar)(unsigned char),	/*pointer to a function that takes a character and either prints it, sends it, or stores it.*/
				const unsigned char* fmt,					/*Formatted string to send*/
				va_list arp									/*Optional arguments*/
				)
{
	uint8_t f, r;
	unsigned  int n, nw, i, j, k, l, w, fw;
	uint32_t v;
	char c, d, s[19], sf[17], *p;
	size_t countof_s = sizeof s / sizeof s[0];
	size_t countof_sf = sizeof sf / sizeof s[0];
	double a, b, u;
	pbuff pb;

	pb.sendChar = sendChar;
	pb.nchr = pb.idx = 0; /* Initialize output buffer */

	for (;;) {
		c = *fmt++;
		if (c == '\0') break;			/* End of string */
		if (c != '%') {				/* Non escape character */
			putc_bfrd(&pb, c);
			continue;
		}
		f = 0; //Format flags
		w = 0; //integer precision
		fw = DEFAULT_FRACTIONAL_PRECISION;
		c = *fmt++;
		if (c == '0') {				/* Flag: '0' padding */
			f = 1; c = *fmt++;
		} else if (c == '-') {			/* Flag: left justified */
		  f = 2; c = *fmt++;
		}
		while (IsDigit(c)) {		/* Integer Precision */
			w = w * 10 + c - '0';
			c = *fmt++;
		}
		if (c == '.') {				/* Floating point fractional digits precision */
			c = *fmt++;
			fw = 0;	//default to full precision.
			while (IsDigit(c)) {
				fw = fw * 10 + c - '0';
				c = *fmt++;
			}
		}

		if (c == 'l' || c == 'L') {	/* Prefix: Size is long int */
			f |= 4; c = *fmt++;
		}
		if (!c) break; 				/* Handle trailing escape characters.*/
		d = c;
		if (IsLower(d)) d -= 0x20;
		switch (d) {				/* Type is... */
		case 'K' :					/* Skipped */
			continue;
		case 'S' :					/* String */
			p = va_arg(arp, char*);
			for (j = 0; p[j]; j++) ;
			if (!(f & 2)) {
				while (j++ < w) putc_bfrd(&pb, ' ');
			}
			while (*p) putc_bfrd(&pb, *p++);
			while (j++ < w) putc_bfrd(&pb, ' ');
			continue;
		case 'C' :					/* Character */
			putc_bfrd(&pb, (char)va_arg(arp, int)); continue;
		case 'B' :					/* Binary */
			r = 2; break;
		case 'O' :					/* Octal */
			r = 8; break;
		case 'D' :					/* Signed decimal */
		case 'U' :					/* Unsigned decimal */
			r = 10; break;
		case 'X' :					/* Hexdecimal */
			r = 16; break;
		case 'F' :					/* Floating Point */
			r = 10;					/* (always base 10 (but could theoretically handle other bases))*/
			b = (double)va_arg(arp, double);

			if(b < 0) {
				b = -b;
				f |= 8;
			}

			u = modf(b,&a);
			i = k = l = 0;
			j = (f & 4);

			if(u > 0) {
				fw = (fw && fw < countof_sf) ? fw : countof_sf;		/* Make fw maximum precision if undefined*/
				do {												/* Fill sf with number after decimal place*/
					u = u * r;										/* Shift decimal point right*/
					d = (char)u; u = u - d;							/* Peel away digit to the left of decimal point*/
					sf[i++] = d;									/* Store*/
					l = d ? 0 : l + 1;								/* Keep running total of consecutive 0s*/
					k = (d == r-1) ? k + 1 : 0; 					/* Keep running total of consecutive 9s*/
				} while (u > DBL_EPSILON && i < fw && (j || (k < ROUND_MAGNITUDE && l < ROUND_MAGNITUDE)));
				nw = i;
				k = (2*u > 1) ? 1 : 0;				/* Use k as carry flag to round up*/
				while(i--) {
					if (k) {
						if (sf[i] == r-1) {
							sf[i] = 0xFF;			/* Turn consecutive 9s into trailing zeros*/
						} else {
							sf[i]++;
							k = 0;
						}
					} else if (l) {
						if(sf[i]== 0) {
							sf[i] = 0xFF;			/* Turn consecutive 0s into trailing zeroes*/
						}
						else l = 0;
					}
				}

				i = 0;								/* Reset i for future use*/
				if(sf[0] != 0xFF)					/* Add decimal place as long as sf is not entirely trailing zeroes*/
					s[i++] = '.';
			}
			else {
				fw = nw = 0;
			}

			u = b + k;
			do {									/* Generate integer part directly as string, backwards*/
				d = (char)(fmod(u,r)); u /= r;
				if (d > 9) d += 7 + 0x20 * IsLower(c);	/* Dumb bit logic to convert from number to ASCII*/
				s[i++] = d + '0';
			} while (u >= 1 && i < countof_s);
			if (f & 8) s[i++] = '-';
			j = i; d = (f & 1) ? '0' : ' ';
			while (!(f & 2) && j++ < w+1) putc_bfrd(&pb, d);	/* Left pad*/
			do putc_bfrd(&pb, s[--i]); while (i); i = 0;		/* Send integer part*/
			while (i < nw) {									/* Send fractional part*/
				k = sf[i++];
				if (k == 0xFF && (f & 1)) k = 0;
				if (k != 0xFF) {
					if (k > 9) k += 7 + 0x20 * IsLower(c);
					j++;
					putc_bfrd(&pb,k + '0');
				}
			}
			w+=fw;
			while (j++ < w) putc_bfrd(&pb, d);					/* Right pad*/

			continue;
		default:					/* Unknown type (pass-through) */
			putc_bfrd(&pb, c); continue;
		}

		/* Get an argument and put it in numeral */
		v = (f & 4) ? (uint32_t)va_arg(arp, long) : ((d == 'D') ? (uint32_t)(long)va_arg(arp, int) : (uint32_t)va_arg(arp, unsigned int));
		if (d == 'D' && (v & 0x80000000)) {
			v = 0 - v;
			f |= 8;
		}
		i = 0;
		do {
			d = (char)(v % r); v /= r;
			if (d > 9) d += (c == 'x') ? 0x27 : 0x07;
			s[i++] = d + '0';
		} while (v && i < countof_s);
		if (f & 8) s[i++] = '-';
		j = i; d = (f & 1) ? '0' : ' ';
		while (!(f & 2) && j++ < w) putc_bfrd(&pb, d);
		do putc_bfrd(&pb, s[--i]); while (i);
		while (j++ < w) putc_bfrd(&pb, d);
	}
	if(pb.idx >= 0)	/* Flush buffered characters to the file */
	{
		nw = 0;
		while(nw < pb.idx) pb.sendChar(pb.buf[nw++]);
		return pb.nchr;
	}
}

//================
//====FILE SYS====
//================

/* Lists all files in a directory and its subdirectories.
 * Example from http://elm-chan.org/fsw/ff/en/readdir.html
 */
FRESULT scan_files (char* path, bool hidSys)
/*path: Start node to be scanned (also used as work area) */
{
	FRESULT res;
	FILINFO fno;
	DIR dir;
	int i;
	unsigned char *fn;   /* This function assumes non-Unicode configuration */
#if _USE_LFN
	static char lfn[_MAX_LFN + 1];   /* Buffer to store the LFN */
	fno.lfname = lfn;
	fno.lfsize = sizeof lfn;
#endif


	res = f_opendir(&dir, path);                       /* Open the directory */
	if (res == FR_OK)
	{
		i = strlen(path);
		for (;;)
		{
			res = f_readdir(&dir, &fno);                   /* Read a directory item */
			if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */

			if (fn[0] == '.') continue;                    /* Ignore dot entry */
#if _USE_LFN
			fn = *fno.lfname ? fno.lfname : fno.fname;
#else
			fn = fno.fname;
#endif




			if (fno.fattrib & AM_DIR) {                    /* It is a directory */
				sprintf(&path[i], "/%s", fn);
				res = scan_files(path, hidSys);
				path[i] = 0;
				if (res != FR_OK) break;
			} else {                                       /* It is a file. */
				if(hidSys && (strzMatch(fn,".") == -1)) continue;       /* Skip extensionless files if requested*/
				AS_printf("%s/%s\r\n",path, fn);
			}
		}
		f_closedir(&dir);
	}

	return res;
}
