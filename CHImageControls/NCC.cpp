#include "stdafx.h"
#include "NCC.h"
#include <stdlib.h>
#include <memory.h>
#include <math.h>


double InnerProduct( 
				  unsigned char *sourcePixels,  int sourceCols,  int sourceRows
				 ,unsigned char *patternPixels, int patternCols, int patternRows
				 ,double norm
				 ,int xx
				 ,int yy)
{
	double NCC_value;
	int numerator;
	int i,j,x,y;
	i=yy;j=xx;
	{
		numerator=0;

		for(y=0;y<patternRows;y++)
		for(x=0;x<patternCols;x++)
		{
			int idx1 = (i+y)*sourceCols+ j + x;
			int idx2 = y*patternCols + x;
			numerator+=sourcePixels[idx1]*patternPixels[idx2];
		}

		NCC_value = numerator / (norm);
	}
	return NCC_value;
}


//=================================
// for determining partition order
//=================================
void Gradient(BYTE* source, int rows, int cols, int* GradientMap)

{
	int i,j;
	int index1, index2;

	index1=index2=0;
	memset(GradientMap,0,rows*cols*sizeof(int));

	for(i=0;i<rows-1;i++)
	for(j=0;j<cols-1;j++)
	{
			int idx=i*cols+j;

			GradientMap[idx]=
				abs(source[idx]-source[idx+1]);
//				+abs(source[idx]-source[idx+cols]);

	}
}


//========================================================================
// Winner Update Scheme + Adaptive Partition
//========================================================================
double Adaptive_WUS(
		  unsigned char *sourcePixels,  int sourceCols,  int sourceRows
		 ,unsigned char *patternPixels, int patternCols, int patternRows
		 ,int &best_x
		 ,int &best_y
		 )
{


	int *GradientMap = (int*) new int[patternCols*patternRows];

	//=================================
	//Build Pyramid
	//=================================
	CPyramid TemplatePyd, GradientPyd; //128x128
	CPyramid SourcePyd;    //512x512

	int Pyd_level_max= (int)(log10(double(patternCols))/log10(2.0) + 1.0);

	TemplatePyd.Init(patternRows, patternCols, Pyd_level_max);
	GradientPyd.Init(patternRows, patternCols, Pyd_level_max);
	SourcePyd.Init(sourceRows, sourceCols, Pyd_level_max);

	//==================================	

	CPyramid *pCandidatePyd, *pTemplatePyd;
	CPyramid *cur_GPyd;


	int i;

	TemplatePyd.BuildPyd(patternPixels, patternRows, patternCols, Pyd_level_max);//, &operation_count);						
	pTemplatePyd=&TemplatePyd;

	Gradient(patternPixels, patternRows, patternCols, GradientMap);//, &operation_count);

	GradientPyd.BuildPyd(GradientMap, patternRows, patternCols, Pyd_level_max);//, &operation_count);
	cur_GPyd=&GradientPyd;

	SourcePyd.BuildPyd(sourcePixels, sourceRows, sourceCols, Pyd_level_max);//, &operation_count);
	pCandidatePyd=&SourcePyd;


	int reject_level_max=(patternRows*patternCols-1)/3;


	//=========================================
	//Determin Elimination Order by Cur_GPyd
	//=========================================
	CheckingPoint *OrderQueue=(CheckingPoint*)malloc(sizeof(CheckingPoint)*reject_level_max);
	CheckingPoint *ResidualQueue=(CheckingPoint*)malloc(sizeof(CheckingPoint)*reject_level_max);

	int cx=0;
	int cy=0;

	//====================================
	//splitting order based on Gradient
	//====================================
	OrderQueue[0].x=0;//cx;
	OrderQueue[0].y=0;//cy;
	OrderQueue[0].level=0;
	OrderQueue[0].g=cur_GPyd->ImgLayer[0].Array2D[cy][cx];
	int Qfront=0;
	int Qrear=1;



	int Rfront=0;
	int Rrear=0;
	int g[4];

	while(Qfront<Qrear)
	{
		//================================================
		// Select the max one of gradient from queue
		//================================================

		//searching for max gradient
		int max=OrderQueue[Qfront].g;
		int idx=Qfront;
		for(i=Qfront;i<Qrear;i++)
		{
			if(max<OrderQueue[i].g)
			{
				max=OrderQueue[i].g;
				idx=i;
			}
		}

		//exchange the max one with the front one
		CheckingPoint t;
		memcpy(&t, &OrderQueue[Qfront],sizeof(CheckingPoint));
		memcpy(&OrderQueue[Qfront], &OrderQueue[idx], sizeof(CheckingPoint));
		memcpy(&OrderQueue[idx], &t, sizeof(CheckingPoint));


		//=================================
		//Pop checking point from Queue
		//=================================
		int xx=OrderQueue[Qfront].x;
		int yy=OrderQueue[Qfront].y;
		int level=OrderQueue[Qfront].level;
		int gg=OrderQueue[Qfront].g;
		Qfront++;

		if(level >= Pyd_level_max-2)//4)
			continue;

		//=================================
		//Divide region to 4 sub-block
		//cal homogeneity of 4 sub-block
		//=================================
		level++;
		int h=patternRows>>level;

		g[0]=cur_GPyd->ImgLayer[level].Array2D[yy+cy  ][xx+cx  ];
		g[1]=cur_GPyd->ImgLayer[level].Array2D[yy+cy  ][xx+cx+h];
		g[2]=cur_GPyd->ImgLayer[level].Array2D[yy+cy+h][xx+cx  ];
		g[3]=cur_GPyd->ImgLayer[level].Array2D[yy+cy+h][xx+cx+h];

		//========================
		//Push g into queue
		//========================
		if(g[0]>=_G_TH)
		{
			OrderQueue[Qrear].x=xx;
			OrderQueue[Qrear].y=yy;
			OrderQueue[Qrear].level=level;
			OrderQueue[Qrear].g=g[0];//*scale[level];
			Qrear++;
		}else{
			ResidualQueue[Rrear].x=xx;
			ResidualQueue[Rrear].y=yy;
			ResidualQueue[Rrear].level=level;
			Rrear++;
		}
		if(g[1]>=_G_TH)
		{
			OrderQueue[Qrear].x=xx+h;
			OrderQueue[Qrear].y=yy;
			OrderQueue[Qrear].level=level;
			OrderQueue[Qrear].g=g[1];//*scale[level];
			Qrear++;
		}else{
			ResidualQueue[Rrear].x=xx+h;
			ResidualQueue[Rrear].y=yy;
			ResidualQueue[Rrear].level=level;
			Rrear++;
		}
		if(g[2]>=_G_TH)
		{
			OrderQueue[Qrear].x=xx;
			OrderQueue[Qrear].y=yy+h;
			OrderQueue[Qrear].level=level;
			OrderQueue[Qrear].g=g[2];//*scale[level];
			Qrear++;
		}else{
			ResidualQueue[Rrear].x=xx;
			ResidualQueue[Rrear].y=yy+h;
			ResidualQueue[Rrear].level=level;
			Rrear++;
		}
		if(g[3]>=_G_TH)
		{
			OrderQueue[Qrear].x=xx+h;
			OrderQueue[Qrear].y=yy+h;
			OrderQueue[Qrear].level=level;
			OrderQueue[Qrear].g=g[3];//*scale[level];
			Qrear++;
		}else{
			ResidualQueue[Rrear].x=xx+h;
			ResidualQueue[Rrear].y=yy+h;
			ResidualQueue[Rrear].level=level;
			Rrear++;
		}
	}//while(Qfront<Qrear)


	int level_max=Qrear;


	double norm_template, norm_candidate;
	double norm;
	int idx=0;

	//=======================================================
	//Init WUS
	//=======================================================

	//=============================
	//Build Hash
	//=============================
	int num = (sourceRows-patternRows+1) * (sourceCols-patternCols+1);

	//==================
	//Memory Allocation
	//==================
	CHash* Hash    = (CHash*)malloc( sizeof(CHash) * _HASH_SIZE);
	LISTARRAY* listarray=(LISTARRAY*) malloc(sizeof(LISTARRAY) * num);

	int temp_hash_idx;

	//===============================================================================

	int** Template2D;
	int** Candidate2D;

	int px,py,plevel;

	double BoundryValue, UB;

	int xx=0;
	int yy=0;


	//================
	//Init Hash
	//================
	for(i=0;i<_HASH_SIZE;i++)
	{
		Hash[i].front = -1;
		Hash[i].end   = -1;
	}


	//================
	//Init ListArray
	//================

	Candidate2D = SourcePyd.ImgLayer[0].Array2D;
	Template2D = TemplatePyd.ImgLayer[0].Array2D;

	norm_template=sqrt(double(Template2D[cy][cx]));
	i=0;


	int h;
	double v1,v2,v3,v4;

	//===========================
	//pop block from Queue,
	//===========================
	idx=0;
	px=OrderQueue[idx].x;
	py=OrderQueue[idx].y;
	plevel=OrderQueue[idx].level;


	plevel++;
	Template2D  = TemplatePyd.ImgLayer[plevel].Array2D;
	Candidate2D = SourcePyd.ImgLayer[plevel].Array2D;

	int** Candidate2D0;
	Candidate2D0 = SourcePyd.ImgLayer[0].Array2D;

	h=patternCols>>plevel;
	
	for(yy=0; yy<sourceRows-patternRows+1; ++yy) //Search range Y
	for(xx=0; xx<sourceCols-patternCols+1; ++xx) //Search range X
	{

		//====================
		//similarity measure
		//====================

		//======================================
		// norm_candidate
		//======================================

		norm_candidate=sqrt(double(Candidate2D0[yy][xx]));

		v1=sqrt(double(Candidate2D[yy+py  ][xx+px  ])*(Template2D[cy+py  ][cx+px  ]));
		v2=sqrt(double(Candidate2D[yy+py+h][xx+px  ])*(Template2D[cy+py+h][cx+px  ]));
		v3=sqrt(double(Candidate2D[yy+py  ][xx+px+h])*(Template2D[cy+py  ][cx+px+h]));
		v4=sqrt(double(Candidate2D[yy+py+h][xx+px+h])*(Template2D[cy+py+h][cx+px+h]));
		UB=v1+v2+v3+v4;

		//==================
		//Init data
		//==================
		listarray[i].LEVEL=1;
		listarray[i].Hash_UB_table=UB;
		listarray[i].Hash_Norm  =norm_candidate*norm_template;
		listarray[i].position.x=xx;
		listarray[i].position.y=yy;

		BoundryValue=UB/(norm_template*norm_candidate);


		//===========================
		// Calculate Bucket Number
		//===========================
		if(BoundryValue>=1.0) temp_hash_idx=0;
		else temp_hash_idx = (int)(_HASH_SIZE-BoundryValue*_HASH_SIZE);




		int temp_list_idx = Hash[temp_hash_idx].end;		

		//==================
		//If new bucket
		//==================
		if(Hash[temp_hash_idx].front == -1)
		{
			Hash[temp_hash_idx].front = i;
			Hash[temp_hash_idx].end   = i;
			Hash[temp_hash_idx].len   = 0;
		}
		else
		{
			//==========================
			//add to the end of list
			//==========================
			listarray[temp_list_idx].next = i;

			//==========================
			//update hash.end
			//==========================
			Hash[temp_hash_idx].end = i;
		}
		listarray[i].next = -1;//end of list



		i++;
	}





	//===========================================
	//Winner Update Strategy
	//===========================================
	int IsReachTop=_FALSE;
	int idxNonEmpty=0;
	
	int Idx_c_nn;
	int level;

	int g_count=0;

	double dv;

	//=======================================================
	//Update Winner Until Reach the Max Level
	//=======================================================
	while( !IsReachTop )
	{
		//=============================
		//Finding the non_empty bucket
		//=============================
		while ( Hash[idxNonEmpty].front == -1) 
			idxNonEmpty++;

		//=====================================
		//pop temporary winner from List_array
		//=====================================
		Idx_c_nn=Hash[idxNonEmpty].front;
		
		//================================================================
		//Upate the Upper Bound (UB) and LEVEL of temporary winnder
		//================================================================
		level         = listarray[Idx_c_nn].LEVEL;
		UB            = listarray[Idx_c_nn].Hash_UB_table;
		norm          = listarray[Idx_c_nn].Hash_Norm;
		xx            = listarray[Idx_c_nn].position.x;
		yy            = listarray[Idx_c_nn].position.y;


		idx=level;

		//===============================================
		//Calculate Boundary Value of temporary winner
		//===============================================

		//===========================
		//pop block from Queue,
		//===========================
		px=OrderQueue[idx].x;
		py=OrderQueue[idx].y;
		plevel=OrderQueue[idx].level;

		//====================================
		//subtract the value of block
		//====================================
		Template2D  = TemplatePyd.ImgLayer[plevel].Array2D;
		Candidate2D = SourcePyd.ImgLayer[plevel].Array2D;
		
		dv=sqrt(double(Candidate2D[yy+py][xx+px])*(Template2D[cy+py][cx+px]));
		
		UB-=dv;
		
		
		plevel++;
		
		//===================================
		//add the value of four sub_blocks
		//===================================
		Template2D  = TemplatePyd.ImgLayer[plevel].Array2D;
		Candidate2D = SourcePyd.ImgLayer[plevel].Array2D;
		
		h=patternCols>>plevel;

		v1=sqrt(double(Candidate2D[yy+py  ][xx+px  ])*(Template2D[cy+py  ][cx+px  ]));
		v2=sqrt(double(Candidate2D[yy+py+h][xx+px  ])*(Template2D[cy+py+h][cx+px  ]));
		v3=sqrt(double(Candidate2D[yy+py  ][xx+px+h])*(Template2D[cy+py  ][cx+px+h]));
		v4=sqrt(double(Candidate2D[yy+py+h][xx+px+h])*(Template2D[cy+py+h][cx+px+h]));
		
		// update Upper Bound
		UB=UB+v1+v2+v3+v4;

		// update Boundary value
		BoundryValue=UB/(norm);


		//===============================================
		// Push temporary winner into Hash Table
		//===============================================

		//===============================================
		// Calculate Bucket Number by Boundary Value
		//===============================================
		if(BoundryValue>=1.0) temp_hash_idx=0;
		else temp_hash_idx = (int)(_HASH_SIZE-BoundryValue*_HASH_SIZE);

		// Update level
		level++;

		listarray[Idx_c_nn].LEVEL = level;
		listarray[Idx_c_nn].Hash_UB_table = UB;


		// Is reach the top level?
		if(level >= level_max)
		{
			IsReachTop = _TRUE;
			best_x = listarray[Idx_c_nn].position.x;
			best_y = listarray[Idx_c_nn].position.y;

			break;
		}
		else
		{
			//======================================================
			//remove from node_array, point to the next position
			//======================================================
			Hash[idxNonEmpty].front = listarray[Idx_c_nn].next;
		}


		//============================================
		//push temporary winner back to Hash Table
		//============================================
		int temp_list_idx = Hash[temp_hash_idx].end;		

		//==============================
		//If new bucket
		//==============================
		if(Hash[temp_hash_idx].front == -1)
		{
			Hash[temp_hash_idx].front = Idx_c_nn;
			Hash[temp_hash_idx].end   = Idx_c_nn;
			Hash[temp_hash_idx].len   = 0;
		}
		else
		{
			//==============================
			//add to the end of list
			//==============================
			listarray[temp_list_idx].next = Idx_c_nn;
			//update hash.end
			Hash[temp_hash_idx].end = Idx_c_nn;
			Hash[temp_hash_idx].len++;
		}

		listarray[Idx_c_nn].next = -1;

		g_count++;
	}


	double NCC_max;
	double NCC_value;
	int best_idx;
	if(IsReachTop)
	{
		// Checking all candidates in the same bucket
		// bucket: idxNonEmpty
		//===================		
		//init
		//===================		
		int idx=Hash[idxNonEmpty].front;
		int x1 = listarray[idx].position.x;
		int y1 = listarray[idx].position.y;

		//========================
		//similarity measure
		//========================
		best_idx  = idx;
		NCC_max
			=InnerProduct( sourcePixels,  sourceCols,  sourceRows
										 ,patternPixels, patternCols, patternRows
										 ,listarray[idx].Hash_Norm
										 ,x1//j
										 ,y1);//i);
		
		//==============================
		//tracing the next position
		//==============================
		idx=listarray[idx].next;

		while( -1 != idx )
		{
			//=======================
			//similarity measure
			//=======================
			x1 = listarray[idx].position.x;
			y1 = listarray[idx].position.y;


			NCC_value
			=InnerProduct( sourcePixels,  sourceCols,  sourceRows
										 ,patternPixels, patternCols, patternRows
										 ,listarray[idx].Hash_Norm
										 ,x1//j
										 ,y1);//i);


			//===============================
			//update the min and min_idx
			//===============================
			if(NCC_value > NCC_max)
			{
				NCC_max=NCC_value;
				best_idx=idx;
			}

			//tracing
			idx=listarray[idx].next;
		}
	
	}

	best_x = listarray[best_idx].position.x;
	best_y = listarray[best_idx].position.y;


	//==================
	//free memory
	//==================
	free(Hash);
	free(listarray);

	delete [] GradientMap;
	TemplatePyd.ClearPyd();
	GradientPyd.ClearPyd();
	SourcePyd.ClearPyd();
	free(OrderQueue);
	free(ResidualQueue);
	return NCC_max;
}
