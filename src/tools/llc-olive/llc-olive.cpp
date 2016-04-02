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

std::vector<NODEPTR> *buildSubtreeForInstruction(Value &v, RegisterAllocator *ra) {
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
      kid = new Node(nodeType, nullptr, v, ra);
      subtree->push_back(kid);
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

static void compileFunction(Function &func, RegisterAllocator *ra) {
  bool hasHadReturn = false;

  //Print frame
  std::string funcName(func.getName());
  std::cout << funcName << ":\n";
  std::cout << "  push %ebp\n";
  std::cout << "  mov %ebp, %esp\n";
  //sub   esp, N //TODO: Grow the stack by N bytes to reserve space for local variables

  //Prework for register allocation
  ra->buildIntervalsForFunction(func);

  ra->debugPrintIntervals();

  //Build Expression Tree
  NODEPTR root = nullptr;
  std::vector<NODEPTR> *children = nullptr;
  int rootType = -1;
  for (Function::iterator bb = func.begin(), be = func.end(); bb != be; ++bb) {
    BasicBlock &BB = *bb;
    for (BasicBlock::iterator i = BB.begin(), ie = BB.end(); i != ie; ++i) {
      Instruction &inst = *i;
      rootType = -1;
      children = buildSubtreeForInstruction(inst, ra);
      if (ReturnInst::classof(&inst)) {
        hasHadReturn = true;
        rootType = RET;
      } else if (StoreInst::classof(&inst)) {
        rootType = MOV;
      }
      if (rootType != -1) {
        root = new Node(rootType, children, &inst, ra);
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
