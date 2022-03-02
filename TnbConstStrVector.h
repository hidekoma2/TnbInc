#pragma once
/**
 *@file
 * ��������z��Ǘ��֌W�̃w�b�_
 *
 *		������̔z����Ǘ����܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbBlockLinkVector.h"



//TNB Library
namespace TNB
{



/**@ingroup STRING
 * �s�ϓI��������z��Ǘ��N���X.
 *
 *		��̃e�L�X�g����A�~�������ɂ����ĕ������z��Ƃ��ċL�����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbConstStrVector.h
 * 
 *	@date 09/08/31	�V�K�쐬
 */
template<typename TYP, TYP C1 = 0x0D, TYP C2 = 0x0A>
class CConstStrVectorT : CCopyImpossible
{
public:

	/// �R���X�g���N�^
	CConstStrVectorT(void) : m_lpszText(NULL)
	{
	}

	/**
	 * [�ݒ�] ����ۉ�.
	 *		�ێ����Ă���f�[�^��j�����܂��B
	 */
	void Empty(void)
	{
		m_work.Free();
		m_params.RemoveAll();
	}

	/**
	 * [�ݒ�] �f�[�^�ݒ�.
	 *		�w��̕�����f�[�^�𕡐����A�L�����܂��B
	 *	@param lpszText ������f�[�^
	 *	@return �������B
	 */
	size_t SetClone(const TYP* lpszText)
	{
		size_t len = STRLIB::GetLen(lpszText);
		m_work.Resize(len + 1);
		STRLIB::Copy(m_work, lpszText);
		return SetRefered(m_work);
	}

	/**
	 * [�ݒ�] �f�[�^�ݒ�.
	 *		�w��̕�����f�[�^�̃A�h���X���L�����g�p���܂��B
	 *		���̂��߁A�{�N���X�g�p���́A�w�肵��������f�[�^��j�����Ȃ��ł��������B
	 *	@param lpszText ������f�[�^
	 *	@return �������B
	 */
	size_t SetRefered(TYP* lpszText)
	{
		m_lpszText = lpszText;
		m_params.RemoveAll();
		INDEX ind = 0;
		while ( true )
		{
			TYP* P = lpszText;
			size_t len = 0;
			while ( true )
			{
				if ( *P == 0 )
				{
					if ( len == 0 )
					{
						return m_params.GetSize();
					}
					else
					{
						break;
					}
				}
				else if ( *P == C1 || *P == C2 )
				{
					*P++ = 0;
					if ( C2 != 0 && *P == C2 )
					{
						*P++ = 0;
					}
					break;
				}
				int s = STRLIB::GetCharSize(*P);
				P += s;
				len += s;
			}
			m_params.Add(TParam(ind, len));
			ind += P - lpszText;
			lpszText += P - lpszText;
		}
		ASSERT( false );
		return 0;
	}

	/**
	 * [�擾] ���C�����擾.
	 *	@return ���C����
	 */
	size_t GetLineCount(void) const
	{
		return m_params.GetSize();
	}

	/**
	 * [�擾] �����擾.
	 *	@param index �C���f�b�N�X�B�O�ȏ� GetLength() �����B
	 *	@return ����
	 */
	size_t GetLength(INDEX index) const
	{
		return m_params[index].size;
	}

	/**
	 * [�擾] ������擾.
	 *	@param index �C���f�b�N�X�B�O�ȏ� GetLength() �����B
	 *	@return ������
	 */
	const TYP* GetString(INDEX index) const
	{
		return &m_lpszText[m_params[index].index];
	}

private:
	struct TParam
	{
		INDEX	index;
		size_t	size;
		// �R���X�g���N�^.
		TParam(INDEX i = 0, size_t s = 0) : index(i), size(s)
		{
		}
	};
	CBlockLinkVectorT<TParam>	m_params;
	TYP*						m_lpszText;
	CWorkMemT<TYP>				m_work;
};



/**
 * �s�ϓI��������z��Ǘ��N���X.
 *
 *	@par�K�v�t�@�C��
 *			TnbConstStrVector.h
 * 
 *	@date 09/08/31	�V�K�쐬
 */
typedef CConstStrVectorT<TCHAR> CConstStrVector;



}; // TNB