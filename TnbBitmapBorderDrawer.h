#pragma once
/**
 *@file
 * �r�b�g�}�b�v�̈�`����̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDrawable.h"
#include "TnbBitmapImage.h"



//TNB Library
namespace TNB 
{



/**@ingroup DRAWABLE BITMAP
 * �r�b�g�}�b�v�̈�`��N���X
 *
 *		��̃r�b�g�}�b�v��3x3�ɕ������A���A�g�A���̃p�[�c�ɂ��A�����`��Ɏg�p���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapBorderDrawer.h
 * 
 *	@date 07/10/06	�V�K�쐬
 *	@date 10/04/21	���f�[�^��菬�����傫���ł̕`�揈�������P
 */
class CBitmapBorderDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
public:

	/**
	 * �R���X�g���N�^
	 *	@param bm �g�p����r�b�g�}�b�v
	 */
	CBitmapBorderDrawer(CBitmapHandle bm)
	{
		m_bmps[0] = bm;
		m_ApartBitmap();
	}

	/**
	 * [�擾] �r�b�g�}�b�v
	 *	@param idx �擾�ӏ��B0;�S�� 1�`9;�e���L�[���т�9�ӏ��̃r�b�g�}�b�v�B
	 *	@return �r�b�g�}�b�v
	 */
	CBitmapHandle GetParts(int idx) const
	{
		if ( idx >= 0 && idx < 10 )
		{
			return m_bmps[idx].Clone();
		}
		return CBitmapHandle();
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const 
	{
		CBitmapBorderDrawer* P = new CBitmapBorderDrawer(*this);
		return P;
	}

	/**
	 * [�`��] �`��.
	 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��̍��W�ɕ`�悵�܂��B
	 *	@note �{�N���X�͉������܂���B
	 *	@param dc �f�o�C�X�R���e�L�X�g
	 *	@param x �w���W
	 *	@param y �x���W
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const
	{
		POINT po;
		::GetBrushOrgEx(dc, &po);
		const SIZE& winSize = m_size;
		SIZE sz = m_bmps[0].GetSize();
		sz.cx /= 3;
		sz.cy /= 3;
		bool isNarrow = (sz.cx * 2 > m_size.cx);	//����
		bool isLow = (sz.cy * 2 > m_size.cy);		//�Ⴂ
		RECT rc;
		//===== �l�ς�
		//-- ����
		::SetRect(&rc, 0, 0, min(sz.cx, m_size.cx / 2 + 1), min(sz.cy, m_size.cy / 2 + 1));
		if ( sz.cx == rc.right && sz.cy == rc.bottom )
		{
			m_bmps[7].Draw(dc, 0, 0);
		}
		else
		{
			m_bmps[7].Cut(rc).Draw(dc, 0, 0);
		}
		//-- ����
		::SetRect(&rc, 0, 0, min(sz.cx, m_size.cx / 2 + 1), sz.cy);
		if ( sz.cx == rc.right )
		{
			m_Draw(dc, m_bmps[1], 0, winSize.cy - sz.cy, winSize);
		}
		else
		{
			m_Draw(dc, m_bmps[1].Cut(rc), 0, winSize.cy - sz.cy, winSize);
		}
		//-- �E��
		::SetRect(&rc, 0, 0, sz.cx, min(sz.cy, m_size.cy / 2 + 1));
		if ( sz.cy == rc.bottom )
		{
			m_Draw(dc, m_bmps[9], winSize.cx - sz.cx , 0, winSize);
		}
		else
		{
			m_Draw(dc, m_bmps[9].Cut(rc), winSize.cx - sz.cx , 0, winSize);
		}
		//-- �E��
		m_Draw(dc, m_bmps[3], winSize.cx - sz.cx , winSize.cy - sz.cy, winSize);
		//===== ��
		if ( ! isNarrow )
		{
			//-- ���
			::SetRect(&rc, sz.cx, 0, winSize.cx - sz.cx, sz.cy);
			if ( rc.bottom > m_size.cy / 2 )
			{
				rc.bottom = m_size.cy / 2;
			}
			::SetBrushOrgEx(dc, po.x, 0, NULL);
			m_bmps[8].DrawTile(dc, rc);
			//-- ����
			::SetRect(&rc, sz.cx, winSize.cy - sz.cy, winSize.cx - sz.cx, winSize.cy);
			int t = rc.top;
			if ( rc.top < m_size.cy / 2 )
			{
				rc.top = m_size.cy / 2;
			}
			::SetBrushOrgEx(dc, po.x, t, NULL);
			m_bmps[2].DrawTile(dc, rc);
		}
		if ( ! isLow )
		{
			//-- ����
			::SetRect(&rc, 0, sz.cy, sz.cx, winSize.cy - sz.cy);
			if ( rc.right > m_size.cx / 2 )
			{
				rc.right = m_size.cx / 2;
			}
			::SetBrushOrgEx(dc, 0, po.y, NULL);
			m_bmps[4].DrawTile(dc, rc);
			//-- �E��
			::SetRect(&rc, winSize.cx - sz.cx, sz.cy, winSize.cx, winSize.cy - sz.cy);
			int t = rc.left;
			if ( rc.left < m_size.cx / 2 )
			{
				rc.left = m_size.cx / 2;
			}
			::SetBrushOrgEx(dc, t, po.y, NULL);
			m_bmps[6].DrawTile(dc, rc);
		}
		//===== ����
		if ( ! isNarrow && ! isLow )
		{
			::SetRect(&rc, sz.cx, sz.cy, winSize.cx - sz.cx, winSize.cy - sz.cy);
			::SetBrushOrgEx(dc, po.x, po.y, NULL);
			m_bmps[5].DrawTile(dc, rc);
		}
	}

private:
	/// m_bmps[0] �𕪉����āAm_bmps[1�`9]�ɐݒ�
	void m_ApartBitmap(void)
	{
		CBitmapImage& bi = m_bmps[0];
		SIZE sz = bi.GetSize();
		sz.cx /= 3;
		sz.cy /= 3;
		RECT rc = { 0, 0, sz.cx, sz.cy };
		m_bmps[7] = bi.Cut(rc);
		::OffsetRect(&rc, sz.cx, 0);
		m_bmps[8] = bi.Cut(rc);
		::OffsetRect(&rc, sz.cx, 0);
		m_bmps[9] = bi.Cut(rc);
		::SetRect(&rc, 0, sz.cy, sz.cx, sz.cy * 2);
		m_bmps[4] = bi.Cut(rc);
		::OffsetRect(&rc, sz.cx, 0);
		m_bmps[5] = bi.Cut(rc);
		::OffsetRect(&rc, sz.cx, 0);
		m_bmps[6] = bi.Cut(rc);
		::SetRect(&rc, 0, sz.cy * 2, sz.cx, sz.cy * 3);
		m_bmps[1] = bi.Cut(rc);
		::OffsetRect(&rc, sz.cx, 0);
		m_bmps[2] = bi.Cut(rc);
		::OffsetRect(&rc, sz.cx, 0);
		m_bmps[3] = bi.Cut(rc);
	}
	void m_Draw(HDC dc, const CBitmapImage& bi, int px, int py, const SIZE& size) const
	{
		SIZE sz = bi.GetSize();
		RECT rc = { 0, 0, sz.cx, sz.cy };
		bool r = false;
		int dx = sz.cx - (size.cx / 2 + 1);
		if ( dx > 0 )
		{
			rc.left += dx;
			px += dx;
			r = true;
		}
		int dy = sz.cy - (size.cy / 2 + 1);
		if ( dy > 0 )
		{
			rc.top += dy;
			py += dy;
			r = true;
		}
		if ( r )
		{
			bi.Cut(rc).Draw(dc, px, py);
		}
		else
		{
			bi.Draw(dc, px, py);
		}
	}

	CBitmapImage	m_bmps[10];		///< 0;�S�� 1�`9;�e���L�[���т�9�ӏ��̃r�b�g�}�b�v
};



}; //TNB
