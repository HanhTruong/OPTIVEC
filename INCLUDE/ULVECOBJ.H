/* ulVecObj.H

  object-oriented interface for OptiVec vector functions
  of type "unsigned long"

  This file was written by Brian Dale, Case Western Reserve Univ.

  Copyright (c) 1996-2002 by OptiCode
  All Rights Reserved.
*/

#ifndef ULVECOBJ_H

#include <VecObj.h>

#define cvta const vector<unsigned long>&
#define cuia const ui&
#define cta  const unsigned long&

#define vta  vector<unsigned long>&
#define uia  ui&
#define ta   unsigned long&

#define vt   vector<unsigned long>
#define t    unsigned long
#define tVector ulVector

#define tVecObj ulVecObj
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
		{	if(size){	vx_vect= VUL_vector(size);	ui_size= size;}
			else{	vx_vect= 0;	ui_size= 0;}}

		vector(cuia size, cta fill)
		{	if(!size) //size == 0
			{	vx_vect=0;	ui_size=0;}
			else if(!fill) //fill == 0.0
			{	vx_vect= VUL_vector0(size);	ui_size= size;}
			else
			{	vx_vect= VUL_vector(size);	ui_size= size;	VUL_equC(vx_vect,ui_size,fill);}}

		vector(cvta init)
		{	if(init.ui_size)
			{ vx_vect= VUL_vector(init.ui_size);	ui_size= init.ui_size;	VUL_equV(vx_vect, init.vx_vect, ui_size);}
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
				vx_vect= VUL_vector(size);
				ui_size= size;}}

		void setSize(cvta X)
		{	if(!X.ui_size) free(); //can set size to zero
			if(ui_size != X.ui_size) //only allocate if different sizes
			{	if(vx_vect) V_free(vx_vect);
				vx_vect= VUL_vector(X.ui_size);
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
			VUL_equV(vx_vect, vect.vx_vect, ui_size);
			return *this;}

		vta operator=(cta scalar)
		{	enforceSize();
			VUL_equC(vx_vect, ui_size, scalar);
			return *this;}

		//this is the only really dangerous operator
		ta operator[](const int& index){return vx_vect[index];}

		vta operator+=(cvta X)
		{	enforceSize();
			VUL_accV(vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator+=(cta scalar)
		{	enforceSize();
			VUL_addC(vx_vect, vx_vect, ui_size, scalar);
			return *this;}

		vta operator*=(cvta X)
		{	enforceSize();
			VUL_mulV(vx_vect, vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator*=(cta scalar)
		{	enforceSize();
			VUL_mulC(vx_vect,vx_vect,ui_size,scalar);
			return *this;}

		vt operator+(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VUL_addV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator+(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VUL_addC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator+(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VUL_addC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator-(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VUL_subV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator-(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VUL_subC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator-(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VUL_subrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator*(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VUL_mulV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator*(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VUL_mulC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator*(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VUL_mulC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator/(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VUL_divV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator/(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VUL_divC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator/(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VUL_divrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
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
			if(size) vx_vect= VUL_vector(size);
			ui_size=size;
		}

		void vector0(ui size)
		{
			free();
			if(size) vx_vect= VUL_vector0(size);
			ui_size=size;
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Addressing single vector elements
//
		t* Pelement(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VUL_Pelement(vx_vect,n);
			//not in funcref
		}

		t element(ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VUL_element(vx_vect,n);
			//not in funcref
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Initialization
//

		void equ0()
		{
			enforceSize();
			VUL_equ0(vx_vect,ui_size);
		}

		void equC(cta C)
		{
			enforceSize();
			VUL_equC(vx_vect, ui_size, C);
		}

		void equV(cvta X)
		{
			enforceSize();
			VUL_equV(vx_vect, X.vx_vect, ui_size);
		}

		void o_ramp(cta Start, cta Rise)
		{
			enforceSize();
			VULo_ramp(vx_vect, ui_size, Start, Rise);
		}

		void ramp(cta Start, cta Rise)
		{
			enforceSize();
			VUL_ramp(vx_vect, ui_size, Start, Rise);
		}

		long random(const long& seed, cta Min, cta Max)
		{
			enforceSize();
			return VUL_random(vx_vect, ui_size, seed, Min, Max);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Data-type interconversions
//

		void BItoI(const biVecObj& X);
		void SItoI(const siVecObj& X);
		void LItoI(const liVecObj& X);
		void QItoI(const qiVecObj& X);

		void UtoI(const uVecObj& X);

		//defined by including liVecObj.h
		void o_LItoUL(const liVecObj& X);
		//defined by including liVecObj.h
		void LItoUL(const liVecObj& X);

		//defined by including ubVecObj.h
		void UBtoUL(const ubVecObj& X);
		//defined by including usVecObj.h
		void UStoUL(const usVecObj& X);

		//defined by including uVecObj.h
		void UtoUL(const uVecObj& X);

#ifdef V_HUGE //uiVecObj members
   #define UItoU               ULtoU
   #define UtoUI               UtoUL
   #define UItoUB              ULtoUB
   #define UBtoUI              UBtoUL
   #define UItoUS              ULtoUS
   #define UStoUI              UStoUL
   #define ULtoUI              equV
   #define UItoUL              equV
   #define UItoF               ULtoF
   #define UItoD               ULtoD
   #define UItoE               ULtoE
#endif //V_HUGE


/////////////////////////////////////////////////////////////////////////////////////////
//Index-oriented manipulations
//
		void reflect()
		{
			enforceSize();
			VUL_reflect(vx_vect, ui_size);
		}

		void rev(cvta X)
		{
			enforceSize();
			VUL_rev(vx_vect, X.vx_vect, ui_size);
		}

#ifdef V_HUGE
		void rotate( cvta X, long pos )
#else
		void rotate( cvta X, int pos )
#endif
		{
			enforceSize();
			VUL_rotate(vx_vect, X.vx_vect, ui_size, pos);
		}


		void _delete(ui pos)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VUL_delete(vx_vect, ui_size, pos);
		}

		void insert(ui pos, cta C)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VUL_insert(vx_vect, ui_size, pos, C);
		}

		void sort(cvta X, int dir=1)
		{
			enforceSize();
			VUL_sort(vx_vect,X.vx_vect,ui_size,dir);
		}

		//sortind member of uiVecObj

#ifdef V_HUGE //uiVecObj members
		//defined by including ubVecObj.h
		void sortind(const ubVecObj& X, int dir);

		//defined by including usVecObj.h
		void sortind(const usVecObj& X, int dir);

		//defined by including uVecObj.h
		void sortind(const uVecObj& X, int dir);

		//valid for both ul and ui
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
			VUL_subvector(vx_vect, ui_size, X.vx_vect+start, step);
		}

		void subvector_equC(const unsigned& samp, cta C)
		{
			enforceSize();
			VUL_subvector_equC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_equV(const unsigned& samp, cvta X)
		{
			enforceSize();
			VUL_subvector_equV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void indpick(const vector<ui>& Ind, cvta X);
		void indput(cvta X, const vector<ui>& Ind);

		ui searchC(cta C, int mode=0)
		{
			enforceSize();
			return VUL_searchC(vx_vect, ui_size, C, mode);
		}

		//searchV member of uiVecObj

#ifdef V_HUGE //uiVecObj members
		//defined by including ubVecObj.h
		void searchV(const ubVecObj& X, const ubVecObj& Tab, int mode);

		//defined by including usVecObj.h
		void searchV(const usVecObj& X, const usVecObj& Tab, int mode);

		//defined by including uVecObj.h
		void searchV(const uVecObj& X, const uVecObj& Tab, int mode);

		//valid for both ul and ui
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
			return VULo_sum(vx_vect, ui_size);
		}

		void o_runsum(cvta X)
		{
			enforceSize();
			VULo_runsum(vx_vect, X.vx_vect, ui_size);
		}

		t sum()
		{
			enforceSize();
			return VUL_sum(vx_vect, ui_size);
		}

		void runsum(cvta X)
		{
			enforceSize();
			VUL_runsum(vx_vect, X.vx_vect, ui_size);
		}

		//underscore needed because max defined in windef.h
		t _max()
		{
			enforceSize();
			return VUL_max(vx_vect, ui_size);
		}

		//underscore needed because min defined in windef.h
		t _min()
		{
			enforceSize();
			return VUL_min(vx_vect, ui_size);
		}

		t maxind(ui* Ind)
		{
			enforceSize();
			return VUL_maxind(Ind, vx_vect, ui_size);
		}

		t minind(ui* Ind)
		{
			enforceSize();
			return VUL_minind(Ind, vx_vect, ui_size);
		}

		double fsum() const
		{
			enforceSize();
			return VUL_fsum(vx_vect,ui_size);
		}

		double mean() const
		{
			enforceSize();
			return VUL_mean(vx_vect,ui_size);
		}

		void runmax(cvta X)
		{
			enforceSize();
			VUL_runmax(vx_vect, X.vx_vect, ui_size);
		}

		void runmin(cvta X)
		{
			enforceSize();
			VUL_runmin(vx_vect, X.vx_vect, ui_size);
		}

		int iselementC(cta C)
		{
			enforceSize();
			return VUL_iselementC(vx_vect, ui_size, C);
		}

		ui iselementV(cvta X, cvta Tab)
		{
			enforceSize();
			return VUL_iselementV(vx_vect, X.vx_vect,
				ui_size, Tab.vx_vect, Tab.ui_size);
		}

#ifdef V_HUGE //uiVecObj members
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

#ifdef V_HUGE //uiVecObj members
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
			VUL_fprint(stream,vx_vect,ui_size,nperline,linewidth);
		}

#if !defined _Windows || defined __FLAT__ || defined _WIN32
		void cprint(unsigned nperline) const
		{
			enforceSize();
			VUL_cprint(vx_vect,ui_size,nperline);
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
			VUL_read(vx_vect,ui_size,stream);
		}

		void write(FILE* stream) const
		{
			enforceSize();
			VUL_write(stream,vx_vect,ui_size);
		}

//nread
//nwrite

		void setWriteFormat(char* formatString) const
		{
			VUL_setWriteFormat(formatString);
		}

		void setWriteSeparate(char* sepString) const
		{
			VUL_setWriteSeparate(sepString);
		}

//setNWriteSeparate

		void store(FILE* stream) const
		{
			enforceSize();
			fwrite(&ui_size,sizeof(ui),1,stream);
			VUL_store(stream,vx_vect,ui_size);
		}

		void recall(FILE* stream)
		{
			ui sz;
			fread(&sz,sizeof(ui),1,stream);
			setSize(sz);
			VUL_recall(vx_vect,ui_size,stream);
		}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//from VXmath.h
//


/////////////////////////////////////////////////////////////////////////////////////////
//Rounding
//
		//defined by including fVecObj.h
		int roundtoUL(const fVecObj& X);

		//defined by including fVecObj.h
		int floortoUL(const fVecObj& X);
		
		//defined by including fVecObj.h
		int ceiltoUL(const fVecObj& X);
		
		//defined by including fVecObj.h
		int choptoUL(const fVecObj& X);
		
		int trunctoUL(const fVecObj& X)
		{
			return choptoUL(X);
		}
		
		//defined by including dVecObj.h
		int roundtoUL(const dVecObj& X);
		
		//defined by including dVecObj.h
		int floortoUL(const dVecObj& X);
		
		//defined by including dVecObj.h
		int ceiltoUL(const dVecObj& X);
		
		//defined by including dVecObj.h
		int choptoUL(const dVecObj& X);
		
		int trunctoUL(const dVecObj& X)
		{
			return choptoUL(X);
		}
		
#ifdef __BORLANDC__ //80-bit numbers supported
		//defined by including eVecObj.h
		int roundtoUL(const eVecObj& X);
		
		//defined by including eVecObj.h
		int floortoUL(const eVecObj& X);
		
		//defined by including eVecObj.h
		int ceiltoUL(const eVecObj& X);
		
		//defined by including eVecObj.h
		int choptoUL(const eVecObj& X);
		
		int trunctoUL(const eVecObj& X)
		{
			return choptoUL(X);
		}
#endif //__BORLANDC__

#ifdef V_HUGE //uiVecObj members
#define roundtoUI roundtoUL
#define floortoUI floortoUL
#define ceiltoUI  ceiltoUL
#define choptoUI  choptoUL		
#define trunctoUI trunctoUL
#endif //V_HUGE


/////////////////////////////////////////////////////////////////////////////////////////
//Comparisons
//
#ifdef V_HUGE //uiVecObj members
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
			VULo_addC(vx_vect, X.vx_vect, ui_size, C);
		}

		void o_subC(cvta X, cta C)
		{
			enforceSize();
			VULo_subC(vx_vect, X.vx_vect, ui_size, C);
		}

		void o_subrC(cvta X, cta C)
		{
			enforceSize();
			VULo_subrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void o_mulC(cvta X, cta C)
		{
			enforceSize();
			VULo_mulC(vx_vect, X.vx_vect, ui_size, C);
		}

		void addC(cvta X, cta C)
		{
			enforceSize();
			VUL_addC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subC(cvta X, cta C)
		{
			enforceSize();
			VUL_subC(vx_vect, X.vx_vect, ui_size, C);
		}

		void subrC(cvta X, cta C)
		{
			enforceSize();
			VUL_subrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void mulC(cvta X, cta C)
		{
			enforceSize();
			VUL_mulC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divC(cvta X, cta C)
		{
			enforceSize();
			VUL_divC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divrC(cvta X, cta C)
		{
			enforceSize();
			VUL_divrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void modC(cvta X, cta C)
		{
			enforceSize();
			VUL_modC(vx_vect, X.vx_vect, ui_size, C);
		}

		void maxC(cvta X, cta C)
		{
			enforceSize();
			VUL_maxC( vx_vect, X.vx_vect, ui_size, C);
		}

		void minC(cvta X, cta C)
		{
			enforceSize();
			VUL_minC( vx_vect, X.vx_vect, ui_size, C);
		}

		void o_addV(cvta X, cvta Y)
		{
			enforceSize();
			VULo_addV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void o_subV(cvta X, cvta Y)
		{
			enforceSize();
			VULo_subV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void o_subrV(cvta X, cvta Y)
		{
			enforceSize();
			VULo_subrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void o_mulV(cvta X, cvta Y)
		{
			enforceSize();
			VULo_mulV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void addV(cvta X, cvta Y)
		{
			enforceSize();
			VUL_addV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subV(cvta X, cvta Y)
		{
			enforceSize();
			VUL_subV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void subrV(cvta X, cvta Y)
		{
			enforceSize();
			VUL_subrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void mulV(cvta X, cvta Y)
		{
			enforceSize();
			VUL_mulV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divV(cvta X, cvta Y)
		{
			enforceSize();
			VUL_divV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divrV(cvta X, cvta Y)
		{
			enforceSize();
			VUL_divrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void modV(cvta X, cvta Y)
		{
			enforceSize();
			VUL_modV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void maxV(cvta X, cvta Y)
		{
			enforceSize();
			VUL_maxV( vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void minV(cvta X, cvta Y)
		{
			enforceSize();
			VUL_minV( vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Accumulation
//
		void accV(cvta X)
		{
			enforceSize();
			VUL_accV(vx_vect,X.vx_vect,ui_size);
		}

		//defined by including ubVecObj.h
		void accVUB(const ubVecObj& X);

		//defined by including usVecObj.h
		void accVUS(const usVecObj& X);

		//defined by including uVecObj.h
		void accVU(const uVecObj& X);


/////////////////////////////////////////////////////////////////////////////////////////
//Bit-wise operations
//
		void shl(cvta X, unsigned C)
		{
			enforceSize();
			VUL_shl(vx_vect,X.vx_vect,ui_size,C);
		}

		void shr(cvta X, unsigned C)
		{
			enforceSize();
			VUL_shr(vx_vect,X.vx_vect,ui_size,C);
		}

		void and(cvta X, cta C)
		{
			enforceSize();
			VUL_and(vx_vect,X.vx_vect,ui_size,C);
		}

		void or(cvta X, cta C)
		{
			enforceSize();
			VUL_or(vx_vect,X.vx_vect,ui_size,C);
		}

		void xor(cvta X, cta C)
		{
			enforceSize();
			VUL_xor(vx_vect,X.vx_vect,ui_size,C);
		}

		void not(cvta X)
		{
			enforceSize();
			VUL_not(vx_vect,X.vx_vect,ui_size);
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
                    VUL_xyAutoPlot( X.vx_vect, vx_vect, ui_size, form, color );
                }

                void xy2AutoPlot( cvta X1, unsigned form1, COLORREF color1,
                         cvta X2, cvta Y2, unsigned form2, COLORREF color2 )
                {
                    enforceSize();
                    enforceSize(X1.ui_size);
                    Y2.enforceSize(X2.ui_size);
                    VUL_xy2AutoPlot( X1.vx_vect, vx_vect, ui_size, form1, color1,
                              X2.vx_vect, Y2.vx_vect, X2.ui_size, form2, color2 );
                }

                void yAutoPlot( unsigned form, COLORREF color )
                {
                    enforceSize();
                    VUL_yAutoPlot( vx_vect, ui_size, form, color );
                }

                void y2AutoPlot( unsigned form1, COLORREF color1,
                        cvta Y2, unsigned form2, COLORREF color2 )
                {
                    enforceSize();
                    Y2.enforceSize();
                    VUL_y2AutoPlot( vx_vect, ui_size, form1, color1,
                             Y2.vx_vect, Y2.ui_size, form2, color2 );
                }

                void xyDataPlot( cvta X, unsigned form, COLORREF color )
                {
                    enforceSize();
                    enforceSize(X.ui_size);
                    VUL_xyDataPlot( X.vx_vect, vx_vect, ui_size, form, color );
                }

                void yDataPlot( unsigned form, COLORREF color )
                {
                    enforceSize();
                    VUL_yDataPlot( vx_vect, ui_size, form, color );
                }
            #endif   // __VGRAPH_H

	};


	inline void uiVecObj::sortind(const ulVecObj& X, int dir)
	{
		enforceSize();
		VUL_sortind(vx_vect,X.vx_vect,ui_size,dir);
	}

	inline void uiVecObj::searchV(const ulVecObj& X, const ulVecObj& Tab, int mode=0)
	{
		enforceSize();
		Tab.enforceSize();
		VUL_searchV(vx_vect,X.vx_vect,ui_size,Tab.vx_vect,Tab.ui_size,mode);
	}

#ifdef UBVECOBJ_H
	inline void ubVecObj::ULtoUB(const ulVecObj& X)
	{
		enforceSize();
		V_ULtoUB(vx_vect,X.vx_vect,ui_size);
	}

	inline void ulVecObj::UBtoUL(const ubVecObj& X)
	{
		enforceSize();
		V_UBtoUL(vx_vect,X.vx_vect,ui_size);
	}

	inline void ulVecObj::accVUB(const ubVecObj& X)
	{
		enforceSize();
		VUL_accVUB(vx_vect,X.vx_vect,ui_size);
	}
#endif //UBVECOBJ_H

#ifdef USVECOBJ_H
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
#endif //USVECOBJ

#ifdef UVECOBJ_H
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
#endif //UVECOBJ_H

#ifdef LIVECOBJ_H
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
#endif //LIVECOBJ_H

#ifdef QIVECOBJ_H
	inline void qiVecObj::accVUL(const ulVecObj& X)
	{
		enforceSize();
		VQI_accVUL(vx_vect,X.vx_vect,ui_size);
	}
#endif //QIVECOBJ_H

#ifdef FVECOBJ_H
	inline void fVecObj::accVUL(const ulVecObj& X)
	{
		enforceSize();
		VF_accVUL(vx_vect,X.vx_vect,ui_size);
	}

	inline void fVecObj::ULtoF(const ulVecObj& X)
	{
		enforceSize();
		V_ULtoF(vx_vect,X.vx_vect,ui_size);
	}

	inline int ulVecObj::roundtoUL(const fVecObj& X)
	{
		enforceSize();
		return VF_roundtoUL(vx_vect,X.vx_vect,ui_size);
	}

	inline int ulVecObj::floortoUL(const fVecObj& X)
	{
		enforceSize();
		return VF_floortoUL(vx_vect,X.vx_vect,ui_size);
	}

	inline int ulVecObj::ceiltoUL(const fVecObj& X)
	{
		enforceSize();
		return VF_ceiltoUL(vx_vect,X.vx_vect,ui_size);
	}

	inline int ulVecObj::choptoUL(const fVecObj& X)
	{
		enforceSize();
		return VF_choptoUL(vx_vect,X.vx_vect,ui_size);
	}
#endif //FVECOBJ_H

#ifdef DVECOBJ_H
	inline void dVecObj::accVUL(const ulVecObj& X)
	{
		enforceSize();
		VD_accVUL(vx_vect,X.vx_vect,ui_size);
	}

	inline void dVecObj::ULtoD(const ulVecObj& X)
	{
		enforceSize();
		V_ULtoD(vx_vect,X.vx_vect,ui_size);
	}

	inline int ulVecObj::roundtoUL(const dVecObj& X)
	{
		enforceSize();
		return VD_roundtoUL(vx_vect,X.vx_vect,ui_size);
	}

	inline int ulVecObj::floortoUL(const dVecObj& X)
	{
		enforceSize();
		return VD_floortoUL(vx_vect,X.vx_vect,ui_size);
	}

	inline int ulVecObj::ceiltoUL(const dVecObj& X)
	{
		enforceSize();
		return VD_ceiltoUL(vx_vect,X.vx_vect,ui_size);
	}

	inline int ulVecObj::choptoUL(const dVecObj& X)
	{
		enforceSize();
		return VD_choptoUL(vx_vect,X.vx_vect,ui_size);
	}
#endif //DVECOBJ_H

#ifdef EVECOBJ_H
#ifdef __BORLANDC__ //80-bit numbers supported
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

#define ULVECOBJ_H
#endif //ULVECOBJ_H
