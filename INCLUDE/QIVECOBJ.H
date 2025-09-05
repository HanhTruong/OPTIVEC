/* qiVecObj.H

  object-oriented interface for OptiVec vector functions of type "quad"

  This file was written by Brian Dale, Case Western Reserve Univ.

  Copyright (c) 1996-2002 by OptiCode
  All Rights Reserved.
*/

#ifndef QIVECOBJ_H

#include <VecObj.h>

#define cvta const vector<quad>&
#define cuia const ui&
#define cta  const quad&

#define vta  vector<quad>&
#define uia  ui&
#define ta   quad&

#define vt   vector<quad>
#define t    quad
#define tVector qiVector

#define tVecObj qiVecObj
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
		{	if(size){	vx_vect= VQI_vector(size);	ui_size= size;}
			else{	vx_vect= 0;	ui_size= 0;}}

		vector(cuia size, cta fill)
		{	if(!size) //size == 0
			{	vx_vect=0;	ui_size=0;}
			else
			{	vx_vect= VQI_vector(size);	ui_size= size;	VQI_equC(vx_vect,ui_size,fill);}}

		vector(cvta init)
		{	if(init.ui_size)
			{ vx_vect= VQI_vector(init.ui_size);	ui_size= init.ui_size;	VQI_equV(vx_vect, init.vx_vect, ui_size);}
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
				vx_vect= VQI_vector(size);
				ui_size= size;}}

		void setSize(cvta X)
		{	if(!X.ui_size) free(); //can set size to zero
			if(ui_size != X.ui_size) //only allocate if different sizes
			{	if(vx_vect) V_free(vx_vect);
				vx_vect= VQI_vector(X.ui_size);
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
			VQI_equV(vx_vect, vect.vx_vect, ui_size);
			return *this;}

		vta operator=(cta scalar)
		{	enforceSize();
			VQI_equC(vx_vect, ui_size, scalar);
			return *this;}

		//this is the only really dangerous operator
		ta operator[](const int& index){return vx_vect[index];}

		vta operator+=(cvta X)
		{	enforceSize();
			VQI_accV(vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator+=(cta scalar)
		{	enforceSize();
			VQI_addC(vx_vect, vx_vect, ui_size, scalar);
			return *this;}

		vta operator*=(cvta X)
		{	enforceSize();
			VQI_mulV(vx_vect, vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator*=(cta scalar)
		{	enforceSize();
			VQI_mulC(vx_vect,vx_vect,ui_size,scalar);
			return *this;}

		vt operator+(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VQI_addV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator+(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VQI_addC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator+(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VQI_addC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator-(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VQI_subV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator-(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VQI_subC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator-(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VQI_subrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator*(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VQI_mulV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator*(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VQI_mulC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator*(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VQI_mulC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator/(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VQI_divV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator/(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VQI_divC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator/(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VQI_divrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
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
			if(size) vx_vect= VQI_vector(size);
			ui_size=size;
		}

		void vector0(ui size)
		{
			free();
			if(size) vx_vect= VQI_vector0(size);
			ui_size=size;
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Addressing single vector elements
//
		t* Pelement(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VQI_Pelement(vx_vect,n);
			//not in funcref
		}

		t element(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VQI_element(vx_vect,n);
			//not in funcref
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Initialization
//
		void equ0()
		{
			enforceSize();
			VQI_equ0(vx_vect,ui_size);
		}

		void equC(cta C)
		{
			enforceSize();
			VQI_equC(vx_vect, ui_size, C);
		}

		void equV(cvta X)
		{
			enforceSize();
			VQI_equV(vx_vect, X.vx_vect, ui_size);
		}

		void ramp(cta Start, cta Rise)
		{
			enforceSize();
			VQI_ramp(vx_vect, ui_size, Start, Rise);
		}

		long random(const long& seed, cta Min, cta Max)
		{
			enforceSize();
			return VQI_random(vx_vect, ui_size, seed, Min, Max);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Data-type interconversions
//
		//defined by including biVecObj.h
		void BItoQI(const biVecObj& X);

		//defined by including siVecObj.h
		void SItoQI(const siVecObj& X);
		
		//defined by including liVecObj.h
		void LItoQI(const liVecObj& X);
		
		//defined by including iVecObj.h
		void  ItoQI(const  iVecObj& X);


/////////////////////////////////////////////////////////////////////////////////////////
//Index-oriented manipulations
//
		void reflect()
		{
			enforceSize();
			VQI_reflect(vx_vect, ui_size);
		}

		void rev(cvta X)
		{
			enforceSize();
			VQI_rev(vx_vect, X.vx_vect, ui_size);
		}

#ifdef V_HUGE
		void rotate( cvta X, long pos )
#else
		void rotate( cvta X, int pos )
#endif
		{
			enforceSize();
			VQI_rotate(vx_vect, X.vx_vect, ui_size, pos);
		}


		void _delete(ui pos)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VQI_delete(vx_vect, ui_size, pos);
		}

		void insert(ui pos, cta C)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VQI_insert(vx_vect, ui_size, pos, C);
		}

		void sort(cvta X, int dir=1)
		{
			enforceSize();
			VQI_sort(vx_vect,X.vx_vect,ui_size,dir);
		}

		//sortind member of uiVecObj

		void subvector(cvta X, int step, ui start=0)
		{
			enforceSize();
			if(vx_vect==X.vx_vect) throw OptiVec_Err();
			VQI_subvector(vx_vect, ui_size, X.vx_vect+start, step);
		}

		void subvector_equC(const unsigned& samp, cta C)
		{
			enforceSize();
			VQI_subvector_equC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_equV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VQI_subvector_equV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void indpick(const vector<ui>& Ind, cvta X);
		void indput(cvta X, const vector<ui>& Ind);

		ui searchC(cta C, int mode=0)
		{
			enforceSize();
			return VQI_searchC(vx_vect, ui_size, C, mode);
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
			return VQI_sum(vx_vect, ui_size);
		}

		t _max()
		{
			enforceSize();
			return VQI_max(vx_vect, ui_size);
		}

		t _min()
		{
			enforceSize();
			return VQI_min(vx_vect, ui_size);
		}

		t maxind(ui* Ind)
		{
			enforceSize();
			return VQI_maxind(Ind, vx_vect, ui_size);
		}

		t minind(ui* Ind)
		{
			enforceSize();
			return VQI_minind(Ind, vx_vect, ui_size);
		}

		void runsum(cvta X)
		{
			enforceSize();
			VQI_runsum(vx_vect, X.vx_vect, ui_size);
		}

		void runmax(cvta X)
		{
			enforceSize();
			VQI_runmax(vx_vect, X.vx_vect, ui_size);
		}

		void runmin(cvta X)
		{
			enforceSize();
			VQI_runmin(vx_vect, X.vx_vect, ui_size);
		}

		double fsum() const
		{
			enforceSize();
			return VQI_fsum(vx_vect,ui_size);
		}

		double mean() const
		{
			enforceSize();
			return VQI_mean(vx_vect,ui_size);
		}

		int iselementC(cta C)
		{
			enforceSize();
			return VQI_iselementC(vx_vect, ui_size, C);
		}

		ui iselementV(cvta X, cvta Tab)
		{
			enforceSize();
			return VQI_iselementV(vx_vect, X.vx_vect,
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
			VQI_fprint(stream,vx_vect,ui_size,nperline,linewidth);
		}

#if !defined _Windows || defined __FLAT__ || defined _WIN32
		void cprint(unsigned nperline) const
		{
			enforceSize();
			VQI_cprint(vx_vect,ui_size,nperline);
		}
#endif
		void print(unsigned nperline) const
		{
			fprint(stdout,nperline,80);
		}

		void read(FILE* stream)
		{
			enforceSize();
			VQI_read(vx_vect,ui_size,stream);
		}

		void write(FILE* stream) const
		{
			enforceSize();
			VQI_write(stream,vx_vect,ui_size);
		}

//nread
//nwrite

		void setWriteFormat(char* formatString) const
		{
			VQI_setWriteFormat(formatString);
		}

		void setWriteSeparate(char* sepString) const
		{
			VQI_setWriteSeparate(sepString);
		}

//setNWriteSeparate

		void store(FILE* stream) const
		{
			enforceSize();
			fwrite(&ui_size,sizeof(ui),1,stream);
			VQI_store(stream,vx_vect,ui_size);
		}

		void recall(FILE* stream)
		{
			ui sz;
			fread(&sz,sizeof(ui),1,stream);
			setSize(sz);
			VQI_recall(vx_vect,ui_size,stream);
		}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//from VXmath.h
//


/////////////////////////////////////////////////////////////////////////////////////////
//Rounding
//
		//defined by including fVecObj.h
		int roundtoQI(const fVecObj& X);

		//defined by including fVecObj.h
		int floortoQI(const fVecObj& X);
		
		//defined by including fVecObj.h
		int ceiltoQI(const fVecObj& X);
		
		//defined by including fVecObj.h
		int choptoQI(const fVecObj& X);
		
		int trunctoQI(const fVecObj& X)
		{
			return choptoQI(X);
		}
		
		//defined by including dVecObj.h
		int roundtoQI(const dVecObj& X);
		
		//defined by including dVecObj.h
		int floortoQI(const dVecObj& X);
		
		//defined by including dVecObj.h
		int ceiltoQI(const dVecObj& X);
		
		//defined by including dVecObj.h
		int choptoQI(const dVecObj& X);
		
		int trunctoQI(const dVecObj& X)
		{
			return choptoQI(X);
		}
		
#ifdef __BORLANDC__ //80-bit numbers supported
		//defined by including eVecObj.h
		int roundtoQI(const eVecObj& X);
		
		//defined by including eVecObj.h
		int floortoQI(const eVecObj& X);
		
		//defined by including eVecObj.h
		int ceiltoQI(const eVecObj& X);
		
		//defined by including eVecObj.h
		int choptoQI(const eVecObj& X);
		
		int trunctoQI(const eVecObj& X)
		{
			return choptoQI(X);
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
			VQI_neg(vx_vect, X.vx_vect, ui_size);
		}

		void abs(cvta X)
		{
			enforceSize();
			VQI_abs(vx_vect, X.vx_vect, ui_size);
		}

		void addC(cvta X, cta C)
		{
			enforceSize();
			VQI_addC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subC(cvta X, cta C)
		{
			enforceSize();
			VQI_subC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subrC(cvta X, cta C)
		{
			enforceSize();
			VQI_subrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void mulC(cvta X, cta C)
		{
			enforceSize();
			VQI_mulC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divC(cvta X, cta C)
		{
			enforceSize();
			VQI_divC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divrC(cvta X, cta C)
		{
			enforceSize();
			VQI_divrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void modC(cvta X, cta C)
		{
			enforceSize();
			VQI_modC(vx_vect, X.vx_vect, ui_size, C);
		}

		void maxC(cvta X, cta C)
		{
			enforceSize();
			VQI_maxC( vx_vect, X.vx_vect, ui_size, C);
		}

		void minC(cvta X, cta C)
		{
			enforceSize();
			VQI_minC( vx_vect, X.vx_vect, ui_size, C);
		}

		void addV(cvta X, cvta Y)
		{
			enforceSize();
			VQI_addV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subV(cvta X, cvta Y)
		{
			enforceSize();
			VQI_subV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subrV(cvta X, cvta Y)
		{
			enforceSize();
			VQI_subrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void mulV(cvta X, cvta Y)
		{
			enforceSize();
			VQI_mulV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divV(cvta X, cvta Y)
		{
			enforceSize();
			VQI_divV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divrV(cvta X, cvta Y)
		{
			enforceSize();
			VQI_divrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void modV(cvta X, cvta Y)
		{
			enforceSize();
			VQI_modV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void maxV(cvta X, cvta Y)
		{
			enforceSize();
			VQI_maxV( vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void minV(cvta X, cvta Y)
		{
			enforceSize();
			VQI_minV( vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Accumulation
//
		void accV(cvta X)
		{
			enforceSize();
			VQI_accV(vx_vect,X.vx_vect,ui_size);
		}

		//defined by including biVecObj.h
		void accVBI(const biVecObj& X);

		//defined by including siVecObj.h
		void accVSI(const siVecObj& X);
		
		//defined by including iVecObj.h
		void accVI (const  iVecObj& X);
		
		//defined by including liVecObj.h
		void accVLI(const liVecObj& X);
		
		//defined by including ubVecObj.h
		void accVUB(const ubVecObj& X);
		
		//defined by including usVecObj.h
		void accVUS(const usVecObj& X);
		
		//defined by including uVecObj.h
		void accVU (const  uVecObj& X);
		
		//defined by including ulVecObj.h
		void accVUL(const ulVecObj& X);
		
		//defined by including uiVecObj.h
		void accVUI(const uiVecObj& X);


/////////////////////////////////////////////////////////////////////////////////////////
//Bit-wise operations
//
		void shl(cvta X, unsigned C)
		{
			enforceSize();
			VQI_shl(vx_vect,X.vx_vect,ui_size,C);
		}

		void shr(cvta X, unsigned C)
		{
			enforceSize();
			VQI_shr(vx_vect,X.vx_vect,ui_size,C);
		}

		void and(cvta X, cta C)
		{
			enforceSize();
			VQI_and(vx_vect,X.vx_vect,ui_size,C);
		}

		void or(cvta X, cta C)
		{
			enforceSize();
			VQI_or(vx_vect,X.vx_vect,ui_size,C);
		}

		void xor(cvta X, cta C)
		{
			enforceSize();
			VQI_xor(vx_vect,X.vx_vect,ui_size,C);
		}

		void not(cvta X)
		{
			enforceSize();
			VQI_not(vx_vect,X.vx_vect,ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Functions of a sub-set of elements
//


/////////////////////////////////////////////////////////////////////////////////////////
//Mathematical Functions
//

/////////////////////////////////////////////////////////////////////////////////////////
// Graphics
//

            #ifdef __VGRAPH_H
                void xyAutoPlot( cvta X, unsigned form, COLORREF color )
                {
                    enforceSize();
                    enforceSize(X.ui_size);
                    VQI_xyAutoPlot( X.vx_vect, vx_vect, ui_size, form, color );
                }

                void xy2AutoPlot( cvta X1, unsigned form1, COLORREF color1,
                         cvta X2, cvta Y2, unsigned form2, COLORREF color2 )
                {
                    enforceSize();
                    enforceSize(X1.ui_size);
                    Y2.enforceSize(X2.ui_size);
                    VQI_xy2AutoPlot( X1.vx_vect, vx_vect, ui_size, form1, color1,
                              X2.vx_vect, Y2.vx_vect, X2.ui_size, form2, color2 );
                }

                void yAutoPlot( unsigned form, COLORREF color )
                {
                    enforceSize();
                    VQI_yAutoPlot( vx_vect, ui_size, form, color );
                }

                void y2AutoPlot( unsigned form1, COLORREF color1,
                        cvta Y2, unsigned form2, COLORREF color2 )
                {
                    enforceSize();
                    Y2.enforceSize();
                    VQI_y2AutoPlot( vx_vect, ui_size, form1, color1,
                             Y2.vx_vect, Y2.ui_size, form2, color2 );
                }

                void xyDataPlot( cvta X, unsigned form, COLORREF color )
                {
                    enforceSize();
                    enforceSize(X.ui_size);
                    VQI_xyDataPlot( X.vx_vect, vx_vect, ui_size, form, color );
                }

                void yDataPlot( unsigned form, COLORREF color )
                {
                    enforceSize();
                    VQI_yDataPlot( vx_vect, ui_size, form, color );
                }
            #endif   // __VGRAPH_H

	};


#ifdef UBVECOBJ_H
	inline void qiVecObj::accVUB(const ubVecObj& X)
	{
		enforceSize();
		VQI_accVUB(vx_vect,X.vx_vect,ui_size);
	}
#endif //UBVECOBJ_H

#ifdef USVECOBJ_H
	inline void qiVecObj::accVUS(const usVecObj& X)
	{
		enforceSize();
		VQI_accVUS(vx_vect,X.vx_vect,ui_size);
	}
#endif //USVECOBJ

#ifdef UVECOBJ_H
	inline void qiVecObj::accVU(const uVecObj& X)
	{
		enforceSize();
		VQI_accVU(vx_vect,X.vx_vect,ui_size);
	}
#endif //UVECOBJ_H

#ifdef ULVECOBJ_H
	inline void qiVecObj::accVUL(const ulVecObj& X)
	{
		enforceSize();
		VQI_accVUL(vx_vect,X.vx_vect,ui_size);
	}
#endif //ULVECOBJ_H

#ifdef BIVECOBJ_H
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
#endif //BIVECOBJ_H

#ifdef SIVECOBJ_H
	inline void siVecObj::o_QItoSI(const qiVecObj& X)
	{
		enforceSize();
		Vo_QItoSI(vx_vect,X.vx_vect,ui_size);
	}

	inline void siVecObj::QItoSI(const qiVecObj& X)
	{
		enforceSize();
		V_QItoSI(vx_vect,X.vx_vect,ui_size);
	}

	inline void qiVecObj::SItoQI(const siVecObj& X)
	{
		enforceSize();
		V_SItoQI(vx_vect,X.vx_vect,ui_size);
	}

	inline void qiVecObj::accVSI(const siVecObj& X)
	{
		enforceSize();
		VQI_accVSI(vx_vect,X.vx_vect,ui_size);
	}
#endif //SIVECOBJ_H

#ifdef IVECOBJ_H
	inline void iVecObj::o_QItoI(const qiVecObj& X)
	{
		enforceSize();
		Vo_QItoI(vx_vect,X.vx_vect,ui_size);
	}

	inline void iVecObj::QItoI(const qiVecObj& X)
	{
		enforceSize();
		V_QItoI(vx_vect,X.vx_vect,ui_size);
	}

	inline void qiVecObj::ItoQI(const iVecObj& X)
	{
		enforceSize();
		V_ItoQI(vx_vect,X.vx_vect,ui_size);
	}

	inline void qiVecObj::accVI(const iVecObj& X)
	{
		enforceSize();
		VQI_accVI(vx_vect,X.vx_vect,ui_size);
	}
#endif //IVECOBJ_H

#ifdef LIVECOBJ_H
	inline void liVecObj::o_QItoLI(const qiVecObj& X)
	{
		enforceSize();
		Vo_QItoLI(vx_vect,X.vx_vect,ui_size);
	}

	inline void liVecObj::QItoLI(const qiVecObj& X)
	{
		enforceSize();
		V_QItoLI(vx_vect,X.vx_vect,ui_size);
	}

	inline void qiVecObj::LItoQI(const liVecObj& X)
	{
		enforceSize();
		V_LItoQI(vx_vect,X.vx_vect,ui_size);
	}

	inline void qiVecObj::accVLI(const liVecObj& X)
	{
		enforceSize();
		VQI_accVLI(vx_vect,X.vx_vect,ui_size);
	}
#endif //LIVECOBJ_H

#ifdef FVECOBJ_H
	inline void fVecObj::accVQI(const qiVecObj& X)
	{
		enforceSize();
		VF_accVQI(vx_vect,X.vx_vect,ui_size);
	}

	inline void fVecObj::QItoF(const qiVecObj& X)
	{
		enforceSize();
		V_QItoF(vx_vect,X.vx_vect,ui_size);
	}

	inline int qiVecObj::roundtoQI(const fVecObj& X)
	{
		enforceSize();
		return VF_roundtoQI(vx_vect,X.vx_vect,ui_size);
	}

	inline int qiVecObj::floortoQI(const fVecObj& X)
	{
		enforceSize();
		return VF_floortoQI(vx_vect,X.vx_vect,ui_size);
	}

	inline int qiVecObj::ceiltoQI(const fVecObj& X)
	{
		enforceSize();
		return VF_ceiltoQI(vx_vect,X.vx_vect,ui_size);
	}

	inline int qiVecObj::choptoQI(const fVecObj& X)
	{
		enforceSize();
		return VF_choptoQI(vx_vect,X.vx_vect,ui_size);
	}
#endif //FVECOBJ_H

#ifdef DVECOBJ_H
	inline void dVecObj::accVQI(const qiVecObj& X)
	{
		enforceSize();
		VD_accVQI(vx_vect,X.vx_vect,ui_size);
	}

	inline void dVecObj::QItoD(const qiVecObj& X)
	{
		enforceSize();
		V_QItoD(vx_vect,X.vx_vect,ui_size);
	}

	inline int qiVecObj::roundtoQI(const dVecObj& X)
	{
		enforceSize();
		return VD_roundtoQI(vx_vect,X.vx_vect,ui_size);
	}

	inline int qiVecObj::floortoQI(const dVecObj& X)
	{
		enforceSize();
		return VD_floortoQI(vx_vect,X.vx_vect,ui_size);
	}

	inline int qiVecObj::ceiltoQI(const dVecObj& X)
	{
		enforceSize();
		return VD_ceiltoQI(vx_vect,X.vx_vect,ui_size);
	}

	inline int qiVecObj::choptoQI(const dVecObj& X)
	{
		enforceSize();
		return VD_choptoQI(vx_vect,X.vx_vect,ui_size);
	}
#endif //DVECOBJ_H

#ifdef EVECOBJ_H
#ifdef __BORLANDC__ //80-bit numbers supported
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

#define QIVECOBJ_H
#endif //QIVECOBJ_H
