// GlassAlign.h: interface for the CGlassAlign class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLASSALIGN_H__A4EDE408_69C7_440F_9842_CCDFAA98048A__INCLUDED_)
#define AFX_GLASSALIGN_H__A4EDE408_69C7_440F_9842_CCDFAA98048A__INCLUDED_

#include "Math.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



// 원점의 위치 와 x축의 방향에 대한 정의.
// 반 시계방향 설정, 순차적으로 앞순서의 뒷자를 시작 글자로 지정.
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

// x와 y의 부호 변환,, x와 y의 치환은 x축의 방향 보고 결정.
static int transx[4][4][2]= {	 {{1, 1},	{1, -1},	{-1, -1},	{-1, 1}} // 1 : (1~4) = Src(1) : Target(1~4)
,{{1, -1},	{1, 1},		{-1, 1},	{-1, -1}} // 2 : (1~4)
,{{-1, -1},	{-1, 1},	{1, 1},		{1, -1}} // 3 : (1~4)
,{{-1, 1},	{-1, -1},	{1, -1},	{1, 1}} // 4 : (1~4)
};


// [pox, xdir]의 곱으로 좌표계 변환의 결과를 반영한다.
// [LEFT_BOTTOM, XD_HORIZONTAL]=1, 에서 [RIGHT_BOTTOM, XD_VERTICAL]= 1 이면 1*1 = 1 이되므로 THETA의 합이 최종 THETA값이 된다.
static int thetasign[4][2]= {{-1, 1}, {1, -1}, {-1, 1}, {1, -1} };


#define TRANS_PT(trans, pt, targetPt) trans.TransToTarget(pt.x, pt.y, targetPt.x, targetPt.y);
#define ROTATE_PT(trans, pt, targetPt) trans.RotateToTarget(pt.x, pt.y, targetPt.x, targetPt.y);

template <typename TTT>
class CCoordTrans
{
	CCoordSystem<TTT>	m_SrcSystem;
	CCoordSystem<TTT>	m_TargetSystem;


	int a, b, width, height;// 방향 전환용 flag

public:
	BOOL IsNeedeToTrans()
	{
		if(m_SrcSystem == m_TargetSystem)
			return FALSE;// 좌표계가 같으면 변환 필요 없음.
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


		if(m_SrcSystem.m_XDirection == XD_HORIZONTAL)// HORIZONTAL을 기준으로 테이블 만들어짐.
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
		// 축 방향 전환.(무변화, x2= width- x, y2= height- y)
		x= x*a+ width;
		y= y*b+ height;

		// 축간 전환. (무변화, x <-> y)
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
		// 축 방향 전환.(무변화, x2= width- x, y2= height- y)
		x= x*a;
		y= y*b;

		// 축간 전환. (무변화, x <-> y)
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
	// 1 or -1, theta값의 부호를 결정한다.
	int	GetThetaSign()
	{
		int srcDir= thetasign[m_SrcSystem.m_OriginPos][m_SrcSystem.m_XDirection];
		int tarDir= thetasign[m_TargetSystem.m_OriginPos][m_TargetSystem.m_XDirection];
		return srcDir*tarDir;
	}
};


// 얼라인 마크를 이용한 좌표 보정.
// 두개의 얼라인 마크중 한개의 얼라인 마크를 기준점으로 theta 사용.
// 정보 : 얼라인마크(얼라인 기준점)의 x, y, dx, dy(shift위치), theta(틀어진 각)
// 얼라인 마크기준에 의한 보정과, 원점보정을 통한 보정 두가지 방법 사용 가능.
// 1. SetAlignData
// 2. If Istilted(), then Correct()..

struct stAlignPoint
{
	int	x, y;
	stAlignPoint()			{ResetPoint();}
	void	ResetPoint()	{x= -1; y= -1;}// 음수를 가질 수 없다.
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

	// 1. 글라스 상에서의 얼라인마크 위치와 정보.
	CCoordSystem<int>	m_csOperator;		// 기준 좌표계 (OP_LEFT_TOP, XD_HORIZONTAL : 작업자 시각, 불변), SetAlignInGlass 에서 설정된다.

	stAlignPoint		m_FirstInGlass;			// um 단위
	stAlignPoint		m_SecondInGlass;		// um 단위
	stAlignPoint		m_FirstFindInGlass;
	stAlignPoint		m_SecondFindInGlass;

	double				m_BasicTheta;			// 설정 얼라인 마크 위치가 가진 고유 tilt
	double				m_ExtraTheta;
	double				m_TotalTheta;
	stAlignPoint		m_AlignShift;
	stAlignPoint		m_AlignDelta;


	// 2. debug을 위해서 저장만 해둔다. 카메라에서의 마크 정보. 외부에서 설정되는 입력값들이다. 
protected:
	stAlignPoint		m_FirstAlign;			// 기준 얼라인 마크 위치. um 단위
	stAlignPoint		m_FirstFind;			// 기준 얼라인 마크 찾은 곳. um 단위
	CCoordSystem<int>	m_csFirst;

	stAlignPoint		m_SecondAlign;			// 보조 얼라인 마크 위치. um 단위
	stAlignPoint		m_SecondFind;			// 보조 얼라인 마크 찾은 곳. um 단위
	CCoordSystem<int>	m_csSecond;


	// 3. debug을 위해서 저장만 해둔다. 글라스 상 얼라인 마크의 위치와 좌표.
protected:
	int					m_Dx, m_Dy;					// 글라스 끝에서 얼라인 마크 까지의 거리..
	OriginPos			m_FirstPos;					// 얼라인 마크 위치.
	OriginPos			m_SecondPos;				// 얼라인 마크 위치.
public:
	int					m_GlassWidth, m_GlassHeight;// 글라스 크기.


public:
	CAlignFind();
	CCoordSystem<int>	GetOperatorCoordSystem(){return m_csOperator;}
	void ResetAlignFind()	{	m_FirstFindInGlass.ResetPoint(); m_SecondFindInGlass.ResetPoint();}
	BOOL IsAlignFound()	{	return m_FirstFindInGlass.IsPointSet() && m_SecondFindInGlass.IsPointSet();}


	// 1. 거리 정보로 글라스 얼라인마크 위처 구하기, 글라스 끝단(left, top)에서의 얼라인마크 거리 dx, dy와 glass Width, height를 가지고 각 얼라인마크 위치 좌표 구하기.
	void FillPos(int &x, int &y, OriginPos pos, int dx, int dy, int glassWidth, int glassHeight);
public:
	// 2. 글라스의 얼라인마크 정보 설정. 단위 um, 작업자 방향에서의 left, top을 원점으로 xDirection은 Horizontal을 기준으로 하는 데이터다.
	void SetAlignInGlass(int dx, int dy, int glassWidth, int glassHeight, OriginPos firstPos, OriginPos secondPos, double basicTheta);

	// 3. 얼라인마크 정보 설정.. 단위 um, 카메라에서의 위치와 좌표계정보.
	void SetFirstAlign(stAlignPoint first, stAlignPoint find, double rx, double ry, CCoordSystem<int> cameraCoord);
	void SetSecondAlign(stAlignPoint second, stAlignPoint find, double rx, double ry, CCoordSystem<int> cameraCoord);


	// 4. 얼라인 보정값 구하기.
	BOOL CalcAlignInfo(CCoordSystem<int> &targetCoord, stAlignInfo &alignInfo);// operator좌표계로 설정된 얼라인마크로 theta계산후 targetCoord좌표계로 값을 환산해 준다.
	double CalcAlignTheta_Operator(stAlignPoint delta, OriginPos opSrc, XDirection xdSrc);// operator좌표계에서의 theta값 리턴.
	double CalcAlignTheta_Operator(stAlignPoint a1, stAlignPoint a2, OriginPos opSrc, XDirection xdSrc)
	{
		return CalcAlignTheta_Operator(a2- a1, opSrc, xdSrc);
	}
};


// 얼라인 마크의 shift와 theta 를 이용해 좌표 계산하기.
// 얼라인마크의 shift를 이용해 원점의 shift량을 계산한다. theta는 동일. SetAlignData
// 원점에서의 tilt(shift, theta)를 이용해 임의 좌표의 tilt된 좌표를 계산해낸다.
// 좌표는 um을 기본 단위로 사용한다.
class AFX_EXT_CLASS CBaseAligning : public stAlignInfo
{
protected:
	// AlignData에서 2차 가공되는 정보.
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

	void	Correct_00(int x, int y, int &xPrime, int &yPrime);		// 좌표 기준을 얼라인마크 기준이 아니라 원점기준으로 변환 - 연산개수가 준다.
	void	Correct_00(int &x, int &y)	{Correct_00(x, y, x, y);}

	void	Correct(int x, int y, int &xPrime, int &yPrime, int bForwardScan, int nGlassHeight);		// x, y를 얼라인마크 기준으로 좌표변환후 틸트 보정 및 글라스 좌표계로 복원.
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

// CCoordTrans 용례. 좌표계 변환에 따른 좌표 변환 예.
void Example_CoordTrans();

// 스캔에서 글라스로 좌표 변환값이 올바른지 테스트해보자.
void Test_MakeScanAlignData(CScanAligning &alining);

#endif // !defined(AFX_GLASSALIGN_H__A4EDE408_69C7_440F_9842_CCDFAA98048A__INCLUDED_)
