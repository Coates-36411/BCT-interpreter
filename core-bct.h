#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma once

#ifndef CORE_BCT_H
#define CORE_BCT_H

typedef struct bct {
	char val;	// '1' or '0'
	struct bct* r;	// Next part of string (on the right)
	struct bct* l;	// Last part of string (on the left)
} bct;

bct* newbct(char, bct*, bct*);          // Creates a new bct queue element. Left and right arguments should be left as NULL if this is the first ('root') element.
void delbct(bct*);                      // Deletes a bct
bct* leftbct(bct*);                     // Gets the leftmost element in a queue (the argument must be a member of the queue)
bct* rightbct(bct*);                    // Gets the rightmost element in a queue (the argument must be a member of the queue)
int countbct(bct*);                     // Counts the amount of elements in the queue, and returns the value (argument must be member of queue)
void dumpbct(bct*, int);                // Goes along the queue of which the first argument is a part of, and prints out the value of each element. The second argument is the amount of spaces printed before the line (leave this at zero if you want, but it can make the formatting look much better)

#endif /* end of include guard: CORE_BCT_H */
