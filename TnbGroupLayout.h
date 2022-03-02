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
 * �O���[�v���C�A�E�g�A�C�e���N���X.
 *
 *		���C�A�E�g�C���^�[�t�F�[�X�����I�u�W�F�N�g�𕡐��Ǘ�����N���X�ł��B
 *
 *	@par�g�p��
 *	<table><tr><td><pre>
 *
 *		// �_�C�A���O�N���X��		
 *
 *		CStatic	m_txtFirst;	
 *		CStatic	m_txtSecond;
 *		CEdit	m_editFirst;
 *		CEdit	m_editSecond;
 *		
 *		BOOL CFoo::OnInitDialog()
 *		{
 *				;
 *		
 *			CGroupLayout layout;		
 *
 *			// �E�B���h�E�g���� 2 dot ���Ԃ��w��
 *			layout.SetMargnSize(CSize(2, 2));	
 *			// �e�A�C�e���̊Ԃ́A�� 5, �c 10 �󂭂悤�Ɏw��
 *			layout.SetGapSize(CSize(5, 10));
 *			// ���������̃��C�A�E�g
 *			layout.SetHorizontalGroup			
 *			(
 *				AddGroup(
 *				    Add(Li(&m_txtFirst))  //TEXT�͂�\�[�X�̑傫���̂܂�		
 *				    .Add(Li(&m_txtSecond))
 *				)
 *				.AddGroup(
 *				    Add(Li(&m_editFirst), CGroupLayout::ADJUST)  //EDIT�͘g��t�ɍL���� 
 *				    .Add(Li(&m_editSecond))
 *				)
 *			);
 *			// ���������̃��C�A�E�g
 *			layout.SetVerticalGroup
 *			(
 *				AddGroup(
 *				    Add(Li(&m_txtFirst))
 *				    .Add(Li(&m_editFirst), CGroupLayout::ADJUST)
 *				)
 *				.AddGroup(
 *				    Add(Li(&m_txtSecond))
 *				    .Add(Li(&m_editSecond), CGroupLayout::ADJUST)
 *				)
 *				.AutoEqualityDistance()  // �c�����́A�ϓ������悤
 *			);
 *
 *				;
 *		}
 *		
 *	</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbGroupLayout.h
 *
 *	@date 08/07/17 �V�K�쐬
 */
class CGroupLayout : public CAbstractLayout
{
	DEFSUPER(CAbstractLayout);
public:

	/**@ingroup LAYOUT
	 * �p�������O���[�v��`�N���X.
	 *
	 *		�����̃��C�A�E�g�A�C�e�����Ǘ��B
	 *
	 *	@note �ʏ� Add() �ȊO�A�ӎ����Ďg���K�v�͂���܂���B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbGroupLayout.h
	 *	
	 *	@date 08/07/17 �V�K�쐬
	 */
	class CParallelGroup
	{
	public:

		/**
		 * �R���X�g���N�^
		 *	@note �K��̔z�u����w�肷�邱�Ƃ��o���܂��B�ȍ~�A�z�u����ȗ����� Add() ����ƋK��̔z�u�킪�g���܂��B
		 *	@param defloc ����̔z�u��B�ȗ�����ƁA DEFAULT �ł��B
		 */
		CParallelGroup(ELocation defloc = DEFAULT) : m_defaultLocation(defloc)
		{
			m_params = new CParamVector;
		}

		/**
		 * [�ݒ�] �A�C�e���ǉ�.
		 *	@param layout ���C�A�E�g�A�C�e��.
		 *	@param loc �z�u��.
		 *	@return �����̎Q��.
		 */
		CParallelGroup& Add(const ILayout& layout, ELocation loc)
		{
			TParam p(loc, layout.Clone());
			m_params->Add(p);
			return *this;
		}

		/**
		 * [�ݒ�] �A�C�e���ǉ�.
		 *	@note ����̔z�u��ŃA�C�e����ǉ����܂��B
		 *	@param layout ���C�A�E�g�A�C�e��.
		 *	@return �����̎Q��.
		 */
		CParallelGroup& Add(const ILayout& layout)
		{
			return Add(layout, m_defaultLocation);
		}

		/**
		 * [�擾] �A�C�e�����擾.
		 *	@return �A�C�e����
		 */
		size_t GetItemCount(void) const
		{
			return m_params->GetSize();
		}

		/**
		 * [�擾] ���C�A�E�g�A�C�e���擾.
		 *	@param index �C���f�b�N�X�B GetItemCount() �̌��ʂ�菬�����l���w�肵�܂��B
		 *	@return ���C�A�E�g�A�C�e��
		 */
		ILayout* GetItemLayout(INDEX index)
		{
			return m_params->At(index).pLayout;
		}

		/**
		 * [�擾] ���C�A�E�g�A�C�e���擾.
		 *	@param index �C���f�b�N�X�B GetItemCount() �̌��ʂ�菬�����l���w�肵�܂��B
		 *	@return ���C�A�E�g�A�C�e��
		 */
		const ILayout* GetItemLayout(INDEX index) const
		{
			return m_params->At(index).pLayout;
		}

		/**
		 * [�擾] �A�C�e���̔z�u��擾.
		 *	@param index �C���f�b�N�X�B GetItemCount() �̌��ʂ�菬�����l���w�肵�܂��B
		 *	@return �A�C�e���̔z�u��
		 */
		ELocation GetItemLocation(INDEX index) const
		{
			return m_params->At(index).locate;
		}

		/**
		 * [�擾] �ŏ��T�C�Y�擾.
		 *	@note �ݒ肳��Ă���A�C�e���̍ŏ��T�C�Y�����ׂē���T�C�Y���v�Z���ĕԂ��܂��B
		 *	@return �T�C�Y�B
		 */
		SIZE GetMinimumSize(void) const
		{
			SIZE ret = { 0, 0 };
			loop ( i, GetItemCount() )
			{
				SIZE sz;
				GetItemLayout(i)->GetMinimumSize(sz);
				ret.cx = max(ret.cx, sz.cx);
				ret.cy = max(ret.cy, sz.cy);
			}
			return ret;
		}

	private:
		/// �Ǘ��p�����[�^�^
		struct TParam
		{
			ELocation		locate;		///< �A�C�e���̔z�u��
			ILayout::Ptr	pLayout;	///< ���C�A�E�g�A�C�e��
			/// �R���X�g���N�^
			TParam(void) { }
			/// �R���X�g���N�^
			TParam(ELocation loc, ILayout* p) : locate(loc), pLayout(p) { }
		};
		typedef CSimpleVectorT<TParam> CParamVector;		///< TParam �̔z��^
		CPointerHandleT<CParamVector>	m_params;			///< �A�C�e���Q
		ELocation						m_defaultLocation;	///< ����̔z�u
	};

	/**
	 * ParallelGroup �̒�����.
	 *	CSequentialGroup �� CParallelGroup ��o�^���鎞�Ɏw�肵�܂��B
	 */
	enum EDistance
	{
		Distance_Default,		///< �f�t�H���g�i Distance_ItemMax �A�Ō�̂� Distance_Rast �ɂȂ�܂�)�B
		Distance_Absolute,		///< ��Βl�ivalue �͏c���̐�Βl MAKELONG(X, Y) �Ŋi�[�j
		Distance_Rate,			///< �䗦�ivalue �͔䗦���i�[�j
		Distance_ItemMax,		///< �A�C�e���ő�
		Distance_Rast,			///< �]��i��̃V�[�P���V�����O���[�v�ɕ����w��ł��܂���)
	};
	
	/**@ingroup LAYOUT
	 * �V�[�P���V�����O���[�v��`�N���X.
	 *
	 *		�����̃p�������O���[�v���Ǘ�
	 *
	 *	@note �ʏ� AddGroup() �ȊO�A�ӎ����Ďg���K�v�͂���܂���B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbGroupLayout.h
	 *	
	 *	@date 08/07/17 �V�K�쐬
	 */
	class CSequentialGroup
	{
	public:

		/// �R���X�g���N�^
		CSequentialGroup(void) : m_params(new CParamVector)
		{
		}

		/**
		 * [�ǉ�] �p�������O���[�v�ǉ�.
		 *	@param g �p�������O���[�v�B�{�N���X�ɂ̓R�s�[���i�[����܂��B
		 *	@param dis ������B�ȗ������ Distance_Default ���w�肳��܂��B
		 *	@param val �l�B�����킪 Distance_Absolute �̏ꍇ�͏c���̐�Βl MAKELONG(X, Y) ���w��B 
		 *					Distance_Rate �̏ꍇ�͔䗦�i���j���w��B����ȊO�̏ꍇ�͏ȗ��\�B
		 *	@return �����̎Q�ƁB
		 */
		CSequentialGroup& AddGroup(CParallelGroup& g, EDistance dis = Distance_Default, int val = 0)
		{
			TParam p(dis, val, g);
			m_params->Add(p);
			return *this;
		}

		/**
		 * [�ݒ�] �������ω�.
		 *	@note �K�v�� AddGroup() �I����A���̃��\�b�h���g�p����ƁA�����𕽋ω����܂��B
		 *	@param p �p�[�Z���g�B�ȗ������100�ł��B
		 *	@return �����̎Q�ƁB
		 */
		CSequentialGroup& AutoEqualityDistance(int p = 100)
		{
			size_t num = GetGroupCount();
			size_t v = 0;
			loop ( i, num )
			{
				TParam& par = m_params->At(i);
				par.distance = Distance_Rate;
				double a = p;
				a /= num;
				a *= (i + 1);
				par.value = 1;
				if ( a >= v )
				{
					par.value = static_cast<size_t>(a - v);
					if ( par.value <= 0 )
					{
						par.value = 1;
					}
				}
				v = static_cast<size_t>(a);
			}
			return *this;
		}
		
		/**
		 * [�擾] �O���[�v���擾.
		 *	@return �O���[�v��
		 */
		size_t GetGroupCount(void) const
		{
			return m_params->GetSize();
		}

		/**
		 * [�擾] �O���[�v�擾.
		 *	@param index �C���f�b�N�X�B GetGroupCount() �̌��ʂ�菬�����l���w�肵�܂��B
		 *	@return �O���[�v���
		 */
		CParallelGroup& GetGroup(INDEX index)
		{
			return m_params->At(index).parallel;
		}

		/**
		 * [�擾] �O���[�v�擾.
		 *	@param index �C���f�b�N�X�B GetGroupCount() �̌��ʂ�菬�����l���w�肵�܂��B
		 *	@return �O���[�v���
		 */
		const CParallelGroup& GetGroup(INDEX index) const
		{
			return m_params->At(index).parallel;
		}

		/**
		 * [�擾] �O���[�v������擾.
		 *	@param index �C���f�b�N�X�B GetGroupCount() �̌��ʂ�菬�����l���w�肵�܂��B
		 *	@return ������
		 */
		EDistance GetGroupDistance(INDEX index) const
		{
			return m_params->At(index).distance;
		}

		/**
		 * [�擾] �O���[�v�����l�擾.
		 *	@param index �C���f�b�N�X�B GetGroupCount() �̌��ʂ�菬�����l���w�肵�܂��B
		 *	@return �����l
		 */
		size_t GetGroupValue(INDEX index) const
		{
			return m_params->At(index).value;
		}

		/**
		 * [�擾] �ŏ��T�C�Y�擾.
		 *	@note �ݒ肳��Ă����񂩂�A�ŏ��̃T�C�Y���v�Z���ĕԂ��܂��B
		 *	@return �T�C�Y�B
		 */
		SIZE GetMinimumSize(void) const
		{
			SIZE ret = { 0, 0 };
			CWorkMemT<SIZE> sizes(GetGroupCount());
			loop ( i, GetGroupCount() )
			{
				const CParallelGroup& pg = GetGroup(i);
				SIZE sz;
				switch ( GetGroupDistance(i) )
				{
				case Distance_Absolute:// ��Βl
					{
						size_t val = GetGroupValue(i);
						sz.cx = LOWORD(val);
						sz.cy = HIWORD(val);
					}
					break;
				case Distance_Default:// �f�t�H���g
				case Distance_Rate:// �䗦
				case Distance_ItemMax:// �A�C�e���ő�
				case Distance_Rast:// �]��
					sz = pg.GetMinimumSize();
					break;
				default:
					sz .cx = 0;
					sz .cy = 0;
					ASSERT( false );
					break;
				}
				ret.cx += sz.cx;
				ret.cy += sz.cy;
				sizes[i] = sz;
			}
			// Rate�`�F�b�N
			SIZE adSize = { 0, 0 };
			loop ( i, GetGroupCount() )
			{
				if ( GetGroupDistance(i) == Distance_Rate )
				{
					size_t val = GetGroupValue(i);
					INT_PTR x = ret.cx * val / 100;
					if ( sizes[i].cx > x )
					{
						adSize.cx = max(adSize.cx, down_cast<LONG>(sizes[i].cx * 100 / val));
					}
					INT_PTR y = ret.cy * val / 100; 
					if ( sizes[i].cy > y )
					{
						adSize.cy = max(adSize.cy, ToInt(sizes[i].cy * 100 / val));
					}
				}
			}
			if ( adSize.cx != 0 || adSize.cy != 0 )
			{
				ret.cx = max(ret.cx, adSize.cx);
				ret.cy = max(ret.cy, adSize.cy);
			}
			return ret;
		}

	private:
		/// �Ǘ��p�����[�^�^
		struct TParam
		{
			EDistance		distance;	///< ������B
			size_t			value;		///< �����l�B Distance_Absolute , Distance_Rate ���w�肵�����K�{�B
			CParallelGroup	parallel;	///< �p�������O���[�v�B
			/// �R���X�g���N�^
			TParam(void) { }
			/// �R���X�g���N�^
			TParam(EDistance dis, int v, CParallelGroup& para) : distance(dis), value(v), parallel(para) { }
		};
		/// TParam �̔z��^
		typedef CSimpleVectorT<TParam> CParamVector;
		CPointerHandleT<CParamVector>	m_params;	///< �p�������O���[�v�Q
	};


	//-------------------------------------


	/// �R���X�g���N�^
	CGroupLayout(void)
	{
	}

	/**
	 * [�o�^] �����V�[�P���V�����O���[�v�o�^.
	 *	@param g �V�[�P���V�����O���[�v�B
	 */
	void SetHorizontalGroup(CSequentialGroup& g)
	{
		m_hGroup = g;
	}

	/**
	 * [�o�^] �����V�[�P���V�����O���[�v�o�^.
	 *	@param g �V�[�P���V�����O���[�v�B
	 */
	void SetVerticalGroup(CSequentialGroup& g)
	{
		m_vGroup = g;
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual ILayout* Clone(void) const
	{
		CGroupLayout* P = new CGroupLayout();
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
		// X���v�Z
		size_t hcnt = m_hGroup.GetGroupCount();
		_size.cx = (hcnt == 0) ? 0 :m_hGroup.GetMinimumSize().cx;
		// Y���v�Z
		size_t vcnt = m_vGroup.GetGroupCount();
		_size.cy = (vcnt == 0) ? 0 : m_vGroup.GetMinimumSize().cy;
		// �␳
		AddChinkSize(_size, hcnt, vcnt);
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
		size_t hcnt = m_hGroup.GetGroupCount();
		size_t vcnt = m_vGroup.GetGroupCount();
		SIZE chinkSize = { 0, 0 };
		AddChinkSize(chinkSize, hcnt, vcnt);
		//======= X���W 
		if ( x >= 0 && hcnt > 0 )
		{
			//-- �L����
			INT_PTR width = m_layoutSize.cx - chinkSize.cx;
			INT_PTR widthOrg = width;
			//
			INT_PTR xx = m_margn.left;
			CWorkMemT<INT_PTR> xpos(hcnt);
			CWorkMemT<size_t> xlen(hcnt);
			INDEX rastItem = INVALID_INDEX;
			loop ( index, hcnt )
			{
				xpos[index] = xx;
				const CParallelGroup& pg = m_hGroup.GetGroup(index);
				size_t val = m_hGroup.GetGroupValue(index);
				EDistance dis = m_hGroup.GetGroupDistance(index);
				if ( dis == Distance_Default )
				{
					dis = Distance_ItemMax;
					if ( index == hcnt - 1 && rastItem == INVALID_INDEX )
					{
						dis = Distance_Rast;
					}
				}
				switch ( dis )
				{
				case Distance_Absolute:// ��Βl
					val = LOWORD(val);
					break;
				case Distance_Rate:// �䗦
					val = widthOrg * val / 100;
					break;
				case Distance_ItemMax:// �A�C�e���ő�
					val = pg.GetMinimumSize().cx;
					break;
				case Distance_Rast:// �]��(��w��֎~)
					val = 0;
					ASSERT( rastItem == INVALID_INDEX );
					rastItem = index;
					break;
				default:
					ASSERT( false );
					break;
				}
				xlen[index] = val;
				width -= val;
				xx += val;
				xx += m_gapSize.cx; //�A�C�e���Ԃ̃T�C�Y
			}
			if ( rastItem != INVALID_INDEX )
			{
				// rast�A�C�e������B�ȍ~�A���炷
				xlen[rastItem] = width;
				for ( size_t i = rastItem + 1; i < hcnt; i++ )
				{
					xpos[i] += width;
				}
			}
			loop ( index, hcnt )
			{
				CParallelGroup& pg = m_hGroup.GetGroup(index);
				loop ( i, pg.GetItemCount() )
				{
					HorizontalItemDecide(pg.GetItemLayout(i), x, xpos[index], xlen[index], pg.GetItemLocation(i), wParam, lParam);
				}
			}
		}
		//======= Y���W 
		if ( y >= 0 && vcnt > 0 )
		{
			//-- �L����
			INT_PTR height = m_layoutSize.cy - chinkSize.cy;
			INT_PTR heightOrg = height;
			//
			INT_PTR yy = m_margn.top;
			CWorkMemT<INT_PTR> ypos(vcnt);
			CWorkMemT<size_t> ylen(vcnt);
			INDEX rastItem = INVALID_INDEX;
			loop ( index, vcnt )
			{
				ypos[index] = yy;
				const CParallelGroup& pg = m_vGroup.GetGroup(index);
				size_t val = m_vGroup.GetGroupValue(index);
				EDistance dis = m_vGroup.GetGroupDistance(index);
				if ( dis == Distance_Default )
				{
					dis = Distance_ItemMax;
					if ( index == vcnt - 1 && rastItem == INVALID_INDEX )
					{
						dis = Distance_Rast;
					}
				}
				switch ( dis )
				{
				case Distance_Absolute:// ��Βl
					val = HIWORD(val);
					break;
				case Distance_Rate:// �䗦
					val = heightOrg * val / 100;
					break;
				case Distance_ItemMax:// �A�C�e���ő�
					val = pg.GetMinimumSize().cy;
					break;
				case Distance_Rast:// �]��(��w��֎~)
					val = 0;
					ASSERT( rastItem == INVALID_INDEX );
					rastItem = index;
					break;
				default:
					ASSERT( false );
					break;
				}
				ylen[index] = val;
				height -= val;
				yy += val;
				yy += m_gapSize.cy; //�A�C�e���Ԃ̃T�C�Y
			}
			if ( rastItem != INVALID_INDEX )
			{
				// rast�A�C�e������B�ȍ~�A���炷
				ylen[rastItem] = height;
				for ( size_t i = rastItem + 1; i < vcnt; i++ )
				{
					ypos[i] += height;
				}
			}
			loop ( index, vcnt )
			{
				CParallelGroup& pg = m_vGroup.GetGroup(index);
				loop ( i, pg.GetItemCount() )
				{
					VerticalItemDecide(pg.GetItemLayout(i), y, ypos[index], ylen[index], pg.GetItemLocation(i), wParam, lParam);
				}
			}
		}
	}


	//------------------------


	/**
	 * [�쐬] �p�������O���[�v�쐬.
	 *	@note �O���[�o���֐��ł��B������g�����ƂŁA�p�������O���[�v�N���X�쐬���ÖٓI�ɍς܂����Ƃ��o���܂��B
	 *	@note �K��̔z�u����w�肵�܂��B�ȍ~�A�z�u����ȗ����� Add ����ƋK��̔z�u�킪�g���܂��B
	 *	@param defloc ����̔z�u��
	 *	@return CParallelGroup.
	 */
	inline friend CParallelGroup Add(ELocation defloc)
	{
		return CParallelGroup(defloc);
	}

	/**
	 * [�쐬] �p�������O���[�v�쐬���A�C�e���ǉ�.
	 *	@note �O���[�o���֐��ł��B������g�����ƂŁA�p�������O���[�v�N���X�쐬���ÖٓI�ɍς܂����Ƃ��o���܂��B
	 *	@param layout ���C�A�E�g�A�C�e��.
	 *	@return CParallelGroup.
	 */
	inline friend CParallelGroup Add(const ILayout& layout)
	{
		return CParallelGroup().Add(layout);
	}

	/**
	 * [�쐬] �p�������O���[�v�쐬���A�C�e���ǉ�.
	 *	@note �O���[�o���֐��ł��B������g�����ƂŁA�p�������O���[�v�N���X�쐬���ÖٓI�ɍς܂����Ƃ��o���܂��B
	 *	@param layout ���C�A�E�g�A�C�e��.
	 *	@param loc �z�u��.
	 *	@return CParallelGroup.
	 */
	inline friend CParallelGroup Add(const ILayout& layout, ELocation loc)
	{
		return CParallelGroup().Add(layout, loc);
	}

	/**
	 * [�쐬] �V�[�P���V�����O���[�v�쐬���p�������O���[�v�ǉ�.
	 *	@note �O���[�o���֐��ł��B������g�����ƂŁA�V�[�P���V�����O���[�v�N���X�쐬���ÖٓI�ɍς܂����Ƃ��o���܂��B
	 *	@param g �p�������O���[�v�B�{�N���X�ɂ̓R�s�[���i�[����܂��B
	 *	@param dis ������B�ȗ������ Distance_Default ���w�肳��܂��B
	 *	@param val �l�B�����킪 Distance_Absolute �̏ꍇ�͏c���̐�Βl MAKELONG(X, Y) ���w��B 
	 *					Distance_Rate �̏ꍇ�͔䗦�i���j���w��B����ȊO�̏ꍇ�͏ȗ��\�B
	 *	@return CSequentialGroup.
	 */
	inline friend CSequentialGroup AddGroup(CParallelGroup& g, EDistance dis = Distance_Default, int val = 0)
	{
		return CSequentialGroup().AddGroup(g, dis, val);
	}

private:
	CSequentialGroup	m_hGroup;			///< �����O���[�v X���W�̊Ǘ�
	CSequentialGroup	m_vGroup;			///< �����O���[�v Y���W�̊Ǘ�
};



}; // TNB