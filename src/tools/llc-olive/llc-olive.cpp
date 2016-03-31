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

static int compileModule(char **, LLVMContext &);
static void compileFunction(Function &);

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

  for (Module::iterator I = M->begin(), E = M->end(); I != E; ++I) {
    compileFunction(*I);
  }
  //Iterate through all instructions.
  //If instruction in uses has a tree already, merge trees.
  //after soemthing (undefined at the moment) send tree to olive and be happy.
  //
  //The expression tree can have information from LLVM IR

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

static void compileFunction(Function &func) {
  bool hasHadReturn = false;
  
  //Print frame
  std::string funcName(func.getName());
  std::cout << funcName << ":\n";
  std::cout << "  push ebp\n";
  std::cout << "  mov ebp, esp\n";
  //sub   esp, N //Grow the stack by N bytes to reserve space for local variables
  
  //Build Expression Tree
  NODEPTR root = nullptr, kid = nullptr;
  std::vector<NODEPTR> *children = new std::vector<NODEPTR>();
  
  for (Function::iterator bb = func.begin(), be = func.end(); bb != be; ++bb) {
    BasicBlock &BB = *bb;
    for (BasicBlock::iterator i = BB.begin(), ie = BB.end(); i != ie; ++i) {
      Instruction &inst = *i;
      children->clear();

      if (TerminatorInst::classof(&inst)) {
        for (unsigned k = 0; k < inst.getNumOperands(); k++){
          Value *v = inst.getOperand(inst.getNumOperands() - k - 1);
          errs() << std::string(v->getName()) << "\n";
          if (ConstantInt::classof(v)) {
            errs() << "T-constant\n";
            kid = new Node(CONST, nullptr, v);
            children->push_back(kid);
          } else {
            errs() << "T-variable\n";
            kid = new Node(ADDR, nullptr, v);
            children->push_back(kid);
          }
        }
        hasHadReturn = true;
        root = new Node(RET, children, &inst); 
      }
      else if (StoreInst::classof(&inst)) {
        for (unsigned k = 0; k < inst.getNumOperands(); k++){
          Value *v = inst.getOperand(inst.getNumOperands() - k - 1);
          if (ConstantInt::classof(v)) {
            errs() << "S-constant\n";
            kid = new Node(CONST, nullptr, v);
          } else {
            errs() << "S-variable\n";
            kid = new Node(ADDR, nullptr, v);
          }
          children->push_back(kid);
        }
        root = new Node(MOV, children, &inst);
      }
      
      if (root != nullptr) {
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
