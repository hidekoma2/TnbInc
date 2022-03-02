#pragma once
/**
 *@file
 * ���������֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbException.h"
#include "TnbTickCount.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



#include <malloc.h>



//TNB Library
namespace TNB
{



/**@ingroup SYNC
 * �r����{�C���^�[�t�F�[�X
 *	
 *		Lock �� Unlock �������܂��B
 *
 *	@date 06/01/01	�V�K�쐬
 *	@date 06/08/23	�����o��const�錾
 */
struct ISynchronized
{
	/** �f�X�g���N�^ */
	virtual ~ISynchronized(void) {}

	/** 
	 * [�r��] ���b�N
	 *	@param dwTime ���b�N�Ƀg���C���������ؕb�B�ȗ�����Ɩ����ɑ҂��܂��B
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const = 0;

	/** [�r��] �A�����b�N */
	virtual void Unlock(void) const = 0;
};



/**@ingroup SYNC
 * HANDLE�^�r�����ۃN���X
 *
 *		�n���h�����g�����r���̒��ۃN���X�ł��B
 *		GetHandle() ����������ƁA ISynchronized::Lock() ����������܂��B
 *
 *	@date 06/10/31	�V�K�쐬
 */
class CAbstractSyncHandle : public ISynchronized
{
public:

	/**
	 * [�擾] �n���h���擾
	 *	@return �n���h��
	 */
	virtual HANDLE GetHandle(void) const = 0;

	/**
	 * [�擾] �n���h���擾
	 *	@return �n���h��
	 */
	operator HANDLE(void) const 
	{
		return GetHandle(); 
	}
	
	/** 
	 * [�r��] ���b�N
	 *	@param dwTime ���b�N�Ƀg���C���������ؕb�B�ȗ�����Ɩ����ɑ҂��܂��B
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const
	{
		HANDLE h = GetHandle();
		if ( h != NULL )
		{
			DWORD dwRc = ::WaitForSingleObject(h, dwTime);
			ASSERTLIB(dwRc != WAIT_FAILED);
			if ( dwRc == WAIT_OBJECT_0 )
			{
				return true;//��������
			}
		}
		return false;
	}
};


/**@ingroup SYNC
 * Section�r���Ǘ��N���X
 *
 *		CriticalSection���g�����������������܂��B
 *
 *	@note ���C���X�^���X�݂̂œ��������܂��B
 *	@attention ���X���b�h�ł̓��b�N�ł��܂���B����X���b�h�̏ꍇ�l�X�g���b�N�ɂȂ�܂��B
 *			���b�N�̐������A�����b�N�����Ă��������B
 *
 *	@see http://msdn.microsoft.com/library/ja/jpdllpro/html/_win32_initializecriticalsection.asp
 *
 *	@par�K�v�t�@�C��
 *			TnbSync.h
 *
 *	@date 06/01/01	�V�K�쐬
 */
class CSyncSection : public ISynchronized, CCopyImpossible
{
public:

	/// �R���X�g���N�^
	CSyncSection(void)
	{
		::InitializeCriticalSection(&m_cs); 
	}

	/** �f�X�g���N�^ */
	virtual ~CSyncSection(void)
	{
		::DeleteCriticalSection(&m_cs); 
	}

	/** 
	 * [�r��] ���b�N
	 *	@attention ���̃N���X��Lock()���\�b�h�Ƃ͈قȂ�AdwTime�͖�������A
	 *			INFINITE�Ƃ��ē��삵�܂��B�܂�ǂ�Ȓl�����Ă��A���b�N
	 *			�o����܂Ń��^�[������܂���B
	 *	@param dwTime �l�͖�������܂��B
	 *	@return true �Œ�
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const
	{
		::EnterCriticalSection(&m_cs);
		return true;
	}

	/** [�r��] �A�����b�N */
	virtual void Unlock(void) const
	{
		::LeaveCriticalSection(&m_cs); 
	}

private:
	mutable CRITICAL_SECTION	m_cs;	///< �r�����
	friend class CSyncSectionEx;		///< ���Ԑ錾
};



/**@ingroup SYNC
 * �g��Section�r���Ǘ��N���X
 *
 *		CriticalSection���g�����������������܂��B
 *
 *	@note CSyncSection �Ƃ̍��ق́A Lock() ���\�b�h�ɂ����āA
 *		���Ԃ��w��ł���_�ł��B		
 *
 *	@note ���C���X�^���X�݂̂œ��������܂��B
 *	@attention ���X���b�h�ł̓��b�N�ł��܂���B����X���b�h�̏ꍇ�l�X�g���b�N�ɂȂ�܂��B
 *			���b�N�̐������A�����b�N�����Ă��������B
 *
 *	@see http://msdn.microsoft.com/library/ja/jpdllpro/html/_win32_initializecriticalsection.asp
 *
 *	@par�K�v�t�@�C��
 *			TnbSync.h
 *
 *	@date 06/07/18	�V�K�쐬
 *	@date 06/07/27	dwTime ��L���ɂ����B
 */
class CSyncSectionEx : public CSyncSection
{
	DEFSUPER(CSyncSection);	
public:

	/** �R���X�g���N�^ */
	CSyncSectionEx(void) : _super(), m_hModule(NULL), m_pTryEntryCs(NULL)
	{
	}

	/** �f�X�g���N�^ */
	virtual ~CSyncSectionEx(void)
	{
		if ( m_hModule != NULL )
		{
			::FreeLibrary(m_hModule);
			m_hModule = NULL;
		}
	}

	/** 
	 * [�r��] ���b�N
	 *	@param dwTime ���b�N�Ƀg���C���������ؕb�B�ȗ�����Ɩ����ɑ҂��܂��B
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const
	{
		if ( dwTime == INFINITE )
		{
			return _super::Lock(dwTime);
		}
		if ( m_hModule == NULL )
		{
			m_hModule = ::LoadLibraryA("kernel32.dll");
			m_pTryEntryCs = reinterpret_cast<P_TryEntryCs>(::GetProcAddressA(m_hModule, "TryEnterCriticalSection"));
		}
		if ( m_pTryEntryCs == NULL )
		{
			ASSERT0(false, "CSyncSectionEx::Lock()", 
					"API 'TryEnterCriticalSection()' ��������܂���B\nOS ��NT4.0�ȍ~�łȂ��\��������܂��B");
			return false;
		}
		if ( dwTime == 0 )
		{
			return !! m_pTryEntryCs(&m_cs);	//TryEntryCriticalSection���s
		}
		CTickCount tick;
		while ( ! tick.IsPassed(dwTime) )
		{
			if ( m_pTryEntryCs(&m_cs) )	//TryEntryCriticalSection���s
			{
				return true;
			}
			::Sleep(1);
		}
		return false;
	}

private:
	typedef DWORD (WINAPI* P_TryEntryCs)(LPCRITICAL_SECTION);
	mutable P_TryEntryCs m_pTryEntryCs;			///< �uTryEntryCriticalSection�v�̊֐��A�h���X
	mutable HMODULE			m_hModule;			///< ���W���[���n���h��
};



/**@ingroup SYNC
 * Mutex�r���Ǘ��N���X
 *
 *		MUTEX���g�����������������܂��B
 *
 *	@note �ʃv���Z�X�ł��ʃC���X�^���X�ł��������O�Ȃ瓯�������܂��i�����r���I�u�W�F�N�g�Ƃ��ē����܂��j�B
 *	@attention ���X���b�h�ł̓��b�N�ł��܂���B����X���b�h�̏ꍇ�l�X�g���b�N�ɂȂ�܂��B
 *			���b�N�̐������A�����b�N�����Ă��������B
 *
 *	@see http://msdn.microsoft.com/library/ja/jpdllpro/html/_win32_createmutex.asp
 *
 *	@par�K�v�t�@�C��
 *			TnbSync.h
 *
 *	@date 06/01/01 �V�K�쐬
 *	@date 06/10/31 ���ۃN���X���g�p����悤�ɕύX�B
 *	@date 06/11/15 �쐬���s������ �X���[����悤�ɕύX�B
 */
class CSyncMutex : public CAbstractSyncHandle , CCopyImpossible
{
	DEFSUPER(CAbstractSyncHandle);
public:

	/**
	 * �R���X�g���N�^
	 *	@param boIsLock ������ԁBtrue�Ȃ�Lock��ԁA�ȗ���false�Ȃ�Unlock��Ԃō쐬���܂��B
	 *	@param lpszName MUTEX�̖��O�B�ȗ��\�B
	 *	@throw CInvalidParamException ���O�s���ȂǁA�����I�u�W�F�N�g���쐬�ł��Ȃ��������A�X���[����܂��B
	 */
	explicit CSyncMutex(bool boIsLock = false, LPCTSTR lpszName = NULL) : _super()
	{
		m_hMutex = ::CreateMutex(NULL, boIsLock, lpszName);
		DWORD r = ::GetLastError();
		if ( r != 0 && r != 0xB7 )
		{
			_GetLastError("CreateMutex");
		}
		ASSERT0(
			m_hMutex != NULL,
			"CSyncMutex::CSyncMutex()",
			"Mutex���������ł��܂��񂵂��B"
		);
		if ( m_hMutex == NULL )
		{
			throw CInvalidParamException();
		}
	}

	/**
	 * �R���X�g���N�^
	 *	@param hMutex CreateMutex()�ō쐬�����n���h��
	 *	@throw CInvalidParamException NULL�n���h�����w�肵�����A�X���[����܂��B
	 */
	explicit CSyncMutex(HANDLE hMutex) : _super()
	{
		m_hMutex = hMutex;
		if ( m_hMutex == NULL )
		{
			throw CInvalidParamException();
		}
	}

	/** �f�X�g���N�^ */
	virtual ~CSyncMutex(void)
	{
		if ( m_hMutex != NULL )
		{
			::ReleaseMutex(m_hMutex);
			::CloseHandle(m_hMutex);
		}
	}

	/** [�r��] �A�����b�N */
	virtual void Unlock(void) const
	{
		if ( m_hMutex != NULL )
		{
			::ReleaseMutex(m_hMutex);
		}
	}

	/**
	 * [�擾] Mutex�n���h���擾
	 *	@return �n���h��
	 */
	virtual HANDLE GetHandle(void) const 
	{
		return m_hMutex; 
	}

private:
	HANDLE	m_hMutex;				///< �r���n���h��
};



/**@ingroup SYNC
 * Semaphore�r���Ǘ��N���X
 *
 *		Semaphore���g�����������������܂��B
 *
 *	@note �ʃv���Z�X�ł��ʃC���X�^���X�ł��������O�Ȃ瓯�������܂��B
 *
 *	@note ���X���b�h�ł����b�N�ł��܂��i�f�b�h���b�N�ɒ��Ӂj�B
 *
 *	@see http://msdn.microsoft.com/library/ja/jpdllpro/html/_win32_createsemaphore.asp
 *
 *	@par�K�v�t�@�C��
 *			TnbSync.h
 *
 *	@date 06/05/01	�V�K�쐬
 *	@date 06/09/19	��������Unlock���\�b�h�A�ǉ��B
 *	@date 06/10/31	���ۃN���X���g�p����悤�ɕύX�B
 *	@date 06/11/15	�쐬���s������ �X���[����悤�ɕύX�B
 */
class CSyncSemaphore : public CAbstractSyncHandle , CCopyImpossible
{
	DEFSUPER(CAbstractSyncHandle);
public:

	/**
	 * �R���X�g���N�^
	 *	@param lpszName �����p�̖��O���w�肵�܂��B
	 *					�ȗ�����ƕʃC���X�^���X���m�̓����͎��܂���B
	 *	@param iMaximumCount �����Ɏ��s�ł��鐔���w�肵�܂��B�ȗ�����ƂP�ł��B
	 *					�������O�����łɍ쐬����Ă���ꍇ�A���̒l�͖�������܂��B
	 *	@throw CInvalidParamException ���O�s���ȂǁA�����I�u�W�F�N�g���쐬�ł��Ȃ��������A�X���[����܂��B
	 */
	explicit CSyncSemaphore(LPCTSTR lpszName = NULL, int iMaximumCount = 1) : _super()
	{
		m_hSema = ::CreateSemaphore(NULL, iMaximumCount, iMaximumCount, lpszName);
		ASSERT0(
			m_hSema != NULL,
			"CSyncSemaphore::CSyncSemaphore()",
			"Semaphore���������ł��܂��񂵂��B"
		);
		if ( m_hSema == NULL )
		{
			throw CInvalidParamException();
		}
	}

	/**
	 * �R���X�g���N�^
	 *	@param hSema CreateSemaphore()�ō쐬�����n���h��
	 *	@throw CInvalidParamException NULL�n���h�����w�肵�����A�X���[����܂��B
	 */
	explicit CSyncSemaphore(HANDLE hSema) : _super()
	{
		m_hSema = hSema;
		if ( m_hSema == NULL )
		{
			throw CInvalidParamException();
		}
	}

	/** �f�X�g���N�^ */
	virtual ~CSyncSemaphore(void)
	{
		if ( m_hSema != NULL )
		{
			::ReleaseSemaphore(m_hSema, 1, NULL);
			::CloseHandle(m_hSema);
		}
	}

	/** [�r��] �A�����b�N */
	virtual void Unlock(void) const
	{
		if ( m_hSema != NULL )
		{
			::ReleaseSemaphore(m_hSema, 1, NULL);
		}
	}

	/**
	 * [�r��] �A�����b�N
	 *	@param lCount �A�����b�N���鐔
	 *	@return ���݂̉�
	 */
	LONG Unlock(LONG lCount) const
	{
		LONG l = 0;
		if ( m_hSema != NULL )
		{
			::ReleaseSemaphore(m_hSema, lCount, &l);
		}
		return l;
	}

	/**
	 * [�擾] Mutex�n���h���擾
	 *	@return �n���h��
	 */
	virtual HANDLE GetHandle(void) const 
	{
		return m_hSema; 
	}

private:
	HANDLE	m_hSema;				///< �r���n���h��
};



/**@ingroup SYNC
 * Event�r���Ǘ��N���X
 *
 *		EVENT���g�����������������܂��B
 *
 *	@note �ʃv���Z�X�ł��ʃC���X�^���X�ł��������O�Ȃ瓯�������܂��B
 *
 *	@note ���X���b�h�ł����b�N�ł��܂��i�f�b�h���b�N�ɒ��Ӂj�B
 *
 *	@see http://msdn.microsoft.com/library/ja/jpdllpro/html/_win32_createevent.asp
 *
 *	@par�K�v�t�@�C��
 *			TnbSync.h
 *
 *	@date 06/04/19	�V�K�쐬
 *	@date 06/09/13	�R���X�g���N�^�̑��f�t�H���g������ύX�B
 *					Lock��ԂƃV�O�i����Ԃ̊֌W������Ă����B
 *	@date 06/10/31 ���ۃN���X���g�p����悤�ɕύX�B
 *	@date 06/11/15 �쐬���s������ �X���[����悤�ɕύX�B
 */
class CSyncEvent : public CAbstractSyncHandle , CCopyImpossible
{
	DEFSUPER(CAbstractSyncHandle);
public:

	/**
	 * �R���X�g���N�^
	 *	@param boIsManualReset false���ȗ��Ȃ玩�����Z�b�g�I�u�W�F�N�g�A
							true�Ȃ�蓮���Z�b�g�I�u�W�F�N�g�ɂȂ�܂��B
	 *	@param boIsLock ������ԁBtrue�Ȃ�Lock��ԁA�ȗ���false�Ȃ�Unlock��Ԃō쐬���܂��B
	 *	@param lpszName Event�̖��O�B�ȗ��\�B
	 *	@throw CInvalidParamException ���O�s���ȂǁA�����I�u�W�F�N�g���쐬�ł��Ȃ��������A�X���[����܂��B
	 */
	explicit 
	CSyncEvent(bool boIsManualReset = false, bool boIsLock = false, LPCTSTR lpszName = NULL) : _super()
	{
		m_hEvent = ::CreateEvent(NULL, boIsManualReset, ! boIsLock, lpszName);
		ASSERT0(
			m_hEvent != NULL,
			"CSyncEvent::CSyncEvent()",
			"Event���������ł��܂��񂵂��B���O�ɖ�肪����\�����L��܂��B"
		);
		if ( m_hEvent == NULL )
		{
			throw CInvalidParamException();
		}
	}

	/**
	 * �R���X�g���N�^
	 *	@param hEvent CreateEvent()�ō쐬�����n���h��
	 *	@throw CInvalidParamException NULL�n���h�����w�肵�����A�X���[����܂��B
	 */
	explicit CSyncEvent(HANDLE hEvent) : _super()
	{
		m_hEvent = hEvent;
		if ( m_hEvent == NULL )
		{
			throw CInvalidParamException();
		}
	}

	/** �f�X�g���N�^ */
	virtual ~CSyncEvent(void)
	{
		if ( m_hEvent != NULL )
		{
			::SetEvent(m_hEvent);
			::CloseHandle(m_hEvent);
		}
	}

	/** [�r��] �A�����b�N */
	virtual void Unlock(void) const
	{
		if ( m_hEvent != NULL )
		{
			::SetEvent(m_hEvent);
		}
	}

	/**
	 * [�r��] ���b�N��Ԃɂ���
	 *	@note �ҋ@�͂��܂���B���b�N�ɐ������Ȃ��\�����L��܂��B
	 */
	void ToLock(void)
	{
		if ( m_hEvent != NULL )
		{
			::ResetEvent(m_hEvent);
		}
	}

	/**
	 * [�r��] ���b�N��Ԃɂ���
	 *	@note ��u�A�����b�N��Ԃɂ��܂��B
	 */
	void Pulse(void)
	{
		if ( m_hEvent != NULL )
		{
			::PulseEvent(m_hEvent);
		}
	}

	/**
	 * [�擾] Mutex�n���h���擾
	 *	@return �n���h��
	 */
	virtual HANDLE GetHandle(void) const 
	{
		return m_hEvent; 
	}

private:
	HANDLE	m_hEvent;				///< �r���n���h��
};



/**@ingroup SYNC
 * �J�E���^���r���N���X
 *
 *		�J�E���^���g�����r���N���X�ł��B�u���b�N���邱�Ƃ��A�i���\���`�F�b�N���A
 *		�g�p���ł���΃��^�[������A�Ƃ����r���𑽗p����̂Ɍ����Ă��܂��B
 *
 *	@note ���C���X�^���X�݂̂œ��������܂��B
 *	@note ���X���b�h�ł����b�N�ł��܂��i�f�b�h���b�N�ɒ��Ӂj�B
 *
 *	@par�K�v�t�@�C��
 *			TnbSync.h
 *
 *	@date 06/09/20	�V�K�쐬
 */
class CSyncCounter : public ISynchronized, CCopyImpossible
{
public:

	/**
	 * �R���X�g���N�^ 
	 *	@param lMax �i�����ő吔�B�ȗ�����ƂP�ł��B0�ȉ��͎w�肵�Ȃ��ŉ������B
	 */
	CSyncCounter(LONG lMax=1) : m_lCounter(0), m_lMax(lMax) 
	{
	}

	/** �f�X�g���N�^ */
	virtual ~CSyncCounter(void)
	{
		ASSERT0(
			m_lCounter == 0,
			"CSyncCounter::~CSyncCounter()",
			"�������b�N�J�E���g�̐��������܂���B\n\tLock()��Unlock()�񐔂������Ă��Ȃ��\�����L��܂��B"
		);
	}

	/** 
	 * [�r��] ���b�N
	 *	@param dwTime ���b�N�Ƀg���C���������ؕb�B�ȗ�����Ɩ����ɑ҂��܂��B
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const
	{
		if ( dwTime == 0 )
		{
			return m_check();
		}
		else if ( dwTime == INFINITE )
		{
			while ( true )
			{
				if ( m_check() )
				{
					return true;
				}
				::Sleep(1);
			}
		}
		else
		{
			CTickCount tick;
			while ( ! tick.IsPassed(dwTime) )
			{
				if ( m_check() )
				{
					return true;
				}
				::Sleep(1);
			}
		}
		return false;
	}

	/** [�r��] �A�����b�N */
	virtual void Unlock(void) const
	{
		if ( ::InterlockedDecrement(&m_lCounter) < 0 )
		{
			ASSERT0(false, "CSyncCounter::Unlock()", "Unlock()�񐔂�Lock()���������ł��B");
			::InterlockedExchange(&m_lCounter, 0);
		}
	}

private:
	mutable LONG	m_lCounter;	///< ���݂̃J�E���^
	LONG			m_lMax;		///< �i���\��

	/// �i���`�F�b�N
	bool m_check(void) const
	{
		if ( ::InterlockedIncrement(&m_lCounter) <= m_lMax )
		{
			return true;
		}
		::InterlockedDecrement(&m_lCounter);
		return false;
	}
};



/**@ingroup SYNC
 * �r���Ǘ��_�~�[�N���X
 *
 *		���\�b�h�͗p�ӂ���܂����A�����͉������܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbSync.h
 *
 *	@date 06/05/01	�V�K�쐬
 */
class CSyncDummy : public ISynchronized
{
public:

	/** �R���X�g���N�^ */
	CSyncDummy(void) {}

	/** �f�X�g���N�^ */
	virtual ~CSyncDummy(void) {}

	/** 
	 * [�r��] ���b�N
	 *	@param dwTime �_�~�[�B
	 *	@return ��� true�B
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const 
	{
		return true; 
	}

	/** [�r��] �A�����b�N */
	virtual void Unlock(void) const {}
};



/**@ingroup SYNC
 * �ȈՔr������N���X
 *
 *		�R���X�g���N�g���Ƀ��b�N���A�f�X�g���N�g���ɃA�����b�N���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbSync.h
 *
 *	@date 06/01/01	�V�K�쐬
 *	@date 06/04/17	�N���X���ύX�iCSync -> CExclusive�j
 *	@date 06/07/27	Unlock���\�b�h�ǉ�
 *	@date 06/09/13	���ʂ��Ȃ���
 *	@date 06/11/15 �����ُ펞�X���[����悤�ɕύX�B
 */
class CExclusive
{
public:

	/**
	 * �R���X�g���N�^
	 *	@param P ISynchronized�����������N���X�̃|�C���^
	 *	@throw CNullPointerException P �� NULL ���w�肷��ƃX���[����܂��B
	 */
	explicit CExclusive(const ISynchronized* P) : m_psyncObj(P)
	{
		if ( m_psyncObj == NULL )
		{
			throw CNullPointerException();
		}
		m_psyncObj->Lock();
	}

	/** �f�X�g���N�^ */
	~CExclusive(void)
	{
		m_psyncObj->Unlock(); 
	}

private:
	const ISynchronized *	m_psyncObj;	///< �g�p���铯���I�u�W�F�N�g 
};



/**@ingroup SYNC
 * �ȈՔr������}�N��.
 *
 *		CLS�� ISynchronized �h���̃C���X�^���X���w�肷��ƁA���̃X�R�[�v�Ԃ́A
 *		�r������܂��B
 *
 *	@par�g�p��
 *	<table><tr><td><pre>
 *
 *		class CFoo{
 *		  CSyncSection m_syc;
 *		          ;
 *		  void ab(void){
 *		    if ( XX )
 *		    {
 *		      EXCLUSIVE(m_syc); //���̊ԁA�r�������B		
 *		            ;           //�^
 *		    }
 *		  }
 *		};
 *	</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbSync.h
 *
 *	@date 06/04/17	�V�K�쐬
 */
#define EXCLUSIVE(CLS)		CExclusive _cExclus_(CLS);


/**@ingroup SYNC
 * TRACE�t���ȈՔr������}�N��.
 *
 *		CLS�� ISynchronized �h���̃C���X�^���X���w�肷��ƁA���̃X�R�[�v�Ԃ́A
 *		�r������܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbSync.h
 *
 *	@date 06/07/27	�V�K�쐬
 */
#define EXCLUSIVE_(CLS)	\
					TRACE3("%s(%d):[%d] Lock�v��\n",_T(__FILE__),__LINE__,::GetCurrentThreadId());	\
					CExclusive _cExclus_(CLS);														\
					TRACE3("%s(%d):[%d] Lock����\n",_T(__FILE__),__LINE__,::GetCurrentThreadId());



/**@ingroup SYNC
 * �ȈՔr���c�C������}�N��.
 *
 *		CLS1,CLS2 �� ISynchronized �h���̃C���X�^���X���w�肷��ƁA���̃X�R�[�v�Ԃ́A
 *		�r������܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbSync.h
 *
 *	@date 06/07/03	�V�K�쐬
 */
#define EXCLUSIVE2(CLS1,CLS2)	CExclusive _cExclus1_(CLS1); CExclusive _cExclus2_(CLS2);



/**@ingroup SYNC
 * TRACE�t���ȈՔr���c�C������}�N��.
 *
 *		CLS1,CLS2 �� ISynchronized �h���̃C���X�^���X���w�肷��ƁA���̃X�R�[�v�Ԃ́A
 *		�r������܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbSync.h
 *
 *	@date 06/07/27	�V�K�쐬
 */
#define EXCLUSIVE2_(CLS1,CLS2)	\
					TRACE3("%s(%d):[%d] Lock�v��\n",_T(__FILE__),__LINE__,::GetCurrentThreadId());	\
					CExclusive _cExclus1_(CLS1); CExclusive _cExclus2_(CLS2);						\
					TRACE3("%s(%d):[%d] Lock����\n",_T(__FILE__),__LINE__,::GetCurrentThreadId());



/**@ingroup SYNC
 * �ȈՔr������N���X
 *
 *		�C���X�^���X�쐬���A���b�N���A�f�X�g���N�^���A�����b�N���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbSync.h
 *
 *	@date 06/01/01	�V�K�쐬
 *	@date 06/04/17	�N���X���ύX�iCSync -> CExclusive�j
 *	@date 06/07/27	Unlock���\�b�h�ǉ�
 */
class CExclusiveEx : public CExclusive
{
	DEFSUPER(CExclusive); 
public:

	/**
	 * �R���X�g���N�^
	 *	@param P ISynchronized�����������N���X�̃|�C���^
	 *	@throw CNullPointerException P �� NULL ���w�肷��ƃX���[����܂��B
	 */
	explicit CExclusiveEx(const ISynchronized* P) : _super(P), m_iChkCnt(0) 
	{
	}

	/**
	 * �J�E���^�C���N�������g
	 *	@note SYNCBLOCK�p�̊Ǘ����\�b�h�ł��B�ʏ풼�ڎg���܂���B
	 *	@return �J�E���^
	 */
	operator int(void)
	{
		return m_iChkCnt++;
	}

private:
	int			m_iChkCnt;	///< �J�E���^
};



/**@ingroup SYNC
 * �ȈՔr���u���b�N����}�N��.
 *
 *		CLS�� ISynchronized �h���̃C���X�^���X���w�肷��ƁA���̌�ɑ����u���b�N�́A
 *		�r������܂��B
 *
 *	@par�g�p��
 *	<table><tr><td><pre>
 *
 *		class CFoo{
 *		  CSyncSection m_syc;
 *		          ;
 *		  void ab(void){
 *		    int i=0;
 *		    SYNCBLOCK(syc)
 *		    {                //���̊ԁA�r�������B		
 *		            ;        //�^
 *		    }
 *		    i=3;
 *		  }
 *		};
 *	</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbSync.h
 *
 *	@date 06/05/01	�V�K�쐬
 */
#define SYNCBLOCK(CLS) if (false); else for ( CExclusiveEx _cExclusEx_(CLS); _cExclusEx_ == 0; )



}; // TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")
