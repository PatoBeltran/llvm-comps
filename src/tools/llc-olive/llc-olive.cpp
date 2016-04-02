#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/Triple.h"

#include "llvm/CodeGen/MIRParser/MIRParser.h"
#include "llvm/IRReader/IRReader.h"

#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/SourceMgr.h"

#include <fstream>
#include "llc-olive-o.h"

using namespace llvm;

static cl::opt<std::string>
InputFilename(cl::Positional, cl::desc("<input bitcode>"), cl::init("-"));

static cl::opt<std::string>
OutputFilename("o", cl::desc("Output filename"), cl::value_desc("filename"), cl::Required);

static cl::opt<unsigned>
AvailableRegisters("num_regs", cl::desc("Available number of registers"), cl::init(16u), cl::Optional);

//TODO: DELETE FROM HERE
class RegisterAllocator;

static int compileModule(char **, LLVMContext &);
static void compileFunction(Function &, RegisterAllocator *);
static RegisterAllocator *setLocationsToInstructionsInModule(Module *);

int main(int argc, char **argv) {
  LLVMContext &Context = getGlobalContext();
  cl::ParseCommandLineOptions(argc, argv, "llvm system compiler\n");

  if (int RetVal = compileModule(argv, Context))
    return RetVal;
  return 0;
}

static int compileModule(char **argv, LLVMContext &Context) {
  SMDiagnostic Err;  
  std::unique_ptr<Module> M;
  M = parseIRFile(InputFilename, Err, Context);

  if (!M) {
    Err.print(argv[0], errs());
    return 1;
  }

  // Figure out where we are going to send the output.
  std::ofstream of(OutputFilename);
  if (!of) return 1;
  
  //redirect cout to output file
  std::streambuf *coutbuf = std::cout.rdbuf();
  std::cout.rdbuf(of.rdbuf());

  //Loop through global variables
//  for (Module::const_global_iterator I = M->global_begin(), E = M->global_end();
//      I != E; ++I) {
//  }

  RegisterAllocator *ra = setLocationsToInstructionsInModule(M.get());
  for (Module::iterator I = M->begin(), E = M->end(); I != E; ++I) {
    compileFunction(*I, ra);
  }

  //Loop through aliases
//  for (Module::const_alias_iterator I = M->alias_begin(), E = M->alias_end();
//        I != E; ++I) {
//  }

  //Loop through named metadata
//  for (Module::const_named_metadata_iterator I = M->named_metadata_begin(),
//          E = M->named_metadata_end(); I != E; ++I) {
//  }

  std::cout.rdbuf(coutbuf);
  of.close();
  return 0;
}

std::vector<NODEPTR> *buildSubtreeForInstruction(Value &v) {
  NODEPTR kid = nullptr;
  int nodeType = -1;
  std::vector<NODEPTR> *subtree = new std::vector<NODEPTR>();
  if (Instruction::classof(&v)) {
    Instruction &inst = cast<Instruction>(v);
    for (unsigned k = 0; k < inst.getNumOperands(); k++){
      nodeType = -1;
      Value *v = inst.getOperand(inst.getNumOperands() - k - 1);
      if (ConstantInt::classof(v)) {
        nodeType = CONST;
      } else {
        nodeType = ADDR;
      }
      kid = new Node(nodeType, nullptr, v);
      subtree->push_back(kid);
    }
  }
  return subtree;
}

class Register {
  bool _busy;
  int _type;
  int _freeUntilPos;
  int _nextUsePos;
public:
  enum {
    RAX=0, RBX=1, RCX=2, RDX=3, RDI=4, RSI=5,
    R8=6, R9=7, R10=8, R11=9, R12=10, R13=11, R14=12, R15=13
  };
  Register(int type, bool allocate) : _busy(allocate), _type(type), _freeUntilPos(INT_MAX), _nextUsePos(INT_MAX) { }
  
  void allocate() { _busy = true; }
  void deallocate() { _busy = false; }

  bool isAllocated() { return _busy; }
  void setFreeUntilPos(int pos) { _freeUntilPos = pos; }
  int getFreeUntilPos() { return _freeUntilPos; }
  void setNextUsePos(int pos) { _nextUsePos = pos; }
  int getNextUsePos() { return _nextUsePos; }

  std::string getName() {
    switch (_type) {
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
  }
};

class LifeInterval {
  std::pair<int, int>_interval;
  Register *_assigned;

public:
  LifeInterval(int s, int e) : _interval(std::make_pair(s,e)) { }
  
  bool isAssigned() { return _assigned != nullptr; }
  void assignToRegister(Register *r) { _assigned = r; }
  void removeFromRegister() { _assigned = nullptr; }

  void registerIsAssigned() { _assigned->setFreeUntilPos(0); }
  
  void registerWillBeAssignedAtStart() { 
    if (isAssigned() && _assigned->getFreeUntilPos() > _interval.first)
      _assigned->setFreeUntilPos(_interval.first);
  }

  int registerFreeUntilPos() { return _assigned->getFreeUntilPos(); }
  int getRegisterType() { return isAssigned() ? _assigned->getType() : -1; }
  int getStart() const { return _interval.first; }
  int getEnd() const { return _interval.second; }

  void changeStart(int start) { _interval.first = start; }
  void changeEnd(int end) { _interval.second = end; }

  void setNextUseOfRegister(int val) { 
    if (isAssigned() && _assigned->getNextUsePos() > val) 
      _assigned->setNextUsePos(val);
  }
  
  bool operator<(const LifeInterval& b) {
    return _interval.first < b.getStart();
  }

  bool operator>(const LifeInterval& b) {
    return _interval.first > b.getStart();    
  }
};

class LifeRange {
  llvm::Value *_v;
  int _start;
  int _end;
  std::vector<LifeInterval *>_alive;

  void addToAlive(LifeInterval *li) {
    _alive.push_back(li);
    std::sort(_alive.begin(), _alive.end());
  }
  
public:
  LifeRange(llvm::Value *v, int s, int e) : _v(v), _start(s), _end(e) {
    LifeInterval *li = new LifeInterval(s,e);
    addToAlive(li);
  }
  
  bool inLifeTimeHole(int pos) {
    if (pos < _start || pos > _end) return false;

    for (unsigned i = 0; _alive.size(); ++i) {
      int currStart =  _alive.at(i)->getStart();
      int currEnd = _alive.at(i)->getEnd();

      if (currStart <= pos && currEnd >= pos) return false;
    }
    return true;
  }

  llvm::Value *getValue() { return _v; }
  int getStart() { return _start; }
  int getEnd() { return _end; }

  void changeStart(int start) {
    _start = start;
    _alive.at(0)->changeStart(start);
  }

  bool hasMoreIntervals() {
    return _alive.size() > 0;
  }

  LifeInterval *getCurrentInterval() {
    return _alive.at(0);
  }

  void killCurrentInterval() {
    if (_alive.size() > 0) _alive.erase(_alive.begin());
  }    
  
  void assignToRegister(Register *r) {
    LifeInterval *ci = getCurrentInterval();
    ci->assignToRegister(r);
  }

  void splitBefore(int pos) {
    for (unsigned i = 0; _alive.size(); ++i) {
      int currStart =  _alive.at(i)->getStart();
      int currEnd = _alive.at(i)->getEnd();

      if (currStart < pos && currEnd > pos) {
        _alive.at(i)->changeEnd(pos);
        LifeInterval *li = new LifeInterval(pos, currEnd);      
        addToAlive(li);
      }
    }
  }

  void addRange(int start, int end) {
    if (start > end) return;

    int old_start = _start;
    int old_end = _end;
    
    if (start < old_start) _start = start;
    if (end > old_end) _end = end;
    
    if (start > old_start && start > old_end) {
      LifeInterval *li = new LifeInterval(start, end);      
      addToAlive(li);
      return;
    }
    
    if (start < old_start && end < old_start) {
      LifeInterval *li = new LifeInterval(start, end);      
      addToAlive(li);
      return;
    }

    for (unsigned i = 0; _alive.size(); ++i) {
      int currStart =  _alive.at(i)->getStart();
      int currEnd = _alive.at(i)->getEnd();

      if (start >= currStart && end <= currEnd) {
        return;
      }
      if (start < currStart  && end > currEnd) {
        _alive.at(i)->changeStart(start);
        _alive.at(i)->changeEnd(end);
        return;
      }
      if (start < currStart && end <= currEnd && end >= currStart) {
        _alive.at(i)->changeStart(start);
        return;
      }
      if (start >= currStart && end > currEnd && start <= currEnd) {
        _alive.at(i)->changeEnd(end);
        return;
      }
    }
    LifeInterval *li = new LifeInterval(start, end);      
    addToAlive(li);
    return;
  }
};

class RegisterAllocator {
  const unsigned MAX_REGS_TO_USE = 14;
  const unsigned MIN_REGS_TO_USE = 2;
  
  std::map<llvm::Value *, int>_instructionLocation;
  std::map<llvm::Value *, LifeRange *>_unhandledLifeRanges;
  unsigned _num_regs;

  LifeRange *current;
  std::vector<LifeRange *>_active;
  std::vector<LifeRange *>_inactive;
  std::vector<LifeRange *>_handled;

  std::vector<Register *>registers;

  void addRange(Value *v, int start, int end) {
    if (_unhandledLifeRanges.find(v) != _unhandledLifeRanges.end()) {
      _unhandledLifeRanges[v]->addRange(start, end);
    }
  }
  
  void setLastIntervalFrom(Value *v, int start) {
    if (_unhandledLifeRanges.find(v) != _unhandledLifeRanges.end()) {
      _unhandledLifeRanges[v]->changeStart(start);
    }
  }

  void killCurrentIntervalForRange(LifeRange *lr) {
    lr->killCurrentInterval();
    if (!lr->hasMoreIntervals()) {
      _unhandledLifeRanges.erase(lr->getValue());
    }
  }
  
  bool tryAllocateFreeReg(LifeRange *current) {
    for (Register *r : registers) r->setFreeUntilPos(INT_MAX);

    for (LifeRange *lr : _active) {
      lr->getCurrentInterval()->registerIsAssigned();
    }
    
    for (LifeRange *lr : _inactive) {
      lr->getCurrentInterval()->registerWillBeAssignedAtStart();
    }

    Register *reg = nullptr;
    int maxFreeUP = -1;
    for (Register *r : registers) {
      int fup = r->getFreeUntilPos();
      if (fup > maxFreeUP) {
        maxFreeUP = fup;
        reg = r;
      }
    }

    if (reg->getFreeUntilPos() <= 0) { return false; }
    else if (current->getEnd() < reg->getFreeUntilPos()) {
      current->assignToRegister(reg);
    } else {
      current->assignToRegister(reg);
      current->splitBefore(reg->getFreeUntilPos());
    }
    return true;
  }

  void allocateBlockedReg(LifeRange *current) {
    for (Register *r : registers) r->setNextUsePos(INT_MAX);
    int startOfCurrent = current->getCurrentInterval()->getStart();
    llvm::Value *v = current->getValue();
    
    int nextUsePos = 0;
    for (auto i = v->use_begin(); v != v->use_end(); ++v) {
      llvm::Value *v = i->getUser();
      if (_instructionLocation[v] > startOfCurrent) {
        nextUsePos = _instructionLocation[v];
        break;
      }
    }
    
    for (LifeRange *lr : _active) {
      current->getStart();
      lr->getCurrentInterval()->setNextUseOfRegister(nextUsePos);
    }
    
    for (LifeRange *lr : _inactive) {
      lr->getCurrentInterval()->setNextUseOfRegister(nextUsePos);
    }

    Register *reg = nullptr;
    int maxNextUP = -1;
    for (Register *r : registers) {
      int nup = r->getFreeUntilPos();
      if (nup > maxNextUP) {
        maxNextUP = nup;
        reg = r;
      }
    }
    
    int firstUseLoc = _instructionLocation[v->use_begin()->getUser()];
    if (useLoc > maxNextUP) {
      //assign spill slot to current
      current->splitBefore(useLoc);
    } else {
      current->assignToRegister(reg);
      for (LifeRange *lr : _active) {
        if (lr->getCurrentInterval()->getRegisterType() == reg->getType()) {
          lr->splitBefore(startOfCurrent);
        }
      }
      for (LifeRange *lr : _inactive) {
        if (lr->getCurrentInterval()->getRegisterType() == reg->getType()) {
          lr->splitBefore(startOfCurrent);
        }
      }

      //split active interval for reg at position
      //split any inactive interval for reg at the end of its lifetime hole
    }
  }
  
public:
  RegisterAllocator(unsigned num_regs) {
    _num_regs = std::min(std::max(num_regs, MIN_REGS_TO_USE), MAX_REGS_TO_USE);
    for (unsigned i = 0; i<_num_regs; i++) {
      Register *reg = new Register(i, false);
      registers.push_back(reg);
    }
  }
  
  void addInstructionToLocation(llvm::Value *inst, int loc) {
    _instructionLocation[inst] = loc;
  }

  void linearScanForValue(Value *v) {
    auto it = _unhandledLifeRanges.find(v);
    if (it == _unhandledLifeRanges.end()) return;
    
    LifeRange *current = it->second;
    LifeInterval *currentInt = current->getCurrentInterval();
    int position = currentInt->getStart();

    for (auto ii = _active.begin(); ii != _active.end(); ){
      LifeRange *it = *ii;
      if (it->getEnd() < position) {
        _handled.push_back(it);
        killCurrentIntervalForRange(it);
        ii = _active.erase(ii);
      } else if (it->inLifeTimeHole(position)) {
        _inactive.push_back(it);
        killCurrentIntervalForRange(it);
        ii = _active.erase(ii);
      } else {
        ++ii;
      }
    }

    for (auto ii = _inactive.begin(); ii != _inactive.end(); ){
      LifeRange *it = *ii;
      if (it->getEnd() < position) {
        _handled.push_back(it);
        killCurrentIntervalForRange(it);
        ii = _inactive.erase(ii);
      } else if (!it->inLifeTimeHole(position)) {
        _active.push_back(it);
        ii = _inactive.erase(ii);
      } else {
        ++ii;
      }
    }

    bool allocated = tryAllocateFreeReg(current);
    if (!allocated) allocateBlockedReg(current);

    if (currentInt->isAssigned()) _active.push_back(current);
  }

  void buildIntervalsForFunction(llvm::Function &f) {
    std::set<llvm::Value *>live = {};
    std::map< llvm::BasicBlock *, std::set<llvm::Value *> >liveIn;

    llvm::Function::BasicBlockListType &basic_blocks = f.getBasicBlockList();
    for (auto bb = basic_blocks.rbegin(), bg = basic_blocks.rend(); bb != bg; ++bb) {
      live.clear();
      llvm::BasicBlock &BB = *bb;
      llvm::TerminatorInst *TI = BB.getTerminator();
      if (TI) {
        for (llvm::BasicBlock *succ : TI->successors()) {
          std::set<llvm::Value *> li = liveIn[succ];
          if (!li.empty()) {
            live.insert(std::begin(li), std::end(li));
          }
          for (auto II = succ->begin(), IE = succ->end(); II != IE; ++II) {
            llvm::Instruction &sin = *II;
            if (llvm::PHINode::classof(&sin)){
              llvm::PHINode *PN = llvm::cast<llvm::PHINode>(&sin);
              llvm::Value *v = PN->getIncomingValueForBlock(&BB);
              if (v != nullptr) live.insert(v);
            }
          }
        }
      }
      
      for (llvm::Value *v : live) {
        Instruction &first_i = *(BB.begin());
        Instruction &last_i = *(BB.end());
        addRange(v, _instructionLocation[&first_i], _instructionLocation[&last_i]);
      }

      for (auto i = BB.rbegin(), ib = BB.rend(); i != ib; ++i) {
        Instruction &inst = *i;
        setLastIntervalFrom(&inst, _instructionLocation[&inst]);
        live.erase(&inst);
        for (unsigned k = 0; k < inst.getNumOperands(); k++){
          Value *v = inst.getOperand(k);
          Instruction &first_i = *(BB.begin());
          addRange(v, _instructionLocation[&first_i], _instructionLocation[&inst]);
          live.insert(v);
        }
        if (llvm::PHINode::classof(&inst)) {
          live.erase(&inst);
        }
      }
      
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

  void resolve() {
    //for each control flow edge from predecessor to successor do
      //for each interval it live at begin of successor do
        //if it starts at begin of successor then
          //phi = phi function defining it 
          //opd = phi.inputOf(predecessor) 
          //if opd is a constant then
            //moveFrom = opd 
          //else
            //moveFrom = location of intervals[opd] at end of predecessor 
        //else
          //moveFrom = location of it at end of predecessor
        //moveTo = location of it at begin of successor
        //if moveFrom ≠ moveTo then
          //mapping.add(moveFrom, moveTo) 
      //mapping.orderAndInsertMoves()
  }
};

static RegisterAllocator *setLocationsToInstructionsInModule(Module *M) {
  RegisterAllocator *ra = new RegisterAllocator(AvailableRegisters);
  int locationCounter = 1;

  for (Module::iterator I = M->begin(), E = M->end(); I != E; ++I) {  
    Function &func = *I;
    for (Function::iterator bb = func.begin(), be = func.end(); bb != be; ++bb) {
      BasicBlock &BB = *bb;
      for (BasicBlock::iterator i = BB.begin(), ie = BB.end(); i != ie; ++i) {
        Instruction &inst = *i;
        ra->addInstructionToLocation(&inst, locationCounter++);
      }
    }
  }
  return ra;
}

static void compileFunction(Function &func, RegisterAllocator *ra) {
  bool hasHadReturn = false;
  
  //Print frame
  std::string funcName(func.getName());
  std::cout << funcName << ":\n";
  std::cout << "  push ebp\n";
  std::cout << "  mov ebp, esp\n";
  //sub   esp, N //Grow the stack by N bytes to reserve space for local variables

  //Prework for register allocation
  ra->buildIntervalsForFunction(func);

  //Build Expression Tree
  NODEPTR root = nullptr;
  std::vector<NODEPTR> *children = nullptr;
  int rootType = -1;
  for (Function::iterator bb = func.begin(), be = func.end(); bb != be; ++bb) {
    BasicBlock &BB = *bb;
    for (BasicBlock::iterator i = BB.begin(), ie = BB.end(); i != ie; ++i) {
      Instruction &inst = *i;
      rootType = -1;
      children = buildSubtreeForInstruction(inst);
      if (ReturnInst::classof(&inst)) {
        hasHadReturn = true;
        rootType = RET;
      } else if (StoreInst::classof(&inst)) {
        rootType = MOV;
      }
      if (rootType != -1) {
        root = new Node(rootType, children, &inst);
        CodeGenerator::generateCode(root);
        delete root;
        root = nullptr;
      }
    }
  }

  if (!hasHadReturn) {
    //Function epilog for void functions
    std::cout << "  mov esp, ebp\n";
    std::cout << "  pop ebp\n";
    std::cout << "  ret\n";
  }
  std::cout << "\n";
}
