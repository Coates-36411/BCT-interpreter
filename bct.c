/*	Bitwise cyclic tag interpreter in c.
	Coded by Coates, 9-10th July 2016.

	Currently, the program is very inefficient
	as it takes an entire byte to store either
	a '1' or a '0'. This may be handy, however
	if future versions support other values
	(if you can choose between BCT and an
	extended version). Some form of I/O may
	also be added.
*/

#include "core-bct.h"

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
		printf("[!!]: No initial data string entered. Setting the data string to one.\n");
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

	if(len == 0) {				// Does strlen include the null byte or not? This assumes not.
		done = 1;
		printf("No input program!\n");
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
