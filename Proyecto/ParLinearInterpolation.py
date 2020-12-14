import pycuda.autoinit
import pycuda.driver as drv
from pycuda.compiler import SourceModule
from sys import stdin
import numpy as np
from time import time
import random

def read_data():
    n = 8023
    data_x = [None for _ in range(n)]
    data_y = [None for _ in range(n)]
    for i in range(n):
        line = stdin.readline().split()
        data_x[i] = float(line[0])
        data_y[i] = float(line[1])
    return data_x, data_y

def ran_bd():
    n = 1000000
    data_x = [None for _ in range(n)]
    data_y = [None for _ in range(n)]
    for i in range(n):
      data_x[i] = int(i+1)
      data_y[i] = random.randint(900,4000)
    return data_x, data_y

def interpolation(data_x, data_y):
    assert len(data_x) == len(data_y)
    n = len(data_x)
    ker = SourceModule("""
        __global__ void linear_interpo(float *A1, float *A2, int n, float *x, float *y){
        	int i = threadIdx.x+ blockDim.x * blockIdx.x;
        	float m = (y[i+1] - y[i]) / (x[i+1] - x[i]);
        	float b = y[i] - (m * x[i]);
        	A1[i] = m;
        	A2[i] = b;
        }
    """)
    A1 = np.float32(np.zeros(n-1))
    A2 = np.float32(np.zeros(n-1))
    BLOCK_SIZE = 1024
    GRID_SIZE = (n + BLOCK_SIZE-1)//BLOCK_SIZE
    linear_interpo = ker.get_function('linear_interpo')
    t1 = time()
    linear_interpo(drv.Out(A1), drv.Out(A2), np.int32(n), drv.In(data_x), drv.In(data_y), block=(BLOCK_SIZE,1,1), grid=(GRID_SIZE,1))
    t2 = time()
    print("Execution Time {0}".format(t2-t1))

    #return list(A1), list(A2)
    return

def main():
    #data_x, data_y = read_data()
    data_x, data_y = ran_bd()
    interpolation(np.float32(data_x), np.float32(data_y))
    return

main()
