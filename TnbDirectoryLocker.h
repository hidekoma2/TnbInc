#pragma once
/**
 *@file
 * �t�H���_���b�N�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbFileLocker.h"
#include "TnbSimpleVector.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * �t�H���_���b�N�Ǘ��N���X
 *
 *	�{�N���X�́A�C���X�^���X��ɕt��������A�f�B���N�g����Lock���邱�Ƃ��o���܂��B
 *
 *	@todo �������B
 *
 *	@par�K�v�t�@�C��
 *			TnbDirectoryLocker.h
 *
 *	@date 08/03/26 �V�K�쐬
 */
class CDirectoryLocker : CCopyImpossible
{
	typedef CPointerHandleT<CFileLocker> CFileLockerHandle;
	CSimpleVectorT<CFileLockerHandle> m_v;

public:

	/// �R���X�g���N�^
	CDirectoryLocker(void)
	{
	}

	/// �f�X�g���N�^
	~CDirectoryLocker(void)
	{
		Unlock();
	}

	/** 
	 * [�r��] ���b�N
	 *	@param lpszDir ���b�N����f�B���N�g���B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Lock(LPCTSTR lpszDir)
	{
		Unlock();




	}

	/**
	 * [�r��] �A�����b�N
	 *	@see Lock
	 */
	void Unlock(void)
	{
		m_v.RemoveAll();
	}
};



}; // TNB
