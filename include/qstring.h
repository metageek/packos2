#ifndef _QSTRING_H_
#define _QSTRING_H_

/* Bits of the standard <string.h>, and others, implemented as I need them.
 */

int atoi(const char* s);

// Not thread-safe.
const char* itoa(int i);

int strcmp(const char* s, const char* t);

#endif /* _QSTRING_H_ */
