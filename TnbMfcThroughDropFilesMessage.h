#pragma once
/**
 *@file
 * ファイルドロップメッセージ透過処理関係のヘッダ
 *
 *		コントロールから親にファイルドロップメッセージを透過させる機能を提供します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * ファイルドロップメッセージ透過アドインクラス
 *
 *		本クラスを使用すると、ドロップされたメッセージを親に透過することが可能になります。
 *
 *	@par使用例
 *	\code
 *		// .h
 *		typedef CThroughDropFilesMessageAddinT<CStatic> CMyStatic;
 *
 *		class CFooDlg : public CDialog
 *		{
 *			CMyStatic	m_staticDrop;			// 元々 CStatic だったメンバを書き換えた
 *					:
 *	\endcode
 *	\code
 *		// .cpp
 *		BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
 *				;
 *			ON_WM_DROPFILES()
 *				;
 *
 *		void CFooDlg::OnDropFiles(HDROP hDropInfo)
 *		{
 *			if ( ! m_staticDrop.IsDroped() )	// m_staticDrop にドロップされたやつ？
 *			{
 *				return;
 *			}
 *			UINT count = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
 *			CStr fileName;
 *			::DragQueryFile(hDropInfo, 0, fileName.GetBuffer(MAX_PATH - 1), MAX_PATH);
 *			fileName.ReleaseBuffer();
 *				;
 *	\endcode
 *
 *	@par必要ファイル
 *			TnbMfcThroughDropFilesMessage.h
 * 
 *	@date 12/03/02 新規作成
 */
template<typename TYP = CStatic>
class CThroughDropFilesMessageAddinT : public TYP
{
	DEFSUPER(TYP);
public:

	/// コンストラクタ
	CThroughDropFilesMessageAddinT(void) : m_isDroped(false)
	{
	}

	/**
	 * [確認] ドロップされた？
	 *	@note 本メソッドは親ウィンドウに WM_DROPFILES が通知された時に使用します。
	 *	@retval true ドロップされた
	 *	@retval false 否
	 */
	bool IsDroped(void) const
	{
		return m_isDroped;
	}

	/**
	 * [取得] ドロップ数取得.
	 *	@param hDrop ドロップハンドル
	 *	@return ドロップ数
	 */
	static size_t QueryDragFileCount(HDROP hDrop)
	{
		return ::DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	}

	/**
	 * [取得] ドロップファイル名取得.
	 *	@param hDrop ドロップハンドル
	 *	@param index インデックス。 0以上、 QueryDragFileCount() 未満。
	 *	@retval empty文字列 失敗。 index の値がおかしい。
	 *	@retval 上記以外 成功。内容はファイル名。
	 */
	static CStr QueryDragFileName(HDROP hDrop, INDEX index)
	{
		CStr s;
		size_t count =QueryDragFileCount(hDrop);
		if ( count > index )
		{
			::DragQueryFile(hDrop, index, s.GetBuffer(MAX_PATH), MAX_PATH);
			s.ReleaseBuffer();
		}
		return s;
	}

protected:

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		m_isDroped = false;
		::DragAcceptFiles(_super::GetSafeHwnd(), TRUE);
		_super::PreSubclassWindow();
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
		if ( message == WM_DROPFILES )
		{
			CWnd* P = _super::GetParent();
			if ( P != NULL )
			{
				m_isDroped = true;
				LRESULT r = P->SendMessage(message, wParam, lParam);
				m_isDroped = false;
				return r;
			}
		}
		return _super::WindowProc(message, wParam, lParam);
	}
private:
	bool	m_isDroped;	///< ドロップされた
};



}; //MFC
}; //TNB



