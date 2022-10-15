//
// Created by 程龙 on 2022-10-15.
//

#include <cstdlib>

#ifndef UNTITLED1_INITMATRIX_H
#define UNTITLED1_INITMATRIX_H

#endif //UNTITLED1_INITMATRIX_H

void readConf();
float ** mallocArray(int rows, int cols);

void readConf(){
    // 这写个函数，从文件中读取相关参数

}

float ** mallocArray(int rows, int cols) {
    // 初始化二维数组并赋值
    float **arr = (float **) malloc(sizeof(float *) * rows);
    for (int i = 0; i < rows; i ++) {
        arr[i] = (float *) malloc(sizeof(float *) * cols);
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            arr[i][j] = 0;
        }
    }
    return arr;
}

