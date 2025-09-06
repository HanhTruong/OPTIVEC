QalibServer repository is using OptiVec library for the analysis functions.


      ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
      ³                                                               ³
      ³                     OptiVec for C/C++                         ³
      ³                                                               ³
      ³                    Registered Version 3                       ³
      ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

Contents:
     1. Introduction
     2. System requirements
     3. Installation
        3.1 Installation from CD-ROM
        3.2 Installation of the Electronically Distributed Version
     4. Running the examples
     5. Documentation
     6. What's New?
     7. Copyright
   

1. Introduction
---------------
The largest portion of almost all programs processing numerical data
consists of loops. Replace these loops by the specialized vector
and matrix functions of OptiVec, and exploit all the advantages of
vectorized programming:

- OptiVec was almost entirely written in Assembly language. The result is
  highly optimized and numerically accurate code, running 2-3 times faster
  than compiled C/C++ source code of the same functionality.

- fully vectorized forms of all operators and functions of the language
  C/C++ for all integer and floating-point (including complex!) data types

- Additionally many other real and complex math functions, statistics,
  analysis, FFT techniques, graphics - in total more than 3000!

- Simple and logical syntax, for example
  "VF_exp( Y, X, size );"      for Y[i] = exp( X[i] ) with "float" arrays,
  "VD_FFT( Y, X, size, 1 );"   for double-precision forward FFT


2. System requirements
----------------------
This version of OptiVec is designed for IBM AT, PS/2 or compatibles.
Depending on your choice when ordering, this is the version

- for Borland C++ (Turbo C++), version 3.0 or higher, all memory models
  of Win32, Windows3.x, and DOS; separate libraries optimized for various
  degrees of back-compatibility with processors from Pentium III down
  to 286 with or without coprocessor;  or

- for Microsoft Visual C++ 5.0 or higher, all PC models using the
  "single-thread", "multi-thread", or "multi-thread DLL" run-time
  libraries, both for debug and release, in versions for Pentium III+
  and for 486DX/Pentium.


3. Installation
---------------
If you have the Shareware version of OptiVec installed on your computer,
BE SURE TO REPLACE ALL LIBRARIES WITH THE REGISTERED VERSION, either by
choosing the same target directory, or by calling UNINSTAL.EXE for the
complete removal of the Shareware version prior to the installation of
the Registered version!

3.1 Installation from CD-ROM
----------------------------
a) On all operating systems except Windows 2000 or XP:
   Please run the installation program, INSTALL.EXE (which is in the root
   directory of the CD-ROM).
   If you don't like the default directory structure, you may change it
   afterwards.
b) On Windows 2000 or XP:
   You cannot use INSTALL.EXE, as it will hang on Windows 2000 or XP!
   Instead, please manually create a directory OPTIVEC and unzip the file
   DOC.ZIP into it.
   Then create a subdirectory OPTIVEC\LIB and unzip the file LIB.ZIP into it.
   Create a subdirectory OPTIVEC\INCLUDE and unzip the file INCLUDE.ZIP
   into it.

After you completed the installation, you must set the library search path
and the include-file search path according to your OptiVec directory choice
("Options/Directories" in Borland C 3.x, "Options/Project/Directories" in
Borland C++ 4.x or higher; "Extras/Options/Directories" in Visual C++).
The CD-ROM contains a fully installed copy of OptiVec, which is in the
directory OPTIVEC.

As an alternative to installing OptiVec on your hard disk, you may set
the include-file search path and the library search path to
D:\OPTIVEC\INCLUDE  and  D:\OPTIVEC\LIB, respectively (assuming your
CD-ROM drive is D:).


3.2 Installation of the Electronically Distributed (ESD) Version
----------------------------------------------------------------
In order to minimize the download time, only the frequently used libraries
are contained in the ESD version:
VCF4W.LIB and VCF4WD.LIB for Borland C++,
OVVCSD.LIB, OVVCSR.LIB, OVVCMTD.LIB, and OVVCMTR.LIB for Visual C++.
If you need any other libraries, please go to http://www.optivec.com/update/
to fetch them and drop us a note at  support@optivec.com  to receive your
password. If you need one or more of those rarely used libraries which we
did not put on our web-site, we'll send them to you by email upon request.

To install the ESD version, please unzip the downloaded ZIP file into a
temporary directory. Then proceed as described above for installation
from CD-ROM.


4. Running the examples
-----------------------

Check your installation by compiling and running the appropriate demo file.
Follow the instructions in the header of that file.
If you get compiler errors like "Cannot open include file ...", or a
linker error "Unresolved external", you probably did not set the library
and include paths correctly.

OptiVec for Borland C/C++:
  VDEMOB.BPR  is a BC++ Builder 4+ project,
  VDEMOW.CPP  is for Windows with Borland C++ 5.0 or higher.
  VDEMO.CPP   is a DOS program,
  Separate versions of the Windows demo program are supplied for
  older BC versions:
  VDEMOW1.CPP is for Borland C++ 3.x with OWL 1.0
  VDEMOW2.CPP is for Borland C++ 4.0 with OWL 2.0

  For BC++ Builder 4+, open the project VDEMOB.BPR, compile and run.

  For Windows95, 98 or NT with Borland C++ 4.x, 5.x or higher (without OWL),
      either do a command-line compile:
          a) 32-bit: type
               BCC32 -W -Iinclude vdemow.cpp lib\vcf3w.lib
          b) 16-bit: type
               BCC -ml -W -Iinclude vdemow.cpp lib\vcl3w.lib
      and run the program by typing:  vdemow

      or create a project containing
      VDEMOW.CPP and VCF3W.LIB.
      In the TargetExpert, choose as target
        Application [.EXE]  for Win32, GUI model.
        Tick the boxes for Standard Libraries ClassLibrary, and Runtime.
        Use Static linking and Single-Thread

  For Windows95, 98 or NT with Borland C++ 4.x and OWL 2.0
      create a project containing
      VDEMOW2.CPP and VCF3W.LIB.
      In the TargetExpert, choose as target
        Application [.EXE]  for Win32, GUI model.
        Tick the boxes for Standard Libraries OWL, ClassLibrary, and Runtime.
        Use Static linking and Single-Thread (which means: don't check the
        Multithread box).

  For 16-bit Windows with Borland C++ 4.x, 5.x or higher (without OWL)
      create a project containing
      VDEMOW.CPP and VCL3W.LIB.
      In the TargetExpert, choose as target
        Application [.EXE]  for Windows 3.x, model LARGE.
        Tick the boxes for Standard Libraries ClassLibrary, and Runtime.
        Use, as you prefer, either Dynamic or Static linking.

  For DOS, create a project containing
     VDEMO.CPP,  VCL3.LIB, and  GRAPHICS.LIB.
     Choose the application either as DOS Standard or as DOS Overlay.
     VCL3.LIB may or may not be used as an overlay (option to be set:
     "Project/Local Options/Overlay this module").
     Choose the memory  model LARGE.
     Check the floating-point options. Neither "No floating-point" nor
     "Floating-point emulation" should be selected.

  For 16-bit Windows with Borland C++ 3.x and OWL 1.0
      create a project containing
      VDEMOW1.CPP,  VCL3W.LIB, and OWL.DEF.
      Choose the application as Windows-EXE, model LARGE.
      Choose the memory model LARGE and 287 floating-point operations

  For 16-bit Windows with Borland C++ 4.x and OWL 2.0
      create a project containing
      VDEMOW2.CPP and VCL3W.LIB.
      In the TargetExpert, choose as target
        Application [.EXE]  for Windows 3.x, model LARGE.
        Tick the boxes for Standard Libraries OWL, ClassLibrary, and Runtime.
        Use, as you prefer, either Dynamic or Static linking.


OptiVec for Microsoft Visual C++:
    create a project as a "Win32 application", containing
    VDEMOW.CPP  and  OVVCSD.LIB.
    Choose single-thread debug.

Data-fitting functions demo:
    There is a demo program available for the data-fitting functions,
    FITDEMO.CPP (for Borland C++, DOS), or FITDEMOW.CPP for Windows.
    The latter file can be used with Borland C++ (4.x or higher) or Visual C++,
    under Windows 3.1 or Windows 95/98/NT.
    With BC++ Builder, you can alternatively open the project FITDEMOB.BPR.

    Follow the instructions given above for VDEMOW.CPP and VDEMOB.BPR.
    With 16-bit BC++, you must additionally include the library MCL3.LIB
    (for DOS) or MCL3W.LIB (for Windows).


5. Documentation
----------------
The full OptiVec documentation is to be found in the accompanying
printed handbooks. Additionally, the HTML files HANDBOOK.HTM, FUNCREF.HTM,
MATRIX.HTM, and CMATH.HTM allow for quick online reference using a
browser like Netscape or Microsoft IE.


6. What's New?
--------------
Version 3.2.2:
a) Function MFsym_sqrt
b) Bug fixes in the P6 version of VFu_sqrt, VF_cmp_inclrange0C, ...CC,
   VF_cmp_exclrange0C, ...CC, VCDx_divV, and V?_derivC
c) Faster LU decomposition
d) All nonlinfit functions check if the "best guess" A values lie within
   the specified limits before chi-square is calculated for the first time.
   Thereby, possible failure due to input A values outside the limits is
   avoided.

Version 3.2.1:
e) Functions VF_mulVconj (multiply X by the complex conjugate of Y),
   MCF_mulMH (multiply matrix X by the hermitian conjugate of matrix Y),
   MCF_HmulM (multiply the hermitian conjugate of matrix X by matrix Y)
f) Bug fixes, affecting MF_deconvolve, MF_autocorr, MF_xcorr, MF_spectrum,
   and VF_polyfitwW

Version 3.2:
g) additional Pentium III/IV version, about 20-40% faster for many functions.
h) In the functions for data-type down-conversion (V_DtoF etc.),
   OVERFLOW errors do no longer lead to an error message, but are
   silently treated by setting the result to the maximum value possible
   (with the correct sign).
i) Extra-fast square-root function without error handling: VFu_sqrt 

Version 3.1 introduced Complex matrix decomposition, inversion,
linear systems solution


7. Copyright
------------
The copyright owner of this product as a whole and of all its constituent
parts is
         OptiCode
         Dr. Martin Sander Software Development
         Steinachstr. 9A
         D-69198 Schriesheim
         Germany
         e-mail: MartinSander@optivec.com
         http://www.optivec.com

THIS VERSION OF OPTIVEC IS NOT TO BE DISTRIBUTED AS SHAREWARE!


    * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

Copyright (C) OptiCode - Dr. Martin Sander Software Dev. 1996-2002
