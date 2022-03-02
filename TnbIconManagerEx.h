#pragma once
/**
 *@file
 * �r�b�g�}�b�v�A�C�R���Ǘ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbBitmapImage.h"
#include "TnbIconManager.h"



//TNB Library
namespace TNB
{



/**@ingroup ICON BITMAP
 * �g���A�C�R���Ǘ��N���X
 *
 *		�{�N���X���g�p���ăA�C�R�������[�h����ƁA�J�����ׂ��A�C�R���̊J���R���h���܂��B
 *		�܂��A���\�[�XID�ƃn���h�����֘A�t���ċL�����Ă������߁A�L���b�V���I�Ȏg�������\�ł��B
 *
 *		�J�[�\�����A�C�R���Ɠ��l�ɖ{�N���X�ŊǗ����邱�Ƃ��\�ł��B
 *
 *		�{�N���X�́A�r�b�g�}�b�v����A�C�R���쐬�i CreateIcon() �j�A�J�[�\���쐬�i CreateCursor() �j�������Ă���ȊO�A 
 *		CIconManager �Ƃ܂����������ł��B
 *
 * @note	�{�N���X�̃C���X�^���X�Ŏ擾���� �A�C�R���n���h���́A�g�p��A API�� DestroyIcon() ���g�p�����A
 *			Destroy() ���g�p���j�����܂�(�C���X�^���X���폜���Ă��n���h���͔j������܂�) �B
 *
 *	@par�K�v�t�@�C��
 *			TnbIconManagerEx.h
 *
 *	@date 07/10/30 �V�K�쐬
 *	@date 07/11/16 �A�C�R���Ɏg����HBITMAP��ێ�����K�v�Ȃ�
 */
class CIconManagerEx : public CIconManager
{
	DEFSUPER(CIconManager);
	/// �쐬
	HICON m_Create(ICONINFO& _ii)
	{
		HICON hIcon = ::CreateIconIndirect(&_ii);
		if ( hIcon != NULL )
		{
			_super::Attach(hIcon);
		}
		return hIcon;
	}
	/// �쐬
	HICON m_Create(ICONINFO& _ii, CBitmapHandle bmp, COLORREF maskColor, BYTE alpha)
	{
		CBitmapImage bi;
		bi.Set(bmp);
		CBitmapHandle bmpMask = bi.CreateMaskBitmap(false, maskColor);
		HRGN rgn = bi.CreateRgn(maskColor);
		if ( rgn == NULL )
		{
			return NULL;
		}
		RECT rect = { 0, 0, bi.GetSize().cx, bi.GetSize().cy };
		HDC dc = bi.GetDC();
		::ExtSelectClipRgn(dc, rgn, RGN_DIFF);
		::SetBkColor(dc, RGB(0, 0, 0)); // XOR ����邩�� 0�ɂ��Ă���
		::ExtTextOut(dc, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
		bi.ReleaseDC();
		if ( alpha != 0xFF )
		{
			bi.CustomizeAlpha(alpha, RGB(0, 0, 0), 0);
		}
		_ii.hbmColor = bi.GetBitmapHandle();
		_ii.hbmMask = bmpMask;
		_DeleteObject(rgn);
		return m_Create(_ii);
	}
public:

	/// �R���X�g���N�^
	CIconManagerEx(void) : _super()
	{
	}

	/// �f�X�g���N�^
	virtual ~CIconManagerEx(void)
	{
	}

	/**
	 * [�쐬] �r�b�g�}�b�v����쐬.
	 *		�w��̃r�b�g�}�b�v����A�C�R�����쐬���܂��B
	 *	@note �r�b�g�}�b�v����A�C�R�������ɂ́A CBitmapImage ���g�p�ł��܂��B
	 *	@param bmpColor	XOR �r�b�g�}�X�N�i�摜�j�p�̃r�b�g�}�b�v���w�肵�܂��B
	 *	@param bmpMask	AND �r�b�g�}�X�N�i���߁j�p�̃r�b�g�}�b�v���w�肵�܂��B
	 *	@retval NULL�ȊO �A�C�R���n���h���B API�� DestroyIcon() �͎g�p���Ȃ��ł��������B
	 *	@retval NULL �G���[
	 */
	HICON CreateIcon(CBitmapHandle bmpColor, CBitmapHandle bmpMask)
	{
		ICONINFO ii = { true, 0, 0, bmpMask, bmpColor };
		ASSERTLIB( ii.fIcon && ii.yHotspot == 0 && ii.hbmMask == bmpMask.operator->() );
		return m_Create(ii);
	}

	/**
	 * [�쐬] �r�b�g�}�b�v����쐬.
	 *		�w��̃r�b�g�}�b�v����A�C�R�����쐬���܂��B
	 *	@param bmp �r�b�g�}�b�v �B
	 *	@param maskColor �}�X�N�F���w�肵�܂��B 
	 *						�ȗ�����ƁABITMAP�̍����̐F���}�X�N�F�Ƃ��܂��B\n
	 *						CLR_INVALID ���w�肷��Ɠ��ߐF�Ȃ��ŃA�C�R�����쐬���܂��B
	 *	@retval NULL�ȊO �A�C�R���n���h���B API�� DestroyIcon() �͎g�p���Ȃ��ł��������B
	 *	@retval NULL �G���[
	 */
	HICON CreateIcon(CBitmapHandle bmp, COLORREF maskColor = CLR_AUTOSELECT)
	{
		ICONINFO ii = { true, 0, 0 };
		ASSERTLIB( ii.fIcon && ii.yHotspot == 0 );
		return m_Create(ii, bmp, maskColor, 0xFF);
	}

	/**
	 * [�쐬] �r�b�g�}�b�v����쐬.
	 *		�w��̃r�b�g�}�b�v����J�[�\�����쐬���܂��B
	 *	@param xHotspot	�J�[�\���̃z�b�g�X�|�b�g�� X ���W���w�肵�܂��B
	 *	@param yHotspot	�J�[�\���̃z�b�g�X�|�b�g�� Y ���W���w�肵�܂��B
	 *	@param bmpColor	XOR �r�b�g�}�X�N�i�摜�j�p�̃r�b�g�}�b�v �n���h�����w�肵�܂��B
	 *	@param bmpMask	AND �r�b�g�}�X�N�i���߁j�p�̃r�b�g�}�b�v �n���h�����w�肵�܂��B
	 *	@retval NULL�ȊO �J�[�\���n���h���i�A�C�R���n���h���j�B API�� DestroyIcon() DestroyCursor() �͎g�p���Ȃ��ł��������B
	 *	@retval NULL �G���[
	 */
	HCURSOR CreateCursor(DWORD xHotspot, DWORD yHotspot, CBitmapHandle bmpColor, CBitmapHandle bmpMask)
	{
		ICONINFO ii = { false, xHotspot, yHotspot, bmpMask, bmpColor };
		ASSERTLIB( ! ii.fIcon && ii.yHotspot == yHotspot && ii.hbmMask == bmpMask.operator->() );
		return m_Create(ii);
	}

	/**
	 * [�쐬] �r�b�g�}�b�v����쐬.
	 *		�w��̃r�b�g�}�b�v����J�[�\�����쐬���܂��B
	 *	@param xHotspot	�J�[�\���̃z�b�g�X�|�b�g�� X ���W���w�肵�܂��B
	 *	@param yHotspot	�J�[�\���̃z�b�g�X�|�b�g�� Y ���W���w�肵�܂��B
	 *	@param bmp �r�b�g�}�b�v �n���h�����w�肵�܂��B
	 *	@param parsent �J�[�\���\�����̔Z��(�P��;%)�B �ȗ��Ȃ�w��̃r�b�g�}�b�v�̃A���t�@�`���l�������̂܂ܓK������܂��B
	 *				0 ���w�肷��ƁA���������Ȃ��Ȃ�܂��B
	 *	@param maskColor �}�X�N�F���w�肵�܂��B 
	 *						�ȗ�����ƁABITMAP�̍����̐F���}�X�N�F�Ƃ��܂��B\n
	 *						CLR_INVALID ���w�肷��Ɠ��ߐF�Ȃ��ŃA�C�R�����쐬���܂��B
	 *	@retval NULL�ȊO �J�[�\���n���h���i�A�C�R���n���h���j�B API�� DestroyIcon() DestroyCursor() �͎g�p���Ȃ��ł��������B
	 *	@retval NULL �G���[
	 */
	HCURSOR CreateCursor(DWORD xHotspot, DWORD yHotspot, CBitmapHandle bmp, int parsent = 100, COLORREF maskColor = CLR_AUTOSELECT)
	{
		ICONINFO ii = { false, xHotspot, yHotspot };
		ASSERTLIB( ! ii.fIcon && ii.yHotspot == yHotspot );
		return m_Create(ii, bmp, maskColor, static_cast<BYTE>(parsent * 255 / 100));
	}
};



};///TNB

