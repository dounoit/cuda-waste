#include "emulator.h"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <queue>
#include "tree.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Cuda Runtime API/Emulator implementation.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////

cudaError_t CUDA_EMULATOR::_cudaConfigureCall(dim3 gridDim, dim3 blockDim, size_t sharedMem, cudaStream_t stream)
{
	this->ConfigureBlock(blockDim);
	this->ConfigureGrid(gridDim);
	this->ConfigureSharedMemory(sharedMem);
	this->ConfigureStream(stream);
	return cudaSuccess;
}

cudaError_t CUDA_EMULATOR::_cudaThreadSynchronize()
{
	return cudaSuccess;
}

void ** CUDA_EMULATOR::_cudaRegisterFunction(void * fun, char * name)
{
	std::pair<void*, char*> i;
	i.first = fun;
	i.second = name;
	this->fun_to_name.insert(i);
	return 0;
}

cudaError_t CUDA_EMULATOR::_cudaSetupArgument(const void *arg, size_t size, size_t offset)
{
    // record argument, size, offset.
	CUDA_EMULATOR::arg * a = new CUDA_EMULATOR::arg();
	assert(size == 4);
	a->argument = malloc(size);
	memcpy(const_cast<void*>(a->argument), arg, size);
	a->size = size;
	a->offset = offset;
	this->arguments.push_back(a);
	return cudaSuccess;
}

cudaError_t CUDA_EMULATOR::_cudaLaunch(const char *hostfun)
{
    // Given the address of the kernel function in the host, determine the name of the kernel
    // it is calling in PTX, using information provided by RegisterFatBinary and _cudaRegisterFunction.
	std::map<void*, char*>::iterator i = this->fun_to_name.find((void*)hostfun);
	assert(i != this->fun_to_name.end());
	char * name = i->second;

    // Now, given the name of the kernel function being called, find
    // the entry for it.
	std::map<char*, TREE *, ltstr>::iterator j = this->entry.find(name);
	assert(j != this->entry.end());
	TREE * entry = j->second;
	this->Execute(entry);
	return cudaSuccess;
}

cudaError_t CUDA_EMULATOR::_cudaGetDevice(int * device)
{
	*device = 0;
	return cudaSuccess;
}

void CUDA_EMULATOR::_cudaSetDevice(char * device)
{
	this->device = this->string_table->Entry(device);
}


cudaError_t CUDA_EMULATOR::_cudaGetDeviceProperties(struct cudaDeviceProp *prop, int device)
{
	if (strcmp(this->device, "compute_20") == 0)
	{
		static cudaDeviceProp p = {
			"emulator",                     // char name[256];
			3000000000,                     // size_t totalGlobalMem;
			3000000,                        // size_t sharedMemPerBlock;
			500,                            // int regsPerBlock;
			500,                            // int warpSize;
			10,                             // size_t memPitch;
			16000,                          // int maxThreadsPerBlock;
			{1000,1,1},                     // int maxThreadsDim[3];
			{1000,1,1},                     // int maxGridSize[3];
			111,                            // int clockRate;
			100000000,                      // size_t totalConstMem;
			2,                              // int major;
			0,                              // int minor;
			11,                             // size_t textureAlignment;
			11,                             // int deviceOverlap;
			11,                             // int multiProcessorCount;
			1,                              // int kernelExecTimeoutEnabled;
			1,                              // int integrated;
			1,                              // int canMapHostMemory;
			1                               // int computeMode;
		};
		*prop = p;
	} else
	{
		static cudaDeviceProp p = {
			"emulator",                     // char name[256];
			3000000000,                     // size_t totalGlobalMem;
			3000000,                        // size_t sharedMemPerBlock;
			500,                            // int regsPerBlock;
			500,                            // int warpSize;
			10,                             // size_t memPitch;
			16000,                          // int maxThreadsPerBlock;
			{1000,1,1},                     // int maxThreadsDim[3];
			{1000,1,1},                     // int maxGridSize[3];
			111,                            // int clockRate;
			100000000,                      // size_t totalConstMem;
			1,                              // int major;
			4,                              // int minor;
			11,                             // size_t textureAlignment;
			11,                             // int deviceOverlap;
			11,                             // int multiProcessorCount;
			1,                              // int kernelExecTimeoutEnabled;
			1,                              // int integrated;
			1,                              // int canMapHostMemory;
			1                               // int computeMode;
		};
		*prop = p;
	}
	return cudaSuccess;
}


cudaError_t CUDA_EMULATOR::_cudaStreamCreate(cudaStream_t *pStream)
{
	// nop
	return cudaSuccess;
}

cudaError_t CUDA_EMULATOR::_cudaStreamDestroy(cudaStream_t stream)
{
	// nop
	return cudaSuccess;
}

cudaError_t CUDA_EMULATOR::_cudaStreamSynchronize(cudaStream_t stream)
{
	// nop
	return cudaSuccess;
}

cudaError_t CUDA_EMULATOR::_cudaStreamQuery(cudaStream_t stream)
{
	// nop
	return cudaSuccess;
}

