#pragma once
/**
 *@file
 * �r�b�g�}�b�v�A�j���֌W�̃w�b�_
 *
 *		�؂�ւ������(���C�v)���`���� IBitmapWipable �C���^�[�t�F�[�X�A
 *		��������������A CPenetrateWiper , CStretchWiper �N���X�A
 *		�A�_�v�^�I�ȋ@�\������ CReverseAdaptWiper, CMultiAdaptWiper �N���X�A
 *		�����Ă������g���A�j�����쐬�A�Ǘ����� CBitmapAnimater �N���X������܂��B
 *
 *		TnbBitmapWipers.h �ɂ����̃��C�p�[������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbBitmapImage.h"
#include "TnbSimpleVector.h"



//TNB Library
namespace TNB
{



/**@ingroup ANIMATE
 * �r�b�g�}�b�v���C�p�u���C���^�[�t�F�[�X
 *
 *		��̃r�b�g�}�b�v�����X�ɐ؂�ւ��Ă����������T�|�[�g���܂��B
 *	
 *	@see CBitmapAnimater 
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapAnimater.h
 *
 *	@date 07/11/30 �V�K�쐬
 */
struct IBitmapWipable
{
	/// �f�X�g���N�^
	virtual ~IBitmapWipable(void) { }

	/**
 	 * [����] �N���[��
	 *	@return �N���[���B�s�K�v�ɂȂ����� delete ����K�v������܂��B
	 */
	virtual IBitmapWipable* Clone(void) const = 0;

	/**
	 * [�ϊ�] ���C�v. 
	 *		baseBmp �̏�� overBmp ������������Ԃ��܂��B
	 *		par �� 0�Ȃ� baseBmp ���̂��́A par �� max �Ȃ� overBmp ���̂��̂ɂȂ�܂�
	 *	@param baseBmp ���G�B
	 *	@param overBmp ��G�B�@�����G�Ɠ����傫���ȊO�͌��ʕۏ�O�ɂȂ�܂��B
	 *	@param par �i��(���q)
	 *	@param max �S��(����)
	 *	@return �ϊ���̊G
	 */
	virtual CBitmapHandle Wipe(CBitmapHandle baseBmp, CBitmapHandle overBmp, UINT par, UINT max) const = 0;

	/// �|�C���^�n���h���^�錾
	typedef CPointerHandleT<IBitmapWipable> Ptr;
};



/**@ingroup ANIMATE
 * ���o�[�X�A�_�v�g���C�p�[�N���X
 *
 *		���C�v�����𔽓]���܂��B
 *		
 *	@par�K�v�t�@�C��
 *			TnbBitmapAnimater.h
 *
 *	@date 07/11/30 �V�K�쐬
 */
class CReverseAdaptWiper : public IBitmapWipable
{
	DEFSUPER(IBitmapWipable);
protected:
	IBitmapWipable::Ptr m_pWiper;		///< �g�p���郏�C�p�[
public:

	/// �R���X�g���N�^
	CReverseAdaptWiper(const IBitmapWipable& w) : m_pWiper(w.Clone())
	{
	}

	/**
 	 * [����] �N���[��
	 *	@return �N���[���B�s�K�v�ɂȂ����� delete ����K�v������܂��B
	 */
	virtual IBitmapWipable* Clone(void) const
	{
		if ( m_pWiper.IsNull() ) return NULL;
		return new CReverseAdaptWiper(*m_pWiper);
	}

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
		return m_pWiper->Wipe(overBmp, baseBmp, max - par, max);
	}
};



/**@ingroup ANIMATE
 * �}���`�A�_�v�g���C�p�[�N���X
 *
 *		���C�v�����𕡐��o�^�ł��܂��B
 *		
 *	@note SetWiper() �������C�p�[���g���A���G�Ɛ�G�����C�v���܂��B���̌��ʂ��A
 *		���� AddWiper() �������C�p�[���g���A���G�Ƀ��C�v���܂��B���̌J��Ԃ������܂��B
 *	@par�g�p��
 *		�ȉ��́A��ڂɁA�傫����ς��郏�C�p�[��o�^���A2�ڂɓ��߂��郏�C�p�[��o�^���Ă��܂��B
 *		<table><tr><td><pre>
 *		
 *		CMultiAdaptWiper w;
 *		w.SetWiper(CStretchWiper(2));	///< �X�g���b�`���C�p�[�B
 *		w.AddWiper(CPenetrateWiper());	///< ���߃��C�p�[�B
 *		</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapAnimater.h
 *
 *	@date 07/11/30 �V�K�쐬
 *	@date 09/04/11 CSimpleVectorT �̎d�l�ύX�ɑΉ��B
 */
class CMultiAdaptWiper : public IBitmapWipable
{
	DEFSUPER(IBitmapWipable);
protected:
	typedef CSimpleVectorT<IBitmapWipable::Ptr> CWipablesVecter;///< ���C�p�[�|�C���^�z��^
	CWipablesVecter		m_wipers;		///< �g�p���郏�C�p�[
	/// �S��񂯂�
	void m_AllRemove(void)
	{
		m_wipers.RemoveAll();
	}
public:

	/// �R���X�g���N�^
	CMultiAdaptWiper(void)
	{
	}

	/// �f�X�g���N�^
	~CMultiAdaptWiper(void)
	{
		m_AllRemove();
	}

	/**
 	 * [����] �N���[��
	 *	@return �N���[���B�s�K�v�ɂȂ����� delete ����K�v������܂��B
	 */
	virtual IBitmapWipable* Clone(void) const
	{
		CMultiAdaptWiper* P = new CMultiAdaptWiper();
		loop ( i, m_wipers.GetSize() )
		{
			P->AddWiper(*m_wipers[i]);
		}
		return P;
	}

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
		loop ( i, m_wipers.GetSize() )
		{
			overBmp = m_wipers[i]->Wipe(baseBmp, overBmp, par, max);
		}
		return overBmp;
	}

	/**
	 * [�ݒ�] ���C�p�[�ݒ�.
	 *	@note ��ɐݒ肵�Ă��������C�p�[�̓N���A���A�w��̃��C�p�[��ݒ肵�܂��B
	 *	@param w ���C�p�[�B�{�I�u�W�F�N�g�ɂ͕������ێ������̂ŁA w �͔j�����Ă��܂��܂���B
	 */
	void SetWiper(const IBitmapWipable& w)
	{
		m_AllRemove();
		AddWiper(w);
	}

	/**
	 * [�ݒ�] ���C�p�[�ݒ�(�ǉ�).
	 *	@note ��̃I�u�W�F�N�g�ɕ����w�肪�o���܂��B�ǉ����ɏ�������܂��B
	 *	@param w ���C�p�[�B�{�I�u�W�F�N�g�ɂ͕������ێ������̂ŁA w �͔j�����Ă��܂��܂���B
	 */
	void AddWiper(const IBitmapWipable& w)
	{
		m_wipers.Add(w.Clone());
	}
};



/**@ingroup ANIMATE
 * ���߃��C�v�����N���X
 *
 *	��̉摜���A���߂��Đ؂�ւ��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapAnimater.h
 *
 *	@date 07/11/30 �V�K�쐬
 */
class CPenetrateWiper : public IBitmapWipable
{
	DEFSUPER(IBitmapWipable);
public:
	/**
 	 * [����] �N���[��
	 *	@return �N���[���B�s�K�v�ɂȂ����� delete ����K�v������܂��B
	 */
	virtual IBitmapWipable* Clone(void) const { return new CPenetrateWiper(); }

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
		bi.InsertOnAlphaBlend(0, 0, overBmp, 100 * par / max);
		return bi.GetBitmapHandle();
	}
};



/**@ingroup ANIMATE
 * �L�����C�v�����N���X
 *
 *	��̉摜���A�w��̏ꏊ����A�傫�����Ă����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapAnimater.h
 *
 *	@date 07/11/30 �V�K�쐬
 *	@date 08/12/03 ���A�����Œ�t���O�A�t��
 */
class CStretchWiper : public IBitmapWipable
{
	DEFSUPER(IBitmapWipable);
	BYTE	m_pos;	///< �|�W�V����
public:

	/// �t���O
	enum
	{
		HEIGHT_FIX = 0x10,	///< �����Œ�
		WIDTH_FIX = 0x20	///< ���Œ�
	};
	
	/**
	 * �R���X�g���N�^
	 *	@param uPos �ǂ��𒆐S�ɍL����n�߂邩�B �P�`�X�B�T�������B�W���㒆�B �X�� HEIGHT_FIX , WIDTH_FIX �� or �Ŏw��ł��܂��B
	 */
	CStretchWiper(BYTE uPos = 5) : m_pos(uPos)
	{
	}

	/**
 	 * [����] �N���[��
	 *	@return �N���[���B�s�K�v�ɂȂ����� delete ����K�v������܂��B
	 */
	virtual IBitmapWipable* Clone(void) const { return new CStretchWiper(m_pos); }

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
		int cx = size.cx;
		if ( (m_pos & WIDTH_FIX) == 0 )
		{
			cx = cx * par / max;
		}
		int cy = size.cy;
		if ( (m_pos & HEIGHT_FIX) == 0 )
		{
			cy = cy * par / max;
		}
		if ( cx > 0 && cy > 0 )
		{
			int po = m_pos & 0x0F;
			int x = size.cx - cx;
			int y = size.cy - cy;
			if ( po >= 7 ) 
			{
				y = 0; 
			}
			else if ( po >= 4 ) 
			{
				y /= 2;
			}
			switch ( po % 3 )
			{
			case 1: x = 0; break;
			case 2: x /= 2; break;
			}
			bi.Insert(x, y, overBmp, SRCCOPY, cx, cy);
		}
		return bi.GetBitmapHandle();
	}
};



/**@ingroup ANIMATE
 * �r�b�g�}�b�v�A�j���N���X
 *
 *		IBitmapWipable �������������C�p�[���g���A�o�^���ꂽ�r�b�g�}�b�v��
 *		���H���܂��B
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *		
 *		CBitmapAnimater anime;
 *		anime.SetFirstFrame("1.bmp", CPenetrateWiper());
 *		anime.AddBaseFrame("1.bmp", 5);
 *		anime.AddBaseFrame("2.bmp", 5);
 *		anime.AddBaseFrame("3.bmp", 5);
 *		anime.AddBaseFrame("4.bmp", 5);
 *		ASSERT( anime.GetLength() == 21 ); 
 *		anime.FrameAt(6).Save("1.5.bmp");
 *		</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapAnimater.h
 *
 *	@date 07/10/03 �V�K�쐬
 *	@date 09/04/11 CSimpleVectorT �̎d�l�ύX�ɑΉ��B
 */
class CBitmapAnimater //: CCopyImmposible
{
	struct TBaseFrame
	{
		CBitmapHandle		bmp;		///< �ω���̉摜
		UINT				count;		///< �ω�����R�}�� 1 �Ȃ�⊮�Ȃ��B
		IBitmapWipable::Ptr	pWiper;		///< �g�p���郏�C�p�[
	};
	CSimpleVectorT<TBaseFrame>		m_frames;	///< �x�[�X�t���[�����
	size_t							m_length;	///< �S�t���[����
	CSimpleVectorT<CBitmapHandle>	m_cache;	///< �L���b�V��
	// �S����
	void m_RemoveAll(void)
	{
		m_frames.RemoveAll();
		m_length = 0;
		m_cache.RemoveAll();
	}
	//
	bool m_AddBaseFrame(CBitmapHandle bmp, UINT count, const IBitmapWipable* pWiper)
	{
		if ( ! bmp.IsNull() )
		{
			if ( count == 0 ) 
			{
				count = 1;
			}
			TBaseFrame b;
			b.bmp = bmp;
			b.count = count;
			if ( pWiper != NULL )
			{
				b.pWiper = pWiper->Clone();
			}
			m_frames.Add(b);
			m_length += b.count;
			return true;
		}
		return false;
	}
public:

	/// �R���X�g���N�^
	CBitmapAnimater(void) : m_length(0)
	{
	}

	/**
	 * [�ݒ�] �ŏ��̃t���[���摜�ݒ�
	 *	@note ��������s����ƁA���܂ł̏��̓N���A����܂��B
	 *	@param bmp �r�b�g�}�b�v
	 *	@param defaultWiper AddBaseFrame() �Ń��C�p�[���ȗ����ꂽ���Ɏg�p����郏�C�p�[�B
	 *				�������L�����܂��̂ŁA�j�����Ă��܂��܂���B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetFirstFrame(CBitmapHandle bmp, const IBitmapWipable& defaultWiper)
	{
		m_RemoveAll();
		return m_AddBaseFrame(bmp, 1, &defaultWiper);
	}

	/**
	 * [�ݒ�] �ŏ��̃t���[���摜�ݒ�
	 *	@note ��������s����ƁA���܂ł̏��̓N���A����܂��B
	 *	@param bmp �r�b�g�}�b�v
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetFirstFrame(CBitmapHandle bmp)
	{
		m_RemoveAll();
		CPenetrateWiper w;
		return m_AddBaseFrame(bmp, 1, &w);
	}

	/**
	 * [�ݒ�] ���̃x�[�X�t���[���摜�ݒ�
	 *	@param bmp �r�b�g�}�b�v
	 *	@param count ���t���[���ǉ����邩�B 1 �Ȃ烏�C�p�[�͎g�킸�A�؂�ւ��܂��B
	 *	@param wiper ��O�̃x�[�X�摜����{�摜�̐؂�ւ��Ŏg�����C�p�[�B
	 *				�������L�����܂��̂ŁA�j�����Ă��܂��܂���B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool AddBaseFrame(CBitmapHandle bmp, UINT count, const IBitmapWipable& wiper)
	{
		if ( m_length > 0 )
		{
			return m_AddBaseFrame(bmp, count, &wiper);
		}
		return false;
	}

	/**
	 * [�ݒ�] ���̃x�[�X�t���[���摜�ݒ�
	 *	@note ���C�p�[�� SetFirstFrame() ���Ɏw�肵�����̂��g���܂��B
	 *	@param bmp �r�b�g�}�b�v
	 *	@param count ���t���[���ǉ����邩�B 1 �Ȃ烏�C�p�[�͎g�킸�A�؂�ւ��܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool AddBaseFrame(CBitmapHandle bmp, UINT count = 1)
	{
		if ( m_length > 0 )
		{
			return m_AddBaseFrame(bmp, count, NULL);
		}
		return false;
	}

	/**
	 * [�擾] �t���[�����擾
	 *	@return �t���[�����B
	 */
	size_t GetLength(void) const
	{
		return m_length;
	}

	/**
	 * [�擾] �t���[���擾
	 *	@note �擾���ɉ摜���������܂��B
	 *	@param index �t���[��NO�B�O�ȏ�A GetLength() �̒l�����ł��B
	 *	@return �t���[���摜�B
	 */
	CBitmapHandle FrameAt(INDEX index) const
	{
		if ( index >= m_length ) { return CBitmapHandle(); }
		const TBaseFrame* P = NULL;
		loop ( i, m_frames.GetSize() )
		{
			P = &m_frames[i];
			if ( index < P->count )
			{
				break;
			}
			index -= P->count;
		}
		ASSERTLIB( P != NULL );
		if ( P->count == 1 )
		{
			return P->bmp;
		}
		const IBitmapWipable* pWiper = P->pWiper;
		if ( pWiper == NULL )
		{
			pWiper = m_frames[0].pWiper;
		}
		if ( pWiper == NULL || index + 1 == P->count )
		{
			return P->bmp;
		}
		return pWiper->Wipe((P - 1)->bmp, P->bmp, down_cast<UINT>(index + 1), P->count);
	}

	/**
	 * [�擾] �t���[���擾
	 *	@note ���߂Ă̎擾���ɉ摜���������A�L���b�V�����܂��B
	 *	@param index �t���[��NO�B�O�ȏ�A GetLength() �̒l�����ł��B
	 *	@return �t���[���摜�B
	 */
	CBitmapHandle FrameAtWithCache(INDEX index)
	{
		CBitmapHandle b;
		if ( index < m_length )
		{
			if ( m_cache.GetSize() != m_length )
			{
				m_cache.SetSize(m_length);
			}
			b = m_cache.At(index);
			if ( b.IsNull() )
			{
				b = FrameAt(index);
				m_cache.Set(index, b);
			}
		}
		return b;
	}

	/// [����] �L���b�V������
	void FlashCache(void)
	{
		m_cache.RemoveAll();
	}
};



}; //TNB
