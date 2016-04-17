

#include "stdafx.h"

#include "vec1d.h"
#include "basefwt.h"
#include "fwt2d.h"



///////////////////////////////////constructors/destructors///////////////////////////////////////////////////////////////////
FWT2D::FWT2D(const wchar_t* fname) : BaseFWT2D(fname)
{
}

FWT2D::FWT2D(const wchar_t* fname, const float* tH, unsigned int thL, int thZ,
             const float* tG, unsigned int tgL, int tgZ,
             const float* H, unsigned int hL, int hZ,
             const float* G, unsigned int gL, int gZ) : BaseFWT2D(fname, tH, thL, thZ, tG, tgL, tgZ, H, hL, hZ, G, gL, gZ)
{
}
///////////////////////////////////constructors/destructors///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////transforms/////////////////////////////////////////////////////////////////////
void FWT2D::transrows(char** dest, char** sour, unsigned int w, unsigned int h) const
{
    int n;
    float s, d;
    unsigned int w2 = w / 2;

    const vec1D& tH = gettH();
    const vec1D& tG = gettG();

    for (unsigned int y = 0; y < h; y++) 
	{
        for (unsigned int k = 0; k < w2; k++) 
		{
			s = 0.0f;
            d = 0.0f;

            for (int m = tH.first(); m <= tH.last(); m++) 
			{
                n = 2 * k + m;
                if (n < 0) n = 0 - n;
                if (n >= (int)w) n -= 2 * (1 + n - w);
                s += tH[m] * float(sour[y][n]);
            }

            for (int m = tG.first(); m <= tG.last(); m++) 
			{
                n = 2 * k + m;
                if (n < 0) n = 0 - n;
                if (n >= (int)w) n -= 2 * (1 + n - w);
                d += tG[m] * float(sour[y][n]);
            }

            dest[y][k] = mmxround(s);
            dest[y][k+w2] = mmxroundTH(d);
        }
    }
}

void FWT2D::transcols(char** dest, char** sour, unsigned int w, unsigned int h) const
{
    int n;
    float s, d;
    unsigned int h2 = h / 2;

    const vec1D& tH = gettH();
    const vec1D& tG = gettG();

    for (unsigned int x = 0; x < w; x++) 
	{
        for (unsigned int k = 0; k < h2; k++) 
		{
            s = 0.0f;
            d = 0.0f;

            for (int m = tH.first(); m <= tH.last(); m++) 
			{
                n = 2 * k + m;
                if (n < 0) n = 0 - n;
                if (n >= (int)h) n -= 2 * (1 + n - h);
                s += tH[m] * float(sour[n][x]);
            }

            for (int m = tG.first(); m <= tG.last(); m++) 
			{
                n = 2 * k + m;
                if (n < 0) n = 0 - n;
                if (n >= (int)h) n -= 2 * (1 + n - h);
                d += tG[m] * float(sour[n][x]);
            }

            if (x < w / 2)
				dest[k][x] = mmxround(s);
            else
                dest[k][x] = mmxroundTH(s); 	        //is this needed? hi band were TH'ed on transrows

            dest[k+h2][x] = mmxroundTH(d);          //is this needed? hi band were TH'ed on transrows on x>w/2
        }
    }
}
///////////////////////////////////////////////transforms/////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////synths//////////////////////////////////////////////////////////////////////////
void FWT2D::synthrows(char** dest, char** sour, unsigned int w, unsigned int h) const     //w,h of the LO part
{
    int n;
    float s2k, s2k1;

    const vec1D& H2m = getH2m();
    const vec1D& G2m = getG2m();
    const vec1D& H2m1 = getH2m1();
    const vec1D& G2m1 = getG2m1();

    for (unsigned int y = 0; y < 2*h; y++) 
	{
        for (unsigned int k = 0; k < w; k++) 
		{
            s2k = 0.0f;
            s2k1 = 0.0f;

            for (int m = H2m.first(); m <= H2m.last(); m++) //s2k even H
			{      
                n = k - m;
                if (n < 0) n = 0 - n;
                if (n >= (int)w) n -= 2 * (1 + n - w);
                s2k += H2m[m] * float(sour[y][n]);
            }

            for (int m = G2m.first(); m <= G2m.last(); m++) //s2k even G
			{      
                n = k - m;
                if (n < 0) n = 0 - n;
                if (n >= (int)w) n -= 2 * (1 + n - w);
                s2k += G2m[m] * float(sour[y][n+w]);
            }

            for (int m = H2m1.first(); m <= H2m1.last(); m++) //s2k1 odd H
			{   
                n = k - m;
                if (n < 0) n = 0 - n;
                if (n >= (int)w) n -= 2 * (1 + n - w);
                s2k1 += H2m1[m] * float(sour[y][n]);
            }

            for (int m = G2m1.first(); m <= G2m1.last(); m++) //s2k1 odd G
			{   
                n = k - m;
                if (n < 0) n = 0 - n;
                if (n >= (int)w) n -= 2 * (1 + n - w);
                s2k1 += G2m1[m] * float(sour[y][n+w]);
            }

            dest[y][2*k] = mmxround(2.0f * s2k);
            dest[y][2*k+1] = mmxround(2.0f * s2k1);
        }
    }
}

void FWT2D::synthcols(char** dest, char** sour, unsigned int w, unsigned int h) const     //w,h of the LO part
{
    int n;
    float s2k, s2k1;

    const vec1D& H2m = getH2m();
    const vec1D& G2m = getG2m();
    const vec1D& H2m1 = getH2m1();
    const vec1D& G2m1 = getG2m1();

    for (unsigned int x = 0; x < 2*w; x++) 
	{
        for (unsigned int k = 0; k < h; k++) 
		{
            s2k = 0;
            s2k1 = 0;

            for (int m = H2m.first(); m <= H2m.last(); m++) //s2k even H
			{      
                n = k - m;
                if (n < 0) n = 0 - n;
                if (n >= (int)h) n -= 2 * (1 + n - h);
                s2k += H2m[m] * float(sour[n][x]);
            }

            for (int m = G2m.first(); m <= G2m.last(); m++) //s2k even G
			{      
                n = k - m;
                if (n < 0) n = 0 - n;
                if (n >= (int)h) n -= 2 * (1 + n - h);
                s2k += G2m[m] * float(sour[n+h][x]);
            }

            for (int m = H2m1.first(); m <= H2m1.last(); m++) //s2k1 odd H
			{    
                n = k - m;
                if (n < 0) n = 0 - n;
                if (n >= (int)h) n -= 2 * (1 + n - h);
                s2k1 += H2m1[m] * float(sour[n][x]);
            }

            for (int m = G2m1.first(); m <= G2m1.last(); m++) //s2k1 odd G
			{    
                n = k - m;
                if (n < 0) n = 0 - n;
                if (n >= (int)h) n -= 2 * (1 + n - h);
                s2k1 += G2m1[m] * float(sour[n+h][x]);
            }

            dest[2*k][x] = mmxround(2.0f * s2k);
            dest[2*k+1][x] = mmxround(2.0f * s2k1);
        }
    }
}
//////////////////////////////////////////////synths//////////////////////////////////////////////////////////////////////////