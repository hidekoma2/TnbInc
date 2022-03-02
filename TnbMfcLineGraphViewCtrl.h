#pragma once
/**
 *@file
 * グラフ描画コントロール関係のヘッダ
 *	
 *		グラフを描画するコントロールクラスがあります。
 */



#include "TnbBitmapImage.h"
#include "TnbMfcBitmapDC.h"
#include "TnbMap.h"
#include "TnbTextDrawer.h"
#include <float.h>
#include <math.h>



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * 折れ線グラフコントロール.
 *
 *		最下限 ～ 0(基準) ～ 最上限 の縦軸を持った折れ線グラフを表示するコントロールです。
 *		表示の上限と下限を指定することが出来ます。
 *		この表示の上限下限がコントロールの上部下部にマッピングされて表示されます。
 *		→ SetViewValueRange()
 *
 *		横軸はコントロールのクライアント幅以上も指定可能です。
 *		その場合、横スクロールバーが表示されます。
 *		→ SetBackgroundWidth()
 *
 *		また、スクロールすると親ウィンドウに、 WM_HSCROLL メッセージが飛びます。その際、
 *		WPARAM の下位ワードは SB_THUMBPOSITION 固定、上位ワードは現在の水平表示位置が入っています。 
 *		LPARAM は本コントロールのウィンドウハンドルです（MFC のハンドラの場合、 OnHScroll() の第三引数は
 *		CScrollBar* ですが、 このコントロールのポインタが入ってますので扱いには注意してください)。
 *
 *		横軸インデックス１個を何ピクセルにするか指定することが出来ます。デフォルトは１です。
 *		→ SetHorizontalIndexPixel()
 *
 *		スケール線を表示することが出来ます（メイン線とサブ線）。それぞれ、スタイル、色を指定可能です。
 *		→ SetScaleLine() , SetSubScaleLine()
 *
 *		任意の文字を、グラフ内に表示することが出来ます（８隅の位置）。それぞれフォント、色の指定が可能。
 *		これらの文字は横スクロールには影響されません。
 *		→ SetOverlayText()
 *
 *		線は、色毎に管理され、連続したポイントを線で接続することが可能です。
 *		
 *		横軸インデックス０は、通常左ですが、右にすることも可能です（表示が左右反転する）。
 *		→ SetHorizontalIndexPixel()
 *
 *		静的なグラフの他、全体をスクロールさせて連続的に新しい情報を表示していくことも可能です（最新を最右か最左に固定）。
 *		→ Shift()
 *
 *		また、DCを取得し、自由に描画することも可能です。
 *		→ GetImageDC() , ReleaseImageDC()
 *
 *	@par使い方
 *		リソースエディタでスタティックコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CStatic）を本クラスに置き換えて使用します。
 *
 *	@par使用例(固定の値を表示するグラフ)
 *		\code
 *		// *.h
 *		class CFooDlg : public CDialog {
 *			;
 *		CLineGraphViewCtrl m_graph;
 *
 *		// *.cpp
 *		void CFooDlg::OnInitDialog(void) {
 *				;
 *			m_graph.SetHorizontalIndexPixel(5); //横軸インデックス１個の幅は 5ピクセルとする
 *			m_graph.SetBackgroundWidth(RGB(0, 0, 0), 1000); //最新が一番右 背景は黒。幅は 1000ピクセル。横軸インデックスでは 200個となる。
 *			m_graph.SetPointMakertSize(1); //ポイントマーカーの大きさは 1ピクセルとする。
 *			m_graph.SetViewValueRange(5000, -5000); //縦軸値の表示範囲は 5000 ～ -5000 とする
 *			m_graph.SetScaleLine(RGB(255, 255, 255), m_graph.LineStyle_Bold); //主線は白の太線とする。
 *			m_graph.SetSubScaleLine(1, m_graph.LineStyle_Dot, RGB(50, 50, 50), 1000); //副線１は 1000毎にグレーの破線とする。
 *			m_graph.SetSubScaleLine(2, m_graph.LineStyle_EdgeDot, RGB(50, 50, 50), 100); //副線２は 100毎に左右に目印とする。
 *			m_graph.SetOverlayText(7, RGB(255, 255, 255), _T("タイトル")); // 左上に文字表示
 *				;
 *			loop ( i, 200 ) {
 *				int d = this->GetData(i);
 *				if ( (i % 10) == 0 ){
 *					m_graph.SetVerticalLine(RGB(100, 20, 20), i, 20); //中央軸にちっちゃい縦線
 *				}
 *				m_graph.SetPoint(RGB(255, 0, 0), d, i);
 *			}
 *			m_graph.Settle();
 *		}
 *		\endcode
 *	
 *	@par使用例(動的に変化し続けるグラフ)
 *		\code
 *		// *.h
 *		class CFooDlg : public CDialog {
 *			;
 *		CLineGraphViewCtrl m_graph;
 *
 *		// *.cpp
 *		void CFooDlg::OnInitDialog(void) {
 *				;
 *			m_graph.SetHorizontalIndexPixel(5, true); //横軸インデックス１個の幅は 5ピクセルとする　最新が一番右 
 *			m_graph.SetBackgroundWidth(RGB(0, 0, 0)); //背景は黒
 *			m_graph.SetPointMakertSize(3); //ポイントマーカーの大きさは 3ピクセルとする。
 *			m_graph.SetViewValueRange(5000, -10); //縦軸値の表示範囲は 5000 ～ -10 とする。
 *			m_graph.SetScaleLine(RGB(255, 255, 255), m_graph.LineStyle_Bold); //主線は白の太線とする。
 *			m_graph.SetSubScaleLine(1, m_graph.LineStyle_Dot, RGB(50, 50, 50), 100); //副線は 100毎にグレーの破線とする。
 *			m_graph.SetOverlayText(7, RGB(255, 255, 255), _T("タイトル")); // 左上に文字表示。
 *				;
 *
 *		// データ受信
 *		void CFooDlg::OnReceivedData(int data) {
 *			// 受信したデータをグラフの右端に追加していく。左に行くにつれ、古いデータになる。
 *			m_graph.Shift(); // まず、グラフを右に一つシフト。
 *			m_graph.SetPoint(RGB(255, 0, 0), data);  //一番右にポイントマーカーを打つ
 *			m_graph.SetOverlayText(1, RGB(155, 155, 155), CStr::Fmt(_T("%d"), data); // 左下に文字表示
 *			m_graph.Settle(); // 確定
 *		}
 *		\endcode
 *
 *
 *	@par必要ファイル
 *			TnbMfcLineGraphViewCtrl.h
 * 
 *	@date 11/05/23 新規作成
 *	@date 11/06/15 画面をプレスして移動する処理を修正。
 *	@date 11/08/02 ClearPoint() 追加。
 */
class CLineGraphViewCtrl : public CWnd, CCopyImpossible
{
	DEFSUPER(CWnd);
public:

	/// 定数
	enum
	{
		EDGE_VALUE = 10000,				///< デフォルトの上下端のポイント値
		INTERNALLY_COLOR = RGB(1, 1, 1)	///< 内部処理用カラー
	};

	/// ラインスタイル
	enum ELineStyle
	{
		LineStyle_Non,		///< 非表示
		LineStyle_Solid,	///< 実線
		LineStyle_Dot,		///< 破線
		LineStyle_Bold,		///< 太線
		LineStyle_EdgeDot,	///< 両端のみ
	};

	/// コンストラクタ.
	CLineGraphViewCtrl(void) : m_isReverseIndexMode(false), m_viewHorizontalPos(0), m_markerWidth(6), m_scrollOneStep(1)
		, m_viewTopValue(EDGE_VALUE), m_viewbottomValue(-EDGE_VALUE), m_scrollBarHeight(::GetSystemMetrics(SM_CYHSCROLL))
		, m_backColor(RGB(0, 0, 0)), m_isGraphImageDcLeading(false), m_isOndemandRemakeViewImage(false), m_horzIndexPixel(1)
		, m_isMouseButtonPressed(false), m_isScrollBarUsed(true)
	{
		m_viewHeight = m_viewTopValue - m_viewbottomValue + 0.0; //表示範囲
	}

	/**
	 * [設定] グラフ背景幅設定.
	 *	@note SetPoint() でポイントを描画後、でこのメソッドをコールすると、ポイントがクリアされます。
	 *	@param backgroundColor 背景色
	 *	@param graphWidth グラフのピクセル幅を指定します。クライアント幅より大きい場合、スクロールバーが表示されます。
	 *					省略するか0を指定すると、クライアント幅と同じになります。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetBackgroundWidth(COLORREF backgroundColor, size_t graphWidth = 0)
	{
		if ( ::IsWindow(GetSafeHwnd()) )
		{
			CRect rc;
			GetClientRect(&rc);
			CSize sz = rc.Size();
			m_backColor = backgroundColor;
			if ( graphWidth == 0 )
			{
				graphWidth = sz.cx;
			}
			if ( sz.cx == 0 || sz.cy == 0 )
			{
				return false;
			}
			m_Resize(sz, graphWidth);
			SetViewHorizontalPos(0);
			return true;
		}
		return false;
	}

	/**
	 * [設定] インデックスピクセル設定.
	 *		横軸インデックス１が何ピクセルかを指定できます。通常１ですが、例えば、ポイントマーカーを 3 ピクセル幅にしたら、
	 *		横軸も 3 ピクセル以上あけないとマーカーがかぶってしまいます。
	 *		そこでこのガイド設定をすることで、横軸インデックス指定時に意識することなく＋１するだけでいいようになります。
	 *		SetPoint(), SetVerticalLine() , Shift() , SetViewHorizontalPos() に影響があります。
	 *	\code
	 *		m_graph.SetPointMakertSize(3); //ポイントマーカーの大きさは 3ピクセルとする。
	 *		m_graph.SetHorizontalIndexPixel(5); //横軸インデックス１個の幅は 5ピクセルとする
	 *				;
	 *		m_graph.SetPoint(color, 100, 0);	//この二点は横軸インデックスが１しか違わないが、
	 *		m_graph.SetPoint(color, 100, 1);	//　間は 5 ピクセルになる。
	 *	\endcode
	 *	@note SetPoint() でポイントを描画後、でこのメソッドをコールすると、ポイントがクリアされます。
	 *	@param pixel 横軸インデックス１が何ピクセルかを指定できます。
	 *	@param isReverseIndexMode 省略か false なら横軸インデックス０が一番左、 true なら一番右になります。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetHorizontalIndexPixel(int pixel = 1, bool isReverseIndexMode = false)
	{
		if ( ::IsWindow(GetSafeHwnd()) )
		{
			m_horzIndexPixel = pixel;
			if ( m_horzIndexPixel <= 0 )
			{
				m_horzIndexPixel = 1;
			}
			m_isReverseIndexMode = isReverseIndexMode;
			m_Resize();
			return true;
		}
		return false;
	}
	
	/**
	 * [取得] インデックスピクセル取得.
	 *	@note SetHorizontalIndexPixel() 設定した値が返ります。
	 *	@return インデックスピクセル
	 */
	int GetHorizontalIndexPixel(void) const
	{
		return m_horzIndexPixel;
	}

	/**
	 * [設定] ポイントマーカーサイズ設定.
	 *	@note SetPoint() の直前に変えても対応できます。
	 *	@param width 幅
	 *	@return 設定前の値
	 */
	size_t SetPointMakertSize(size_t width)
	{
		size_t r = m_markerWidth;
		m_markerWidth = width;
		return r;
	}

	/**
	 * [取得] 垂直表示範囲取得.
	 *	@param[out] _topValue 最上の縦軸値
	 *	@param[out] _bottomValue 最下の縦軸値
	 */
	void GetViewValueRange(double& _topValue, double& _bottomValue) const
	{
		_topValue = m_viewTopValue;
		_bottomValue = m_viewbottomValue;
	}

	/**
	 * [設定] 垂直表示範囲設定.
	 *		表示範囲を指定します。指定が無い場合、最上が 10000、最下が -10000 になります。
	 *	@note SetPoint() でポイントを描画後、でこのメソッドをコールすると、ポイントがクリアされます。
	 *	@note 縦軸の上限下限は「int で表すことの出来る値 / クライアントの高さ(pixel)」になります。
	 *	@param topValue 最上の縦軸値
	 *	@param bottomValue 最下の縦軸値
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetViewValueRange(double topValue, double bottomValue)
	{
		if ( ::IsWindow(GetSafeHwnd()) )
		{
			if ( topValue < bottomValue )
			{
				Swap(topValue, bottomValue);
			}
			if ( m_viewTopValue == topValue && m_viewbottomValue == bottomValue )
			{
				return true;
			}
			m_viewTopValue = topValue;
			m_viewbottomValue = bottomValue;
			m_viewHeight = m_viewTopValue - m_viewbottomValue + 0.0; //表示範囲
			//
			const SIZE& sz = m_graphImage.GetSize();
			if ( m_graphImageDc.GetSafeHdc() != NULL )
			{
				m_graphImageDc.FillSolidRect(0, 0, sz.cx, sz.cy, INTERNALLY_COLOR);
			}
			m_pointMap.RemoveAll();
			return true;
		}
		return false;
	}
	
	/**
	 * [設定] スクロールバー高さ.
	 *	@note SetPoint() でポイントを描画後、でこのメソッドをコールすると、ポイントがクリアされます。
	 *	@param height 高さ（単位ピクセル）。０ならシステムの高さになります。マイナスなら、スクロールバーを出さないようにします。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetScrollBarHeight(int height = 0)
	{
		if ( ::IsWindow(GetSafeHwnd()) )
		{
			m_scrollBarHeight = height;
			if ( height <= 0 )
			{
				m_scrollBarHeight = ::GetSystemMetrics(SM_CYHSCROLL);
			}
			m_isScrollBarUsed = (height >= 0);
			m_Resize();
			return true;
		}
		return false;
	}


	/**
	 * [設定] スクロールバー１ステップ幅.
	 *		デフォルトでは 1 です。
	 *	@param os 幅
	 */
	void SetScrollOneStep(size_t os = 1)
	{
		m_scrollOneStep = os;
	}

	/**
	 * [設定] スケール線設定.
	 *		縦軸値０の位置に表示される線の設定をします。
	 *	@param color 色
	 *	@param style スタイル
	 */
	void SetScaleLine(COLORREF color, ELineStyle style = LineStyle_Solid)
	{
		m_scaleLine.Set(style, color, 0.0);
	}

	/**
	 * [設定] サブスケール線設定.
	 *		指定のステップごとに表示される線の設定をします。最大３つ指定できます。
	 *	@param number サブスケール番号 1～3 を指定できます。
	 *	@param style スタイル
	 *	@param color 色
	 *	@param step ステップ。 表示の縦軸値が -10000 ～ 10000 の場合、1000 を指定するとスケール線の上下にそれぞれ、10本引かれます。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetSubScaleLine(INDEX number, ELineStyle style = LineStyle_Non, COLORREF color = 0, double step = 0.0)
	{
		if ( number >= 1 && number <= 3 )
		{
			TScaleLineInfo& I = m_subScaleLines[number - 1];
			if ( step <= 0.0 )
			{
				style = LineStyle_Non;
			}
			I.Set(style, color, step);
			return true;
		}
		return false;
	}

	/**
	 * [設定] 文字列設定.
	 *		グラフの上に表示される文字を設定します。一度設定したら、取り消すまで表示されます。
	 *	@param pos 位置を示す 1 ～ 9（テンキーの並び）。
	 *	@param color 文字の色。省略すると、表示取り消しになります。
	 *	@param lpsz 文字。
	 *	@param hFont フォント。本クラスで使用が終わるまで、破棄しないでください。
	 *			 省略すると、コントロールに設定されているフォントが使用される。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool SetOverlayText(INDEX pos, COLORREF color = RGB(0, 0, 0), LPCTSTR lpsz = NULL, HFONT hFont = NULL)
	{
		if ( pos > 0 && pos < 10 )
		{
			TTextInfo& t = m_testInfo[pos];
			t.hFont =hFont;
			t.text = lpsz;
			t.color = color;
			return true;
		}
		return false;
	}

	/**
	 * [確認] 逆インデックスモード？.
	 *		正インデックスモードは最左が横軸インデックス０。逆インデックスモードは横軸インデックス０が最右です。
	 *	@retval true 逆インデックスモード
	 *	@retval false 正インデックスモード
	 */
	bool IsReverseHorizontalIndexMode(void) const
	{
		return m_isReverseIndexMode;
	}

	/**
	 * [確認] スクロールバーあり？.
	 *	@note SetBackgroundWidth() のグラフの幅により、スクロールバーが現れます。その確認が出来ます。 
	 *	@retval true あり
	 *	@retval false なし
	 */
	bool HasScrollBar(void) const
	{
		return !! ::IsWindow(m_scrollBar);
	}

	/**
	 * [取得] グラフイメージ DC 取得.
	 *		グラフの書かれているイメージの DC を取得できます。
	 *		DC のサイズは GetImageSize() で得られます。
	 *		描画後は、 ReleaseImageDC() を実行する必要があります。
	 *	@retval NULL 失敗.
	 *	@retval NULL以外 成功。値は CDC のポインタ.
	 */
	CDC* GetImageDC(void)
	{
		if ( m_isGraphImageDcLeading || m_graphImageDc.GetSafeHdc() == NULL )
		{
			return NULL;
		}
		m_isGraphImageDcLeading = true;
		return &m_graphImageDc;
	}

	/**
	 * [設定] グラフイメージ DC 返却.
	 *		GetImageDC() で取得した DC を返却します。
	 */
	void ReleaseImageDC(void)
	{
		if ( m_isGraphImageDcLeading )
		{
			m_isGraphImageDcLeading = false;
		}
	}

	/**
	 * [取得] グラフイメージサイズ取得.
	 *		グラフの書かれているイメージのサイズを取得できます。
	 *	@return サイズ
	 */
	const SIZE& GetImageSize(void) const
	{
		return m_graphImage.GetSize();
	}

	/**
	 * [取得] 横軸インデックス to X座標.
	 *		横軸インデックスを GetImageDC() で描画するための X 座標に変換します。
	 *	@param horizontalIndex 横軸インデックス.
	 *	@return X 座標
	 */
	int HorizontalIndexToX(INDEX horizontalIndex)
	{
		int xx = ToInt(horizontalIndex * m_horzIndexPixel + m_horzIndexPixel / 2);
		if ( m_isReverseIndexMode )
		{
			xx = (m_graphImage.GetSize().cx - 1) - xx;
		}
		return xx;
	}

	/**
	 * [移動] シフト.
	 *		指定数だけ、グラフをシフトします。
	 *	@note グラフ全体をシフトします。
	 *		（正インデックスモードなら、右にシフトします）
	 *	@param count シフト数。省略すると１です。
	 */
	void Shift(int count = 1)
	{
		if ( m_graphImageDc.GetSafeHdc() != NULL )
		{
			const SIZE& sz = m_graphImage.GetSize();
			count *= m_horzIndexPixel;
			if ( m_isReverseIndexMode )
			{
				count = -count;
			}
			m_graphImageDc.ScrollDC(count, 0, NULL, NULL, NULL, NULL);
			if ( count > 0 )
			{
				m_graphImageDc.FillSolidRect(0, 0, count, sz.cy, INTERNALLY_COLOR);
			}
			else if ( count < 0 )
			{
				m_graphImageDc.FillSolidRect(sz.cx + count, 0, -count, sz.cy, INTERNALLY_COLOR);
			}
			loop ( i, m_pointMap )
			{
				m_pointMap.Ref(i).second->point.x += count;
			}
		}
	}

	/**
	 * [取得] 横軸最大インデックス取得.
	 *		SetPoint() や SetVerticalLine() に指定できる横軸インデックスの最大を取得できます。
	 *	@note GetViewHorizontalPosLimit() とは値が異なります。
	 *	@return 最大インデックス.
	 */
	INDEX GetHorizontalIndexLimit(void) const
	{
		return m_graphImage.GetSize().cx / m_horzIndexPixel;
	}

	/**
	 * [設定] ポイントクリア.
	 *		SetPoint() で指定したポイントマーカーを全てクリアします。
	 */
	void ClearPoint(void)
	{
		if ( m_graphImageDc.GetSafeHdc() != NULL )
		{
			const SIZE& sz = m_graphImage.GetSize();
			m_graphImageDc.FillSolidRect(0, 0, sz.cx, sz.cy, INTERNALLY_COLOR);
			m_pointMap.RemoveAll();
		}
	}

	/**
	 * [設定] ポイント設定.
	 *		指定の横軸インデックス、縦軸値の位置に指定の色でポイントマーカーが描かれます。また、一つ前のポイントと連続線が引かれます。
	 *	@note 色がキーとなり、連続線が引かれます。
	 *	@note 色に INTERNALLY_COLOR は指定しないでください。
	 *	@param color 色。
	 *	@param value 縦軸値。 
	 *	@param horizontalIndex 横軸インデックス。正インデックスモードなら 0　が一番左です。
	 *	@param withContinuousLine true なら連続線を引きます。 false なら引きません。
	 */
	void SetPoint(COLORREF color, double value, INDEX horizontalIndex = 0, bool withContinuousLine = true)
	{
		if ( m_graphImageDc.GetSafeHdc() != NULL )
		{
			int xx = HorizontalIndexToX(horizontalIndex);
			m_SetMaker(m_graphImageDc, xx, value, color, withContinuousLine);
		}
	}

	/**
	 * [設定] 縦スケール設定.
	 *		指定の index に指定の色で縦線のスケールを書きます。
	 *	@note 色に INTERNALLY_COLOR は指定しないでください。
	 *	@param color 色。
	 *	@param horizontalIndex 横軸インデックス。正インデックスモードなら 0　が一番左です。
	 *	@param length 中心線からの上下それぞれの長さ(pixel)。-1 なら上から下まで書かれます。
	 */
	void SetVerticalLine(COLORREF color, INDEX horizontalIndex, int length = -1)
	{
		if ( m_graphImageDc.GetSafeHdc() != NULL )
		{
			int xx = HorizontalIndexToX(horizontalIndex);
			CPen pen((length != -2)  ? PS_SOLID : PS_DASH, 1, color);
			CPen* pOldPen = m_graphImageDc.SelectObject(&pen);
			if ( length <= 0 )
			{
				m_graphImageDc.MoveTo(xx, 0);
				m_graphImageDc.LineTo(xx, m_clientSize.cy);
			}
			else
			{
				int yy = m_CalcDrawY(0.0);
				m_graphImageDc.MoveTo(xx, yy - length);
				m_graphImageDc.LineTo(xx, yy + length);
			}
			m_graphImageDc.SelectObject(pOldPen);
		}
	}

	/**
	 * [設定] 確定.
	 *		設定情報を確定し、画面に反映させます。
	 *	@note SetPoint() , SetVerticalLine() や SetOverlayText() など実行後、これをコールしないと、画面に反映されません。
	 *	@param isForce true を指定すると、すぐに描画用イメージを作成します。通常、 省略か false を指定します（必要な時に作成する）。
	 */
	void Settle(bool isForce = false)
	{
		if ( isForce )
		{
			m_MakeViewImage();
		}
		else
		{
			OndemandRemakeViewImage();
		}
		RedrawWindow();
	}

	/**
	 * [取得] 水平表示最大位置取得.
	 *		SetViewHorizontalPos() に渡せる最大値を取得できます。
	 *	@return 最大値 
	 */
	size_t GetViewHorizontalPosLimit(void)
	{
		return m_scrollBar.GetScrollLimit();
	}

	/**
	 * [取得] 水平表示位置取得.
	 *	@return 表示位置.
	 */
	INDEX GetViewHorizontalPos(void) const
	{
		return m_viewHorizontalPos / m_horzIndexPixel;
	}

	/**
	 * [設定] 水平表示位置設定.
	 *	@note 左端に表示したい横軸インデックスを指定します（逆インデックスモードなら右端）。
	 *	@param pos 表示位置。
	 */
	void SetViewHorizontalPos(INDEX pos)
	{
		if ( ::IsWindow(m_scrollBar) )
		{
			INDEX xx = pos;
			if ( m_isReverseIndexMode )
			{
				xx = m_scrollBar.GetScrollLimit() - xx;
			}
			m_scrollBar.SetScrollPos(ToInt(xx));
			xx = m_scrollBar.GetScrollPos();
			if ( m_isReverseIndexMode )
			{
				xx = m_scrollBar.GetScrollLimit() - xx;
			}
			m_viewHorizontalPos = xx * m_horzIndexPixel;
			OndemandRemakeViewImage();
			RedrawWindow();
		}
	}

protected:

	/**
	 * [通知] 再構築要求通知.
	 */
	void OndemandRemakeViewImage(void)
	{
		m_isOndemandRemakeViewImage = true;
	}

	/**
	 * [通知] 表示用イメージ作成通知.
	 *		表示用のイメージを作成後に通知されます。
	 *		渡された pDC に描画することも可能です。
	 *	@param pDC デバイスコンテキスト
	 *	@param sz デバイスコンテキストの大きさ
	 */
	virtual void OnMadeViewImage(CDC* pDC, const SIZE& sz)
	{
	}

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::PreSubclassWindow();
		CRect rc;
		GetClientRect(&rc);
		m_clientSize = rc.Size();
		const SIZE& sz = m_clientSize;
		m_viewImage.Set(sz.cx, sz.cy, m_backColor);
		m_graphImageDc.Detach();
		m_graphImage.Set(sz.cx, sz.cy, INTERNALLY_COLOR);
		m_graphImageDc.Attach(&m_graphImage);
	}

	/**
	 * [通知] for processing Windows messages.
	 *		メッセージ受信したらコールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 *	@param message メッセージ
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return リザルト。
	 */
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch ( message )
		{
		case WM_ERASEBKGND:
			return 0;
		case WM_PRINTCLIENT:
			if ( m_isOndemandRemakeViewImage )
			{
				m_isOndemandRemakeViewImage = false;
				m_MakeViewImage();
			}
			m_viewImage.Draw(reinterpret_cast<HDC>(wParam));
			return 0;
		case WM_PAINT:
			if ( m_isOndemandRemakeViewImage )
			{
				m_isOndemandRemakeViewImage = false;
				m_MakeViewImage();
			}
			m_viewImage.Draw(CPaintDC(this));
			return 0;
		case WM_ENABLE:
			return 0;
		case WM_SIZE:
			{
				CSize sz(LOWORD(lParam), HIWORD(lParam));
				if ( sz != m_clientSize )
				{
					CSize gsz = m_graphImage.GetSize();
					m_Resize(sz, gsz.cx);
					OndemandRemakeViewImage();
				}
			}
			break;
		case WM_HSCROLL:
			if ( reinterpret_cast<HWND>(lParam) == m_scrollBar )
			{
				int sbCode = LOWORD(wParam);
				int nPos = HIWORD(wParam);
				switch ( sbCode )
				{
				case SB_THUMBPOSITION: // 絶対位置へスクロール。現在位置はパラメータ nPos で指定されます。
				case SB_THUMBTRACK: // スクロール ボックスを指定位置へドラッグ。現在位置はパラメータ nPos で指定されます。
					{
						SCROLLINFO si;
						if ( m_scrollBar.GetScrollInfo(&si, SIF_TRACKPOS) )
						{
							nPos = si.nTrackPos;
						}
					}
					break;
				}
				m_OnScrollBar(sbCode, nPos);
			}
			break;
		case WM_LBUTTONDOWN:
			if ( ::IsWindow(m_scrollBar) )
			{
				m_isMouseButtonPressed = true;
				SetCapture();
				m_pressedPos = CPoint(lParam);
			}
			#ifdef _WIN32_WCE
				return _super::DefWindowProc(message, wParam, lParam);
			#endif
			break;
		case WM_LBUTTONUP:
			if ( m_isMouseButtonPressed )
			{
				m_isMouseButtonPressed = false;
				ReleaseCapture();
			}
			break;
		case WM_MOUSEMOVE:
			if ( m_isMouseButtonPressed )
			{
				if ( GetCapture() != this )
				{
					m_isMouseButtonPressed = false;
				}
				else
				{
					CPoint pos(lParam);
					int d = (pos.x - m_pressedPos.x) / m_horzIndexPixel;
					if ( d != 0 )
					{
						m_OnScrollBar(SB_THUMBPOSITION, m_scrollBar.GetScrollPos() - d);
						m_pressedPos = pos;
						Invalidate();
						UpdateWindow();
					}
				}
			}
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	/// テキスト情報
	struct TTextInfo
	{
		HFONT		hFont;	///< フォント
		CStr		text;	///< 文字列
		COLORREF	color;	///< 色
		//コンストラクタ
		TTextInfo(void) : hFont(NULL)
		{
		}
	};
	/// 線情報
	struct TLineInfo : CCopyImpossible
	{
		ELineStyle	style;		///< 実線、破線、太線、右のみ、左のみ
		CPen		colorPen;	///< 色のペン
		//コンストラクタ
		TLineInfo(void) : style(LineStyle_Non)
		{
		}
		void Set(ELineStyle	ls, COLORREF color)
		{
			style = ls;
			if ( colorPen.GetSafeHandle() != NULL )
			{
				colorPen.DeleteObject();
			}
			CPen* pPen = NULL;
			switch ( style )
			{
			case LineStyle_Solid:	// 実線
			case LineStyle_EdgeDot: // 両端のみ
				pPen = new CPen(PS_SOLID, 1, color);
				break;
			case LineStyle_Bold:	// 太線
				pPen = new CPen(PS_SOLID, 3, color);
				break;
			case LineStyle_Dot:	// 破線
				pPen = new CPen(PS_DOT, 1, color);
				break;
			}
			if ( pPen != NULL )
			{
				colorPen.Attach(pPen->Detach());
				delete pPen;
			}
		}
	};
	/// スケール線情報
	struct TScaleLineInfo : TLineInfo
	{
		double			step;	///< ステップ(０を中心としていくつおきに線を引くか。マイナスならなし)。
		void Set(ELineStyle	ls, COLORREF color, double sp)
		{
			step = sp;
			TLineInfo::Set(ls, color);
		}
	};
	/// スクロールバー
	class CMyScrollBar : public CScrollBar
	{
		DEFSUPER(CScrollBar);
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
		{
			if ( message == WM_LBUTTONDOWN )
			{
				return _super::DefWindowProc(message, wParam, lParam);
			}
			return _super::WindowProc(message, wParam, lParam);
		}
	};

	int m_CalcDrawY(double y) const
	{	
		double yy = (m_viewTopValue - y);
		yy *= (m_clientSize.cy - 1);
		yy /= m_viewHeight;
		return down_cast<int>(yy);
	}

	/// スクロールメッセージ処理
	void m_OnScrollBar(int sbCode, int nPos)
	{
		INT_PTR np = GetViewHorizontalPos();
		INT_PTR maxPos = m_scrollBar.GetScrollLimit();
		INT_PTR p = m_scrollBar.GetScrollPos();
		switch ( sbCode )
		{
		case SB_LEFT: // 左端へスクロール。
			p = 0;
			break;
		case SB_ENDSCROLL: // スクロール終了。
			return;
		case SB_LINELEFT: // 左へスクロール。
			p -= m_scrollOneStep;
			break;
		case SB_LINERIGHT: // 右へスクロール。
			p += m_scrollOneStep;
			break;
		case SB_PAGELEFT: // 1 ページ左へスクロール。
			p -= m_clientSize.cx / m_horzIndexPixel;
			break;
		case SB_PAGERIGHT: // 1 ページ右へスクロール。
			p += m_clientSize.cx / m_horzIndexPixel;
			break;
		case SB_RIGHT: // 右端へスクロール。
			p = maxPos;
			break;
		case SB_THUMBPOSITION: // 絶対位置へスクロール。現在位置はパラメータ nPos で指定されます。
		case SB_THUMBTRACK: // スクロール ボックスを指定位置へドラッグ。現在位置はパラメータ nPos で指定されます。
			p = nPos;
			break;
		default:
			ASSERT( false );
			break;
		}
		m_scrollBar.SetScrollPos(ToInt(p));
		p = m_scrollBar.GetScrollPos();
		if ( m_isReverseIndexMode )
		{
			p = m_scrollBar.GetScrollLimit() - p;
		}
		if ( np != p )
		{
			SetViewHorizontalPos(p);
			CWnd* pParent = GetParent();
			if ( pParent != NULL )
			{
				pParent->SendMessage(WM_HSCROLL, MAKELONG(SB_THUMBPOSITION, p), reinterpret_cast<LPARAM>(GetSafeHwnd()));
			}
		}
	}
	/**
	 * サイズ変更.
	 *	@param size コントロールのクライアント領域サイズ
	 *	@param graphWidht グラフのサイズ。クライアントより大きくても OK。
	 */
	void m_Resize(const SIZE& sz, size_t graphWidth)
	{
		if ( sz.cx < 0 || sz.cx > 0x7FFF || sz.cy < 0 || sz.cy > 0x7FFF )
		{
			return;
		}
		m_clientSize = sz;
		//== スクロールバーを消す
		if ( ::IsWindow(m_scrollBar) )
		{
			m_scrollBar.DestroyWindow();
		}
		if ( ToInt(graphWidth) > m_clientSize.cx )
		{
			//== スクロールバー用意
			DWORD style = SBS_HORZ | WS_CHILD;
			if ( m_isScrollBarUsed )
			{
				m_clientSize.cy -= ToInt(m_scrollBarHeight);
				style |= WS_VISIBLE;
			}
			m_scrollBar.Create(style, CRect(0, m_clientSize.cy, m_clientSize.cx, ToInt(m_clientSize.cy + m_scrollBarHeight)), this, 1000);
			SCROLLINFO si;
			m_scrollBar.GetScrollInfo(&si);
			si.nPage = m_clientSize.cx / m_horzIndexPixel;
			si.nMin = 0;
			si.nMax = ToInt(graphWidth / m_horzIndexPixel);
			if ( m_isReverseIndexMode )
			{
				si.nPos = si.nMax;
			}
			m_scrollBar.SetScrollInfo(&si);
		}
		m_viewImage.Set(sz.cx, sz.cy, m_backColor);
		m_graphImageDc.Detach();
		if ( sz.cx > 0 && sz.cy > 0 && m_clientSize.cy > 0 )
		{
			VERIFY( m_graphImage.Set(ToInt(graphWidth), m_clientSize.cy, INTERNALLY_COLOR) );
			m_graphImageDc.Attach(&m_graphImage);
		}
		m_pointMap.RemoveAll();
	}
	void m_Resize(void)
	{
		const CSize& gsz = m_graphImage.GetSize();
		CRect rc;
		GetClientRect(&rc);
		m_Resize(rc.Size(), gsz.cx);
	}
	/**
	 * マーカー描画
	 *	@param dc DC
	 *	@param xx 実座標。
	 *	@param y 縦軸値
	 *	@param color 色
	 *	@param withContinuousLine 連続線も一緒？
	 */
	void m_SetMaker(CDC& dc, int xx, double y, COLORREF color, bool withContinuousLine)
	{
		if ( ! IS_RGBVALUE(color) )
		{
			return;
		}
		if ( ! ::_finite(y) )
		{
			return;
		}
		int yy = m_CalcDrawY(y);
		bool hasColorKey = m_pointMap.HasKey(color);
		//== ブラシ、ペン、作成
		if ( ! hasColorKey )
		{
			m_pointMap[color] = new TPointInfo(color);
		}
		TPointInfo* P = m_pointMap[color];
		ASSERT( P != NULL );
		CPen* pOldPen = dc.SelectObject(&P->pen);
		{
			//== 連続線描画
			if ( withContinuousLine && hasColorKey )
			{
				dc.MoveTo(P->point);
				dc.LineTo(xx, yy);
			}
			//== マーカー描画
			if ( m_markerWidth <= 1 )
			{
				dc.SetPixel(xx, yy, color);
			}
			else if ( m_markerWidth <= 3 )
			{
				int mw = ToInt(m_markerWidth);
				int d1 = mw / 2;
				dc.FillSolidRect(xx - d1, yy - d1, mw, mw, color);
			}
			else
			{
				CBrush* pOldBrush = dc.SelectObject(&P->brush);
				int d1 = ToInt(m_markerWidth / 2);
				int d2 = ToInt(m_markerWidth - d1);
				dc.Ellipse(xx - d1, yy - d1, xx + d2, yy + d2);
				dc.SelectObject(pOldBrush);
			}
		}
		dc.SelectObject(pOldPen);
		//== 記憶
		P->point.x = xx;
		P->point.y = yy;
	}
	/**
	 * [計算] テンキー位置からDrawText位置属性へ
	 *	@param テンキー位置
	 *	@return DrawText位置
	 */
	DWORD m_ToDtStyle(INDEX idx) const
	{
		DWORD r = 0;
		switch ( idx )
		{
		case 1:	r = DT_LEFT		| DT_BOTTOM;	break;
		case 2:	r = DT_CENTER	| DT_BOTTOM;	break;
		case 3:	r = DT_RIGHT	| DT_BOTTOM;	break;
		case 4:	r = DT_LEFT		| DT_VCENTER;	break;
		case 5:	r = DT_CENTER	| DT_VCENTER;	break;
		case 6:	r = DT_RIGHT	| DT_VCENTER;	break;
		case 7:	r = DT_LEFT		| DT_TOP;		break;
		case 8:	r = DT_CENTER	| DT_TOP;		break;
		case 9:	r = DT_RIGHT	| DT_TOP;		break;
		}
		return r;
	}
	// 再構築
	void m_MakeViewImage(void)
	{
		const SIZE& sz = m_clientSize;
		CBitmapDC dc(&m_viewImage);
		dc.FillSolidRect(0, 0, sz.cx, sz.cy, m_backColor);
		//== Scale描画
		m_DrawScaleLine(dc, m_subScaleLines[2], false);
		m_DrawScaleLine(dc, m_subScaleLines[1], false);
		m_DrawScaleLine(dc, m_subScaleLines[0], false);
		m_DrawScaleLine(dc, m_scaleLine, true);
		//== グラフ描画（重ね合わせ）
		if ( ! m_graphImage.IsEmpty() )
		{
			const SIZE& sz = m_graphImage.GetSize();
			INT_PTR x = -down_cast<INT_PTR>(m_viewHorizontalPos);
			if ( m_isReverseIndexMode )
			{
				x = m_clientSize.cx - sz.cx + m_viewHorizontalPos;
			}
			::TransparentBlt(dc, ToInt(x), 0, sz.cx, sz.cy, m_graphImageDc, 0, 0, sz.cx, sz.cy, INTERNALLY_COLOR);
		}
		//== 文字描画
		{
			CRect rc(2, 2, sz.cx - 2, sz.cy - 2);
			CWnd* pParent = GetParent();
			CFont* pParentFont = GetFont();
			if ( pParent != NULL )
			{
				pParentFont = pParent->GetFont();
			}
			CFont* pOldFont = dc.SelectObject(GetFont());
			dc.SetBkMode(TRANSPARENT);
			loop ( i, 9 )
			{
				INDEX idx = i + 1;
				const TTextInfo& t = m_testInfo[idx];
				if ( ! t.text.IsEmpty() )
				{
					if ( t.hFont == NULL  )
					{
						dc.SelectObject(pParentFont);
					}
					else
					{
						::SelectObject(dc, t.hFont);
					}
					CTextDrawer::DrawTextRect(dc, rc, m_ToDtStyle(idx), t.color, t.text);
				}
			}
			dc.SelectObject(pOldFont);
		}
		//==
		OnMadeViewImage(&dc, sz);
		//==
		dc.Detach();
	}
	// 指定スタイル水平線描画
	void m_DrawStyleHorzLine(CDC& dc, bool isEdgeDotMode, int yy)
	{
		dc.MoveTo(0, yy);
		if ( ! isEdgeDotMode )
		{
			// 線
			dc.LineTo(m_clientSize.cx, yy);
		}
		else
		{
			// 両端のみ
			dc.LineTo(4, yy);
			dc.MoveTo(m_clientSize.cx, yy);
			dc.LineTo(m_clientSize.cx - 4, yy);
		}
	}
	// スケール線描画
	void m_DrawScaleLine(CDC& dc, TScaleLineInfo& sli, bool isMaster)
	{
		if ( sli.colorPen.GetSafeHandle() == NULL )
		{
			return;
		}
		bool isEdgeDotMode = (sli.style == LineStyle_EdgeDot);
		dc.SetBkColor(m_backColor);
		CPen* pOldPen = dc.SelectObject(&sli.colorPen);
		if ( isMaster )
		{
			m_DrawStyleHorzLine(dc, isEdgeDotMode, m_CalcDrawY(0));
		}
		else
		{
			double ss = sli.step;
			double hh = max(::fabs(m_viewTopValue), ::fabs(m_viewbottomValue));
			while ( ss <= hh )
			{
				m_DrawStyleHorzLine(dc, isEdgeDotMode, m_CalcDrawY(ss));
				m_DrawStyleHorzLine(dc, isEdgeDotMode, m_CalcDrawY(-ss));
				ss += sli.step;
			}
		}
		dc.SelectObject(pOldPen);
	}
	// ポイント管理型
	struct TPointInfo
	{
		CPoint	point;
		CBrush	brush;
		CPen	pen;
		TPointInfo(COLORREF c) : brush(c), pen(PS_SOLID, 1, c)
		{
		}
	};
	// ポイント管理マップクラス
	typedef CMapT<COLORREF, CPointerHandleT<TPointInfo> >  CMyPointMap;

	bool				m_isScrollBarUsed;			///< スクロールバー使用する？
	CMyScrollBar		m_scrollBar;				///< スクロールバーコントロール
	size_t				m_scrollBarHeight;			///< スクロールバー高さ
	size_t				m_scrollOneStep;			///< スクロールワンステップ幅
	TTextInfo			m_testInfo[10];				///< テキスト情報. [1]～[9] がそれぞれテンキーでいう上下左右の位置
	CMyPointMap			m_pointMap;					///< 色毎の最終ポイント座標
	TScaleLineInfo		m_scaleLine;				///< スケール線情報
	TScaleLineInfo		m_subScaleLines[3];			///< サブスケール線情報
	COLORREF			m_backColor;				///< 背景色
	size_t				m_markerWidth;				///< ポイントマーカー幅
	double				m_viewTopValue;				///< 表示上限。
	double				m_viewbottomValue;			///< 表示下限。
	double				m_viewHeight;				///< 表示範囲(m_viewTopValue - m_viewbottomValue + 1)
	bool				m_isReverseIndexMode;		///< 逆インデックスモード？
	CSize				m_clientSize;				///< クライアントサイズ
	CBitmapDC			m_graphImageDc;				///< グラフイメージの DC
	CBitmapImage		m_graphImage;				///< 表示イメージ（折れ線グラフのみ。高さはクライアント領域と同じ）
	CBitmapImage		m_viewImage;				///< 表示イメージ（文字やスケール線あり。大きさはクライアント領域と同じ）
	bool				m_isGraphImageDcLeading;	///< true ならDC貸し出し中
	INDEX				m_viewHorizontalPos;		///< 表示水平位置
	bool				m_isOndemandRemakeViewImage;///< 表示イメージ再作成要求
	int					m_horzIndexPixel;			///< 横軸インデックス１が何ピクセルか
	bool				m_isMouseButtonPressed;		///< プレス中？
	CPoint				m_pressedPos;				///< プレスされた座標
};



};//MFC
};//TNB




