/***********************   FITDEMO.CPP  **********************************
*                                                                        *
*            Data-fitting demo program for                               *
*                  O p t i V e c                                         *
*          with Borland C++ Builder 4 or higher                          *
*                                                                        *
*   Copyright 1996-2002 by OptiCode - Dr. Martin Sander Software Dev.    *
*                                                                        *
*                                                                        *
*       This sample program is meant to provide you with some basic      *
*       examples of code for the use of OptiVec's data-fitting routines. *
*       Especially for the non-linear and multi-experiment functions,    *
*       the user will have to adapt this code to his specific problem.   *
*                                                                        *
**************************************************************************/
/*
In the Borland C++ Builder IDE:
    1. Open the project FITDEMOB.BPR.
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
#include <VDstd.h>                      /* OptiVec include files */
#include <VDmath.h>
#include <MDstd.h>
#include <VIstd.h>
#include <Vgraph.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1   *Form1;
int      vview;
dVector  XExp, X2, YExp, YFit, YExp2, YFit2, YExp3, YFit3;
ui       sizex;
double   FitPars[7]; // the highest number of parameters we will have in our examples
int      ParStatus[7];
#define  polydeg 5     // refers to the polynomial fitting example
#ifndef M_PI
    #define M_PI  3.14159265358979323846
#endif

    // the following function is used with VD_linfit:
static void PolyModel( dVector BasFuncs, double x, unsigned  nfuncs )
{  /* This function fills the vector BasFuncs with powers of x.
      VD_linfit will then determine the coefficient for each of these
      powers.
      Note that the coefficients do not occur in the model function!
      The basis functions with known coefficients (whose fitting has
      been disabled) are still part of the model and must be calculated.
      You will see below that the values of the known (disabled)
      coefficients  must be set prior to calling VD_linfit.          */
     BasFuncs[0] = 1.0;
     for( unsigned i=1; i<nfuncs; i++ )
          BasFuncs[i] = BasFuncs[i-1]*x;
}

   // the following function is used with VD_nonlinfit:
static void VPolyModel( dVector Y, dVector X, ui size )
{   /* Here, the model function has to fill a whole result vector,
       using your first guess of FitPars. In contrast to the
       linear case, now the coefficients are explicitly used in the
       model function. You must initialize FitPars with something,
       even if you have no idea about the result.
       FitPars must be global, so that the model function can access the
       parameters. With VD_nonlinfit, you can use just any functions
       in your model.
       For better comparison, we use the same polynomial approximation
       as before, but now we code it as if we didn't know that a
       polynomial actually is linear in its coefficients (and as if
       we didn't know either how to efficiently code a polynomial at all).  */
    double xi;
    for( ui i=0; i<size; i++ )
    {
        xi  = X[i];
        Y[i]= FitPars[0]
            + FitPars[1] * xi
            + FitPars[2] * xi * xi
            + FitPars[3] * xi * xi * xi
            + FitPars[4] * xi * xi * xi * xi
            + FitPars[5] * xi * xi * xi * xi * xi;
   }
}

   // the following function is used with VD_multiNonlinfit:
static void VSineModel( dVector Y, dVector X, ui size, unsigned nExperiment )
{   /* According to the parameter nExperiment, the model function must
       choose the correct parameters for the calculation of the model
       function. The model function itself is the same for all experiments. */
    double omega, phase, amp;
    switch( nExperiment )
    {
        case 0: phase = FitPars[1];
                amp   = FitPars[4];
                break;
        case 1: phase = FitPars[2];
                amp   = FitPars[5];
                break;
        case 2: phase = FitPars[3];
                amp   = FitPars[6];
    }
    omega = FitPars[0];  // we assume this parameter to be the same
    VDx_sin( Y, X, size, omega, phase, amp );
}


//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}

void __fastcall TForm1::ShowView(TObject *Sender)
{
    VD_NONLINFITOPTIONS Opt;
    VD_EXPERIMENT       ExpList[3];
    char                DataText[80];
    HDC                 vDC;

    vDC = PaintBox1->Canvas->Handle;
    V_initPlot( WindowHandle, vDC );
    V_setPlotRegion( PaintBox1->Width/4, 0, PaintBox1->Width, PaintBox1->Height );
      // must explicitly call the above two functions in BC++ Builder!

    switch( vview )
    {
        default: break;
        case 0:
          PaintBox1->Canvas->TextOut( 10, 10,
          "This is a series of graphs illustrating OptiVec data-fitting functions." );
          PaintBox1->Canvas->TextOut( 10, 40,
          "Press [Alt] [F4] to end the demonstration." );
            sizex   = 200;
            XExp  = VD_vector( sizex );
            YExp  = VD_vector( sizex );
            YFit  = VD_vector( sizex );
            YExp2 = VD_vector( sizex );
            YExp3 = VD_vector( sizex );
            YFit2 = VD_vector( sizex );
            YFit3 = VD_vector( sizex );
        break;
        case 1:
          PaintBox1->Canvas->TextOut( 2, 8, "Suppose these are" );
          PaintBox1->Canvas->TextOut( 2, 26, "your experimental" );
          PaintBox1->Canvas->TextOut( 2, 44, "data points." );
          PaintBox1->Canvas->TextOut( 2, 62, "(Actually, they consist" );
          PaintBox1->Canvas->TextOut( 2, 80, "of a simple cubic with" );
          PaintBox1->Canvas->TextOut( 2, 98, "1% added noise.)" );

          VD_ramp( XExp, sizex, 0, 1.0/(sizex-1) ); // "experimental" x-axis from 0 to 1
          VD_cubic( YExp, XExp, sizex );        // fake "measured" y-data as y = x^3
          VD_noise( YFit, sizex, 1, 0.005 );
          VD_addV( YExp, YExp, YFit, sizex ); // add 1% peak-to-peak "experimental noise"
          VD_xyAutoPlot( XExp, YExp, sizex, PS_NULL+SY_CROSS, GREEN );
        break;
        case 2:   // fit your data to one of the simplest models, a polynomial
          PaintBox1->Canvas->TextOut( 2,  8, "The red curve is a" );
          PaintBox1->Canvas->TextOut( 2, 26, "fifth-order polynomial" );
          PaintBox1->Canvas->TextOut( 2, 44, "fitted to your data." );
          PaintBox1->Canvas->TextOut( 2, 62, "Without noise, the" );
          PaintBox1->Canvas->TextOut( 2, 80, "coefficients should" );
          PaintBox1->Canvas->TextOut( 2, 98, "have been:" );
          PaintBox1->Canvas->TextOut( 2,116, "{0, 0, 0, 1.0, 0, 0}" );

          VD_polyfit( FitPars, polydeg, XExp, YExp, sizex );
          VD_poly( YFit, XExp, sizex, FitPars, polydeg ); // calculate fit curve
          PaintBox1->Canvas->TextOut( 2,140, "Actually, we got:" );
          sprintf( DataText, "a0 = %+7.5lf", FitPars[0] );
          PaintBox1->Canvas->TextOut( 2,158, DataText );
          sprintf( DataText, "a1 = %+7.5lf", FitPars[1] );
          PaintBox1->Canvas->TextOut( 2,176, DataText );
          sprintf( DataText, "a2 = %+7.5lf", FitPars[2] );
          PaintBox1->Canvas->TextOut( 2,194, DataText );
          sprintf( DataText, "a3 = %+7.5lf", FitPars[3] );
          PaintBox1->Canvas->TextOut( 2,212, DataText );
          sprintf( DataText, "a4 = %+7.5lf", FitPars[4] );
          PaintBox1->Canvas->TextOut( 2,230, DataText );
          sprintf( DataText, "a5 = %+7.5lf", FitPars[5] );
          PaintBox1->Canvas->TextOut( 2,248, DataText );
          PaintBox1->Canvas->TextOut( 2,268, "Note how even moderate" );
          PaintBox1->Canvas->TextOut( 2,286, "noise leads to rather" );
          PaintBox1->Canvas->TextOut( 2,304, "large errors in the" );
          PaintBox1->Canvas->TextOut( 2,322, "fit parameters, if" );
          PaintBox1->Canvas->TextOut( 2,340, "there are too many" );
          PaintBox1->Canvas->TextOut( 2,358, "'free' parameters." );
          VD_xy2AutoPlot( XExp, YExp, sizex, PS_NULL | SY_CROSS, LIGHTGREEN,
                  XExp, YFit, sizex, PS_SOLID, LIGHTRED );
        break;
        case 3:  // refine your fit by switching to a general linear model,
                // giving you the chance to consider only the uneven terms
          PaintBox1->Canvas->TextOut( 2,  8, "Suppose you know that" );
          PaintBox1->Canvas->TextOut( 2, 26, "the coefficients of" );
          PaintBox1->Canvas->TextOut( 2, 44, "all even terms are 0." );
          PaintBox1->Canvas->TextOut( 2, 62, "Then you fit to your" );
          PaintBox1->Canvas->TextOut( 2, 80, "own linear model," );
          PaintBox1->Canvas->TextOut( 2, 98, "consisting only of" );
          PaintBox1->Canvas->TextOut( 2,116, "uneven terms." );
          PaintBox1->Canvas->TextOut( 2,134, "Now we get:" );

          ParStatus[0] = ParStatus[2] = ParStatus[4] = 0;  //disable fitting of even terms
          FitPars[0] = FitPars[2] = FitPars[4] = 0.0;  // set their coefficients to the known value, 0.0
          ParStatus[1] = ParStatus[3] = ParStatus[5] = 1;  // enable fitting of uneven terms
             // the disabled fitting parameters must be initialized before calling VD_linfit !
          VD_linfit( FitPars, ParStatus, polydeg+1,
                     XExp, YFit, sizex,
                     PolyModel );
          VD_poly( YFit, XExp, sizex, FitPars, polydeg ); // calculate new fit curve

          PaintBox1->Canvas->TextOut( 2,160, "a0 = 0 (fix)" );
          sprintf( DataText, "a1 = %+7.5lf", FitPars[1] );
          PaintBox1->Canvas->TextOut( 2,178, DataText );
          PaintBox1->Canvas->TextOut( 2,196, "a2 = 0 (fix)" );
          sprintf( DataText, "a3 = %+7.5lf", FitPars[3] );
          PaintBox1->Canvas->TextOut( 2,214, DataText );
          PaintBox1->Canvas->TextOut( 2,232, "a4 = 0 (fix)" );
          sprintf( DataText, "a5 = %+7.5lf", FitPars[5] );
          PaintBox1->Canvas->TextOut( 2,250, DataText );
          PaintBox1->Canvas->TextOut( 2,268, "This is about as close" );
          PaintBox1->Canvas->TextOut( 2,286, "as we can get in the" );
          PaintBox1->Canvas->TextOut( 2,304, "presence of noise." );
          VD_xy2AutoPlot( XExp, YExp, sizex, PS_NULL | SY_CROSS, LIGHTGREEN,
                  XExp, YFit, sizex, PS_SOLID, LIGHTRED );
          break;
               case 4:  // here, we mis-use a non-linear fitting algorithm
                // for our simple problem.
          PaintBox1->Canvas->TextOut( 2,  8, "Let's fire with the" );
          PaintBox1->Canvas->TextOut( 2, 26, "'cannon' of a non-" );
          PaintBox1->Canvas->TextOut( 2, 44, "linear fit on our" );
          PaintBox1->Canvas->TextOut( 2, 62, "simple 'sparrow'" );
          PaintBox1->Canvas->TextOut( 2, 80, "problem." );
          PaintBox1->Canvas->TextOut( 2, 98, "It takes much longer" );
          PaintBox1->Canvas->TextOut( 2,116, "to find the result..." );

          ParStatus[0] = ParStatus[2] = ParStatus[4] = 0;  //disable fitting of even terms, as before
          ParStatus[1] = ParStatus[3] = ParStatus[5] = 1;  // enable fitting of uneven terms
          VD_getNonlinfitOptions( &Opt );
          Opt.FigureOfMerit = 0;  // choose least-square fitting
          Opt.AbsTolChi = 1.e-6;
          Opt.FracTolChi = 1.e-3;  // makes the fit fast, but not very accurate
          Opt.LevelOfMethod = 3;  // if you fear you might jump into a
             // local rather than the true global parameter optimum, try
             // LevelOfMethod = 7 - but only if you have time to wait for the result
          VD_setNonlinfitOptions( &Opt );

          FitPars[0] = FitPars[2] = FitPars[4] = 0.0;  // set known coefficients to the value, 0.0, as before
          FitPars[1] = FitPars[3] = FitPars[5] = 1.5;  // you must provide some guess here!
                // all fitting parameters must be initialized before calling VD_nonlinfit !
          VD_nonlinfit( FitPars, ParStatus, polydeg+1,
                XExp, YExp, sizex,
                VPolyModel, NULL );
           // If you know the derivatives with respect to each parameter, put
           // your knowledge into a DerivModel function and replace the 'NULL'
           // parameter with it. (Actually, here we do know; but let's assume
           // we don't, and have VD_nonlinfit call the numeric differentiation
           // procedure.)
          VPolyModel( YFit, XExp, sizex );  // get fit curve from model

          PaintBox1->Canvas->TextOut( 2,137, "But finally we get:" );
          PaintBox1->Canvas->TextOut( 2,158, "a0 = 0 (fix)" );
          sprintf( DataText, "a1 = %+7.5lf", FitPars[1] );
          PaintBox1->Canvas->TextOut( 2,176, DataText );
          PaintBox1->Canvas->TextOut( 2,194, "a2 = 0 (fix)" );
          sprintf( DataText, "a3 = %+7.5lf", FitPars[3] );
          PaintBox1->Canvas->TextOut( 2,212, DataText );
          PaintBox1->Canvas->TextOut( 2,230, "a4 = 0 (fix)" );
          sprintf( DataText, "a5 = %+7.5lf", FitPars[5] );
          PaintBox1->Canvas->TextOut( 2,248, DataText );
          PaintBox1->Canvas->TextOut( 2,274, "That is virtually the" );
          PaintBox1->Canvas->TextOut( 2,292, "same as before." );
          VD_xy2AutoPlot( XExp, YExp, sizex, PS_NULL | SY_CROSS, GREEN,
                  XExp, YFit, sizex, PS_SOLID, LIGHTRED );
        break;
        case 5:  // finally, let's suppose you have several experimental
                // curves, measuring the same physical process under
                // slightly different conditions.
                // Say, we have a vibration, and each measurement
                // begins with a different phase and has a somewhat
                // different amplitude, but the same frequency.
          PaintBox1->Canvas->TextOut( 2,  8, "Finally, let us fit " );
          PaintBox1->Canvas->TextOut( 2, 26, "several sets of experi-" );
          PaintBox1->Canvas->TextOut( 2, 44, "mental data at once" );
          PaintBox1->Canvas->TextOut( 2, 62, "(sine waves with the" );
          PaintBox1->Canvas->TextOut( 2, 80, "same frequency, but" );
          PaintBox1->Canvas->TextOut( 2, 98, "different phases and" );
          PaintBox1->Canvas->TextOut( 2,116, "amplitudes)." );
          PaintBox1->Canvas->TextOut( 2,140, "First you see the" );
          PaintBox1->Canvas->TextOut( 2,158, "'experimental' data." );

          VD_ramp( XExp, sizex, 0, 1.0/(sizex-1) ); // x-axis again from 0 to 1
          VDx_sin( YExp,  XExp, sizex, 15.0,  0.0, 1.2 );   // first "measurement"
          VDx_sin( YExp2, XExp, sizex, 15.0,  0.5, 1.0 );   // second "measurement"
          VDx_sin( YExp3, XExp, sizex, 15.0, -1.8, 0.75 ); // third "measurement"

          VD_xy2AutoPlot( XExp, YExp,  sizex, PS_NULL + SY_CROSS, GREEN,
                  XExp, YExp2, sizex, PS_NULL + SY_CROSS, BLUE );
          VD_xyDataPlot(  XExp, YExp3, sizex, PS_NULL + SY_CROSS, MAGENTA );

              // cram your experiments into the array of VD_EXPERIMENT structs:
          ExpList[0].X = XExp;   ExpList[0].Y = YExp;    ExpList[0].size = sizex;
          ExpList[1].X = XExp;   ExpList[1].Y = YExp2;   ExpList[1].size = sizex;
          ExpList[2].X = XExp;   ExpList[2].Y = YExp3;   ExpList[2].size = sizex;
               // we are not using the InvVar and WeightOfExperiment fields
               // of ExpList, as we are not weighting the data.

          VI_equC( ParStatus, 7, 1 ); // we have 1 frequency, 3 phases, and 3 amplitudes,
                              // and all these 7 parameters are unknown.
                              // We must provide a first guess for each of them:
          FitPars[0] = 10.0;   // the frequency term
          FitPars[1] = FitPars[2] = FitPars[3] = 0.0;  // the three phases
          FitPars[4] = FitPars[5] = FitPars[6] = 1.5;  // the three amplitudes
          VD_getNonlinfitOptions( &Opt );
          Opt.AbsTolChi  = 1.e-8;
          Opt.FracTolChi = 1.e-6;  // force higher accuracy to avoid premature break-off
          /*  Unlike almost every other fitting routine available, you
              can get a result even for input parameters much farther off
              from the true value than the "guesses" chosen above.
              But then you must run VD_multiNonlinfit at "full power" and
              enable the following line:                       */
          // Opt.LevelOfMethod = 7;
          VD_setNonlinfitOptions( &Opt );

          VD_multiNonlinfit( FitPars, ParStatus, 7,
                     ExpList, 3,
                     VSineModel,
                     NULL ); // Again, we pretend we don't know the derivatives
             // bring the phases into the range -PI < phase < + PI :
          FitPars[1] = fmod( FitPars[1], 2.0*M_PI );
          if( FitPars[1] > M_PI ) FitPars[1] -= 2.0*M_PI;
          else if( FitPars[1] < -M_PI ) FitPars[1] += 2.0*M_PI;
          FitPars[2] = fmod( FitPars[2], 2.0*M_PI );
          if( FitPars[2] > M_PI ) FitPars[2] -= 2.0*M_PI;
          else if( FitPars[2] < -M_PI ) FitPars[2] += 2.0*M_PI;
          FitPars[3] = fmod( FitPars[3], 2.0*M_PI );
          if( FitPars[3] > M_PI ) FitPars[3] -= 2.0*M_PI;
          else if( FitPars[3] < -M_PI ) FitPars[3] += 2.0*M_PI;
          VSineModel( YFit,  XExp, sizex, 0 );  // get fit curves from your model
          VSineModel( YFit2, XExp, sizex, 1 );
          VSineModel( YFit3, XExp, sizex, 2 );
          SetViewportOrgEx( vDC, 0, 0, NULL );
             // to write text after plotting, go back to full window

          PaintBox1->Canvas->TextOut( 2,186, "Here are the results" );
          PaintBox1->Canvas->TextOut( 2,204, "(in brackets: 'true')" );
          sprintf( DataText, "freq =%+7.5lf (15.0) ", FitPars[0] );
          PaintBox1->Canvas->TextOut( 2,226, DataText );
          sprintf( DataText, "ph1  = %+7.5lf (0.0)", FitPars[1] );
          PaintBox1->Canvas->TextOut( 2,244, DataText );
          sprintf( DataText, "ph2  = %+7.5lf (0.5)", FitPars[2] );
          PaintBox1->Canvas->TextOut( 2,262, DataText );
          sprintf( DataText, "ph3  = %+7.5lf (-1.8)", FitPars[3] );
          PaintBox1->Canvas->TextOut( 2,280, DataText );
          sprintf( DataText, "amp1 = %+7.5lf (1.2)", FitPars[4] );
          PaintBox1->Canvas->TextOut( 2,298, DataText );
          sprintf( DataText, "amp2 = %+7.5lf (1.0)", FitPars[5] );
          PaintBox1->Canvas->TextOut( 2,316, DataText );
          sprintf( DataText, "amp3 = %+7.5lf (0.75)", FitPars[6] );
          PaintBox1->Canvas->TextOut( 2,334, DataText );

          V_continuePlot();   // go back to plot viewport
          VD_xyDataPlot(  XExp, YFit, sizex, PS_SOLID, LIGHTRED );
          VD_xyDataPlot(  XExp, YFit2, sizex, PS_SOLID, LIGHTRED );
          VD_xyDataPlot(  XExp, YFit3, sizex, PS_SOLID, LIGHTRED );
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
