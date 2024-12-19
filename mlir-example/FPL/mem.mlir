func.func @producer_consumer_fusion(%arg0: memref<10x224x3xf32>, %arg1: memref<10x224x3xf32>) {
    %0 = memref.alloc() : memref<10xf32>
    %cst = arith.constant 0.000000e+00 : f32
    affine.for %arg2 = 0 to 10 {
      affine.for %arg3 = 0 to 224 {
        affine.for %arg4 = 0 to 3 {
          affine.store %cst, %arg0[%arg2, %arg3, %arg4] : memref<10x224x3xf32>
          affine.store %cst, %0[%arg2] :  memref<10xf32>
          affine.store %cst, %arg0[%arg2, %arg3, %arg4] : memref<10x224x3xf32>
        }
      }
    }
    return
}

