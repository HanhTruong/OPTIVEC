/* liVecObj.H

  object-oriented interface for OptiVec vector functions of type "long int"

  This file was written by Brian Dale, Case Western Reserve Univ.

  Copyright (c) 1996-2002 by OptiCode
  All Rights Reserved.
*/

#ifndef LIVECOBJ_H

#include <VecObj.h>

#define cvta const vector<long>&
#define cuia const ui&
#define cta  const long&

#define vta  vector<long>&
#define uia  ui&
#define ta   long&

#define vt   vector<long>
#define t    long
#define tVector liVector

#define tVecObj liVecObj
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
		{	if(size){	vx_vect= VLI_vector(size);	ui_size= size;}
			else{	vx_vect= 0;	ui_size= 0;}}

		vector(cuia size, cta fill)
		{	if(!size) //size == 0
			{	vx_vect=0;	ui_size=0;}
			else if(!fill) //fill == 0.0
			{	vx_vect= VLI_vector0(size);	ui_size= size;}
			else
			{	vx_vect= VLI_vector(size);	ui_size= size;	VLI_equC(vx_vect,ui_size,fill);}}

		vector(cvta init)
		{	if(init.ui_size)
			{ vx_vect= VLI_vector(init.ui_size);	ui_size= init.ui_size;	VLI_equV(vx_vect, init.vx_vect, ui_size);}
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
				vx_vect= VLI_vector(size);
				ui_size= size;}}

		void setSize(cvta X)
		{	if(!X.ui_size) free(); //can set size to zero
			if(ui_size != X.ui_size) //only allocate if different sizes
			{	if(vx_vect) V_free(vx_vect);
				vx_vect= VLI_vector(X.ui_size);
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
			VLI_equV(vx_vect, vect.vx_vect, ui_size);
			return *this;}

		vta operator=(cta scalar)
		{	enforceSize();
			VLI_equC(vx_vect, ui_size, scalar);
			return *this;}

		//this is the only really dangerous operator
		ta operator[](const int& index){return vx_vect[index];}

		vta operator+=(cvta X)
		{	enforceSize();
			VLI_accV(vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator+=(cta scalar)
		{	enforceSize();
			VLI_addC(vx_vect, vx_vect, ui_size, scalar);
			return *this;}

		vta operator*=(cvta X)
		{	enforceSize();
			VLI_mulV(vx_vect, vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator*=(cta scalar)
		{	enforceSize();
			VLI_mulC(vx_vect,vx_vect,ui_size,scalar);
			return *this;}

		vt operator+(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VLI_addV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator+(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VLI_addC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator+(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VLI_addC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator-(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VLI_subV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator-(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VLI_subC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator-(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VLI_subrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator*(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VLI_mulV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator*(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VLI_mulC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator*(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VLI_mulC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator/(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VLI_divV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator/(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VLI_divC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator/(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VLI_divrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
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
			if(size) vx_vect= VLI_vector(size);
			ui_size=size;
		}

		void vector0(ui size)
		{
			free();
			if(size) vx_vect= VLI_vector0(size);
			ui_size=size;
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Addressing single vector elements
//
		t* Pelement(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VLI_Pelement(vx_vect,n);
			//not in funcref
		}

		t element(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VLI_element(vx_vect,n);
			//not in funcref
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Initialization
//

		void equ0()
		{
			enforceSize();
			VLI_equ0(vx_vect,ui_size);
		}

		void equC(cta C)
		{
			enforceSize();
			VLI_equC(vx_vect, ui_size, C);
		}

		void equV(cvta X)
		{
			enforceSize();
			VLI_equV(vx_vect, X.vx_vect, ui_size);
		}

		void o_ramp(cta Start, cta Rise)
		{
			enforceSize();
			VLIo_ramp(vx_vect, ui_size, Start, Rise);
		}

		void ramp(cta Start, cta Rise)
		{
			enforceSize();
			VLI_ramp(vx_vect, ui_size, Start, Rise);
		}

		long random(const long& seed, cta Min, cta Max)
		{
			enforceSize();
			return VLI_random(vx_vect, ui_size, seed, Min, Max);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Data-type interconversions
//
		//defined by including ulVecObj.h
		void o_ULtoLI(const ulVecObj& X);

		//defined by including ulVecObj.h
		void ULtoLI(const ulVecObj& X);

		//defined by including biVecObj.h
		void BItoLI(const biVecObj& X);

		//defined by including siVecObj.h
		void SItoLI(const siVecObj& X);

		//defined by including iVecObj.h
		void  ItoLI(const  iVecObj& X);
		
		//defined by including qiVecObj.h
		void o_QItoLI(const qiVecObj& X);

		//defined by including qiVecObj.h
		void QItoLI(const qiVecObj& X);


/////////////////////////////////////////////////////////////////////////////////////////
//Index-oriented manipulations
//
		void reflect()
		{
			enforceSize();
			VLI_reflect(vx_vect, ui_size);
		}

		void rev(cvta X)
		{
			enforceSize();
			VLI_rev(vx_vect, X.vx_vect, ui_size);
		}

#ifdef V_HUGE
		void rotate( cvta X, long pos )
#else
		void rotate( cvta X, int pos )
#endif
		{
			enforceSize();
			VLI_rotate(vx_vect, X.vx_vect, ui_size, pos);
		}


		void _delete(ui pos)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VLI_delete(vx_vect, ui_size, pos);
		}

		void insert(ui pos, cta C)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VLI_insert(vx_vect, ui_size, pos, C);
		}

		void sort(cvta X, int dir=1)
		{
			enforceSize();
			VLI_sort(vx_vect,X.vx_vect,ui_size,dir);
		}

		//sortind member of uiVecObj

		void subvector(cvta X, int step, ui start=0)
		{
			enforceSize();
			if(vx_vect==X.vx_vect) throw OptiVec_Err();
			VLI_subvector(vx_vect, ui_size, X.vx_vect+start, step);
		}

		void subvector_equC(const unsigned& samp, cta C)
		{
			enforceSize();
			VLI_subvector_equC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_equV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VLI_subvector_equV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void indpick(const vector<ui>& Ind, cvta X);
		void indput(cvta X, const vector<ui>& Ind);

		ui searchC(cta C, int mode=0)
		{
			enforceSize();
			return VLI_searchC(vx_vect, ui_size, C, mode);
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
			return VLIo_sum(vx_vect, ui_size);
		}

		void o_runsum(cvta X)
		{
			enforceSize();
			VLIo_runsum(vx_vect, X.vx_vect, ui_size);
		}

		t sum()
		{
			enforceSize();
			return VLI_sum(vx_vect, ui_size);
		}

		void runsum(cvta X)
		{
			enforceSize();
			VLI_runsum(vx_vect, X.vx_vect, ui_size);
		}

		t _max()
		{
			enforceSize();
			return VLI_max(vx_vect, ui_size);
		}

		t _min()
		{
			enforceSize();
			return VLI_min(vx_vect, ui_size);
		}

		t maxind(ui* Ind)
		{
			enforceSize();
			return VLI_maxind(Ind, vx_vect, ui_size);
		}

		t minind(ui* Ind)
		{
			enforceSize();
			return VLI_minind(Ind, vx_vect, ui_size);
		}

		void runmax(cvta X)
		{
			enforceSize();
			VLI_runmax(vx_vect, X.vx_vect, ui_size);
		}

		void runmin(cvta X)
		{
			enforceSize();
			VLI_runmin(vx_vect, X.vx_vect, ui_size);
		}

		double fsum() const
		{
			enforceSize();
			return VLI_fsum(vx_vect,ui_size);
		}

		double mean() const
		{
			enforceSize();
			return VLI_mean(vx_vect,ui_size);
		}

		int iselementC(cta C)
		{
			enforceSize();
			return VLI_iselementC(vx_vect, ui_size, C);
		}

		ui iselementV(cvta X, cvta Tab)
		{
			enforceSize();
			return VLI_iselementV(vx_vect, X.vx_vect,
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
			VLI_fprint(stream,vx_vect,ui_size,nperline,linewidth);
		}

#if !defined _Windows || defined __FLAT__ || defined _WIN32
		void cprint(unsigned nperline) const
		{
			enforceSize();
			VLI_cprint(vx_vect,ui_size,nperline);
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
			VLI_read(vx_vect,ui_size,stream);
		}

		void write(FILE* stream) const
		{
			enforceSize();
			VLI_write(stream,vx_vect,ui_size);
		}

//nread
//nwrite

		void setWriteFormat(char* formatString) const
		{
			VLI_setWriteFormat(formatString);
		}

		void setWriteSeparate(char* sepString) const
		{
			VLI_setWriteSeparate(sepString);
		}

//setNWriteSeparate

		void store(FILE* stream) const
		{
			enforceSize();
			fwrite(&ui_size,sizeof(ui),1,stream);
			VLI_store(stream,vx_vect,ui_size);
		}

		void recall(FILE* stream)
		{
			ui sz;
			fread(&sz,sizeof(ui),1,stream);
			setSize(sz);
			VLI_recall(vx_vect,ui_size,stream);
		}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//from VXmath.h
//


/////////////////////////////////////////////////////////////////////////////////////////
//Rounding
//
		//defined by including fVecObj.h
		int roundtoLI(const fVecObj& X);

		//defined by including fVecObj.h
		int floortoLI(const fVecObj& X);
		
		//defined by including fVecObj.h
		int ceiltoLI(const fVecObj& X);
		
		//defined by including fVecObj.h
		int choptoLI(const fVecObj& X);
		
		int trunctoLI(const fVecObj& X)
		{
			return choptoLI(X);
		}
		
		//defined by including dVecObj.h
		int roundtoLI(const dVecObj& X);
		
		//defined by including dVecObj.h
		int floortoLI(const dVecObj& X);
		
		//defined by including dVecObj.h
		int ceiltoLI(const dVecObj& X);
		
		//defined by including dVecObj.h
		int choptoLI(const dVecObj& X);
		
		int trunctoLI(const dVecObj& X)
		{
			return choptoLI(X);
		}
		
#ifdef __BORLANDC__ //80-bit numbers supported
		//defined by including eVecObj.h
		int roundtoLI(const eVecObj& X);
		
		//defined by including eVecObj.h
		int floortoLI(const eVecObj& X);
		
		//defined by including eVecObj.h
		int ceiltoLI(const eVecObj& X);
		
		//defined by including eVecObj.h
		int choptoLI(const eVecObj& X);
		
		int trunctoLI(const eVecObj& X)
		{
			return choptoLI(X);
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
			VLI_neg(vx_vect, X.vx_vect, ui_size);
		}

		void abs(cvta X)
		{
			enforceSize();
			VLI_abs(vx_vect, X.vx_vect, ui_size);
		}

		void o_addC(cvta X, cta C)
		{
			enforceSize();
			VLIo_addC(vx_vect, X.vx_vect, ui_size, C);
		}

		void o_subC(cvta X, cta C)
		{
			enforceSize();
			VLIo_subC(vx_vect, X.vx_vect, ui_size, C);
		}

		void o_subrC(cvta X, cta C)
		{
			enforceSize();
			VLIo_subrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void o_mulC(cvta X, cta C)
		{
			enforceSize();
			VLIo_mulC(vx_vect, X.vx_vect, ui_size, C);
		}

		void addC(cvta X, cta C)
		{
			enforceSize();
			VLI_addC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subC(cvta X, cta C)
		{
			enforceSize();
			VLI_subC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subrC(cvta X, cta C)
		{
			enforceSize();
			VLI_subrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void mulC(cvta X, cta C)
		{
			enforceSize();
			VLI_mulC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divC(cvta X, cta C)
		{
			enforceSize();
			VLI_divC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divrC(cvta X, cta C)
		{
			enforceSize();
			VLI_divrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void modC(cvta X, cta C)
		{
			enforceSize();
			VLI_modC(vx_vect, X.vx_vect, ui_size, C);
		}

		void maxC(cvta X, cta C)
		{
			enforceSize();
			VLI_maxC( vx_vect, X.vx_vect, ui_size, C);
		}

		void minC(cvta X, cta C)
		{
			enforceSize();
			VLI_minC( vx_vect, X.vx_vect, ui_size, C);
		}

		void o_addV(cvta X, cvta Y)
		{
			enforceSize();
			VLIo_addV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void o_subV(cvta X, cvta Y)
		{
			enforceSize();
			VLIo_subV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void o_subrV(cvta X, cvta Y)
		{
			enforceSize();
			VLIo_subrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void o_mulV(cvta X, cvta Y)
		{
			enforceSize();
			VLIo_mulV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void addV(cvta X, cvta Y)
		{
			enforceSize();
			VLI_addV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subV(cvta X, cvta Y)
		{
			enforceSize();
			VLI_subV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subrV(cvta X, cvta Y)
		{
			enforceSize();
			VLI_subrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void mulV(cvta X, cvta Y)
		{
			enforceSize();
			VLI_mulV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divV(cvta X, cvta Y)
		{
			enforceSize();
			VLI_divV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divrV(cvta X, cvta Y)
		{
			enforceSize();
			VLI_divrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void modV(cvta X, cvta Y)
		{
			enforceSize();
			VLI_modV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void maxV(cvta X, cvta Y)
		{
			enforceSize();
			VLI_maxV( vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void minV(cvta X, cvta Y)
		{
			enforceSize();
			VLI_minV( vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Accumulation
//
		void accV(cvta X)
		{
			enforceSize();
			VLI_accV(vx_vect,X.vx_vect,ui_size);
		}

		//defined by including biVecObj.h
		void accVBI(const biVecObj& X);

		//defined by including siVecObj.h
		void accVSI(const siVecObj& X);

		//defined by including iVecObj.h
		void accVI(const iVecObj& X);


/////////////////////////////////////////////////////////////////////////////////////////
//Bit-wise operations
//
		void shl(cvta X, unsigned C)
		{
			enforceSize();
			VLI_shl(vx_vect,X.vx_vect,ui_size,C);
		}

		void shr(cvta X, unsigned C)
		{
			enforceSize();
			VLI_shr(vx_vect,X.vx_vect,ui_size,C);
		}

		void and(cvta X, cta C)
		{
			enforceSize();
			VLI_and(vx_vect,X.vx_vect,ui_size,C);
		}

		void or(cvta X, cta C)
		{
			enforceSize();
			VLI_or(vx_vect,X.vx_vect,ui_size,C);
		}

		void xor(cvta X, cta C)
		{
			enforceSize();
			VLI_xor(vx_vect,X.vx_vect,ui_size,C);
		}

		void not(cvta X)
		{
			enforceSize();
			VLI_not(vx_vect,X.vx_vect,ui_size);
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
                    VLI_xyAutoPlot( X.vx_vect, vx_vect, ui_size, form, color );
                }

                void xy2AutoPlot( cvta X1, unsigned form1, COLORREF color1,
                         cvta X2, cvta Y2, unsigned form2, COLORREF color2 )
                {
                    enforceSize();
                    enforceSize(X1.ui_size);
                    Y2.enforceSize(X2.ui_size);
                    VLI_xy2AutoPlot( X1.vx_vect, vx_vect, ui_size, form1, color1,
                              X2.vx_vect, Y2.vx_vect, X2.ui_size, form2, color2 );
                }

                void yAutoPlot( unsigned form, COLORREF color )
                {
                    enforceSize();
                    VLI_yAutoPlot( vx_vect, ui_size, form, color );
                }

                void y2AutoPlot( unsigned form1, COLORREF color1,
                        cvta Y2, unsigned form2, COLORREF color2 )
                {
                    enforceSize();
                    Y2.enforceSize();
                    VLI_y2AutoPlot( vx_vect, ui_size, form1, color1,
                             Y2.vx_vect, Y2.ui_size, form2, color2 );
                }

                void xyDataPlot( cvta X, unsigned form, COLORREF color )
                {
                    enforceSize();
                    enforceSize(X.ui_size);
                    VLI_xyDataPlot( X.vx_vect, vx_vect, ui_size, form, color );
                }

                void yDataPlot( unsigned form, COLORREF color )
                {
                    enforceSize();
                    VLI_yDataPlot( vx_vect, ui_size, form, color );
                }
            #endif   // __VGRAPH_H

	};


#ifdef ULVECOBJ_H
	inline void ulVecObj::o_LItoUL(const liVecObj& X)
	{
		enforceSize();
		Vo_LItoUL(vx_vect,X.vx_vect,ui_size);
	}

	inline void liVecObj::o_ULtoLI(const ulVecObj& X)
	{
		enforceSize();
		Vo_ULtoLI(vx_vect,X.vx_vect,ui_size);
	}

	inline void ulVecObj::LItoUL(const liVecObj& X)
	{
		enforceSize();
		V_LItoUL(vx_vect,X.vx_vect,ui_size);
	}

	inline void liVecObj::ULtoLI(const ulVecObj& X)
	{
		enforceSize();
		V_ULtoLI(vx_vect,X.vx_vect,ui_size);
	}
#endif //ULVECOBJ_H

#ifdef BIVECOBJ_H
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
#endif //BIVECOBJ_H

#ifdef SIVECOBJ_H
	inline void siVecObj::o_LItoSI(const liVecObj& X)
	{
		enforceSize();
		Vo_LItoSI(vx_vect,X.vx_vect,ui_size);
	}

	inline void siVecObj::LItoSI(const liVecObj& X)
	{
		enforceSize();
		V_LItoSI(vx_vect,X.vx_vect,ui_size);
	}

	inline void liVecObj::SItoLI(const siVecObj& X)
	{
		enforceSize();
		V_SItoLI(vx_vect,X.vx_vect,ui_size);
	}

	inline void liVecObj::accVSI(const siVecObj& X)
	{
		enforceSize();
		VLI_accVSI(vx_vect,X.vx_vect,ui_size);
	}
#endif //SIVECOBJ_H

#ifdef IVECOBJ_H
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
#endif //IVECOBJ_H

#ifdef QIVECOBJ_H
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
#endif //QIVECOBJ_H

#ifdef FVECOBJ_H
	inline void fVecObj::accVLI(const liVecObj& X)
	{
		enforceSize();
		VF_accVLI(vx_vect,X.vx_vect,ui_size);
	}

	inline void fVecObj::LItoF(const liVecObj& X)
	{
		enforceSize();
		V_LItoF(vx_vect,X.vx_vect,ui_size);
	}

	inline int liVecObj::roundtoLI(const fVecObj& X)
	{
		enforceSize();
		return VF_roundtoLI(vx_vect,X.vx_vect,ui_size);
	}

	inline int liVecObj::floortoLI(const fVecObj& X)
	{
		enforceSize();
		return VF_floortoLI(vx_vect,X.vx_vect,ui_size);
	}

	inline int liVecObj::ceiltoLI(const fVecObj& X)
	{
		enforceSize();
		return VF_ceiltoLI(vx_vect,X.vx_vect,ui_size);
	}

	inline int liVecObj::choptoLI(const fVecObj& X)
	{
		enforceSize();
		return VF_choptoLI(vx_vect,X.vx_vect,ui_size);
	}
#endif //FVECOBJ_H

#ifdef DVECOBJ_H
	inline void dVecObj::accVLI(const liVecObj& X)
	{
		enforceSize();
		VD_accVLI(vx_vect,X.vx_vect,ui_size);
	}

	inline void dVecObj::LItoD(const liVecObj& X)
	{
		enforceSize();
		V_LItoD(vx_vect,X.vx_vect,ui_size);
	}

	inline int liVecObj::roundtoLI(const dVecObj& X)
	{
		enforceSize();
		return VD_roundtoLI(vx_vect,X.vx_vect,ui_size);
	}

	inline int liVecObj::floortoLI(const dVecObj& X)
	{
		enforceSize();
		return VD_floortoLI(vx_vect,X.vx_vect,ui_size);
	}

	inline int liVecObj::ceiltoLI(const dVecObj& X)
	{
		enforceSize();
		return VD_ceiltoLI(vx_vect,X.vx_vect,ui_size);
	}

	inline int liVecObj::choptoLI(const dVecObj& X)
	{
		enforceSize();
		return VD_choptoLI(vx_vect,X.vx_vect,ui_size);
	}
#endif //DVECOBJ_H

#ifdef EVECOBJ_H
#ifdef __BORLANDC__ //80-bit numbers supported
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

#define LIVECOBJ_H
#endif //LIVECOBJ_H
