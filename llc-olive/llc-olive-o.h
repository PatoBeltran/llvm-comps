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
#include <algorithm>

#include "llvm/Analysis/LoopInfo.h"

#include "llvm/IR/Instructions.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Constants.h"

#include "llvm/IR/Value.h"

#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Casting.h"

/** ============= Helpers ===================== **/

class Location {
protected:
  enum { REGISTER, SPILL };
  int _locationType;
  Location(int type) : _locationType(type) { }
public:
  bool isTypeOfClass(int type) { return _locationType == type; }
  virtual std::string getName() { return ""; };
};

class Spill : public Location {
  const unsigned SUPPORTED_OFFSET_DATATYPE = 8; 
  int _offset;
public:
  Spill(int offset) : 
  Location(SPILL),
  _offset(SUPPORTED_OFFSET_DATATYPE*offset) { }
  static bool classof(Location *l) { return l->isTypeOfClass(SPILL); }
  static bool classof(Location l) { return l.isTypeOfClass(SPILL); }
  int getOffset() { return _offset / SUPPORTED_OFFSET_DATATYPE; }
  std::string getName() {
    return "qword ptr [rbp - "+std::to_string(_offset)+"]";
  }
};

class Register : public Location {
  int _type;
public:
  enum {
    RBX=0, RCX=1, RDX=2, RDI=3, RSI=4,
    R8=5, R9=6, R10=7, R11=8, R12=9, R13=10, R14=11, R15=12
  };
  Register(int type) : Location(REGISTER), _type(type) { }

  static bool classof(Location *l) { return l->isTypeOfClass(REGISTER); }
  static bool classof(Location l) { return l.isTypeOfClass(REGISTER); }
  
  int getType() { return _type; };
  std::string getName() {
    switch (_type) {
      case RBX: return "rbx";
      case RCX: return "rcx";
      case RDX: return "rdx";
      case RDI: return "rdi";
      case RSI: return "rsi";
      case R8: return "r8";
      case R9: return "r9";
      case R10: return "r10";
      case R11: return "r11";
      case R12: return "r12";
      case R13: return "r13";
      case R14: return "r14";
      case R15: return "r15";
    }
    return "";
  }
};

class LifeInterval {
  const llvm::Value *_v;
  std::pair<int, int>_interval;
  Register *_assigned;

public:
  LifeInterval(const llvm::Value *v, int s, int e) : _v(v), 
  _interval(std::make_pair(s,e)), _assigned(nullptr) { }

  bool isAssigned() { return _assigned != nullptr; }
  void assignToRegister(Register *r) { _assigned = r; }
  void removeFromRegister() { _assigned = nullptr; }

  int getRegisterType() { return isAssigned() ? _assigned->getType() : -1; }
  int getStart() const { return _interval.first; }
  int getEnd() const { return _interval.second; }
  const llvm::Value *getValue() { return _v; }

  void changeStart(int start) { _interval.first = start; }
  void changeEnd(int end) { _interval.second = end; }

  void addRange(int start, int end) {
    if (start > end) return;
    if (start < _interval.first) _interval.first = start;
    if (end > _interval.second) _interval.second = end;
  }

  static bool compareStart(LifeInterval *a, LifeInterval *b) { return a->getStart() < b->getStart(); }
  static bool compareEnd(LifeInterval *a, LifeInterval *b) { return b->getEnd() < b->getEnd(); }

  bool operator<(const LifeInterval& b) { return _interval.first < b.getStart(); }
  bool operator>(const LifeInterval& b) { return _interval.first > b.getStart(); }
};

class RegisterAllocator {
  const unsigned MAX_REGS_TO_USE = 13;
  const unsigned MIN_REGS_TO_USE = 2;
  unsigned _num_regs;

  std::map<const llvm::Value *, int>_instructionLocation;
  std::map<const llvm::Value *, LifeInterval *>_unhandledLifeIntervals;
  std::vector<LifeInterval *>_active;
  std::vector<Register *>freeRegisters;
  std::map<const llvm::Value *, Location *>_currentLocations;

  void addRange(const llvm::Value *v, int start, int end) {
    if (_unhandledLifeIntervals.find(v) != _unhandledLifeIntervals.end()) {
      _unhandledLifeIntervals[v]->addRange(start, end);
    } else {
      LifeInterval *li = new LifeInterval(v, start, end);
      _unhandledLifeIntervals[v] = li;
    }
  }

  int getNextSpillOffset() {
    int currentMaxOffset = 0; 
    for(auto it = _currentLocations.begin(); it != _currentLocations.end(); ++it) {
      if (Spill::classof(&(*(it->second)))) {
        int offset = ((Spill *)it->second)->getOffset();
        if (currentMaxOffset < offset) currentMaxOffset = offset;
      }
    }
    return ++currentMaxOffset;
  }

  void setLastIntervalFrom(const llvm::Value *v, int start) {
    if (_unhandledLifeIntervals.find(v) != _unhandledLifeIntervals.end()) {
      _unhandledLifeIntervals[v]->changeStart(start);
    }
  }

  void expireOldIntervals(LifeInterval *li) {
    std::vector<LifeInterval *> orderedActive(_active);
    std::sort(orderedActive.begin(), orderedActive.end(), LifeInterval::compareEnd);

    for (auto j = _active.begin(); j != _active.end(); ){
      LifeInterval *i = *j;
      if (i->getEnd() >= li->getStart()) return;
      j = _active.erase(j);

      Register *r = (Register *)_currentLocations[i->getValue()];
      _currentLocations.erase(i->getValue());
      freeRegisters.push_back(r);
    }
  }

  void spillAtInterval(LifeInterval *li) {
    LifeInterval *spill = _active.back();
    Spill *sp = new Spill(getNextSpillOffset());
    
    if (spill->getEnd() > li->getEnd()) {
      Register *r = (Register *)_currentLocations[spill->getValue()];
      _currentLocations.erase(spill->getValue());
      _active.pop_back();
      _currentLocations[li->getValue()] = r;
      _active.push_back(li);
      std::sort(_active.begin(), _active.end(), LifeInterval::compareEnd);
      _currentLocations[spill->getValue()] = sp;
    } else {
      _currentLocations[li->getValue()] = sp;
    }
  }
  
  void linearScanForValue(const llvm::Value *v) {
    if (_unhandledLifeIntervals.find(v) != _unhandledLifeIntervals.end()) {
      LifeInterval *li = _unhandledLifeIntervals[v];
      expireOldIntervals(li);
      if (_active.size() >= _num_regs) {
        spillAtInterval(li);
      } else if(freeRegisters.size() > 0) {
        Register *r = freeRegisters.at(0);
        freeRegisters.erase( freeRegisters.begin() );
        _currentLocations[li->getValue()] = r;
        _active.push_back(li);
        std::sort(_active.begin(), _active.end(), LifeInterval::compareEnd);
      }
    }
  }
public:
  RegisterAllocator(unsigned num_regs) {
    _num_regs = std::min(std::max(num_regs, MIN_REGS_TO_USE), MAX_REGS_TO_USE);
    for (unsigned i = 0; i<_num_regs; i++) {
      Register *reg = new Register(i);
      freeRegisters.push_back(reg);
    }
  }

  void addInstructionToLocation(const llvm::Value *inst, int loc) {
    _instructionLocation[inst] = loc;
  }

  std::string getLocationNameForValue(const llvm::Value *v) {
    if (_currentLocations.find(v) != _currentLocations.end()) {
      return _currentLocations[v]->getName();
    }
    linearScanForValue(v);
    return getLocationNameForValue(v);
  }

  void buildIntervalsForFunction(llvm::Function &f) {
    std::set<const llvm::Value *>live = {};
    std::map< llvm::BasicBlock *, std::set<const llvm::Value *> >liveIn;

    llvm::Function::BasicBlockListType &basic_blocks = f.getBasicBlockList();
    for (auto bb = basic_blocks.rbegin(), bg = basic_blocks.rend(); bb != bg; ++bb) {
      live.clear();
      llvm::BasicBlock &BB = *bb;
      llvm::TerminatorInst *TI = BB.getTerminator();
      if (TI) {
        for (llvm::BasicBlock *succ : TI->successors()) {
          std::set<const llvm::Value *> li = liveIn[succ];
          if (!li.empty()) {
            live.insert(std::begin(li), std::end(li));
          }
          //TODO: Should this be reverse order?
          for (auto II = succ->begin(), IE = succ->end(); II != IE; ++II) {
            llvm::Instruction &sin = *II;
            if (llvm::PHINode::classof(&sin)){
              llvm::PHINode *PN = llvm::cast<llvm::PHINode>(&sin);
              const llvm::Value *v = PN->getIncomingValueForBlock(&BB);
              if (v != nullptr) live.insert(v);
            }
          }
        }
      }

      for (const llvm::Value *v : live) {
        llvm::Instruction &first_i = *(BB.begin());
        llvm::Instruction &last_i = *(BB.end());
        addRange(v, _instructionLocation[&first_i], _instructionLocation[&last_i]);
      }

      for (auto i = BB.rbegin(), ib = BB.rend(); i != ib; ++i) {
        llvm::Instruction &inst = *i;
        setLastIntervalFrom(&inst, _instructionLocation[&inst]);
        live.erase(&inst);
        for (unsigned k = 0; k < inst.getNumOperands(); k++){
          const llvm::Value *v = inst.getOperand(k);
          llvm::Instruction &first_i = *(BB.begin());
          addRange(v, _instructionLocation[&first_i], _instructionLocation[&inst]);
          live.insert(v);
        }
        if (llvm::PHINode::classof(&inst)) {
          live.erase(&inst);
        }
      }

      //TODO
      //llvm::LoopInfo &li = llvm::getAnalysis<LoopInfo>(f);
      //if (li.isLoopHeader(BB)) {
      //}
      //if b is loop header then
      //loopEnd = last block of the loop starting at b 
      //for each opd in live do
      //intervals[opd].addRange(b.from, loopEnd.to) 

      liveIn[&BB] = live;
    }
  }
};

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
  
  RegisterAllocator *_ra;
  std::string _mem_val;

  unsigned _opType;
public:
  Node (unsigned op, std::vector<Node *> *kids, const llvm::Value *value, RegisterAllocator *ra, unsigned opT=-1) 
    : _op(op),
    _state(0),
    _value(value),
    _ra(ra),
    _opType(opT) {
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
	~Node(){ delete _kids; }
	unsigned getOp() { return _op; }
  unsigned getOpType() { return _opType; }
	void setOp(int op) { _op = op; }
  void setOpType(int opt) { _opType = opt; }
  bool isLeaf() { return _num_kids == 0; }
  std::string getMemVal() { return _mem_val;}
  void setMemVal(bool getRegister, std::string val = "") {
    if (getRegister) {
      _mem_val = _ra->getLocationNameForValue(_value);
      return;
    } else {
      _mem_val = val;
    }
  }

	StatePtr getState() { return _state; }
	void setState(StatePtr state) { _state = state; }
	const llvm::Value* getValue() { return _value; }

	int getNumKids() { return _num_kids;}
	NODEPTR *getKids() { return _kids; }
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
	COST(int cost) { _cost = cost; }
	int getCost() { return _cost; }
	void setCost(int cost) { _cost = cost; }
};

#define COST_LESS(a,b) ((a).getCost() < (b).getCost())

static COST COST_INFINITY = COST(32767);
static COST COST_ZERO     = COST(0);

/** ================ Operations ================ **/

enum {
  CONST=0,
  VAR,
  RET,
  STORE,
  OP,
  ADD,
  MUL,
  SUB,
  DIV,
  REM
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
#define VAR 1
#define RET 2
#define STORE 3
#define OP 4

struct burm_state {
  int op;
  NODEPTR node;
  struct burm_state **kids;
  COST cost[8];
  struct {
    unsigned burm_stmt:3;
    unsigned burm_op:2;
    unsigned burm_var_op_const:2;
    unsigned burm_var_op:2;
    unsigned burm_const_op:2;
    unsigned burm_var:1;
    unsigned burm_const:1;
  } rule;
};


struct burm_state *burm_label(NODEPTR);
struct burm_state *burm_label1(NODEPTR);

void dumpCover(NODEPTR,int,int);

#endif
