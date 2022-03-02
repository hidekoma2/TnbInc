#pragma once
/**
 *@file
 * TinyMacroLang �v�Z�֌W�̃w�b�_
 *	
 *		TML�̌v�Z�̃N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbTmlResult.h"
#include "TnbTmlValue.h"
#include "TnbStrLib.h"



//TNB Library
namespace TNB
{



/**@ingroup TML
 * TinyMacroLang ��������
 *
 *		������\�L�� TML �p�ɕϊ����邽�߂Ɏg�p���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbTmlTextEscape.h
 *
 *	@date 06/01/01 �V�K�쐬
 *	@date 09/08/18 ���V
 */
class CTmlTextEscape : public CTml
{
public:

	/// �R���X�g���N�^
	CTmlTextEscape(void)
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^;
	 *	@param other �R�s�[��
	 */
	CTmlTextEscape(const CTmlTextEscape& other) : m_contents(other.m_contents), m_aWildPos(other.m_aWildPos)
	{
	}

	/// �f�X�g���N�^
	virtual ~CTmlTextEscape(void)
	{
	}

	/// Empty
	void Empty(void)
	{
		m_contents.RemoveAll();
		m_aWildPos.RemoveAll();
	}

	/** 
	 * [�擾] CTmlValue �^�Ŏ擾.
	 *		�ێ����Ă���f�[�^�� CTmlValue �ɕϊ����܂��B
	 *	@return �ϊ���� CTmlValue
	 */
	CTmlValue ToValue(void) const
	{
		CTmlValue v;
		v = m_contents;
		loop ( i, m_aWildPos.GetSize() )
		{
			v.AddWildPoint(m_aWildPos[i]);
		}
		return v;
	}
	
	/**
	 * [���] ���������.
	 *		�{�C���X�^���X�Ɏw�蕶������i�[���܂��B
	 *		���̍ہA�G�X�P�[�v�V�[�P���X���`�F�b�N���܂��B
	 *	@param lpszData ���������
	 *	@return ���ʁB Result_Nop �͕Ԃ�܂���B
	 */
	CTmlResult SetData(LPCSTR lpszData)
	{
		return m_SetNormal(lpszData);
	}

	/**
	 * [���] HEX�\�L���������.
	 *		�{�C���X�^���X�Ɏw�肳�ꂽ������� HEX�\�L�Ƃ��ĉ�͂��A�i�[���܂��B
	 *	@param lpszData ���������
	 *	@return ���ʁB Result_Nop �͕Ԃ�܂���B
	 */
	CTmlResult SetHexToData(LPCSTR lpszData)
	{
		return m_SetHex(lpszData);
	}

private:
	CByteVector		m_contents;	///< ���e
	CVectorT<INDEX>	m_aWildPos;	///< ���C���h�̈ʒu

	/**
	 * ���������
	 *	@param lpszData ������
	 *	@return ���ʁB
	 */
	CTmlResult m_SetNormal(LPCSTR lpszData)
	{
		Empty();
		CByteVector ba;
		int iPos = 0;
		char c;
		while ( true )
		{
			c = lpszData[iPos++];
			if ( c == 0 )
			{
				//�I�[�� " �Ȃ�I��
				break;
			}
			else if ( STRLIB::GetCharSize(c) == 2 )
			{
				//�S�p
				ba.Add(c);
				ba.Add(lpszData[iPos++]);
			}
			else if ( c=='\\' )
			{
				//���̕����B
				c = lpszData[iPos++];
				switch ( c )
				{
				case '0':
					ba.Add(0);
					break;
				case 'x':
					//HEX�\�L
					{
						int r = 0;
						int i;
						c = lpszData[iPos];
						if ( c == 0 )
						{
							//ESC�R�[�h���s��
							return CTmlResult(Operation_Text_EscHex, Result_InvalidEscSeq);
						}
						i = STRLIB::HexCharToInt(c);
						if ( i < 0 )
						{
							// HEX�\�L���ُ�
							return CTmlResult(Operation_Text_EscHex, Result_InvalidHexNotation);
						}
						r = i * 0x10;
						c = lpszData[iPos + 1];
						if ( c == 0 )
						{
							//ESC�R�[�h���s��
							return CTmlResult(Operation_Text_EscHex, Result_InvalidEscSeq);
						}
						i =  STRLIB::HexCharToInt(c);
						if ( i < 0 )
						{
							// HEX�\�L���ُ�
							return CTmlResult(Operation_Text_EscHex, Result_InvalidHexNotation);
						}
						r += i;
						iPos += 2;
						ba.Add(static_cast<BYTE>(r));
					}
					break;
				case '*':
					//���C���h�J�[�h
					m_aWildPos.Add(ba.GetSize());
					ba.Add('_' );
					break;
				default:
					//ESC�R�[�h���s��
					return CTmlResult(Operation_Text_Esc, Result_InvalidEscSeq);
				}
			}
			else
			{
				ba.Add(c);
			}
		}
		m_contents = ba;
		return Result_Success;
	}

	/**
	 * HEX�w��ő��
	 * "ABCD" ��������ƁA0xAB 0xCD �Ƃ��ēo�^
	 *	���C���h�J�[�h�͎g���܂���B
	 *	@param lpszData ������
	 *	@return ���ʁB
	 */
	CTmlResult m_SetHex(LPCSTR lpszData)
	{
		Empty();
		//
		CAscii s = lpszData;
		s.Remove(' ');
		s.Remove('-');
		s.Remove('_');
		lpszData = s;
		//
		CByteVector ba;
		size_t max = STRLIB::GetLen(lpszData);
		for ( UINT i = 0; i < max; i += 2 )
		{
			if ( lpszData[i] == 0 )
			{
				break;
			}
			int r1 = STRLIB::HexCharToInt(lpszData[i]);
			int r2 = STRLIB::HexCharToInt(lpszData[i + 1]);
			if ( r1 >= 0 && r2 >= 0 )
			{
				//HEX�\�L��!
				ba.Add (static_cast<BYTE>(r1 * 0x10 + r2));
			}
			else
			{
				// HEX�\�L���ُ�
				return CTmlResult(Operation_Text, Result_InvalidHexNotation);
			}
		}
		m_contents = ba;
		return Result_Success;
	}

	friend class CTmlTextEscapeTest;
};



}; // TNB


