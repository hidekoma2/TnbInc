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
 * ボーダーレイアウトアイテムクラス.
 *
 *		レイアウトアイテムを東西南北、中央に配置することが出来るレイアウトアイテムです。
 *
 *		\code
 *		┌－－－－－－－－－－－－－－－－┐
 *		ｌ┏──────────────┓ｌ
 *		ｌ┃　　　　　北（上）　　　　　┃ｌ
 *		ｌ┣━━━━┳────┳━━━━┫ｌ
 *		ｌ│　西　　│　中央　│　東　　│ｌ
 *		ｌ│　（左）│　　　　│　（右）│ｌ
 *		ｌ┣━━━━┻────┻━━━━┫ｌ
 *		ｌ┃　　　　　南（下）　　　　　┃ｌ　
 *		ｌ┗──────────────┛ｌ
 *		└－－－－－－－－－－－－－－－－┘
 *		\endcode
 *
 *		リサイズすると、北、南のレイアウトアイテムは左右に、西、東のレイアウトアイテムは上下に、中央のレイアウトアイテムは、上下左右に可変します。
 *
 *	@note 全部の位置にレイアウトアイテムを配置する必要はありません。
 *		
 *	@par必要ファイル
 *			TnbBorderLayout.h
 *
 *	@date 08/07/29 新規作成
 */
class CBorderLayout : public CAbstractLayout
{
	DEFSUPER(CAbstractLayout);
public:

	/**
	 * ポジション種.
	 */
	enum EPosition
	{
		Position_North,				///< 北
		Position_South,				///< 南
		Position_West,				///< 西
		Position_Center,				///< 真ん中
		Position_East,				///< 東 
		//
		Position_Top = Position_North,		///< 上
		Position_Left = Position_West,		///< 左
		Position_Right = Position_East,		///< 右 
		Position_Bottom = Position_South,	///< 下
	};

	/**
	 * コンストラクタ
	 */
	CBorderLayout(void)
	{
		m_params = new TParams();
	}

	/**
	 * [設定] アイテム追加.
	 *	@param pos ポジション.
	 *	@param layout レイアウトアイテム.
	 *	@param hloc 水平配置種.
	 *	@param vloc 垂直配置種.
	 *	@return 自分の参照.
	 */
	CBorderLayout& Add(EPosition pos, const ILayout& layout, ELocation hloc = DEFAULT, ELocation vloc = DEFAULT)
	{
		if ( pos >= Position_North && pos <= Position_East )
		{
			m_params->param[pos] = TParam(hloc, vloc, layout);
		}
		else
		{
			ASSERT( false );
		}
		return *this;
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual ILayout* Clone(void) const
	{
		CBorderLayout* P = new CBorderLayout();
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
		SIZE sz;
		for ( int e = Position_West; e <= Position_East; e++ )
		{
			if ( m_params->param[e].pLayout != NULL && m_params->param[e].pLayout->GetMinimumSize(sz) )
			{
				_size.cy = max(_size.cy, sz.cy);
				if ( _size.cx != 0 )
				{
					_size.cx += m_gapSize.cx;
				}
				_size.cx += sz.cx;
			}
		}
		for ( int e = Position_North; e <= Position_South; e++ )
		{
			if ( m_params->param[e].pLayout != NULL && m_params->param[e].pLayout->GetMinimumSize(sz) )
			{
				_size.cx = max(_size.cx, sz.cx);
				if ( _size.cy != 0 )
				{
					_size.cy += m_gapSize.cy;
				}
				_size.cy += sz.cy;
			}
		}
		//
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
		POINT cpos = { m_margn.left, m_margn.top };
		SIZE csize = areaSize;
		SIZE sz; //汎用
		POINT pp[5] = { 0 };
		SIZE ss[5] = { 0 };
		//南（下）
		{
			TParam& pa = m_params->param[Position_South];
			if ( ! pa.pLayout.IsNull() && pa.pLayout->GetSize(sz) )
			{
				pp[Position_South].x = cpos.x;
				ss[Position_South].cx = csize.cx;
				pp[Position_South].y = cpos.y + csize.cy - sz.cy;
				ss[Position_South].cy = sz.cy;
				csize.cy -= sz.cy + m_gapSize.cy;
			}
		}
		//北（上）
		{
			TParam& pa = m_params->param[Position_North];
			if ( ! pa.pLayout.IsNull() && pa.pLayout->GetSize(sz) )
			{
				pp[Position_North].x = cpos.x;
				ss[Position_North].cx = csize.cx;
				pp[Position_North].y = cpos.y;
				ss[Position_North].cy = sz.cy;
				csize.cy -= sz.cy + m_gapSize.cy;
				cpos.y += sz.cy + m_gapSize.cy;
			}
		}
		//西（右）
		{
			TParam& pa = m_params->param[Position_East];
			if ( ! pa.pLayout.IsNull() && pa.pLayout->GetSize(sz) )
			{
				pp[Position_East].x = cpos.x + csize.cx - sz.cx;
				ss[Position_East].cx = sz.cx;
				pp[Position_East].y = cpos.y;
				ss[Position_East].cy = csize.cy;
				csize.cx -= sz.cx+ m_gapSize.cx;
			}
		}	
		//東（左）
		{
			TParam& pa = m_params->param[Position_West];
			if ( ! pa.pLayout.IsNull() && pa.pLayout->GetSize(sz) )
			{
				pp[Position_West].x = cpos.x;
				ss[Position_West].cx = sz.cx;
				pp[Position_West].y = cpos.y;
				ss[Position_West].cy = csize.cy;
				csize.cx -= sz.cx + m_gapSize.cx;
				cpos.x += sz.cx + m_gapSize.cx;
			}
		}	
		//中
		{
			TParam& pa = m_params->param[Position_Center];
			if ( ! pa.pLayout.IsNull() && pa.pLayout->GetSize(sz) )
			{
				pp[Position_Center].x = cpos.x;
				ss[Position_Center].cx = csize.cx;
				pp[Position_Center].y = cpos.y;
				ss[Position_Center].cy = csize.cy;
			}
		}
		//レイアウト
		int row[5] = { Position_North, Position_West, Position_Center, Position_East, Position_South };
		loop ( j, 5 )
		{
			int i = row[j];
			TParam& pa = m_params->param[i];
			if ( ! pa.pLayout.IsNull() )
			{
				HorizontalItemDecide(pa.pLayout, x, pp[i].x, ss[i].cx, pa.horizontalLocate, wParam, lParam);
				VerticalItemDecide(pa.pLayout, y, pp[i].y, ss[i].cy, pa.verticalLocate, wParam, lParam);
			}
		}
	}

private:
	struct TParams
	{
		TParam	param[5];
	};
	CPointerHandleT<TParams>	m_params;		///< アイテム群 m_sizex * m_sizey の数がある
};



}; // TNB

