#pragma once
/**
 *@file
 * �̈�q�b�g�e�X�g�N���X�֌W�̃w�b�_
 *
 *		�̈�̃q�b�g�e�X�g���Ǘ�����N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"
#include "TnbSimpleVector.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �̈�q�b�g�e�X�g�T�|�[�g�N���X
 *
 *		�ʏ�A�R���g���[���ɃA�h�C�����Ďg�p���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcHitTest.h
 * 
 *	@date 09/04/13 �V�K�쐬�B
 */
template<typename WND = CWnd>
class CHitTestAddinT : public WND
{
	DEFSUPER(WND);
public:
	/// �R���X�g���N�^
	CHitTestAddinT(void) : _super()
	{
	}

	/// �f�X�g���N�^
	virtual ~CHitTestAddinT(void)
	{
		DeleteAllAreas();
	}

	/**
	 * [�ǉ�] �G���A�ݒ�.
	 *	@param id ID�B
	 *	@param rgn ���[�W�����B
	 *	@retval false ���s�B
	 *	@retval true �����B
	 */
	bool AddArea(INDEX id, HRGN rgn)
	{
		HRGN h = ::CreateRectRgn(0, 0, 0, 0);
		if ( ::CombineRgn(h, rgn, NULL, RGN_COPY) == ERROR )
		{
			return false;
		}
		TParam p = { id, h };
		return m_areas.Add(p) != INVALID_INDEX;
	}

	/**
	 * [�ǉ�] �G���A�ݒ�.
	 *	@param id ID�B
	 *	@param rc �͈́B
	 *	@retval false ���s�B
	 *	@retval true �����B
	 */
	bool AddArea(INDEX id, const RECT& rc)
	{
		HRGN h = ::CreateRectRgnIndirect(&rc);
		if ( h == NULL )
		{
			return false;
		}
		TParam p = { id, h };
		return m_areas.Add(p) != INVALID_INDEX;
	}

	/**
	 * [�폜] �S�G���A���j��
	 */
	void DeleteAllAreas(void)
	{
		loop ( i, m_areas.GetSize() )
		{
			_DeleteObject(m_areas[i].hRgn);
		}
		m_areas.RemoveAll();
	}

	/**
	 * [�m�F] �q�b�g�e�X�g
	 *	@param po �|�C���g
	 *	@retval INVALID_INDEX �ǂ̃G���A�ɂ��܂܂�Ă��Ȃ�����
	 *	@retval ��L�ȊO �q�b�g�����G���A�̃C���f�b�N�X
	 */
	INDEX HitTest(const POINT& po) const
	{
		loop ( i, m_areas.GetSize() )
		{
			if ( ::PtInRegion(m_areas[i].hRgn, po.x, po.y) )
			{
				return m_areas[i].id;
			}
		}
		return INVALID_INDEX;
	}

protected:
	/// �Ǘ��p�����[�^
	struct TParam
	{
		INDEX	id;		///< ID
		HRGN	hRgn;	///< ���[�W����
	};
	CSimpleVectorT<TParam> m_areas;		///< �G���A�Ǘ�
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
		case WM_GETDLGCODE:
			break;
		default:
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}
	/**
	 * [�m�F] �q�b�g�e�X�g.
	 *	@param point �e�X�g����N���C�A���g���W
	 *	@param pTI NULL���w�肵�Ă����Ă��������B
	 *	@retval -1 �q�b�g�����B
	 *	@retval 0�ȏ� �q���g�����B
	 */
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const
	{
		INDEX idx = HitTest(point);
		if ( idx != INVALID_INDEX )
		{
			return idx;
		}
		return -1;
	}
};



}; //MFC
}; //TNB



