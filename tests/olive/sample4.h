#ifndef __OLIVE_HEADER_INCLUDED__
#define __OLIVE_HEADER_INCLUDED__
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <string>
/*
   FILE: sample4.brg
  
   Copyright (c) 1997 Princeton University

   All rights reserved.

   This software is to be used for non-commercial purposes only,
   unless authorized permission to do otherwise is obtained.  
   For more information, contact spam@ee.princeton.edu
*/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
enum {
	ADDI=309, ADDRLP=295, ASGNI=53,
	CNSTI=21, CVCI=85, I0I=661, INDIRC=67
};

typedef struct tree {
	int op;
	struct tree *kids[2];
	int val;
	struct { struct burm_state *state; } x;
} *NODEPTR, *Tree;
#define GET_KIDS(p)	((p)->kids)
#define PANIC printf
#define STATE_LABEL(p) ((p)->x.state)
#define SET_STATE(p,s) (p)->x.state=(s)
#define DEFAULT_COST	break
#define NO_ACTION(x)

typedef struct COST {
    int cost;
} COST;
#define COST_LESS(a,b) ((a).cost < (b).cost)

static COST INFINITY = { 32767 };
static COST ZERO     = { 0 };

/*static char* burm_string = "FOO";*/
static int _ern = 0;

static int shouldTrace = 0;
static int shouldCover = 0;

int OP_LABEL(NODEPTR p) {
	switch (p->op) {
	case CNSTI:  if (p->val == 0) return 661 /* I0I */;
	default:     return p->op;
	}
}

static void burm_trace(NODEPTR, int, COST);

#define BURP 0
#define ADDI 1
#define ADDRLP 2
#define ASGNI 3
#define CNSTI 4
#define CVCI 5
#define I0I 6
#define INDIRC 7

struct burm_state {
  int op;
  NODEPTR node;
  struct burm_state **kids;
  COST cost[7];
  struct {
    unsigned burm_stmt:2;
    unsigned burm_reg:3;
    unsigned burm_disp:2;
    unsigned burm_rc:2;
    unsigned burm_con:2;
    unsigned burm__:1;
  } rule;
};


struct burm_state *burm_label(NODEPTR);
struct burm_state *burm_label1(NODEPTR);

void dumpCover(NODEPTR,int,int);

#endif
