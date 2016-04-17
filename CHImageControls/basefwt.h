
#ifndef BASEFWT2D_H
#define BASEFWT2D_H

/*
     BaseFWT2D (abstract)

    //derived (redefine transrows, transcols, synthrows, synthcols)
       -FWT2D
       -mHaar
       -mBior53
       -mBior97
*/

class vec1D;

class BaseFWT2D
{        
public:
    BaseFWT2D(const wchar_t* fname);
    BaseFWT2D(const wchar_t* fname, const float* tH, unsigned int thL, int thZ,
                const float* tG, unsigned int tgL, int tgZ,
                const float* H, unsigned int hL, int hZ,
                const float* G, unsigned int gL, int gZ);
    virtual ~BaseFWT2D();

// Operators

// Operations
    void init(unsigned int width, unsigned int height);                                 //allocate buffers for transforms  status=1
    void init(char* data, char* tdata, unsigned int width, unsigned int height);        //use supplied buffers  status=2
    void close(void);                                                                   //deallocate buffers        

    //FWT transforms
    int trans(unsigned int scales, unsigned int th = 0);                               //forward transform data in spec buffer
    int trans(const char* data, unsigned int scales, unsigned int th = 0);             //forward transform DC corrected data
    int trans(const unsigned char* data, unsigned int scales, unsigned int th = 0);    //forward transform DC uncorrected data
    int synth();                                                                       //synthesis fwt spectrum in spec buffer
    int synth(char* data);                                                             //synthesis to DC corrected data buffer
    int synth(unsigned char* data);                                                    //synthesis to DC uncorrected data buffer

    //denoising
    void remove_LLband();

// Access

// Inquiry
    const wchar_t* status(int& status) const;                                           //get status after constructor
    void tracefilters(const wchar_t* fname) const;                                      //trace filters

    inline unsigned int width() const;                                                  //data width
    inline unsigned int height() const;                                                 //data height
    inline char* getspec() const;                                                       //get spectrum buffer
    inline char** getspec2d() const;                                                    //get 2D spectrum buffer
    inline char* gettspec() const;                                                      //get temp fwt buffer

    inline unsigned int getJ() const;
    inline void setJ(unsigned int j);                


protected:
    inline const vec1D& gettH() const;
    inline const vec1D& gettG() const;
    inline const vec1D& getH2m() const;
    inline const vec1D& getG2m() const;
    inline const vec1D& getH2m1() const;
    inline const vec1D& getG2m1() const;

    inline unsigned int getTH() const;

    inline char mmxround(float f) const;
    inline char mmxroundTH(float f) const;

    //virtual functions should be defined in derived ones
    virtual void transrows(char** dest, char** sour, unsigned int w, unsigned int h) const = 0;      //transform rows
    virtual void transcols(char** dest, char** sour, unsigned int w, unsigned int h) const = 0;      //transform cols
    virtual void synthrows(char** dest, char** sour, unsigned int w, unsigned int h) const = 0;      //transform rows
    virtual void synthcols(char** dest, char** sour, unsigned int w, unsigned int h) const = 0;      //transform cols


private:

    int m_status;   //constructor status 0 - not initilized, 
                    //1 - class allocated its own buffers, separate for spec and tspec
                    //2 - class was supplied with buffers for spec and tspec

    wchar_t filter[_MAX_PATH];                      //filter name

    vec1D* tH;                                      //transform filters
    vec1D* tG;
    vec1D* H;                                       //synthesis filters (never used directly)
    vec1D* G;
    vec1D* H2m;                                     //even 2m coeffs from H,G filters
    vec1D* G2m;
    vec1D* H2m1;                                    //odd 2m+1 coeffs from H,G filters
    vec1D* G2m1;

    unsigned int J;                                 //number of scales
    unsigned int TH;                                //threshold for denoising

    unsigned int m_width;                           //array width
    unsigned int m_height;                          //array height
    char* spec;                                     //2*width*height fwt spectrum
    char* tspec;                                    //spec + width*height temp fwt buffer
    char** spec2d;                                  //spec 2D version with addreses from spec
    char** tspec2d;                                 //tspec 2D version with addreses from spec


    vec1D* loadfilter(FILE* flt) const;             //load filter from file  0-error
    void makeHGsynth(void);                         //create 2m,2m1 filters from H,G   -2,-1,0,1,2,3

    void mmxmemcpy(char* dest, const char* sour, unsigned int size);                 //copy mem routine
    void sub128(char* dest, const unsigned char* sour, unsigned int size);           //dest = sour - 128
    void add128(unsigned char *dest, const char* sour, unsigned int size);           //dest = sour + 128

	//not intended functions
    BaseFWT2D(BaseFWT2D& basefwt);
    const BaseFWT2D& operator=(const BaseFWT2D& basefwt);

};



/*
 filter.flt

  size zerooffset
   -3   coeff1
   -2   coeff2
   -1   coeff3
    0   coeff4
    1   coeff5
    2   coeff6

    tH
    tG
    H
    G


  usage:
   FWT2D fwt(L"bior97.flt");
    fwt.init(width,height);    //init buffers
     fwt.trans(data,3,30);     //fwt transform from data buffer J=3,TH=30, data is intact after fwt
     fwt.synth(data);          //fwt synthesis to data buffer
     ...
     ...  //keep going multiple times transforming and synthesing
     ...
    fwt.close();
*/


inline unsigned int BaseFWT2D::getJ() const
{
	return J;
}

inline void BaseFWT2D::setJ(unsigned int j)
{
	J = j;
}

inline unsigned int BaseFWT2D::width() const
{
	return m_width;
}

inline unsigned int BaseFWT2D::height() const
{
	return m_height;
}

inline char* BaseFWT2D::getspec() const
{
	return spec;
}

inline char** BaseFWT2D::getspec2d() const
{
	return spec2d;
}

inline char* BaseFWT2D::gettspec() const
{
	return tspec;
}

inline const vec1D& BaseFWT2D::gettH() const
{
	return *tH;
}

inline const vec1D& BaseFWT2D::gettG() const
{
	return *tG;
}

inline const vec1D& BaseFWT2D::getH2m() const
{
	return *H2m;
}

inline const vec1D& BaseFWT2D::getG2m() const
{
	return *G2m;
}

inline const vec1D& BaseFWT2D::getH2m1() const
{
	return *H2m1;
}

inline const vec1D& BaseFWT2D::getG2m1() const
{
	return *G2m1;
}

inline unsigned int BaseFWT2D::getTH() const
{
	return TH;
}

inline char BaseFWT2D::mmxroundTH(float f) const
{
    char c[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    __m64 *mc = (__m64 *) c;

    __m128 mf = _mm_load_ss(&f);
    *mc = _mm_cvtps_pi8(mf);

    _mm_empty();

    int th = TH;
    if (th != 0) 
	{       
		//denoise
        if (c[0] > 0) 
		{
			if (c[0] <= th)
				c[0] = 0;
        } else 
		{
			if (c[0] >= -th)
				c[0] = 0;
        }
    }

    //if(c[0]%2)                  //quantize
    // c[0] -= c[0]%2;

    return c[0];
}

inline char BaseFWT2D::mmxround(float f) const
{
    char c[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    __m64 *mc = (__m64 *)c;

    __m128 mf = _mm_load_ss(&f);
    *mc = _mm_cvtps_pi8(mf);

    _mm_empty();

    return c[0];
}

#endif
