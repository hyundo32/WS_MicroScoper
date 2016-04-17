#pragma once

class AFX_EXT_CLASS CAssemUtil
{
public:
	// Assem을 쓰는 김에 여기다 만든다.
	static void ByteToShort( unsigned char* pSrc, short* pDest, int nSize);
	// 피치 구하기, 변수를 좌우 탐색 개념으로 재편
	static void CheckPitchSmall16(int pitch16, int searchCount, int left, int top, int width, int height, BYTE *pData, int buffWidth, int Dpc_suppress, int &findPitch16, int &iPitch, int &spr);
	static void CheckPitchSmallVert16(int pitch16, int searchCount, int left, int top, int width, int height, BYTE *pData, int buffWidth, int Dpc_suppress, int &findPitch16, int &iPitch, int &spr);

	// 장주기 솔루션 : Vertical 이면서 Tilt된 이미지에서 Dynamic 피치 구하기
	// 1. Vertical Pitch & Tilt 구하기
	static void CheckPitchSmallVPT16(int pitchStart16, int pitchEnd16, int tiltStart16, int tiltEnd16, int left, int right, int top, int bottom, BYTE *pData, int buffWidth, int Dpc_suppress, short* conBuff, short* conBuff2, int &findPitch16, int &findTilt16);
	// 2. Vertical Pitch에서 Tilt 구하기
	static void CheckPitchSmallVP_T16(int pitch16, int tiltStart16, int tiltEnd16, int left, int right, int top, int bottom, BYTE *pData, int buffWidth, int Dpc_suppress, short* conBuff, short* conBuff2, int &findTilt16);
	// 3. Tilt 에서 Vertical Pitch 구하기
	static void CheckPitchSmallVT_P16(int pitchStart16, int pitchEnd16, int Tilt16, int left, int right, int top, int bottom, BYTE *pData, int buffWidth, int Dpc_suppress, short* conBuff, short* conBuff2, int &findPitch16);
	static BOOL CheckPitchSmallVT_P16(int pitchStart16, int pitchEnd16, int Tilt16, int left, int right, int top, int bottom, BYTE *pData, int buffWidth, int Dpc_suppress, short* conBuff, short* conBuff2, int &findPitch16, int PitchStep);




	// start 와 end가 integer로 되어 있다. 3.0을 검사하려면 48번(3*16)의 검사를 수행한다.
	// sub pixel의 기본단위는 1/16 이다.
	static void CheckPitch(int StartPitch, int EndPitch, int Spr, int left, int top, int width, int height, BYTE *pData, int buffWidth, int Dpc_suppress, int &rPitch, int &rfSpr);
	// 최초 pitch는 start~end까지 검사하나, 피치 변화가 급격하지 않기때문에 다음 위치의 피치는 1/16 단위로 피치 검사 수행
	static void CheckPitchSmall(int StartPitch, int EndPitch, int Spr, int fSpr, int sSpr, int left, int top, int width, int height, BYTE *pData, int buffWidth, int Dpc_suppress, int &rPitch, int &rfSpr);
	// 최초 pitch부터 count를 줄여 나간다. 입력된피치는 16배한 int값이다.
	static void CheckPitchPixel(int startPitch, int endPitch, int left, int top, int width, int height, BYTE *pData, int buffWidth, int Dpc_suppress, int &iPitch);
	static void CheckPitchEcho(int StartPitch, int EndPitch, int Spr, int left, int top, int width, int height, BYTE *pData, int buffWidth, int Dpc_suppress, int &rPitch, int &rfSpr);


};
