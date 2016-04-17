#include "stdafx.h"
#include "pyramid.h"
#include <stdlib.h>
#include <memory.h>

CPyramid::CPyramid()
{
	layerNum=0;
	ImgLayer=NULL;
}

void CPyramid::Init(int s_row, int s_col, int levelNum)
{
	int i,j,k;

	//new pyramid
	this->ImgLayer = new CMatrix[levelNum];
	this->layerNum = levelNum;
	
	//calculate level
	int delta=1;
	int level=levelNum-1;
	
	//memory allocation
//	for(i=0;i<levelNum;i++)
	{
		int row =  s_row-delta+1;
		int col =  s_col-delta+1;
		int len = row*col;
		
		int * buf1D = (int*) malloc( len * sizeof(int));
		buf1Dtemp=(int*) malloc( len * sizeof(int));
		
		int** buf2D     = (int**) malloc( row * sizeof(int*));
		buf2Dtemp = (int**) malloc( row * sizeof(int*));
		
		for(j=0;j<row;j++)
		{
			buf2D[j]    = buf1D + col * j;
			buf2Dtemp[j]= buf1Dtemp + col * j;
		}

		this->ImgLayer[level].Array1D= buf1D;
		this->ImgLayer[level].Array2D= buf2D;
		
		this->ImgLayer[level].row = row;
		this->ImgLayer[level].col = col;
		this->ImgLayer[level].len = len;
		
		delta*=2;
		level--;
	}
	
	
	for(k=1; k<levelNum; k++)
	{
		int row =  s_row-delta+1;
		int col =  s_col-delta+1;
		int len = row*col;
		
		int * buf1D = (int*) malloc( len * sizeof(int));
		
		int** buf2D = (int**) malloc( row * sizeof(int*));
		
		for(i=0; i<row; i++)
		{
			buf2D[i]= buf1D + col * i;
		}


		this->ImgLayer[level].Array1D= buf1D;
		this->ImgLayer[level].Array2D= buf2D;
		
		this->ImgLayer[level].row = row;
		this->ImgLayer[level].col = col;
		this->ImgLayer[level].len = len;

		delta*=2;
		level--;
	}
}


void CPyramid::BuildPyd(unsigned char* Source, int s_row, int s_col, int levelNum)
{
	int i,j,k;

	//calculate level	
	int delta=1;
	int level=levelNum-1;
	
	//memory allocation
//	for(i=0;i<levelNum;i++)
	{
		int row =  s_row-delta+1;
		int col =  s_col-delta+1;
		int len = row*col;
		
		for(i=0;i<len;i++)
		{
			this->ImgLayer[level].Array1D[i] = Source[i]*Source[i];
		}	

		delta = delta<<1;
		level--;
	}
	
	
	for(k=1; k<levelNum; k++)
	{
		int** preLevel2D=this->ImgLayer[level+1].Array2D;
		
		int row =  s_row-delta+1;
		int col =  s_col-delta+1;
		int len = row*col;
		
		memset(buf1Dtemp,0,s_row*s_col*sizeof(int));

		int pre_delta=delta>>1;
		for(i=0; i<this->ImgLayer[level+1].row;i++)
		for(j=0; j<col; j++)
		{
			buf2Dtemp[i][j]=preLevel2D[i][j]+preLevel2D[i][j+pre_delta];
		}
		for(i=0; i<row; i++)
		for(j=0; j<col; j++)
		{
			ImgLayer[level].Array2D[i][j]=buf2Dtemp[i][j]+buf2Dtemp[i+pre_delta][j];
		}

		delta = delta<<1;
		level--;
	}
	
}


void CPyramid::BuildPyd(int* Source, int s_row, int s_col, int levelNum)//, unsigned int *operation)
{
	int i,j,k;

	int delta=1;
	int level=levelNum-1;
	
	//memory allocation
//	for(i=0;i<levelNum;i++)
	{
		int row =  s_row-delta+1;
		int col =  s_col-delta+1;
		int len = row*col;
		
		for(i=0;i<len;i++)
		{
			this->ImgLayer[level].Array1D[i] = Source[i]*Source[i];
		}
		
		delta*=2;
		level--;
	}
	
	
	for(k=1; k<levelNum; k++)
	{
		int** preLevel2D=this->ImgLayer[level+1].Array2D;
		
		int row =  s_row-delta+1;
		int col =  s_col-delta+1;
		int len = row*col;
		
		memset(buf1Dtemp,0,s_row*s_col*sizeof(int));

		int pre_delta=delta>>1;
		for(i=0; i<this->ImgLayer[level+1].row;i++)
		for(j=0; j<col; j++)
		{
			buf2Dtemp[i][j]=preLevel2D[i][j]+preLevel2D[i][j+pre_delta];
		}
		for(i=0; i<row; i++)
		for(j=0; j<col; j++)
		{
			ImgLayer[level].Array2D[i][j]=buf2Dtemp[i][j]+buf2Dtemp[i+pre_delta][j];
		}

		delta = delta<<1;
		level--;
	}
}

void CPyramid::ClearPyd(void)
{
	int level=this->layerNum;
	for(int i=0; i<level; i++)
	{
		free( this->ImgLayer[i].Array1D );
		free( this->ImgLayer[i].Array2D );
	}
	free(buf1Dtemp);
	free(buf2Dtemp);		
}

//=================================================================
//=================================================================

void ClearPyd(CPyramid *p)
{
	int level=p->layerNum;
	for(int i=0; i<level; i++)
	{
		free( p->ImgLayer[i].Array1D );
		free( p->ImgLayer[i].Array2D );
	}
}

