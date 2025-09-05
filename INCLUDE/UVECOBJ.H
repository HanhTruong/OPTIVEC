/* uVecObj.H

  object-oriented interface for OptiVec vector functions
  of type "unsigned"

  This file was written by Brian Dale, Case Western Reserve Univ.

  Copyright (c) 1996-2002 by OptiCode
  All Rights Reserved.
*/

#ifndef UVECOBJ_H

#include <VecObj.h>

#define cvta const vector<unsigned int>&
#define cuia const ui&
#define cta  const unsigned int&

#define vta  vector<unsigned int>&
#define uia  ui&
#define ta   unsigned int&

#define vt   vector<unsigned int>
#define t    unsigned int
#define tVector uVector

#define tVecObj uVecObj
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
		{	if(size){	vx_vect= VU_vector(size);	ui_size= size;}
			else{	vx_vect= 0;	ui_size= 0;}}

		vector(cuia size, cta fill)
		{	if(!size) //size == 0
			{	vx_vect=0;	ui_size=0;}
			else if(!fill) //fill == 0.0
			{	vx_vect= VU_vector0(size);	ui_size= size;}
			else
			{	vx_vect= VU_vector(size);	ui_size= size;	VU_equC(vx_vect,ui_size,fill);}}

		vector(cvta init)
		{	if(init.ui_size)
			{ vx_vect= VU_vector(init.ui_size);	ui_size= init.ui_size;	VU_equV(vx_vect, init.vx_vect, ui_size);}
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
				vx_vect= VU_vector(size);
				ui_size= size;}}

		void setSize(cvta X)
		{	if(!X.ui_size) free(); //can set size to zero
			if(ui_size != X.ui_size) //only allocate if different sizes
			{	if(vx_vect) V_free(vx_vect);
				vx_vect= VU_vector(X.ui_size);
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
			VU_equV(vx_vect, vect.vx_vect, ui_size);
			return *this;}

		vta operator=(cta scalar)
		{	enforceSize();
			VU_equC(vx_vect, ui_size, scalar);
			return *this;}

		//this is the only really dangerous operator
		ta operator[](const int& index){return vx_vect[index];}

		vta operator+=(cvta X)
		{	enforceSize();
			VU_accV(vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator+=(cta scalar)
		{	enforceSize();
			VU_addC(vx_vect, vx_vect, ui_size, scalar);
			return *this;}

		vta operator*=(cvta X)
		{	enforceSize();
			VU_mulV(vx_vect, vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator*=(cta scalar)
		{	enforceSize();
			VU_mulC(vx_vect,vx_vect,ui_size,scalar);
			return *this;}

		vt operator+(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VU_addV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator+(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VU_addC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator+(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VU_addC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator-(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VU_subV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator-(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VU_subC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator-(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VU_subrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator*(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VU_mulV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator*(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VU_mulC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator*(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VU_mulC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator/(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VU_divV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator/(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VU_divC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator/(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VU_divrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
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
			if(size) vx_vect= VU_vector(size);
			ui_size=size;
		}

		void vector0(ui size)
		{
			free();
			if(size) vx_vect= VU_vector0(size);
			ui_size=size;
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Addressing single vector elements
//
		t* Pelement(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VU_Pelement(vx_vect,n);
			//not in funcref
		}

		t element(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VU_element(vx_vect,n);
			//not in funcref
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Initialization
//
		void equ0()
		{
			enforceSize();
			VU_equ0(vx_vect,ui_size);
		}

		void equC(cta C)
		{
			enforceSize();
			VU_equC(vx_vect, ui_size, C);
		}

		void equV(cvta X)
		{
			enforceSize();
			VU_equV(vx_vect, X.vx_vect, ui_size);
		}

		void o_ramp(cta Start, cta Rise)
		{
			enforceSize();
			VUo_ramp(vx_vect, ui_size, Start, Rise);
		}

		void ramp(cta Start, cta Rise)
		{
			enforceSize();
			VU_ramp(vx_vect, ui_size, Start, Rise);
		}

		long random(const long& seed, cta Min, cta Max)
		{
			enforceSize();
			return VU_random(vx_vect, ui_size, seed, Min, Max);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Data-type interconversions
//
		//defined by including iVecObj.h
		void o_ItoU(const iVecObj& X);
		//defined by including iVecObj.h
		void ItoU(const iVecObj& X);

		//defined by including ubVecObj.h
		void UBtoU(const ubVecObj& X);
		//defined by including usVecObj.h
		void UStoU(const usVecObj& X);

		//defined by including ulVecObj.h
		void o_ULtoU(const ulVecObj& X);
		//defined by including ulVecObj.h
		void ULtoU(const ulVecObj& X);


#ifndef V_HUGE //uiVecObj members
   #define UItoU               equV
   #define UtoUI               equV
   #define UItoUB              UtoUB
   #define UBtoUI              UBtoU
   #define UItoUS              UtoUS
   #define UStoUI              UStoU
   #define ULtoUI              ULtoU
   #define UItoUL              UtoUL
   #define UItoF               UtoF
   #define UItoD               UtoD
   #define UItoE               UtoE
#endif //V_HUGE

/////////////////////////////////////////////////////////////////////////////////////////
//Index-oriented manipulations
//
		void reflect()
		{
			enforceSize();
			VU_reflect(vx_vect, ui_size);
		}

		void rev(cvta X)
		{
			enforceSize();
			VU_rev(vx_vect, X.vx_vect, ui_size);
		}

#ifdef V_HUGE
		void rotate( cvta X, long pos )
#else
		void rotate( cvta X, int pos )
#endif
		{
			enforceSize();
			VU_rotate(vx_vect, X.vx_vect, ui_size, pos);
		}


		void _delete(ui pos)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VU_delete(vx_vect, ui_size, pos);
		}

		void insert(ui pos, cta C)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VU_insert(vx_vect, ui_size, pos, C);
		}

		void sort(cvta X, int dir=1)
		{
			enforceSize();
			VU_sort(vx_vect,X.vx_vect,ui_size,dir);
		}

		//sortind member of uiVecObj

#ifndef V_HUGE //uiVecObj members
		//defined by including ubVecObj.h
		void sortind(const ubVecObj& X, int dir);

		//defined by including usVecObj.h
		void sortind(const usVecObj& X, int dir);

		//valid for both u and ui
		void sortind(const uVecObj& X, int dir);

		//defined by including ulVecObj.h
		void sortind(const ulVecObj& X, int dir);

		//defined by including biVecObj.h
		void sortind(const biVecObj& X, int dir);

		//defined by including siVecObj.h
		void sortind(const siVecObj& X, int dir);

		//defined by including iVecObj.h
		void sortind(const iVecObj& X, int dir);
		
		//defined by including liVecObj.h
		void sortind(const liVecObj& X, int dir);
		
		//defined by including qiVecObj.h
		void sortind(const qiVecObj& X, int dir);

		//defined by including fVecObj.h
		void sortind(const fVecObj& X, int dir);
		
		//defined by including dVecObj.h
		void sortind(const dVecObj& X, int dir);

#ifdef __BORLANDC__ //80-bit numbers supported
		//defined by including eVecObj.h
		void sortind(const eVecObj& X, int dir);
#endif //__BORLANDC__
#endif //V_HUGE

		void subvector(cvta X, int step, ui start=0)
		{
			enforceSize();
			if(vx_vect==X.vx_vect) throw OptiVec_Err();
			VU_subvector(vx_vect, ui_size, X.vx_vect+start, step);
		}

		void subvector_equC(const unsigned& samp, cta C)
		{
			enforceSize();
			VU_subvector_equC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_equV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VU_subvector_equV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void indpick(const vector<ui>& Ind, cvta X);
		void indput(cvta X, const vector<ui>& Ind);

		ui searchC(cta C, int mode=0)
		{
			enforceSize();
			return VU_searchC(vx_vect, ui_size, C, mode);
		}

		//searchV is member of uiVecObj

#ifndef V_HUGE //uiVecObj members
		//defined by including ubVecObj.h
		void searchV(const ubVecObj& X, const ubVecObj& Tab, int mode);

		//defined by including usVecObj.h
		void searchV(const usVecObj& X, const usVecObj& Tab, int mode);

		//valid for both u and ui
		void searchV(const uVecObj& X, const uVecObj& Tab, int mode);

		//defined by including ulVecObj.h
		void searchV(const ulVecObj& X, const ulVecObj& Tab, int mode);

		//defined by including biVecObj.h
		void searchV(const biVecObj& X, const biVecObj& Tab, int mode);

		//defined by including siVecObj.h
		void searchV(const siVecObj& X, const siVecObj& Tab, int mode);

		//defined by including iVecObj.h
		void searchV(const iVecObj& X, const iVecObj& Tab, int mode);

		//defined by including liVecObj.h
		void searchV(const liVecObj& X, const liVecObj& Tab, int mode);

		//defined by including qiVecObj.h
		void searchV(const qiVecObj& X, const qiVecObj& Tab, int mode);

		//defined by including fVecObj.h
		void searchV(const fVecObj& X, const fVecObj& Tab, int mode);

		//defined by including dVecObj.h
		void searchV(const dVecObj& X, const dVecObj& Tab, int mode);
		
#ifdef __BORLANDC__ //80-bit numbers supported
		//defined by including eVecObj.h
		void searchV(const eVecObj& X, const eVecObj& Tab, int mode);
#endif //__BORLANDC__
#endif //V_HUGE
/////////////////////////////////////////////////////////////////////////////////////////
//Functions of a sub-set of elements
//


/////////////////////////////////////////////////////////////////////////////////////////
//One-Dimensional Vector Operations
//
		t o_sum()
		{
			enforceSize();
			return VUo_sum(vx_vect, ui_size);
		}

		void o_runsum(cvta X)
		{
			enforceSize();
			VUo_runsum(vx_vect, X.vx_vect, ui_size);
		}

		t sum()
		{
			enforceSize();
			return VU_sum(vx_vect, ui_size);
		}

		void runsum(cvta X)
		{
			enforceSize();
			VU_runsum(vx_vect, X.vx_vect, ui_size);
		}

		//underscore needed because max defined in windef.h
		t _max()
		{
			enforceSize();
			return VU_max(vx_vect, ui_size);
		}

		//underscore needed because min defined in windef.h
		t _min()
		{
			enforceSize();
			return VU_min(vx_vect, ui_size);
		}

		t maxind(ui* Ind)
		{
			enforceSize();
			return VU_maxind(Ind, vx_vect, ui_size);
		}

		t minind(ui* Ind)
		{
			enforceSize();
			return VU_minind(Ind, vx_vect, ui_size);
		}

		double fsum() const
		{
			enforceSize();
			return VU_fsum(vx_vect,ui_size);
		}

		double mean() const
		{
			enforceSize();
			return VU_mean(vx_vect,ui_size);
		}

		void runmax(cvta X)
		{
			enforceSize();
			VU_runmax(vx_vect, X.vx_vect, ui_size);
		}

		void runmin(cvta X)
		{
			enforceSize();
			VU_runmin(vx_vect, X.vx_vect, ui_size);
		}

		int iselementC(cta C)
		{
			enforceSize();
			return VU_iselementC(vx_vect, ui_size, C);
		}

		ui iselementV(cvta X, cvta Tab)
		{
			enforceSize();
			return VU_iselementV(vx_vect, X.vx_vect,
				ui_size, Tab.vx_vect, Tab.ui_size);
		}

#ifndef V_HUGE //uiVecObj members
		//defined by including fVecObj.h
		ui localmaxima(const fVecObj& X);

		//defined by including dVecObj.h
		ui localmaxima(const dVecObj& X);

#ifdef __BORLANDC__ //80-bit numbers supported
		//defined by including eVecObj.h
		ui localmaxima(const eVecObj& X);
#endif //__BORLANDC__

		//defined by including fVecObj.h
		ui localminima(const fVecObj& X);

		//defined by including dVecObj.h
		ui localminima(const dVecObj& X);

#ifdef __BORLANDC__ //80-bit numbers supported
		//defined by including eVecObj.h
		ui localminima(const eVecObj& X);
#endif //__BORLANDC__
#endif //V_HUGE


/////////////////////////////////////////////////////////////////////////////////////////
//Statistical Functions and Building Blocks
//

#ifndef V_HUGE //uiVecObj members
		//defined by including fVecObj.h
		ui distribution(const fVecObj& Limits, const fVecObj& X, int mode);

		//defined by including dVecObj.h
		ui distribution(const dVecObj& Limits, const dVecObj& X, int mode);

#ifdef __BORLANDC__ //80-bit numbers supported
		//defined by including eVecObj.h
		ui distribution(const eVecObj& Limits, const eVecObj& X, int mode);
#endif //__BORLANDC__
#endif //V_HUGE



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
			VU_fprint(stream,vx_vect,ui_size,nperline,linewidth);
		}

#if !defined _Windows || defined __FLAT__ || defined _WIN32
		void cprint(unsigned nperline) const
		{
			enforceSize();
			VU_cprint(vx_vect,ui_size,nperline);
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
			VU_read(vx_vect,ui_size,stream);
		}

		void write(FILE* stream) const
		{
			enforceSize();
			VU_write(stream,vx_vect,ui_size);
		}

//nwrite
//nread

		void setWriteFormat(char* formatString) const
		{
			VU_setWriteFormat(formatString);
		}

		void setWriteSeparate(char* sepString) const
		{
			VU_setWriteSeparate(sepString);
		}

//setNWriteSeparate

		void store(FILE* stream) const
		{
			enforceSize();
			fwrite(&ui_size,sizeof(ui),1,stream);
			VU_store(stream,vx_vect,ui_size);
		}

		void recall(FILE* stream)
		{
			ui sz;
			fread(&sz,sizeof(ui),1,stream);
			setSize(sz);
			VU_recall(vx_vect,ui_size,stream);
		}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//from VXmath.h
//


/////////////////////////////////////////////////////////////////////////////////////////
//Rounding
//
		//defined by including fVecObj.h
		int roundtoU(const fVecObj& X);

		//defined by including fVecObj.h
		int floortoU(const fVecObj& X);
		
		//defined by including fVecObj.h
		int ceiltoU(const fVecObj& X);
		
		//defined by including fVecObj.h
		int choptoU(const fVecObj& X);
		
		int trunctoU(const fVecObj& X)
		{
			return choptoU(X);
		}
		
		//defined by including dVecObj.h
		int roundtoU(const dVecObj& X);
		
		//defined by including dVecObj.h
		int floortoU(const dVecObj& X);
		
		//defined by including dVecObj.h
		int ceiltoU(const dVecObj& X);
		
		//defined by including dVecObj.h
		int choptoU(const dVecObj& X);
		
		int trunctoU(const dVecObj& X)
		{
			return choptoU(X);
		}
		
#ifdef __BORLANDC__ //80-bit numbers supported
		//defined by including eVecObj.h
		int roundtoU(const eVecObj& X);
		
		//defined by including eVecObj.h
		int floortoU(const eVecObj& X);
		
		//defined by including eVecObj.h
		int ceiltoU(const eVecObj& X);
		
		//defined by including eVecObj.h
		int choptoU(const eVecObj& X);
		
		int trunctoU(const eVecObj& X)
		{
			return choptoU(X);
		}
#endif //__BORLANDC__

#ifndef V_HUGE //uiVecObj members
#define roundtoUI roundtoU
#define floortoUI floortoU
#define ceiltoUI  ceiltoU
#define choptoUI  choptoU		
#define trunctoUI trunctoU
#endif //V_HUGE


/////////////////////////////////////////////////////////////////////////////////////////
//Comparisons
//
#ifndef V_HUGE //uiVecObj members
		//defined by including fVecObj.h
		ui cmp_eq0ind(const fVecObj& X);
		ui cmp_ne0ind(const fVecObj& X);
		ui cmp_le0ind(const fVecObj& X);
		ui cmp_lt0ind(const fVecObj& X);
		ui cmp_ge0ind(const fVecObj& X);
		ui cmp_gt0ind(const fVecObj& X);
		ui cmp_eqCind(const fVecObj& X,const float& C);
		ui cmp_neCind(const fVecObj& X,const float& C);
		ui cmp_leCind(const fVecObj& X,const float& C);
		ui cmp_ltCind(const fVecObj& X,const float& C);
		ui cmp_geCind(const fVecObj& X,const float& C);
		ui cmp_gtCind(const fVecObj& X,const float& C);
		ui cmp_eqVind(const fVecObj& X,const fVecObj& Y);
		ui cmp_neVind(const fVecObj& X,const fVecObj& Y);
		ui cmp_leVind(const fVecObj& X,const fVecObj& Y);
		ui cmp_ltVind(const fVecObj& X,const fVecObj& Y);
		ui cmp_geVind(const fVecObj& X,const fVecObj& Y);
		ui cmp_gtVind(const fVecObj& X,const fVecObj& Y);
		ui cmp_inclrange0Cind(const fVecObj& X, const float& C);
		ui cmp_exclrange0Cind(const fVecObj& X, const float& C);
		ui cmp_inclrangeCCind(const fVecObj& X, const float& CLo, const float& CHi);
		ui cmp_exclrangeCCind(const fVecObj& X, const float& CLo, const float& CHi);

		//defined by including cfVecObj.h
		ui cmp_eq0ind(const cfVecObj& X);
		ui cmp_ne0ind(const cfVecObj& X);
		ui cmp_eqCind(const cfVecObj& X, const fComplex& C);
		ui cmp_neCind(const cfVecObj& X, const fComplex& C);
		ui cmp_eqVind(const cfVecObj& X, const cfVecObj& Y);
		ui cmp_neVind(const cfVecObj& X, const cfVecObj& Y);

		//defined by including pfVecObj.h
		ui cmp_eq0ind(const pfVecObj& X);
		ui cmp_ne0ind(const pfVecObj& X);
		ui cmp_eqCind(const pfVecObj& X, const fPolar& C);
		ui cmp_neCind(const pfVecObj& X, const fPolar& C);
		ui cmp_eqVind(const pfVecObj& X, const pfVecObj& Y);
		ui cmp_neVind(const pfVecObj& X, const pfVecObj& Y);

		//defined by including dVecObj.h
		ui cmp_eq0ind(const dVecObj& X);
		ui cmp_ne0ind(const dVecObj& X);
		ui cmp_le0ind(const dVecObj& X);
		ui cmp_lt0ind(const dVecObj& X);
		ui cmp_ge0ind(const dVecObj& X);
		ui cmp_gt0ind(const dVecObj& X);
		ui cmp_eqCind(const dVecObj& X,const float& C);
		ui cmp_neCind(const dVecObj& X,const float& C);
		ui cmp_leCind(const dVecObj& X,const float& C);
		ui cmp_ltCind(const dVecObj& X,const float& C);
		ui cmp_geCind(const dVecObj& X,const float& C);
		ui cmp_gtCind(const dVecObj& X,const float& C);
		ui cmp_eqVind(const dVecObj& X,const dVecObj& Y);
		ui cmp_neVind(const dVecObj& X,const dVecObj& Y);
		ui cmp_leVind(const dVecObj& X,const dVecObj& Y);
		ui cmp_ltVind(const dVecObj& X,const dVecObj& Y);
		ui cmp_geVind(const dVecObj& X,const dVecObj& Y);
		ui cmp_gtVind(const dVecObj& X,const dVecObj& Y);
		ui cmp_inclrange0Cind(const dVecObj& X, const float& C);
		ui cmp_exclrange0Cind(const dVecObj& X, const float& C);
		ui cmp_inclrangeCCind(const dVecObj& X, const float& CLo, const float& CHi);
		ui cmp_exclrangeCCind(const dVecObj& X, const float& CLo, const float& CHi);

		//defined by including cdVecObj.h
		ui cmp_eq0ind(const cdVecObj& X);
		ui cmp_ne0ind(const cdVecObj& X);
		ui cmp_eqCind(const cdVecObj& X, const dComplex& C);
		ui cmp_neCind(const cdVecObj& X, const dComplex& C);
		ui cmp_eqVind(const cdVecObj& X, const cdVecObj& Y);
		ui cmp_neVind(const cdVecObj& X, const cdVecObj& Y);

		//defined by including pdVecObj.h
		ui cmp_eq0ind(const pdVecObj& X);
		ui cmp_ne0ind(const pdVecObj& X);
		ui cmp_eqCind(const pdVecObj& X, const dPolar& C);
		ui cmp_neCind(const pdVecObj& X, const dPolar& C);
		ui cmp_eqVind(const pdVecObj& X, const pdVecObj& Y);
		ui cmp_neVind(const pdVecObj& X, const pdVecObj& Y);

#ifdef __BORLANDC__ //80-bit numbers supported
		//defined by including eVecObj.h
		ui cmp_eq0ind(const eVecObj& X);
		ui cmp_ne0ind(const eVecObj& X);
		ui cmp_le0ind(const eVecObj& X);
		ui cmp_lt0ind(const eVecObj& X);
		ui cmp_ge0ind(const eVecObj& X);
		ui cmp_gt0ind(const eVecObj& X);
		ui cmp_eqCind(const eVecObj& X,const float& C);
		ui cmp_neCind(const eVecObj& X,const float& C);
		ui cmp_leCind(const eVecObj& X,const float& C);
		ui cmp_ltCind(const eVecObj& X,const float& C);
		ui cmp_geCind(const eVecObj& X,const float& C);
		ui cmp_gtCind(const eVecObj& X,const float& C);
		ui cmp_eqVind(const eVecObj& X,const eVecObj& Y);
		ui cmp_neVind(const eVecObj& X,const eVecObj& Y);
		ui cmp_leVind(const eVecObj& X,const eVecObj& Y);
		ui cmp_ltVind(const eVecObj& X,const eVecObj& Y);
		ui cmp_geVind(const eVecObj& X,const eVecObj& Y);
		ui cmp_gtVind(const eVecObj& X,const eVecObj& Y);
		ui cmp_inclrange0Cind(const eVecObj& X, const float& C);
		ui cmp_exclrange0Cind(const eVecObj& X, const float& C);
		ui cmp_inclrangeCCind(const eVecObj& X, const float& CLo, const float& CHi);
		ui cmp_exclrangeCCind(const eVecObj& X, const float& CLo, const float& CHi);

		//defined by including ceVecObj.h
		ui cmp_eq0ind(const ceVecObj& X);
		ui cmp_ne0ind(const ceVecObj& X);
		ui cmp_eqCind(const ceVecObj& X, const eComplex& C);
		ui cmp_neCind(const ceVecObj& X, const eComplex& C);
		ui cmp_eqVind(const ceVecObj& X, const ceVecObj& Y);
		ui cmp_neVind(const ceVecObj& X, const ceVecObj& Y);

		//defined by including peVecObj.h
		ui cmp_eq0ind(const peVecObj& X);
		ui cmp_ne0ind(const peVecObj& X);
		ui cmp_eqCind(const peVecObj& X, const ePolar& C);
		ui cmp_neCind(const peVecObj& X, const ePolar& C);
		ui cmp_eqVind(const peVecObj& X, const peVecObj& Y);
		ui cmp_neVind(const peVecObj& X, const peVecObj& Y);
#endif //__BORLANDC__
#endif //V_HUGE


/////////////////////////////////////////////////////////////////////////////////////////
//Standard Arithmetics or Basic Arithmetics
//
		void o_addC(cvta X, cta C)
		{
			enforceSize();
			VUo_addC(vx_vect, X.vx_vect, ui_size, C);
		}

		void o_subC(cvta X, cta C)
		{
			enforceSize();
			VUo_subC(vx_vect, X.vx_vect, ui_size, C);
		}

		void o_subrC(cvta X, cta C)
		{
			enforceSize();
			VUo_subrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void o_mulC(cvta X, cta C)
		{
			enforceSize();
			VUo_mulC(vx_vect, X.vx_vect, ui_size, C);
		}

		void addC(cvta X, cta C)
		{
			enforceSize();
			VU_addC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subC(cvta X, cta C)
		{
			enforceSize();
			VU_subC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subrC(cvta X, cta C)
		{
			enforceSize();
			VU_subrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void mulC(cvta X, cta C)
		{
			enforceSize();
			VU_mulC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divC(cvta X, cta C)
		{
			enforceSize();
			VU_divC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divrC(cvta X, cta C)
		{
			enforceSize();
			VU_divrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void modC(cvta X, cta C)
		{
			enforceSize();
			VU_modC(vx_vect, X.vx_vect, ui_size, C);
		}

		void maxC(cvta X, cta C)
		{
			enforceSize();
			VU_maxC( vx_vect, X.vx_vect, ui_size, C);
		}

		void minC(cvta X, cta C)
		{
			enforceSize();
			VU_minC( vx_vect, X.vx_vect, ui_size, C);
		}

		void o_addV(cvta X, cvta Y)
		{
			enforceSize();
			VUo_addV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void o_subV(cvta X, cvta Y)
		{
			enforceSize();
			VUo_subV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void o_subrV(cvta X, cvta Y)
		{
			enforceSize();
			VUo_subrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void o_mulV(cvta X, cvta Y)
		{
			enforceSize();
			VUo_mulV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void addV(cvta X, cvta Y)
		{
			enforceSize();
			VU_addV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subV(cvta X, cvta Y)
		{
			enforceSize();
			VU_subV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subrV(cvta X, cvta Y)
		{
			enforceSize();
			VU_subrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void mulV(cvta X, cvta Y)
		{
			enforceSize();
			VU_mulV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divV(cvta X, cvta Y)
		{
			enforceSize();
			VU_divV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divrV(cvta X, cvta Y)
		{
			enforceSize();
			VU_divrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void modV(cvta X, cvta Y)
		{
			enforceSize();
			VU_modV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void maxV(cvta X, cvta Y)
		{
			enforceSize();
			VU_maxV( vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void minV(cvta X, cvta Y)
		{
			enforceSize();
			VU_minV( vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Accumulation
//
		void accV(cvta X)
		{
			enforceSize();
			VU_accV(vx_vect,X.vx_vect,ui_size);
		}

		//defined by including ubVecObj.h
		void accVUB(const ubVecObj& X);

		//defined by including usVecObj.h
		void accVUS(const usVecObj& X);

#ifndef V_HUGE //uiVecObj members
#define accVU accV		
#endif //V_HUGE


/////////////////////////////////////////////////////////////////////////////////////////
//Bit-wise operations
//
		void shl(cvta X, unsigned C)
		{
			enforceSize();
			VU_shl(vx_vect,X.vx_vect,ui_size,C);
		}

		void shr(cvta X, unsigned C)
		{
			enforceSize();
			VU_shr(vx_vect,X.vx_vect,ui_size,C);
		}

		void and(cvta X, cta C)
		{
			enforceSize();
			VU_and(vx_vect,X.vx_vect,ui_size,C);
		}

		void or(cvta X, cta C)
		{
			enforceSize();
			VU_or(vx_vect,X.vx_vect,ui_size,C);
		}

		void xor(cvta X, cta C)
		{
			enforceSize();
			VU_xor(vx_vect,X.vx_vect,ui_size,C);
		}

		void not(cvta X)
		{
			enforceSize();
			VU_not(vx_vect,X.vx_vect,ui_size);
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
                    VU_xyAutoPlot( X.vx_vect, vx_vect, ui_size, form, color );
                }

                void xy2AutoPlot( cvta X1, unsigned form1, COLORREF color1,
                         cvta X2, cvta Y2, unsigned form2, COLORREF color2 )
                {
                    enforceSize();
                    enforceSize(X1.ui_size);
                    Y2.enforceSize(X2.ui_size);
                    VU_xy2AutoPlot( X1.vx_vect, vx_vect, ui_size, form1, color1,
                              X2.vx_vect, Y2.vx_vect, X2.ui_size, form2, color2 );
                }

                void yAutoPlot( unsigned form, COLORREF color )
                {
                    enforceSize();
                    VU_yAutoPlot( vx_vect, ui_size, form, color );
                }

                void y2AutoPlot( unsigned form1, COLORREF color1,
                        cvta Y2, unsigned form2, COLORREF color2 )
                {
                    enforceSize();
                    Y2.enforceSize();
                    VU_y2AutoPlot( vx_vect, ui_size, form1, color1,
                             Y2.vx_vect, Y2.ui_size, form2, color2 );
                }

                void xyDataPlot( cvta X, unsigned form, COLORREF color )
                {
                    enforceSize();
                    enforceSize(X.ui_size);
                    VU_xyDataPlot( X.vx_vect, vx_vect, ui_size, form, color );
                }

                void yDataPlot( unsigned form, COLORREF color )
                {
                    enforceSize();
                    VU_yDataPlot( vx_vect, ui_size, form, color );
                }
            #endif   // __VGRAPH_H

	};


	inline void uiVecObj::sortind(const uVecObj& X, int dir)
	{
		enforceSize();
		VU_sortind(vx_vect,X.vx_vect,ui_size,dir);
	}

	inline void uiVecObj::searchV(const uVecObj& X, const uVecObj& Tab, int mode=0)
	{
		enforceSize();
		Tab.enforceSize();
		VU_searchV(vx_vect,X.vx_vect,ui_size,Tab.vx_vect,Tab.ui_size,mode);
	}

#ifdef UBVECOBJ_H
	inline void ubVecObj::UtoUB(const uVecObj& X)
	{
		enforceSize();
		V_UtoUB(vx_vect,X.vx_vect,ui_size);
	}

	inline void uVecObj::UBtoU(const ubVecObj& X)
	{
		enforceSize();
		V_UBtoU(vx_vect,X.vx_vect,ui_size);
	}

	inline void uVecObj::accVUB(const ubVecObj& X)
	{
		enforceSize();
		VU_accVUB(vx_vect,X.vx_vect,ui_size);
	}
#endif //UBVECOBJ_H

#ifdef USVECOBJ_H
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
#endif //USVECOBJ

#ifdef ULVECOBJ_H
	inline void uVecObj::o_ULtoU(const ulVecObj& X)
	{
		enforceSize();
		Vo_ULtoU(vx_vect,X.vx_vect,ui_size);
	}

	inline void uVecObj::ULtoU(const ulVecObj& X)
	{
		enforceSize();
		V_ULtoU(vx_vect,X.vx_vect,ui_size);
	}

	inline void ulVecObj::UtoUL(const uVecObj& X)
	{
		enforceSize();
		V_UtoUL(vx_vect,X.vx_vect,ui_size);
	}

	inline void ulVecObj::accVU(const uVecObj& X)
	{
		enforceSize();
		VUL_accVU(vx_vect,X.vx_vect,ui_size);
	}
#endif //ULVECOBJ_H

#ifdef IVECOBJ_H
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
#endif //IVECOBJ_H

#ifdef QIVECOBJ_H
	inline void qiVecObj::accVU(const uVecObj& X)
	{
		enforceSize();
		VQI_accVU(vx_vect,X.vx_vect,ui_size);
	}
#endif //QIVECOBJ_H

#ifdef FVECOBJ_H
	inline void fVecObj::accVU(const uVecObj& X)
	{
		enforceSize();
		VF_accVU(vx_vect,X.vx_vect,ui_size);
	}

	inline void fVecObj::UtoF(const uVecObj& X)
	{
		enforceSize();
		V_UtoF(vx_vect,X.vx_vect,ui_size);
	}

	inline int uVecObj::roundtoU(const fVecObj& X)
	{
		enforceSize();
		return VF_roundtoU(vx_vect,X.vx_vect,ui_size);
	}

	inline int uVecObj::floortoU(const fVecObj& X)
	{
		enforceSize();
		return VF_floortoU(vx_vect,X.vx_vect,ui_size);
	}

	inline int uVecObj::ceiltoU(const fVecObj& X)
	{
		enforceSize();
		return VF_ceiltoU(vx_vect,X.vx_vect,ui_size);
	}

	inline int uVecObj::choptoU(const fVecObj& X)
	{
		enforceSize();
		return VF_choptoU(vx_vect,X.vx_vect,ui_size);
	}
#endif //FVECOBJ_H

#ifdef DVECOBJ_H
	inline void dVecObj::accVU(const uVecObj& X)
	{
		enforceSize();
		VD_accVU(vx_vect,X.vx_vect,ui_size);
	}

	inline void dVecObj::UtoD(const uVecObj& X)
	{
		enforceSize();
		V_UtoD(vx_vect,X.vx_vect,ui_size);
	}

	inline int uVecObj::roundtoU(const dVecObj& X)
	{
		enforceSize();
		return VD_roundtoU(vx_vect,X.vx_vect,ui_size);
	}

	inline int uVecObj::floortoU(const dVecObj& X)
	{
		enforceSize();
		return VD_floortoU(vx_vect,X.vx_vect,ui_size);
	}

	inline int uVecObj::ceiltoU(const dVecObj& X)
	{
		enforceSize();
		return VD_ceiltoU(vx_vect,X.vx_vect,ui_size);
	}

	inline int uVecObj::choptoU(const dVecObj& X)
	{
		enforceSize();
		return VD_choptoU(vx_vect,X.vx_vect,ui_size);
	}
#endif //DVECOBJ_H

#ifdef EVECOBJ_H
#ifdef __BORLANDC__ //80-bit numbers supported
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

#define UVECOBJ_H
#endif //UVECOBJ_H
