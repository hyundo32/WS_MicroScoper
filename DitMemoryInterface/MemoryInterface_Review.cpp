#include "StdAfx.h"
#include "MemoryInterface_Review.h"
#include "MemoryData_Review.h"

CMemoryInterface_Review::CMemoryInterface_Review(int nType, IMemoryInterface2Parent* pIMI2P, DWORD dwPeriod) : CMemoryInterface(nType, pIMI2P, dwPeriod)
{
	m_dwMemoryDataSize			= sizeof(CMemoryData_Review);
	m_dwGlassDataSize			= sizeof(CGlassData_Review);
	m_dwRecipeDataSize			= sizeof(CRecipeData_Review);
	m_dwAlignDataSize			= sizeof(CAlignData_Review);
	m_dwCellDataSize			= sizeof(CCellData_Review);
	m_dwDefectDataSize			= sizeof(CDefectData_Review);
	m_dwImageDataSize			= sizeof(CImageData_Review);

	m_dwCellSizePerOneGlass		= m_dwCellDataSize * MAX_REVIEW_CELL_COUNT;
	m_dwDefectSizePerOneGlass	= m_dwDefectDataSize * MAX_REVIEW_DEFECT_COUNT;
	m_dwImageSizeTotalMemory	= m_dwImageDataSize * MAX_REVIEW_IMAGE_COUNT;

	m_dwGlassDataIndex			= m_dwMemoryDataSize;
	m_dwRecipeDataIndex			= m_dwGlassDataIndex + (m_dwGlassDataSize*MAX_REVIEW_GLASS_COUNT);
	m_dwAlignDataIndex			= m_dwRecipeDataIndex + (m_dwRecipeDataSize*MAX_REVIEW_GLASS_COUNT);
	m_dwCellDataIndex			= m_dwAlignDataIndex + (m_dwAlignDataSize*MAX_REVIEW_GLASS_COUNT);
	m_dwDefectDataIndex			= m_dwCellDataIndex + (m_dwCellSizePerOneGlass*MAX_REVIEW_GLASS_COUNT);
	m_dwImageDataIndex			= m_dwDefectDataIndex + (m_dwDefectSizePerOneGlass*MAX_REVIEW_GLASS_COUNT);

	DWORD64 dwTotalMemorySize	= m_dwImageDataIndex + m_dwImageSizeTotalMemory;

	CreateMemory(dwTotalMemorySize, _T("MemoryInterface_Review"));
}

CMemoryInterface_Review::~CMemoryInterface_Review(void)
{

}

BOOL CMemoryInterface_Review::StartThread()
{
	return CreateTimerThread(this);
}

void CMemoryInterface_Review::TimerThreadProcess(PVOID pParameter)
{
	if (m_pIMI2P==NULL) return;

	const CMemoryData* pData = static_cast<const CMemoryData*>(GetMemoryAddress(0));
	if (pData==NULL) return;

	// check prev signal
	for (int nSignalType=0; nSignalType<MAX_REVIEW_SIGNAL_TYPE_COUNT; nSignalType++)
	{
		if(pData->m_dwSendSignal[nSignalType] != m_prevMemoryData.m_dwSendSignal[nSignalType]) 
		{
			int nBitCount = sizeof(CMemoryData) * 8;

			DWORD64 dwBitValue = 1;
			BOOL bCurValue = FALSE;
			BOOL bPrevValue = FALSE;
			for (int nSignalIdx=0; nSignalIdx<nBitCount; nSignalIdx++)
			{
				// compare current and prev
				bCurValue = BOOL(dwBitValue & pData->m_dwSendSignal[nSignalType]);
				bPrevValue = BOOL(dwBitValue & m_prevMemoryData.m_dwSendSignal[nSignalType]);

				// change check
				if ( bCurValue != bPrevValue )
				{
					m_pIMI2P->IMI2P_SendSignalValue(m_nMemoryType, nSignalType, nSignalIdx, bCurValue);
				}			

				dwBitValue = dwBitValue << 1;
			}
		}
	}

	// save current signal
	m_prevMemoryData = *pData;
}

LPVOID CMemoryInterface_Review::GetGlassData(int nGlassIdx)
{
	if (nGlassIdx<0 || nGlassIdx>=MAX_REVIEW_GLASS_COUNT) return NULL;
	DWORD dwIndex = m_dwGlassDataIndex + (m_dwGlassDataSize * nGlassIdx);
	return GetMemoryAddress(dwIndex);
}

const LPVOID CMemoryInterface_Review::GetGlassData(int nGlassIdx) const
{
	if (nGlassIdx<0 || nGlassIdx>=MAX_REVIEW_GLASS_COUNT) return NULL;
	DWORD dwIndex = m_dwGlassDataIndex + (m_dwGlassDataSize * nGlassIdx);
	return GetMemoryAddress(dwIndex);
}

LPVOID CMemoryInterface_Review::GetRecipeData(int nGlassIdx)
{
	if (nGlassIdx<0 || nGlassIdx>=MAX_REVIEW_GLASS_COUNT) return NULL;
	DWORD dwIndex = m_dwRecipeDataIndex + (m_dwRecipeDataSize * nGlassIdx);
	return GetMemoryAddress(dwIndex);
}

const LPVOID CMemoryInterface_Review::GetRecipeData(int nGlassIdx) const
{
	if (nGlassIdx<0 || nGlassIdx>=MAX_REVIEW_GLASS_COUNT) return NULL;
	DWORD dwIndex = m_dwRecipeDataIndex + (m_dwRecipeDataSize * nGlassIdx);
	return GetMemoryAddress(dwIndex);
}

LPVOID CMemoryInterface_Review::GetAlignData(int nGlassIdx)
{
	if (nGlassIdx<0 || nGlassIdx>=MAX_REVIEW_GLASS_COUNT) return NULL;
	DWORD dwIndex = m_dwAlignDataIndex + (m_dwAlignDataSize * nGlassIdx);
	return GetMemoryAddress(dwIndex);
}

const LPVOID CMemoryInterface_Review::GetAlignData(int nGlassIdx) const
{
	if (nGlassIdx<0 || nGlassIdx>=MAX_REVIEW_GLASS_COUNT) return NULL;
	DWORD dwIndex = m_dwAlignDataIndex + (m_dwAlignDataSize * nGlassIdx);
	return GetMemoryAddress(dwIndex);
}

LPVOID CMemoryInterface_Review::GetCellData(int nGlassIdx, int nCellIdx)
{
	if (nGlassIdx<0 || nGlassIdx>=MAX_REVIEW_GLASS_COUNT) return NULL;
	if (nCellIdx<0 || nCellIdx>=MAX_REVIEW_CELL_COUNT) return NULL;
	DWORD dwIndex = m_dwCellDataIndex + (m_dwCellSizePerOneGlass * nGlassIdx) + (m_dwCellDataSize * nCellIdx);
	return GetMemoryAddress(dwIndex);
}

const LPVOID CMemoryInterface_Review::GetCellData(int nGlassIdx, int nCellIdx) const
{
	if (nGlassIdx<0 || nGlassIdx>=MAX_REVIEW_GLASS_COUNT) return NULL;
	if (nCellIdx<0 || nCellIdx>=MAX_REVIEW_CELL_COUNT) return NULL;
	DWORD dwIndex = m_dwCellDataIndex + (m_dwCellSizePerOneGlass * nGlassIdx) + (m_dwCellDataSize * nCellIdx);
	return GetMemoryAddress(dwIndex);
}

LPVOID CMemoryInterface_Review::GetDefectData(int nGlassIdx, int nDefectIdx)
{
	if (nGlassIdx<0 || nGlassIdx>=MAX_REVIEW_GLASS_COUNT) return NULL;
	if (nDefectIdx<0 || nDefectIdx>=MAX_REVIEW_DEFECT_COUNT) return NULL;
	DWORD dwIndex = m_dwDefectDataIndex + (m_dwDefectSizePerOneGlass * nGlassIdx) + (m_dwDefectDataSize * nDefectIdx);
	return GetMemoryAddress(dwIndex);
}

const LPVOID CMemoryInterface_Review::GetDefectData(int nGlassIdx, int nDefectIdx) const
{
	if (nGlassIdx<0 || nGlassIdx>=MAX_REVIEW_GLASS_COUNT) return NULL;
	if (nDefectIdx<0 || nDefectIdx>=MAX_REVIEW_DEFECT_COUNT) return NULL;
	DWORD dwIndex = m_dwDefectDataIndex + (m_dwDefectSizePerOneGlass * nGlassIdx) + (m_dwDefectDataSize * nDefectIdx);
	return GetMemoryAddress(dwIndex);;
}

LPVOID CMemoryInterface_Review::GetImageData(int nImageIdx)
{
	if (nImageIdx<0 || nImageIdx>=MAX_REVIEW_IMAGE_COUNT) return NULL;
	DWORD dwIndex = m_dwImageDataIndex + (m_dwImageDataSize * nImageIdx);
	return GetMemoryAddress(dwIndex);
}

const LPVOID CMemoryInterface_Review::GetImageData(int nImageIdx) const
{
	if (nImageIdx<0 || nImageIdx>=MAX_REVIEW_IMAGE_COUNT) return NULL;
	DWORD dwIndex = m_dwImageDataIndex + (m_dwImageDataSize * nImageIdx);
	return GetMemoryAddress(dwIndex);
}

int CMemoryInterface_Review::GetCurrentGlassIndex() const
{
	const CMemoryData_Review* pData = static_cast<const CMemoryData_Review*>(GetMemoryData());
	if (pData==NULL) return -1;
	return pData->m_nGlassIndex;
}

int CMemoryInterface_Review::GetCurrentGlassCount() const
{
	const CMemoryData_Review* pData = static_cast<const CMemoryData_Review*>(GetMemoryData());
	if (pData==NULL) return -1;
	return pData->m_nGlassCount;
}

int CMemoryInterface_Review::GetCurrentImageIndex() const
{
	const CMemoryData_Review* pData = static_cast<const CMemoryData_Review*>(GetMemoryData());
	if (pData==NULL) return -1;
	return pData->m_nImageIndex;
}

int CMemoryInterface_Review::GetCurrentImageCount() const
{
	const CMemoryData_Review* pData = static_cast<const CMemoryData_Review*>(GetMemoryData());
	if (pData==NULL) return -1;
	return pData->m_nImageCount;
}

LPVOID CMemoryInterface_Review::GetCurrentGlassData()
{
	int nGlassIdx = GetCurrentGlassIndex();
	return GetGlassData(nGlassIdx);
}

const LPVOID CMemoryInterface_Review::GetCurrentGlassData() const
{
	int nGlassIdx = GetCurrentGlassIndex();
	return GetGlassData(nGlassIdx);
}

LPVOID CMemoryInterface_Review::GetCurrentRecipeData()
{
	int nGlassIdx = GetCurrentGlassIndex();
	return GetRecipeData(nGlassIdx);
}

const LPVOID CMemoryInterface_Review::GetCurrentRecipeData() const
{
	int nGlassIdx = GetCurrentGlassIndex();
	return GetRecipeData(nGlassIdx);
}

LPVOID CMemoryInterface_Review::GetCurrentAlignData()
{
	int nGlassIdx = GetCurrentGlassIndex();
	return GetAlignData(nGlassIdx);
}

const LPVOID CMemoryInterface_Review::GetCurrentAlignData() const
{
	int nGlassIdx = GetCurrentGlassIndex();
	return GetAlignData(nGlassIdx);
}

LPVOID CMemoryInterface_Review::GetCurrentCellData(int nCellIdx)
{
	int nGlassIdx = GetCurrentGlassIndex();
	return GetCellData(nGlassIdx, nCellIdx);
}

const LPVOID CMemoryInterface_Review::GetCurrentCellData(int nCellIdx) const
{
	int nGlassIdx = GetCurrentGlassIndex();
	return GetCellData(nGlassIdx, nCellIdx);
}

LPVOID CMemoryInterface_Review::GetCurrentDefectData(int nDefectIdx)
{
	int nGlassIdx = GetCurrentGlassIndex();
	return GetDefectData(nGlassIdx, nDefectIdx);
}

const LPVOID CMemoryInterface_Review::GetCurrentDefectData(int nDefectIdx) const
{
	int nGlassIdx = GetCurrentGlassIndex();
	return GetDefectData(nGlassIdx, nDefectIdx);
}

LPVOID CMemoryInterface_Review::GetCurrentImageData()
{
	int nImageIdx = GetCurrentImageIndex();
	return GetImageData(nImageIdx);
}

const LPVOID CMemoryInterface_Review::GetCurrentImageData() const
{
	int nImageIdx = GetCurrentImageIndex();
	return GetImageData(nImageIdx);
}