#include "llvm/Pass.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/PassRegistry.h"

#include "llvm/PassSupport.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/LegacyPassManager.h"


#include <string>

using namespace llvm;

namespace {

struct HelloWorld : public FunctionPass {
  static char ID;
  HelloWorld() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    Module *M = F.getParent();
    //errs() << "helloworld pass: function: ";
    //errs().write_escaped(F.getName()) << '\n';
    std::vector<Instruction*> vec;
    for (auto &BB : F) {
      for (auto &I : BB) {

	if (auto callInst = dyn_cast<CallInst>(&I)) {
	  Function* calledFunc = callInst->getCalledFunction();
	  if (calledFunc && (calledFunc->getName() == "mutex_lock" || calledFunc->getName() == "mutex_lock_nested" || calledFunc->getName() == "mutex_unlock")) {
	    errs() << calledFunc->getName() << "\n";
	    vec.push_back(&I);
	    //I.eraseFromParent();
	  }
	}
      }
    }

    for (auto I : vec) {
      I->eraseFromParent();
    }

    return false;
  }
}; // end of struct Hello
}  // end of anonymous namespace

char HelloWorld::ID = 0;
static RegisterPass<HelloWorld> X("helloworld", "Hello World Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);

static void registerHostBlockProfilerPass(const PassManagerBuilder &,
                                            legacy::PassManagerBase &PM) {
  PM.add(new HelloWorld());
}

// Register Host Pass early so the analysis will work better
static RegisterStandardPasses
    RegisterHostPass(PassManagerBuilder::EP_ModuleOptimizerEarly,
                   registerHostBlockProfilerPass);

static RegisterStandardPasses
    RegisterHostPass0(PassManagerBuilder::EP_EnabledOnOptLevel0, registerHostBlockProfilerPass);
