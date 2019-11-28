#include "llvm/Pass.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

#include <string>

using namespace llvm;

namespace {

struct HelloWorld : public FunctionPass {
  static char ID;
  HelloWorld() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    Module *M = F.getParent();
    errs() << "helloworld pass: function: ";
    errs().write_escaped(F.getName()) << '\n';
    for (auto &BB : F) {
      for (auto &I : BB) {

	if (auto callInst = dyn_cast<CallInst>(&I)) {
	  Function* calledFunc = callInst->getCalledFunction();
	  errs() << "Called function: " << calledFunc->getName() << "\n";
	}
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
