/* usVecObj.H

  object-oriented interface for OptiVec vector functions
  of type "unsigned short"

  This file was written by Brian Dale, Case Western Reserve Univ.

  Copyright (c) 1996-2002 by OptiCode
  All Rights Reserved.
*/

#ifndef USVECOBJ_H

#include <VecObj.h>
#include <uiVecObj.h>

#define cvta const vector<unsigned short>&
#define cuia const ui&
#define cta  const unsigned short&

#define vta  vector<unsigned short>&
#define uia  ui&
#define ta   unsigned short&

#define vt   vector<unsigned short>
#define t    unsigned short
#define tVector usVector

#define tVecObj usVecObj
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
		{	if(size){	vx_vect= VUS_vector(size);	ui_size= size;}
			else{	vx_vect= 0;	ui_size= 0;}}

		vector(cuia size, cta fill)
		{	if(!size) //size == 0
			{	vx_vect=0;	ui_size=0;}
			else if(!fill) //fill == 0.0
			{	vx_vect= VUS_vector0(size);	ui_size= size;}
			else
			{	vx_vect= VUS_vector(size);	ui_size= size;	VUS_equC(vx_vect,ui_size,fill);}}

		vector(cvta init)
		{	if(init.ui_size)
			{ vx_vect= VUS_vector(init.ui_size);	ui_size= init.ui_size;	VUS_equV(vx_vect, init.vx_vect, ui_size);}
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
				vx_vect= VUS_vector(size);
				ui_size= size;}}

		void setSize(cvta X)
		{	if(!X.ui_size) free(); //can set size to zero
			if(ui_size != X.ui_size) //only allocate if different sizes
			{	if(vx_vect) V_free(vx_vect);
				vx_vect= VUS_vector(X.ui_size);
				ui_size= X.ui_size;}}

		ui getSize() { return ui_size; }

		void free()
		{	if(vx_vect) V_free(vx_vect);
			vx_vect=0;
			ui_size=0;}

		ta at(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return vx_vect[n];
		}

		
///////////////////////////////////////////////////////////////////////////////////////////////////
// Overloaded operators
//
		vta operator=(cvta vect)
		{	setSize(vect.ui_size);
			enforceSize();
			VUS_equV(vx_vect, vect.vx_vect, ui_size);
			return *this;}

		vta operator=(cta scalar)
		{	enforceSize();
			VUS_equC(vx_vect, ui_size, scalar);
			return *this;}

		//this is the only really dangerous operator
		ta operator[](const int& index){return vx_vect[index];}

		vta operator+=(cvta X)
		{	enforceSize();
			VUS_accV(vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator+=(cta scalar)
		{	enforceSize();
			VUS_addC(vx_vect, vx_vect, ui_size, scalar);
			return *this;}

		vta operator*=(cvta X)
		{	enforceSize();
			VUS_mulV(vx_vect, vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator*=(cta scalar)
		{	enforceSize();
			VUS_mulC(vx_vect,vx_vect,ui_size,scalar);
			return *this;}

		vt operator+(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VUS_addV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator+(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VUS_addC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator+(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VUS_addC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator-(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VUS_subV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator-(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VUS_subC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator-(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VUS_subrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator*(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VUS_mulV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator*(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VUS_mulC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator*(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VUS_mulC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator/(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VUS_divV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator/(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VUS_divC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator/(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VUS_divrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
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
			if(size) vx_vect= VUS_vector(size);
			ui_size=size;
		}

		void vector0(ui size)
		{
			free();
			if(size) vx_vect= VUS_vector0(size);
			ui_size=size;
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Addressing single vector elements
//
		t* Pelement(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VUS_Pelement(vx_vect,n);
			//not in funcref
		}

		t element(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VUS_element(vx_vect,n);
			//not in funcref
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Initialization
//

		void equ0()
		{
			enforceSize();
			VUS_equ0(vx_vect,ui_size);
		}

		void equC(cta C)
		{
			enforceSize();
			VUS_equC(vx_vect, ui_size, C);
		}

		void equV(cvta X)
		{
			enforceSize();
			VUS_equV(vx_vect, X.vx_vect, ui_size);
		}

		void o_ramp(cta Start, cta Rise)
		{
			enforceSize();
			VUSo_ramp(vx_vect, ui_size, Start, Rise);
		}

		void ramp(cta Start, cta Rise)
		{
			enforceSize();
			VUS_ramp(vx_vect, ui_size, Start, Rise);
		}

		long random(const long& seed, cta Min, cta Max)
		{
			enforceSize();
			return VUS_random(vx_vect, ui_size, seed, Min, Max);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Data-type interconversions
//
		//defined by including siVecObj.h
		void o_SItoUS(const siVecObj& X);

		//defined by including siVecObj.h
		void SItoUS(const siVecObj& X);

		//defined by including ubVecObj.h
		void UBtoUS(const ubVecObj& X);

		//defined by including uVecObj.h
		void o_UtoUS(const uVecObj& X);
		
		//defined by including uVecObj.h
		void UtoUS(const uVecObj& X);
		
		//defined by including ulVecObj.h
		void o_ULtoUS(const ulVecObj& X);
		
		//defined by including ulVecObj.h
		void ULtoUS(const ulVecObj& X);


/////////////////////////////////////////////////////////////////////////////////////////
//Index-oriented manipulations
//
		void reflect()
		{
			enforceSize();
			VUS_reflect(vx_vect, ui_size);
		}

		void rev(cvta X)
		{
			enforceSize();
			VUS_rev(vx_vect, X.vx_vect, ui_size);
		}

#ifdef V_HUGE
		void rotate( cvta X, long pos )
#else
		void rotate( cvta X, int pos )
#endif
		{
			enforceSize();
			VUS_rotate(vx_vect, X.vx_vect, ui_size, pos);
		}


		void _delete(ui pos)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VUS_delete(vx_vect, ui_size, pos);
		}

		void insert(ui pos, cta C)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VUS_insert(vx_vect, ui_size, pos, C);
		}

		void sort(cvta X, int dir=1)
		{
			enforceSize();
			VUS_sort(vx_vect,X.vx_vect,ui_size,dir);
		}

		//sortind member of uiVecObj

		void subvector(cvta X, int step, ui start=0)
		{
			enforceSize();
			if(vx_vect==X.vx_vect) throw OptiVec_Err();
			VUS_subvector(vx_vect, ui_size, X.vx_vect+start, step);
		}

		void subvector_equC(const unsigned& samp, cta C)
		{
			enforceSize();
			VUS_subvector_equC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_equV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VUS_subvector_equV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void indpick(const vector<ui>& Ind, cvta X);
		void indput(cvta X, const vector<ui>& Ind);

		ui searchC(cta C, int mode=0)
		{
			enforceSize();
			return VUS_searchC(vx_vect, ui_size, C, mode);
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
			return VUSo_sum(vx_vect, ui_size);
		}

		void o_runsum(cvta X)
		{
			enforceSize();
			VUSo_runsum(vx_vect, X.vx_vect, ui_size);
		}

		t sum()
		{
			enforceSize();
			return VUS_sum(vx_vect, ui_size);
		}

		void runsum(cvta X)
		{
			enforceSize();
			VUS_runsum(vx_vect, X.vx_vect, ui_size);
		}

		//underscore needed because max defined in windef.h
		t _max()
		{
			enforceSize();
			return VUS_max(vx_vect, ui_size);
		}

		//underscore needed because min defined in windef.h
		t _min()
		{
			enforceSize();
			return VUS_min(vx_vect, ui_size);
		}

		t maxind(ui* Ind)
		{
			enforceSize();
			return VUS_maxind(Ind, vx_vect, ui_size);
		}

		t minind(ui* Ind)
		{
			enforceSize();
			return VUS_minind(Ind, vx_vect, ui_size);
		}

		double fsum() const
		{
			enforceSize();
			return VUS_fsum(vx_vect,ui_size);
		}

		double mean() const
		{
			enforceSize();
			return VUS_mean(vx_vect,ui_size);
		}

		void runmax(cvta X)
		{
			enforceSize();
			VUS_runmax(vx_vect, X.vx_vect, ui_size);
		}

		void runmin(cvta X)
		{
			enforceSize();
			VUS_runmin(vx_vect, X.vx_vect, ui_size);
		}

		int iselementC(cta C)
		{
			enforceSize();
			return VUS_iselementC(vx_vect, ui_size, C);
		}

		ui iselementV(cvta X, cvta Tab)
		{
			enforceSize();
			return VUS_iselementV(vx_vect, X.vx_vect,
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
			VUS_fprint(stream,vx_vect,ui_size,nperline,linewidth);
		}

#if !defined _Windows || defined __FLAT__ || defined _WIN32
		void cprint(unsigned nperline) const
		{
			enforceSize();
			VUS_cprint(vx_vect,ui_size,nperline);
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
			VUS_read(vx_vect,ui_size,stream);
		}

		void write(FILE* stream) const
		{
			enforceSize();
			VUS_write(stream,vx_vect,ui_size);
		}

//nread
//nwrite

		void setWriteFormat(char* formatString) const
		{
			VUS_setWriteFormat(formatString);
		}

		void setWriteSeparate(char* sepString) const
		{
			VUS_setWriteSeparate(sepString);
		}

//setNWriteSeparate

		void store(FILE* stream) const
		{
			enforceSize();
			fwrite(&ui_size,sizeof(ui),1,stream);
			VUS_store(stream,vx_vect,ui_size);
		}

		void recall(FILE* stream)
		{
			ui sz;
			fread(&sz,sizeof(ui),1,stream);
			setSize(sz);
			VUS_recall(vx_vect,ui_size,stream);
		}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//from VXmath.h
//


/////////////////////////////////////////////////////////////////////////////////////////
//Rounding
//
		//defined by including fVecObj.h
		int roundtoUS(const fVecObj& X);

		//defined by including fVecObj.h
		int floortoUS(const fVecObj& X);
		
		//defined by including fVecObj.h
		int ceiltoUS(const fVecObj& X);
		
		//defined by including fVecObj.h
		int choptoUS(const fVecObj& X);
		
		int trunctoUS(const fVecObj& X)
		{
			return choptoUS(X);
		}
		
		//defined by including dVecObj.h
		int roundtoUS(const dVecObj& X);
		
		//defined by including dVecObj.h
		int floortoUS(const dVecObj& X);
		
		//defined by including dVecObj.h
		int ceiltoUS(const dVecObj& X);
		
		//defined by including dVecObj.h
		int choptoUS(const dVecObj& X);
		
		int trunctoUS(const dVecObj& X)
		{
			return choptoUS(X);
		}
		
#ifdef __BORLANDC__ //80-bit numbers supported
		//defined by including eVecObj.h
		int roundtoUS(const eVecObj& X);
		
		//defined by including eVecObj.h
		int floortoUS(const eVecObj& X);
		
		//defined by including eVecObj.h
		int ceiltoUS(const eVecObj& X);
		
		//defined by including eVecObj.h
		int choptoUS(const eVecObj& X);
		
		int trunctoUS(const eVecObj& X)
		{
			return choptoUS(X);
		}
#endif //__BORLANDC__


/////////////////////////////////////////////////////////////////////////////////////////
//Comparisons
//


/////////////////////////////////////////////////////////////////////////////////////////
//Standard Arithmetics or Basic Arithmetics
//

		void o_addC(cvta X, cta C)
		{
			enforceSize();
			VUSo_addC(vx_vect, X.vx_vect, ui_size, C);
		}

		void o_subC(cvta X, cta C)
		{
			enforceSize();
			VUSo_subC(vx_vect, X.vx_vect, ui_size, C);
		}

		void o_subrC(cvta X, cta C)
		{
			enforceSize();
			VUSo_subrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void o_mulC(cvta X, cta C)
		{
			enforceSize();
			VUSo_mulC(vx_vect, X.vx_vect, ui_size, C);
		}

		void addC(cvta X, cta C)
		{
			enforceSize();
			VUS_addC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subC(cvta X, cta C)
		{
			enforceSize();
			VUS_subC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subrC(cvta X, cta C)
		{
			enforceSize();
			VUS_subrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void mulC(cvta X, cta C)
		{
			enforceSize();
			VUS_mulC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divC(cvta X, cta C)
		{
			enforceSize();
			VUS_divC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divrC(cvta X, cta C)
		{
			enforceSize();
			VUS_divrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void modC(cvta X, cta C)
		{
			enforceSize();
			VUS_modC(vx_vect, X.vx_vect, ui_size, C);
		}

		void maxC(cvta X, cta C)
		{
			enforceSize();
			VUS_maxC( vx_vect, X.vx_vect, ui_size, C);
		}

		void minC(cvta X, cta C)
		{
			enforceSize();
			VUS_minC( vx_vect, X.vx_vect, ui_size, C);
		}

		void o_addV(cvta X, cvta Y)
		{
			enforceSize();
			VUSo_addV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void o_subV(cvta X, cvta Y)
		{
			enforceSize();
			VUSo_subV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void o_subrV(cvta X, cvta Y)
		{
			enforceSize();
			VUSo_subrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void o_mulV(cvta X, cvta Y)
		{
			enforceSize();
			VUSo_mulV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void addV(cvta X, cvta Y)
		{
			enforceSize();
			VUS_addV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subV(cvta X, cvta Y)
		{
			enforceSize();
			VUS_subV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subrV(cvta X, cvta Y)
		{
			enforceSize();
			VUS_subrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void mulV(cvta X, cvta Y)
		{
			enforceSize();
			VUS_mulV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divV(cvta X, cvta Y)
		{
			enforceSize();
			VUS_divV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divrV(cvta X, cvta Y)
		{
			enforceSize();
			VUS_divrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void modV(cvta X, cvta Y)
		{
			enforceSize();
			VUS_modV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void maxV(cvta X, cvta Y)
		{
			enforceSize();
			VUS_maxV( vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void minV(cvta X, cvta Y)
		{
			enforceSize();
			VUS_minV( vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Accumulation
//
		void accV(cvta X)
		{
			enforceSize();
			VUS_accV(vx_vect,X.vx_vect,ui_size);
		}

		//defined by including ubVecObj.h
		void accVUB(const ubVecObj& X);


/////////////////////////////////////////////////////////////////////////////////////////
//Bit-wise operations
//
		void shl(cvta X, unsigned C)
		{
			enforceSize();
			VUS_shl(vx_vect,X.vx_vect,ui_size,C);
		}

		void shr(cvta X, unsigned C)
		{
			enforceSize();
			VUS_shr(vx_vect,X.vx_vect,ui_size,C);
		}

		void and(cvta X, cta C)
		{
			enforceSize();
			VUS_and(vx_vect,X.vx_vect,ui_size,C);
		}

		void or(cvta X, cta C)
		{
			enforceSize();
			VUS_or(vx_vect,X.vx_vect,ui_size,C);
		}

		void xor(cvta X, cta C)
		{
			enforceSize();
			VUS_xor(vx_vect,X.vx_vect,ui_size,C);
		}

		void not(cvta X)
		{
			enforceSize();
			VUS_not(vx_vect,X.vx_vect,ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Functions of a sub-set of elements
//


/////////////////////////////////////////////////////////////////////////////////////////
//Mathematical Functions
//


/////////////////////////////////////////////////////////////////////////////////////////
//Optical Density Y = log10( X0 / X )
//
//Optical Density functions members of fVecObj, dVecObj, and eVecObj


/////////////////////////////////////////////////////////////////////////////////////////
// Graphics
//

            #ifdef __VGRAPH_H
                void xyAutoPlot( cvta X, unsigned form, COLORREF color )
                {
                    enforceSize();
                    enforceSize(X.ui_size);
                    VUS_xyAutoPlot( X.vx_vect, vx_vect, ui_size, form, color );
                }

                void xy2AutoPlot( cvta X1, unsigned form1, COLORREF color1,
                         cvta X2, cvta Y2, unsigned form2, COLORREF color2 )
                {
                    enforceSize();
                    enforceSize(X1.ui_size);
                    Y2.enforceSize(X2.ui_size);
                    VUS_xy2AutoPlot( X1.vx_vect, vx_vect, ui_size, form1, color1,
                              X2.vx_vect, Y2.vx_vect, X2.ui_size, form2, color2 );
                }

                void yAutoPlot( unsigned form, COLORREF color )
                {
                    enforceSize();
                    VUS_yAutoPlot( vx_vect, ui_size, form, color );
                }

                void y2AutoPlot( unsigned form1, COLORREF color1,
                        cvta Y2, unsigned form2, COLORREF color2 )
                {
                    enforceSize();
                    Y2.enforceSize();
                    VUS_y2AutoPlot( vx_vect, ui_size, form1, color1,
                             Y2.vx_vect, Y2.ui_size, form2, color2 );
                }

                void xyDataPlot( cvta X, unsigned form, COLORREF color )
                {
                    enforceSize();
                    enforceSize(X.ui_size);
                    VUS_xyDataPlot( X.vx_vect, vx_vect, ui_size, form, color );
                }

                void yDataPlot( unsigned form, COLORREF color )
                {
                    enforceSize();
                    VUS_yDataPlot( vx_vect, ui_size, form, color );
                }
            #endif   // __VGRAPH_H
	};


	inline void uiVecObj::sortind(const usVecObj& X, int dir)
	{
		enforceSize();
		VUS_sortind(vx_vect,X.vx_vect,ui_size,dir);
	}

	inline void uiVecObj::searchV(const usVecObj& X, const usVecObj& Tab, int mode=0)
	{
		enforceSize();
		Tab.enforceSize();
		VUS_searchV(vx_vect,X.vx_vect,ui_size,Tab.vx_vect,Tab.ui_size,mode);
	}

#ifdef UBVECOBJ_H
	inline void usVecObj::UBtoUS(const ubVecObj& X)
	{
		enforceSize();
		V_UBtoUS(vx_vect,X.vx_vect,ui_size);
	}

	inline void ubVecObj::UStoUB(const usVecObj& X)
	{
		enforceSize();
		V_UStoUB(vx_vect,X.vx_vect,ui_size);
	}

	inline void usVecObj::accVUB(const ubVecObj& X)
	{
		enforceSize();
		VUS_accVUB(vx_vect,X.vx_vect,ui_size);
	}
#endif //UBVECOBJ_H

#ifdef UVECOBJ_H
	inline void usVecObj::o_UtoUS(const uVecObj& X)
	{
		enforceSize();
		Vo_UtoUS(vx_vect,X.vx_vect,ui_size);
	}

	inline void usVecObj::UtoUS(const uVecObj& X)
	{
		enforceSize();
		V_UtoUS(vx_vect,X.vx_vect,ui_size);
	}

	inline void uVecObj::UStoU(const usVecObj& X)
	{
		enforceSize();
		V_UStoU(vx_vect,X.vx_vect,ui_size);
	}

	inline void uVecObj::accVUS(const usVecObj& X)
	{
		enforceSize();
		VU_accVUS(vx_vect,X.vx_vect,ui_size);
	}
#endif //UVECOBJ_H

#ifdef ULVECOBJ_H
	inline void usVecObj::o_ULtoUS(const ulVecObj& X)
	{
		enforceSize();
		Vo_ULtoUS(vx_vect,X.vx_vect,ui_size);
	}

	inline void usVecObj::ULtoUS(const ulVecObj& X)
	{
		enforceSize();
		V_ULtoUS(vx_vect,X.vx_vect,ui_size);
	}

	inline void ulVecObj::UStoUL(const usVecObj& X)
	{
		enforceSize();
		V_UStoUL(vx_vect,X.vx_vect,ui_size);
	}

	inline void ulVecObj::accVUS(const usVecObj& X)
	{
		enforceSize();
		VUL_accVUS(vx_vect,X.vx_vect,ui_size);
	}
#endif //ULVECOBJ_H

#ifdef SIVECOBJ_H
	inline void usVecObj::o_SItoUS(const siVecObj& X)
	{
		enforceSize();
		Vo_SItoUS(vx_vect,X.vx_vect,ui_size);
	}

	inline void siVecObj::o_UStoSI(const usVecObj& X)
	{
		enforceSize();
		Vo_UStoSI(vx_vect,X.vx_vect,ui_size);
	}

	inline void usVecObj::SItoUS(const siVecObj& X)
	{
		enforceSize();
		V_SItoUS(vx_vect,X.vx_vect,ui_size);
	}

	inline void siVecObj::UStoSI(const usVecObj& X)
	{
		enforceSize();
		V_UStoSI(vx_vect,X.vx_vect,ui_size);
	}
#endif //SIVECOBJ_H

#ifdef QIVECOBJ_H
	inline void qiVecObj::accVUS(const usVecObj& X)
	{
		enforceSize();
		VQI_accVUS(vx_vect,X.vx_vect,ui_size);
	}
#endif //QIVECOBJ_H

#ifdef FVECOBJ_H
	inline void fVecObj::accVUS(const usVecObj& X)
	{
		enforceSize();
		VF_accVUS(vx_vect,X.vx_vect,ui_size);
	}

	inline void fVecObj::UStoF(const usVecObj& X)
	{
		enforceSize();
		V_UStoF(vx_vect,X.vx_vect,ui_size);
	}

	inline int usVecObj::roundtoUS(const fVecObj& X)
	{
		enforceSize();
		return VF_roundtoUS(vx_vect,X.vx_vect,ui_size);
	}

	inline int usVecObj::floortoUS(const fVecObj& X)
	{
		enforceSize();
		return VF_floortoUS(vx_vect,X.vx_vect,ui_size);
	}

	inline int usVecObj::ceiltoUS(const fVecObj& X)
	{
		enforceSize();
		return VF_ceiltoUS(vx_vect,X.vx_vect,ui_size);
	}

	inline int usVecObj::choptoUS(const fVecObj& X)
	{
		enforceSize();
		return VF_choptoUS(vx_vect,X.vx_vect,ui_size);
	}
#endif //FVECOBJ_H

#ifdef DVECOBJ_H
	inline void dVecObj::accVUS(const usVecObj& X)
	{
		enforceSize();
		VD_accVUS(vx_vect,X.vx_vect,ui_size);
	}

	inline void dVecObj::UStoD(const usVecObj& X)
	{
		enforceSize();
		V_UStoD(vx_vect,X.vx_vect,ui_size);
	}

	inline int usVecObj::roundtoUS(const dVecObj& X)
	{
		enforceSize();
		return VD_roundtoUS(vx_vect,X.vx_vect,ui_size);
	}

	inline int usVecObj::floortoUS(const dVecObj& X)
	{
		enforceSize();
		return VD_floortoUS(vx_vect,X.vx_vect,ui_size);
	}

	inline int usVecObj::ceiltoUS(const dVecObj& X)
	{
		enforceSize();
		return VD_ceiltoUS(vx_vect,X.vx_vect,ui_size);
	}

	inline int usVecObj::choptoUS(const dVecObj& X)
	{
		enforceSize();
		return VD_choptoUS(vx_vect,X.vx_vect,ui_size);
	}
#endif //DVECOBJ_H

#ifdef EVECOBJ_H
#ifdef __BORLANDC__ //80-bit numbers supported
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

#define USVECOBJ_H
#endif //USVECOBJ_H
