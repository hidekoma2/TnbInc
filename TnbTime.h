#pragma once
/**
 *@file
 * 時間管理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
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
 * ファイルタイム管理クラス
 *
 *		基本的に保持している情報は UTC です。
 *		LocalTimeを代入すると、UTCに変換し格納します。
 *
 *		FILETIME を public継承していますので、 FILETIME を引数にもつAPIに直接使えます。
 *		その際、変換処理は発生しません。
 *
 *	@par必要ファイル
 *			TnbTime.h
 *
 *	@date 06/06/06 新規作成
 *	@date 06/07/12 名前変更
 *	@date 06/08/24 時間の加減算、追加。
 *	@date 11/08/25 構造整理
 *	@date 12/10/03 GetCurrentUtc() を追加。
 */
class CFileTimeEx : public FILETIME, public IComparableT<TNB::CFileTimeEx> 
{
	DEFSUPER(FILETIME);
public:

	/**
	 * コンストラクタ
	 *	@note 0が入ります。
	 */
	CFileTimeEx(void) : _super()
	{
		Empty(); 
	}

	/**
	 * コンストラクタ
	 *	@note 現在の時間が入ります。
	 *	@param b ダミー
	 */
	CFileTimeEx(bool b) : _super()
	{
		operator=(GetCurrent());
	}

	/**
	 * 代入コンストラクタ
	 *	@param ft 代入するFILETIME(UTC)
	 */
	CFileTimeEx(const FILETIME& ft) : _super()
	{
		operator=(ft); 
	}

	/**
	 * 代入コンストラクタ
	 *	@param st 代入するSYSTEMTIME(LocalTime)
	 */
	CFileTimeEx(const SYSTEMTIME& st) : _super()
	{
		operator=(st); 
	}

	/**
	 * 代入コンストラクタ
	 *	@param tm 代入する time_t(UTC)
	 */
	CFileTimeEx(const time_t& tm) : _super()
	{
		operator=(tm); 
	}

	/**
	 * [代入] 代入
	 *	@param ft 代入する FILETIME(UTC)
	 *	@return 自分の参照
	 */
	CFileTimeEx& operator=(const FILETIME& ft)
	{
		_super::dwLowDateTime = ft.dwLowDateTime;
		_super::dwHighDateTime = ft.dwHighDateTime;
		return *this;
	}

	/**
	 * [代入] 代入
	 *	@param st 代入する SYSTEMTIME(LocalTime)
	 *	@return 自分の参照
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
	 * [代入] 代入
	 *	@param tm 代入する time_t(UTC)
	 *	@return 自分の参照
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
	 * [取得] time_t型で取得
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
	 * [設定] 空化
	 */
	void Empty(void)
	{
		m_Refer() = 0;
	}

	/**
	 * [確認] 状態確認
	 *	@retval true 空（値が０）
	 *	@retval false 情報有り
	 */
	bool IsEmpty(void) const
	{
		return m_Refer() == 0;
	}

	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval ０		自分は t と同じ
	 *	@retval １以上	自分は t より大きい
	 *	@retval ０未満	自分は t より小さい
	 */
	virtual INT_PTR Compare(const CFileTimeEx& t) const
	{
		return ::CompareFileTime(this, &t);
	}

	/**
	 * [取得] 時間加算
	 *	@param llMs 加算する時間（ミリ秒）
	 *	@return 自分の参照
	 */
	CFileTimeEx& operator+=(LONGLONG llMs)
	{
		(m_Refer()) += (llMs * 10000);
		return *this;
	}

	/**
	 * [取得] 時間減算
	 *	@param llMs 減算する時間（ミリ秒）
	 *	@return 自分の参照
	 */
	CFileTimeEx& operator-=(LONGLONG llMs)
	 {
		(m_Refer()) -= (llMs * 10000);
		return *this;
	}

	/**
	 * [取得] 時間加算
	 *	@param llMs 加算する時間（ミリ秒）
	 *	@return 加算後のCFileTimeEx
	 */
	CFileTimeEx operator+(LONGLONG llMs) const
	{
		CFileTimeEx t = *this;
		return t += llMs;
	}

	/**
	 * [取得] 時間減算
	 *	@param llMs 減算する時間（ミリ秒）
	 *	@return 減算後のCFileTimeEx
	 */
	CFileTimeEx operator-=(LONGLONG llMs) const
	 {
		CFileTimeEx t = *this;
		return t -= llMs;
	}

	/**
	 * [取得] 時間差
	 *	@param t 比較対象
	 *	@return 時間差（ミリ秒）
	 */
	LONGLONG operator-(const CFileTimeEx& t) const 
	{
		LONGLONG r = (m_Refer() - t.m_Refer()) / 10000;
		return r;
	}

	/**
	 * [作成] 文字列作成
	 *	@param lpszFormat フォーマット
	 *	@see http://www.microsoft.com/JAPAN/developer/library/vccore/_crt_strftime.2c_.wcsftime.htm
	 *	@return 文字列
	 */
	CStr Format(LPCTSTR lpszFormat = NULL) const;

	/**
	 * [取得] FILETIME構造体
	 *	@return FILETIME ポインタ
	 */
	operator FILETIME*(void)
	{
		return this;
	}

	/**
	 * [取得] 現在の時間
	 *	@return 現在の時間（UTC）
	 */
	static CFileTimeEx GetCurrent(void);

private:
	/// FILETIME を ULONGLONGで参照
	const ULONGLONG& m_Refer(void) const
	{
		return *reinterpret_cast<const ULONGLONG*>(&(_super::dwLowDateTime));
	}
	/// FILETIME を ULONGLONGで参照
	ULONGLONG& m_Refer(void)
	{
		return *reinterpret_cast<ULONGLONG*>(&(_super::dwLowDateTime));
	}
};



/**@ingroup TIME
 * システムタイム管理クラス
 *
 *		基本的に保持している情報は現在のタイムゾーンの Local Time です。
 *		UTC を代入すると、 Local Time に変換し格納します。
 *
 *		SYSTEMTIME を public継承していますので、 SYSTEMTIME を引数にもつ API に直接使えます。
 *
 *	@note 曜日( wDayOfWeek )は計算後正しくない値を示している可能性が有ります。
 *
 *	@par必要ファイル
 *			TnbTime.h
 *
 *	@date 06/06/06	新規作成
 *	@date 06/08/24	各要素に対する計算メソッド追加。
 *	@date 07/01/24	和暦に対する計算メソッド追加。
 *	@date 11/07/27	IsValid() メソッドを追加。
 *	@date 11/08.08	Format() で、 例外が発生する問題を修正。
 *	@date 11/01/06	AddSecond() の引数の型を LONGLONG へ変更。
 *	@date 12/05/03	StringTo() 新規。
 *	@date 19/04/26	ToJapaneseEra() の「令和」対応。
 */
class CSystemTime : public SYSTEMTIME, public IComparableT<TNB::CSystemTime> 
{
	DEFSUPER(SYSTEMTIME);
public:

	/**
	 * コンストラクタ
	 *	@note 年、月、日、他、すべて０になります。
	 */
	CSystemTime(void) : _super()
	{
		Empty();
	}

	/**
	 * 代入コンストラクタ
	 *	@param st 代入する SYSTEMTIME(LocalTime)
	 */
	CSystemTime(const SYSTEMTIME& st) : _super()
	{
		operator=(st); 
	}

	/**
	 * 代入コンストラクタ
	 *	@param ft 代入する FILETIME(UTC)
	 */
	CSystemTime(const FILETIME& ft) : _super()
	{
		operator=(ft); 
	}

	/**
	 * 代入コンストラクタ
	 *	@param tm 代入する time_t(UTC)
	 */
	CSystemTime(const time_t& tm) : _super()
	{
		operator=(tm); 
	}

	/**
	 * [代入] 代入
	 *	@param st 代入する SYSTEMTIME(LocalTime)
	 *	@return 自分の参照
	 */
	CSystemTime& operator=(const SYSTEMTIME& st)
	{
		ASSERTLIB( offsetof(CSystemTime, wYear) == sizeof(this) ); //wYearが vtable の次にあることを確認
		SYSTEMTIME* P = this;
		*P = st;
		return *this;
	}

	/**
	 * [代入] 代入
	 *	@param ft 代入する FILETIME(UTC)
	 *	@return 自分の参照
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
	 * [代入] 代入
	 *	@param tm 代入する time_t(UTC)
	 *	@return 自分の参照
	 */
	CSystemTime& operator=(const time_t& tm)
	{
		CFileTimeEx ft(tm);
		return operator=(ft);
	}

	/**
	 * [代入] 代入.
	 *		年、月、日を設定。他はすべて０になります。
	 *	@param iYear 年。
	 *	@param iMonth 月。
	 *	@param iDay 日。
	 */
	void Set(int iYear, int iMonth, int iDay)
	{
		Empty();
		_super::wYear = ToWord(iYear);
		_super::wMonth = ToWord(iMonth);
		_super::wDay = ToWord(iDay);
	}

	/** [取得] 年取得	@return 年 */
	WORD GetYear(void) const	{ return _super::wYear; }
	/** [取得] 月取得	@return 月 */
	WORD GetMonth(void) const	{ return _super::wMonth; }
	/** [取得] 日取得	@return 日 */
	WORD GetDay(void) const		{ return _super::wDay; }
	/** [取得] 時取得	@return 時 */
	WORD GetHour(void) const	{ return _super::wHour; }
	/** [取得] 分取得	@return 分 */
	WORD GetMinute(void) const	{ return _super::wMinute; }
	/** [取得] 秒取得	@return 秒 */
	WORD GetSecond(void) const	{ return _super::wSecond; }
	/** [取得] ミリ秒取得	@return ミリ秒 */
	WORD GetMillSec(void) const	{ return _super::wMilliseconds; }

	/**
	 * [設定] 空化
	 */
	void Empty(void)
	{
		memset(&(_super::wYear), 0, sizeof(SYSTEMTIME));
	}

	/**
	 * [確認] 状態確認
	 *	@retval true 空（値が０）
	 *	@retval false 情報有り
	 */
	bool IsEmpty(void) const
	{
		return _super::wYear == 0;
	}

	/**
	 * [確認] 有効確認.
	 *		年月日時分秒ミリ秒の値が正しい(範囲内)であること確認します。
	 *		曜日はチェックしません。
	 *	@retval true 有効
	 *	@retval false 異常
	 */
	bool IsValid(void) const
	{
		FILETIME ft;
		return !! ::SystemTimeToFileTime(this, &ft);
	}

	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval ０		自分は t と同じ
	 *	@retval １以上	自分は t より大きい
	 *	@retval ０未満	自分は t より小さい
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
	 * [加算] 年加算
	 *	@param iYear 加算する年(マイナスも可能)
	 *	@return 自分の参照
	 */
	CSystemTime& AddYear(int iYear)
	{
		int y = _super::wYear + iYear;
		_super::wYear = static_cast<WORD>(y);
		return *this;
	}

	/**
	 * [加算] 月加算
	 *	@param iMonth 加算する月(マイナスも可能)
	 *	@return 自分の参照
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
	 * [加算] 日加算
	 *	@param iDay 加算する日(マイナスも可能)
	 *	@return 自分の参照
	 */
	CSystemTime& AddDay(int iDay)
	{
		return AddHour(iDay * 24);
	}

	/**
	 * [加算] 時加算
	 *	@param iHour 加算する時間(マイナスも可能)
	 *	@return 自分の参照
	 */
	CSystemTime& AddHour(int iHour)
	{
		return AddMinute(iHour * 60);
	}

	/**
	 * [加算] 分加算
	 *	@param iMinute 加算する分(マイナスも可能)
	 *	@return 自分の参照
	 */
	CSystemTime& AddMinute(int iMinute)
	{
		return AddSecond(iMinute * 60);
	}

	/**
	 * [加算] 秒加算
	 *	@param iSecond 加算する秒(マイナスも可能)
	 *	@return 自分の参照
	 */
	CSystemTime& AddSecond(LONGLONG iSecond)
	{
		return AddMillSec(iSecond * 1000);
	}

	/**
	 * [加算] ミリ秒加算
	 *	@param llMillSec 加算するミリ秒(マイナスも可能)
	 *	@return 自分の参照
	 */
	CSystemTime& AddMillSec(LONGLONG llMillSec)
	{
		return operator=(CFileTimeEx(*this) + llMillSec);
	}

	/**
	 * [取得] 時間差
	 *	@param t 比較対象
	 *	@return 時間差（ミリ秒）
	 */
	LONGLONG operator-(const CSystemTime& t) const 
	{
		return CFileTimeEx(*this) - CFileTimeEx(t);
	}
	
	/**
	 * [取得] UTC取得
	 *	@note 保持している時間を現在のタイムゾーンのローカル時間として、UTCに変換します。
	 *	@return 時間(UTC)
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
	 * [取得] ローカルタイム取得
	 *	@note 保持している時間をUTCとして、現在のタイムゾーンのローカル時間に変換します。
	 *	@return 時間(LocalTime)
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
	 * [取得] 指定ローカルタイム取得
	 *	@note	保持している時間を現在のタイムゾーンのローカル時間として、
	 *			指定のタイムゾーンのローカル時間に変換します。
	 *	@note WindowCEでは使用できません。
	 *	@param lpTzInfo タイムゾーン情報
	 *	@return 時間(特殊LocalTime)
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
	 * [代入] 文字列による代入.
	 *		年4桁、月2桁、日2桁、時2桁、分2桁、秒2桁、ミリ秒3桁の文字列から代入します。
	 *	@note 各数値の敷居はハイフン、コロン、スラッシュ、ピリオド、アポストロフィー、スペースが認められます（なくてもOK）。
	 *		また、各数字は必ず０パディングしておく必要があります。
	 *	\par例:
	 *	\code
	 *		"1921/01/01 12:30:00.111"
	 *		"19710216193000"
	 *	\endcode
	 *	@param lpsz 文字列
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
	 * [作成] 文字列作成
	 *	@param lpszFormat フォーマット。省略すると、"%Y-%m-%d %H:%M:%S"が適用されます
	 *	@see https://msdn.microsoft.com/ja-jp/library/fe06s4ak.aspx
	 *	@return 文字列
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
	 * [作成] 文字列作成
	 *	@note ”時:分:秒.㍉秒”というフォーマットの文字列が得られます。
	 *	@return 文字列
	 */
	CStr ToTimeString(void) const
	{
		return CStr::Fmt(_T("%02d:%02d:%02d.%03d"), wHour, wMinute, wSecond, wMilliseconds);
	}

	/**
	 * [作成] 和年号作成.
	 *		現在保持している年を和暦に変換します。
	 *		明治は 1868/1/25、大正は 1912/7/30、昭和は 1926/12/25、平成は 1989/1/7、令和は 2019/5/1 を始まりとしています。
	 *	@note 明治天皇の測位は 1868/10/23 だが、改元の詔書には「改慶應四年爲明治元年」とあり、
	 *		改元が年の呼称を改めるということから、1月1日（グレゴリオ暦1868年1月25日）に遡って適用されている。
	 *	@param[out] _iYear 和暦の年
	 *	@retval -1 エラー
	 *	@retval 0 明治
	 *	@retval 1 大正
	 *	@retval 2 昭和
	 *	@retval 3 平成
	 *	@retval 4 令和
	 */
	int ToJapaneseEra(int& _iYear) const
	{
		CSystemTime st;
		st.Set(1868, 1, 25);	///明治の始まり
		if ( *this < st )
		{
			return -1;
		}
		st.Set(1912, 7, 30);	///大正の始まり
		if ( *this < st )
		{
			//明治だ
			_iYear = _super::wYear - 1868 +1;
			return 0;
		}
		st.Set(1926, 12, 25);	///昭和の始まり
		if ( *this < st )
		{
			//大正だ
			_iYear = _super::wYear - 1912 +1;
			return 1;
		}
		st.Set(1989, 1, 7);	///平成の始まり
		if ( *this < st )
		{
			//昭和だ
			_iYear = _super::wYear - 1926 +1;
			return 2;
		}
		st.Set(2019, 5, 1);	///令和の始まり
		if ( *this < st )
		{
			//平成だ
			_iYear = _super::wYear - 1989 +1;
			return 3;
		}
		_iYear = _super::wYear - 2019 +1;
		return 4;
	}

	/**
	 * [取得] 現在の時間(LOCAL)
	 *	@return 現在の時間(LOCAL)
	 */
	static CSystemTime GetCurrent(void)
	{
		SYSTEMTIME st;
		::GetLocalTime(&st);
		return st;
	}

	/**
	 * [取得] 現在の時間(UTC)
	 *	@return 現在の時間(UTC)
	 */
	static CSystemTime GetCurrentUtc(void)
	{
		SYSTEMTIME st;
		::GetSystemTime(&st);
		return st;
	}

	/**
	 * [計算] 曜日計算.
	 *		指定日の曜日番号を算出します。
	 *	@param t 時間
	 *	@return 曜日番号。0=日曜、1=月曜、2=火曜、3=水曜、4=木曜、5=金曜、6=土曜
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
	 * [確認] 閏年判定
	 *	@param dwYear 年
	 *	@retval true	閏年
	 *	@retval false	平年
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
	 * [計算] 末日計算
	 *	@param dwYear 年
	 *	@param dwMonth 月
	 *	@return 末日
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



/// FileTime のフォーマット関数
inline CStr CFileTimeEx::Format(LPCTSTR lpszFormat) const
{
	CSystemTime st;
	::FileTimeToSystemTime(this, &st);
	return st.Format(lpszFormat);
}



/// FileTimeの今の時間取得関数
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



#ifdef _TnbDOXYGEN	//Document作成用シンボル

/** 
 * システムタイム型.
 *		月、日、年、曜日、時、分、秒、ミリ秒の各メンバを使用し日付と時間を表します。
 *	@note winbase.h で定義されている構造体です。
 */
struct SYSTEMTIME 
{
	WORD wYear;			///< 年
	WORD wMonth;		///< 月
	WORD wDayOfWeek;	///< 曜日
	WORD wDay;			///< 日
	WORD wHour;			///< 時
	WORD wMinute;		///< 分
	WORD wSecond;		///< 秒
	WORD wMilliseconds;	///< ミリ秒
};

/**
 * ファイルタイム型.
 *		1601 年 1 月 1 日からの 100 ナノ秒間隔の数を表す 64 ビットの値です。
 *	@note winbase.h で定義されている構造体です。
 */
struct FILETIME 
{
	DWORD dwLowDateTime;	///< low 32bits
	DWORD dwHighDateTime;	///< high 32bits
};

#endif

