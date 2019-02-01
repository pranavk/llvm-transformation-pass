#include "llvm/Analysis/DemandedBits.h"
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
struct RemoveOptnone : public FunctionPass {
  static char ID;
  RemoveOptnone() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    if (F.hasFnAttribute(Attribute::OptimizeNone))
      llvm::outs() << F.getName() << " has optnone\n";
    F.removeFnAttr(Attribute::OptimizeNone);
    F.removeFnAttr(Attribute::NoInline);
    return true;
  }
}; // end of struct Hello
}  // end of anonymous namespace

char RemoveOptnone::ID = 0;
static RegisterPass<RemoveOptnone> X("remove-optnone", "Remove optnone Pass",
                                     false, false);

namespace llvm {
void initializeRemoveOptnonePass(llvm::PassRegistry &);
}

