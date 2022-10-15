//
// Created by 程龙 on 2022-10-15.
//
#include "iostream"
#ifndef UNTITLED1_OUTMATRIX_H
#define UNTITLED1_OUTMATRIX_H

#endif //UNTITLED1_OUTMATRIX_H
void outMatrix(float ** matrix, int rows, int cols);

void outMatrix(float ** matrix, int rows, int cols){
    for (int i = 1; i < rows + 1; ++i) {
        for (int j = 1; j < cols + 1; ++j) {
            std::cout << matrix[i][j] << "\t\t";
        }
        std::cout << " " << std::endl;
    }
}