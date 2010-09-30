/*
   Copyright 2010 Ken Domino

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#pragma once

#include <vector>
#include <cuda.h>
#include <cuda_runtime.h> // cudaError_t, CUDARTAPI, etc.
#include "_cuda.h"
#include "_cuda_runtime.h"

class HookManager;

#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

/// Wrapper for CUDA's dynamic link library.
#ifndef __dv
#define __dv(x)
#endif


class DLL_API CUDA_WRAPPER
{
public:
    struct data
    {
        void * ptr;
        bool is_host;
        int size;
        char * context;
    };
    friend std::vector<data>;
private:
    CUDA_WRAPPER();
    static CUDA_WRAPPER * singleton;
public:
    HookManager * hook_manager;

        size_t padding_size;
    unsigned char padding_byte;
    bool device_pointer_to_first_byte_in_block;
    std::ostream * output_stream;
    bool trace_all_calls;
    bool quit_on_error;
    bool do_not_call_cuda_after_sanity_check_fail;
    bool do_emulation;
    bool do_debug_halt;
    bool init;
    static void ExitHandler();
    std::vector<data> alloc_list;
    static char * Context(int lines = 1);
    char * global_context;
    char * device; // device to run.
    _CUDA * _cuda;
	_CUDA_RUNTIME * _cuda_runtime;

public:
    static CUDA_WRAPPER * Singleton();
    void DoInit();
    bool WrapModule(char * modname);
    static BOOL CUDARTAPI WrapCuda();
    static void MakeContext(char * file_name, int line);

    enum return_type {
        NOT_OK = 0,
        OK = 1
    };
    static return_type CheckOverwrite();
    static return_type CUDARTAPI SetPaddingSize(size_t s);
    static return_type CUDARTAPI SetPaddingByte(unsigned char b);
    static return_type CUDARTAPI SetDevicePointerToFirstByteInBlock(bool b);
    static return_type CUDARTAPI SetOutputStream(std::ostream * fp);
    static return_type CUDARTAPI SetTraceAllCalls(bool b);
    static return_type CUDARTAPI SetQuitOnError(bool b);
    static return_type CUDARTAPI SetDoNotCallCudaAfterSanityCheckFail(bool b);
    static void CUDARTAPI SetEmulationMode(int yes_no);
    static return_type CopyOptions(CUDA_WRAPPER * ptr);
    static return_type CUDARTAPI RunDevice(char * device);
    static void CUDARTAPI SetTrace(int level);

    static return_type CheckSinglePtrOverwrite(const data * d);
    static bool IsBadPointer(const void * ptr);
    static int FindAllocatedBlock(const void * pointer);

};

