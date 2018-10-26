/*
 * File:   StringBuffer.h
 * Author: Triston J. Taylor (C) 2012
 *
 * This work is released under the "Artistic License" you should have
 * received a copy of the license with this file. If you have not, you may
 * wish to consult your source provider or direct your browser to the
 * the following link: http://dev.perl.org/licenses/artistic.html
 *
 * Author claims zero warranty or liability for losses or damages incurred
 * by the use of this work.
 *
 * Created on April 22, 2012, 12:10 AM
 */

#ifndef STRINGBUFFER_H
#define	STRINGBUFFER_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef _STDDEF_H
#include <stddef.h>
#endif

typedef struct StringBuffer {

   char * address;
   size_t index;
   size_t allocated;
   size_t limit;

} StringBuffer;

/* It will never matter what these are set to as long as we can test them */

#ifndef SEEK_SET
    #define SEEK_SET	0	/* Seek from beginning of file.  */
    #define SEEK_CUR	1	/* Seek from current position.  */
    #define SEEK_END	2	/* Seek from end of file.  */
#endif

/* Fill a new StringBuffer declaration with zeroes */
/* Example: StringBuffer thing = NewStringBuffer(); */
extern StringBuffer NewStringBuffer(void);

/* Initialize a new StringBuffer with memory and an optional allocation
 * governor.
 *
 * Example: StringBufferInit(&thing, 1024, 4096);
 * The above initializes a 1K buffer with a 4K size restriction.
 *                                                                        */
extern int StringBufferInit(StringBuffer* b, size_t allocate, size_t limit);

/* Write a zero terminated source to the location specified by
 * StringBuffer.index.
 * If the buffer is too small it will grow to a proper length, provided
 * that the system returns a valid memory handle from realloc, and that
 * the governor restrictions on this buffer will not be exceeded.
 * StringBuffer.address+StringBuffer.index will point to the zero
 * terminator written. Return value is positive success.
 *                                                                        */
extern int StringBufferWrite(StringBuffer * buffer, const char * src);

// Erase StringBuffer.index..extent or StringBuffer.allocated
extern void StringBufferClear(StringBuffer * buffer, size_t extent);

// Free the underlying memory block indicated by StringBuffer.address and
// reset all buffer fields.
extern void StringBufferRelease(StringBuffer * buffer);

// Perform a seek operation on the buffer's index member fseek style
// see fseek documentation for more clarity on this operation.
// return value is the char * address located at address+index,
// or zero if the call does not succeed.
extern char * StringBufferSeek(StringBuffer * b, size_t i, int o);

// Capture StringBuffer Image
// Example: StringBuffer thing = StringBuffer(&that);
extern StringBuffer StringBufferProperties(StringBuffer *buffer);

// Duplicate a StringBuffer
// Example: StringBuffer thing = StringBufferDuplicate(&that);
extern StringBuffer StringBufferDuplicate(StringBuffer *buffer);

// Directly set the size of a new or existing StringBuffer with respect to
// its governor value.
extern int StringBufferExtent(StringBuffer *buffer, size_t size);

#ifdef	__cplusplus
}
#endif

#endif	/* STRINGBUFFER_H */

