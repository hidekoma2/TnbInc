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
 * �{�[�_�[���C�A�E�g�A�C�e���N���X.
 *
 *		���C�A�E�g�A�C�e���𓌐���k�A�����ɔz�u���邱�Ƃ��o���郌�C�A�E�g�A�C�e���ł��B
 *
 *		\code
 *		���|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|��
 *		������������������������������������
 *		�����@�@�@�@�@�k�i��j�@�@�@�@�@����
 *		������������������������������������
 *		�����@���@�@���@�����@���@���@�@����
 *		�����@�i���j���@�@�@�@���@�i�E�j����
 *		������������������������������������
 *		�����@�@�@�@�@��i���j�@�@�@�@�@�����@
 *		������������������������������������
 *		���|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|��
 *		\endcode
 *
 *		���T�C�Y����ƁA�k�A��̃��C�A�E�g�A�C�e���͍��E�ɁA���A���̃��C�A�E�g�A�C�e���͏㉺�ɁA�����̃��C�A�E�g�A�C�e���́A�㉺���E�ɉς��܂��B
 *
 *	@note �S���̈ʒu�Ƀ��C�A�E�g�A�C�e����z�u����K�v�͂���܂���B
 *		
 *	@par�K�v�t�@�C��
 *			TnbBorderLayout.h
 *
 *	@date 08/07/29 �V�K�쐬
 */
class CBorderLayout : public CAbstractLayout
{
	DEFSUPER(CAbstractLayout);
public:

	/**
	 * �|�W�V������.
	 */
	enum EPosition
	{
		Position_North,				///< �k
		Position_South,				///< ��
		Position_West,				///< ��
		Position_Center,				///< �^��
		Position_East,				///< �� 
		//
		Position_Top = Position_North,		///< ��
		Position_Left = Position_West,		///< ��
		Position_Right = Position_East,		///< �E 
		Position_Bottom = Position_South,	///< ��
	};

	/**
	 * �R���X�g���N�^
	 */
	CBorderLayout(void)
	{
		m_params = new TParams();
	}

	/**
	 * [�ݒ�] �A�C�e���ǉ�.
	 *	@param pos �|�W�V����.
	 *	@param layout ���C�A�E�g�A�C�e��.
	 *	@param hloc �����z�u��.
	 *	@param vloc �����z�u��.
	 *	@return �����̎Q��.
	 */
	CBorderLayout& Add(EPosition pos, const ILayout& layout, ELocation hloc = DEFAULT, ELocation vloc = DEFAULT)
	{
		if ( pos >= Position_North && pos <= Position_East )
		{
			m_params->param[pos] = TParam(hloc, vloc, layout);
		}
		else
		{
			ASSERT( false );
		}
		return *this;
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual ILayout* Clone(void) const
	{
		CBorderLayout* P = new CBorderLayout();
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
		SIZE sz;
		for ( int e = Position_West; e <= Position_East; e++ )
		{
			if ( m_params->param[e].pLayout != NULL && m_params->param[e].pLayout->GetMinimumSize(sz) )
			{
				_size.cy = max(_size.cy, sz.cy);
				if ( _size.cx != 0 )
				{
					_size.cx += m_gapSize.cx;
				}
				_size.cx += sz.cx;
			}
		}
		for ( int e = Position_North; e <= Position_South; e++ )
		{
			if ( m_params->param[e].pLayout != NULL && m_params->param[e].pLayout->GetMinimumSize(sz) )
			{
				_size.cx = max(_size.cx, sz.cx);
				if ( _size.cy != 0 )
				{
					_size.cy += m_gapSize.cy;
				}
				_size.cy += sz.cy;
			}
		}
		//
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
		POINT cpos = { m_margn.left, m_margn.top };
		SIZE csize = areaSize;
		SIZE sz; //�ėp
		POINT pp[5] = { 0 };
		SIZE ss[5] = { 0 };
		//��i���j
		{
			TParam& pa = m_params->param[Position_South];
			if ( ! pa.pLayout.IsNull() && pa.pLayout->GetSize(sz) )
			{
				pp[Position_South].x = cpos.x;
				ss[Position_South].cx = csize.cx;
				pp[Position_South].y = cpos.y + csize.cy - sz.cy;
				ss[Position_South].cy = sz.cy;
				csize.cy -= sz.cy + m_gapSize.cy;
			}
		}
		//�k�i��j
		{
			TParam& pa = m_params->param[Position_North];
			if ( ! pa.pLayout.IsNull() && pa.pLayout->GetSize(sz) )
			{
				pp[Position_North].x = cpos.x;
				ss[Position_North].cx = csize.cx;
				pp[Position_North].y = cpos.y;
				ss[Position_North].cy = sz.cy;
				csize.cy -= sz.cy + m_gapSize.cy;
				cpos.y += sz.cy + m_gapSize.cy;
			}
		}
		//���i�E�j
		{
			TParam& pa = m_params->param[Position_East];
			if ( ! pa.pLayout.IsNull() && pa.pLayout->GetSize(sz) )
			{
				pp[Position_East].x = cpos.x + csize.cx - sz.cx;
				ss[Position_East].cx = sz.cx;
				pp[Position_East].y = cpos.y;
				ss[Position_East].cy = csize.cy;
				csize.cx -= sz.cx+ m_gapSize.cx;
			}
		}	
		//���i���j
		{
			TParam& pa = m_params->param[Position_West];
			if ( ! pa.pLayout.IsNull() && pa.pLayout->GetSize(sz) )
			{
				pp[Position_West].x = cpos.x;
				ss[Position_West].cx = sz.cx;
				pp[Position_West].y = cpos.y;
				ss[Position_West].cy = csize.cy;
				csize.cx -= sz.cx + m_gapSize.cx;
				cpos.x += sz.cx + m_gapSize.cx;
			}
		}	
		//��
		{
			TParam& pa = m_params->param[Position_Center];
			if ( ! pa.pLayout.IsNull() && pa.pLayout->GetSize(sz) )
			{
				pp[Position_Center].x = cpos.x;
				ss[Position_Center].cx = csize.cx;
				pp[Position_Center].y = cpos.y;
				ss[Position_Center].cy = csize.cy;
			}
		}
		//���C�A�E�g
		int row[5] = { Position_North, Position_West, Position_Center, Position_East, Position_South };
		loop ( j, 5 )
		{
			int i = row[j];
			TParam& pa = m_params->param[i];
			if ( ! pa.pLayout.IsNull() )
			{
				HorizontalItemDecide(pa.pLayout, x, pp[i].x, ss[i].cx, pa.horizontalLocate, wParam, lParam);
				VerticalItemDecide(pa.pLayout, y, pp[i].y, ss[i].cy, pa.verticalLocate, wParam, lParam);
			}
		}
	}

private:
	struct TParams
	{
		TParam	param[5];
	};
	CPointerHandleT<TParams>	m_params;		///< �A�C�e���Q m_sizex * m_sizey �̐�������
};



}; // TNB

