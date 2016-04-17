#pragma once


class AFX_EXT_CLASS CPadPoint
{
public:
	int x, y;

	void SetPoint(int px, int py)	{ x= px; y= py;	}
	void VectorTo(CPadPoint &point)	{	x= point.x- x;	y= point.y- y;	}

	BOOL FindCrossX(CPadPoint &point, int cy, int &tx);// 임의의 cy에 해당하는 x 값 구하기
	BOOL FindCrossY(CPadPoint &point, int cx, int &ty);// 임의의 cx에 해당하는 y 값 구하기

	BOOL ChangeCrossHori(CPadPoint &point, int cy);// 임의의 cy에 해당하는 x, y 로 point 값 바꾸기
	BOOL ChangeCrossVert(CPadPoint &point, int cx);// 임의의 cx에 해당하는 x, y 로 point 값 바꾸기
};

class AFX_EXT_CLASS CTrapezium
{
public:
	CPadPoint	m_TrapPoint[4];

public:
	CTrapezium*	GetSelfTrapezium()	{return this;}
	int		GetTop()	{return m_TrapPoint->y;}
	int		GetBottom()	{return m_TrapPoint[3].y;}
	void	OffsetPlus(int x, int y)
	{
		for(int i= 0; i< 4; i++)
		{
			m_TrapPoint[i].x+= x;
			m_TrapPoint[i].y+= y;
		}
	}
	void	ChangeAxis()
	{
		int temp;
		for(int i= 0; i< 4; i++)
		{
			temp= m_TrapPoint[i].x;
			m_TrapPoint[i].x= m_TrapPoint[i].y;
			m_TrapPoint[i].y= temp;
		}
	}
	void OpenTrap(CPadPoint &TopLeft, CPadPoint &TopRight)
	{
		m_TrapPoint[0]= TopLeft;
		m_TrapPoint[1]= TopRight;
	}
	void CloseTrap(CPadPoint &BotLeft, CPadPoint &BotRight)
	{
		m_TrapPoint[2]= BotLeft;
		m_TrapPoint[3]= BotRight;
	}
};

class AFX_EXT_CLASS CTrapeziumStorage
{
protected:
	int m_nTrapSpace;
	int m_nTrap;
	CTrapezium	m_Trapeziums[10];
	
public:
	CTrapeziumStorage(){m_nTrapSpace= 10; ResetTrapeziumCount();}

public:
	void ResetTrapeziumCount(){m_nTrap= 0;}
	CTrapezium*	GetTrapezium(int i){	return m_Trapeziums+ i;}
	int	GetTrapeziumCount(){return m_nTrap;}

	BOOL OpenTrap(CPadPoint &TopLeft, CPadPoint &TopRight)
	{
		if(m_nTrap >= m_nTrapSpace)
			return FALSE;
		//m_Trapeziums[m_nTrap].m_TrapPoint[0]= TopLeft;
		//m_Trapeziums[m_nTrap].m_TrapPoint[1]= TopRight;
		m_Trapeziums[m_nTrap].OpenTrap(TopLeft, TopRight);
		return TRUE;
	}
	BOOL CloseTrap(CPadPoint &BotLeft, CPadPoint &BotRight)
	{
		if(m_nTrap >= m_nTrapSpace)
			return FALSE;
		//m_Trapeziums[m_nTrap].m_TrapPoint[2]= BotLeft;
		//m_Trapeziums[m_nTrap].m_TrapPoint[3]= BotRight;
		m_Trapeziums[m_nTrap].CloseTrap(BotLeft, BotRight);
		m_nTrap++;
		return TRUE;
	}
};

class AFX_EXT_CLASS CPadPoligon
{
	int			m_nPadPoint;	// 유효 포인트 수
	BOOL		m_bHorizontal;	// 검사 방향
public:
	int			m_iTopLeft, m_iTopRight;	// m_iTopLeft = 0 이어야 한다.
	int			m_iBottomRight, m_iBottomLeft;		// 

//	int			m_iLeftBottom, m_iLeftTop;	// vertical 이면 Top이 Left로 바뀌는 것이지롱..
//	int			m_iRightTop, m_iRightBottom;

	CPadPoint	m_PadPoints[10];
	CRect		m_OutRect;
	float		m_PitchMin;
	float		m_PitchMax;

public:
	int		GetPointCount(){return m_nPadPoint;}
	CPadPoint* GetPadPoint(int i){ return m_PadPoints+ i;}
	CPadPoint* GetPadPoint_Round(int i){ return m_PadPoints+ GetRoundIndex(i);}

	int		GetRoundIndex(int id){return id%m_nPadPoint;}
	int		GetNextIndex(int id){ return (id+ 1)%m_nPadPoint;}
	int		GetPreIndex(int id){ return (id+ m_nPadPoint- 1)%m_nPadPoint;}
	void	CalOutRect();

public:
	CPadPoligon()	{		ResetPoligon();	}
	CPadPoligon* GetSelfPoligon(){return this;}
	/////------- test debug
	void Test();
	void OffsetPlusIndex(int start);
	void OffsetPlus(int dx, int dy);
	void ReverseIndex(int start);
	/////======= test debug
	BOOL IsClockwise();

	void ResetPoligon(){m_nPadPoint= 0;}
	void AddPoint(int x, int y)
	{
		m_PadPoints[m_nPadPoint].x= x;
		m_PadPoints[m_nPadPoint].y= y;
		m_nPadPoint++;
	}
	void AddPoint(CPadPoint& point)
	{
		m_PadPoints[m_nPadPoint]= point;
		m_nPadPoint++;
	}

	void Cut_Margin(int left, int right, CPadPoligon &PadPoligon)
	{	Cut_LeftRight(left, right, PadPoligon);}
	void Cut_LeftRight(int left, int right, CPadPoligon &PadPoligon);
	void Cut_TopBottom(int top, int bottom, CPadPoligon &PadPoligon);
	void Cut_3Pitch(float fPitch);
	void Find_Corner();

	BOOL Cut_TrapeziumHori(CTrapeziumStorage	*pTrapeziums);
	BOOL Cut_TrapeziumVert(CTrapeziumStorage	*pTrapeziums);
};

class AFX_EXT_CLASS CPoligonStorage
{
	int			m_nPadPoligon;
	CPadPoligon	m_PadPoligons[10];
public:
	CPadPoligon* GetPadPoligon(int i){ return m_PadPoligons+ i;}
};
