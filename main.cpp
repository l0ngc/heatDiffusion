#include<stdio.h>
#include<math.h>
#include "iostream"
#include "outMatrix.h"
#include "calModuls.h"
#include "initMatrix.h"
using namespace std;
/* TODO：
 * 1. 读取运行脚本指令，来设置所需要的参数
 * 2. 读取目标文件，来初始化数组
 *      - 第一行为rows和cols
 *      - 第二行为初始点的坐标以及初始值（float）
 * 3. 计算每一步的热量传递
 * 4. 计算平均温度
 * 5. 进行输出
*/


int main(int argc, char *argv[]) {
// 修改为argv[0]以及转换数据类型
    int iterations = 100;
    float constant = 0.01;

    const int rows = 100;
    const int cols = 100;

    int initX = 50;
    int initY = 50;

    float initVal = 1000000;
    // 矩阵扩增一圈，并只处理这一圈以内对数据
    float ** heatMatrix = mallocArray(rows + 2, cols + 2);
    float ** nextMatrix = mallocArray(rows + 2, cols + 2);

    heatMatrix[initX + 1][initY + 1] = initVal;

    // iterations
    for (int it = 0; it < iterations; ++it) {
        // rows
        for (int i = 1; i < rows + 1; ++i) {
            // cols
            for (int j = 1; j < cols + 1; ++j) {
                diffuse(i, j, heatMatrix, nextMatrix, constant);
            }
        }
        // transfer
        heatMatrix = nextMatrix;
    }

    // print matrix
    outMatrix(nextMatrix, rows, cols);

    // cal average temperature
    float matrixAvg = calAvg(nextMatrix, rows, cols);

    // temperature difference
    float matrixAvgDiff = calAvgDiff(nextMatrix, matrixAvg, rows, cols);

    return 0;
}





