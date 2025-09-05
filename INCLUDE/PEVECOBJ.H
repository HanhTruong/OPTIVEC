/* peVecObj.H

  object-oriented interface for OptiVec vector functions of type "ePolar"

  This file was written by Brian Dale, Case Western Reserve Univ.

  Copyright (c) 1996-2002 by OptiCode
  All Rights Reserved.
*/

#ifndef PEVECOBJ_H

#include <VecObj.h>
#include <uiVecObj.h>
#include <eVecObj.h>

#ifdef __BORLANDC__ //80-bit numbers defined

#define cvta const vector<ePolar>&
#define cuia const ui&
#define cta  const ePolar&

#define vta  vector<ePolar>&
#define uia  ui&
#define ta   ePolar&

#define vt   vector<ePolar>
#define t    ePolar
#define tVector peVector

#define vtbase vector<extended>
#define tbase  extended

#define tVecObj peVecObj
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
		{	if(size){	vx_vect= VPE_vector(size);	ui_size= size;}
			else{	vx_vect= 0;	ui_size= 0;}}

		vector(cuia size, cta fill)
		{	if(!size) //size == 0
			{	vx_vect=0;	ui_size=0;}
			else if(!(fill.Mag) && !(fill.Arg)) //fill == 0.0
			{	vx_vect= VPE_vector0(size);	ui_size= size;}
			else
			{	vx_vect= VPE_vector(size);	ui_size= size;	VPE_equC(vx_vect,ui_size,fill);}}

		vector(cvta init)
		{	if(init.ui_size)
			{ vx_vect= VPE_vector(init.ui_size);	ui_size= init.ui_size;	VPE_equV(vx_vect, init.vx_vect, ui_size);}
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
				vx_vect= VPE_vector(size);
				ui_size= size;}}

		void setSize(cvta X)
		{	if(!X.ui_size) free(); //can set size to zero
			if(ui_size != X.ui_size) //only allocate if different sizes
			{	if(vx_vect) V_free(vx_vect);
				vx_vect= VPE_vector(X.ui_size);
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
			VPE_equV(vx_vect, vect.vx_vect, ui_size);
			return *this;}

		vta operator=(cta scalar)
		{	enforceSize();
			VPE_equC(vx_vect, ui_size, scalar);
			return *this;}

		//this is the only really dangerous operator
		ta operator[](const int& index){return vx_vect[index];}

		vta operator*=(cvta X)
		{	enforceSize();
			VPE_mulV(vx_vect, vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator*=(cta scalar)
		{	enforceSize();
			VPE_mulC(vx_vect,vx_vect,ui_size,scalar);
			return *this;}

		vt operator*(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VPE_mulV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator*(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VPE_mulC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator*(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VPE_mulC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator/(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VPE_divV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator/(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VPE_divC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator/(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VPE_divrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
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
			if(size) vx_vect= VPE_vector(size);
			ui_size=size;
		}

		void vector0(ui size)
		{
			free();
			if(size) vx_vect= VPE_vector0(size);
			ui_size=size;
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Addressing single vector elements
//
		t* Pelement(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VPE_Pelement(vx_vect,n);
			//not in funcref
		}

		t element(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VPE_element(vx_vect,n);
			//not in funcref
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Initialization
//
		void equ0()
		{
			enforceSize();
			VPE_equ0(vx_vect,ui_size);
		}

		void equ1()
		{
			enforceSize();
			VPE_equ1(vx_vect,ui_size);
		}

		void equm1()
		{
			enforceSize();
			VPE_equm1(vx_vect, ui_size);
		}

		void equC(cta C)
		{
			enforceSize();
			VPE_equC(vx_vect, ui_size, C);
		}

		void equV(cvta X)
		{
			enforceSize();
			VPE_equV(vx_vect, X.vx_vect, ui_size);
		}

		void principal(cvta X)
		{
			enforceSize();
			VPE_principal(vx_vect, X.vx_vect, ui_size);
		}

		//PtoMagArg member of eVecObj

		void MagArgtoP(const vtbase& Mag, const vtbase& Arg)
		{
			enforceSize();
			VE_MagArgtoP(vx_vect, Mag.vx_vect, Arg.vx_vect, ui_size);
		}

		void MagArgtoPrincipal(const vtbase& Mag, const vtbase& Arg)
		{
			enforceSize();
			VE_MagArgtoPrincipal(vx_vect, Mag.vx_vect, Arg.vx_vect, ui_size);
		}

		//PtoMag member of eVecObj

		void MagtoP(const vtbase& Mag)
		{
			enforceSize();
			VE_MagtoP(vx_vect, Mag.vx_vect, ui_size);
		}

		//PtoArg member of eVecObj

		void ArgtoP(const vtbase& Arg)
		{
			enforceSize();
			VE_ArgtoP(vx_vect, Arg.vx_vect, ui_size);
		}

		//defined by including ceVecObj.h
		void CtoP(const ceVecObj&);

		//PtoC member of ceVecObj

		//PtoNorm member of eVecObj
		//PtoReIm member of eVecObj
		
		void ReImtoP(const vtbase& Re, const vtbase& Im)
		{
			enforceSize();
			VE_ReImtoP(vx_vect, Re.vx_vect, Im.vx_vect, ui_size);
		}

		//PtoRe member of eVecObj
		//PtoIm member of eVecObj
		//abs member of eVecObj


/////////////////////////////////////////////////////////////////////////////////////////
//Data-type interconversions
//
		//defined by including pfVecObj.h
		void PFtoPE(const pfVecObj& X);

		//defined by including pdVecObj.h
		void PDtoPE(const pdVecObj& X);


/////////////////////////////////////////////////////////////////////////////////////////
//Index-oriented manipulations
//
		void reflect()
		{
			enforceSize();
			VPE_reflect(vx_vect, ui_size);
		}

		void _delete(ui pos)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VPE_delete(vx_vect, ui_size, pos);
		}

		void insert(ui pos, cta C)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VPE_insert(vx_vect, ui_size, pos, C);
		}

		void rev(cvta X)
		{
			enforceSize();
			VPE_rev(vx_vect, X.vx_vect, ui_size);
		}

#ifdef V_HUGE
		void rotate( cvta X, long pos )
#else
		void rotate( cvta X, int pos )
#endif
		{
			enforceSize();
			VPE_rotate(vx_vect, X.vx_vect, ui_size, pos);
		}

		void indpick(const vector<ui>& Ind, cvta X);
		void indput(cvta X, const vector<ui>& Ind);

		void subvector(cvta X, int step, ui start=0)
		{
			enforceSize();
			if(vx_vect==X.vx_vect) throw OptiVec_Err();
			VPE_subvector(vx_vect, ui_size, X.vx_vect+start, step);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Functions of a sub-set of elements
//
		void subvector_equC(const unsigned& samp, cta C)
		{
			enforceSize();
			VPE_subvector_equC(vx_vect, ((ui_size-1)/samp)+1, samp, C);
		}

		void subvector_equV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VPE_subvector_equV(vx_vect, ((ui_size-1)/samp)+1, samp, X.vx_vect);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//One-Dimensional Vector Operations
//

		tbase absmax()
		{
			enforceSize();
			return VPE_absmax(vx_vect, ui_size);
		}

		tbase absmin()
		{
			enforceSize();
			return VPE_absmin(vx_vect, ui_size);
		}

		tbase absmaxind(ui* Ind)
		{
			enforceSize();
			return VPE_absmaxind(Ind, vx_vect, ui_size);
		}

		tbase absminind(ui* Ind)
		{
			enforceSize();
			return VPE_absminind(Ind, vx_vect, ui_size);
		}

		t prod()
		{
			enforceSize();
			return VPE_prod(vx_vect, ui_size);
		}

		void runprod(cvta X)
		{
			enforceSize();
			VPE_runprod(vx_vect, X.vx_vect, ui_size);
		}

		int iselementC(cta C)
		{
			enforceSize();
			return VPE_iselementC(vx_vect, ui_size, C);
		}

		ui iselementV(cvta X, cvta Tab)
		{
			enforceSize();
			Tab.enforceSize();
			return VPE_iselementV(vx_vect, X.vx_vect,
				ui_size, Tab.vx_vect, Tab.ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Statistical Functions and Building Blocks
//


/////////////////////////////////////////////////////////////////////////////////////////
//Fourier Transforms, Convolutions, Filtering
//


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
			VPE_fprint(stream, vx_vect, ui_size, nperline, linewidth);
		}

#if !defined _Windows || defined __FLAT__ || defined _WIN32
	    void  cprint( const unsigned& nperline )
		{
			enforceSize();
			VPE_cprint( vx_vect, ui_size, nperline);
		}
#endif

		void print(const unsigned& npl)
		{
			enforceSize();
			VPE_print(vx_vect, ui_size, npl);
		}

		void write(FILE* stream)
		{
			enforceSize();
			VPE_write(stream, vx_vect, ui_size);
		}

		void read(FILE* stream)
		{
			enforceSize();
			VPE_read(vx_vect, ui_size, stream);
		}


//nwrite
//nread

		void setWriteFormat(char* formatString) const
		{
			VPE_setWriteFormat(formatString);
		}

		void setWriteSeparate(char* sepString) const
		{
			VPE_setWriteSeparate(sepString);
		}

//setNWriteSeparate

		void store(FILE* stream) const
		{
			enforceSize();
			fwrite(&ui_size,sizeof(ui),1,stream);
			VPE_store(stream,vx_vect,ui_size);
		}

		void recall(FILE* stream)
		{
			ui sz;
			fread(&sz,sizeof(ui),1,stream);
			setSize(sz);
			VPE_recall(vx_vect,ui_size,stream);
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
			return VPE_cmp_eq0(vx_vect,X.vx_vect,ui_size);
		}

		ui cmp_ne0(cvta X)
		{
			enforceSize();
			return VPE_cmp_ne0(vx_vect,X.vx_vect,ui_size);
		}

		ui cmp_eqC(cvta X, cta C)
		{
			enforceSize();
			return VPE_cmp_eqC(vx_vect,X.vx_vect,ui_size,C);
		}

		ui cmp_neC(cvta X, cta C)
		{
			enforceSize();
			return VPE_cmp_neC(vx_vect,X.vx_vect,ui_size,C);
		}

		ui cmp_eqV(cvta X, cvta Y)
		{
			enforceSize();
			return VPE_cmp_eqV(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
		}

		ui cmp_neV(cvta X, cvta Y)
		{
			enforceSize();
			return VPE_cmp_neV(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
		}

		//indexed comparisons are members of uiVecObj


/////////////////////////////////////////////////////////////////////////////////////////
//Standard Arithmetics or Basic Arithmetics
//
		void mulC(cvta X, cta C)
		{
			enforceSize();
			VPE_mulC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divC(cvta X, cta C)
		{
			enforceSize();
			VPE_divC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divrC(cvta X, cta C)
		{
			enforceSize();
			VPE_divrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void mulReC(cvta X, const tbase& C)
		{
			enforceSize();
			VPE_mulReC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divReC(cvta X, const tbase& C)
		{
			enforceSize();
			VPE_divReC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divrReC(cvta X, const tbase& C)
		{
			enforceSize();
			VPE_divrReC(vx_vect, X.vx_vect, ui_size, C);
		}

		void mulV(cvta X, cvta Y)
		{
			enforceSize();
			VPE_mulV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divV(cvta X, cvta Y)
		{
			enforceSize();
			VPE_divV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divrV(cvta X, cvta Y)
		{
			enforceSize();
			VPE_divrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void mulReV(cvta X, vtbase Y)
		{
			enforceSize();
			VPE_mulReV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divReV(cvta X, vtbase Y)
		{
			enforceSize();
			VPE_divReV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divrReV(cvta X, vtbase Y)
		{
			enforceSize();
			VPE_divrReV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Accumulation
//


/////////////////////////////////////////////////////////////////////////////////////////
//Bit-wise operations
//


/////////////////////////////////////////////////////////////////////////////////////////
//Functions of a sub-set of elements
//
		//subvector_equC, subvector_equV defined above

		void subvector_mulC(const unsigned& samp, cta C)
		{
			enforceSize();
			VPE_subvector_mulC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_divC(const unsigned& samp, cta C)
		{
			enforceSize();
			VPE_subvector_divC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_divrC(const unsigned& samp, cta C)
		{
			enforceSize();
			VPE_subvector_divrC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_mulV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VPE_subvector_mulV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void subvector_divV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VPE_subvector_divV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void subvector_divrV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VPE_subvector_divrV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Mathematical Functions
//
		int neg(cvta X)
		{
			enforceSize();
			return VPE_neg(vx_vect, X.vx_vect, ui_size);
		}

		int conj(cvta X)
		{
			enforceSize();
			return VPE_conj(vx_vect,X.vx_vect,ui_size);
		}

		//abs defined above

		int inv(cvta X)
		{
			enforceSize();
			return VPE_inv(vx_vect, X.vx_vect, ui_size);
		}

		int square(cvta X)
		{
			enforceSize();
			return VPE_square(vx_vect,X.vx_vect,ui_size);
		}

		int cubic(cvta X)
		{
			enforceSize();
			return VPE_cubic(vx_vect,X.vx_vect,ui_size);
		}

		int quartic(cvta X)
		{
			enforceSize();
			return VPE_quartic(vx_vect,X.vx_vect,ui_size);
		}

		int ipow(cvta X, int pow)
		{
			enforceSize();
			return VPE_ipow(vx_vect, X.vx_vect, ui_size, pow);
		}

		int u_square(cvta X)
		{
			enforceSize();
			return VPEu_square(vx_vect,X.vx_vect,ui_size);
		}

		int u_cubic(cvta X)
		{
			enforceSize();
			return VPEu_cubic(vx_vect,X.vx_vect,ui_size);
		}

		int u_quartic(cvta X)
		{
			enforceSize();
			return VPEu_quartic(vx_vect,X.vx_vect,ui_size);
		}

		int u_ipow(cvta X, int pow)
		{
			enforceSize();
			return VPEu_ipow(vx_vect, X.vx_vect, ui_size, pow);
		}

		int powReExpo(cvta X, const tbase& expo)
		{
			enforceSize();
			return VPE_powReExpo(vx_vect,X.vx_vect,ui_size,expo);
		}

		int sqrt(cvta X)
		{
			enforceSize();
			return VPE_sqrt(vx_vect,X.vx_vect,ui_size);
		}

		//logtoC member of ceVecObj
		//log2toC member of ceVecObj
		//log10toC member of ceVecObj
		//lntoC member of ceVecObj

		//defined by including ceVecObj.h
		int exptoP(const ceVecObj& X);


	};


	inline ui uiVecObj::cmp_eq0ind(cvta X)
	{
		enforceSize();
		return VPE_cmp_eq0ind(vx_vect,X.vx_vect,ui_size);
	}

	inline ui uiVecObj::cmp_ne0ind(cvta X)
	{
		enforceSize();
		return VPE_cmp_ne0ind(vx_vect,X.vx_vect,ui_size);
	}

	inline ui uiVecObj::cmp_eqCind(cvta X, cta C)
	{
		enforceSize();
		return VPE_cmp_eqCind(vx_vect,X.vx_vect,ui_size,C);
	}

	inline ui uiVecObj::cmp_neCind(cvta X, cta C)
	{
		enforceSize();
		return VPE_cmp_neCind(vx_vect,X.vx_vect,ui_size,C);
	}

	inline ui uiVecObj::cmp_eqVind(cvta X, cvta Y)
	{
		enforceSize();
		return VPE_cmp_eqVind(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
	}

	inline ui uiVecObj::cmp_neVind(cvta X, cvta Y)
	{
		enforceSize();
		return VPE_cmp_neVind(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
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
	inline void vtbase::PtoMagArg(eVecObj* Arg, cvta X)
	{
		enforceSize();
		if(ui_size != Arg->ui_size) throw OptiVec_SizeErr();
		VE_PtoMagArg(vx_vect, Arg->vx_vect, X.vx_vect, ui_size);
	}

        inline void vtbase::PtoMagArg(eVecObj Arg, cvta X)
	{
		enforceSize();
                if(ui_size != Arg.ui_size) throw OptiVec_SizeErr();
                VE_PtoMagArg(vx_vect, Arg.vx_vect, X.vx_vect, ui_size);
	}

	inline void vtbase::PtoMag(cvta X)
	{
		enforceSize();
		VE_PtoMag(vx_vect,X.vx_vect,ui_size);
	}

	inline void vtbase::PtoArg(cvta X)
	{
		enforceSize();
		VE_PtoArg(vx_vect,X.vx_vect,ui_size);
	}

	inline void vtbase::PtoNorm(cvta cplx)
	{
		enforceSize();
		VE_PtoNorm(vx_vect, cplx.vx_vect, ui_size);
	}

	inline void vtbase::PtoReIm(eVecObj* Im, cvta X)
	{
		enforceSize();
		if(ui_size != Im->ui_size) throw OptiVec_SizeErr();
		VE_PtoReIm(vx_vect, Im->vx_vect, X.vx_vect, ui_size);
	}

        inline void vtbase::PtoReIm(eVecObj Im, cvta X)
	{
		enforceSize();
                if(ui_size != Im.ui_size) throw OptiVec_SizeErr();
                VE_PtoReIm(vx_vect, Im.vx_vect, X.vx_vect, ui_size);
	}

	inline void vtbase::PtoRe(cvta X)
	{
		enforceSize();
		VE_PtoRe(vx_vect, X.vx_vect, ui_size);
	}

	inline void vtbase::PtoIm(cvta X)
	{
		enforceSize();
		VE_PtoIm(vx_vect, X.vx_vect, ui_size);
	}

	inline int vtbase::abs(cvta cplx)
	{
		enforceSize();
		return VPE_abs(vx_vect, cplx.vx_vect, ui_size);
	}
#endif //__BORLANDC__
#endif //EVECOBJ_H

#ifdef CFVECOBJ_H
#endif //CFVECOBJ_H

#ifdef CDVECOBJ_H
#endif //CDVECOBJ_H

#ifdef CEVECOBJ_H
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
		return VPE_logtoC(vx_vect,X.vx_vect,ui_size);
	}

	inline int ceVecObj::log2toC(const peVecObj& X)
	{
		enforceSize();
		return VPE_log2toC(vx_vect,X.vx_vect,ui_size);
	}

	inline int ceVecObj::log10toC(const peVecObj& X)
	{
		enforceSize();
		return VPE_log10toC(vx_vect,X.vx_vect,ui_size);
	}

	inline int peVecObj::exptoP(const ceVecObj& X)
	{
		enforceSize();
		return VCE_exptoP(vx_vect,X.vx_vect,ui_size);
	}
#endif //__BORLANDC__
#endif //CEVECOBJ_H

#ifdef PFVECOBJ_H
	inline void pfVecObj::PEtoPF(const peVecObj& X)
	{
		enforceSize();
		V_PEtoPF(vx_vect,X.vx_vect,ui_size);
	}

	inline void peVecObj::PFtoPE(const pfVecObj& X)
	{
		enforceSize();
		V_PFtoPE(vx_vect,X.vx_vect,ui_size);
	}
#endif //PFVECOBJ_H

#ifdef PDVECOBJ_H
	inline void peVecObj::PDtoPE(const pdVecObj& X)
	{
		enforceSize();
		V_PDtoPE(vx_vect,X.vx_vect,ui_size);
	}

	inline void pdVecObj::PEtoPD(const peVecObj& X)
	{
		enforceSize();
		V_PEtoPD(vx_vect,X.vx_vect,ui_size);
	}
#endif //PDVECOBJ_H

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

#define PEVECOBJ_H
#endif //PEVECOBJ_H
