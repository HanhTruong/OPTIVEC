/*****************************   VDEMOW1.CPP *****************************
*                                                                        *
*                Demo program for                                        *
*                  O p t i V e c                                         *
*           for Borland C++ for Windows                                  *
*     with OWL 1.0  (shipped with Borland C++ versions 3.0 and 3.1)      *
*                                                                        *
*   Copyright 1996-2002 by OptiCode - Dr. Martin Sander Software Dev.    *
*                                                                        *
*                                                                        *
*       This sample program is meant to demonstrate how to use           *
*       OptiVec within your Windows programs. It is not intended         *
*       to demonstrate best Windows programming.                         *
*                                                                        *
**************************************************************************/

/*  The include-file search path must contain
    \bc\include; \bc\classlib\include; \bc\owl\include; \bc\optivec\include
    The library search path must contain
    \bc\lib; bc\classlib\lib; bc\owl\lib; bc\optivec\lib
    You must build a project with the following entries:
    \bc\optivec\vdemow1.cpp
    \bc\optivec\lib\vcl3w.lib
    \bc\owl\lib\owl.def
    (If your Borland C++ directory has a name other than "bc",
     you must use that other name, of course)
    Additionally, you have to link in the class libraries, ObjectWindows,
    and the standard run-time libraries (use Options/Linker/Libraries)
*/

#define  WIN30
#include <owl.h>
#include <VFstd.h>
#include <VCFstd.h>
#include <VFmath.h>
#include <VIstd.h>
#include <VCFmath.h>
#include <Vgraph.h>
#include <math.h>
#include <string.h>
#include <float.h>
NEWMATHERR

class TVecApp : public TApplication
{
public:
  TVecApp(LPSTR AName, HANDLE hInstance, HANDLE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
    : TApplication(AName, hInstance, hPrevInstance, lpCmdLine, nCmdShow) {};
  virtual void InitMainWindow();
};

_CLASSDEF(TVecWindow)
class TVecWindow : public TWindow
{
public:
  fVector  X1, X2, Y1, Y2, Y3, Y4, Spc, Freq, Win;
  cfVector CX1, CX2;
  iVector  I1;
  float    deltat, fOscill, fNyquist;
  ui       vsize, spcsize;
  int      vview;
  struct time tStart, tStop;
  TVecWindow(PTWindowsObject AParent, LPSTR ATitle);
  ~TVecWindow();
  virtual void Paint(HDC DC, PAINTSTRUCT& PS);
  virtual void WMRButtonDown(RTMessage Msg)
    = [WM_FIRST + WM_RBUTTONDOWN];
  void   StartTime( void );
  double StopTime( void );
};

TVecWindow::TVecWindow(PTWindowsObject AParent, LPSTR ATitle)
  : TWindow(AParent, ATitle)
{
  vsize  = 2048;
  spcsize = 128;
  vview  =    0;
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

}

TVecWindow::~TVecWindow()
{
  V_nfree( 12, I1, X1, X2, Y1, Y2, Y3, Y4, CX1, CX2, Spc, Freq, Win );
}


void TVecWindow::WMRButtonDown(RTMessage /*Msg*/)
{
  InvalidateRect(HWindow, NULL, TRUE);
  switch( vview )
  {
     default: vview=-1;
     case 0:  deltat   = 0.001;  /* sampling interval 1 millisecond */
              fNyquist = 0.5 / deltat;   /* Nyquist frequency = 500 Hz */
              fOscill  = 100;   /* say we are sampling a 100 Hz oscillation */
              VF_ramp( X1, vsize, 0, deltat );  /* Time axis */
              VF_ramp( Freq, spcsize+1, 0, fNyquist / spcsize );
              VFx_sin( Y1, X1, vsize, 2*M_PI*fOscill, 0, 1 ); /* sine wave( omega*t) */
              break;
     case 1:  VF_cmp_gtC( Y2, Y1, vsize, 0.7 );
                    //  transform the sine into an asymmetric square wave
              break;
     case 2:  VF_Hanning( Win, 2*spcsize );
              Spc[spcsize] = VFs_spectrum( Spc, spcsize, Y2, vsize, Win );
                    //  analyse the frequency spectrum of the square wave
              break;
     case 3:  VI_ramp( I1, vsize, 0, 1 );
              VF_ramp( X1, vsize, 0, 4.0 /(vsize-1) );
              VF_sinrpi2( Y1, I1, vsize, (vsize-1)/2);
              VFx_equV( Y1, Y1, vsize, 10.0, 20.0 );
              VF_tanrpi2( Y2, I1, vsize, (vsize-1)/2);
              VF_limit( Y2, Y2, vsize, -40, 40 );
              VF_cosecrpi2( Y3, I1, vsize, (vsize-1)/2);
              VF_limit( Y3, Y3, vsize, -40, 40 );
              VF_subC( Y3, Y3, vsize, 20.0 );
              break;
     case 4:  VCF_ramp( CX1, vsize, fcplx( M_PI, 0 ),
                                    fcplx( 0.04, 0.0001 ));
              VCF_cos( CX2, CX1, vsize );
              VCF_sin( CX1, CX1, vsize );
                /*  Try also with other complex functions! */
              break;
     case 5:  break;
  }
  vview++;
}


void TVecWindow::Paint(HDC DC, PAINTSTRUCT&)
{
   char     Explanation[100];
   unsigned i, j;
   float    xt, A=1.2, B=-0.13, C=0.85;

   V_initPlot( HWindow, DC );
   switch( vview )
   {
       default: break;
       case 0: TextOut( DC, 10, 10,
               "This is a series of graphs illustrating OptiVec functions.", 58 );
               TextOut( DC, 10, 40,
               "Always press the right mouse button to see the next view!", 56 );
               TextOut( DC, 10, 70,
               "Press [Alt] [F4] to end the demonstration.", 41 );
               break;
       case 1: TextOut( DC, 0, 10, "Sine wave", 9 );
               TextOut( DC, 0, 30, "f = 100 Hz", 10 );
               VF_xyAutoPlot( X1, Y1, 101, PS_SOLID, LIGHTRED );
                            // show only 10 waves
               break;
       case 2: TextOut( DC, 0, 10, "Asymmetric", 10 );
               TextOut( DC, 0, 30, "square wave", 11 );
               TextOut( DC, 0, 50, "f = 100 Hz", 10 );
               V_drawAxes( 0, 100, -0.3, 1.2 );
               VF_yDataPlot( Y2, 101, PS_SOLID, BLUE );
                           // show again only 10 waves
               break;
       case 3: TextOut( DC, 0,  10, "Frequency spectrum", 18 );
               TextOut( DC, 0,  30, "of the square wave", 18 );
               TextOut( DC, 0,  50, "Note the higher", 15 );
               TextOut( DC, 0,  70, "harmonics of the", 16 );
               TextOut( DC, 0,  90, "100 Hz ground freq.", 19 );
               TextOut( DC, 0, 110, "and the f=0 term", 16 );
               TextOut( DC, 0, 130, "due to the asymmetry", 20 );
               TextOut( DC, 0, 170, "Don't be sur-", 13 );
               TextOut( DC, 0, 190, "prised; the next", 16 );
               TextOut( DC, 0, 210, "example will show", 17 );
               TextOut( DC, 0, 230, "a bit of error", 14 );
               TextOut( DC, 0, 250, "handling!", 9 );
               VF_xyAutoPlot( Freq, Spc, spcsize+1, PS_SOLID | SY_CROSS, GREEN );
               break;
       case 4: TextOut( DC, 0,  10, "Trigonometric", 13 );
               TextOut( DC,10,  30, "functions:", 10 );
               TextOut( DC, 0,  50, "Red:   sine", 11 );
               TextOut( DC, 0,  70, "Green: tangent", 14 );
               TextOut( DC, 0,  90, "Blue:  cosecant", 15 );
               TextOut( DC, 0, 120, "(sine:", 6 );
               TextOut( DC, 0, 140, "   *10 and +20,", 15 );
               TextOut( DC, 0, 160, " cosecant: -20)", 15 );
               VF_xy2AutoPlot( X1, Y2, vsize, PS_SOLID, LIGHTGREEN,
                               X1, Y3, vsize, PS_SOLID, LIGHTBLUE );
               VF_xyDataPlot( X1, Y1, vsize, PS_SOLID, LIGHTRED );
               break;
       case 5: TextOut( DC, 0,  10, "Playing with functions", 22 );
               TextOut( DC, 0,  30, "in the complex plane:", 21 );
               TextOut( DC, 0,  60, "Red:   complex sine",  19 );
               TextOut( DC, 0,  80, "Green: complex cosine",  21 );
               VCF_2AutoPlot( CX1, vsize, PS_SOLID, LIGHTRED,
                              CX2, vsize, PS_SOLID, LIGHTGREEN );
               break;
       case 6: TextOut( DC, 0, 10, "To end the demo, let's make a crude speed comparison", 52 );
               TextOut( DC, 0, 30, "between compiled code and OptiVec code.", 39 );
               V_nfree( 2, X1, X2 );
               X1 = VF_vector( 4096 );
               VF_ramp( X1, 4096, -10, 0.005 );
               X2 = VF_vector( 4096 );
               TextOut( DC, 0, 55, "E.g., evaluate the exponential function for a whole vector", 58 );
               TextOut( DC, 20, 80, "Y[i] = c * exp[ a * X[i] + b],  i=0,..,size-1", 45 );
               TextOut( DC, 0, 105, "(20 runs of 4096 elements each).   Please wait...", 49 );
               StartTime();
                   for( i=0; i<20; i++ )
                      for( j=0; j<4096; j++ )
                         X2[j] = C * exp( A * X1[j] + B );
               xt = StopTime();
               sprintf( Explanation, "Execution Time for compiled version:  %lf sec", xt );
               TextOut( DC, 0, 130, Explanation, strlen( Explanation ));
               StartTime();
                   for( i=0; i<20; i++ )
                         VFx_exp( X2, X1, 4096, A, B, C );
               xt = StopTime();
               sprintf( Explanation, "Execution Time for OptiVec version:    %lf sec", xt );
               TextOut( DC, 0, 155, Explanation, strlen( Explanation ));
               TextOut( DC, 0, 190, "Generally, the OptiVec version is about 2 times faster than compiled", 68 );
               TextOut( DC, 0, 210, "code; in some environments, up to a factor of 8 can be gained!", 62 );
       break;
   }
}

void TVecWindow::StartTime( void )
{
    gettime( &tStart );
}

double TVecWindow::StopTime( void )
{
   double         tBegin, tEnd;
   gettime( &tStop );
   tBegin = (tStart.ti_hour * 60 + tStart.ti_min) * 60L
            + tStart.ti_sec + tStart.ti_hund * 0.01;
   tEnd   = (tStop.ti_hour * 60 + tStop.ti_min)  * 60L
            + tStop.ti_sec + tStop.ti_hund * 0.01;

   return( tEnd - tBegin );
}

void TVecApp::InitMainWindow()
{
  MainWindow = new TVecWindow(NULL, Name);
}

int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance,
  LPSTR lpCmdLine, int /*nCmdShow*/)
{
  TVecApp VecApp("OptiVec Demo", hInstance, hPrevInstance,
               lpCmdLine, SW_SHOWMAXIMIZED);  // override nCmdShow
  VecApp.Run();
  return VecApp.Status;
}


