
#include "stdafx.h"
#include "basefwt.h"
#include "vec1d.h"

///////////////////////////////////constructors/destructors///////////////////////////////////////////////////////////////////
//constructor status 0-not initilized, 1,2-ok after init(), -1,-2,... errors
BaseFWT2D::BaseFWT2D(const wchar_t* fname) : m_status(0),
                tH(0), tG(0), H(0), G(0), H2m(0), G2m(0), H2m1(0), G2m1(0),
                J(0), TH(0), m_width(0), m_height(0),
                spec(0), tspec(0), spec2d(0), tspec2d(0)
{
    wcscpy(filter, fname);

    FILE* flt = _wfopen(fname, L"rt");
    if (flt) 
	{
        if ((tH = loadfilter(flt)) == 0) 
		{
            m_status = -2;
            return;
        }

        if ((tG = loadfilter(flt)) == 0) 
		{
            m_status = -3;
            return;
        }

        if ((H = loadfilter(flt)) == 0) 
		{
            m_status = -4;
            return;
        }

        if ((G = loadfilter(flt)) == 0) 
		{
            m_status = -5;
            return;
        }

        fclose(flt);

        makeHGsynth();
    } 
	else
	{
		m_status = -1;    //filter file failed to open
	}
}

BaseFWT2D::BaseFWT2D(const wchar_t* fname,
                     const float* tH, unsigned int thL, int thZ,
                     const float* tG, unsigned int tgL, int tgZ,
                     const float* H, unsigned int hL, int hZ,
                     const float* G, unsigned int gL, int gZ) : m_status(0),
                tH(0), tG(0), H(0), G(0), H2m(0), G2m(0), H2m1(0), G2m1(0),
                J(0), TH(0), m_width(0), m_height(0),
                spec(0), tspec(0), spec2d(0), tspec2d(0)
{
    wcscpy(filter, fname);

    this->tH = new vec1D(thL, -thZ, tH);
    this->tG = new vec1D(tgL, -tgZ, tG);
    this->H = new vec1D(hL, -hZ, H);
    this->G = new vec1D(gL, -gZ, G);

    makeHGsynth();
}

BaseFWT2D::~BaseFWT2D()
{
    if (tH) delete tH;
    if (tG) delete tG;
    if (H) delete H;
    if (G) delete G;
    if (H2m) delete H2m;
    if (G2m) delete G2m;
    if (H2m1) delete H2m1;
    if (G2m1) delete G2m1;
    close();                //close spec,tspec,spec2d,tspec2d buffers
}
///////////////////////////////////constructors/destructors///////////////////////////////////////////////////////////////////



//////////////////////////////////init/status functions//////////////////////////////////////////////////////////////////////
const wchar_t* BaseFWT2D::status(int& status) const
{
    status = m_status;

    switch (m_status) 
	{

    default:

    case 0:
            return 0;

    case 1:
    case 2:
            return L"ready for transforms";

    case -1:
            return L"failed to open filter file";

    case -2:
            return L"failed to load tH filter";

    case -3:
            return L"failed to load tG filter";

    case -4:
            return L"failed to load H filter";

    case -5:
            return L"failed to load G filter";

    }
}

vec1D* BaseFWT2D::loadfilter(FILE* flt) const
{
    int L, Z;
    if (fwscanf(flt, L"%d %d", &L, &Z) != 2)
		return 0;

    vec1D* wf = new vec1D(L, -Z);

    for (int i = wf->first(); i <= wf->last(); i++) 
	{
        float tmp;
        if (fwscanf(flt, L"%*d %f", &tmp) != 1)
			return 0;
        else
            (*wf)(i) = tmp;
    }

    return wf;
}

void BaseFWT2D::makeHGsynth()
{
    int size2m, offset2m;
    int size2m1, offset2m1;
	
    size2m = 0;
    size2m1 = 0;
    //arrange H2m,H2m1
    for (int m = H->first(); m <= H->last(); m++) //count how many odd even coeffs
	{      
		if (m % 2)
			size2m1++;
		else
			size2m++;
    }
    offset2m = (H->first() - (H->first() % 2)) / 2;
    offset2m1 = (H->first() + (H->first() % 2)) / 2;

    H2m = new vec1D(size2m, offset2m);
    H2m1 = new vec1D(size2m1, offset2m1);

    for (int m = H2m->first(); m <= H2m->last(); m++)
		(*H2m)(m) = (*H)(2 * m);

    for (int m = H2m1->first(); m <= H2m1->last(); m++)
        (*H2m1)(m) = (*H)(2 * m + 1);

	size2m = 0;
    size2m1 = 0;

    //arrange G2m,G2m1
    for (int m = G->first(); m <= G->last(); m++) 
	{
        if (m % 2)
			size2m1++;
        else
            size2m++;
    }
    offset2m = (G->first() - (G->first() % 2)) / 2;
    offset2m1 = (G->first() + (G->first() % 2)) / 2;

    G2m = new vec1D(size2m, offset2m);
    G2m1 = new vec1D(size2m1, offset2m1);

    for (int m = G2m->first(); m <= G2m->last(); m++)
		(*G2m)(m) = (*G)(2 * m);

    for (int m = G2m1->first(); m <= G2m1->last(); m++)
        (*G2m1)(m) = (*G)(2 * m + 1);

}

void BaseFWT2D::tracefilters(const wchar_t* fname) const
{
    FILE* fp = _wfopen(fname, L"wt");
    if (fp) {
        fwprintf(fp, L"tH\n");
        for (int i = tH->first(); i <= tH->last(); i++)
			fwprintf(fp, L" %2d  %g\n", i, (*tH)(i));
        fwprintf(fp, L"\ntG\n");

        for (int i = tG->first(); i <= tG->last(); i++)
			fwprintf(fp, L" %2d  %g\n", i, (*tG)(i));
        fwprintf(fp, L"\nH\n");

        for (int i = H->first(); i <= H->last(); i++)
			fwprintf(fp, L" %2d  %g\n", i, (*H)(i));
        fwprintf(fp, L"\nG\n");

        for (int i = G->first(); i <= G->last(); i++)
			fwprintf(fp, L" %2d  %g\n", i, (*G)(i));
		fwprintf(fp, L"\n\nH2m\n");

        for (int i = H2m->first(); i <= H2m->last(); i++)
			fwprintf(fp, L" %2d  %g\n", 2*i, (*H2m)(i));
        fwprintf(fp, L"\nH2m1\n");

        for (int i = H2m1->first(); i <= H2m1->last(); i++)
			fwprintf(fp, L" %2d  %g\n", 2*i + 1, (*H2m1)(i));
        fwprintf(fp, L"\nG2m\n");

        for (int i = G2m->first(); i <= G2m->last(); i++)
			fwprintf(fp, L" %2d  %g\n", 2*i, (*G2m)(i));
        fwprintf(fp, L"\nG2m1\n");

        for (int i = G2m1->first(); i <= G2m1->last(); i++)
			fwprintf(fp, L" %2d  %g\n", 2*i + 1, (*G2m1)(i));

        fclose(fp);
    }
}

void BaseFWT2D::init(unsigned int width, unsigned int height)
{
    if (width != m_width || height != m_height) 
	{
        close();
        m_width = width;
        m_height = height;

        spec = (char*)malloc(2 * width * height * sizeof(char));
        tspec = spec + width * height;

        spec2d = (char**)malloc(2 * height * sizeof(char*));     //setup rows

        for (unsigned int j = 0; j < 2*height; j++)
			spec2d[j] = &spec[j*width];   //setup cols

        tspec2d = &spec2d[height];

        m_status = 1;
    }
}

void BaseFWT2D::init(char* data, char* tdata, unsigned int width, unsigned int height)
{
    close();
    m_width = width;
    m_height = height;

    spec = data;          //data - tdata  might be not continuous in memory
    tspec = tdata;

    spec2d = (char**)malloc(2 * height * sizeof(char *));     //setup rows

    for (unsigned int j = 0; j < height; j++)
		spec2d[j] = &spec[j*width];     //setup cols

    for (unsigned int j = 0; j < height; j++)
        spec2d[j+height] = &tspec[j*width];   //setup cols

    tspec2d = &spec2d[height];

    m_status = 2;
}

void BaseFWT2D::close(void)
{
    m_width = 0;
    m_height = 0;
    if (m_status == 1) 
	{
        if (spec != 0) 
		{
            free(spec);
            spec = 0;
        }
    }
    if (spec2d != 0) 
	{
        free(spec2d);
        spec2d = 0;
    }
    m_status = 0;
}
//////////////////////////////////init/status functions//////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////transforms/////////////////////////////////////////////////////////////////////
int BaseFWT2D::trans(unsigned int scales, unsigned int th)
{
    if (m_status <= 0)
            return -1;

    J = scales;
    TH = th;
    unsigned int w = m_width;
    unsigned int h = m_height;

    for (unsigned int j = 0; j < J; j++) 
	{
        transrows(tspec2d, spec2d, w, h);
        transcols(spec2d, tspec2d, w, h);
        w /= 2;
        h /= 2;
        TH /= 4;
    }

    return 0;
}

int BaseFWT2D::trans(const char* data, unsigned int scales, unsigned int th)
{
    if (m_status <= 0)
		return -1;

    mmxmemcpy(spec, data, m_width*m_height);    //copy data to spec
    return trans(scales, th);     //fwt transform
}

int BaseFWT2D::trans(const unsigned char* data, unsigned int scales, unsigned int th)
{
    if (m_status <= 0)
		return -1;

    sub128(spec, data, m_width*m_height);    //copy data to spec and correct DC
    return trans(scales, th);    //fwt transform
}
///////////////////////////////////////////////transforms/////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////synths//////////////////////////////////////////////////////////////////////////
int BaseFWT2D::synth()
{
    if (m_status <= 0)
		return -1;

    unsigned int w = m_width / (unsigned int)pow(2.0f, (float)J);
    unsigned int h = m_height / (unsigned int)pow(2.0f, (float)J);

    while (J) 
	{
        synthcols(tspec2d, spec2d, w, h);
        synthrows(spec2d, tspec2d, w, h);
        w *= 2;
        h *= 2;
        J--;
    }

    return 0;
}

int BaseFWT2D::synth(char* data)
{
    int res;
    res = synth();
    
	if (res != 0)
		return res;

	mmxmemcpy(data, spec, m_width*m_height);      //copy restored spec to data
    
	return res;
}

int BaseFWT2D::synth(unsigned char* data)
{
    int res;
    res = synth();
    
	if (res != 0)
		return res;

    add128(data, spec, m_width*m_height);     //copy restored spec to data and correct DC

    return res;
}
//////////////////////////////////////////////synths//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////denoising///////////////////////////////////////////////////////////////////////
void BaseFWT2D::remove_LLband()
{
    if (m_status <= 0)
		return;

    unsigned int width = m_width / (unsigned int)(pow(2.0f, (float)getJ()));
    unsigned int height = m_height / (unsigned int)(pow(2.0f, (float)getJ()));
        
    for (unsigned int y = 0; y < height; y++) 
        for (unsigned int x = 0; x < width; x++)                         
			spec2d[y][x] = 0;                

}
//////////////////////////////////////////////denoising///////////////////////////////////////////////////////////////////////

//////////////////////////mmx routines////////////////////////////////////////////////////////////////////////////////////////
void BaseFWT2D::mmxmemcpy(char* dest, const char* sour, unsigned int size)
{
    //for(int i=0; i<m_width*m_height; i++)  //stub
    //dest[i] = sour[i];
	unsigned int modsize8 = size % 8;
    __m64* pdest = (__m64*)dest;
    const __m64* psour = (__m64*)sour;

    for (unsigned int i = 0; i < size / 8; i++)
		*pdest++ = *psour++;

    if (modsize8) 
	{
        dest = (char*)pdest;
        sour = (char*)psour;
        for (unsigned int i = 0; i < modsize8; i++)
			*dest++ = *sour++;
    }

    _mm_empty();
}

void BaseFWT2D::sub128(char* dest, const unsigned char* sour, unsigned int size)
{
    //for(int i=0; i<m_width*m_height; i++)
    // dest[i] = (char)((int)sour[i]-128);

    unsigned int modsize8 = size % 8;
    __m64 m128;
    m128.m64_u64 = 0x8080808080808080;
    __m64* pdest = (__m64*)dest;
    const __m64* psour = (__m64*)sour;

    for (unsigned int i = 0; i < size / 8; i++)
		*pdest++ = _mm_sub_pi8(*psour++ , m128);

    if (modsize8) 
	{
        dest = (char*)pdest;
        sour = (unsigned char*)psour;
        for (unsigned int i = 0; i < modsize8; i++)
			*dest++ = (char)((int) * sour++ - 128);
    }

    _mm_empty();
}
void BaseFWT2D::add128(unsigned char* dest, const char* sour, unsigned int size)
{
    //for(int i=0; i<m_width*m_height; i++)
    // data[i] = (unsigned char)((int)spec[i]+128);

    unsigned int modsize8 = size % 8;
    __m64 m128;
    m128.m64_u64 = 0x8080808080808080;
    __m64* pdest = (__m64*)dest;
    __m64* psour = (__m64*)sour;

    for (unsigned int i = 0; i < size / 8; i++)
		*pdest++ = _mm_add_pi8(*psour++ , m128);

    if (modsize8) 
	{
        dest = (unsigned char*)pdest;
        sour = (char*)psour;
        for (unsigned int i = 0; i < modsize8; i++)
			*dest++ = (unsigned char)((int) * sour++ + 128);
    }

    _mm_empty();
}

