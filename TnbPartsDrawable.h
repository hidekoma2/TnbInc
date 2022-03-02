#pragma once
/**
 *@file
 * �p�[�c�`����̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"



//TNB Library
namespace TNB {
namespace MFC {



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 #define DFCS_FOCUS			0x10000
#endif



/**@ingroup WINAPI
 * WINDOW�p�[�c�`��C���^�[�t�F�[�X
 *
 *	�E�B���h�E�̃^�C�g���o�[�A�t���[���R���g���[���A�t�H�[�J�X�g�������C���^�[�t�F�[�X�������܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbPartsDrawable.h
 * 
 *	@date 07/10/24 �V�K�쐬
 */
struct IPartsDrawable
{
	/// �f�X�g���N�^
	virtual ~IPartsDrawable(void) { }

	/**
	 * [�`��] �����`�̕ӂ�`��.
	 *	@param hdc �f�o�C�X�R���e�L�X�g�̃n���h�����w�肵�܂��B 
	 *	@param lprc �����`�̘_�����W��ێ����� 1 �� RECT �\���̂ւ̃|�C���^���w�肵�܂��B
	 *	@param edge �ӂ̓����ƊO���̕`����@���w�肵�܂��B�����̋��E�t���O���� 1 ���A�O���̋��E�t���O������� 1 ��I�сA�g�ݍ��킹�Ďw�肵�܂��B
	 *	@param grfFlags ���E�̎�ނ��w�肵�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B ::GetLastError() �ŃG���[�����擾�ł��܂��B
	 */
	virtual bool DrawEdge(HDC hdc, LPCRECT lprc, UINT edge, UINT grfFlags) const = 0;

	/**
	 * [�`��] �{�^���R���g���[���`��.
	 *		�w�肳�ꂽ�^�C�v�ƃX�^�C���������A�{�^���R���g���[����`�悵�܂��B
	 *	@param[in] hdc		�R���g���[���̕`��Ɏg�������f�o�C�X�R���e�L�X�g�̃n���h�����w�肵�܂��B 
	 *						NULL ���w�肷��ƁA���ۂɕ`�悷��傫���� lprc �ɕԂ��܂��B
	 *	@param[in,out] _lprc	�����`�̘_�����W��ێ����� 1 �� RECT �\���̂ւ̃|�C���^���w�肵�܂��B 
	 *	@param[in] uState	�{�^���R���g���[���̏�Ԃ��w�肵�܂��B\n
	 *			�ȉ��̒l���w�肵�܂��B
	 *			<table><tr><td>
	 *			�l									</td><td>
	 *					�Ӗ�						</td></tr><tr><td>
	 *			DFCS_BUTTON3STATE						</td><td>
	 *					3 �X�e�[�g�{�^���i�I���A�O���C�\���A�I�t�j  </td></tr><tr><td>
	 *			DFCS_BUTTONCHECK						</td><td>
	 *					�`�F�b�N�{�b�N�X </td></tr><tr><td>
	 *			DFCS_BUTTONPUSH						</td><td>
	 *					�v�b�V���{�^�� </td></tr><tr><td>
	 *			DFCS_BUTTONRADIO						</td><td>
	 *					�I�v�V�����{�^�� </td></tr><tr><td>
	 *			DFCS_BUTTONRADIOIMAGE						</td><td>
	 *					�I�v�V�����{�^���̃C���[�W�i�����`�Ƃ͈قȂ�C���[�W�j </td></tr><tr><td>
	 *			DFCS_BUTTONRADIOMASK						</td><td>
	 *					�I�v�V�����{�^���̃}�X�N�i�����`�Ƃ͈قȂ�}�X�N�j </td></tr>
	 *			</table>
	 *			����ɁA�R���g���[���̏�Ԃ�ݒ肷�邽�߂ɁA���̒l�� 1 �܂��͕������w�肷�邱�Ƃ��ł��܂��B 		
	 *			<table><tr><td>
	 *			�l									</td><td>
	 *					�Ӗ�						</td></tr><tr><td>
	 *			DFCS_CHECKED						</td><td>
	 *					�`�F�b�N����Ă���{�^����`�悵�܂��B </td></tr><tr><td>
	 *			DFCS_FLAT							</td><td>
	 *					�i�ʉ��̕\���̂Ȃ��j����ȃf�U�C���̃{�^����`�悵�܂��B</td></tr><tr><td>
	 *			DFCS_HOT							</td><td>
	 *					�{�^�����z�b�g�g���b�L���O�\�����܂��i�}�E�X��L�[�{�[�h�̑���ɉ����āA�{�^����ω������܂��j�B </td></tr><tr><td>
	 *			DFCS_INACTIVE						</td><td>
	 *					�����ȁi�W�F�\���́j�{�^����`�悵�܂��B </td></tr><tr><td>
	 *			DFCS_MONO							</td><td>
	 *					���m�N���[���̋��E�����{�^����`�悵�܂��B </td></tr><tr><td>
	 *			DFCS_PUSHED							</td><td>
	 *					�����ꂽ��Ԃ̃{�^����`�悵�܂��B </td></tr><tr><td>
	 *			DFCS_FOCUS							</td><td>
	 *					�t�H�[�J�X�̂����Ԃ̃{�^����`�悵�܂��i��WIN32API�ł͂Ȃ��V���{���ł��j�B</td></tr><tr><td>
	 *			DFCS_TRANSPARENT					</td><td>
	 *					�w�i�����̂܂܂ɂ��܂��i���ߕ\���j�B </td></tr>
	 *			</table>
	 *	@retval true �����B
	 *	@retval false ���s�B ::GetLastError() �ŃG���[�����擾�ł��܂��B
	 */
	virtual bool DrawButtonControl(HDC hdc, LPRECT _lprc, UINT uState) const = 0;
	
	/**
	 * [�`��] �t�H�[�J�X�g�`��.
	 *		�t�H�[�J�X�����݂��邱�Ƃ����[�U�[�Ɏ������߂Ɏg���钷���`�̘g��`�悵�܂��B
	 *	@param hdc		�f�o�C�X�R���e�L�X�g�̃n���h�����w�肵�܂��B 
	 *	@param lpRect		�����`�̘_�����W��ێ����� 1 �� RECT �\���̂ւ̃|�C���^���w�肵�܂��B 
	 *	@retval true �����B
	 *	@retval false ���s�B ::GetLastError() �ŃG���[�����擾�ł��܂��B
	 */
	virtual bool DrawFocusRect(HDC hdc, LPCRECT lpRect) const = 0;

	/**
	 * [�擾] ������Ԃ̂���.
	 *		�����������ɁA�R���g���[���̏�ɕ\��������e�����炷�����𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _po ����
	 */
	virtual void GetPushedOffset(POINT& _po) const = 0;

	/**
	 * [�擾] �G�b�W�̕��A����.
	 *		�G�b�W�̕��A������Ԃ��܂��B
	 *	@param[out] _po ���A����
	 */
	virtual void GetEdgeOffset(POINT& _po) const = 0;
};



/**@ingroup WINAPI
 * �W��WINDOW�p�[�c�`��N���X
 *
 *	WINAPI �� ::DrawFrameControl(), ::DrawFocusRect() �����b�v���Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbPartsDrawable.h
 * 
 *	@date 07/11/23 �V�K�쐬
 */
class CStandardPartsDrawer : public IPartsDrawable 
{
	DEFSUPER(IPartsDrawable);
public:
	/**
	 * [�`��] �����`�̕ӂ�`��.
	 *	@param hdc �f�o�C�X�R���e�L�X�g�̃n���h�����w�肵�܂��B 
	 *	@param lprc �����`�̘_�����W��ێ����� 1 �� RECT �\���̂ւ̃|�C���^���w�肵�܂��B
	 *	@param edge �ӂ̓����ƊO���̕`����@���w�肵�܂��B�����̋��E�t���O���� 1 ���A�O���̋��E�t���O������� 1 ��I�сA�g�ݍ��킹�Ďw�肵�܂��B
	 *	@param grfFlags ���E�̎�ނ��w�肵�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B ::GetLastError() �ŃG���[�����擾�ł��܂��B
	 */
	virtual bool DrawEdge(HDC hdc, LPCRECT lprc, UINT edge, UINT grfFlags) const
	{
		RECT rc = *lprc;
		return !! ::DrawEdge(hdc, &rc, edge, grfFlags);
	}

	/**
	 * [�`��] �{�^���R���g���[���`��.
	 *		�w�肳�ꂽ�^�C�v�ƃX�^�C���������A�{�^���R���g���[����`�悵�܂��B
	 *	@param[in] hdc		�R���g���[���̕`��Ɏg�������f�o�C�X�R���e�L�X�g�̃n���h�����w�肵�܂��B 
	 *						NULL ���w�肷��ƁA���ۂɕ`�悷��傫���� lprc �ɕԂ��܂��B
	 *	@param[in,out] _lprc	�����`�̘_�����W��ێ����� 1 �� RECT �\���̂ւ̃|�C���^���w�肵�܂��B 
	 *	@param[in] uState	�{�^���R���g���[���̏�Ԃ��w�肵�܂��B IPartsDrawable::DrawButtonControl() �Q�ƁB
	 *	@note DFCS_FOCUS �͖�������܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B ::GetLastError() �ŃG���[�����擾�ł��܂��B
	 */
	virtual bool DrawButtonControl(HDC hdc, LPRECT _lprc, UINT uState) const
	{
		switch ( uState & 0xFF )
		{
		case DFCS_BUTTON3STATE:
		case DFCS_BUTTONCHECK:
		case DFCS_BUTTONRADIO:
			_lprc->right = _lprc->left + 13;
			_lprc->bottom = _lprc->top + 13;
			break;
		}
		if ( hdc == NULL ) { return true; }
		return !! ::DrawFrameControl(hdc, _lprc, DFC_BUTTON, uState);
	}
	
	/**
	 * [�`��] �t�H�[�J�X�g�`��.
	 *		�t�H�[�J�X�����݂��邱�Ƃ����[�U�[�Ɏ������߂Ɏg���钷���`�̘g��`�悵�܂��B
	 *	@param hdc		�f�o�C�X�R���e�L�X�g�̃n���h�����w�肵�܂��B 
	 *	@param lpRect		�����`�̘_�����W��ێ����� 1 �� RECT �\���̂ւ̃|�C���^���w�肵�܂��B 
	 *	@retval true �����B
	 *	@retval false ���s�B ::GetLastError() �ŃG���[�����擾�ł��܂��B
	 */
	virtual bool DrawFocusRect(HDC hdc, LPCRECT lpRect) const
	{
		return !! ::DrawFocusRect(hdc, lpRect);
	}

	/**
	 * [�擾] ������Ԃ̂���.
	 *		�����������ɁA�R���g���[���̏�ɕ\��������e�����炷�����𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _po ����
	 */
	virtual void GetPushedOffset(POINT& _po) const
	{
		_po.x = 1;
		_po.y = 1;
	}

	/**
	 * [�擾] �G�b�W�̕��A����.
	 *		�G�b�W�̕��A������Ԃ��܂��B
	 *	@param[out] _po ���A����
	 */
	virtual void GetEdgeOffset(POINT& _po) const
	{
		_po.x = ::GetSystemMetrics(SM_CXEDGE);
		_po.y = ::GetSystemMetrics(SM_CYEDGE);
	}
};



}; //MFC
}; //TNB



