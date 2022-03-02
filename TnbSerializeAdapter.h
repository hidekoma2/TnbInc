#pragma once
/**
 *@file
 * 情報群管理のシリアライザーアダプタ関係のヘッダ
 *
 *		情報群管理のシリアライザーアダプタクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbCollection.h"
#include "TnbCollectionAdapter.h"
#include "TnbSerializer.h"



//TNB Library
namespace TNB
{



/**@ingroup SERIALIZE COLLECT
 * シリアライズアダプタ.
 *
 *		BYTE 型のコレクションに対し、データをシリアライズします。
 *
 *	@note	シリアライズ時の型は保存していません。
 *			デシリアライズ時、シリアライズした順番と型を一致させる必要が有ります。
 *
 *	@par必要ファイル
 *			TnbSerializeAdapter.h
 *
 *	@date 06/08/17 新規作成
 *	@date 07/03/15 size管理を unsignedに変更。
 */
class CSerializeAdapter : public ISerializer
{
	ICollectionT<BYTE> *	m_pColl;	///< 対象のコレクション

public:

	/**
	 * コンストラクタ
	 *	@param in 格納対象のBYTEコレクション（追記されます）。
	 */
	CSerializeAdapter(ICollectionT<BYTE>* in) : m_pColl(in)
	{
	}

	/**
	 * [追加] 追加.
	 *		指定データを追加します。
	 *	@param size 追加するデータサイズ(単位BYTE)
	 *	@param P 追加するデータ
	 *	@throw CAppendFailureException 追加に失敗した時、スローされます。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void AddRaw(size_t size, LPCVOID P)
	{
		if ( m_pColl->AddElements(size, static_cast<const BYTE*>(P)) != size )
		{
			throw CAppendFailureException();
		}
	}
};



/**@ingroup SERIALIZE COLLECT
 * デシリアライズアダプタ.
 *
 *		BYTE型のコレクションから、デシリアライします。
 *
 *	@note	シリアライズ時の型は保存していません。
 *			デシリアライズ時、シリアライズした順番と型を一致させる必要が有ります。
 *
 *	@par必要ファイル
 *			TnbSerializeAdapter.h
 *
 *	@date 06/08/17 新規作成
 *	@date 07/03/15 size管理を unsignedに変更。
 */
class CDeserializeAdapter : public IDeserializer
{
	const IConstCollectionT<BYTE> *	m_pColl;	///< 対象のコレクション
	mutable INDEX					m_pos;		///< 現在のアクセスポジション

public:

	/**
	 * コンストラクタ
	 *	@param in 取り出し対象のBYTEコレクション。
	 */
	CDeserializeAdapter(const IConstCollectionT<BYTE>* in)
		: m_pColl(in), m_pos(0)
	{
	}

	/**
	 * [取得] 取得.
	 *		指定データを取得します。
	 *	@param[in] size 取得するデータサイズ(単位BYTE)
	 *	@param[out] _P 取得するデータ
	 *	@throw CShortOfDataException 読み出しサイズが足りない時、スローされます。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void GetRaw(size_t size, LPVOID _P) const
	{
		CConstOffsetAdapterT<BYTE> ad(m_pColl, m_pos);
		size_t l = ad.GetElements(size, static_cast<BYTE*>(_P));
		if ( l != size )
		{
			throw CShortOfDataException();
		}
		m_pos += l;
	}
};



/**@ingroup COLLECT
 * デルタ符号コレクション符号化
 *	@note index0のデータは変化させません。
 *	@param[in,out] _c 符号化するコレクション。
 *	@throw CTnbException 操作時にエラーがあると、スローされる可能性があります。
 */
template<typename TYP>
inline void EncodeDeltaCollection(ICollectionT<TYP>& _c)
{
	size_t size = _c.GetSize();
	if ( size > 1 )
	{
		TYP last = _c.Get(0);
		for ( INDEX i = 1; i < size; i++ )
		{
			TYP t = _c.Get(i);
			_c.Set(i, t - last);
			last = t;
		}
	}
}



/**@ingroup COLLECT
 * デルタ符号コレクション復号化
 *	@note index0のデータを基点に復号します。
 *	@param[in,out] _c 復号化するコレクション。
 *	@throw CTnbException 操作時にエラーがあると、スローされる可能性があります。
 */
template<typename TYP>
inline void DecodeDeltaCollection(ICollectionT<TYP>& _c)
{
	size_t size = _c.GetSize();
	if ( size > 1 )
	{
		TYP last = _c.Get(0);
		for ( INDEX i = 1; i < size; i++ )
		{
			TYP t = _c.Get(i) + last;
			_c.Set(i, t);
			last = t;
		}
	}
}



}; // TNB

