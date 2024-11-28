func.func @producer_consumer_fusion(%arg0: memref<10xf32>, %arg1: memref<10xf32>) {
    %0 = memref.alloc() : memref<10xf32>
    %1 = memref.alloc() : memref<10xf32>
    %cst = arith.constant 0.000000e+00 : f32
    affine.for %arg2 = 0 to 10 {
        affine.store %cst, %0[%arg2] : memref<10xf32>
        affine.store %cst, %1[%arg2] : memref<10xf32>
    }
    return
}