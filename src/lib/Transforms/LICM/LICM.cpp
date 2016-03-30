#define DEBUG_TYPE "LICM"

#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar.h"

STATISTIC(numOfHoists, "Number of hoisted expressions.");

using namespace llvm;

namespace {
  struct DebugHelperMethods {
    private:
      static std::string boolToS(bool b) {
        return b ? "true" : "false";
      }

    public:
      static void printStartHeader() {
        DEBUG(errs() << "\n\n");
        DEBUG(errs() << " ===================================================== \n");
        DEBUG(errs() << " ============= START PASS DEBUG INFO ================= \n");
        DEBUG(errs() << " ===================================================== \n");
        DEBUG(errs() << "\n\n");
      }
      static void printLoopInvariantDebugInfo(Value *v, Loop *loop) {
        if (Instruction::classof(v)) { 
          Instruction *inst = cast<Instruction>(v);
          DEBUG(errs() << "-- isGetElementPrt: " << boolToS(GetElementPtrInst::classof(inst)) <<
              " | isSelect: " << boolToS(SelectInst::classof(inst)) << " | isShift: "<< boolToS(inst->isShift()) << 
              " | isCast: "<< boolToS(inst->isCast()) << " | isBinaryOp: "<< boolToS(inst->isBinaryOp()) << "\n");
        }
      }
      static void printLoopInvariantDebugInfoOperator(Value *v, Loop *loop) {
        if (Instruction::classof(v) && loop->contains(cast<Instruction>(v))){
          DEBUG(errs() << " | defined inside loop |");
        } else if (Constant::classof(v)) {
          DEBUG(errs() << " | constant | ");
        }
      }
      static void printSafeToHoistDebugInfo(Instruction *inst, bool dominatesExiting) {
        DEBUG(errs() << "-- safeToSpeculateExecute: " << boolToS(isSafeToSpeculativelyExecute(inst) ) << " | dominates all exits: " << boolToS(dominatesExiting) << "\n");      
      }
  };
  
  struct LICMPass : public FunctionPass {
    static char ID;
    bool codeHasBeenModified = false;
    LICMPass() : FunctionPass(ID) {}

    /**
     * SafeToHoist
     */
    bool safeToHoist(Instruction *inst, Loop *loop) {
      SmallVector<BasicBlock *, 4> exitingBlocks;
      loop->getExitingBlocks(exitingBlocks);

      bool dominatesExiting = true;
      DominatorTree &DT = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
      //Check if the basic block of the instruction dominates every exiting block from the loop
      for (BasicBlock *ex_blk : exitingBlocks) {
        if (!DT.dominates(inst->getParent(), ex_blk)) dominatesExiting = false;
      }
      DEBUG(DebugHelperMethods::printSafeToHoistDebugInfo(inst, dominatesExiting));
      //An instruction is safe to hoist if it has no side effects or dominates all exiting blocks
      return isSafeToSpeculativelyExecute(inst) || dominatesExiting;
    }

    /**
     * IsLoopInvariant
     */
    bool isLoopInvariant(Instruction *inst, Loop *loop) {
      DEBUG(errs() << "Check instruction to be loop invariant: " << inst->getName() << "\n");
      int j = 1; //Used for debugging
      DEBUG(DebugHelperMethods::printLoopInvariantDebugInfo(inst, loop));
      bool couldBeInvariant = (GetElementPtrInst::classof(inst) || SelectInst::classof(inst) || inst->isShift() || inst->isCast() || inst->isBinaryOp());
      if (!couldBeInvariant) return false; //If is not any of those clases we are done checking. If it is, let's check it's operands
      for (auto i = inst->op_begin(); i != inst->op_end(); ++i) {
        DEBUG(errs() << "-- " << j++ << " OP: ");
        DEBUG(DebugHelperMethods::printLoopInvariantDebugInfoOperator(*i, loop));
        if ((Instruction::classof(*i) && loop->contains(cast<Instruction>(*i))) && !Constant::classof(*i)) {
          DEBUG(errs() << " not invariant.\n");
          return false; //Every operand has to be either constant or declared outside of the loop to be considered invariant
        }
        DEBUG(errs() << " invariant.\n");
      }
      DEBUG(errs() << "-- Instruction loop invariant!\n"); //If all operands are invariant, instruction is invariant
      return true;
    }

    /**
     * CheckAndHoistInstruction
     */
    void checkAndHoistInstructionIfPossible(Instruction *inst, Loop *loop) {
      BasicBlock *preheader = loop->getLoopPreheader();
      if (isLoopInvariant(inst, loop) && safeToHoist(inst, loop) && preheader != nullptr) {
        DEBUG(errs() << "-- Hoisting expression: " << inst->getName() << "\n");
        ++numOfHoists;
        codeHasBeenModified = true;
        inst->moveBefore(preheader->getTerminator());
      }
      DEBUG(errs() << "\n");
    }

    /**
     * CheckForLICMInDominatorTreeNode
     */
    void checkForLICMWithDominatorTreeNode(DomTreeNodeBase<BasicBlock>* node, Loop *loop, LoopInfo &info) {
      BasicBlock *blk = node->getBlock();
      //Make sure you only check for blocks inside the natural loop and not in sub-loops
      if (info.getLoopFor(blk) == loop) {
        for (BasicBlock::iterator i = blk->begin(); i != blk->end();) {
          Instruction &inst = *i++;
          checkAndHoistInstructionIfPossible(&inst, loop);
        }
      }
      // Pre-order depth traversal to check each DomTreeNodeBase dominated by current node
      std::vector<DomTreeNodeBase<BasicBlock> *> children = node->getChildren();      
      for (unsigned long i = 0; i < children.size(); ++i) {
        checkForLICMWithDominatorTreeNode(children.at(i), loop, info);
      }
    }

    /**
     * FindBasicBlockInDominatorTree
     */
    DomTreeNodeBase<BasicBlock> *findBasicBlockInDominatorTree(BasicBlock *bb, DomTreeNodeBase<BasicBlock> *domTreeStartingNode){
      //Depth-first traversal looking for DomTreeNodeBase that matches given basic block
      if (domTreeStartingNode->getBlock() == bb) {
        return domTreeStartingNode;
      }
      std::vector<DomTreeNodeBase<BasicBlock> *> children = domTreeStartingNode->getChildren();
      DomTreeNodeBase<BasicBlock> *newNode = nullptr;
      for (unsigned long i = 0; i < children.size(); ++i) {
        newNode = findBasicBlockInDominatorTree(bb, children.at(i));
        if (newNode != nullptr) return newNode;
      }
      return nullptr;
    }

    /**
     * LoopInvariantCodeMotion
     */
    void loopInvariantCodeMotion(Loop *loop, LoopInfo &info) {
      //depth first traversal of sub-loops
      std::vector<Loop*> subLoops = loop->getSubLoops(); 
      for (Loop::iterator j = subLoops.begin(); j != subLoops.end(); ++j) {
        loopInvariantCodeMotion(*j, info);
      }

      DominatorTree &domTree = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
      //Find loop header in dominator tree. Useful because we only care about nodes dominated by loop header.
      DomTreeNodeBase<BasicBlock> *headerNode = findBasicBlockInDominatorTree(loop->getHeader(), domTree.getRootNode());
      //Look for licm in current loop.
      if (headerNode != nullptr) {
        checkForLICMWithDominatorTreeNode(headerNode, loop, info);
      }
    }

    /**
     * RunOnFunction
     */
    bool runOnFunction(Function &F) override {
      DEBUG(DebugHelperMethods::printStartHeader());
      LoopInfo &info = getAnalysis<LoopInfoWrapperPass>().getLoopInfo(); 
      for (LoopInfo::iterator i = info.begin(); i != info.end(); ++i) {
        //Check optimization for all natural loops in function
        loopInvariantCodeMotion(*i, info);
      }
      return codeHasBeenModified;
    }

    /**
     * GetAnalysisUsage
     */
    virtual void getAnalysisUsage(AnalysisUsage& AU) const override {
      AU.setPreservesCFG();
      AU.addRequiredID(LoopSimplifyID); // Preheader and other transforms 
      AU.addRequired <LoopInfoWrapperPass>(); // Identify natural loops 
      AU.addRequired <DominatorTreeWrapperPass>(); // Need dominators for code motion
    }
  };
}

char LICMPass::ID = 0;
static RegisterPass<LICMPass> X("simple-licm", "Loop Invariant Code Motion Pass", false, false);
