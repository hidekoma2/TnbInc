#pragma once
/**
 *@file
 * ビットマップDC関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbBitmapImage.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup WINAPI BITMAP
 * ビットマップデバイスコンテキストクラス
 *
 *		CBitmapImage を保持し、 CDC クラス同等に操作できるようにするラッパクラスです。
 *
 *	@note 構築時に CBitmapImage::GetDC() 関数を実行し、破棄時に CBitmapImage::ReleaseDC() 関数を実行します。 
 *		本インスタンスで使用中の CBitmapImage オブジェクトは、基本的に操作しないようにします。
 *
 *	@par必要ファイル
 *			TnbMfcBitmapDC.h
 * 
 *	@date 07/10/19 新規作成
 *	@date 07/11/13 アタッチ、デタッチ、追加
 *	@date 07/11/25 Draw() 追加
 *	@date 11/05/19 デフォルトコンストラクタ、用意。
 */
class CBitmapDC : public CDC
{
	DEFSUPER(CDC);
public:

	/**
	 * コンストラクタ
	 *	@param pBI 正しいイメージを持つ CBitmapImage オブジェクトへのポインタ。
	 *	@throw CResourceException 不正な CBitmapImage を渡した時、通知されます。
	 */
	explicit CBitmapDC(CBitmapImage* pBI) : m_pBitImg(pBI)
	{
		if ( m_pBitImg != NULL && _super::Attach(m_pBitImg->GetDC()) )
		{
		}
		else
		{
			TRACE0(" CBitmapDC 有効でない CBItmapImage を指定しました。\n" );
			AfxThrowResourceException();
		}
	}

	/**
	 * コンストラクタ.
	 *	@note DC をアタッチされていないインスタンスになりますので、必ず、 Attach() を最初に使ってください。
	 */
	explicit CBitmapDC(void) : m_pBitImg(NULL)
	{
	}

	/// デストラクタ
	virtual ~CBitmapDC(void)
	{
		Detach();
	}

	/**
	 * [処理] イメージ描画.
	 *		指定のデバイスコンテキストの指定の位置に、本インスタンスで持っているイメージを描画します。
	 *	@param dc 描画対象のデバイスコンテキスト。
	 *	@param x 描画X位置(左が0)。省略すると０。
	 *	@param y 描画Y位置(上が0)。省略すると０。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Draw(HDC dc, int x = 0, int y = 0) const
	{
		if ( m_pBitImg != NULL )
		{	
			const SIZE& size = m_pBitImg->GetSize();
			return !! ::BitBlt(dc, x, y, size.cx, size.cy, *this, 0, 0, SRCCOPY);
		}
		return false;
	}

	/**
	 * [設定] アタッチ
	 *	@param pBI 正しいイメージを持つ CBitmapImage オブジェクトへのポインタ。
	 *			通常、 Detach() したポインタを指定します。
	 *	@retval true 成功。
	 *	@retval false 失敗。不正な CBitmapImage を渡した時、通知されます。
	 */
	bool Attach(CBitmapImage* pBI)
	{
		Detach();
		if ( pBI != NULL && _super::Attach(pBI->GetDC()) )
		{
			m_pBitImg = pBI;
			return true;
		}
		return false;
	}

	/**
	 * [設定] デタッチ.
	 *	@note デタッチしたらそのポインタを Attach() します。
	 *	@retval NULL以外 成功。CBitmapImage のポインタです。破棄してはいけません。
	 *	@retval NULL 失敗。不正な CBitmapImage が渡されていませんでした。
	 */
	CBitmapImage* Detach(void)
	{
		CBitmapImage* P = m_pBitImg;
		_super::Detach();
		if ( m_pBitImg != NULL )
		{
			m_pBitImg->ReleaseDC();
		}
		m_pBitImg = NULL;
		return P;
	}

private:
	CBitmapImage*	m_pBitImg;	///< 管理する BitmapImage 
};



}; //MFC
}; //TNB


