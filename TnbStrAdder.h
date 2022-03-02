#pragma once
/**
 *@file
 * ������A���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbBlockLinkVector.h"



//TNB Library
namespace TNB
{



/**@ingroup STRING
 * ������A�����Ǘ�
 *
 *		������̘A���ɓ�������������Ǘ��N���X�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbStrAdder.h
 *
 *	@date 09/10/28	�V�K�쐬
 *	@date 11/08/03	�\���ύX�B��e���v���[�g���B
 */
class CStrAdder : CCopyImpossible
{
public:

	/// �R���X�g���N�^.
	CStrAdder(void) : m_length(0)
	{
	}

	/// �f�X�g���N�^.
	~CStrAdder(void)
	{
		Empty();
	}

	/**
	 * [�擾] �����񒷎擾.
	 *	@return ������
	 */
	size_t GetLength(void) const
	{
		return m_length;
	}

	/**
	 * [�擾] ������擾.
	 *	@note �{���\�b�h�R�[�����ɁA���ە������A�����邽�߁A���ڂ͎��Ԃ������邱�Ƃ��L��܂��B
	 *		�܂��A�Ԃ����A�h���X�́A�{�C���X�^���X�𑀍삷��ƁA�����ɂȂ�܂��̂ł����ӂ��������B
	 *	@return ������
	 */
	operator LPCTSTR(void) const
	{
		if ( m_string.GetSize() == 0 )
		{
			m_string.Resize(m_length + 1);
			m_string[m_length] = 0;
			size_t a = 0;
			loop ( i, m_work )
			{
				const TParam& p = m_work[i];
				if ( p.length == 1 )
				{
					m_string[a] = p.one;
				}
				else if ( p.length > 1 )
				{
					memcpy(&m_string[a], p.pMany, p.length * sizeof(TCHAR));
				}
				a += p.length;
			}
		}
		return m_string;
	}

	/**
	 * [�ݒ�] ����ۉ�.
	 *	@note �o�b�t�@�͊J�����܂���B
	 */
	void Empty(void)
	{
		m_length = 0;
		loop ( i, m_work )
		{
			m_work[i].Empty();
		}
		m_work.RemoveAll();
		m_string.Free();
	}

	/**
	 * [�ǉ�] ������ǉ�.
	 *	@param len �ǉ����\�z�����ő咷�B
	 *	@param lpszFmt �t�H�[�}�b�g.
	 *	@param ... �p�����[�^
	 *	@return �ǉ�����������
	 */
	int AddFormat(size_t len, LPCTSTR lpszFmt, ...)
	{
		va_list args;
		va_start(args, lpszFmt);
		TParam& p = m_New();
		p.SetLength(max(2, len));
		LPTSTR lpszWork = p.pMany;
		#if _MSC_VER < 1400 || defined(_WIN32_WCE)
			int r = _vstprintf(lpszWork, lpszFmt, args);
		#else
			int r = _vstprintf_s(lpszWork, len, lpszFmt, args);
		#endif
		if ( ToInt(len) < r )
		{
			ASSERT0( false, "CStrAdder::AddFormat", "�p�ӂ����T�C�Y�𒴂��܂���" );
			r = -1;
		}
		if ( r == 1 )
		{
			TCHAR c = lpszFmt[0];
			p.Set(c);
		}
		else if ( r > 1 )
		{
			p.length = r;
			m_length += r;
		}
		else
		{
			p.Empty();
			size_t l = m_work.GetSize();
			m_work.SetSize(l - 1);
		}
		va_end(args);
		return r;
	}

	/**
	 * [�ǉ�] ������ǉ�.
	 *	@note �ǉ������T�C�Y���s���ȏꍇ�ɂ�������g���܂��B
	 *	@param lpszFmt �t�H�[�}�b�g.
	 *	@param ... �p�����[�^
	 *	@return �ǉ�����������
	 */
	size_t AddFormat(LPCTSTR lpszFmt, ...)
	{
		va_list args;
		va_start(args, lpszFmt);
		CStr s;
		s.FormatV(lpszFmt, args);
		Add(s);
		va_end(args);
		return s.GetLength();
	}

	/**
	 * [�ǉ�] ������ǉ�.
	 *	@param lpsz ������.
	 */
	void Add(LPCTSTR lpsz)
	{
		TParam& p = m_New();
		p.Set(lpsz);
		m_length += p.length;
	}

	/**
	 * [�ǉ�] ������ǉ�.
	 *	@param lpsz ������.
	 */
	void operator+=(LPCTSTR lpsz)
	{
		Add(lpsz);
	}

	/**
	 * [�ǉ�] �����ǉ�.
	 *	@param c ����.
	 */
	void Add(TCHAR c)
	{
		TParam& p = m_New();
		p.Set(c);
		m_length += 1;
	}

	/**
	 * [�ǉ�] �����ǉ�.
	 *	@param c ����.
	 */
	void operator+=(TCHAR c)
	{
		Add(c);
	}

	/**
	 * [�ݒ�] �o�b�t�@����.
	 *	@note ���߂Ƀo�b�t�@���w�肵�Ă������ƂŁA�o�b�t�@�Ċm�ۂ̕K�v������A�������x�������Ȃ�܂��B
	 *	@param len �ǉ�����o�b�t�@�T�C�Y
	 */
	void Regulate(size_t len)
	{
	}

private:
	/// �p�����^
	struct TParam
	{
		size_t	length;			///< ����
		union
		{
			TCHAR	one;		///< ����1�̏ꍇ�̃f�[�^
			TCHAR*	pMany;		///< ����2�ȏ�̏ꍇ�̃f�[�^�Ǘ�
		};
		/// �R���X�g���N�^
		TParam(void) : length(0)
		{
		}
		/// �����̂ݐݒ�
		void SetLength(size_t len)
		{
			Empty();
			length = len;
			if ( length > 1 )
			{
				pMany = new TCHAR[length + 1];
			}
		}
		/// ������ݒ�
		void Set(LPCTSTR lpsz)
		{
			Empty();
			length = STRLIB::GetLen(lpsz);
			if ( length == 1 )
			{
				one = lpsz[0];
			}
			else if ( length > 1 )
			{
				pMany = new TCHAR[length + 1];
				STRLIB::Copy(pMany, lpsz);
			}
		}
		/// �Z�b�^�[
		void Set(TCHAR c)
		{
			Empty();
			length = 1;
			one = c;
		}
		/// ��
		void Empty(void)
		{
			if ( length > 1 )
			{
				delete[] pMany;
			}
			length = 0;
		}
	};
	/// �V�K�쐬
	TParam& m_New(void)
	{
		m_string.Free();
		INDEX r = m_work.Add(TParam());
		return m_work[r];
	}
	CBlockLinkVectorT<TParam>	m_work;		///< ���[�N
	size_t						m_length;	///< ������
	mutable CWorkMemT<TCHAR>	m_string;	///< operator LPCTSTR �p
};



}; //TNB
