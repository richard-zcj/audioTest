//
// MATLAB Compiler: 8.4 (R2022a)
// Date: Thu May  4 16:18:40 2023
// Arguments:
// "-B""macro_default""-W""cpplib:Fr,all,version=1.0""-T""link:lib""-d""D:\MC\Fr
// \for_testing""-v""D:\MC\Fr.m"
//

#ifndef Fr_h
#define Fr_h 1

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
#ifndef LIB_Fr_C_API 
#define LIB_Fr_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_Fr_C_API 
bool MW_CALL_CONV FrInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_Fr_C_API 
bool MW_CALL_CONV FrInitialize(void);

extern LIB_Fr_C_API 
void MW_CALL_CONV FrTerminate(void);

extern LIB_Fr_C_API 
void MW_CALL_CONV FrPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_Fr_C_API 
bool MW_CALL_CONV mlxFr(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_Fr
#define PUBLIC_Fr_CPP_API __declspec(dllexport)
#else
#define PUBLIC_Fr_CPP_API __declspec(dllimport)
#endif

#define LIB_Fr_CPP_API PUBLIC_Fr_CPP_API

#else

#if !defined(LIB_Fr_CPP_API)
#if defined(LIB_Fr_C_API)
#define LIB_Fr_CPP_API LIB_Fr_C_API
#else
#define LIB_Fr_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_Fr_CPP_API void MW_CALL_CONV Fr(int nargout, mwArray& result, const mwArray& stim, const mwArray& response, const mwArray& fs, const mwArray& position, const mwArray& sen);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
