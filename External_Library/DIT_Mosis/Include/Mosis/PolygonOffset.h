#pragma once

#include "Pad.h"

class CPolygonOffset
{
public:
	void Offset(CPadPoligon &from, CPadPoligon &to, int margin)
	{
		CPadPoint *pt1, *pt2, *pt3; 
		int x, y;
		CPadPoint pt;
		to.ResetPoligon();
		for(int i= 0; i< from.GetPointCount(); i++)
		{
			pt1= from.GetPadPoint_Round(from.GetPreIndex(i));
			pt2= from.GetPadPoint_Round(i);
			pt3= from.GetPadPoint_Round(i+1);
			CrossLine(pt1->x, pt1->y, pt2->x, pt2->y, pt3->x, pt3->y, margin, pt.x, pt.y);
			//pt.x= pt2->x+ 10;
			//pt.y= pt2->y+ 10;
			to.AddPoint(pt);
		}
		to.CalOutRect();
	}
	BOOL CrossLine(int x1, int y1, int x2, int y2, int x3, int y3, int dist, int &crx, int &cry)
	{
		double xx1, yy1, xx2, yy2, xx22, yy22, xx3, yy3;
		trans(x1, y1, x2, y2, dist, xx1, yy1, xx2, yy2);
		trans(x2, y2, x3, y3, dist, xx22, yy22, xx3, yy3);

		double dx, dy, dx2, dy2;
		double slope, slope2, cross, cross2;

		dx= xx2 - xx1; dy= yy2 - yy1;
		if(dx == 0.)
		{
			slope= 0.;
		}else
		{
			slope= (double)dy/dx;
		}
		if(dy == 0.)
		{
			slope= 0.;
		}
		cross= -slope*xx1+ yy1;

		dx2= xx3 - xx22; dy2= yy3 - yy22;
		if(dx2 == 0.)
		{
			slope2= 0.;
		}else
		{
			slope2= (double)dy2/dx2;
		}
		if(dy2 == 0.)
		{
			slope2= 0.;
		}
		cross2= -slope2*xx22+ yy22;

		if(dx == 0.)
		{
			crx= xx2;
			cry= crx*slope2+ cross2;
			return FALSE;
		}

		if(dx2 == 0.)
		{
			crx= xx3;
			cry= crx*slope+ cross;
			return FALSE;
		}

// 		if(dy == 0 && dy2 == 0)
// 			crx= cross;
// 		else
		double ccc= (double)(cross- cross2)/(slope2- slope);
		crx= ccc;
		cry= ccc*slope+ cross;

		return TRUE;
	}
	void trans(int x, int y, int xx, int yy, int dist, double &rx, double &ry, double &rxx, double &ryy)
	{
		int xxx, yyy;
		xxx= xx- x;
		yyy= yy- y;
		double slope;

		if(xxx == 0)
		{
			if(yyy > 0)
				dist= -dist;
			rx= x+ dist;
			ry= y;
			rxx= xx+ dist;
			ryy= yy;
			return;
		}else if(yyy == 0)
		{
			if(xxx < 0)
				dist= -dist;
			rx= x;
			ry= y+ dist;
			rxx= xx;
			ryy= yy+ dist;
			return;
		}

		slope= (double)yyy/xxx;

		double dim= ((double)dist*dist/(1.+ (double)1./(slope*slope)));
		dim= sqrt(dim);
		double dim2= ((double)dist*dist/(slope*slope+ 1));//dist/slope;
		dim2= sqrt(dim2);

		if(xxx > 0)
		{
			if(yyy > 0)
			{
				dim= -dim;
			}
		}else
		{
			if(yyy > 0)
			{
				dim= -dim;
				dim2= -dim2;
			}else
			{
				dim2= -dim2;
			}
		}

		rx= x+ dim;
		rxx= xx+ dim;
		ry= y+ dim2;
		ryy= yy+ dim2;
	}
};
