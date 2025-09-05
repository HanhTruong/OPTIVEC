/***********************   VDEMOB.CPP  ***********************************
*                                                                        *
*                Demo program for                                        *
*                  O p t i V e c                                         *
*          with Borland C++ Builder,                                     *
*                                                                        *
*  Copyright 1996-2002 by OptiCode - Dr. Martin Sander Software Dev.     *
*                                                                        *
*                                                                        *
*       This sample program is meant to demonstrate how to use           *
*       OptiVec within your Windows programs. It is not intended         *
*       to demonstrate best Windows programming.                         *
*                                                                        *
**************************************************************************/
/*
Borland C++ Builder IDE:
    1. Open the project VDEMOB.BPR.
    2. In the menu Project/Options/Directories,
       add your OptiVec directories to the
       include and library paths.
    3. In Project/Options/Linker, be sure that
       "Dynamic RTL" is  n o t  checked.
    4. In Project/Add_to_Project, add VCF3W.LIB.
       (In order to avoid errors when you open the
       project, this library is not yet included in
       the project - we could not know in which
       directory you would put it!)
    5. Compile and run.
*/

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USELIB("lib\vcf3w.lib");
USERES("fitdemob.res");
USEFORM("fitdemof.cpp", Form1);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
