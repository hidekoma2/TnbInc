#pragma once
/**
 *@file
 * �����t��������Ǘ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include "TnbVector.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup STRING
 * �����t��������Ǘ�
 *
 *		ASCII/SJIS �� UNICODE�i�v���W�F�N�g�̐ݒ�ɂ��j�̈ꕶ�����Ƃɑ΂ɂȂ�A
 *		�����iTYP)���Ǘ����܂��B
 *
 *	@note	printf�n�� Format() Fmt() �� ��s �ɑ΂��āA���̂܂ܓn���܂���B
 *			CStr �ɕϊ����Ă���n���Ă��������B
 *
 *	@param TYP �����̌^
 *
 *	@par�K�v�t�@�C��
 *			TnbAttributedStr.h
 * 
 *	@date 06/10/10	�V�K�쐬
 */
template<typename TYP>
class CAttributedStrT
{
	CStr			m_str;		///< ������
	CVectorT<TYP>	m_vattr;	///< ����

public:

	/**
	 * �R���X�g���N�^
	 *	@note �����͑S�� TYP() �ɂȂ�܂��B
	 *	@param lpsz ���������
	 */
	explicit CAttributedStrT(LPCTSTR lpsz = NULL) : m_str(lpsz)
	{
		m_vattr.SetSize(m_str.GetLength());
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param as �R�s�[���B
	 */
	CAttributedStrT(const CAttributedStrT& as) : m_str(as.m_str), m_vattr(as.m_vattr)
	{
		ASSERTLIB(m_vattr.GetSize() == m_str.GetLength());
	}

	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@param other �R�s�[���B
	 *	@return �����̎Q�ƁB
	 */
	CAttributedStrT& operator=(const CAttributedStrT& other)
	{
		m_str = other.m_str;
		m_vattr = other.m_vattr;
		return *this;
	}

	/**
	 * [���] ���
	 *	@note �����͑S�� TYP() �ɂȂ�܂��B
	 *	@param lpsz ���������
	 *	@return �����̎Q�ƁB
	 */
	CAttributedStrT& operator=(LPCTSTR lpsz)
	{
		m_str = lpsz;
		m_vattr.RemoveAll();
		m_vattr.SetSize(m_str.GetLength());
		return *this;
	}

	/** [�폜] �� */
	void Empty(void)
	{
		m_str.Empty();
		m_vattr.RemoveAll();
	}

	/**
	 * [�擾] �Q��
	 *	@return �����Ă��镶����̐擪�|�C���^
	 */
	LPCTSTR Refer(void) const
	{
		return m_str;
	}

	/**
	 * [�ǉ�] ���Z����I�y���[�^
	 *	@param as �ǉ����e
	 *	@return �����̎Q��
	 */
	CAttributedStrT& operator+=(const CAttributedStrT& as)
	{
		m_str += as.m_str;
		m_vattr += as.m_vattr;
		ASSERTLIB(m_vattr.GetSize() == static_cast<size_t>(m_str.GetLength()));
		return *this;
	}

	/**
	 * [�ǉ�] ���Z����I�y���[�^
	 *	@note �ǉ����ꂽ�����̑����͑S�� TYP() �ɂȂ�܂��B
	 *	@param lpsz ������
	 *	@return �����̎Q��
	 */
	CAttributedStrT& operator+=(LPCTSTR lpsz)
	{
		m_str += lpsz;
		m_vattr.SetSize(m_str.GetLength());
		ASSERTLIB(m_vattr.GetSize() == m_str.GetLength());
		return *this;
	}

	/**
	 * [����] �ꕶ���ǉ�
	 *	@param c �ǉ����镶���B
	 *	@param t ���̑����B
	 *	@return �����̎Q��
	 */
	CAttributedStrT& AddAt(TCHAR c, TYP t)
	{
		m_str += c;
		m_vattr.Add(t);
		ASSERTLIB(m_vattr.GetSize() == m_str.GetLength());
		return *this;
	}

	/**
	 * [����] �ꕶ���u������
	 *	@param index �u�������C���f�b�N�X
	 *	@param c �u�������镶���B
	 *	@param t ���̑����B
	 */
	void SetAt(INDEX index, TCHAR c, TYP t)
	{
		m_str.SetAt(index, c);
		m_vattr.Set(index, t);
	}

	/**
	 * [�}��] �����}��
	 *	@param index �}���J�n�C���f�b�N�X
	 *	@param c �����B
	 *	@param t ���̑����B
	 */
	void InsertAt(INDEX index, TCHAR c, TYP t)
	{
		m_str.InsertAt(index, c);
		m_vattr.Insert(index, t);
	}

	/**
	 * [�}��] ������}��
	 *	@param index �}���J�n�C���f�b�N�X
	 *	@param lpsz ������B
	 *	@param t ���̑���(�S�Ă��̑����ɂȂ�܂�)�B
	 */
	void Insert(INDEX index, LPCTSTR lpsz, TYP t=TYP())
	{
		m_str.Insert(index, lpsz);
		loop ( i, STRLIB::GetLen(lpsz) )
		{
			m_vattr.Insert(index, t);
		}
	}

	/**
	 * [�}��] ������}��
	 *	@param index �}���J�n�C���f�b�N�X
	 *	@param as ������B
	 */
	void Insert(INDEX index, const CAttributedStrT& as)
	{
		m_str.Insert(index, as.m_str);
		loop ( i, as.m_vattr.GetSize() )
		{
			m_vattr.Insert(index + i, as.m_vattr[i]);
		}
	}

	/**
	 * [����] �����폜.
	 *	@note ���[�h�o�C�g(�����R�[�h�̈�o�C�g��)���w�肷��ƁA�����g���C���o�C�g���폜����܂��B
	 *	@param index �폜����C���f�b�N�X
	 *	@param iLen �폜���镶����(�����𒴂��Ă���ꍇ�A�w��C���f�b�N�X�ȉ������ׂč폜����܂�)
	 *	@retval 0 �폜���Ă��Ȃ�
	 *	@retval 1�ȏ� �폜�����������BASCII/SJIS�̏ꍇ�AiLen �Ɠ����l�ɂȂ�Ƃ͌���܂���B
	 */
	int Delete(INDEX index, int iLen = 1)
	{
		int l = m_str.Delete(index, iLen);
		loop ( i, l )
		{
			m_vattr.Remove(index);
		}
		return l;
	}

	/**
	 * [�擾] �ꕶ���擾
	 *	@param index �C���f�b�N�X�B1�����ڂ��C���f�b�N�X�O�ɂȂ�܂��B
	 *	@return �C���f�b�N�X�̈ʒu�̕����B
	 */
	TCHAR GetAt(INDEX index) const
	{
		return m_str.GetAt(index);
	}

	/**
	 * [�擾] �����擾
	 *	@param index �C���f�b�N�X�B1�����ڂ��C���f�b�N�X�O�ɂȂ�܂��B
	 *	@return �C���f�b�N�X�̈ʒu�̑����B
	 */
	TYP GetAttribute(INDEX index) const
	{
		return m_vattr.Get(index);
	}

	/**
	 * [�ݒ�] �����ݒ�
	 *	@param index �C���f�b�N�X�B1�����ڂ��C���f�b�N�X�O�ɂȂ�܂��B
	 *	@param t �ݒ肷�鑮��
	 */
	void SetAttribute(INDEX index, TYP t)
	{
		m_vattr.Set(index, t);
	}

	/**
	 * [�擾] ������
	 *	@return ������
	 */
	size_t GetLength(void) const
	{
		return m_str.GetLength();
	}

	/**
	 * [�m�F] ��`�F�b�N
	 *	@retval true	�����
	 *	@retval false	�f�[�^�L��
	 */
	bool IsEmpty(void) const
	{
		return m_str.IsEmpty();
	}

	/**
	 * [�m�F] ����
	 *	@param lpsz �������镶����
	 *	@param iFromIndex �������J�n����C���f�b�N�X�B�ȗ�����Ɠ�����ɂȂ�܂��B
	 *	@retval -1 ������
	 *	@retval 0�ȏ� �����C���f�b�N�X
	 */
	int Find(LPCTSTR lpsz, INDEX iFromIndex = 0) const
	{
		return m_str.Find(lpsz, iFromIndex);
	}

	/**
	 * [�m�F] ����
	 *	@param c �������镶��
	 *	@param iFromIndex �������J�n����C���f�b�N�X�B�ȗ�����Ɠ�����ɂȂ�܂��B
	 *	@retval -1 ������
	 *	@retval 0�ȏ� �����C���f�b�N�X
	 */
	int Find(TCHAR c, INDEX iFromIndex = 0) const
	{
		return m_str.Find(c, iFromIndex);
	}

	/**
	 * [�m�F] ��������
	 *	@param t �������鑮��
	 *	@param iFromIndex �������J�n����C���f�b�N�X�B�ȗ�����Ɠ�����ɂȂ�܂��B
	 *	@retval -1 ������
	 *	@retval 0�ȏ� �����C���f�b�N�X
	 */
	int FindAttribute(TYP t, INDEX iFromIndex = 0) const
	{
		return m_vattr.Find(c, iFromIndex);
	}
};



}; // TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")

