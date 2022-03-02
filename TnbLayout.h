#pragma once
/**
 *@file
 * ���C�A�E�g�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbPointerHandle.h"



//TNB Library
namespace TNB
{



/**@ingroup LAYOUT
 * ���C�A�E�g�C���^�[�t�F�[�X.
 *
 *		���C�A�E�g���Ǘ����Ă���C���^�[�t�F�[�X�ł��B
 *
 *		�{�C���^�[�t�F�[�X���������邱�ƂŁA CGroupLayout �ȂǂŊǗ��A�����ł��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbLayout.h
 *
 *	@date 08/07/17 �V�K�쐬
 */
struct ILayout
{
	/// �f�X�g���N�^
	virtual ~ILayout(void) {}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual ILayout* Clone(void) const = 0;

	/**
	 * [�擾] �T�C�Y�擾.
	 *		���݂̃T�C�Y�𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _size �T�C�Y�擾�B
	 *	@retval true �T�C�Y�擾�����B 
	 *	@retval false �f�[�^���Ȃ��B
	 */
	virtual bool GetSize(SIZE& _size) const = 0;
	
	/**
	 * [�擾] �ŏ��T�C�Y�擾.
	 *		�ŏ��̃T�C�Y�𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _size �T�C�Y�擾�B
	 *	@retval true �T�C�Y�擾�����B 
	 *	@retval false �f�[�^���Ȃ��B
	 */
	virtual bool GetMinimumSize(SIZE& _size) const = 0;

	/**
	 * [�ݒ�] �T�C�Y�ݒ�.
	 *		���݂̃T�C�Y��ύX���邱�Ƃ��o���܂��B
	 *	@param size �T�C�Y�B
	 *	@retval true �T�C�Y�ύX�����B 
	 *	@retval false ���s�B
	 */
	virtual bool Resize(const SIZE& size) = 0;
	
	/**
	 * [����] ����.
	 *	@note �I�u�W�F�N�g���Ǘ����Ă���T�C�Y�Ǝw��̃T�C�Y�Ƀ��C�A�E�g���܂��B
	 *	@param x  X���W�B-1�Ȃ�w��Ȃ��B
	 *	@param y  Y���W�B-1�Ȃ�w��Ȃ��B
	 *	@param wParam W �p�����[�^�B�B
	 *	@param lParam L �p�����[�^�B
	 */
	virtual void Decide(int x, int y, WPARAM wParam, LPARAM lParam) = 0;

	/// �|�C���^�n���h���^�錾
	typedef CPointerHandleT<ILayout> Ptr;
};



/**@ingroup LAYOUT
 * ���C�A�E�g�X�y�[�T�[.
 *
 *		���C�A�E�g�Ō��Ԃ��󂯂����Ƃ��ɃX�y�[�T�[�Ƃ��Ďg�p�ł��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbLayout.h
 *
 *	@date 09/04/15 �V�K�쐬
 */
class CLayoutSpacer : public ILayout
{
	DEFSUPER(ILayout);
public:

	/**
	 * �R���X�g���N�^
	 *	@param cx �]�T�C�Y
	 *	@param cy �x�T�C�Y
	 */
	CLayoutSpacer(int cx = 0, int cy = 0)
	{
		m_size.cx = cx;
		m_size.cy = cy;
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual ILayout* Clone(void) const
	{
		return new CLayoutSpacer(m_size.cx, m_size.cy);
	}

	/**
	 * [�擾] �T�C�Y�擾.
	 *		���݂̃T�C�Y�𓾂邱�Ƃ��o���܂��B
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
	 * [�擾] �ŏ��T�C�Y�擾.
	 *		�ŏ��̃T�C�Y�𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _size �T�C�Y�擾�B
	 *	@retval true �T�C�Y�擾�����B 
	 *	@retval false �f�[�^���Ȃ��B
	 */
	virtual bool GetMinimumSize(SIZE& _size) const
	{
		return GetSize(_size);
	}

	/**
	 * [�ݒ�] �T�C�Y�ݒ�.
	 *		���݂̃T�C�Y��ύX���邱�Ƃ��o���܂��B
	 *	@param size �T�C�Y�B
	 *	@retval true �T�C�Y�ύX�����B 
	 *	@retval false ���s�B
	 */
	virtual bool Resize(const SIZE& size)
	{
		m_size = size;
		return true;
	}
	
	/**
	 * [����] ����.
	 *	@note �������܂���B
	 *	@param x  X���W�B-1�Ȃ�w��Ȃ��B
	 *	@param y  Y���W�B-1�Ȃ�w��Ȃ��B
	 *	@param wParam W �p�����[�^�B�{�N���X�ł͎g�p���܂���B �{�N���X�ɓo�^�������C�A�E�g�ɂ���Ă͎g�p�����\��������܂��B
	 *	@param lParam L �p�����[�^�B�{�N���X�ł͎g�p���܂���B �{�N���X�ɓo�^�������C�A�E�g�ɂ���Ă͎g�p�����\��������܂��B
	 */
	virtual void Decide(int x, int y, WPARAM wParam, LPARAM lParam)
	{
	}

private:
	SIZE	m_size;		///< �ێ��T�C�Y
};



/**@ingroup LAYOUT
 * ���C�A�E�g�A�C�e�����ۃN���X.
 *
 *		���C�A�E�g���`���Ă���N���X�̒��ۃN���X�ł��B
 *
 *	@note CE�ł́A SetMargnSize() , SetGapSize() �̒l�� 96DPI�̏ꍇ�ł̐��l���w�肵�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbLayout.h
 *
 *	@date 08/07/17 �V�K�쐬
 *	@date 08/07/29 DPI�l��
 */
class CAbstractLayout : public ILayout
{
	DEFSUPER(ILayout);
public:

	/**
	 * ���C�A�E�g�A�C�e���z�u���@.
	 *	@note �z�u����ꏊ�̑傫���ƁA���C�A�E�g�A�C�e���̑傫�����قȂ�P�[�X���������܂��B
	 *		���̍ہA�ǂ̂悤�Ƀ��C�A�E�g�A�C�e����u�������`���Ă��܂��B
	 */
	enum ELocation
	{
		DEFAULT,				///< �������͍��񂹁A�c�����͒���
		TOP,					///< ��񂹁i�c�����p�j
		CENTER,					///< ����
		BOTTOM,					///< ���񂹁i�c�����p�j
		ADJUST,					///< �����iResize�o���Ȃ��ꍇ�����j 
		LEFT = TOP,				///< ���񂹁i�������p�j
		RIGHT = BOTTOM,			///< �E�񂹁i�������p�j
/*		EL_Default,				///< �������͍��񂹁A�c�����͒���
		EL_Top,					///< ��񂹁i�c�����p�j
		EL_Center,				///< ����
		EL_Bottom,				///< ���񂹁i�c�����p�j
		EL_Adjust,				///< �����iResize�o���Ȃ��ꍇ�����j 
		EL_Left = EL_Top,		///< ���񂹁i�������p�j
		EL_Right = EL_Bottom,	///< �E�񂹁i�������p�j
*/	};

	/// �R���X�g���N�^
	CAbstractLayout(void) : m_isValidLayoutSize(false)
	{
		Zero(m_layoutSize);
		Zero(m_gapSize);
		::SetRectEmpty(&m_margn);
	}

	/**
	 * [�ݒ�] �O���}�[�W���ݒ�
	 *	@param rc �}�[�W�� 
	 */
	void SetMargnSize(const RECT& rc)
	{
		m_margn = rc;
		CalcScale(m_margn);
	}
	
	/**
	 * [�ݒ�] �O���}�[�W���ݒ�
	 *	@param mg �}�[�W��
	 */
	void SetMargnSize(int mg)
	{
		::SetRect(&m_margn, mg, mg, mg, mg);
		CalcScale(m_margn);
	}
	
	/**
	 * [�ݒ�] �e�A�C�e���Ԃ̃M���b�v�ݒ�
	 *	@param size �M���b�v
	 */
	void SetGapSize(const SIZE& size)
	{
		m_gapSize = size;
		CalcScale(m_gapSize);
	}

	/**
	 * [�ݒ�] �e�A�C�e���Ԃ̃M���b�v�ݒ�
	 *	@param size �M���b�v
	 */
	void SetGapSize(int size)
	{
		m_gapSize.cx = CalcScaleX(size);
		m_gapSize.cy = CalcScaleY(size);
	}

	/**
	 * [�擾] �T�C�Y�擾.
	 *		���݂̃T�C�Y�𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _size �T�C�Y�擾�B
	 *	@retval true �T�C�Y�擾�����B 
	 *	@retval false �f�[�^���Ȃ��B
	 */
	virtual bool GetSize(SIZE& _size) const
	{
		if ( ! m_isValidLayoutSize )
		{
			if ( ! GetMinimumSize(m_layoutSize) )
			{
				return false;
			}
			m_isValidLayoutSize = true;
		}
		_size = m_layoutSize;
		return true;
	}

	/**
	 * [�ݒ�] �T�C�Y�ݒ�.
	 *		���݂̃T�C�Y��ύX���邱�Ƃ��o���܂��B
	 *	@param size �T�C�Y�B
	 *	@retval true �T�C�Y�ύX�����B 
	 *	@retval false ���s�B
	 */
	virtual bool Resize(const SIZE& size)
	{
		m_layoutSize = size;
		m_isValidLayoutSize = true;
		return true;
	}

protected:
	/// �Ǘ��p�����[�^�^
	struct TParam
	{
		ELocation		horizontalLocate;	///< �A�C�e���̐����z�u��
		ELocation		verticalLocate;		///< �A�C�e���̐����z�u��
		ILayout::Ptr	pLayout;			///< ���C�A�E�g�A�C�e��
		/// �R���X�g���N�^
		TParam(void)
		{}
		/** �R���X�g���N�^
		 *	@param hloc	�A�C�e���̐����z�u��
		 *	@param vloc	�A�C�e���̐����z�u��
		 *	@param lay	���C�A�E�g�A�C�e��
		 */
		TParam(ELocation hloc, ELocation vloc, const ILayout& lay)
			: horizontalLocate(hloc), verticalLocate(vloc), pLayout(lay.Clone())
		{}
	};
	mutable bool		m_isValidLayoutSize;		///< ���C�A�E�g�S�̃T�C�Y�̐ݒ�̗L���t���O
	mutable SIZE		m_layoutSize;				///< ���C�A�E�g�S�̃T�C�Y�i�c�A���j
	RECT				m_margn;					///< �O���̃}�[�W��
	SIZE				m_gapSize;					///< �e�A�C�e���Ԃ̃M���b�v�i�c�A���j

	/**
	 * [�ݒ�] ���������A�C�e������.
	 *	@param pLayout ���C�A�E�g
	 *	@param x X �ʒu�B Decide()�� x ��n���܂��B
	 *	@param xx X �ʒu�I�t�Z�b�g
	 *	@param width ��
	 *	@param loc �z�u
	 *	@param wParam W �p�����[�^�B�{�N���X�ł͎g�p���܂���B �{�N���X�ɓo�^�������C�A�E�g�ɂ���Ă͎g�p�����\��������܂��B
	 *	@param lParam L �p�����[�^�B�{�N���X�ł͎g�p���܂���B �{�N���X�ɓo�^�������C�A�E�g�ɂ���Ă͎g�p�����\��������܂��B
	 */
	void HorizontalItemDecide(ILayout* pLayout, INT_PTR x, INT_PTR xx, INT_PTR width, ELocation loc, WPARAM wParam, LPARAM lParam)
	{
		SIZE sz;
		if ( x < 0 || pLayout == NULL || ! pLayout->GetSize(sz) )
		{
			return;
		}
		INT_PTR w = width - sz.cx;
		switch ( loc )
		{
		case DEFAULT: // �f�t�H���g
		case LEFT: // ����
			w = 0;
			break;
		case RIGHT: // �E��
			break;
		case ADJUST: // �����iResize�o���Ȃ��ꍇ�����j 
			sz.cx = ToInt(width);
			if ( pLayout->Resize(sz) )
			{
				w = 0;
				break;
			}
			//�ȉ��ɑ���
		case CENTER: // ����
			w /= 2;
			break;
		}
		pLayout->Decide(ToInt(x + xx + w), -1, wParam, lParam);
	}

	/**
	 * [�ݒ�] ���������A�C�e������.
	 *	@param pLayout ���C�A�E�g
	 *	@param y Y �ʒu�B Decide()�� y ��n���܂��B
	 *	@param yy Y �ʒu�I�t�Z�b�g
	 *	@param height ����
	 *	@param loc �z�u
	 *	@param wParam W �p�����[�^�B�{�N���X�ł͎g�p���܂���B �{�N���X�ɓo�^�������C�A�E�g�ɂ���Ă͎g�p�����\��������܂��B
	 *	@param lParam L �p�����[�^�B�{�N���X�ł͎g�p���܂���B �{�N���X�ɓo�^�������C�A�E�g�ɂ���Ă͎g�p�����\��������܂��B
	 */
	void VerticalItemDecide(ILayout* pLayout, INT_PTR y, INT_PTR yy, INT_PTR height, ELocation loc, WPARAM wParam, LPARAM lParam)
	{
		SIZE sz;
		if ( y < 0 || pLayout == NULL || ! pLayout->GetSize(sz) )
		{
			return;
		}
		INT_PTR w = height - sz.cy;
		switch ( loc )
		{
		case TOP: // ����
			w = 0;
			break;
		case BOTTOM: // �E��
			break;
		case ADJUST: // �����iResize�o���Ȃ��ꍇ�����j 
			sz.cy = ToInt(height);
			if ( pLayout->Resize(sz) )
			{
				w = 0;
				break;
			}
			//�ȉ��ɑ���
		case CENTER: // ����
		case DEFAULT: // �f�t�H���g
			w /= 2;
			break;
		}
		pLayout->Decide(-1, ToInt(y + yy + w), wParam, lParam);
	}

	/**
	 * [�擾] �}�[�W���T�C�Y�擾.
	 *	@param[out] _size �}�[�W���T�C�Y
	 */
	void GetMargnSize(SIZE& _size) const
	{
		_size.cx = m_margn.left + m_margn.right;	//���[�̃}�[�W��
		_size.cy = m_margn.top + m_margn.bottom;	//���[�̃}�[�W��
	}

	/**
	 * [���Z] �}�[�W���T�C�Y���Z.
	 *	@note �}�[�W�������Z
	 *	@param[in,out] _size �}�[�W���T�C�Y
	 */
	void AddMargnSize(SIZE& _size) const
	{
		_size.cx += m_margn.left + m_margn.right;	//���[�̃}�[�W��
		_size.cy += m_margn.top + m_margn.bottom;	//���[�̃}�[�W��
	}

	/**
	 * [���Z] ���ԃT�C�Y���Z.
	 *	@note �}�[�W���A�M���b�v�̍��v�����Z
	 *	@param[in,out] _size ���ԃT�C�Y
	 *	@param[in] cx ���̃A�C�e����
	 *	@param[in] cy �c�̃A�C�e����
	 */
	void AddChinkSize(SIZE& _size, size_t cx, size_t cy) const
	{
		AddMargnSize(_size);
		if ( cx > 0 )
		{
			_size.cx += static_cast<LONG>(m_gapSize.cx * (cx - 1)); //�A�C�e���Ԃ̃T�C�Y
		}
		if ( cy > 0 )
		{
			_size.cy += static_cast<LONG>(m_gapSize.cy * (cy - 1)); //�A�C�e���Ԃ̃T�C�Y
		}
	}
};



}; // TNB
