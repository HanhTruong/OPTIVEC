/***********************   FITDEMOW.CPP  *********************************
*                                                                        *
*            Data-fitting demo program for                               *
*                  O p t i V e c                                         *
*          with Borland C++ 4.x, 5.x, C++ Builder,                       *
*           or Microsoft Visual C++ 5.0 or 6.0                           *
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
BC++ Builder 4+:  Don't use this file, but open the project FITDEMOB.BPR.

Borland C++, Command-line:
    1. Go to your OptiVec directory.
    2. Be sure, your Borland C++ BIN directory is contained in the
       PATH statement in your AUTOEXEC.BAT.
    3. a) 32-bit: type
               BCC32 -W -Iinclude fitdemo.cpp lib\vcf3w.lib
       b) 16-bit: type
               BCC -ml -W -Iinclude fitdemo.cpp lib\vcl3w.lib
    4. Run the program by typing:  fitdemo

Borland C++, IDE (not for C++ Builder):
    1. Open the new-project menu with  Project/New.
    2. Create a project FITDEMO in the OptiVec directory,
       e.g., \bc\optivec.
    3.a) 32-bit: Choose Application[EXE] for Win32 GUI, static linking,
                 single-thread.
    3.b) 16-bit: Choose Application[EXE] for Windows3.x, memory model LARGE.
    4. Hit OK.
    5. In the Project window that will now be on the screen, delete the
       nodes FITDEMO.DEF and FITDEMO.RC.
    6.a) 32-bit: Add the node VCF3W.LIB.
    6.b) 16-bit: Add the nodes VCL3W.LIB and MCL3W.LIB.
    7. In Options/Project, be sure the include-file search path and the
       library search path both include the respective  OPTIVEC directories,
       e.g.: \bc\optivec\include and bc\optivec\lib, resprectively
    8. Compile and run.

Microsoft Visual C++:
    1. Create a new project as a "Win32 application".
    2. In the project settings, C/C++, Code Generation,
       verify that single-thread debug is chosen.
    3. Add \OptiVec\include to the include-file search path.
    4. Add the files  FITDEMOW.CPP  and  OVVCSD.LIB to your project.
    5. Compile and run.
*/

#include <windows.h>                    /* Compiler's include files */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <VDstd.h>                      /* OptiVec include files */
#include <VDmath.h>
#include <MDstd.h>
#include <VIstd.h>
#include <Vgraph.h>
HWND     hWndMain;
int      vview;
dVector  XExp, X2, YExp, YFit, YExp2, YFit2, YExp3, YFit3;
ui       sizex;
double   FitPars[7];   // the highest number of parameters we will have in our examples
int      ParStatus[7];
#define  polydeg 5     // refers to the polynomial fitting example
#ifndef M_PI
    #define M_PI  3.14159265358979323846
#endif
NEWMATHERR  // this macro has an effect only for 16-bit BC

LONG FAR PASCAL MainMessageHandler (HWND, UINT, WPARAM, LPARAM);

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

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR /* lpCmdLine */, int /* nCmdShow */)
{
    MSG       msg;                      /* MSG structure to pass to windows proc */
    WNDCLASS  wc;
    char      *AppName;

    AppName = "FitDemo";
    if(!hPrevInstance)
    {
        wc.style      = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc= MainMessageHandler;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance  = hInstance;
        wc.hIcon      = LoadIcon (hInstance, AppName);
        wc.hCursor    = LoadCursor (NULL, IDC_ARROW);
        wc.hbrBackground  = (HBRUSH) GetStockObject (WHITE_BRUSH);
        wc.lpszMenuName   = AppName;
        wc.lpszClassName  = AppName;
        RegisterClass (&wc);
    }
                             /* create application's Main window:  */
    hWndMain = CreateWindow (AppName,
                             "OptiVec Data-Fitting Demo",
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT,     /* Use default X, Y, and width  */
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             NULL,              /* Parent window's handle      */
                             NULL,              /* Default to Class Menu       */
                             hInstance,         /* Instance of window          */
                             NULL);             /* Create struct for WM_CREATE */


    if (hWndMain == NULL)
    {
        MessageBox(NULL, "Could not create window in WinMain", NULL, MB_ICONEXCLAMATION);
        return (1);
    }

    ShowWindow(hWndMain, SW_SHOWMAXIMIZED);     /* Display main window      */
    UpdateWindow(hWndMain);

    while(GetMessage(&msg, NULL, 0, 0)) /* Main message loop */
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnregisterClass (AppName, hInstance);
    return (msg.wParam);
}

LONG FAR PASCAL MainMessageHandler(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    HDC         vDC;
    PAINTSTRUCT ps;
    RECT        r;

    VD_NONLINFITOPTIONS Opt;
    VD_EXPERIMENT       ExpList[3];
    char                DataText[80];


    switch (Message)                    /* Windows Message Loop           */
    {
        case WM_CREATE:
            sizex   = 200;
            vview   = 0;
            XExp  = VD_vector( sizex );
            YExp  = VD_vector( sizex );
            YFit  = VD_vector( sizex );
            YExp2 = VD_vector( sizex );
            YExp3 = VD_vector( sizex );
            YFit2 = VD_vector( sizex );
            YFit3 = VD_vector( sizex );
            break;

        case WM_RBUTTONDOWN:
            if( ++vview > 5 ) vview = 0;
            InvalidateRect( hWnd, NULL, TRUE ); // Demand new Paint to display the next view
            break;

        case WM_PAINT:
            vDC = BeginPaint(hWndMain, &ps);
            V_initPlot( hWndMain, vDC );
            GetClientRect( hWndMain, &r );
            V_setPlotRegion( r.left+(r.right-r.left)/4, r.top, r.right, r.bottom );
            switch( vview )
            {
                default: break;
                case 0:
                  TextOut( vDC, 10, 10,
                  "This is a series of graphs illustrating OptiVec data-fitting functions.", 71 );
                  TextOut( vDC, 10, 40,
                  "Always press the right mouse button to see the next view!", 56 );
                  TextOut( vDC, 10, 70,
                  "Press [Alt] [F4] to end the demonstration.", 41 );
               break;
               case 1:
                  TextOut( vDC, 2, 10, "Suppose these are", 17 );
                  TextOut( vDC, 2, 30, "your experimental", 17 );
                  TextOut( vDC, 2, 50, "data points.", 12 );
                  TextOut( vDC, 2, 70, "(Actually, they consist", 23 );
                  TextOut( vDC, 2, 90, "of a simple cubic with", 22 );
                  TextOut( vDC, 2,110, "1% added noise.)", 16 );

                  VD_ramp( XExp, sizex, 0, 1.0/(sizex-1) ); // "experimental" x-axis from 0 to 1
                  VD_cubic( YExp, XExp, sizex );        // fake "measured" y-data as y = x^3
                  VD_noise( YFit, sizex, 1, 0.005 );
                  VD_addV( YExp, YExp, YFit, sizex ); // add 1% peak-to-peak "experimental noise"
                  VD_xyAutoPlot( XExp, YExp, sizex, PS_NULL+SY_CROSS, GREEN );
               break;
               case 2:   // fit your data to one of the simplest models, a polynomial
                  TextOut( vDC, 2, 10, "The red curve is a", 18 );
                  TextOut( vDC, 2, 30, "fifth-order polynomial", 22 );
                  TextOut( vDC, 2, 50, "fitted to your data.", 20 );
                  TextOut( vDC, 2, 70, "Without noise, the", 18 );
                  TextOut( vDC, 2, 90, "coefficients should", 19 );
                  TextOut( vDC, 2,110, "have been:", 10 );
                  TextOut( vDC, 2,130, "{0, 0, 0, 1.0, 0, 0}", 20 );

                  VD_polyfit( FitPars, polydeg, XExp, YExp, sizex );
                  VD_poly( YFit, XExp, sizex, FitPars, polydeg ); // calculate fit curve
                  TextOut( vDC, 2,160, "Actually, we got:", 17 );
                  sprintf( DataText, "a0 = %+7.5lf", FitPars[0] );
                  TextOut( vDC, 2,180, DataText, 13 );
                  sprintf( DataText, "a1 = %+7.5lf", FitPars[1] );
                  TextOut( vDC, 2,200, DataText, 13 );
                  sprintf( DataText, "a2 = %+7.5lf", FitPars[2] );
                  TextOut( vDC, 2,220, DataText, 13 );
                  sprintf( DataText, "a3 = %+7.5lf", FitPars[3] );
                  TextOut( vDC, 2,240, DataText, 13 );
                  sprintf( DataText, "a4 = %+7.5lf", FitPars[4] );
                  TextOut( vDC, 2,260, DataText, 13 );
                  sprintf( DataText, "a5 = %+7.5lf", FitPars[5] );
                  TextOut( vDC, 2,280, DataText, 13 );
                  TextOut( vDC, 2,310, "Note how even moderate", 22 );
                  TextOut( vDC, 2,330, "noise leads to rather", 21 );
                  TextOut( vDC, 2,350, "large errors in the", 19 );
                  TextOut( vDC, 2,370, "fit parameters, if", 18 );
                  TextOut( vDC, 2,390, "there are too many", 18 );
                  TextOut( vDC, 2,410, "'free' parameters.", 18 );
                  VD_xy2AutoPlot( XExp, YExp, sizex, PS_NULL | SY_CROSS, GREEN,
                                  XExp, YFit, sizex, PS_SOLID, LIGHTRED );
               break;
               case 3:  // refine your fit by switching to a general linear model,
                        // giving you the chance to consider only the uneven terms
                  TextOut( vDC, 2, 10, "Suppose you know that", 21 );
                  TextOut( vDC, 2, 30, "the coefficients of", 19 );
                  TextOut( vDC, 2, 50, "all even terms are 0.", 21 );
                  TextOut( vDC, 2, 70, "Then you fit to your", 20 );
                  TextOut( vDC, 2, 90, "own linear model,", 17 );
                  TextOut( vDC, 2,110, "consisting only of", 18 );
                  TextOut( vDC, 2,130, "uneven terms.", 13 );
                  TextOut( vDC, 2,160, "Now we get:", 11 );

                  ParStatus[0] = ParStatus[2] = ParStatus[4] = 0;  //disable fitting of even terms
                  FitPars[0] = FitPars[2] = FitPars[4] = 0.0;  // set their coefficients to the known value, 0.0
                  ParStatus[1] = ParStatus[3] = ParStatus[5] = 1;  // enable fitting of uneven terms
                     // the disabled fitting parameters must be initialized before calling VD_linfit !
                  VD_linfit( FitPars, ParStatus, polydeg+1,
                             XExp, YFit, sizex,
                             PolyModel );
                  VD_poly( YFit, XExp, sizex, FitPars, polydeg ); // calculate new fit curve

                  TextOut( vDC, 2,180, "a0 = 0 (fix)", 12 );
                  sprintf( DataText, "a1 = %+7.5lf", FitPars[1] );
                  TextOut( vDC, 2,200, DataText, 13 );
                  TextOut( vDC, 2,220, "a2 = 0 (fix)", 12 );
                  sprintf( DataText, "a3 = %+7.5lf", FitPars[3] );
                  TextOut( vDC, 2,240, DataText, 13 );
                  TextOut( vDC, 2,260, "a4 = 0 (fix)", 12 );
                  sprintf( DataText, "a5 = %+7.5lf", FitPars[5] );
                  TextOut( vDC, 2,280, DataText, 13 );
                  TextOut( vDC, 2,310, "This is about as close", 22 );
                  TextOut( vDC, 2,330, "as we can get in the", 20 );
                  TextOut( vDC, 2,350, "presence of noise.", 18 );
                  VD_xy2AutoPlot( XExp, YExp, sizex, PS_NULL | SY_CROSS, GREEN,
                                  XExp, YFit, sizex, PS_SOLID, LIGHTRED );
                  break;
               case 4:  // here, we mis-use a non-linear fitting algorithm
                        // for our simple problem.
                  TextOut( vDC, 2, 10, "Let's fire with the", 19 );
                  TextOut( vDC, 2, 30, "'cannon' of a non-", 18 );
                  TextOut( vDC, 2, 50, "linear fit on our", 17 );
                  TextOut( vDC, 2, 70, "simple 'sparrow'", 16 );
                  TextOut( vDC, 2, 90, "problem.", 8 );
                  TextOut( vDC, 2,110, "It takes much longer", 20 );
                  TextOut( vDC, 2,130, "to find the result...", 21 );

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

                  TextOut( vDC, 2,160, "But finally we get:", 19 );
                  TextOut( vDC, 2,180, "a0 = 0 (fix)", 12 );
                  sprintf( DataText, "a1 = %+7.5lf", FitPars[1] );
                  TextOut( vDC, 2,200, DataText, 13 );
                  TextOut( vDC, 2,220, "a2 = 0 (fix)", 12 );
                  sprintf( DataText, "a3 = %+7.5lf", FitPars[3] );
                  TextOut( vDC, 2,240, DataText, 13 );
                  TextOut( vDC, 2,260, "a4 = 0 (fix)", 12 );
                  sprintf( DataText, "a5 = %+7.5lf", FitPars[5] );
                  TextOut( vDC, 2,280, DataText, 13 );
                  TextOut( vDC, 2,310, "That is virtually the", 21 );
                  TextOut( vDC, 2,330, "same as before.", 15 );
                  VD_xy2AutoPlot( XExp, YExp, sizex, PS_NULL | SY_CROSS, GREEN,
                                  XExp, YFit, sizex, PS_SOLID, LIGHTRED );
               break;
               case 5:  // finally, let's suppose you have several experimental
                        // curves, measuring the same physical process under
                        // slightly different conditions.
                        // Say, we have a vibration, and each measurement
                        // begins with a different phase and has a somewhat
                        // different amplitude, but the same frequency.
                  TextOut( vDC, 2, 10, "Finally, let us fit", 19 );
                  TextOut( vDC, 2, 30, "several sets of experi-", 23 );
                  TextOut( vDC, 2, 50, "mental data at once", 19 );
                  TextOut( vDC, 2, 70, "(sine waves with the", 20 );
                  TextOut( vDC, 2, 90, "same frequency, but", 19 );
                  TextOut( vDC, 2,110, "different phases and", 20 );
                  TextOut( vDC, 2,130, "amplitudes):", 12 );
                  TextOut( vDC, 2,160, "First you see the", 17 );
                  TextOut( vDC, 2,180, "'experimental' data", 19 );
                  TextOut( vDC, 2,200, "Please wait (may take", 21 );
                  TextOut( vDC, 2,220, "several minutes)...", 19 );

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
                      enable the following line:                               */
                  // Opt.LevelOfMethod = 7;
                  VD_setNonlinfitOptions( &Opt );

                  VD_multiNonlinfit( FitPars, ParStatus, 7,
                                     ExpList, 3,
                                     VSineModel,
                                     NULL ); // Again, we pretend we don't know the derivatives
                            // bring the phases into the range -PI < phase < + PI
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
                  #if defined __FLAT__ || defined _WIN32
                     SetViewportOrgEx( vDC, r.left, r.top, NULL );
                  #else
                     SetViewportOrg( vDC, r.left, r.top );
                  #endif  // to write text after plotting, go back to full window

                  TextOut( vDC, 2,255, "Here are the results", 20 );
                  TextOut( vDC, 2,275, "(in brackets: 'true')", 21 );
                  sprintf( DataText, "freq =%+7.5lf (15.0) ", FitPars[0] );
                  TextOut( vDC, 2,310, DataText, 22 );
                  sprintf( DataText, "ph1  = %+7.5lf (0.0)", FitPars[1] );
                  TextOut( vDC, 2,330, DataText, 21 );
                  sprintf( DataText, "ph2  = %+7.5lf (0.5)", FitPars[2] );
                  TextOut( vDC, 2,350, DataText, 21 );
                  sprintf( DataText, "ph3  = %+7.5lf (-1.8)", FitPars[3] );
                  TextOut( vDC, 2,370, DataText, 22 );
                  sprintf( DataText, "amp1 = %+7.5lf (1.2)", FitPars[4] );
                  TextOut( vDC, 2,390, DataText, 21 );
                  sprintf( DataText, "amp2 = %+7.5lf (1.0)", FitPars[5] );
                  TextOut( vDC, 2,410, DataText, 21 );
                  sprintf( DataText, "amp3 = %+7.5lf (0.75)", FitPars[6] );
                  TextOut( vDC, 2,430, DataText, 22 );

                  V_continuePlot();   // go back to plot viewport
                  VD_xyDataPlot(  XExp, YFit, sizex, PS_SOLID, LIGHTRED );
                  VD_xyDataPlot(  XExp, YFit2, sizex, PS_SOLID, LIGHTRED );
                  VD_xyDataPlot(  XExp, YFit3, sizex, PS_SOLID, LIGHTRED );
            }
            EndPaint(hWndMain, &ps);
            break;

        case WM_CLOSE:
            V_freeAll();                    /* delete all allocated vectors */
            DestroyWindow(hWnd);
            if (hWnd == hWndMain)
                PostQuitMessage(0);
            break;

        default:
            return (DefWindowProc(hWnd, Message, wParam, lParam));
    }  // end of switch( message )
    return (0);
}


