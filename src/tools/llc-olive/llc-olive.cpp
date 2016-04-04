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

  //TODO: Loop through global variables
  //  for (Module::const_global_iterator I = M->global_begin(), E = M->global_end();
  //      I != E; ++I) {
  //  }

  RegisterAllocator *ra = setLocationsToInstructionsInModule(M.get());
  for (auto I = M->rbegin(), E = M->rend(); I != E; ++I) {
    compileFunction(*I, ra);
  }
  
  std::cout.rdbuf(coutbuf);
  of.close();
  return 0;
}

std::vector<NODEPTR> *buildSubtreeForInstruction(Value &v, RegisterAllocator *ra) {
  NODEPTR kid = nullptr;
  int nodeType = -1, nodeOpType = -1;
  std::vector<NODEPTR> *children;

  std::vector<NODEPTR> *subtree = new std::vector<NODEPTR>();
  if (Instruction::classof(&v)) {
    Instruction &inst = cast<Instruction>(v);
    for (unsigned k = 0; k < inst.getNumOperands(); k++){
      children = nullptr;
      nodeType = -1;
      Value *v = inst.getOperand(k);      
      if (LoadInst::classof(v)){
        Value *va = ((LoadInst *)v)->getPointerOperand();
        v = va;
      }
      if (ConstantInt::classof(v)) {
        kid = new Node(CONST, nullptr, v, ra);
        subtree->push_back(kid);
        continue;
      } else if (AllocaInst::classof(v)) {
        kid = new Node(VAR, nullptr, v, ra);
        subtree->push_back(kid);
        continue;
      }

      if (BinaryOperator::classof(v)) {
        unsigned op = ((BinaryOperator*)v)->getOpcode();
        nodeType = OP;
        switch (op) {
          case llvm::Instruction::Add: nodeOpType = ADD; break;
          case llvm::Instruction::Mul: nodeOpType = MUL; break;
          case llvm::Instruction::Sub: nodeOpType = SUB; break;
          case llvm::Instruction::UDiv: nodeOpType = DIV; break;
          case llvm::Instruction::URem: nodeOpType = REM; break;
        }
        children = buildSubtreeForInstruction(*v, ra);
        kid = new Node(nodeType, children, v, ra, nodeOpType);
        subtree->push_back(kid);
      } 
    }
  }
  return subtree;
}

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

void printDebugTree(Node *p, int indent=0) {
  if(p != nullptr) {
    if (indent) errs() << "|"; 
    int i = 0;
    for (; i < indent-4; ++i) errs() << " ";
    if (indent-4 > 0) errs() << "|";
    for (; i < indent; ++i) errs() << "-";
    errs() << "+ " << p->getOp() << "\n";
    
    if (p->getNumKids() > 0) {
      Node **n = p->getKids();
      for (int i = 0; i < p->getNumKids(); ++i) {
        printDebugTree(n[i], (indent+4));
      }
    }
  }
}

static void compileFunction(Function &func, RegisterAllocator *ra) {
  //Function name label
  std::string funcName(func.getName());
  std::cout << "_" << funcName << ":\n";

  //Prework for register allocation
  ra->buildIntervalsForFunction(func);
  std::vector<NODEPTR> *children = nullptr;
  int rootType = -1;
  NODEPTR root = nullptr;
  
  Function::ArgumentListType &args = func.getArgumentList();
  std::set<Value *>arguments;
  for (auto a = args.begin(); a != args.end(); ++a) {
    Value &vv = *a;
    arguments.insert(&vv);
  }

  //Build Expression Tree
  for (auto bb = func.begin(), be = func.end(); bb != be; ++bb) {
    BasicBlock &BB = *bb;
    for (BasicBlock::iterator i = BB.begin(), ie = BB.end(); i != ie; ++i) {
      Instruction &inst = *i;
      rootType = -1;
      children = buildSubtreeForInstruction(inst, ra);
      Value *v = nullptr;
      if (StoreInst::classof(&inst)) {
        v = ((StoreInst *)&inst)->getValueOperand();
        // If it's a local variable (or intermediate value)
        if (arguments.find(v) == arguments.end()) {
          rootType = STORE;
        }
      }
      else if (ReturnInst::classof(&inst)) {
        rootType = RET;
        v = &inst;
      }
      if (rootType != -1) {
        root = new Node(rootType, children, v, ra);

        //TODO: Delete when not needed
        errs()<<"\n";
        printDebugTree(root);

        CodeGenerator::generateCode(root);
        delete root;
        root = nullptr;
      }
    }
  }
  std::cout << "\n";
}
