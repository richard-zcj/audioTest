//
// MATLAB Compiler: 8.4 (R2022a)
// Date: Mon Nov 20 15:59:54 2023
// Arguments:
// "-B""macro_default""-W""cpplib:wav,all,version=1.0""-T""link:lib""-d""D:\zcj\
// zcjfft\wav\for_testing""-v""D:\zcj\zcjfft\wav.m"
//

#ifndef wav_h
#define wav_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" { // sbcheck:ok:extern_c
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_wav_C_API 
#define LIB_wav_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_wav_C_API 
bool MW_CALL_CONV wavInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_wav_C_API 
bool MW_CALL_CONV wavInitialize(void);

extern LIB_wav_C_API 
void MW_CALL_CONV wavTerminate(void);

extern LIB_wav_C_API 
void MW_CALL_CONV wavPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_wav_C_API 
bool MW_CALL_CONV mlxWav(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_wav
#define PUBLIC_wav_CPP_API __declspec(dllexport)
#else
#define PUBLIC_wav_CPP_API __declspec(dllimport)
#endif

#define LIB_wav_CPP_API PUBLIC_wav_CPP_API

#else

#if !defined(LIB_wav_CPP_API)
#if defined(LIB_wav_C_API)
#define LIB_wav_CPP_API LIB_wav_C_API
#else
#define LIB_wav_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_wav_CPP_API void MW_CALL_CONV wav(int nargout, mwArray& ret, const mwArray& path, const mwArray& col);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
