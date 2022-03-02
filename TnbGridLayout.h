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
 * グリッドレイアウトアイテムクラス.
 *
 *		レイアウトアイテムを左上から格子状に配置出来るレイアウトアイテムです。
 *		コンストラクタ時、横に並べる数を指定します（縦の数は追加した数から計算される）。
 *		\code
 *		┌－－－－－－－－－－－－－┐
 *		ｌ┏───┳───┳───┓ｌ　ex)横3つの場合
 *		ｌ┃　　１┃　２　┃　３　┃ｌ
 *		ｌ┣───╋───╋───┫ｌ
 *		ｌ┃４　　┃　５　┃６　　┃ｌ
 *		ｌ┣───╋───╋───┛ｌ
 *		ｌ┃　７　┃　　８┃　　　　ｌ
 *		ｌ┗───┻───┛　　　　ｌ
 *		ｌ　　　　　　　　　　　　　ｌ
 *		ｌ　　　　　　　　　　　　　ｌ
 *		└－－－－－－－－－－－－－┘
 *		※各レイアウトアイテムの幅は、全体の幅の等分になります。
 *		※各レイアウトアイテムの高さはその列の一番高いものになります。
 *		\endcode
 *
 *	@par使用例
 *		@code
 *		// 設定
 *		CStatic	m_txtFirst;	
 *		CStatic	m_txtSecond;
 *		CEdit	m_editFirst;
 *		CEdit	m_editSecond;
 *			:
 *
 *		BOOL CFoo::OnInitDialog()
 *		{
 *				;
 *		
 *			//-- 水平方向のパーツ二つ 垂直方向に２パーツ
 *			CGridLayout lay(2);
 *			lay.Add(Li(&m_txtFirst));
 *			lay.Add(Li(&m_editFirst));
 *			lay.Add(Li(&m_txtSecond));
 *			lay.Add(Li(&m_editSecond));
 *			//-- ウィンドウ枠から 2 dot 隙間を指定
 *			lay.SetMargnSize(CSize(2, 2));	
 *			// 各アイテムの間は、横 5, 縦 10 空くように指定
 *			lay.SetGapSize(CSize(5, 10));
 *		
 *				;
 *		}
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbGridLayout.h
 *
 *	@date 08/07/28 新規作成
 */
class CGridLayout : public CAbstractLayout
{
	DEFSUPER(CAbstractLayout);
public:

	/**
	 * コンストラクタ
	 *	@param cx 横のアイテム数
	 */
	CGridLayout(size_t cx) : m_sizex(cx)
	{
		ASSERT( m_sizex != 0 );
		m_params = new CParamVector();
	}

	/**
	 * [設定] アイテム追加.
	 *	@note サイズ は layout のサイズが使用されます。
	 *	@param layout レイアウトアイテム.
	 *	@param hloc 水平配置種.
	 *	@param vloc 垂直配置種.
	 *	@return 自分の参照.
	 */
	CGridLayout& Add(const ILayout& layout, ELocation hloc = DEFAULT, ELocation vloc = DEFAULT)
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
		CGridLayout* P = new CGridLayout(m_sizex);
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
		int ly = 0;
		CWorkMemT<LONG> lxs(GetHorizontalItemCount());
		loop ( y, GetVerticalItemCount() )
		{
			loop ( x, GetHorizontalItemCount() )
			{
				if ( m_params->GetSize() > y * GetHorizontalItemCount() + x)
				{
					SIZE sz;
					if ( m_params->At(y * GetHorizontalItemCount() + x).pLayout->GetMinimumSize(sz) )
					{
						ly = max(ly, sz.cy);
						lxs[x] = max(lxs[x], sz.cx);
					}
				}
			}
		}
		_size.cy += ToInt(ly * GetVerticalItemCount());
		// Xを調べる
		int lx = 0;
		loop ( x, GetHorizontalItemCount() )
		{
			lx = max(lx, lxs[x]);
		}
		_size.cx += ToInt(lx * GetVerticalItemCount());
		//
		AddChinkSize(_size, GetHorizontalItemCount(), GetVerticalItemCount());
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
			SIZE sz = { 0, 0 };
			AddChinkSize(sz, GetHorizontalItemCount(), GetVerticalItemCount());
			areaSize.cx = ToInt((areaSize.cx - sz.cx) / GetHorizontalItemCount());
			areaSize.cy = ToInt((areaSize.cy - sz.cy) / GetVerticalItemCount());
		}
		int posy = m_margn.top;
		loop ( yy, GetVerticalItemCount() )
		{
			int posx = m_margn.left;
			loop ( xx, GetHorizontalItemCount() )
			{
				if ( m_params->GetSize() > yy * GetHorizontalItemCount() + xx)
				{
					TParam& pa = m_params->At(yy * GetHorizontalItemCount() + xx);
					HorizontalItemDecide(pa.pLayout, x, posx, areaSize.cx, pa.horizontalLocate, wParam, lParam);
					VerticalItemDecide(pa.pLayout, y, posy, areaSize.cy, pa.verticalLocate, wParam, lParam);
					posx += areaSize.cx + m_gapSize.cx;
				}
			}
			posy += areaSize.cy + m_gapSize.cy;
		}
	}

private:
	typedef CSimpleVectorT<TParam> CParamVector;	///< TParam の配列型
	CPointerHandleT<CParamVector>	m_params;		///< アイテム群 m_sizex * m_sizey の数がある
	size_t							m_sizex;		///< 横数
	/// 水平アイテム数
	size_t GetHorizontalItemCount(void) const
	{
		return m_sizex;
	}
	/// 垂直アイテム数
	size_t GetVerticalItemCount(void) const
	{
		return (m_params->GetSize() + m_sizex - 1) / m_sizex;
	}
};



}; // TNB

