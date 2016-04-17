// CFRecipeJudge.h: interface for the CCFRecipeJudge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CFRECIPEJUDGE_H__19D17C40_D748_433F_B0C7_66E6CE8DCB37__INCLUDED_)
#define AFX_CFRECIPEJUDGE_H__19D17C40_D748_433F_B0C7_66E6CE8DCB37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;

class CHMRegiConfig;
#define MAX_STEP_COUNT 20

enum CompOperator	{ COp_None = 0, COp_Greater, COp_GreaterEqual, COp_Lesser, COp_LesserEqual, COp_Equal, COp_NotEqual };
enum LogicOperator	{ LOp_And = 0, LOp_Or };
enum CompResult		{ Op_Fail = -1, Op_False = 0, Op_True = 1};
template <typename T> class CComparison
{
public:
	CComparison()
	{
		ResetComparison();
	}
	CComparison(T tL, CompOperator COpL, T tR, CompOperator COpR, LogicOperator LOp = LOp_And)
		: m_tLeft(tL), m_COpLeft(COpL), m_tRight(tR), m_COpRight(COpR), m_LOperator(LOp)	{}
	void			ResetComparison()
	{
		m_COpLeft	= COp_None;
		m_COpRight	= COp_None;
		m_LOperator	= LOp_And;
	}
	void			SetLeft(T t)					{ m_tLeft = t; }
	void			SetRight(T t)					{ m_tRight = t; }
	void			SetLeft(T t, CompOperator COp)	{ m_tLeft = t;	m_COpLeft = COp; }
	void			SetRight(T t, CompOperator COp)	{ m_tRight = t;	m_COpRight = COp; }
	void			SetLeftRight(T tL, T tR)		{ m_tLeft = tL;	m_tRight = tR; }
	void			SetLeftOp(CompOperator COp)		{ m_COpLeft = COp; }
	void			SetRightOp(CompOperator COp)	{ m_COpRight = COp; }
	void			SetLeftRightOp(CompOperator COpL, CompOperator COpR)	{ m_COpLeft = COpL;	m_COpRight = COpR; }
	void			SetLOperator(LogicOperator LOp)	{ m_LOperator = LOp; }
	void			SetAll(T tL, CompOperator COpL, T tR, CompOperator COpR, LogicOperator LOp = LOp_And)
	{
		m_tLeft			= tL;
		m_tRight		= tR;
		m_COpLeft		= COpL;
		m_COpRight		= COpR;
		m_LOperator		= LOp;
	}
	
	T				GetLeft() const			{ return m_tLeft; }
	T				GetRight() const		{ return m_tRight; }
	CompOperator	GetLeftOp() const		{ return m_COpLeft; }
	CompOperator	GetRightOp() const		{ return m_COpRight; }
	LogicOperator	GetLOperator() const	{ return m_LOperator; }

	BOOL			CompLeft(T t) const
	{
		if (m_COpLeft == Op_None)
			return Op_Fail;

		CompResult CR = Op_False;
		switch (m_COpLeft)
		{
		case COp_Greater:		if (t > m_tLeft)	CR = Op_True;	break;
		case COp_GreaterEqual:	if (t >= m_tLeft)	CR = Op_True;	break;
		case COp_Lesser:			if (t < m_tLeft)	CR = Op_True;	break;
		case COp_LesserEqual:	if (t <= m_tLeft)	CR = Op_True;	break;
		case COp_Equal:			if (t == m_tLeft)	CR = Op_True;	break;
		case COp_NotEqual:		if (t != m_tLeft)	CR = Op_True;	break;
		}
		return CR;
	}
	BOOL			CompRight(T t) const
	{
		if (m_COpRight == COp_None)
			return Op_Fail;
		
		CompResult CR = Op_False;
		switch (m_COpRight)
		{
		case COp_Greater:		if (t > m_tRight)	CR = Op_True;	break;
		case COp_GreaterEqual:	if (t >= m_tRight)	CR = Op_True;	break;
		case COp_Lesser:			if (t < m_tRight)	CR = Op_True;	break;
		case COp_LesserEqual:	if (t <= m_tRight)	CR = Op_True;	break;
		case COp_Equal:			if (t == m_tRight)	CR = Op_True;	break;
		case COp_NotEqual:		if (t != m_tRight)	CR = Op_True;	break;
		}
		return CR;
	}
	BOOL			CompBoth(T t) const
	{
		if (m_COpLeft == COp_None && m_COpRight == COp_None)
			return Op_Fail;
		
		CompResult CR = Op_False;
		CompResult CL = Op_False;
		if (m_COpLeft != COp_None)
		{
			switch (m_COpLeft)
			{
			case COp_Greater:		if (t > m_tLeft)	CL = Op_True;	break;
			case COp_GreaterEqual:	if (t >= m_tLeft)	CL = Op_True;	break;
			case COp_Lesser:			if (t < m_tLeft)	CL = Op_True;	break;
			case COp_LesserEqual:	if (t <= m_tLeft)	CL = Op_True;	break;
			case COp_Equal:			if (t == m_tLeft)	CL = Op_True;	break;
			case COp_NotEqual:		if (t != m_tLeft)	CL = Op_True;	break;
			}
			if(m_COpRight == COp_None)
				return CL;
		}
		if (m_COpRight != COp_None)
		{
			switch (m_COpRight)
			{
			case COp_Greater:		if (t > m_tRight)	CR = Op_True;	break;
			case COp_GreaterEqual:	if (t >= m_tRight)	CR = Op_True;	break;
			case COp_Lesser:		if (t < m_tRight)	CR = Op_True;	break;
			case COp_LesserEqual:	if (t <= m_tRight)	CR = Op_True;	break;
			case COp_Equal:			if (t == m_tRight)	CR = Op_True;	break;
			case COp_NotEqual:		if (t != m_tRight)	CR = Op_True;	break;
			}
			if(m_COpLeft == COp_None)
				return CR;
		}

		if(m_LOperator == LOp_And)
			return CL&CR;
		return CL|CL;
	}
	CComparison& operator=(const CComparison& rhs)
	{
		if (this != &rhs)
		{
			m_tLeft = rhs.m_tLeft;
			m_tRight = rhs.m_tRight;
			m_COpLeft = rhs.m_COpLeft;
			m_COpRight = rhs.m_COpRight;
			m_LOperator = rhs.m_LOperator;
		}
		
		return *this;
	}
	CComparison(const CComparison& rhs)
	{
		if (this != &rhs)
		{
			m_tLeft = rhs.m_tLeft;
			m_tRight = rhs.m_tRight;
			m_COpLeft = rhs.m_COpLeft;
			m_COpRight = rhs.m_COpRight;
			m_LOperator = rhs.m_LOperator;
		}
	}
protected:
	T				m_tLeft;
	T				m_tRight;
	CompOperator	m_COpLeft;
	CompOperator	m_COpRight;
	LogicOperator	m_LOperator;
};

template <typename T> class CComparisonEx : public CComparison<typename T>
{
public:
	CComparisonEx()
	{
		ResetComparison();
	}
	CComparisonEx(BOOL bUse, T tL, CompOperator COpL, T tR, CompOperator COpR, LogicOperator LOp = LOp_And)
		: m_bCompareUse(bUse)
	{
		m_tLeft = tL;
		m_COpLeft = COpL;
		m_tRight = tR;
		m_COpRight = COpR;
		m_LOperator = LOp;
	}
	CComparisonEx(BOOL bUse, const CComparison& rhs)
	{
		if (this != &rhs)
		{
			*this = rhs;
			m_bCompareUse = bUse;
		}
	}
	CComparisonEx(const CComparisonEx& rhs)
	{
		if (this != &rhs)
		{
			CComparison(rhs);
			m_bCompareUse	= rhs.m_bCompareUse;
		}
	}
	CComparisonEx& operator=(const CComparisonEx& rhs)
	{
		if (this != &rhs)
		{
			static_cast<CComparison<T>&>(*this) = static_cast<const CComparison<T>&>(rhs);
			m_bCompareUse	= rhs.m_bCompareUse;
		}
		
		return *this;
	}
	void			ResetComparison()
	{
		CComparison<T>::ResetComparison();
		m_bCompareUse = FALSE;
	}

	void			SetCompareUse(BOOL bUse)	{ m_bCompareUse = bUse; }
	BOOL			GetCompareUse() const		{ return m_bCompareUse; }

	BOOL			CompLeft(T t) const			{ return m_bCompareUse ? CComparison<T>::CompLeft(t) : TRUE; }
	BOOL			CompRight(T t) const		{ return m_bCompareUse ? CComparison<T>::CompRight(t) : TRUE; }
	BOOL			CompBoth(T t) const			{ return m_bCompareUse ? CComparison<T>::CompBoth(t) : TRUE; }
protected:
	BOOL			m_bCompareUse;
};

class AFX_EXT_CLASS CSelectionItem
{
public:
	CSelectionItem()
	{
		m_nItemCount = 0;
		m_pItem = NULL;
	}
	virtual ~CSelectionItem()
	{
		m_nItemCount = 0;
		if (m_pItem)
			delete [] m_pItem;
		m_pItem = NULL;
	}

	//김은호(STEP)
	void			ResetStep()
	{
		int i;
		for(i = 0 ; i < m_nItemCount ; i++)
		{
			m_pItem[i] = 0;

		}
		for(i =0; i < MAX_STEP_COUNT; i++)
			m_strItem[i] = _T("");
	}			

	//김은호(STEP) (사용 안함)
	void			DeleteItemValue(int nIndex)
	{
		m_pItem[nIndex] = 0;
		m_strItem[nIndex] = _T("");
		for(int i = nIndex ; i < GetTrueStringCount() ; i++)
		{
			m_strItem[nIndex] = m_strItem[i + 1];
		}
	}

	void			SetItemStringCount(int nCount)
	{
		if (nCount < 0)
			return;
		
		m_nItemCount = nCount;
		if (m_pItem)
			delete [] m_pItem;
		

		m_pItem = NULL;

		if (m_nItemCount > 0)
		{
			m_pItem = new BOOL[m_nItemCount];
		
			for (int i = 0; i < m_nItemCount; i++)
				m_pItem[i] = FALSE;

		}
		
	}

	void			SetItemCount(int nCount)
	{
		if (nCount < 0)
			return;

		m_nItemCount = nCount;
		if (m_pItem)
			delete [] m_pItem;
		m_pItem = NULL;
		if (m_nItemCount > 0)
		{
			m_pItem = new BOOL[m_nItemCount];
			for (int i = 0; i < m_nItemCount; i++)
				m_pItem[i] = FALSE;
		}
	}
	int				GetItemCount() const		{ return m_nItemCount; }
	void			SetItemValue(int nIdx, BOOL bValue)
	{
		if (!m_pItem || nIdx < 0 || nIdx >= m_nItemCount)
			return;
		m_pItem[nIdx] = bValue;
	}

	//김은호(STEP)
	void			SetItemValueString(int nIdx, BOOL nCheck, CString strValue)
	{
		if (!m_pItem || nIdx < 0 || nIdx >= m_nItemCount)
			return;
		m_pItem[nIdx] = nCheck;
		m_strItem[nIdx] = strValue;
	}

	BOOL			GetItemValue(int nIdx) const
	{
		if (!m_pItem || nIdx < 0 || nIdx >= m_nItemCount)
			return FALSE;
		return m_pItem[nIdx];
	}

	CString			GetItemValueString(int nIdx) const
	{
		return m_strItem[nIdx];
	}

	int				GetTrueCount() const
	{
		int nCount = 0;
		for (int i = 0; i < m_nItemCount; i++)
		{
			if (m_pItem[i])
				nCount++;
		}
		return nCount;
	}
	//김은호(STEP)
	int				GetTrueStringCount() const
	{
		int nCount = 0;
		for (int i = 0; i < MAX_STEP_COUNT; i++)
		{
			if (m_strItem[i].GetLength() > 0)
				nCount++;
		}
		return nCount;
	}

	CSelectionItem& operator=(const CSelectionItem& rhs)
	{
		if (this != &rhs)
		{
			m_nItemCount = rhs.m_nItemCount;
			if (m_pItem)
				delete [] m_pItem;
			m_pItem = NULL;
			if (m_nItemCount > 0)
			{
				m_pItem = new BOOL[m_nItemCount];
				for (int i = 0; i < m_nItemCount; i++)
					m_pItem[i] = rhs.m_pItem[i];
				for(int j = 0 ; j < MAX_STEP_COUNT; j++)
					m_strItem[j] = rhs.m_strItem[j];
			}
		}

		return *this;
	}
	CSelectionItem(const CSelectionItem& rhs)
	{
		if (this != &rhs)
		{
			m_pItem = NULL;
			m_nItemCount = rhs.m_nItemCount;
			if (m_nItemCount > 0)
			{
				m_pItem = new BOOL[m_nItemCount];
				for (int i = 0; i < m_nItemCount; i++)
					m_pItem[i] = rhs.m_pItem[i];
				for(int j = 0 ; j < MAX_STEP_COUNT; j++)
					m_strItem[j] = rhs.m_strItem[j];
			}
		}
	}
protected:
	int				m_nItemCount;
	BOOL*			m_pItem;
	CString			m_strItem[MAX_STEP_COUNT];
};

class AFX_EXT_CLASS CSelectionItemEx : public CSelectionItem
{
public:
	CSelectionItemEx()
	{
		m_bCompareUse = FALSE;
	}
	virtual ~CSelectionItemEx()
	{
	}
	CSelectionItemEx& operator=(const CSelectionItemEx& rhs)
	{
		if (this != &rhs)
		{
			static_cast<CSelectionItem&>(*this) = static_cast<const CSelectionItem&>(rhs);
			m_bCompareUse = rhs.m_bCompareUse;
		}
		
		return *this;
	}
	CSelectionItemEx(const CSelectionItemEx& rhs)
	{
		if (this != &rhs)
		{
			*this = rhs;
			m_bCompareUse = rhs.m_bCompareUse;
		}
	}
	
	void			SetCompareUse(BOOL bUse)		{ m_bCompareUse = bUse; }
	BOOL			GetCompareUse() const			{ return m_bCompareUse; }

protected:
	BOOL			m_bCompareUse;
};



class AFX_EXT_CLASS CCFDefectJudgeItem
{
public:
	CCFDefectJudgeItem(int nCamNo, int nScanNo);
	virtual ~CCFDefectJudgeItem();
	CCFDefectJudgeItem(const CCFDefectJudgeItem& rhs);
	CCFDefectJudgeItem& operator=(const CCFDefectJudgeItem& rhs);

	BOOL				ReadDefectJudgeItem(CString strType, int nIdx, CHMRegiConfig* pCfg);
	BOOL				WriteDefectJudgeItem(CString strType, int nIdx, CHMRegiConfig* pCfg) const;

	// Set
	void				SetUseCamType(BOOL bValue)					{ m_SelCamType.SetCompareUse(bValue); }
	void				SetUseStackFlag(BOOL bValue)				{ m_SelStackFlag.SetCompareUse(bValue); }
	void				SetUseProcType(BOOL bValue)					{ m_SelProcType.SetCompareUse(bValue); }
	void				SetUseBeforeJudge(BOOL bValue)				{ m_SelBeforeJudge.SetCompareUse(bValue); }
	void				SetUseZone(BOOL bValue)						{ m_SelZone.SetCompareUse(bValue); }
	void				SetUseCamNScan(BOOL bValue)					{ m_SelCamNScan.SetCompareUse(bValue); }
	void				SetUseStep(BOOL bValue)						{ m_SelStep.SetCompareUse(bValue); }

	void				SetSelCamType(int nIdx, BOOL bValue)		{ m_SelCamType.SetItemValue(nIdx, bValue); }
	void				SetSelStackFlag(int nIdx, BOOL bValue)		{ m_SelStackFlag.SetItemValue(nIdx, bValue); }
	void				SetSelProcType(int nIdx, BOOL bValue)		{ m_SelProcType.SetItemValue(nIdx, bValue); }
	void				SetSelBeforeJudge(int nIdx, BOOL bValue)	{ m_SelBeforeJudge.SetItemValue(nIdx, bValue); }
	void				SetSelZone(int nIdx, BOOL bValue)			{ m_SelZone.SetItemValue(nIdx, bValue); }
	void				SetSelCamNScan(int nIdx, BOOL bValue)		{ m_SelCamNScan.SetItemValue(nIdx, bValue); }
	void				SetSelStep(int nIdx, BOOL bValue)			{ m_SelStep.SetItemValue(nIdx, bValue); } //김은호(STEP)

	void				SetCompRScale(CComparisonEx<int>& Cmp)		{ m_CompRScale = Cmp; }
	void				SetCompSizeW(CComparisonEx<int>& Cmp)		{ m_CompSizeW = Cmp; }
	void				SetCompSizeL(CComparisonEx<int>& Cmp)		{ m_CompSizeL = Cmp; }
	void				SetCompPixelSize(CComparisonEx<int>& Cmp)	{ m_CompPixelSize = Cmp; }
	void				SetCompPeak(CComparisonEx<int>& Cmp)		{ m_CompPeak = Cmp; }
	void				SetCompSrcMin(CComparisonEx<int>& Cmp)		{ m_CompSrcMin = Cmp; }
	void				SetCompSrcMax(CComparisonEx<int>& Cmp)		{ m_CompSrcMax = Cmp; }
	void				SetCompSrcAvg(CComparisonEx<int>& Cmp)		{ m_CompSrcAvg = Cmp; }
	void				SetCompRefMin(CComparisonEx<int>& Cmp)		{ m_CompRefMin = Cmp; }
	void				SetCompRefMax(CComparisonEx<int>& Cmp)		{ m_CompRefMax = Cmp; }
	void				SetCompRefAvg(CComparisonEx<int>& Cmp)		{ m_CompRefAvg = Cmp; }

	void				SetCompSrcMinCM(CComparisonEx<int>& Cmp)		{ m_CompSrcMinCM = Cmp; }
	void				SetCompSrcMaxCM(CComparisonEx<int>& Cmp)		{ m_CompSrcMaxCM = Cmp; }
	void				SetCompSrcAvgCM(CComparisonEx<int>& Cmp)		{ m_CompSrcAvgCM = Cmp; }
	void				SetCompRefMinCM(CComparisonEx<int>& Cmp)		{ m_CompRefMinCM = Cmp; }
	void				SetCompRefMaxCM(CComparisonEx<int>& Cmp)		{ m_CompRefMaxCM = Cmp; }
	void				SetCompRefAvgCM(CComparisonEx<int>& Cmp)		{ m_CompRefAvgCM = Cmp; }

	void				SetCompRTDC(CComparisonEx<CString>& Cmp)	{ m_CompRTDC = Cmp; }

	void				SetnMaxNRCount(int nValue)					{ m_nMaxNRCount = nValue; }
	void				SetnSameDefectMargin(int nValue)			{ m_nSameDefectMargin = nValue; }
	void				SetZoneAndOr(BOOL bValue)					{ m_bSelZoneAndOr = bValue; }


	void				SetJudgement(Judgement Judge)				{ m_Judgement = Judge; }
	void				SetJudgeComment(CString str)				{ m_strJudgeComment = str; }

	// Compare
	BOOL				CompareRScale(int nValue) const			{ return m_CompRScale.CompBoth(nValue); }
	BOOL				CompareSizeW(int nValue) const			{ return m_CompSizeW.CompBoth(nValue); }
	BOOL				CompareSizeL(int nValue) const			{ return m_CompSizeL.CompBoth(nValue); }
	BOOL				ComparePixelSize(int nValue) const		{ return m_CompPixelSize.CompBoth(nValue); }
	BOOL				ComparePeak(int nValue) const			{ return m_CompPeak.CompBoth(nValue); }
	BOOL				CompareSrcMin(int nValue) const			{ return m_CompSrcMin.CompBoth(nValue); }
	BOOL				CompareSrcMax(int nValue) const			{ return m_CompSrcMax.CompBoth(nValue); }
	BOOL				CompareSrcAvg(int nValue) const			{ return m_CompSrcAvg.CompBoth(nValue); }
	BOOL				CompareRefMin(int nValue) const			{ return m_CompRefMin.CompBoth(nValue); }
	BOOL				CompareRefMax(int nValue) const			{ return m_CompRefMax.CompBoth(nValue); }
	BOOL				CompareRefAvg(int nValue) const			{ return m_CompRefAvg.CompBoth(nValue); }

	BOOL				CompareSrcMinCM(int nValue) const			{ return m_CompSrcMinCM.CompBoth(nValue); }
	BOOL				CompareSrcMaxCM(int nValue) const			{ return m_CompSrcMaxCM.CompBoth(nValue); }
	BOOL				CompareSrcAvgCM(int nValue) const			{ return m_CompSrcAvgCM.CompBoth(nValue); }
	BOOL				CompareRefMinCM(int nValue) const			{ return m_CompRefMinCM.CompBoth(nValue); }
	BOOL				CompareRefMaxCM(int nValue) const			{ return m_CompRefMaxCM.CompBoth(nValue); }
	BOOL				CompareRefAvgCM(int nValue) const			{ return m_CompRefAvgCM.CompBoth(nValue); }

	BOOL				CompareRTDC(CString str) const			{ return m_CompRTDC.CompBoth(str); }
	
	// Get
	const CSelectionItemEx&		GetSelCamType() const		{ return m_SelCamType; }
	const CSelectionItemEx&		GetSelStackFlag() const		{ return m_SelStackFlag; }
	const CSelectionItemEx&		GetSelProcType() const		{ return m_SelProcType; }
	const CSelectionItemEx&		GetSelBeforeJudge() const	{ return m_SelBeforeJudge; }
	const CSelectionItemEx&		GetSelZone() const			{ return m_SelZone; }
	const CSelectionItemEx&		GetSelCamNScan() const		{ return m_SelCamNScan; }
	const CSelectionItemEx&		GetSelStep() const		{ return m_SelStep; } //김은호(STEP) 
	
	BOOL						GetSelCamType(int nIdx) const		{ return m_SelCamType.GetItemValue(nIdx); }
	BOOL						GetSelStackFlag(int nIdx) const		{ return m_SelStackFlag.GetItemValue(nIdx); }
	BOOL						GetSelProcType(int nIdx) const		{ return m_SelProcType.GetItemValue(nIdx); }
	BOOL						GetSelBeforeJudge(int nIdx) const	{ return m_SelBeforeJudge.GetItemValue(nIdx); }
	BOOL						GetSelZone(int nIdx) const			{ return m_SelZone.GetItemValue(nIdx); }
	BOOL						GetSelCamNScan(int nIdx) const		{ return m_SelCamNScan.GetItemValue(nIdx); }
	BOOL						GetSelStep(int nIdx) const		{ return m_SelStep.GetItemValue(nIdx); } //김은호(STEP)
	
	const CComparisonEx<int>&	GetCompRScale() const			{ return m_CompRScale; }
	const CComparisonEx<int>&	GetCompSizeW() const			{ return m_CompSizeW; }
	const CComparisonEx<int>&	GetCompSizeL() const			{ return m_CompSizeL; }
	const CComparisonEx<int>&	GetCompPixelSize() const		{ return m_CompPixelSize; }
	const CComparisonEx<int>&	GetCompPeak() const				{ return m_CompPeak; }
	const CComparisonEx<int>&	GetCompSrcMin() const			{ return m_CompSrcMin; }
	const CComparisonEx<int>&	GetCompSrcMax() const			{ return m_CompSrcMax; }
	const CComparisonEx<int>&	GetCompSrcAvg() const			{ return m_CompSrcAvg; }
	const CComparisonEx<int>&	GetCompRefMin() const			{ return m_CompRefMin; }
	const CComparisonEx<int>&	GetCompRefMax() const			{ return m_CompRefMax; }
	const CComparisonEx<int>&	GetCompRefAvg() const			{ return m_CompRefAvg; }
	const CComparisonEx<int>&	GetCompSrcMinCM() const			{ return m_CompSrcMinCM; }
	const CComparisonEx<int>&	GetCompSrcMaxCM() const			{ return m_CompSrcMaxCM; }
	const CComparisonEx<int>&	GetCompSrcAvgCM() const			{ return m_CompSrcAvgCM; }
	const CComparisonEx<int>&	GetCompRefMinCM() const			{ return m_CompRefMinCM; }
	const CComparisonEx<int>&	GetCompRefMaxCM() const			{ return m_CompRefMaxCM; }
	const CComparisonEx<int>&	GetCompRefAvgCM() const			{ return m_CompRefAvgCM; }
	const CComparisonEx<CString>&	GetCompRTDC() const			{ return m_CompRTDC; }
	
	int						GetnMaxNRCount() const				{ return m_nMaxNRCount; }
	int						GetnSameDefectMargin() const		{ return m_nSameDefectMargin; }
	BOOL					GetZoneAndOr() const				{ return m_bSelZoneAndOr; }

	
	Judgement				GetJudgement() const				{ return m_Judgement; }
	CString					GetJudgeComment() const				{ return m_strJudgeComment; }
	int						GetUseItemCount() const;
protected:
	CCFDefectJudgeItem();	// CamNo, ScanNo가 없는 경우를 막기 위함.
	int						m_nCameraNum;
	int						m_nScanNum;
	
	BOOL					ReadComparisonInt(CComparisonEx<int>& Cmp, CString strType, CString strSubType, int nIdx, CHMRegiConfig* pCfg);
	BOOL					ReadComparisonString(CComparisonEx<CString>& Cmp, CString strType, CString strSubType, int nIdx, CHMRegiConfig* pCfg);
	BOOL					WriteComparisonInt(const CComparisonEx<int>& Cmp, CString strType, CString strSubType, int nIdx, CHMRegiConfig* pCfg) const;
	BOOL					WriteComparisonString(const CComparisonEx<CString>& Cmp, CString strType, CString strSubType, int nIdx, CHMRegiConfig* pCfg) const;
	BOOL					ReadSelectionItem(CSelectionItemEx& Sel, CString strType, CString strSubType, int nIdx, CHMRegiConfig* pCfg);
	BOOL					ReadSelectionItemStep(CSelectionItemEx& Sel, CString strType, CString strSubType, int nIdx, CHMRegiConfig* pCfg); //김은호(STEP)
	BOOL					WriteSelectionItem(const CSelectionItemEx& Cmp, CString strType, CString strSubType, int nIdx, CHMRegiConfig* pCfg) const;
	BOOL					WriteSelectionItemStep(const CSelectionItemEx& Cmp, CString strType, CString strSubType, int nIdx, CHMRegiConfig* pCfg) const; //김은호(STEP)

	//////////////////////////////////////////////////////////////////////////
	// 조건
	CSelectionItemEx		m_SelCamType;
	CSelectionItemEx		m_SelStackFlag;
	CSelectionItemEx		m_SelProcType;
	CSelectionItemEx		m_SelBeforeJudge; //윤정민 이전공정 판정 가져오기
	CSelectionItemEx		m_SelZone;
	CSelectionItemEx		m_SelCamNScan;
	CSelectionItemEx		m_SelStep; //김은호(STEP)

	CComparisonEx<int>		m_CompRScale;
	CComparisonEx<int>		m_CompSizeW;
	CComparisonEx<int>		m_CompSizeL;
	CComparisonEx<int>		m_CompPixelSize;
	CComparisonEx<int>		m_CompPeak;
	CComparisonEx<int>		m_CompSrcMin;
	CComparisonEx<int>		m_CompSrcMax;
	CComparisonEx<int>		m_CompSrcAvg;
	CComparisonEx<int>		m_CompRefMin;
	CComparisonEx<int>		m_CompRefMax;
	CComparisonEx<int>		m_CompRefAvg;

	CComparisonEx<int>		m_CompSrcMinCM;
	CComparisonEx<int>		m_CompSrcMaxCM;
	CComparisonEx<int>		m_CompSrcAvgCM;
	CComparisonEx<int>		m_CompRefMinCM;
	CComparisonEx<int>		m_CompRefMaxCM;
	CComparisonEx<int>		m_CompRefAvgCM;

	CComparisonEx<CString>	m_CompRTDC;

	int						m_nMaxNRCount;
	int						m_nSameDefectMargin;

	BOOL					m_bSelZoneAndOr;

	// 판정값
	Judgement				m_Judgement;	// 0 : OK, 2 : TR, 3 : PR
	CString					m_strJudgeComment;
};
typedef std::vector<CCFDefectJudgeItem>				VDefectJudgeItem;
typedef std::vector<CCFDefectJudgeItem>::iterator	VDefectJudgeItemIt;

class AFX_EXT_CLASS CCFPanelJudgeItem
{
public:
	CCFPanelJudgeItem()
	{
		m_nConditionType = 0;
		m_JudgeConditionF = Judge_OK;
		m_nConditionCountF = 0;
		m_JudgeConditionS = Judge_OK;
		m_nConditionCountS = 0;
		m_Judgement = Judge_OK;
		m_strJudgement = _T("");
	}
	CCFPanelJudgeItem(const CCFPanelJudgeItem& rhs)
	{
		if (this != &rhs)
		{
			m_nConditionType	= rhs.m_nConditionType;
			m_JudgeConditionF	= rhs.m_JudgeConditionF;
			m_nConditionCountF	= rhs.m_nConditionCountF;
			m_JudgeConditionS	= rhs.m_JudgeConditionS;
			m_nConditionCountS	= rhs.m_nConditionCountS;
			m_Judgement			= rhs.m_Judgement;
			m_strJudgement		= rhs.m_strJudgement;
		}
	}
	CCFPanelJudgeItem& operator=(const CCFPanelJudgeItem& rhs)
	{
		if (this != &rhs)
		{
			m_nConditionType	= rhs.m_nConditionType;
			m_JudgeConditionF	= rhs.m_JudgeConditionF;
			m_nConditionCountF	= rhs.m_nConditionCountF;
			m_JudgeConditionS	= rhs.m_JudgeConditionS;
			m_nConditionCountS	= rhs.m_nConditionCountS;
			m_Judgement			= rhs.m_Judgement;
			m_strJudgement		= rhs.m_strJudgement;
		}

		return *this;
	}

	BOOL				ReadPanelJudgeItem(CString strType, int nIdx, CHMRegiConfig* pCfg);
	BOOL				WritePanelJudgeItem(CString strType, int nIdx, CHMRegiConfig* pCfg) const;
	
	void				SetConditionType(int nType)		{ m_nConditionType = nType; }
	void				SetJudgeConditionF(Judgement DJ){ m_JudgeConditionF = DJ; }
	void				SetConditionCountF(int nCount)	{ m_nConditionCountF = nCount; }
	void				SetJudgeConditionS(Judgement DJ){ m_JudgeConditionS = DJ; }
	void				SetConditionCountS(int nCount)	{ m_nConditionCountS = nCount; }
	void				SetJudgement(Judgement Judge)	{ m_Judgement = Judge; }
	void				SetJudgement(CString str)		{ m_strJudgement = str; }

	int					GetConditionType() const		{ return m_nConditionType; }
	Judgement			GetJudgeConditionF() const		{ return m_JudgeConditionF; }
	int					GetConditionCountF() const		{ return m_nConditionCountF; }
	Judgement			GetJudgeConditionS() const		{ return m_JudgeConditionS; }
	int					GetConditionCountS() const		{ return m_nConditionCountS; }
	Judgement			GetJudgement() const			{ return m_Judgement; }
	CString				GetJudgementString() const		{ return m_strJudgement; }

protected:
	int					m_nConditionType;	// 0 : Cell Judge, 1 : Defect Judge
	Judgement			m_JudgeConditionF;
	int					m_nConditionCountF;
	Judgement			m_JudgeConditionS;
	int					m_nConditionCountS;
	Judgement			m_Judgement;		// Subpanel에 사용
	CString				m_strJudgement;		// Panel에 사용
};
typedef std::vector<CCFPanelJudgeItem>				VPanelJudgeItem;
typedef std::vector<CCFPanelJudgeItem>::iterator	VPanelJudgeItemIt;

//////////////////////////////////////////////////////////////////////////
// Recip별 판정 조건
class AFX_EXT_CLASS CCFRecipeJudge
{
public:
	CCFRecipeJudge(int nCamNo, int nScanNo);
	virtual ~CCFRecipeJudge();
	CCFRecipeJudge(const CCFRecipeJudge& rhs);
	CCFRecipeJudge& operator=(const CCFRecipeJudge& rhs);

	void						ResetRecipeJudge();
	
	BOOL						ReadRecipeJudge(int iModelID, CHMRegiConfig* pCfg);
	BOOL						WriteRecipeJudge(int iModelID, CHMRegiConfig* pCfg);

	void						SetCutoffCondition(const CCFDefectJudgeItem& DJudge)	{ m_VDefectCutoff.push_back(DJudge); }
	void						SetTDCondition(const CCFDefectJudgeItem& DJudge)		{ m_VDefectTD.push_back(DJudge); }
	void						SetPDCondition(const CCFDefectJudgeItem& DJudge)		{ m_VDefectPD.push_back(DJudge); }
	void						SetSICondition(const CCFDefectJudgeItem& DJudge)		{ m_VDefectSI.push_back(DJudge); }
	void						SetNRCondition(const CCFDefectJudgeItem& DJudge)		{ m_VDefectNR.push_back(DJudge); }
	void						SetSubpanelCondition(const CCFPanelJudgeItem& PJudge)	{ m_VSubPanel.push_back(PJudge); }
	void						SetPanelCondition(const CCFPanelJudgeItem& PJudge)		{ m_VPanel.push_back(PJudge); }
	
	void						RemoveCutoffCondition(int nIdx)			{ m_VDefectCutoff.erase(m_VDefectCutoff.begin() + nIdx); }
	void						RemoveTDCondition(int nIdx)				{ m_VDefectTD.erase(m_VDefectTD.begin() + nIdx); }
	void						RemovePDCondition(int nIdx)				{ m_VDefectPD.erase(m_VDefectPD.begin() + nIdx); }
	void						RemoveSICondition(int nIdx)				{ m_VDefectSI.erase(m_VDefectSI.begin() + nIdx); }
	void						RemoveNRCondition(int nIdx)				{ m_VDefectNR.erase(m_VDefectNR.begin() + nIdx); }
	void						RemoveSubpanelCondition(int nIdx)		{ m_VSubPanel.erase(m_VSubPanel.begin() + nIdx); }
	void						RemovePanelCondition(int nIdx)			{ m_VPanel.erase(m_VPanel.begin() + nIdx); }
	
	int							GetCutoffConditionCount() const			{ return static_cast<int>(m_VDefectCutoff.size()); }
	int							GetTDConditionCount() const				{ return static_cast<int>(m_VDefectTD.size()); }
	int							GetPDConditionCount() const				{ return static_cast<int>(m_VDefectPD.size()); }
	int							GetSIConditionCount() const				{ return static_cast<int>(m_VDefectSI.size()); }
	int							GetNRConditionCount() const				{ return static_cast<int>(m_VDefectNR.size()); }
	int							GetSubpanelConditionCount() const		{ return static_cast<int>(m_VSubPanel.size()); }
	int							GetPanelConditionCount() const			{ return static_cast<int>(m_VPanel.size()); }
	
	const CCFDefectJudgeItem&	GetCutoffCondition(int nIdx) const		{ return m_VDefectCutoff[nIdx]; }
	const CCFDefectJudgeItem&	GetTDCondition(int nIdx) const			{ return m_VDefectTD[nIdx]; }
	const CCFDefectJudgeItem&	GetPDCondition(int nIdx) const			{ return m_VDefectPD[nIdx]; }
	const CCFDefectJudgeItem&	GetSICondition(int nIdx) const			{ return m_VDefectSI[nIdx]; }
	const CCFDefectJudgeItem&	GetNRCondition(int nIdx) const			{ return m_VDefectNR[nIdx]; }
	const CCFPanelJudgeItem&	GetSubpanelCondition(int nIdx) const	{ return m_VSubPanel[nIdx]; }
	const CCFPanelJudgeItem&	GetPanelCondition(int nIdx) const		{ return m_VPanel[nIdx]; }
	
	BOOL						SwapCutoffCondition(int nFirst, int nSecond);
	BOOL						SwapTDCondition(int nFirst, int nSecond);
	BOOL						SwapPDCondition(int nFirst, int nSecond);
	BOOL						SwapSICondition(int nFirst, int nSecond);
	BOOL						SwapNRCondition(int nFirst, int nSecond);
// minion't,2009-11-08
public:
	int						GetCameraNumber(void) { return m_nCameraNum; }
	int						GetScanNumber(void) { return m_nScanNum; }

protected:
	CCFRecipeJudge();	// CamNo, ScanNo가 없는 경우를 막기 위함.
	int						m_nCameraNum;
	int						m_nScanNum;
	
	//////////////////////////////////////////////////////////////////////////
	// Cutoff 조건
	VDefectJudgeItem		m_VDefectCutoff;

	//////////////////////////////////////////////////////////////////////////
	// TD 판정 조건
	VDefectJudgeItem		m_VDefectTD;

	//////////////////////////////////////////////////////////////////////////
	// PD 판정 조건
	VDefectJudgeItem		m_VDefectPD;

	//////////////////////////////////////////////////////////////////////////
	// SaveImage 판정 조건
	VDefectJudgeItem		m_VDefectSI;

	//////////////////////////////////////////////////////////////////////////
	// Defect 판정 조건
	VDefectJudgeItem		m_VDefectNR;

	//////////////////////////////////////////////////////////////////////////
	// Cell 판정 조건
	VPanelJudgeItem			m_VSubPanel;

	//////////////////////////////////////////////////////////////////////////
	// Glass 판정 조건
	VPanelJudgeItem			m_VPanel;
};

#endif // !defined(AFX_CFRECIPEJUDGE_H__19D17C40_D748_433F_B0C7_66E6CE8DCB37__INCLUDED_)
