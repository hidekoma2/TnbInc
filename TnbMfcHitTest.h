#pragma once
/**
 *@file
 * 領域ヒットテストクラス関係のヘッダ
 *
 *		領域のヒットテストを管理するクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"
#include "TnbSimpleVector.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * 領域ヒットテストサポートクラス
 *
 *		通常、コントロールにアドインして使用します。
 *
 *	@par必要ファイル
 *			TnbMfcHitTest.h
 * 
 *	@date 09/04/13 新規作成。
 */
template<typename WND = CWnd>
class CHitTestAddinT : public WND
{
	DEFSUPER(WND);
public:
	/// コンストラクタ
	CHitTestAddinT(void) : _super()
	{
	}

	/// デストラクタ
	virtual ~CHitTestAddinT(void)
	{
		DeleteAllAreas();
	}

	/**
	 * [追加] エリア設定.
	 *	@param id ID。
	 *	@param rgn リージョン。
	 *	@retval false 失敗。
	 *	@retval true 成功。
	 */
	bool AddArea(INDEX id, HRGN rgn)
	{
		HRGN h = ::CreateRectRgn(0, 0, 0, 0);
		if ( ::CombineRgn(h, rgn, NULL, RGN_COPY) == ERROR )
		{
			return false;
		}
		TParam p = { id, h };
		return m_areas.Add(p) != INVALID_INDEX;
	}

	/**
	 * [追加] エリア設定.
	 *	@param id ID。
	 *	@param rc 範囲。
	 *	@retval false 失敗。
	 *	@retval true 成功。
	 */
	bool AddArea(INDEX id, const RECT& rc)
	{
		HRGN h = ::CreateRectRgnIndirect(&rc);
		if ( h == NULL )
		{
			return false;
		}
		TParam p = { id, h };
		return m_areas.Add(p) != INVALID_INDEX;
	}

	/**
	 * [削除] 全エリア情報破棄
	 */
	void DeleteAllAreas(void)
	{
		loop ( i, m_areas.GetSize() )
		{
			_DeleteObject(m_areas[i].hRgn);
		}
		m_areas.RemoveAll();
	}

	/**
	 * [確認] ヒットテスト
	 *	@param po ポイント
	 *	@retval INVALID_INDEX どのエリアにも含まれていなかった
	 *	@retval 上記以外 ヒットしたエリアのインデックス
	 */
	INDEX HitTest(const POINT& po) const
	{
		loop ( i, m_areas.GetSize() )
		{
			if ( ::PtInRegion(m_areas[i].hRgn, po.x, po.y) )
			{
				return m_areas[i].id;
			}
		}
		return INVALID_INDEX;
	}

protected:
	/// 管理パラメータ
	struct TParam
	{
		INDEX	id;		///< ID
		HRGN	hRgn;	///< リージョン
	};
	CSimpleVectorT<TParam> m_areas;		///< エリア管理
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
		case WM_GETDLGCODE:
			break;
		default:
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}
	/**
	 * [確認] ヒットテスト.
	 *	@param point テストするクライアント座標
	 *	@param pTI NULLを指定しておいてください。
	 *	@retval -1 ヒット無し。
	 *	@retval 0以上 ヒント発見。
	 */
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const
	{
		INDEX idx = HitTest(point);
		if ( idx != INVALID_INDEX )
		{
			return idx;
		}
		return -1;
	}
};



}; //MFC
}; //TNB



