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
 * �{�b�N�X���C�A�E�g�A�C�e���N���X.
 *
 *		�c�����A���ɕ������ׂ邱�Ƃ��o���郌�C�A�E�g�A�C�e���ł��B
 *
 *		\code
 *		���|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|��
 *		��������������������������������������������������
 *		�����@�P���Q�@���@�@���@�@���@�E�E�E�E�E�E�E������
 *		�����@�@���@�@���R�@���S�@���T�@�@�@�@�@�@�@�@����
 *		��������������������������������������������������
 *		���|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|��
 *		���e���C�A�E�g�A�C�e���̕��͌Œ�ł�(�o�^���̑傫���̂܂�)�B
 *		�@�Ō�(�E�[)�̃��C�A�E�g�A�C�e���̕��́A�I�v�V�����ŁA�S�̂̕��ɂ��ĉςł��܂��B
 *		���e���C�A�E�g�A�C�e���̍����́A�S�̂̍����ɒǏ]���ĉς��܂��B
 *		\endcode
 *		or
 *		\code
 *		���|�|�|�|�|��
 *		��������������
 *		�����P�@�@����
 *		��������������
 *		�����@�Q�@����
 *		��������������
 *		�����R�@�@����
 *		��������������
 *		�����S�@�@����
 *		��������������
 *		�����@�T�@����
 *		�����@�F�@����
 *		�����@�F�@����
 *		�����@�F�@����
 *		�����@���@����
 *		��������������
 *		���|�|�|�|�|��
 *		���e���C�A�E�g�A�C�e���̍����͌Œ�ł�(�o�^���̑傫���̂܂�)�B
 *		�@�Ō�(���[)�̃��C�A�E�g�A�C�e���̍����́A�I�v�V�����ŁA�S�̂̍����ɂ��ĉςł��܂��B
 *		���e���C�A�E�g�A�C�e���̕��́A�S�̂̕��ɒǏ]���ĉς��܂��B
 *		\endcode
 *
 *	@par�g�p��
 *	<table><tr><td><pre>
 *
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
 *			//-- ���������̃p�[�c���
 *			MFCLIB::ChangeClientSize(&m_txtFirst, 100, -1);	//TXT�̕��𓯂��ɂ��Ă���
 *			MFCLIB::ChangeClientSize(&m_txtSecond, 100, -1);
 *			CBoxLayout lay1(true);
 *			lay1.Add(Li(&m_txtFirst)).Add(Li(&m_editFirst), lay.ADJUST);
 *			CBoxLayout lay2(true);
 *			lay2.Add(Li(&m_txtSecond)).Add(Li(&m_editSecond), lay.ADJUST);
 *			//-- ���������ɑg�ݍ��킹��
 *			CBoxLayout lay(false, false);
 *			//-- �E�B���h�E�g���� 2 dot ���Ԃ��w��
 *			lay.SetMargnSize(CSize(2, 2));	
 *			// �e�A�C�e���̊Ԃ́A�� 5, �c 10 �󂭂悤�Ɏw��
 *			lay.SetGapSize(CSize(5, 10));
 *			lay.Add(lay1, lay.ADJUST, lay.ADJUST).Add(lay2, lay.ADJUST, lay.ADJUST);
 *		
 *				;
 *		}
 *
 *	</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbBoxLayout.h
 *
 *	@date 08/07/23 �V�K�쐬
 */
class CBoxLayout : public CAbstractLayout
{
	DEFSUPER(CAbstractLayout);
public:

	/**
	 * �R���X�g���N�^
	 *	@param isHorizental true �Ȃ牡���сA false �Ȃ�c����
	 *	@param isLastFilling true �̏ꍇ�A�ŏI�A�C�e���𒲐����܂��B
	 */
	CBoxLayout(bool isHorizental = true, bool isLastFilling = true)
		: m_isHorizental(isHorizental), m_isLastFilling(isLastFilling)
	{
		m_params = new CParamVector();
	}

	/**
	 * [�ݒ�] �������[�h�ݒ�
	 *	@param isLastFilling true �̏ꍇ�A�ŏI�A�C�e���𒲐����܂��B
	 */
	void SetLastFilling(bool isLastFilling)
	{
		m_isLastFilling = isLastFilling;
	}

	/**
	 * [�ݒ�] ���ь����ݒ�
	 *	@param isHorizental true �Ȃ牡���сA false �Ȃ�c����
	 */
	void SetHorizentalMode(bool isHorizental)
	{
		m_isHorizental = isHorizental;
	}

	/**
	 * [�ݒ�] �A�C�e���ǉ�.
	 *	@note �T�C�Y �� layout �̃T�C�Y���g�p����܂��B
	 *	@param layout ���C�A�E�g�A�C�e��.
	 *	@param hloc �����z�u��.
	 *	@param vloc �����z�u��.
	 *	@return �����̎Q��.
	 */
	CBoxLayout& Add(const ILayout& layout, ELocation hloc = DEFAULT, ELocation vloc = DEFAULT)
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
		CBoxLayout* P = new CBoxLayout();
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
		return m_Calc(_size, NULL);
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
		CSizeEx position(m_margn.left, m_margn.top);
		CSizeEx minSize;
		CSimpleVectorT<TResult> poss;
		if ( ! m_Calc(minSize, &poss) )
		{
			return;
		}
		size_t itemNum = m_params->GetSize();
		loop ( item, itemNum )
		{
			TParam& pa = m_params->At(item);
			CSizeEx nowItemSize;
			if ( ! pa.pLayout->GetSize(nowItemSize) )
			{
				return;
			}
			CSizeEx areaSize = poss[item].size;
			int ll = 0;
			if ( m_isHorizental )
			{
				areaSize.cy = m_layoutSize.cy - m_margn.top - m_margn.bottom;
				ll = m_layoutSize.cx - minSize.cx;
			}
			else
			{
				areaSize.cx = m_layoutSize.cx - m_margn.left - m_margn.right;
				ll = m_layoutSize.cy - minSize.cy;
			}
			if ( m_isLastFilling && item == (itemNum - 1) )
			{
				areaSize.At(m_isHorizental) += ll;
			}
			loop ( hos, 2 )
			{
				bool isHos = (hos == 0);
				if ( position.At(isHos) < 0 )
				{
					continue;
				}
				int ww = position.At(isHos);
				if ( m_isHorizental == isHos )
				{
					ww += poss[item].pos;
				}
				if ( isHos )
				{
					HorizontalItemDecide(pa.pLayout, x, ww, areaSize.At(isHos), GetLocation(pa, isHos), wParam, lParam);
				}
				else
				{
					VerticalItemDecide(pa.pLayout, y, ww, areaSize.At(isHos), GetLocation(pa, isHos), wParam, lParam);
				}
			}
		}
	}

private:
	/// �T�C�Y�Ǘ��N���X
	class CSizeEx : public SIZE
	{
		DEFSUPER(SIZE);
	public:
		CSizeEx() {}
		CSizeEx(int x, int y) { cx = x; cy = y; }
		CSizeEx(SIZE initSize) { cx = initSize.cx; cy = initSize.cy; }
		LONG& At(bool isHorizen) { return isHorizen ? cx : cy; }
		const LONG& At(bool isHorizen) const { return isHorizen ? cx : cy; }
	};
	/// ���ʌ^
	struct TResult
	{
		LONG	pos;	///< �G���A�ʒu
		CSizeEx	size;	///< �G���A�T�C�Y
	};
	/// �v�Z
	bool m_Calc(SIZE& _size, CSimpleVectorT<TResult>* pRes) const
	{
		CSizeEx gapSize = m_gapSize;
		TResult r;
		r.pos = 0;
		LONG hh = 0;
		loop ( item, m_params->GetSize() )
		{
			const TParam& pa = m_params->At(item);
			if ( ! pa.pLayout->GetMinimumSize(r.size) )
			{
				return false;
			}
			if ( item != 0 )
			{
				r.pos += gapSize.At(m_isHorizental);
			}
			if ( pRes != NULL )
			{
				pRes->Add(r);
			}
			r.pos += r.size.At(m_isHorizental);
			hh = max(hh, r.size.At(! m_isHorizental));
		}
		CSizeEx sz;
		GetMargnSize(sz);
		sz.At(m_isHorizental) += r.pos;
		sz.At(! m_isHorizental) += hh;
		_size = sz;
		return true;
	}
	/// �z�u��擾
	static ELocation GetLocation(TParam& p, bool isHorizen) 
	{
		return isHorizen ? p.horizontalLocate : p.verticalLocate; 
	}
	typedef CSimpleVectorT<TParam> CParamVector;	///< TParam �̔z��^
	CPointerHandleT<CParamVector>	m_params;		///< �A�C�e���Q
	bool m_isHorizental;							///< true �Ȃ牡����
	bool m_isLastFilling;							///< true�Ȃ�Ō�̃A�C�e���̓T�C�Y��t��
};



}; // TNB
