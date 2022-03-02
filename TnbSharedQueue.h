#pragma once
/**
 *@file
 * 共有メモリキュー型情報管理関係のヘッダ
 *
 *		情報をFIFO(First In First Out)で管理するクラスです。 
 *		情報は、共有メモリに保管するため、プロセス間でも、情報の共有が出来ます。
 *		ただし、扱える情報は基本型、及び、基本型のみ扱う構造体のみとなります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbQueue.h"
#include "TnbSharedMemory.h"
#include "TnbSync.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * 共有キュー型情報管理テンプレート
 *	
 *		情報をFIFO(First In First Out)で管理するクラスです。 
 *		情報は、共有メモリに保管するため、プロセス間でも、情報の共有が出来ます。
 *
 *	@note キューの大きさは固定です。最初にすべてのインスタンスが作られます
 *
 *	@param TYP 保持する型（基本型、あるいは、基本型のみの構造体に限る）
 *	@param SIZ キューの数。省略すると 10000 です。
 *
 *	@see	IConstCollectionT を実装しています。
 *	@see	ICollectionT を実装しています。
 *	@see	IQueueT を実装しています。
 *
 *	@par必要ファイル
 *			TnbSharedQueue.h
 * 
 *	@date 06/09/12 新規作成
 *	@date 07/03/15 index,size管理を unsignedに変更。
 */
template<typename TYP, size_t SIZ=10000>
class CSharedQueueT : public ICollectionMidT<TYP> , public IQueueT<TYP> , CCopyImpossible
{
	DEFSUPER(ICollectionMidT<TYP>);

	/// 配列型情報管理の管理型
	struct TInfo
	{
		size_t	bufferSize;		///< バッファの長さ
		size_t	dataSize;		///< 実際のデータの長さ
		INDEX	readIndex;		///< 読込みポジション
		INDEX	addIndex;		///< 追加ポジション
		TYP		atData[SIZ];	///< データ
	};

	CSharedStructT<TInfo>	m_ssInfo;		///< データ共有メモリ
	CSyncMutex *			m_psyncData;	///< データ排他用

	/// 共有メモリオープン時の処理クラス
	class CSmOpenListener : public CSharedStructT<TInfo>::IListener
	{
	public:
		virtual void OnAction(TInfo* P)
		{
			P->bufferSize	= SIZ;
			P->dataSize	= 0;
			P->readIndex		= 0;
			P->addIndex		= 0;
		}
	};

protected:

	/**
	 * [取得] 要素アドレス取得
	 *	@param index インデックス
	 *	@retval NULL		失敗（インデックス範囲外）
	 *	@retval NULL以外	アドレス
	 */
	virtual const TYP* m_GetConstPointer(INDEX index) const
	{
		if ( m_psyncData == NULL ){ return NULL; }
		if ( ! IsInRange(index) ){ return NULL; }
		INDEX tmp = (m_ssInfo->readIndex) + index;
		tmp %= (m_ssInfo->bufferSize);
		return &(m_ssInfo->atData[tmp]);
	}

	/**
	 * [取得] 要素アドレス取得
	 *	@param index インデックス
	 *	@retval NULL		失敗（インデックス範囲外）
	 *	@retval NULL以外	アドレス
	 */
	virtual TYP* m_GetPointer(INDEX index)
	{
		if ( m_psyncData == NULL ){ return NULL; }
		if ( ! IsInRange(index) ){ return NULL; }
		INDEX tmp = (m_ssInfo->readIndex) + index;
		tmp %= (m_ssInfo->bufferSize);
		return &(m_ssInfo->atData[tmp]);
	}

public:

	/**
	 * コンストラクタ
	 *	@param boCanClose falseならクローズ時共有メモリを閉じない。trueか省略なら閉じる。
	 *	@see CSharedStructT::CSharedStructT
	 */
	CSharedQueueT(bool boCanClose = true) : _super(), m_ssInfo(boCanClose)
	{
		m_psyncData = NULL;
	}

	/**
	 * デストラクタ
	 */
	virtual ~CSharedQueueT(void)
	{
		Close();
	}

	/**
	 * [設定] オープン
	 *	@note このメソッドを行なって初めてコレクションとして使用できます。
	 *	@param lpszName 共有名。TYPとSIZが異なるクラスのインスタンスで同じ共有名は使用しないようにしてください。
	 *							同じ型のインスタンスでも異なる共有名にすると別のキューになります。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Open(LPCTSTR lpszName)
	{
		Close();
		CSmOpenListener l;
		if ( m_ssInfo.Open(lpszName,&l) )
		{
			m_psyncData = new CSyncMutex(false, CStr::Fmt(_T("%s_Sync"), lpszName));
			if ( m_psyncData == NULL )
			{
				m_ssInfo.Close();
			}
		}
		return m_psyncData != NULL;
	}

	/**
	 * [設定] クローズ
	 *		このメソッドを行なうとコレクションとして使用できなくなります。
	 *	@note 通常使用する必要はありません。
	 */
	void Close(void)
	{
		m_ssInfo.Close();
		if ( m_psyncData != NULL )
		{
			delete m_psyncData;
			m_psyncData = NULL;
		}
	}

	/** 
	 * [排他] ロック
	 *	@note	Lock/Unlockは情報の排他のために使用します。スレッドセーブに
	 *			するために、一連の情報操作前にLockし操作終了後にUnlockする必要が
	 *			あります（単発の操作時は大丈夫です）。
	 *	@param dwTime ダミーです。省略してください。
	 *	@return 常にtrue。
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const
	{
		if ( m_psyncData == NULL ){ return false; }
		return m_psyncData->Lock();
	}

	/** [排他] アンロック */
	virtual void Unlock(void) const
	{
		if ( m_psyncData == NULL ){ return; }
		m_psyncData->Unlock();
	}

	/**
	 * [取得] 要素数取得
	 *	@return サイズ
	 */
	virtual size_t GetSize(void) const 
	{
		if ( m_psyncData == NULL ){ return 0; }
		return m_ssInfo->dataSize;
	}

	/**
	 * [追加] 要素追加.
	 *		要素一つをキューの最後に追加する。
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 AddEx() を使用してください。
	 *	@param t 追加する要素。
	 *	@retval INVALID_INDEX 失敗。（バッファ一杯）。
	 *	@retval 上記以外 追加したINDEX。
	 */
	virtual INDEX Add(const TYP& t)
	{
		size_t r = GetSize();
		return (AddElements(1, &t) == 1) ? r : INVALID_INDEX;
	}

	/**
	 * [削除] データ全削除.
	 *		キューの内容を空にします。
	 *	@return 常にtrue
	 */
	virtual bool RemoveAll(void)
	{
		if ( m_psyncData == NULL ){ return true; }
		return TakeElements(m_ssInfo->dataSize) != INVALID_SIZE;
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
	virtual size_t AddElements(size_t size, const TYP* P = NULL)
	{
		if ( m_psyncData == NULL )
		{
			return INVALID_SIZE; 
		}
		EXCLUSIVE(m_psyncData);
		if ( (m_ssInfo->dataSize) + size > (m_ssInfo->bufferSize) )
		{
			return INVALID_SIZE;	//それ以上になるならエラー
		}
		size_t bs	= m_ssInfo->bufferSize;
		INDEX tmp	= m_ssInfo->addIndex;
		if ( P != NULL )
		{
			loop ( i, size )
			{
				m_ssInfo->atData[tmp++] = *P++;
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
				m_ssInfo->atData[tmp++] = TYP();
				if ( tmp >= bs )
				{
					tmp -= bs;
				}
			}
		}
		(m_ssInfo->addIndex) += size;
		(m_ssInfo->addIndex) %= bs;
		(m_ssInfo->dataSize) += size;
		return size;
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
	virtual size_t TakeElements(size_t size, TYP* P = NULL)
	{
		if ( m_psyncData == NULL )
		{
			return INVALID_SIZE;
		}
		EXCLUSIVE(m_psyncData);
		if ( (m_ssInfo->dataSize) < size )
		{
			return INVALID_SIZE;	//キューにその長さの情報がないのでエラー
		}
		size_t bs = m_ssInfo->bufferSize;
		if ( P != NULL )
		{
			INDEX tmp = m_ssInfo->readIndex;
			loop ( i, size )
			{
				*P++ = m_ssInfo->atData[tmp];
				if ( ++tmp >= bs )
				{
					tmp -= bs;
				}
			}
		}
		(m_ssInfo->readIndex) += size;
		(m_ssInfo->readIndex) %= bs;
		(m_ssInfo->dataSize) -= size;
		return size;
	}

	/**
	 * [取得] 格納可能数取得.
	 *	@note Ringキューはリングキューのサイズが固定です。
	 *	@return サイズ
	 */
	size_t GetFreeSize(void) const
	{
		if ( m_psyncData == NULL )
		{
			return 0;
		}
		return (m_ssInfo->bufferSize) - (m_ssInfo->dataSize);
	}

private:
	friend class CSharedQueueTest;///< フレンドクラス宣言
};



}; // TNB
