#pragma once
/**
 *@file
 * �_�~�[�A�j���Ǘ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbBitmapImage.h"



//TNB Library
namespace TNB
{



/**@ingroup ANIMATE
 * �_�~�[�A�j���[�^�[.
 *
 *		�w��̑傫���ŊȒP�ȃA�j���[�V�������쐬���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDummyAnimater.h
 *
 *	@date 09/09/02 �V�K�쐬
 */
class CDummyAnimater
{
public:

	/// �R���X�g���N�^
	CDummyAnimater(void)
	{
		m_size.cx = 0;
		m_size.cy = 0;
	}

	/**
	 * [�쐬] �V�摜�쐬.
	 *	@param count �ʂ̐�
	 *	@param cx �摜�̕�
	 *	@param cy �摜�̍���
	 *	@param ph �ʂ̒��a
	 */
	void NewImage(size_t count, int cx, int cy, int ph = 10)
	{
		m_size.cx = cx;
		m_size.cy = cy;
		//
		m_image.Set(m_size.cx, m_size.cy, RGB(0, 0, 0));
		m_blackImage.Set(m_size.cx, m_size.cy);
		m_blackImage.GradationFill(RGB(120, 0, 0), RGB(0, 0, 120), true);
		m_pieces.Resize(count);
		::srand(::GetTickCount());
		loop ( i, m_pieces.GetSize() )
		{
			m_pieces[i].Init(m_size, ph);
		}
		m_Remake();
	}

	/**
	 * [�쐬] ���摜�쐬.
	 *		���̉摜���쐬���܂��B
	 */
	void Next(void)
	{
		loop ( i, m_pieces.GetSize() )
		{
			m_pieces[i].Next(m_size);
		}
		m_Remake();
	}

	/**
	 * [�擾] �擾.
	 *		���݂̉摜���擾���܂��B
	 *	@return �摜
	 */
	const CBitmapImage& Get(void) const
	{
		return m_image;
	}


private:
	// �摜�ĕ`��
	void m_Remake(void)
	{
		HDC dc = m_image.GetDC();
		m_blackImage.BitBlt(dc, 0, 0);
		loop ( i, m_pieces.GetSize() )
		{
			const TPiece& P = m_pieces[i];
			HBRUSH b = ::CreateSolidBrush(P.color);
//			RECT rc = { P.pos.x, P.pos.y, P.pos.x + P.size, P.pos.y + P.size };
//			::FillRect(dc, &rc, b);
			HGDIOBJ old = ::SelectObject(dc, b);
			::RoundRect(dc, P.pos.x, P.pos.y, P.pos.x + P.size, P.pos.y + P.size, P.size, P.size);
			::SelectObject(dc, old);
			_DeleteObject(b);
		}
		m_image.ReleaseDC();
	}
	/// �R�}
	struct TPiece
	{
		POINT		pos;		///< ���W
		POINT		vector;		///< ����
		COLORREF	color;		///< �F
		LONG		size;		///< �T�C�Y
		void Init(const SIZE& sz, int sizeMax = 4)
		{
			sizeMax /= 2;
			size = (::rand() % sizeMax) + sizeMax;
			pos.x = ::rand() % (sz.cx - size);
			pos.y = ::rand() % (sz.cy - size);
			color = RGB(::rand() & 0xFF, ::rand() & 0xFF, ::rand() & 0xFF);
			vector.x = ((::rand() % 7) + 1) * ((::rand() & 1) == 0 ? 1 : -1);
			vector.y = ((::rand() % 7) + 1) * ((::rand() & 1) == 0 ? 1 : -1);
		}
		void Next(const SIZE& sz)
		{
			pos.x += vector.x;
			if ( pos.x < 0 ) 
			{
				pos.x = -pos.x;
				vector.x = -vector.x;
			}
			else if ( pos.x > sz.cx - size )
			{
				pos.x -= (pos.x - (sz.cx - size)) * 2;
				vector.x = -vector.x;
			}
			pos.y += vector.y;
			if ( pos.y < 0 )
			{
				pos.y = -pos.y;
				vector.y = -vector.y;
			}
			else if ( pos.y > sz.cy - size)
			{
				pos.y -= (pos.y - (sz.cy - size)) * 2;
				vector.y = -vector.y;
			}
		}

	};
	CWorkMemT<TPiece>	m_pieces;		///< �R�}���
	SIZE				m_size;			///< �摜�T�C�Y
	CBitmapImage		m_blackImage;	///< �w�i�C���[�W
	CBitmapImage		m_image;		///< �C���[�W
};



};
