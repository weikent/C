/* @(#)word.h
 */

#ifndef _WORD_H
#define _WORD_H 1

/************************************************************/
/* read_word: Reads the next word from the input and 	    */
/*            stores it in word. Makes wordempty if no	    */
/*            word could be read because of end-of-file.    */
/*            Truncates the word if its length exceeds len. */
/************************************************************/

void read_word(char *word, int len);

#endif /* _WORD_H */

