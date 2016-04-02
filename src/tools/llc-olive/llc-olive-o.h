#ifndef __OLIVE_HEADER_INCLUDED__
#define __OLIVE_HEADER_INCLUDED__
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <set>
#include <climits>

#include "llvm/Analysis/LoopInfo.h"

#include "llvm/IR/Instructions.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Constants.h"

#include "llvm/IR/Value.h"

#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Casting.h"

/** ================== Tree ==================== **/

class Node; // forward declaration
typedef Node *NODEPTR;

struct burm_state;
typedef burm_state *StatePtr;

class Node {
	unsigned _op;
	StatePtr _state;  
	const llvm::Value *_value;
	int _num_kids;
	Node **_kids;
public:
	Node (unsigned op, std::vector<Node *> *kids, const llvm::Value *value) 
		: _op(op),
			_state(0),
			_value(value) {
		if ((kids == NULL) || (kids->size() == 0)) {
			_num_kids = 0;
			_kids = NULL;
		}
		else {
			_num_kids = kids->size();
			_kids = new NODEPTR[_num_kids];
			for (int i = 0; i < _num_kids; ++i) 
				_kids[i]=(*kids)[i];
		}
	}
	~Node(){
		delete _kids;
	}
	unsigned getOp() {
		return _op;
	}
	void setOp(int op) {
		_op = op;
	}
	StatePtr getState() {
		return _state;
	}
	void setState(StatePtr state) {
		_state = state;
	}
	const llvm::Value* getValue() {
		return _value;
	}
	int getNumKids() {
		return _num_kids;
	}
	NODEPTR *getKids() {
		return _kids;
	}
};

#define GET_KIDS(p)	((p)->getKids())
#define PANIC printf
#define STATE_LABEL(p) ((p)->getState())
#define SET_STATE(p,s) ((p)->setState(s))
#define DEFAULT_COST	break
#define NO_ACTION(x)

/** ================= Cost =================== **/

class COST {
  int _cost;
public:
	COST(int cost) {
		_cost = cost;
	}
	int getCost() {
		return _cost;
	}
	void setCost(int cost) {
		_cost = cost;
	}
};

#define COST_LESS(a,b) ((a).getCost() < (b).getCost())

static COST COST_INFINITY = COST(32767);
static COST COST_ZERO     = COST(0);

/** ================ Operations ================ **/
enum {
  CONST=0,        //Constant
  ADD=1,          //Addition
  MOV=2,          //Move
  ADDR=3,         //Address
  RET=4
};

/** =========================================== **/

class CodeGenerator {
public:
  static void generateCode(NODEPTR p);
};

int OP_LABEL(NODEPTR node);
void burm_trace(NODEPTR node, int eRuleNo, COST cost);

//You need to pass in the necessary info (e.g. registers, memory locations)
// to the action call when you call it to generate code
#define CONST 0
#define ADD 1
#define MOV 2
#define ADDR 3
#define RET 4

struct burm_state {
  int op;
  NODEPTR node;
  struct burm_state **kids;
  COST cost[5];
  struct {
    unsigned burm_stmt:2;
    unsigned burm_rc:2;
    unsigned burm_mem:1;
    unsigned burm_const:1;
  } rule;
};


struct burm_state *burm_label(NODEPTR);
struct burm_state *burm_label1(NODEPTR);

void dumpCover(NODEPTR,int,int);

#endif
