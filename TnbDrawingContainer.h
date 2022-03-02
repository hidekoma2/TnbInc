#pragma once
/**
 *@file
 * �`��R���e�i�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSimpleVector.h"
#include "TnbBitmapDrawer.h"



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE BITMAP
 * �`��R���e�i�N���X
 *
 *		{@link IDrawable �`����}�ƍ��W��΂ɕ����ێ�����N���X�ł��B
 *
 *		INDEX ��{@link IDrawable �`����}���Ǘ����邱�Ƃ��\�ł��B
 *
 *		�Ǘ����Ă���{@link IDrawable �`����}���A�܂Ƃ߂āA�f�o�C�X�R���e�L�X�g�ɕ`�悷�邱�Ƃ��\�ł��B
 *
 *	@note	�{�N���X�ɕ`�����ǉ�����ہA���΍��W�������ɋL�����Ă��邽�߁A
 *			Draw() �ŕ`�掞�ɂ��ꂼ��̑��΍��W�ɕ`�悷�邱�Ƃ��ł��܂��B
 *
 *	@note	���΍��W�́A�C���X�^���X�������͍���i0,0�j�ɂȂ��Ă��܂��B
 *			AddCloneObject() �� AddNewPointer() , AddBitmap() , AppendBitmaps() , MoveAppendPosition() , 
 *			SetAppendPosition() , OffsetAppendPosition() �ŔC�ӂɈړ����܂��B
 *
 *	@note	�w��� �C���f�b�N�X�ɑ΂���ʒu�ɑ΂��鑊�΍��W�ł��A�ǉ��\�ł��B( ELocation )
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *		//= �錾
 *		CDrawingContainer m_bmpc; // �R���e�i�쐬
 *
 *		//= �ݒ�
 *		m_bmpc.AddBitmap("c:\\BMP\\BackTop1.bmp", bc.RIGHT);  //BackTop1.bmp�o�^��AOffset��^�E�ɁB
 *		m_bmpc.AddBitmap("c:\\BMP\\BackTop2.bmp", bc.RETURN);  //BackTop2.bmp�o�^��AOffset�����̍��ցB		
 *		m_bmpc.AddBitmap("c:\\BMP\\Back.bmp", bc.DOWN);  //Back.bmp�o�^��AOffset��^���ցB
 *		m_bmpc.AddBitmap("c:\\BMP\\BackBottom.bmp");  //BackBottom.bmp�o�^�B
 *
 *		//= �\�� (�f�o�C�X�R���e�L�X�g�ɕ\�� )
 *		void CXXX::OnPaint() { 
 *			CPaintDC dc(this);
 *			m_bmpc.Draw(dc);
 *		}
 *		</pre></td></tr>
 *
 *	@par�K�v�t�@�C��
 *			TnbDrawingContainer.h
 *
 *	@date 07/10/03 �V�K�쐬
 *	@date 07/11/02 ���ۉ����邽�߂ɁA IDrawable �̃R���e�i�ɕύX�B
 *	@date 07/11/17 BitmapConainer �ƈ�{���B
 *	@date 07/11/20 ELocation �ǉ��B CNullDrawer ���z���B
 *	@date 07/12/12 GetAllRgn() �ǉ��B
 *	@date 07/12/19 Resize() �ɑΉ��B
 */
class CDrawingContainer : public IDrawable
{
	DEFSUPER(IDrawable);
public:

	/// �R���X�g���N�^
	CDrawingContainer(void)
	{
		m_appendPos.x = 0;
		m_appendPos.y = 0;
	}

	/// �f�X�g���N�^
	virtual ~CDrawingContainer(void)
	{
		m_drawers.RemoveAll();
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		CDrawingContainer* pNew = new CDrawingContainer();
		pNew->m_appendPos = m_appendPos;
		pNew->m_drawers.RemoveAll();
		size_t l = m_drawers.GetSize();
		pNew->m_drawers.SetSize(l);
		loop ( i, l )
		{
			pNew->m_drawers[i] = m_drawers[i];
		}
		return pNew;
	}

	/**
	 * [�擾] �T�C�Y�擾.
	 *		�{�I�u�W�F�N�g�̕`�悷��̈�̊O�ڂ���l�p�̃T�C�Y�𓾂邱�Ƃ��o���܂��B
	 *	@note ��ԍ���ɕ`�悷��`���񂪂Ȃ��Ă��A���ォ��E���̃T�C�Y�ɂȂ�܂��B
	 *	@param[out] _size �T�C�Y�擾�B
	 *	@retval true �T�C�Y�擾�����B 
	 *	@retval false �f�[�^���Ȃ��B
	 */
	virtual bool GetSize(SIZE& _size) const
	{
		RECT rc;
		if ( GetAllRect(rc) )
		{
			_size.cx = rc.right;
			_size.cy = rc.bottom;
			return true;
		}
		return false;
	}

	/**
	 * [�ݒ�] �T�C�Y�ݒ�.
	 *		�{�I�u�W�F�N�g�̕`�悷��̈�̊O�ڂ���l�p�̃T�C�Y��ݒ肷�邱�Ƃ��o���܂��B
	 *	@note �Ǘ����Ă���`����̂����ꂩ���T�C�Y�ݒ�ɑΉ����Ă��Ȃ��Ă��A
	 *			�{�N���X�̖{���\�b�h�͐������܂��B�T�C�Y�ݒ�ł�����̂̂݁A�w��T�C�Y���ɓ���悤�ɂ��܂��B
	 *	@param size �T�C�Y�B
	 *	@retval true �T�C�Y�ύX�����B 
	 *	@retval false ���s�B(�{�N���X�ł͎��s�͂���܂���)
	 */
	virtual bool Resize(const SIZE& size)
	{
		loop ( i, m_drawers.GetSize() )
		{
			TParam& A = m_drawers[i];
			SIZE sz = A.orgSize;
			if ( sz.cx + A.pos.x > size.cx )
			{
				sz.cx = size.cx - A.pos.x;
				if ( sz.cx < 0 )
				{
					sz.cx = 0;
				}
			}
			if ( sz.cy + A.pos.y > size.cy )
			{
				sz.cy = size.cy - A.pos.y;
				if ( sz.cy < 0 )
				{
					sz.cy = 0;
				}
			}
			A.pDrawer->Resize(sz);
		}
		return true;
	}

	/**
	 * [�ݒ�] �T�C�Y���Z�b�g.
	 *		Resize() �̐ݒ�𖳌��ɂ��A�o�^���̃T�C�Y�ɖ߂��܂��B
	 */
	void DefaultSize(void)
	{
		loop ( i, m_drawers.GetSize() )
		{
			TParam& A = m_drawers[i];
			A.pDrawer->Resize(A.orgSize);
		}
	}

	/**
	 * [�쐬] �r�b�g�}�b�v�쐬.
	 *		�{�I�u�W�F�N�g�ŊǗ����Ă���`���񂩂�A�r�b�g�}�b�v���쐬���Ԃ��܂��B
	 *	@param color �w�i�F���w�肵�܂��B�ȗ�����ƕs��i�h��Ԃ����Ȃ�) �ɂȂ�܂��B
	 *				�S�G���A�`�悷��`����̏ꍇ�A�w�i�F���w�肷��K�v�͂���܂���B
	 *	@return �r�b�g�}�b�v�B�����񂪂Ȃ��ꍇ�A NULL �r�b�g�}�b�v���Ԃ�܂��B
	 */
	CBitmapHandle CreateBitmap(COLORREF color = CLR_INVALID) const
	{
		return CBitmapDrawer::ToBitmap(*this, color);
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
		DrawEx(dc, x, y, 0);
	}

	/**
	 * [�`��] �`��.
	 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��̍��W�ɕ`�悵�܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g
	 *	@param x �w���W
	 *	@param y �x���W
	 *	@param lParam LPARAM�B�Ǘ����Ă���`����ɓn���܂��B
	 */
	virtual void DrawEx(HDC dc, int x, int y, LPARAM lParam) const
	{
		if ( dc == NULL )
		{
			loop ( i, m_drawers.GetSize() )
			{
				const TParam& A = m_drawers.At(i);
				A.pDrawer->DrawEx(dc, x + A.pos.x, y + A.pos.y, lParam);
			}
		}
		else
		{
			loop ( i, m_drawers.GetSize() )
			{
				const TParam& A = m_drawers.At(i);
				SIZE size;
				if ( A.pDrawer->GetSize(size) )
				{
					RECT rc = { x + A.pos.x, y + A.pos.y, x + A.pos.x + size.cx, y + A.pos.y + size.cy };
					if ( ::RectVisible(dc, &rc) )
					{
						A.pDrawer->DrawEx(dc, rc.left, rc.top, lParam);
					}
				}
			}
		}
	}

	/**
	 * [�擾] �i�[���擾.
	 *		�i�[���Ă���r�b�g�}�b�v����Ԃ��܂��B
	 *	@return ���B
	 */
	size_t GetCount(void) const
	{
		return m_drawers.GetSize();
	}
	
	/**
	 * [�Q��] �w��INDEX�̎Q��.
	 *	@param index �C���f�b�N�X.
	 *	@retval NULL �G���[�B�C���f�b�N�X�w�肪�ُ�B
	 *	@retval �`����B
	 */
	const IDrawable* At(INDEX index) const
	{
		if ( m_drawers.IsInRange(index) )
		{
			return m_drawers.At(index).pDrawer;
		}
		return NULL;
	}

	/**
	 * [�폜] �w��INDEX�̍폜.
	 *	@param index �C���f�b�N�X.
	 *	@retval true �����B
	 *	@retval false ���s�B�C���f�b�N�X�w�肪�ُ�B
	 */
	bool RemoveAt(INDEX index)
	{
		return m_drawers.Remove(index);
	}

	/**
	 * [�폜] ���ׂč폜.
	 *		�i�[���Ă���`��������ׂĔj�����A�ǉ����΍��W������i0,0�j �ɂ��܂��B
	 */
	void RemoveAll(void)
	{
		m_drawers.RemoveAll();
		m_appendPos.x = 0;
		m_appendPos.y = 0;
	}

	/**
	 * [�擾] �r�b�g�}�b�v�擾.
	 *		�w��INDEX�ɓo�^����Ă���`����̃r�b�g�}�b�v���擾���邱�Ƃ��o���܂��B
	 *	@note �r�b�g�}�b�v�n���h���́A�ꎞ�I�Ȃ��̂̉\��������܂��B
	 *	@param index �C���f�b�N�X�B
	 *	@return �r�b�g�}�b�v�B�����Ă��Ȃ��ꍇ��C���f�b�N�X���ُ�̏ꍇ�A NULL �r�b�g�}�b�v���Ԃ�܂��B
	 */
//	CBitmapHandle GetAtBitmap(INDEX index) const
//	{
//		if ( m_drawers.IsInRange(index) )
//		{
//			const TParam& A = m_drawers.At(index);
//			return A.pDrawer->GetBitmap();
//		}
//		return CBitmapHandle();
//	}

	/**
	 * [�擾] ��`�͈͎擾.
	 *		�w��INDEX�ɓo�^����Ă���`����͈̔͂��킩��܂��B
	 *	@param[in] index �C���f�b�N�X�B
	 *	@param[out] _rect ��`�͈́B
	 *	@retval true �����B
	 *	@retval false ���s�B�C���f�b�N�X�w�肪�ُ�B
	 */
	bool GetAtRect(INDEX index, RECT& _rect) const
	{
		if ( ! m_drawers.IsInRange(index) ) { return false; }
		const TParam& A = m_drawers.At(index);
		SIZE size;
		bool r = A.pDrawer->GetSize(size);
		if ( r )
		{
			::SetRect(&_rect, A.pos.x, A.pos.y, A.pos.x + size.cx, A.pos.y + size.cy);
		}
		else
		{
			::SetRectEmpty(&_rect);
		}
		return r;
	}

	/**
	 * [�擾] ��`�͈͎擾.
	 *		�o�^����Ă���`����̕`��͈͂��킩��܂��B
	 *	@param[out] _rect ��`�͈́B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool GetAllRect(RECT& _rect) const
	{
		bool r = true;
		::SetRectEmpty(&_rect);
		loop ( i, m_drawers.GetSize() )
		{
			RECT rc;
			r &= GetAtRect(i, rc);
			::UnionRect(&_rect, &_rect, &rc);
		}
		return r;
	}

	/**
	 * [�擾] �̈�͈͎擾.
	 *		�o�^����Ă���`����̕`��̈悪�킩��܂��B
	 *	@retval NULL ���s�B
	 *	@retval NULL�ȊO ���[�W�����B�g�p��́A�j������K�v������܂��B
	 */
	HRGN GetAllRgn(void) const
	{
		if ( GetCount() == 0 ) { return NULL; }
		HRGN rgn = ::CreateRectRgn(0, 0, 0, 0);
		loop ( i, GetCount() )
		{
			RECT r;
			if ( GetAtRect(i, r) )
			{
				HRGN rgnTemp = ::CreateRectRgnIndirect(&r);
				::CombineRgn(rgn, rgn, rgnTemp, RGN_OR);
				_DeleteObject(rgnTemp);
			}
		}
		return rgn;
	}

	/**
	 * [�`��] �`��.
	 *		�w��INDEX�ɓo�^����Ă���`������w��̃f�o�C�X�R���e�L�X�g�̎w��̍��W�ɕ`�悵�܂��B
	 *	@param[in] index �C���f�b�N�X�B
	 *	@param dc �f�o�C�X�R���e�L�X�g
	 *	@param x �w���W
	 *	@param y �x���W
	 */
	void DrawAt(INDEX index, HDC dc, int x = 0, int y = 0) const
	{
		if ( m_drawers.IsInRange(index) )
		{
			const TParam& A = m_drawers.At(index);
			A.pDrawer->Draw(dc, x, y);
		}
	}

	/**
	 * [�擾] �ǉ����΍��W�擾.
	 *		���ɒǉ�����`����̑��΍��W�ɂȂ��񂪓����܂��B
	 *	@return �ǉ����΍��W
	 */
	const POINT& GetAppendPosition(void) const
	{
		return m_appendPos;
	}

	/**
	 * [����] �ǉ����΍��W�ړ�.
	 *		���ɒǉ�����`����̑��΍��W�����炷���Ƃ��\�ł��B
	 *	@param x �w������OFFSET�l�B
	 *	@param y �x������OFFSET�l�B
	 */
	void OffsetAppendPosition(int x, int y)
	{
		m_appendPos.x += x;
		m_appendPos.y += y;
	}

	/**
	 * [����] �ǉ����΍��W�ݒ�.
	 *		���ɒǉ�����`����̑��΍��W��ݒ肷�邱�Ƃ��\�ł��B
	 *	@param x �w�����̈ʒu�B
	 *	@param y �x�����̈ʒu�B
	 */
	void SetAppendPosition(int x, int y)
	{
		m_appendPos.x = x;
		m_appendPos.y = y;
	}

	/// �����l
	enum EDirection
	{	
		DOWN,	///< ���ֈړ�
		RIGHT,	///< �E�ֈړ�
		RETURN,	///< ���ֈړ����Ĉ�ԍ���
		VRETURN,///< �E�ֈړ����Ĉ�ԏ��
		STAY	///< �ړ��Ȃ�
	};

	/**
	 * [�ړ�] �ǉ����΍��W�ݒ�.
	 *		���ɒǉ�����`����̑��΍��W��ݒ肷�邱�Ƃ��\�ł��B
	 *	@note	�w��C���f�b�N�X�ɓo�^����Ă���`����Ƒ��΍��W����A
	 *			�w��̕����Ɉړ��������W�ɁAOFFSET�����������W��ǉ����΍��W�ɂ��܂��B
	 *	@param index �C���f�b�N�X.
	 *	@param dire �ړ�����.
	 *	@param x �w������OFFSET�l�B
	 *	@param y �x������OFFSET�l�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool MoveAppendPosition(INDEX index, EDirection dire = DOWN, int x = 0, int y = 0)
	{
		if ( m_drawers.IsInRange(index) )
		{
			const TParam& A = m_drawers.At(index);
			SIZE size;
			if ( A.pDrawer->GetSize(size) )
			{
				m_appendPos.x = A.pos.x + x;
				m_appendPos.y = A.pos.y + y;
				m_Offset(dire, size.cx, size.cy);
				return true;
			}
		}
		return false;
	}

	/// �ʒu�w��l
	enum ELocation
	{
		RIGHTTOP,		///< �゠�킹�ŁA�E�ɒǉ��B
		RIGHTCENTER,	///< �������킹�ŁA�E�ɒǉ��B
		RIGHTBOTTOM,	///< �����킹�ŁA�E�ɒǉ��B
		//
		DOWNLEFT,		///< �����킹�ŁA���ɒǉ��B
		DOWNCENTER,		///< �������킹�ŁA���ɒǉ��B
		DOWNRIGHT,		///< �E���킹�ŁA���ɒǉ��B
		//
		CENTER,			///< �����ɒǉ��B
	};

	/**
	 * [�ǉ�] �`����ǉ�.
	 *		�ǉ����΍��W�Ɏw��̕`������֘A�Â��ċL�����܂��B
	 *	@note �ǉ��������ɃC���f�b�N�X 0, 1, 2, ��� �Ɗ���U���܂��B
	 *	@param pDraw �`����. �K���A new �ō쐬�����C���X�^���X��n���Ă��������B
	 *				�j���͖{�I�u�W�F�N�g���s���܂��i�{���\�b�h�����s�������͑��j�����܂��j�B
	 *	@param dire �ړ�����.
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval INVALID_INDEX�ȊO �����B�l�́A�ǉ������C���f�b�N�X�ł��B
	 */
	INDEX AddNewPointer(IDrawable* pDraw, EDirection dire = DOWN)
	{
		TParam p;
		if ( pDraw != NULL && pDraw->GetSize(p.orgSize) )
		{
			p.pos = m_appendPos;
			p.pDrawer = pDraw;
			INDEX r = m_drawers.Add(p);
			if ( r != INVALID_INDEX )
			{
				m_Offset(dire, p.orgSize.cx, p.orgSize.cy);
				return r;
			}
		}
		delete pDraw;
		return INVALID_INDEX;
	};

	/**
	 * [�ǉ�] �`����ǉ�.
	 *		�w��̍��W�Ɏw��̕`������֘A�Â��ċL�����܂��B
	 *	@note	�w��C���f�b�N�X�ɓo�^����Ă���`����Ƒ��΍��W����A
	 *			�w��̈ʒu�ɉ摜����ǉ����܂��B
	 *	@note �ǉ��������ɃC���f�b�N�X 0, 1, 2, ��� �Ɗ���U���܂��B
	 *	@param index �C���f�b�N�X.
	 *	@param locate �ʒu
	 *	@param pDraw �`����. �K���A new �ō쐬�����C���X�^���X��n���Ă��������B
	 *				�j���͖{�I�u�W�F�N�g���s���܂��i�{���\�b�h�����s�������͑��j�����܂��j�B
	 *	@param dire �ړ�����.
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval INVALID_INDEX�ȊO �����B�l�́A�ǉ������C���f�b�N�X�ł��B
	 */
	INDEX AddNewPointer(INDEX index, ELocation locate, IDrawable* pDraw, EDirection dire = DOWN)
	{
		if ( ! m_Locate(index, locate, pDraw) ) { return INVALID_INDEX; }
		return AddNewPointer(pDraw, dire);
	}
	
	/**
	 * [�ǉ�] �`����ǉ�.
	 *		�ǉ����΍��W�Ɏw��̕`������֘A�Â��ċL�����܂��B
	 *	@note �ǉ��������ɃC���f�b�N�X 0, 1, 2, ��� �Ɗ���U���܂��B
	 *	@param draw �`����. �{�I�u�W�F�N�g�ɂ̓N���[�����L�������̂ŁA draw �͔j�����Ă����܂��܂���B
	 *	@param dire �ړ�����.
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval INVALID_INDEX�ȊO �����B�l�́A�ǉ������C���f�b�N�X�ł��B
	 */
	INDEX Add(const IDrawable& draw, EDirection dire = DOWN)
	{
		return AddNewPointer(draw.Clone(), dire);
	};

	/**
	 * [�ǉ�] �`����ǉ�.
	 *		�w��̍��W�Ɏw��̕`������֘A�Â��ċL�����܂��B
	 *	@note	�w��C���f�b�N�X�ɓo�^����Ă���`����Ƒ��΍��W����A
	 *			�w��̈ʒu�ɉ摜����ǉ����܂��B
	 *	@note �ǉ��������ɃC���f�b�N�X 0, 1, 2, ��� �Ɗ���U���܂��B
	 *	@param index �C���f�b�N�X.
	 *	@param locate �ʒu
	 *	@param draw �`����. �{�I�u�W�F�N�g�ɂ̓N���[�����L�������̂ŁA draw �͔j�����Ă����܂��܂���B
	 *	@param dire �ړ�����.
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval INVALID_INDEX�ȊO �����B�l�́A�ǉ������C���f�b�N�X�ł��B
	 */
	INDEX Add(INDEX index, ELocation locate, const IDrawable& draw, EDirection dire = DOWN)
	{
		return AddNewPointer(index, locate, draw.Clone(), dire);
	};

	/**
	 * [�ǉ�] �r�b�g�}�b�v�`����ǉ�.
	 *		�ǉ����΍��W�Ɏw��̕`������֘A�Â��ċL�����܂��B
	 *	@note �ǉ��������ɃC���f�b�N�X 0, 1, 2, ��� �Ɗ���U���܂��B
	 *	@param bmp �r�b�g�}�b�v�B
	 *	@param dire �ړ�����.
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval INVALID_INDEX�ȊO �����B�l�́A�ǉ������C���f�b�N�X�ł��B
	 */
	INDEX AddBitmap(CBitmapHandle bmp, EDirection dire = DOWN)
	{
		return AddNewPointer(new CBitmapDrawer(bmp), dire);
	}

	/**
	 * [�ǉ�] �r�b�g�}�b�v�`����ǉ�.
	 *		�w����W�Ɏw��̕`������֘A�Â��ċL�����܂��B
	 *	@note �ǉ��������ɃC���f�b�N�X 0, 1, 2, ��� �Ɗ���U���܂��B
	 *	@note	�w��C���f�b�N�X�ɓo�^����Ă���`����Ƒ��΍��W����A
	 *			�w��̈ʒu�ɉ摜����ǉ����܂��B
	 *	@param index �C���f�b�N�X.
	 *	@param locate �ʒu
	 *	@param bmp �r�b�g�}�b�v�B
	 *	@param dire �ړ�����.
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval INVALID_INDEX�ȊO �����B�l�́A�ǉ������C���f�b�N�X�ł��B
	 */
	INDEX AddBitmap(INDEX index, ELocation locate, CBitmapHandle bmp, EDirection dire = DOWN)
	{
		return AddNewPointer(index, locate, new CBitmapDrawer(bmp), dire);
	}

	/**
	 * [�ǉ�] �ꊇ�r�b�g�}�b�v�t�@�C���Ǎ���.
	 *		�w��t�@�C�����Ɋւ���r�b�g�}�b�v�t�@�C����ǂݍ��݂܂��B
	 *	@note �ŏ��� RemoveAll() �����s����܂��B
	 *
	 *	@par�g�p��
	 *		Back001a.bmp , Back002.bmp , Back004.bmp , Back006_DC.bmp , Back010.bmp �̃t�@�C��������ꍇ�A
	 *		�ȉ��̃R�[�h�����s����ƁA INDEX 1 �� Back001a.bmp �AINDEX 2 �� Back002.bmp �A
	 *		INDEX 4 �� Back004.bmp �AINDEX 6 �� Back006_DC.bmp �AINDEX 10 �� Back010.bmp �����蓖�Ă��܂��B
	 *		�����̃r�b�g�}�b�v�n���h���́A GetAtBitmap() �ŗe�ՂɎ��o�����Ƃ��o���܂��B�܂��A DrawAt() 
	 *		�ŗe�ՂɃf�o�C�X�R���e�L�X�g�ɕ`����o���܂��B
	 *		<table><tr><td><pre>
	 *
	 *		CBitmapContainer m_bmpc; // �R���e�i�쐬
	 *		m_bmpc.AppendBitmaps("c:\\BMP\\Back%03d*.bmp", 10, bc.DOWN);		
	 *		</pre></td></tr></table>
	 *		����L��� Back001a.bmp , Back001b.bmp ������ꍇ�A�ǂ��炪�ǂݍ��܂�邩�͕s��ɂȂ�܂�
	 *		�i��Ɍ����Ō��������ق����g�p����܂��j�B
	 *
	 *	@param lpszPath �p�X���B�t�@�C�����̐����ɂȂ�Ƃ���ɁA ��d �𖄂߂Ă����܂��B
	 *	@param maxIndex �C���f�b�N�X�̍ő�l�B 5 �ł���΁A 0�`5 ��6�������܂��B
	 *	@param dire �ړ�����.
	 *	@retval �}�C�i�X	�G���[�B
	 *	@retval 0			����Ȃ�����
	 *	@retval 1�ȏ�		�ǂݍ��񂾐��BINDEX�̍ő�l�� GetCount() �Ŋm�F���Ă��������B
	 */
	int AppendBitmaps(LPCTSTR lpszPath, INDEX maxIndex, EDirection dire = DOWN)
	{
		RemoveAll();
		WIN32_FIND_DATA wfd;
		SIZE nullSize = { 0, 0 };
		CWorkMemT<TCHAR> str(MAX_PATH + 1);
		INDEX lastValidIndex = INVALID_INDEX;
		int foundCount = 0;
		loop( i, maxIndex + 1 )
		{
			if ( ! STRLIB::PrintF(str, MAX_PATH, lpszPath, i) )
			{
				return -1;
			}
			IDrawable* pDraw = NULL; 
			HANDLE hFind = ::FindFirstFile(str, &wfd);
			if ( hFind != INVALID_HANDLE_VALUE )
			{
				::FindClose(hFind);
				CWorkMemT<TCHAR> s(MAX_PATH + 1);
				STRLIB::Copy(s, str);
				INT_PTR l = STRLIB::PathIndexOf(s) + 1;
				STRLIB::Copy(&s[l], wfd.cFileName);
				CBitmapHandle bmp(s);
				if ( ! bmp.IsNull() )
				{
					pDraw = new CBitmapDrawer(bmp);
					lastValidIndex = i;
					foundCount++;
				}
			}
			if ( pDraw == NULL )
			{
				pDraw = new CNullDrawer(nullSize);
			}
			INDEX idx = AddNewPointer(pDraw, dire);
			ASSERTLIB( idx == i );
		}
		if ( lastValidIndex != INVALID_INDEX )
		{
			while ( RemoveAt(lastValidIndex + 1 ));
		}
		else
		{
			RemoveAll();
		}
		return foundCount;
	}

private:

	/// �Ǘ��p�����[�^
	struct TParam
	{
		POINT						pos;		///< ���΍��W
		SIZE						orgSize;	///< �I���W�i���T�C�Y
		IDrawable::Ptr				pDrawer;	///< �`����
	};
	CSimpleVectorT<TParam>	m_drawers;			///< �`����Q
	POINT					m_appendPos;		///< �ǉ����΍��W
	// OFFSET
	void m_Offset(EDirection dire, int x, int y)
	{
		if ( dire == DOWN ) 
		{
			m_appendPos.y += y;
		}
		else if ( dire == RIGHT ) 
		{
			m_appendPos.x += x;
		}
		else if ( dire == RETURN )
		{ 
			m_appendPos.x = 0;
			m_appendPos.y += y;
		}
		else if ( dire == VRETURN )
		{ 
			m_appendPos.x += x;
			m_appendPos.y = 0;
		}
	}
	/** LOCATE
	 *	@param index �C���f�b�N�X.
	 *	@param locate �ʒu
	 *	@param pDraw �`����
	 */
	bool m_Locate(INDEX index, ELocation locate, const IDrawable* pDraw)
	{
		if ( ! m_drawers.IsInRange(index) ) { return false; }
		const TParam& A = m_drawers.At(index);
		SIZE baseSize;
		if ( ! A.pDrawer->GetSize(baseSize) ) { return false; }
		SIZE mySize;
		if ( ! pDraw->GetSize(mySize) ) { return false; }
		m_appendPos.x = A.pos.x;
		m_appendPos.y = A.pos.y;
		// Y����
		if ( locate == RIGHTCENTER || locate == CENTER )
		{
			m_appendPos.y += (baseSize.cy - mySize.cy) / 2;
		}
		else if( locate == RIGHTBOTTOM )
		{
			m_appendPos.y += baseSize.cy - mySize.cy;
		}
		else if ( locate != RIGHTTOP )//���ɒǉ�
		{
			m_appendPos.y += baseSize.cy;
		}
		// X����
		if ( locate == DOWNCENTER || locate == CENTER )
		{
			m_appendPos.x += (baseSize.cx - mySize.cx) / 2;
		}
		else if( locate == DOWNRIGHT )
		{
			m_appendPos.x += baseSize.cx - mySize.cx;
		}
		else if ( locate != DOWNLEFT )//�E�ɒǉ�
		{
			m_appendPos.x += baseSize.cx;
		}
		return true;
	}
};



};///TNB

