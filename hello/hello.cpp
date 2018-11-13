#include "llvm/Analysis/ValueTracking.h"
#include "llvm/Analysis/LazyValueInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Pass.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/ConstantRange.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/KnownBits.h"
#include "llvm/Support/raw_ostream.h"


#include <string>

using namespace llvm;

namespace {
struct HelloWorld : public FunctionPass {
  static char ID;
  HelloWorld() : FunctionPass(ID) {}

  void getAnalysisUsage(AnalysisUsage &Info) const {
    Info.addRequired<LazyValueInfoWrapperPass>();
    Info.addRequired<ScalarEvolutionWrapperPass>();
  }

  bool runOnFunction(Function &F) override {
    auto LVI = &getAnalysis<LazyValueInfoWrapperPass>().getLVI();
    auto SE = &getAnalysis<ScalarEvolutionWrapperPass>().getSE();
    Module *M = F.getParent();
    errs() << "function: ";
    errs().write_escaped(F.getName()) << '\n';
    for (auto &BB : F) {
      for (auto &I : BB) {
        if (!I.getType()->isIntegerTy())
          continue;
        errs() << I << "\n";
        errs() << "    ";
        KnownBits Known = computeKnownBits(&I, M->getDataLayout());
        int W = Known.getBitWidth();
        std::string s;
        APInt Zero = Known.Zero;
        APInt One = Known.One;
        for (int x = 0; x < W; ++x) {
          if (Zero.isSignBitSet())
            errs() << "0";
          else if (One.isSignBitSet())
            errs() << "1";
          else
            errs() << "?";
          Zero <<= 1;
          One <<= 1;
        }
        errs() << "  ";
        errs() << LVI->getConstantRange(&I, &BB);
        errs() << "  ";
        auto SC = SE->getSCEV(&I);
        SC->print(errs());
        errs() << "  " << SE->getSignedRange(SC);
        errs() << "  " << SE->getUnsignedRange(SC);
        errs() << "\n";
      }
    }
    return false;
  }
}; // end of struct Hello
}  // end of anonymous namespace

char HelloWorld::ID = 0;
static RegisterPass<HelloWorld> X("helloworld", "Hello World Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);

namespace llvm {
void initializeHelloWorldPass(llvm::PassRegistry &);
}

INITIALIZE_PASS_BEGIN(HelloWorld, "print-lazy-value-info",
                      "Lazy Value Info Printer Pass", false, false)
INITIALIZE_PASS_DEPENDENCY(LazyValueInfoWrapperPass)
INITIALIZE_PASS_DEPENDENCY(ScalarEvolutionWrapperPass)
INITIALIZE_PASS_END(HelloWorld, "print-lazy-value-info",
                    "Lazy Value Info Printer Pass", false, false)
