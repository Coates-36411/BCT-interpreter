/*	Bitwise cyclic tag interpreter in c.
	Coded by Coates, 9-10th July 2016.

	Currently, the program is very inneficient
	as it takes an entire byte to store either
	a '1' or a '0'.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bct {
	char val;	// '1' or '0'
	struct bct* r;	// Next part of string (on the right)
	struct bct* l;	// Last part of string (on the left)
} bct;

bct* newbct(char val, bct* left, bct* right) {
	bct* b = malloc(sizeof(bct));

	if(b == NULL) fprintf(stderr, "[!]: Allocating data string member failed!\n");

	b -> val = val;
	b -> r = right;
	b -> l = left;

	if(b -> l != NULL) b -> l -> r = b;
	if(b -> r != NULL) b -> r -> l = b;
	return b;
}

void delbct(bct* b) {
	bct* r = b -> r;
	bct* l = b -> l;

	if(r != NULL && l != NULL) {		// If deleted part was in the middle of others
		r -> l = l;
		l -> r = r;
	} else if(r != NULL && l == NULL) {	// If it was the first one
		r -> l = NULL;
	} else if(r == NULL && l != NULL) {	// If it was the last
		l -> r = NULL;
	} else {}				// Do we need to do anything?

	free(b);
}

bct* leftbct(bct* b) {
	bct* e;
	for(e = b; e -> l != NULL; e = e -> l);	// Move e to the leftmost one
	return e;
}

bct* rightbct(bct* b) {
	bct* e;
	for(e = b; e -> r != NULL; e = e -> r);
	return e;
}

int countbct(bct* b) {
	int i;
	bct* e;
	for(e = leftbct(b), i = 0; e -> r != NULL; e = e -> r, i++);
	i++;
	return i;
}


void dumpbct(bct* b, int spc) {			// BCT element in the queue, number of spaces
	bct* t;
	int i;
	for(i = 0; i < spc; i++) {		// Pad it out with spaces
		printf(" ");
	}

	for(t = leftbct(b); t != NULL; t = t -> r) {
		printf("%c", t -> val);
	}
	printf("\n");
}

int isValid(char* s) {
/*	int i;
	for(i = 0; i <= strlen(s); i++)
		if(s[i] != '0' || s[i] != '1' || s[i] != '\0' || s[i] != '\n') return 0;	// False
	need to redo this */
	return 1;
}

#define BLOCKSZ 256

int main(int argc, char** argv) {
	int c, i, sz = BLOCKSZ;			// c is an int so that it can pick up on EOF
	char* p, ra = 0;			// Program memory, is root assigned?
	bct* r, *l, *t;				// Root, last and temp bcts

	printf("Welcome to the C implementation of a BCT interpreter!\n");
	printf("Enter the initial data string:\t");

	/* This loop gets the initial state of the data string *
	 * it definitely works. Note that we don't store the   *
	 * input in any array as it is immediately turned into *
	 * the data string instead.			       */
	while((c = getc(stdin)) != '\n' && c != '\0' && c != '\r' && c != EOF) {
		if(c == '1' || c == '0') {		// Only adds correct characters
			if(!ra) {			// If we have not yet made the root
				r = newbct(c, NULL, NULL);
				t = l = r;		// Set up root
				ra = 1;
			} else {			// We have already made root. Do this stuff now.
				t = newbct(c, l, NULL);	// t is a pointer to the current bct (on the right)
				l = l -> r;
			}
		}
	}


	if(ra == 0) {
		printf("No initial data string entered. Setting the root value to one.\n");
		r = newbct(c, NULL, NULL);
		t = l = r;
		ra = 1;
	}

	printf("Enter the program:\t");
	i = 0;
	p = malloc(sizeof(char) * sz);			// Initial allocation of program

	/* This loop gets the program from standard input.     */
	while((c = getc(stdin)) != '\n' && c != '\0' && c != '\r' && c != EOF) {
		if(i >= sz) {
			sz += BLOCKSZ;			// If we have exceeded the buffer length, increase it.
			p = realloc(p, sz);
		}
		if(c == '1' || c == '0') {
			p[i] = c;			// If it is an accepted character, add it.
			i++;
		}
	}
	p[i] = '\0';

	// Run the program
	char done = 0;				// Done variable. Set to 1 (true) when we are done
	i = 0;					// Reset i so that it can be reused
	int len = strlen(p), spc = 0;		// Optimises this so we don't have to keep computing strlen. Spaces variable allows for nice formatting.

	if(len == 0) {
		done = 1;
		printf("No input program!\n");
	}

	if(!isValid(p)) {
		fprintf(stderr, "[!!]: Incorrect program input! Exiting NOW!\n");
		done = 1;
		exit(-1);
	}

	while(!done) {
		dumpbct(leftbct(r), spc);

		//printf("Count is %d.\n", countbct(r));
		if(countbct(r) == 0) {
			printf("No data left. Exiting...\n");
			done = 1;
			break;			// Don't carry on with the loop
		}

		switch(p[i % len]) {		// Mod makes the program cyclic
			case '\0':		// This shouldn't occur
				i = -1;
				break;
			case '0':
				spc++;		// Increase spaces (we have deleted one of the values, but we still want the rest to line up).
				r = rightbct(r);
				delbct(leftbct(r));
				if(r == NULL) done = 1;
				break;	// Could do with checking this
			case '1':
				i++;
				if(leftbct(r) -> val == '1') r = newbct(p[i % len], rightbct(r), NULL);	// Do I need to assign the newbct? Really?
				break;
			default:	// Should there be an error message?
				fprintf(stderr, "Incorrect symbol '%c' at character number %d.\n", p[i%len], i%len);
				break;
		}
		i++;
	}
	free(p);

	if(r != NULL) {
		r = leftbct(r);				// Move r furthest left
		while(r -> r != NULL) {
			delbct(rightbct(r));		// Delete bcts to the right of r
		}
		delbct(r);
	}

	printf("Done.\n");
	return 0;
}
