#ifndef __OLIVE_HEADER_INCLUDED__
#define __OLIVE_HEADER_INCLUDED__
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <vector>

#include "llvm/Instruction.h"
#include "llvm/Instructions.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Value.h"
#include "codeGenerator.h"

struct burm_state; // forward declaration
typedef burm_state *StatePtr;

class Node; // forward declaration
typedef Node *NODEPTR;

class Node {
	unsigned _op;
	StatePtr _state;
	const llvm::Value *_value;
	unsigned _num_kids;
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
			for (unsigned i = 0; i < _num_kids; ++i) 
				_kids[i]=(*kids)[i];
		}
	}
	~Node(){
		delete _kids;
	}
	unsigned getOp() {
		return _op;
	}
	void setOp(unsigned op) {
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
	unsigned getNumKids() {
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

int OP_LABEL(NODEPTR node);
void burm_trace(NODEPTR node, int eRuleNo, COST cost);
void generateCode(NODEPTR node);

// Note that all llvm::Value have an indent of 2 by default, except constants and arguments
inline void printIndent(unsigned indent);

extern bool TraceFlag;

extern CodeGenerator codeGenerator;

#define BURP 0
#define REG 1
#define ADDR 2
#define CONST 3
#define ARGS 4
#define FUNC 5
#define INVALS 6
#define INDICES 7
#define ALLOCA 8
#define LOAD 9
#define STORE 10
#define GETELEMPTR 11
#define BINARY 12
#define TERMINST 13
#define VOIDTERMINST 14
#define CAST 15
#define CMP 16
#define CALL 17
#define CALLNOPARAM 18
#define PHINODE 19
#define SELECT 20

struct burm_state {
  int op;
  NODEPTR node;
  struct burm_state **kids;
  COST cost[12];
  struct {
    unsigned burm_stmt:3;
    unsigned burm_reg:5;
    unsigned burm_con:1;
    unsigned burm_regORcon:2;
    unsigned burm_addr:3;
    unsigned burm_regORaddr:2;
    unsigned burm_regORconORaddr:2;
    unsigned burm_args:2;
    unsigned burm_func:1;
    unsigned burm_invals:2;
    unsigned burm_indices:2;
  } rule;
};


struct burm_state *burm_label(NODEPTR);
struct burm_state *burm_label1(NODEPTR);

void dumpCover(NODEPTR,int,int);

#endif
