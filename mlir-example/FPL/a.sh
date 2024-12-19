build=/Users/niuyibo/workspace/llvm-project/build

# ${build}/bin/mlir-opt mem.mlir \
#  --pass-pipeline='builtin.module(func.func(fpl-study))'

${build}/bin/mlir-opt map.mlir \
 --pass-pipeline='builtin.module(func.func(fpl-study2))'