#pragma once
/**
 *@file
 * コントロール位置の調整関係のヘッダ
 *
 *		IDrawable を利用したコントロール調整用クラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcPositionAdjuster.h"
#include "TnbPointerHandle.h"



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
 *			TnbMfcPositionAdjusterEx.h
 *
 *	@date 08/09/17 新規作成
 */
class CPositionAdjusterEx : public CPositionAdjuster
{
	DEFSUPER(CPositionAdjuster);
protected:

	CPointerHandleT<CWnd> m_phWnd;			///< コントロール

public:

	/**
	 * コンストラクタ
	 *	@param pWnd 対象ウィンドウ(コントロール)。  new で作成したオブジェクトを渡します。
	 *	@param canResize 省略か true ならリサイズ対応。 false ならリサイズは禁止します。
	 *	@param boAlways	true なら DC がNULL 以外でも配置変更。省略か false なら dc が NULL の場合のみ調整します。
	 */
	CPositionAdjusterEx(CWnd* pWnd, bool canResize = true, bool boAlways = false)
		: m_phWnd(pWnd), _super(pWnd, canResize, boAlways)
	{
	}

	/**
	 * [作成] クローン作成.
	 *		自分と同じ処理を行うクラスを作成します。
	 *	@return ポインタ。使用後は delete する必要があります。
	 */
	virtual IDrawable* Clone(void) const
	{
		CPositionAdjusterEx* P = new CPositionAdjusterEx(NULL);
		P->m_phWnd = m_phWnd;
		P->m_pWnd = m_pWnd;
		P->m_boCanResize = m_boCanResize;
		P->m_boAlways = m_boAlways;
		return P;
	}
};



};///TNB

