#include <metal_stdlib>

using namespace metal;

kernel void add(device float *A,
                device float *B,
                device float *result,
                uint index[[thread_position_in_grid]]) {

  result[index] = A[index] + B[index];
}
