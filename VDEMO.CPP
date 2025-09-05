/*******************************   VDEMO.CPP *****************************
*                                                                        *
*                  Sample DOS program for the demonstration              *
*                        of OptiVec for Borland C/C++                    *
*                                                                        *
*                                                                        *
*   Copyright 1996-2002 by OptiCode - Dr. Martin Sander Software Dev.    *
*                                                                        *
**************************************************************************/

#define BGI_PATH "\\BC\\BGI\\"  // adjust this line, if necessary!
#include <VFstd.h>
#include <VFmath.h>
#include <VCFstd.h>
#include <VCFmath.h>
#include <Vgraph.h>
#include <math.h>
#include <DOS.h>
#include <conio.h>
#include <stdlib.h>
NEWMATHERR

struct time    t1, t2;
void starttime( void )
{
   gettime( &t1 );
}

double stoptime( void )
{
   double         tBegin, tEnd;
   gettime( &t2 );
   tBegin = t1.ti_min * 60L + t1.ti_sec + t1.ti_hund * 0.01;
   tEnd   = t2.ti_min * 60L + t2.ti_sec + t2.ti_hund * 0.01;

   return( tEnd - tBegin );
}

extern unsigned _ovrbuffer = 0x0800;
void main( void )
{
   ui             xsize=4096, specsiz=512, i, j;
   fVector        X, Y, Win, Spc, Time, Freq;
   float          a, b, c, deltat, fOscill, fNyquist;
   cfVector       CX, CY;

   printf( "\n\nThis is a short demonstration of OptiVec for C/C++"
           "\nin DOS real mode."
           "\nHit any key to continue  or abort with [ESC] !" );
   if( getch() == 27 ) exit(0);

   X    = VF_vector( xsize );
   Y    = VF_vector( xsize );
   Spc  = VF_vector( specsiz+1);
   Win  = VF_vector( 2*specsiz );
   Time = VF_vector( xsize );
   Freq = VF_vector( specsiz+1 );

   deltat   = 0.001;  /* sampling interval 1 millisecond */
   fNyquist = 0.5 / deltat;   /* Nyquist frequency = 500 Hz */
   fOscill  = 100;   /* say we are sampling a 100 Hz oscillation */
   VF_ramp( Time, xsize, 0, deltat );
   VF_ramp( Freq, specsiz+1, 0, fNyquist / specsiz );
   VFx_sin( X, Time, xsize, 2*M_PI*fOscill, 0, 1 );  /* sin( omega*t) */
   VF_cmpC( X, X, xsize, 0.3 ); /* get an asymmetric square wave */
   V_initGraph( BGI_PATH );

   VF_Welch( Win, 2*specsiz );
               /* You might as well take VF_Parzen or VF_Hanning  */
   Spc[specsiz] = VF_spectrum( Spc, specsiz, X, xsize, Win );

   VF_xyAutoPlot( Time, X, 101, PS_SOLID, GREEN );
   setviewport( 1, 1, getmaxx() / 5, 200, 0 ); clearviewport();
   settextjustify( LEFT_TEXT, TOP_TEXT );
   setcolor( WHITE );
     outtextxy( 5,  20, "Portion" );
     outtextxy( 5,  40, "of an" );
     outtextxy( 5,  60, "asymmetric" );
     outtextxy( 5,  80, "square" );
     outtextxy( 5, 100, "wave" );
     outtextxy( 5, 120, "f = 100 Hz" );
   if( getch() == 27 ) exit(0);
   VF_xyAutoPlot( Freq, Spc, specsiz+1, PS_SOLID | SY_CROSS, GREEN );
   setviewport( 1, 1, getmaxx() / 5, 200, 0 ); clearviewport();
   settextjustify( LEFT_TEXT, TOP_TEXT );
   setcolor( WHITE );
     outtextxy( 5,  20, "Power" );
     outtextxy( 5,  40, "Density" );
     outtextxy( 5,  60, "Spectrum" );
     outtextxy( 5,  80, "of the" );
     outtextxy( 5, 100, "same" );
     outtextxy( 5, 120, "square" );
     outtextxy( 5, 140, "wave." );
     outtextxy( 5, 160, "Note the" );
     outtextxy( 5, 180, "higher" );
     outtextxy( 5, 200, "harmonics" );
     outtextxy( 5, 220, "of the" );
     outtextxy( 5, 240, "100 Hz" );
     outtextxy( 5, 260, "ground freq." );
   if( getch() == 27 ) exit(0);
   V_nfree( 4, Spc, Win, Time, Freq );

   /* let's have a look at complex arithmetics  */
   CX = VCF_vector( 1000 );
   CY = VCF_vector( 1000 );
   VCF_ramp( CX, 1000, fcplx(M_PI, 0), fcplx(0.04, 0.0001) );
   VCF_sin( CY, CX, 1000 );
   VCF_cos( CX, CX, 1000 );
        /*  Try also with other complex functions! */

   VCF_2AutoPlot( CY, 1000, PS_SOLID, GREEN,
                  CX, 1000, PS_SOLID, RED );
   setviewport( 1, 1, getmaxx() / 5, 200, 0 ); clearviewport();
   settextjustify( LEFT_TEXT, TOP_TEXT );
   setcolor( WHITE );
     outtextxy( 5,  20, "Playing with" );
     outtextxy( 5,  40, "functions in" );
     outtextxy( 5,  60, "the complex" );
     outtextxy( 5,  80, "plane:" );
     outtextxy( 5, 105, "Green:" );
     outtextxy( 5, 125, "    sine," );
     outtextxy( 5, 150, "Red:");
     outtextxy( 5, 170, "    cosine." );
   if( getch() == 27 ) exit(0);
   V_nfree( 2, CX, CY );
   closegraph();

   VF_ramp( X, xsize, -10, 0.020 );
   a = 1.1;  b = -0.15;  c = 0.02;   /* or any other arbitrary values... */
   printf( "\nTo end this short program, let us make a crude speed comparison"
           "\nbetween compiled code and OptiVec functions."
           "\nE.g., evaluate the exponential function for a whole vector"
           "\n\n          y[i] = c * exp( a * x[i] + b ),     i=0,..,size-1"
           "\n\n(200 runs of 4096 elements each).      Please wait...\n" );
   starttime();
   for( i=0; i<200; i++ )   /* repeat enough times to see the difference */
      for( j=0; j<xsize; j++ )  Y[j] = c * exp( a * X[j] + b );
   printf( "\nExecution Time of compiled code:        %lf sec", stoptime() );
      /* now the OptiVec solution to the same simple problem:  */
   starttime();
   for( i=0; i<200; i++ )
      VFx_exp( Y, X, xsize, a, b, c );
   printf( "\nExecution Time of the OptiVec function: %lf sec", stoptime() );

   printf( "\n\nNormally, the OptiVec function is about 2-3 times faster"
             "\nthan compiled code. In some environments, up to a factor of 10"
             "\nmay be gained."
           "\n\nChange the source code of this example program and try also"
             "\nother mathematical functions, like the sine, the hyperbolic"
             "\nsine, and so on..." );
            getch();
   V_nfree( 2, X, Y );
}

