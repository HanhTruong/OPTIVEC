/* ceVecObj.H

  object-oriented interface for OptiVec vector functions of type "eComplex"

  This file was written by Brian Dale, Case Western Reserve Univ.

  Copyright (c) 1996-2002 by OptiCode
  All Rights Reserved.
*/

#ifndef CEVECOBJ_H

#include <VecObj.h>
#include <uiVecObj.h>
#include <eVecObj.h>

#ifdef __BORLANDC__ //80-bit numbers defined

#define cvta const vector<eComplex>&
#define cuia const ui&
#define cta  const eComplex&

#define vta  vector<eComplex>&
#define uia  ui&
#define ta   eComplex&

#define vt   vector<eComplex>
#define t    eComplex
#define tVector ceVector

#define vtbase vector<extended>
#define tbase  extended

#define tVecObj ceVecObj
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
		{	if(size){	vx_vect= VCE_vector(size);	ui_size= size;}
			else{	vx_vect= 0;	ui_size= 0;}}

		vector(cuia size, cta fill)
		{	if(!size) //size == 0
			{	vx_vect=0;	ui_size=0;}
			else if(!(fill.Re) && !(fill.Im)) //fill == 0.0
			{	vx_vect= VCE_vector0(size);	ui_size= size;}
			else
			{	vx_vect= VCE_vector(size);	ui_size= size;	VCE_equC(vx_vect,ui_size,fill);}}

		vector(cvta init)
		{	if(init.ui_size)
			{ vx_vect= VCE_vector(init.ui_size);	ui_size= init.ui_size;	VCE_equV(vx_vect, init.vx_vect, ui_size);}
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
				vx_vect= VCE_vector(size);
				ui_size= size;}}

		void setSize(cvta X)
		{	if(!X.ui_size) free(); //can set size to zero
			if(ui_size != X.ui_size) //only allocate if different sizes
			{	if(vx_vect) V_free(vx_vect);
				vx_vect= VCE_vector(X.ui_size);
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
			VCE_equV(vx_vect, vect.vx_vect, ui_size);
			return *this;}

		vta operator=(cta scalar)
		{	enforceSize();
			VCE_equC(vx_vect, ui_size, scalar);
			return *this;}

		//this is the only really dangerous operator
		ta operator[](const int& index){return vx_vect[index];}

		vta operator+=(cvta X)
		{	enforceSize();
			VCE_accV(vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator+=(cta scalar)
		{	enforceSize();
			VCE_addC(vx_vect, vx_vect, ui_size, scalar);
			return *this;}

		vta operator*=(cvta X)
		{	enforceSize();
			VCE_mulV(vx_vect, vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator*=(cta scalar)
		{	enforceSize();
			VCE_mulC(vx_vect,vx_vect,ui_size,scalar);
			return *this;}

		vt operator+(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VCE_addV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator+(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VCE_addC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator+(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VCE_addC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator-(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VCE_subV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator-(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VCE_subC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator-(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VCE_subrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator*(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VCE_mulV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator*(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VCE_mulC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator*(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VCE_mulC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator/(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VCE_divV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator/(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VCE_divC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator/(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VCE_divrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
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
			if(size) vx_vect= VCE_vector(size);
			ui_size=size;
		}

		void vector0(ui size)
		{
			free();
			if(size) vx_vect= VCE_vector0(size);
			ui_size=size;
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Addressing single vector elements
//
		t* Pelement(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VCE_Pelement(vx_vect,n);
			//not in funcref
		}

		t element(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VCE_element(vx_vect,n);
			//not in funcref
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Initialization
//

		void equ0()
		{
			enforceSize();
			VCE_equ0(vx_vect,ui_size);
		}

		void equ1()
		{
			enforceSize();
			VCE_equ1(vx_vect,ui_size);
		}

		void equm1()
		{
			enforceSize();
			VCE_equm1(vx_vect, ui_size);
		}

		void equC(cta C)
		{
			enforceSize();
			VCE_equC(vx_vect, ui_size, C);
		}

		void equV(cvta X)
		{
			enforceSize();
			VCE_equV(vx_vect, X.vx_vect, ui_size);
		}

		void x_equV(cvta X, cta A, cta B)
		{
			enforceSize();
			VCEx_equV(vx_vect, X.vx_vect, ui_size, A, B);
		}

		void ramp(cta Start, cta Rise)
		{
			enforceSize();
			VCE_ramp(vx_vect, ui_size, Start, Rise);
		}

		//CtoReIm member of eVecObj

		void ReImtoC(const vtbase& Re, const vtbase& Im)
		{
			enforceSize();
			VE_ReImtoC(vx_vect, Re.vx_vect, Im.vx_vect, ui_size);
		}

		//CtoRe member of eVecObj

		void RetoC(const vtbase& Re)
		{
			enforceSize();
			VE_RetoC(vx_vect, Re.vx_vect, ui_size);
		}

		//CtoIm member of eVecObj

		void ImtoC(const vtbase& Im)
		{
			enforceSize();
			VE_ImtoC(vx_vect, Im.vx_vect, ui_size);
		}

		//CtoPolar member of eVecObj

		void PolartoC(const vtbase& Mag, const vtbase& Arg)
		{
			enforceSize();
			VE_PolartoC(vx_vect, Mag.vx_vect, Arg.vx_vect, ui_size);
		}

                void normtoC(cvta X)
		{
			enforceSize();
                        VCE_normtoC(vx_vect, X.vx_vect, ui_size);
		}

		//CtoNorm member of eVecObj
		//CtoAbs member of eVecObj
		//CtoArg member of eVecObj

		//abs member of eVecObj
		//real member of eVecObj
		//imag member of eVecObj
		//arg member of eVecObj
		//norm member of eVecObj

		void polar(const vtbase& Mag, const vtbase& Arg){ PolartoC(Mag,Arg); }

		void complex(const vtbase& Re, const vtbase& Im){ ReImtoC(Re,Im); }

		//defined by including peVecObj.h
		void PtoC(const peVecObj& X);


/////////////////////////////////////////////////////////////////////////////////////////
//Data-type interconversions
//
		//defined by including cfVecObj.h
		void CFtoCE(const cfVecObj& X);

		//defined by including cdVecObj.h
		void CDtoCE(const cdVecObj& X);


/////////////////////////////////////////////////////////////////////////////////////////
//Index-oriented manipulations
//
		void reflect()
		{
			enforceSize();
			VCE_reflect(vx_vect, ui_size);
		}

		void _delete(ui pos)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VCE_delete(vx_vect, ui_size, pos);
		}

		void insert(ui pos, cta C)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VCE_insert(vx_vect, ui_size, pos, C);
		}

		void rev(cvta X)
		{
			enforceSize();
			VCE_rev(vx_vect, X.vx_vect, ui_size);
		}

#ifdef V_HUGE
		void rotate( cvta X, long pos )
#else
		void rotate( cvta X, int pos )
#endif
		{
			enforceSize();
			VCE_rotate(vx_vect, X.vx_vect, ui_size, pos);
		}

		void indpick(const vector<ui>& Ind, cvta X);
		void indput(cvta X, const vector<ui>& Ind);

		void subvector(cvta X, int step, ui start=0)
		{
			enforceSize();
			if(vx_vect==X.vx_vect) throw OptiVec_Err();
			VCE_subvector(vx_vect, ui_size, X.vx_vect+start, step);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Functions of a sub-set of elements
//
		void subvector_equC(const unsigned& samp, cta C)
		{
			enforceSize();
			VCE_subvector_equC(vx_vect, ((ui_size-1)/samp)+1, samp, C);
		}

		void subvector_equV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VCE_subvector_equV(vx_vect, ((ui_size-1)/samp)+1, samp, X.vx_vect);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//One-Dimensional Vector Operations
//

		tbase absmax()
		{
			enforceSize();
			return VCE_absmax(vx_vect, ui_size);
		}

		tbase absmin()
		{
			enforceSize();
			return VCE_absmin(vx_vect, ui_size);
		}

		tbase absmaxind(ui* Ind)
		{
			enforceSize();
			return VCE_absmaxind(Ind, vx_vect, ui_size);
		}

		tbase absminind(ui* Ind)
		{
			enforceSize();
			return VCE_absminind(Ind, vx_vect, ui_size);
		}

		t maxReIm()
		{
			enforceSize();
			return VCE_maxReIm(vx_vect, ui_size);
		}

		t minReIm()
		{
			enforceSize();
			return VCE_minReIm(vx_vect, ui_size);
		}

		t absmaxReIm()
		{
			enforceSize();
			return VCE_absmaxReIm(vx_vect, ui_size);
		}

		t absminReIm()
		{
			enforceSize();
			return VCE_absminReIm(vx_vect, ui_size);
		}

		t cabsmax()
		{
			enforceSize();
			return VCE_cabsmax(vx_vect, ui_size);
		}

		t cabsmin()
		{
			enforceSize();
			return VCE_cabsmin(vx_vect, ui_size);
		}

		t sabsmax()
		{
			enforceSize();
			return VCE_sabsmax(vx_vect, ui_size);
		}

		t sabsmin()
		{
			enforceSize();
			return VCE_sabsmin(vx_vect, ui_size);
		}

		t sum()
		{
			enforceSize();
			return VCE_sum(vx_vect, ui_size);
		}

		t prod()
		{
			enforceSize();
			return VCE_prod(vx_vect, ui_size);
		}

		t mean()
		{
			enforceSize();
			return VCE_mean(vx_vect, ui_size);
		}

		t meanwW(const vtbase& Wt)
		{
			enforceSize();
			return VCE_meanwW(vx_vect, Wt.vx_vect, ui_size);
		}

		t scalprod(cvta X)
		{
			enforceSize();
			return VCE_scalprod(vx_vect, X.vx_vect, ui_size);
		}

		void runsum(cvta X)
		{
			enforceSize();
			VCE_runsum(vx_vect, X.vx_vect, ui_size);
		}

		void runprod(cvta X)
		{
			enforceSize();
			VCE_runprod(vx_vect, X.vx_vect, ui_size);
		}

		int iselementC(cta C)
		{
			enforceSize();
			return VCE_iselementC(vx_vect, ui_size, C);
		}

		ui iselementV(cvta X, cvta Tab)
		{
			enforceSize();
			Tab.enforceSize();
			return VCE_iselementV(vx_vect, X.vx_vect,
				ui_size, Tab.vx_vect, Tab.ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Statistical Functions and Building Blocks
//


/////////////////////////////////////////////////////////////////////////////////////////
//Fourier Transforms, Convolutions, Filtering
//
		void l_FFT(cvta X, int dir=1)
		{
			enforcePow2();
			VCEl_FFT(vx_vect,X.vx_vect,ui_size,dir);
		}

		void l_FFTtoC(const vtbase& X)
		{
			enforceSize();
			VEl_FFTtoC(vx_vect,X.vx_vect,ui_size);
		}

		void l_filter(cvta X, cvta Flt)
		{
			enforcePow2();
			VCEl_filter(vx_vect, X.vx_vect, Flt.vx_vect, ui_size);
		}

		void s_FFT(cvta X, int dir=1)
		{
			enforcePow2();
			VCEs_FFT(vx_vect,X.vx_vect,ui_size,dir);
		}

		void s_FFTtoC(const vtbase& X)
		{
			enforceSize();
			VEs_FFTtoC(vx_vect,X.vx_vect,ui_size);
		}

		void s_filter(cvta X, cvta Flt)
		{
			enforcePow2();
			VCEs_filter(vx_vect, X.vx_vect, Flt.vx_vect, ui_size);
		}

		void FFT(cvta X, int dir=1)
		{
			enforcePow2();
			VCE_FFT(vx_vect,X.vx_vect,ui_size,dir);
		}

		void FFTtoC(const vtbase& X)
		{
			enforceSize();
			VE_FFTtoC(vx_vect,X.vx_vect,ui_size);
		}

		void filter(cvta X, cvta Flt)
		{
			enforcePow2();
			VCE_filter(vx_vect, X.vx_vect, Flt.vx_vect, ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Analysis
//


/////////////////////////////////////////////////////////////////////////////////////////
//Geometrical Vector Arithmetics
//


/////////////////////////////////////////////////////////////////////////////////////////
//Input and Output
//
		void fprint(FILE* stream, const unsigned& nperline, const unsigned& linewidth)
		{
			enforceSize();
			VCE_fprint(stream, vx_vect, ui_size, nperline, linewidth);
		}

#if !defined _Windows || defined __FLAT__ || defined _WIN32
	    void  cprint( const unsigned& nperline )
		{
			enforceSize();
			VCE_cprint( vx_vect, ui_size, nperline);
		}
#endif

		void print(const unsigned& npl)
		{
			enforceSize();
			VCE_print(vx_vect, ui_size, npl);
		}

		void write(FILE* stream)
		{
			enforceSize();
			VCE_write(stream, vx_vect, ui_size);
		}

		void read(FILE* stream)
		{
			enforceSize();
			VCE_read(vx_vect, ui_size, stream);
		}

//nwrite
//nread

		void setWriteFormat(char* formatString) const
		{
			VCE_setWriteFormat(formatString);
		}

		void setWriteSeparate(char* sepString) const
		{
			VCE_setWriteSeparate(sepString);
		}

//setNWriteSeparate

		void store(FILE* stream) const
		{
			enforceSize();
			fwrite(&ui_size,sizeof(ui),1,stream);
			VCE_store(stream,vx_vect,ui_size);
		}

		void recall(FILE* stream)
		{
			ui sz;
			fread(&sz,sizeof(ui),1,stream);
			setSize(sz);
			VCE_recall(vx_vect,ui_size,stream);
		}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//from VXmath.h
//


/////////////////////////////////////////////////////////////////////////////////////////
//Rounding
//


/////////////////////////////////////////////////////////////////////////////////////////
//Comparisons
//
		ui cmp_eq0(cvta X)
		{
			enforceSize();
			return VCE_cmp_eq0(vx_vect,X.vx_vect,ui_size);
		}

		ui cmp_ne0(cvta X)
		{
			enforceSize();
			return VCE_cmp_ne0(vx_vect,X.vx_vect,ui_size);
		}


		ui cmp_eqC(cvta X, cta C)
		{
			enforceSize();
			return VCE_cmp_eqC(vx_vect,X.vx_vect,ui_size,C);
		}

		ui cmp_neC(cvta X, cta C)
		{
			enforceSize();
			return VCE_cmp_neC(vx_vect,X.vx_vect,ui_size,C);
		}

		ui cmp_eqV(cvta X, cvta Y)
		{
			enforceSize();
			return VCE_cmp_eqV(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
		}

		ui cmp_neV(cvta X, cvta Y)
		{
			enforceSize();
			return VCE_cmp_neV(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
		}

		//indexed comparisons are members of uiVecObj


/////////////////////////////////////////////////////////////////////////////////////////
//Standard Arithmetics or Basic Arithmetics
//
		void limit(cvta X, cta Min, cta Max)
		{
			enforceSize();
			VCE_limit( vx_vect, X.vx_vect, ui_size, Min, Max);
		}

		void flush0(cvta X, cta AbsMin)
		{
			enforceSize();
			VCE_flush0( vx_vect, X.vx_vect, ui_size, AbsMin);
		}

		void flushInv(cvta X, cta AbsMin)
		{
			enforceSize();
			VCE_flushInv(vx_vect, X.vx_vect, ui_size, AbsMin);
		}

		//x_equV defined above

		void addC(cvta X, cta C)
		{
			enforceSize();
			VCE_addC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subC(cvta X, cta C)
		{
			enforceSize();
			VCE_subC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subrC(cvta X, cta C)
		{
			enforceSize();
			VCE_subrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void mulC(cvta X, cta C)
		{
			enforceSize();
			VCE_mulC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divC(cvta X, cta C)
		{
			enforceSize();
			VCE_divC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divrC(cvta X, cta C)
		{
			enforceSize();
			VCE_divrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void x_divrC(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			VCEx_divrC(vx_vect, X.vx_vect, ui_size, A, B, C);
		}

		void addReC(cvta X, const tbase& C)
		{
			enforceSize();
			VCE_addReC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subReC(cvta X, const tbase& C)
		{
			enforceSize();
			VCE_subReC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subrReC(cvta X, const tbase& C)
		{
			enforceSize();
			VCE_subrReC(vx_vect, X.vx_vect, ui_size, C);
		}

		void mulReC(cvta X, const tbase& C)
		{
			enforceSize();
			VCE_addReC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divReC(cvta X, const tbase& C)
		{
			enforceSize();
			VCE_addReC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divrReC(cvta X, const tbase& C)
		{
			enforceSize();
			VCE_addReC(vx_vect, X.vx_vect, ui_size, C);
		}

		void addV(cvta X, cvta Y)
		{
			enforceSize();
			VCE_addV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subV(cvta X, cvta Y)
		{
			enforceSize();
			VCE_subV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subrV(cvta X, cvta Y)
		{
			enforceSize();
			VCE_subrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void mulV(cvta X, cvta Y)
		{
			enforceSize();
			VCE_mulV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divV(cvta X, cvta Y)
		{
			enforceSize();
			VCE_divV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divrV(cvta X, cvta Y)
		{
			enforceSize();
			VCE_divrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void addReV(cvta X, vtbase Y)
		{
			enforceSize();
			VCE_addReV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subReV(cvta X, vtbase Y)
		{
			enforceSize();
			VCE_subReV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subrReV(cvta X, vtbase Y)
		{
			enforceSize();
			VCE_subrReV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void mulReV(cvta X, vtbase Y)
		{
			enforceSize();
			VCE_mulReV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divReV(cvta X, vtbase Y)
		{
			enforceSize();
			VCE_divReV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divrReV(cvta X, vtbase Y)
		{
			enforceSize();
			VCE_divrReV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

                void mulVconj(cvta X, cvta Y)
		{
			enforceSize();
                        VCE_mulVconj(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void x_addV(cvta X, cvta Y, cta A, cta B)
		{
			enforceSize();
			VCEx_addV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void x_subV(cvta X, cvta Y, cta A, cta B)
		{
			enforceSize();
			VCEx_subV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void x_subrV(cvta X, cvta Y, cta A, cta B)
		{
			enforceSize();
			VCEx_subrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void x_mulV(cvta X, cvta Y, cta A, cta B)
		{
			enforceSize();
			VCEx_mulV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void x_divV(cvta X, cvta Y, cta A, cta B)
		{
			enforceSize();
			VCEx_divV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void x_divrV(cvta X, cvta Y, cta A, cta B)
		{
			enforceSize();
			VCEx_divrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void x_addReV(cvta X, vtbase Y, cta A, cta B)
		{
			enforceSize();
			VCEx_addReV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void x_subReV(cvta X, vtbase Y, cta A, cta B)
		{
			enforceSize();
			VCEx_subReV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void x_subrReV(cvta X, vtbase Y, cta A, cta B)
		{
			enforceSize();
			VCEx_subrReV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void x_mulReV(cvta X, vtbase Y, cta A, cta B)
		{
			enforceSize();
			VCEx_mulReV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void x_divReV(cvta X, vtbase Y, cta A, cta B)
		{
			enforceSize();
			VCEx_divReV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void x_divrReV(cvta X, vtbase Y, cta A, cta B)
		{
			enforceSize();
			VCEx_divrReV(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

                void x_mulVconj(cvta X, cvta Y, cta A, cta B)
		{
			enforceSize();
                        VCEx_mulVconj(vx_vect, X.vx_vect, Y.vx_vect, ui_size, A, B);
		}

		void lincomb(cvta X, cvta Y, cta CX, cta CY)
		{
			enforceSize();
			VCE_lincomb( vx_vect, X.vx_vect, Y.vx_vect, ui_size, CX, CY);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Accumulation
//
		void accV(cvta X)
		{
			enforceSize();
			VCE_accV(vx_vect,X.vx_vect,ui_size);
		}

		//defined by including cfVecObj.h
		void accVCF(const cfVecObj& X);

		//defined by including cdVecObj.h
		void accVCE(const cdVecObj& X);


/////////////////////////////////////////////////////////////////////////////////////////
//Bit-wise operations
//


/////////////////////////////////////////////////////////////////////////////////////////
//Functions of a sub-set of elements
//
		//subvector_equC, subvector_equV defined above

		void subvector_addC(const unsigned& samp, cta C)
		{
			enforceSize();
			VCE_subvector_addC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_subC(const unsigned& samp, cta C)
		{
			enforceSize();
			VCE_subvector_subC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_subrC(const unsigned& samp, cta C)
		{
			enforceSize();
			VCE_subvector_subrC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_mulC(const unsigned& samp, cta C)
		{
			enforceSize();
			VCE_subvector_mulC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_divC(const unsigned& samp, cta C)
		{
			enforceSize();
			VCE_subvector_divC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_divrC(const unsigned& samp, cta C)
		{
			enforceSize();
			VCE_subvector_divrC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_addV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VCE_subvector_addV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void subvector_subV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VCE_subvector_subV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void subvector_subrV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VCE_subvector_subrV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void subvector_mulV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VCE_subvector_mulV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void subvector_divV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VCE_subvector_divV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void subvector_divrV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VCE_subvector_divrV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Mathematical Functions
//
		int neg(cvta X)
		{
			enforceSize();
			return VCE_neg(vx_vect, X.vx_vect, ui_size);
		}

		int conj(cvta X)
		{
			enforceSize();
			return VCE_conj(vx_vect,X.vx_vect,ui_size);
		}

		//abs, normtoC defined above

		int inv(cvta X)
		{
			enforceSize();
			return VCE_inv(vx_vect, X.vx_vect, ui_size);
		}

		int x_inv(cvta X, cta A, cta B)
		{
			enforceSize();
			return VCEx_inv(vx_vect, X.vx_vect, ui_size, A, B);
		}

		int square(cvta X)
		{
			enforceSize();
			return VCE_square(vx_vect,X.vx_vect,ui_size);
		}

		int x_square(cvta X, cta A, cta B)
		{
			enforceSize();
			return VCEx_square(vx_vect,X.vx_vect,ui_size,A,B);
		}

		int cubic(cvta X)
		{
			enforceSize();
			return VCE_cubic(vx_vect,X.vx_vect,ui_size);
		}

		int x_cubic(cvta X, cta A, cta B)
		{
			enforceSize();
			return VCEx_cubic(vx_vect,X.vx_vect,ui_size,A,B);
		}

		int quartic(cvta X)
		{
			enforceSize();
			return VCE_quartic(vx_vect,X.vx_vect,ui_size);
		}

		int x_quartic(cvta X, cta A, cta B)
		{
			enforceSize();
			return VCEx_quartic(vx_vect,X.vx_vect,ui_size,A,B);
		}

		int ipow(cvta X, int pow)
		{
			enforceSize();
			return VCE_ipow(vx_vect, X.vx_vect, ui_size, pow);
		}

		int x_ipow(cvta X, int pow, cta A, cta B, cta C)
		{
			enforceSize();
			return VCEx_ipow(vx_vect, X.vx_vect, ui_size, pow, A, B, C);
		}

		int u_square(cvta X)
		{
			enforceSize();
			return VCEu_square(vx_vect,X.vx_vect,ui_size);
		}

		int ux_square(cvta X, cta A, cta B)
		{
			enforceSize();
			return VCEux_square(vx_vect,X.vx_vect,ui_size,A,B);
		}

		int u_cubic(cvta X)
		{
			enforceSize();
			return VCEu_cubic(vx_vect,X.vx_vect,ui_size);
		}

		int ux_cubic(cvta X, cta A, cta B)
		{
			enforceSize();
			return VCEux_cubic(vx_vect,X.vx_vect,ui_size,A,B);
		}

		int u_quartic(cvta X)
		{
			enforceSize();
			return VCEu_quartic(vx_vect,X.vx_vect,ui_size);
		}

		int ux_quartic(cvta X, cta A, cta B)
		{
			enforceSize();
			return VCEux_quartic(vx_vect,X.vx_vect,ui_size,A,B);
		}

		int u_ipow(cvta X, int pow)
		{
			enforceSize();
			return VCEu_ipow(vx_vect, X.vx_vect, ui_size, pow);
		}

		int ux_ipow(cvta X, int pow, cta A, cta B, cta C)
		{
			enforceSize();
			return VCEux_ipow(vx_vect, X.vx_vect, ui_size, pow, A, B, C);
		}

		int pow(cvta X, cta expo)
		{
			enforceSize();
			return VCE_pow(vx_vect,X.vx_vect,ui_size,expo);
		}

		int x_pow(cvta X, cta expo, cta A, cta B, cta C)
		{
			enforceSize();
			return VCEx_pow(vx_vect,X.vx_vect,ui_size,expo,A,B,C);
		}

		int powReExpo(cvta X, const tbase& expo)
		{
			enforceSize();
			return VCE_powReExpo(vx_vect,X.vx_vect,ui_size,expo);
		}

		int x_powReExpo(cvta X, const tbase& expo, cta A, cta B, cta C)
		{
			enforceSize();
			return VCEx_powReExpo(vx_vect,X.vx_vect,ui_size,expo,A,B,C);
		}

		int sqrt(cvta X)
		{
			enforceSize();
			return VCE_sqrt(vx_vect,X.vx_vect,ui_size);
		}

		int x_sqrt(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VCEx_sqrt(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int log(cvta X)
		{
			enforceSize();
			return VCE_log(vx_vect,X.vx_vect,ui_size);
		}

		int log2(cvta X)
		{
			enforceSize();
			return VCE_log2(vx_vect,X.vx_vect,ui_size);
		}

		int log10(cvta X)
		{
			enforceSize();
			return VCE_log10(vx_vect,X.vx_vect,ui_size);
		}

		int ln(cvta X)
		{
			return log(X);
		}

		int x_log(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VCEx_log(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int x_log2(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VCEx_log2(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int x_log10(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VCEx_log10(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int x_ln(cvta X, cta A, cta B, cta C)
		{
			return x_log(X,A,B,C);
		}

		//defined by including peVecObj.h
		int logtoC(const peVecObj& X);

		//defined by including peVecObj.h
		int log2toC(const peVecObj& X);

		//defined by including peVecObj.h
		int log10toC(const peVecObj& X);

		int lntoC(const peVecObj& X){ return logtoC(X); }

		int exp(cvta X)
		{
			enforceSize();
			return VCE_exp(vx_vect,X.vx_vect,ui_size);
		}

		int x_exp(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VCEx_exp(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int expArbBase(cvta X, cta Base)
		{
			enforceSize();
			return VCE_expArbBase(vx_vect,X.vx_vect,ui_size,Base);
		}

		int x_expArbBase(cvta X, cta Base, cta A, cta B, cta C)
		{
			enforceSize();
			return VCEx_expArbBase(vx_vect,X.vx_vect,ui_size,Base,A,B,C);
		}

		int sin(cvta X)
		{
			enforceSize();
			return VCE_sin(vx_vect,X.vx_vect,ui_size);
		}

		int x_sin(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VCEx_sin(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int cos(cvta X)
		{
			enforceSize();
			return VCE_cos(vx_vect,X.vx_vect,ui_size);
		}

		int x_cos(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VCEx_cos(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int tan(cvta X)
		{
			enforceSize();
			return VCE_tan(vx_vect,X.vx_vect,ui_size);
		}

		int x_tan(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VCEx_tan(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int asin(cvta X)
		{
			enforceSize();
			return VCE_asin(vx_vect,X.vx_vect,ui_size);
		}

		int x_asin(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VCEx_asin(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int acos(cvta X)
		{
			enforceSize();
			return VCE_acos(vx_vect,X.vx_vect,ui_size);
		}

		int x_acos(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VCEx_acos(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int atan(cvta X)
		{
			enforceSize();
			return VCE_atan(vx_vect,X.vx_vect,ui_size);
		}

		int x_atan(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VCEx_atan(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int sinh(cvta X)
		{
			enforceSize();
			return VCE_sinh(vx_vect,X.vx_vect,ui_size);
		}

		int x_sinh(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VCEx_sinh(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int cosh(cvta X)
		{
			enforceSize();
			return VCE_cosh(vx_vect,X.vx_vect,ui_size);
		}

		int x_cosh(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VCEx_cosh(vx_vect,X.vx_vect,ui_size,A,B,C);
		}

		int tanh(cvta X)
		{
			enforceSize();
			return VCE_tanh(vx_vect,X.vx_vect,ui_size);
		}

		int x_tanh(cvta X, cta A, cta B, cta C)
		{
			enforceSize();
			return VCEx_tanh(vx_vect,X.vx_vect,ui_size,A,B,C);
		}


            #ifdef __VGRAPH_H
                void autoPlot( unsigned form, COLORREF color )
                {
                    enforceSize();
                    VCE_autoPlot( vx_vect, ui_size, form, color );
                }

                void _2AutoPlot( unsigned form1, COLORREF color1,
                       cvta X2, unsigned form2, COLORREF color2 )
                {
                    enforceSize();
                    X2.enforceSize();
                    VCE_2AutoPlot( vx_vect,    ui_size, form1, color1,
                                X2.vx_vect, X2.ui_size, form2, color2 );
                }

                void dataPlot( unsigned form, COLORREF color )
                {
                    enforceSize();
                    VCE_dataPlot( vx_vect, ui_size, form, color );
                }
            #endif // __VGRAPH_H


	};


	inline ui uiVecObj::cmp_eq0ind(cvta X)
	{
		enforceSize();
		return VCE_cmp_eq0ind(vx_vect,X.vx_vect,ui_size);
	}

	inline ui uiVecObj::cmp_ne0ind(cvta X)
	{
		enforceSize();
		return VCE_cmp_ne0ind(vx_vect,X.vx_vect,ui_size);
	}

	inline ui uiVecObj::cmp_eqCind(cvta X, cta C)
	{
		enforceSize();
		return VCE_cmp_eqCind(vx_vect,X.vx_vect,ui_size,C);
	}

	inline ui uiVecObj::cmp_neCind(cvta X, cta C)
	{
		enforceSize();
		return VCE_cmp_neCind(vx_vect,X.vx_vect,ui_size,C);
	}

	inline ui uiVecObj::cmp_eqVind(cvta X, cvta Y)
	{
		enforceSize();
		return VCE_cmp_eqVind(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
	}

	inline ui uiVecObj::cmp_neVind(cvta X, cvta Y)
	{
		enforceSize();
		return VCE_cmp_neVind(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
	}

#ifdef UBVECOBJ_H
#endif //UBVECOBJ_H

#ifdef USVECOBJ_H
#endif //USVECOBJ

#ifdef UVECOBJ_H
#endif //UVECOBJ_H

#ifdef ULVECOBJ_H
#endif //ULVECOBJ_H

#ifdef BIVECOBJ_H
#endif //BIVECOBJ_H

#ifdef SIVECOBJ_H
#endif //SIVECOBJ_H

#ifdef IVECOBJ_H
#endif //IVECOBJ_H

#ifdef LIVECOBJ_H
#endif //LIVECOBJ_H

#ifdef QIVECOBJ_H
#endif //QIVECOBJ_H

#ifdef FVECOBJ_H
#endif //FVECOBJ_H

#ifdef DVECOBJ_H
#endif //DVECOBJ_H

#ifdef EVECOBJ_H
#ifdef __BORLANDC__ //80-bit numbers supported
	inline void vtbase::CtoReIm(vtbase* Im,cvta X)
	{
		enforceSize();
		if(ui_size != Im->ui_size) throw OptiVec_SizeErr();
		VE_CtoReIm(vx_vect, Im->vx_vect, X.vx_vect, ui_size);
	}

        inline void vtbase::CtoReIm(vtbase Im,cvta X)
	{
		enforceSize();
                if(ui_size != Im.ui_size) throw OptiVec_SizeErr();
                VE_CtoReIm(vx_vect, Im.vx_vect, X.vx_vect, ui_size);
	}

	inline void vtbase::CtoRe(cvta cplx)
	{
		enforceSize();
		VE_CtoRe(vx_vect, cplx.vx_vect, ui_size);
	}

	inline void vtbase::CtoIm(cvta cplx)
	{
		enforceSize();
		VE_CtoIm(vx_vect, cplx.vx_vect, ui_size);
	}

	inline void vtbase::CtoPolar(vtbase* Arg, cvta X)
	{
		enforceSize();
		if(ui_size != Arg->ui_size) throw OptiVec_SizeErr();
		VE_CtoReIm(vx_vect, Arg->vx_vect, X.vx_vect, ui_size);
	}

        inline void vtbase::CtoPolar(vtbase Arg, cvta X)
	{
		enforceSize();
                if(ui_size != Arg.ui_size) throw OptiVec_SizeErr();
                VE_CtoReIm(vx_vect, Arg.vx_vect, X.vx_vect, ui_size);
	}

	inline void vtbase::CtoNorm(cvta cplx)
	{
		enforceSize();
		VE_CtoNorm(vx_vect, cplx.vx_vect, ui_size);
	}

	inline void vtbase::CtoAbs(cvta cplx)
	{
		enforceSize();
		VE_CtoAbs(vx_vect, cplx.vx_vect, ui_size);
	}

	inline void vtbase::CtoArg(cvta cplx)
	{
		enforceSize();
		VE_CtoArg(vx_vect, cplx.vx_vect, ui_size);
	}

	inline int vtbase::abs(cvta cplx)
	{
		enforceSize();
		return VCE_abs(vx_vect, cplx.vx_vect, ui_size);
	}
#endif //__BORLANDC__
#endif //EVECOBJ_H

#ifdef CFVECOBJ_H
	inline void cfVecObj::CEtoCF(const ceVecObj& X)
	{
		enforceSize();
		V_CEtoCF(vx_vect,X.vx_vect,ui_size);
	}

	inline void ceVecObj::CFtoCE(const cfVecObj& X)
	{
		enforceSize();
		V_CFtoCE(vx_vect,X.vx_vect,ui_size);
	}
#endif //CFVECOBJ_H

#ifdef CDVECOBJ_H
	inline void cdVecObj::CEtoCD(const ceVecObj& X)
	{
		enforceSize();
		V_CEtoCD(vx_vect,X.vx_vect,ui_size);
	}

	inline void ceVecObj::CDtoCE(const cdVecObj& X)
	{
		enforceSize();
		V_CDtoCE(vx_vect,X.vx_vect,ui_size);
	}
#endif //CDVECOBJ_H

#ifdef PFVECOBJ_H
#endif //PFVECOBJ_H

#ifdef PDVECOBJ_H
#endif //PDVECOBJ_H

#ifdef PEVECOBJ_H
#ifdef __BORLANDC__ //80-bit numbers supported
	inline void peVecObj::CtoP(const ceVecObj& X)
	{
		enforceSize();
		VE_CtoP(vx_vect,X.vx_vect,ui_size);
	}

	inline void ceVecObj::PtoC(const peVecObj& X)
	{
		enforceSize();
		VE_PtoC(vx_vect,X.vx_vect,ui_size);
	}

	inline int ceVecObj::logtoC(const peVecObj& X)
	{
		enforceSize();
		VPE_logtoC(vx_vect,X.vx_vect,ui_size);
	}

	inline int ceVecObj::log2toC(const peVecObj& X)
	{
		enforceSize();
		VPE_log2toC(vx_vect,X.vx_vect,ui_size);
	}

	inline int ceVecObj::log10toC(const peVecObj& X)
	{
		enforceSize();
		VPE_log10toC(vx_vect,X.vx_vect,ui_size);
	}

	inline int peVecObj::exptoP(const ceVecObj& X)
	{
		enforceSize();
                VCE_exptoP(vx_vect,X.vx_vect,ui_size);
	}
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

#undef vtbase
#undef tbase

#undef tVecObj

#endif //__BORLANDC__
#ifdef __BORLANDC__
#pragma warn .inl
#endif

#define CEVECOBJ_H
#endif //CEVECOBJ_H
