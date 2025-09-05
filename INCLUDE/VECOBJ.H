/* VecObj.H

  Basic definitions of the object-oriented interface for OptiVec.

  This file was written by Brian Dale, Case Western Reserve Univ.

  Copyright (c) 1996-2002 by OptiCode
  All Rights Reserved.
*/

#ifndef VECOBJ_H
#define VECOBJ_H

#include <VecAll.h>

#define cvta const vector<T>&
#define cuia const ui&
#define cta  const T&

#define vta  vector<T>&
#define uia  ui&
#define ta   T&

#define vt   vector<T>
#define t    T
#if defined _MSC_VER || (defined __BORLANDC__ && __BORLANDC__ >= 0x540)
            // new syntax for explicit specialization of templates
    #define VECOBJ_NEW_TEMPLATE template<>
#else
    #define VECOBJ_NEW_TEMPLATE
#endif

#ifdef _MSC_VER
namespace OptiVec{
#endif

	template<class T> class vector;

	VECOBJ_NEW_TEMPLATE class vector<char>;
	VECOBJ_NEW_TEMPLATE class vector<short>;
	VECOBJ_NEW_TEMPLATE class vector<int>;
	VECOBJ_NEW_TEMPLATE class vector<long>;
	VECOBJ_NEW_TEMPLATE class vector<quad>;
	VECOBJ_NEW_TEMPLATE class vector<unsigned char>;
	VECOBJ_NEW_TEMPLATE class vector<unsigned short>;
	VECOBJ_NEW_TEMPLATE class vector<unsigned int>;
	VECOBJ_NEW_TEMPLATE class vector<unsigned long>;
	VECOBJ_NEW_TEMPLATE class vector<ui>;
	VECOBJ_NEW_TEMPLATE class vector<float>;
	VECOBJ_NEW_TEMPLATE class vector<double>;
	VECOBJ_NEW_TEMPLATE class vector<extended>;
	VECOBJ_NEW_TEMPLATE class vector<fComplex>;
	VECOBJ_NEW_TEMPLATE class vector<dComplex>;
	VECOBJ_NEW_TEMPLATE class vector<eComplex>;
	VECOBJ_NEW_TEMPLATE class vector<fPolar>;
	VECOBJ_NEW_TEMPLATE class vector<dPolar>;
	VECOBJ_NEW_TEMPLATE class vector<ePolar>;

	typedef vector<char>           biVecObj;
	typedef vector<short>          siVecObj;
	typedef vector<int>             iVecObj;
	typedef vector<long>           liVecObj;
	typedef vector<quad>           qiVecObj;
	typedef vector<unsigned char>  ubVecObj;
	typedef vector<unsigned short> usVecObj;
	typedef vector<unsigned int>    uVecObj;
	typedef vector<unsigned long>  ulVecObj;
	typedef vector<ui>             uiVecObj;
	typedef vector<float>           fVecObj;
	typedef vector<double>          dVecObj;
	typedef vector<extended>        eVecObj;
	typedef vector<fComplex>       cfVecObj;
	typedef vector<dComplex>       cdVecObj;
	typedef vector<eComplex>       ceVecObj;
	typedef vector<fPolar>         pfVecObj;
	typedef vector<dPolar>         pdVecObj;
	typedef vector<ePolar>         peVecObj;

#if !defined _MSC_VER && defined __BORLANDC__ && __BORLANDC__ >= 0x500
namespace OptiVec {   // at least the error classes must be in namespace
#endif
#if !defined __BORLANDC__ || __BORLANDC__ >= 0x500
	class Err {};

	class SizeErr : public Err {};
	class BigSizeErr : public SizeErr {};
	class SmallSizeErr : public SizeErr {};
	class EmptySizeErr : public SizeErr {};
	class Pow2SizeErr : public SizeErr {};

	class FixedErr : public Err {};
	class Div0FixedErr : public FixedErr {};
	class OverflowFixedErr : public FixedErr {};

	class FloatingErr : public Err {};
	class DomainFloatingErr : public FloatingErr {};
	class SingFloatingErr : public FloatingErr {};
	class OverflowFloatingErr : public FloatingErr {};
	class TlossFloatingErr : public FloatingErr {};

    #define OptiVec_Err          OptiVec::Err
    #define OptiVec_SizeErr      OptiVec::SizeErr
    #define OptiVec_BigSizeErr   OptiVec::BigSizeErr
    #define OptiVec_SmallSizeErr OptiVec::SmallSizeErr
    #define OptiVec_EmptySizeErr OptiVec::EmptySizeErr
    #define OptiVec_Pow2SizeErr  OptiVec::Pow2SizeErr
    #define OptiVec_FixedErr     OptiVec::FixedErr
    #define OptiVec_Div0FixedErr OptiVec::Div0FixedErr
    #define OptiVec_OverflowFixedErr     OptiVec::OverflowFixedErr
    #define OptiVec_FloatingErr          OptiVec::FloatingErr
    #define OptiVec_DomainFloatingErr    OptiVec::DomainFloatingErr
    #define OptiVec_SingFloatingErr      OptiVec::SingFloatingErr
    #define OptiVec_OverflowFloatingErr  OptiVec::OverflowFloatingErr
    #define OptiVec_TlossFloatingErr     OptiVec::TlossFloatingErr
}  // end of namespace OptiVec

#else  // old BC++ versions cannot handle namespaces. Use prefix OptiVec_ instead
	class OptiVec_Err {};

	class OptiVec_SizeErr : public OptiVec_Err {};
	class OptiVec_BigSizeErr : public OptiVec_SizeErr {};
	class OptiVec_SmallSizeErr : public OptiVec_SizeErr {};
	class OptiVec_EmptySizeErr : public OptiVec_SizeErr {};
	class OptiVec_Pow2SizeErr : public OptiVec_SizeErr {};

	class OptiVec_FixedErr : public OptiVec_Err {};
	class OptiVec_Div0FixedErr : public OptiVec_FixedErr {};
	class OptiVec_OverflowFixedErr : public OptiVec_FixedErr {};

	class OptiVec_FloatingErr : public OptiVec_Err {};
	class OptiVec_DomainFloatingErr : public OptiVec_FloatingErr {};
	class OptiVec_SingFloatingErr : public OptiVec_FloatingErr {};
	class OptiVec_OverflowFloatingErr : public OptiVec_FloatingErr {};
	class OptiVec_TlossFloatingErr : public OptiVec_FloatingErr {};
#endif

#undef cvta
#undef cuia
#undef cta

#undef vta
#undef uia
#undef ta

#undef vt
#undef t


#endif //VECOBJ_H
