#pragma once
/**
 *@file
 * バンク描画情報関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDrawable.h"
#include "TnbSimpleVector.h"



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE
 * バンク描画情報クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		LPARAM により、描画情報を管理します。
 *
 *	@par必要ファイル
 *			TnbBankDrawable.h
 *
 *	@date 08/10/06 新規作成
 */
class CBankDrawer : public IDrawable
{
	DEFSUPER(IDrawable);
public:

	/// コンストラクタ
	CBankDrawer(void)
	{
	}

	/**
	 * コンストラクタ
	 *	@param draw LPARAM 0 の描画情報。複製を本クラスで保持します。
	 */
	CBankDrawer(const IDrawable& draw)
	{
		m_phDraws.Add(draw.Clone());
	}

	/**
	 * [追加] 追加
	 *	@param draw 描画情報
	 *	@return 自分の参照
	 */
	CBankDrawer& Add(const IDrawable& draw)
	{
		m_phDraws.Add(draw.Clone());
		return *this;
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		CBankDrawer* P = new CBankDrawer(*this);
		return P;
	}

	/**
	 * [取得] サイズ取得.
	 *		本オブジェクトの描画する領域の外接する四角のサイズを得ることが出来ます。
	 *	@param[out] _size サイズ取得。
	 *	@retval true サイズ取得成功。 
	 *	@retval false データがない。
	 */
	virtual bool GetSize(SIZE& _size) const
	{
		if ( m_phDraws.GetSize() > 0 )
		{
			return m_phDraws[0]->GetSize(_size);
		}
		return false;
	}
	
	/**
	 * [設定] サイズ設定.
	 *		本オブジェクトの描画する領域の外接する四角のサイズを設定することが出来ます。
	 *	@note 本クラスはサイズ変更をサポートしません。
	 *	@param size サイズ。
	 *	@retval true サイズ変更成功。 
	 *	@retval false 失敗。サイズ変更はサポートしません。
	 */
	virtual bool Resize(const SIZE& size)
	{
		size_t num = m_phDraws.GetSize();
		if ( num > 0 )
		{
			bool r = true;
			loop ( i, num )
			{
				r &= m_phDraws[i]->Resize(size);
			}
			return r;
		}
		return false;
	}

	/**
	 * [描画] 描画.
	 *		指定のデバイスコンテキストの指定の座標に描画します。
	 *	@param dc デバイスコンテキスト
	 *	@param x Ｘ座標
	 *	@param y Ｙ座標
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const
	{
		DrawEx(dc, x, y, 0);
	}

	/**
	 * [描画] 描画.
	 *		指定のデバイスコンテキストの指定の座標に描画します。
	 *	@param dc デバイスコンテキスト
	 *	@param x Ｘ座標
	 *	@param y Ｙ座標
	 *	@param lParam LPARAM。管理している描画情報に渡します。
	 */
	virtual void DrawEx(HDC dc, int x, int y, LPARAM lParam) const
	{
		if ( m_phDraws.GetSize() <= static_cast<size_t>(lParam) )
		{
			lParam = 0;
		}
		m_phDraws[lParam]->DrawEx(dc, x, y, lParam);
	}
protected:
	CSimpleVectorT<IDrawable::Ptr>	m_phDraws;	///< バンク管理
};



};///TNB

