#pragma once
/**
 *@file
 * �`����֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbPointerHandle.h"
#include "TnbDcSelectAssistant.h"



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE
 * �`����C���^�[�t�F�[�X
 *
 *		�`������Ǘ����Ă���C���^�[�t�F�[�X�ł��B
 *
 *		�{�C���^�[�t�F�[�X���������邱�ƂŁA CDrawingContainer �ȂǂŊǗ��A�����ł��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDrawable.h
 *
 *	@date 07/11/01 �V�K�쐬
 *	@date 07/11/19 HasRgn() �p�~
 *	@date 07/11/30 ���O�ύX�B
 */
struct IDrawable
{
	/// �f�X�g���N�^
	virtual ~IDrawable(void) { }

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const = 0;

	/**
	 * [�擾] �T�C�Y�擾.
	 *		�{�I�u�W�F�N�g�̕`�悷��̈�̊O�ڂ���l�p�̃T�C�Y�𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _size �T�C�Y�擾�B
	 *	@retval true �T�C�Y�擾�����B 
	 *	@retval false �f�[�^���Ȃ��B
	 */
	virtual bool GetSize(SIZE& _size) const = 0;

	/**
	 * [�ݒ�] �T�C�Y�ݒ�.
	 *		�{�I�u�W�F�N�g�̕`�悷��̈�̊O�ڂ���l�p�̃T�C�Y��ݒ肷�邱�Ƃ��o���܂��B
	 *	@param size �T�C�Y�B
	 *	@retval true �T�C�Y�ύX�����B 
	 *	@retval false ���s�B�T�C�Y�ύX�̓T�|�[�g���܂���B
	 */
	virtual bool Resize(const SIZE& size) = 0;

	/**
	 * [�`��] �`��.
	 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��̍��W�ɕ`�悵�܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g
	 *	@param x �w���W
	 *	@param y �x���W
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const = 0;

	/**
	 * [�`��] �`��.
	 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��̍��W�ɕ`�悵�܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g
	 *	@param x �w���W
	 *	@param y �x���W
	 *	@param lParam LPARAM�B
	 */
	virtual void DrawEx(HDC dc, int x, int y, LPARAM lParam) const
	{
		Draw(dc, x, y);
	}

	/// �|�C���^�n���h���^�錾
	typedef CPointerHandleT<IDrawable> Ptr;
};



/**@ingroup DRAWABLE
 * NULL�`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *		�傫���݂̂�ێ����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDrawable.h
 *
 *	@date 07/11/21 �V�K�쐬
 */
class CNullDrawer : public IDrawable
{
	DEFSUPER(IDrawable);
protected:
	SIZE			m_size;		///< �T�C�Y
public:

	/**
	 * �R���X�g���N�^
	 *	@param size �T�C�Y�B
	 */
	CNullDrawer(const SIZE& size) : m_size(size)
	{
	}

	/**
	 * �R���X�g���N�^.
	 *	@note �T�C�Y�� 0, 0 �ɂȂ�܂��B
	 */
	CNullDrawer(void)
	{
		m_size.cx = 0;
		m_size.cy = 0;
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const 
	{
		return new CNullDrawer(m_size); 
	}

	/**
	 * [�擾] �T�C�Y�擾.
	 *		�{�I�u�W�F�N�g�̕`�悷��̈�̊O�ڂ���l�p�̃T�C�Y�𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _size �T�C�Y�擾�B
	 *	@retval true �T�C�Y�擾�����B 
	 *	@retval false �f�[�^���Ȃ��B
	 */
	virtual bool GetSize(SIZE& _size) const
	{
		_size = m_size;
		return true;
	}

	/**
	 * [�ݒ�] �T�C�Y�ݒ�.
	 *		�{�I�u�W�F�N�g�̕`�悷��̈�̊O�ڂ���l�p�̃T�C�Y��ݒ肷�邱�Ƃ��o���܂��B
	 *	@param size �T�C�Y�B
	 *	@retval true �T�C�Y�ύX�����B 
	 *	@retval false ���s�B�T�C�Y�ύX�̓T�|�[�g���܂���B
	 */
	virtual bool Resize(const SIZE& size)
	{
		m_size = size;
		return true;
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
	}
};



/**@ingroup DRAWABLE
 * �`����ێ��`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�w�肳�ꂽ�`������ێ����܂��B
 *		�����ׂĕ\�����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDrawable.h
 *
 *	@date 08/09/26 �V�K�쐬
 */
class CPairDrawer : public IDrawable
{
	DEFSUPER(IDrawable);
	IDrawable::Ptr	m_pDrawLeft;	///< ���`����
	IDrawable::Ptr	m_pDrawRight;	///< �E�`����
	size_t			m_orgLeftWidth;	///< ���̕`����̃I���W�i����
	SIZE			m_size;			///< �T�C�Y

public:

	/**
	 * �R���X�g���N�^
	 *	@param drawLeft ���̕`����B������{�N���X�ŕێ����܂��B�I���W�i�������g���܂��̂ŁA�������T�C�Y���w�肵�Ă����K�v������܂��B
	 *	@param drawRight �E�̕`����B������{�N���X�ŕێ����܂��B
	 */
	CPairDrawer(const IDrawable& drawLeft, const IDrawable& drawRight)
	{
		m_pDrawLeft = drawLeft.Clone();
		m_pDrawRight = drawRight.Clone();
		SIZE orgSize;
		m_pDrawLeft->GetSize(orgSize);
		m_orgLeftWidth = orgSize.cx;
		m_size.cx = 0;
		m_size.cy = 0;
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		CPairDrawer* P = new CPairDrawer(*this);
		return P;
	}

	/**
	 * [�擾] �T�C�Y�擾.
	 *		�{�I�u�W�F�N�g�̕`�悷��̈�̊O�ڂ���l�p�̃T�C�Y�𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _size �T�C�Y�擾�B
	 *	@retval true �T�C�Y�擾�����B 
	 *	@retval false �f�[�^���Ȃ��B
	 */
	virtual bool GetSize(SIZE& _size) const
	{
		_size = m_size;
		return true;
	}
	
	/**
	 * [�ݒ�] �T�C�Y�ݒ�.
	 *		�{�I�u�W�F�N�g�̕`�悷��̈�̊O�ڂ���l�p�̃T�C�Y��ݒ肷�邱�Ƃ��o���܂��B
	 *	@note �{�N���X�̓T�C�Y�ύX���T�|�[�g���܂���B
	 *	@param size �T�C�Y�B
	 *	@retval true �T�C�Y�ύX�����B 
	 *	@retval false ���s�B�T�C�Y�ύX�̓T�|�[�g���܂���B
	 */
	virtual bool Resize(const SIZE& size)
	{
		m_size = size;
		SIZE s1 = { min(ToInt(m_orgLeftWidth), size.cx), size.cy };
		SIZE s2 = { max(down_cast<LONG>(size.cx - m_orgLeftWidth), 0), size.cy };
		m_pDrawLeft->Resize(s1);
		m_pDrawRight->Resize(s2);
		return true;
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
		SIZE s1;
		m_pDrawLeft->GetSize(s1);
		m_pDrawLeft->DrawEx(dc, x, y + (m_size.cy - s1.cy) / 2, lParam);
		SIZE s2;
		m_pDrawRight->GetSize(s2);
		m_pDrawRight->DrawEx(dc, x + s1.cx, y + (m_size.cy - s2.cy) / 2, lParam);
	}
};



/**@ingroup DRAWABLE
 * �`����ێ��`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�w�肳�ꂽ�`������ێ����܂��B
 *		�x�[�X�`����̏�ɁA�w���OFFSET�����炵���I�[�o�[�`��������悤�ɂ��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDrawable.h
 *
 *	@date 07/11/26 �V�K�쐬
 */
class COffsetDrawer : public IDrawable
{
	DEFSUPER(IDrawable);
	IDrawable::Ptr	m_pDrawBase;	///< �x�[�X�`����
	POINT			m_offsetOver;	///< �I�[�o�[�`����I�t�Z�b�g
	IDrawable::Ptr	m_pDrawOver;	///< �I�[�o�[�`����
	SIZE			m_size;			///< �T�C�Y
	void m_CalcSize(void)
	{
		m_size.cx = 0;
		m_size.cy = 0;
		SIZE s1;
		SIZE s2;
		if ( m_pDrawBase->GetSize(s1) && m_pDrawOver->GetSize(s2) )
		{
			RECT r1 = { 0, 0, s1.cx, s1.cy };
			RECT r2 = { m_offsetOver.x, m_offsetOver.y, m_offsetOver.x + s2.cx, m_offsetOver.y + s2.cy };
			if ( ::UnionRect(&r1, &r1, &r2) )
			{
				m_size.cx = r1.right - r1.left;
				m_size.cy = r1.bottom - r1.top;
			}
		}
	}

public:

	/**
	 * �R���X�g���N�^
	 *	@param drawBase �x�[�X�`����B������{�N���X�ŕێ����܂��B
	 *	@param off �I�[�o�[�`����̂���
	 *	@param drawOver �I�[�o�[�`����B������{�N���X�ŕێ����܂��B
	 */
	COffsetDrawer(const IDrawable& drawBase, const POINT& off, const IDrawable& drawOver)
		: m_offsetOver(off)
	{
		m_pDrawBase = drawBase.Clone();
		m_pDrawOver = drawOver.Clone();
		m_CalcSize();
	}

	/**
	 * �R���X�g���N�^.
	 *	@note 
	 *	@param size �T�C�Y�B
	 *	@param off �`����̂���
	 *	@param draw �`����B������{�N���X�ŕێ����܂��B
	 */
	COffsetDrawer(const SIZE& size, const POINT& off, const IDrawable& draw)
		: m_offsetOver(off)
	{
		m_pDrawBase = new CNullDrawer(size);
		m_pDrawOver = draw.Clone();
		m_CalcSize();
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		return new COffsetDrawer(*m_pDrawBase, m_offsetOver, *m_pDrawOver);
	}

	/**
	 * [�擾] �T�C�Y�擾.
	 *		�{�I�u�W�F�N�g�̕`�悷��̈�̊O�ڂ���l�p�̃T�C�Y�𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _size �T�C�Y�擾�B
	 *	@retval true �T�C�Y�擾�����B 
	 *	@retval false �f�[�^���Ȃ��B
	 */
	virtual bool GetSize(SIZE& _size) const
	{
		_size = m_size;
		return true;
	}
	
	/**
	 * [�ݒ�] �T�C�Y�ݒ�.
	 *		�{�I�u�W�F�N�g�̕`�悷��̈�̊O�ڂ���l�p�̃T�C�Y��ݒ肷�邱�Ƃ��o���܂��B
	 *	@note �{�N���X�̓T�C�Y�ύX���T�|�[�g���܂���B
	 *	@param size �T�C�Y�B
	 *	@retval true �T�C�Y�ύX�����B 
	 *	@retval false ���s�B�T�C�Y�ύX�̓T�|�[�g���܂���B
	 */
	virtual bool Resize(const SIZE& size)
	{
		return false;
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
		if ( ! m_pDrawBase.IsNull() ) 
		{
			m_pDrawBase->DrawEx(dc, x, y, lParam);
		}
		if ( ! m_pDrawOver.IsNull() ) 
		{
			m_pDrawOver->DrawEx(dc, x + m_offsetOver.x, y + m_offsetOver.y, lParam);
		}
	}
};



/**@ingroup DRAWABLE
 * �l�p�`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�R���X�g���N�^���Ɏw�肳�ꂽ�F�Ŏl�p��`�悵�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDrawable.h
 *
 *	@date 07/11/15 �V�K�쐬
 *	@date 08/03/23 �R���X�g���N�^��ǉ��B
 */
class CBoxDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
	COLORREF	m_color;	///< �F
	int			m_width;	///< ����
public:

	/**
	 * �R���X�g���N�^
	 *	@param size �T�C�Y�B
	 *	@param color �F�B
	 *	@param width ���̕��B�ȗ�����ƂP�D
	 */
	CBoxDrawer(const SIZE& size, COLORREF color, int width = 1) 
		: _super(size), m_color(color), m_width(width)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@note �T�C�Y�� 0, 0 �ɂȂ�܂��B
	 *	@param color �F�B
	 *	@param width ���̕��B�ȗ�����ƂP�D
	 */
	CBoxDrawer(COLORREF color, int width = 1) 
		: _super(), m_color(color), m_width(width)
	{
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CBoxDrawer(m_size, m_color, m_width);
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
		int z1 = m_width / 2;
		int z2 = (m_width + 1) / 2 - 1;
		HPEN pen = ::CreatePen(PS_SOLID, m_width, m_color);
		CDcSelectAssistant dca(dc);
//		HGDIOBJ oldPen = ::SelectObject(dc, pen);
//		HGDIOBJ oldBrush = ::SelectObject(dc, ::GetStockObject(NULL_BRUSH));
		dca.SelectPen(pen);
		dca.SelectBrush(static_cast<HBRUSH>(::GetStockObject(NULL_BRUSH)));
		::RoundRect(dc, x + z1, y + z1, m_size.cx + x - z2, m_size.cy + y - z2, 0, 0);
//		::SelectObject(dc, oldBrush);
//		::SelectObject(dc, oldPen);
		dca.Restore();
		_DeleteObject(pen);
	}
};



/**@ingroup DRAWABLE
 * �l�p�h��Ԃ��`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�R���X�g���N�^���Ɏw�肳�ꂽ�F�Ŏl�p��`�悵�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDrawable.h
 *
 *	@date 07/11/15 �V�K�쐬
 *	@date 08/03/23 �R���X�g���N�^��ǉ��B
 */
class CBoxFillDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
	COLORREF	m_color;	///< �F
public:

	/**
	 * �R���X�g���N�^
	 *	@param size �T�C�Y�B
	 *	@param color �F�B
	 */
	CBoxFillDrawer(const SIZE& size, COLORREF color) : _super(size), m_color(color)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@note �T�C�Y�� 0, 0 �ɂȂ�܂��B
	 *	@param color �F�B
	 */
	CBoxFillDrawer(COLORREF color) : _super(), m_color(color)
	{
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CBoxFillDrawer(m_size, m_color);
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
		RECT rc = { x, y, x + m_size.cx, y + m_size.cy };
		COLORREF c = ::GetBkColor(dc);
		::SetBkColor(dc, m_color);
		::ExtTextOut(dc, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
		::SetBkColor(dc, c);
	}
};



/**@ingroup DRAWABLE
 * ���]�`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�͈͂̐F�𔽓]���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDrawable.h
 *
 *	@date 07/12/13 �V�K�쐬
 *	@date 08/03/23 �R���X�g���N�^��ǉ��B
 */
class CBoxInverter : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
public:

	/**
	 * �R���X�g���N�^
	 *	@param size �T�C�Y�B
	 */
	CBoxInverter(const SIZE& size) : _super(size)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@note �T�C�Y�� 0, 0 �ɂȂ�܂��B
	 */
	CBoxInverter(void) : _super()
	{
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CBoxInverter(m_size);
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
		RECT rc = { x, y, x + m_size.cx, y + m_size.cy };
		::InvertRect(dc, &rc);
	}
};



/**@ingroup DRAWABLE
 * �t���[���`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�t���[����`�悵�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDrawable.h
 *
 *	@date 08/10/10 �R���X�g���N�^��ǉ��B
 */
class CFrameDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
	bool		m_isFrameMode;
	UINT		m_param1;
	UINT		m_param2;
public:

	/**
	 * �R���X�g���N�^
	 *	@param size �T�C�Y�B
	 *	@param isFrameMode true �Ȃ� DrawFrameControl() �Afalse �Ȃ� DrawEdge() ���g���܂��B
	 *	@param p1 �p�����[�^�P
	 *	@param p2 �p�����[�^�Q
	 */
	CFrameDrawer(const SIZE& size, bool isFrameMode, UINT p1, UINT p2) 
		: _super(size), m_isFrameMode(isFrameMode), m_param1(p1), m_param2(p2)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@note �T�C�Y�� 0, 0 �ɂȂ�܂��B
	 *	@param isFrameMode true �Ȃ� DrawFrameControl() �Afalse �Ȃ� DrawEdge() ���g���܂��B
	 *	@param p1 �p�����[�^�P
	 *	@param p2 �p�����[�^�Q
	 */
	CFrameDrawer(bool isFrameMode, UINT p1, UINT p2) 
		: _super(), m_isFrameMode(isFrameMode), m_param1(p1), m_param2(p2)
	{
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CFrameDrawer(*this);
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
		RECT rc = { x, y, x + m_size.cx, y + m_size.cy };
		m_isFrameMode ? ::DrawFrameControl(dc, &rc, m_param1, m_param2) : ::DrawEdge(dc, &rc, m_param1, m_param2);
	}
};



/**@ingroup DRAWABLE
 * �t�H�[�J�X�t���[���`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�t�H�[�J�X�t���[����`�悵�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDrawable.h
 *
 *	@date 07/12/13 �V�K�쐬
 *	@date 08/03/23 �R���X�g���N�^��ǉ��B
 */
class CFocusFrameDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
public:

	/**
	 * �R���X�g���N�^
	 *	@param size �T�C�Y�B
	 */
	CFocusFrameDrawer(const SIZE& size) : _super(size)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@note �T�C�Y�� 0, 0 �ɂȂ�܂��B
	 */
	CFocusFrameDrawer(void) : _super()
	{
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CFocusFrameDrawer(m_size);
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
		RECT rc = { x, y, x + m_size.cx, y + m_size.cy };
		::DrawFocusRect(dc, &rc);
	}
};



};///TNB

