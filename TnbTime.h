#pragma once
/**
 *@file
 * ���ԊǗ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include "TnbComparable.h"
#ifndef _WIN32_WCE
 #include <time.h>
 #include <tchar.h>
#endif



//TNB Library
namespace TNB
{



/**@ingroup TIME
 * �t�@�C���^�C���Ǘ��N���X
 *
 *		��{�I�ɕێ����Ă������ UTC �ł��B
 *		LocalTime��������ƁAUTC�ɕϊ����i�[���܂��B
 *
 *		FILETIME �� public�p�����Ă��܂��̂ŁA FILETIME �������ɂ���API�ɒ��ڎg���܂��B
 *		���̍ہA�ϊ������͔������܂���B
 *
 *	@par�K�v�t�@�C��
 *			TnbTime.h
 *
 *	@date 06/06/06 �V�K�쐬
 *	@date 06/07/12 ���O�ύX
 *	@date 06/08/24 ���Ԃ̉����Z�A�ǉ��B
 *	@date 11/08/25 �\������
 *	@date 12/10/03 GetCurrentUtc() ��ǉ��B
 */
class CFileTimeEx : public FILETIME, public IComparableT<TNB::CFileTimeEx> 
{
	DEFSUPER(FILETIME);
public:

	/**
	 * �R���X�g���N�^
	 *	@note 0������܂��B
	 */
	CFileTimeEx(void) : _super()
	{
		Empty(); 
	}

	/**
	 * �R���X�g���N�^
	 *	@note ���݂̎��Ԃ�����܂��B
	 *	@param b �_�~�[
	 */
	CFileTimeEx(bool b) : _super()
	{
		operator=(GetCurrent());
	}

	/**
	 * ����R���X�g���N�^
	 *	@param ft �������FILETIME(UTC)
	 */
	CFileTimeEx(const FILETIME& ft) : _super()
	{
		operator=(ft); 
	}

	/**
	 * ����R���X�g���N�^
	 *	@param st �������SYSTEMTIME(LocalTime)
	 */
	CFileTimeEx(const SYSTEMTIME& st) : _super()
	{
		operator=(st); 
	}

	/**
	 * ����R���X�g���N�^
	 *	@param tm ������� time_t(UTC)
	 */
	CFileTimeEx(const time_t& tm) : _super()
	{
		operator=(tm); 
	}

	/**
	 * [���] ���
	 *	@param ft ������� FILETIME(UTC)
	 *	@return �����̎Q��
	 */
	CFileTimeEx& operator=(const FILETIME& ft)
	{
		_super::dwLowDateTime = ft.dwLowDateTime;
		_super::dwHighDateTime = ft.dwHighDateTime;
		return *this;
	}

	/**
	 * [���] ���
	 *	@param st ������� SYSTEMTIME(LocalTime)
	 *	@return �����̎Q��
	 */
	CFileTimeEx& operator=(const SYSTEMTIME& st)
	{
		FILETIME ft;
		if ( ! ::SystemTimeToFileTime(&st, &ft) || ! ::LocalFileTimeToFileTime(&ft, this) )
		{
			Empty();
		}
		return *this;
	}

	/**
	 * [���] ���
	 *	@param tm ������� time_t(UTC)
	 *	@return �����̎Q��
	 */
	CFileTimeEx& operator=(const time_t& tm)
	{
		ULONGLONG ull = tm;
		ull += 11644473600;
		ull *= 10000000;
		m_Refer() = ull;
		return *this;
	}

	/**
	 * [�擾] time_t�^�Ŏ擾
	 *	@return time_t(UTC)
	 */
	time_t ToTimet(void)
	{
		ULONGLONG ull = m_Refer();
		ull /= 10000000;
		ull -= 11644473600;
		return static_cast<time_t>(ull & DWORD_MAX);
	}

	/**
	 * [�ݒ�] ��
	 */
	void Empty(void)
	{
		m_Refer() = 0;
	}

	/**
	 * [�m�F] ��Ԋm�F
	 *	@retval true ��i�l���O�j
	 *	@retval false ���L��
	 */
	bool IsEmpty(void) const
	{
		return m_Refer() == 0;
	}

	/**
	 * [�m�F] ��r
	 *	@param t ��r�Ώ�
	 *	@retval �O		������ t �Ɠ���
	 *	@retval �P�ȏ�	������ t ���傫��
	 *	@retval �O����	������ t ��菬����
	 */
	virtual INT_PTR Compare(const CFileTimeEx& t) const
	{
		return ::CompareFileTime(this, &t);
	}

	/**
	 * [�擾] ���ԉ��Z
	 *	@param llMs ���Z���鎞�ԁi�~���b�j
	 *	@return �����̎Q��
	 */
	CFileTimeEx& operator+=(LONGLONG llMs)
	{
		(m_Refer()) += (llMs * 10000);
		return *this;
	}

	/**
	 * [�擾] ���Ԍ��Z
	 *	@param llMs ���Z���鎞�ԁi�~���b�j
	 *	@return �����̎Q��
	 */
	CFileTimeEx& operator-=(LONGLONG llMs)
	 {
		(m_Refer()) -= (llMs * 10000);
		return *this;
	}

	/**
	 * [�擾] ���ԉ��Z
	 *	@param llMs ���Z���鎞�ԁi�~���b�j
	 *	@return ���Z���CFileTimeEx
	 */
	CFileTimeEx operator+(LONGLONG llMs) const
	{
		CFileTimeEx t = *this;
		return t += llMs;
	}

	/**
	 * [�擾] ���Ԍ��Z
	 *	@param llMs ���Z���鎞�ԁi�~���b�j
	 *	@return ���Z���CFileTimeEx
	 */
	CFileTimeEx operator-=(LONGLONG llMs) const
	 {
		CFileTimeEx t = *this;
		return t -= llMs;
	}

	/**
	 * [�擾] ���ԍ�
	 *	@param t ��r�Ώ�
	 *	@return ���ԍ��i�~���b�j
	 */
	LONGLONG operator-(const CFileTimeEx& t) const 
	{
		LONGLONG r = (m_Refer() - t.m_Refer()) / 10000;
		return r;
	}

	/**
	 * [�쐬] ������쐬
	 *	@param lpszFormat �t�H�[�}�b�g
	 *	@see http://www.microsoft.com/JAPAN/developer/library/vccore/_crt_strftime.2c_.wcsftime.htm
	 *	@return ������
	 */
	CStr Format(LPCTSTR lpszFormat = NULL) const;

	/**
	 * [�擾] FILETIME�\����
	 *	@return FILETIME �|�C���^
	 */
	operator FILETIME*(void)
	{
		return this;
	}

	/**
	 * [�擾] ���݂̎���
	 *	@return ���݂̎��ԁiUTC�j
	 */
	static CFileTimeEx GetCurrent(void);

private:
	/// FILETIME �� ULONGLONG�ŎQ��
	const ULONGLONG& m_Refer(void) const
	{
		return *reinterpret_cast<const ULONGLONG*>(&(_super::dwLowDateTime));
	}
	/// FILETIME �� ULONGLONG�ŎQ��
	ULONGLONG& m_Refer(void)
	{
		return *reinterpret_cast<ULONGLONG*>(&(_super::dwLowDateTime));
	}
};



/**@ingroup TIME
 * �V�X�e���^�C���Ǘ��N���X
 *
 *		��{�I�ɕێ����Ă�����͌��݂̃^�C���]�[���� Local Time �ł��B
 *		UTC ��������ƁA Local Time �ɕϊ����i�[���܂��B
 *
 *		SYSTEMTIME �� public�p�����Ă��܂��̂ŁA SYSTEMTIME �������ɂ��� API �ɒ��ڎg���܂��B
 *
 *	@note �j��( wDayOfWeek )�͌v�Z�㐳�����Ȃ��l�������Ă���\�����L��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbTime.h
 *
 *	@date 06/06/06	�V�K�쐬
 *	@date 06/08/24	�e�v�f�ɑ΂���v�Z���\�b�h�ǉ��B
 *	@date 07/01/24	�a��ɑ΂���v�Z���\�b�h�ǉ��B
 *	@date 11/07/27	IsValid() ���\�b�h��ǉ��B
 *	@date 11/08.08	Format() �ŁA ��O��������������C���B
 *	@date 11/01/06	AddSecond() �̈����̌^�� LONGLONG �֕ύX�B
 *	@date 12/05/03	StringTo() �V�K�B
 *	@date 19/04/26	ToJapaneseEra() �́u�ߘa�v�Ή��B
 */
class CSystemTime : public SYSTEMTIME, public IComparableT<TNB::CSystemTime> 
{
	DEFSUPER(SYSTEMTIME);
public:

	/**
	 * �R���X�g���N�^
	 *	@note �N�A���A���A���A���ׂĂO�ɂȂ�܂��B
	 */
	CSystemTime(void) : _super()
	{
		Empty();
	}

	/**
	 * ����R���X�g���N�^
	 *	@param st ������� SYSTEMTIME(LocalTime)
	 */
	CSystemTime(const SYSTEMTIME& st) : _super()
	{
		operator=(st); 
	}

	/**
	 * ����R���X�g���N�^
	 *	@param ft ������� FILETIME(UTC)
	 */
	CSystemTime(const FILETIME& ft) : _super()
	{
		operator=(ft); 
	}

	/**
	 * ����R���X�g���N�^
	 *	@param tm ������� time_t(UTC)
	 */
	CSystemTime(const time_t& tm) : _super()
	{
		operator=(tm); 
	}

	/**
	 * [���] ���
	 *	@param st ������� SYSTEMTIME(LocalTime)
	 *	@return �����̎Q��
	 */
	CSystemTime& operator=(const SYSTEMTIME& st)
	{
		ASSERTLIB( offsetof(CSystemTime, wYear) == sizeof(this) ); //wYear�� vtable �̎��ɂ��邱�Ƃ��m�F
		SYSTEMTIME* P = this;
		*P = st;
		return *this;
	}

	/**
	 * [���] ���
	 *	@param ft ������� FILETIME(UTC)
	 *	@return �����̎Q��
	 */
	CSystemTime& operator=(const FILETIME& ft)
	{
		FILETIME ff;
		if ( ! ::FileTimeToLocalFileTime(&ft, &ff) || ! ::FileTimeToSystemTime(&ff, this) )
		{
			Empty();
		}
		return *this;
	}

	/**
	 * [���] ���
	 *	@param tm ������� time_t(UTC)
	 *	@return �����̎Q��
	 */
	CSystemTime& operator=(const time_t& tm)
	{
		CFileTimeEx ft(tm);
		return operator=(ft);
	}

	/**
	 * [���] ���.
	 *		�N�A���A����ݒ�B���͂��ׂĂO�ɂȂ�܂��B
	 *	@param iYear �N�B
	 *	@param iMonth ���B
	 *	@param iDay ���B
	 */
	void Set(int iYear, int iMonth, int iDay)
	{
		Empty();
		_super::wYear = ToWord(iYear);
		_super::wMonth = ToWord(iMonth);
		_super::wDay = ToWord(iDay);
	}

	/** [�擾] �N�擾	@return �N */
	WORD GetYear(void) const	{ return _super::wYear; }
	/** [�擾] ���擾	@return �� */
	WORD GetMonth(void) const	{ return _super::wMonth; }
	/** [�擾] ���擾	@return �� */
	WORD GetDay(void) const		{ return _super::wDay; }
	/** [�擾] ���擾	@return �� */
	WORD GetHour(void) const	{ return _super::wHour; }
	/** [�擾] ���擾	@return �� */
	WORD GetMinute(void) const	{ return _super::wMinute; }
	/** [�擾] �b�擾	@return �b */
	WORD GetSecond(void) const	{ return _super::wSecond; }
	/** [�擾] �~���b�擾	@return �~���b */
	WORD GetMillSec(void) const	{ return _super::wMilliseconds; }

	/**
	 * [�ݒ�] ��
	 */
	void Empty(void)
	{
		memset(&(_super::wYear), 0, sizeof(SYSTEMTIME));
	}

	/**
	 * [�m�F] ��Ԋm�F
	 *	@retval true ��i�l���O�j
	 *	@retval false ���L��
	 */
	bool IsEmpty(void) const
	{
		return _super::wYear == 0;
	}

	/**
	 * [�m�F] �L���m�F.
	 *		�N���������b�~���b�̒l��������(�͈͓�)�ł��邱�Ɗm�F���܂��B
	 *		�j���̓`�F�b�N���܂���B
	 *	@retval true �L��
	 *	@retval false �ُ�
	 */
	bool IsValid(void) const
	{
		FILETIME ft;
		return !! ::SystemTimeToFileTime(this, &ft);
	}

	/**
	 * [�m�F] ��r
	 *	@param t ��r�Ώ�
	 *	@retval �O		������ t �Ɠ���
	 *	@retval �P�ȏ�	������ t ���傫��
	 *	@retval �O����	������ t ��菬����
	 */
	virtual INT_PTR Compare(const CSystemTime& t) const
	{
		int r = wYear - t.wYear;
		if ( r != 0 ) { return r; }
		r = wMonth - t.wMonth;
		if ( r != 0 ) { return r; }
		r = wDay - t.wDay;
		if ( r != 0 ) { return r; }
		r = wHour - t.wHour;
		if ( r != 0 ) { return r; }
		r = wMinute - t.wMinute;
		if ( r != 0 ) { return r; }
		r = wSecond - t.wSecond;
		if ( r != 0 ) { return r; }
		return wMilliseconds - t.wMilliseconds;
	}

	/**
	 * [���Z] �N���Z
	 *	@param iYear ���Z����N(�}�C�i�X���\)
	 *	@return �����̎Q��
	 */
	CSystemTime& AddYear(int iYear)
	{
		int y = _super::wYear + iYear;
		_super::wYear = static_cast<WORD>(y);
		return *this;
	}

	/**
	 * [���Z] �����Z
	 *	@param iMonth ���Z���錎(�}�C�i�X���\)
	 *	@return �����̎Q��
	 */
	CSystemTime& AddMonth(int iMonth)
	{
		if ( iMonth != 0 )
		{
			FILETIME ft;
			bool boRc = !! ::SystemTimeToFileTime(this, &ft);
			iMonth += _super::wMonth;
			while ( iMonth > 12 )
			{
				iMonth -= 12;
				_super::wYear++;
			}
			while ( iMonth < 1 )
			{
				iMonth += 12;
				_super::wYear--;
			}
			_super::wMonth = static_cast<WORD>(iMonth);
			if ( boRc )
			{
				while ( ! ::SystemTimeToFileTime(this, &ft) )
				{
					_super::wDay--;
					if ( _super::wDay == 0 )
					{
						Empty();
						break;
					}
				}
			}
		}
		return *this;
	}

	/**
	 * [���Z] �����Z
	 *	@param iDay ���Z�����(�}�C�i�X���\)
	 *	@return �����̎Q��
	 */
	CSystemTime& AddDay(int iDay)
	{
		return AddHour(iDay * 24);
	}

	/**
	 * [���Z] �����Z
	 *	@param iHour ���Z���鎞��(�}�C�i�X���\)
	 *	@return �����̎Q��
	 */
	CSystemTime& AddHour(int iHour)
	{
		return AddMinute(iHour * 60);
	}

	/**
	 * [���Z] �����Z
	 *	@param iMinute ���Z���镪(�}�C�i�X���\)
	 *	@return �����̎Q��
	 */
	CSystemTime& AddMinute(int iMinute)
	{
		return AddSecond(iMinute * 60);
	}

	/**
	 * [���Z] �b���Z
	 *	@param iSecond ���Z����b(�}�C�i�X���\)
	 *	@return �����̎Q��
	 */
	CSystemTime& AddSecond(LONGLONG iSecond)
	{
		return AddMillSec(iSecond * 1000);
	}

	/**
	 * [���Z] �~���b���Z
	 *	@param llMillSec ���Z����~���b(�}�C�i�X���\)
	 *	@return �����̎Q��
	 */
	CSystemTime& AddMillSec(LONGLONG llMillSec)
	{
		return operator=(CFileTimeEx(*this) + llMillSec);
	}

	/**
	 * [�擾] ���ԍ�
	 *	@param t ��r�Ώ�
	 *	@return ���ԍ��i�~���b�j
	 */
	LONGLONG operator-(const CSystemTime& t) const 
	{
		return CFileTimeEx(*this) - CFileTimeEx(t);
	}
	
	/**
	 * [�擾] UTC�擾
	 *	@note �ێ����Ă��鎞�Ԃ����݂̃^�C���]�[���̃��[�J�����ԂƂ��āAUTC�ɕϊ����܂��B
	 *	@return ����(UTC)
	 */
	CSystemTime LocalToUtc(void) const
	{
		FILETIME ft1;
		FILETIME ft2;
		CSystemTime st;
		if ( ! ::SystemTimeToFileTime(this, &ft1)
			|| ! ::LocalFileTimeToFileTime(&ft1, &ft2)
			|| ! ::FileTimeToSystemTime(&ft2, &st) )
		{
			st.Empty();
		}
		return st;
	}

	/**
	 * [�擾] ���[�J���^�C���擾
	 *	@note �ێ����Ă��鎞�Ԃ�UTC�Ƃ��āA���݂̃^�C���]�[���̃��[�J�����Ԃɕϊ����܂��B
	 *	@return ����(LocalTime)
	 */
	CSystemTime UtcToLocal(void) const
	{
		FILETIME ft1;
		FILETIME ft2;
		CSystemTime st;
		if ( ! ::SystemTimeToFileTime(this, &ft1)
			|| ! ::FileTimeToLocalFileTime(&ft1, &ft2)
			|| ! ::FileTimeToSystemTime(&ft2, &st) )
		{
			st.Empty();
		}
		return st;
	}

	#ifndef _WIN32_WCE

	/**
	 * [�擾] �w�胍�[�J���^�C���擾
	 *	@note	�ێ����Ă��鎞�Ԃ����݂̃^�C���]�[���̃��[�J�����ԂƂ��āA
	 *			�w��̃^�C���]�[���̃��[�J�����Ԃɕϊ����܂��B
	 *	@note WindowCE�ł͎g�p�ł��܂���B
	 *	@param lpTzInfo �^�C���]�[�����
	 *	@return ����(����LocalTime)
	 */
	CSystemTime LocalToTzSpecificLocal(LPTIME_ZONE_INFORMATION lpTzInfo) const
	{
		CSystemTime r;
		CSystemTime st = LocalToUtc();
		if ( ! ::SystemTimeToTzSpecificLocalTime(lpTzInfo, &r, &st) )
		{
			r.Empty();
		}
		return r;
	}

	#endif

	/**
	 * [���] ������ɂ����.
	 *		�N4���A��2���A��2���A��2���A��2���A�b2���A�~���b3���̕����񂩂������܂��B
	 *	@note �e���l�̕~���̓n�C�t���A�R�����A�X���b�V���A�s���I�h�A�A�|�X�g���t�B�[�A�X�y�[�X���F�߂��܂��i�Ȃ��Ă�OK�j�B
	 *		�܂��A�e�����͕K���O�p�f�B���O���Ă����K�v������܂��B
	 *	\par��:
	 *	\code
	 *		"1921/01/01 12:30:00.111"
	 *		"19710216193000"
	 *	\endcode
	 *	@param lpsz ������
	 */
	void StringTo(LPCTSTR lpsz)
	{
		CStr s = lpsz;
		s.Remove(_T('-'));
		s.Remove(_T(':'));
		s.Remove(_T('/'));
		s.Remove(_T('.'));
		s.Remove(_T('\''));
		s.Remove(_T(' '));
		s += _T("000000000000000000");
		_super::wYear = static_cast<WORD>(s.Mid(0, 4).ToInt());
		_super::wMonth = static_cast<WORD>(s.Mid(4, 2).ToInt());
		_super::wDay = static_cast<WORD>(s.Mid(6, 2).ToInt());
		_super::wHour = static_cast<WORD>(s.Mid(8, 2).ToInt());
		_super::wMinute = static_cast<WORD>(s.Mid(10, 2).ToInt());
		_super::wSecond = static_cast<WORD>(s.Mid(12, 2).ToInt());
		_super::wMilliseconds = static_cast<WORD>(s.Mid(14, 3).ToInt());
		_super::wDayOfWeek = 0;
	}

	/**
	 * [�쐬] ������쐬
	 *	@param lpszFormat �t�H�[�}�b�g�B�ȗ�����ƁA"%Y-%m-%d %H:%M:%S"���K�p����܂�
	 *	@see https://msdn.microsoft.com/ja-jp/library/fe06s4ak.aspx
	 *	@return ������
	 */
	CStr Format(LPCTSTR lpszFormat = NULL) const
	{
		if ( lpszFormat == NULL )
		{
			lpszFormat = _T("%Y-%m-%d %H:%M:%S");
		}
		CStr s;
		if ( IsValid() )
		{
			tm tTime;
			tTime.tm_year	= wYear - 1900;	/* years since 1900 */
			tTime.tm_mon	= wMonth - 1;	/* months since January - [0,11] */
			tTime.tm_mday	= wDay;			/* day of the month - [1,31] */
			tTime.tm_hour	= wHour;		/* hours since midnight - [0,23] */
			tTime.tm_min	= wMinute;		/* minutes after the hour - [0,59] */
			tTime.tm_sec	= wSecond;		/* seconds after the minute - [0,59] */
			if ( _tcsftime(s.GetBuffer(1024), 1024, lpszFormat, &tTime) == 0 )
			{
				s.Empty();
			}
			s.ReleaseBuffer();
		}
		return s;
	}

	/**
	 * [�쐬] ������쐬
	 *	@note �h��:��:�b.�_�b�h�Ƃ����t�H�[�}�b�g�̕����񂪓����܂��B
	 *	@return ������
	 */
	CStr ToTimeString(void) const
	{
		return CStr::Fmt(_T("%02d:%02d:%02d.%03d"), wHour, wMinute, wSecond, wMilliseconds);
	}

	/**
	 * [�쐬] �a�N���쐬.
	 *		���ݕێ����Ă���N��a��ɕϊ����܂��B
	 *		������ 1868/1/25�A�吳�� 1912/7/30�A���a�� 1926/12/25�A������ 1989/1/7�A�ߘa�� 2019/5/1 ���n�܂�Ƃ��Ă��܂��B
	 *	@note �����V�c�̑��ʂ� 1868/10/23 �����A�����ُ̏��ɂ́u���c��l�Nਖ������N�v�Ƃ���A
	 *		�������N�̌ď̂����߂�Ƃ������Ƃ���A1��1���i�O���S���I��1868�N1��25���j�ɑk���ēK�p����Ă���B
	 *	@param[out] _iYear �a��̔N
	 *	@retval -1 �G���[
	 *	@retval 0 ����
	 *	@retval 1 �吳
	 *	@retval 2 ���a
	 *	@retval 3 ����
	 *	@retval 4 �ߘa
	 */
	int ToJapaneseEra(int& _iYear) const
	{
		CSystemTime st;
		st.Set(1868, 1, 25);	///�����̎n�܂�
		if ( *this < st )
		{
			return -1;
		}
		st.Set(1912, 7, 30);	///�吳�̎n�܂�
		if ( *this < st )
		{
			//������
			_iYear = _super::wYear - 1868 +1;
			return 0;
		}
		st.Set(1926, 12, 25);	///���a�̎n�܂�
		if ( *this < st )
		{
			//�吳��
			_iYear = _super::wYear - 1912 +1;
			return 1;
		}
		st.Set(1989, 1, 7);	///�����̎n�܂�
		if ( *this < st )
		{
			//���a��
			_iYear = _super::wYear - 1926 +1;
			return 2;
		}
		st.Set(2019, 5, 1);	///�ߘa�̎n�܂�
		if ( *this < st )
		{
			//������
			_iYear = _super::wYear - 1989 +1;
			return 3;
		}
		_iYear = _super::wYear - 2019 +1;
		return 4;
	}

	/**
	 * [�擾] ���݂̎���(LOCAL)
	 *	@return ���݂̎���(LOCAL)
	 */
	static CSystemTime GetCurrent(void)
	{
		SYSTEMTIME st;
		::GetLocalTime(&st);
		return st;
	}

	/**
	 * [�擾] ���݂̎���(UTC)
	 *	@return ���݂̎���(UTC)
	 */
	static CSystemTime GetCurrentUtc(void)
	{
		SYSTEMTIME st;
		::GetSystemTime(&st);
		return st;
	}

	/**
	 * [�v�Z] �j���v�Z.
	 *		�w����̗j���ԍ����Z�o���܂��B
	 *	@param t ����
	 *	@return �j���ԍ��B0=���j�A1=���j�A2=�Ηj�A3=���j�A4=�ؗj�A5=���j�A6=�y�j
	 */
	static DWORD GetDayOfTheWeek(const SYSTEMTIME& t)
	{
		DWORD dwYear	= t.wYear;
		DWORD dwMonth	= t.wMonth;
		DWORD dwDay		= t.wDay;
		if ( dwMonth == 1 || dwMonth == 2 )
		{
			dwMonth += 12;
			dwYear--;
		}
		double d = dwYear + (dwYear / 4) - (dwYear / 100) + (dwYear / 400) + (2.6 * dwMonth) + 1.6 + dwDay;
		return DWORD(d) % 7;
	}

	/**
	 * [�m�F] �[�N����
	 *	@param dwYear �N
	 *	@retval true	�[�N
	 *	@retval false	���N
	 */
	static bool IsLeap(DWORD dwYear)
	{
		if ( (dwYear % 4) == 0 && (dwYear % 100) != 0 || (dwYear % 400) == 0 )
		{
			return true;
		}
		return false;
	}

	/**
	 * [�v�Z] �����v�Z
	 *	@param dwYear �N
	 *	@param dwMonth ��
	 *	@return ����
	*/
	static DWORD GetEndOfMonth(DWORD dwYear, DWORD dwMonth)
	{
		switch ( dwMonth )
		{
		case 2:
			return IsLeap(dwYear) ? 29 : 28;
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
		default:
			break;
		}
		return 31;
	}
};



/// FileTime �̃t�H�[�}�b�g�֐�
inline CStr CFileTimeEx::Format(LPCTSTR lpszFormat) const
{
	CSystemTime st;
	::FileTimeToSystemTime(this, &st);
	return st.Format(lpszFormat);
}



/// FileTime�̍��̎��Ԏ擾�֐�
inline CFileTimeEx CFileTimeEx::GetCurrent(void)
{
	#ifndef _WIN32_WCE
		FILETIME ft;
		::GetSystemTimeAsFileTime(&ft);
		return ft;
	#else
		return CSystemTime::GetCurrent();
	#endif
}



};//TNB



#ifdef _TnbDOXYGEN	//Document�쐬�p�V���{��

/** 
 * �V�X�e���^�C���^.
 *		���A���A�N�A�j���A���A���A�b�A�~���b�̊e�����o���g�p�����t�Ǝ��Ԃ�\���܂��B
 *	@note winbase.h �Œ�`����Ă���\���̂ł��B
 */
struct SYSTEMTIME 
{
	WORD wYear;			///< �N
	WORD wMonth;		///< ��
	WORD wDayOfWeek;	///< �j��
	WORD wDay;			///< ��
	WORD wHour;			///< ��
	WORD wMinute;		///< ��
	WORD wSecond;		///< �b
	WORD wMilliseconds;	///< �~���b
};

/**
 * �t�@�C���^�C���^.
 *		1601 �N 1 �� 1 ������� 100 �i�m�b�Ԋu�̐���\�� 64 �r�b�g�̒l�ł��B
 *	@note winbase.h �Œ�`����Ă���\���̂ł��B
 */
struct FILETIME 
{
	DWORD dwLowDateTime;	///< low 32bits
	DWORD dwHighDateTime;	///< high 32bits
};

#endif

