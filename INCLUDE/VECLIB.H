/*  VecLib.h

  vector management functions.

  Contains the basic definitions for VectorLib.

  Copyright (c) 1996-2002 by OptiCode - Dr. Martin Sander Software Dev.
  All Rights Reserved.
*/

#ifndef __VECLIB_H
#define __VECLIB_H

/*    #define V_trapIntError 1   */
           /*  activate this line, if you wish to generate an error
               message and/or terminate program execution in case
               of INTEGER OVERFLOW or INTEGER DOMAIN errors!
               Additionally, you have to make one call to
               V_setIntErrorHandling with either ierrNote or
               ierrAbort as argument.    */

#include <stdio.h>
#ifdef __BORLANDC__
       /* the following statements apply to
          VectorLib for Borland C++ */
    #ifdef _Windows
        #define _WINDOWS
        #include <windows.h>
    #else
        #include <alloc.h>
    #endif
    #if defined (V_useHUGE) || defined (__HUGE__)
        #define V_HUGE 1
    #endif   /* use the HUGE library within the memory model LARGE */

    #if defined( V_HUGE )
         #define   V_DATAPTR  huge
         #define   _VFAR      far
         typedef   unsigned long   ui;
    #else
         #if defined __TINY || defined __SMALL__ || defined __MEDIUM__
             #if defined(_RTLDLL) || defined(_CLASSDLL)
                 #error Must use static BC Runtime Library with OptiVec in models TINY, SMALL, MEDIUM
             #endif
             #define   V_DATAPTR near    /* even in case of DS!=SS  */
             #define   _VFAR     near
         #elif defined __FLAT__
             #define  _VFAR
             #define  V_DATAPTR
         #else
             #define   V_DATAPTR far
             #define   _VFAR     far
         #endif
         typedef   unsigned int    ui;
    #endif
    #if (__BORLANDC__ >= 0x450)
         #define __vf _RTLENTRY _EXPFUNC
    #else
         #define __vf  _Cdecl _FARFUNC
    #endif
    #pragma option -a-  /* avoid insertion of dummy bytes */
    typedef  struct { long Lo, Hi; }  quad;  /* "quadruple int", 64 bit */
    #if !defined( _CMATH_DEFS )
       typedef long double  extended; /* Borland C supports IEEE 80-bit real numbers */
       typedef struct {float    Re,  Im;}  fComplex;
       typedef struct {double   Re,  Im;}  dComplex;
       typedef struct {extended Re,  Im;}  eComplex;
       typedef struct {float    Mag, Arg;} fPolar;
       typedef struct {double   Mag, Arg;} dPolar;
       typedef struct {extended Mag, Arg;} ePolar;
       typedef fComplex fcomplex;
       typedef dComplex dcomplex;
       typedef eComplex ecomplex;
       typedef fPolar fpolar;
       typedef dPolar dpolar;
       typedef ePolar epolar;  // tolerate all-lower case
       #define _CMATH_DEFS
    #endif
    #pragma option -a.   /* restore default data packing  */

#else  /* the following definitions apply to
          OptiVec for Microsoft's Visual C++ and
          OptiVec for Powersoft's Optima++ */
    #if !defined(_WIN32)
        #error ERROR: Only Win32 target supported!
    #endif
    #pragma pack(push,1)  /* pack structs without dummy bytes */
    #ifdef _MSC_VER
        #include <windows.h>
    #endif
    #ifndef _WINDOWS
        #define _WINDOWS
    #endif
    #define __vf  __cdecl
    #define   _VFAR
    #define   V_DATAPTR
    typedef  unsigned int  ui;
    typedef  struct { long Lo, Hi; }  quad;  /* "quadruple int", 64 bit */
    #if !defined( _CMATH_DEFS )
       typedef  double extended; /* Visual C++ and Optima++ do not
                                    support 80-bit IEEE numbers.
                                    So make extended equal to double. */
       typedef struct {float    Re,  Im;}  fComplex;
       typedef struct {double   Re,  Im;}  dComplex;
       typedef dComplex                    eComplex;
       typedef struct {float    Mag, Arg;} fPolar;
       typedef struct {double   Mag, Arg;} dPolar;
       typedef dPolar                      ePolar;
       typedef fComplex fcomplex;
       typedef dComplex dcomplex;
       typedef eComplex ecomplex;
       typedef fPolar   fpolar;
       typedef dPolar   dpolar;
       typedef ePolar   epolar;   // tolerate all-lower case
       #define _CMATH_DEFS
    #endif
    #pragma pack(pop)   /* restore default packing */
#endif    /*  Borland C/C++, Visual C++, or Optima++ */

typedef   char           V_DATAPTR *    biVector;
typedef   short          V_DATAPTR *    siVector;
typedef   int            V_DATAPTR *    iVector;
typedef   long           V_DATAPTR *    liVector;
typedef   quad           V_DATAPTR *    qiVector;
typedef   unsigned char  V_DATAPTR *    ubVector;
typedef   unsigned short V_DATAPTR *    usVector;
typedef   unsigned int   V_DATAPTR *    uVector;
typedef   unsigned long  V_DATAPTR *    ulVector;
typedef   ui             V_DATAPTR *    uiVector;
typedef   float          V_DATAPTR *    fVector;
typedef   double         V_DATAPTR *    dVector;
typedef   extended       V_DATAPTR *    eVector;
typedef   fComplex       V_DATAPTR *    cfVector;
typedef   dComplex       V_DATAPTR *    cdVector;
typedef   eComplex       V_DATAPTR *    ceVector;
typedef   fPolar         V_DATAPTR *    pfVector;
typedef   dPolar         V_DATAPTR *    pdVector;
typedef   ePolar         V_DATAPTR *    peVector;
#undef V_DATAPTR

#ifdef __cplusplus
extern "C" {
#endif

/** Allocation of memory is specific for the respective data type and
    is consequently included in VFstd.h etc.                            */

/*    Freeing memory is the same for all data types: */
void  __vf  V_free( void _VFAR *X );
void  __vf  V_freeAll( void );  /* frees all vectors allocated by V.._vector */
void  __vf  V_nfree( unsigned numfree,...); /* e.g., V_nfree( 2, X, Y ); */


/************  Controlling coprocessor accuracy  ******************/
void     __vf V_setFPAccuracy(unsigned level);/*1:float, 2:double, 3:extended*/
unsigned __vf V_getFPAccuracy(void);

/********  user-accessible error handling functions ***************/

void  __vf  V_noteError( char _VFAR *fname, unsigned why );
void  __vf  V_printErrorMsg( char _VFAR *ErrMsg );
void  __vf  V_setErrorEventFile( char _VFAR *filename,  unsigned ScreenAndFile );
void  __vf  V_closeErrorEventFile( void );


#if defined __BORLANDC__
    /*** translation of calls to matherr() into _matherr() for BorlandC 4.0+ ***/
    #if (__BORLANDC__ >= 0x450) && !defined (__FLAT__)
        #if !defined( __MATH_H )
            #include <math.h>
        #endif
        int  _Cdecl _FARFUNC matherr (struct exception _VFAR *__e);
        #define NEWMATHERR  \
            int matherr( struct exception _VFAR *__e ) \
            {  return( _matherr( __e )); }
    #else   /* either FLAT model or older versions of Borland C++ */
        #define NEWMATHERR
    #endif
#else  /* for Visual C++ and Optima++  */
    #define NEWMATHERR
#endif


/************ Integer OVERFLOW handling **************************/

typedef enum
{
         ierrIgnore = 0,
         ierrNote,
         ierrAbort
}   V_ihand;

void __vf  V_setIntErrorHandling( V_ihand a );

#if defined __cplusplus
}   // end of extern "C" statement
#endif

        /*  constructors of complex data types:   */
fComplex __vf fcplx( float    __ReVal,  float    __ImVal );
dComplex __vf dcplx( double   __ReVal,  double   __ImVal );
fPolar   __vf fpolr( float    __MagVal, float    __ArgVal );
dPolar   __vf dpolr( double   __MagVal, double   __ArgVal );
#ifdef __BORLANDC__
   eComplex __vf ecplx( extended __ReVal,  extended __ImVal);
   ePolar   __vf epolr( extended __MagVal, extended __ArgVal );
#else  /* Visual C++, Optima++ */
   #define ecplx dcplx
   #define epolr dpolr
#endif
     /*  for overloaded constructors and for all "non-vector"
         operations on complex numbers, see CMATH with the
         include-files <newcplx.h> and <cmath.h> !          */

#endif  /*  __VECLIB_H  */
