// 3x224x224 -> 224x224x3
#map1 = affine_map<(i0,i1,i2)[s0,s1,s2] -> (i0*s1*s2+i1*s2+i2)>
func.func @perm(%arg0: memref<150528xf32>) {
    %d0 = arith.constant 3 : index
    %d1 = arith.constant 224 : index
    %d2 = arith.constant 224 : index
    affine.for %i0 = 0 to 3 {
      affine.for %i1 = 0 to 224 {
        affine.for %i2 = 0 to 224 {
          %idx = affine.apply #map1 (%i0,%i1,%i2)[%d0,%d1,%d2]
          %1 = affine.load %arg0[%idx] : memref<150528xf32>
        }
      }
    }
    return
}