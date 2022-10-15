//
// Created by 程龙 on 2022-10-15.
//

#ifndef UNTITLED1_CALMODULS_H
#define UNTITLED1_CALMODULS_H

#endif //UNTITLED1_CALMODULS_H
#include "iostream"

using namespace std;
void diffuse(int posX, int posY, float ** nowMatrix, float ** nextMatrix, float constant)
{
    // h(i, j) = h(i, j) + c * ((h(i - 1, j) + h(i + 1, j) + h(i, j - 1) + h(i, j + 1)) / 4 - h(i,j))
    float tmpVal = nowMatrix[posX][posY];
    float surrounding =  (nowMatrix[posX - 1][posY] +
                          nowMatrix[posX + 1][posY] +
                          nowMatrix[posX][posY - 1] +
                          nowMatrix[posX][posY + 1]) / 4 - tmpVal;
    float nextValue =  tmpVal + constant * surrounding;
    nextMatrix[posX][posY] = nextValue;
}

float calAvg(float ** matrix, int rows, int cols){
    long double sum = 0;
    for (int i = 1; i < rows + 1; ++i) {
        for (int j = 1; j < cols + 1; ++j) {
            sum += matrix[i][j];
        }
    }

    float matrixAvg = sum / rows / cols;
    cout << "Avg of the matrix is " << matrixAvg <<endl;
    return matrixAvg;
}

float calAvgDiff(float ** matrix, float avg, int rows, int cols){
    long double avgDiff = 0;
    for (int i = 1; i < rows + 1; ++i) {
        for (int j = 1; j < cols + 1; ++j) {
            avgDiff += abs(avg - matrix[i][j]);
        }
    }

    float matrixAvgDiff = avgDiff / rows / cols;
    cout << "Average absolute difference of each temperature to the average of all temperatures is  " << matrixAvgDiff <<endl;
    return matrixAvgDiff;
}
