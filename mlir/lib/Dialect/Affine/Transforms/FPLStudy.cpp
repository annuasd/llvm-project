#include "mlir/Dialect/Affine/Passes.h"

#include "mlir/Dialect/Affine/Analysis/AffineAnalysis.h"
#include "mlir/Dialect/Affine/Analysis/AffineStructures.h"
#include "mlir/Dialect/Affine/Analysis/LoopAnalysis.h"
#include "mlir/Dialect/Affine/Analysis/Utils.h"
#include "mlir/Dialect/Affine/IR/AffineOps.h"
#include "mlir/Dialect/Affine/IR/AffineValueMap.h"
#include "mlir/Dialect/Affine/LoopUtils.h"
#include "mlir/Dialect/Affine/Utils.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/IRMapping.h"
#include "mlir/IR/Visitors.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"

namespace mlir {
namespace affine {
#define GEN_PASS_DEF_FPLSTUDY
#include "mlir/Dialect/Affine/Passes.h.inc"
} // namespace affine
} // namespace mlir

// build=
using namespace mlir;
using namespace mlir::affine;
using namespace mlir::presburger;

#define DEBUG_TYPE "affine-loop-tile"

namespace {
/// A pass to perform loop tiling on all suitable loop nests of a Function.
struct FPLStudy : public affine::impl::FPLStudyBase<FPLStudy> {
  void runOnOperation() override {
    auto f = getOperation();
    AffineForOp top;
    f->walk([&](AffineForOp forOp) {
      top = forOp;
      return WalkResult::interrupt();
    });
    std::vector<SmallVector<DependenceComponent, 2>> depCompsVec;
    auto op1 = *top.getBody()->op_begin<affine::AffineStoreOp>();
    auto op2 = *++top.getBody()->op_begin<affine::AffineStoreOp>();
    MemRefAccess access1(op1);
    MemRefAccess access2(op2);
    SmallVector<DependenceComponent, 2> depComps;
    PresburgerSpace space = PresburgerSpace::getRelationSpace();
    IntegerRelation rel1(space), rel2(space);
    if(failed(access1.getAccessRelation(rel1))) {
      llvm::errs() << "啥情况1" << "\n";
    }
    if(failed(access2.getAccessRelation(rel2))) {
      llvm::errs() << "啥情况2" << "\n";
    }
    
    FlatAffineValueConstraints domain1(rel1.getDomainSet());
    FlatAffineValueConstraints domain2(rel2.getDomainSet());

    llvm::errs() << *op1 << "\n";
    rel1.dump();
    domain1.dump();
    llvm::errs() << *op2 << "\n";
    rel2.dump();    
    domain2.dump();
    exit(0);
  }
};

} // namespace
// transpose-ln-add-transpose
namespace mlir {
std::unique_ptr<Pass> mlir::affine::createFPLStudyPass() {
  return std::make_unique<FPLStudy>();
}
} // namespace mlir
