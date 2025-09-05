/* uiVecObj.H

  object-oriented interface for OptiVec vector functions
  of type "ui"

  This file was written by Brian Dale, Case Western Reserve Univ.

  Copyright (c) 1996-2002 by OptiCode
  All Rights Reserved.
*/

#ifndef UIVECOBJ_H

#include <VecObj.h>

#if defined( V_HUGE )
   #include <ulVecObj.h>
#else
   #include <uVecObj.h>
#endif //V_HUGE

#define UIVECOBJ_H
#endif //UIVECOBJ_H
