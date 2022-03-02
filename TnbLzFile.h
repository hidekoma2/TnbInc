#pragma once
/**
 *@file
 * LZファイル関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbReader.h"



#include <lzexpand.h>
#pragma comment(lib,"lz32.lib")



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * LZファイル読み込みクラス
 *
 *		ファイル一つの読み込みをサポートするクラスです。
 *		Microsoft ファイル圧縮ユーティリティ（COMPRESS.EXE）で圧縮されたファイルも読み込めます。
 *
 *	@todo 圧縮されたファイルの展開動作が未確認です。
 *
 *	@par必要ファイル
 *			TnbLzFile.h
 *
 *	@date 07/01/22 新規作成
 */
class CLzFileReader : public IReader
{
	INT		m_iHandle;		///< ハンドル
	CStr	m_strName;		///< ファイル名

	/// クローズ
	void m_Close(void)
	{
		m_strName.Empty();
		if ( m_iHandle >= 0 )
		{
			::LZClose(m_iHandle);
			m_iHandle = -1;
		}
	}

public:

	/// コンストラクタ
	CLzFileReader(void) : m_iHandle(-1)
	{
	}

	/// デストラクタ
	virtual ~CLzFileReader(void)
	{
		m_Close();
	}

	/**
	 * [設定] オープン
	 *	@param lpszLzFile Compress.exeで圧縮されたファイル名か、通常のファイル名。
	 *	@param boIsShare 省略かtrueなら、共有モードでオープンします。
	 *	@retval true オープン成功
	 *	@retval false オープン失敗
	 */
	bool Open(LPCTSTR lpszLzFile, bool boIsShare = true)
	{
		CStr s = lpszLzFile;
		Close();
		CWorkMemT<OFSTRUCT> work;
		INT r = ::LZOpenFile(
					s.GetBuffer(), work, static_cast<WORD>(OF_READ | (boIsShare ? OF_SHARE_DENY_NONE : OF_SHARE_EXCLUSIVE)));
		if ( r >= 0 )
		{
			m_strName = lpszLzFile;
			m_iHandle = r;
		}
		return r >= 0;
	}

	/**
	 * [複製] コピー
	 *	@note コピー元は、 Open() で指定してください。
	 *	@param lpszDstFile 複製先ファイル名。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Copy(LPCTSTR lpszDstFile)
	{
		if ( ! CanRead() ){ return false; }
		CWorkMemT<OFSTRUCT> work;
		CStr s = lpszDstFile;
		INT r = ::LZOpenFile(s.GetBuffer(), work, OF_CREATE);
		if ( r >= 0 )
		{
			if ( ::LZCopy(m_iHandle, r) >= 0 )
			{
				return true;
			}
		}		
		return false;
	}

	/**
	 * [取得] オープン名取得
	 *	@return 名前
	 */
	virtual LPCTSTR GetOpenName(void) const
	{
		return m_strName;
	}

	/**
	 * [操作] クローズ
	 *	@note 使用していたテンポラリファイルも削除。
	 */
	virtual void Close(void)
	{
		m_Close();
	}

	/**
	 * [確認] 読み込み可能か
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	virtual bool CanRead(void) const
	{
		return m_iHandle >= 0;
	}

	/**
	 * [取得] サイズ取得
	 *	@retval ０以上 サイズ
	 *	@retval マイナス 取得失敗
	 */
	virtual LONGLONG GetSize(void) const
	{
		LONGLONG c = Seek(0, CURRENT);
		LONGLONG r = Seek(0, END);
		Seek(c, TOP);
		return r;
	}

	/**
	 * [操作] シーク.
	 *		アクセスポインタを移動します。
	 *	@param llOffset アクセスポインタの移動数。
	 *	@param eSeek 移動開始アクセスポインタ位置。省略すると TOP になります。
	 *	@retval ０以上	シーク成功。数値は、現在のファイルポインタ
	 *	@retval マイナス シーク失敗
	 */
	virtual LONGLONG Seek(LONGLONG llOffset, ESeekMode eSeek = TOP) const
	{
		 return ::LZSeek(m_iHandle, ToInt(llOffset), eSeek);
	}

	 /**
	 * [取得] 読み込み
	 *	@param[in] size 読み込みサイズ。
	 *	@param[out] _P 読み込みアドレス。 iSize 分書き込める領域である必要が有ります。
	 *	@return 読み込めたサイズ。
	 *	@throw CReadFailureException 読み込みに失敗すると、スローされます。
	 */
	virtual size_t Read(size_t size, LPVOID _P) const
	{
		INT r = ::LZRead(m_iHandle, static_cast<char *>(_P), ToInt(size));
		if ( r < 0 )
		{
			throw CReadFailureException(ERROR_ACCESS_DENIED);
		}
		return r;
	}

	/**
	 * [取得] 圧縮前名前取得
	 *	@param lpszLzFile Compress.exeで圧縮されたファイル名か、通常のファイル名。
	 *	@retval Empty文字列 エラー。
	 *	@retval 文字あり ファイル名。
	 */
	static CStr GetExpandedName(LPCTSTR lpszLzFile)
	{
		CStr strSrc = lpszLzFile;
		CStr strDst;
		if ( ::GetExpandedName(strSrc.GetBuffer(), strDst.GetBuffer(MAX_PATH)) >= 0 )
		{
			strDst.ReleaseBuffer();
			return strDst;
		}
		return CStr();
	}
};



}; // TNB
