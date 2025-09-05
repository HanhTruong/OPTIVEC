/***********************   VDEMOW.CPP  ***********************************
*                                                                        *
*                Demo program for                                        *
*                  O p t i V e c                                         *
*          with Borland C++ 4.x, 5.x, C++ Builder,                       *
*           or Microsoft Visual C++ 5.0 or 6.0                           *
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
BC++ Builder 4+:  Don't use this file, but open the project VDEMOB.BPR.

Borland C++, Command-line:
    1. Go to your OptiVec directory.
    2. Be sure, your Borland C++ BIN directory is contained in the
       PATH statement in your AUTOEXEC.BAT.
    3. a) 32-bit: type
               BCC32 -W -Iinclude vdemow.cpp lib\vcf3w.lib
       b) 16-bit: type
               BCC -ml -W -Iinclude vdemow.cpp lib\vcl3w.lib
    4. Run the program by typing:  vdemow

Borland C++, IDE (not for C++ Builder):
    1. Open the new-project menu with  Project/New.
    2. Create a project VDEMOW in the OptiVec directory,
       e.g., \bc\optivec.
    3.a) 32-bit: Choose Application[EXE] for Win32 GUI, static linking,
                 single-thread.
    3.b) 16-bit: Choose Application[EXE] for Windows3.x, memory model LARGE.
    4. Hit OK.
    5. In the Project window that will now be on the screen, delete the
       nodes VDEMOW.DEF and VDEMOW.RC.
    6.a) 32-bit: Add the node VCF3W.LIB.
    6.b) 16-bit: Add the node VCL3W.LIB.
    7. In Options/Project, be sure the include-file search path and the
       library search path both include the respective  OPTIVEC directories,
       e.g.: \bc\optivec\include and bc\optivec\lib, resprectively
    8. Compile and run.

Microsoft Visual C++:
    1. Create a new project as a "Win32 application".
    2. In the project settings, C/C++, Code Generation,
       verify that single-thread debug is chosen.
    3. Add \OptiVec\include to the include-file search path.
    4. Add the files  VDEMOW.CPP  and  OVVCSD.LIB to your project.
    5. Compile and run.
*/

#include <windows.h>                    /* Compiler's include files */
#include <stdio.h>
#include <math.h>
#include <string.h>
#if defined __FLAT__ || defined _WIN32
    #include <winbase.h>
#else
    #include <dos.h>
#endif

#include <VFstd.h>                      /* OptiVec include files */
#include <VCFstd.h>
#include <VFmath.h>
#include <VCFmath.h>
#include <VIstd.h>
#include <Vgraph.h>
#ifndef M_PI
    #define M_PI  3.14159265358979323846
#endif
HWND     hWndMain;
fVector  X1, X2, Y1, Y2, Y3, Y4, Spc, Freq, Win;
cfVector CX1, CX2;
iVector  I1;
ui       vsize, spcsize;
float    deltat, fOscill, fNyquist;
int      vview;
#if defined __FLAT__ || defined WIN32 || defined _WIN32
   struct _SYSTEMTIME tStart, tStop;
#else
    struct   time tStart, tStop;
#endif
NEWMATHERR

LONG FAR PASCAL MainMessageHandler (HWND, UINT, WPARAM, LPARAM);

void StartTime( void )
{
#if defined __FLAT__ || defined WIN32 || defined _WIN32
    GetLocalTime( &tStart );
#else
    gettime( &tStart );
#endif
}

double StopTime( void )
{
   double   tBegin, tEnd;
#if defined __FLAT__ || defined WIN32 || defined _WIN32
    GetLocalTime( &tStop );
    tBegin = (tStart.wHour * 60 + tStart.wMinute) * 60L
            + tStart.wSecond + tStart.wMilliseconds * 0.001;
    tEnd   = (tStop.wHour * 60 + tStop.wMinute) * 60L
            + tStop.wSecond + tStop.wMilliseconds * 0.001;
#else
   gettime( &tStop );
   tBegin = (tStart.ti_hour * 60 + tStart.ti_min) * 60L
            + tStart.ti_sec + tStart.ti_hund * 0.01;
   tEnd   = (tStop.ti_hour * 60 + tStop.ti_min)  * 60L
            + tStop.ti_sec + tStop.ti_hund * 0.01;
#endif
   return( tEnd - tBegin );
}


int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR /* lpCmdLine */, int /* nCmdShow */)
{
    MSG       msg;                      /* MSG structure to pass to windows proc */
    WNDCLASS  wc;
    char      *AppName;                 /* Name for the window */

    AppName = "VDemoW";                 /* The name of this application */
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
                             "OptiVec Demo",
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

LONG FAR PASCAL
MainMessageHandler(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    HDC         vDC;
    PAINTSTRUCT ps;

    char      TimeMsg[100];
    unsigned  i, j;
    float     A=1.2f, B=-0.13f, C=0.85f;
	double    xt;


    switch (Message)                    /* Windows Message Loop           */
    {
        case WM_CREATE:
            vsize  = 4096;
            spcsize = 256;
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
            break;

        case WM_PAINT:
            vDC = BeginPaint(hWndMain, &ps);
            V_initPlot( hWndMain, vDC );
            switch( vview )
            {
                default: break;
                case 0:
                  TextOut( vDC, 10, 10,
                  "This is a series of graphs illustrating OptiVec functions.", 58 );
                  TextOut( vDC, 10, 40,
                  "Always press the right mouse button to see the next view!", 56 );
                  TextOut( vDC, 10, 70,
                  "Press [Alt] [F4] to end the demonstration.", 41 );
               break;
               case 1:
                  TextOut( vDC, 0, 10, "Sine wave", 9 );
                  TextOut( vDC, 0, 30, "f = 100 Hz", 10 );
                  VF_xyAutoPlot( X1, Y1, 101, PS_SOLID, LIGHTRED );
                             // show only 10 waves
               break;
               case 2:
                  TextOut( vDC, 0, 10, "Asymmetric", 10 );
                  TextOut( vDC, 0, 30, "square wave", 11 );
                  TextOut( vDC, 0, 50, "f = 100 Hz", 10 );
                  V_drawAxes( 0, 100, -0.3, 1.2 );
                  VF_yDataPlot( Y2, 101, PS_SOLID, BLUE );
                             // show again only 10 waves
               break;
               case 3:
                  TextOut( vDC, 0,  10, "Frequency spectrum", 18 );
                  TextOut( vDC, 0,  30, "of the square wave", 18 );
                  TextOut( vDC, 0,  50, "Note the higher", 15 );
                  TextOut( vDC, 0,  70, "harmonics of the", 16 );
                  TextOut( vDC, 0,  90, "100 Hz ground freq.", 19 );
                  TextOut( vDC, 0, 110, "and the f=0 term", 16 );
                  TextOut( vDC, 0, 130, "due to the asymmetry", 20 );
                  TextOut( vDC, 0, 170, "Don't be sur-", 13 );
                  TextOut( vDC, 0, 190, "prised; the next", 16 );
                  TextOut( vDC, 0, 210, "example will show", 17 );
                  TextOut( vDC, 0, 230, "a bit of error", 14 );
                  TextOut( vDC, 0, 250, "handling!", 9 );
                  VF_xyAutoPlot( Freq, Spc, spcsize+1, PS_SOLID | SY_CROSS, GREEN );
               break;
               case 4:
                  TextOut( vDC, 0,  10, "Trigonometric", 13 );
                  TextOut( vDC,10,  30, "functions:", 10 );
                  TextOut( vDC, 0,  50, "Red:   sine", 11 );
                  TextOut( vDC, 0,  70, "Green: tangent", 14 );
                  TextOut( vDC, 0,  90, "Blue:  cosecant", 15 );
                  TextOut( vDC, 0, 120, "(sine:", 6 );
                  TextOut( vDC, 0, 140, "   *10 and +20,", 15 );
                  TextOut( vDC, 0, 160, " cosecant: -20)", 15 );
                  VF_xy2AutoPlot( X1, Y2, vsize, PS_SOLID, LIGHTGREEN,
                                  X1, Y3, vsize, PS_SOLID, LIGHTBLUE );
                  VF_xyDataPlot( X1, Y1, vsize, PS_SOLID, LIGHTRED );
               break;
               case 5:
                  TextOut( vDC, 0,  10, "Playing with functions", 22 );
                  TextOut( vDC, 0,  30, "in the complex plane:", 21 );
                  TextOut( vDC, 0,  60, "Red:   complex sine",  19 );
                  TextOut( vDC, 0,  80, "Green: complex cosine",  21 );
                  VCF_2AutoPlot( CX1, vsize, PS_SOLID, LIGHTRED,
                                 CX2, vsize, PS_SOLID, LIGHTGREEN );
               break;
               case 6:
                  TextOut( vDC, 0, 10, "To end the demo, let's make a crude speed comparison", 52 );
                  TextOut( vDC, 0, 30, "between compiled code and OptiVec code.", 39 );
                  V_nfree( 2, X1, X2 );
                  X1 = VF_vector( 4096 );
                  VF_ramp( X1, 4096, -10, 0.005f );
                  X2 = VF_vector( 4096 );
                  TextOut( vDC, 0, 55, "E.g., evaluate the exponential function for a whole vector", 58 );
                  TextOut( vDC, 20, 80, "Y[i] = c * exp[ a * X[i] + b],  i=0,..,size-1", 45 );
                  TextOut( vDC, 0, 105, "(200 runs of 4096 elements each).   Please wait...", 49 );
                  StartTime();
                      for( i=0; i<200; i++ )
                         for( j=0; j<4096; j++ )
                            X2[j] = (float)(C * exp( A * X1[j] + B ));
                  xt = StopTime();
                  sprintf( TimeMsg, "Execution Time for classic loop version: %lf sec", xt );
                  TextOut( vDC, 0, 130, TimeMsg, strlen( TimeMsg ));
                  StartTime();
                      for( i=0; i<200; i++ )
                            VFx_exp( X2, X1, 4096, A, B, C );
                  xt = StopTime();
                  sprintf( TimeMsg, "Execution Time for OptiVec version:       %lf sec", xt );
                  TextOut( vDC, 0, 155, TimeMsg, strlen( TimeMsg ));
                  TextOut( vDC, 0, 190, "Generally, the OptiVec version is about 1.5 to 3 times faster than classic", 74 );
                  TextOut( vDC, 0, 210, "loops; in some environments, up to a factor of 8 can be gained!", 63 );
                break;
            }
            EndPaint(hWndMain, &ps);
            break;

        case WM_RBUTTONDOWN:
            switch( vview )
            {
               default: vview=-1;
               case 0:  deltat   = 0.001f;  /* sampling interval 1 millisecond */
                        fNyquist = 0.5f / deltat;   /* Nyquist frequency = 500 Hz */
                        fOscill  = 100;   /* say we are sampling a 100 Hz oscillation */
                        VF_ramp( X1, vsize, 0, deltat );  /* Time axis */
                        VF_ramp( Freq, spcsize+1, 0, fNyquist / spcsize );
                        VFx_sin( Y1, X1, vsize, (float)(2*M_PI*fOscill), 0, 1 ); /* sine wave( omega*t) */
                        break;
               case 1:  VF_cmp_gtC( Y2, Y1, vsize, 0.7f );
                              //  transform the sine into an asymmetric square wave
                        break;
               case 2:  VF_Hanning( Win, 2*spcsize );
                        Spc[spcsize] = VFs_spectrum( Spc, spcsize, Y2, vsize, Win );
                              //  analyse the frequency spectrum of the square wave
                        break;
               case 3:  VI_ramp( I1, vsize, 0, 1 );
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
                        break;
               case 4:  VCF_ramp( CX1, vsize, fcplx( (float)(M_PI), 0),
                                              fcplx(0.4f, 0.008f) );
                        VCF_cos( CX2, CX1, vsize );
                        VCF_sin( CX1, CX1, vsize );
                              //  Try also with other complex functions!
                        break;
               case 5:  break;
            }
            vview++;
            InvalidateRect( hWnd, NULL, TRUE ); // Repaint to display the next view
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


