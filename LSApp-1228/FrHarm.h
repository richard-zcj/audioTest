//
// MATLAB Compiler: 8.4 (R2022a)
// Date: Tue Oct 10 15:57:39 2023
// Arguments:
// "-B""macro_default""-W""cpplib:FrHarm,all,version=1.0""-T""link:lib""-d""D:\F
// RTHDNRB\FrHarm\for_testing""-v""D:\FRTHDNRB\FrHarm.m"
//

#ifndef FrHarm_h
#define FrHarm_h 1

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
#ifndef LIB_FrHarm_C_API 
#define LIB_FrHarm_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_FrHarm_C_API 
bool MW_CALL_CONV FrHarmInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_FrHarm_C_API 
bool MW_CALL_CONV FrHarmInitialize(void);

extern LIB_FrHarm_C_API 
void MW_CALL_CONV FrHarmTerminate(void);

extern LIB_FrHarm_C_API 
void MW_CALL_CONV FrHarmPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_FrHarm_C_API 
bool MW_CALL_CONV mlxFrHarm(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_FrHarm
#define PUBLIC_FrHarm_CPP_API __declspec(dllexport)
#else
#define PUBLIC_FrHarm_CPP_API __declspec(dllimport)
#endif

#define LIB_FrHarm_CPP_API PUBLIC_FrHarm_CPP_API

#else

#if !defined(LIB_FrHarm_CPP_API)
#if defined(LIB_FrHarm_C_API)
#define LIB_FrHarm_CPP_API LIB_FrHarm_C_API
#else
#define LIB_FrHarm_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_FrHarm_CPP_API void MW_CALL_CONV FrHarm(int nargout, mwArray& Fr, mwArray& Harmonic, mwArray& Totalnoise, mwArray& Phase, mwArray& Re_rms, const mwArray& stim, const mwArray& response, const mwArray& fs, const mwArray& position, const mwArray& symbol1, const mwArray& symbol2);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
