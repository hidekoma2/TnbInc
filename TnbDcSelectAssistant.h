#pragma once
/**
 *@file
 * デバイステキストセレクトアシスタント関係のヘッダ
 *
 *		HDC に対して SelectObject など管理をアシストします。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * デバイステキストセレクトアシスタント
 *
 *
 *	@par必要ファイル
 *			TnbDcSelectAssistant.h
 *
 *	@date	10/05/13 新規作成。
 */
class CDcSelectAssistant : CCopyImpossible
{
public:

	/**
	 * コンストラクタ.
	 *	@param dc アシスト対象のデバイスコンテキスト.
	 */
	CDcSelectAssistant(HDC dc) : m_dc(dc) 
		, m_textColor(CLR_INVALID), m_backColor(CLR_INVALID), m_backMode(-1)
		, m_font(NULL), m_brush(NULL), m_pen(NULL), m_bitmap(NULL)
	{
	}

	/**
	 * デストラクタ.
	 *	@note Restore() がコールされます。
	 */
	~CDcSelectAssistant(void)
	{
		Restore();
	}

	/**
	 * [設定] リストア.
	 *		本インスタンス有効中に選択/設定された内容を元に戻します。
	 */
	void Restore(void)
	{
		if ( m_textColor != CLR_INVALID )
		{
			::SetTextColor(m_dc, m_textColor);
			m_textColor = CLR_INVALID;
		}
		if ( m_backColor != CLR_INVALID )
		{
			::SetBkColor(m_dc,m_backColor);
			m_backColor = CLR_INVALID;
		}
		if ( m_backMode >= 0 )
		{
			::SetBkMode(m_dc, m_backMode);
			m_backMode = -1;
		}
		if ( m_font != NULL )
		{
			::SelectObject(m_dc, m_font);
			m_font = NULL;
		}
		if ( m_brush != NULL )
		{
			::SelectObject(m_dc, m_brush);
			m_brush = NULL;
		}
		if ( m_pen != NULL )
		{
			::SelectObject(m_dc, m_pen);
			m_pen = NULL;
		}
		if ( m_bitmap != NULL )
		{
			::SelectObject(m_dc, m_bitmap);
			m_bitmap = NULL;
		}
	}

	/**
	 * [設定] 文字色設定. 
	 *	@param c カラー
	 */
	void SetTextColor(COLORREF c)
	{
		COLORREF r = ::SetTextColor(m_dc, c);
		if ( m_textColor == CLR_INVALID )
		{
			m_textColor = r;
		}
	}

	/**
	 * [設定] 背景色設定. 
	 *	@param c カラー
	 */
	void SetBkColor(COLORREF c)
	{
		COLORREF r = ::SetBkColor(m_dc, c);
		if ( m_backColor == CLR_INVALID )
		{
			m_backColor = r;
		}
	}

	/**
	 * [設定] 背景モード設定. 
	 *	@param mode モード。 TRANSPARENT か OPAQUE を指定できます。
	 */
	void SetBkMode(int mode)
	{
		int r = ::SetBkMode(m_dc, mode);
		if ( m_backMode < 0 )
		{
			m_backMode = r;
		}
	}

	/**
	 * [選択] ブラシ選択. 
	 *	@param brush ブラシ
	 */
	void SelectBrush(HBRUSH brush)
	{
		HGDIOBJ r = ::SelectObject(m_dc, brush);
		if ( m_brush == NULL )
		{
			m_brush = static_cast<HBRUSH>(r);
		}
	}

	/**
	 * [選択] ペン選択. 
	 *	@param pen ペン
	 */
	void SelectPen(HPEN pen)
	{
		HGDIOBJ r = ::SelectObject(m_dc, pen);
		if ( m_pen == NULL )
		{
			m_pen = static_cast<HPEN>(r);
		}
	}

	/**
	 * [選択] 画像選択. 
	 *	@param bitmap 画像
	 */
	void SelectBitmap(HBITMAP bitmap)
	{
		HGDIOBJ r = ::SelectObject(m_dc, bitmap);
		if ( m_bitmap == NULL )
		{
			m_bitmap = static_cast<HBITMAP>(r);
		}
	}

	/**
	 * [選択] フォント選択. 
	 *	@param font フォント
	 */
	void SelectFont(HFONT font)
	{
		HGDIOBJ r = ::SelectObject(m_dc, font);
		if ( m_font == NULL )
		{
			m_font = static_cast<HFONT>(r);
		}
	}

	#ifdef __AFX_H__

	/**
	 * [選択] フォント選択. 
	 *	@param pFont フォント
	 */
	void SelectFont(CFont* pFont)
	{
		if ( pFont != NULL )
		{
			SelectFont(static_cast<HFONT>(pFont->GetSafeHandle()));
		}
	}

	#endif

private:
	HDC			m_dc;			///< 管理する DC
	COLORREF	m_textColor;	///< 文字カラー
	COLORREF	m_backColor;	///< 背景カラー
	int			m_backMode;		///< 背景モード
	HFONT		m_font;			///< フォント
	HBRUSH		m_brush;		///< ブラシ
	HPEN		m_pen;			///< ペン
	HBITMAP		m_bitmap;		///< 画像
};



};//TNB
