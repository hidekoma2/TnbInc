#pragma once
/**
 *@file
 * ファイル関係のヘッダ
 *
 *		ファイルをコレクションとして操作するためのアダプタクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbFile.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE COLLECT ITERATOR
 * ファイル読み込みアダプタ
 *
 *		IReader を持つオブジェクト（ファイルなど）を一つのコレクションとしてアクセスするためのアダプタです。
 *
 *	@note 情報読み込み時 CIoFailureException が発生する可能性があります。
 *
 *	@note インスタンスの複製すると複製したすべてのインスタンスで同じファイルハンドルを
 *		使うようになります。すべてのインスタンスでCloseしないとファイルハンドルがクローズ
 *		されなくなります。
 *
 *	@see	IConstCollectionT<BYTE> を実装しています。
 *
 *	@note IConstCollectionT の TYP は BYTE になります。
 *
 *	@par必要ファイル
 *			TnbFileAdapter.h
 *
 *	@date 06/08/04	新規作成
 *	@date 06/08/13 イテレータを STL にあわせて、改修。
 *	@date 06/12/20 IReader を使用するように、改修。
 *	@date 07/03/15 index,size管理を unsignedに変更。
 *	@date 09/09/08 クラス名、 CConstFileAdapter → CReaderAdapter に変更。
 */
class CReaderAdapter : public IConstCollectionT<BYTE>
{
	DEFSUPER(IConstCollectionT<BYTE>);
public:

	/// コンストラクタ
	CReaderAdapter(void) : _super(), m_pReader(NULL), m_iTempNo(0)
	{
	}

	/** 
	 * [排他] ロック
	 *	@param dwTime ダミー。省略してください。
	 *	@return 常にtrue。
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const 
	{
		return true; 
	}

	/** [排他] アンロック */
	virtual void Unlock(void) const
	{
	}

	/**
	 * [操作] アタッチ.
	 *	@param pReader	オープンしている IReader。 デタッチするか、
	 *					本インスタンスをデストラクトするまで破棄しないで下さい。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Attach(const IReader* pReader)
	{
		Detach();
		if ( ! pReader->CanRead() )
		{
			return false;
		}
		m_pReader = pReader;
		return true;
	}

	/**
	 * [操作] デタッチ.
	 */
	void Detach(void)
	{
		if ( m_pReader == NULL )
		{
			m_pReader = NULL;
		}
	}

	/**
	 * [取得] 要素数取得.
	 *	@return 要素数
	 */
	virtual size_t GetSize(void) const
	{
		if ( m_pReader == NULL )
		{
			return 0;
		}
		return ToIndex(m_pReader->GetSize());
	}

	/**
	 * [取得] 要素の取得.
	 *	@param index インデックス
	 *	@return 内容
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 *	@throw CReadFailureException ファイル読み込みに失敗したことを示すためにスローされます。
	 */
	virtual BYTE Get(INDEX index) const
	{
		if ( m_pReader == NULL || ! IsInRange(index) )
		{
			throw CIndexOutOfBoundsException();
		}
		if ( m_pReader->Seek(index) >= 0 )
		{
			CByteVector vb = m_pReader->ReadExactly(1);
			if ( vb.IsValid() )
			{
				return vb[0];
			}
		}
		throw CReadFailureException(ERROR_ACCESS_DENIED);
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@attention 参照先はインスタンス内のワークのアドレスを指しています。
	 *			次の At() により先の内容が変化する可能性が有りますので、
	 *			必要に応じて値をコピーしておくようにしてください。
	 *	@param index インデックス
	 *	@return 一時的な参照
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 *	@throw CReadFailureException ファイル読み込みに失敗したことを示すためにスローされます。
	 */
	virtual const BYTE& At(INDEX index) const
	{
		m_iTempNo = (m_iTempNo + 1) & 0x0F;
		return m_abTemp[m_iTempNo] = Get(index);
	}

	/**
	 * [取得] 要素の取得.
	 *		Get() と同じ動作です。
	 *	@param index インデックス
	 *	@return 内容
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	BYTE operator[](INDEX index) const
	{
		return Get(index);
	}

private:
	const IReader*		m_pReader;		///< ファイル読み込みクラス	
	mutable BYTE		m_abTemp[16];	///< 参照用
	mutable INDEX		m_iTempNo;		///< 参照用Index
};



/**@ingroup FILE COLLECT ITERATOR
 * ファイル読み書きアダプタ
 *
 *		IWriter を持つオブジェクト（ファイルなど）を一つのコレクションとしてアクセスするためのアダプタです。
 *
 *	@note 情報読み込み時 CReadFailureException が発生する可能性があります。
 *
 *	@note インスタンスの複製すると複製したすべてのインスタンスで同じファイルハンドルを
 *		使うようになります。すべてのインスタンスでCloseしないとファイルハンドルがクローズ
 *		されなくなります。
 *
 *	@see	ICollectionT<BYTE> を実装しています。
 *
 *	@note ICollectionT の TYP は BYTE になります。
 *
 *	@par必要ファイル
 *			TnbFileAdapter.h
 *
 *	@date 06/08/04	新規作成
 *	@date 06/08/11 Element() を At() に改名。ReferElementPtr() を廃止。
 *	@date 06/08/13 イテレータを STL にあわせて、改修。
 *	@date 07/03/15 index,size管理を unsignedに変更。
 */
class CWriterAdapter : public ICollectionT<BYTE>
{
	DEFSUPER(ICollectionT<BYTE>);
public:

	/**@ingroup FILE COLLECT
	 * ファイル読み書きの参照クラス
	 *
	 *		ファイルの一バイトを操作するためのクラスです。
	 *
	 *	@note	CWriterAdapter で作成したものを短命で使用することを想定しているため、
	 *			コンストラクタは private 宣言しています。
	 *	
	 *	@see CWriterAdapter::operator[]()
	 *
	 *	@par使用例
	 *		<table><tr><td><pre>
	 *		
	 *		CWriterAdapter fa;
	 *		CFileWriter fw;
	 *		fw.New("c:\\temp\\aaa.dat");
	 *		fa.Attach(&fw);
	 *		fa.SetSize(10);
	 *		    ;
	 *		BYTE bb = fa[0];    // ファイルの 最初のデータを bb に読み込みます。		
	 *		fa[3] = 7;          // ファイルの 4Byte目に 7 を書き込みます。		 
	 *		fa[5] += 2;           // ファイルの 6Byte目のデータを +2 します。
	 *		</pre></td></tr></table>
	 *
	 *	@par必要ファイル
	 *			TnbFileAdapter.h
	 *
	 *	@date 06/08/11	新規作成
	 */
	class CRef
	{
		friend class CWriterAdapter;
		CWriterAdapter*	m_fa;
		INDEX			m_index;
		BYTE m_Set(BYTE b)
		{
			if ( ! m_fa->Set(m_index, b) )
			{
				throw CWriteFailureException(ERROR_ACCESS_DENIED);
			}
			return b;
		}
		BYTE m_Get(void) const
		{
			return m_fa->Get(m_index);
		}
		CRef(CWriterAdapter* P, INDEX i)
		{
			m_fa = P;
			m_index = i;
		}
	public:
		/**
		 * [取得] データ取得
		 *	@return データ。
		 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
		 *	@throw CReadFailureException ファイル読み込みに失敗したことを示すためにスローされます。
		 */
		operator BYTE(void) const
		{
			return m_Get();
		}
		/**
		 * [設定] データ設定
		 *	@param b 書き込むデータ。
		 *	@return 書き込んだデータ。
		 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
		 *	@throw CWriteFailureException ファイル書き込みに失敗したことを示すためにスローされます。
		 */
		BYTE operator=(BYTE b)
		{
			return m_Set(static_cast<BYTE>(b));
		}
		/**
		 * [計算] データ加算
		 *	@param b 加算するデータ。
		 *	@return 書き込んだデータ。
		 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
		 *	@throw CIoFailureException ファイル読み書きに失敗したことを示すためにスローされます。
		 */
		BYTE operator+=(BYTE b)
		{
			return m_Set(static_cast<BYTE>(m_Get() + b));
		}
		/**
		 * [取得] データ引算
		 *	@param b 引算するデータ。
		 *	@return 書き込んだデータ。
		 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
		 *	@throw CIoFailureException ファイル読み書きに失敗したことを示すためにスローされます。
		 */
		BYTE operator-=(BYTE b)
		{
			return m_Set(static_cast<BYTE>(m_Get() - b));
		}
	};


	//-----------------


	#ifndef _TnbDOXYGEN	//Document作成用シンボル
	// const_iterator型宣言
	typedef CConstIteratorBaseT<CWriterAdapter, BYTE> const_iterator;
	// iterator宣言
	class iterator
	{
		_ITERATORCORE(iterator, CWriterAdapter)
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


	//-----------------


	/// コンストラクタ
	CWriterAdapter(void) : _super(), m_pWriter(NULL), m_iTempNo(0)
	{
	}

	/** 
	 * [排他] ロック
	 *	@param dwTime ダミー。省略してください。
	 *	@return 常にtrue。
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const 
	{
		return true; 
	}

	/** [排他] アンロック */
	virtual void Unlock(void) const 
	{
	}

	/**
	 * [操作] アタッチ.
	 *	@param pWriter	オープンしている IWriter デタッチするか、
	 *					本インスタンスをデストラクトするまで破棄しないで下さい。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Attach(IWriter* pWriter)
	{
		Detach();
		if ( ! pWriter->CanWrite() )
		{
			return false;
		}
		m_pWriter = pWriter;
		return true;
	}

	/**
	 * [操作] デタッチ.
	 */
	void Detach(void)
	{
		if ( m_pWriter == NULL )
		{
			m_pWriter = NULL;
		}
	}

	/**
	 * [取得] 要素数取得.
	 *	@return 要素数
	 */
	virtual size_t GetSize(void) const
	{
		if ( m_pWriter == NULL )
		{
			return 0;
		}
		return ToIndex(m_pWriter->GetSize());
	}

	/**
	 * [設定] 要素数設定.
	 *	@note 大きくすることも小さくすることも出来ます。
	 *	@note 大きくした時、そのデータは 0 で埋められます。
	 *	@param size サイズ
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	virtual bool SetSize(size_t size)
	{
		if ( m_pWriter == NULL || m_pWriter->Seek(size) < 0 )
		{
			return false;
		}
		m_pWriter->SetEnd();
		return true;
	}

	/**
	 * [取得] 要素の取得.
	 *	@param index インデックス
	 *	@return 内容
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 *	@throw CReadFailureException ファイル読み込みに失敗したことを示すためにスローされます。
	 */
	virtual BYTE Get(INDEX index) const
	{
		if ( m_pWriter == NULL || ! IsInRange(index) )
		{
			throw CIndexOutOfBoundsException(); 
		}
		if ( m_pWriter->Seek(index) >= 0 )
		{
			CByteVector vb = m_pWriter->ReadExactly(1);
			if ( vb.IsValid() )
			{
				return vb[0];
			}
		}
		throw CReadFailureException(ERROR_ACCESS_DENIED);
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@attention 参照先はインスタンス内のワークのアドレスを指しています。
	 *			次の At() により先の内容が変化する可能性が有りますので、
	 *			必要に応じて値をコピーしておくようにしてください。
	 *	@param index インデックス
	 *	@return 一時的な参照
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 *	@throw CReadFailureException ファイル読み込みに失敗したことを示すためにスローされます。
	 */
	virtual const BYTE& At(INDEX index) const
	{
		m_iTempNo = (m_iTempNo + 1) & 0x0F;
		m_abTemp[m_iTempNo] = Get(index);
		return m_abTemp[m_iTempNo];
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@attention サポートできません。 operator[] を使用します。
	 *	@param index インデックス
	 *	@return -
	 *	@throw CNotSupportException サポートしていないことを示すためにスローされます。
	 */
	virtual BYTE& Ref(INDEX index)
	{
		throw CNotSupportException();
	}

	/**
	 * [取得] 要素の取得.
	 *		Get() と同じ動作です。
	 *	@param index インデックス
	 *	@return 内容
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	BYTE operator[](INDEX index) const
	{
		return Get(index);
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 参照オブジェクト。これを操作するとファイルに影響が有ります。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	CRef operator[](INDEX index)
	{
		if ( m_pWriter == NULL || ! IsInRange(index) )
		{
			throw CIndexOutOfBoundsException(); 
		}
		return CRef(this, index);
	}

	/**
	 * [追加] 要素一つ追加.
	 *	@param t 要素
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval 上記以外 追加したINDEX。
	 */
	virtual INDEX Add(const BYTE& t)
	{
		if ( m_pWriter == NULL )
		{
			return INVALID_INDEX;
		}
		INDEX r = static_cast<INDEX>(m_pWriter->Seek(0, CFileWriter::END));
		if ( r != INVALID_INDEX )
		{
			m_pWriter->Write(1, &t);
		}
		return r; 
	}

	/**
	 * [削除] 要素一つ削除.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 RemoveEx() を使用してください。
	 *	@attention サポートしません。
	 *	@param index インデックス
	 *	@return 常に false(失敗)
	 */
	virtual bool Remove(INDEX index)
	{
		return false;
	}

	/**
	 * [削除] 全要素削除 .
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	virtual bool RemoveAll(void)
	{
		if ( m_pWriter == NULL )
		{
			return false;
		}
		m_pWriter->Seek(0);
		m_pWriter->SetEnd();
		return true;
	}

	/**
	 * [設定] 要素の設定.
	 *	@param index インデックス
	 *	@param t 内容
	 *	@retval true	設定成功
	 *	@retval false	失敗
	 */
	virtual bool Set(INDEX index, const BYTE& t)
	{
		if ( m_pWriter != NULL && IsInRange(index) && m_pWriter->Seek(index) >= 0 )
		{
			m_pWriter->Write(1, &t);
			return true;
		}
		return false;
	}

private:
	IWriter*		m_pWriter;		///< ファイル書き込みクラス
	mutable BYTE	m_abTemp[16];	///< 参照用
	mutable INDEX	m_iTempNo;		///< 参照用Index
};



}; // TNB
