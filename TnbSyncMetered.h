#pragma once
/**
 *@file
 * ���������֌W�̃w�b�_
 *
 *		MeteredSection���g������������������N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSync.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
	namespace NMetered
	{
		//Mr.Dan Chou �̔r��Obj
		#include "other/MeteredSection.h"
	};
#endif



/**@ingroup SYNC
 * Metered�r���Ǘ��N���X
 *
 *		MeteredSection���g�����������������܂��B
 *
 *	@note �ʃv���Z�X�ł��ʃC���X�^���X�ł��������O�Ȃ瓯�������܂��B
 *	@note ���X���b�h�ł̓��b�N�ł��܂��i�f�b�h���b�N�ɒ��Ӂj�B
 *	@note �J�E���^���w��ł��܂��B
 *
 *	@see http://msdn.microsoft.com/library/en-us/dndllpro/html/msdn_metrsect.asp
 *
 *	@par�K�v�t�@�C�� 
 *			TnbSyncMetered.h
 *
 *	@date 06/04/14	�V�K�쐬
 *	@date 06/11/15 �쐬���s������ �X���[����悤�ɕύX�B
 */
class CSyncMetered : public ISynchronized, CCopyImpossible
{
	NMetered::LPMETERED_SECTION	m_pms;		///< �r���n���h��
	mutable LONG	m_lLockCount;			///< �{�C���X�^���X�ł̃��b�N��
public:

	/** 
	 * �R���X�g���N�^
	 *	@param lpszName �����p�̖��O���w�肵�܂��B
	 *					�ȗ�����ƕʃC���X�^���X���m�̓����͎��܂���B
	 *	@param iMaximumCount �����Ɏ��s�ł��鐔���w�肵�܂��B�ȗ�����ƂP�ł��B
	 *					�������O�����łɍ쐬����Ă���ꍇ�A���̒l�͖�������܂��B
	 *	@throw CInvalidParamException ���O�s���ȂǁA�����I�u�W�F�N�g���쐬�ł��Ȃ��������A�X���[����܂��B
	 */
	explicit CSyncMetered(LPCTSTR lpszName = NULL, int iMaximumCount = 1)
	{
		ASSERTLIB( lpszName == NULL || STRLIB::GetLen(lpszName) < MAX_METSECT_NAMELEN );
		m_pms = NMetered::CreateMeteredSection(iMaximumCount, iMaximumCount, lpszName);
		m_lLockCount = 0;
		ASSERT0(
			m_pms != NULL,
			"CSyncMetered::CSyncMetered()",
			"MeteredSection���������ł��܂��񂵂��B���O�ɖ�肪����\�����L��܂��B"
		);
		if ( m_pms == NULL )
		{
			throw CInvalidParamException();
		}
	}

	/** 
	 * �f�X�g���N�^
	 *	@note ���b�N���������A�����b�N���Ă܂��B
	 */
	virtual ~CSyncMetered(void)
	{
		if ( m_pms != NULL )
		{
			if ( m_lLockCount > 0 )
			{
				NMetered::LeaveMeteredSection(m_pms, m_lLockCount, NULL);
			}
			NMetered::CloseMeteredSection(m_pms);
			m_pms = NULL;
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
		if ( m_pms != NULL )
		{
			DWORD dwRc = NMetered::EnterMeteredSection(m_pms, dwTime);
			if ( dwRc == WAIT_OBJECT_0 )
			{
				::InterlockedIncrement(&m_lLockCount);
				return true;
			}
		}
		return false;
	}

	/** [�r��] �A�����b�N */
	virtual void Unlock(void) const
	{
		if ( m_pms != NULL && m_lLockCount > 0 )
		{
			NMetered::LeaveMeteredSection(m_pms, 1, NULL);
			::InterlockedDecrement(&m_lLockCount);
		}
	}
};



}; // TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")
