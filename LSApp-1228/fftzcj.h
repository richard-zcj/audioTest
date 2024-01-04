//
// MATLAB Compiler: 8.4 (R2022a)
// Date: Tue Jul  4 15:01:34 2023
// Arguments:
// "-B""macro_default""-W""cpplib:fftzcj,all,version=1.0""-T""link:lib""-d""D:\t
// emp\Xiamen\fftzcj\for_testing""-v""D:\temp\Xiamen\fftzcj.m"
//

#ifndef fftzcj_h
#define fftzcj_h 1

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
#ifndef LIB_fftzcj_C_API 
#define LIB_fftzcj_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_fftzcj_C_API 
bool MW_CALL_CONV fftzcjInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_fftzcj_C_API 
bool MW_CALL_CONV fftzcjInitialize(void);

extern LIB_fftzcj_C_API 
void MW_CALL_CONV fftzcjTerminate(void);

extern LIB_fftzcj_C_API 
void MW_CALL_CONV fftzcjPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_fftzcj_C_API 
bool MW_CALL_CONV mlxFftzcj(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_fftzcj
#define PUBLIC_fftzcj_CPP_API __declspec(dllexport)
#else
#define PUBLIC_fftzcj_CPP_API __declspec(dllimport)
#endif

#define LIB_fftzcj_CPP_API PUBLIC_fftzcj_CPP_API

#else

#if !defined(LIB_fftzcj_CPP_API)
#if defined(LIB_fftzcj_C_API)
#define LIB_fftzcj_CPP_API LIB_fftzcj_C_API
#else
#define LIB_fftzcj_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_fftzcj_CPP_API void MW_CALL_CONV fftzcj(int nargout, mwArray& acf, mwArray& acap, const mwArray& in);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
