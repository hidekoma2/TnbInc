#pragma once
/**
 *@file
 * �����_�t�����l��p����Edit�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcEditValue.h"
#include <math.h>
#include <wchar.h>



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �����v�J�[�\��.
 *
 *		MFC �� CWaitCursor ���g���������̂ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcWaitCursor.h
 * 
 *	@date 14/11/19	�V�K�쐬
 */
class CWaitCursorEx : CCopyImpossible
{
public:

	/**
	 * �R���X�g���N�^.
	 *		�����v�J�[�\�������܂��B
	 */
	CWaitCursorEx(void) : m_pCursor(new CWaitCursor)
	{
	}

	/**
	 * �f�X�g���N�^.
	 *		�ʏ�̃J�[�\���ɖ߂��܂��B
	 */
	~CWaitCursorEx(void)
	{
		Restore();
	}

	/**
	 * [�ݒ�] �ύX.
	 *		�����v�J�[�\�������܂��B
	 */
	void Change(void)
	{
		Restore();
		m_pCursor = new CWaitCursor();
	}

	/**
	 * [�ݒ�] ����.
	 *		�ʏ�̃J�[�\���ɖ߂��܂��B
	 */
	void Restore(void)
	{
		if ( m_pCursor != NULL )
		{
			delete m_pCursor;
			m_pCursor = NULL;
		}
	}

private:
	CWaitCursor*	m_pCursor;	///< �C���X�^���X
};



};
};
