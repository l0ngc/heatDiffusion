#include<stdio.h>
#include<math.h>
#include "iostream"
#include "outMatrix.h"
#include "calModuls.h"
#include "initMatrix.h"
#define BLOCK_SIZE 16
/* TODO：
 * 1. 读取运行脚本指令，来设置所需要的参数
 * 2. 读取目标文件，来初始化数组
 *      - 第一行为rows和cols
 *      - 第二行为初始点的坐标以及初始值（float）
 * 3. 计算每一步的热量传递
 * 4. 计算平均温度
 * 5. 进行输出
 * add an outer layer of 0
*/

__global__ void gpu_diffuse(const float * nowMatrix, float * nextMatrix, int rows, int cols, float constant)
{
    // every block share same memory
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    int wholeCol = cols + 2;
    float tmpVal = nowMatrix[row * wholeCol + col];

    if(0 < row && row <= rows && col > 0 && col <= cols)
    {
        float surrounding =  (nowMatrix[row * wholeCol + col - 1] +
                        nowMatrix[row * wholeCol + col + 1] +
                        nowMatrix[(row - 1) * wholeCol  + col] +
                        nowMatrix[(row + 1) * wholeCol  + col]) / 4 - tmpVal;

        float nextValue =  tmpVal + constant * surrounding;

        nextMatrix[row * wholeCol + col] = nextValue;
    } 
    __syncthreads();
}


int main(int argc, char *argv[]) {
// 修改为argv[0]以及转换数据类型
    int iterations = 1000;
    float constant = 0.02;

    const int rows = 10000;
    const int cols = 10000;

    int initX = 5000;
    int initY = 5000;

    float initVal = 10000000000;

    float *nowMatrix;
    float *nextMatrix;

    // init host
    nowMatrix = (float*) malloc(sizeof(float) * (rows + 2) * (cols + 2));
    nextMatrix = (float*) malloc(sizeof(float) * (rows + 2) * (cols + 2));

    nowMatrix[(initX + 1) * (cols + 2) + initY + 1] = initVal;

    // init device
    float * cudaNow, *cudaNext;
    cudaMalloc((void**)&cudaNow, sizeof(float) * (rows + 2) * (cols + 2));
    cudaMalloc((void**)&cudaNext, sizeof(float) * (rows + 2) * (cols + 2));

    cudaMemcpy(cudaNow, nowMatrix, sizeof(float) * (rows + 2) * (cols + 2), cudaMemcpyHostToDevice);

    unsigned int grid_rows = (rows + BLOCK_SIZE - 1) / BLOCK_SIZE;
    unsigned int grid_cols = (cols + BLOCK_SIZE - 1) / BLOCK_SIZE;

    dim3 dimGrid(grid_cols, grid_rows);
    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);

    // gpu_diffuse <<<dimGrid, dimBlock>>> (cudaNow, cudaNext, rows, cols, constant);

    cudaMemcpy(nextMatrix, cudaNext, sizeof(float) * (rows + 2) * (cols + 2), cudaMemcpyDeviceToHost);
    
    for(int it = 0; it < iterations; it++){
        gpu_diffuse <<<dimGrid, dimBlock>>> (cudaNow, cudaNext, rows, cols, constant);
        cudaThreadSynchronize();
        cudaNow = cudaNext;
    }
    cudaMemcpy(nextMatrix, cudaNext, sizeof(float) * (rows + 2) * (cols + 2), cudaMemcpyDeviceToHost);

    // print matrix

    // for (int i = 1; i < rows + 1; ++i) {
    //     for (int j = 1; j < cols + 1; ++j) {
    //         std::cout << nextMatrix[i * (cols + 2) + j] << "\t\t";
    //     }
    //     std::cout << " " << std::endl;
    // }

    // cal average temperature
    float matrixAvg = calAvg(nextMatrix, rows, cols);

    // temperature difference
    float matrixAvgDiff = calAvgDiff(nextMatrix, matrixAvg, rows, cols);

    return 0;
}