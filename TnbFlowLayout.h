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
 * �t���[���C�A�E�g�A�C�e���N���X.
 *
 *		���C�A�E�g�A�C�e����������E�֏��ɔz�u���܂��B���ɔz�u������Ȃ��Ȃ�����A���s���Ĕz�u���܂��B
 *
 *		\code
 *		���|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|��
 *		������������������������������������
 *		�����P�@���@�Q�@���@�R�@�@���S�@����
 *		������������������������������������
 *		�����@�T�@���U�@�@���V�@���@�@�@�@��
 *		���������������������������@�@�@�@��
 *		�����@�W�@�@�@�@�@�@���@�@�@�@�@�@��
 *		�����������������������@�@�@�@�@�@��
 *		���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
 *		���|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|��
 *		���|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|��
 *		��������������������������������������������
 *		�����P�@���@�Q�@���@�R�@�@���S�@���@�T�@����
 *		��������������������������������������������
 *		�����U�@�@���V�@���@�W�@�@�@�@�@�@���@�@�@��
 *		�������������������������������������@�@�@��
 *		���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
 *		���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
 *		���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
 *		���|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|��
 *		���e���C�A�E�g�A�C�e���̕��͌Œ�ł�(�o�^���̑傫���̂܂�)�B
 *		���e���C�A�E�g�A�C�e���̍����͂��̗�̈�ԍ������̂ɂȂ�܂��B
 *		\endcode
 *
 *		�e���C�A�E�g�A�C�e���̓��T�C�Y�����Ɏg�p����̂ŁA Add() �̑O�ɁA���炩���ߑ傫����ݒ肵�Ă����܂��B
 *
 *	@note �e�s�̔z�u�� SetLineLocation() �ɂ���Ďw��ł��܂��B
 *		\code
 *		ex) SetLineLocation(RIGHT) �̏ꍇ
 *		���|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|��
 *		������������������������������������
 *		�����P�@���@�Q�@���@�R�@�@���S�@����
 *		������������������������������������
 *		���@�@�@�@���@�T�@���U�@�@���V�@����
 *		���@�@�@�@��������������������������
 *		���@�@�@�@�@�@���@�W�@�@�@�@�@�@����
 *		���@�@�@�@�@�@����������������������
 *		���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
 *		���|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|��
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbFlowLayout.h
 *
 *	@date 08/07/24 �V�K�쐬
 */
class CFlowLayout : public CAbstractLayout
{
	DEFSUPER(CAbstractLayout);
public:

	/// �R���X�g���N�^
	CFlowLayout(void) : m_hLocation(LEFT)
	{
		m_params = new CParamVector();
	}

	/**
	 * [�ݒ�] �z�u�ݒ�.
	 *		��̔z�u��ݒ肵�܂��B�f�t�H���g�ł� LEFT �ɂȂ��Ă��܂��B
	 *	@param hloc �z�u�B CENTER , LEFT , RIGHT �̂����ꂩ���w��ł��܂��B
	 */
	void SetLineLocation(ELocation hloc = LEFT)
	{
		m_hLocation = hloc;
	}

	/**
	 * [�ݒ�] �A�C�e���ǉ�.
	 *	@param layout ���C�A�E�g�A�C�e��.
	 *	@param vloc �����z�u��. TOP , CENTER , BOTTOM �̂����ꂩ���w��ł��܂��B
	 *	@return �����̎Q��.
	 */
	CFlowLayout& Add(const ILayout& layout, ELocation vloc = DEFAULT)
	{
		m_params->Add(TParam(vloc, DEFAULT, layout));
		return *this;
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual ILayout* Clone(void) const
	{
		CFlowLayout* P = new CFlowLayout();
		*P = *this;
		return P;
	}

	/**
	 * [�擾] �ŏ��T�C�Y�擾.
	 *		�ŏ��̃T�C�Y�𓾂邱�Ƃ��o���܂��B
	 *	@note ���̃��C�A�E�g�ł͍������O�A�������C�A�E�g�A�C�e���̍ő啝�A�ɂȂ�܂��B
	 *	@param[out] _size �T�C�Y�擾�B
	 *	@retval true �T�C�Y�擾�����B 
	 *	@retval false �f�[�^���Ȃ��B
	 */
	virtual bool GetMinimumSize(SIZE& _size) const
	{
		_size.cx = 0;
		_size.cy = 0;
		loop ( item, m_params->GetSize() )
		{
			TParam& pa = m_params->At(item);
			SIZE sz;
			if ( ! pa.pLayout->GetSize(sz) )
			{
				return false;
			}
			_size.cx = max(_size.cx, sz.cx);
		}
		AddMargnSize(_size);
		return true;
	}
	
	/**
	 * [�擾] �ŏ������T�C�Y�擾.
	 *		�w�蕝�̏ꍇ�̍ŏ������𓾂邱�Ƃ��o���܂��B
	 *	@param width ���B
	 *	@retval �}�C�i�X �G���[�B 
	 *	@retval 0�ȏ� �ŏ������B
	 */
	int GetMinimumHeight(size_t width) const
	{
		CSimpleVectorT<POINT> lineNum;	// ���C�������B X ���A�C�e�����A Y ��HIWORD�������ALOWORD����
		CSimpleVectorT<SIZE> sizes;		// �A�C�e�������B
		int h = m_Calc(lineNum, sizes, width);
		if ( h >= 0 )
		{
			h += m_margn.top + m_margn.bottom;
		}
		return h;
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
		if ( m_params.IsNull() )
		{
			return;
		}
		// �\���͈�
		int clientWidth = m_layoutSize.cx - m_margn.left - m_margn.right;
		// �S�A�C�e���̑傫�����擾
		CSimpleVectorT<POINT> lineNum;	// ���C�������B X ���A�C�e�����A Y ��HIWORD�������ALOWORD����
		CSimpleVectorT<SIZE> sizes;		// �A�C�e�������B
		if ( m_Calc(lineNum, sizes, clientWidth) < 0 )
		{
			return;
		}
		// ���C�����̏���
		int item = 0;
		int yy = m_margn.top;
		loop ( line, lineNum.GetSize() )
		{
			int hh = LOWORD(lineNum[line].y);
			int xx = (clientWidth - HIWORD(lineNum[line].y));
			switch ( m_hLocation )
			{
			case LEFT:
				xx = 0;
				break;
			case CENTER:
			case DEFAULT:
				if ( xx < 0 ) { xx = 0; }
				xx /= 2;
				break;
			}
			xx += m_margn.left;
			loop ( i, lineNum[line].x )
			{
				TParam& pa = m_params->At(item);
				SIZE& sz = sizes[item];
				HorizontalItemDecide(pa.pLayout, x, xx, sz.cx, LEFT, wParam, lParam);
				VerticalItemDecide(pa.pLayout, y, yy, sz.cy, pa.verticalLocate, wParam, lParam);
				item++;
				xx += sz.cx + m_gapSize.cx;
			}
			yy += m_gapSize.cy + hh;
		}
	}

private:

	// _lineNum ���C�������B X ���A�C�e�����A Y ��HIWORD�������ALOWORD����
	// _sizes �A�C�e�������B
	int m_Calc(CSimpleVectorT<POINT>& _lineNum, CSimpleVectorT<SIZE>& _sizes, size_t clientWidth) const
	{
		size_t itemNum = m_params->GetSize();
		_sizes.SetSize(itemNum);
		_lineNum.RemoveAll();
		int height = 0;
		int im = 0;
		int h = 0;
		int w = 0;
		loop ( item, itemNum )
		{
			TParam& pa = m_params->At(item);
			SIZE& sz = _sizes[item];
			if ( ! pa.pLayout->GetSize(sz) )
			{
				return -1;
			}
			if ( im > 0 && w + sz.cx + m_gapSize.cx > ToInt(clientWidth) )
			{
				// ���𒴂���
				POINT p = { im, MAKELONG(h, w) };
				height += h;
				if ( _lineNum.GetSize() != 0 )
				{
					height += m_gapSize.cy;
				}
				_lineNum.Add(p);
				im = 0;
				h = 0;
				w = 0;
			}
			w += sz.cx;
			if ( im > 0 )
			{
				w += m_gapSize.cx;
			}
			h = max(h, sz.cy);
			im++;
		}
		if ( im > 0 )
		{
			// ���𒴂���
			POINT p = { im, MAKELONG(h, w) };
			height += h;
			if ( _lineNum.GetSize() != 0 )
			{
				height += m_gapSize.cy;
			}
			_lineNum.Add(p);
		}
		return height;
	}

	typedef CSimpleVectorT<TParam> CParamVector;	///< TParam �̔z��^
	CPointerHandleT<CParamVector>	m_params;		///< �A�C�e���Q
	ELocation						m_hLocation;	///< �񖈂̈ʒu
};



}; // TNB
