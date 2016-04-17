#pragma once

class AFX_EXT_CLASS CAssemUtil
{
public:
	// Assem�� ���� �迡 ����� �����.
	static void ByteToShort( unsigned char* pSrc, short* pDest, int nSize);
	// ��ġ ���ϱ�, ������ �¿� Ž�� �������� ����
	static void CheckPitchSmall16(int pitch16, int searchCount, int left, int top, int width, int height, BYTE *pData, int buffWidth, int Dpc_suppress, int &findPitch16, int &iPitch, int &spr);
	static void CheckPitchSmallVert16(int pitch16, int searchCount, int left, int top, int width, int height, BYTE *pData, int buffWidth, int Dpc_suppress, int &findPitch16, int &iPitch, int &spr);

	// ���ֱ� �ַ�� : Vertical �̸鼭 Tilt�� �̹������� Dynamic ��ġ ���ϱ�
	// 1. Vertical Pitch & Tilt ���ϱ�
	static void CheckPitchSmallVPT16(int pitchStart16, int pitchEnd16, int tiltStart16, int tiltEnd16, int left, int right, int top, int bottom, BYTE *pData, int buffWidth, int Dpc_suppress, short* conBuff, short* conBuff2, int &findPitch16, int &findTilt16);
	// 2. Vertical Pitch���� Tilt ���ϱ�
	static void CheckPitchSmallVP_T16(int pitch16, int tiltStart16, int tiltEnd16, int left, int right, int top, int bottom, BYTE *pData, int buffWidth, int Dpc_suppress, short* conBuff, short* conBuff2, int &findTilt16);
	// 3. Tilt ���� Vertical Pitch ���ϱ�
	static void CheckPitchSmallVT_P16(int pitchStart16, int pitchEnd16, int Tilt16, int left, int right, int top, int bottom, BYTE *pData, int buffWidth, int Dpc_suppress, short* conBuff, short* conBuff2, int &findPitch16);
	static BOOL CheckPitchSmallVT_P16(int pitchStart16, int pitchEnd16, int Tilt16, int left, int right, int top, int bottom, BYTE *pData, int buffWidth, int Dpc_suppress, short* conBuff, short* conBuff2, int &findPitch16, int PitchStep);




	// start �� end�� integer�� �Ǿ� �ִ�. 3.0�� �˻��Ϸ��� 48��(3*16)�� �˻縦 �����Ѵ�.
	// sub pixel�� �⺻������ 1/16 �̴�.
	static void CheckPitch(int StartPitch, int EndPitch, int Spr, int left, int top, int width, int height, BYTE *pData, int buffWidth, int Dpc_suppress, int &rPitch, int &rfSpr);
	// ���� pitch�� start~end���� �˻��ϳ�, ��ġ ��ȭ�� �ް����� �ʱ⶧���� ���� ��ġ�� ��ġ�� 1/16 ������ ��ġ �˻� ����
	static void CheckPitchSmall(int StartPitch, int EndPitch, int Spr, int fSpr, int sSpr, int left, int top, int width, int height, BYTE *pData, int buffWidth, int Dpc_suppress, int &rPitch, int &rfSpr);
	// ���� pitch���� count�� �ٿ� ������. �Էµ���ġ�� 16���� int���̴�.
	static void CheckPitchPixel(int startPitch, int endPitch, int left, int top, int width, int height, BYTE *pData, int buffWidth, int Dpc_suppress, int &iPitch);
	static void CheckPitchEcho(int StartPitch, int EndPitch, int Spr, int left, int top, int width, int height, BYTE *pData, int buffWidth, int Dpc_suppress, int &rPitch, int &rfSpr);


};
