/***********************   FITDEMO.CPP  **********************************
*                                                                        *
*            Data-fitting demo program for                               *
*                  O p t i V e c                                         *
*              with Borland C++ for DOS                                  *
*                                                                        *
*   Copyright 1996-2002 by OptiCode - Dr. Martin Sander Software Dev.    *
*                                                                        *
*                                                                        *
*       This sample program is meant to provide you with some basic      *
*       examples of code for the use of OptiVec's data-fitting routines. *
*       Especially for the non-linear and multi-experiment functions,    *
*       the user will have to adapt this code to his specific problem.   *
*                                                                        *
*   before compiling, please check and correct the BGI path statement    *
*   below as necessary!                                                  *
*                                                                        *
*   Your project must consist of the following files:                    *
*   FITDEMO.CPP, VCL3.LIB,  and  MCL3.LIB                                *
**************************************************************************/
#define BGI_PATH "\\BC\\BGI\\"  // adjust this line, if necessary!

#include <Vgraph.h>       /* Compiler's include files */
#include <math.h>
#include <DOS.h>
#include <conio.h>
#include <stdlib.h>
#include <VDstd.h>       /* OptiVec include files */
#include <VDmath.h>
#include <MDstd.h>
#include <VIstd.h>
#include <Vgraph.h>
NEWMATHERR

dVector             XExp, X2, YExp, YFit, YExp2, YFit2, YExp3, YFit3;
ui                  sizex;
double              FitPars[7];   // the highest number of parameters we will have in our examples
int                 ParStatus[7];
VD_NONLINFITOPTIONS Opt;
VD_EXPERIMENT       ExpList[3];
char                DataText[80];
#define  polydeg 5     // refers to the polynomial fitting example
#ifndef M_PI
    #define M_PI  3.14159265358979323846
#endif

    // the following function is used with VD_linfit:
static void __vf PolyModel( dVector BasFuncs, double x, unsigned  nfuncs )
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
static void __vf VPolyModel( dVector Y, dVector X, ui size )
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
static void __vf VSineModel( dVector Y, dVector X, ui size, unsigned nExperiment )
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

extern unsigned _ovrbuffer = 0x0800;
void main( void )
{
   unsigned xx, yy;

   printf( "\n\nThis is a short demonstration of OptiVec data-fitting"
           "\nfunctions in DOS real mode."
           "\nAlways hit any key to continue  or abort with [ESC] !" );
   if( getch() == 27 ) exit(0);

   sizex   = 200;
   XExp  = VD_vector( sizex );
   YExp  = VD_vector( sizex );
   YFit  = VD_vector( sizex );
   YExp2 = VD_vector( sizex );
   YExp3 = VD_vector( sizex );
   YFit2 = VD_vector( sizex );
   YFit3 = VD_vector( sizex );
   VD_ramp( XExp, sizex, 0, 1.0/(sizex-1) ); /* "experimental" x-axis from 0 to 1 */
   VD_cubic( YExp, XExp, sizex );        /* fake "measured" y-data as y = x^3 */
   VD_noise( YFit, sizex, 1, 0.005 );
   VD_addV( YExp, YExp, YFit, sizex ); /* add 1% peak-to-peak "experimental noise" */

   V_initGraph( BGI_PATH );  /* this should be your correct path, see above! */
   yy = getmaxy() / 15;
   xx = int( getmaxx() * 0.3 );
   V_setPlotRegion( xx, yy, getmaxx(), getmaxy()-yy );
   VD_xyAutoPlot( XExp, YExp, sizex, PS_NULL+SY_CROSS, LIGHTGREEN );
     setviewport( 1, 1, xx, getmaxy(), 0 ); clearviewport();
     settextjustify( LEFT_TEXT, TOP_TEXT );
     settextstyle( DEFAULT_FONT, HORIZ_DIR, 1 );
     setcolor( WHITE );
     outtextxy( 2,  20, "Suppose these are" );
     outtextxy( 2,  40, "your experimental" );
     outtextxy( 2,  60, "data points." );
     outtextxy( 2,  80, "(Actually, they consist" );
     outtextxy( 2, 100, "of a simple cubic with" );
     outtextxy( 2, 120, "1% added noise.)" );
     if( getch() == 27 ) exit(0);

        /* now fit your data to one of the simplest models, a polynomial */
   VD_polyfit( FitPars, polydeg, XExp, YExp, sizex );
   VD_poly( YFit, XExp, sizex, FitPars, polydeg ); /* calculate fit curve */
   V_continuePlot;   /* go back to the last plotting region */
   VD_xy2AutoPlot( XExp, YExp, sizex, PS_NULL + SY_CROSS, LIGHTGREEN,
                   XExp, YFit, sizex, PS_SOLID, LIGHTRED );
     setviewport( 1, 1, xx, getmaxy(), 0 ); clearviewport();
     settextjustify( LEFT_TEXT, TOP_TEXT );
     settextstyle( DEFAULT_FONT, HORIZ_DIR, 1 );
     setcolor( WHITE );

     outtextxy( 2, 10, "The red curve is a" );
     outtextxy( 2, 30, "fifth-order polynomial" );
     outtextxy( 2, 50, "fitted to your data." );
     outtextxy( 2, 70, "Without noise, the" );
     outtextxy( 2, 90, "coefficients should" );
     outtextxy( 2,110, "have been:" );
     outtextxy( 2,130, "{0, 0, 0, 1.0, 0, 0}" );

     outtextxy( 2,160, "Actually, we got:" );
     sprintf( DataText, "a0 = %+7.5lf", FitPars[0] );
     outtextxy( 2,180, DataText );
     sprintf( DataText, "a1 = %+7.5lf", FitPars[1] );
     outtextxy( 2,200, DataText );
     sprintf( DataText, "a2 = %+7.5lf", FitPars[2] );
     outtextxy( 2,220, DataText );
     sprintf( DataText, "a3 = %+7.5lf", FitPars[3] );
     outtextxy( 2,240, DataText );
     sprintf( DataText, "a4 = %+7.5lf", FitPars[4] );
     outtextxy( 2,260, DataText );
     sprintf( DataText, "a5 = %+7.5lf", FitPars[5] );
     outtextxy( 2,280, DataText );
     outtextxy( 2,310, "Note how even moderate" );
     outtextxy( 2,330, "noise leads to rather" );
     outtextxy( 2,350, "large errors in the" );
     outtextxy( 2,370, "fit parameters, if" );
     outtextxy( 2,390, "there are too many" );
     outtextxy( 2,410, "\"free\" parameters." );
     if( getch() == 27 ) exit(0);

        /* now refine your fit by switching to a general linear model,
           giving you the chance to consider only the uneven terms    */
   ParStatus[0] = ParStatus[2] = ParStatus[4] = 0;  //disable fitting of even terms
   FitPars[0] = FitPars[2] = FitPars[4] = 0.0;  // set their coefficients to the known value, 0.0
   ParStatus[1] = ParStatus[3] = ParStatus[5] = 1;  // enable fitting of uneven terms
   VD_linfit( FitPars, ParStatus, polydeg+1,
              XExp, YFit, sizex,
              PolyModel );
   VD_poly( YFit, XExp, sizex, FitPars, polydeg );
             /* calculate new fit curve  */
   V_continuePlot;   /* go back to the last plotting region */
   VD_xy2AutoPlot( XExp, YExp, sizex, PS_NULL + SY_CROSS, LIGHTGREEN,
                   XExp, YFit, sizex, PS_SOLID, LIGHTRED );
     setviewport( 1, 1, xx, getmaxy(), 0 ); clearviewport();
     settextjustify( LEFT_TEXT, TOP_TEXT );
     settextstyle( DEFAULT_FONT, HORIZ_DIR, 1 );
     setcolor( WHITE );
     outtextxy(  2, 10, "Suppose you know that" );
     outtextxy(  2, 30, "the coefficients of" );
     outtextxy(  2, 50, "all even terms are 0." );
     outtextxy(  2, 70, "Then you fit to your" );
     outtextxy(  2, 90, "own linear model," );
     outtextxy(  2,110, "consisting only of" );
     outtextxy(  2,130, "uneven terms." );
     outtextxy(  2,160, "Now we get:" );
     outtextxy(  2,180, "a0 = 0 (fix)" );
     sprintf( DataText, "a1 = %+7.5lf", FitPars[1] );
     outtextxy(  2,200, DataText );
     outtextxy(  2,220, "a2 = 0 (fix)" );
     sprintf( DataText, "a3 = %+7.5lf", FitPars[3] );
     outtextxy(  2,240, DataText );
     outtextxy(  2,260, "a4 = 0 (fix)" );
     sprintf( DataText, "a5 = %+7.5lf", FitPars[5] );
     outtextxy(  2,280, DataText );
     outtextxy(  2,310, "This is about as close" );
     outtextxy(  2,330, "as we can get in the" );
     outtextxy(  2,350, "presence of noise." );
   if( getch() == 27 ) exit(0);

        /* here, we mis-use a non-linear fitting algorithm
           for our simple problem. ParStatus remains set as before */
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
      /* If you know the derivatives with respect to each parameter, put
         your knowledge into a DerivModel function and replace the "nil"
         parameter with it. (Actually, here we do know; but let us assume
         we do not, and have VD_nonlinfit call the numeric differentiation
         procedure. */
   VPolyModel( YFit, XExp, sizex );  /* get fit curve from model */

   V_continuePlot;   /* go back to the last plotting region */
   VD_xy2AutoPlot( XExp, YExp, sizex, PS_NULL + SY_CROSS, LIGHTGREEN,
                   XExp, YFit, sizex, PS_SOLID, LIGHTRED );
     setviewport( 1, 1, xx, getmaxy(), 0 ); clearviewport();
     settextjustify( LEFT_TEXT, TOP_TEXT );
     settextstyle( DEFAULT_FONT, HORIZ_DIR, 1 );
     setcolor( WHITE );
     outtextxy(  2, 10, "Let us fire with the" );
     outtextxy(  2, 30, "\"cannon\" of a non-" );
     outtextxy(  2, 50, "linear fit on our" );
     outtextxy(  2, 70, "simple \"sparrow\"" );
     outtextxy(  2, 90, "problem." );
     outtextxy(  2,110, "It takes much longer" );
     outtextxy(  2,130, "to find the result..." );
     outtextxy(  2,160, "But finally we get:" );
     outtextxy(  2,180, "a0 = 0 (fix)" );
     sprintf( DataText, "a1 = %+7.5lf", FitPars[1] );
     outtextxy(  2,200, DataText );
     outtextxy(  2,220, "a2 = 0 (fix)" );
     sprintf( DataText, "a3 = %+7.5lf", FitPars[3] );
     outtextxy(  2,240, DataText );
     outtextxy(  2,260, "a4 = 0 (fix)" );
     sprintf( DataText, "a5 = %+7.5lf", FitPars[5] );
     outtextxy(  2,280, DataText );
     outtextxy(  2,310, "That is virtually the" );
     outtextxy(  2,330, "same as before." );
   if( getch() == 27 ) exit(0);

        /* finally, suppose you have several experimental
           curves, measuring the same physical process under
           slightly different conditions.
           Say, we have a vibration, and each measurement
           begins with a different phase and has a somewhat
           different amplitude, but the same frequency.  */
     VD_ramp( XExp, sizex, 0, 1.0/(sizex-1) ); /* x-axis again from 0 to 1 */
     VDx_sin( YExp,  XExp, sizex, 15.0,  0.0, 1.2 );  /* first "measurement" */
     VDx_sin( YExp2, XExp, sizex, 15.0,  0.5, 1.0 );  /* second "measurement"*/
     VDx_sin( YExp3, XExp, sizex, 15.0, -1.8, 0.75 ); /* third "measurement" */

   V_continuePlot;   /* go back to the last plotting region */
   VD_xy2AutoPlot( XExp, YExp,  sizex, PS_NULL + SY_CROSS, LIGHTGREEN,
                   XExp, YExp2, sizex, PS_NULL + SY_CROSS, LIGHTBLUE );
   VD_xyDataPlot(  XExp, YExp3, sizex, PS_NULL + SY_CROSS, YELLOW );
     setviewport( 1, 1, xx, getmaxy(), 0 ); clearviewport();
     settextjustify( LEFT_TEXT, TOP_TEXT );
     settextstyle( DEFAULT_FONT, HORIZ_DIR, 1 );
     setcolor( WHITE );
     outtextxy(  2, 10, "Finally, let us fit" );
     outtextxy(  2, 25, "several sets of experi-" );
     outtextxy(  2, 40, "mental data at once" );
     outtextxy(  2, 55, "(sine waves with the" );
     outtextxy(  2, 70, "same frequency, but" );
     outtextxy(  2, 85, "different phases and" );
     outtextxy(  2,100, "amplitudes):" );
     outtextxy(  2,115, "First you see the" );
     outtextxy(  2,130, "\"experimental\" data" );
     outtextxy(  2,145, "Please wait (may take" );
     outtextxy(  2,160, "several minutes)..." );

         /* cram your experiments into the array of VD_EXPERIMENT structs: */
   ExpList[0].X = XExp;   ExpList[0].Y = YExp;   ExpList[0].size = sizex;
   ExpList[1].X = XExp;   ExpList[1].Y = YExp2;  ExpList[1].size = sizex;
   ExpList[2].X = XExp;   ExpList[2].Y = YExp3;  ExpList[2].size = sizex;
          /* we are not using the InvVar and WeightOfExperiment fields
             of ExpList, as we are not weighting the data. */

   VI_equC( ParStatus, 7, 1 ); /* we have 1 frequency, 3 phases, and 3 amplitudes,
                  and all these 7 parameters are unknown.
                  We must provide a first guess for each of them: */
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
                enable the following line:                               */
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
   VSineModel( YFit,  XExp, sizex, 0 );  /* get fit curves from your model */
   VSineModel( YFit2, XExp, sizex, 1 );
   VSineModel( YFit3, XExp, sizex, 2 );

   V_continuePlot;   /* go back to the last plotting region */
   VD_xy2AutoPlot( XExp, YExp,  sizex, PS_NULL + SY_CROSS, LIGHTGREEN,
                   XExp, YExp2, sizex, PS_NULL + SY_CROSS, LIGHTBLUE );
   VD_xyDataPlot(  XExp, YExp3, sizex, PS_NULL + SY_CROSS, YELLOW );
   VD_xyDataPlot(  XExp, YFit, sizex, PS_SOLID, LIGHTRED );
   VD_xyDataPlot(  XExp, YFit2, sizex, PS_SOLID, LIGHTRED );
   VD_xyDataPlot(  XExp, YFit3, sizex, PS_SOLID, LIGHTRED );
     setviewport( 1, 1, xx, getmaxy(), 0 ); clearviewport();
     settextjustify( LEFT_TEXT, TOP_TEXT );
     settextstyle( DEFAULT_FONT, HORIZ_DIR, 1 );
     setcolor( WHITE );
     outtextxy(  2, 10, "Several sets of experi-" );
     outtextxy(  2, 25, "mental data at once" );
     outtextxy(  2, 40, "(sine waves with the" );
     outtextxy(  2, 55, "same frequency, but" );
     outtextxy(  2, 70, "different phases and" );
     outtextxy(  2, 85, "amplitudes):" );
     outtextxy(  2,110, "Here are the results" );
     outtextxy(  2,125, "(in brackets: \"true\")" );
     sprintf( DataText, "freq =%+7.5lf (15.0) ", FitPars[0] );
     outtextxy(  2,140, DataText );
     sprintf( DataText, "ph1  = %+7.5lf (0.0)", FitPars[1] );
     outtextxy(  2,155, DataText );
     sprintf( DataText, "ph2  = %+7.5lf (0.5)", FitPars[2] );
     outtextxy(  2,170, DataText );
     sprintf( DataText, "ph3  = %+7.5lf (-1.8)", FitPars[3] );
     outtextxy(  2,185, DataText );
     sprintf( DataText, "amp1 = %+7.5lf (1.2)", FitPars[4] );
     outtextxy(  2,200, DataText );
     sprintf( DataText, "amp2 = %+7.5lf (1.0)", FitPars[5] );
     outtextxy(  2,215, DataText );
     sprintf( DataText, "amp3 = %+7.5lf (0.75)", FitPars[6] );
     outtextxy(  2,230, DataText );

   getch();
   closegraph();
   V_freeAll();
}


