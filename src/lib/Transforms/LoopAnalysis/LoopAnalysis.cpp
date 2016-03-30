#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"

using namespace llvm;

namespace {
  struct LoopPassInfoHelper {
    std::string functionName;
    int depth, BBs, instr, atomics, branches;
    bool hasSubloops;

    LoopPassInfoHelper(std::string name, int d) { 
      this->functionName = name;
      this->depth = d; 
      this->hasSubloops = false;
      this->BBs = 0;
      this->instr = 0;
      this->atomics = 0;
      this->branches = 0;
    }
  };

  struct LoopAnalysisPass : public FunctionPass {
    static char ID;
    int loopId = 0;
    std::string functionName = "";
    LoopAnalysisPass() : FunctionPass(ID) {}

    /**
     * PrintLoopInfo
     */
    void printLoopInfo(Loop *loop, LoopInfo &info, int depth=0) {
      LoopPassInfoHelper currentLoop = getInfoOnLoop(loop, depth, info);
      std::vector<Loop*> subLoops = loop->getSubLoops(); 
      
      for (Loop::iterator j = subLoops.begin(); j != subLoops.end(); ++j) {
        currentLoop.hasSubloops = true;
        printLoopInfo(*j, info, depth + 1);
      }

      errs() << std::to_string(loopId) + ": func=";
      errs().write_escaped(currentLoop.functionName) << ", ";
      errs() << "depth=" + std::to_string(currentLoop.depth) + ", ";
      errs() << "subLoops=" + std::string(currentLoop.hasSubloops ? "true" : "false") + ", ";
      errs() << "BBs=" + std::to_string(currentLoop.BBs) + ", ";
      errs() << "instrs=" + std::to_string(currentLoop.instr) + ", ";
      errs() << "atomics=" + std::to_string(currentLoop.atomics) + ", ";
      errs() << "branches=" + std::to_string(currentLoop.branches) + "\n";
      loopId+=1;
    }

    /**
     * GetInfoOnLoop
     */
    LoopPassInfoHelper getInfoOnLoop(Loop *loop, int depthLevel, LoopInfo &info) {
      LoopPassInfoHelper currentLoop = LoopPassInfoHelper(functionName, depthLevel);

      for(Loop::block_iterator bb = loop->block_begin(); bb != loop->block_end(); ++bb) {
        BasicBlock *blk = *bb;
        for (BasicBlock::iterator i = blk->begin(); i != blk->end(); ++i) {
          Instruction& inst = *i;
          if (AtomicCmpXchgInst::classof(&inst) || AtomicRMWInst::classof(&inst)) currentLoop.atomics+=1;
        }
        currentLoop.instr+= blk->size();
        if (info.getLoopFor(blk) == loop) {
          currentLoop.BBs+=1;
          if (BranchInst::classof(blk->getTerminator())) currentLoop.branches+=1;
        }
      }
      return currentLoop;
    }

    /**
     * RunOnFunction
     */
    bool runOnFunction(Function &F) override {
      functionName = F.getName();
      LoopInfo &info = getAnalysis<LoopInfoWrapperPass>().getLoopInfo(); 
      for (LoopInfo::iterator i = info.begin(); i != info.end(); ++i) { 
        printLoopInfo(*i, info);
      }
      return false;
    }

    /**
     * GetAnalysisUsage
     */
    virtual void getAnalysisUsage(AnalysisUsage& AU) const override {
      AU.addRequired<LoopInfoWrapperPass>();
      AU.setPreservesAll();
    }
  };
}

char LoopAnalysisPass::ID = 0;
static RegisterPass<LoopAnalysisPass> X("loop-props", "Loop Analysis Pass", false, false);
