#include<stdio.h>
#include<math.h>
#include "iostream"
#include "outMatrix.h"
#include "calModuls.h"
#include "initMatrix.h"
/* TODO：
 * 1. 读取运行脚本指令，来设置所需要的参数
 * 2. 读取目标文件，来初始化数组
 *      - 第一行为rows和cols
 *      - 第二行为初始点的坐标以及初始值（float）
 * 3. 计算每一步的热量传递
 * 4. 计算平均温度
 * 5. 进行输出
*/
// 做出一个一个维度的版本，一个维度的话怎么来处理边界值呢
void diffuse(int posX, int posY, float * nowMatrix, float * nextMatrix, float constant);

int main(int argc, char *argv[]) {
// 修改为argv[0]以及转换数据类型
    int iterations = 1;
    float constant = 0.0333333;

    const int rows = 5;
    const int cols = 5;

    int initX = 2;
    int initY = 2;

    float initVal = 1000000;

    float *nowMatrix;
    float *nextMatrix;

    nowMatrix = (float*) malloc(sizeof(float) * (rows + 2) * (cols + 2));
    nextMatrix = (float*) malloc(sizeof(float) * (rows + 2) * (cols + 2));

    nowMatrix[(initX + 1) * cols + initY + 1] = initVal;

    // iterations
    for (int it = 0; it < iterations; ++it) {
        // rows
        for (int i = 1; i < rows + 1; ++i) {
            // cols
            for (int j = 1; j < cols + 1; ++j) {
                diffuse(i, j, rows, cols, nowMatrix, nextMatrix, constant);
            }
        }
        // transfer
//        heatMatrix = nextMatrix;
    }

    // print matrix
    for (int i = 1; i < rows + 1; ++i) {
        for (int j = 1; j < cols + 1; ++j) {
            std::cout << nextMatrix[i * cols + j] << "\t\t";
        }
        std::cout << " " << std::endl;
    }

    // cal average temperature
    float matrixAvg = calAvg(nextMatrix, rows, cols);

    // temperature difference
    float matrixAvgDiff = calAvgDiff(nextMatrix, matrixAvg, rows, cols);

    return 0;
}






