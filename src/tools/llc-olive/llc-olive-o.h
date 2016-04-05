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

class Memory : public Location {
  const unsigned SUPPORTED_OFFSET_DATATYPE = 8; 
  int _offset;
public:
  Memory(int offset) : 
  Location(SPILL),
  _offset(SUPPORTED_OFFSET_DATATYPE*offset) { }
  
  static bool classof(Location *l) { 
    if (l != nullptr) return l->isTypeOfClass(SPILL); 
    return false;
    }
  static bool classof(Location l) { return l.isTypeOfClass(SPILL); }
  int getOffset() { return _offset / SUPPORTED_OFFSET_DATATYPE; }
  
  std::string getName() {
    return "qword ptr [rbp - "+std::to_string(_offset)+"]";
  }
};

class Register : public Location {
  int _name;
  int _type;
public:
  /**
    Names for all of the x86 64 bits registers.
  **/
  enum {
    RSP, RBP, RAX, RBX, RCX, RDX, RDI, RSI,
    R8, R9, R10, R11, R12, R13, R14, R15
  };
  
  /**
    CALLEE_SAVE: function callee (call receiver) must save/restore these 
                 register when using them. Caller expects callee not 
                 to change them.
    
    CALLER_SAVE: function caller must save/restore these registers when 
                 live across call so that callee is free to use them.
    
    SPECIAL:     don't allocate them. Used for everything to work.
    
    ARGUMENT:    function arguments should use this. They should also 
                 be threated as caller-save.
  **/
  enum { CALLEE_SAVE, CALLER_SAVE, SPECIAL, ARGUMENT };
  
  Register(int name, int type) : Location(REGISTER), _name(name), _type(type) { }

  static bool classof(Location *l) { 
    if (l != nullptr) return l->isTypeOfClass(REGISTER);
    return false;
  }
  static bool classof(Location l) { return l.isTypeOfClass(REGISTER); }
  int getType() { return _type; };
  
  std::string getName() {
    switch (_name) {
      case RSP: return "rsp";
      case RBP: return "rbp";
      case RAX: return "rax";
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
  static bool compareEnd(LifeInterval *a, LifeInterval *b) { return a->getEnd() < b->getEnd(); }

  bool operator<(const LifeInterval& b) { return _interval.first < b.getStart(); }
  bool operator>(const LifeInterval& b) { return _interval.first > b.getStart(); }
};

class FreeRegisterManager {
  const int NOT_AVAILABLE_REGISTER_NUMBER = 3;
  
  int _num_regs;
  std::set<Register *>_special;
  std::set<Register *>_callee_save;
  std::set<Register *>_caller_save;
  std::set<Register *>_argument;
public:
  FreeRegisterManager(int n) : _num_regs(n) {
    Register *reg;
    for (int i = NOT_AVAILABLE_REGISTER_NUMBER; i<(_num_regs+NOT_AVAILABLE_REGISTER_NUMBER); i++) {
      switch (i) {
        case Register::RSP: case Register::RBP:
          reg = new Register(i, Register::SPECIAL);
          _special.insert(reg);
          break;
        case Register::RBX: case Register::R12: 
        case Register::R13: case Register::R14: 
        case Register::R15: 
          reg = new Register(i, Register::CALLEE_SAVE);
          _callee_save.insert(reg);
          break;
        case Register::RDI: case Register::RSI: 
        case Register::RDX: case Register::RCX: 
        case Register::R8: case Register::R9:
          reg = new Register(i, Register::ARGUMENT);
          _argument.insert(reg);
          break;
        default:
          reg = new Register(i, Register::CALLER_SAVE);
          _caller_save.insert(reg);
          break;
      }
    }
  }
  int availableRegistersNum() { return _callee_save.size() + _caller_save.size() + _argument.size(); }

  Register *getAvailableRegister(int type=-1) {
    if (availableRegistersNum() > 0) {
      switch (type) {
        case Register::SPECIAL: if (_special.size() > 0) return *(_special.begin()); break;
        case Register::CALLEE_SAVE: if (_callee_save.size() > 0) return *(_callee_save.begin()); break;
        case Register::CALLER_SAVE: if (_caller_save.size() > 0) return *(_caller_save.begin()); break;
        case Register::ARGUMENT: if (_argument.size() > 0) return *(_argument.begin()); break;
      }
      if (_callee_save.size() > 0) return *(_callee_save.begin());
      if (_caller_save.size() > 0) return *(_caller_save.begin());
      return *(_argument.begin());
    }
    return nullptr;
  }

  void allocateRegister(Register *r) {
    switch(r->getType()) {
      case Register::SPECIAL: _special.erase(r); break;
      case Register::CALLEE_SAVE: _callee_save.erase(r); break;
      case Register::CALLER_SAVE: _caller_save.erase(r); break;
      case Register::ARGUMENT: _argument.erase(r); break;
    }
  }
  
  void setAsFree(Register *r) {
    switch(r->getType()) {
      case Register::SPECIAL: _special.insert(r); break;
      case Register::CALLEE_SAVE: _callee_save.insert(r); break;
      case Register::CALLER_SAVE: _caller_save.insert(r); break;
      case Register::ARGUMENT: _argument.insert(r); break;
    }
  }
};

class RegisterAllocator {
  const unsigned MAX_REGS_FOR_ARGUMENTS = 6;
  const unsigned MAX_REGS_TO_USE = 13;
  const unsigned MIN_REGS_TO_USE = 2;
  unsigned _num_regs;

  std::map<const llvm::Value *, int>_instructionLocation;
  std::map<const llvm::Value *, LifeInterval *>_lifeIntervals;

  FreeRegisterManager *_freeRegisters;
  std::vector<LifeInterval *>_active;
  std::map<const llvm::Value *, Location *>_symbolTable;

  void addRange(const llvm::Value *v, int start, int end) {
    if (_lifeIntervals.find(v) != _lifeIntervals.end()) {
      _lifeIntervals[v]->addRange(start, end);
    } else {
      LifeInterval *li = new LifeInterval(v, start, end);
      _lifeIntervals[v] = li;
    }
  }

  int getNextMemoryOffset() {
    int currentMaxOffset = 0;
    for(auto it = _symbolTable.begin(); it != _symbolTable.end(); ++it) {
      if (Memory::classof(&(*(it->second)))) {
        int offset = ((Memory *)it->second)->getOffset();
        if (currentMaxOffset < offset) currentMaxOffset = offset;
      }
    }
    return ++currentMaxOffset;
  }

  void expireOldIntervals(LifeInterval *i) {
    for (auto active_iterator = _active.begin(); active_iterator != _active.end(); ){
      LifeInterval *j = *active_iterator;
      
      if (j->getEnd() >= i->getStart()) return;

      active_iterator = _active.erase(active_iterator);

      Location *l = _symbolTable[j->getValue()];
      _symbolTable.erase(j->getValue());
      if (Register::classof(l)) {
        _freeRegisters->setAsFree((Register *)l);
      }
    }
  }

  void spillAtInterval(LifeInterval *i) {
    LifeInterval *spill = _active.back();
    
    Memory *sp = new Memory(getNextMemoryOffset());
    
    if (spill->getEnd() > i->getEnd()) {
      Location *l = _symbolTable[spill->getValue()];
      _symbolTable.erase(spill->getValue());
      _active.pop_back();
      _symbolTable[i->getValue()] = l;
      _active.push_back(i);
      std::sort(_active.begin(), _active.end(), LifeInterval::compareEnd);
      _symbolTable[spill->getValue()] = sp;
    } else {
      _symbolTable[i->getValue()] = sp;
    }
  }
  
  void linearScanForValue(const llvm::Value *v) {
    if (_lifeIntervals.find(v) != _lifeIntervals.end()) {
      LifeInterval *li = _lifeIntervals[v];
      expireOldIntervals(li);
      
      if (_active.size() >= _num_regs) {
        spillAtInterval(li);
      } else if(_freeRegisters->availableRegistersNum() > 0) {
        Register *r = _freeRegisters->getAvailableRegister();
        _freeRegisters->allocateRegister(r);
        _symbolTable[li->getValue()] = r;
        _active.push_back(li);
        std::sort(_active.begin(), _active.end(), LifeInterval::compareEnd);
      }
    }
  }
public:
  RegisterAllocator(unsigned num_regs) {
    _num_regs = std::min(std::max(num_regs, MIN_REGS_TO_USE), MAX_REGS_TO_USE);
    _freeRegisters = new FreeRegisterManager(_num_regs);
  }

  void addInstructionToLocation(const llvm::Value *inst, int loc) {
    _instructionLocation[inst] = loc;
  }

  void freeRegistersNotUsedAnymoreAfterValue(const llvm::Value *v) {
    int location = _instructionLocation[v];
    for (auto active_iterator = _active.begin(); active_iterator != _active.end(); ) {
      LifeInterval *j = *active_iterator;

      if (j->getEnd() > location) return;
      active_iterator = _active.erase(active_iterator);
      
      Location *l = _symbolTable[j->getValue()];
      _symbolTable.erase(j->getValue());
      if (Register::classof(l)) {
        _freeRegisters->setAsFree((Register *)l);
      }
    }
  }

  std::string getIntervalForValue(const llvm::Value *v) {
    if (_lifeIntervals.find(v) != _lifeIntervals.end()) {    
      return std::to_string(_lifeIntervals[v]->getStart()) + ", " +std::to_string(_lifeIntervals[v]->getEnd());
    }
    return "";
  }

  std::string getLocationNameForValue(const llvm::Value *v) {
    if (_symbolTable.find(v) != _symbolTable.end()) {
      return _symbolTable[v]->getName();
    }
    linearScanForValue(v);
    return getLocationNameForValue(v);
  }

  void buildIntervalsForFunction(llvm::Function &f) {
    for (auto bb = f.begin(), be = f.end(); bb != be; ++bb) {
      llvm::BasicBlock &BB = *bb;
      for (auto i = BB.begin(), ie = BB.end(); i != ie; ++i) {
        llvm::Instruction &inst = *i;
        if (inst.getNumUses() > 0) {
          const llvm::Value *v = inst.user_back();
          addRange(&inst, _instructionLocation[&inst], _instructionLocation[v]);
        }
      }
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
  Node (unsigned op, std::vector<Node *> *kids, const llvm::Value *value, RegisterAllocator *ra, unsigned opT=0) 
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

  void freeRegisters() { _ra->freeRegistersNotUsedAnymoreAfterValue(_value); }
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
  CONST=0, VAR, RET, STORE, OP, ADD, MUL, SUB
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
