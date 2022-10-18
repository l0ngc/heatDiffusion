#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <string.h>
void diffuse(int posX, int posY, int rows, int cols, float * nowMatrix, float * nextMatrix, float constant);

int main(int argc, char ** argv) {
	MPI_Init(&argc, &argv);
	MPI_Status status;
    
	// get the number of processes
	int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
	// get the rank of process
	int myrank;
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	// init task parameter and Matrix
	int iterations = 2;
	int initX = 1;
	int initY = 1;
	float constant = 0.03333333;
	float initVal = 1000000;
    const int rows = 3;
    const int cols = 3;
	const int wholeSize = (rows + 2) * (cols + 2);
	const int wholeCol = cols + 2;

    float *nowMatrix, *nextMatrix, *tmp_matrix, *recv;
	int *displs, *recvCount;
	// nowMatrix
	nowMatrix = (float*)malloc(sizeof(float) * wholeSize);
	memset(nowMatrix, 0, sizeof(float) * wholeSize);
	nowMatrix[(initX + 1) * (cols + 2) + initY + 1] = initVal;

	// nextMatrix
	nextMatrix = (float*)malloc(sizeof(float) * wholeSize);
	memset(nextMatrix, 0, sizeof(float) * wholeSize);
	// divide work
	int threadWidth = ceil((rows + 2) / size);

	int rowStart = myrank * threadWidth;
	int rowEnd = rowStart + threadWidth;
	if(myrank == size - 1){
		rowEnd = rows + 2 ;
	}
	// init tmpArray
	int tmpArrSize = (rowEnd - rowStart) * wholeCol;
	tmp_matrix = (float*) malloc(sizeof(float) * tmpArrSize);
	// init displs
	displs = (int *) malloc(sizeof(int) * size);
	for(int i = 0; i < size; i++)
		displs[i] = i * threadWidth * wholeCol;

	// init recvCount
	recvCount = (int *)malloc(sizeof(int) * size);
	for(int i = 0; i < size - 1; i++)
		recvCount[i] = threadWidth * wholeCol;
	recvCount[size - 1] = (rows + 2 - (size - 1) * threadWidth) * wholeCol;
	
	for(int it = 0; it < iterations; it++){	
		for(int i  = rowStart; i < rowEnd; i++){
			if(i != 0 && i < rows + 1){
				for(int j = 1; j < cols + 1; j++){
					// printf("computing rows %d\n", j);
					diffuse(i, j, rows, cols, nowMatrix, nextMatrix, constant);
				}
			}
		}
		
		memcpy(tmp_matrix, &nextMatrix[rowStart* (cols + 2)], sizeof(float) * tmpArrSize);

		MPI_Barrier(MPI_COMM_WORLD);

		recv = (float*)malloc(sizeof(float) * wholeSize);
		
		MPI_Allgatherv(tmp_matrix, tmpArrSize, MPI_FLOAT, recv, recvCount, displs, MPI_FLOAT, MPI_COMM_WORLD);

		memcpy(nowMatrix, recv, sizeof(float) * wholeSize);
		//now it is not full share
	}

	if(myrank == 0){
		for(int i = 1; i < rows + 1; i++){
			for(int j = 1; j < cols + 1; j++){
				printf("%f\t", recv[i * (cols + 2) + j]);
				// printf("%f\t", recv[i * (cols + 2) + j]);
			}
			printf("\n");
		}
	}
	
	MPI_Finalize();
}

void diffuse(int posX, int posY, int rows, int cols, float * nowMatrix, float * nextMatrix, float constant)
{
    // h(i, j) = h(i, j) + c * ((h(i - 1, j) + h(i + 1, j) + h(i, j - 1) + h(i, j + 1)) / 4 - h(i,j))
	int wholeCol = cols + 2;
    float tmpVal = nowMatrix[posX * wholeCol + posY];
    
    float surrounding =  (nowMatrix[posX * wholeCol + posY - 1] +
                          nowMatrix[posX * wholeCol + posY + 1] +
                          nowMatrix[(posX - 1) * wholeCol  + posY] +
                          nowMatrix[(posX + 1) * wholeCol  + posY]) / 4 - tmpVal;

    float nextValue =  tmpVal + constant * surrounding;
    nextMatrix[posX * wholeCol + posY] = nextValue;
}