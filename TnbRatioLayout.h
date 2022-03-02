#pragma once
/**
 *@file
 * ���C�A�E�g�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbLayout.h"



//TNB Library
namespace TNB
{



/**@ingroup LAYOUT
 * �䗦���C�A�E�g�A�C�e���N���X.
 *
 *		�Q�̃��C�A�E�g�A�C�e���c�����ɔz�u���郌�C�A�E�g�A�C�e���ł��B
 *		�����̕��A���邢�́A�㑤�̍�����䗦�i�p�[�Z���g�j�Ŏw��ł��܂��B
 *		\code
 *		ex) �����т̏ꍇ
 *		���|�|�|�|�|�|�|�|�|��
 *		����������������������
 *		�����@�P�@���@�Q�@����
 *		����������������������
 *		���|�|�|�|�|�|�|�|�|��
 *		���e���C�A�E�g�̕��́A�S�̂̕�����A�䗦�w��ł��܂��B
 *		���e���C�A�E�g�̍����́A�S�̂̍����ɂȂ�܂��B
 *		\endcode
 *		\code
 *		ex) �c���т̏ꍇ
 *		���|�|�|�|�|��
 *		��������������
 *		�����@�P�@����
 *		��������������
 *		�����@�Q�@����
 *		��������������
 *		���|�|�|�|�|��
 *		���e���C�A�E�g�̍����́A�S�̂̍�������A�䗦�w��ł��܂��B
 *		���e���C�A�E�g�̕��́A�S�̂̍����ɂȂ�܂��B
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbRatioLayout.h
 *
 *	@date 09/09/24 �V�K�쐬
 */
class CRatioLayout : public CAbstractLayout
{
	DEFSUPER(CAbstractLayout);
public:

	/**
	 * �R���X�g���N�^
	 *	@param isHorizental true �Ȃ牡���сA false �Ȃ�c���т��܂��B
	 *	@param parsent �ォ���̃��C�A�E�g�̃p�[�Z���g�B�f�t�H���g�� 50�� �ł��B
	 */
	CRatioLayout(bool isHorizental = true, DWORD parsent = 50) : m_isWideMode(isHorizental), m_center(parsent)
	{
	}

	/**
	 * [�ݒ�] �p�����[�^�ݒ�.
	 *	@param isHorizental true �Ȃ牡���сA false �Ȃ�c���т��܂��B
	 *	@param parsent �ォ���̃��C�A�E�g�̃p�[�Z���g�B�f�t�H���g�� 50�� �ł��B
	 */
	void SetParameter(bool isHorizental = true, DWORD parsent = 50)
	{
		m_isWideMode = isHorizental;
		m_center = parsent;
	}

	/**
	 * [�ݒ�] �A�C�e���ǉ�.
	 *	@note ���ɐݒ肳��Ă���ꍇ�A����͔j������܂��B
	 *	@param isFirst true �Ȃ� 1st(�ォ��) �A false �Ȃ� 2nd(�����E) �̃A�C�e���ݒ�.
	 *	@param layout ���C�A�E�g�A�C�e��.
	 *	@param hloc �����z�u��.
	 *	@param vloc �����z�u��.
	 *	@return �����̎Q��.
	 */
	CRatioLayout& Add(bool isFirst, const ILayout& layout, ELocation hloc = DEFAULT, ELocation vloc = DEFAULT)
	{
		m_param[isFirst ? 0 : 1] = TParam(hloc, vloc, layout);
		return *this;
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual ILayout* Clone(void) const
	{
		CRatioLayout* P = new CRatioLayout();
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
		if ( ! m_IsValid() )
		{
			return false;
		}
		_size.cx =0;
		_size.cy =0;
		SIZE sz;
		loop ( i, 2 )
		{
			if ( m_param[i].pLayout->GetMinimumSize(sz) )
			{
				if ( m_isWideMode )
				{
					//== �����
					_size.cy = max(_size.cy, sz.cy);
					if ( _size.cx != 0 )
					{
						_size.cx += m_gapSize.cx;
					}
					_size.cx += sz.cx;
				}
				else
				{
					//== �c���
					_size.cx = max(_size.cx, sz.cx);
					if ( _size.cy != 0 )
					{
						_size.cy += m_gapSize.cy;
					}
					_size.cy += sz.cy;
				}
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
			SIZE sz = { 0, 0 };
			if ( m_isWideMode )
			{
				AddChinkSize(sz, 2, 1);
			}
			else
			{
				AddChinkSize(sz, 1, 2);
			}
			areaSize.cx -= sz.cx;
			areaSize.cy -= sz.cy;
		}
		POINT pos[2] = { 0 };
		SIZE size[2];
		size[0] = areaSize;
		size[1] = areaSize;
		if ( m_isWideMode )
		{
			//== �����
			size[0].cx *= m_center;
			size[0].cx /= 100;
			size[1].cx -= size[0].cx;
			pos[1].x = size[0].cx + m_gapSize.cx;
		}
		else
		{
			//== �c���
			size[0].cy *= m_center;
			size[0].cy /= 100;
			size[1].cy -= size[0].cy;
			pos[1].y = size[0].cy + m_gapSize.cy;
		}
		//���C�A�E�g
		loop ( i, 2 )
		{
			TParam& pa = m_param[i];
			HorizontalItemDecide(pa.pLayout, x, pos[i].x + m_margn.left, size[i].cx, pa.horizontalLocate, wParam, lParam);
			VerticalItemDecide(pa.pLayout, y, pos[i].y + m_margn.top, size[i].cy, pa.verticalLocate, wParam, lParam);
		}
	}

private:
	// �L���H
	bool m_IsValid(void) const
	{
		return m_param[0].pLayout != NULL && m_param[1].pLayout != NULL;
	}
	TParam	m_param[2];		///< �p�����[�^ [0];first [1];second
	size_t	m_center;		///< �����̊���
	bool	m_isWideMode;	///< true;������ false;�c����
};



}; // TNB
