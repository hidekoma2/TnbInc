#pragma once
/**
 *@file
 * �r�b�g�}�b�v���C�p�[�֌W�̃w�b�_
 *
 *	�e�탏�C�p�[������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbBitmapAnimater.h"



//TNB Library
namespace TNB
{



/**@ingroup ANIMATE
 * �k�����C�v�����N���X
 *
 *	��̉摜���A��{����w��̑傫���ɂ����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapWipers.h
 *
 *	@date 07/11/30 �V�K�쐬
 */
class CShrinkWiper : public IBitmapWipable
{
	DEFSUPER(IBitmapWipable);
public:
	
	/**
 	 * [����] �N���[��
	 *	@return �N���[���B�s�K�v�ɂȂ����� delete ����K�v������܂��B
	 */
	virtual IBitmapWipable* Clone(void) const { return new CShrinkWiper(); }

	/**
	 * [�ϊ�] ���C�v. 
	 *		baseBmp �̏�� overBmp ������������Ԃ��܂��B
	 *		par �� 0�Ȃ� baseBmp ���̂��́A par �� max �Ȃ� overBmp ���̂��̂ɂȂ�܂�
	 *	@param baseBmp ���G.
	 *	@param overBmp ��G.���G�Ɠ����傫���ȊO�͌��ʕۏ�O�ɂȂ�܂��B
	 *	@param par �i��(���q)
	 *	@param max �S��(����)
	 *	@return �ϊ���̊G
	 */
	virtual CBitmapHandle Wipe(CBitmapHandle baseBmp, CBitmapHandle overBmp, UINT par, UINT max) const
	{
		CBitmapImage bi;
		bi.Set(baseBmp);
		const SIZE& size = bi.GetSize();
		int cx = size.cx * (max - par) / max;
		int cy = size.cy * (max - par) / max;
		bi.Insert(-cx, -cy, overBmp, SRCCOPY, size.cx + cx * 2, size.cy + cy * 2);
		return bi.GetBitmapHandle();
	}
};



/**@ingroup ANIMATE
 * �X���C�h���C�v�����N���X
 *
 *	��̉摜���A�w��̕�������X���C�h���Ē����ɗ���悤�ɂ��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapWipers.h
 *
 *	@date 07/11/30 �V�K�쐬
 */
class CSlideWiper : public IBitmapWipable
{
	DEFSUPER(IBitmapWipable);
	UINT	m_pos;	///< �|�W�V����
public:
	
	/**
	 * �R���X�g���N�^
	 *	@param uPos �ǂ���̕�������X���C�h���Ă��邩�B �P�`�S�A�U�`�X�B�R���E���B�W���㒆�B
	 */
	CSlideWiper(UINT uPos = 5) : m_pos(uPos)
	{
	}

	/**
 	 * [����] �N���[��
	 *	@return �N���[���B�s�K�v�ɂȂ����� delete ����K�v������܂��B
	 */
	virtual IBitmapWipable* Clone(void) const { return new CSlideWiper(m_pos); }

	/**
	 * [�ϊ�] ���C�v. 
	 *		baseBmp �̏�� overBmp ������������Ԃ��܂��B
	 *		par �� 0�Ȃ� baseBmp ���̂��́A par �� max �Ȃ� overBmp ���̂��̂ɂȂ�܂�
	 *	@param baseBmp ���G.
	 *	@param overBmp ��G.���G�Ɠ����傫���ȊO�͌��ʕۏ�O�ɂȂ�܂��B
	 *	@param par �i��(���q)
	 *	@param max �S��(����)
	 *	@return �ϊ���̊G
	 */
	virtual CBitmapHandle Wipe(CBitmapHandle baseBmp, CBitmapHandle overBmp, UINT par, UINT max) const
	{
		CBitmapImage bi;
		bi.Set(baseBmp);
		const SIZE& size = bi.GetSize();
		int cx = size.cx * (max - par) / max;
		int cy = size.cy * (max - par) / max;
		if ( m_pos >= 7 ) 
		{
			cy = -cy; 
		}
		else if ( m_pos >= 4 ) 
		{
			cy = 0;
		}
		switch ( m_pos % 3 )
		{
		case 2: cx = 0; break;
		case 1: cx = -cx; break;
		}
		bi.Insert(cx, cy, overBmp, SRCCOPY);
		return bi.GetBitmapHandle();
	}
};



/**@ingroup ANIMATE
 * �}�X�N�^���ۃ��C�v�����N���X
 *
 *		�}�X�N�p�^�[�����g���āA�؂�ւ��郏�C�p�[�̒��ۃN���X�ł��B
 *
 *	@note Clone() �� CreateMaskRgn() ����������K�v������܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapWipers.h
 *
 *	@date 07/12/04 �V�K�쐬
 */
class CAbstractMaskWiper : public IBitmapWipable
{
	DEFSUPER(IBitmapWipable);
protected:
	/**
	 * [�쐬] �}�X�N�̈�쐬. 
	 *		par �� 0�Ȃ�̈�Ȃ��A par �� max �Ȃ� �S�̈�
	 *	@param size �T�C�Y.
	 *	@param par �i��(���q)
	 *	@param max �S��(����)
	 *	@retval NULL �̈�Ȃ�
	 *	@retval NULL�ȊO �̈�
	 */
	virtual HRGN CreateMaskRgn(const SIZE& size, UINT par, UINT max) const = 0;

public:
	/**
	 * [�ϊ�] ���C�v. 
	 *		baseBmp �̏�� overBmp ������������Ԃ��܂��B
	 *		par �� 0�Ȃ� baseBmp ���̂��́A par �� max �Ȃ� overBmp ���̂��̂ɂȂ�܂�
	 *	@param baseBmp ���G.
	 *	@param overBmp ��G.
	 *	@param par �i��(���q)
	 *	@param max �S��(����)
	 *	@return �ϊ���̊G
	 */
	virtual CBitmapHandle Wipe(CBitmapHandle baseBmp, CBitmapHandle overBmp, UINT par, UINT max) const
	{
		CBitmapImage bi;
		bi.Set(baseBmp);
		if ( ! bi.IsEmpty() )
		{
			HRGN hRgn = CreateMaskRgn(bi.GetSize(), par, max);
			if ( hRgn != NULL )
			{
				HDC dc = bi.GetDC();
				::SelectClipRgn(dc, hRgn);
				CBitmapImage b(overBmp);
				b.Draw(dc, 0, 0);
				::SelectClipRgn(dc, NULL);
				bi.ReleaseDC();
				_DeleteObject(hRgn);
			}
		}
		return bi.GetBitmapHandle();
	}
};



/**@ingroup ANIMATE
 * �X���C�h�}�X�N���C�v�����N���X
 *
 *	��̉摜���A�w��̕������猩���Ă���悤�ɂȂ�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapWipers.h
 *
 *	@date 07/12/04 �V�K�쐬
 */
class CSlideMaskWiper : public CAbstractMaskWiper
{
	DEFSUPER(CAbstractMaskWiper);
	UINT	m_pos;	/// 2,4,6,8
protected:
	/**
	 * [�쐬] �}�X�N�̈�쐬. 
	 *		par �� 0�Ȃ�̈�Ȃ��A par �� max �Ȃ� �S�̈�
	 *	@param size �T�C�Y.
	 *	@param par �i��(���q)
	 *	@param max �S��(����)
	 *	@retval NULL �̈�Ȃ�
	 *	@retval NULL�ȊO �̈�
	 */
	virtual HRGN CreateMaskRgn(const SIZE& size, UINT par, UINT max) const
	{
		HRGN rgn = ::CreateRectRgn(0, 0, size.cx, size.cy);
		int cx = size.cx * (max - par) / max;
		int cy = size.cy * (max - par) / max;
		if ( m_pos >= 7 ) 
		{
			cy = -cy; 
		}
		else if ( m_pos >= 4 ) 
		{
			cy = 0;
		}
		switch ( m_pos % 3 )
		{
		case 2: cx = 0; break;
		case 1: cx = -cx; break;
		}
		::OffsetRgn(rgn, cx, cy);
		return rgn;
	}

public:

	/**
	 * �R���X�g���N�^
	 *	@param p �ǂ���̕������猩����悤�ɂ��邩�B �P�`�S�A�U�`�X�B�R���E���B�W���㒆�B
	 */
	CSlideMaskWiper(UINT p) : m_pos(p)
	{
	}

	/**
 	 * [����] �N���[��
	 *	@return �N���[���B�s�K�v�ɂȂ����� delete ����K�v������܂��B
	 */
	virtual IBitmapWipable* Clone(void) const 
	{
		return new CSlideMaskWiper(m_pos); 
	}
};



/**@ingroup ANIMATE
 * �X���C�h�}�X�N���C�v�����N���X
 *
 *	��̉摜���A�����_���̃|�C���g����`���`���Ɛ؂�ւ��悤�ɂȂ�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapWipers.h
 *
 *	@date 07/12/04 �V�K�쐬
 */
class CRandomMaskWiper : public CAbstractMaskWiper
{
	DEFSUPER(CAbstractMaskWiper);
	size_t				m_size;		///< 1�ȏ�B ���̐��~���̐��̃}�X�N�����܂��B
	mutable LPRGNDATA	m_pRgnData;	///< �}�X�N�f�[�^
	mutable CWorkMem	m_rgnWork;
protected:

	/**
	 * [�쐬] �}�X�N�̈�쐬. 
	 *		par �� 0�Ȃ�̈�Ȃ��A par �� max �Ȃ� �S�̈�
	 *	@param size �T�C�Y.
	 *	@param par �i��(���q)
	 *	@param max �S��(����)
	 *	@retval NULL �̈�Ȃ�
	 *	@retval NULL�ȊO �̈�
	 */
	virtual HRGN CreateMaskRgn(const SIZE& size, UINT par, UINT max) const
	{
		size_t len = m_size * m_size;
		if ( m_pRgnData == NULL )
		{
			m_rgnWork.Resize(sizeof(RGNDATAHEADER) + sizeof(RECT) * len);
			m_pRgnData = reinterpret_cast<LPRGNDATA>(m_rgnWork.Ref());
			LPRECT pRect = reinterpret_cast<LPRECT>(m_pRgnData->Buffer);
			int i = 0;
			loop ( y, m_size )
			{
				loop ( x, m_size )
				{
					::SetRect(&pRect[i++], ToInt(x), ToInt(y), ToInt(x + 1), ToInt(y + 1));
				}
			}
			loop ( i, len )
			{
				INDEX m = rand() % len;
				Swap(pRect[i], pRect[m]);
			}
			m_pRgnData->rdh.dwSize			= sizeof(RGNDATAHEADER);
			m_pRgnData->rdh.iType			= RDH_RECTANGLES;
			m_pRgnData->rdh.rcBound.left	= 0;
			m_pRgnData->rdh.rcBound.top		= 0;
			m_pRgnData->rdh.rcBound.right	= size.cx;
			m_pRgnData->rdh.rcBound.bottom	= size.cy;
		}
		//
		size_t c = len * par / max;
		if ( c == 0 ) 
		{
			return NULL; 
		}
		m_pRgnData->rdh.nRgnSize = ToDword(sizeof(RGNDATAHEADER) + sizeof(RECT) * c);
		m_pRgnData->rdh.nCount = ToDword(c);
		HRGN hTmpRgn = ::ExtCreateRegion(NULL, m_pRgnData->rdh.nRgnSize, m_pRgnData);
		HRGN hRgn = ::CreateRectRgn(0, 0, 0, 0);
		loop ( x, (size.cx / m_size) + 1 )
		{
			::CombineRgn(hRgn, hRgn, hTmpRgn, RGN_OR);
			::OffsetRgn(hTmpRgn, ToInt(m_size), 0);
		}
		::CombineRgn(hTmpRgn, hRgn, NULL, RGN_COPY);
		loop ( y, (size.cy / m_size) )
		{
			::OffsetRgn(hTmpRgn, 0, ToInt(m_size));
			::CombineRgn(hRgn, hRgn, hTmpRgn, RGN_OR);
		}
		_DeleteObject(hTmpRgn);
		return hRgn;
	}

public:

	/**
	 * �R���X�g���N�^
	 *	@param size �����_���^�C���̑傫�����w�肵�܂��B�ʏ�w�肷��K�v�͂���܂���B
	 */
	CRandomMaskWiper(size_t size = 32) : m_size(size), m_pRgnData(NULL)
	{
		ASSERT0( size > 1, "CRandomMaskWiper", "1���傫�����l���w�肵�Ă��������B" );
	}

	/// �f�X�g���N�^
	~CRandomMaskWiper(void)
	{
		if ( m_pRgnData != NULL )
		{
			m_pRgnData = NULL;
			m_rgnWork.Free();
		}
	}

	/**
 	 * [����] �N���[��
	 *	@return �N���[���B�s�K�v�ɂȂ����� delete ����K�v������܂��B
	 */
	virtual IBitmapWipable* Clone(void) const 
	{
		return new CRandomMaskWiper(m_size); 
	}
};



}; //TNB
