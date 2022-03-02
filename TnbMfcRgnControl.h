#pragma once
/**
 *@file
 * �̈�w��R���g���[���N���X�֌W�̃w�b�_
 *
 *		�l�p�ȊO�̃R���g���[�����쐬���邽�߂̃T�|�[�g�N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �̈�w��R���g���[���T�|�[�g�N���X
 *
 *		inline �ȃR���g���[���ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcRgnControl.h
 * 
 *	@date 07/12/12 ���ۉ����ĐV�K�쐬
 */
template<typename WND = CWnd>
class CRgnControlAddinT : public WND
{
	DEFSUPER(WND);
protected:

	HRGN			m_clientRgn;		///< �N���C�A���g���[�W����

	/**
	 * [�폜] �N���C�A���g���[�W�����j��
	 */
	void DeleteClientRgnHandle(void)
	{
		::SetWindowRgn(m_hWnd, NULL, FALSE);
		if ( m_clientRgn != NULL )
		{
			_DeleteObject(m_clientRgn);
			m_clientRgn = NULL;
		}
	}

	/**
	 * [�ʒm] for processing Windows messages.
	 *		���b�Z�[�W��M������R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *	@param message ���b�Z�[�W
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return ���U���g�B
	 */	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch ( message )
		{
		case WM_PRINT:
			{
				HDC dc = reinterpret_cast<HDC>(wParam);
				if ( m_clientRgn != NULL )
				{
					HRGN rgn = ::CreateRectRgn(0, 0, 0, 0);
					CPoint po = MFCLIB::GetClientPoint(this);
					::CombineRgn(rgn, m_clientRgn, NULL, RGN_COPY);
					::OffsetRgn(rgn, po.x, po.y);
					::SelectClipRgn(dc, rgn);
					LRESULT r = _super::WindowProc(message, wParam, lParam);
					_DeleteObject(rgn);
					return r;
				}
			}
			break;
#ifndef _WIN32_WCE
		case WM_NCPAINT:
			if ( m_clientRgn != NULL )
			{
				wParam = reinterpret_cast<WPARAM>(m_clientRgn);
			}
			break;
#endif
		case WM_DESTROY:
			DeleteClientRgnHandle();
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

public:

	/// �R���X�g���N�^
	CRgnControlAddinT(void) : m_clientRgn(NULL)
	{
	}

	/// �f�X�g���N�^
	virtual ~CRgnControlAddinT(void)
	{
		DeleteClientRgnHandle();
	}

	/**
	 * [�ݒ�] �E�B���h�E���[�W�����ݒ�.
	 *	@param rgn ���[�W�����B
	 *	@retval true �����B�w�肵�� rgn �͖{�I�u�W�F�N�g���Ǘ�����̂ŁA�j�����Ȃ��ł��������i�s�K�v�ɂȂ�����j�����܂��j�B
	 *	@retval false ���s�B�w�肵�� rgn �͗L���Ȃ̂ŁA DeleteObject ����K�v������܂��B
	 */
	bool SetClientRgn(HRGN rgn)
	{
		DeleteClientRgnHandle();
		m_clientRgn = ::CreateRectRgn(0, 0, 0, 0);
		::CombineRgn(m_clientRgn, rgn, NULL, RGN_COPY);
		bool r = !! ::SetWindowRgn(m_hWnd, rgn, TRUE);
		if ( ! r )
		{
			DeleteClientRgnHandle();
		}
		return r;
	}
};



}; //MFC
}; //TNB



