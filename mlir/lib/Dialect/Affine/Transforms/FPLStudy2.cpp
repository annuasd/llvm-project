#include "mlir/Analysis/Presburger/IntegerRelation.h"
#include "mlir/Analysis/Presburger/PresburgerSpace.h"
#include "mlir/Dialect/Affine/Passes.h"

#include "mlir/Dialect/Affine/Analysis/AffineAnalysis.h"
#include "mlir/Dialect/Affine/Analysis/AffineStructures.h"
#include "mlir/Dialect/Affine/Analysis/LoopAnalysis.h"
#include "mlir/Dialect/Affine/Analysis/Utils.h"
#include "mlir/Dialect/Affine/IR/AffineOps.h"
#include "mlir/Dialect/Affine/LoopUtils.h"
#include "mlir/Dialect/Affine/Utils.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinAttributes.h"
#include "mlir/IR/BuiltinTypeInterfaces.h"
#include "mlir/IR/Value.h"
#include "mlir/IR/Visitors.h"
#include "llvm/Support/Debug.h"

namespace mlir {
namespace affine {
#define GEN_PASS_DEF_FPLSTUDY2
#include "mlir/Dialect/Affine/Passes.h.inc"
} // namespace affine
} // namespace mlir

using namespace mlir;
using namespace mlir::affine;
using namespace mlir::presburger;

namespace {
/// A pass to perform loop tiling on all suitable loop nests of a Function.
struct FPLStudy2 : public affine::impl::FPLStudy2Base<FPLStudy2> {
  void runOnOperation() override {
    auto f = getOperation();
    
    AffineForOp top;
    f->walk([&](AffineForOp forOp) {
      top = forOp;
      return WalkResult::interrupt();
    });
    // auto apply = *top.getBody()->op_begin<affine::AffineLoadOp>();
    auto op = *top.getBody()->op_begin<AffineLoadOp>();
    MemRefAccess access(op);
    PresburgerSpace space = PresburgerSpace::getRelationSpace();
    IntegerRelation rel(space);
    if (failed(access.getAccessRelation(rel))) {
      llvm::errs() << "啥情况2" << "\n";
    }
    FlatAffineValueConstraints domain(rel.getDomainSet());

    llvm::errs() << *op << "\n";
    rel.dump();
    domain.dump();

    auto loopNums = domain.getNumDimVars();
    llvm::errs() << "domain:" << "\n";
    for (unsigned i = 0; i < loopNums; ++i) {
      auto dimVar = domain.getValue(i);
      llvm::errs()
          << *llvm::cast<BlockArgument>(dimVar).getOwner()->getParentOp()
          << "\n";
    }
    domain.getNumLocalVars();
    auto local = domain.getVarKindOffset(VarKind::Local);
    auto idxNums = domain.getNumLocalVars();
    for (unsigned i = 0; i < idxNums; ++i) {
      llvm::errs() << "local:" << i << "\n";
      auto ub = domain.getConstantBound(BoundType::UB, local + i);
      auto lb = domain.getConstantBound(BoundType::LB, local + i);
      if (ub.has_value())
        llvm::errs() << "ub" << *ub << "\n";
      if (lb.has_value())
        llvm::errs() << "lb" << *lb << "\n";
    }
  }
};

} // namespace
// transpose-ln-add-transpose
namespace mlir {
std::unique_ptr<Pass> mlir::affine::createFPLStudy2Pass() {
  return std::make_unique<FPLStudy2>();
}
} // namespace mlir
