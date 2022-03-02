#pragma once
/**
 *@file
 * ������Ǘ��֌W�̃w�b�_
 *
 *		��������Ǘ�����N���X���L��܂��B
 *		ASCII/SJIS��UNICODE���v���W�F�N�g�Ɋ֌W�Ȃ��g�����Ƃ��ł��܂��B
 *		�܂��A�����͑��݂ɗe�Ղɕϊ��\�ł��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSerializer.h"
#include "TnbStrLib.h"
#include "TnbTinyStringFormater.h"



//T-TestCase�R�[�h�J�o���b�WDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document �쐬�p�V���{��
 // CStrT��NULL���
// _SELECTANY LPCVOID gpStrNull = &g_plNullBase[3];
#define gpStrNull (LPCVOID)(&(g_plNullBase[3]))
#endif



/**@ingroup STRING ITERATOR
 * ������Ǘ��e���v���[�g
 *
 *		�����A���`�A�ۊǂ��e�Ղɏo���镶����Ǘ��N���X�ł��B
 *
 *	@note IComparableT �͎������Ă��܂��񂪁A��r�@�\�͎����Ă��܂��B
 *
 *	@note ISerializable �͎������Ă��܂��񂪁A�V���A���C�Y�@�\�͎g�p�\�ł��B
 *
 *	@note begin() / end() �ɂ��{@link ITERATOR �����q}�擾���\�ł��B
 *			SJIS �������Ă���ꍇ�A�Q�o�C�g�������l������܂���̂Œ��ӂ��Ă��������B
 *
 *	@attention	�{�e���v���[�g�𒼐ڎg���P�[�X�͗L��܂���B
 *			CStr,CAscii,CUnicode�N���X���p�ӂ���Ă��܂��B
 *
 *	@see CStr, CAscii, CUnicode
 *
 *	@par�K�v�t�@�C��
 *			TnbStr.h
 *
 *	@date 06/01/01	�V�K�쐬
 *	@date 06/05/12	����n�͂�����
 *	@date 06/06/26	UNICODE�n�̃o�b�t�@�m�ې����������Ȃ̂�Byte���ɂȂ��Ă����̂��C���B
 *	@date 06/07/28	operator+ �������o����O���[�o���֐��֕ύX�B LPCTSTR + CStr ���\�ɂ����B
 *	@date 06/08/13	�C�e���[�^�� STL �ɂ��킹�āA���C�B
 *	@date 06/08/30	�V���A���C�Y�֌W��ǉ��B
 *	@date 07/06/29	Sandwich() �̒ǉ��B
 *	@date 10/08/03	FindOneOf(), ReverseFindOneOf() �̒ǉ��B�\�������B
 *	@date 12/03/07	IntToString(), DwordToString(), ToHexString() �ǉ��B
 *	@date 15/03/16	ReferBuffer() ��ǉ��B
 *	@date 19/07/03	MakeUpper(), MakeLower() �V�K�B
 */
template<typename TYP>
class CStrT
{
public:

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
	// const_iterator�^�錾
	typedef const TYP* const_iterator;
	// iterator�^�錾
	typedef TYP* iterator;
	#endif //_TnbDOXYEM

	/**
	 * [����] �擪const_iterator.
	 *		�擪�v�f���w��{@link ITERATOR �����q}���擾
	 *	@return const_iterator
	 */
	const_iterator begin(void) const { return m_lpBuf; }

	/**
	 * [����] �Ō�const_iterator.
	 *		�Ō�v�f�̎����w��{@link ITERATOR �����q}���擾
	 *	@return const_iterator
	 */
	const_iterator end(void) const { return begin() + GetLength(); }

	/**
	 * [����] �擪iterator.
	 *		�擪�v�f���w��{@link ITERATOR �����q}���擾
	 *	@return iterator
	 */
	iterator begin(void) { GetBuffer(); ReleaseBuffer(); return m_lpBuf; }

	/**
	 * [����] �Ō�iterator.
	 *		�Ō�v�f�̎����w��{@link ITERATOR �����q}���擾
	 *	@return iterator
	 */
	iterator end(void) { return begin() + GetLength(); }

	/**
	 * [����] �}��
	 *	@param ite �}������ʒu�� iterator
	 *	@param t �v�f
	 *	@return �}�������ꏊ�� iterator
	 */
	iterator insert(iterator ite, const TYP& t = TYP())
	{
		INDEX index = ite - begin();
		Insert(index, t);
		return begin() + index;
	}

	/**
	 * [����] �擪�ɑ}��
	 *	@param t �v�f
	 */
	void push_front(TYP t) { Insert(0, t); }

	/**
	 * [����] �Ō�ɒǉ�
	 *	@param t �v�f
	 */
	void push_back(TYP t) { operator+=(t); }


	//-------------------------


	/// �R���X�g���N�^
	CStrT(void)
	{
		m_InitBuf(); 
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param str ���̃C���X�^���X
	 */
	CStrT(const CStrT& str)
	{
		if ( ! str.IsEmpty() )
		{
			::InterlockedIncrement(str.m_GetRefP());
			m_lpBuf = str.m_lpBuf;
		}
		else
		{
			m_InitBuf();
		}
	}

	/**
	 * ����R���X�g���N�^(ASCII/SJIS�p)
	 *	@param lpText ������
	 */
	CStrT(const char* lpText)
	{
		if ( lpText != NULL && *lpText != 0 )
		{
			m_lpBuf = ms_NewBuf(TYP(), lpText);
		}
		else
		{
			m_InitBuf();
		}
	}

	/**
	 * ����R���X�g���N�^(UNICODE�p)
	 *	@param lpText ������
	 */
	CStrT(const WCHAR* lpText)
	{
		if ( lpText != NULL && *lpText != 0 )
		{
			m_lpBuf = ms_NewBuf(TYP(), lpText);
		}
		else
		{
			m_InitBuf();
		}
	}

	/** [�폜] �� */
	void Empty(void)
	{
		if ( m_lpBuf != static_cast<const TYP*>(gpStrNull) ) 
		{
			if ( ::InterlockedDecrement(m_GetRefP()) <= 0 )	
			{
				ms_Delete(m_lpBuf);
			}
			m_InitBuf();
		}
		ASEERT_NULLBASE;
	}

	/** �f�X�g���N�^ */
	~CStrT(void)
	{
		Empty(); 
	}

	/**
	 * [���] �R�s�[�I�y���[�^
	 *	@param other ���̃C���X�^���X
	 *	@return �����̎Q��
	 */
	CStrT& operator=(const CStrT& other)
	{
		if ( ! other.IsEmpty() )
		{
			::InterlockedIncrement(other.m_GetRefP());
			m_SetBuf(other.m_lpBuf);
		}
		else
		{
			Empty();
		}
		return *this;
	}

	/**
	 * [���] ���(ASCII/SJIS�p)
	 *	@param lpText ������
	 *	@return �����̎Q��
	 */
	CStrT& operator=(const char* lpText)
	{
		if ( lpText != NULL && *lpText != 0 )
		{
			m_SetBuf(ms_NewBuf(TYP(), lpText));
		}
		else 
		{
			Empty();
		}
		return *this;
	}

	/**
	 * [���] ���(UNICODE�p)
	 *	@param lpText ������
	 *	@return �����̎Q��
	 */
	CStrT& operator=(const WCHAR* lpText)
	{
		if ( lpText != NULL && *lpText != 0 ) 
		{
			m_SetBuf(ms_NewBuf(TYP(), lpText));
		}
		else 
		{
			Empty();
		}
		return *this;
	}

	/**
	 * [���] �������������.
	 *		�w��̕�����̍�(�擪)����w�蕶��������������܂��B
	 *	@param lpText ������
	 *	@param iLen ������
	 *	@return �����̎Q��
	 */
	CStrT& SetFromLeft(const TYP* lpText, size_t iLen)
	{
		if ( lpText == NULL || STRLIB::GetLen(lpText) <= iLen )
		{
			operator=(lpText);
		}
		else if ( iLen <= 0 )
		{
			Empty();
		}
		else
		{
			TYP* P = ms_NewBuf(TYP(), ms_NullStr(), iLen);
			MemCopy(P, lpText, iLen);
			P[iLen] = 0;
			ms_CalcInfoPointer(P)->lLength = iLen;
			m_SetBuf(P);
		}
		return *this;
	}

	/**
	 * [�ǉ�] �ǉ��I�y���[�^(ASCII/SJIS�p)
	 *	@param lpText ������
	 *	@return �����̎Q��
	 */
	CStrT& operator+=(const char* lpText)
	{
		return m_AddText(lpText);
	}

	/**
	 * [�ǉ�] �ǉ��I�y���[�^(UNICODE�p)
	 *	@param lpText ������
	 *	@return �����̎Q��
	 */
	CStrT& operator+=(const WCHAR* lpText)
	{
		return m_AddText(lpText);
	}

	/**
	 * [�ǉ�] �ǉ��I�y���[�^.
	 *	@note �o�b�t�@�����L���Ă��Ȃ��ꍇ�ŁA�i�[�ł���Ȃ�ă������m�ۂ͍s��Ȃ�
	 *	@param t ����
	 *	@return �����̎Q��
	 */
	CStrT& operator+=(TYP t)
	{
		TYP at[2] = {t, 0};
		return operator+=(at);
	}

	/**
	 * [���] �����t����������.
	 *	@par�g�p��
	 *		\code
	 *		void Log(LPCTSTR lpszFormat, ...)
	 *		{
	 *			CStr s;
	 *			va_list va;
	 *			va_start(va, lpszFormat);
	 *			s.FormatV(lpszFormat, va);
	 *			va_end(va);
	 *			::OutputDebugString(s);
	 *		}
	 *		\endcode
	 *
	 *	@param lpszFormat �����w�蕶����
	 *	@param V VA-LIST
	 */
	void FormatV(const TYP* lpszFormat, va_list V)
	{
		m_SetBuf(ms_Format(lpszFormat, V));
	}

	/**
	 * [���] �����t����������.
	 *	@param lpszFormat �����w�蕶����
	 *	@param ... �ȗ��\�Ȉ���
	 */
	void Format(const TYP* lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);
		FormatV(lpszFormat, args);
		va_end(args);
	}

	/**
	 * [�擾] �Q�ƃI�y���[�^.
	 *	@return �����Ă��镶����̐擪�|�C���^
	 */
	operator const TYP*(void) const
	{
		return m_lpBuf;
	}

	/**
	 * [�擾] �o�b�t�@�Q��.
	 *	@return �����Ă��镶����̐擪�|�C���^
	 */
	const TYP* ReferBuffer(void) const
	{
		return m_lpBuf;
	}

	/**
	 * [����] �ꕶ���u������
	 *	@param index �u�������C���f�b�N�X
	 *	@param t �u�������镶��
	 */
	void SetAt(INDEX index, TYP t)
	{
		if ( ! IsInRange(index, GetLength()) )
		{
			ASSERT1( false, "CStrT::SetAt()", "�͈͊O��Index(%d)���w�肳��܂����B", index );
			return;
		}
		m_Separate();
		m_lpBuf[index] = t;
	}

	/**
	 * [�}��] ������}��
	 *	@param index �}���J�n�C���f�b�N�X
	 *	@param lpText �}��������
	 */
	void Insert(INDEX index, const TYP* lpText)
	{
		if ( GetLength() <= index )
		{
			m_AddText(lpText);
			return;
		}
		size_t iAddSize = STRLIB::GetLen(lpText);
		size_t iNowSize = GetLength();
		if ( *(m_GetRefP()) != 1 || m_GetBufSize() <= iAddSize + iNowSize )
		{
			TYP* P = ms_NewBuf(TYP(), m_lpBuf, iAddSize);	//����Ȃ��̂ŐV���Ɋm��
			m_SetBuf(P);
		}
		// ���炷(NUL�������܂߂�̂ŁALOOP��񑽂����Ă���) 
		for ( INT_PTR i = iNowSize - index; i >= 0; i-- )
		{
			m_lpBuf[index + i + iAddSize] = m_lpBuf[index + i];
		}
		// �A������
		loop ( i, iAddSize )
		{
			m_lpBuf[index + i] = lpText[i];
		}
		m_GetInfoP()->lLength = iAddSize + iNowSize;
	}

	/**
	 * [�}��] �����}��
	 *	@param index �}���J�n�C���f�b�N�X
	 *	@param c ����
	 */
	void InsertAt(INDEX index, TYP c)
	{
		TYP buf[2] = { c, 0 };
		Insert(index, buf);
	}

	/**
	 * [����] �����폜.
	 *	@note ���[�h�o�C�g(�����R�[�h�̈�o�C�g��)���w�肷��ƁA�����g���C���o�C�g���폜����܂��B
	 *	@param index �폜����C���f�b�N�X
	 *	@param iLen �폜���镶����(�����𒴂��Ă���ꍇ�A�w��C���f�b�N�X�ȉ������ׂč폜����܂�)
	 *	@retval 0 �폜���Ă��Ȃ�
	 *	@retval 1�ȏ� �폜�����������BASCII/SJIS�̏ꍇ�AiLen �Ɠ����l�ɂȂ�Ƃ͌���܂���B
	 */
	int Delete(INDEX index, size_t iLen = 1)
	{
		if ( ! IsInRange(index, GetLength()) )
		{
			ASSERT1( false, "CStrT::Delete()", "�͈͊O��Index(%d)���w�肳��܂����B", index );
			return 0;
		}
		if ( iLen == 0 )
		{
			ASSERT1( false, "CStrT::Delete()", "�͈͊O�̒���(%d)���w�肳��܂����B", 0 );
			return 0;
		}
		iLen--;
		if ( index + iLen >= GetLength() )
		{
			*this = Left(index);
			return 0;
		}
		m_Separate();
		size_t iDelSize = STRLIB::GetCharSize(m_lpBuf[index + iLen]) + iLen;
		size_t iSize = GetLength() - iDelSize;
		for ( size_t i = index; i < iSize; i++ ) 
		{
			m_lpBuf[i] = m_lpBuf[i + iDelSize];
		}
		m_lpBuf[iSize] = 0;
		m_GetInfoP()->lLength -= iDelSize;
		ASSERTLIB( STRLIB::GetLen(m_lpBuf) == GetLength() );
		return iDelSize;
	}

	/**
	 * [����] �ŏI�����폜.
	 *	@note �ꕶ�����܂��B
	 */
	void DeleteLast(void)
	{
		INT_PTR len = GetLength();
		if ( len > 0 )
		{
			m_Separate();
			m_lpBuf[len - 1] = 0;
			m_GetInfoP()->lLength -= 1;
			ASSERTLIB( STRLIB::GetLen(m_lpBuf) == GetLength() );
		}
	}

	/**
	 * [�擾] �ꕶ���擾
	 *	@param index �C���f�b�N�X�B1�����ڂ��C���f�b�N�X�O�ɂȂ�܂��B
	 *	@return �C���f�b�N�X�̈ʒu�̕����B
	 */
	TYP GetAt(INDEX index) const
	{
		if ( IsInRange(index, GetLength()) )
		{
			return m_lpBuf[index];
		}
		ASSERT1( false, "CStrT::GetAt()", "�͈͊O��Index(%d)���w�肳��܂����B", index );
		return 0;
	}

	/**
	 * [�擾] ������
	 *	@return ������
	 */
	size_t GetLength(void) const
	{
		return m_GetInfoP()->lLength;
	}

	/**
	 * [�m�F] ��`�F�b�N
	 *	@retval true	�����
	 *	@retval false	�f�[�^�L��
	 */
	bool IsEmpty(void) const
	{
		return m_GetInfoP()->lLength == 0;
	}

	/**
	 * [�m�F] ����.
	 *	@param t �������镶��
	 *	@param iFromIndex �������J�n����C���f�b�N�X�B�ȗ�����Ɠ�����ɂȂ�܂��B
	 *	@retval -1 ������
	 *	@retval 0�ȏ� �����C���f�b�N�X
	 */
	INT_PTR Find(TYP t, INDEX iFromIndex = 0) const
	{
		return STRLIB::IndexOf(m_lpBuf, t, iFromIndex);
	}

	/**
	 * [�m�F] ����.
	 *	@param lpsz �������镶����
	 *	@param iFromIndex �������J�n����C���f�b�N�X�B�ȗ�����Ɠ�����ɂȂ�܂��B
	 *	@retval -1 ������
	 *	@retval 0�ȏ� �����C���f�b�N�X
	 */
	INT_PTR Find(const TYP* lpsz, INDEX iFromIndex = 0) const
	{
		return STRLIB::IndexOf(m_lpBuf, lpsz, iFromIndex);
	}

	/**
	 * [�m�F] ����.
	 *	@param lpsz �������镶���Z�b�g
	 *	@param iFromIndex �������J�n����C���f�b�N�X�B�ȗ�����Ɠ�����ɂȂ�܂��B
	 *	@retval -1 ������
	 *	@retval 0�ȏ� �����C���f�b�N�X
	 */
	INT_PTR FindOneOf(const TYP* lpsz, INDEX iFromIndex = 0) const
	{
		return STRLIB::IndexOneOf(m_lpBuf, lpsz, iFromIndex);
	}

	/**
	 * [�m�F] ����(��납��)
	 *	@param t �������镶��
	 *	@retval -1 ������
	 *	@retval 0�ȏ� �����C���f�b�N�X
	 */
	INT_PTR ReverseFind(TYP t) const
	{
		INT_PTR lRc = -1;
		INT_PTR iPos = 0;
		while ( true )
		{
			INT_PTR r = STRLIB::IndexOf(m_lpBuf, t, iPos);
			if ( r < 0 )
			{
				break;
			}
			lRc = r;
			iPos = r + STRLIB::GetCharSize(m_lpBuf[r]);
		}
		return lRc;
	}

	/**
	 * [�m�F] ����(��납��)
	 *	@param lpsz �������镶����
	 *	@retval -1 ������
	 *	@retval 0�ȏ� �����C���f�b�N�X
	 */
	INT_PTR ReverseFind(const TYP* lpsz) const
	{
		INT_PTR lRc = -1;
		INT_PTR iPos = 0;
		while ( true )
		{
			INT_PTR r = STRLIB::IndexOf(m_lpBuf, lpsz, iPos);
			if ( r < 0 )
			{
				break;
			}
			lRc = r;
			iPos = r + STRLIB::GetCharSize(m_lpBuf[r]);
		}
		return lRc;
	}

	/**
	 * [�m�F] ����(��납��)
	 *	@param lpsz �������镶���Z�b�g
	 *	@retval -1 ������
	 *	@retval 0�ȏ� �����C���f�b�N�X
	 */
	INT_PTR ReverseFindOneOf(const TYP* lpsz) const
	{
		INT_PTR lRc = -1;
		INT_PTR iPos = 0;
		while ( true )
		{
			INT_PTR r = STRLIB::IndexOneOf(m_lpBuf, lpsz, iPos);
			if ( r < 0 )
			{
				break;
			}
			lRc = r;
			iPos = r + STRLIB::GetCharSize(m_lpBuf[r]);
		}
		return lRc;
	}

	/**
	 * [�m�F] �������r.
	 *	@note �����񓯎m�̈����Z������ƁA��r���ʂ��Ԃ�܂��B
	 *	@param lpszSubject ��r�Ώƕ�����
	 *	@retval �O		������ lpszSubject�Ɠ���
	 *	@retval �P�ȏ�	������ lpszSubject ���傫��
	 *	@retval �O����	������ lpszSubject ��菬����
	 */
	int operator-(const TYP* lpszSubject) const
	{
		return Compare(lpszSubject);
	}

	/**
	 * [�m�F] �������r
	 *	@param lpszSubject ��r�Ώƕ�����
	 *	@retval �O		������ lpszSubject�Ɠ���
	 *	@retval �P�ȏ�	������ lpszSubject ���傫��
	 *	@retval �O����	������ lpszSubject ��菬����
	 */
	int Compare(const TYP* lpszSubject) const
	{
		return STRLIB::Compare(m_lpBuf, lpszSubject);
	}

	/**
	 * [�m�F] �������r
	 *	@param lpszSubject ��r�Ώƕ�����
	 *	@retval true	����
	 *	@retval false	�قȂ�
	 */
	bool IsEqual(const TYP* lpszSubject) const { return Compare(lpszSubject) == 0; }

	/**
	 * [�m�F] �������r(�召��ʖ�����r)
	 *	@param lpszSubject ��r�Ώƕ�����
	 *	@retval �O		������ lpszSubject�Ɠ���
	 *	@retval �P�ȏ�	������ lpszSubject ���傫��
	 *	@retval �O����	������ lpszSubject ��菬����
	 */
	int CompareNoCase(const TYP* lpszSubject) const
	{
		return STRLIB::Compare(m_lpBuf, lpszSubject, -1, NORM_IGNORECASE);
	}

	/**
	 * [�m�F] �������r(��r�I�v�V�����w��)
	 *	@param lpszSubject ��r�Ώƕ�����
	 *	@param dwCmpFlags ��r�I�v�V����(�ȉ��̒l�̘_���a) <BR>
	 *			NORM_IGNORECASE	�啶��/����������ʂ��Ȃ�					<BR>
	 *			NORM_IGNOREKANATYPE	�Ђ炪��/�J�^�J�i����ʂ��Ȃ�			<BR>
	 *			NORM_IGNORENONSPACE	�ꏊ�����Ȃ���������ʂ��Ȃ�			<BR>
	 *			NORM_IGNORESYMBOLS	�L���𖳎�����							<BR>
	 *			NORM_IGNOREWIDTH	1�o�C�g�����Ƃ���Ɠ���2�o�C�g��������ʂ��Ȃ�<BR>
	 *			SORT_STRINGSORT	��Ǔ_���L���Ƃ��Ĉ���						<BR>
	 *	@retval �O		������ lpszSubject�Ɠ���
	 *	@retval �P�ȏ�	������ lpszSubject ���傫��
	 *	@retval �O����	������ lpszSubject ��菬����
	 */
	int CompareByOption(const TYP* lpszSubject, DWORD dwCmpFlags) const
	{
		return STRLIB::Compare(m_lpBuf, lpszSubject, -1, dwCmpFlags);
	}

	/**
	 * [�m�F] �������r(�召��ʖ�����r)
	 *	@param lpszSubject ��r�Ώƕ�����
	 *	@retval true	����
	 *	@retval false	�قȂ�
	 */
	bool IsEqualNoCase(const TYP* lpszSubject) const { return CompareNoCase(lpszSubject) == 0; }

	/**
	 * [�m�F] �������r
	 *	@param lpszSubject ��r�Ώƕ�����
	 *	@retval true	����
	 *	@retval false	�قȂ�
	 */
	bool operator==(const TYP* lpszSubject) const { return Compare(lpszSubject) == 0; }

	/**
	 * [�m�F] �������r
	 *	@param lpszSubject ��r�Ώƕ�����
	 *	@retval true	�قȂ�
	 *	@retval false	����
	 */
	bool operator!=(const TYP* lpszSubject) const { return Compare(lpszSubject) != 0; }

	/**
	 * [�m�F] �������r
	 *	@param lpszSubject ��r�Ώƕ�����
	 *	@retval true	������ lpszSubject ���傫��
	 *	@retval false	�U
	 */
	bool operator>(const TYP* lpszSubject) const { return Compare(lpszSubject) > 0; }

	/**
	 * [�m�F] �������r
	 *	@param lpszSubject ��r�Ώƕ�����
	 *	@retval true	������ lpszSubject �Ɠ������傫��
	 *	@retval false	�U
	 */
	bool operator>=(const TYP* lpszSubject) const { return Compare(lpszSubject) >= 0; }

	/**
	 * [�m�F] �������r
	 *	@param lpszSubject ��r�Ώƕ�����
	 *	@retval true	������ lpszSubject ��菬����
	 *	@retval false	�U
	 */
	bool operator<(const TYP* lpszSubject) const { return Compare(lpszSubject) < 0; }

	/**
	 * [�m�F] �������r
	 *	@param lpszSubject ��r�Ώƕ�����
	 *	@retval true	������ lpszSubject �Ɠ�����������
	 *	@retval false	�U
	 */
	bool operator<=(const TYP* lpszSubject) const { return Compare(lpszSubject) <= 0; }

	/**
	 * [�쐬] �͈͎擾.
	 *	@note	�QByte�R�[�h���ӎ����A�r���ŕ��f����Ȃ��悤�ɂ��܂��B
	 *			���ׁ̈AiSize �Ŏw�肵���T�C�Y��蒷�������񂪓�����\��������܂��B
	 *	@param iOffset ���o���J�nOffset
	 *	@param iSize ���o���������B�ȗ�����ƍŌ�܂Ŏ��o���܂��B
	 *	@return ������
	 */
	CStrT Mid(INDEX iOffset, size_t iSize = INVALID_SIZE) const
	{
		CStrT str;
		size_t l = GetLength();
		if ( l > iOffset && iSize != 0 )
		{
			if ( iSize == INVALID_SIZE || iOffset + iSize > l )
			{
				iSize = l - iOffset;
			}
			// = �T�C�Y�̕␳
			if ( sizeof(TYP) == 1 )
			{
				size_t iPos = 0;
				while ( iPos < iSize )
				{
					iPos += STRLIB::GetCharSize(m_lpBuf[iOffset + iPos]);
				}
				iSize = iPos;
			}			
			TYP* P = ms_NewBuf(TYP(), ms_NullStr(), iSize);
			MemCopy(P, &m_lpBuf[iOffset], iSize);
			P[iSize] = 0;
			ms_CalcInfoPointer(P)->lLength = iSize;
			str.m_SetBuf(P);
			ASSERTLIB( STRLIB::GetLen(str.m_lpBuf) == str.GetLength() );
		}
		return str;
	}

	/**
	 * [�쐬] �͈͎擾.
	 *	@param iSize �擪������o���������B
	 *	@return ������
	 */
	CStrT Left(size_t iSize) const
	{
		return Mid(0, iSize);
	}

	/**
	 * [�쐬] �͈͎擾.
	 *	@param iSize ����������o���������B
	 *	@return ������
	 */
	CStrT Right(INT_PTR iSize) const
	{
		size_t l = GetLength();
		if ( static_cast<size_t>(iSize) > l )
		{
			return *this;
		}
		return Mid(l - iSize);
	}

	/**
	 * [�쐬] �O��A��.
	 *	@param lpszBefore �O�ɂ��镶����
	 *	@param lpszAfter ��ɂ��镶����
	 *	@return ������
	 */
	CStrT Sandwich(const TYP* lpszBefore, const TYP* lpszAfter) const
	{
		CStrT str = lpszBefore;
		str += m_lpBuf;
		str += lpszAfter;
		return str;
	}

	/**
	 * [�擾] ���l(int)�֕ϊ�
	 *	@param iOffset ���o���J�nOffset
	 *	@param iBase �x�[�X�ɂȂ�l�B���� 10 �� 16 ���w��B 0���w�肷��ƁA 
	 *					lpsz �̕\�L�ɏ]���i�擪��0x,0X�Ȃ�16�i��,0�Ȃ�8�i��,����ȊO��10�i���j�B
	 *	@return �ϊ������l
	 */
	int ToInt(INDEX iOffset = 0, int iBase = 10) const
	{
		int i = 0;
		if ( IsInRange(iOffset, GetLength()) )
		{
			i = STRLIB::ToInt(&m_lpBuf[iOffset], iBase);
		}
		return i;
	}

	/**
	 * [�擾] ���l(DWORD)�֕ϊ�
	 *	@param iOffset ���o���J�nOffset
	 *	@param iBase �x�[�X�ɂȂ�l�B���� 10 �� 16 ���w��B 0���w�肷��ƁA 
	 *					lpsz �̕\�L�ɏ]���i�擪��0x,0X�Ȃ�16�i��,0�Ȃ�8�i��,����ȊO��10�i���j�B
	 *	@return �ϊ������l
	 */
	DWORD ToDword(INDEX iOffset = 0, int iBase = 10) const
	{
		int i = 0;
		if ( IsInRange(iOffset, GetLength()) )
		{
			i = STRLIB::ToDword(&m_lpBuf[iOffset], iBase);
		}
		return i;
	}

	/**
	 * [�擾] ���l(double)�֕ϊ�
	 *	@param iOffset ���o���J�nOffset
	 *	@return �ϊ������l
	 */
	double ToDouble(INDEX iOffset = 0) const
	{
		double a = 0;
		if ( IsInRange(iOffset, GetLength()) )
		{
			a = STRLIB::ToDouble(&m_lpBuf[iOffset]);
		}
		return a;
	}

	/**
	 * [�擾] ���l(LONGLONG)�֕ϊ�
	 *	@param iOffset ���o���J�nOffset
	 *	@return �ϊ������l
	 */
	LONGLONG ToLonglong(INDEX iOffset = 0) const
	{
		LONGLONG ll = 0;
		if ( IsInRange(iOffset, GetLength()) )
		{
			ll = STRLIB::ToLonglong(&m_lpBuf[iOffset]);
		}
		return ll;
	}

	/**
	 * [����] �������݃o�b�t�@�v��.
	 *		�K�v�T�C�Y�̏�������ł��悢�o�b�t�@��Ԃ��܂��B
	 *	@note	�g�p��A�K�� ReleaseBuffer() ���R�[�����Ă��������B
	 *	@par�g�p��
	 *	<table><tr><td><pre>
	 *
	 *	  CStr str;
	 *	  DWORD dwRc = GetModuleFileName(NULL, str.GetBuffer(MAX_PATH), MAX_PATH);		
	 *	  str.ReleaseBuffer();
	 *	  int p1 = str.ReverseFind('\\');
	 *	</pre></td></tr></table>
	 *	@param iLength �K�v�Ȓ����B�ȗ��Ȃ猻�݂̒���
	 *	@return �������݉\�A�h���X
	 */
	TYP* GetBuffer(size_t iLength = 0)
	{
		size_t lNowBufSize = m_GetBufSize();
		if ( lNowBufSize == 0 )
		{
			//���A�����
			if ( iLength > 0 )
			{
				m_SetBuf(ms_New(TYP(), iLength + 1));
				*m_lpBuf = 0;
			}
			else
			{
				return NULL;
			}
		}
		else if ( lNowBufSize < iLength )
		{
			//�T�C�Y�g��
			TYP* P = ms_New(TYP(), iLength); 
			MemCopy(P, m_lpBuf, lNowBufSize);
			m_SetBuf(P);
		}
		else if ( *(m_GetRefP()) > 1 )
		{
			//���L���Ă���,��������K�v�L��
			TYP* P = ms_New(TYP(), lNowBufSize); 
			MemCopy(P, m_lpBuf, lNowBufSize);
			m_SetBuf(P);
		}
		m_GetInfoP()->lLength = 0;
		return m_lpBuf;
	}

	/**
	 * [����] ���蓖�Ă��o�b�t�@���J��.
	 *		GetBuffer() �Ŋ��蓖�Ă��o�b�t�@���J�����A�������{�N���X�Ɏ�荞�݂܂��B
	 *	@note GetBuffer()�Ŏ擾�����̈�ɏ������݂��I�������K�� ReleaseBuffer() ���Ă��������B
	 *	@see GetBuffer()
	 */
	void ReleaseBuffer(void)
	{
		if ( *(m_GetRefP()) == 1 )
		{
			m_GetInfoP()->lLength = STRLIB::GetLen(m_lpBuf);
		}
	}

	/**
	 * [����] �擪���當�����g����.
	 *	@note �ێ�������ɑ΂��������܂��B
	 *	@param t ��菜�������B�ȗ�����󔒂ɂȂ�܂��B
	 *	@return �����̎Q��
	 */
	CStrT& TrimLeft(TYP t = ' ')
	{
		return m_TrimLeft(t);
	}

	/**
	 * [����] �擪���當�����g����.
	 *	@note �ێ�������ɑ΂��������܂��B
	 *	@param lpsz ��菜�������Z�b�g�B
	 *	@return �����̎Q��
	 */
	CStrT& TrimLeft(const TYP* lpsz)
	{
		return m_TrimLeft(lpsz);
	}

	/**
	 * [����] �������當�����g����.
	 *	@note �ێ�������ɑ΂��������܂��B
	 *	@param t ��菜�������B�ȗ�����Ƌ󔒂ɂȂ�܂��B
	 *	@return �����̎Q��
	 */
	CStrT& TrimRight(TYP t = ' ')
	{
		return m_TrimRight(t);
	}

	/**
	 * [����] �������當�����g����.
	 *	@note �ێ�������ɑ΂��������܂��B
	 *	@param lpsz ��菜�������Z�b�g�B
	 *	@return �����̎Q��
	 */
	CStrT& TrimRight(const TYP* lpsz)
	{
		return m_TrimRight(lpsz);
	}

	/**
	 * [����] ���[���當�����g����.
	 *	@note �ێ�������ɑ΂��������܂��B
	 *	@param t ��菜�������B�ȗ�����Ƌ󔒂ɂȂ�܂��B
	 *	@return �����̎Q��
	 */
	CStrT& Trim(TYP t = ' ')
	{
		m_TrimLeft(t);
		return m_TrimRight(t);
	}

	/**
	 * [����] ���[���當�����g����.
	 *	@note �ێ�������ɑ΂��������܂��B
	 *	@param lpsz ��菜�������Z�b�g�B
	 *	@return �����̎Q��
	 */
	CStrT& Trim(const TYP* lpsz)
	{
		m_TrimLeft(lpsz);
		return m_TrimRight(lpsz);
	}

	/**
	 * [����] �����u��.
	 *	@note �ێ�������ɑ΂��������܂��B
	 *	@note �������Ȃ����Ƃ͂ł��܂���B�u�����邾���ł��B
	 *	@param tOld �u���������镶��
	 *	@param tNew �u�������镶��
	 *	@return �u��������
	 */
	int Replace(TYP tOld, TYP tNew)
	{
		return m_Replace(tOld, tNew);
	}

	/**
	 * [����] �����u��.
	 *	@note �ێ�������ɑ΂��������܂��B
	 *	@note �������Ȃ����Ƃ͂ł��܂���B�u�����邾���ł��B
	 *	@param lpszOld �u���������镶���Z�b�g
	 *	@param tNew �u�������镶��
	 *	@return �u��������
	 */
	int Replace(const TYP* lpszOld, TYP tNew)
	{
		return m_Replace(lpszOld, tNew);
	}

	/**
	 * [����] �����u��.
	 *	@note �ێ�������ɑ΂��������܂��B
	 *	@param lpszOld �u���������镶����
	 *	@param lpszNew �u�������镶����
	 *	@return �u��������
	 */
	int Replace(const TYP* lpszOld, const TYP* lpszNew)
	{
		int iRc = 0;
		if ( lpszOld != NULL) 
		{
			size_t iOldLen = STRLIB::GetLen(lpszOld);
			size_t iNewLen = (lpszNew == NULL) ? 0 : STRLIB::GetLen(lpszNew);
			size_t iLength = GetLength();
			//�������邩���ׂ�
			int iCount = 0;
			INT_PTR index = 0;
			INT_PTR r;
			while ( (r = STRLIB::IndexOf(m_lpBuf, lpszOld, index)) >= 0 )
			{
				iCount++;
				index = r + iOldLen;
			}
			iLength += (iNewLen - iOldLen) * iCount;
			//�m�ۂ��ď����R�s�[����
			TYP* P = ms_NewBuf(TYP(), ms_NullStr(), iLength);
			TYP* pNew = P;
			TYP* pOld = m_lpBuf;
			while ( (r = STRLIB::IndexOf(pOld, lpszOld, 0)) >= 0 )
			{
				MemCopy(pNew, pOld, r);			//�L�[�����܂ŃR�s�[
				pNew += r;
				MemCopy(pNew, lpszNew, iNewLen);	//�V�����������R�s�[
				pNew += iNewLen;
				pOld += iOldLen+r;
				iRc++;
			}
			MemCopy(pNew, pOld, STRLIB::GetLen(pOld) + 1);
			m_SetBuf(P);
			m_GetInfoP()->lLength = iLength;
			ASSERTLIB( STRLIB::GetLen(m_lpBuf) == GetLength() );
		}
		return iRc;
	}

	/**
	 * [����] �����폜.
	 *	@note �ێ�������ɑ΂��������܂��B
	 *	@param t �폜���镶��
	 *	@return �폜������
	 */
	int Remove(TYP t)
	{
		TYP aBuf[2] = {t, 0};
		const TYP* P = NULL;
		return Replace(aBuf, P);
	}

	/**
	 * [�쐬] �؂蕪��
	 *	@note �w��Z�p���[�^�����̑O��̕�������쐬���܂��B�Ȃ��ꍇ�A�����񂪂��̂܂܃R�s�[����܂��B
	 *	@param[in] c �Z�p���[�^����
	 *	@param[out]	_pstrRest �Z�p���[�^�����̌�̕����񂪊i�[����܂��B
	 *	@return �Z�p���[�^�����O�܂ł̕�����
	 */
	CStrT FindCut(TYP c, CStrT* _pstrRest = NULL) const
	{
		INT_PTR r = STRLIB::IndexOf(m_lpBuf, c, 0);
		if ( r >= 0 && _pstrRest != NULL )
		{
			*_pstrRest = Mid(r + 1);
		}
		return Mid(0, r);
	}

	/**
	 * [�ϊ�] �啶����
	 */
	void MakeUpper(void)
	{
		ms_strupr(m_lpBuf);
	}

	/**
	 * [�ϊ�] ��������
	 */
	void MakeLower(void)
	{
		ms_strlwr(m_lpBuf);
	}


	//------------------------------------


	/**
	 * [�ݒ�] int�^������.
	 *		�w��� int �^�̐��l�𕶎���ɕϊ����܂��B
	 *	@param value int�^���l.
	 *	@return ����������������
	 */
	static CStrT IntToString(int value)
	{
		CStrT s;
		LPTSTR p = s.GetBuffer(32);
		INT_PTR i = CTinyStringFormaterT<TYP>::FormatInt(p, value);
		p[i] = 0;
		s.ReleaseBuffer();
		return s;
	}

	/**
	 * [�ݒ�] DWORD�^������.
	 *		�w��� DWORD �^�̐��l�𕶎���ɕϊ����܂��B
	 *	@param value DWORD�^���l.
	 *	@return ����������������
	 */
	static CStrT DwordToString(DWORD value)
	{
		CStrT s;
		LPTSTR p = s.GetBuffer(32);
		INT_PTR i = CTinyStringFormaterT<TYP>::FormatDword(p, value);
		p[i] = 0;
		s.ReleaseBuffer();
		return s;
	}

	/**
	 * [�ݒ�] DWORD�^ HEX ������(�O�p�f�B���O�t��).
	 *		�w��� int �^�̐��l���w��̕������̕�����ɕϊ����܂��B
	 *	@param value DWORD �^���l.
	 *	@param width �ϊ���̕�����
	 *	@return ����������������
	 */
	static CStrT ToHexString(DWORD value, size_t width)
	{
		CStrT s;
		LPTSTR p = s.GetBuffer(width);
		 CTinyStringFormaterT<TYP>::FormatHex(p, value, width);
		s.ReleaseBuffer();
		return s;
	}

	/**
	 * [�쐬] �����t��������쐬
	 *	@param lpszFormat �����w�蕶����
	 *	@param ... �ȗ��\�Ȉ���
	 *	@return ������
	 */
	static CStrT Fmt(const TYP* lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);
		CStrT str;
		str.FormatV(lpszFormat, args);
		va_end(args);
		return str;
	}

	/**
	 * [�쐬] �w�蕶������ׂ�������쐬
	 *	@param t ����
	 *	@param length ������
	 *	@return ������
	 */
	static CStrT Lineup(TYP t, size_t length)
	{
		TYP* P = ms_NewBuf(TYP(), ms_NullStr(), length);
		loop ( i, length )
		{
			P[i] = t;
		}
		P[length] = 0;
		ms_CalcInfoPointer(P)->lLength = length;
		CStrT str;
		str.m_SetBuf(P);
		ASSERTLIB( STRLIB::GetLen(str.m_lpBuf) == str.GetLength() );
		return str;
	}

	/**
	 * [�쐬] �w�蕶������ׂ�������쐬
	 *	@param lpsz ������
	 *	@param iSize ������
	 *	@return ������
	 */
	static CStrT Lineup(const TYP* lpsz, int iSize)
	{
		size_t l = STRLIB::GetLen(lpsz);
		TYP* P = ms_NewBuf(TYP(), ms_NullStr(), iSize * l);
		TYP* Q = P;
		loop ( i, iSize )
		{
			loop ( j, l )
			{
				*Q++ = lpsz[j];
			}
		}
		*Q = 0;
		ms_CalcInfoPointer(P)->lLength = iSize * l;
		CStrT str;
		str.m_SetBuf(P);
		ASSERTLIB( STRLIB::GetLen(str.m_lpBuf) == str.GetLength() );
		return str;
	}

	/**
	 * [���] �C�e���[�^���.
	 *		�C�e���[�^�w��ő�����܂��B�ԕ��C�e���[�^�� ���O�� max������������������ɂ��܂��B
	 *	@param is �J�n�C�e���[�^�B
	 *	@param ie �ԕ��C�e���[�^�B
	 *	@param max �ő啶����
	 *	@return ������
	 */
	template<typename ITE>
	static CStrT FromIterator(ITE is, ITE ie = ITE(), size_t max = INVALID_SIZE)
	{
		CStrT s;
		while ( is != ie && *is != 0 && max-- != 0)
		{
			s += *is++;
		}
		return s;
	}

	/**
	 * [���] WindowText���.
	 *		�w���Window�̃e�L�X�g���������܂��B
	 *	@param hWnd �E�B���h�E�n���h��
	 *	@return ������
	 */
	static CStrT FromWindowText(HWND hWnd)
	{
		CStrT s;
		int iLen = ::GetWindowTextLength(hWnd);
		if ( iLen != 0 )
		{
			CWorkMemT<TCHAR> m(iLen + 1);
			::GetWindowText(hWnd, m, iLen + 1);
			s.m_SetBuf(ms_NewBuf(TYP(), m));
		}
		return s;
	}

	#ifdef __AFX_H__

	/**
	 * [���] WindowText���.
	 *		�w���Window�̃e�L�X�g���������܂��B
	 *	@param pcWnd �E�B���h�E
	 *	@return ������
	 */
	static CStrT FromWindowText(CWnd* pcWnd)
	{
		return FromWindowText(pcWnd->GetSafeHwnd());
	}

	#endif

private:

	/**
	 * �}�[�N����.
	 *	
	 *	@par�K�v�t�@�C��
	 *			TnbStrOperator.h
	 * 
	 *	@date 10/08/02	�V�K�쐬
	 */
	template<typename TYP>
	class CGroupT
	{
	public:
		/**
		 * �R���X�g���N�^.
		 *	@param c ����
		 */
		CGroupT(TYP c) : m_char(c), m_lpsz(NULL)
		{
		}
		/**
		 * �R���X�g���N�^.
		 *	@param lpsz �����Q
		 */
		CGroupT(const TYP* lpsz) : m_char(0), m_lpsz(lpsz)
		{
		}
		bool IsEqual(TYP c) const
		{
			if ( m_char != 0 )
			{
				return c == m_char;
			}
			return STRLIB::IndexOf(m_lpsz, c) >= 0;
		}
	private:
		TYP					m_char;
		const TYP*			m_lpsz;
	};

	/**
	 * [����] �擪���當�����g����.
	 *	@note �ێ�������ɑ΂��������܂��B
	 *	@param t ��菜�������B�ȗ�����󔒂ɂȂ�܂��B
	 *	@return �����̎Q��
	 */
	CStrT& m_TrimLeft(CGroupT<TYP> t)
	{
		m_Separate();
		size_t l = GetLength();
		loop ( i, l ) 
		{
			if ( ! t.IsEqual(m_lpBuf[i]) ) 
			{
				m_GetInfoP()->lLength -= i;
				for ( size_t j = 0; i < l + 1; i++, j++ ) 
				{
					m_lpBuf[j] = m_lpBuf[i];
				}
				ASSERTLIB( STRLIB::GetLen(m_lpBuf) == GetLength() );
				return *this;
			}
		}
		Empty();
		return *this;
	}

	/**
	 * [����] �������當�����g����.
	 *	@note �ێ�������ɑ΂��������܂��B
	 *	@param t ��菜�������B�ȗ�����Ƌ󔒂ɂȂ�܂��B
	 *	@return �����̎Q��
	 */
	CStrT& m_TrimRight(CGroupT<TYP> t)
	{
		m_Separate();
		size_t l = GetLength();
		for ( INT_PTR i = l - 1; i >= 0; i-- ) 
		{
			if ( ! t.IsEqual(m_lpBuf[i]) ) 
			{
				m_GetInfoP()->lLength = static_cast<size_t>(i + 1);
				m_lpBuf[i + 1] = 0;
				ASSERTLIB( STRLIB::GetLen(m_lpBuf) == GetLength() );
				return *this;
			}
		}
		Empty();
		return *this;
	}

	/**
	 * [����] �����u��.
	 *	@note �ێ�������ɑ΂��������܂��B
	 *	@note �������Ȃ����Ƃ͂ł��܂���B�u�����邾���ł��B
	 *	@param tOld �u���������镶��
	 *	@param tNew �u�������镶��
	 *	@return �u��������
	 */
	int m_Replace(CGroupT<TYP> tOld, TYP tNew)
	{
		int iRc = 0;
		if ( tNew != 0 ) 
		{
			m_Separate();
			size_t l = GetLength();
			for ( size_t i = 0; i < l; i += STRLIB::GetCharSize(m_lpBuf[i]) ) 
			{
				if ( tOld.IsEqual(m_lpBuf[i]) )
				{
					m_lpBuf[i] = tNew;
					iRc++;
				}
			}
		}
		return iRc;
	}
	
	/**
	 * �Ǘ��\���̌^.
	 *
	 *	@note ������̃|�C���^�̑O�ɊǗ��̈�iTInfo�j�����݂���悤�ɂȂ��Ă��܂��B 
	 */
	struct TInfo
	{
		LONG	lRef;			///< �Q�Ɛ�
		size_t	lLength;		///< �L��������(NULL����������)
		size_t	lBufSize;		///< �i�[�\������(NULL�������܂�)
	};

	/**
	 * �Ǘ����̃|�C���^�ꏊ�v�Z
	 *	@param lpsz ������̃|�C���^
	 *	@return �|�C���^
	 */
	static TInfo* ms_CalcInfoPointer(LPVOID lpsz)
	{
		BYTE* B = static_cast<BYTE*>(lpsz);
		return reinterpret_cast<TInfo*>(B - sizeof(TInfo));
	}

	/**
	 * �������m��(ASCII/SJIS�p)
	 *	@param d �_�~�[
	 *	@param iLen	�m�ۂ���������
	 *	@return �m�ۂ����|�C���^�i�J����ms_Delete���g�p���邱�Ɓj
	 */
	static char* ms_New(char d, size_t iLen)
	{
		BYTE* B = new BYTE[iLen + sizeof(TInfo)];
		TInfo* I = reinterpret_cast<TInfo*>(B);
		I->lRef = 1;
		I->lLength = 0;
		I->lBufSize = iLen;
		return reinterpret_cast<char*>(B + sizeof(TInfo));
	}

	/**
	 * �������m��(UNICODE�p)
	 *	@param d �_�~�[
	 *	@param iLen	�m�ۂ���������
	 *	@return �m�ۂ����|�C���^�i�J����ms_Delete���g�p���邱�Ɓj
	 */
	static WCHAR* ms_New(WCHAR d, size_t iLen)
	{
		BYTE* B = new BYTE[iLen * sizeof(WCHAR) + sizeof(TInfo)];
		TInfo* I = reinterpret_cast<TInfo*>(B);
		I->lRef = 1;
		I->lLength = 0;
		I->lBufSize = iLen;
		return reinterpret_cast<WCHAR*>(B + sizeof(TInfo));
	}
	
	/**
	 * �������J��
	 *	@param pMem	ms_New�Ŋm�ۂ����|�C���^
	 */
	static void ms_Delete(LPVOID pMem)
	{
		ASSERTLIB( pMem != gpStrNull );
		BYTE* B = static_cast<BYTE*>(pMem);
		delete[] (B - sizeof(TInfo));
	}

	/** 
	 * �����t��������쐬
	 *	@param lpsz �����w�蕶����
	 *	@param V �ȗ��\�Ȉ���
	 *	@return �m�ۂ��ĕ�����̓������������i�J����ms_Delete���g�p���邱�Ɓj
	 */
	template<typename T>
	static T* ms_Format(const T* lpsz, va_list V)
	{
		size_t iLen = 1024 + STRLIB::GetLen(lpsz);
		while ( true )
		{
			CWorkMemT<T> work(iLen); 
			if ( STRLIB::VPrintF(work, iLen, lpsz, V) )
			{
				size_t r = STRLIB::GetLen(work);
				T* X = ms_New(T(), r + 1);
				const T* W = work;
				MemCopy(X, W, r + 1);
				ms_CalcInfoPointer(X)->lLength = r;
				return X;
			}
			iLen *= 2;
		}
	}

	/**
	 * �o�b�t�@�T�C�Y
	 *	@param iSize �K�{�T�C�Y
	 *	@param iExSize �g���T�C�Y�B�}�C�i�X�Ȃ�K�{�T�C�Y�{�P���Ԃ�B0�ȏ�Ȃ�A
	 *					�K�{�T�C�Y�{�g���T�C�Y�{�]���{�P���Ԃ�B
	 *	@return �m�ۂ��ׂ��o�b�t�@�T�C�Y
	 */
	static size_t ms_GetSecureSize(size_t iSize, size_t iExSize = INVALID_SIZE)
	{
		if ( iExSize != INVALID_SIZE )
		{
			iSize += iExSize;
			size_t l = iSize / 8;
			l = (l < 4) ? 4 : ((l > 1024) ? 1024 : l);
			iSize += l;
		}
		return iSize + 1;
	}

	/** 
	 * ���(ASCII/SJIS to ASCII/SJIS)
	 *	@param d 0�Ȃ烁�����m�ہA1�Ȃ珈������NULL��Ԃ��B
	 *	@param lpsz ��������
	 *	@param iExSize �g���T�C�Y
	 *	@retval NULL�ȊO �m�ۂ��ĕ�����̓������������i�J����ms_Delete���g�p���邱�Ɓj
	 *	@retval NULL	d��1���������B
	 */
	static LPSTR ms_NewBuf(char d, LPCSTR lpsz, size_t iExSize = INVALID_SIZE)
	{
		if ( d == 1 )
		{
			return NULL; 
		}
		size_t iSize = STRLIB::GetLen(lpsz);
		LPSTR P = ms_New(char(), ms_GetSecureSize(iSize, iExSize)); 
		MemCopy(P, lpsz, iSize + 1);
		ms_CalcInfoPointer(P)->lLength = iSize;
		return P;
	}

	/** 
	 * ���(UNICODE to ASCII/SJIS)
	 *	@param d �_�~�[
	 *	@param lpsz ��������
	 *	@param iExSize �g���T�C�Y
	 *	@return �m�ۂ��ĕ�����̓������������i�J����ms_Delete���g�p���邱�Ɓj
	 */
	static LPSTR ms_NewBuf(char d, LPCWSTR lpsz, size_t iExSize = INVALID_SIZE)
	{
		int size = ::WideCharToMultiByte(CP_ACP, 0, lpsz, -1, NULL, 0, NULL, NULL);
		char* P = ms_New(char(), ms_GetSecureSize(size, iExSize)); 
		::WideCharToMultiByte(CP_ACP, 0, lpsz, TNB::ToInt(STRLIB::GetLen(lpsz) + 1), P, size, NULL, NULL);
		ms_CalcInfoPointer(P)->lLength = size - 1;
		return P; 
	}
	
	/** 
	 * ���(ASCII/SJIS to UNICODE)
	 *	@param d �_�~�[
	 *	@param lpsz ��������
	 *	@param iExSize �g���T�C�Y
	 *	@return �m�ۂ��ĕ�����̓������������i�J����ms_Delete���g�p���邱�Ɓj
	 */
	static LPWSTR ms_NewBuf(WCHAR d, LPCSTR lpsz, size_t iExSize = INVALID_SIZE)
	{
		int size = ::MultiByteToWideChar(CP_ACP, 0, lpsz, -1, NULL, 0); 
		WCHAR* P = ms_New(WCHAR(), ms_GetSecureSize(size, iExSize));
		::MultiByteToWideChar(CP_ACP, 0, lpsz, TNB::ToInt(STRLIB::GetLen(lpsz) + 1), P, size); 
		ms_CalcInfoPointer(P)->lLength = size - 1;
		return P; 
	}
	
	/** 
	 * ���(UNICODE to UNICODE)
	 *	@param d 0�Ȃ烁�����m�ہA1�Ȃ珈������NULL��Ԃ��B
	 *	@param lpsz ��������
	 *	@param iExSize �g���T�C�Y
	 *	@retval NULL�ȊO �m�ۂ��ĕ�����̓������������i�J����ms_Delete���g�p���邱�Ɓj
	 *	@retval NULL	d��1���������B
	 */
	static LPWSTR ms_NewBuf(WCHAR d, LPCWSTR lpsz, size_t iExSize = INVALID_SIZE)
	{
		if ( d == 1 )
		{
			return NULL;
		}
		size_t iSize = STRLIB::GetLen(lpsz);
		LPWSTR P = ms_New(WCHAR(), ms_GetSecureSize(iSize, iExSize)); 
		MemCopy(P, lpsz, iSize + 1);
		ms_CalcInfoPointer(P)->lLength = iSize;
		return P;
	}

	/**
	 * NULL������
	 *	@return �T�C�Y0�̕�����B
	 */
	static const TYP* ms_NullStr(void) 
	{
		return static_cast<const TYP*>(gpStrNull);
	}

	// ��������
	static void ms_strlwr(LPSTR _p)
	{
		strlwr(_p);
	}

	// ��������
	static void ms_strlwr(LPWSTR _p)
	{
		wcslwr(_p);
	}
	
	// �啶����
	static void ms_strupr(LPSTR _p)
	{
		strupr(_p);
	}

	// �啶����
	static void ms_strupr(LPWSTR _p)
	{
		wcsupr(_p);
	}


	//--------------------------
	

	/** ������ */
	void m_InitBuf(void)
	{
		const TYP* P = static_cast<const TYP*>(gpStrNull); 
		m_lpBuf = const_cast<TYP*>(P); 
	}

	/**
	 * �Ǘ����̃|�C���^�擾
	 *	@return �|�C���^
	 */
	TInfo* m_GetInfoP(void) const
	{
		return ms_CalcInfoPointer(m_lpBuf);
	}

	/**
	 * �Q�Ɛ��̃|�C���^�擾
	 *	@return �Q�Ɛ��̃|�C���^
	 */
	LONG* m_GetRefP(void) const
	{
		return &(m_GetInfoP()->lRef);
	}

	/**
	 * �o�b�t�@�T�C�Y�擾
	 *	@return �T�C�Y�̎Q��
	 */
	size_t m_GetBufSize(void) const
	{
		return m_GetInfoP()->lBufSize;
	}
	
	/** 
	 * �o�b�t�@��ݒ�
	 *	@note ��񂪂����Empty���Ă���������B
	 *	@param P �o�b�t�@
	 */
	void m_SetBuf(TYP* P)
	{
		Empty();
		m_lpBuf = P;
	}
	
	/** ���ƃo�b�t�@�����L���Ă����番������ */
	void m_Separate(void)
	{
		if ( *(m_GetRefP()) > 1 )
		{
			//���L���Ă���,��������K�v�L��
			TYP* P = ms_NewBuf(TYP(), m_lpBuf);
			m_SetBuf(P);
		}
	}

	/**
	 * ���Z
	 *	@param lpText ������
	 *	@return �����̎Q��
	 */
	template<typename T>
	CStrT& m_AddText(const T* lpText)
	{
		if ( lpText != NULL && *lpText != 0 )
		{
			TYP* pNew = ms_NewBuf(TYP(), lpText);
			size_t iAddSize = ms_CalcInfoPointer(pNew)->lLength;
			size_t iNowSize = GetLength();
			if ( *(m_GetRefP()) == 1 && m_GetBufSize() > iAddSize + iNowSize )
			{
				MemCopy(&m_lpBuf[iNowSize], pNew, iAddSize + 1);
			}
			else
			{
				TYP* P = ms_NewBuf(TYP(), m_lpBuf, iAddSize);
				MemCopy(&P[iNowSize], pNew, iAddSize + 1);
				m_SetBuf(P);
			}
			m_GetInfoP()->lLength = iAddSize + iNowSize;
			ASSERTLIB( STRLIB::GetLen(m_lpBuf) == GetLength() );
			ms_Delete(pNew);
		}
		return *this;
	}

	TYP*	m_lpBuf;			///< �f�[�^
};



/**@ingroup STRING
 * ASCII������N���X
 *	@note UNICODE �v���W�F�N�g�ł�ASCII/SJIS���g���܂��B
 *	@note �ꕔ�̑���n�I�y���[�^�́A�Ⴄ�����R�[�h������̑�����\�ł��B
 *	@note printf�n�� Format() Fmt() �� ��s �ɑ΂��āA���̂܂ܓn�����Ƃ��o���܂��B
 *	@attention �C���f�b�N�X�w�������ہA�g���C���o�C�g(�����R�[�h�̓�o�C�g��)���w���Ɠ���͕ۏ�ł��܂���B
 *	@see CStrT ���̃e���v���[�g�� TYP �́uchar�v�ɂȂ�܂��B
 *	@par�K�v�t�@�C��
 *			TnbStr.h
 */
typedef TNB::CStrT<char>	CAscii;



/**\ingroup STRING
 * UNICODE������N���X
 *	@note ASCII �v���W�F�N�g�ł�UNICODE���g���܂��B
 *	@note �ꕔ�̑���n�I�y���[�^�́A�Ⴄ�����R�[�h������̑�����\�ł��B
 *	@note printf�n�� Format() Fmt() �� ��s �ɑ΂��āA���̂܂ܓn�����Ƃ��o���܂��B
 *	@see CStrT ���̃e���v���[�g�� TYP �́uWCHAR�v�ɂȂ�܂��B
 *	@par�K�v�t�@�C��
 *			TnbStr.h
 */
typedef TNB::CStrT<WCHAR>	CUnicode;



/**@ingroup STRING
 * ������N���X
 *	@note �v���W�F�N�g�Ŏw�肵�������R�[�h���g���܂��B
 *	@note �ꕔ�̑���n�I�y���[�^�́A�Ⴄ�����R�[�h������̑�����\�ł��B
 *	@note printf�n�� Format() Fmt() �� ��s �ɑ΂��āA���̂܂ܓn�����Ƃ��o���܂��B
 *	@attention �C���f�b�N�X�w�������ہA�g���C���o�C�g(�����R�[�h�̓�o�C�g��)���w���Ɠ���͕ۏ�ł��܂���B
 *	@see CStrT ���̃e���v���[�g�� TYP �́uTCHAR�v�ɂȂ�܂��B
 *	@par�K�v�t�@�C��
 *			TnbStr.h
 */
typedef TNB::CStrT<TCHAR>	CStr;



/**@ingroup STRING
 * ��������Z(ASCII/SJIS�p).
 *	@note ������������������^�[�����܂��B
 *	@param lpsz ���̕�����
 *	@param str ���Z���镶����
 *	@return ������
 */
inline CAscii operator+(LPCSTR lpsz, const CAscii& str)
{
	CAscii s = lpsz;
	return s += str;
}

/**@ingroup STRING
 * ��������Z(ASCII/SJIS�p).
 *	@note ������������������^�[�����܂��B
 *	@param str1 ���̕�����
 *	@param str2 ���Z���镶����
 *	@return ������
 */
inline CAscii operator+(const CAscii& str1, const CAscii& str2)
{
	CAscii s = str1;
	return s += str2;
}

/**@ingroup STRING
 * ��������Z(UNICODE�p)
 *	@note ������������������^�[�����܂��B
 *	@param lpsz ���̕�����
 *	@param str ���Z���镶����
 *	@return ������
 */
inline CUnicode operator+(LPCWSTR lpsz, const CUnicode& str)
{
	CUnicode s = lpsz;
	return s += str;
}

/**@ingroup STRING
 * ��������Z(UNICODE�p)
 *	@note ������������������^�[�����܂��B
 *	@param str1 ���̕�����
 *	@param str2 ���Z���镶����
 *	@return ������
 */
inline CUnicode operator+(const CUnicode& str1, const CUnicode& str2)
{
	CUnicode s = str1;
	return s += str2;
}

/**@ingroup STRING
 * �V���A���C�Y(ASCII/SJIS).
 *	@note �w��̃V���A���C�U�[�ɖ{�I�u�W�F�N�g�̕ێ����Ă�������o�͂��܂��B
 *	@param[out] _sr �V���A���C�U�[
 *	@param[in] P ������
 *	@return �V���A���C�U�[
 */
inline ISerializer& operator<<(ISerializer& _sr, const char* P)
{
	TRACE0("Serialize[string(ascii)]\n");
	size_t l = STRLIB::GetLen(P);
	ASSERTLIB( l >= 0 );
	_sr << l << l;
	size_t iSize = (l + 1) * sizeof(char);
	_sr.AddRaw(iSize, P);
	return _sr;
}

/**@ingroup STRING
 * �V���A���C�Y(ASCII/SJIS).
 *	@note �w��̃V���A���C�U�[�ɖ{�I�u�W�F�N�g�̕ێ����Ă�������o�͂��܂��B
 *	@param[out] _sr �V���A���C�U�[
 *	@param[in] t ������
 *	@return �V���A���C�U�[
 */
inline ISerializer& operator<<(ISerializer& _sr, const CAscii& t)		
{
	const char* P = t;
	return operator<<(_sr, P);
}

/**@ingroup STRING
 * �f�V���A���C�Y(ASCII/SJIS).
 *	@note �w��̃f�V���A���C�U�[����ɖ{�I�u�W�F�N�g�ɏ�����荞�݂܂��B
 *	@param[in] ds �f�V���A���C�U�[
 *	@param[out] _t ������
 *	@return �f�V���A���C�U�[
 *	@throw CInvalidParamException ������񂪈ُ�̎��X���[����܂��B
 */
inline const IDeserializer& operator>>(const IDeserializer& ds, CAscii& _t)		
{
	TRACE0("Deserialize[string(ascii)]\n");
	int l1, l2;
	ds >> l1 >> l2;
	if ( l1 != l2 )
	{
		throw CInvalidParamException();
	}
	l1++;
	ds.GetRaw(l1 * sizeof(char), _t.GetBuffer(l1 * sizeof(char)));
	_t.ReleaseBuffer();
	return ds;
}

/**@ingroup STRING
 * �V���A���C�Y(UNICODE).
 *	@note �w��̃V���A���C�U�[�ɖ{�I�u�W�F�N�g�̕ێ����Ă�������o�͂��܂��B
 *	@param[out] _sr �V���A���C�U�[
 *	@param[in] P ������
 *	@return �V���A���C�U�[
 */
inline ISerializer& operator<<(ISerializer& _sr, const WCHAR* P)
{
	TRACE0("Serialize[string(unicode)]\n");
	size_t l = STRLIB::GetLen(P);
	ASSERTLIB( l >= 0 );
	_sr << l << l;
	size_t iSize = (l + 1) * sizeof(WCHAR);
	_sr.AddRaw(iSize, P);
	return _sr;
}

/**@ingroup STRING
 * �V���A���C�Y(UNICODE).
 *	@note �w��̃V���A���C�U�[�ɖ{�I�u�W�F�N�g�̕ێ����Ă�������o�͂��܂��B
 *	@param[out] _sr �V���A���C�U�[
 *	@param[in] t ������
 *	@return �V���A���C�U�[
 */
inline ISerializer& operator<<(ISerializer& _sr, const CUnicode& t)		
{
	const WCHAR* P = t;
	return operator<<(_sr, P);
}

/**@ingroup STRING
 * �f�V���A���C�Y(UNICODE).
 *	@note �w��̃f�V���A���C�U�[����ɖ{�I�u�W�F�N�g�ɏ�����荞�݂܂��B
 *	@param[in] ds �f�V���A���C�U�[
 *	@param[out] _t ������
 *	@return �f�V���A���C�U�[
 *	@throw CInvalidParamException ������񂪈ُ�̎��X���[����܂��B
 */
inline const IDeserializer& operator>>(const IDeserializer& ds, CUnicode& _t)	
{
	TRACE0("Deserialize[string(unicode)]\n");
	int l1, l2;
	ds >> l1 >> l2;
	if ( l1 != l2 )
	{
		throw CInvalidParamException();
	}
	l1++;
	ds.GetRaw(l1 * sizeof(WCHAR), _t.GetBuffer(l1 * sizeof(WCHAR)));
	_t.ReleaseBuffer();
	return ds;
}



namespace HASH
{
	/** STRING
	 * [HASH] �n�b�V���l�v�Z.
	 *	@note ASCII/SJIS������p�ł��B
	 *	@param str CAscii
	 *	@return �n�b�V���l
	 */
	inline DWORD CalcHash(const CAscii& str)
	{
		return CalcHash(str.GetLength(), str.operator LPCSTR());
	}

	/** STRING
	 * [HASH] �n�b�V���l�v�Z.
	 *	@note UNICODE������p�ł��B
	 *	@param str CUnicode
	 *	@return �n�b�V���l
	 */
	inline DWORD CalcHash(const CUnicode& str)
	{
		return CalcHash(str.GetLength() * 2, str.operator LPCWSTR());
	}
};



}; // TNB



//T-TestCase�R�[�h�J�o���b�WEnable
#pragma comment(user,"T-Coverage Enable")


