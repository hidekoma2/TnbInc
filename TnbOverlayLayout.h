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
 * �I�[�o�[���C���C�A�E�g�A�C�e���N���X.
 *
 *		�����̃��C�A�E�g�A�C�e�����d�Ȃ荇���悤�ɔz�u���邱�Ƃ��o���܂��B 
 *		�O���[�v�{�b�N�X�Ƃ��̒��̃A�C�e����z�u������A���ꂼ��� Show/Hide �����A�y�[�W�̂悤�Ɏg���ꍇ�ɓK���Ă��܂��B
 *		\code
 *		���|�|�|�|�|��
 *		���������������P,�Q,�R�A���ɓ����ꏊ�œ����傫���ɂȂ�܂��B
 *		�����@�P�@����
 *		�����i�Q�j����
 *		�����i�R�j����
 *		��������������
 *		���|�|�|�|�|��
 *		���e���C�A�E�g�A�C�e���̃T�C�Y�́A�S�̂̃T�C�Y�Ɠ����Ȃ�܂��B
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbOverlayLayout.h
 *
 *	@date 08/07/28 �V�K�쐬
 */
class COverlayLayout : public CAbstractLayout
{
	DEFSUPER(CAbstractLayout);
public:

	/// �R���X�g���N�^
	COverlayLayout(void)
	{
		m_params = new CParamVector();
	}

	/**
	 * [�ݒ�] �A�C�e���ǉ�.
	 *	@note �T�C�Y �� layout �̃T�C�Y���g�p����܂��B
	 *	@param layout ���C�A�E�g�A�C�e��.
	 *	@param hloc �����z�u��. �ȗ������ ADJUST �ɂȂ�܂��B
	 *	@param vloc �����z�u��. �ȗ������ ADJUST �ɂȂ�܂��B
	 *	@return �����̎Q��.
	 */
	COverlayLayout& Add(const ILayout& layout, ELocation hloc = ADJUST, ELocation vloc = ADJUST)
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
		COverlayLayout* P = new COverlayLayout();
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
		loop ( i, m_params->GetSize() )
		{
			SIZE sz;
			if ( m_params->At(i).pLayout->GetMinimumSize(sz) )
			{
				_size.cx = max(_size.cx, sz.cy);
				_size.cy = max(_size.cy, sz.cy);
			}
		}
		AddMargnSize(_size);
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
			SIZE sz;
			GetMargnSize(sz);
			areaSize.cx -= sz.cx;
			areaSize.cy -= sz.cy;
		}
		int posy = m_margn.top;
		int posx = m_margn.left;
		loop ( i, m_params->GetSize() )
		{
			TParam& pa = m_params->At(i);
			HorizontalItemDecide(pa.pLayout, x, posx, areaSize.cx, pa.horizontalLocate, wParam, lParam);
			VerticalItemDecide(pa.pLayout, y, posy, areaSize.cy, pa.verticalLocate, wParam, lParam);
		}
	}

private:
	typedef CSimpleVectorT<TParam> CParamVector;	///< TParam �̔z��^
	CPointerHandleT<CParamVector>	m_params;		///< �A�C�e���Q
};



}; // TNB

