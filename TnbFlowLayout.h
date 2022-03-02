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
 * フローレイアウトアイテムクラス.
 *
 *		レイアウトアイテムを左から右へ順に配置します。一列に配置しきれなくなったら、改行して配置します。
 *
 *		\code
 *		┌－－－－－－－－－－－－－－－－┐
 *		ｌ┏━━┳━━━┳━━━━┳━━┓ｌ
 *		ｌ┃１　┃　２　┃　３　　┃４　┃ｌ
 *		ｌ┣━━┻┳━━┻┳━━┳┻━━┛ｌ
 *		ｌ┃　５　┃６　　┃７　┃　　　　ｌ
 *		ｌ┣━━━┻━━━┻┳━┛　　　　ｌ
 *		ｌ┃　８　　　　　　┃　　　　　　ｌ
 *		ｌ┗━━━━━━━━┛　　　　　　ｌ
 *		ｌ　　　　　　　　　　　　　　　　ｌ
 *		└－－－－－－－－－－－－－－－－┘
 *		┌－－－－－－－－－－－－－－－－－－－－┐
 *		ｌ┏━━┳━━━┳━━━━┳━━┳━━━┓ｌ
 *		ｌ┃１　┃　２　┃　３　　┃４　┃　５　┃ｌ
 *		ｌ┣━━┻┳━━╋━━━━┻━━┻┳━━┛ｌ
 *		ｌ┃６　　┃７　┃　８　　　　　　┃　　　ｌ
 *		ｌ┗━━━┻━━┻━━━━━━━━┛　　　ｌ
 *		ｌ　　　　　　　　　　　　　　　　　　　　ｌ
 *		ｌ　　　　　　　　　　　　　　　　　　　　ｌ
 *		ｌ　　　　　　　　　　　　　　　　　　　　ｌ
 *		└－－－－－－－－－－－－－－－－－－－－┘
 *		※各レイアウトアイテムの幅は固定です(登録時の大きさのまま)。
 *		※各レイアウトアイテムの高さはその列の一番高いものになります。
 *		\endcode
 *
 *		各レイアウトアイテムはリサイズせずに使用するので、 Add() の前に、あらかじめ大きさを設定しておきます。
 *
 *	@note 各行の配置は SetLineLocation() によって指定できます。
 *		\code
 *		ex) SetLineLocation(RIGHT) の場合
 *		┌－－－－－－－－－－－－－－－－┐
 *		ｌ┏━━┳━━━┳━━━━┳━━┓ｌ
 *		ｌ┃１　┃　２　┃　３　　┃４　┃ｌ
 *		ｌ┗━━┻┳━━┻┳━━━╋━━┫ｌ
 *		ｌ　　　　┃　５　┃６　　┃７　┃ｌ
 *		ｌ　　　　┗━┳━┻━━━┻━━┫ｌ
 *		ｌ　　　　　　┃　８　　　　　　┃ｌ
 *		ｌ　　　　　　┗━━━━━━━━┛ｌ
 *		ｌ　　　　　　　　　　　　　　　　ｌ
 *		└－－－－－－－－－－－－－－－－┘
 *		\endcode
 *
 *	@par必要ファイル
 *			TnbFlowLayout.h
 *
 *	@date 08/07/24 新規作成
 */
class CFlowLayout : public CAbstractLayout
{
	DEFSUPER(CAbstractLayout);
public:

	/// コンストラクタ
	CFlowLayout(void) : m_hLocation(LEFT)
	{
		m_params = new CParamVector();
	}

	/**
	 * [設定] 配置設定.
	 *		列の配置を設定します。デフォルトでは LEFT になっています。
	 *	@param hloc 配置。 CENTER , LEFT , RIGHT のいずれかを指定できます。
	 */
	void SetLineLocation(ELocation hloc = LEFT)
	{
		m_hLocation = hloc;
	}

	/**
	 * [設定] アイテム追加.
	 *	@param layout レイアウトアイテム.
	 *	@param vloc 垂直配置種. TOP , CENTER , BOTTOM のいずれかを指定できます。
	 *	@return 自分の参照.
	 */
	CFlowLayout& Add(const ILayout& layout, ELocation vloc = DEFAULT)
	{
		m_params->Add(TParam(vloc, DEFAULT, layout));
		return *this;
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual ILayout* Clone(void) const
	{
		CFlowLayout* P = new CFlowLayout();
		*P = *this;
		return P;
	}

	/**
	 * [取得] 最小サイズ取得.
	 *		最小のサイズを得ることが出来ます。
	 *	@note このレイアウトでは高さ＝０、幅＝レイアウトアイテムの最大幅、になります。
	 *	@param[out] _size サイズ取得。
	 *	@retval true サイズ取得成功。 
	 *	@retval false データがない。
	 */
	virtual bool GetMinimumSize(SIZE& _size) const
	{
		_size.cx = 0;
		_size.cy = 0;
		loop ( item, m_params->GetSize() )
		{
			TParam& pa = m_params->At(item);
			SIZE sz;
			if ( ! pa.pLayout->GetSize(sz) )
			{
				return false;
			}
			_size.cx = max(_size.cx, sz.cx);
		}
		AddMargnSize(_size);
		return true;
	}
	
	/**
	 * [取得] 最小高さサイズ取得.
	 *		指定幅の場合の最小高さを得ることが出来ます。
	 *	@param width 幅。
	 *	@retval マイナス エラー。 
	 *	@retval 0以上 最小高さ。
	 */
	int GetMinimumHeight(size_t width) const
	{
		CSimpleVectorT<POINT> lineNum;	// ライン毎情報。 X がアイテム数、 Y のHIWORDが高さ、LOWORDが幅
		CSimpleVectorT<SIZE> sizes;		// アイテム毎情報。
		int h = m_Calc(lineNum, sizes, width);
		if ( h >= 0 )
		{
			h += m_margn.top + m_margn.bottom;
		}
		return h;
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
		if ( m_params.IsNull() )
		{
			return;
		}
		// 表示範囲
		int clientWidth = m_layoutSize.cx - m_margn.left - m_margn.right;
		// 全アイテムの大きさを取得
		CSimpleVectorT<POINT> lineNum;	// ライン毎情報。 X がアイテム数、 Y のHIWORDが高さ、LOWORDが幅
		CSimpleVectorT<SIZE> sizes;		// アイテム毎情報。
		if ( m_Calc(lineNum, sizes, clientWidth) < 0 )
		{
			return;
		}
		// ライン毎の処理
		int item = 0;
		int yy = m_margn.top;
		loop ( line, lineNum.GetSize() )
		{
			int hh = LOWORD(lineNum[line].y);
			int xx = (clientWidth - HIWORD(lineNum[line].y));
			switch ( m_hLocation )
			{
			case LEFT:
				xx = 0;
				break;
			case CENTER:
			case DEFAULT:
				if ( xx < 0 ) { xx = 0; }
				xx /= 2;
				break;
			}
			xx += m_margn.left;
			loop ( i, lineNum[line].x )
			{
				TParam& pa = m_params->At(item);
				SIZE& sz = sizes[item];
				HorizontalItemDecide(pa.pLayout, x, xx, sz.cx, LEFT, wParam, lParam);
				VerticalItemDecide(pa.pLayout, y, yy, sz.cy, pa.verticalLocate, wParam, lParam);
				item++;
				xx += sz.cx + m_gapSize.cx;
			}
			yy += m_gapSize.cy + hh;
		}
	}

private:

	// _lineNum ライン毎情報。 X がアイテム数、 Y のHIWORDが高さ、LOWORDが幅
	// _sizes アイテム毎情報。
	int m_Calc(CSimpleVectorT<POINT>& _lineNum, CSimpleVectorT<SIZE>& _sizes, size_t clientWidth) const
	{
		size_t itemNum = m_params->GetSize();
		_sizes.SetSize(itemNum);
		_lineNum.RemoveAll();
		int height = 0;
		int im = 0;
		int h = 0;
		int w = 0;
		loop ( item, itemNum )
		{
			TParam& pa = m_params->At(item);
			SIZE& sz = _sizes[item];
			if ( ! pa.pLayout->GetSize(sz) )
			{
				return -1;
			}
			if ( im > 0 && w + sz.cx + m_gapSize.cx > ToInt(clientWidth) )
			{
				// 幅を超えた
				POINT p = { im, MAKELONG(h, w) };
				height += h;
				if ( _lineNum.GetSize() != 0 )
				{
					height += m_gapSize.cy;
				}
				_lineNum.Add(p);
				im = 0;
				h = 0;
				w = 0;
			}
			w += sz.cx;
			if ( im > 0 )
			{
				w += m_gapSize.cx;
			}
			h = max(h, sz.cy);
			im++;
		}
		if ( im > 0 )
		{
			// 幅を超えた
			POINT p = { im, MAKELONG(h, w) };
			height += h;
			if ( _lineNum.GetSize() != 0 )
			{
				height += m_gapSize.cy;
			}
			_lineNum.Add(p);
		}
		return height;
	}

	typedef CSimpleVectorT<TParam> CParamVector;	///< TParam の配列型
	CPointerHandleT<CParamVector>	m_params;		///< アイテム群
	ELocation						m_hLocation;	///< 列毎の位置
};



}; // TNB
