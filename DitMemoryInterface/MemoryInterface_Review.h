#pragma once

#include "MemoryInterface.h"
 
#define MAX_REVIEW_GLASS_COUNT			100
#define MAX_REVIEW_CELL_COUNT			500
#define MAX_REVIEW_DEFECT_COUNT			1000
#define MAX_REVIEW_SIGNAL_TYPE_COUNT	1
#define MAX_REVIEW_IMAGE_COUNT			1000

class AFX_EXT_CLASS CMemoryInterface_Review : public CMemoryInterface
{
public:
	CMemoryInterface_Review(int nType, IMemoryInterface2Parent* pIMI2P=NULL, DWORD dwPeriod=50);
	virtual ~CMemoryInterface_Review(void);	
	
	// CTimerThreadPool	
	virtual BOOL	StartThread();
	virtual void	TimerThreadProcess(PVOID pParameter);

	LPVOID			GetGlassData(int nGlassIdx);
	LPVOID			GetRecipeData(int nGlassIdx);
	LPVOID			GetAlignData(int nGlassIdx);
	LPVOID			GetCellData(int nGlassIdx, int nCellIdx=0);
	LPVOID			GetDefectData(int nGlassIdx, int nDefectIdx=0);
	LPVOID			GetImageData(int nImageIdx=0);

	const LPVOID	GetGlassData(int nGlassIdx) const;
	const LPVOID	GetRecipeData(int nGlassIdx) const;
	const LPVOID	GetAlignData(int nGlassIdx) const;
	const LPVOID	GetCellData(int nGlassIdx, int nCellIdx=0) const;
	const LPVOID	GetDefectData(int nGlassIdx, int nDefectIdx=0) const;
	const LPVOID	GetImageData(int nImageIdx=0) const;

	int				GetCurrentGlassIndex() const;
	int				GetCurrentGlassCount() const;
	int				GetCurrentImageIndex() const;
	int				GetCurrentImageCount() const;
	
	LPVOID			GetCurrentGlassData();
	LPVOID			GetCurrentRecipeData();
	LPVOID			GetCurrentAlignData();
	LPVOID			GetCurrentCellData(int nCellIdx=0);
	LPVOID			GetCurrentDefectData(int nDefectIdx=0);
	LPVOID			GetCurrentImageData();

	const LPVOID	GetCurrentGlassData() const;
	const LPVOID	GetCurrentRecipeData() const;
	const LPVOID	GetCurrentAlignData() const;
	const LPVOID	GetCurrentCellData(int nCellIdx=0) const;
	const LPVOID	GetCurrentDefectData(int nDefectIdx=0) const;
	const LPVOID	GetCurrentImageData() const;

protected:
	DWORD			m_dwMemoryDataSize;			// memory data size
	DWORD			m_dwGlassDataSize;			// glass data size
	DWORD			m_dwRecipeDataSize;			// recipe data size
	DWORD			m_dwAlignDataSize;			// align data size
	DWORD			m_dwCellDataSize;			// cell data size
	DWORD			m_dwDefectDataSize;			// defect data size
	DWORD			m_dwImageDataSize;			// image data size

	DWORD			m_dwGlassDataIndex;			// glass data start index
	DWORD			m_dwRecipeDataIndex;		// recipe data start index
	DWORD			m_dwAlignDataIndex;			// align data start index
	DWORD			m_dwCellDataIndex;			// cell data start index
	DWORD			m_dwDefectDataIndex;		// defect data start index
	DWORD			m_dwImageDataIndex;			// image data start index

	DWORD			m_dwCellSizePerOneGlass;	// cell data size / one glass
	DWORD			m_dwDefectSizePerOneGlass;	// defect data size / one glass
	DWORD			m_dwImageSizeTotalMemory;	// image data size / total memory
};

