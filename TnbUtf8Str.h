#pragma once
/**
 *@file
 * Unicode-Translation Format-8(UTF8)型文字列管理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#include "TnbVector.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup STRING
 * Unicode-Translation-Format-8(UTF8)型文字列管理
 *
 *		UTF8型の文字列を一つ保持するクラスです。
 *		ASCII,UNICODEとの相互変換用のメソッドを備えています。
 *
 *	@note	printf系や Format() Fmt() の ％s に対して、そのまま渡せません。
 *			CStr に変換してから渡してください。
 *
 *	@par必要ファイル
 *			TnbUft8Str.h
 * 
 *	@date 06/05/18	新規作成
 *	@date 06/05/19 IComparableT 実装。
 */
class CUtf8Str : public IComparableT<TNB::CUtf8Str>
{
	CVectorT<char> m_vtcText;			///< 情報
public:

	/// コンストラクタ
	CUtf8Str(void)
	{
		Empty();
	}

	/**
	 * コピーコンストラクタ
	 *	@param uft8 コピー元
	 */
	CUtf8Str(const CUtf8Str& uft8) : m_vtcText(uft8.m_vtcText) 
	{
	}

	/**
	 * 代入コンストラクタ(UTF8用)
	 *	@param lpszUtf8 代入するUTF8型文字列
	 */
	CUtf8Str(LPCSTR lpszUtf8)
	{
		SetOfUtf8(lpszUtf8);
	}

	/**
	 * 代入コンストラクタ(ASCII用)
	 *	@param ascii 代入するASCII文字列
	 */
	CUtf8Str(const CAscii& ascii)
	{
		SetOfAscii(ascii);
	}

	/**
	 * 代入コンストラクタ(UNICODE用)
	 *	@param unicode 代入するUNICODE文字列
	 */
	CUtf8Str(const CUnicode& unicode)
	{
		SetOfUnicode(unicode);
	}

	/**
	 * [代入] コピーオペレータ
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CUtf8Str& operator=(const CUtf8Str& other)
	{
		m_vtcText = other.m_vtcText;
		return *this;
	}

	/**
	 * [確認] 比較
	 *	@param t 比較対象
	 *	@retval ０		自分は t と同じ
	 *	@retval １以上	自分は t より大きい
	 *	@retval ０未満	自分は t より小さい
	 */
	virtual INT_PTR Compare(const CUtf8Str& t) const
	{
		return STRLIB::Compare(ReferBuffer(), t.ReferBuffer());
	}

	/**
	 * [設定] 空化
	 */
	void Empty(void)
	{
		m_vtcText.RemoveAll();
		m_vtcText.Add(0);
	}

	/**
	 * [確認] 空か
	 *	@retval true 空
	 *	@retval false 文字有り
	 */
	bool IsEmpty(void) const
	{
		return GetLength() == 0;
	}

	/**
	 * [設定] 代入(UTF8用)
	 *	@param lpszUtf8 代入するUTF8型文字列
	 */
	void SetOfUtf8(LPCSTR lpszUtf8)
	{
		if ( lpszUtf8 == NULL )
		{
			Empty();
		}
		else
		{
			size_t iSize = STRLIB::GetLen(lpszUtf8);
			MemCopy(m_vtcText.GetBuffer(iSize + 1), lpszUtf8, iSize + 1);
			m_vtcText.ReleaseBuffer();
		}
	}

	/**
	 * [設定] 代入(UNICODE用)
	 *	@param lpszUnicode 代入するUNICODE文字列
	 */
	void SetOfUnicode(LPCWSTR lpszUnicode)
	{
		TTRACE0("UNICODE 文字列を UTF8 文字列に変換\n");
		if ( lpszUnicode != NULL )
		{
			size_t iSize = STRLIB::GetLen(lpszUnicode);
			if ( iSize > 0 )
			{
				int nLen = ::WideCharToMultiByte(CP_UTF8, 0, lpszUnicode, -1, NULL, 0, NULL, NULL);
				::WideCharToMultiByte(CP_UTF8, 0, lpszUnicode, -1, m_vtcText.GetBuffer(nLen), nLen, NULL, NULL);
				m_vtcText.ReleaseBuffer();
				return;
			}
		}
		Empty();
	}

	/**
	 * [設定] 代入(ASCII用)
	 *	@param lpszAscii 代入するASCII文字列
	 */
	void SetOfAscii(LPCSTR lpszAscii)
	{
		SetOfUnicode(CUnicode(lpszAscii));
	}

	/**
	 * [追加] 追加オペレータ
	 *	@note 自分の持っている文字列に指定文字列を連結します。
	 *	@param uft8 プラス情報
	 *	@return 自分の参照
	 */
	CUtf8Str& operator+=(const CUtf8Str& uft8)
	{
		m_vtcText.SetSize(GetLength());
		m_vtcText += uft8.m_vtcText;
		return *this;
	}

	/**
	 * [作成] 加算オペレータ
	 *	@note 自分の持っている文字列と指定文字列を連結した文字列を作成します。
	 *	@param uft8 加算文字列
	 *	@return 新しいUTF8文字列
	 */
	CUtf8Str operator+(const CUtf8Str& uft8) const
	{
		CUtf8Str newUTF8 = *this;
		newUTF8 += uft8;
		return newUTF8;
	}

	/**
	 * [取得] UTF8文字列長さ 
	 *	@return UTF8文字列長さ
	 */
	size_t GetLength(void) const
	{
		return STRLIB::GetLen(ReferBuffer());
	}

	/**
	 * [取得] UNICODE文字列取得 
	 *	@return UNICODE文字列
	 */
	CUnicode ToUnicode(void) const
	{
		TTRACE0("UTF8 文字列を UNICODE 文字列に変換\n");
		LPCSTR P = m_vtcText.ReferBuffer();
		int nLen = ::MultiByteToWideChar(CP_UTF8, 0, P, -1, NULL, 0); 
		CUnicode uni;
		::MultiByteToWideChar(CP_UTF8, 0, P, -1, uni.GetBuffer(nLen), nLen); 
		uni.ReleaseBuffer();
		return uni;
	}

	/**
	 * [取得] ASCII文字列取得 
	 *	@return ASCII(SJIS)文字列
	 */
	CAscii ToAscii(void) const
	{
		return CAscii(ToUnicode()); 
	}

	/**
	 * [取得] 文字列取得 
	 *	@return 文字列(ASCII/SJISかUNICODE)
	 */
	CStr ToStr(void) const
	{
		return CStr(ToUnicode());
	}

	/**
	 * [取得] UTF8文字列参照取得 
	 *	@return UTF8文字列の先頭ポインタ
	 */
	LPCSTR ReferBuffer(void) const
	{
		return m_vtcText.ReferBuffer();
	}

	/**
	 * [取得] UTF8文字列参照取得 
	 *	@return UTF8文字列の先頭ポインタ
	 */
	operator LPCSTR (void) const
	{
		return ReferBuffer();
	}

	/**
	 * [操作] 書き込みバッファ要求
	 *	@note UTF8型文字列を書き込んでください。
	 *	@note 必要サイズの書き込んでもよいバッファを返します。
	 *	@note 使用後、必ず ReleaseBuffer() をコールしてください。
	 *	@param iLength 必要な長さ
	 *	@return 書き込み可能アドレス
	 */
	LPSTR GetBuffer(int iLength)
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
		CUtf8Str s;
		s.SetOfUtf8(m_vtcText.ReferBuffer());
		m_vtcText = s.m_vtcText;
	}
};



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")
