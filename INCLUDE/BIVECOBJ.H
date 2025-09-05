/* biVecObj.H

  object-oriented interface for OptiVec vector functions of type "byte"

  This file was written by Brian Dale, Case Western Reserve Univ.

  Copyright (c) 1996-2002 by OptiCode
  All Rights Reserved.
*/

#ifndef BIVECOBJ_H

#include <VecObj.h>
#include <uiVecObj.h>

#define cvta const vector<char>&
#define cuia const ui&
#define cta  const char&

#define vta  vector<char>&
#define uia  ui&
#define ta   char&

#define vt   vector<char>
#define t    char
#define tVector biVector

#define tVecObj biVecObj
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
		{	if(size){	vx_vect= VBI_vector(size);	ui_size= size;}
			else{	vx_vect= 0;	ui_size= 0;}}

		vector(cuia size, cta fill)
		{	if(!size) //size == 0
			{	vx_vect=0;	ui_size=0;}
			else if(!fill) //fill == 0.0
			{	vx_vect= VBI_vector0(size);	ui_size= size;}
			else
			{	vx_vect= VBI_vector(size);	ui_size= size;	VBI_equC(vx_vect,ui_size,fill);}}

		vector(cvta init)
		{	if(init.ui_size)
			{ vx_vect= VBI_vector(init.ui_size);	ui_size= init.ui_size;	VBI_equV(vx_vect, init.vx_vect, ui_size);}
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
				vx_vect= VBI_vector(size);
				ui_size= size;}}

		void setSize(cvta X)
		{	if(!X.ui_size) free(); //can set size to zero
			if(ui_size != X.ui_size) //only allocate if different sizes
			{	if(vx_vect) V_free(vx_vect);
				vx_vect= VBI_vector(X.ui_size);
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
			VBI_equV(vx_vect, vect.vx_vect, ui_size);
			return *this;}

		vta operator=(cta scalar)
		{	enforceSize();
			VBI_equC(vx_vect, ui_size, scalar);
			return *this;}

		//this is the only really dangerous operator
		ta operator[](const int& index){return vx_vect[index];}

		vta operator+=(cvta X)
		{	enforceSize();
			VBI_accV(vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator+=(cta scalar)
		{	enforceSize();
			VBI_addC(vx_vect, vx_vect, ui_size, scalar);
			return *this;}

		vta operator*=(cvta X)
		{	enforceSize();
			VBI_mulV(vx_vect, vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator*=(cta scalar)
		{	enforceSize();
			VBI_mulC(vx_vect,vx_vect,ui_size,scalar);
			return *this;}

		vt operator+(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VBI_addV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator+(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VBI_addC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator+(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VBI_addC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator-(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VBI_subV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator-(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VBI_subC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator-(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VBI_subrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator*(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VBI_mulV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator*(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VBI_mulC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator*(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VBI_mulC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator/(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VBI_divV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator/(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VBI_divC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator/(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VBI_divrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
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
			if(size) vx_vect= VBI_vector(size);
			ui_size=size;
		}

		void vector0(ui size)
		{
			free();
			if(size) vx_vect= VBI_vector0(size);
			ui_size=size;
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Addressing single vector elements
//
		t* Pelement(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VBI_Pelement(vx_vect,n);
			//not in funcref
		}

		t element(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VBI_element(vx_vect,n);
			//not in funcref
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Initialization
//
		void equ0()
		{
			enforceSize();
			VBI_equ0(vx_vect,ui_size);
		}

		void equC(cta C)
		{
			enforceSize();
			VBI_equC(vx_vect, ui_size, C);
		}

		void equV(cvta X)
		{
			enforceSize();
			VBI_equV(vx_vect, X.vx_vect, ui_size);
		}

		void ramp(cta Start, cta Rise)
		{
			enforceSize();
			VBI_ramp(vx_vect, ui_size, Start, Rise);
		}

		long random(const long& seed, cta Min, cta Max)
		{
			enforceSize();
			return VBI_random(vx_vect, ui_size, seed, Min, Max);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Data-type interconversions
//
		//defined by including ubVecObj.h
		void UBtoBI(const ubVecObj& X);

		//defined by including siVecObj.h
		void SItoBI(const siVecObj& X);

		//defined by including biVecObj.h
		void  ItoBI(const  iVecObj& X);

		//defined by including liVecObj.h
		void LItoBI(const liVecObj& X);

		//defined by including qiVecObj.h
		void QItoBI(const qiVecObj& X);


/////////////////////////////////////////////////////////////////////////////////////////
//Index-oriented manipulations
//
		void reflect()
		{
			enforceSize();
			VBI_reflect(vx_vect, ui_size);
		}

		void rev(cvta X)
		{
			enforceSize();
			VBI_rev(vx_vect, X.vx_vect, ui_size);
		}

#ifdef V_HUGE
		void rotate( cvta X, long pos )
#else
		void rotate( cvta X, int pos )
#endif
		{
			enforceSize();
			VBI_rotate(vx_vect, X.vx_vect, ui_size, pos);
		}


		void _delete(ui pos)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VBI_delete(vx_vect, ui_size, pos);
		}

		void insert(ui pos, cta C)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VBI_insert(vx_vect, ui_size, pos, C);
		}

		void sort(cvta X, int dir=1)
		{
			enforceSize();
			VBI_sort(vx_vect,X.vx_vect,ui_size,dir);
		}

		//sortind member of uiVecObj

		void subvector(cvta X, int step, ui start=0)
		{
			enforceSize();
			if(vx_vect==X.vx_vect) throw OptiVec_Err();
			VBI_subvector(vx_vect, ui_size, X.vx_vect+start, step);
		}

		void subvector_equC(const unsigned& samp, cta C)
		{
			enforceSize();
			VBI_subvector_equC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_equV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VBI_subvector_equV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void indpick(const vector<ui>& Ind, cvta X);
		void indput(cvta X, const vector<ui>& Ind);

		ui searchC(cta C, int mode=0)
		{
			enforceSize();
			return VBI_searchC(vx_vect, ui_size, C, mode);
		}

		//searchV member of uiVecObj


/////////////////////////////////////////////////////////////////////////////////////////
//Functions of a sub-set of elements
//


/////////////////////////////////////////////////////////////////////////////////////////
//One-Dimensional Vector Operations
//
		t sum()
		{
			enforceSize();
			return VBI_sum(vx_vect, ui_size);
		}

		void runsum(cvta X)
		{
			enforceSize();
			VBI_runsum(vx_vect, X.vx_vect, ui_size);
		}

		t _max()
		{
			enforceSize();
			return VBI_max(vx_vect, ui_size);
		}

		t _min()
		{
			enforceSize();
			return VBI_min(vx_vect, ui_size);
		}

		t maxind(ui* Ind)
		{
			enforceSize();
			return VBI_maxind(Ind, vx_vect, ui_size);
		}

		t minind(ui* Ind)
		{
			enforceSize();
			return VBI_minind(Ind, vx_vect, ui_size);
		}

		void runmax(cvta X)
		{
			enforceSize();
			VBI_runmax(vx_vect, X.vx_vect, ui_size);
		}

		void runmin(cvta X)
		{
			enforceSize();
			VBI_runmin(vx_vect, X.vx_vect, ui_size);
		}

		double mean() const
		{
			enforceSize();
			return VBI_mean(vx_vect,ui_size);
		}

		int iselementC(cta C)
		{
			enforceSize();
			return VBI_iselementC(vx_vect, ui_size, C);
		}

		ui iselementV(cvta X, cvta Tab)
		{
			enforceSize();
			return VBI_iselementV(vx_vect, X.vx_vect,
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
		void fprint(FILE* stream, unsigned nperline, unsigned linewidth) const
		{
			enforceSize();
			VBI_fprint(stream,vx_vect,ui_size,nperline,linewidth);
		}

#if !defined _Windows || defined __FLAT__ || defined _WIN32
		void cprint(unsigned nperline) const
		{
			enforceSize();
			VBI_cprint(vx_vect,ui_size,nperline);
		}
#endif
		void print(unsigned nperline) const
		{
			fprint(stdout,nperline,80);
		}

		void setRadix(int i)
		{
			V_setRadix(i);
		}

		void read(FILE* stream)
		{
			enforceSize();
			VBI_read(vx_vect,ui_size,stream);
		}

		void write(FILE* stream) const
		{
			enforceSize();
			VBI_write(stream,vx_vect,ui_size);
		}

//nread
//nwrite

		void setWriteFormat(char* formatString) const
		{
			VBI_setWriteFormat(formatString);
		}

		void setWriteSeparate(char* sepString) const
		{
			VBI_setWriteSeparate(sepString);
		}

//setNWriteSeparate

		void store(FILE* stream) const
		{
			enforceSize();
			fwrite(&ui_size,sizeof(ui),1,stream);
			VBI_store(stream,vx_vect,ui_size);
		}

		void recall(FILE* stream)
		{
			ui sz;
			fread(&sz,sizeof(ui),1,stream);
			setSize(sz);
			VBI_recall(vx_vect,ui_size,stream);
		}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//from VXmath.h
//


/////////////////////////////////////////////////////////////////////////////////////////
//Rounding
//
		//defined by including fVecObj.h
		int roundtoBI(const fVecObj& X);

		//defined by including fVecObj.h
		int floortoBI(const fVecObj& X);
		
		//defined by including fVecObj.h
		int ceiltoBI(const fVecObj& X);
		
		//defined by including fVecObj.h
		int choptoBI(const fVecObj& X);
		
		int trunctoBI(const fVecObj& X)
		{
			return choptoBI(X);
		}
		
		//defined by including dVecObj.h
		int roundtoBI(const dVecObj& X);
		
		//defined by including dVecObj.h
		int floortoBI(const dVecObj& X);
		
		//defined by including dVecObj.h
		int ceiltoBI(const dVecObj& X);
		
		//defined by including dVecObj.h
		int choptoBI(const dVecObj& X);
		
		int trunctoBI(const dVecObj& X)
		{
			return choptoBI(X);
		}
		
#ifdef __BORLANDC__ //80-bit numbers supported
		//defined by including eVecObj.h
		int roundtoBI(const eVecObj& X);
		
		//defined by including eVecObj.h
		int floortoBI(const eVecObj& X);
		
		//defined by including eVecObj.h
		int ceiltoBI(const eVecObj& X);
		
		//defined by including eVecObj.h
		int choptoBI(const eVecObj& X);
		
		int trunctoBI(const eVecObj& X)
		{
			return choptoBI(X);
		}
#endif //__BORLANDC__


/////////////////////////////////////////////////////////////////////////////////////////
//Comparisons
//


/////////////////////////////////////////////////////////////////////////////////////////
//Standard Arithmetics or Basic Arithmetics
//
		void neg(cvta X)
		{
			enforceSize();
			VBI_neg(vx_vect, X.vx_vect, ui_size);
		}

		void abs(cvta X)
		{
			enforceSize();
			VBI_abs(vx_vect, X.vx_vect, ui_size);
		}

		void addC(cvta X, cta C)
		{
			enforceSize();
			VBI_addC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subC(cvta X, cta C)
		{
			enforceSize();
			VBI_subC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subrC(cvta X, cta C)
		{
			enforceSize();
			VBI_subrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void mulC(cvta X, cta C)
		{
			enforceSize();
			VBI_mulC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divC(cvta X, cta C)
		{
			enforceSize();
			VBI_divC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divrC(cvta X, cta C)
		{
			enforceSize();
			VBI_divrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void modC(cvta X, cta C)
		{
			enforceSize();
			VBI_modC(vx_vect, X.vx_vect, ui_size, C);
		}

		void maxC(cvta X, cta C)
		{
			enforceSize();
			VBI_maxC( vx_vect, X.vx_vect, ui_size, C);
		}

		void minC(cvta X, cta C)
		{
			enforceSize();
			VBI_minC( vx_vect, X.vx_vect, ui_size, C);
		}

		void addV(cvta X, cvta Y)
		{
			enforceSize();
			VBI_addV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subV(cvta X, cvta Y)
		{
			enforceSize();
			VBI_subV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subrV(cvta X, cvta Y)
		{
			enforceSize();
			VBI_subrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void mulV(cvta X, cvta Y)
		{
			enforceSize();
			VBI_mulV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divV(cvta X, cvta Y)
		{
			enforceSize();
			VBI_divV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divrV(cvta X, cvta Y)
		{
			enforceSize();
			VBI_divrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void modV(cvta X, cvta Y)
		{
			enforceSize();
			VBI_modV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void maxV(cvta X, cvta Y)
		{
			enforceSize();
			VBI_maxV( vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void minV(cvta X, cvta Y)
		{
			enforceSize();
			VBI_minV( vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Accumulation
//
		void accV(cvta X)
		{
			enforceSize();
			VBI_accV(vx_vect,X.vx_vect,ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Bit-wise operations
//
		void shl(cvta X, unsigned C)
		{
			enforceSize();
			VBI_shl(vx_vect,X.vx_vect,ui_size,C);
		}

		void shr(cvta X, unsigned C)
		{
			enforceSize();
			VBI_shr(vx_vect,X.vx_vect,ui_size,C);
		}

		void and(cvta X, cta C)
		{
			enforceSize();
			VBI_and(vx_vect,X.vx_vect,ui_size,C);
		}

		void or(cvta X, cta C)
		{
			enforceSize();
			VBI_or(vx_vect,X.vx_vect,ui_size,C);
		}

		void xor(cvta X, cta C)
		{
			enforceSize();
			VBI_xor(vx_vect,X.vx_vect,ui_size,C);
		}

		void not(cvta X)
		{
			enforceSize();
			VBI_not(vx_vect,X.vx_vect,ui_size);
		}

/////////////////////////////////////////////////////////////////////////////////////////
// Graphics
//

            #ifdef __VGRAPH_H
                void xyAutoPlot( cvta X, unsigned form, COLORREF color )
                {
                    enforceSize();
                    enforceSize(X.ui_size);
                    VBI_xyAutoPlot( X.vx_vect, vx_vect, ui_size, form, color );
                }

                void xy2AutoPlot( cvta X1, unsigned form1, COLORREF color1,
                         cvta X2, cvta Y2, unsigned form2, COLORREF color2 )
                {
                    enforceSize();
                    enforceSize(X1.ui_size);
                    Y2.enforceSize(X2.ui_size);
                    VBI_xy2AutoPlot( X1.vx_vect, vx_vect, ui_size, form1, color1,
                              X2.vx_vect, Y2.vx_vect, X2.ui_size, form2, color2 );
                }

                void yAutoPlot( unsigned form, COLORREF color )
                {
                    enforceSize();
                    VBI_yAutoPlot( vx_vect, ui_size, form, color );
                }

                void y2AutoPlot( unsigned form1, COLORREF color1,
                        cvta Y2, unsigned form2, COLORREF color2 )
                {
                    enforceSize();
                    Y2.enforceSize();
                    VBI_y2AutoPlot( vx_vect, ui_size, form1, color1,
                             Y2.vx_vect, Y2.ui_size, form2, color2 );
                }

                void xyDataPlot( cvta X, unsigned form, COLORREF color )
                {
                    enforceSize();
                    enforceSize(X.ui_size);
                    VBI_xyDataPlot( X.vx_vect, vx_vect, ui_size, form, color );
                }

                void yDataPlot( unsigned form, COLORREF color )
                {
                    enforceSize();
                    VBI_yDataPlot( vx_vect, ui_size, form, color );
                }
            #endif   // __VGRAPH_H

	};


	inline void uiVecObj::sortind(const biVecObj& X, int dir)
	{
		enforceSize();
		VBI_sortind(vx_vect,X.vx_vect,ui_size,dir);
	}

	inline void uiVecObj::searchV(const biVecObj& X, const biVecObj& Tab, int mode=0)
	{
		enforceSize();
		Tab.enforceSize();
		VBI_searchV(vx_vect,X.vx_vect,ui_size,Tab.vx_vect,Tab.ui_size,mode);
	}

#ifdef UBVECOBJ_H
	inline void ubVecObj::BItoUB(const biVecObj& X)
	{
		enforceSize();
		V_BItoUB(vx_vect,X.vx_vect,ui_size);
	}

	inline void biVecObj::UBtoBI(const ubVecObj& X)
	{
		enforceSize();
		V_UBtoBI(vx_vect,X.vx_vect,ui_size);
	}
#endif //UBVECOBJ_H

#ifdef SIVECOBJ_H
	inline void siVecObj::BItoSI(const biVecObj& X)
	{
		enforceSize();
		V_BItoSI(vx_vect,X.vx_vect,ui_size);
	}

	inline void biVecObj::SItoBI(const siVecObj& X)
	{
		enforceSize();
		V_SItoBI(vx_vect,X.vx_vect,ui_size);
	}

	inline void siVecObj::accVBI(const biVecObj& X)
	{
		enforceSize();
		VSI_accVBI(vx_vect,X.vx_vect,ui_size);
	}
#endif //SIVECOBJ_H

#ifdef IVECOBJ_H
	inline void biVecObj::ItoBI(const iVecObj& X)
	{
		enforceSize();
		V_ItoBI(vx_vect,X.vx_vect,ui_size);
	}

	inline void iVecObj::BItoI(const biVecObj& X)
	{
		enforceSize();
		V_BItoI(vx_vect,X.vx_vect,ui_size);
	}

	inline void iVecObj::accVBI(const biVecObj& X)
	{
		enforceSize();
		VI_accVBI(vx_vect,X.vx_vect,ui_size);
	}
#endif //IVECOBJ_H

#ifdef LIVECOBJ_H
	inline void biVecObj::LItoBI(const liVecObj& X)
	{
		enforceSize();
		V_LItoBI(vx_vect,X.vx_vect,ui_size);
	}

	inline void liVecObj::BItoLI(const biVecObj& X)
	{
		enforceSize();
		V_BItoLI(vx_vect,X.vx_vect,ui_size);
	}

	inline void liVecObj::accVBI(const biVecObj& X)
	{
		enforceSize();
		VLI_accVBI(vx_vect,X.vx_vect,ui_size);
	}
#endif //LIVECOBJ_H

#ifdef QIVECOBJ_H
	inline void biVecObj::QItoBI(const qiVecObj& X)
	{
		enforceSize();
		V_QItoBI(vx_vect,X.vx_vect,ui_size);
	}

	inline void qiVecObj::BItoQI(const biVecObj& X)
	{
		enforceSize();
		V_BItoQI(vx_vect,X.vx_vect,ui_size);
	}

	inline void qiVecObj::accVBI(const biVecObj& X)
	{
		enforceSize();
		VQI_accVBI(vx_vect,X.vx_vect,ui_size);
	}
#endif //QIVECOBJ_H

#ifdef FVECOBJ_H
	inline void fVecObj::accVBI(const biVecObj& X)
	{
		enforceSize();
		VF_accVBI(vx_vect,X.vx_vect,ui_size);
	}

	inline void fVecObj::BItoF(const biVecObj& X)
	{
		enforceSize();
		V_BItoF(vx_vect,X.vx_vect,ui_size);
	}

	inline int biVecObj::roundtoBI(const fVecObj& X)
	{
		enforceSize();
		return VF_roundtoBI(vx_vect,X.vx_vect,ui_size);
	}

	inline int biVecObj::floortoBI(const fVecObj& X)
	{
		enforceSize();
		return VF_floortoBI(vx_vect,X.vx_vect,ui_size);
	}

	inline int biVecObj::ceiltoBI(const fVecObj& X)
	{
		enforceSize();
		return VF_ceiltoBI(vx_vect,X.vx_vect,ui_size);
	}

	inline int biVecObj::choptoBI(const fVecObj& X)
	{
		enforceSize();
		return VF_choptoBI(vx_vect,X.vx_vect,ui_size);
	}
#endif //FVECOBJ_H

#ifdef DVECOBJ_H
	inline void dVecObj::accVBI(const biVecObj& X)
	{
		enforceSize();
		VD_accVBI(vx_vect,X.vx_vect,ui_size);
	}

	inline void dVecObj::BItoD(const biVecObj& X)
	{
		enforceSize();
		V_BItoD(vx_vect,X.vx_vect,ui_size);
	}

	inline int biVecObj::roundtoBI(const dVecObj& X)
	{
		enforceSize();
		return VD_roundtoBI(vx_vect,X.vx_vect,ui_size);
	}

	inline int biVecObj::floortoBI(const dVecObj& X)
	{
		enforceSize();
		return VD_floortoBI(vx_vect,X.vx_vect,ui_size);
	}

	inline int biVecObj::ceiltoBI(const dVecObj& X)
	{
		enforceSize();
		return VD_ceiltoBI(vx_vect,X.vx_vect,ui_size);
	}

	inline int biVecObj::choptoBI(const dVecObj& X)
	{
		enforceSize();
		return VD_choptoBI(vx_vect,X.vx_vect,ui_size);
	}
#endif //DVECOBJ_H

#ifdef EVECOBJ_H
#ifdef __BORLANDC__ //80-bit numbers supported
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
#endif //__BORLANDC__
#endif //EVECOBJ_H

#ifdef CFVECOBJ_H
#endif //CFVECOBJ_H

#ifdef CDVECOBJ_H
#endif //CDVECOBJ_H

#ifdef CEVECOBJ_H
#ifdef __BORLANDC__ //80-bit numbers supported
#endif //__BORLANDC__
#endif //CEVECOBJ_H

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
#ifdef __BORLANDC__
#pragma warn .inl
#endif

#define BIVECOBJ_H
#endif //BIVECOBJ_H
