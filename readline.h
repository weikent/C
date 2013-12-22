// -- C++ --

/* @(#)readline.h
 */

#ifndef _READLINE_H
#define _READLINE_H 1

/**********************************************************/
/* read_line: Skips leading white-space characters, then  */
/*            reads the remainder of the input line and	  */
/*            stores it in str. Truncates the line if its */
/*            length exceed n. Returns the number of 	  */
/*            characters stored.			  */
/**********************************************************/
int read_line(char str[], int n);
#endif /* _READLINE_H */

