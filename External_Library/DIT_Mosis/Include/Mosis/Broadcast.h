#if !defined(AFX_BROADCAST_H__180A3328_BAE3_4BA3_92F8_4F7C54938BC1__INCLUDED_)
#define AFX_BROADCAST_H__180A3328_BAE3_4BA3_92F8_4F7C54938BC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Broadcast.h : header file
//

#include <vector>

class CAudience
{
public:
	virtual void OnAir(int channel, int program, void* pContent)= 0;
};

class CBroadcast
{
	std::vector<CAudience*> audiences;
public:
	void Register(CAudience* pAucience)
	{
		audiences.push_back(pAucience);
	}

	int Broadcast(int channel, int program, void* pContent)
	{
		std::vector<CAudience*>::iterator iter;
		iter= audiences.begin();
		while(iter != audiences.end())
		{
			(*iter)->OnAir(channel, program, pContent);
			iter++;
		}
		return audiences.size();
	}
};


#endif // !defined(AFX_BROADCAST_H__180A3328_BAE3_4BA3_92F8_4F7C54938BC1__INCLUDED_)
