#pragma once
/**
 *@file
 * �e�L�X�g�t�@�C���֌W�̃w�b�_
 *
 *		�e�L�X�g�t�@�C���̓ǂݏ������T�|�[�g����N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbFile.h"
#include "TnbConstStrVector.h"
#include "TnbStrAdder.h"
#include "TnbStrVector.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE STRING
 * �e�L�X�g�t�@�C�������݃N���X
 *
 *	��������w�肵�ăt�@�C���ɏ������ނ��Ƃ��ł��܂��B
 *
 *	@note UNICODE �e�L�X�g�t�@�C�����쐬�ł��܂��B
 *	@note ���s�R�[�h�� ��r��n �ɂȂ�܂��B
 *	@note �Ō�̍s�ɉ��s�R�[�h�͕t���܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbTextFile.h
 *
 *	@date 11/02/10 �V�K�쐬
 *	@date 11/10/12 NewFile() �V�K�B
 */
class CTextFileWriter : CCopyImpossible
{
public:

	/// �R���X�g���N�^
	CTextFileWriter(void) : m_pWriter(&m_fw), m_isUnicode(false)
	{
	}

	/**
	 * [�m�F] �����݉\�H.
	 *		Open ���Ă��邩�m�F�ł��܂��B
	 *	@retval true �I�[�v���ς݁i�����݉\�j
	 *	@retval false ���I�[�v��
	 */
	bool CanWrite(void) const
	{
		m_pWriter->CanWrite();
	}

	/**
	 * [����] �N���[�Y.
	 *	@note �I�[�v�����Ă��Ȃ������牽�����܂���B
	 */
	void Close(void)
	{
		m_pWriter->Close();
		m_pWriter = &m_fw;
	}

	/**
	 * [�쐬] �V�K�t�@�C���쐬.
	 *	@param lpszName �t�@�C����.
	 *	@param isUnicode true �Ȃ� UNICODE TEXT �t�@�C�����쐬���܂��B
	 *					�ȗ��� false �Ȃ� S-JIS TEXT �t�@�C�����쐬���܂��B
	 *	@retval true ����.
	 *	@retval	false ���s.
	 */
	bool New(LPCTSTR lpszName, bool isUnicode = false)
	{
		Close();
		bool r = m_fw.New(lpszName);
		if ( r )
		{
			r = m_Open(&m_fw, isUnicode);
		}
		return r;
	}

	/**
	 * [�쐬] �V�K�t�@�C���쐬.
	 *	@param pWriter ���C�^�[�B�������݃I�[�v���ς݁A�T�C�Y0�ł���K�v������܂��B
	 *	@param isUnicode true �Ȃ� UNICODE TEXT �t�@�C�����쐬���܂��B
	 *					�ȗ��� false �Ȃ� S-JIS TEXT �t�@�C�����쐬���܂��B
	 *	@retval true ����.
	 *	@retval	false ���s.
	 */
	bool Open(IWriter* pWriter, bool isUnicode = false)
	{
		Close();
		return m_Open(pWriter, isUnicode);
	}

	/**
	 * [����] ��s��������.
	 *	@note ���s�R�[�h�͂��Ȃ��ł�������
	 *	@param lpszLine ������
	 *	@throw CWriteFailureException �����݂Ɏ��s����ƁA�X���[����܂��B
	 */
	void WriteLine(LPCSTR lpszLine)
	{
		CAscii s = lpszLine;
		s += "\r\n";
		m_Write(s);
	}

	/**
	 * [����] ��s��������.
	 *	@note ���s�R�[�h�͂��Ȃ��ł�������
	 *	@param lpszLine ������
	 *	@throw CWriteFailureException �����݂Ɏ��s����ƁA�X���[����܂��B
	 */
	void WriteLine(LPCWSTR lpszLine)
	{
		CUnicode s = lpszLine;
		s += L"\r\n";
		m_Write(s);
	}

	/**
	 * [����] ������������.
	 *	@note �K�v�ɉ����ĉ��s�R�[�h�͂����Ă��������B
	 *	@param lpszFormat ����
	 *	@param ... �p�����[�^
	 *	@throw CWriteFailureException �����݂Ɏ��s����ƁA�X���[����܂��B
	 */
	void WriteFormat(LPCSTR lpszFormat, ...)
	{
		CAscii s;
		va_list va;
		va_start(va, lpszFormat);
		s.FormatV(lpszFormat, va);
		va_end(va);
		m_Write(s);
	}

	/**
	 * [����] ������������.
	 *	@note �K�v�ɉ����ĉ��s�R�[�h�͂����Ă��������B
	 *	@param lpszFormat ����
	 *	@param ... �p�����[�^
	 *	@throw CWriteFailureException �����݂Ɏ��s����ƁA�X���[����܂��B
	 */
	void WriteFormat(LPCWSTR lpszFormat, ...)
	{
		CUnicode s;
		va_list va;
		va_start(va, lpszFormat);
		s.FormatV(lpszFormat, va);
		va_end(va);
		m_Write(s);
	}

	/**
	 * [����] ��������.
	 *	@note ������z��v�f���ƂɁA�����I�ɉ��s���}������܂��B
	 *	@param vs ������z��
	 *	@throw CWriteFailureException �����݂Ɏ��s����ƁA�X���[����܂��B
	 */
	void Write(const CStrVector& vs)
	{
		CStr s;
		loop ( i, vs )
		{
			s = vs[i];
			s += _T("\r\n");
			m_Write(s);
		}
	}

	/**
	 * [�쐬] �V�K�t�@�C���쐬.
	 *	@note ���s���Ă��Athrow ���܂���B
	 *	@param lpszName �t�@�C����.
	 *	@param vs ������z��
	 *	@param isUnicode true �Ȃ� UNICODE TEXT �t�@�C�����쐬���܂��B
	 *					�ȗ��� false �Ȃ� S-JIS TEXT �t�@�C�����쐬���܂��B
	 *	@retval true ����.
	 *	@retval	false ���s.
	 */
	static bool NewFile(LPCTSTR lpszName, const CStrVector& vs, bool isUnicode = false)
	{
		CTextFileWriter w;
		if ( w.New(lpszName, isUnicode) )
		{
			try
			{
				w.Write(vs);
				return true;
			}
			catch ( ... )
			{
			}
		}
		return false;
 	}

private:
	/**
	 * [�쐬] �V�K�t�@�C���쐬.
	 *	@param pWriter ���C�^�[�B�������݃I�[�v���ς݁A�T�C�Y0�ł���K�v������܂��B
	 *	@param isUnicode true �Ȃ� UNICODE TEXT �t�@�C�����쐬���܂��B
	 *					�ȗ��� false �Ȃ� S-JIS TEXT �t�@�C�����쐬���܂��B
	 *	@retval true ����.
	 *	@retval	false ���s.
	 */
	bool m_Open(IWriter* pWriter, bool isUnicode = false)
	{
		if ( pWriter->CanWrite() && pWriter->GetSize() == 0 )
		{
			m_isUnicode = isUnicode;
			if ( ! m_isUnicode )
			{
				return true;
			}
			else
			{
				const BYTE buf[2] = { 0xFF, 0xFE };
				try
				{
					pWriter->Write(2, buf);
					m_pWriter = pWriter;
					return true;
				}
				catch ( ... )
				{
				}
			}
		}
		return false;
	}
	// ������(ASCII������w��)
	void m_Write(const CStrT<char>& str)
	{
		if ( m_isUnicode )
		{
			CUnicode uni = str;
			m_pWriter->Write(uni.GetLength() * 2, uni.operator LPCWSTR());
		}
		else
		{
			m_pWriter->Write(str.GetLength(), str.operator LPCSTR());
		}
	}
	// ������(UNICODE������w��)
	void m_Write(const CStrT<WCHAR>& str)
	{
		if ( m_isUnicode )
		{
			m_pWriter->Write(str.GetLength() * 2, str.operator LPCWSTR());
		}
		else
		{
			CAscii asc = str;
			m_pWriter->Write(asc.GetLength(), asc.operator LPCSTR());
		}
	}
	bool		m_isUnicode;	///< UNICODE���[�h�H
	IWriter*	m_pWriter;		///< ���C�^�[
	CFileWriter	m_fw;			///< �t�@�C�����C�^�[
};



/**@ingroup FILE STRING
 * �e�L�X�g�t�@�C���Ǎ��݃N���X
 *
 *	�t�@�C����ǂݍ���ŁA�s�P�ʂɎQ�Ƃł��܂��B
 *
 *	@note UNICODE �e�L�X�g�t�@�C���ǂݍ��݉\�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbTextFile.h
 *
 *	@date 11/02/10 �V�K�쐬
 */
class CTextFileReader : CCopyImpossible
{
public:

	/// �R���X�g���N�^
	CTextFileReader(void) : m_hasUnicode(false)
	{
	}

	/**
	 * [�m�F] ���e����H
	 *	@retval true Read�������A���e�������Ă���B
	 *	@retval false �Ȃ��B
	 */
	bool HasContents(void) const
	{
		return ! m_contents.IsEmpty();
	}

	/**
	 * [�m�F] UNICODE���e����H
	 *	@retval true Read�������AUNICODE �̓��e�������Ă���B
	 *	@retval false �Ȃ��B
	 */
	bool hasUnicodeContents(void) const
	{
		return HasContents() && m_hasUnicode;
	}

	/**
	 * [�ݒ�] ����ۉ�.
	 */
	void Empty(void)
	{
		m_contents.RemoveAll();
	}

	/**
	 * [�Ǎ�] �t�@�C���ǂݍ���.
	 *		�w��t�@�C����ǂݍ��݂܂��B
	 *	@param lpszFileName �t�@�C����
	 *	@retval true �����B���e���C���X�^���X���Ɏ�荞��ł��܂��B
	 *	@retval false ���s�B
	 */
	bool Read(LPCTSTR lpszFileName)
	{
		Empty();
		CFileReader fr;
		if ( fr.Open(lpszFileName) )
		{
			return Read(&fr);
		}
		return false;
	}

	/**
	 * [�Ǎ�] �t�@�C���ǂݍ���.
	 *		�w��t�@�C����ǂݍ��݂܂��B
	 *	@param pReader �ǂݍ��߂��ԂɂȂ��Ă��郊�[�_�[�B
	 *	@retval true �����B���e���C���X�^���X���Ɏ�荞��ł��܂��B
	 *	@retval false ���s�B
	 */
	bool Read(IReader* pReader)
	{
		Empty();
		if ( pReader->CanRead() )
		{
			try
			{
				m_contents = pReader->ReadInto();
				m_contents.Add(0);
				m_contents.Add(0);
				m_hasUnicode = (m_contents[0] == 0xFF && m_contents[1] == 0xFE);
				return true;
			}
			catch ( ... )
			{
			}
		}
		return false;
	}

	/**
	 * [�擾] ���e�擾.
	 *	@note Read �������e�𕶎���z��Ƃ��Ď��o���܂��B
	 *	@param[out] _vs �擾�������e�B
	 *	@retval true �擾����
	 *	@retval false ���s
	 */
	bool GetContents(CConstStrVectorT<char>& _vs) const
	{
		if ( HasContents() )
		{
			if ( ! m_hasUnicode )
			{
				_vs.SetClone(reinterpret_cast<LPCSTR>(m_contents.ReferBuffer()));
			}
			else
			{
				LPCWSTR lpsz = reinterpret_cast<LPCWSTR>(m_contents.ReferBuffer());
				CAscii a = reinterpret_cast<LPCSTR>(&lpsz[1]);
				_vs.SetClone(a);
			}
			return true;
		}
		return false;
	}

	/**
	 * [�擾] ���e�擾.
	 *	@note Read �������e�𕶎���z��Ƃ��Ď��o���܂��B
	 *	@param[out] _vs �擾�������e�B
	 *	@retval true �擾����
	 *	@retval false ���s
	 */
	bool GetContents(CConstStrVectorT<WCHAR>& _vs) const
	{
		if ( HasContents() )
		{
			if ( ! m_hasUnicode )
			{
				CUnicode a = reinterpret_cast<LPCSTR>(m_contents.ReferBuffer());
				_vs.SetClone(a);
			}
			else
			{
				LPCWSTR lpsz = reinterpret_cast<LPCWSTR>(m_contents.ReferBuffer());
				_vs.SetClone(&lpsz[1]);
			}
			return true;
		}
		return false;
	}

private:
	CByteVector	m_contents;		///< ���e��
	bool		m_hasUnicode;	///< UNICODE?
};



}; // TNB
