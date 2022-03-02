#pragma once
/**
 *@file
 * Disabled描画関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDrawable.h"



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE
 * Disabled描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		コンストラクタ時に指定された描画情報を Disabled風に描画します。
 *	
 *	@note 描画した部分がグレーになりますので、 アイコンや文字などの描画情報を用意する必要があります。
 *
 *	@par必要ファイル
 *			TnbDisabledDrawer.h
 *
 *	@date 08/05/21 新規作成
 */
class CDisabledDrawer : public IDrawable
{
	DEFSUPER(IDrawable);
	IDrawable::Ptr	m_pDraw;	///< 描画情報
	/// コールバック
	static BOOL CALLBACK s_DrawIconProc(HDC hDC, LPARAM lParam, WPARAM wParam, int cx, int cy)
	{
		const CDisabledDrawer* P = reinterpret_cast<const CDisabledDrawer*>(lParam);
		P->m_pDraw->Draw(hDC, 0, 0);
		return TRUE;
	}
public:

	/**
	 * コンストラクタ
	 *	@param draw 描画情報。
	 */
	CDisabledDrawer(const IDrawable& draw)
	{
		m_pDraw = draw.Clone();
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CDisabledDrawer(*m_pDraw);
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
		return m_pDraw->GetSize(_size);
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
		return m_pDraw->Resize(size);
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
		SIZE sz;
		if ( m_pDraw->GetSize(sz) )
		{
			::DrawState(dc, NULL, &s_DrawIconProc, reinterpret_cast<LPARAM>(this), 0, x, y, sz.cx, sz.cy, DST_COMPLEX | DSS_DISABLED);
		}
	}
};



};///TNB

