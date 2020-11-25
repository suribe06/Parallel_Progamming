import pycuda.autoinit
import pycuda.driver as drv
from pycuda.compiler import SourceModule
from pycuda import gpuarray
import numpy as np
from time import time

ker = SourceModule(
    """
    __device__ void swap(int & a, int & b){
        int tmp = a;
        a = b;
        b = tmp;
    }

    __global__ void bitonicSort(int * values, int * ans, int N){
        extern __shared__ int shared[];
    	int tid = threadIdx.x + blockDim.x * blockIdx.x;
        // Copy input to shared mem.
        shared[tid] = values[tid];
        __syncthreads();
        // Parallel bitonic sort.
        for (int k = 2; k <= N; k *= 2){
            // Bitonic merge:
            for (int j = k / 2; j>0; j /= 2){
                int ixj = tid ^ j;
                if (ixj > tid){
                    if ((tid & k) == 0){
                        //Sort ascending
                        if (shared[tid] > shared[ixj]){
                            swap(shared[tid], shared[ixj]);
                        }
                    }
                    else{
                        //Sort descending
                        if (shared[tid] < shared[ixj]){
                            swap(shared[tid], shared[ixj]);
                        }
                    }
                }
                __syncthreads();
            }
        }
        // Write result.
        values[tid] = shared[tid];
        ans[tid] = shared[tid];
    }
    """
)

N = 8 #lenght of A
A = np.int32(np.random.randint(1, 20, N)) #random numbers in A
ans = np.int32(np.zeros(N))
BLOCK_SIZE = 256
NUM_BLOCKS = (N + BLOCK_SIZE - 1) // BLOCK_SIZE
memSize = 4*N
#memSize = A.nbytes
bitonicSort = ker.get_function("bitonicSort")
t1 = time()
bitonicSort(drv.In(A), drv.Out(ans), np.int32(N), block=(BLOCK_SIZE,1,1), grid=(NUM_BLOCKS, 1), shared=memSize)
t2 = time()
print("Execution Time {0}".format(t2 - t1))
print(ans)
