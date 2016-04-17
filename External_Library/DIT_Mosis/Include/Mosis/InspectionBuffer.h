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


// ������ ������ ���� ���� ����ü.
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


// �ּ�, width, height ���� ������ �پ��� ó���� �ϱ����� ��� Ŭ�����̴�. �����͸� ���� �������� �ʴ´�.
// �˻� ���� Ư���� boundary�� �Ѵ� ������ ���ٵ� �����ϵ��� �����. ���̳ʽ� x, y, �� width, height�� �Ѿ�� x, y �ȼ����� ������ �����ϴ�.
// ���� �޸𸮿� �������� ������ �����������.. ���ۿ��� �� �����ؼ� ����ؾ� �Ѵ�.

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
	CRect IntersectRect(CRect &rect);// RECT�� ���� ũ�⿡ �°� �� ����. Return Value�� rect�� ���۸� ������� false, �ƴϸ� true
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


// �ּ�, width, height ���� ������ �پ��� ó���� �ϱ����� ��� Ŭ�����̴�. �����͸� ���� �������� �ʴ´�.
// ���� �ּ� �� ������ x, y��ŭ ���۸� shift�ؼ� ����� �� �ְ� ���ش�.
// ���� �ּ� ü�谡 �ش� ���� �� shift x, y ���ٴ� ũ�ٴ� �����Ͽ� ��밡��.
class  AFX_EXT_CLASS  CShiftBuffer : public CMosisBuffer
{
	int x, y;// x�� y�� pitch ��ŭ �̵��� ����..
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
// �ּ�, width, height ���� ������ �پ��� ó���� �ϱ����� ��� Ŭ�����̴�. �����͸� ���� �������� �ʴ´�.
// ���� �ּ�, width, height �� ������ rectROI ũ���� �����ΰ� ó�� �����ϰ� �Ѵ�.
// ū ���۸� ��ġ ���� �����ξ� �����ϰ� �ϴ� ����.
class  AFX_EXT_CLASS  CCropBuffer : public CMosisBuffer
{
	int x, y;// x�� y�� pitch ��ŭ �̵��� ����..
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

// �ּ�, width, height ���� ������ �پ��� ó���� �ϱ����� ��� Ŭ�����̴�. �����͸� ���� �������� �ʴ´�.
// �ٸ� ���ۿ��� �����͸� �����ͼ� �� ���ۿ� ī���Ѵ�.
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


// �ּ�, width, height ���� ������ �پ��� ó���� �ϱ����� ��� Ŭ�����̴�. �����͸� ���� �������� �ʴ´�.
// ���Ͽ��� �����͸� �����ͼ� �� ���ۿ� ī���Ѵ�.
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


// �����ϰ� �ڽ��� �����͸� �޸𸮿� ����� ���� ����� �Ѵ�.
// �ٸ� ���� ó�� ����� �ϱ� ���ؼ� �����۸� copy���۳� shift����, file���۵ �����ڷ� �Ѱ��ָ� �ȴ�.
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


// �־��� �̸��� ������ ����� ���� �����Ѵ�.
// ������ ȣ���Ҷ� ���� �ض�...������.
// AttachToFile �Լ��� ȣ��ɶ����� �̹����� �߰��Ѵ�.
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



// ������ ������ �̹����� �����ϱ� ���� Ŭ����.
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


// ���۸� ���������� ���Ͽ� �����ϱ� ���� Ŭ����.
class AFX_EXT_CLASS  CFrameSave_OneFile
{
	CBufferAttach *pBufferAttach;
	int width, height;
	
	// ������ �����Ϸ��� �ϴ� ���� �ε���.
	int	idStartFrame;
	
	// ������ ������ �� �� ������ ������ ��.
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


// width, height ��ŭ�� ���۸� �������� ���� �Ѵ�.
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
