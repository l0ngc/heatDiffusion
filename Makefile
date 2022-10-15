NVCC=nvcc
NVCC_FLAGS=-O2 -arch=sm_61
NVCC_INC_FLAGS=-I/chalmers/sw/sup64/cuda_toolkit-11.2.2/include
NVCC_LIB_DIR=-L/chalmers/sw/sup64/cuda_toolkit-11.2.2/lib64
NVCC_LIBS=-lcudart

all: cudaHeat dgemv_shmem_linear dgemv_shmem_binary dgemv_shmem_tiled

cudaHeat.o: cudaHeat.cu
	$(NVCC) $(NVCC_FLAGS) $(NVCC_INC_FLAGS) -c cudaHeat.cu -o cudaHeat.o

cudaHeat: cudaHeat.o
	$(NVCC) $(NVCC_FLAGS) $(NVCC_INC_FLAGS) -o cudaHeat $< $(CUDA_LIB_DIR) $(CUDA_LINK_LIBS)

build_pt1: cudaHeat

run_pt1:
	./cudaHeat