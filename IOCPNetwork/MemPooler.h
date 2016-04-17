#pragma once
#include <assert.h>

// Edited from C8MemPool class (Hitel, gma 7 3) - 1CoinClear -
template<class Type> class CMemPooler
{
public:
	// 생성자 
	CMemPooler(int nNumOfBlock):m_nNumofBlock(nNumOfBlock),
		                       m_pFreeList(NULL),
							   m_pMemBlock(NULL),
							   m_nAllocCount(0)
	{
		assert(nNumOfBlock > 0);
		m_nListBlockSize = sizeof(BlockNode) + sizeof(Type);
		
		Create();
	}

    // 소멸자
	~CMemPooler()
	{
		Destroy();
	}

	// 메모리 할당 
	Type* Alloc()
	{
		EnterCriticalSection(&m_csAlloc);

	    BlockNode* pNode = NULL;
		Type* pRet = NULL;

		////////////////////////////
        pNode = m_pFreeList;
		if(pNode != NULL)
		{
			m_pFreeList = m_pFreeList->pNext;
			m_nAllocCount++;
			pRet = reinterpret_cast<Type*>(pNode+1);
		}

		////////////////////////////
		LeaveCriticalSection(&m_csAlloc);

		return pRet;
	}

	BOOL Free(Type* freeBlock)
	{
		EnterCriticalSection(&m_csAlloc);

		BlockNode* pNode = NULL;
        BOOL bRet = FALSE;

		///////////////////////////

		pNode = (reinterpret_cast<BlockNode*>(freeBlock)) - 1;
		if(m_nAllocCount > 0)
		{
			pNode->pNext = m_pFreeList;
			m_pFreeList = pNode;
			m_nAllocCount--;
            bRet = TRUE;
		}

		///////////////////////////
		LeaveCriticalSection(&m_csAlloc);

		return bRet;
	}

	int GetAllocCount()
	{
		return m_nAllocCount;
	}
	int GetBlockCount()
	{
		return m_nNumofBlock;
	}

protected:
	void Create()
	{
		const int AllocationSize = (m_nListBlockSize) * m_nNumofBlock; // 메모리 할당할 크기 
		m_pMemBlock = VirtualAlloc(NULL, AllocationSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		assert(m_pMemBlock);

		BlockNode* pNode = reinterpret_cast<BlockNode*>(m_pMemBlock);

		pNode = reinterpret_cast<BlockNode*>((reinterpret_cast<DWORD>(pNode))+(m_nNumofBlock-1)* (m_nListBlockSize) );
		for(int i = m_nNumofBlock - 1; i >= 0; i--)
		{
			pNode->pNext = m_pFreeList; // 처음에는 NULL , 즉 Tail 은 NULL 로 한다.
			m_pFreeList = pNode;
			pNode = reinterpret_cast<BlockNode*>((reinterpret_cast<DWORD>(pNode))-m_nListBlockSize);
		}

		InitializeCriticalSection(&m_csAlloc);
	}

	void Destroy()
	{
		if(m_pMemBlock)
		{
			VirtualFree(m_pMemBlock,0,MEM_RELEASE);
		}

		DeleteCriticalSection(&m_csAlloc);
	}

    ///////////////////////////////////////////
	// 링크드 리스트 처럼 관리를 위한 노드 타입
	struct BlockNode
	{
		BlockNode* pNext;
		BlockNode()
		{
			pNext=NULL;
		}
	};
	//////////////////////////////////////////

protected:
	BlockNode* m_pFreeList; // 남아 있는 메모리 블럭 리스트
	void* m_pMemBlock;

	int m_nNumofBlock;      // 메모리 할당할 블럭 수
	int m_nListBlockSize;   // 한 블럭 사이즈 
	int m_nAllocCount;      // 할당된 메모리 블럭 갯수

	CRITICAL_SECTION m_csAlloc;  // For Thread-Safe;
};