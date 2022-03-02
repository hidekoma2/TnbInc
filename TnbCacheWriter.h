#pragma once
/**
 *@file
 * キャッシュ書込み処理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbWriter.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * キャッシュ書き込みクラス
 *
 *	@note 読みこみはキャッシュしません。
 *
 *	\par 使い方:
 *	@code
 *		CStr s;
 *		CFileWriter fw;
 *		fw.New("NewFile.dat");
 *		CTickCount t1;
 *		try
 *		{
 *			CCacheWriter cw;
 *			cw.SetWriter(&fw, 1024 * 1024);	//キャッシュサイズは 1MB
 *			loop ( i, GetTextSize() )
 *			{
 *				s = GetText(i);
 *				fw.Write(s.GetLength(), s.operator LPCTSTR());
 *			}
 *		}
 *		catch ( CWriteFailureException& e )
 *		{
 *			e.OnCatch();
 *		}
 *	@endcode
 *
 *	@par必要ファイル
 *			TnbCacheWriter.h
 *
 *	@date 11/08/17 新規作成
 */
class CCacheWriter : public IWriter, CCopyImpossible
{
public:

	/// コンストラクタ 
	CCacheWriter(void) : m_pWriter(NULL)
	{
	}

	/// デストラクタ
	~CCacheWriter(void)
	{
		m_Push();
	}

	/**
	 * [設定] ライター設定
	 *	@param pfw オープンしてあるWriterを指定します。
	 *				Openしていないオブジェクトを渡すと失敗します。
	 *				pfw は参照しますので、使用中は破棄しないで下さい。
	 *	@param size キャッシュサイズ
	 *	@retval true 設定成功
	 *	@retval false 失敗
	 */
	bool SetWriter(IWriter* pfw, size_t size = 10000)
	{
		if ( ! pfw->CanWrite() )
		{
			return false;
		}
		m_pWriter = pfw;
		m_cache.Resize(size);
		m_cacheSize = 0;
		return true;
	}

	/**
	 * [取得] オープン名取得
	 *	@return 名前
	 */
	virtual LPCTSTR GetOpenName(void) const
	{
		m_Check();
		return m_pWriter->GetOpenName();
	}

	/**
	 * [操作] クローズ
	 */
	virtual void Close(void)
	{
		m_CheckAndPush();
		m_pWriter->Close();
	}

	/**
	 * [確認] 読み込み可能か
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	virtual bool CanRead(void) const
	{
		m_Check();
		return m_pWriter->CanRead();
	}

	/**
	 * [取得] サイズ取得
	 *	@retval ０以上 サイズ
	 *	@retval マイナス 取得失敗
	 */
	virtual LONGLONG GetSize(void) const
	{
		m_Check();
		return m_pWriter->GetSize();
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
		m_CheckAndPush();
		return m_pWriter->Seek(llOffset, eSeek);
	}

	/**
	 * [取得] 読み込み
	 *	@param[in] size 読み込みサイズ。
	 *	@param[out] _P 読み込みアドレス。 size 分書き込める領域である必要が有ります。
	 *	@return 読み込めたサイズ。
	 *	@throw CReadFailureException 読み込みに失敗すると、スローされます。
	 */
	virtual size_t Read(size_t size, LPVOID _P) const
	{
		m_CheckAndPush();
		return m_pWriter->Read(size, _P);
	}

	/**
	 * [確認] 書込み可能か
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	virtual bool CanWrite(void) const
	{
		m_Check();
		return m_pWriter->CanWrite();
	}

	/**
	 * [設定] EOFを指定.
	 *		現在のファイルポインタの位置をEOFにします。
	 *	@throw CTnbException 書込みに失敗すると、スローされます。
	 */
	virtual void SetEnd(void)
	{
		m_CheckAndPush();
		m_pWriter->SetEnd();
	}

	/**
	 * [保存] 書き込み
	 *	@param size 書き込みサイズ。
	 *	@param P 書き込みデータ。
	 *	@throw CWriteFailureException 書込みに失敗すると、スローされます。
	 */
	virtual void Write(size_t size, LPCVOID P)
	{
		m_Check();
		if ( m_cacheSize + size >= m_cache.GetSize() )
		{
			// キャッシュがあふれる
			m_Push();
			ASSERT( m_cacheSize == 0 );
			if ( size >= m_cache.GetSize() )
			{
				// キャッシュに入らない
				m_pWriter->Write(size, P);
			}
			else
			{
				memcpy(m_cache, P, size);
				m_cacheSize = size;
			}
		}
		else
		{
			memcpy(&m_cache[m_cacheSize], P, size);
			m_cacheSize += size;
		}
	}

private:
	// NULL チェック
	void m_Check(void) const
	{
		if ( m_pWriter == NULL )
		{
			throw CNullPointerException();
		}
	}
	// 追い出す
	void m_Push(void) const
	{
		if ( m_cacheSize > 0 )
		{
			m_pWriter->Write(m_cacheSize, m_cache);
			m_cacheSize = 0;
		}
	}
	// NULL チェック＆追い出す
	void m_CheckAndPush(void) const
	{
		m_Check();
		m_Push();
	}
	mutable size_t		m_cacheSize;		///< キャッシュしている数
	mutable CWorkMem	m_cache;			///< キャッシュ
	mutable IWriter*	m_pWriter;			///< ファイルクラス
};



}; // TNB
