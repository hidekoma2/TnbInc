#pragma once
/**
 *@file
 * テキストファイル関係のヘッダ
 *
 *		テキストファイルの読み書きをサポートするクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbFile.h"
#include "TnbConstStrVector.h"
#include "TnbStrAdder.h"
#include "TnbStrVector.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE STRING
 * テキストファイル書込みクラス
 *
 *	文字列を指定してファイルに書き込むことができます。
 *
 *	@note UNICODE テキストファイルも作成できます。
 *	@note 改行コードは ￥r￥n になります。
 *	@note 最後の行に改行コードは付きません。
 *
 *	@par必要ファイル
 *			TnbTextFile.h
 *
 *	@date 11/02/10 新規作成
 *	@date 11/10/12 NewFile() 新規。
 */
class CTextFileWriter : CCopyImpossible
{
public:

	/// コンストラクタ
	CTextFileWriter(void) : m_pWriter(&m_fw), m_isUnicode(false)
	{
	}

	/**
	 * [確認] 書込み可能？.
	 *		Open しているか確認できます。
	 *	@retval true オープン済み（書込み可能）
	 *	@retval false 未オープン
	 */
	bool CanWrite(void) const
	{
		m_pWriter->CanWrite();
	}

	/**
	 * [処理] クローズ.
	 *	@note オープンしていなかったら何もしません。
	 */
	void Close(void)
	{
		m_pWriter->Close();
		m_pWriter = &m_fw;
	}

	/**
	 * [作成] 新規ファイル作成.
	 *	@param lpszName ファイル名.
	 *	@param isUnicode true なら UNICODE TEXT ファイルを作成します。
	 *					省略か false なら S-JIS TEXT ファイルを作成します。
	 *	@retval true 成功.
	 *	@retval	false 失敗.
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
	 * [作成] 新規ファイル作成.
	 *	@param pWriter ライター。書き込みオープン済み、サイズ0である必要があります。
	 *	@param isUnicode true なら UNICODE TEXT ファイルを作成します。
	 *					省略か false なら S-JIS TEXT ファイルを作成します。
	 *	@retval true 成功.
	 *	@retval	false 失敗.
	 */
	bool Open(IWriter* pWriter, bool isUnicode = false)
	{
		Close();
		return m_Open(pWriter, isUnicode);
	}

	/**
	 * [書込] 一行書き込み.
	 *	@note 改行コードはつけないでください
	 *	@param lpszLine 文字列
	 *	@throw CWriteFailureException 書込みに失敗すると、スローされます。
	 */
	void WriteLine(LPCSTR lpszLine)
	{
		CAscii s = lpszLine;
		s += "\r\n";
		m_Write(s);
	}

	/**
	 * [書込] 一行書き込み.
	 *	@note 改行コードはつけないでください
	 *	@param lpszLine 文字列
	 *	@throw CWriteFailureException 書込みに失敗すると、スローされます。
	 */
	void WriteLine(LPCWSTR lpszLine)
	{
		CUnicode s = lpszLine;
		s += L"\r\n";
		m_Write(s);
	}

	/**
	 * [書込] 書式書き込み.
	 *	@note 必要に応じて改行コードはをつけてください。
	 *	@param lpszFormat 書式
	 *	@param ... パラメータ
	 *	@throw CWriteFailureException 書込みに失敗すると、スローされます。
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
	 * [書込] 書式書き込み.
	 *	@note 必要に応じて改行コードはをつけてください。
	 *	@param lpszFormat 書式
	 *	@param ... パラメータ
	 *	@throw CWriteFailureException 書込みに失敗すると、スローされます。
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
	 * [書込] 書き込み.
	 *	@note 文字列配列要素ごとに、自動的に改行が挿入されます。
	 *	@param vs 文字列配列
	 *	@throw CWriteFailureException 書込みに失敗すると、スローされます。
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
	 * [作成] 新規ファイル作成.
	 *	@note 失敗しても、throw しません。
	 *	@param lpszName ファイル名.
	 *	@param vs 文字列配列
	 *	@param isUnicode true なら UNICODE TEXT ファイルを作成します。
	 *					省略か false なら S-JIS TEXT ファイルを作成します。
	 *	@retval true 成功.
	 *	@retval	false 失敗.
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
	 * [作成] 新規ファイル作成.
	 *	@param pWriter ライター。書き込みオープン済み、サイズ0である必要があります。
	 *	@param isUnicode true なら UNICODE TEXT ファイルを作成します。
	 *					省略か false なら S-JIS TEXT ファイルを作成します。
	 *	@retval true 成功.
	 *	@retval	false 失敗.
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
	// 書込み(ASCII文字列指定)
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
	// 書込み(UNICODE文字列指定)
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
	bool		m_isUnicode;	///< UNICODEモード？
	IWriter*	m_pWriter;		///< ライター
	CFileWriter	m_fw;			///< ファイルライター
};



/**@ingroup FILE STRING
 * テキストファイル読込みクラス
 *
 *	ファイルを読み込んで、行単位に参照できます。
 *
 *	@note UNICODE テキストファイル読み込み可能です。
 *
 *	@par必要ファイル
 *			TnbTextFile.h
 *
 *	@date 11/02/10 新規作成
 */
class CTextFileReader : CCopyImpossible
{
public:

	/// コンストラクタ
	CTextFileReader(void) : m_hasUnicode(false)
	{
	}

	/**
	 * [確認] 内容あり？
	 *	@retval true Read完了し、内容を持っている。
	 *	@retval false なし。
	 */
	bool HasContents(void) const
	{
		return ! m_contents.IsEmpty();
	}

	/**
	 * [確認] UNICODE内容あり？
	 *	@retval true Read完了し、UNICODE の内容を持っている。
	 *	@retval false なし。
	 */
	bool hasUnicodeContents(void) const
	{
		return HasContents() && m_hasUnicode;
	}

	/**
	 * [設定] 空っぽ化.
	 */
	void Empty(void)
	{
		m_contents.RemoveAll();
	}

	/**
	 * [読込] ファイル読み込み.
	 *		指定ファイルを読み込みます。
	 *	@param lpszFileName ファイル名
	 *	@retval true 成功。内容をインスタンス内に取り込んでいます。
	 *	@retval false 失敗。
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
	 * [読込] ファイル読み込み.
	 *		指定ファイルを読み込みます。
	 *	@param pReader 読み込める状態になっているリーダー。
	 *	@retval true 成功。内容をインスタンス内に取り込んでいます。
	 *	@retval false 失敗。
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
	 * [取得] 内容取得.
	 *	@note Read した内容を文字列配列として取り出します。
	 *	@param[out] _vs 取得した内容。
	 *	@retval true 取得成功
	 *	@retval false 失敗
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
	 * [取得] 内容取得.
	 *	@note Read した内容を文字列配列として取り出します。
	 *	@param[out] _vs 取得した内容。
	 *	@retval true 取得成功
	 *	@retval false 失敗
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
	CByteVector	m_contents;		///< 内容物
	bool		m_hasUnicode;	///< UNICODE?
};



}; // TNB
