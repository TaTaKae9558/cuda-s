%%cuda
#include <stdio.h>
#include <cuda.h>
#include <device_launch_parameters.h>
#include <cuda_runtime.h>

__global__ void sum(int *a, int *b, int *c, int size) {
  int i = threadIdx.x;
  if (i < size) {
    c[i] = a[i] + b[i];
  }
}

int main() {
  int *a_cpu, *b_cpu, *c_cpu;
  int size = 500;
  a_cpu = (int*)malloc(sizeof(int) * size);
  b_cpu = (int*)malloc(sizeof(int) * size);
  c_cpu = (int*)malloc(sizeof(int) * size);

  for (int i = 0; i < size; i++) {
    a_cpu[i] = i;
    b_cpu[i] = i;

  }

  int *a_gpu, *b_gpu, *c_gpu;
  cudaMalloc( (void **)&a_gpu, size * sizeof(int));
  cudaMalloc( (void **)&b_gpu, size *sizeof(int));
  cudaMalloc( (void **)&c_gpu, size * sizeof(int));

  cudaMemcpy(a_gpu, a_cpu , size * sizeof(int), cudaMemcpyHostToDevice);
  cudaMemcpy(b_gpu,b_cpu,size * sizeof(int) ,cudaMemcpyHostToDevice);

  dim3 grid(1, 1, 1);
  dim3 block(size, 1, 1);
  sum<<< grid, block >>>(a_gpu, b_gpu, c_gpu, size);

    cudaMemcpy(c_cpu, c_gpu , size * sizeof(int), cudaMemcpyDeviceToHost) ;
    cudaFree( a_gpu );
    cudaFree( b_gpu );
    cudaFree( c_gpu );

   for (int y = 0; y < size; y++) {
     printf("%d : %d \n", y, c_cpu[y]);
   }

   free(a_cpu);
   free(b_cpu);
   free(c_cpu);
}

