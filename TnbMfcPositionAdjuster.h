#pragma once
/**
 *@file
 * コントロール位置の調整関係のヘッダ
 *
 *		IDrawable を利用したコントロール調整用クラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"
#include "TnbDrawable.h"



//TNB Library
namespace TNB
{



/**@ingroup DRAWABLE
 * コントロール位置調整用描画クラス
 *
 *		IDrawable インターフェースを持っていますが、描画しません。
 *		描画位置情報を利用して、コントロールの位置を制御します。
 *
 *		コンストラクタ時に指定されたコントロールを、指定の位置に移動します。
 *
 *	@par必要ファイル
 *			TnbMfcPositionAdjuster.h
 *
 *	@date 07/12/05 新規作成
 *	@date 08/09/19 MFCLIB::SetProcessingFlag() を使うようにした。
 */
class CPositionAdjuster : public IDrawable
{
	DEFSUPER(IDrawable);
protected:

	CWnd*	m_pWnd;			///< コントロール
	bool	m_boCanResize;	///< true ならリサイズ対応
	bool	m_boAlways;		///< true なら DC がNULL 以外でも配置変更
public:

	/**
	 * コンストラクタ
	 *	@param pWnd 対象ウィンドウ(コントロール)。
	 *	@param canResize 省略か true ならリサイズ対応。 false ならリサイズは禁止します。
	 *	@param boAlways	true なら DC がNULL 以外でも配置変更。省略か false なら dc が NULL の場合のみ調整します。
	 */
	CPositionAdjuster(CWnd*	pWnd, bool canResize = true, bool boAlways = false)
		: m_pWnd(pWnd), m_boCanResize(canResize), m_boAlways(boAlways)
	{
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CPositionAdjuster(m_pWnd, m_boCanResize, m_boAlways);
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
		if ( m_pWnd != NULL )
		{
			CRect rc;
			m_pWnd->GetWindowRect(&rc);
			_size.cx = rc.Width();
			_size.cy = rc.Height();
			return true;
		}
		return false;
	}

	/**
	 * [設定] サイズ設定.
	 *		本オブジェクトの描画する領域の外接する四角のサイズを設定することが出来ます。
	 *	@note 本クラスは管理しているWindowのサイズを変更します。
	 *	@param size サイズ。
	 *	@retval true サイズ変更成功。 
	 *	@retval false 失敗。ウィンドウが生成されていません。
	 */
	virtual bool Resize(const SIZE& size)
	{
		if ( m_boCanResize && m_pWnd != NULL )
		{
			return !! m_pWnd->SetWindowPos(NULL, 0, 0, size.cx, size.cy, SWP_NOMOVE | SWP_NOZORDER);
		}
		return false;
	}
	
	/**
	 * [描画] 描画.
	 *		描画せず、指定のコントロールを指定の位置に移動させます。
	 *	@note 本クラスの描画は、 dc が NULL の時のみ動作します。
	 *	@param dc デバイスコンテキスト
	 *	@param x Ｘ座標
	 *	@param y Ｙ座標
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const
	{
		if ( m_boAlways || dc == NULL )
		{
			CRect rc;
			m_pWnd->GetWindowRect(&rc);
			m_pWnd->MoveWindow(x, y, rc.Width(), rc.Height(), false);
			m_pWnd->RedrawWindow();
			MFCLIB::SetProcessingFlag(m_pWnd->GetSafeHwnd(), true);
		}
	}
};



};///TNB

