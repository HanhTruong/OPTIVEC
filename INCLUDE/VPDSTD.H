/*  VPDstd.h

  vector management functions:
  manipulations on whole arrays or vectors of data type dPolar
  (double-precision polar complex numbers)

  Copyright (c) 1996-2002 by OptiCode - Dr. Martin Sander Software Dev.
  All Rights Reserved.
*/

#ifndef __VPDSTD_H
#define __VPDSTD_H
#if !defined( __VECLIB_H )
#include <VecLib.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*************************  Generation  **************************/

pdVector   __vf  VPD_vector( ui size );
pdVector   __vf  VPD_vector0( ui size );

/***************  Addressing single vector elements ******************/

dPolar _VFAR * __vf VPD_Pelement( pdVector X, ui n );
      /* returns a pointer to the n'th element of X. For the memory model
         HUGE, the pointer is normalized. */
#define VPD_element( X, n )  (*VPD_Pelement( X, n ))

/*******************  Initialization **********************************/

void  __vf  VPD_equ0( pdVector X, ui size );
void  __vf  VPD_equ1( pdVector X, ui size );
void  __vf  VPD_equm1( pdVector X, ui size );
void  __vf  VPD_equC( pdVector X, ui size, dPolar C );
void  __vf  VPD_equV( pdVector Y, pdVector X, ui size );
void  __vf  VPD_principal( pdVector Y, pdVector X, ui size );

void  __vf  VD_PtoMagArg( dVector Mag, dVector Arg, pdVector X, ui size );
void  __vf  VD_MagArgtoP( pdVector Y, dVector Mag, dVector Arg, ui size );
void  __vf  VD_MagArgtoPrincipal( pdVector Y,
                                  dVector Mag, dVector Arg, ui size );
void  __vf  VD_PtoMag( dVector Mag, pdVector X, ui size );
void  __vf  VD_MagtoP( pdVector Y, dVector Mag, ui size );
void  __vf  VD_PtoArg( dVector Arg, pdVector X, ui size );
void  __vf  VD_ArgtoP( pdVector Y, dVector Arg, ui size );
void  __vf  VD_CtoP( pdVector YP, cdVector XC, ui size );
void  __vf  VD_PtoC( cdVector YC, pdVector XP, ui size );
void  __vf  VD_PtoNorm( dVector Norm, pdVector X, ui size );
void  __vf  VD_PtoReIm( dVector Re, dVector Im, pdVector X, ui size );
void  __vf  VD_ReImtoP( pdVector Y, dVector Re, dVector Im, ui size );
void  __vf  VD_PtoRe( dVector Re, pdVector X, ui size );
void  __vf  VD_PtoIm( dVector Im, pdVector X, ui size );
int   __vf  VPD_abs(   dVector Y, pdVector X, ui size );
#define     VPD_real( Y, X, sz )         VD_PtoRe( Y, X, sz )
#define     VPD_imag( Y, X, sz )         VD_PtoIm( Y, X, sz )
#define     VPD_arg( Y, X, sz )          VD_PtoArg( Y, X, sz )
#define     VPD_norm( Y, X, sz )         VD_PtoNorm( Y, X, sz )


/****************  Data-type interconversions  ***************************/

void  __vf   V_PFtoPD( pdVector Y, pfVector X, ui size );
void  __vf   V_PDtoPF( pfVector Y, pdVector X, ui size );
#ifdef __BORLANDC__   /* 80-bit IEEE numbers supported */
    void  __vf   V_PDtoPE( peVector Y, pdVector X, ui size );
    void  __vf   V_PEtoPD( pdVector Y, peVector X, ui size );
#else  /* no IEEE number support with other compilers */
    #define V_PDtoPE VPD_equV
    #define V_PEtoPD VPD_equV
#endif


/****************  Index-oriented manipulations ***********************/

void  __vf  VPD_reflect( pdVector X, ui size );
void  __vf  VPD_delete( pdVector X, ui size, ui pos );
void  __vf  VPD_insert( pdVector X, ui size, ui pos, dPolar C );
void  __vf  VPD_rev( pdVector Y, pdVector X, ui size );
#ifdef V_HUGE
   void  __vf  VPD_rotate( pdVector Y, pdVector X, ui size, long pos );
#else
   void  __vf  VPD_rotate( pdVector Y, pdVector X, ui size, int pos );
#endif

void  __vf  VPD_indpick( pdVector Y, uiVector Ind, ui sizey, pdVector X );
void  __vf  VPD_indput(  pdVector Y, pdVector X, uiVector Ind, ui sizex );
void  __vf  VPD_subvector( pdVector Y, ui sizey, pdVector X, int step );


/***************** Functions of a sub-set of elements  ********************/

void  __vf  VPD_subvector_equC(  pdVector Y, ui subsiz, unsigned samp,
                                 dPolar C );
void  __vf  VPD_subvector_equV(  pdVector Y, ui subsiz, unsigned samp,
                                 pdVector X );
    /*   for arithmetic functions of subsets, see <VPDmath.h>  */


/**************** One-Dimensional Vector Operations ***********************/

double    __vf  VPD_absmax(  pdVector X, ui size );
double    __vf  VPD_absmin(  pdVector X, ui size );
double    __vf  VPD_absmaxind( ui _VFAR *pos, pdVector X, ui size );
double    __vf  VPD_absminind( ui _VFAR *pos, pdVector X, ui size );
#if defined __cplusplus && defined _CMATH_CLASSDEFS
} // the following functions cannot be extern "C", if dPolar is a class
#endif
dPolar  __vf  VPD_prod(    pdVector X, ui size );
#if defined __cplusplus && defined _CMATH_CLASSDEFS
extern "C" {
#endif
void      __vf  VPD_runprod( pdVector Y, pdVector X, ui size );
int       __vf  VPD_iselementC( pdVector Tab, ui size, dPolar C );
ui        __vf  VPD_iselementV( pdVector Y, pdVector X, ui sizex,
                                pdVector Tab, ui sizetab );


/***************************  Input and Output *****************************/

void  __vf  VPD_fprint( FILE _VFAR *stream, pdVector X, ui size,
                                 unsigned nperline, unsigned linewidth );
#if !defined _Windows || defined __FLAT__ || defined _WIN32
    void  __vf  VPD_cprint( pdVector X, ui size, unsigned nperline );
#endif
#define     VPD_print( X, sz, npl ) VPD_fprint( stdout, X, sz, npl, 80 )
  /*  VPD_print, VPD_cprint usable only for DOS and Win32 console applications! */
void  __vf  VPD_write( FILE _VFAR *stream, pdVector X, ui size );
void  __vf  VPD_read( pdVector X, ui size, FILE _VFAR *stream );
void  __vf  VPD_nwrite( FILE _VFAR *stream, unsigned n, ui size, ... );
void  __vf  VPD_nread( unsigned n, ui size, FILE _VFAR *stream, ... );
void  __vf  VPD_setWriteFormat( char _VFAR *FormatString );
                                    /* for VPD_write and VPD_nwrite */
void  __vf  VPD_setWriteSeparate( char _VFAR *SepString ); /* for VPD_write */
void  __vf  VPD_setNWriteSeparate( char _VFAR *SepString ); /* for VPD_nwrite */
#ifdef V_HUGE
    void  __vf  VPD_store( FILE _VFAR *stream, pdVector X, ui size );
    void  __vf  VPD_recall( pdVector X, ui size, FILE _VFAR *stream  );
#else
    #ifdef __cplusplus
         void  inline VPD_store( FILE _VFAR *stream, pdVector X, ui size )
         {    fwrite( X, sizeof(dPolar), size, stream );
         }
         void  inline VPD_recall( pdVector X, ui size, FILE _VFAR *stream )
         {    fread(  X, sizeof(dPolar), size, stream );
         }
    #else
        #define VPD_store( str, X, sz )  \
                     fwrite( X, sizeof(dPolar), sz, str )
        #define VPD_recall( X, sz, str ) \
                     fread(  X, sizeof(dPolar), sz, str )
    #endif
#endif      /* VPD_store, VPD_recall in binary format */


#ifdef __cplusplus
}
#endif

#endif /*  __VPDSTD_H */
