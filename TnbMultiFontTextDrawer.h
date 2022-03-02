#pragma once
/**
 *@file
 * �}���`�t�H���g�e�L�X�g�`��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbTextDrawer.h"
#include "TnbVector.h"
#include "TnbStrLib.h"



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE
 * �}���`�t�H���g�e�L�X�g�`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�t�H���g�ƐF�ƕ�����������Z�b�g�ɁA�����w�肵�A�`�悷�邱�Ƃ��o���܂��B
 *		���̍ہA�x�[�X���C�������낦�ĕ\�����܂��B
 *
 *	@todo ���݁A���s(�����s)�͖��Ή��ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMultiFontTextDrawer.h
 *
 *	@date 07/12/15 �V�K�쐬
 *	@date 08/03/23 �R���X�g���N�^��ǉ��B
 */
class CMultiFontTextDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
protected:
	/// �p�[�g���
	struct TPart
	{
		CVectorT<TCHAR>	str;			///< ������
		HFONT			font;			///< �t�H���g
		COLORREF		textColor;		///< �e�L�X�g�J���[
	};
	CVectorT<TPart>		m_aParts;		///< ���
	COLORREF			m_backColor;	///< �w�i�J���[
	DWORD				m_drawStyle;	///< DrawText�X�^�C��
	//
	mutable bool			m_boIsCalced;	///< �v�Z�ς�
	mutable CVectorT<POINT>	m_aOffsets;		///< �ePart��OFFSET
	mutable SIZE			m_partsSize;	///< �SPart�̑傫��
	/// �v�Z
	bool m_calc(HDC dc, DWORD style) const
	{
		if ( m_boIsCalced ) { return true; }
		UINT un = DT_PATH_ELLIPSIS | DT_END_ELLIPSIS | DT_WORD_ELLIPSIS;
		DWORD st = style & (0xFFFFFFF0 - un);
		m_aOffsets.RemoveAll();
		m_partsSize.cx = 0;
		m_partsSize.cy = 0;
		int h = 0;
		HGDIOBJ oldFont = ::GetCurrentObject(dc, OBJ_FONT);
		loop ( i, m_aParts.GetSize() )
		{
			const TPart& pa = m_aParts.At(i);
			::SelectObject(dc, pa.font);
			TEXTMETRIC tTextMetric;
			if ( ! ::GetTextMetrics(dc, &tTextMetric) )
			{
				return false;
			}
			SIZE sz = { 0, 0 };
			if ( ! CTextDrawer::CalcTextSize(sz, dc, st, pa.str.ReferBuffer()) )
			{
				return false;
			}
			ASSERT( sz.cy == tTextMetric.tmHeight );
			int l = tTextMetric.tmAscent;//�A���_�[�̃T�C�Y
			h = max(h, l);
			m_aOffsets.Add(CPoint(m_partsSize.cx, -l));
			m_partsSize.cx += sz.cx;
			m_partsSize.cy = max(m_partsSize.cy, tTextMetric.tmHeight);
		}
		::SelectObject(dc, oldFont);
		int x = 0;
		int y = 0;
		switch( style & 0x03 )
		{
		case DT_CENTER:
			x = (m_size.cx - m_partsSize.cx) / 2;
			break;
		case DT_RIGHT:
			x = m_size.cx - m_partsSize.cx;
			break;
		}
		if ( x < 0 )
		{
			x = 0;
		}
		switch( style & 0x0C )
		{
		case DT_VCENTER:
			y = (m_size.cy - m_partsSize.cy) / 2;
			break;
		case DT_BOTTOM:
			y = m_size.cy - m_partsSize.cy;
			break;
		}
		if ( y < 0 )
		{
			y = 0;
		}
		loop ( i, m_aOffsets.GetSize() )
		{
			m_aOffsets[i].x += x;
			m_aOffsets[i].y += y + h;
		}
		m_boIsCalced = true;
		return true;
	}
public:

	/**
	 * �R���X�g���N�^
	 *	@param size	�T�C�Y
	 *	@param backColor �w�i�F
	 *	@param drawStyle DrawText�X�^�C���w��B	�ȗ������ �uDT_CENTER | DT_VCENTER�v
	 */
	CMultiFontTextDrawer(const SIZE& size, COLORREF backColor = CLR_INVALID, DWORD drawStyle = DT_CENTER | DT_VCENTER)
		: _super(size), m_backColor(backColor), m_drawStyle(drawStyle | DT_SINGLELINE), m_boIsCalced(false)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@note �T�C�Y�� 0, 0 �ɂȂ�܂��B
	 *	@param backColor �w�i�F
	 *	@param drawStyle DrawText�X�^�C���w��B	�ȗ������ �uDT_CENTER | DT_VCENTER�v
	 */
	CMultiFontTextDrawer(COLORREF backColor = CLR_INVALID, DWORD drawStyle = DT_CENTER | DT_VCENTER)
		: _super(), m_backColor(backColor), m_drawStyle(drawStyle | DT_SINGLELINE), m_boIsCalced(false)
	{
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		CMultiFontTextDrawer* P =new CMultiFontTextDrawer(m_size, m_backColor, m_drawStyle);
		P->m_aParts = m_aParts;
		return P;
	}

	/**
	 * [�`��] �`��.
	 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��̍��W�ɕ`�悵�܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g
	 *	@param x �w���W
	 *	@param y �x���W
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const
	{
		if ( m_calc(dc, m_drawStyle) )
		{
			RECT rc = { x, y, x + m_size.cx, y + m_size.cy };
			COLORREF oldColor = ::GetBkColor(dc);
			int oldBkMode = ::SetBkMode(dc, TRANSPARENT);
			if ( IS_RGBVALUE(m_backColor) )
			{
				::SetBkColor(dc, m_backColor);
				::SetBkMode(dc, OPAQUE);
			}
			HGDIOBJ oldFont = ::GetCurrentObject(dc, OBJ_FONT);
			loop ( i, m_aParts.GetSize() )
			{
				const TPart& pa = m_aParts.At(i);
				::SelectObject(dc, pa.font);
				const POINT& offset = m_aOffsets[i];
				RECT r = rc;
				r.left += offset.x;
				r.top += offset.y;
				if ( r.right > r.left && r.bottom > r.top )
				{
					CTextDrawer::DrawTextRect(dc, r, m_drawStyle & 0xFFFFFFF0, pa.textColor, pa.str.ReferBuffer());
				}
			}
			::SelectObject(dc, oldFont);
			::SetBkMode(dc, oldBkMode);
			::SetBkColor(dc, oldColor);
		}
	}

	/**
	 * [�ǉ�] ���ǉ�.
	 *	@param str ������B
	 *	@param font �����t�H���g�B�g�p�I���܂Ŕj�����Ȃ��ł��������B
	 *	@param color �����F�B
	 *	@retval IVALID_INDEX �G���[�B
	 *	@retval 0�ȏ� �����B�l�̓C���f�b�N�X�B
	 */
	INDEX Add(LPCTSTR str, HFONT font, COLORREF color)
	{
		m_boIsCalced = false;
		TPart p;
		p.font = font;
		p.textColor = color;
		LPTSTR P = p.str.GetBuffer(_tcslen(str) + 1);
		STRLIB::Copy(P, str);
		p.str.ReleaseBuffer();
		return m_aParts.Add(p);
	}

	/**
	 * [�ݒ�] �T�C�Y����.
	 *		���݂̃T�C�Y�ƕ������DrawText�X�^�C�����g���āA�傫���𒲐����܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g�B�v�Z�Ɏg�p���܂��B�`��͂��܂���B
	 *	@param margin �}�[�W���B�K�v�ȃT�C�Y�ɉ�����T�C�Y�ł��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool AdjustSize(HDC dc, int margin = 1)
	{
		m_boIsCalced = false;
		if ( ! m_calc(dc, m_drawStyle & 0xFFFFFFF0) )
		{
			return false;
		}
		ASSERTLIB( m_boIsCalced );
		m_size.cx = m_partsSize.cx + margin * 2;
		m_size.cy = m_partsSize.cy + margin * 2;
		m_boIsCalced = false; //�v�Z�t���OOFF�ɂ���
		return true;
	}
};



}; //TNB


