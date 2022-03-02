#pragma once
/**
 *@file
 * レイアウト関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbLayout.h"



//TNB Library
namespace TNB
{



/**@ingroup LAYOUT
 * 比率レイアウトアイテムクラス.
 *
 *		２つのレイアウトアイテム縦か横に配置するレイアウトアイテムです。
 *		左側の幅、あるいは、上側の高さを比率（パーセント）で指定できます。
 *		\code
 *		ex) 横並びの場合
 *		┌－－－－－－－－－┐
 *		ｌ┏───┳───┓ｌ
 *		ｌ│　１　│　２　│ｌ
 *		ｌ┗───┻───┛ｌ
 *		└－－－－－－－－－┘
 *		※各レイアウトの幅は、全体の幅から、比率指定できます。
 *		※各レイアウトの高さは、全体の高さになります。
 *		\endcode
 *		\code
 *		ex) 縦並びの場合
 *		┌－－－－－┐
 *		ｌ┏───┓ｌ
 *		ｌ│　１　│ｌ
 *		ｌ┣───┫ｌ
 *		ｌ│　２　│ｌ
 *		ｌ┗───┛ｌ
 *		└－－－－－┘
 *		※各レイアウトの高さは、全体の高さから、比率指定できます。
 *		※各レイアウトの幅は、全体の高さになります。
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbRatioLayout.h
 *
 *	@date 09/09/24 新規作成
 */
class CRatioLayout : public CAbstractLayout
{
	DEFSUPER(CAbstractLayout);
public:

	/**
	 * コンストラクタ
	 *	@param isHorizental true なら横並び、 false なら縦並びします。
	 *	@param parsent 上か左のレイアウトのパーセント。デフォルトは 50％ です。
	 */
	CRatioLayout(bool isHorizental = true, DWORD parsent = 50) : m_isWideMode(isHorizental), m_center(parsent)
	{
	}

	/**
	 * [設定] パラメータ設定.
	 *	@param isHorizental true なら横並び、 false なら縦並びします。
	 *	@param parsent 上か左のレイアウトのパーセント。デフォルトは 50％ です。
	 */
	void SetParameter(bool isHorizental = true, DWORD parsent = 50)
	{
		m_isWideMode = isHorizental;
		m_center = parsent;
	}

	/**
	 * [設定] アイテム追加.
	 *	@note 既に設定されている場合、それは破棄されます。
	 *	@param isFirst true なら 1st(上か左) 、 false なら 2nd(下か右) のアイテム設定.
	 *	@param layout レイアウトアイテム.
	 *	@param hloc 水平配置種.
	 *	@param vloc 垂直配置種.
	 *	@return 自分の参照.
	 */
	CRatioLayout& Add(bool isFirst, const ILayout& layout, ELocation hloc = DEFAULT, ELocation vloc = DEFAULT)
	{
		m_param[isFirst ? 0 : 1] = TParam(hloc, vloc, layout);
		return *this;
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual ILayout* Clone(void) const
	{
		CRatioLayout* P = new CRatioLayout();
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
		if ( ! m_IsValid() )
		{
			return false;
		}
		_size.cx =0;
		_size.cy =0;
		SIZE sz;
		loop ( i, 2 )
		{
			if ( m_param[i].pLayout->GetMinimumSize(sz) )
			{
				if ( m_isWideMode )
				{
					//== 横画面
					_size.cy = max(_size.cy, sz.cy);
					if ( _size.cx != 0 )
					{
						_size.cx += m_gapSize.cx;
					}
					_size.cx += sz.cx;
				}
				else
				{
					//== 縦画面
					_size.cx = max(_size.cx, sz.cx);
					if ( _size.cy != 0 )
					{
						_size.cy += m_gapSize.cy;
					}
					_size.cy += sz.cy;
				}
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
			SIZE sz = { 0, 0 };
			if ( m_isWideMode )
			{
				AddChinkSize(sz, 2, 1);
			}
			else
			{
				AddChinkSize(sz, 1, 2);
			}
			areaSize.cx -= sz.cx;
			areaSize.cy -= sz.cy;
		}
		POINT pos[2] = { 0 };
		SIZE size[2];
		size[0] = areaSize;
		size[1] = areaSize;
		if ( m_isWideMode )
		{
			//== 横画面
			size[0].cx *= m_center;
			size[0].cx /= 100;
			size[1].cx -= size[0].cx;
			pos[1].x = size[0].cx + m_gapSize.cx;
		}
		else
		{
			//== 縦画面
			size[0].cy *= m_center;
			size[0].cy /= 100;
			size[1].cy -= size[0].cy;
			pos[1].y = size[0].cy + m_gapSize.cy;
		}
		//レイアウト
		loop ( i, 2 )
		{
			TParam& pa = m_param[i];
			HorizontalItemDecide(pa.pLayout, x, pos[i].x + m_margn.left, size[i].cx, pa.horizontalLocate, wParam, lParam);
			VerticalItemDecide(pa.pLayout, y, pos[i].y + m_margn.top, size[i].cy, pa.verticalLocate, wParam, lParam);
		}
	}

private:
	// 有効？
	bool m_IsValid(void) const
	{
		return m_param[0].pLayout != NULL && m_param[1].pLayout != NULL;
	}
	TParam	m_param[2];		///< パラメータ [0];first [1];second
	size_t	m_center;		///< 中央の割合
	bool	m_isWideMode;	///< true;横並び false;縦並び
};



}; // TNB
