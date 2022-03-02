#pragma once
/**
 *@file
 * MD5(message-digest algorithm)関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル
	namespace MD5
	{
		//RSA Data Security のソース
		#include "other/MD5.h"
	};
#endif



/**@ingroup CIPHER
 * MD5(message-digest algorithm)計算クラス.
 *
 *	指定情報の 128BITのハッシュコードを計算します。
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *		
 *		CStr s = CMd5Computer::ComputeHash("abc").GetString();
 *		ASSERT( s == "900150983cd24fb0d6963f7d28e17f72" );
 *		</pre></td></tr></table>
 *
 *	@see http://www.faqs.org/rfcs/rfc1321.html
 *
 *	@par必要ファイル 
 *			TnbMd5Computer.h
 *
 *	@date 06/09/26 新規作成
 *	@date 06/09/27 Updateを2つ以上行なう可能性があることを考慮。
 */
class CMd5Computer : CCopyImpossible
{
	MD5::MD5_CTX	m_context;
public:

	/**@ingroup CIPHER
	 * Md5処理クラスの結果型.
	 *	@see CMd5Computer::Compute()
	 */
	struct TResult
	{
		/// ハッシュ情報
		BYTE hash[16];
		/// ハッシュ情報(文字列)取得
		CStr GetString(void)
		{
			CStr s;
			loop ( i, 16 )
			{
				s += CStr::Fmt(_T("%02x"), hash[i]);
			}
			return s;
		}
	};

	/// コンストラクタ
	CMd5Computer(void)
	{
		MD5::MD5Init(&m_context);
	}

	/**
	 * [設定] ロードマジック値設定
	 *	@param a マジック A
	 *	@param b マジック B
	 *	@param c マジック C
	 *	@param d マジック D
	 */
	void SetLoadMagic(DWORD a, DWORD b, DWORD c, DWORD d)
	{
		m_context.state[0] = a;
		m_context.state[1] = b;
		m_context.state[2] = c;
		m_context.state[3] = d;
	}

	/**
	 * [設定] 対象データ追加
	 *	@param size データサイズ
	 *	@param P データ
	 */
	void Update(size_t size, LPCVOID P)
	{
		MD5::MD5Update(&m_context, static_cast<const BYTE*>(P), size);
	}

	/**
	 * [設定] 対象データ追加
	 *	@param lpszText ASCII文字列
	 */
	void Update(LPCSTR lpszText)
	{
		Update(STRLIB::GetLen(lpszText), lpszText);
	}

	/**
	 * [計算] ハッシュ取得
	 *	@note Update() で追加したデータのハッシュを計算して返します。
	 *	@return 結果
	 */
	TResult GetHash(void)
	{
		TResult t;
		MD5::MD5Final(t.hash, &m_context);
		MD5::MD5Init(&m_context);
		return t;
	}

	/**
	 * [計算] ハッシュ計算
	 *	@param size データサイズ
	 *	@param P データ
	 *	@return 結果
	 */
	static TResult ComputeHash(size_t size, LPCVOID P)
	{
		TResult t;
		MD5::MD5_CTX context;
		MD5::MD5Init(&context);
		MD5::MD5Update(&context, static_cast<const BYTE*>(P), size);
		MD5::MD5Final(t.hash, &context);
		return t;
	}

	/**
	 * [計算] ハッシュ計算
	 *	@param lpszText ASCII文字列
	 *	@return 結果
	 */
	static TResult ComputeHash(LPCSTR lpszText)
	{
		return ComputeHash(STRLIB::GetLen(lpszText), lpszText);
	}
#if 0
	/**
	 *	[計算] HMAC_MD5計算.
	 *		MD5のダイジェストを生成する
	 *	@param sizeInput Inputデータサイズ
	 *	@param pInput Inputデータ
	 *	@param sizeKey Keyデータサイズ
	 *	@param pKey Keyデータ
	 *	@return 結果
	 */
	static TResult ComputeHmacMd5(size_t sizeInput, LPCVOID pInput, size_t sizeKey, LPCVOID pKey)
	{
		TResult t;
		BYTE ipad[64];
		BYTE opad[64];
		if ( sizeKey>64 ) 
		{
			// キーが64バイトより大きい場合はキーのダイジェストをキーにする
			t		= ComputeHash(sizeKey, pKey);
			pKey	= t.hash;
			sizeKey = 16;
		}
		// MD5(key XOR opad, MD5(key XOR ipad, input)) の計算
		::FillMemory(ipad, sizeof(ipad), 0x36);
		::FillMemory(opad, sizeof(opad), 0x5c);
		const BYTE* B = static_cast<const BYTE*>(pKey);
		for ( size_t i = 0; i < sizeKey; i++ )
		{
			ipad[i] ^= B[i];
			opad[i] ^= B[i];
		}
		CMd5Computer m;
		m.Update(64, ipad);
		m.Update(sizeInput, pInput);
		t = m.GetHash();
		m.Update(64, opad);
		m.Update(16, t.hash);
		return m.GetHash();
	}

	/**
	 *	[計算] HMAC_MD5計算.
	 *		MD5のダイジェストを生成する
	 *	@param lpszInput Inputデータ
	 *	@param lpszKey Keyデータ
	 *	@return 結果
	 */
	static TResult ComputeHmacMd5(LPCSTR lpszInput, LPCSTR lpszKey)
	{
		return ComputeHmacMd5(STRLIB::GetLen(lpszInput), lpszInput, STRLIB::GetLen(lpszKey), lpszKey);
	}
#endif
};



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")
