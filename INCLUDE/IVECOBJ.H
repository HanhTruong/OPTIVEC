/* iVecObj.H

  object-oriented interface for OptiVec vector functions of type "int"

  This file was written by Brian Dale, Case Western Reserve Univ.

  Copyright (c) 1996-2002 by OptiCode
  All Rights Reserved.
*/

#ifndef IVECOBJ_H

#include <VecObj.h>

#define cvta const vector<int>&
#define cuia const ui&
#define cta  const int&

#define vta  vector<int>&
#define uia  ui&
#define ta   int&

#define vt   vector<int>
#define t    int
#define tVector iVector

#define tVecObj iVecObj
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
		{	if(size){	vx_vect= VI_vector(size);	ui_size= size;}
			else{	vx_vect= 0;	ui_size= 0;}}

		vector(cuia size, cta fill)
		{	if(!size) //size == 0
			{	vx_vect=0;	ui_size=0;}
			else if(!fill) //fill == 0.0
			{	vx_vect= VI_vector0(size);	ui_size= size;}
			else
			{	vx_vect= VI_vector(size);	ui_size= size;	VI_equC(vx_vect,ui_size,fill);}}

		vector(cvta init)
		{	if(init.ui_size)
			{ vx_vect= VI_vector(init.ui_size);	ui_size= init.ui_size;	VI_equV(vx_vect, init.vx_vect, ui_size);}
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
				vx_vect= VI_vector(size);
				ui_size= size;}}

		void setSize(cvta X)
		{	if(!X.ui_size) free(); //can set size to zero
			if(ui_size != X.ui_size) //only allocate if different sizes
			{	if(vx_vect) V_free(vx_vect);
				vx_vect= VI_vector(X.ui_size);
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
			VI_equV(vx_vect, vect.vx_vect, ui_size);
			return *this;}

		vta operator=(cta scalar)
		{	enforceSize();
			VI_equC(vx_vect, ui_size, scalar);
			return *this;}

		//this is the only really dangerous operator
		ta operator[](const int& index){return vx_vect[index];}

		vta operator+=(cvta X)
		{	enforceSize();
			VI_accV(vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator+=(cta scalar)
		{	enforceSize();
			VI_addC(vx_vect, vx_vect, ui_size, scalar);
			return *this;}

		vta operator*=(cvta X)
		{	enforceSize();
			VI_mulV(vx_vect, vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator*=(cta scalar)
		{	enforceSize();
			VI_mulC(vx_vect,vx_vect,ui_size,scalar);
			return *this;}

		vt operator+(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VI_addV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator+(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VI_addC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator+(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VI_addC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator-(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VI_subV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator-(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VI_subC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator-(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VI_subrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator*(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VI_mulV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator*(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VI_mulC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator*(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VI_mulC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator/(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VI_divV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator/(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VI_divC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator/(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VI_divrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
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
			if(size) vx_vect= VI_vector(size);
			ui_size=size;
		}

		void vector0(ui size)
		{
			free();
			if(size) vx_vect= VI_vector0(size);
			ui_size=size;
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Addressing single vector elements
//
		t* Pelement(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VI_Pelement(vx_vect,n);
			//not in funcref
		}

		t element(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VI_element(vx_vect,n);
			//not in funcref
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Initialization
//

		void equ0()
		{
			enforceSize();
			VI_equ0(vx_vect,ui_size);
		}

		void equC(cta C)
		{
			enforceSize();
			VI_equC(vx_vect, ui_size, C);
		}

		void equV(cvta X)
		{
			enforceSize();
			VI_equV(vx_vect, X.vx_vect, ui_size);
		}

		void o_ramp(cta Start, cta Rise)
		{
			enforceSize();
			VIo_ramp(vx_vect, ui_size, Start, Rise);
		}

		void ramp(cta Start, cta Rise)
		{
			enforceSize();
			VI_ramp(vx_vect, ui_size, Start, Rise);
		}

		long random(const long& seed, cta Min, cta Max)
		{
			enforceSize();
			return VI_random(vx_vect, ui_size, seed, Min, Max);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Data-type interconversions
//
		//defined by including uVecObj.h
		void o_UtoI(const uVecObj& X);

		//defined by including uVecObj.h
		void UtoI(const uVecObj& X);

		//defined by including biVecObj.h
		void BItoI(const biVecObj& X);

		//defined by including siVecObj.h
		void SItoI(const siVecObj& X);

		//defined by including liVecObj.h
		void o_LItoI(const liVecObj& X);

		//defined by including liVecObj.h
		void LItoI(const liVecObj& X);

		//defined by including qiVecObj.h
		void o_QItoI(const qiVecObj& X);

		//defined by including qiVecObj.h
		void QItoI(const qiVecObj& X);


/////////////////////////////////////////////////////////////////////////////////////////
//Index-oriented manipulations
//
		void reflect()
		{
			enforceSize();
			VI_reflect(vx_vect, ui_size);
		}

		void rev(cvta X)
		{
			enforceSize();
			VI_rev(vx_vect, X.vx_vect, ui_size);
		}

#ifdef V_HUGE
		void rotate( cvta X, long pos )
#else
		void rotate( cvta X, int pos )
#endif
		{
			enforceSize();
			VI_rotate(vx_vect, X.vx_vect, ui_size, pos);
		}


		void _delete(ui pos)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VI_delete(vx_vect, ui_size, pos);
		}

		void insert(ui pos, cta C)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VI_insert(vx_vect, ui_size, pos, C);
		}

		void sort(cvta X, int dir=1)
		{
			enforceSize();
			VI_sort(vx_vect,X.vx_vect,ui_size,dir);
		}

		//sortind member of uiVecObj

		void subvector(cvta X, int step, ui start=0)
		{
			enforceSize();
			if(vx_vect==X.vx_vect) throw OptiVec_Err();
			VI_subvector(vx_vect, ui_size, X.vx_vect+start, step);
		}

		void subvector_equC(const unsigned& samp, cta C)
		{
			enforceSize();
			VI_subvector_equC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_equV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VI_subvector_equV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void indpick(const vector<ui>& Ind, cvta X);
		void indput(cvta X, const vector<ui>& Ind);

		ui searchC(cta C, int mode=0)
		{
			enforceSize();
			return VI_searchC(vx_vect, ui_size, C, mode);
		}

		//searchV member of uiVecObj


/////////////////////////////////////////////////////////////////////////////////////////
//Functions of a sub-set of elements
//


/////////////////////////////////////////////////////////////////////////////////////////
//One-Dimensional Vector Operations
//

		t o_sum()
		{
			enforceSize();
			return VIo_sum(vx_vect, ui_size);
		}

		void o_runsum(cvta X)
		{
			enforceSize();
			VIo_runsum(vx_vect, X.vx_vect, ui_size);
		}

		t sum()
		{
			enforceSize();
			return VI_sum(vx_vect, ui_size);
		}

		void runsum(cvta X)
		{
			enforceSize();
			VI_runsum(vx_vect, X.vx_vect, ui_size);
		}

		//underscore needed because max defined in windef.h
		t _max()
		{
			enforceSize();
			return VI_max(vx_vect, ui_size);
		}

		//underscore needed because min defined in windef.h
		t _min()
		{
			enforceSize();
			return VI_min(vx_vect, ui_size);
		}

		t maxind(ui* Ind)
		{
			enforceSize();
			return VI_maxind(Ind, vx_vect, ui_size);
		}

		t minind(ui* Ind)
		{
			enforceSize();
			return VI_minind(Ind, vx_vect, ui_size);
		}

		void runmax(cvta X)
		{
			enforceSize();
			VI_runmax(vx_vect, X.vx_vect, ui_size);
		}

		void runmin(cvta X)
		{
			enforceSize();
			VI_runmin(vx_vect, X.vx_vect, ui_size);
		}


		double fsum() const
		{
			enforceSize();
			return VI_fsum(vx_vect,ui_size);
		}

		double mean() const
		{
			enforceSize();
			return VI_mean(vx_vect,ui_size);
		}

		int iselementC(cta C)
		{
			enforceSize();
			return VI_iselementC(vx_vect, ui_size, C);
		}

		ui iselementV(cvta X, cvta Tab)
		{
			enforceSize();
			return VI_iselementV(vx_vect, X.vx_vect,
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
			VI_fprint(stream,vx_vect,ui_size,nperline,linewidth);
		}

#if !defined _Windows || defined __FLAT__ || defined _WIN32
		void cprint(unsigned nperline) const
		{
			enforceSize();
			VI_cprint(vx_vect,ui_size,nperline);
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
			VI_read(vx_vect,ui_size,stream);
		}

		void write(FILE* stream) const
		{
			enforceSize();
			VI_write(stream,vx_vect,ui_size);
		}

//nread
//nwrite

		void setWriteFormat(char* formatString) const
		{
			VI_setWriteFormat(formatString);
		}

		void setWriteSeparate(char* sepString) const
		{
			VI_setWriteSeparate(sepString);
		}

//setNWriteSeparate

		void store(FILE* stream) const
		{
			enforceSize();
			fwrite(&ui_size,sizeof(ui),1,stream);
			VI_store(stream,vx_vect,ui_size);
		}

		void recall(FILE* stream)
		{
			ui sz;
			fread(&sz,sizeof(ui),1,stream);
			setSize(sz);
			VI_recall(vx_vect,ui_size,stream);
		}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//from VXmath.h
//


/////////////////////////////////////////////////////////////////////////////////////////
//Rounding
//
		//defined by including fVecObj.h
		int roundtoI(const fVecObj& X);

		//defined by including fVecObj.h
		int floortoI(const fVecObj& X);
		
		//defined by including fVecObj.h
		int ceiltoI(const fVecObj& X);
		
		//defined by including fVecObj.h
		int choptoI(const fVecObj& X);
		
		int trunctoI(const fVecObj& X)
		{
			return choptoI(X);
		}
		
		//defined by including dVecObj.h
		int roundtoI(const dVecObj& X);
		
		//defined by including dVecObj.h
		int floortoI(const dVecObj& X);
		
		//defined by including dVecObj.h
		int ceiltoI(const dVecObj& X);
		
		//defined by including dVecObj.h
		int choptoI(const dVecObj& X);
		
		int trunctoI(const dVecObj& X)
		{
			return choptoI(X);
		}
		
#ifdef __BORLANDC__ //80-bit numbers supported
		//defined by including eVecObj.h
		int roundtoI(const eVecObj& X);
		
		//defined by including eVecObj.h
		int floortoI(const eVecObj& X);
		
		//defined by including eVecObj.h
		int ceiltoI(const eVecObj& X);
		
		//defined by including eVecObj.h
		int choptoI(const eVecObj& X);
		
		int trunctoI(const eVecObj& X)
		{
			return choptoI(X);
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
			VI_neg(vx_vect, X.vx_vect, ui_size);
		}

		void abs(cvta X)
		{
			enforceSize();
			VI_abs(vx_vect, X.vx_vect, ui_size);
		}

		void o_addC(cvta X, cta C)
		{
			enforceSize();
			VIo_addC(vx_vect, X.vx_vect, ui_size, C);
		}

		void o_subC(cvta X, cta C)
		{
			enforceSize();
			VIo_subC(vx_vect, X.vx_vect, ui_size, C);
		}

		void o_subrC(cvta X, cta C)
		{
			enforceSize();
			VIo_subrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void o_mulC(cvta X, cta C)
		{
			enforceSize();
			VIo_mulC(vx_vect, X.vx_vect, ui_size, C);
		}

		void addC(cvta X, cta C)
		{
			enforceSize();
			VI_addC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subC(cvta X, cta C)
		{
			enforceSize();
			VI_subC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subrC(cvta X, cta C)
		{
			enforceSize();
			VI_subrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void mulC(cvta X, cta C)
		{
			enforceSize();
			VI_mulC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divC(cvta X, cta C)
		{
			enforceSize();
			VI_divC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divrC(cvta X, cta C)
		{
			enforceSize();
			VI_divrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void modC(cvta X, cta C)
		{
			enforceSize();
			VI_modC(vx_vect, X.vx_vect, ui_size, C);
		}

		void maxC(cvta X, cta C)
		{
			enforceSize();
			VI_maxC( vx_vect, X.vx_vect, ui_size, C);
		}

		void minC(cvta X, cta C)
		{
			enforceSize();
			VI_minC( vx_vect, X.vx_vect, ui_size, C);
		}

		void o_addV(cvta X, cvta Y)
		{
			enforceSize();
			VIo_addV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void o_subV(cvta X, cvta Y)
		{
			enforceSize();
			VIo_subV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void o_subrV(cvta X, cvta Y)
		{
			enforceSize();
			VIo_subrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void o_mulV(cvta X, cvta Y)
		{
			enforceSize();
			VIo_mulV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void addV(cvta X, cvta Y)
		{
			enforceSize();
			VI_addV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subV(cvta X, cvta Y)
		{
			enforceSize();
			VI_subV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subrV(cvta X, cvta Y)
		{
			enforceSize();
			VI_subrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void mulV(cvta X, cvta Y)
		{
			enforceSize();
			VI_mulV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divV(cvta X, cvta Y)
		{
			enforceSize();
			VI_divV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divrV(cvta X, cvta Y)
		{
			enforceSize();
			VI_divrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void modV(cvta X, cvta Y)
		{
			enforceSize();
			VI_modV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void maxV(cvta X, cvta Y)
		{
			enforceSize();
			VI_maxV( vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void minV(cvta X, cvta Y)
		{
			enforceSize();
			VI_minV( vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Accumulation
//
		void accV(cvta X)
		{
			enforceSize();
			VI_accV(vx_vect,X.vx_vect,ui_size);
		}

		//defined by including biVecObj.h
		void accVBI(const biVecObj& X);

		//defined by including siVecObj.h
		void accVSI(const siVecObj& X);


/////////////////////////////////////////////////////////////////////////////////////////
//Bit-wise operations
//
		void shl(cvta X, unsigned C)
		{
			enforceSize();
			VI_shl(vx_vect,X.vx_vect,ui_size,C);
		}

		void shr(cvta X, unsigned C)
		{
			enforceSize();
			VI_shr(vx_vect,X.vx_vect,ui_size,C);
		}

		void and(cvta X, cta C)
		{
			enforceSize();
			VI_and(vx_vect,X.vx_vect,ui_size,C);
		}

		void or(cvta X, cta C)
		{
			enforceSize();
			VI_or(vx_vect,X.vx_vect,ui_size,C);
		}

		void xor(cvta X, cta C)
		{
			enforceSize();
			VI_xor(vx_vect,X.vx_vect,ui_size,C);
		}

		void not(cvta X)
		{
			enforceSize();
			VI_not(vx_vect,X.vx_vect,ui_size);
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
                    VI_xyAutoPlot( X.vx_vect, vx_vect, ui_size, form, color );
                }

                void xy2AutoPlot( cvta X1, unsigned form1, COLORREF color1,
                         cvta X2, cvta Y2, unsigned form2, COLORREF color2 )
                {
                    enforceSize();
                    enforceSize(X1.ui_size);
                    Y2.enforceSize(X2.ui_size);
                    VI_xy2AutoPlot( X1.vx_vect, vx_vect, ui_size, form1, color1,
                              X2.vx_vect, Y2.vx_vect, X2.ui_size, form2, color2 );
                }

                void yAutoPlot( unsigned form, COLORREF color )
                {
                    enforceSize();
                    VI_yAutoPlot( vx_vect, ui_size, form, color );
                }

                void y2AutoPlot( unsigned form1, COLORREF color1,
                        cvta Y2, unsigned form2, COLORREF color2 )
                {
                    enforceSize();
                    Y2.enforceSize();
                    VI_y2AutoPlot( vx_vect, ui_size, form1, color1,
                             Y2.vx_vect, Y2.ui_size, form2, color2 );
                }

                void xyDataPlot( cvta X, unsigned form, COLORREF color )
                {
                    enforceSize();
                    enforceSize(X.ui_size);
                    VI_xyDataPlot( X.vx_vect, vx_vect, ui_size, form, color );
                }

                void yDataPlot( unsigned form, COLORREF color )
                {
                    enforceSize();
                    VI_yDataPlot( vx_vect, ui_size, form, color );
                }
            #endif   // __VGRAPH_H

	};


#ifdef UVECOBJ_H
	inline void uVecObj::o_ItoU(const iVecObj& X)
	{
		enforceSize();
		Vo_ItoU(vx_vect,X.vx_vect,ui_size);
	}

	inline void iVecObj::o_UtoI(const uVecObj& X)
	{
		enforceSize();
		Vo_UtoI(vx_vect,X.vx_vect,ui_size);
	}

	inline void uVecObj::ItoU(const iVecObj& X)
	{
		enforceSize();
		V_ItoU(vx_vect,X.vx_vect,ui_size);
	}

	inline void iVecObj::UtoI(const uVecObj& X)
	{
		enforceSize();
		V_UtoI(vx_vect,X.vx_vect,ui_size);
	}
#endif //UVECOBJ_H

#ifdef BIVECOBJ_H
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
#endif //BIVECOBJ_H

#ifdef SIVECOBJ_H
	inline void siVecObj::o_ItoSI(const iVecObj& X)
	{
		enforceSize();
		Vo_ItoSI(vx_vect,X.vx_vect,ui_size);
	}

	inline void siVecObj::ItoSI(const iVecObj& X)
	{
		enforceSize();
		V_ItoSI(vx_vect,X.vx_vect,ui_size);
	}

	inline void iVecObj::SItoI(const siVecObj& X)
	{
		enforceSize();
		V_SItoI(vx_vect,X.vx_vect,ui_size);
	}

	inline void iVecObj::accVSI(const siVecObj& X)
	{
		enforceSize();
		VI_accVSI(vx_vect,X.vx_vect,ui_size);
	}
#endif //SIVECOBJ_H

#ifdef LIVECOBJ_H
	inline void iVecObj::o_LItoI(const liVecObj& X)
	{
		enforceSize();
		Vo_LItoI(vx_vect,X.vx_vect,ui_size);
	}

	inline void iVecObj::LItoI(const liVecObj& X)
	{
		enforceSize();
		V_LItoI(vx_vect,X.vx_vect,ui_size);
	}

	inline void liVecObj::ItoLI(const iVecObj& X)
	{
		enforceSize();
		V_ItoLI(vx_vect,X.vx_vect,ui_size);
	}

	inline void liVecObj::accVI(const iVecObj& X)
	{
		enforceSize();
		VLI_accVI(vx_vect,X.vx_vect,ui_size);
	}
#endif //LIVECOBJ_H

#ifdef QIVECOBJ_H
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
#endif //QIVECOBJ_H

#ifdef FVECOBJ_H
	inline void fVecObj::accVI(const iVecObj& X)
	{
		enforceSize();
		VF_accVI(vx_vect,X.vx_vect,ui_size);
	}

	inline void fVecObj::ItoF(const iVecObj& X)
	{
		enforceSize();
		V_ItoF(vx_vect,X.vx_vect,ui_size);
	}

	inline int iVecObj::roundtoI(const fVecObj& X)
	{
		enforceSize();
		return VF_roundtoI(vx_vect,X.vx_vect,ui_size);
	}

	inline int iVecObj::floortoI(const fVecObj& X)
	{
		enforceSize();
		return VF_floortoI(vx_vect,X.vx_vect,ui_size);
	}

	inline int iVecObj::ceiltoI(const fVecObj& X)
	{
		enforceSize();
		return VF_ceiltoI(vx_vect,X.vx_vect,ui_size);
	}

	inline int iVecObj::choptoI(const fVecObj& X)
	{
		enforceSize();
		return VF_choptoI(vx_vect,X.vx_vect,ui_size);
	}

        inline int fVecObj::mantexp(iVecObj* Exp, const fVecObj& X)
	{
		enforceSize();
		if(ui_size != Exp->ui_size) throw OptiVec_SizeErr();
		return VF_mantexp(vx_vect,Exp->vx_vect,X.vx_vect,ui_size);
	}

        inline int fVecObj::mantexp(iVecObj Exp, const fVecObj& X)
	{
		enforceSize();
                if(ui_size != Exp.ui_size) throw OptiVec_SizeErr();
                return VF_mantexp(vx_vect,Exp.vx_vect,X.vx_vect,ui_size);
	}

	inline int fVecObj::ipow2(const iVecObj& X)
	{
		enforceSize();
		return VF_ipow2(vx_vect,X.vx_vect,ui_size);
	}

	inline int fVecObj::ipow10(const iVecObj& X)
	{
		enforceSize();
		return VF_ipow10(vx_vect,X.vx_vect,ui_size);
	}

	inline int fVecObj::sinrpi(const iVecObj& P, int q)
	{
		enforceSize();
		return VF_sinrpi(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::cosrpi(const iVecObj& P, int q)
	{
		enforceSize();
		return VF_cosrpi(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::sincosrpi(fVecObj* Cos, const iVecObj& P, int q)
	{
		enforceSize();
		if(ui_size != Cos->ui_size) throw OptiVec_SizeErr();
		return VF_sincosrpi(vx_vect,Cos->vx_vect,P.vx_vect,ui_size,q);
	}

        inline int fVecObj::sincosrpi(fVecObj Cos, const iVecObj& P, int q)
	{
		enforceSize();
                if(ui_size != Cos.ui_size) throw OptiVec_SizeErr();
                return VF_sincosrpi(vx_vect,Cos.vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::tanrpi(const iVecObj& P, int q)
	{
		enforceSize();
		return VF_tanrpi(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::cotrpi(const iVecObj& P, int q)
	{
		enforceSize();
		return VF_cotrpi(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::secrpi(const iVecObj& P, int q)
	{
		enforceSize();
		return VF_secrpi(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::cosecrpi(const iVecObj& P, int q)
	{
		enforceSize();
		return VF_cosecrpi(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::sinrpi2(const iVecObj& P, int q)
	{
		enforceSize();
		return VF_sinrpi2(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::cosrpi2(const iVecObj& P, int q)
	{
		enforceSize();
		return VF_cosrpi2(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::sincosrpi2(fVecObj* Cos, const iVecObj& P, int q)
	{
		enforceSize();
		if(ui_size != Cos->ui_size) throw OptiVec_SizeErr();
		return VF_sincosrpi2(vx_vect,Cos->vx_vect,P.vx_vect,ui_size,q);
	}

        inline int fVecObj::sincosrpi2(fVecObj Cos, const iVecObj& P, int q)
	{
		enforceSize();
                if(ui_size != Cos.ui_size) throw OptiVec_SizeErr();
                return VF_sincosrpi2(vx_vect,Cos.vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::tanrpi2(const iVecObj& P, int q)
	{
		enforceSize();
		return VF_tanrpi2(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::cotrpi2(const iVecObj& P, int q)
	{
		enforceSize();
		return VF_cotrpi2(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::secrpi2(const iVecObj& P, int q)
	{
		enforceSize();
		return VF_secrpi2(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::cosecrpi2(const iVecObj& P, int q)
	{
		enforceSize();
		return VF_cosecrpi2(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::sinrpi3(const iVecObj& P, int q)
	{
		enforceSize();
		return VF_sinrpi3(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::cosrpi3(const iVecObj& P, int q)
	{
		enforceSize();
		return VF_cosrpi3(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::sincosrpi3(fVecObj* Cos, const iVecObj& P, int q)
	{
		enforceSize();
		if(ui_size != Cos->ui_size) throw OptiVec_SizeErr();
		return VF_sincosrpi3(vx_vect,Cos->vx_vect,P.vx_vect,ui_size,q);
	}

        inline int fVecObj::sincosrpi3(fVecObj Cos, const iVecObj& P, int q)
	{
		enforceSize();
                if(ui_size != Cos.ui_size) throw OptiVec_SizeErr();
                return VF_sincosrpi3(vx_vect,Cos.vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::tanrpi3(const iVecObj& P, int q)
	{
		enforceSize();
		return VF_tanrpi3(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::cotrpi3(const iVecObj& P, int q)
	{
		enforceSize();
		return VF_cotrpi3(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::secrpi3(const iVecObj& P, int q)
	{
		enforceSize();
		return VF_secrpi3(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int fVecObj::cosecrpi3(const iVecObj& P, int q)
	{
		enforceSize();
		return VF_cosecrpi3(vx_vect,P.vx_vect,ui_size,q);
	}
#endif //FVECOBJ_H

#ifdef DVECOBJ_H
	inline void dVecObj::accVI(const iVecObj& X)
	{
		enforceSize();
		VD_accVI(vx_vect,X.vx_vect,ui_size);
	}

	inline void dVecObj::ItoD(const iVecObj& X)
	{
		enforceSize();
		V_ItoD(vx_vect,X.vx_vect,ui_size);
	}

	inline int iVecObj::roundtoI(const dVecObj& X)
	{
		enforceSize();
		return VD_roundtoI(vx_vect,X.vx_vect,ui_size);
	}

	inline int iVecObj::floortoI(const dVecObj& X)
	{
		enforceSize();
		return VD_floortoI(vx_vect,X.vx_vect,ui_size);
	}

	inline int iVecObj::ceiltoI(const dVecObj& X)
	{
		enforceSize();
		return VD_ceiltoI(vx_vect,X.vx_vect,ui_size);
	}

	inline int iVecObj::choptoI(const dVecObj& X)
	{
		enforceSize();
		return VD_choptoI(vx_vect,X.vx_vect,ui_size);
	}

        inline int dVecObj::mantexp(iVecObj* Exp, const dVecObj& X)
	{
		enforceSize();
		if(ui_size != Exp->ui_size) throw OptiVec_SizeErr();
		return VD_mantexp(vx_vect,Exp->vx_vect,X.vx_vect,ui_size);
	}

        inline int dVecObj::mantexp(iVecObj Exp, const dVecObj& X)
	{
		enforceSize();
                if(ui_size != Exp.ui_size) throw OptiVec_SizeErr();
                return VD_mantexp(vx_vect,Exp.vx_vect,X.vx_vect,ui_size);
	}

	inline int dVecObj::ipow2(const iVecObj& X)
	{
		enforceSize();
		return VD_ipow2(vx_vect,X.vx_vect,ui_size);
	}

	inline int dVecObj::ipow10(const iVecObj& X)
	{
		enforceSize();
		return VD_ipow10(vx_vect,X.vx_vect,ui_size);
	}

	inline int dVecObj::sinrpi(const iVecObj& P, int q)
	{
		enforceSize();
		return VD_sinrpi(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::cosrpi(const iVecObj& P, int q)
	{
		enforceSize();
		return VD_cosrpi(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::sincosrpi(dVecObj* Cos, const iVecObj& P, int q)
	{
		enforceSize();
		if(ui_size != Cos->ui_size) throw OptiVec_SizeErr();
		return VD_sincosrpi(vx_vect,Cos->vx_vect,P.vx_vect,ui_size,q);
	}

        inline int dVecObj::sincosrpi(dVecObj Cos, const iVecObj& P, int q)
	{
		enforceSize();
                if(ui_size != Cos.ui_size) throw OptiVec_SizeErr();
                return VD_sincosrpi(vx_vect,Cos.vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::tanrpi(const iVecObj& P, int q)
	{
		enforceSize();
		return VD_tanrpi(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::cotrpi(const iVecObj& P, int q)
	{
		enforceSize();
		return VD_cotrpi(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::secrpi(const iVecObj& P, int q)
	{
		enforceSize();
		return VD_secrpi(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::cosecrpi(const iVecObj& P, int q)
	{
		enforceSize();
		return VD_cosecrpi(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::sinrpi2(const iVecObj& P, int q)
	{
		enforceSize();
		return VD_sinrpi2(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::cosrpi2(const iVecObj& P, int q)
	{
		enforceSize();
		return VD_cosrpi2(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::sincosrpi2(dVecObj* Cos, const iVecObj& P, int q)
	{
		enforceSize();
		if(ui_size != Cos->ui_size) throw OptiVec_SizeErr();
		return VD_sincosrpi2(vx_vect,Cos->vx_vect,P.vx_vect,ui_size,q);
	}

        inline int dVecObj::sincosrpi2(dVecObj Cos, const iVecObj& P, int q)
	{
		enforceSize();
                if(ui_size != Cos.ui_size) throw OptiVec_SizeErr();
                return VD_sincosrpi2(vx_vect,Cos.vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::tanrpi2(const iVecObj& P, int q)
	{
		enforceSize();
		return VD_tanrpi2(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::cotrpi2(const iVecObj& P, int q)
	{
		enforceSize();
		return VD_cotrpi2(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::secrpi2(const iVecObj& P, int q)
	{
		enforceSize();
		return VD_secrpi2(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::cosecrpi2(const iVecObj& P, int q)
	{
		enforceSize();
		return VD_cosecrpi2(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::sinrpi3(const iVecObj& P, int q)
	{
		enforceSize();
		return VD_sinrpi3(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::cosrpi3(const iVecObj& P, int q)
	{
		enforceSize();
		return VD_cosrpi3(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::sincosrpi3(dVecObj* Cos, const iVecObj& P, int q)
	{
		enforceSize();
		if(ui_size != Cos->ui_size) throw OptiVec_SizeErr();
		return VD_sincosrpi3(vx_vect,Cos->vx_vect,P.vx_vect,ui_size,q);
	}

        inline int dVecObj::sincosrpi3(dVecObj Cos, const iVecObj& P, int q)
	{
		enforceSize();
                if(ui_size != Cos.ui_size) throw OptiVec_SizeErr();
                return VD_sincosrpi3(vx_vect,Cos.vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::tanrpi3(const iVecObj& P, int q)
	{
		enforceSize();
		return VD_tanrpi3(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::cotrpi3(const iVecObj& P, int q)
	{
		enforceSize();
		return VD_cotrpi3(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::secrpi3(const iVecObj& P, int q)
	{
		enforceSize();
		return VD_secrpi3(vx_vect,P.vx_vect,ui_size,q);
	}

	inline int dVecObj::cosecrpi3(const iVecObj& P, int q)
	{
		enforceSize();
		return VD_cosecrpi3(vx_vect,P.vx_vect,ui_size,q);
	}
#endif //DVECOBJ_H

#ifdef EVECOBJ_H
#ifdef __BORLANDC__ //80-bit numbers supported
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

#define IVECOBJ_H
#endif //IVECOBJ_H
