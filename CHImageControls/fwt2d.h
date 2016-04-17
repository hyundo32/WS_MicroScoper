
#ifndef FWT2D_H
#define FWT2D_H

class BaseFWT2D;

class FWT2D : public BaseFWT2D    //generic 2D fwt
{
public:
    FWT2D(const wchar_t *fname);
    FWT2D(const wchar_t* fname, const float* tH, unsigned int thL, int thZ,
            const float* tG, unsigned int tgL, int tgZ,
            const float* H, unsigned int hL, int hZ,
            const float* G, unsigned int gL, int gZ);

private:
    void transrows(char** dest, char** sour, unsigned int w, unsigned int h) const;
    void transcols(char** dest, char** sour, unsigned int w, unsigned int h) const;
    void synthrows(char** dest, char** sour, unsigned int w, unsigned int h) const;
    void synthcols(char** dest, char** sour, unsigned int w, unsigned int h) const;
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

#endif