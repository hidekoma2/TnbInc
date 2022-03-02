#pragma once
/**
 *@file
 * ファイル関係のヘッダ
 *
 *		キャッシュ付き、読み込みクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbReader.h"
#include "TnbQueue.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * キャッシュ読み込みクラス
 *
 *	@note 指定の IReader から読み込みます。その際、先行読み出し、仮読み出しのサポートを行います。
 *
 *	@par必要ファイル
 *			TnbCacheReader.h
 *
 *	@date 10/02/03 新規作成
 */
class CCacheReader : public IReader, CCopyImpposible
{
public:

	CCacheReader(void) : m_pReader(NULL), m_minimum(100)
	{
	}

	void SetEnvironmrent(IReport* pRep, size_t minimum = 100)
	{
		m_pReader = pRep;
		m_minimum = minimum;
	}

	/**
	 * [取得] オープン名取得
	 *	@return 名前
	 */
	virtual LPCTSTR GetOpenName(void) const
	{
		return (m_pReader != NULL) ? m_pReport->GetOpenName() : _T("");
	}

	/**
	 * [操作] クローズ
	 */
	virtual void Close(void)
	{
		if ( m_pReader != NULL )
		{
			m_pReader->Close();
		}
	}

	/**
	 * [確認] 読み込み可能か
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	virtual bool CanRead(void) const
	{
		return (m_pReader != NULL) ? m_pReport->CanRead() : false;
	}

	/**
	 * [取得] サイズ取得
	 *	@retval ０以上 サイズ
	 *	@retval マイナス 取得失敗
	 */
	virtual LONGLONG GetSize(void) const
	{
		return (m_pReader != NULL) ? m_pReport->GetSize() : 0;
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
		return (m_pReader != NULL) ? m_pReport->Seek(llOffset, eSeek) : -1;
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
		return 0;
	}

	/**
	 * [取得] ためし読み
	 *	@note ファイルポインタは変化しません。
	 *	@param size 読み込みサイズ。
	 *	@param offset 読み込み開始アクセスポインタ位置
	 *	@return ためし読みしたデータ。
	 *	@throw CReadFailureException 読み込みに失敗すると、スローされます。
	 */
	const IConstCollectionT<BYTE>& Peek(size_t size = 0, LONGLONG offset = 0) const
	{
	}





private:
	size_t							m_minimum;	///< 読み込み最小長さ
	IReader*						m_pReader;	///< 対象のリーダー
	LONGLONG						m_pointer;	///< 先読みバッファのファイルポインタ
	mutable CVectorT<BYTE>			m_buffer;	///< 先読みのバッファ
};

