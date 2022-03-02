#pragma once
/**
 *@file
 * 簡易配列型情報管理関係のヘッダ
 *
 *		型の配列（連続したメモリ配置）を管理します。
 *		メモリを連続して配置しているため、他のAPIに配列として渡すのが容易です。
 *
 *		動的に配列を増やすことができます(メモリの再配置が発生する可能性があります)。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbException.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT
 * 簡易配列型情報管理テンプレート
 *
 *		型の配列（連続したメモリ配置）を管理します。
 *		メモリを連続して配置しているため、他のAPIに配列として渡すのが容易です。
 *
 *		動的に配列を増やすことができます(メモリの再配置が発生する可能性があります)。
 *
 *	@note 一度多く確保した要素は、SetSize() や Remove() で減らしても、確保されたままになります。
 *		その際、その要素は破棄されない（デストラクタが動かない）ことに注意。
 *		SetSize() や RemoveAll() 、デストラクタで、全開放されます。
 *
 *	@see CVectorT 
 *
 *	@param TYP 保持する型（コピー可能の型に限る）
 *
 *	@par必要ファイル
 *			TnbSimpleVector.h
 *
 *	@date 07/12/01 新規作成.
 *	@date 08/08/22 コピー系を追加.
 *	@date 09/04/22 イテレータを廃止.
 *	@date 09/08/28 SetSize() の速度改善。
 *	@date 09/10/27 Find() 追加。
 *	@date 10/03/03 IsEmpty() , operator size_t() 追加。
 *	@date 11/09/07 SetSize() の動作を改善
 *	@date 19/07/07 サイズ指定のコンストラクタ追加
 */
template<typename TYP>
class CSimpleVectorT
{
public:
	/// コンストラクタ
	explicit CSimpleVectorT(void) : m_bufferSize(0), m_dataSize(0), m_pBuffer(NULL)
	{
	}

	/// コンストラクタ
	/// @param bufferSize バッファサイズ
	explicit CSimpleVectorT(size_t bufferSize) : m_bufferSize(bufferSize), m_dataSize(0)
	{
		m_pBuffer = new TYP[m_bufferSize];
	}

	/**
	 * コピーコンストラクタ
	 *	@param other コピー元。
	 */
	CSimpleVectorT(const CSimpleVectorT& other) : m_bufferSize(0), m_dataSize(0), m_pBuffer(NULL)
	{
		operator=(other);
	}

	/// デストラクタ
	virtual ~CSimpleVectorT(void)
	{
		RemoveAll();
	}

	/**
	 * [複製] コピーオペレータ
	 *	@param other コピー元。
	 *	@return 自分の参照
	 */
	CSimpleVectorT& operator=(const CSimpleVectorT& other)
	{
		size_t l = other.GetSize();
		SetSize(l);
		loop ( i, l )
		{
			Set(i, other.At(i));
		}
		return *this;
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@param index インデックス
	 *	@return 参照。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	const TYP& At(INDEX index) const
	{
		if ( m_dataSize <= index ) { throw CIndexOutOfBoundsException(); }
		return m_pBuffer[index];
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@param index インデックス
	 *	@return 参照。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	TYP& At(INDEX index) 
	{
		if ( m_dataSize <= index ) { throw CIndexOutOfBoundsException(); }
		return m_pBuffer[index];
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@param index インデックス
	 *	@return 参照。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	const TYP& operator[](INDEX index) const
	{
		if ( m_dataSize <= index ) { throw CIndexOutOfBoundsException(); }
		return m_pBuffer[index];
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@param index インデックス
	 *	@return 参照。操作するとオブジェクト内の要素に影響が有ります。
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	TYP& operator[](INDEX index)
	{
		if ( m_dataSize <= index ) { throw CIndexOutOfBoundsException(); }
		return m_pBuffer[index];
	}
	
	/**
	 * [設定] 要素の設定.
	 *	@param index インデックス
	 *	@param t 内容
	 *	@retval true	設定成功
	 *	@retval false	失敗
	 */
	bool Set(INDEX index, const TYP& t)
	{
		if ( m_dataSize <= index ) { return false; }
		m_pBuffer[index] = t;
		return true;
	}

	/**
	 * [確認] INDEXの有効確認.
	 *	@param index インデックス
	 *	@retval true	指定Indexは有効。
	 *	@retval false	指定Indexは無効(範囲外)。
	 */
	bool IsInRange(INDEX index) const
	{
		return TNB::IsInRange(index, GetSize());
	}

	/**
	 * [確認] 要素の有無確認.
	 *	@note GetSize()の戻り値が０か否かでチェックしています。
	 *	@retval true	要素なし
	 *	@retval false	要素有り
	 */
	bool IsEmpty(void) const
	{
		return m_dataSize == 0;
	}

	/**
	 * [取得] サイズ取得
	 *	@return 本インスタンスが管理しているデータの長さ。
	 */
	size_t GetSize(void) const
	{
		return m_dataSize;
	}

	/**
	 * [取得] サイズ取得
	 *	@return 本インスタンスが管理しているデータの長さ。
	 */
	operator size_t(void) const
	{
		return m_dataSize;
	}

	/**
	 * [取得] バッファ取得
	 *	@return 本インスタンスが管理しているバッファアドレス。
	 */
	TYP* ReferBuffer(void)
	{
		return m_pBuffer;
	}

	/**
	 * [取得] バッファ取得
	 *	@return 本インスタンスが管理しているバッファアドレス。
	 */
	const TYP* ReferBuffer(void) const
	{
		return m_pBuffer;
	}

	/**
	 * [設定] サイズ設定
	 *	@param s サイズ。
	 */
	void SetSize(size_t s)
	{
		if ( s == 0 )
		{
			RemoveAll();
		}
		else if ( s > GetSize() )
		{
			if ( m_bufferSize < s )
			{
				m_bufferSize = m_GetSecureSize(s + 1);
				TYP* P = new TYP[m_bufferSize];
				loop ( i, m_dataSize )
				{
					P[i] = m_pBuffer[i];
				}
				if ( m_pBuffer != NULL )
				{
					delete[] m_pBuffer;
				}
				m_pBuffer = P;
			}
		}
		m_dataSize = s;
	}

	/**
	 * [追加] 要素一つ追加.
	 *		自インスタンスで管理しているバッファに指定の一データを連結します。
	 *	@param t 要素。
	 *	@retval INVALID_INDEX 失敗。
	 *	@retval 上記以外 追加したINDEX。
	 */
	INDEX Add(const TYP& t)
	{
		if ( m_bufferSize <= m_dataSize )
		{
			m_bufferSize = m_GetSecureSize(m_bufferSize + 1);
			TYP* P = new TYP[m_bufferSize];
			loop ( i, m_dataSize )
			{
				P[i] = m_pBuffer[i];
			}
			if ( m_pBuffer != NULL )
			{
				delete[] m_pBuffer;
			}
			m_pBuffer = P;
		}
		m_pBuffer[m_dataSize] = t;
		m_dataSize += 1;
		return m_dataSize - 1;
	}

	/**
	 * [削除] 要素一つ削除.
	 *	@param index インデックス
	 *	@retval false	エラー。
	 *	@retval true	成功。
	 */
	bool Remove(INDEX index)
	{
		if ( IsInRange(index) ) 
		{
			for ( INDEX i = index ; i < m_dataSize - 1; i++ )
			{
				m_pBuffer[i] = m_pBuffer[i + 1];
			}
			m_dataSize--;
			return true;
		}
		return false;
	}

	/**
	 * [削除] 空化
	 *	@note 自インスタンスで管理しているバッファを空にします。
	 */
	void RemoveAll(void)
	{
		m_bufferSize = 0;
		m_dataSize = 0;
		if ( m_pBuffer != NULL )
		{
			delete[] m_pBuffer;
			m_pBuffer = NULL;
		}
	}

	/**
	 * [検索] 検索.
	 *	@param t 検索要素.
	 *	@retval INVALID_INDEX 未発見.
	 *	@retval 上記以外 発見。値は、インデックス。
	 */
	INDEX Find(const TYP& t) const
	{
		loop ( i, m_dataSize )
		{
			if ( m_pBuffer[i] == t )
			{
				return i;
			}
		}
		return INVALID_INDEX;
	}

private:
	size_t	m_bufferSize;	///< バッファの長さ
	size_t	m_dataSize;		///< 実際のデータの長さ
	TYP *	m_pBuffer;		///< バッファ
	/// 確保すべきサイズ取得
	size_t m_GetSecureSize(size_t size)
	{
		size_t marginSize = size / 8;
		marginSize = (marginSize < 4) ? 4 : ((marginSize > 1024) ? 1024 : marginSize);
		return size + marginSize;
	}
};



}; // TNB

