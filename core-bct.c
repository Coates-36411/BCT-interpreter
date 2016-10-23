#include "core-bct.h"

bct* newbct(char val, bct* left, bct* right) {
	bct* b = malloc(sizeof(bct));

	if(b == NULL) {
                fprintf(stderr, "[!]: Allocating data string member failed!\n");
                return NULL;
        }

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
