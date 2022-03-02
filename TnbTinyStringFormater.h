#pragma once
/**
 *@file
 * 簡易文字列書式関係のヘッダ
 *
 *		文字列を書式化するクラスが有ります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include <TnbDef.h>



//TNB Library
namespace TNB
{



/**@ingroup STRING 
 * 簡易文字列書式.
 *
 *		追加される文字数が想定される場合で、速度が求められる場合に、非常に有効な文字列フォーマッターです。
 *
 *	@see CTinyStringFormater
 *
 *	@par必要ファイル
 *			TnbTinyStringFormater.h
 *
 *	@date 11/08/25	新規作成 
 *	@date 11/09/27	AddFormatHexInt() , operator LPCSTR() 追加。
 *	@date 12/03/07	テンプレート化
 */
template<typename TYP = TCHAR>
class CTinyStringFormaterT : CCopyImpossible
{
public:

	/// コンストラクタ.
	CTinyStringFormaterT(void) : m_pos(0), m_pWork(NULL), m_workSize(0)
	{
	}

	/**
	 * [設定] 初期化.
	 *		本インスタンスで格納していくワークメモリのアドレスと長さを指定します。
	 *	@note 必ずはじめに実行すること。
	 *	@param _P 文字列を格納していくワークメモリのアドレス
	 *	@param len _P のワークメモリの長さ
	 */
	void Initialize(TYP* _P, size_t len)
	{
		m_pWork = _P;
		m_workSize = len;
		m_pos = 0;
	}

	/**
	 * [取得] 文字列数取得.
	 *	@return 追加した文字数.
	 */
	size_t GetLength(void) const
	{
		return m_pos;
	}

	/**
	 * [取得] 文字列取得.
	 *	@return Initialize() で指定したワークメモリ.
	 */
	operator const TYP*(void) const
	{
		return m_pWork;
	}

	/**
	 * [取得] ワークメモリ残り取得.
	 *	@return 残りのワークメモリサイズ.
	 */
	size_t GetRemainingLength(void) const
	{
		if ( m_workSize < m_pos )
		{
			return 0;
		}
		return m_workSize - m_pos;
	}

	/**
	 * [追加] 終端文字追加.
	 *		他のメソッドでは終端文字は付加されません。
	 */
	void AddTerminateChar(void)
	{
		ASSERT( m_pWork != NULL );
		ASSERT( m_pos + 1 <= m_workSize );
		m_pWork[m_pos] = 0;
	}

	/**
	 * [追加] int 型追加.
	 *		指定の int型数値を文字列化して追加します。
	 *	@param value int 型数値
	 *	@param width 文字数.
	 */
	void AddFormatInt(int value, size_t width)
	{
		ASSERT( m_pWork != NULL );
		ASSERT( m_pos + width <= m_workSize );
		FormatInt(&m_pWork[m_pos], value, width);
		m_pos += width;
	}

	/**
	 * [追加] int 型追加.
	 *		指定の int型数値を文字列化して追加します。
	 *	@note 最大11文字になるので、残りバッファが 11以上あるようにしてください。
	 *	@param value int 型数値
	 */
	void AddFormatInt(int value)
	{
		ASSERT( m_pWork != NULL );
		m_pos += FormatInt(&m_pWork[m_pos], value);
	}

	/**
	 * [追加] DWORD 型 HEX 追加.
	 *		指定の DWORD 型数値を HEX 文字列化して追加します。
	 *	@param value DWORD 型数値
	 *	@param width 文字数.
	 */
	void AddFormatHexInt(DWORD value, size_t width)
	{
		ASSERT( m_pWork != NULL );
		ASSERT( m_pos + width <= m_workSize );
		FormatHex(&m_pWork[m_pos], value, width);
		m_pos += width;
	}

	/**
	 * [追加] 一文字追加.
	 *	@param c 文字.
	 */
	void AddChar(TCHAR c)
	{
		ASSERT( m_pWork != NULL );
		ASSERT( m_pos + 1 <= m_workSize );
		m_pWork[m_pos] = c;
		m_pos++;
	}

	/**
	 * [追加] 文字列リテラル追加.
	 *	@param literal 文字列リテラル.
	 */
	template<typename XX>
	void AddLiteral(const XX& literal)
	{
		const TYP* lpsz = literal;			// ここでエラー出る場合、引数に文字列リテラル以外を渡している可能性有りです
		size_t len = sizeof(literal) / sizeof(TYP);
		Add(lpsz, len - 1); //終端は入れない
	}

	/**
	 * [追加] 文字列追加.
	 *	@note 文字数がわかっている場合、 文字数指定の Add(const TYP*, size_t) を使ったほうが高速です。
	 *	@param lpsz 文字列.
	 */
	void Add(const TYP* lpsz)
	{
		ASSERT( m_pWork != NULL );
		size_t len = STRLIB::GetLen(lpsz);
		ASSERT( m_pos + len <= m_workSize );
		memcpy(&m_pWork[m_pos], lpsz, len * sizeof(TYP));
		m_pos += len;
	}

	/**
	 * [追加] 文字列追加.
	 *	@note 裏技になりますが、例えば、以下のように指定することで終端を同時に追加可能です。
	 *	\code
	 *		CTinyStringFormater sf;
	 *		sf.Initilaize(...);
	 *		sf.Add("ABC", 4);		// 一つ多くする。
	 *	\endcode
	 *	@param lpsz 文字列.
	 *	@param len 文字列長さ
	 */
	void Add(const TYP* lpsz, size_t len)
	{
		ASSERT( m_pWork != NULL );
		ASSERT( m_pos + len <= m_workSize );
		memcpy(&m_pWork[m_pos], lpsz, len * sizeof(TYP));
		m_pos += len;
	}


	//-------------------------------


	/**
	 * [設定] int型文字列化(文字数指定).
	 *		指定の int 型の数値を指定の文字数の文字列に変換します。
	 *		終端は書かれません。
	 *	\code
	 *		char	buf[100];
	 *		CTinyStringFormater::FormatInt(buf, -100, 6);
	 *		buf[6] = 0; //終端
	 *		// buf は "  -100" になる。
	 *	\endcode
	 *	@param[out] _P 書式化した文字列を格納する。 width 以上のサイズが必要.
	 *	@param[in] value int型数値.
	 *	@param[in] width 変換後の文字数
	 */
	static void FormatInt(TYP* _P, int value, size_t width)
	{
		unsigned int v = (value < 0) ? -value : value;
		size_t j = 0;
		loop_dn ( i, width )
		{
			j = i;
			_P[i] = static_cast<TYP>('0' + (v % 10));
			v /= 10;
			if ( v == 0 )
			{
				break;
			}
		}
		if ( value < 0 && j > 0 )
		{
			_P[j - 1] = '-';
			j--;
		}
		if ( j > 0 )
		{
			loop ( i , j )
			{
				_P[i] = ' ';
			}
		}
	}

	/**
	 * [設定] int型文字列化.
	 *		指定の int 型の数値を文字列に変換します。
	 *		終端は書かれません。
	 *	\code
	 *		char	buf[100];
	 *		int r = CTinyStringFormater::FormatInt(buf, -100);
	 *		buf[r] = 0; //終端
	 *		// buf は "-100" になる。
	 *	\endcode
	 *	@param[out] _P 書式化した文字列を格納する。 最大11文字になる可能性があるので、それ以上のサイズを用意.
	 *	@param[in] value int型数値.
	 *	@return 書式化した文字数
	 */
	static INT_PTR FormatInt(TYP* _P, int value)
	{
		TYP* Q = _P;
		if ( value == 0 )
		{
			*_P++ = '0';
		}
		else
		{
			unsigned int v = (value < 0) ? -value : value;
			if ( value < 0 )
			{
				*_P++ = '-';
			}
			int j = 0;
			TYP buf[32];
			while ( v > 0 )
			{
				buf[j++] = static_cast<TYP>('0' + (v % 10));
				v /= 10;
			}
			loop_dn ( i, j )
			{
				*_P++ = buf[i];
			}
		}
		return _P - Q;
	}

	/**
	 * [設定] int型文字列化(０パディング付き).
	 *		指定の int 型の数値を指定の文字数の文字列に変換します。
	 *		終端は書かれません。
	 *	\code
	 *		char	buf[100];
	 *		CTinyStringFormater::FormatInt0(buf, -100, 6);
	 *		buf[6] = 0; //終端
	 *		// buf は "-00100" になる。
	 *	\endcode
	 *	@param[out] _P 書式化した文字列を格納する。 width 以上のサイズが必要.
	 *	@param[in] value int型数値.
	 *	@param[in] width 変換後の文字数
	 */
	static void FormatInt0(TYP* _P, int value, size_t width)
	{
		unsigned int v = (value < 0) ? -value : value;
		loop_dn ( i, width )
		{
			_P[i] = static_cast<TYP>('0' + (v % 10));
			v /= 10;
		}
		if ( value < 0 )
		{
			_P[0] = '-';
		}
	}

	/**
	 * [設定] DWORD型文字列化(文字数指定).
	 *		指定の DWORD 型の数値を指定の文字数の文字列に変換します。
	 *		終端は書かれません。
	 *	\code
	 *		char	buf[100];
	 *		CTinyStringFormater::FormatDword(buf, 100, 6);
	 *		buf[6] = 0; //終端
	 *		// buf は "   100" になる。
	 *	\endcode
	 *	@param[out] _P 書式化した文字列を格納する。 width 以上のサイズが必要.
	 *	@param[in] value DWORD型数値.
	 *	@param[in] width 変換後の文字数
	 */
	static void FormatDword(TYP* _P, DWORD value, size_t width)
	{
		loop_dn ( i, width )
		{
			_P[i] = static_cast<TYP>('0' + (v % 10));
			v /= 10;
			if ( v == 0 )
			{
				break;
			}
		}
		if ( j > 0 )
		{
			loop ( i , j )
			{
				_P[i] = ' ';
			}
		}
	}

	/**
	 * [設定] DWORD型文字列化.
	 *		指定の DWORD 型の数値を文字列に変換します。
	 *		終端は書かれません。
	 *	\code
	 *		char	buf[100];
	 *		int r = CTinyStringFormater::FormatDword(buf, 100);
	 *		buf[r] = 0; //終端
	 *		// buf は "100" になる。
	 *	\endcode
	 *	@param[out] _P 書式化した文字列を格納する。 最大11文字になる可能性があるので、それ以上のサイズを用意.
	 *	@param[in] value int型数値.
	 *	@return 書式化した文字数
	 */
	static INT_PTR FormatDword(TYP* _P, DWORD value)
	{
		TYP* Q = _P;
		if ( value == 0 )
		{
			*_P++ = '0';
		}
		else
		{
			int j = 0;
			TCHAR buf[32];
			while ( value > 0 )
			{
				buf[j++] = static_cast<TYP>('0' + (value % 10));
				value /= 10;
			}
			loop_dn ( i, j )
			{
				*_P++ = buf[i];
			}
		}
		return _P - Q;
	}

	/**
	 * [設定] DWORD型文字列化(０パディング付き).
	 *		指定の DWORD 型の数値を指定の文字数の文字列に変換します。
	 *		終端は書かれません。
	 *	\code
	 *		char	buf[100];
	 *		CTinyStringFormater::FormatDword0(buf, 100, 6);
	 *		buf[6] = 0; //終端
	 *		// buf は "000100" になる。
	 *	\endcode
	 *	@param[out] _P 書式化した文字列を格納する。 width 以上のサイズが必要.
	 *	@param[in] value DWORD型数値.
	 *	@param[in] width 変換後の文字数
	 */
	static void FormatDword0(TYP* _P, DWORD value, size_t width)
	{
		loop_dn ( i, width )
		{
			_P[i] = static_cast<TYP>('0' + (value % 10));
			value /= 10;
		}
	}

	/**
	 * [設定] DWORD型 HEX 文字列化(０パディング付き).
	 *		指定の DWORD 型の数値を指定の文字数の文字列に変換します。
	 *		終端は書かれません。
	 *	\code
	 *		char	buf[100];
	 *		CTinyStringFormater::FormatHex(buf, 100, 4);
	 *		buf[4] = 0; //終端
	 *		// buf は "0064" になる。
	 *	\endcode
	 *	@param[out] _P 書式化した文字列を格納する。 width 以上のサイズが必要.
	 *	@param[in] value DWORD 型数値.
	 *	@param[in] width 変換後の文字数
	 */
	static void FormatHex(TYP* _P, DWORD value, size_t width)
	{
		loop_dn ( i, width )
		{
			_P[i] =  STRLIB::IntToHexChar(value & 0xF);
			value >>= 4;
		}
	}

private:
	TYP*			m_pWork;		///< 作業アドレス
	size_t			m_workSize;		///< 作業サイズ
	INDEX			m_pos;			///< 位置
	friend class CTinyStringFormaterTest;
};



/**@ingroup STRING 
 * 簡易文字列書式.
 *
 *		追加される文字数が想定される場合で、速度が求められる場合に、非常に有効な文字列フォーマッターです。
 *
 *		本インスタンス自体バッファを持ちません。ですので、はじめに Initialize() でワークメモリを指定する必要があります。
 *		自動的にメモリサイズを拡張することはしませんので、ご注意ください（その代わり、高速に書式化することが出来ます）。
 *
 *	\par 使用例
 *	\code
 *		#define CR "\r"
 *		// [取得] LOG 文字列取得.
 *		//	@param[out] _lpsz 'datax, xxxx, xxxx, xxxx, xxxx, xxxx' の 40 文字
 *		int ToLogString(LPSTR _lpsz, INDEX index)
 *		{
 *			CTinyStringFormater lsf;
 *			lsf.Initialize(_lpsz, 40);	
 *			lsf.AddLiteral("data");		lsf.AddFormatInt(index, 1);
 *			lsf.AddLiteral(", ");		lsf.AddFormatInt(Get(1, index), 4);
 *			lsf.AddLiteral(", ");		lsf.AddFormatInt(Get(2, index), 4);
 *			lsf.AddLiteral(", ");		lsf.AddFormatInt(Get(3, index), 4);
 *			lsf.AddLiteral(", ");		lsf.AddFormatInt(Get(4, index), 4);
 *			lsf.AddLiteral(", ");		lsf.AddFormatInt(Get(5, index), 4);
 *			lsf.Add(CR ,sizeof(CR));	//終端も付加
 *			return lsf.GetLength() - 1;
 *		}
 *	\endcode
 *
 *	@par必要ファイル
 *			TnbTinyStringFormater.h
 *
 *	@date 11/08/25	新規作成 
 *	@date 11/09/27	AddFormatHexInt() , operator LPCSTR() 追加。
 *	@date 12/03/07	テンプレート化。 UNICODE にも対応。
 */
class CTinyStringFormater : public CTinyStringFormaterT<>
{
};



}; // TNB

