#pragma once
/**
 *@file
 * ���̃r�b�g�}�b�v���[�J�[�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbBitmapImage.h"
#include "TnbHandleHandle.h"



//TNB Library
namespace TNB
{



/**@ingroup BITMAP
 * ���̃r�b�g�}�b�v���[�J�[�N���X
 *
 *		��̉摜�̎���𗧑̂ɂ��A�{�^�����Ɏg�p�ł���悤�ɂ��܂��B
 *
 *	@note ���̉摜���A�㉺�� 3 dot �傫���Ȃ�܂�( SetFaceColor() �Q��)�B
 *
 *	@par�K�v�t�@�C��
 *			TnbSolidBitmapMaker.h
 *
 *	@date 09/11/10 �V�K�쐬
 */
class CSolidBitmapMaker
{
public:

	/// �R���X�g���N�^
	CSolidBitmapMaker(void)
	{
		m_DefaultBrush();
		m_transColor = CLR_AUTOSELECT;
		m_baseColor = RGB(255, 0, 255);
	}

	/**
	 * [�ݒ�] �w�i�F�ݒ�.
	 *	@note �쐬�����摜�̔w�i�F���w�肵�܂��B�w�肪�Ȃ��ꍇ�A �}�[���^(RGB(255, 0, 255))�ɂȂ�܂��B
	 *	@param c �w�i�F.
	 */
	void SetBaseColor(COLORREF c)
	{
		m_baseColor = c;
	}

	/**
	 * [�ݒ�] ���ߐF�ݒ�.
	 *	@note ���̉摜�̓��߂���F���w�肵�܂��B�w�肪�Ȃ��ꍇ�A�����̐F�𓧉ߐF�Ƃ��܂��B
	 *	@param c ���ߐF.
	 */
	void SetTransColor(COLORREF c)
	{
		m_transColor = c;
	}

	/**
	 * [�ݒ�] �t�F�C�X�J���[�ݒ�.
	 *	@note �f�t�H���g�ł́A�V�X�e���̃{�^���Ɠ����F�ɂȂ�܂��B
	 *	\par�t�F�C�X�J���[�z�u:
	 *	\code
	 *		�I���W�i���摜
	 *			
	 *			�@������
	 *			�@������
	 *			�@������
	 *
	 *		�ł��΂�̏ꍇ
	 *			
	 *			�{�{�{�{�{��	�{ ; c1 ���邢�F
	 *			�{����������	�� ; c2 �Â��F
	 *			�{����������	�� ; c3 �����ƈÂ��F
	 *			�{����������
	 *			�{����������
	 *			������������
	 *
	 *		�ւ����݂̏ꍇ
	 *			
	 *			������������	�{ ; c1 ���邢�F
	 *			�����������{	�� ; c2 �Â��F
	 *			�����������{	�� ; c3 �����ƈÂ��F
	 *			�����������{
	 *			�����������{
	 *			���{�{�{�{�{
	 *	\endcode
	 *	@see SetDefaultFaceColor()
	 *	@param c1 ���邢�F
	 *	@param c2 �Â��F
	 *	@param c3 �����ƈÂ��F
	 */
	void SetFaceColor(COLORREF c1, COLORREF c2, COLORREF c3)
	{
		m_brush1 = ::CreateSolidBrush(c1);
		m_currentBrush1 = m_brush1;
		m_brush2 = ::CreateSolidBrush(c2);
		m_currentBrush2 = m_brush2;
		m_brush3 = ::CreateSolidBrush(c3);
		m_currentBrush3 = m_brush3;
	}

	/**
	 * [�ݒ�] �t�F�C�X�J���[�ݒ�.
	 *	@note �V�X�e���̃{�^���Ɠ����F�ɂ��܂��i�f�t�H���g�j�B
	 *	@see SetFaceColor()
	 */
	void SetDefaultFaceColor(void)
	{
		m_DefaultBrush();
		m_brush1.Null();
		m_brush2.Null();
		m_brush3.Null();
	}

	/**
	 * [�쐬] ���̃r�b�g�}�b�v�쐬.
	 *	@param bh ���摜.
	 *	@param isPressed �ȗ�, false �̏ꍇ�ł��΂�A true �̏ꍇ�A�ւ����݁B
	 *	@return �쐬�摜.
	 */
	CBitmapImage Make(CBitmapHandle bh, bool isPressed = false) const
	{
		CBitmapImage bi = bh;
		if ( ! isPressed )
		{
			return m_MakeButton(bi);
		}
		return m_MakeButtonRv(bi);
	}

private:

	/// �W���u���V
	void m_DefaultBrush(void)
	{
		m_currentBrush1 = ::GetSysColorBrush(COLOR_3DHILIGHT);
		m_currentBrush2 = ::GetSysColorBrush(COLOR_3DSHADOW);
		m_currentBrush3 = ::GetSysColorBrush(COLOR_3DDKSHADOW);
	}
	
	/// �摜�쐬
	CBitmapImage m_MakeButton(const CBitmapImage& bmp) const
	{
		CBitmapImage bi;
		bi.Set(bmp.GetSize().cx + 3, bmp.GetSize().cy + 3, m_baseColor);
		HRGN hRgn = bmp.CreateRgn(m_transColor);
		HBRUSH b1 = m_currentBrush1;
		HBRUSH b2 = m_currentBrush2;
		HBRUSH b3 = m_currentBrush3;
		HDC hDC = bi.GetDC();
		// b3
		::OffsetRgn(hRgn, 3, 0);
		{
			::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, -1, 0);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, -1, 0);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, -1, 0);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, +3, -3);
		}
		::OffsetRgn(hRgn, -3, 0);
		// b1
		{
			::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, 0, -2);
			::OffsetRgn(hRgn, 1, 0);	::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, 1, 0);	::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, -2, 0);
		}
		// b2
		::OffsetRgn(hRgn, 2, 1);
		{
			::FillRgn(hDC, hRgn, b2);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b2);
			::OffsetRgn(hRgn, -1, 0);	::FillRgn(hDC, hRgn, b2);
			::OffsetRgn(hRgn, 1, -1);
		}
		::OffsetRgn(hRgn, -2, -1);
		//
		bi.ReleaseDC();
		bi.InsertOnTransparent(1, 1, bmp, m_transColor);
		_DeleteObject(hRgn);
		return bi;
	}

	/// �摜�쐬
	CBitmapImage m_MakeButtonRv(const CBitmapImage& bmp) const
	{
		CBitmapImage bi;
		bi.Set(bmp.GetSize().cx + 3, bmp.GetSize().cy + 3, m_baseColor);
		HRGN hRgn = bmp.CreateRgn(m_transColor);
		HBRUSH b1 = m_currentBrush1;
		HBRUSH b2 = m_currentBrush2;
		HBRUSH b3 = m_currentBrush3;
		HDC hDC = bi.GetDC();
		// b3
		{
			::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, 0, -3);
			::OffsetRgn(hRgn, 1, 0);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, 1, 0);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, 1, 0);	::FillRgn(hDC, hRgn, b3);
			::OffsetRgn(hRgn, -3, 0);
		}
		// b1
		::OffsetRgn(hRgn, 3, 1);
		{
			::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, -1, 0);	::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, -1, 0);	::FillRgn(hDC, hRgn, b1);
			::OffsetRgn(hRgn, 2, -2);
		}
		::OffsetRgn(hRgn, -3, -1);
		// b2
		::OffsetRgn(hRgn, 1, 1);
		{
			::FillRgn(hDC, hRgn, b2);
			::OffsetRgn(hRgn, 1, 0);	::FillRgn(hDC, hRgn, b2);
			::OffsetRgn(hRgn, -1, 0);
			::OffsetRgn(hRgn, 0, 1);	::FillRgn(hDC, hRgn, b2);
			::OffsetRgn(hRgn, 0, -1);
		}
		::OffsetRgn(hRgn, -1, -1);
		//
		bi.ReleaseDC();
		bi.InsertOnTransparent(2, 2, bmp, m_transColor);
		_DeleteObject(hRgn);
		return bi;
	}

	CBrushHandle	m_brush1;			///< ���邢��
	CBrushHandle	m_brush2;			///< �Â���
	CBrushHandle	m_brush3;			///< �����ƈÂ���
	HBRUSH			m_currentBrush1;	///< ���邢��
	HBRUSH			m_currentBrush2;	///< �Â���
	HBRUSH			m_currentBrush3;	///< �����ƈÂ���
	COLORREF		m_transColor;		///< ���ߐF
	COLORREF		m_baseColor;		///< �w�i�F
};



};///TNB
