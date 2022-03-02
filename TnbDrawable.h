#pragma once
/**
 *@file
 * 描画情報関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbPointerHandle.h"
#include "TnbDcSelectAssistant.h"



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE
 * 描画情報インターフェース
 *
 *		描画情報を管理しているインターフェースです。
 *
 *		本インターフェースを実装することで、 CDrawingContainer などで管理、処理できます。
 *
 *	@par必要ファイル
 *			TnbDrawable.h
 *
 *	@date 07/11/01 新規作成
 *	@date 07/11/19 HasRgn() 廃止
 *	@date 07/11/30 名前変更。
 */
struct IDrawable
{
	/// デストラクタ
	virtual ~IDrawable(void) { }

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const = 0;

	/**
	 * [取得] サイズ取得.
	 *		本オブジェクトの描画する領域の外接する四角のサイズを得ることが出来ます。
	 *	@param[out] _size サイズ取得。
	 *	@retval true サイズ取得成功。 
	 *	@retval false データがない。
	 */
	virtual bool GetSize(SIZE& _size) const = 0;

	/**
	 * [設定] サイズ設定.
	 *		本オブジェクトの描画する領域の外接する四角のサイズを設定することが出来ます。
	 *	@param size サイズ。
	 *	@retval true サイズ変更成功。 
	 *	@retval false 失敗。サイズ変更はサポートしません。
	 */
	virtual bool Resize(const SIZE& size) = 0;

	/**
	 * [描画] 描画.
	 *		指定のデバイスコンテキストの指定の座標に描画します。
	 *	@param dc デバイスコンテキスト
	 *	@param x Ｘ座標
	 *	@param y Ｙ座標
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const = 0;

	/**
	 * [描画] 描画.
	 *		指定のデバイスコンテキストの指定の座標に描画します。
	 *	@param dc デバイスコンテキスト
	 *	@param x Ｘ座標
	 *	@param y Ｙ座標
	 *	@param lParam LPARAM。
	 */
	virtual void DrawEx(HDC dc, int x, int y, LPARAM lParam) const
	{
		Draw(dc, x, y);
	}

	/// ポインタハンドル型宣言
	typedef CPointerHandleT<IDrawable> Ptr;
};



/**@ingroup DRAWABLE
 * NULL描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *		大きさのみを保持します。
 *
 *	@par必要ファイル
 *			TnbDrawable.h
 *
 *	@date 07/11/21 新規作成
 */
class CNullDrawer : public IDrawable
{
	DEFSUPER(IDrawable);
protected:
	SIZE			m_size;		///< サイズ
public:

	/**
	 * コンストラクタ
	 *	@param size サイズ。
	 */
	CNullDrawer(const SIZE& size) : m_size(size)
	{
	}

	/**
	 * コンストラクタ.
	 *	@note サイズは 0, 0 になります。
	 */
	CNullDrawer(void)
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
		return new CNullDrawer(m_size); 
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
		_size = m_size;
		return true;
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
		m_size = size;
		return true;
	}

	/**
	 * [描画] 描画.
	 *		指定のデバイスコンテキストの指定の座標に描画します。
	 *	@note 本クラスは何もしません。
	 *	@param dc デバイスコンテキスト
	 *	@param x Ｘ座標
	 *	@param y Ｙ座標
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const
	{
	}
};



/**@ingroup DRAWABLE
 * 描画情報保持描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		指定された描画情報二つを保持します。
 *		二つを並べて表示します。
 *
 *	@par必要ファイル
 *			TnbDrawable.h
 *
 *	@date 08/09/26 新規作成
 */
class CPairDrawer : public IDrawable
{
	DEFSUPER(IDrawable);
	IDrawable::Ptr	m_pDrawLeft;	///< 左描画情報
	IDrawable::Ptr	m_pDrawRight;	///< 右描画情報
	size_t			m_orgLeftWidth;	///< 左の描画情報のオリジナル幅
	SIZE			m_size;			///< サイズ

public:

	/**
	 * コンストラクタ
	 *	@param drawLeft 左の描画情報。複製を本クラスで保持します。オリジナル幅を使いますので、正しいサイズを指定しておく必要があります。
	 *	@param drawRight 右の描画情報。複製を本クラスで保持します。
	 */
	CPairDrawer(const IDrawable& drawLeft, const IDrawable& drawRight)
	{
		m_pDrawLeft = drawLeft.Clone();
		m_pDrawRight = drawRight.Clone();
		SIZE orgSize;
		m_pDrawLeft->GetSize(orgSize);
		m_orgLeftWidth = orgSize.cx;
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
		CPairDrawer* P = new CPairDrawer(*this);
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
		_size = m_size;
		return true;
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
		m_size = size;
		SIZE s1 = { min(ToInt(m_orgLeftWidth), size.cx), size.cy };
		SIZE s2 = { max(down_cast<LONG>(size.cx - m_orgLeftWidth), 0), size.cy };
		m_pDrawLeft->Resize(s1);
		m_pDrawRight->Resize(s2);
		return true;
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
		SIZE s1;
		m_pDrawLeft->GetSize(s1);
		m_pDrawLeft->DrawEx(dc, x, y + (m_size.cy - s1.cy) / 2, lParam);
		SIZE s2;
		m_pDrawRight->GetSize(s2);
		m_pDrawRight->DrawEx(dc, x + s1.cx, y + (m_size.cy - s2.cy) / 2, lParam);
	}
};



/**@ingroup DRAWABLE
 * 描画情報保持描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		指定された描画情報二つを保持します。
 *		ベース描画情報の上に、指定のOFFSETをずらしたオーバー描画情報を持つようにします。
 *
 *	@par必要ファイル
 *			TnbDrawable.h
 *
 *	@date 07/11/26 新規作成
 */
class COffsetDrawer : public IDrawable
{
	DEFSUPER(IDrawable);
	IDrawable::Ptr	m_pDrawBase;	///< ベース描画情報
	POINT			m_offsetOver;	///< オーバー描画情報オフセット
	IDrawable::Ptr	m_pDrawOver;	///< オーバー描画情報
	SIZE			m_size;			///< サイズ
	void m_CalcSize(void)
	{
		m_size.cx = 0;
		m_size.cy = 0;
		SIZE s1;
		SIZE s2;
		if ( m_pDrawBase->GetSize(s1) && m_pDrawOver->GetSize(s2) )
		{
			RECT r1 = { 0, 0, s1.cx, s1.cy };
			RECT r2 = { m_offsetOver.x, m_offsetOver.y, m_offsetOver.x + s2.cx, m_offsetOver.y + s2.cy };
			if ( ::UnionRect(&r1, &r1, &r2) )
			{
				m_size.cx = r1.right - r1.left;
				m_size.cy = r1.bottom - r1.top;
			}
		}
	}

public:

	/**
	 * コンストラクタ
	 *	@param drawBase ベース描画情報。複製を本クラスで保持します。
	 *	@param off オーバー描画情報のずれ
	 *	@param drawOver オーバー描画情報。複製を本クラスで保持します。
	 */
	COffsetDrawer(const IDrawable& drawBase, const POINT& off, const IDrawable& drawOver)
		: m_offsetOver(off)
	{
		m_pDrawBase = drawBase.Clone();
		m_pDrawOver = drawOver.Clone();
		m_CalcSize();
	}

	/**
	 * コンストラクタ.
	 *	@note 
	 *	@param size サイズ。
	 *	@param off 描画情報のずれ
	 *	@param draw 描画情報。複製を本クラスで保持します。
	 */
	COffsetDrawer(const SIZE& size, const POINT& off, const IDrawable& draw)
		: m_offsetOver(off)
	{
		m_pDrawBase = new CNullDrawer(size);
		m_pDrawOver = draw.Clone();
		m_CalcSize();
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		return new COffsetDrawer(*m_pDrawBase, m_offsetOver, *m_pDrawOver);
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
		_size = m_size;
		return true;
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
		if ( ! m_pDrawBase.IsNull() ) 
		{
			m_pDrawBase->DrawEx(dc, x, y, lParam);
		}
		if ( ! m_pDrawOver.IsNull() ) 
		{
			m_pDrawOver->DrawEx(dc, x + m_offsetOver.x, y + m_offsetOver.y, lParam);
		}
	}
};



/**@ingroup DRAWABLE
 * 四角描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		コンストラクタ時に指定された色で四角を描画します。
 *
 *	@par必要ファイル
 *			TnbDrawable.h
 *
 *	@date 07/11/15 新規作成
 *	@date 08/03/23 コンストラクタを追加。
 */
class CBoxDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
	COLORREF	m_color;	///< 色
	int			m_width;	///< 太さ
public:

	/**
	 * コンストラクタ
	 *	@param size サイズ。
	 *	@param color 色。
	 *	@param width 線の幅。省略すると１．
	 */
	CBoxDrawer(const SIZE& size, COLORREF color, int width = 1) 
		: _super(size), m_color(color), m_width(width)
	{
	}

	/**
	 * コンストラクタ
	 *	@note サイズは 0, 0 になります。
	 *	@param color 色。
	 *	@param width 線の幅。省略すると１．
	 */
	CBoxDrawer(COLORREF color, int width = 1) 
		: _super(), m_color(color), m_width(width)
	{
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CBoxDrawer(m_size, m_color, m_width);
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
		int z1 = m_width / 2;
		int z2 = (m_width + 1) / 2 - 1;
		HPEN pen = ::CreatePen(PS_SOLID, m_width, m_color);
		CDcSelectAssistant dca(dc);
//		HGDIOBJ oldPen = ::SelectObject(dc, pen);
//		HGDIOBJ oldBrush = ::SelectObject(dc, ::GetStockObject(NULL_BRUSH));
		dca.SelectPen(pen);
		dca.SelectBrush(static_cast<HBRUSH>(::GetStockObject(NULL_BRUSH)));
		::RoundRect(dc, x + z1, y + z1, m_size.cx + x - z2, m_size.cy + y - z2, 0, 0);
//		::SelectObject(dc, oldBrush);
//		::SelectObject(dc, oldPen);
		dca.Restore();
		_DeleteObject(pen);
	}
};



/**@ingroup DRAWABLE
 * 四角塗りつぶし描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		コンストラクタ時に指定された色で四角を描画します。
 *
 *	@par必要ファイル
 *			TnbDrawable.h
 *
 *	@date 07/11/15 新規作成
 *	@date 08/03/23 コンストラクタを追加。
 */
class CBoxFillDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
	COLORREF	m_color;	///< 色
public:

	/**
	 * コンストラクタ
	 *	@param size サイズ。
	 *	@param color 色。
	 */
	CBoxFillDrawer(const SIZE& size, COLORREF color) : _super(size), m_color(color)
	{
	}

	/**
	 * コンストラクタ
	 *	@note サイズは 0, 0 になります。
	 *	@param color 色。
	 */
	CBoxFillDrawer(COLORREF color) : _super(), m_color(color)
	{
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CBoxFillDrawer(m_size, m_color);
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
		RECT rc = { x, y, x + m_size.cx, y + m_size.cy };
		COLORREF c = ::GetBkColor(dc);
		::SetBkColor(dc, m_color);
		::ExtTextOut(dc, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
		::SetBkColor(dc, c);
	}
};



/**@ingroup DRAWABLE
 * 反転描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		範囲の色を反転します。
 *
 *	@par必要ファイル
 *			TnbDrawable.h
 *
 *	@date 07/12/13 新規作成
 *	@date 08/03/23 コンストラクタを追加。
 */
class CBoxInverter : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
public:

	/**
	 * コンストラクタ
	 *	@param size サイズ。
	 */
	CBoxInverter(const SIZE& size) : _super(size)
	{
	}

	/**
	 * コンストラクタ
	 *	@note サイズは 0, 0 になります。
	 */
	CBoxInverter(void) : _super()
	{
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CBoxInverter(m_size);
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
		RECT rc = { x, y, x + m_size.cx, y + m_size.cy };
		::InvertRect(dc, &rc);
	}
};



/**@ingroup DRAWABLE
 * フレーム描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		フレームを描画します。
 *
 *	@par必要ファイル
 *			TnbDrawable.h
 *
 *	@date 08/10/10 コンストラクタを追加。
 */
class CFrameDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
	bool		m_isFrameMode;
	UINT		m_param1;
	UINT		m_param2;
public:

	/**
	 * コンストラクタ
	 *	@param size サイズ。
	 *	@param isFrameMode true なら DrawFrameControl() 、false なら DrawEdge() を使います。
	 *	@param p1 パラメータ１
	 *	@param p2 パラメータ２
	 */
	CFrameDrawer(const SIZE& size, bool isFrameMode, UINT p1, UINT p2) 
		: _super(size), m_isFrameMode(isFrameMode), m_param1(p1), m_param2(p2)
	{
	}

	/**
	 * コンストラクタ
	 *	@note サイズは 0, 0 になります。
	 *	@param isFrameMode true なら DrawFrameControl() 、false なら DrawEdge() を使います。
	 *	@param p1 パラメータ１
	 *	@param p2 パラメータ２
	 */
	CFrameDrawer(bool isFrameMode, UINT p1, UINT p2) 
		: _super(), m_isFrameMode(isFrameMode), m_param1(p1), m_param2(p2)
	{
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CFrameDrawer(*this);
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
		RECT rc = { x, y, x + m_size.cx, y + m_size.cy };
		m_isFrameMode ? ::DrawFrameControl(dc, &rc, m_param1, m_param2) : ::DrawEdge(dc, &rc, m_param1, m_param2);
	}
};



/**@ingroup DRAWABLE
 * フォーカスフレーム描画クラス
 *
 *		IDrawable インターフェースを持っています。
 *
 *		フォーカスフレームを描画します。
 *
 *	@par必要ファイル
 *			TnbDrawable.h
 *
 *	@date 07/12/13 新規作成
 *	@date 08/03/23 コンストラクタを追加。
 */
class CFocusFrameDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
public:

	/**
	 * コンストラクタ
	 *	@param size サイズ。
	 */
	CFocusFrameDrawer(const SIZE& size) : _super(size)
	{
	}

	/**
	 * コンストラクタ
	 *	@note サイズは 0, 0 になります。
	 */
	CFocusFrameDrawer(void) : _super()
	{
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CFocusFrameDrawer(m_size);
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
		RECT rc = { x, y, x + m_size.cx, y + m_size.cy };
		::DrawFocusRect(dc, &rc);
	}
};



};///TNB

