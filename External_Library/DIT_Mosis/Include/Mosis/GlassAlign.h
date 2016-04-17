// GlassAlign.h: interface for the CGlassAlign class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLASSALIGN_H__A4EDE408_69C7_440F_9842_CCDFAA98048A__INCLUDED_)
#define AFX_GLASSALIGN_H__A4EDE408_69C7_440F_9842_CCDFAA98048A__INCLUDED_

#include "Math.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



// ������ ��ġ �� x���� ���⿡ ���� ����.
// �� �ð���� ����, ���������� �ռ����� ���ڸ� ���� ���ڷ� ����.
enum OriginPos{OP_LEFT_TOP, OP_LEFT_BOTTOM, OP_RIGHT_BOTTOM, OP_RIGHT_TOP, OP_COUNT};
enum XDirection{XD_HORIZONTAL= 0, XD_VERTICAL= 1, XD_COUNT};



template <typename TTT>
class CCoordSystem
{
public:
	OriginPos	m_OriginPos;
	XDirection	m_XDirection;
	TTT			m_Width;
	TTT			m_Height;

public:
	CCoordSystem()
	{
		m_OriginPos= OP_LEFT_TOP;
		m_XDirection= XD_HORIZONTAL;
		m_Width= 0;
		m_Height= 0;
	}
	CCoordSystem(OriginPos pos, XDirection xDir, TTT width, TTT height)
	{
		SetSystem(pos, xDir, width, height);
	}
	void SetSystem(OriginPos pos, XDirection xDir, TTT width, TTT height)
	{
		m_OriginPos		= pos;
		m_XDirection	= xDir;
		m_Width			= width;
		m_Height		= height;
	}

	BOOL operator ==(CCoordSystem<TTT> &comp)
	{
		return m_OriginPos == comp.m_OriginPos && m_XDirection == comp.m_XDirection;
	}
};

// x�� y�� ��ȣ ��ȯ,, x�� y�� ġȯ�� x���� ���� ���� ����.
static int transx[4][4][2]= {	 {{1, 1},	{1, -1},	{-1, -1},	{-1, 1}} // 1 : (1~4) = Src(1) : Target(1~4)
,{{1, -1},	{1, 1},		{-1, 1},	{-1, -1}} // 2 : (1~4)
,{{-1, -1},	{-1, 1},	{1, 1},		{1, -1}} // 3 : (1~4)
,{{-1, 1},	{-1, -1},	{1, -1},	{1, 1}} // 4 : (1~4)
};


// [pox, xdir]�� ������ ��ǥ�� ��ȯ�� ����� �ݿ��Ѵ�.
// [LEFT_BOTTOM, XD_HORIZONTAL]=1, ���� [RIGHT_BOTTOM, XD_VERTICAL]= 1 �̸� 1*1 = 1 �̵ǹǷ� THETA�� ���� ���� THETA���� �ȴ�.
static int thetasign[4][2]= {{-1, 1}, {1, -1}, {-1, 1}, {1, -1} };


#define TRANS_PT(trans, pt, targetPt) trans.TransToTarget(pt.x, pt.y, targetPt.x, targetPt.y);
#define ROTATE_PT(trans, pt, targetPt) trans.RotateToTarget(pt.x, pt.y, targetPt.x, targetPt.y);

template <typename TTT>
class CCoordTrans
{
	CCoordSystem<TTT>	m_SrcSystem;
	CCoordSystem<TTT>	m_TargetSystem;


	int a, b, width, height;// ���� ��ȯ�� flag

public:
	BOOL IsNeedeToTrans()
	{
		if(m_SrcSystem == m_TargetSystem)
			return FALSE;// ��ǥ�谡 ������ ��ȯ �ʿ� ����.
		return TRUE;
	}

	BOOL SetTransSystem(OriginPos srcPos, XDirection srcXDir, TTT width, TTT height, OriginPos targetPos, XDirection targetXDir)
	{
		m_SrcSystem.SetSystem(srcPos, srcXDir, width, height);
		m_TargetSystem.SetSystem(targetPos, targetXDir, width, height);
		SetTransSystem(m_SrcSystem, m_TargetSystem);

		return IsNeedeToTrans();
	}

	BOOL SetTransSystem(CCoordSystem<TTT> &src, CCoordSystem<TTT> &target)
	{
		m_SrcSystem= src;
		m_TargetSystem= target;


		// x= a*x+ width;
		// y= b*y+ height;


		if(m_SrcSystem.m_XDirection == XD_HORIZONTAL)// HORIZONTAL�� �������� ���̺� �������.
		{
			a= transx[m_SrcSystem.m_OriginPos][m_TargetSystem.m_OriginPos][0];
			b= transx[m_SrcSystem.m_OriginPos][m_TargetSystem.m_OriginPos][1];
		}else
		{
			b= transx[m_SrcSystem.m_OriginPos][m_TargetSystem.m_OriginPos][0];
			a= transx[m_SrcSystem.m_OriginPos][m_TargetSystem.m_OriginPos][1];
		}


		if(a < 0)
			width= m_SrcSystem.m_Width;
		else
			width= 0;

		if(b < 0)
			height= m_SrcSystem.m_Height;
		else
			height= 0;

		if(m_SrcSystem.m_XDirection == m_TargetSystem.m_XDirection)
		{
			target.m_Width= m_TargetSystem.m_Width= m_SrcSystem.m_Width;
			target.m_Height= m_TargetSystem.m_Height= m_SrcSystem.m_Height;
		}else
		{
			target.m_Width= m_TargetSystem.m_Width= m_SrcSystem.m_Height;
			target.m_Height= m_TargetSystem.m_Height= m_SrcSystem.m_Width;
		}

		return IsNeedeToTrans();
	}
	void TransToTarget(TTT x, TTT y, TTT &tx, TTT &ty)
	{
		// �� ���� ��ȯ.(����ȭ, x2= width- x, y2= height- y)
		x= x*a+ width;
		y= y*b+ height;

		// �ణ ��ȯ. (����ȭ, x <-> y)
		if(m_SrcSystem.m_XDirection == m_TargetSystem.m_XDirection)
		{
			tx= x;
			ty= y;
		}else
		{
			tx= y;
			ty= x;
		}
	}
	void RotateToTarget(TTT x, TTT y, TTT &tx, TTT &ty)
	{
		// �� ���� ��ȯ.(����ȭ, x2= width- x, y2= height- y)
		x= x*a;
		y= y*b;

		// �ణ ��ȯ. (����ȭ, x <-> y)
		if(m_SrcSystem.m_XDirection == m_TargetSystem.m_XDirection)
		{
			tx= x;
			ty= y;
		}else
		{
			tx= y;
			ty= x;
		}
	}
	// 1 or -1, theta���� ��ȣ�� �����Ѵ�.
	int	GetThetaSign()
	{
		int srcDir= thetasign[m_SrcSystem.m_OriginPos][m_SrcSystem.m_XDirection];
		int tarDir= thetasign[m_TargetSystem.m_OriginPos][m_TargetSystem.m_XDirection];
		return srcDir*tarDir;
	}
};


// ����� ��ũ�� �̿��� ��ǥ ����.
// �ΰ��� ����� ��ũ�� �Ѱ��� ����� ��ũ�� ���������� theta ���.
// ���� : ����θ�ũ(����� ������)�� x, y, dx, dy(shift��ġ), theta(Ʋ���� ��)
// ����� ��ũ���ؿ� ���� ������, ���������� ���� ���� �ΰ��� ��� ��� ����.
// 1. SetAlignData
// 2. If Istilted(), then Correct()..

struct stAlignPoint
{
	int	x, y;
	stAlignPoint()			{ResetPoint();}
	void	ResetPoint()	{x= -1; y= -1;}// ������ ���� �� ����.
	BOOL	IsPointSet()	{return x != -1 && y!= -1;}
	void	SetPoint(int xx, int yy)	{x= xx; y= yy;}

	stAlignPoint operator +(stAlignPoint& pt)
	{
		stAlignPoint npt;
		npt.x= x+ pt.x;
		npt.y= y+ pt.y;
		return npt;
	}
	stAlignPoint operator -(stAlignPoint& pt)
	{
		stAlignPoint npt;
		npt.x= x- pt.x;
		npt.y= y- pt.y;
		return npt;
	}
	void	Scale(double rx, double ry)
	{
		x= (int)(x*rx);
		y= (int)(y*ry);
	}
	stAlignPoint operator-()
	{
		stAlignPoint pt;
		pt.x= -x;
		pt.y= -y;
		return pt;
	}
};

struct stAlignInfo
{
	BOOL			m_bAlignFound;

	int				m_xAlign, m_yAlign;		// Align Position(x, y)
	int				m_dxAlign, m_dyAlign;	// Align Shift.
	double			m_Theta;				// Angle of Glass Tilt == Align Tilt.
	double			m_BasicTheta;
	double			m_ExtraTheta;

public:
	void	ResetAlignInfo()	{m_bAlignFound= FALSE;	m_Theta= 0;	m_dxAlign= 0; m_dyAlign= 0; m_xAlign= m_yAlign= 0;}

	void	SetAlignFound()		{m_bAlignFound= TRUE;}
	BOOL	IsAlignFound()		{return m_bAlignFound;}

	BOOL	IsTilted()			{return	m_Theta != 0 || m_dxAlign != 0 || m_dyAlign != 0;}

public:
	stAlignInfo&	SelfAlignInfo()		{return *this;}
};




class AFX_EXT_CLASS CAlignFind
{
public:

	// 1. �۶� �󿡼��� ����θ�ũ ��ġ�� ����.
	CCoordSystem<int>	m_csOperator;		// ���� ��ǥ�� (OP_LEFT_TOP, XD_HORIZONTAL : �۾��� �ð�, �Һ�), SetAlignInGlass ���� �����ȴ�.

	stAlignPoint		m_FirstInGlass;			// um ����
	stAlignPoint		m_SecondInGlass;		// um ����
	stAlignPoint		m_FirstFindInGlass;
	stAlignPoint		m_SecondFindInGlass;

	double				m_BasicTheta;			// ���� ����� ��ũ ��ġ�� ���� ���� tilt
	double				m_ExtraTheta;
	double				m_TotalTheta;
	stAlignPoint		m_AlignShift;
	stAlignPoint		m_AlignDelta;


	// 2. debug�� ���ؼ� ���常 �صд�. ī�޶󿡼��� ��ũ ����. �ܺο��� �����Ǵ� �Է°����̴�. 
protected:
	stAlignPoint		m_FirstAlign;			// ���� ����� ��ũ ��ġ. um ����
	stAlignPoint		m_FirstFind;			// ���� ����� ��ũ ã�� ��. um ����
	CCoordSystem<int>	m_csFirst;

	stAlignPoint		m_SecondAlign;			// ���� ����� ��ũ ��ġ. um ����
	stAlignPoint		m_SecondFind;			// ���� ����� ��ũ ã�� ��. um ����
	CCoordSystem<int>	m_csSecond;


	// 3. debug�� ���ؼ� ���常 �صд�. �۶� �� ����� ��ũ�� ��ġ�� ��ǥ.
protected:
	int					m_Dx, m_Dy;					// �۶� ������ ����� ��ũ ������ �Ÿ�..
	OriginPos			m_FirstPos;					// ����� ��ũ ��ġ.
	OriginPos			m_SecondPos;				// ����� ��ũ ��ġ.
public:
	int					m_GlassWidth, m_GlassHeight;// �۶� ũ��.


public:
	CAlignFind();
	CCoordSystem<int>	GetOperatorCoordSystem(){return m_csOperator;}
	void ResetAlignFind()	{	m_FirstFindInGlass.ResetPoint(); m_SecondFindInGlass.ResetPoint();}
	BOOL IsAlignFound()	{	return m_FirstFindInGlass.IsPointSet() && m_SecondFindInGlass.IsPointSet();}


	// 1. �Ÿ� ������ �۶� ����θ�ũ ��ó ���ϱ�, �۶� ����(left, top)������ ����θ�ũ �Ÿ� dx, dy�� glass Width, height�� ������ �� ����θ�ũ ��ġ ��ǥ ���ϱ�.
	void FillPos(int &x, int &y, OriginPos pos, int dx, int dy, int glassWidth, int glassHeight);
public:
	// 2. �۶��� ����θ�ũ ���� ����. ���� um, �۾��� ���⿡���� left, top�� �������� xDirection�� Horizontal�� �������� �ϴ� �����ʹ�.
	void SetAlignInGlass(int dx, int dy, int glassWidth, int glassHeight, OriginPos firstPos, OriginPos secondPos, double basicTheta);

	// 3. ����θ�ũ ���� ����.. ���� um, ī�޶󿡼��� ��ġ�� ��ǥ������.
	void SetFirstAlign(stAlignPoint first, stAlignPoint find, double rx, double ry, CCoordSystem<int> cameraCoord);
	void SetSecondAlign(stAlignPoint second, stAlignPoint find, double rx, double ry, CCoordSystem<int> cameraCoord);


	// 4. ����� ������ ���ϱ�.
	BOOL CalcAlignInfo(CCoordSystem<int> &targetCoord, stAlignInfo &alignInfo);// operator��ǥ��� ������ ����θ�ũ�� theta����� targetCoord��ǥ��� ���� ȯ���� �ش�.
	double CalcAlignTheta_Operator(stAlignPoint delta, OriginPos opSrc, XDirection xdSrc);// operator��ǥ�迡���� theta�� ����.
	double CalcAlignTheta_Operator(stAlignPoint a1, stAlignPoint a2, OriginPos opSrc, XDirection xdSrc)
	{
		return CalcAlignTheta_Operator(a2- a1, opSrc, xdSrc);
	}
};


// ����� ��ũ�� shift�� theta �� �̿��� ��ǥ ����ϱ�.
// ����θ�ũ�� shift�� �̿��� ������ shift���� ����Ѵ�. theta�� ����. SetAlignData
// ���������� tilt(shift, theta)�� �̿��� ���� ��ǥ�� tilt�� ��ǥ�� ����س���.
// ��ǥ�� um�� �⺻ ������ ����Ѵ�.
class AFX_EXT_CLASS CBaseAligning : public stAlignInfo
{
protected:
	// AlignData���� 2�� �����Ǵ� ����.
	double			m_dCosine;				// Cosine(Theta)
	double			m_dSine;				// Sine(Theta)
	double			m_dTangent;

	double			m_dCosineBack;				// Cosine(Theta)
	double			m_dSineBack;				// Sine(Theta)

	int				m_dxOrigin, m_dyOrigin;	// Shift of Glass Original Position

public:
	CBaseAligning();


	void	SetAlignInfo(stAlignInfo &alignData);
	void	SetAlignInfo(double theta, int xAlign, int yAlign, int dxAlign, int dyAlign);

	void	Correct_00(int x, int y, int &xPrime, int &yPrime);		// ��ǥ ������ ����θ�ũ ������ �ƴ϶� ������������ ��ȯ - ���갳���� �ش�.
	void	Correct_00(int &x, int &y)	{Correct_00(x, y, x, y);}

	void	Correct(int x, int y, int &xPrime, int &yPrime, int bForwardScan, int nGlassHeight);		// x, y�� ����θ�ũ �������� ��ǥ��ȯ�� ƿƮ ���� �� �۶� ��ǥ��� ����.
	void	Correct(int &x, int &y, int bForwardScan, int nGlassHeigh)		{Correct(x, y, x, y, bForwardScan, nGlassHeigh);}
};




// When use it(Correct_Scan()), first check if belows are called 
// 1. Reset(), 2. SetAlignData(), 3. SetScanData() 4. IsTilted()
class AFX_EXT_CLASS CScanAligning : public CBaseAligning
{
public:
	double		m_xResolution;			// Resolution of X Axis ( Inspector cam)
	double		m_yResolution;			// Resolution of Y Axis ( Inspector cam)
	double		m_xAccum;				// UM of Cam's Left First Pixel (Left Margin)
	double		m_xAccumOrigin;			// UM of Cam's Left Pixel (0)
	int			m_LeftMargin;			// Left Margin of Each Cam.
	BOOL		m_bForwardScan;

	int			m_GlassHeight;			// Length of Glass Height
	int			m_GlassStart;			// Pixel of Glass Start Line.

public:
	void	SetScanData(double xResolution, double yResolution, double xAccum, int leftMargin, int glassHeight, int glassStartLine, BOOL bForward);
	void	Correct_Pixel2UM(int x, int y, int &xPrime, int &yPrime);
	void	Correct_Pixel2Pixel(int x, int y, int &xPrime, int &yPrime);
	void	CorrectBack_Pixel2UM(int x, int y, int &xPrime, int &yPrime);
	void	CorrectBack_Pixel2Pixel(int x, int y, int &xPrime, int &yPrime);

	int		GetDx_Pixel(int x, int y);
	int		GetDy_Pixel(int x, int y);

};

// CCoordTrans ���. ��ǥ�� ��ȯ�� ���� ��ǥ ��ȯ ��.
void Example_CoordTrans();

// ��ĵ���� �۶󽺷� ��ǥ ��ȯ���� �ùٸ��� �׽�Ʈ�غ���.
void Test_MakeScanAlignData(CScanAligning &alining);

#endif // !defined(AFX_GLASSALIGN_H__A4EDE408_69C7_440F_9842_CCDFAA98048A__INCLUDED_)
