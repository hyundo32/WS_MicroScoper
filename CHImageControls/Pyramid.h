#ifndef _PYRAMID_H
#define _PYRAMID_H

class CMatrix
{
public:
	int* Array1D;
	int len;
	
	int** Array2D;
	int row;
	int col;
};

class CPyramid
{
public:
	CPyramid();

	void Init(int row, int col, int level);
	void BuildPyd(unsigned char* Source, int s_row, int s_col, int levelNum);
	void BuildPyd(int*           Source, int s_row, int s_col, int levelNum);
	void ClearPyd(void);

	int layerNum;
	CMatrix* ImgLayer;

	int* buf1Dtemp;
	int** buf2Dtemp;
};

void ClearPyd(CPyramid *p);
void SavePyd(CPyramid *p);
void BuildPyd(unsigned char* Source, int s_row, int s_col, int levelNum, CPyramid *Pyd);

#endif