#pragma once
/**
 *@file
 * 配列型bit情報管理関係のヘッダ
 *
 *		動的に配列を増やすことができます(メモリの再配置が発生する可能性があります)。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbCollection.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT ITERATOR
 * 配列型bit情報管理テンプレート
 *
 *		動的に配列を増やすことができます(メモリの再配置が発生する可能性があります)。
 *	
 *	@see	IConstCollectionT を実装しています。
 *	@see	ICollectionT を実装しています。
 *
 *	@note ICollectionT の TYP は bool になります。
 *
 *	@par必要ファイル
 *			TnbBitVector.h
 *
 *	@date 06/09/01 新規
 *	@date 07/03/15 index,size管理を unsignedに変更。
 */
class CBitVector : public ICollectionT<bool>
{
	DEFSUPER(ICollectionT<bool>);
	CVectorT<BYTE>			m_vbData;		///< 管理用データ
	size_t					m_dataSize;		///< 実際のデータサイズ
	CSyncSection			m_syncFunc;		///< 参照排他
	mutable bool			m_boTemp;		///< 参照用

	/** 
	 * 指定位置から指定数までを右シフトします。あいたところは、０になります。
	 *	ex) 指定位置 4
	 *		0 1001 0110 1110
	 *			↓
	 *		0 0100 1011 1110
	 *	@param startIndex 開始のINDEX
	 *	@param B データ
	 *	@param len データBYTE数
	 */
	static void ms_ShiftRight(INDEX startIndex, BYTE* B, size_t len)
	{
		bool boLastBit = false;
		BYTE bTop = B[startIndex / 8];
		for ( INDEX i = len - 1; i >= startIndex / 8; i-- )
		{
			bool b = (B[i] & _BIT(0)) != 0;
			B[i] >>= 1;
			if ( boLastBit )
			{
				B[i] |= _BIT(7);
			}
			boLastBit = b;
		}
		INDEX pos = startIndex & 7;
		if ( pos != 0 )
		{
			BYTE bMask = static_cast<BYTE>(_BIT(pos) - 1);
			bTop &= bMask;
			B[startIndex / 8] &= ~bMask;
			B[startIndex / 8] |= bTop;
		}
	}

	/** 
	 * 指定位置から指定数までを左シフトします。あいたところは、０になります。
	 *	ex) 指定位置 8
	 *		0 1001 0110 1110
	 *			↓
	 *		1 0010 1100 1110
	 *	@param startIndex 開始のINDEX
	 *	@param B データ
	 *	@param len データBYTE数
	 */
	static void ms_ShiftLeft(INDEX startIndex, BYTE* B, size_t len)
	{
		bool boLastBit = false;
		BYTE bTop = B[startIndex / 8];
		for ( INDEX i = startIndex / 8; i < len; i++ )
		{
			bool b = (B[i] & _BIT(7)) != 0;
			B[i] <<= 1;
			if ( boLastBit )
			{
				B[i] |= _BIT(0);
			}
			boLastBit = b;
		}
		INDEX pos = startIndex & 7;
		if ( pos != 0 )
		{
			BYTE bMask = static_cast<BYTE>(_BIT(pos) - 1);
			bTop &= bMask;
			B[startIndex / 8] &= ~bMask;
			B[startIndex / 8] |= bTop;
		}
	}

public:

	/**@ingroup COLLECT ITERATOR
	 * 配列型bit情報管理の参照クラス
	 *
	 *		BITを操作するためのクラスです。
	 *
	 *	@note	CBitVectorで作成したものを短命で使用することを想定しているため、
	 *			コンストラクタは private 宣言しています。
	 *	
	 *	@see CBitVector::operator[]()
	 *
	 *	@date 06/09/01	新規作成
	 */
	class CRef
	{
		friend class CBitVector;
		CBitVector* m_vb;
		INDEX		m_index;
		/// コンストラクタ
		CRef(const CBitVector* P, INDEX i)
		{
			m_vb = const_cast<CBitVector*>(P);
			m_index = i;
		}
	public:
		/**
		 * [取得] データ取得
		 *	@return データ。
		 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
		 *	@throw CReadFailureException ファイル読み込みに失敗したことを示すためにスローされます。
		 */
		operator bool(void) const
		{
			return m_vb->At(m_index);
		}
		/**
		 * [設定] データ設定
		 *	@param b 書き込むデータ。
		 *	@return 書き込んだデータ。
		 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
		 *	@throw CWriteFailureException ファイル書き込みに失敗したことを示すためにスローされます。
		 */
		bool operator=(bool b)
		{
			m_vb->Set(m_index, b);
			return b;
		}
	};

	
	//--------------------------------------


	#ifndef _TnbDOXYGEN	//Document作成用シンボル
	// const_iterator型宣言
	typedef CConstIteratorBaseT<CBitVector, bool> const_iterator;
	// iterator宣言
	class iterator
	{
		_ITERATORCORE(iterator, CBitVector)
	public:
		CRef operator*(void)
		{
			return (*m_V)[m_ind];
		}
		CRef operator[](int i)
		{
			return (*m_V)[m_ind+i];
		}
	};
	#endif //_TnbDOXYEM

	/**
	 * [反復] 先頭const_iterator.
	 *		先頭要素を指す{@link ITERATOR 反復子}を取得
	 *	@return const_iterator
	 */
	const_iterator begin(void) const { return const_iterator(this, 0); }

	/**
	 * [反復] 最後const_iterator.
	 *		最後要素の次を指す{@link ITERATOR 反復子}を取得
	 *	@return const_iterator
	 */
	const_iterator end(void) const { return const_iterator(this, GetSize()); }

	/**
	 * [反復] 先頭iterator.
	 *		先頭要素を指す{@link ITERATOR 反復子}を取得
	 *	@return iterator
	 */
	iterator begin(void) { return iterator(this, 0); }

	/**
	 * [反復] 最後iterator.
	 *		最後要素の次を指す{@link ITERATOR 反復子}を取得
	 *	@return iterator
	 */
	iterator end(void) { return iterator(this, GetSize()); }

	/**
	 * [反復] 挿入
	 *	@param ite 挿入する位置の iterator
	 *	@param t 要素
	 *	@return 挿入した場所の iterator
	 */
	iterator insert(iterator ite, bool t = false)
	{
		INDEX i = ite.GetIndex();
		Insert(i, t);
		return iterator(this, i);
	}

	/**
	 * [反復] 先頭に挿入
	 *	@param t 要素
	 */
	void push_front(bool t) { Insert(0, t); }


	//------------------------


	/**
	 * コンストラクタ
	 *	@note 指定のメモリ確保時の追加サイズを設定します。
	 *	@note データは保持しません
	 *	@param size メモリ確保時の追加サイズ。 SetIncrementSize() と同じです。
	 */
	explicit CBitVector(size_t size = 0) : _super(), m_vbData(size) 
	{
		m_dataSize = 0;
	}

	/**
	 * コピーコンストラクタ
	 *	@note コピー元とバッファを共有します。
	 *	@param other コピー元の CVectorT インスタンス
	 */
	CBitVector(const CBitVector& other) : _super(), m_vbData(other.m_vbData)
	{
		m_dataSize = other.m_dataSize;
	}
	
	/** デストラクタ */
	virtual ~CBitVector(void)
	{
	}

	/**
	 * [代入] コピーオペレータ
	 *	@note 自インスタンスで管理しているバッファを開放し、	
	 *		代入元の情報を共有するようになります。
	 *	@param other 代入元 
	 *	@return 自分の参照
	 */
	CBitVector& operator=(const CBitVector& other)
	{
		m_vbData = other.m_vbData;
		m_dataSize = other.m_dataSize;
		return *this;
	}

	/**
	 * [設定] 余白サイズ
	 *	@note 新規にメモリを確保したとき、必要なサイズよりいくつ多く
	 *		確保しておくか、という値を設定できます。
	 *	@note 通常は自動です。
	 *	@note コピーではこの値は引き継がれません。
	 *	@param size メモリ確保時の追加サイズ。マイナスなら自動です。
	 */
	void SetIncrementSize(size_t size)
	{
		m_vbData.SetIncrementSize(size);
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
		return m_syncFunc.Lock();
	}

	/** [排他] アンロック */
	virtual void Unlock(void) const
	{
		m_syncFunc.Unlock();
	}

	/**
	 * [取得] サイズ取得
	 *	@return 本インスタンスが管理しているデータの長さ。
	 */
	virtual size_t GetSize(void) const
	{
		return m_dataSize;
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 一時的な参照
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual const bool& At(INDEX index) const
	{
		if ( ! IsInRange(index) )
		{
			throw CIndexOutOfBoundsException();
		}
		m_boTemp = (m_vbData[index / 8] & _BIT(index & 7)) != 0;
		return m_boTemp;
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@attention サポートできません。 operator[] を使用します。
	 *	@param index インデックス
	 *	@return -
	 *	@throw CNotSupportException サポートしていないことを示すためにスローされます。
	 */
	virtual bool& Ref(INDEX index)
	{
		throw CNotSupportException();
	}

	/**
	 * [設定] 要素の設定.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 SetEx() を使用してください。
	 *	@param index インデックス
	 *	@param t 内容
	 *	@retval true	設定成功
	 *	@retval false	失敗
	 */
	virtual bool Set(INDEX index, const bool& t)
	{
		if ( ! IsInRange(index) )
		{
			return false;
		}
		if ( t )
		{
			m_vbData[index / 8] |= _BIT(index & 7);
		}
		else
		{
			m_vbData[index / 8] &= ~_BIT(index & 7);
		}
		return true;
	}

	/**
	 * [追加] 要素一つ追加.
	 *	@note 自インスタンスで管理しているバッファに指定の一データを連結します。
	 *	@param t 要素。
	 *	@return 追加したINDEX
	 */
	virtual INDEX Add(const bool& t)
	{
		if ( m_dataSize == 0 || (m_dataSize & 7) == 7 )
		{
			m_vbData.Add(0);
		}
		if ( t )
		{
			m_vbData[m_dataSize / 8] |= _BIT(m_dataSize & 7);
		}
		else
		{
			m_vbData[m_dataSize / 8] &= ~_BIT(m_dataSize & 7);
		}
		return m_dataSize++;
	}

	/**
	 * [削除] 要素一つ削除.
	 *	@param index インデックス
	 *	@retval true	削除成功
	 *	@retval false	失敗
	 */
	virtual bool Remove(INDEX index)
	{
		if ( ! IsInRange(index) )
		{
			return false;
		}
		m_dataSize--;
		ms_ShiftRight(index, m_vbData.GetBuffer(), m_vbData.GetSize());
		m_vbData.ReleaseBuffer();
		return true;
	}

	/**
	 * [削除] 空化
	 *	@note 自インスタンスで管理しているバッファを空にします。
	 *	@return 常にtrue。
	 */
	virtual bool RemoveAll(void)
	{
		m_vbData.RemoveAll();
		m_dataSize = 0;
		return true;
	}

	/**
	 * [追加] 要素一つ挿入
	 *	@note 指定のインデックスに要素を追加します。
	 *	@param index インデックス
	 *	@param t 要素
	 *	@retval true	挿入成功
	 *	@retval false	失敗（インデックス無効）
	 */
	bool Insert(INDEX index, const bool& t)
	{
		if ( index == m_dataSize )
		{
			return IsValid(Add(t));
		}
		if ( ! IsInRange(index) )
		{
			return false;
		}
		if ( m_dataSize == 0 || (m_dataSize & 7) == 7 )
		{
			m_vbData.Add(0);
		}
		ms_ShiftLeft(index, m_vbData.GetBuffer(), m_vbData.GetSize());
		m_vbData.ReleaseBuffer();
		if ( t )
		{
			m_vbData[index / 8] |= _BIT(index & 7);
		}
		else
		{
			m_vbData[index / 8] &= ~_BIT(index & 7);
		}
		m_dataSize++;
		return true;
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 参照。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	const CRef operator[](INDEX index) const
	{
		if ( ! IsInRange(index) )
		{
			throw CIndexOutOfBoundsException();
		}
		return CRef(this, index);
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 参照。操作するとオブジェクト内の要素に影響が有ります。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	CRef operator[](INDEX index)
	{
		if ( ! IsInRange(index) )
		{
			throw CIndexOutOfBoundsException();
		}
		return CRef(this, index);
	}

private:
	friend class CBitVectorTest;	///<フレンドクラス宣言
};



}; // TNB

