/*
 * Utils.c
 *
 *  Created on: Dec 12, 2014
 *      Author: Engineeer
 */
#include "Utils.h"
#include <ctype.h>

#ifndef _USE_STRFUNC
#define _USE_STRFUNC 1
#endif


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
	else
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
void fssUndo(unsigned char* leftSide, unsigned char* rightSide, unsigned char split)
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
  *dst = '\0';
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

/* modify lNum to equal the base 10 number encoded in str, if possible.
 * If decoding fails, returns default.
 * returns lNum.
 */
long int sToNum(long int* lNum, unsigned char* str)
{
	unsigned char** spTemp = &str;
	long int lTemp = strtol(str, (char**)spTemp, 0);
	if(**spTemp == '\0' || isspace(**spTemp))
	{
		if(lNum != NULL)
		{
			*lNum = lTemp;
		}
	}
	return (lNum != NULL)? *lNum: lTemp;

}



typedef struct //simple char buffer for the cprintf function.
{
	unsigned char (*sendChar)(unsigned char);
	int idx, nchr;
	uint8_t buf[64];
} putbuff;

static void putc_bfd 	(  				//Helper function for csprintf, below.
						putbuff* pb,
						char c
						)
{
	unsigned int bw;
	int i;


	if (_USE_STRFUNC == 2 && c == '\n')	 /* LF -> CRLF conversion */
		putc_bfd(pb, '\r');

	i = pb->idx;	/* Buffer write index (-1:error) */
	if (i < 0) return;

#if _USE_LFN && _LFN_UNICODE
#if _STRF_ENCODE == 3			/* Write a character in UTF-8 */
	if (c < 0x80) {				/* 7-bit */
		pb->buf[i++] = (uint8_t)c;
	} else {
		if (c < 0x800) {		/* 11-bit */
			pb->buf[i++] = (uint8_t)(0xC0 | c >> 6);
		} else {				/* 16-bit */
			pb->buf[i++] = (uint8_t)(0xE0 | c >> 12);
			pb->buf[i++] = (uint8_t)(0x80 | (c >> 6 & 0x3F));
		}
		pb->buf[i++] = (uint8_t)(0x80 | (c & 0x3F));
	}
#elif _STRF_ENCODE == 2			/* Write a character in UTF-16BE */
	pb->buf[i++] = (uint8_t)(c >> 8);
	pb->buf[i++] = (uint8_t)c;
#elif _STRF_ENCODE == 1			/* Write a character in UTF-16LE */
	pb->buf[i++] = (uint8_t)c;
	pb->buf[i++] = (uint8_t)(c >> 8);
#else							/* Write a character in ANSI/OEM */
	c = ff_convert(c, 0);	/* Unicode -> OEM */
	if (!c) c = '?';
	if (c >= 0x100)
		pb->buf[i++] = (uint8_t)(c >> 8);
	pb->buf[i++] = (uint8_t)c;
#endif
#else							/* Write a character without conversion */
	pb->buf[i++] = (uint8_t)c;
#endif

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
 * Shamelessly modified from FATfs's printf function.*/
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
	unsigned int nw, i, j, w;
	uint32_t v;
	char c, d, s[16], *p;
	putbuff pb;

	pb.sendChar = sendChar;
	pb.nchr = pb.idx = 0; /* Initialize output buffer */

	for (;;) {
		c = *fmt++;
		if (c == 0) break;			/* End of string */
		if (c != '%') {				/* Non escape character */
			putc_bfd(&pb, c);
			continue;
		}
		w = f = 0;
		c = *fmt++;
		if (c == '0') {				/* Flag: '0' padding */
			f = 1; c = *fmt++;
		} else {
			if (c == '-') {			/* Flag: left justified */
				f = 2; c = *fmt++;
			}
		}
		while (IsDigit(c)) {		/* Precision */
			w = w * 10 + c - '0';
			c = *fmt++;
		}
		if (c == 'l' || c == 'L') {	/* Prefix: Size is long int */
			f |= 4; c = *fmt++;
		}
		if (!c) break; 				/* Handle trailing escape characters.*/
		d = c;
		if (IsLower(d)) d -= 0x20;
		switch (d) {				/* Type is... */
		case 'S' :					/* String */
			p = va_arg(arp, char*);
			for (j = 0; p[j]; j++) ;
			if (!(f & 2)) {
				while (j++ < w) putc_bfd(&pb, ' ');
			}
			while (*p) putc_bfd(&pb, *p++);
			while (j++ < w) putc_bfd(&pb, ' ');
			continue;
		case 'C' :					/* Character */
			putc_bfd(&pb, (char)va_arg(arp, int)); continue;
		case 'B' :					/* Binary */
			r = 2; break;
		case 'O' :					/* Octal */
			r = 8; break;
		case 'D' :					/* Signed decimal */
		case 'U' :					/* Unsigned decimal */
			r = 10; break;
		case 'X' :					/* Hexdecimal */
			r = 16; break;
		default:					/* Unknown type (pass-through) */
			putc_bfd(&pb, c); continue;
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
		} while (v && i < sizeof s / sizeof s[0]);
		if (f & 8) s[i++] = '-';
		j = i; d = (f & 1) ? '0' : ' ';
		while (!(f & 2) && j++ < w) putc_bfd(&pb, d);
		do putc_bfd(&pb, s[--i]); while (i);
		while (j++ < w) putc_bfd(&pb, d);
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
//(This function requires fatfs

/* Lists all files in a directory and its subdirectories.
 * Example from http://elm-chan.org/fsw/ff/en/readdir.html
 */
FRESULT scan_files (char* path)
/*path: Start node to be scanned (also used as work area) */
{
	FRESULT res;
	FILINFO fno;
	DIR dir;
	int i;
	char *fn;   /* This function assumes non-Unicode configuration */
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
			if (fno.fname[0] == '.') continue;             /* Ignore dot entry */
#if _USE_LFN
			fn = *fno.lfname ? fno.lfname : fno.fname;
#else
			fn = fno.fname;
#endif
			if (fno.fattrib & AM_DIR) {                    /* It is a directory */
				sprintf(&path[i], "/%s", fn);
				res = scan_files(path);
				path[i] = 0;
				if (res != FR_OK) break;
			} else {                                       /* It is a file. */
				AS_printf("%s/%s\n",path, fn);
			}
		}
		f_closedir(&dir);
	}

	return res;
}

