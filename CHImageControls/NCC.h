#ifndef NCC_H
#define NCC_H

#define BYTE unsigned char

#include "pyramid.h"

struct CheckingPoint
{
	int x;
	int y;
	int level;
	int g;
};

//============================

#define _G_TH 00
//#define _G_TH 5000//1000

//============================

#define _HASH_SIZE 10000//10000//256
#define _TRUE 1
#define _FALSE 0

class CHash
{
public:
	int front;
	int end;
	int len;
};


// typedef struct tagPOINT
// { 
// 	long x; 
// 	long y; 
// } POINT; 
//  

class LISTARRAY
{
public:

	int next;

	int  LEVEL;
	double Hash_UB_table;
	double Hash_Norm;
	POINT  position;
};



//===========================================================================================


double InnerProduct( 
				  unsigned char *sourcePixels,  int sourceCols,  int sourceRows
				 ,unsigned char *patternPixels, int patternCols, int patternRows
				 ,double norm_candidate
				 ,double norm_template
				 ,int xx
				 ,int yy);



double Adaptive_WUS(
		  unsigned char *sourcePixels,  int sourceCols,  int sourceRows
		 ,unsigned char *patternPixels, int patternCols, int patternRows
		 ,int &best_x
		 ,int &best_y
		 );

#endif