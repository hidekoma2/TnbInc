#pragma once
/**
 *@file
 * Double Null Terminate(DNT)型文字列操作関係のヘッダ
 *
 *		Windows API の引数、戻り値にある、複数の文字列を扱う方法の一つ、
 *		「￥0を仕切り、￥0￥0が終端」という文字列を扱うクラスです。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStrVector.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup STRING
 * Double Null Terminate(DNT)型文字列管理
 *
 *		Windows API の引数、戻り値にある、複数の文字列を扱う方法の一つ、
 *		「￥0を仕切り、￥0￥0が終端」という文字列を扱うクラスです。
 *
 *	@par使用例
 *	<table><tr><td><pre>
 *
 *		CStrVector vstrSecNames;
 *		CDoubleNullTerminateStr dntstr;
 *		DWORD dwRc = ::GetPrivateProfileSectionNames( //DNT型で受ける		
 *		                   dntstr.GetBuffer(65536),
 *		                   65536 , _T("Abc.ini") );
 *		dntstr.ReleaseBuffer();
 *		if ( dwRc > 0 )
 *		{
 *			vstrSecNames = dntstr.ToStrVector(); //文字列配列の分解		
 *		}
 *		return vstrSecNames;
 *	</pre></td></tr></table>
 *
 *	@note	printf系や Format() Fmt() の ％s に対して、そのまま渡せません。
 *			CStr に変換してから渡してください。
 *
 *	@par必要ファイル
 *			TnbDntStr.h
 * 
 *	@date 06/05/07 新規作成
 *	@date 06/05/19 IComparableT 実装。
 */
class CDoubleNullTerminateStr : public IComparableT<TNB::CDoubleNullTerminateStr>
{
	CVectorT<TCHAR> m_vtcText;			///< 情報
public:

	/**
	 * コンストラクタ
	 *	@param lpszStr 格納する文字列
	 */
	CDoubleNullTerminateStr(LPCTSTR lpszStr = NULL)
	{
		Set(lpszStr);
	}

	/**
	 * コピーコンストラクタ
	 *	@param d コピー元
	 */
	CDoubleNullTerminateStr(const CDoubleNullTerminateStr& d) : m_vtcText(d.m_vtcText) 
	{
	}

	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval ０		自分は t と同じ
	 *	@retval １以上	自分は t より大きい
	 *	@retval ０未満	自分は t より小さい
	 */
	virtual INT_PTR Compare(const CDoubleNullTerminateStr& t) const
	{
		return STRLIB::Compare(ToString(), t.ToString());
	}

	/**
	 * [設定] 空化
	 */
	void Empty(void)
	{
		m_vtcText.RemoveAll();
		m_vtcText.Add(0);
		m_vtcText.Add(0);
	}

	/**
	 * [設定] 代入
	 *	@param lpszzDntStr 格納する DNT型文字列
	 *	@param tcSepaChar 敷居キャラ。省略すると０です。
	 */
	void SetDntStr(LPCTSTR lpszzDntStr, TCHAR tcSepaChar = 0)
	{
		if ( lpszzDntStr != NULL )
		{
			LPCTSTR P = lpszzDntStr;
			int i = 0;
			while ( P[i] != tcSepaChar || P[i + 1] != tcSepaChar )
			{
				i++;
			}
			m_vtcText.SetElements(i + 2, lpszzDntStr);
			if ( tcSepaChar != 0 )
			{
				loop ( i, m_vtcText.GetSize() )
				{
					if ( m_vtcText[i] == tcSepaChar )
					{
						m_vtcText.Set(i, 0);
					}
				}
			}
		}
		else
		{
			Empty();
		}
	}

	/**
	 * [設定] 代入
	 *	@param lpszStr 格納する文字列
	 */
	void Set(LPCTSTR lpszStr)
	{
		Empty();
		Add(lpszStr);
	}

	/**
	 * [設定] 代入
	 *	@note 配列をDNT型で格納します。
	 *	@param vstr 代入する CStrVector のインスタンス
	 */
	void Set(const CStrVector& vstr)
	{
		Empty();
		Add(vstr);
	}

	/**
	 * [設定] 追加.
	 *		保持している DNT型文字列に指定の通常の文字列を追加します。
	 *	@param lpszStr 格納する文字列
	 */
	void Add(LPCTSTR lpszStr)
	{
		if ( lpszStr != NULL )
		{
			size_t iStrSize = STRLIB::GetLen(lpszStr);
			if ( iStrSize > 0 )
			{
				size_t iVecSize = m_vtcText.GetSize();
				if ( iVecSize > 2 )
				{
					//Emptyじゃない時NULLを一つ残す。
					m_vtcText.SetSize(iVecSize - 1);
					m_vtcText.AddElements(iStrSize, lpszStr);
				}
				else
				{
					m_vtcText.SetElements(iStrSize, lpszStr);
				}
				m_vtcText.Add(0);
				m_vtcText.Add(0);
			}
		}
	}

	/**
	 * [設定] 追加.
	 *		保持している DNT型文字列に、配列内容をDNT型で格納します。
	 *	@param vstr 追加する CStrVector のインスタンス
	 */
	void Add(const CStrVector& vstr)
	{
		loop ( i, vstr.GetSize() )
		{
			Add(vstr[i]);
		}
	}

	/**
	 * [取得] DNT型文字列取得 
	 *	@return DNT型文字列の参照
	 */
	operator LPCTSTR(void) const
	{
		return m_vtcText.ReferBuffer();
	}

	/**
	 * [取得] DNT型文字列数種特
	 *	@return DNT型文字列数（NULL二つ含む長さ）
	 */
	size_t GetSize(void) const
	{
		return m_vtcText.GetSize();
	}

	/**
	 * [取得] 文字列配列取得.
	 *		保持している DNT型文字列を終端で分離し、 CStrVector に変換し返します。
	 *	@return CStrVector
	 */
	CStrVector ToStrVector(void) const
	{
		CStrVector vstr;
		LPCTSTR P = m_vtcText.ReferBuffer();
		while ( true )
		{
			vstr.Add(P);
			P += STRLIB::GetLen(P) + 1;
			if ( *P == 0 )
			{
				break;
			}
		}
		return vstr;
	}

	/**
	 * [取得] 文字列取得.
	 *		保持している DNT型文字列の仕切り文字を指定の文字に置き換え、
	 *		通常の文字列に変換し返します。
	 *	@param tcSepaChar 仕切り文字、省略すると '|' になります。
	 *	@return 変換後の普通の文字列。
	 */
	CStr ToString(TCHAR tcSepaChar = '|') const
	{
		CStr strResult;
		CStrVector vstr = ToStrVector();
		loop ( i, vstr.GetSize() )
		{
			strResult += vstr[i];
			strResult += tcSepaChar;
		}
		strResult += tcSepaChar;
		return strResult;
	}

	/**
	 * [操作] 書き込みバッファ要求.
	 *		必要サイズの書き込んでもよいバッファを返します。
	 *	@note DNT型文字列を書き込んでください。
	 *	@note 使用後、必ず ReleaseBuffer() をコールしてください。
	 *	@param iLength 必要な長さ
	 *	@return 書き込み可能アドレス
	 */
	LPTSTR GetBuffer(int iLength)
	{
		return m_vtcText.GetBuffer(iLength);
	}

	/**
	 * [操作] 割り当てたバッファを開放.
	 *		GetBuffer() で割り当てたバッファを開放し、文字列を本クラスに取り込みます。
	 *	@note GetBuffer()で取得した領域に書き込みが終わったら必ず ReleaseBuffer() してください。
	 */
	void ReleaseBuffer(void)
	{
		m_vtcText.ReleaseBuffer();
		CDoubleNullTerminateStr str;
		str.SetDntStr(m_vtcText.ReferBuffer());
		m_vtcText = str.m_vtcText;
	}
};



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")
