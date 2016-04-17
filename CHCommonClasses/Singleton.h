//////////////////////////////////////////////////////////////////////////////////////
// Singleton.h: interface for the Singleton class.
//////////////////////////////////////////////////////////////////////////////////////
//    ex) sample
//
//    class Test : public Singleton < Test >
//    {
//    public:
//	    Test* Sample( int num );
//	    //...
//    };
//
//
//
//    #define g_Test	Test::GetSingleton()
//
//    void SomeFuncton( void )
//    {
//	    Test * pTemp = Test::GetSingleton().Sample( 7 );
//	    // or   Test * pTemp = g_Test.Sample( 7 );
//    }
//
//////////////////////////////////////////////////////////////////////////////////////

/*---------------------------------------------------------------------------

# 파일명 : Singleton.h
# 설  명 : interface for the Singleton template class.
# 내  력 : 
# 비  고 : 

---------------------------------------------------------------------------*/

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

///////////////////////////////////////////////////////////////////////////////
// include define statement
///////////////////////////////////////////////////////////////////////////////

#include "assert.h"

///////////////////////////////////////////////////////////////////////////////
// class define statement
///////////////////////////////////////////////////////////////////////////////

/*---------------------------------------------------------------------------

# 클래스  명 : Singleton
# 부모클래스 : 없음
# 참조클래스 : 없음
# 관리책임자 : Voidhoon
# 설      명 : 
# 변      수 : 
# 내      력 : 
# 문  제  점 : 

---------------------------------------------------------------------------*/

template <typename T> class Singleton
{
    static T* ms_Singleton;

public:
    Singleton(void)
    {
        assert(!ms_Singleton);
        int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
        ms_Singleton = (T*)((int)this + offset);
    }

    ~Singleton(void)
    {
        assert(ms_Singleton);
        ms_Singleton = 0;
    }

	static void SetSingleton(T* pT)
	{
		assert(pT);
		ms_Singleton = pT;
	}

    static T* GetSingleton(void)
    {
        assert(ms_Singleton);
        return ms_Singleton;
    }

	static T* GetSingletonPtr(void)
	{
        assert(ms_Singleton);
        return ms_Singleton;
	}

	static T& GetSingletonInstance(void)
	{
        assert(ms_Singleton);
		return(*ms_Singleton);
	}
};

template <typename T> T* Singleton <T>::ms_Singleton = 0;

#endif
