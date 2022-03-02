#pragma once
/**
 *@file
 * アイコン描画情報関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDrawable.h"



//TNB Library
namespace TNB
{




/**@ingroup DRAWABLE
 * アイコン描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		コンストラクタ時に指定されたアイコンを描画します。
 *
 *	@note 指定されたアイコンハンドルは、本クラス破棄時に何もしません。
 *
 *	@par必要ファイル
 *			TnbIconDrawer.h
 *
 *	@date 08/04/18 新規作成
 *	@date 08/09/26 リサイズ制限、対応
 *	@date 14/10/15 GetSize() でハンドルリークしていた。修正。
 */
class CIconDrawer : public IDrawable
{
	DEFSUPER(IDrawable);
protected:
	HICON 			m_icon;			///< アイコン
	bool			m_canResize;	///< リサイズ可能？
	mutable	SIZE	m_size;			///< サイズ
public:

	/**
	 * コンストラクタ
	 *	@param icon アイコン。
	 *	@param canResize リサイズ可能にするか？ true ならリサイズに対応します。
	 */
	CIconDrawer(HICON icon, bool canResize = true) : m_icon(icon), m_canResize(canResize)
	{
		m_size.cx = 0;
		m_size.cy = 0;
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		CIconDrawer* p = new CIconDrawer(m_icon, m_canResize);
		p->m_size = m_size;
		return p;
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
		if ( m_size.cx == 0 )
		{
			ICONINFO ii;
			if ( ::GetIconInfo(m_icon, &ii) )
			{
				HBITMAP h = ii.hbmColor;
				if ( h == NULL )
				{
					h = ii.hbmMask;
				}
				BITMAP bm;
				if ( ::GetObject(h, sizeof(BITMAP), &bm) > 0 )
				{
					m_size.cx = bm.bmWidth;
					m_size.cy = bm.bmHeight;
				}
				::DeleteObject(ii.hbmColor);
				::DeleteObject(ii.hbmMask);
			}
		}
		_size = m_size;
		return m_size.cx != 0;
	}

	/**
	 * [設定] サイズ設定.
	 *		本オブジェクトの描画する領域の外接する四角のサイズを設定することが出来ます。
	 *	@param size サイズ。
	 *	@retval true サイズ変更成功。 
	 *	@retval false 失敗。サイズ変更はサポートしません。
	 */
	virtual bool Resize(const SIZE& size)
	{
		if ( m_canResize )
		{
			m_size = size;
			return true;
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
		SIZE sz;
		GetSize(sz);
		if ( m_canResize )
		{
			::DrawIconEx(dc, x, y, m_icon, sz.cx, sz.cy, 0, NULL, DI_NORMAL);
		}
		else
		{
			::DrawIconEx(dc, x, y, m_icon, 0, 0, 0, NULL, DI_NORMAL);
		}
	}
};



};