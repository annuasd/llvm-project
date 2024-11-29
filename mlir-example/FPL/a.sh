build=/Users/niuyibo/workspace/llvm-project/build
${build}/bin/mlir-opt /Users/niuyibo/workspace/llvm-project/mlir-example/FPL/for.mlir \
 --pass-pipeline='builtin.module(func.func(fpl-study))'