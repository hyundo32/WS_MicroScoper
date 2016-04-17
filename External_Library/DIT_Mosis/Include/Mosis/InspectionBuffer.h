// InspectionBuffer.h: interface for the CMosisBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSPECTIONBUFFER_H__16133B59_E7ED_46D2_9FCE_FF4D0A62B052__INCLUDED_)
#define AFX_INSPECTIONBUFFER_H__16133B59_E7ED_46D2_9FCE_FF4D0A62B052__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct stBufferSize;
class CMosisBuffer;
class CShiftBuffer;
class CCropBuffer;
class CCopyBuffer;
class CFileBuffer;
class COwnerBuffer;
struct stImageFileHeader;
class CBufferAttach;
class CFrameSave;
class CFrameSave_OneFile;
class CImageBuffer;


// 버퍼의 사이즈 관련 정보 구조체.
struct AFX_EXT_CLASS stBufferSize
{
	int		m_Width, m_Height;
	int		m_DataWidth;
public:
	stBufferSize(int width= 0, int height= 0)
	{
		SetSize(width, height);
	}
	stBufferSize(stBufferSize& size)
	{
		SetSize(size.GetWidth(), size.GetHeight());
	}
	stBufferSize(stBufferSize* pSize)
	{
		SetSize(pSize->GetWidth(), pSize->GetHeight());
	}

public:
	int GetWidth(){return m_Width;}
	int GetHeight(){return m_Height;}
	int GetDataWidth(){return m_DataWidth;}
	UINT GetDataSize(){return (UINT)GetDataWidth()*GetHeight();}

public:
	static int ChangeDataWidth(int width){return (width+ 3)/4*4;}
	void SetSize(int width, int height)
	{
		m_Width= width;
		m_Height= height;
		m_DataWidth= ChangeDataWidth(width);//(width+ 3)/4*4 ;
	}
};


// 주소, width, height 만을 가지고 다양한 처리를 하기위한 기능 클래스이다. 데이터를 실제 소유하지 않는다.
// 검사 버퍼 특성상 boundary를 넘는 데이터 접근도 가능하도록 설계됨. 마이너스 x, y, 및 width, height를 넘어서는 x, y 픽셀로의 접근이 가능하다.
// 물론 메모리에 존재하지 않으면 뻗어버리겠지.. 버퍼에서 잘 조절해서 사용해야 한다.

class  AFX_EXT_CLASS  CMosisBuffer
{
protected:
	BYTE * m_pData;
	int m_Width, m_DataWidth;
	int m_Height;

public:
	CMosisBuffer()
		:m_pData(NULL)
	{SetSize(0, 0);}
	CMosisBuffer(BYTE* pData_, int w, int h)
		: m_pData(pData_)
	{		SetSize(w, h);	}
	virtual ~CMosisBuffer()
	{	};


	virtual BOOL SetSize(int w, int h)
	{
		m_Width= w;
		m_Height= h;
		m_DataWidth= CMosisBuffer::ChangeDataWidth(w);//(m_Width+ 3)/4*4;
		return TRUE;
	}
	void ChangeData(BYTE* pd)	{ m_pData= pd;	}
	void SetBuffer(BYTE* pData_, int w, int h)
	{
		ChangeData(pData_);
		SetSize(w, h);
	}
	void MemSet(BYTE val);
public:
	BYTE * GetDataAddress(){return m_pData;}
	BYTE* GetDataAddress(int x, int y){return m_pData+ GetDataWidth()*y+ x;};

	virtual CRect GetAvailableInspectRect(CRect requestRect)
	{
		CRect rect;
		rect.SetRect(1, 1, GetWidth()- 1, GetHeight()- 1);
		rect.IntersectRect(rect, requestRect);
		return rect;
	}
	CRect IntersectRect(CRect &rect);// RECT를 버퍼 크기에 맞게 재 설정. Return Value는 rect가 버퍼를 벗어났으면 false, 아니면 true
	int GetSize(){return GetWidth()*GetHeight();}
	int GetDataSize(){return GetDataWidth()*GetHeight();}
	int GetWidth(){ return m_Width;}
	int GetDataWidth(){ return m_DataWidth;}
	int GetHeight(){return m_Height;}
	BYTE GetPixel(int idx){return GetPixel(idx%GetDataWidth(), idx/GetDataWidth());};
	BYTE GetPixel(int x, int y){ return *(m_pData+ x+ y*GetDataWidth());}
	BYTE GetPixel(double fx, int y);
	BYTE GetPixel(int x, double fy);
	void SetPixel(int x, int y, BYTE value){*(GetDataAddress(x, y))= value;}
	BOOL SetValidPixel(int x, int y, BYTE value){if(! IsValidPixel(x, y)) return FALSE; SetPixel(x, y, value); return TRUE;}

	int GetDiff32(int x, int y, double xPitch);
	int	GetDiff23(int x, int y, double yPitch);

	int GetXDiff22(int x, int y, double xPitch);
	int GetYDiff22(int x, int y, double yPitch);

	int GetVar(int x, int y, int min);

	float GetMeanBright();
	void AdjustBright(CMosisBuffer &buffer);
	void FlipUpDown();
	BOOL IsValidBuffer();
	virtual BOOL IsValidPixel(int x, int y)
	{
		return x >= 0 && y >= 0 && x < m_Width && y < m_Height;
	}

	static BOOL CopyBtoA(CMosisBuffer &aBuffer, int x, int y, CMosisBuffer &bBuffer, CRect &rectFrom);
	static int ChangeDataWidth(int width){return (width+ 3)/4*4;}

	BOOL Contains(CRect &rect)
	{
		if(rect.left < 0)	return FALSE;
		if(rect.top < 0)	return FALSE;
		if(rect.right > GetWidth())		return FALSE;
		if(rect.bottom > GetHeight())	return FALSE;

		return TRUE;
	}
};


// 주소, width, height 만을 가지고 다양한 처리를 하기위한 기능 클래스이다. 데이터를 실제 소유하지 않는다.
// 원래 주소 를 버리고 x, y만큼 버퍼를 shift해서 사용할 수 있게 해준다.
// 원래 주소 체계가 해당 버퍼 와 shift x, y 보다는 크다는 가정하에 사용가능.
class  AFX_EXT_CLASS  CShiftBuffer : public CMosisBuffer
{
	int x, y;// x와 y는 pitch 만큼 이동한 거지..
public:
	CShiftBuffer(CMosisBuffer& iBuffer, int x_, int y_)
		: CMosisBuffer(iBuffer.GetDataAddress(), iBuffer.GetWidth(), iBuffer.GetHeight()), x(x_), y(y_)
	{
		m_pData = GetDataAddress(x, y);

		CRect	rect(0, 0, GetWidth(), GetHeight());
		CRect	rectShift = rect;
		rectShift.OffsetRect(x, y);
		
		rect.IntersectRect(rectShift, rect);
	}
	virtual CRect GetAvailableInspectRect(CRect requestRect)
	{
		CRect rect;
		rect.SetRect(1, 1, GetWidth()- x- 1, GetHeight()- y- 1);
		rect.IntersectRect(rect, requestRect);
		return rect;
	}
	virtual BOOL IsValidPixel(int xx, int yy)
	{
		xx= xx- x;
		yy= yy- y;
		return CMosisBuffer::IsValidPixel(xx, yy);
	}

};

// 2008-08-28, miniont, begin
// 주소, width, height 만을 가지고 다양한 처리를 하기위한 기능 클래스이다. 데이터를 실제 소유하지 않는다.
// 원래 주소, width, height 를 버리고 rectROI 크기의 버퍼인것 처럼 동작하게 한다.
// 큰 버퍼를 마치 작은 버퍼인양 동작하게 하는 버퍼.
class  AFX_EXT_CLASS  CCropBuffer : public CMosisBuffer
{
	int x, y;// x와 y는 pitch 만큼 이동한 거지..
public:
	CCropBuffer() : CMosisBuffer(NULL, 0, 0){};
	CCropBuffer(CMosisBuffer& iBuffer, CRect &rectROI)
		: CMosisBuffer(iBuffer.GetDataAddress(rectROI.left, rectROI.top), rectROI.Width(), rectROI.Height()), x(rectROI.left), y(rectROI.top)
	{
		CRect	rectImage(0, 0, iBuffer.GetWidth(), iBuffer.GetHeight());

		CRect	rectBoundaryChecked;
		rectBoundaryChecked.IntersectRect(rectROI, rectImage);

		m_Width = rectBoundaryChecked.Width();		
		m_Height = rectBoundaryChecked.Height();	
		m_DataWidth = iBuffer.GetDataWidth();
	}
	void DoCrop(CMosisBuffer& iBuffer, CRect &rectROI)
	{
		SetBuffer(iBuffer.GetDataAddress(rectROI.left, rectROI.top), rectROI.Width(), rectROI.Height());
		x= rectROI.left;
		y= rectROI.top;
		CRect	rectImage(0, 0, iBuffer.GetWidth(), iBuffer.GetHeight());
		CRect	rectBoundaryChecked;
		rectBoundaryChecked.IntersectRect(rectROI, rectImage);
		m_Width = rectBoundaryChecked.Width();		
		m_Height = rectBoundaryChecked.Height();	
		m_DataWidth = iBuffer.GetDataWidth();
	}
	virtual BOOL IsValidPixel(int xx, int yy)
	{
		xx= xx- x;
		yy= yy- y;
		return CMosisBuffer::IsValidPixel(xx, yy);
	}
};

// 주소, width, height 만을 가지고 다양한 처리를 하기위한 기능 클래스이다. 데이터를 실제 소유하지 않는다.
// 다른 버퍼에서 데이터를 가져와서 본 버퍼에 카피한다.
class  AFX_EXT_CLASS CCopyBuffer : public CMosisBuffer
{
public:
	CCopyBuffer(CMosisBuffer& buffer)
		: CMosisBuffer(buffer.GetDataAddress(), buffer.GetWidth(), buffer.GetHeight())
	{
		m_DataWidth= buffer.GetDataWidth();
	}
	CCopyBuffer(unsigned char *pData_, int width, int height)
		: CMosisBuffer(pData_, width, height)
	{
	}
	CCopyBuffer(int width, int height)
		: CMosisBuffer(NULL, width, height)
	{
	}
	CCopyBuffer()
		: CMosisBuffer(NULL, 0, 0)
	{
	}

public:
	BOOL FillBufferCenter(CMosisBuffer &buffer, double xx, double yy)
	{
		return FillBuffer(buffer, xx- GetWidth()/2, yy- GetHeight()/2);
	}
	BOOL FillBuffer(CMosisBuffer &buffer, double xx, double yy);
	BOOL FillBufferCenter(CMosisBuffer& buffer, int x, int y)
	{
		return FillBuffer(buffer, x- GetWidth()/2, y- GetHeight()/2);
	}
	BOOL FillBuffer(CMosisBuffer& buffer, int x, int y);
};


// 주소, width, height 만을 가지고 다양한 처리를 하기위한 기능 클래스이다. 데이터를 실제 소유하지 않는다.
// 파일에서 데이터를 가져와서 본 버퍼에 카피한다.
class  AFX_EXT_CLASS  CFileBuffer : public CMosisBuffer
{
public:
	CFileBuffer(CMosisBuffer &buffer)
		:CMosisBuffer(buffer.GetDataAddress(), buffer.GetWidth(), buffer.GetHeight())
	{
		m_DataWidth= buffer.GetDataWidth();
	}
	CFileBuffer(int width, int height)
		: CMosisBuffer(NULL, width, height)
	{
	}

	CFileBuffer(unsigned char *pData_, int width, int height)
		: CMosisBuffer(pData_, width, height)
	{
	}

	// miniont, 2008-8-28, end
public:
	void FillBuffer(CFile &file, int x, int y, int oWidth, int oHeight);
	void FillBufferReverse(CFile &file, int x, int y, int oWidth, int oHeight);		// miniont, 2008-09-10
	void FillBufferFromTwoFile(CFile &file1, CFile &file2, int x, int y, int FirstHeight, int SecondHeight, int oWidth, int oHeight);		// miniont, 2008-09-29
	void SaveBufferToTwoFile(CFile &file1, CFile &file2, int x, int y, int FirstHeight, int SecondHeight, int oWidth, int oHeight);		// miniont, 2008-11-24

	const int GetContentWidth(CFile &file);															// miniont, 2008-09-04
	const int GetContentHeight(CFile &file);
	
	static BITMAPINFOHEADER GetBitmapInfoHeader(CFile &file);
	// miniont, 2008-09-04

public:
// 	CFile m_File;
// 	BITMAPINFOHEADER m_InfoHeader;
// 	BOOL ReadBuffer(CMosisBuffer &buffer, int x, int y, int width, int height);
// 	BOOL WriteBuffer(CMosisBuffer &buffer, int x, int y, int width, int height);
};


// 유일하게 자신의 데이터를 메모리에 만들어 내는 기능을 한다.
// 다른 버퍼 처리 기능을 하기 위해서 본버퍼를 copy버퍼나 shift버퍼, file버퍼등에 생성자로 넘겨주면 된다.
class AFX_EXT_CLASS COwnerBuffer : public CMosisBuffer
{
	int m_DataSpace;
public:
	COwnerBuffer(int ww= 0, int hh= 0);
	virtual ~COwnerBuffer();
	virtual BOOL SetSize(int ww, int hh);
	void ReleaseSpace();
};


#include <pshpack2.h>

struct AFX_EXT_CLASS stImageFileHeader
{
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')
	BITMAPFILEHEADER	fileHeader;
	BITMAPINFOHEADER	infoHeader;
	RGBQUAD				quad[256];

	stImageFileHeader()
	{
		fileHeader.bfType = DIB_HEADER_MARKER;  // "BM"
		fileHeader.bfReserved1 = 0;
		fileHeader.bfReserved2 = 0;
		
		infoHeader.biSize = 40;// sizeof(BITMAPINFOHEADER)
		infoHeader.biWidth = 0;
		infoHeader.biHeight = 0;
		infoHeader.biPlanes = 1;
		infoHeader.biBitCount = 8;
		infoHeader.biCompression = 0;
		infoHeader.biSizeImage = 0;
		infoHeader.biXPelsPerMeter = 2923;
		infoHeader.biYPelsPerMeter = 2923;
		infoHeader.biClrUsed = 0;
		infoHeader.biClrImportant = 0;


		fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256;
		fileHeader.bfSize = fileHeader.bfOffBits + infoHeader.biSizeImage;

		int i;
		for(i= 0; i< 256; i++)
		{
			quad[i].rgbRed= i;
			quad[i].rgbGreen= i;
			quad[i].rgbBlue= i;
			quad[i].rgbReserved= 0;
		}
	}

public:
	BOOL IsValidHeader()	{	return infoHeader.biWidth > 0;	}// IsValid()
	int GetWidth()	{	return infoHeader.biWidth;	}
	int GetHeight()	{	return infoHeader.biHeight;	}
	UINT GetDataOffset(){	return fileHeader.bfOffBits;}
	UINT GetDataSize(){	return infoHeader.biSizeImage;}

	void ChangeSize(int width, int height)// void ChangeHeader(int width, int height)
	{
		infoHeader.biWidth = width;
		infoHeader.biHeight = height;
		stBufferSize bufferSize(width, height);
		infoHeader.biSizeImage = bufferSize.GetDataSize();//width*height*1;

//		fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256;
		fileHeader.bfSize = fileHeader.bfOffBits + infoHeader.biSizeImage;
	}

	void ReadFromFile(CString &fileName)
	{
		CFile file;
		file.Open(fileName, CFile::modeCreate | CFile::typeBinary);
		ReadFromFile(file);
		file.Close();
	}
	void ReadFromFile(CFile &file)
	{
		file.Seek(0, SEEK_SET);
		file.Read(&fileHeader, sizeof(BITMAPFILEHEADER));
		file.Read(&infoHeader, sizeof(BITMAPINFOHEADER));
		file.Read(quad, sizeof(RGBQUAD)*256);
		ChangeSize(infoHeader.biWidth, infoHeader.biHeight);
	}
	BITMAPINFO* GetBitmapInfo(){return (BITMAPINFO*) &infoHeader;}
};
#include <poppack.h>


// 주어진 이름의 파일을 지우고 새로 생성한다.
// 생성자 호출할때 주의 해라...ㅋㅋㅋ.
// AttachToFile 함수가 호출될때마다 이미지를 추가한다.
class  AFX_EXT_CLASS CBufferAttach
{
	CString m_FileName;
	stImageFileHeader *m_pImageFileHeader;
	int		m_bFileMade;
public:
	CBufferAttach(CString &name);
	~CBufferAttach();
public:
	void	AttachToFile(CMosisBuffer &buffer);
protected:
	BOOL	MakeFile();
};



// 프레임 단위로 이미지를 저장하기 위한 클래스.
class AFX_EXT_CLASS CFrameSave
{
	int iStartFrame;
	stImageFileHeader *pImageFileHeader;
	CString strDir;
	
public:
	CFrameSave(CString &strDir_, int startFrame, int width= 8192, int height= 1024);
	~CFrameSave();
	
public:
	void SaveFrame(BYTE *m_pData, int iFrame);
	void SaveFrame(BYTE *m_pData, int iScan, int iFrame, int cell, int bottom);
};


// 버퍼를 순차적으로 파일에 저장하기 위한 클래스.
class AFX_EXT_CLASS  CFrameSave_OneFile
{
	CBufferAttach *pBufferAttach;
	int width, height;
	
	// 저장을 시작하려고 하는 버퍼 인덱스.
	int	idStartFrame;
	
	// 저장할 프레임 수 및 저장한 프레임 수.
	int nSaveFrame;
	int iSaveFrame;
	
	
public:
	CFrameSave_OneFile(CString &fileName_, int startFrame, int saveFrame, int width_= 8192, int height_= 1024);
	~CFrameSave_OneFile();
public:
	void SaveFrame(int iFrame, CMosisBuffer &frameBuffer);
};



class AFX_EXT_CLASS  CDynamicBuffer : public COwnerBuffer
{
	
public:
	void FillBuffer(CMosisBuffer &buffer, CRect &rect);
};


// width, height 만큼의 버퍼를 동적으로 생성 한다.
class  AFX_EXT_CLASS  CImageBuffer : public CMosisBuffer
{
// 	BYTE * m_pData;
// 	int	m_Width, m_Height;
	int m_DataSpace;
	stImageFileHeader *m_pFileHeader;

public:
	CImageBuffer();
	~CImageBuffer();
public:
	stImageFileHeader* GetFileHeader()	{		return m_pFileHeader;	}
	
public:
//	stBufferSize GetSize(){return stBufferSize(m_Width, m_Height);}
//	int GetWidth(){return m_Width;}
//	int GetHeight(){return m_Height;}

	virtual BOOL SetSize(int ww, int hh);

	void ReleaseImage();
	BOOL ReadFromFile(CString &fileName);
};



#endif // !defined(AFX_INSPECTIONBUFFER_H__16133B59_E7ED_46D2_9FCE_FF4D0A62B052__INCLUDED_)
