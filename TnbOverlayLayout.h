#pragma once
/**
 *@file
 * レイアウト関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSimpleVector.h"
#include "TnbLayout.h"



//TNB Library
namespace TNB
{



/**@ingroup LAYOUT
 * オーバーレイレイアウトアイテムクラス.
 *
 *		複数のレイアウトアイテムを重なり合うように配置することが出来ます。 
 *		グループボックスとその中のアイテムを配置したり、それぞれを Show/Hide させ、ページのように使う場合に適しています。
 *		\code
 *		┌－－－－－┐
 *		ｌ┏───┓ｌ１,２,３、共に同じ場所で同じ大きさになります。
 *		ｌ│　１　│ｌ
 *		ｌ│（２）│ｌ
 *		ｌ│（３）│ｌ
 *		ｌ┗───┛ｌ
 *		└－－－－－┘
 *		※各レイアウトアイテムのサイズは、全体のサイズと同じなります。
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbOverlayLayout.h
 *
 *	@date 08/07/28 新規作成
 */
class COverlayLayout : public CAbstractLayout
{
	DEFSUPER(CAbstractLayout);
public:

	/// コンストラクタ
	COverlayLayout(void)
	{
		m_params = new CParamVector();
	}

	/**
	 * [設定] アイテム追加.
	 *	@note サイズ は layout のサイズが使用されます。
	 *	@param layout レイアウトアイテム.
	 *	@param hloc 水平配置種. 省略すると ADJUST になります。
	 *	@param vloc 垂直配置種. 省略すると ADJUST になります。
	 *	@return 自分の参照.
	 */
	COverlayLayout& Add(const ILayout& layout, ELocation hloc = ADJUST, ELocation vloc = ADJUST)
	{
		m_params->Add(TParam(hloc, vloc, layout));
		return *this;
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual ILayout* Clone(void) const
	{
		COverlayLayout* P = new COverlayLayout();
		*P = *this;
		return P;
	}

	/**
	 * [取得] 最小サイズ取得.
	 *		最小のサイズを得ることが出来ます。
	 *	@param[out] _size サイズ取得。
	 *	@retval true サイズ取得成功。 
	 *	@retval false データがない。
	 */
	virtual bool GetMinimumSize(SIZE& _size) const
	{
		_size.cx =0;
		_size.cy =0;
		loop ( i, m_params->GetSize() )
		{
			SIZE sz;
			if ( m_params->At(i).pLayout->GetMinimumSize(sz) )
			{
				_size.cx = max(_size.cx, sz.cy);
				_size.cy = max(_size.cy, sz.cy);
			}
		}
		AddMargnSize(_size);
		return true;
	}
	
	/**
	 * [処理] 決定.
	 *	@note オブジェクトが管理しているサイズと指定のサイズにレイアウトします。
	 *	@param x  X座標。-1なら指定なし。
	 *	@param y  Y座標。-1なら指定なし。
	 *	@param wParam W パラメータ。本クラスでは使用しません。 本クラスに登録したレイアウトによっては使用される可能性があります。
	 *	@param lParam L パラメータ。本クラスでは使用しません。 本クラスに登録したレイアウトによっては使用される可能性があります。
	 */
	virtual void Decide(int x, int y, WPARAM wParam, LPARAM lParam)
	{
		SIZE areaSize = m_layoutSize;
		{
			SIZE sz;
			GetMargnSize(sz);
			areaSize.cx -= sz.cx;
			areaSize.cy -= sz.cy;
		}
		int posy = m_margn.top;
		int posx = m_margn.left;
		loop ( i, m_params->GetSize() )
		{
			TParam& pa = m_params->At(i);
			HorizontalItemDecide(pa.pLayout, x, posx, areaSize.cx, pa.horizontalLocate, wParam, lParam);
			VerticalItemDecide(pa.pLayout, y, posy, areaSize.cy, pa.verticalLocate, wParam, lParam);
		}
	}

private:
	typedef CSimpleVectorT<TParam> CParamVector;	///< TParam の配列型
	CPointerHandleT<CParamVector>	m_params;		///< アイテム群
};



}; // TNB

