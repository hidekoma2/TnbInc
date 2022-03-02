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
 * ボックスレイアウトアイテムクラス.
 *
 *		縦か横、一列に複数並べることが出来るレイアウトアイテムです。
 *
 *		\code
 *		┌－－－－－－－－－－－－－－－－－－－－－－－┐
 *		ｌ┏━━┳━━┳━━┳━━┳━━┳━━━┳──┓ｌ
 *		ｌ│　１│２　│　　│　　│　・・・・・・・ｎ│ｌ
 *		ｌ│　　│　　│３　│４　│５　　　　　　　　│ｌ
 *		ｌ┗━━┻━━┻━━┻━━┻━━┻━━━┻──┛ｌ
 *		└－－－－－－－－－－－－－－－－－－－－－－－┘
 *		※各レイアウトアイテムの幅は固定です(登録時の大きさのまま)。
 *		　最後(右端)のレイアウトアイテムの幅は、オプションで、全体の幅にして可変できます。
 *		※各レイアウトアイテムの高さは、全体の高さに追従して可変します。
 *		\endcode
 *		or
 *		\code
 *		┌－－－－－┐
 *		ｌ┏───┓ｌ
 *		ｌ┃１　　┃ｌ
 *		ｌ┣───┫ｌ
 *		ｌ┃　２　┃ｌ
 *		ｌ┣───┫ｌ
 *		ｌ┃３　　┃ｌ
 *		ｌ┣───┫ｌ
 *		ｌ┃４　　┃ｌ
 *		ｌ┣───┫ｌ
 *		ｌ┃　５　┃ｌ
 *		ｌ┣　：　┫ｌ
 *		ｌ┃　：　┃ｌ
 *		ｌ┣　：　┫ｌ
 *		ｌ│　ｎ　│ｌ
 *		ｌ┗───┛ｌ
 *		└－－－－－┘
 *		※各レイアウトアイテムの高さは固定です(登録時の大きさのまま)。
 *		　最後(下端)のレイアウトアイテムの高さは、オプションで、全体の高さにして可変できます。
 *		※各レイアウトアイテムの幅は、全体の幅に追従して可変します。
 *		\endcode
 *
 *	@par使用例
 *	<table><tr><td><pre>
 *
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
 *			//-- 水平方向のパーツ二つ
 *			MFCLIB::ChangeClientSize(&m_txtFirst, 100, -1);	//TXTの幅を同じにしておく
 *			MFCLIB::ChangeClientSize(&m_txtSecond, 100, -1);
 *			CBoxLayout lay1(true);
 *			lay1.Add(Li(&m_txtFirst)).Add(Li(&m_editFirst), lay.ADJUST);
 *			CBoxLayout lay2(true);
 *			lay2.Add(Li(&m_txtSecond)).Add(Li(&m_editSecond), lay.ADJUST);
 *			//-- 垂直方向に組み合わせる
 *			CBoxLayout lay(false, false);
 *			//-- ウィンドウ枠から 2 dot 隙間を指定
 *			lay.SetMargnSize(CSize(2, 2));	
 *			// 各アイテムの間は、横 5, 縦 10 空くように指定
 *			lay.SetGapSize(CSize(5, 10));
 *			lay.Add(lay1, lay.ADJUST, lay.ADJUST).Add(lay2, lay.ADJUST, lay.ADJUST);
 *		
 *				;
 *		}
 *
 *	</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbBoxLayout.h
 *
 *	@date 08/07/23 新規作成
 */
class CBoxLayout : public CAbstractLayout
{
	DEFSUPER(CAbstractLayout);
public:

	/**
	 * コンストラクタ
	 *	@param isHorizental true なら横並び、 false なら縦並び
	 *	@param isLastFilling true の場合、最終アイテムを調整します。
	 */
	CBoxLayout(bool isHorizental = true, bool isLastFilling = true)
		: m_isHorizental(isHorizental), m_isLastFilling(isLastFilling)
	{
		m_params = new CParamVector();
	}

	/**
	 * [設定] 調整モード設定
	 *	@param isLastFilling true の場合、最終アイテムを調整します。
	 */
	void SetLastFilling(bool isLastFilling)
	{
		m_isLastFilling = isLastFilling;
	}

	/**
	 * [設定] 並び向き設定
	 *	@param isHorizental true なら横並び、 false なら縦並び
	 */
	void SetHorizentalMode(bool isHorizental)
	{
		m_isHorizental = isHorizental;
	}

	/**
	 * [設定] アイテム追加.
	 *	@note サイズ は layout のサイズが使用されます。
	 *	@param layout レイアウトアイテム.
	 *	@param hloc 水平配置種.
	 *	@param vloc 垂直配置種.
	 *	@return 自分の参照.
	 */
	CBoxLayout& Add(const ILayout& layout, ELocation hloc = DEFAULT, ELocation vloc = DEFAULT)
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
		CBoxLayout* P = new CBoxLayout();
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
		return m_Calc(_size, NULL);
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
		CSizeEx position(m_margn.left, m_margn.top);
		CSizeEx minSize;
		CSimpleVectorT<TResult> poss;
		if ( ! m_Calc(minSize, &poss) )
		{
			return;
		}
		size_t itemNum = m_params->GetSize();
		loop ( item, itemNum )
		{
			TParam& pa = m_params->At(item);
			CSizeEx nowItemSize;
			if ( ! pa.pLayout->GetSize(nowItemSize) )
			{
				return;
			}
			CSizeEx areaSize = poss[item].size;
			int ll = 0;
			if ( m_isHorizental )
			{
				areaSize.cy = m_layoutSize.cy - m_margn.top - m_margn.bottom;
				ll = m_layoutSize.cx - minSize.cx;
			}
			else
			{
				areaSize.cx = m_layoutSize.cx - m_margn.left - m_margn.right;
				ll = m_layoutSize.cy - minSize.cy;
			}
			if ( m_isLastFilling && item == (itemNum - 1) )
			{
				areaSize.At(m_isHorizental) += ll;
			}
			loop ( hos, 2 )
			{
				bool isHos = (hos == 0);
				if ( position.At(isHos) < 0 )
				{
					continue;
				}
				int ww = position.At(isHos);
				if ( m_isHorizental == isHos )
				{
					ww += poss[item].pos;
				}
				if ( isHos )
				{
					HorizontalItemDecide(pa.pLayout, x, ww, areaSize.At(isHos), GetLocation(pa, isHos), wParam, lParam);
				}
				else
				{
					VerticalItemDecide(pa.pLayout, y, ww, areaSize.At(isHos), GetLocation(pa, isHos), wParam, lParam);
				}
			}
		}
	}

private:
	/// サイズ管理クラス
	class CSizeEx : public SIZE
	{
		DEFSUPER(SIZE);
	public:
		CSizeEx() {}
		CSizeEx(int x, int y) { cx = x; cy = y; }
		CSizeEx(SIZE initSize) { cx = initSize.cx; cy = initSize.cy; }
		LONG& At(bool isHorizen) { return isHorizen ? cx : cy; }
		const LONG& At(bool isHorizen) const { return isHorizen ? cx : cy; }
	};
	/// 結果型
	struct TResult
	{
		LONG	pos;	///< エリア位置
		CSizeEx	size;	///< エリアサイズ
	};
	/// 計算
	bool m_Calc(SIZE& _size, CSimpleVectorT<TResult>* pRes) const
	{
		CSizeEx gapSize = m_gapSize;
		TResult r;
		r.pos = 0;
		LONG hh = 0;
		loop ( item, m_params->GetSize() )
		{
			const TParam& pa = m_params->At(item);
			if ( ! pa.pLayout->GetMinimumSize(r.size) )
			{
				return false;
			}
			if ( item != 0 )
			{
				r.pos += gapSize.At(m_isHorizental);
			}
			if ( pRes != NULL )
			{
				pRes->Add(r);
			}
			r.pos += r.size.At(m_isHorizental);
			hh = max(hh, r.size.At(! m_isHorizental));
		}
		CSizeEx sz;
		GetMargnSize(sz);
		sz.At(m_isHorizental) += r.pos;
		sz.At(! m_isHorizental) += hh;
		_size = sz;
		return true;
	}
	/// 配置種取得
	static ELocation GetLocation(TParam& p, bool isHorizen) 
	{
		return isHorizen ? p.horizontalLocate : p.verticalLocate; 
	}
	typedef CSimpleVectorT<TParam> CParamVector;	///< TParam の配列型
	CPointerHandleT<CParamVector>	m_params;		///< アイテム群
	bool m_isHorizental;							///< true なら横並び
	bool m_isLastFilling;							///< trueなら最後のアイテムはサイズ一杯に
};



}; // TNB
