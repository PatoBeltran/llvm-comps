
/*
   FILE: tree.int
  
   Copyright (c) 1997 Princeton University

   All rights reserved.

   This software is to be used for non-commercial purposes only,
   unless authorized permission to do otherwise is obtained.  
   For more information, contact spam@ee.princeton.edu
*/

#define MAX_KIDS 20
typedef struct tree *Tree;
struct tree {		/* tree patterns: */
	void *op;		/* a term or non-terminal */
	int nkids;
	Tree kids[MAX_KIDS];
};

DECLARE_X_ARRAY_0(Tree_list,Tree)

extern Tree tree(char *op, Tree_list *kids);
