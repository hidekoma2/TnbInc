#pragma once
/**
 *@file
 * �J�X�^���`�撊�ۃR���g���[���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �J�X�^���`�撊�ۃR���g���[��
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcAbstractCustomDraw.h
 * 
 *	@date 08/09/11	�V�K�쐬
 */
class CAbstractCustomDraw
{
protected:

	/// �\���p�p�����[�^
	struct TDrawParam
	{
		HDC			dc;			///< �f�o�C�X�R���e�L�X�g
		CRect		rect;		///< �`��͈�
		DWORD_PTR	itemNo;		///< �A�C�e���i���o�[
		DWORD		subItemNo;	///< �T�u�A�C�e���i���o�[
		UINT		state;	/**< ���.
							 *	�ȉ��̒l�� OR ���ꂽ��ԂŐݒ肳��Ă��܂��B\n
							 *			<table><tr><td>
							 *			�l											</td><td>
							 *					����								</td></tr><tr><td>
							 *			CDIS_CHECKED								</td><td>
							 *					�A�C�e���̓`�F�b�N����Ă��܂��B	</td></tr><tr><td>
							 *			CDIS_DEFAULT								</td><td>
							 *					�A�C�e���̓f�t�H���g��Ԃł��B		</td></tr><tr><td>
							 *			CDIS_DISABLED								</td><td>
							 *					�A�C�e���͎g�p�s�ɂȂ��Ă��܂��B	</td></tr><tr><td>
							 *			CDIS_FOCUS									</td><td>
							 *					�A�C�e���̓t�H�[�J�X����Ă��܂��B	</td></tr><tr><td>
							 *			CDIS_GRAYED									</td><td>
							 *					�A�C�e���͊D�F�\���ɂȂ��Ă��܂��B	</td></tr><tr><td>
							 *			CDIS_HOT									</td><td>
							 *					�A�C�e����hot��ԂɂȂ��Ă��܂��B	</td></tr><tr><td>
							 *			CDIS_INDETERMINATE							</td><td>
							 *					�A�C�e����interminate��ԂɂȂ��Ă��܂��B</td></tr><tr><td>
							 *			CDIS_MARKED									</td><td>
							 *					�A�C�e���̓}�[�N����Ă��܂��B		</td></tr><tr><td>
							 *			CDIS_SELECTED								</td><td>
							 *					�A�C�e���͑I������Ă��܂��B		</td></tr>*/
		COLORREF	textColor;	///< �e�L�X�g�J���[
		COLORREF	backColor;	///< �o�b�N�J���[
	};

	/**
	 * [�ʒm] �`��J�n.
	 *		��A�̕`��̊J�n��ʒm���܂��B
	 *	@note dc �͈ȉ��A OnItemPainting() OnItemPainted() OnEndItemPaint() �Ƒ�����A�̕`��ŋ��ʂ�
	 *			�g���f�o�C�X�R���e�L�X�g�ł��B OnItemPainting() �ȂǂŃf�o�C�X�R���e�L�X�g��
	 *			��ԕύX���Ė߂��^�C�~���O���Ȃ��ꍇ�A�{���\�b�h�ŋL�����A OnEndItemPaint()
	 *			�Ō��ɖ߂��悤�ɂ��܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g�B
	 */
	virtual void OnBeginItemPaint(HDC dc) = 0;

	/**
	 * [�ʒm] �`��I��.
	 *		��A�̕`��̏I����ʒm���܂��B
	 *	@see OnBeginItemPaint()
	 *	@param dc �f�o�C�X�R���e�L�X�g�B
	 */
	virtual void OnEndItemPaint(HDC dc) = 0;

	/**
	 * [�ʒm] �A�C�e���`��.
	 *	@note �T�u�A�C�e�����ɒʒm����܂��B
	 *	@param[in,out] pParam �`��p���B�͈͂�A�C�e��NO�������Ă��܂��B
	 *	@retval CDRF_NOTIFYITEMDRAW �ʏ�̕`����s��(�{���\�b�h�ł͉������Ă��Ȃ�)�B
	 *	@retval CDRF_NEWFONT �t�H���g�╶���F��ύX�������Ԃ��܂��B
	 *	@retval CDRF_SKIPDEFAULT �{���\�b�h�ŕ`�悸�݁B
	 */
	virtual LRESULT OnItemPainting(TDrawParam* pParam) = 0;

	/**
	 * [�ʒm] �A�C�e���`��I���.
	 *	@note �T�u�A�C�e���`�悪�I��������ƁA�A�C�e�����ɒʒm����܂��B
	 *	@param[in] pParam �`��p���B rect �́A�T�u�A�C�e�����܂ޔ͈́B
	 */
	virtual void OnItemPainted(const TDrawParam* pParam) = 0;

	/**
	 * [�m�F] �T�u�A�C�e���T�O�̗L��
	 *	@retval ture �T�u�A�C�e������B
	 *	@retval false �Ȃ��B
	 */
	virtual bool HasSubItem(void) const = 0;

	/**
	 * [�m�F] �A�C�e��RECT�v�Z
	 *	@note _pa.itemNo, _pa.subItemNo �Ȃǂ���A _pa.rect �̐ݒ���s���Ă��������B
	 *	@note _pa.state �̒������\�ł��B
	 *	@param[in,out] _pa �`����
	 *	@param[in] isSub true �Ȃ� sub�A�C�e���p�̌v�Z������B
	 */
	virtual void OnCalcItemRect(TDrawParam& _pa, bool isSub) = 0;

	/**
	 * [�ʒm] for notifications from parent
	 *	@note ����(hwndFrom)���m�F�̏�T�u�N���X�� OnChildNotify ���\�b�h����R�[�����Ă��������B
	 *	@param[in] message ���b�Z�[�W
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@param[out] _pResult ���U���g
	 *	@retval TRUE	�����ρB�T�u�N���X�� OnCildNotify �N���X�͂��̂܂� TRUE �Ń��^�[�����Ă��������B
	 *	@retval FALSE	�������B
	 */
	BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* _pResult)
	{
		if ( message == WM_NOTIFY )
		{
			LPNMHDR lpNmHdr = reinterpret_cast<LPNMHDR>(lParam);
			if ( lpNmHdr->code == NM_CUSTOMDRAW ) 
			{
				LPNMLVCUSTOMDRAW lpLvCd = reinterpret_cast<LPNMLVCUSTOMDRAW>(lParam);
				*_pResult = m_OnCustomDraw(lpLvCd);
				return TRUE;
			}
		} 
		return FALSE;
	}

private:

	/// Item�`��T�u
	LRESULT m_OnItemPaintSub(LPNMLVCUSTOMDRAW P, int subItemNo)
	{
		TDrawParam pa;
		pa.dc			= P->nmcd.hdc;
		pa.itemNo		= P->nmcd.dwItemSpec;
		pa.subItemNo	= subItemNo;
		pa.state		= P->nmcd.uItemState;
		pa.textColor	= P->clrText;
		pa.backColor	= P->clrTextBk;
		pa.rect			= P->nmcd.rc;
		OnCalcItemRect(pa, true);
		LRESULT r = OnItemPainting(&pa);
		P->clrText = pa.textColor;
		P->clrTextBk = pa.backColor;
		return r;
	}

	/// �J�X�^���h���[�ʒm�Ή�
	LRESULT m_OnCustomDraw(LPNMLVCUSTOMDRAW P)
	{
		switch ( P->nmcd.dwDrawStage )
		{
		case CDDS_PREPAINT:
			OnBeginItemPaint(P->nmcd.hdc);
			return CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
		case CDDS_POSTPAINT:
			OnEndItemPaint(P->nmcd.hdc) ;
			return CDRF_DODEFAULT;
		case CDDS_ITEMPOSTPAINT:
			{
				TDrawParam pa;
				pa.dc			= P->nmcd.hdc;
				pa.itemNo		= P->nmcd.dwItemSpec;
				pa.subItemNo	= 0;
				pa.state		= P->nmcd.uItemState;
				pa.textColor	= P->clrText;
				pa.backColor	= P->clrTextBk;
				pa.rect			= P->nmcd.rc;
				OnCalcItemRect(pa, false);
				OnItemPainted(&pa);
			}
			break;
		case CDDS_ITEMPREPAINT:
			if ( ! HasSubItem() )
			{
				return m_OnItemPaintSub(P, 0) | CDRF_NOTIFYPOSTPAINT;
			}
			return CDRF_NOTIFYSUBITEMDRAW | CDRF_NOTIFYPOSTPAINT;
		case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
			return m_OnItemPaintSub(P, P->iSubItem);
		}
		return CDRF_DODEFAULT;
	}

public:

	/// �R���X�g���N�^
	CAbstractCustomDraw(void)
	{
	}
};



}; // MFC
}; // TNB
