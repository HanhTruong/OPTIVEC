/* eVecObj.H

  object-oriented interface for OptiVec vector functions of type "extended"

  This file was written by Brian Dale, Case Western Reserve Univ.

  Copyright (c) 1996-2002 by OptiCode
  All Rights Reserved.
*/

#ifndef EVECOBJ_H

#include <VecObj.h>
#include <uiVecObj.h>

#ifdef __BORLANDC__ //80-bit numbers supported

#define cvta const vector<extended>&
#define cuia const ui&
#define cta  const extended&

#define vta  vector<extended>&
#define uia  ui&
#define ta   extended&

#define vt   vector<extended>
#define t    extended
#define tVector eVector

#define vtcplx vector<eComplex>
#define tcplx  eComplex

#define tVecObj eVecObj
#ifdef __BORLANDC__
#pragma warn -inl
#endif


#ifdef _MSC_VER
namespace OptiVec{
#endif

	VECOBJ_NEW_TEMPLATE class vector<t>
	{
#ifdef _MSC_VER 
     public:
		tVector vx_vect;
		ui      ui_size;
#else
		tVector vx_vect;
		ui      ui_size;
		friend class vector<char>;
		friend class vector<short>;
		friend class vector<int>;
		friend class vector<long>;
		friend class vector<quad>;
		friend class vector<unsigned char>;
		friend class vector<unsigned short>;
		friend class vector<unsigned int>;
		friend class vector<unsigned long>;
		friend class vector<ui>;
		friend class vector<float>;
		friend class vector<double>;
		friend class vector<extended>;
		friend class vector<fComplex>;
		friend class vector<dComplex>;
		friend class vector<eComplex>;
		friend class vector<fPolar>;
		friend class vector<dPolar>;
		friend class vector<ePolar>;
#endif
	public:
////////////////////////////////////////////////////////////////////////////////////////////////////
//constructors and destructors
//
		vector(){	vx_vect=0;	ui_size=0;}

		vector(cuia size)
		{	if(size){	vx_vect= VE_vector(size);	ui_size= size;}
			else{	vx_vect= 0;	ui_size= 0;}}

		vector(cuia size, cta fill)
		{	if(!size) //size == 0
			{	vx_vect=0;	ui_size=0;}
			else if(!fill) //fill == 0.0
			{	vx_vect= VE_vector0(size);	ui_size= size;}
			else
			{	vx_vect= VE_vector(size);	ui_size= size;	VE_equC(vx_vect,ui_size,fill);}}

		vector(cvta init)
		{	if(init.ui_size)
			{ vx_vect= VE_vector(init.ui_size);	ui_size= init.ui_size;	VE_equV(vx_vect, init.vx_vect, ui_size);}
			else
			{	vx_vect=0;	ui_size=0;}}

		~vector(){	if(vx_vect) V_free(vx_vect);}


////////////////////////////////////////////////////////////////////////////////////////////////////
//management
//
		void enforceSize()const{ if(!ui_size) throw OptiVec_EmptySizeErr();}

		void enforceSize(ui size)const{ if(ui_size != size) throw OptiVec_SizeErr();}

		void enforcePow2()const{ if(!isipow2(ui_size)) throw OptiVec_Pow2SizeErr();}

		void setSize(cuia size)
		{	if(!size) free(); //can set size to zero
			if(ui_size != size) //only allocate if different sizes
			{	if(vx_vect) V_free(vx_vect);
				vx_vect= VE_vector(size);
				ui_size= size;}}

		void setSize(cvta X)
		{	if(!X.ui_size) free(); //can set size to zero
			if(ui_size != X.ui_size) //only allocate if different sizes
			{	if(vx_vect) V_free(vx_vect);
				vx_vect= VE_vector(X.ui_size);
				ui_size= X.ui_size;}}

		ui getSize() { return ui_size; }

		void free()
		{	if(vx_vect) V_free(vx_vect);
			vx_vect=0;
			ui_size=0;}

		ta at(ui n)
		{	enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return vx_vect[n];}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Overloaded operators
//
		vta operator=(cvta vect)
		{	setSize(vect.ui_size);
			enforceSize();
			VE_equV(vx_vect, vect.vx_vect, ui_size);
			return *this;}

		vta operator=(cta scalar)
		{	enforceSize();
			VE_equC(vx_vect, ui_size, scalar);
			return *this;}

		//this is the only really dangerous operator
		ta operator[](const int& index){return vx_vect[index];}

		vta operator+=(cvta X)
		{	enforceSize();
			VE_accV(vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator+=(cta scalar)
		{	enforceSize();
			VE_addC(vx_vect, vx_vect, ui_size, scalar);
			return *this;}

		vta operator*=(cvta X)
		{	enforceSize();
			VE_mulV(vx_vect, vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator*=(cta scalar)
		{	enforceSize();
			VE_mulC(vx_vect,vx_vect,ui_size,scalar);
			return *this;}

		vt operator+(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VE_addV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator+(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VE_addC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator+(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VE_addC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator-(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VE_subV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator-(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VE_subC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator-(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VE_subrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator*(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VE_mulV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator*(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VE_mulC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator*(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VE_mulC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator/(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VE_divV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator/(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VE_divC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator/(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VE_divrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//OptiVec functions in member format
//

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//from VXstd.h
//

/////////////////////////////////////////////////////////////////////////////////////////
//Generation
//
		//these are not recommended, better to use constructors
		void _vector(ui size) //underscore necessary to avoid confusing with constructor
		{
			free();
			if(size) vx_vect= VE_vector(size);
			ui_size=size;
		}

		void vector0(ui size)
		{
			free();
			if(size) vx_vect= VE_vector0(size);
			ui_size=size;
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Addressing single vector elements
//
		t* Pelement(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VE_Pelement(vx_vect,n);
			//not in funcref
		}

		t element(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VE_element(vx_vect,n);
			//not in funcref
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Initialization
//

		void equ0()
		{
			enforceSize();
			VE_equ0(vx_vect,ui_size);
		}

		void equ1()
		{
			enforceSize();
			VE_equ1(vx_vect,ui_size);
		}

		void equm1()
		{
			enforceSize();
			VE_equm1(vx_vect, ui_size);
		}

		void equC(cta C)
		{
			enforceSize();
			VE_equC(vx_vect, ui_size, C);
		}

		void equV(cvta X)
		{
			enforceSize();
			VE_equV(vx_vect, X.vx_vect, ui_size);
		}

		void x_equV(cvta X, cta A, cta B)
		{
			enforceSize();
			VEx_equV(vx_vect, X.vx_vect, ui_size, A, B);
		}

		void ramp(cta Start, cta Rise)
		{
			enforceSize();
			VE_ramp(vx_vect, ui_size, Start, Rise);
		}

		void Parzen()
		{
			enforceSize();
			VE_Parzen(vx_vect, ui_size);
		}

		void Welch()
		{
			enforceSize();
			VE_Welch(vx_vect, ui_size);
		}

		void Hanning()
		{
			enforceSize();
			VE_Hanning(vx_vect, ui_size);
		}

		void comb(cuia spac, cta C)
		{
			enforceSize();
			if(spac>ui_size) throw OptiVec_SmallSizeErr();
			VE_comb(vx_vect, ui_size, spac, C);
		}

		long random(const long& seed, cta Min, cta Max)
		{
			enforceSize();
			return VE_random(vx_vect, ui_size, seed, Min, Max);
		}

		long noise(const long& seed, cta Amp)
		{
			enforceSize();
			return VE_noise(vx_vect, ui_size, seed, Amp);
		}

		//defined by including ceVecObj.h
		void CtoReIm(vt* Im, const vtcplx& X);
                void CtoReIm(vt  Im, const vtcplx& X);

		//defined by including ceVecObj.h
		void CtoRe(const vtcplx& cplx);

		//defined by including ceVecObj.h
		void CtoIm(const vtcplx& cplx);

		//defined by including ceVecObj.h
		void CtoPolar(vt* Arg, const vtcplx& X);
                void CtoPolar(vt  Arg, const vtcplx& X);

		//defined by including ceVecObj.h
		void CtoNorm(const vtcplx& cplx);

		//defined by including ceVecObj.h
		void CtoAbs(const vtcplx& cplx);

		//defined by including ceVecObj.h
		void CtoArg(const vtcplx& cplx);

		//defined by including ceVecObj.h
		int abs(const vtcplx& cplx);

		//defined by including ceVecObj.h

		void real(const vtcplx& cplx){	CtoRe(cplx);}

		void imag(const vtcplx& cplx){	CtoIm(cplx);}

		void arg(const vtcplx& cplx){	CtoArg(cplx);}

		void norm(const vtcplx& cplx){	CtoNorm(cplx);}

		//defined by including peVecObj.h
		void PtoMagArg(vt* Arg, const peVecObj& X);
                void PtoMagArg(vt  Arg, const peVecObj& X);

		//defined by including peVecObj.h
		void PtoMag(const peVecObj& X);

		//defined by including peVecObj.h
		void PtoArg(const peVecObj& X);

		//defined by including peVecObj.h
		void PtoNorm(const peVecObj& X);

		//defined by including peVecObj.h
		void PtoReIm(vt* Im, const peVecObj& X);
                void PtoReIm(vt  Im, const peVecObj& X);

		//defined by including peVecObj.h
		void PtoRe(const peVecObj& X);

		//defined by including peVecObj.h
		void PtoIm(const peVecObj& X);

		//defined by including peVecObj.h
		int abs(const peVecObj& X);

		void real(const peVecObj& cplx){ PtoRe(cplx);}

		void imag(const peVecObj& cplx){ PtoIm(cplx);}

		void arg(const peVecObj& cplx){ PtoArg(cplx);}

		void norm(const peVecObj& cplx){ PtoNorm(cplx);}


/////////////////////////////////////////////////////////////////////////////////////////
//Data-type interconversions
//

		//defined by including siVecObj.h
		void BItoE(const biVecObj& X);

		//defined by including siVecObj.h
		void SItoE(const siVecObj& X);

		//defined by including iVecObj.h
		void ItoE(const iVecObj& X);

		//defined by including liVecObj.h
		void LItoE(const liVecObj& X);

		//defined by including qiVecObj.h
		void QItoE(const qiVecObj& X);

		//defined by including ubVecObj.h
		void UBtoE(const ubVecObj& X);

		//defined by including usVecObj.h
		void UStoE(const usVecObj& X);

		//defined by including uVecObj.h
		void UtoE(const uVecObj& X);

		//defined by including ulVecObj.h
		void ULtoE(const ulVecObj& X);

		//defined by including fVecObj.h
		void FtoE(const fVecObj& X);

		//defined by including dVecObj.h
		void DtoE(const dVecObj& X);


/////////////////////////////////////////////////////////////////////////////////////////
//Index-oriented manipulations
//
		void reflect()
		{
			enforceSize();
			VE_reflect(vx_vect, ui_size);
		}

		void rev(cvta X)
		{
			enforceSize();
			VE_rev(vx_vect, X.vx_vect, ui_size);
		}

#ifdef V_HUGE
		void rotate( cvta X, long pos )
#else
		void rotate( cvta X, int pos )
#endif
		{
			enforceSize();
			VE_rotate(vx_vect, X.vx_vect, ui_size, pos);
		}


		void _delete(ui pos)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VE_delete(vx_vect, ui_size, pos);
		}

		void insert(ui pos, cta C)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VE_insert(vx_vect, ui_size, pos, C);
		}

		void sort(cvta X, int dir=1)
		{
			enforceSize();
			VE_sort(vx_vect,X.vx_vect,ui_size,dir);
		}

		//sortind member of uiVecObj

		void subvector(cvta X, int step, ui start=0)
		{
			enforceSize();
			if(vx_vect==X.vx_vect) throw OptiVec_Err();
			VE_subvector(vx_vect, ui_size, X.vx_vect+start, step);
		}

		void indpick(const vector<ui>& Ind, cvta X);
		void indput(cvta X, const vector<ui>& Ind);

		ui searchC(cta C, int mode=0)
		{
			enforceSize();
			return VE_searchC(vx_vect, ui_size, C, mode);
		}

		//searchV member of uiVecObj

		void polyinterpol(cvta X, cvta Xtab, cvta Ytab, unsigned deg)
		{
			enforceSize();
			Xtab.enforceSize();
			if(deg>10) throw OptiVec_Err();
			if(Xtab.ui_size<deg) throw OptiVec_SmallSizeErr();
			VE_polyinterpol(vx_vect,X.vx_vect,ui_size,
				Xtab.vx_vect,Ytab.vx_vect,Xtab.ui_size,deg);
		}

		void ratinterpol(cvta X, cvta Xtab, cvta Ytab, unsigned deg)
		{
			enforceSize();
			Xtab.enforceSize();
			if((deg>20)||(deg<3)) throw OptiVec_Err();
			if(Xtab.ui_size<deg) throw OptiVec_SmallSizeErr();
			VE_ratinterpol(vx_vect,X.vx_vect,ui_size,
				Xtab.vx_vect,Ytab.vx_vect,Xtab.ui_size,deg);
		}

		void splinederiv2(cvta Xtab, cvta Ytab, int specify, cta Yp0, cta Ypn)
		{
			enforceSize();
			VE_splinederiv2(vx_vect,Xtab.vx_vect,Ytab.vx_vect,Xtab.ui_size,
				specify,Yp0,Ypn);
		}

		void splineinterpol(cvta X, cvta Xtab, cvta Ytab, cvta Y2tab)
		{
			enforceSize();
			Xtab.enforceSize();
			VE_splineinterpol(vx_vect,X.vx_vect,ui_size,
				Xtab.vx_vect,Ytab.vx_vect,Y2tab.vx_vect,Xtab.ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Functions of a sub-set of elements
//
		//subvector_equC defined below
		//subvector_equV defined below


/////////////////////////////////////////////////////////////////////////////////////////
//One-Dimensional Vector Operations
//
		t _max()
		{
			enforceSize();
			return VE_max(vx_vect, ui_size);
		}

		t _min()
		{
			enforceSize();
			return VE_min(vx_vect, ui_size);
		}


		t absmax()
		{
			enforceSize();
			return VE_absmax(vx_vect, ui_size);
		}

		t absmin()
		{
			enforceSize();
			return VE_absmin(vx_vect, ui_size);
		}

		int maxexp()
		{
			enforceSize();
			return VE_maxexp(vx_vect, ui_size);
			//not in funcref
		}

		int minexp()
		{
			enforceSize();
			return VE_minexp(vx_vect, ui_size);
			//not in funcref
		}

		t maxind(ui* Ind)
		{
			enforceSize();
			return VE_maxind(Ind, vx_vect, ui_size);
		}

		t minind(ui* Ind)
		{
			enforceSize();
			return VE_minind(Ind, vx_vect, ui_size);
		}

		t absmaxind(ui* Ind)
		{
			enforceSize();
			return VE_absmaxind(Ind, vx_vect, ui_size);
		}

		t absminind(ui* Ind)
		{
			enforceSize();
			return VE_absminind(Ind, vx_vect, ui_size);
		}

		//localmaxima member of uiVecObj
		//localminima member of uiVecObj

		void runmax(cvta X)
		{
			enforceSize();
			VE_runmax(vx_vect, X.vx_vect, ui_size);
		}

		void runmin(cvta X)
		{
			enforceSize();
			VE_runmin(vx_vect, X.vx_vect, ui_size);
		}

		t sum()
		{
			enforceSize();
			return VE_sum(vx_vect, ui_size);
		}

		t prod()
		{
			enforceSize();
			return VE_prod(vx_vect, ui_size);
		}

		void runsum(cvta X)
		{
			enforceSize();
			VE_runsum(vx_vect, X.vx_vect, ui_size);
		}

		void runprod(cvta X)
		{
			enforceSize();
			VE_runprod(vx_vect, X.vx_vect, ui_size);
		}

		int iselementC(cta C)
		{
			enforceSize();
			return VE_iselementC(vx_vect, ui_size, C);
		}

		ui iselementV(cvta X, cvta Tab)
		{
			enforceSize();
			return VE_iselementV(vx_vect, X.vx_vect,
				ui_size, Tab.vx_vect, Tab.ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Statistical Functions and Building Blocks
//
		t sumabs() const
		{
			enforceSize();
			return VE_sumabs(vx_vect,ui_size);
		}

		t mean() const
		{
			enforceSize();
			return VE_mean(vx_vect,ui_size);
		}

		t meanwW(cvta Wt) const
		{
			enforceSize();
			return VE_meanwW(vx_vect, Wt.vx_vect, ui_size);
		}

		t meanabs() const
		{
			enforceSize();
			return VE_meanabs(vx_vect,ui_size);
		}

		t selected_mean(ui* nsel, cta Xmin, cta Xmax) const
		{
			enforceSize();
			return VE_selected_mean(nsel,vx_vect,ui_size,Xmin,Xmax);
		}

		t median() const
		{
			enforceSize();
			return VE_median(vx_vect,ui_size);
		}

		t sumdevC(cta C) const
		{
			enforceSize();
			return VE_sumdevC(vx_vect,ui_size,C);
		}

		t sumdevV(cvta Y) const
		{
			enforceSize();
			return VE_sumdevV(vx_vect,Y.vx_vect,ui_size);
		}

		t avdevC(cta C) const
		{
			enforceSize();
			return VE_avdevC(vx_vect,ui_size,C);
		}

		t avdevV(cvta Y) const
		{
			enforceSize();
			return VE_avdevV(vx_vect,Y.vx_vect,ui_size);
		}

		t ssq() const
		{
			enforceSize();
			return VE_ssq(vx_vect,ui_size);
		}

		t rms() const
		{
			enforceSize();
			return VE_rms(vx_vect,ui_size);
		}

		t ssqdevC(cta C) const
		{
			enforceSize();
			return VE_ssqdevC(vx_vect,ui_size,C);
		}

		t ssqdevV(cvta Y) const
		{
			enforceSize();
			return VE_ssqdevV(vx_vect,Y.vx_vect,ui_size);
		}

		t meanvar(t* Var) const
		{
			enforceSize();
			return VE_meanvar(Var,vx_vect,ui_size);
		}

		t meanvarwW(t* Var, cvta Wt) const
		{
			enforceSize();
			return VE_meanvarwW(Var,vx_vect,Wt.vx_vect,ui_size);
		}

		t varianceC(cta C) const
		{
			enforceSize();
			return VE_varianceC(vx_vect,ui_size,C);
		}

		t varianceV(cvta Y) const
		{
			enforceSize();
			return VE_varianceV(vx_vect,Y.vx_vect,ui_size);
		}

		t varianceCwW(cvta Wt,cta C) const
		{
			enforceSize();
			return VE_varianceCwW(vx_vect,Wt.vx_vect,ui_size,C);
		}

		t varianceVwW(cvta Y, cvta Wt) const
		{
			enforceSize();
			return VE_varianceVwW(vx_vect,Y.vx_vect,Wt.vx_vect,ui_size);
		}

		t chi2(cvta Y, cvta InvVar) const
		{
			enforceSize();
			return VE_chi2(vx_vect,Y.vx_vect,InvVar.vx_vect,ui_size);
		}

		t chiabs(cvta Y, cvta Wt) const
		{
			enforceSize();
			return VE_chiabs(vx_vect,Y.vx_vect,Wt.vx_vect,ui_size);
		}

		t corrcoeff(cvta Y, cta Xmean, cta Ymean) const
		{
			enforceSize();
			return VE_corrcoeff(vx_vect,Y.vx_vect,ui_size,Xmean,Ymean);
		}

		//overloaded for convenience
		t corrcoeff(cvta Y) const
		{
			enforceSize();
			return VE_corrcoeff(vx_vect,Y.vx_vect,ui_size,mean(),Y.mean());
		}

		//distribution member of uiVecObj

		void linregress(cvta X, cvta Y)
		{
			X.enforceSize();
			if(ui_size<5) throw OptiVec_SmallSizeErr();
			VE_linregress(vx_vect,X.vx_vect,Y.vx_vect,X.ui_size);
		}

		void linregresswW(cvta X, cvta Y, cvta InvVar)
		{
			X.enforceSize();
			if(ui_size<5) throw OptiVec_SmallSizeErr();
			VE_linregresswW(vx_vect,X.vx_vect,Y.vx_vect,
				InvVar.vx_vect,X.ui_size);
		}





/////////////////////////////////////////////////////////////////////////////////////////
//Fourier Transforms, Convolutions, Filtering
//

		tcplx getRspEdit()
		{
			return VE_getRspEdit();
		}

		void setRspEdit(const tcplx& trunc)
		{
			VE_setRspEdit(trunc);
		}

		void l_FFT(cvta X, int dir=1)
		{
			enforcePow2();
			VEl_FFT(vx_vect,X.vx_vect,ui_size,dir);
		}

		//l_FFTtoC member of ceVecObj

		void l_filter(cvta X, cvta Flt)
		{
			enforcePow2();
			VEl_filter(vx_vect, X.vx_vect, Flt.vx_vect, ui_size);
		}

		void l_autocorr(cvta X)
		{
			enforcePow2();
			VEl_autocorr(vx_vect, X.vx_vect, ui_size);
		}

		void l_xcorr(cvta X, cvta Y)
		{
			enforcePow2();
			VEl_xcorr(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		t l_spectrum(cvta X, cvta Win)
		{
			enforcePow2();
			if(2*ui_size > X.ui_size) throw OptiVec_BigSizeErr();
			if(2*ui_size != Win.ui_size) throw OptiVec_SizeErr();
			return VEl_spectrum(vx_vect, ui_size,
				X.vx_vect, X.ui_size, Win.vx_vect);
		}

		void l_convolve(tVecObj* Flt, cvta X, cvta Rsp)
		{
			enforcePow2();
			if((ui_size!=Flt->ui_size)||(ui_size!=X.ui_size)||
				(ui_size!=Rsp.ui_size)) throw OptiVec_SizeErr();
			if((vx_vect==Rsp.vx_vect)||(Flt->vx_vect==X.vx_vect))
				throw OptiVec_Err();
			VEl_convolve(vx_vect, Flt->vx_vect, X.vx_vect,
				Rsp.vx_vect, ui_size);
		}

		void l_deconvolve(tVecObj* Flt, cvta X, cvta Rsp)
		{
			enforcePow2();
			if((ui_size!=Flt->ui_size)||(ui_size!=X.ui_size)||
				(ui_size!=Rsp.ui_size)) throw OptiVec_SizeErr();
			if((vx_vect==Rsp.vx_vect)||(Flt->vx_vect==X.vx_vect))
				throw OptiVec_Err();
			VEl_deconvolve(vx_vect, Flt->vx_vect, X.vx_vect,
				Rsp.vx_vect, ui_size);
		}

                void l_convolve(tVecObj Flt, cvta X, cvta Rsp)
		{
			enforcePow2();
                        if((ui_size!=Flt.ui_size)||(ui_size!=X.ui_size)||
				(ui_size!=Rsp.ui_size)) throw OptiVec_SizeErr();
                        if((vx_vect==Rsp.vx_vect)||(Flt.vx_vect==X.vx_vect))
				throw OptiVec_Err();
                        VEl_convolve(vx_vect, Flt.vx_vect, X.vx_vect,
				Rsp.vx_vect, ui_size);
		}

                void l_deconvolve(tVecObj Flt, cvta X, cvta Rsp)
		{
			enforcePow2();
                        if((ui_size!=Flt.ui_size)||(ui_size!=X.ui_size)||
				(ui_size!=Rsp.ui_size)) throw OptiVec_SizeErr();
                        if((vx_vect==Rsp.vx_vect)||(Flt.vx_vect==X.vx_vect))
				throw OptiVec_Err();
                        VEl_deconvolve(vx_vect, Flt.vx_vect, X.vx_vect,
				Rsp.vx_vect, ui_size);
		}

		//s_FFTtoC member of ceVecObj

		void s_FFT(cvta X, int dir=1)
		{
			enforcePow2();
			VEs_FFT(vx_vect,X.vx_vect,ui_size,dir);
		}

		void s_filter(cvta X, cvta Flt)
		{
			enforcePow2();
			VEs_filter(vx_vect, X.vx_vect, Flt.vx_vect, ui_size);
		}

		void s_autocorr(cvta X)
		{
			enforcePow2();
			VEs_autocorr(vx_vect, X.vx_vect, ui_size);
		}

		void s_xcorr(cvta X, cvta Y)
		{
			enforcePow2();
			VEs_xcorr(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		t s_spectrum(cvta X, cvta Win)
		{
			enforcePow2();
			if(2*ui_size > X.ui_size) throw OptiVec_BigSizeErr();
			if(2*ui_size != Win.ui_size) throw OptiVec_SizeErr();
			return VEs_spectrum(vx_vect, ui_size,
				X.vx_vect, X.ui_size, Win.vx_vect);
		}

		void s_convolve(tVecObj* Flt, cvta X, cvta Rsp)
		{
			enforcePow2();
			if((ui_size!=Flt->ui_size)||(ui_size!=X.ui_size)||
				(ui_size!=Rsp.ui_size)) throw OptiVec_SizeErr();
			if((vx_vect==Rsp.vx_vect)||(Flt->vx_vect==X.vx_vect))
				throw OptiVec_Err();
			VEs_convolve(vx_vect, Flt->vx_vect, X.vx_vect,
				Rsp.vx_vect, ui_size);
		}

		void s_deconvolve(tVecObj* Flt, cvta X, cvta Rsp)
		{
			enforcePow2();
			if((ui_size!=Flt->ui_size)||(ui_size!=X.ui_size)||
				(ui_size!=Rsp.ui_size)) throw OptiVec_SizeErr();
			if((vx_vect==Rsp.vx_vect)||(Flt->vx_vect==X.vx_vect))
				throw OptiVec_Err();
			VEs_deconvolve(vx_vect, Flt->vx_vect, X.vx_vect,
				Rsp.vx_vect, ui_size);
		}

                void s_convolve(tVecObj Flt, cvta X, cvta Rsp)
		{
			enforcePow2();
                        if((ui_size!=Flt.ui_size)||(ui_size!=X.ui_size)||
				(ui_size!=Rsp.ui_size)) throw OptiVec_SizeErr();
                        if((vx_vect==Rsp.vx_vect)||(Flt.vx_vect==X.vx_vect))
				throw OptiVec_Err();
                        VEs_convolve(vx_vect, Flt.vx_vect, X.vx_vect,
				Rsp.vx_vect, ui_size);
		}

                void s_deconvolve(tVecObj Flt, cvta X, cvta Rsp)
		{
			enforcePow2();
                        if((ui_size!=Flt.ui_size)||(ui_size!=X.ui_size)||
				(ui_size!=Rsp.ui_size)) throw OptiVec_SizeErr();
                        if((vx_vect==Rsp.vx_vect)||(Flt.vx_vect==X.vx_vect))
				throw OptiVec_Err();
                        VEs_deconvolve(vx_vect, Flt.vx_vect, X.vx_vect,
				Rsp.vx_vect, ui_size);
		}

		void FFT(cvta X, int dir=1)
		{
			enforcePow2();
			VE_FFT(vx_vect,X.vx_vect,ui_size,dir);
		}

		//FFTtoC member of ceVecObj

		void filter(cvta X, cvta Flt)
		{
			enforcePow2();
			VE_filter(vx_vect, X.vx_vect, Flt.vx_vect, ui_size);
		}

		void autocorr(cvta X)
		{
			enforcePow2();
			VE_autocorr(vx_vect, X.vx_vect, ui_size);
		}

		void xcorr(cvta X, cvta Y)
		{
			enforcePow2();
			VE_xcorr(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		t spectrum(cvta X, cvta Win)
		{
			enforcePow2();
			if(2*ui_size > X.ui_size) throw OptiVec_BigSizeErr();
			if(2*ui_size != Win.ui_size) throw OptiVec_SizeErr();
			return VE_spectrum(vx_vect, ui_size,
				X.vx_vect, X.ui_size, Win.vx_vect);
		}

		void convolve(tVecObj* Flt, cvta X, cvta Rsp)
		{
			enforcePow2();
			if((ui_size!=Flt->ui_size)||(ui_size!=X.ui_size)||
				(ui_size!=Rsp.ui_size)) throw OptiVec_SizeErr();
			if((vx_vect==Rsp.vx_vect)||(Flt->vx_vect==X.vx_vect))
				throw OptiVec_Err();
			VE_convolve(vx_vect, Flt->vx_vect, X.vx_vect,
				Rsp.vx_vect, ui_size);
		}

		void deconvolve(tVecObj* Flt, cvta X, cvta Rsp)
		{
			enforcePow2();
			if((ui_size!=Flt->ui_size)||(ui_size!=X.ui_size)||
				(ui_size!=Rsp.ui_size)) throw OptiVec_SizeErr();
			if((vx_vect==Rsp.vx_vect)||(Flt->vx_vect==X.vx_vect))
				throw OptiVec_Err();
			VE_deconvolve(vx_vect, Flt->vx_vect, X.vx_vect,
				Rsp.vx_vect, ui_size);
		}

                void convolve(tVecObj Flt, cvta X, cvta Rsp)
		{
			enforcePow2();
                        if((ui_size!=Flt.ui_size)||(ui_size!=X.ui_size)||
				(ui_size!=Rsp.ui_size)) throw OptiVec_SizeErr();
                        if((vx_vect==Rsp.vx_vect)||(Flt.vx_vect==X.vx_vect))
				throw OptiVec_Err();
                        VE_convolve(vx_vect, Flt.vx_vect, X.vx_vect,
				Rsp.vx_vect, ui_size);
		}

                void deconvolve(tVecObj Flt, cvta X, cvta Rsp)
		{
			enforcePow2();
                        if((ui_size!=Flt.ui_size)||(ui_size!=X.ui_size)||
				(ui_size!=Rsp.ui_size)) throw OptiVec_SizeErr();
                        if((vx_vect==Rsp.vx_vect)||(Flt.vx_vect==X.vx_vect))
				throw OptiVec_Err();
                        VE_deconvolve(vx_vect, Flt.vx_vect, X.vx_vect,
				Rsp.vx_vect, ui_size);
		}

/////////////////////////////////////////////////////////////////////////////////////////
//Analysis
//
		void derivC(cvta X, cta Deltat)
		{
			enforceSize();
			VE_derivC(vx_vect,X.vx_vect,ui_size,Deltat);
		}

		void derivV(cvta X, cvta Y)
		{
			enforceSize();
			VE_derivV(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
		}

		t integralC(cta DeltaT) const
		{
			enforceSize();
			return VE_integralC(vx_vect,ui_size,DeltaT);
		}

		void runintegralC(cvta X, cta DeltaT)
		{
			enforceSize();
			VE_runintegralC(vx_vect,X.vx_vect,ui_size,DeltaT);
		}

		t integralV(cvta Y) const
		{
			enforceSize();
			return VE_integralV(vx_vect,Y.vx_vect,ui_size);
		}

		void runintegralV(cvta X, cvta Y)
		{
			enforceSize();
			VE_runintegralV(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
		}

		void smooth(cvta X, const unsigned& deg)
		{
			enforceSize();
			VE_smooth(vx_vect,X.vx_vect,ui_size,deg);
		}

		int ismonoton() const
		{
			enforceSize();
			return VE_ismonoton(vx_vect,ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Geometrical Vector Arithmetics
//
		t scalprod(cvta Y) const
		{
			enforceSize();
			return VE_scalprod(vx_vect, Y.vx_vect, ui_size);
		}

		void xprod(cvta X, cvta Y)
		{
			enforceSize(3);
			VE_xprod(vx_vect,X.vx_vect,Y.vx_vect);
		}

		t Euclid() const
		{
			enforceSize();
			return VE_Euclid(vx_vect,ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Input and Output
//
		void fprint(FILE* stream, unsigned nperline, unsigned linewidth) const
		{
			enforceSize();
			VE_fprint(stream,vx_vect,ui_size,nperline,linewidth);
		}

#if !defined _Windows || defined __FLAT__ || defined _WIN32
		void cprint(unsigned nperline) const
		{
			enforceSize();
			VE_cprint(vx_vect,ui_size,nperline);
		}
#endif
		void print(unsigned nperline) const
		{
			fprint(stdout,nperline,80);
		}

		void write(FILE* stream) const
		{
			enforceSize();
			VE_write(stream,vx_vect,ui_size);
		}

		void read(FILE* stream)
		{
			enforceSize();
			VE_read(vx_vect,ui_size,stream);
		}

//nwrite
//nread

		void setWriteFormat(char* formatString) const
		{
			VE_setWriteFormat(formatString);
		}

		void setWriteSeparate(char* sepString) const
		{
			VE_setWriteSeparate(sepString);
		}

//setNWriteSeparate

		void store(FILE* stream) const
		{
			enforceSize();
			fwrite(&ui_size,sizeof(ui),1,stream);
			VE_store(stream,vx_vect,ui_size);
		}

		void recall(FILE* stream)
		{
			ui sz;
			fread(&sz,sizeof(ui),1,stream);
			setSize(sz);
			VE_recall(vx_vect,ui_size,stream);
		}

		void l_convolve(cvta Flt, cvta X, cvta Rsp, cta TruncRe, cta TruncIm)
		{
			enforceSize();
			VEl_convolve(vx_vect,Flt.vx_vect,X.vx_vect,Rsp.vx_vect,
				ui_size,TruncRe,TruncIm);
		}

		void l_deconvolve(cvta Flt, cvta X, cvta Rsp, cta TruncRe, cta TruncIm)
		{
			enforceSize();
			VEl_convolve(vx_vect,Flt.vx_vect,X.vx_vect,Rsp.vx_vect,
				ui_size,TruncRe,TruncIm);
		}

		void s_convolve(cvta Flt, cvta X, cvta Rsp, cta TruncRe, cta TruncIm)
		{
			enforceSize();
			VEs_convolve(vx_vect,Flt.vx_vect,X.vx_vect,Rsp.vx_vect,
				ui_size,TruncRe,TruncIm);
		}

		void s_deconvolve(cvta Flt, cvta X, cvta Rsp, cta TruncRe, cta TruncIm)
		{
			enforceSize();
			VEs_convolve(vx_vect,Flt.vx_vect,X.vx_vect,Rsp.vx_vect,
				ui_size,TruncRe,TruncIm);
		}

		void convolve(cvta Flt, cvta X, cvta Rsp, cta TruncRe, cta TruncIm)
		{
			enforceSize();
			VE_convolve(vx_vect,Flt.vx_vect,X.vx_vect,Rsp.vx_vect,
				ui_size,TruncRe,TruncIm);
		}

		void deconvolve(cvta Flt, cvta X, cvta Rsp, cta TruncRe, cta TruncIm)
		{
			enforceSize();
			VE_convolve(vx_vect,Flt.vx_vect,X.vx_vect,Rsp.vx_vect,
				ui_size,TruncRe,TruncIm);
		}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//from VXmath.h
//


/////////////////////////////////////////////////////////////////////////////////////////
//Rounding
//
		//other rounding functions are members of the appropriate 
		//class and are defined with the corresponding header file

		int round(cvta X)
		{
			enforceSize();
			return VE_round(vx_vect,X.vx_vect,ui_size);
		}

		int floor(cvta X)
		{
			enforceSize();
			return VE_floor(vx_vect,X.vx_vect,ui_size);
		}

		int ceil(cvta X)
		{
			enforceSize();
			return VE_ceil(vx_vect,X.vx_vect,ui_size);
		}

		int chop(cvta X)
		{
			enforceSize();
			return VE_chop(vx_vect,X.vx_vect,ui_size);
		}

		int trunc(cvta X)
		{
			return chop(X);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Comparisons
//
		void cmp0(cvta X)
		{
			enforceSize();
			VE_cmp0(vx_vect,X.vx_vect,ui_size);
		}

		ui cmp_eq0(cvta X)
		{
			enforceSize();
			return VE_cmp_eq0(vx_vect,X.vx_vect,ui_size);
		}

		ui cmp_ne0(cvta X)
		{
			enforceSize();
			return VE_cmp_ne0(vx_vect,X.vx_vect,ui_size);
		}

		ui cmp_le0(cvta X)
		{
			enforceSize();
			return VE_cmp_le0(vx_vect,X.vx_vect,ui_size);
		}

		ui cmp_lt0(cvta X)
		{
			enforceSize();
			return VE_cmp_lt0(vx_vect,X.vx_vect,ui_size);
		}

		ui cmp_ge0(cvta X)
		{
			enforceSize();
			return VE_cmp_ge0(vx_vect,X.vx_vect,ui_size);
		}

		ui cmp_gt0(cvta X)
		{
			enforceSize();
			return VE_cmp_gt0(vx_vect,X.vx_vect,ui_size);
		}

		//indexed comparisons are members of uiVecObj

		void cmpC(cvta X, cta C)
		{
			enforceSize();
			VE_cmpC(vx_vect,X.vx_vect,ui_size,C);
		}

		ui cmp_eqC(cvta X, cta C)
		{
			enforceSize();
			return VE_cmp_eqC(vx_vect,X.vx_vect,ui_size,C);
		}

		ui cmp_neC(cvta X, cta C)
		{
			enforceSize();
			return VE_cmp_neC(vx_vect,X.vx_vect,ui_size,C);
		}

		ui cmp_leC(cvta X, cta C)
		{
			enforceSize();
			return VE_cmp_leC(vx_vect,X.vx_vect,ui_size,C);
		}

		ui cmp_ltC(cvta X, cta C)
		{
			enforceSize();
			return VE_cmp_ltC(vx_vect,X.vx_vect,ui_size,C);
		}

		ui cmp_geC(cvta X, cta C)
		{
			enforceSize();
			return VE_cmp_geC(vx_vect,X.vx_vect,ui_size,C);
		}

		ui cmp_gtC(cvta X, cta C)
		{
			enforceSize();
			return VE_cmp_gtC(vx_vect,X.vx_vect,ui_size,C);
		}

		//indexed comparisons are members of uiVecObj

		void cmpV(cvta X, cvta Y)
		{
			enforceSize();
			VE_cmpV(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
		}

		ui cmp_eqV(cvta X, cvta Y)
		{
			enforceSize();
			return VE_cmp_eqV(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
		}

		ui cmp_neV(cvta X, cvta Y)
		{
			enforceSize();
			return VE_cmp_neV(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
		}

		ui cmp_leV(cvta X, cvta Y)
		{
			enforceSize();
			return VE_cmp_leV(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
		}

		ui cmp_ltV(cvta X, cvta Y)
		{
			enforceSize();
			return VE_cmp_ltV(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
		}

		ui cmp_geV(cvta X, cvta Y)
		{
			enforceSize();
			return VE_cmp_geV(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
		}

		ui cmp_gtV(cvta X, cvta Y)
		{
			enforceSize();
			return VE_cmp_gtV(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
		}

		//indexed comparisons are members of uiVecObj

		ui cmp_inclrange0C(cvta X, cta C)
		{
			enforceSize();
			return VE_cmp_inclrange0C(vx_vect,X.vx_vect,ui_size,C);
		}

		ui cmp_exclrange0C(cvta X, cta C)
		{
			enforceSize();
			return VE_cmp_exclrange0C(vx_vect,X.vx_vect,ui_size,C);
		}

		ui cmp_inclrangeCC(cvta X, cta CLo, cta CHi)
		{
			enforceSize();
			return VE_cmp_inclrangeCC(vx_vect,X.vx_vect,ui_size,CLo,CHi);
		}

		ui cmp_exclrangeCC(cvta X, cta CLo, cta CHi)
		{
			enforceSize();
			return VE_cmp_exclrangeCC(vx_vect,X.vx_vect,ui_size,CLo,CHi);
		}

		//indexed comparisons are members of uiVecObj


/////////////////////////////////////////////////////////////////////////////////////////
//Standard Arithmetics or Basic Arithmetics
//
		//equC, equV, and x_equV defined above

		void addC(cvta X, cta C)
		{
			enforceSize();
			VE_addC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subC(cvta X, cta C)
		{
			enforceSize();
			VE_subC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subrC(cvta X, cta C)
		{
			enforceSize();
			VE_subrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void mulC(cvta X, cta C)
		{
			enforceSize();
			VE_mulC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divC(cvta X, cta C)
		{
			enforceSize();
			VE_divC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divrC(cvta X, cta C)
		{
			enforceSize();
			VE_divrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void x_divrC(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			VEx_divrC(vx_vect, X.vx_vect, ui_size, A, B, C);
		}

		void addV(cvta X, cvta Y)
		{
			enforceSize();
			VE_addV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subV(cvta X, cvta Y)
		{
			enforceSize();
			VE_subV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subrV(cvta X, cvta Y)
		{
			enforceSize();
			VE_subrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void mulV(cvta X, cvta Y)
		{
			enforceSize();
			VE_mulV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divV(cvta X, cvta Y)
		{
			enforceSize();
			VE_divV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divrV(cvta X, cvta Y)
		{
			enforceSize();
			VE_divrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void x_addV(cvta X, cvta Y, cta A, cta B)
		{
			enforceSize();
			VEx_addV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void x_subV(cvta X, cvta Y, cta A, cta B)
		{
			enforceSize();
			VEx_subV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void x_subrV(cvta X, cvta Y, cta A, cta B)
		{
			enforceSize();
			VEx_subrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void x_mulV(cvta X, cvta Y, cta A, cta B)
		{
			enforceSize();
			VEx_mulV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void x_divV(cvta X, cvta Y, cta A, cta B)
		{
			enforceSize();
			VEx_divV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void x_divrV(cvta X, cvta Y, cta A, cta B)
		{
			enforceSize();
			VEx_divrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void s_addV(cvta X, cvta Y, cta C)
		{
			enforceSize();
			VEs_addV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, C);
		}

		void s_subV(cvta X, cvta Y, cta C)
		{
			enforceSize();
			VEs_subV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, C);
		}

		void s_subrV(cvta X, cvta Y, cta C)
		{
			enforceSize();
			VEs_subrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, C);
		}

		void s_mulV(cvta X, cvta Y, cta C)
		{
			enforceSize();
			VEs_mulV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, C);
		}

		void s_divV(cvta X, cvta Y, cta C)
		{
			enforceSize();
			VEs_divV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, C);
		}

		void s_divrV(cvta X, cvta Y, cta C)
		{
			enforceSize();
			VEs_divrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, C);
		}

		void maxC(cvta X, cta C)
		{
			enforceSize();
			VE_maxC( vx_vect, X.vx_vect, ui_size, C);
		}

		void minC(cvta X, cta C)
		{
			enforceSize();
			VE_minC( vx_vect, X.vx_vect, ui_size, C);
		}

		void limit(cvta X, cta Min, cta Max)
		{
			enforceSize();
			VE_limit( vx_vect, X.vx_vect, ui_size, Min, Max);
		}

		void flush0(cvta X, cta AbsMin)
		{
			enforceSize();
			VE_flush0( vx_vect, X.vx_vect, ui_size, AbsMin);
		}

		void flushInv(cvta X, cta AbsMin)
		{
			enforceSize();
			VE_flushInv(vx_vect, X.vx_vect, ui_size, AbsMin);
		}

		void maxV(cvta X, cvta Y)
		{
			enforceSize();
			VE_maxV( vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void minV(cvta X, cvta Y)
		{
			enforceSize();
			VE_minV( vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void modC(cvta X, cta C)
		{
			enforceSize();
			VE_modC(vx_vect, X.vx_vect, ui_size, C);
		}

		void modV(cvta X, cvta Y)
		{
			enforceSize();
			VE_modV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void x_modV(cvta X, cvta Y, cta A, cta B)
		{
			enforceSize();
			VEx_modV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void fmodC(cvta X, cta C)
		{
			modC(X,C);
		}

		void fmodV(cvta X, cvta Y)
		{
			modV(X,Y);
		}

		void x_fmodV(cvta X, cvta Y, cta A, cta B)
		{
			x_modV(X,Y,A,B);
		}

		void redC(cvta X, cta C)
		{
			enforceSize();
			VE_redC( vx_vect, X.vx_vect, ui_size, C);
		}

		void redV(cvta X, cvta Y)
		{
			enforceSize();
			VE_redV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void x_redV(cvta X, cvta Y, cta A, cta B)
		{
			enforceSize();
			VEx_redV( vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void visC( cvta X, cta C)
		{
			enforceSize();
			VE_visC( vx_vect, X.vx_vect, ui_size, C);
		}

		void visV(cvta X, cvta Y)
		{
			enforceSize();
			VE_visV( vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void x_visV(cvta X, cvta Y, cta A, cta B)
		{
			enforceSize();
			VEx_visV( vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void hypC( cvta X, cta C)
		{
			enforceSize();
			VE_hypC( vx_vect, X.vx_vect, ui_size, C);
		}

		void hypV(cvta X, cvta Y)
		{
			enforceSize();
			VE_hypV( vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void x_hypV(cvta X, cvta Y, cta A, cta B)
		{
			enforceSize();
			VEx_hypV( vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void lincomb(cvta X, cvta Y, cta CX, cta CY)
		{
			enforceSize();
			VE_lincomb( vx_vect, X.vx_vect, Y.vx_vect, ui_size, CX, CY);
		}

		void sgn(cvta X)
		{
			cmp0(X);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Accumulation
//
		void accV(cvta X)
		{
			enforceSize();
			VE_accV(vx_vect,X.vx_vect,ui_size);
		}

		//defined by including fVecObj.h
		void accVF( const  fVecObj& X);

		//defined by including dVecObj.h
		void accVD( const  dVecObj& X);

		//defined by including biVecObj.h
		void accVBI(const biVecObj& X);

		//defined by including siVecObj.h
		void accVSI(const siVecObj& X);

		//defined by including iVecObj.h
		void accVI( const  iVecObj& X);

		//defined by including liVecObj.h
		void accVLI(const liVecObj& X);

		//defined by including qiVecObj.h
		void accVQI(const qiVecObj& X);

		//defined by including ubVecObj.h
		void accVUB(const ubVecObj& X);

		//defined by including usVecObj.h
		void accVUS(const usVecObj& X);

		//defined by including uVecObj.h
		void accVU( const  uVecObj& X);

		//defined by including ulVecObj.h
		void accVUL(const ulVecObj& X);

		void accVUI(const uiVecObj& X)
		{
			enforceSize();
			VE_accVUI(vx_vect,X.vx_vect,ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Bit-wise operations
//


/////////////////////////////////////////////////////////////////////////////////////////
//Functions of a sub-set of elements
//
		void subvector_equC(const unsigned& samp, cta C)
		{
			enforceSize();
			VE_subvector_equC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_equV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VE_subvector_equV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void subvector_addC(const unsigned& samp, cta C)
		{
			enforceSize();
			VE_subvector_addC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_subC(const unsigned& samp, cta C)
		{
			enforceSize();
			VE_subvector_subC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_subrC(const unsigned& samp, cta C)
		{
			enforceSize();
			VE_subvector_subrC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_mulC(const unsigned& samp, cta C)
		{
			enforceSize();
			VE_subvector_mulC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_divC(const unsigned& samp, cta C)
		{
			enforceSize();
			VE_subvector_divC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_divrC(const unsigned& samp, cta C)
		{
			enforceSize();
			VE_subvector_divrC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_addV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VE_subvector_addV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void subvector_subV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VE_subvector_subV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void subvector_subrV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VE_subvector_subrV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void subvector_mulV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VE_subvector_mulV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void subvector_divV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VE_subvector_divV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void subvector_divrV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VE_subvector_divrV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Mathematical Functions
//

		int abs(cvta X)
		{
			enforceSize();
			return VE_abs(vx_vect, X.vx_vect, ui_size);
		}

		int neg(cvta X)
		{
			enforceSize();
			return VE_neg(vx_vect, X.vx_vect, ui_size);
		}

		int inv(cvta X)
		{
			enforceSize();
			return VE_inv(vx_vect, X.vx_vect, ui_size);
		}

		int x_inv(cvta X, cta A, cta B)
		{
			enforceSize();
			return VEx_inv(vx_vect, X.vx_vect, ui_size, A, B);
		}

		int intfrac(vt* Frac, cvta X)
		{
			enforceSize();
			enforceSize(Frac->ui_size);
			return VE_intfrac(vx_vect,Frac->vx_vect,X.vx_vect,ui_size);
		}

                int intfrac(vt Frac, cvta X)
		{
			enforceSize();
                        enforceSize(Frac.ui_size);
                        return VE_intfrac(vx_vect,Frac.vx_vect,X.vx_vect,ui_size);
		}

		int modf(vt* Frac, cvta X)
		{
			return intfrac(Frac,X);
		}

                int modf(vt Frac, cvta X)
		{
			return intfrac(Frac,X);
		}

		//defined by including iVecObj.h
                int mantexp(iVecObj* Exp, const eVecObj& X);
                int mantexp(iVecObj  Exp, const eVecObj& X);

		int frexp(iVecObj* Exp, cvta X){return mantexp(Exp,X);}
                int frexp(iVecObj  Exp, cvta X){return mantexp(Exp,X);}

		int hypotC(cvta X, cta C)
		{
			enforceSize();
			return VE_hypotC(vx_vect, X.vx_vect, ui_size, C);
		}

		int hypotV(cvta X, cvta Y)
		{
			enforceSize();
			return VE_hypotV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		int x_hypotV(cvta X, cvta Y, cta A, cta B)
		{
			enforceSize();
			return VEx_hypotV(vx_vect,X.vx_vect,Y.vx_vect,ui_size,A,B);
		}

		int scale2(cvta X, int expo)
		{
			enforceSize();
			return VE_scale2(vx_vect, X.vx_vect, ui_size, expo);
		}

		int scale10(cvta X, int expo)
		{
			enforceSize();
			return VE_scale10(vx_vect, X.vx_vect, ui_size, expo);
		}

		int ldexp(cvta X, int expo)
		{
			return scale2(X,expo);
		}

		int square(cvta X)
		{
			enforceSize();
			return VE_square(vx_vect,X.vx_vect,ui_size);
		}

		int x_square(cvta X, cta A, cta B)
		{
			enforceSize();
			return VEx_square(vx_vect,X.vx_vect,ui_size,A,B);
		}

		int cubic(cvta X)
		{
			enforceSize();
			return VE_cubic(vx_vect,X.vx_vect,ui_size);
		}

		int x_cubic(cvta X, cta A, cta B)
		{
			enforceSize();
			return VEx_cubic(vx_vect,X.vx_vect,ui_size,A,B);
		}

		int quartic(cvta X)
		{
			enforceSize();
			return VE_quartic(vx_vect,X.vx_vect,ui_size);
		}

		int x_quartic(cvta X, cta A, cta B)
		{
			enforceSize();
			return VEx_quartic(vx_vect,X.vx_vect,ui_size,A,B);
		}

		int poly(cvta X, cvta Coeff)
		{
			enforceSize();
			return VE_poly(vx_vect,X.vx_vect,ui_size,
				Coeff.vx_vect,Coeff.ui_size);
		}

		int x_poly(cvta X, cvta Coeff, cta A, cta B)
		{
			enforceSize();
			return VEx_poly(vx_vect,X.vx_vect,ui_size,
				Coeff.vx_vect,Coeff.ui_size,A,B);
		}

		int ipow(cvta X, int pow)
		{
			enforceSize();
			return VE_ipow(vx_vect, X.vx_vect, ui_size, pow);
		}

		int x_ipow(cvta X, int pow, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_ipow(vx_vect, X.vx_vect, ui_size, pow, A, B, C);
		}

		int u_square(cvta X)
		{
			enforceSize();
			return VEu_square(vx_vect,X.vx_vect,ui_size);
		}

		int ux_square(cvta X, cta A, cta B)
		{
			enforceSize();
			return VEux_square(vx_vect,X.vx_vect,ui_size,A,B);
		}

		int u_cubic(cvta X)
		{
			enforceSize();
			return VEu_cubic(vx_vect,X.vx_vect,ui_size);
		}

		int ux_cubic(cvta X, cta A, cta B)
		{
			enforceSize();
			return VEux_cubic(vx_vect,X.vx_vect,ui_size,A,B);
		}

		int u_quartic(cvta X)
		{
			enforceSize();
			return VEu_quartic(vx_vect,X.vx_vect,ui_size);
		}

		int ux_quartic(cvta X, cta A, cta B)
		{
			enforceSize();
			return VEux_quartic(vx_vect,X.vx_vect,ui_size,A,B);
		}

		int u_poly(cvta X, cvta Coeff)
		{
			enforceSize();
			return VEu_poly(vx_vect,X.vx_vect,ui_size,
				Coeff.vx_vect,Coeff.ui_size);
		}

		int ux_poly(cvta X, cvta Coeff, cta A, cta B)
		{
			enforceSize();
			return VEux_poly(vx_vect,X.vx_vect,ui_size,
				Coeff.vx_vect,Coeff.ui_size,A,B);
		}

		int u_ipow(cvta X, int pow)
		{
			enforceSize();
			return VEu_ipow(vx_vect, X.vx_vect, ui_size, pow);
		}

		int ux_ipow(cvta X, int pow, cta A, cta B, cta C)
		{
			enforceSize();
			return VEux_ipow(vx_vect, X.vx_vect, ui_size, pow, A, B, C);
		}

		//defined by including iVecObj.h
		int ipow2(const iVecObj& X);

		//defined by including iVecObj.h
		int ipow10(const iVecObj& X);

		int pow(cvta X, cta expo)
		{
			enforceSize();
			return VE_pow(vx_vect,X.vx_vect,ui_size,expo);
		}

		int x_pow(cvta X, cta expo, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_pow(vx_vect,X.vx_vect,ui_size,expo,A,B,C);
		}

		int pow2(cvta X)
		{
			enforceSize();
			return VE_pow2(vx_vect,X.vx_vect,ui_size);
		}

		int x_pow2(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_pow2(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int pow10(cvta X)
		{
			enforceSize();
			return VE_pow10(vx_vect,X.vx_vect,ui_size);
		}

		int x_pow10(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_pow10(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int sqrt(cvta X)
		{
			enforceSize();
			return VE_sqrt(vx_vect,X.vx_vect,ui_size);
		}

		int x_sqrt(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_sqrt(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int log(cvta X)
		{
			enforceSize();
			return VE_log(vx_vect,X.vx_vect,ui_size);
		}

		int log2(cvta X)
		{
			enforceSize();
			return VE_log2(vx_vect,X.vx_vect,ui_size);
		}

		int log10(cvta X)
		{
			enforceSize();
			return VE_log10(vx_vect,X.vx_vect,ui_size);
		}

		int ln(cvta X)
		{
			return log(X);
		}

		int x_log(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_log(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int x_log2(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_log2(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int x_log10(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_log10(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int x_ln(cvta X, cta A, cta B, cta C)
		{
			return x_log(X,A,B,C);
		}

		int OD(cvta X, cvta X0)
		{
			enforceSize();
			return VE_OD(vx_vect,X.vx_vect,X0.vx_vect,ui_size);
		}

		int ODwDark(cvta X, cvta XDark, cvta X0, cvta X0Dark)
		{
			enforceSize();
			return VE_ODwDark(vx_vect,X.vx_vect,XDark.vx_vect,
				X0.vx_vect,X0Dark.vx_vect,ui_size);
		}

		void setODThresh(cta minX, cta minX0)
		{
			VE_setODThresh(minX,minX0);
		}

		int exp(cvta X)
		{
			enforceSize();
			return VE_exp(vx_vect,X.vx_vect,ui_size);
		}

		int x_exp(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_exp(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int expArbBase(cvta X, cta Base)
		{
			enforceSize();
			return VE_expArbBase(vx_vect,X.vx_vect,ui_size,Base);
		}

		int x_expArbBase(cvta X, cta Base, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_expArbBase(vx_vect,X.vx_vect,ui_size,Base,A,B,C);
		}

		int expc(cvta X)
		{
			enforceSize();
			return VE_expc(vx_vect,X.vx_vect,ui_size);
		}

		int x_expc(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_expc(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int exp2(cvta X)
		{
			enforceSize();
			return VE_exp2(vx_vect,X.vx_vect,ui_size);
		}

		int x_exp2(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_exp2(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int exp10(cvta X)
		{
			enforceSize();
			return VE_exp10(vx_vect,X.vx_vect,ui_size);
		}

		int x_exp10(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_exp10(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int expmx2(cvta X)
		{
			enforceSize();
			return VE_expmx2(vx_vect,X.vx_vect,ui_size);
		}

		int x_expmx2(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_expmx2(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int erf(cvta X)
		{
			enforceSize();
			return VE_erf(vx_vect,X.vx_vect,ui_size);
		}

		int x_erf(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_erf(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int erfc(cvta X)
		{
			enforceSize();
			return VE_erfc(vx_vect,X.vx_vect,ui_size);
		}

		int x_erfc(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_erfc(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int Gauss(cvta X, cta Wid, cta Cent, cta C)
		{
			enforceSize();
			return VE_Gauss(vx_vect,X.vx_vect,ui_size,Wid,Cent,C);
		}

		//lower case name for convenience
		int gauss(cvta X, cta Wid, cta Cent, cta C)
		{
			return Gauss(X,Wid,Cent,C);
		}

		int Lorentz(cvta X, cta Wid, cta Cent, cta C)
		{
			enforceSize();
			return VE_Lorentz(vx_vect,X.vx_vect,ui_size,Wid,Cent,C);
		}

		//lower case name for convenience
		int lorentz(cvta X, cta Wid, cta Cent, cta C)
		{
			return Lorentz(X,Wid,Cent,C);
		}

		int sin(cvta X)
		{
			enforceSize();
			return VE_sin(vx_vect,X.vx_vect,ui_size);
		}

		int x_sin(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_sin(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int cos(cvta X)
		{
			enforceSize();
			return VE_cos(vx_vect,X.vx_vect,ui_size);
		}

		int x_cos(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_cos(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int sincos(vt* Cos, cvta X)
		{
			enforceSize();
			enforceSize(Cos->ui_size);
			return VE_sincos(vx_vect,Cos->vx_vect,X.vx_vect,ui_size);
		}

                int sincos(vt Cos, cvta X)
		{
			enforceSize();
                        enforceSize(Cos.ui_size);
                        return VE_sincos(vx_vect,Cos.vx_vect,X.vx_vect,ui_size);
		}

		int x_sincos(vt* Cos, cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			enforceSize(Cos->ui_size);
			return VEx_sincos(vx_vect,Cos->vx_vect,X.vx_vect,ui_size,A,B,C);
		}

                int x_sincos(vt Cos, cvta X, cta A, cta B, cta C)
		{
			enforceSize();
                        enforceSize(Cos.ui_size);
                        return VEx_sincos(vx_vect,Cos.vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int tan(cvta X)
		{
			enforceSize();
			return VE_tan(vx_vect,X.vx_vect,ui_size);
		}

		int x_tan(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_tan(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int cot(cvta X)
		{
			enforceSize();
			return VE_cot(vx_vect,X.vx_vect,ui_size);
		}

		int x_cot(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_cot(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int sec(cvta X)
		{
			enforceSize();
			return VE_sec(vx_vect,X.vx_vect,ui_size);
		}

		int x_sec(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_sec(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int cosec(cvta X)
		{
			enforceSize();
			return VE_cosec(vx_vect,X.vx_vect,ui_size);
		}

		int x_cosec(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_cosec(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int sin2(cvta X)
		{
			enforceSize();
			return VE_sin2(vx_vect,X.vx_vect,ui_size);
		}

		int x_sin2(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_sin2(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int cos2(cvta X)
		{
			enforceSize();
			return VE_cos2(vx_vect,X.vx_vect,ui_size);
		}

		int x_cos2(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_cos2(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int sincos2(vt* Cos, cvta X)
		{
			enforceSize();
			enforceSize(Cos->ui_size);
			return VE_sincos2(vx_vect,Cos->vx_vect,X.vx_vect,ui_size);
		}

                int sincos2(vt Cos, cvta X)
		{
			enforceSize();
                        enforceSize(Cos.ui_size);
                        return VE_sincos2(vx_vect,Cos.vx_vect,X.vx_vect,ui_size);
		}

		int x_sincos2(vt* Cos, cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			enforceSize(Cos->ui_size);
			return VEx_sincos2(vx_vect,Cos->vx_vect,X.vx_vect,ui_size,A,B,C);
		}

                int x_sincos2(vt Cos, cvta X, cta A, cta B, cta C)
		{
			enforceSize();
                        enforceSize(Cos.ui_size);
                        return VEx_sincos2(vx_vect,Cos.vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int tan2(cvta X)
		{
			enforceSize();
			return VE_tan2(vx_vect,X.vx_vect,ui_size);
		}

		int x_tan2(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_tan2(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int cot2(cvta X)
		{
			enforceSize();
			return VE_cot2(vx_vect,X.vx_vect,ui_size);
		}

		int x_cot2(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_cot2(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int sec2(cvta X)
		{
			enforceSize();
			return VE_sec2(vx_vect,X.vx_vect,ui_size);
		}

		int x_sec2(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_sec2(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int cosec2(cvta X)
		{
			enforceSize();
			return VE_cosec2(vx_vect,X.vx_vect,ui_size);
		}

		int x_cosec2(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_cosec2(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int r_sin(cvta X)
		{
			enforceSize();
			return VEr_sin(vx_vect,X.vx_vect,ui_size);
		}

		int rx_sin(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VErx_sin(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int r_cos(cvta X)
		{
			enforceSize();
			return VEr_cos(vx_vect,X.vx_vect,ui_size);
		}

		int rx_cos(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VErx_cos(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int r_sincos(vt* Cos, cvta X)
		{
			enforceSize();
			enforceSize(Cos->ui_size);
			return VEr_sincos(vx_vect,Cos->vx_vect,X.vx_vect,ui_size);
		}

                int r_sincos(vt Cos, cvta X)
		{
			enforceSize();
                        enforceSize(Cos.ui_size);
                        return VEr_sincos(vx_vect,Cos.vx_vect,X.vx_vect,ui_size);
		}

		int rx_sincos(vt* Cos, cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			enforceSize(Cos->ui_size);
			return VErx_sincos(vx_vect,Cos->vx_vect,X.vx_vect,ui_size,A,B,C);
		}

                int rx_sincos(vt Cos, cvta X, cta A, cta B, cta C)
		{
			enforceSize();
                        enforceSize(Cos.ui_size);
                        return VErx_sincos(vx_vect,Cos.vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int r_sin2(cvta X)
		{
			enforceSize();
			return VEr_sin2(vx_vect,X.vx_vect,ui_size);
		}

		int rx_sin2(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VErx_sin2(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int r_cos2(cvta X)
		{
			enforceSize();
			return VEr_cos2(vx_vect,X.vx_vect,ui_size);
		}

		int rx_cos2(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VErx_cos2(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int r_sincos2(vt* Cos, cvta X)
		{
			enforceSize();
			enforceSize(Cos->ui_size);
			return VEr_sincos2(vx_vect,Cos->vx_vect,X.vx_vect,ui_size);
		}

                int r_sincos2(vt Cos, cvta X)
		{
			enforceSize();
                        enforceSize(Cos.ui_size);
                        return VEr_sincos2(vx_vect,Cos.vx_vect,X.vx_vect,ui_size);
		}

		int rx_sincos2(vt* Cos, cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			enforceSize(Cos->ui_size);
			return VErx_sincos2(vx_vect,Cos->vx_vect,X.vx_vect,ui_size,A,B,C);
		}

                int rx_sincos2(vt Cos, cvta X, cta A, cta B, cta C)
		{
			enforceSize();
                        enforceSize(Cos.ui_size);
                        return VErx_sincos2(vx_vect,Cos.vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		//defined by including iVecObj.h
		int sinrpi(const iVecObj& P, int q);

		//defined by including iVecObj.h
		int cosrpi(const iVecObj& P, int q);

		//defined by including iVecObj.h
		int sincosrpi(vt* Cos, const iVecObj& P, int q);
                int sincosrpi(vt  Cos, const iVecObj& P, int q);

		//defined by including iVecObj.h
		int tanrpi(const iVecObj& P, int q);

		//defined by including iVecObj.h
		int cotrpi(const iVecObj& P, int q);

		//defined by including iVecObj.h
		int secrpi(const iVecObj& P, int q);

		//defined by including iVecObj.h
		int cosecrpi(const iVecObj& P, int q);


		//defined by including iVecObj.h
		int sinrpi2(const iVecObj& P, int q);

		//defined by including iVecObj.h
		int cosrpi2(const iVecObj& P, int q);

		//defined by including iVecObj.h
		int sincosrpi2(vt* Cos, const iVecObj& P, int q);
                int sincosrpi2(vt  Cos, const iVecObj& P, int q);

		//defined by including iVecObj.h
		int tanrpi2(const iVecObj& P, int q);

		//defined by including iVecObj.h
		int cotrpi2(const iVecObj& P, int q);

		//defined by including iVecObj.h
		int secrpi2(const iVecObj& P, int q);

		//defined by including iVecObj.h
		int cosecrpi2(const iVecObj& P, int q);


		//defined by including iVecObj.h
		int sinrpi3(const iVecObj& P, int q);

		//defined by including iVecObj.h
		int cosrpi3(const iVecObj& P, int q);

		//defined by including iVecObj.h
		int sincosrpi3(vt* Cos, const iVecObj& P, int q);
                int sincosrpi3(vt  Cos, const iVecObj& P, int q);

		//defined by including iVecObj.h
		int tanrpi3(const iVecObj& P, int q);

		//defined by including iVecObj.h
		int cotrpi3(const iVecObj& P, int q);

		//defined by including iVecObj.h
		int secrpi3(const iVecObj& P, int q);

		//defined by including iVecObj.h
		int cosecrpi3(const iVecObj& P, int q);

		int sinc(cvta X)
		{
			enforceSize();
			return VE_sinc(vx_vect,X.vx_vect,ui_size);
		}

		int x_sinc(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_sinc(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int Kepler(cvta X, cta T, cta e)
		{
			enforceSize();
			return VE_Kepler(vx_vect,X.vx_vect,ui_size,T,e);
		}

		int x_Kepler(cvta X, cta T, cta e, cta A, cta B)
		{
			enforceSize();
			return VEx_Kepler(vx_vect,X.vx_vect,ui_size,T,e,A,B);
		}

		//lower case name for convenience
		int kepler(cvta X, cta T, cta e)
		{
			return Kepler(X,T,e);
		}

		//lower case name for convenience
		int x_kepler(cvta X, cta T, cta e, cta A, cta B)
		{
			return x_Kepler(X,T,e,A,B);
		}

		int asin(cvta X)
		{
			enforceSize();
			return VE_asin(vx_vect,X.vx_vect,ui_size);
		}

		int x_asin(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_asin(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int acos(cvta X)
		{
			enforceSize();
			return VE_acos(vx_vect,X.vx_vect,ui_size);
		}

		int x_acos(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_acos(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int atan(cvta X)
		{
			enforceSize();
			return VE_atan(vx_vect,X.vx_vect,ui_size);
		}

		int x_atan(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_atan(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int atan2(cvta X, cvta Y)
		{
			enforceSize();
			return VE_atan2(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
		}

		int x_atan2(cvta X, cvta Y, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_atan2(vx_vect,X.vx_vect,Y.vx_vect,ui_size,A,B,C);
		}

		int sinh(cvta X)
		{
			enforceSize();
			return VE_sinh(vx_vect,X.vx_vect,ui_size);
		}

		int x_sinh(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_sinh(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int cosh(cvta X)
		{
			enforceSize();
			return VE_cosh(vx_vect,X.vx_vect,ui_size);
		}

		int x_cosh(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_cosh(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int tanh(cvta X)
		{
			enforceSize();
			return VE_tanh(vx_vect,X.vx_vect,ui_size);
		}

		int x_tanh(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_tanh(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int coth(cvta X)
		{
			enforceSize();
			return VE_coth(vx_vect,X.vx_vect,ui_size);
		}

		int x_coth(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_coth(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int sech(cvta X)
		{
			enforceSize();
			return VE_sech(vx_vect,X.vx_vect,ui_size);
		}

		int x_sech(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_sech(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int cosech(cvta X)
		{
			enforceSize();
			return VE_cosech(vx_vect,X.vx_vect,ui_size);
		}

		int x_cosech(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_cosech(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int sech2(cvta X)
		{
			enforceSize();
			return VE_sech2(vx_vect,X.vx_vect,ui_size);
		}

		int x_sech2(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VEx_sech2(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

            #ifdef __VGRAPH_H
                void xyAutoPlot( cvta X, unsigned form, COLORREF color )
                {
                    enforceSize();
                    enforceSize(X.ui_size);
                    VE_xyAutoPlot( X.vx_vect, vx_vect, ui_size, form, color );
                }

                void xy2AutoPlot( cvta X1, unsigned form1, COLORREF color1,
                         cvta X2, cvta Y2, unsigned form2, COLORREF color2 )
                {
                    enforceSize();
                    enforceSize(X1.ui_size);
                    Y2.enforceSize(X2.ui_size);
                    VE_xy2AutoPlot( X1.vx_vect, vx_vect, ui_size, form1, color1,
                              X2.vx_vect, Y2.vx_vect, X2.ui_size, form2, color2 );
                }

                void yAutoPlot( unsigned form, COLORREF color )
                {
                    enforceSize();
                    VE_yAutoPlot( vx_vect, ui_size, form, color );
                }

                void y2AutoPlot( unsigned form1, COLORREF color1,
                        cvta Y2, unsigned form2, COLORREF color2 )
                {
                    enforceSize();
                    Y2.enforceSize();
                    VE_y2AutoPlot( vx_vect, ui_size, form1, color1,
                             Y2.vx_vect, Y2.ui_size, form2, color2 );
                }

                void xyDataPlot( cvta X, unsigned form, COLORREF color )
                {
                    enforceSize();
                    enforceSize(X.ui_size);
                    VE_xyDataPlot( X.vx_vect, vx_vect, ui_size, form, color );
                }

                void yDataPlot( unsigned form, COLORREF color )
                {
                    enforceSize();
                    VE_yDataPlot( vx_vect, ui_size, form, color );
                }
            #endif   // __VGRAPH_H

	};


#ifdef UBVECOBJ_H
	inline void eVecObj::accVUB(const ubVecObj& X)
	{
		enforceSize();
		VE_accVUB(vx_vect,X.vx_vect,ui_size);
	}

	inline void eVecObj::UBtoE(const ubVecObj& X)
	{
		enforceSize();
		V_UBtoE(vx_vect,X.vx_vect,ui_size);
	}

	inline int ubVecObj::roundtoUB(const eVecObj& X)
	{
		enforceSize();
		return VE_roundtoUB(vx_vect,X.vx_vect,ui_size);
	}

	inline int ubVecObj::floortoUB(const eVecObj& X)
	{
		enforceSize();
		return VE_floortoUB(vx_vect,X.vx_vect,ui_size);
	}

	inline int ubVecObj::ceiltoUB(const eVecObj& X)
	{
		enforceSize();
		return VE_ceiltoUB(vx_vect,X.vx_vect,ui_size);
	}

	inline int ubVecObj::choptoUB(const eVecObj& X)
	{
		enforceSize();
		return VE_choptoUB(vx_vect,X.vx_vect,ui_size);
	}
#endif //UBVECOBJ_H

#ifdef USVECOBJ_H
	inline void eVecObj::accVUS(const usVecObj& X)
	{
		enforceSize();
		VE_accVUS(vx_vect,X.vx_vect,ui_size);
	}

	inline void eVecObj::UStoE(const usVecObj& X)
	{
		enforceSize();
		V_UStoE(vx_vect,X.vx_vect,ui_size);
	}

	inline int usVecObj::roundtoUS(const eVecObj& X)
	{
		enforceSize();
		return VE_roundtoUS(vx_vect,X.vx_vect,ui_size);
	}

	inline int usVecObj::floortoUS(const eVecObj& X)
	{
		enforceSize();
		return VE_floortoUS(vx_vect,X.vx_vect,ui_size);
	}

	inline int usVecObj::ceiltoUS(const eVecObj& X)
	{
		enforceSize();
		return VE_ceiltoUS(vx_vect,X.vx_vect,ui_size);
	}

	inline int usVecObj::choptoUS(const eVecObj& X)
	{
		enforceSize();
		return VE_choptoUS(vx_vect,X.vx_vect,ui_size);
	}
#endif //USVECOBJ

#ifdef UVECOBJ_H
	inline void eVecObj::accVU(const uVecObj& X)
	{
		enforceSize();
		VE_accVU(vx_vect,X.vx_vect,ui_size);
	}

	inline void eVecObj::UtoE(const uVecObj& X)
	{
		enforceSize();
		V_UtoE(vx_vect,X.vx_vect,ui_size);
	}

	inline int uVecObj::roundtoU(const eVecObj& X)
	{
		enforceSize();
		return VE_roundtoU(vx_vect,X.vx_vect,ui_size);
	}

	inline int uVecObj::floortoU(const eVecObj& X)
	{
		enforceSize();
		return VE_floortoU(vx_vect,X.vx_vect,ui_size);
	}

	inline int uVecObj::ceiltoU(const eVecObj& X)
	{
		enforceSize();
		return VE_ceiltoU(vx_vect,X.vx_vect,ui_size);
	}

	inline int uVecObj::choptoU(const eVecObj& X)
	{
		enforceSize();
		return VE_choptoU(vx_vect,X.vx_vect,ui_size);
	}
#endif //UVECOBJ_H

#ifdef ULVECOBJ_H
	inline void eVecObj::accVUL(const ulVecObj& X)
	{
		enforceSize();
		VE_accVUL(vx_vect,X.vx_vect,ui_size);
	}

	inline void eVecObj::ULtoE(const ulVecObj& X)
	{
		enforceSize();
		V_ULtoE(vx_vect,X.vx_vect,ui_size);
	}

	inline int ulVecObj::roundtoUL(const eVecObj& X)
	{
		enforceSize();
		return VE_roundtoUL(vx_vect,X.vx_vect,ui_size);
	}

	inline int ulVecObj::floortoUL(const eVecObj& X)
	{
		enforceSize();
		return VE_floortoUL(vx_vect,X.vx_vect,ui_size);
	}

	inline int ulVecObj::ceiltoUL(const eVecObj& X)
	{
		enforceSize();
		return VE_ceiltoUL(vx_vect,X.vx_vect,ui_size);
	}

	inline int ulVecObj::choptoUL(const eVecObj& X)
	{
		enforceSize();
		return VE_choptoUL(vx_vect,X.vx_vect,ui_size);
	}
#endif //ULVECOBJ_H

#ifdef BIVECOBJ_H
	inline void eVecObj::accVBI(const biVecObj& X)
	{
		enforceSize();
		VE_accVBI(vx_vect,X.vx_vect,ui_size);
	}

	inline void eVecObj::BItoE(const biVecObj& X)
	{
		enforceSize();
		V_BItoE(vx_vect,X.vx_vect,ui_size);
	}

	inline int biVecObj::roundtoBI(const eVecObj& X)
	{
		enforceSize();
		return VE_roundtoBI(vx_vect,X.vx_vect,ui_size);
	}

	inline int biVecObj::floortoBI(const eVecObj& X)
	{
		enforceSize();
		return VE_floortoBI(vx_vect,X.vx_vect,ui_size);
	}

	inline int biVecObj::ceiltoBI(const eVecObj& X)
	{
		enforceSize();
		return VE_ceiltoBI(vx_vect,X.vx_vect,ui_size);
	}

	inline int biVecObj::choptoBI(const eVecObj& X)
	{
		enforceSize();
		return VE_choptoBI(vx_vect,X.vx_vect,ui_size);
	}
#endif //BIVECOBJ_H

#ifdef SIVECOBJ_H
	inline void eVecObj::accVSI(const siVecObj& X)
	{
		enforceSize();
		VE_accVSI(vx_vect,X.vx_vect,ui_size);
	}

	inline void eVecObj::SItoE(const siVecObj& X)
	{
		enforceSize();
		V_SItoE(vx_vect,X.vx_vect,ui_size);
	}

	inline int siVecObj::roundtoSI(const eVecObj& X)
	{
		enforceSize();
		return VE_roundtoSI(vx_vect,X.vx_vect,ui_size);
	}

	inline int siVecObj::floortoSI(const eVecObj& X)
	{
		enforceSize();
		return VE_floortoSI(vx_vect,X.vx_vect,ui_size);
	}

	inline int siVecObj::ceiltoSI(const eVecObj& X)
	{
		enforceSize();
		return VE_ceiltoSI(vx_vect,X.vx_vect,ui_size);
	}

	inline int siVecObj::choptoSI(const eVecObj& X)
	{
		enforceSize();
		return VE_choptoSI(vx_vect,X.vx_vect,ui_size);
	}
#endif //SIVECOBJ_H

#ifdef IVECOBJ_H
	inline void eVecObj::accVI(const iVecObj& X)
	{
		enforceSize();
		VE_accVI(vx_vect,X.vx_vect,ui_size);
	}

	inline void eVecObj::ItoE(const iVecObj& X)
	{
		enforceSize();
		V_ItoE(vx_vect,X.vx_vect,ui_size);
	}

	inline int iVecObj::roundtoI(const eVecObj& X)
	{
		enforceSize();
		return VE_roundtoI(vx_vect,X.vx_vect,ui_size);
	}

	inline int iVecObj::floortoI(const eVecObj& X)
	{
		enforceSize();
		return VE_floortoI(vx_vect,X.vx_vect,ui_size);
	}

	inline int iVecObj::ceiltoI(const eVecObj& X)
	{
		enforceSize();
		return VE_ceiltoI(vx_vect,X.vx_vect,ui_size);
	}

	inline int iVecObj::choptoI(const eVecObj& X)
	{
		enforceSize();
		return VE_choptoI(vx_vect,X.vx_vect,ui_size);
	}

        inline int eVecObj::mantexp(iVecObj* Exp, const eVecObj& X)
	{
		enforceSize();
		if(ui_size != Exp->ui_size) throw OptiVec_SizeErr();
		return VE_mantexp(vx_vect,Exp->vx_vect,X.vx_vect,ui_size);
	}

        inline int eVecObj::mantexp(iVecObj Exp, const eVecObj& X)
	{
		enforceSize();
                if(ui_size != Exp.ui_size) throw OptiVec_SizeErr();
                return VE_mantexp(vx_vect,Exp.vx_vect,X.vx_vect,ui_size);
	}

	inline int eVecObj::ipow2(const iVecObj& X)
	{
		enforceSize();
		return VE_ipow2(vx_vect,X.vx_vect,ui_size);
	}

	inline int eVecObj::ipow10(const iVecObj& X)
	{
		enforceSize();
		return VE_ipow10(vx_vect,X.vx_vect,ui_size);
	}

	inline int eVecObj::sinrpi(const iVecObj& P, int q)
	{
		enforceSize();
		return VE_sinrpi(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::cosrpi(const iVecObj& P, int q)
	{
		enforceSize();
		return VE_cosrpi(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::sincosrpi(eVecObj* Cos, const iVecObj& P, int q)
	{
		enforceSize();
		if(ui_size != Cos->ui_size) throw OptiVec_SizeErr();
		return VE_sincosrpi(vx_vect,Cos->vx_vect,P.vx_vect,ui_size,q);
	}

        inline int eVecObj::sincosrpi(eVecObj Cos, const iVecObj& P, int q)
	{
		enforceSize();
                if(ui_size != Cos.ui_size) throw OptiVec_SizeErr();
                return VE_sincosrpi(vx_vect,Cos.vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::tanrpi(const iVecObj& P, int q)
	{
		enforceSize();
		return VE_tanrpi(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::cotrpi(const iVecObj& P, int q)
	{
		enforceSize();
		return VE_cotrpi(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::secrpi(const iVecObj& P, int q)
	{
		enforceSize();
		return VE_secrpi(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::cosecrpi(const iVecObj& P, int q)
	{
		enforceSize();
		return VE_cosecrpi(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::sinrpi2(const iVecObj& P, int q)
	{
		enforceSize();
		return VE_sinrpi2(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::cosrpi2(const iVecObj& P, int q)
	{
		enforceSize();
		return VE_cosrpi2(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::sincosrpi2(eVecObj* Cos, const iVecObj& P, int q)
	{
		enforceSize();
		if(ui_size != Cos->ui_size) throw OptiVec_SizeErr();
		return VE_sincosrpi2(vx_vect,Cos->vx_vect,P.vx_vect,ui_size,q);
	}

        inline int eVecObj::sincosrpi2(eVecObj Cos, const iVecObj& P, int q)
	{
		enforceSize();
                if(ui_size != Cos.ui_size) throw OptiVec_SizeErr();
                return VE_sincosrpi2(vx_vect,Cos.vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::tanrpi2(const iVecObj& P, int q)
	{
		enforceSize();
		return VE_tanrpi2(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::cotrpi2(const iVecObj& P, int q)
	{
		enforceSize();
		return VE_cotrpi2(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::secrpi2(const iVecObj& P, int q)
	{
		enforceSize();
		return VE_secrpi2(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::cosecrpi2(const iVecObj& P, int q)
	{
		enforceSize();
		return VE_cosecrpi2(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::sinrpi3(const iVecObj& P, int q)
	{
		enforceSize();
		return VE_sinrpi3(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::cosrpi3(const iVecObj& P, int q)
	{
		enforceSize();
		return VE_cosrpi3(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::sincosrpi3(eVecObj* Cos, const iVecObj& P, int q)
	{
		enforceSize();
		if(ui_size != Cos->ui_size) throw OptiVec_SizeErr();
		return VE_sincosrpi3(vx_vect,Cos->vx_vect,P.vx_vect,ui_size,q);
	}

        inline int eVecObj::sincosrpi3(eVecObj Cos, const iVecObj& P, int q)
	{
		enforceSize();
                if(ui_size != Cos.ui_size) throw OptiVec_SizeErr();
                return VE_sincosrpi3(vx_vect,Cos.vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::tanrpi3(const iVecObj& P, int q)
	{
		enforceSize();
		return VE_tanrpi3(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::cotrpi3(const iVecObj& P, int q)
	{
		enforceSize();
		return VE_cotrpi3(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::secrpi3(const iVecObj& P, int q)
	{
		enforceSize();
		return VE_secrpi3(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int eVecObj::cosecrpi3(const iVecObj& P, int q)
	{
		enforceSize();
		return VE_cosecrpi3(vx_vect,P.vx_vect,ui_size,q);
	}
#endif //IVECOBJ_H

#ifdef LIVECOBJ_H
	inline void eVecObj::accVLI(const liVecObj& X)
	{
		enforceSize();
		VE_accVLI(vx_vect,X.vx_vect,ui_size);
	}

	inline void eVecObj::LItoE(const liVecObj& X)
	{
		enforceSize();
		V_LItoE(vx_vect,X.vx_vect,ui_size);
	}

	inline int liVecObj::roundtoLI(const eVecObj& X)
	{
		enforceSize();
		return VE_roundtoLI(vx_vect,X.vx_vect,ui_size);
	}

	inline int liVecObj::floortoLI(const eVecObj& X)
	{
		enforceSize();
		return VE_floortoLI(vx_vect,X.vx_vect,ui_size);
	}

	inline int liVecObj::ceiltoLI(const eVecObj& X)
	{
		enforceSize();
		return VE_ceiltoLI(vx_vect,X.vx_vect,ui_size);
	}

	inline int liVecObj::choptoLI(const eVecObj& X)
	{
		enforceSize();
		return VE_choptoLI(vx_vect,X.vx_vect,ui_size);
	}
#endif //LIVECOBJ_H

#ifdef QIVECOBJ_H
	inline void eVecObj::accVQI(const qiVecObj& X)
	{
		enforceSize();
		VE_accVQI(vx_vect,X.vx_vect,ui_size);
	}

	inline void eVecObj::QItoE(const qiVecObj& X)
	{
		enforceSize();
		V_QItoE(vx_vect,X.vx_vect,ui_size);
	}

	inline int qiVecObj::roundtoQI(const eVecObj& X)
	{
		enforceSize();
		return VE_roundtoQI(vx_vect,X.vx_vect,ui_size);
	}

	inline int qiVecObj::floortoQI(const eVecObj& X)
	{
		enforceSize();
		return VE_floortoQI(vx_vect,X.vx_vect,ui_size);
	}

	inline int qiVecObj::ceiltoQI(const eVecObj& X)
	{
		enforceSize();
		return VE_ceiltoQI(vx_vect,X.vx_vect,ui_size);
	}

	inline int qiVecObj::choptoQI(const eVecObj& X)
	{
		enforceSize();
		return VE_choptoQI(vx_vect,X.vx_vect,ui_size);
	}
#endif //QIVECOBJ_H

#ifdef FVECOBJ_H
#endif //FVECOBJ_H

#ifdef DVECOBJ_H
#endif //DVECOBJ_H

#ifdef CFVECOBJ_H
#endif //CFVECOBJ_H

#ifdef CDVECOBJ_H
#endif //CDVECOBJ_H

#ifdef CEVECOBJ_H
#endif //CEVECOBJ_H

#ifdef PFVECOBJ_H
#endif //PFVECOBJ_H

#ifdef PDVECOBJ_H
#endif //PDVECOBJ_H

#ifdef PEVECOBJ_H
#ifdef __BORLANDC__ //80-bit numbers supported
#endif //__BORLANDC__
#endif //PEVECOBJ_H

#ifdef _MSC_VER
}  // end of namespace OptiVec
#endif

#undef cvta
#undef cuia
#undef cta

#undef vta
#undef uia
#undef ta

#undef vt
#undef t
#undef tVector

#undef vtcplx
#undef tcplx

#undef tVecObj


#endif //__BORLANDC__
#ifdef __BORLANDC__
#pragma warn .inl
#endif

#define EVECOBJ_H
#endif //EVECOBJ_H
