#pragma once
/**
 *@file
 * キュー型情報管理関係のヘッダ
 *
 *		情報をFIFO(First In First Out)で管理するクラスです。 
 *		Add系メソッドで追加し、Take系メソッドで古いのほうから情報を取り出します。
 *		また、Get,Set メソッドによる、インデックスによりアクセスも可能です。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * 簡易リング式キュー型情報管理テンプレート
 *	
 *	@note キューの大きさは固定です。最初にすべてのインスタンスが作られます
 *			(破棄されるのは、デストラクタの時です)。
 *			そのためメモリの確保開放が発生しません。
 *
 *	@param TYP 保持する型（コピー可能の型に限る）
 *	@param ISI	trueならGet/Eraseした時に、不必要になったバッファの部分を
 *				INV を代入して初期化します。
 *				TYPがクラスでメモリを確保しているときなどにtrueにします。
 *	@param INV	 初期化時に使う値。省略すると、TYP()が使われます。
 *
 *	@par必要ファイル
 *			TnbSimpleQueue.h
 * 
 *	@date 13/08/29 新規作成
 */
template<typename TYP, bool ISI = false, class INV = TYP>
class CSimpleQueueT : CCopyImpossible
{
public:

	/**
	 * コンストラクタ
	 *	@note	コンストラクタ時に、リングキューの最大サイズを指定します。
	 *			動的に大きくなりません。
	 *	@param size リングキューのサイズ。省略すると10000です。
	 */
	explicit CSimpleQueueT(size_t size = 10000) : m_bufferSize(size), m_pBuffer(new TYP[size]), m_dataSize(0), m_readIndex(0)
	{
		ASSERT( size != 0 );
	}
	
	/**
	 * デストラクタ
	 */
	virtual ~CSimpleQueueT(void)
	{
		delete[] m_pBuffer;
	}

	/**
	 * [設定] リングキューサイズ指定.
	 *	@note コンストラクタでも指定できます。
	 *	@note このメソッドを使うと、情報はクリアされます。
	 *	@param size リングキューのサイズ。
	 */
	void SetQueueSize(size_t size)
	{
		ASSERT( size != 0 );
		delete[] m_pBuffer;
		m_bufferSize = size;
		m_pBuffer = new TYP[size];
		m_dataSize = 0;
		m_readIndex = 0;
	}

	/**
	 * [取得] リングキューサイズ取得.
	 *	@return リングキューのサイズ。
	 */
	size_t GetQueueSize(void) const
	{
		return m_bufferSize;
	}

	/**
	 * [取得] 要素数取得
	 *	@return サイズ
	 */
	size_t GetSize(void) const 
	{
		return m_dataSize;
	}

	/**
	 * [確認] 満タン？
	 *	@retval true キューは満タン。これ以上追加できない状態。
	 *	@retval false 否。
	 */
	bool IsFull(void) const
	{
		if ( GetQueueSize() <= GetSize() )
		{
			return true;
		}
		return false;
	}

	/**
	 * [追加] 要素追加.
	 *		要素一つをキューの最後に追加する。
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 AddEx() を使用してください。
	 *	@param t 追加する要素。
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval 上記以外 追加したINDEX。
	 */
	INDEX Add(const TYP& t)
	{
		size_t r = GetSize();
		return (AddElements(1, &t) == 1) ? r : INVALID_INDEX;
	}

	/**
	 * [削除] データ全削除.
	 *		キューの内容を空にします。
	 *	@return 常にtrue
	 */
	bool RemoveAll(void)
	{
		SetQueueSize(m_bufferSize);
		return true;
	}

	/**
	 * [追加] 複数要素追加.
	 *		連続したデータをキューの最後に追加します。
	 *	@attention	指定したデータ数、全部入る空きがないとエラーになります。
	 *	@param size 追加するデータの数
	 *	@param P 追加するデータ列のポインタ。省略すると TYP()が代入されます。
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		追加した数。
	 */
	size_t AddElements(size_t size, const TYP* P = NULL)
	{
		if ( (m_dataSize) + size > (m_bufferSize) )
		{
			//それ以上になるならエラー
			return INVALID_SIZE;
		}
		size_t bs = m_bufferSize;
		INDEX tmp = (m_readIndex + m_dataSize) % bs;
		if ( P != NULL )
		{
			loop ( i, size )
			{
				m_pBuffer[tmp++] = *P++;
				if ( tmp >= bs )
				{
					tmp -= bs;
				}
			}
		}
		else
		{
			loop ( i, size )
			{
				m_pBuffer[tmp++] = TYP();
				if ( tmp >= bs )
				{
					tmp -= bs;
				}
			}
		}
		m_dataSize += size;
		return size;
	}

	/**
	 * [取得] 先頭要素取り出し.
	 *		キューから削除します
	 *	@return 要素内容
	 *	@throw CEmptyException 空であることを示すためにスローされます。
	 */
	TYP Take(void)
	{
		TYP t;
		if ( TakeElements(1, &t) == 1 )
		{
			return t;
		}
		ASSERT0( false, "CSimpleQueueT::Take()", "キューが空の時取得操作されました。" );
		throw CEmptyException();
	}

	/**
	 * [取得] 複数要素取り出し.
	 *		指定バッファにコピーし、キューから削除します。
	 *	@attention	指定数分、データ無いとエラーになります。
	 *	@param[in]	size	取り出す数。
	 *	@param[out]	P		取り出すバッファ。 NULLなら取得しません。
	 *	@retval INVALID_SIZE	エラー。
	 *	@retval それ以外		取り出した数。
	 */
	size_t TakeElements(size_t size, TYP* P = NULL)
	{
		if ( m_dataSize < size )
		{
			//キューにその長さの情報がないのでエラー
			return INVALID_SIZE;
		}
		size_t bs = m_bufferSize;
		if ( P != NULL || ISI )
		{
			INDEX tmp = m_readIndex;
			loop ( i, size )
			{
				if ( P != NULL )
				{
					*P++ = m_pBuffer[tmp];
				}
				if ( ISI )
				{
					m_pBuffer[tmp] = INV();
				}
				if ( ++tmp >= bs )
				{
					tmp -= bs;
				}
			}
		}
		m_readIndex += size;
		m_readIndex %= bs;
		m_dataSize -= size;
		return size;
	}

	/**
	 * [取得] 要素参照
	 *	@param index インデックス
	 *	@return 参照
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	const TYP& Ref(INDEX index) const
	{
		if ( GetSize() > index )
		{
			INDEX tmp = m_readIndex + index;
			tmp %= m_bufferSize;
			return m_pBuffer[tmp];
		}
		throw CIndexOutOfBoundsException();
	}

	/**
	 * [取得] 要素アドレス取得
	 *	@param index インデックス
	 *	@return 参照
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	TYP& Ref(INDEX index)
	{
		if ( GetSize() > index )
		{
			INDEX tmp = m_readIndex + index;
			tmp %= m_bufferSize;
			return m_pBuffer[tmp];
		}
		throw CIndexOutOfBoundsException();
	}

	/**
	 * [取得] 格納可能数取得.
	 *	@note Ringキューはリングキューのサイズが固定です。
	 *	@return サイズ
	 */
	size_t GetFreeSize(void) const
	{
		return (m_bufferSize) - (m_dataSize);
	}

private:

	size_t			m_bufferSize;		///< バッファの長さ
	size_t			m_dataSize;		///< 実際のデータの長さ
	INDEX			m_readIndex;		///< 読込みポジション
	TYP*			m_pBuffer;		///< バッファ
};



}; // TNB
