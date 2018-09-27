#include "llvm/Analysis/ValueTracking.h"
#include "llvm/Pass.h"
#include "llvm/IR/BasicBlock.h"
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

  bool runOnFunction(Function &F) override {
    Module *M = F.getParent();
    errs() << "printing known bits at return points from function: ";
    errs().write_escaped(F.getName()) << '\n';
    for (auto &BB : F) {
      for (auto &I : BB) {
	if (I.getOpcode() == Instruction::Ret) {
	  errs() << "  ";
	  KnownBits Known = computeKnownBits(I.getOperand(0), M->getDataLayout());
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
	  errs() << "\n";
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
