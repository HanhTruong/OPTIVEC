/***********************   VDEMOB.CPP  ***********************************
*                                                                        *
*                Demo program for                                        *
*                  O p t i V e c                                         *
*       with Borland C++ Builder 4 or higher,                            *
*                                                                        *
*   Copyright 1996-2002 by OptiCode - Dr. Martin Sander Software Dev.    *
*                                                                        *
*                                                                        *
*       This sample program is meant to demonstrate how to use           *
*       OptiVec within your Windows programs. It is not intended         *
*       to demonstrate best Windows programming.                         *
*                                                                        *
**************************************************************************/
/*
In the Borland C++ Builder IDE:
    1. Open the project VDEMOB.BPR.
    2. Check that the include and library paths
       contain your OptiVec directories
       (Project/Options/Directories).
    3. In Project/Options/Linker, be sure that
       "Dynamic RTL" is  n o t  checked.
    4. Compile and run.
*/

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "vdemofrm.h"
#include <math.h>
#include <VFstd.h>                      /* OptiVec include files */
#include <VCFstd.h>
#include <VFmath.h>
#include <VCFmath.h>
#include <VIstd.h>
#include <Vgraph.h>
#ifndef M_PI
    #define M_PI  3.14159265358979323846
#endif
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1   *Form1;
fVector  X1, X2, Y1, Y2, Y3, Y4, Spc, Freq, Win;
cfVector CX1, CX2;
iVector  I1;
float    deltat, fOscill, fNyquist;
ui       vsize, spcsize;
int      vview;
struct  _SYSTEMTIME tStart, tStop;

void StartTime( void )
{
    GetLocalTime( &tStart );
}

double StopTime( void )
{
    double   tBegin, tEnd;
    GetLocalTime( &tStop );
    tBegin = (tStart.wHour * 60 + tStart.wMinute) * 60L
            + tStart.wSecond + tStart.wMilliseconds * 0.001;
    tEnd   = (tStop.wHour * 60 + tStop.wMinute) * 60L
            + tStop.wSecond + tStop.wMilliseconds * 0.001;
    return( tEnd - tBegin );
}

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}

void __fastcall TForm1::ShowView(TObject *Sender)
{
    char      TimeMsg[100];
    unsigned  i, j;
    float     xt, A=1.2f, B=-0.13f, C=0.85f; // any arbitrary values!

    V_initPlot( WindowHandle, PaintBox1->Canvas->Handle );
    V_setPlotRegion( 130, 0, PaintBox1->Width, PaintBox1->Height );
      // must explicitly call the above two functions in BC++ Builder!

    switch( vview )
    {
        default: break;
        case 0:
          PaintBox1->Canvas->TextOut( 10, 10,
          "This is a series of graphs illustrating OptiVec functions." );
          PaintBox1->Canvas->TextOut( 10, 40,
          "Press [Alt] [F4] to end the demonstration." );
            vsize  = 4096;
            spcsize = 256;
            I1 = VI_vector( vsize );
            X1 = VF_vector( vsize );
            X2 = VF_vector( vsize );
            Y1 = VF_vector( vsize );
            Y2 = VF_vector( vsize );
            Y3 = VF_vector( vsize );
            Y4 = VF_vector( vsize );
            Spc= VF_vector( spcsize+1 );
            Freq=VF_vector( spcsize+1 );
            Win= VF_vector( 2*spcsize );
            CX1 = VCF_vector( vsize );
            CX2 = VCF_vector( vsize );
        break;
        case 1:
          PaintBox1->Canvas->TextOut( 0, 10, "Sine wave" );
          PaintBox1->Canvas->TextOut( 0, 30, "f = 100 Hz" );
          deltat   = 0.001;  /* sampling interval 1 millisecond */
          fNyquist = 0.5 / deltat;   /* Nyquist frequency = 500 Hz */
          fOscill  = 100;   /* say we are sampling a 100 Hz oscillation */
          VF_ramp( X1, vsize, 0, deltat );  /* Time axis */
          VF_ramp( Freq, spcsize+1, 0, fNyquist / spcsize );
          VFx_sin( Y1, X1, vsize, 2*M_PI*fOscill, 0, 1 ); /* sine wave( omega*t) */
          VF_xyAutoPlot( X1, Y1, 101, PS_SOLID, LIGHTRED );
                     // show only 10 waves
        break;
        case 2:
          PaintBox1->Canvas->TextOut( 0, 10, "Asymmetric" );
          PaintBox1->Canvas->TextOut( 0, 30, "square wave" );
          PaintBox1->Canvas->TextOut( 0, 50, "f = 100 Hz" );
          VF_cmp_gtC( Y2, Y1, vsize, 0.7f );
               //  transform the sine into an asymmetric square wave
          V_drawAxes( 0, 100, -0.3, 1.2 );
          VF_yDataPlot( Y2, 101, PS_SOLID, BLUE );
                    // show again only 10 waves
        break;
        case 3:
          PaintBox1->Canvas->TextOut( 0,  10, "Frequency spectrum" );
          PaintBox1->Canvas->TextOut( 0,  30, "of the square wave" );
          PaintBox1->Canvas->TextOut( 0,  50, "Note the higher" );
          PaintBox1->Canvas->TextOut( 0,  70, "harmonics of the" );
          PaintBox1->Canvas->TextOut( 0,  90, "100 Hz ground freq." );
          PaintBox1->Canvas->TextOut( 0, 110, "and the f=0 term" );
          PaintBox1->Canvas->TextOut( 0, 130, "due to the asymmetry" );
          PaintBox1->Canvas->TextOut( 0, 170, "Don't be sur-" );
          PaintBox1->Canvas->TextOut( 0, 190, "prised; the next" );
          PaintBox1->Canvas->TextOut( 0, 210, "example will show" );
          PaintBox1->Canvas->TextOut( 0, 230, "a bit of error" );
          PaintBox1->Canvas->TextOut( 0, 250, "handling!" );
          VF_Hanning( Win, 2*spcsize );
          Spc[spcsize] = VF_spectrum( Spc, spcsize, Y2, vsize, Win );
                //  analyse the frequency spectrum of the square wave
          VF_xyAutoPlot( Freq, Spc, spcsize+1, PS_SOLID | SY_CROSS, GREEN );
        break;
        case 4:
          PaintBox1->Canvas->TextOut( 0,  10, "Trigonometric" );
          PaintBox1->Canvas->TextOut(10,  30, "functions:" );
          PaintBox1->Canvas->TextOut( 0,  50, "Red:   sine" );
          PaintBox1->Canvas->TextOut( 0,  70, "Green: tangent" );
          PaintBox1->Canvas->TextOut( 0,  90, "Blue:  cosecant" );
          PaintBox1->Canvas->TextOut( 0, 120, "(sine:" );
          PaintBox1->Canvas->TextOut( 0, 140, "   *10 and +20," );
          PaintBox1->Canvas->TextOut( 0, 160, " cosecant: -20)" );
          VI_ramp( I1, vsize, 0, 1 );
          VF_ramp( X1, vsize, 0, 4.0f /(vsize-1) );
          VF_sinrpi2( Y1, I1, vsize, (vsize-1)/2);
          VFx_equV( Y1, Y1, vsize, 10.0f, 20.0f );
             // get a sine wave, magnify it by a factor of 10
             // and displace it vertically by +20 units.
          VF_tanrpi2( Y2, I1, vsize, (vsize-1)/2);
          VF_limit( Y2, Y2, vsize, -40.f, 40.f );
             //  limit the range of tangent values
          VF_cosecrpi2( Y3, I1, vsize, (vsize-1)/2);
          VF_limit( Y3, Y3, vsize, -40.f, 40.f );
             //  the same for cosecant values
          VF_subC( Y3, Y3, vsize, 20.0f );
             //  displace the cosecant by -20 units.
          VF_xy2AutoPlot( X1, Y2, vsize, PS_SOLID, LIGHTGREEN,
                          X1, Y3, vsize, PS_SOLID, LIGHTBLUE );
          VF_xyDataPlot( X1, Y1, vsize, PS_SOLID, LIGHTRED );
        break;
        case 5:
          PaintBox1->Canvas->TextOut( 0,  10, "Playing with functions" );
          PaintBox1->Canvas->TextOut( 0,  30, "in the complex plane:" );
          PaintBox1->Canvas->TextOut( 0,  60, "Red:   complex sine" );
          PaintBox1->Canvas->TextOut( 0,  80, "Green: complex cosine" );
          VCF_ramp( CX1, vsize, fcplx( M_PI, 0),
                                fcplx(0.4f, 0.008f) );
          VCF_cos( CX2, CX1, vsize );
          VCF_sin( CX1, CX1, vsize );
                //  Try also with other complex functions!
          VCF_2AutoPlot( CX1, vsize, PS_SOLID, LIGHTRED,
                         CX2, vsize, PS_SOLID, LIGHTGREEN );
        break;
        case 6:
          PaintBox1->Canvas->TextOut( 0, 10, "To end the demo, let's make a crude speed comparison" );
          PaintBox1->Canvas->TextOut( 0, 30, "between compiled code and OptiVec code." );
          V_nfree( 2, X1, X2 );
          X1 = VF_vector( 4096 );
          VF_ramp( X1, 4096, -10, 0.005f );
          X2 = VF_vector( 4096 );
          PaintBox1->Canvas->TextOut( 0, 55, "E.g., evaluate the exponential function for a whole vector" );
          PaintBox1->Canvas->TextOut( 20, 80, "Y[i] = c * exp[ a * X[i] + b],  i=0,..,size-1" );
          PaintBox1->Canvas->TextOut( 0, 105, "(250 runs of 4096 elements each).   Please wait..." );
          StartTime();
          for( i=0; i<250; i++ )
              for( j=0; j<4096; j++ )
                  X2[j] = C * exp( A * X1[j] + B );
          xt = StopTime();
          sprintf( TimeMsg, "Execution Time for classic loop version: %lf sec", xt );
          PaintBox1->Canvas->TextOut( 0, 130, TimeMsg );
          StartTime();
          for( i=0; i<250; i++ )
              VFx_exp( X2, X1, 4096, A, B, C );
          xt = StopTime();
          sprintf( TimeMsg, "Execution Time for OptiVec version:      %lf sec", xt );
          PaintBox1->Canvas->TextOut( 0, 155, TimeMsg );
          PaintBox1->Canvas->TextOut( 0, 190, "Generally, the OptiVec version is about 1.5 to 3 times faster than classic" );
          PaintBox1->Canvas->TextOut( 0, 210, "loops; in some environments, up to a factor of 8 can be gained!" );
          V_freeAll();
          vview = -1;
        break;
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ShowNext(TObject *Sender)
{
   vview++;
   PaintBox1->Invalidate();
}
//---------------------------------------------------------------------------
