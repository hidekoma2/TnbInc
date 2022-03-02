#pragma once
/**
 *@file
 * 文字列連結関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbBlockLinkVector.h"



//TNB Library
namespace TNB
{



/**@ingroup STRING
 * 文字列連結専門管理
 *
 *		文字列の連結に特化した文字列管理クラスです。
 *
 *	@par必要ファイル
 *			TnbStrAdder.h
 *
 *	@date 09/10/28	新規作成
 *	@date 11/08/03	構造変更。非テンプレート化。
 */
class CStrAdder : CCopyImpossible
{
public:

	/// コンストラクタ.
	CStrAdder(void) : m_length(0)
	{
	}

	/// デストラクタ.
	~CStrAdder(void)
	{
		Empty();
	}

	/**
	 * [取得] 文字列長取得.
	 *	@return 文字列長
	 */
	size_t GetLength(void) const
	{
		return m_length;
	}

	/**
	 * [取得] 文字列取得.
	 *	@note 本メソッドコール時に、実際文字列を連結するため、一回目は時間がかかることが有ります。
	 *		また、返したアドレスは、本インスタンスを操作すると、無効になりますのでご注意ください。
	 *	@return 文字列
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
	 * [設定] 空っぽ化.
	 *	@note バッファは開放しません。
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
	 * [追加] 文字列追加.
	 *	@param len 追加が予想される最大長。
	 *	@param lpszFmt フォーマット.
	 *	@param ... パラメータ
	 *	@return 追加した文字列長
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
			ASSERT0( false, "CStrAdder::AddFormat", "用意したサイズを超えました" );
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
	 * [追加] 文字列追加.
	 *	@note 追加されるサイズが不明な場合にこちらを使います。
	 *	@param lpszFmt フォーマット.
	 *	@param ... パラメータ
	 *	@return 追加した文字列長
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
	 * [追加] 文字列追加.
	 *	@param lpsz 文字列.
	 */
	void Add(LPCTSTR lpsz)
	{
		TParam& p = m_New();
		p.Set(lpsz);
		m_length += p.length;
	}

	/**
	 * [追加] 文字列追加.
	 *	@param lpsz 文字列.
	 */
	void operator+=(LPCTSTR lpsz)
	{
		Add(lpsz);
	}

	/**
	 * [追加] 文字追加.
	 *	@param c 文字.
	 */
	void Add(TCHAR c)
	{
		TParam& p = m_New();
		p.Set(c);
		m_length += 1;
	}

	/**
	 * [追加] 文字追加.
	 *	@param c 文字.
	 */
	void operator+=(TCHAR c)
	{
		Add(c);
	}

	/**
	 * [設定] バッファ調整.
	 *	@note 多めにバッファを指定しておくことで、バッファ再確保の必要が減り、処理速度が速くなります。
	 *	@param len 追加するバッファサイズ
	 */
	void Regulate(size_t len)
	{
	}

private:
	/// パラメタ
	struct TParam
	{
		size_t	length;			///< 長さ
		union
		{
			TCHAR	one;		///< 長さ1の場合のデータ
			TCHAR*	pMany;		///< 長さ2以上の場合のデータ管理
		};
		/// コンストラクタ
		TParam(void) : length(0)
		{
		}
		/// 長さのみ設定
		void SetLength(size_t len)
		{
			Empty();
			length = len;
			if ( length > 1 )
			{
				pMany = new TCHAR[length + 1];
			}
		}
		/// 文字列設定
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
		/// セッター
		void Set(TCHAR c)
		{
			Empty();
			length = 1;
			one = c;
		}
		/// 空化
		void Empty(void)
		{
			if ( length > 1 )
			{
				delete[] pMany;
			}
			length = 0;
		}
	};
	/// 新規作成
	TParam& m_New(void)
	{
		m_string.Free();
		INDEX r = m_work.Add(TParam());
		return m_work[r];
	}
	CBlockLinkVectorT<TParam>	m_work;		///< ワーク
	size_t						m_length;	///< 文字列長
	mutable CWorkMemT<TCHAR>	m_string;	///< operator LPCTSTR 用
};



}; //TNB
