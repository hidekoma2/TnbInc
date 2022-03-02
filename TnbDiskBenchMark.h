#pragma once
/**
 *@file
 * �f�B�X�N�x���`�}�[�N�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbTemporaryFile.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * �x���`�}�[�N�N���X
 *
 *	@todo �쐬���B
 *
 *	@par�K�v�t�@�C��
 *			TnbDiskBenchMark.h
 *
 *	@date 08/03/26 �V�K�쐬
 */
class CDiskBenchMark : CCopyImpossible
{
	CTemporaryFile	m_tempFile;	///> �e���|�����t�@�C��

public:

	/// �R���X�g���N�^
	CDiskBenchMark(void)
	{
	}

	/**
	 * [�ݒ�] �^�[�Q�b�g�ݒ�
	 *	@param drv �h���C�u 'A'�`'Z'
	 *	@param size �e�X�g�p�t�@�C���T�C�Y
	 *	@retval 0 ����
	 *	@retval 0�ȊO ���s
	 */
	DWORD SetTarget(TCHAR drv, size_t size)
	{
		try
		{
			CStr s;
			s.Format(_T("%c:"), drv);
			m_tempFile.Free();
			m_tempFile.SetTempraryFolder(s);
			m_tempFile.Create(size);
		}
		catch ( CIoFailureException& e )
		{
			return e.GetParam();
		}
		return 0;
	}

	/**
	 * [����] �Ǎ��݃e�X�g
	 *	@param time ���{����(ms)�B1ms�ȏ���w�肵�܂��B
	 *	@param one ��p�P�b�g�T�C�Y�B
	 *	@retval DWORD_MAX �G���[�B
	 *	@retval ��L�ȊO �ǂݍ��ݑ��x(byte/second)�B
	 */
	DWORD ReadTest(DWORD time, size_t one = 1024) const
	{
		if ( time == 0 ) { return DWORD_MAX; }
		if ( ! m_tempFile.HasThat() )
		{
			return DWORD_MAX;
		}
		CStr file = m_tempFile.GetName();
		HANDLE h = ::CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if ( h == INVALID_HANDLE_VALUE )
		{
			return DWORD_MAX;
		}
		ULONGLONG total = 0;
		DWORD startTick = ::GetTickCount();
		DWORD dwRead;
		CWorkMem work(one);
		BYTE* B = work.Ref();
		while ( true )
		{
			dwRead = 0;
			if ( ! ::ReadFile(h, B, ToDword(one), &dwRead, NULL) )
			{
				total = DWORD_MAX;
				break;
			}
			DWORD nowTick = ::GetTickCount();
			DWORD d = nowTick - startTick;
			total += dwRead;
			if ( d >= time )
			{
//				TRACE2("read %dKB / %dms\n", (DWORD)(total / 1024), d);
				total *= 1000;
				total /= d;
				break;
			}
			if ( dwRead != one )
			{
				::SetFilePointer(h, 0, NULL, FILE_BEGIN);
			}
		}
		::CloseHandle(h);
		return static_cast<DWORD>(total);
	}






private :
	friend class CDiskBenchMarkTest;
};



}; // TNB
