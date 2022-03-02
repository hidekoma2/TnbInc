#pragma once
/**
 *@file
 * ファイル関係のヘッダ
 *
 *		 ファイル風メモリ管理クラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbWriter.h"



//TNB Library
namespace TNB
{


/**@ingroup FILE MEMORY
 * ファイル風メモリ管理クラス
 *
 *	 メモリをファイルに見立てる
 *
 *	@par必要ファイル
 *			TnbLikenMemToFile.h
 *
 *	@date 06/10/06 新規作成
 */
class CLikenMemToFile : public IWriter, CCopyImpossible
{
	const BYTE*					m_pbcConstPtr;		///< 外部メモリ参照用
	BYTE*						m_pbPtr;			///< 外部メモリアクセス用
	LONGLONG					m_llSize;			///< 外部メモリサイズ
	mutable CAutoVectorT<BYTE>	m_vbContent;		///< 可変有効用バッファ
	mutable LONGLONG			m_llPos;			///< アクセスポインタ

public:

	/**
	 * コンストラクタ.
	 *	@note	指定サイズの可変可能なオブジェクトを作成します。
	 *	@param size サイズ。
	 */
	CLikenMemToFile(size_t size = 0)
		: m_pbcConstPtr(NULL)
		, m_pbPtr(NULL)
		, m_llSize(size)
		, m_llPos(0)
	{
		m_vbContent.SetSize(size);
	}

	/**
	 * コンストラクタ.
	 *	@note	指定データの、読み込み専用のオブジェクトを作成します。
	 *	@param size データサイズ。
	 *	@param P データ内容。
	 */
	CLikenMemToFile(size_t size, LPCVOID P)
		: m_pbcConstPtr(static_cast<const BYTE*>(P))
		, m_pbPtr(NULL)
		, m_llSize(size)
		, m_llPos(0)
	{
	}

	/**
	 * コンストラクタ.
	 *	@note	指定データの変更可能のオブジェクトを作成します。
	 *			変更は出来ません。
	 *	@param size データサイズ。
	 *	@param P データ内容。
	 */
	CLikenMemToFile(size_t size, LPVOID P)
		: m_pbcConstPtr(NULL)
		, m_pbPtr(static_cast<BYTE*>(P))
		, m_llSize(size)
		, m_llPos(0)
	{
	}

	/**
	 * [取得] オープン名取得
	 *	@return 名前
	 */
	virtual LPCTSTR GetOpenName(void) const { return _T("-"); }

	/**
	 * [操作] クローズ
	 */
	virtual void Close(void) {}

	/**
	 * [確認] 読み込み可能か
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	virtual bool CanRead(void) const
	{
		return true;
	}

	/**
	 * [確認] 書込み可能か
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	virtual bool CanWrite(void) const
	{
		return m_pbcConstPtr == NULL;
	}

	/**
	 * [取得] サイズ取得
	 *	@retval ０以上 サイズ
	 *	@retval マイナス 取得失敗
	 */
	virtual LONGLONG GetSize(void) const
	{
		if ( m_pbcConstPtr != NULL || m_pbPtr != NULL )
		{
			return m_llSize;
		}
		return m_vbContent.GetSize();
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
		LONGLONG l = GetSize();
		LONGLONG p;
		switch ( eSeek )
		{
		case TOP:
			p = llOffset;
			break;
		case END:
			p = l + llOffset;
			break;
		default:
			p = m_llPos + llOffset;
			break;
		}
		if ( p < 0 )
		{
			return -1;
		}
		else if ( p > l )
		{
			if ( m_pbcConstPtr == NULL && m_pbPtr == NULL )
			{
				m_vbContent.SetSize(ToInt(p));
			}
			else
			{
				return -1;
			}
		}
		m_llPos = p;
		return p;
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
		const BYTE* B = NULL;
		if ( m_pbcConstPtr != NULL )
		{
			B = m_pbcConstPtr;
		}
		else if ( m_pbPtr != NULL )
		{
			B = m_pbPtr;
		}
		else
		{
			B = m_vbContent.ReferBuffer();
		}
		LONGLONG s = size;
		if ( s > GetLeftoverSize() )
		{
			s = GetLeftoverSize();
		}
		BYTE* Q = static_cast<BYTE*>(_P);
		MemCopy(Q, &B[m_llPos], ToInt(s));
		m_llPos += s;
		return ToInt(s);
	}

	/**
	 * [保存] 書き込み
	 *	@param size 書き込みサイズ。
	 *	@param P 書き込みデータ。
	 *	@throw CWriteFailureException コンストラクタ throw を有効にしていて、書込みに失敗すると、スローされます。
	 */
	virtual void Write(size_t size, LPCVOID P)
	{
		if ( m_pbcConstPtr != NULL )
		{
			throw CWriteFailureException(ERROR_ACCESS_DENIED);
		}
		if ( m_pbPtr != NULL )
		{
			if ( static_cast<LONGLONG>(size) >= GetLeftoverSize() )
			{
				throw CWriteFailureException(ERROR_ACCESS_DENIED);
			}
			MemCopy(&m_pbPtr[m_llPos], static_cast<const BYTE*>(P), size);
		}
		else
		{
			if ( m_llPos + size > m_vbContent.GetSize() )
			{
				m_vbContent.SetSize(ToInt(m_llPos + size));
			}
			const BYTE* B = static_cast<const BYTE*>(P);
			loop ( i, size )
			{
				m_vbContent.Set(ToInt(m_llPos + i), *B++);
			}
		}
		m_llPos += size;
	}

	/**
	 * [設定] EOFを指定.
	 *		現在のファイルポインタの位置をEOFにします。
	 *	@throw CTnbException 書込みに失敗すると、スローされます。
	 */
	virtual void SetEnd(void)
	{
		if ( m_pbcConstPtr != NULL && m_pbPtr != NULL )
		{
			throw CWriteFailureException(ERROR_ACCESS_DENIED);
		}
		m_vbContent.SetSize(ToInt(m_llPos));
	}
};



}; // TNB


