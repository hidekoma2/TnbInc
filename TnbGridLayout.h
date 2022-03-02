#pragma once
/**
 *@file
 * ���C�A�E�g�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSimpleVector.h"
#include "TnbLayout.h"



//TNB Library
namespace TNB
{



/**@ingroup LAYOUT
 * �O���b�h���C�A�E�g�A�C�e���N���X.
 *
 *		���C�A�E�g�A�C�e�������ォ��i�q��ɔz�u�o���郌�C�A�E�g�A�C�e���ł��B
 *		�R���X�g���N�^���A���ɕ��ׂ鐔���w�肵�܂��i�c�̐��͒ǉ�����������v�Z�����j�B
 *		\code
 *		���|�|�|�|�|�|�|�|�|�|�|�|�|��
 *		�������������������������������@ex)��3�̏ꍇ
 *		�����@�@�P���@�Q�@���@�R�@����
 *		������������������������������
 *		�����S�@�@���@�T�@���U�@�@����
 *		������������������������������
 *		�����@�V�@���@�@�W���@�@�@�@��
 *		���������������������@�@�@�@��
 *		���@�@�@�@�@�@�@�@�@�@�@�@�@��
 *		���@�@�@�@�@�@�@�@�@�@�@�@�@��
 *		���|�|�|�|�|�|�|�|�|�|�|�|�|��
 *		���e���C�A�E�g�A�C�e���̕��́A�S�̂̕��̓����ɂȂ�܂��B
 *		���e���C�A�E�g�A�C�e���̍����͂��̗�̈�ԍ������̂ɂȂ�܂��B
 *		\endcode
 *
 *	@par�g�p��
 *		@code
 *		// �ݒ�
 *		CStatic	m_txtFirst;	
 *		CStatic	m_txtSecond;
 *		CEdit	m_editFirst;
 *		CEdit	m_editSecond;
 *			:
 *
 *		BOOL CFoo::OnInitDialog()
 *		{
 *				;
 *		
 *			//-- ���������̃p�[�c��� ���������ɂQ�p�[�c
 *			CGridLayout lay(2);
 *			lay.Add(Li(&m_txtFirst));
 *			lay.Add(Li(&m_editFirst));
 *			lay.Add(Li(&m_txtSecond));
 *			lay.Add(Li(&m_editSecond));
 *			//-- �E�B���h�E�g���� 2 dot ���Ԃ��w��
 *			lay.SetMargnSize(CSize(2, 2));	
 *			// �e�A�C�e���̊Ԃ́A�� 5, �c 10 �󂭂悤�Ɏw��
 *			lay.SetGapSize(CSize(5, 10));
 *		
 *				;
 *		}
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbGridLayout.h
 *
 *	@date 08/07/28 �V�K�쐬
 */
class CGridLayout : public CAbstractLayout
{
	DEFSUPER(CAbstractLayout);
public:

	/**
	 * �R���X�g���N�^
	 *	@param cx ���̃A�C�e����
	 */
	CGridLayout(size_t cx) : m_sizex(cx)
	{
		ASSERT( m_sizex != 0 );
		m_params = new CParamVector();
	}

	/**
	 * [�ݒ�] �A�C�e���ǉ�.
	 *	@note �T�C�Y �� layout �̃T�C�Y���g�p����܂��B
	 *	@param layout ���C�A�E�g�A�C�e��.
	 *	@param hloc �����z�u��.
	 *	@param vloc �����z�u��.
	 *	@return �����̎Q��.
	 */
	CGridLayout& Add(const ILayout& layout, ELocation hloc = DEFAULT, ELocation vloc = DEFAULT)
	{
		m_params->Add(TParam(hloc, vloc, layout));
		return *this;
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual ILayout* Clone(void) const
	{
		CGridLayout* P = new CGridLayout(m_sizex);
		*P = *this;
		return P;
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
		_size.cx =0;
		_size.cy =0;
		int ly = 0;
		CWorkMemT<LONG> lxs(GetHorizontalItemCount());
		loop ( y, GetVerticalItemCount() )
		{
			loop ( x, GetHorizontalItemCount() )
			{
				if ( m_params->GetSize() > y * GetHorizontalItemCount() + x)
				{
					SIZE sz;
					if ( m_params->At(y * GetHorizontalItemCount() + x).pLayout->GetMinimumSize(sz) )
					{
						ly = max(ly, sz.cy);
						lxs[x] = max(lxs[x], sz.cx);
					}
				}
			}
		}
		_size.cy += ToInt(ly * GetVerticalItemCount());
		// X�𒲂ׂ�
		int lx = 0;
		loop ( x, GetHorizontalItemCount() )
		{
			lx = max(lx, lxs[x]);
		}
		_size.cx += ToInt(lx * GetVerticalItemCount());
		//
		AddChinkSize(_size, GetHorizontalItemCount(), GetVerticalItemCount());
		return true;
	}
	
	/**
	 * [����] ����.
	 *	@note �I�u�W�F�N�g���Ǘ����Ă���T�C�Y�Ǝw��̃T�C�Y�Ƀ��C�A�E�g���܂��B
	 *	@param x  X���W�B-1�Ȃ�w��Ȃ��B
	 *	@param y  Y���W�B-1�Ȃ�w��Ȃ��B
	 *	@param wParam W �p�����[�^�B�{�N���X�ł͎g�p���܂���B �{�N���X�ɓo�^�������C�A�E�g�ɂ���Ă͎g�p�����\��������܂��B
	 *	@param lParam L �p�����[�^�B�{�N���X�ł͎g�p���܂���B �{�N���X�ɓo�^�������C�A�E�g�ɂ���Ă͎g�p�����\��������܂��B
	 */
	virtual void Decide(int x, int y, WPARAM wParam, LPARAM lParam)
	{
		SIZE areaSize = m_layoutSize;
		{
			SIZE sz = { 0, 0 };
			AddChinkSize(sz, GetHorizontalItemCount(), GetVerticalItemCount());
			areaSize.cx = ToInt((areaSize.cx - sz.cx) / GetHorizontalItemCount());
			areaSize.cy = ToInt((areaSize.cy - sz.cy) / GetVerticalItemCount());
		}
		int posy = m_margn.top;
		loop ( yy, GetVerticalItemCount() )
		{
			int posx = m_margn.left;
			loop ( xx, GetHorizontalItemCount() )
			{
				if ( m_params->GetSize() > yy * GetHorizontalItemCount() + xx)
				{
					TParam& pa = m_params->At(yy * GetHorizontalItemCount() + xx);
					HorizontalItemDecide(pa.pLayout, x, posx, areaSize.cx, pa.horizontalLocate, wParam, lParam);
					VerticalItemDecide(pa.pLayout, y, posy, areaSize.cy, pa.verticalLocate, wParam, lParam);
					posx += areaSize.cx + m_gapSize.cx;
				}
			}
			posy += areaSize.cy + m_gapSize.cy;
		}
	}

private:
	typedef CSimpleVectorT<TParam> CParamVector;	///< TParam �̔z��^
	CPointerHandleT<CParamVector>	m_params;		///< �A�C�e���Q m_sizex * m_sizey �̐�������
	size_t							m_sizex;		///< ����
	/// �����A�C�e����
	size_t GetHorizontalItemCount(void) const
	{
		return m_sizex;
	}
	/// �����A�C�e����
	size_t GetVerticalItemCount(void) const
	{
		return (m_params->GetSize() + m_sizex - 1) / m_sizex;
	}
};



}; // TNB

