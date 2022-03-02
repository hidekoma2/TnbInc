#pragma once
/**
 *@file
 * 暗号情報関係のヘッダ
 *
 *		暗号化、複合化、をサポート
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#include "TnbSlideCompressor.h"



//TNB Library
namespace TNB
{



/**@ingroup CIPHER
 * 情報暗号復号クラス
 *
 *		簡易的な暗号化/復号化をすることが出来ます。
 *		圧縮も可能です。
 *
 *	@par必要ファイル
 *			TnbBytesCipher.h
 *
 *	@date 06/07/12 新規作成
 *	@date 06/08/10 出力先も抽象化指定できるようにした。
 *	@date 06/11/15 エラー処理強化。
 *	@date 07/05/10 クラス名変更。
 */
class CBytesCipher
{
	CAscii		m_ascKey;					///< 暗号、復号のキー
	bool		m_boIsCompressionEnable;	///< 圧縮有効

	/**
	 * 各データをADD/SUB変換
	 *	@param[in,out] _out 変換したデータを追加します。
	 *	@param[in] in 変換元データ。
	 *	@param[in] boIsAdd true キー情報を足す。 false キー情報を引く。
	 *	@throw CEmptyException キーが設定されていないとスローされます。
	 */
	bool m_Add(ICollectionT<BYTE>& _out, const IConstCollectionT<BYTE>& in, bool boIsAdd = true)
	{
		if ( m_ascKey.IsEmpty() )
		{
			ASSERT0(false, "CBytesCipher", "暗号、復号キーが指定されていません。");
			throw CEmptyException();
		}
		bool r = true;
		const BYTE* B = reinterpret_cast<const BYTE*>(LPCSTR(m_ascKey));
		size_t iKeySize = m_ascKey.GetLength();
		size_t base = _out.GetSize();
		_out.SetSize(base + in.GetSize());
		if ( boIsAdd )
		{
			loop ( i, in.GetSize() )
			{
				r &= _out.Set(base + i, static_cast<BYTE>(in.At(i) + B[i % iKeySize]));
			}
		}
		else
		{
			loop ( i, in.GetSize() )
			{
				r &= _out.Set(base + i, static_cast<BYTE>(in.At(i) - B[i % iKeySize]));
			}
		}
		return r;
	}

public:

	/// コンストラクタ
	CBytesCipher(void) : m_boIsCompressionEnable(false)
	{
	}

	/**
	 * [設定] キー設定.
	 *		暗号、復号に使用するキーを指定します。当然、暗号と同じキーでないと復号できません。
	 *	@note 内部では ASCII に変換され使用されます。
	 *	@param ascKey 暗号キー
	 */
	void SetKey(const CAscii& ascKey)
	{
		m_ascKey = ascKey;
	}

	/**
	 * [設定] 圧縮モード.
	 *		暗号時に圧縮するか指定します。デフォルトは disableです。
	 *		暗号と同じモードでないと復号できません。
	 *	@param boIsEnable true を指定すると圧縮モードありです(暗号データが元データと異なるサイズになる)。<BR>
	 *					false なら暗号化するだけです（暗号データが元データと同じサイズになる）。
	 */
	void Compression(bool boIsEnable)
	{
		m_boIsCompressionEnable = boIsEnable;
	}

	/**
	 * [設定] 暗号化
	 *	@param[out] _out 暗号データ格納先。追加されますので、通常Empty状態のコレクションを指定するようにします。
	 *	@param[in] in 元データ
	 *	@retval true エンコード成功。
	 *	@retval false 失敗。
	 *	@throw CEmptyException キーが設定されていないとスローされます。
	 *	@throw CTnbException 指定コレクションの構造により、読込み処理でスローされる可能性があります。
	 */
	bool Encode(ICollectionT<BYTE>& _out, const IConstCollectionT<BYTE>& in)
	{
		EXCLUSIVE2(&_out, &in);
		bool r = false;
		if ( m_boIsCompressionEnable )
		{
			CSlideCompressor comp;
			CByteVector vb;
			if ( comp.Encode(vb, in) )
			{
				r = m_Add(_out, vb);
			}
		}
		else
		{
			r = m_Add(_out, in);
		}
		return r;
	}

	/**
	 * [設定] 暗号化
	 *	@param c 元データ
	 *	@return 暗号データ
	 *	@throw CEmptyException キーが設定されていないとスローされます。
	 *	@throw CTnbException 指定コレクションの構造により、読込み処理でスローされる可能性があります。
	 */
	CByteVector Encode(const IConstCollectionT<BYTE>& c)
	{
		CByteVector vb;
		if ( ! Encode(vb, c) )
		{
			vb.Invalid();
		}
		return vb;
	}

	/**
	 * [設定] 復号化
	 *	@param[out] _out 暗号データ格納先。追加されますので、通常Empty状態のコレクションを指定するようにします。
	 *	@param[in] in 暗号データ
	 *	@retval true デコード成功。
	 *	@retval false 失敗。
	 *	@throw CEmptyException キーが設定されていないとスローされます。
	 *	@throw CTnbException 指定コレクションの構造により、読込み処理でスローされる可能性があります。
	 */
	bool Decode(ICollectionT<BYTE>& _out, const IConstCollectionT<BYTE>& in)
	{
		EXCLUSIVE2(&_out, &in);
		bool r = false;
		if ( m_boIsCompressionEnable )
		{
			CSlideCompressor comp;
			CByteVector vb;
			if ( m_Add(vb, in, false) )
			{
				r = comp.Decode(_out, vb);
			}
		}
		else
		{
			r = m_Add(_out, in, false);
		}
		return r;
	}

	/**
	 * [設定] 復号化
	 *	@param c 暗号データ
	 *	@return 元データ。Invalid状態ならエラー(圧縮展開エラーが発生の可能性有り)。
	 *	@throw CEmptyException キーが設定されていないとスローされます。
	 *	@throw CTnbException 指定コレクションの構造により、読込み処理でスローされる可能性があります。
	 */
	CByteVector Decode(const IConstCollectionT<BYTE>& c)
	{
		CByteVector vb;
		if ( ! Decode(vb, c) )
		{
			vb.Invalid();
		}
		return vb;
	}
};



};//TNB
