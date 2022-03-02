#pragma once
/**
 *@file
 * ファイル選択ダイアログ関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStrVector.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * 履歴付き EDITコントロール
 *
 *		履歴付きのEDITコントロールを提供します。
 *
 *	@par使い方
 *		リソースエディタでコンボボックスコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CComboBox）を本クラスに置き換えて使用します。
 *
 *	@note ドロップダウンにし、ソートは false にします。
 *		
 *	@param EDT EDITコントロールのクラス。省略すると CEdit
 *	@param CMB コンボボックスコントロールのクラス。省略すると CComboBox
 *
 *	@par必要ファイル
 *			TnbMfcHistoryComboEdit.h
 * 
 *	@date 09/06/30	新規作成
 */
template<typename EDT = CEdit, typename CMB = CComboBox>
class CHistoryComboEditT : public CMB
{
	DEFSUPER(CMB);
public:

	/// コンストラクタ
	CHistoryComboEditT(void) : m_depth(10)
	{
	}

	/**
	 * [設定] 履歴数設定.
	 *		履歴を何個まで残すか、設定します。設定していないと 10 です。
	 *	@param depth 履歴数
	 */
	void SetHistoryDepth(int depth)
	{
		m_depth = depth;
	}

	/**
	 * [取得] 全履歴文字列取得.
	 *	@return 全履歴文字列。
	 */
	CStrVector GetHistoryStrings(void) const
	{
		CString s;
		CStrVector vs;
		loop ( i, _super::GetCount() )
		{
			_super::GetLBText(i, s);
			vs.Add(CStr(s));
		}
		return vs;
	}

	/**
	 * [取得] 全文字列取得.
	 *		先頭に Editコントロールの文字列、以下、履歴文字列が並びます。
	 *	@return 全文字列。
	 */
	CStrVector GetAllStrings(void) const
	{
		CStrVector vs;
		CString s;
		_super::GetWindowText(s);
		vs.Add(CStr(s));
		vs += GetHistoryStrings();
		return vs;
	}

	/**
	 * [設定] 全履歴文字列設定.
	 *	@note Editコントロールは変化しません。
	 *	@param vs 全履歴文字列。
	 */
	void SetHistoryStrings(const CStrVector& vs)
	{
		CString s;
		_super::GetWindowText(s);
		_super::ResetContent();
		loop ( i, vs.GetSize() )
		{
			_super::AddString(vs[i]);
		}
		m_CheckDepth();
		_super::SetWindowText(s);
	}

	/**
	 * [設定] 全文字列設定.
	 *		GetAllStrings() で得た文字列群を設定します。
	 *	@param vs 全文字列。
	 */
	void SetAllStrings(const CStrVector& vs)
	{
		_super::ResetContent();
		if ( vs.GetSize() > 0 )
		{
			loop ( i, vs.GetSize() - 1 )
			{
				_super::AddString(vs[i + 1]);
			}
			m_CheckDepth();
			_super::SetWindowText(vs[0]);
		}
	}

	/**
	 * [設定] 確定.
	 *		現在表示中の文字列を履歴に残します。
	 */
	void Apply(void)
	{
		CString s;
		_super::GetWindowText(s);
		if ( ! s.IsEmpty() )
		{
			int i = _super::FindStringExact(0, s);
			if ( i >= 0 )
			{
				_super::DeleteString(i);
			}
			_super::InsertString(0, s);
			m_CheckDepth();
			_super::SetWindowText(s);
		}
	}

	/**
	 * [参照] Editコントロール参照.
	 *	@return Editコントロール参照
	 */
	EDT& ReferEdit(void) { return m_edit; }

	/**
	 * [参照] Editコントロール参照.
	 *	@return Editコントロール参照
	 */
	const EDT& ReferEdit(void) const { return m_edit; }

protected:

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		DWORD s = GetStyle();
		ASSERT0( (s & CBS_DROPDOWN) != 0, "CHistoryComboEditT", "ドロップダウンになっていません。" );
		ASSERT0( (s & CBS_SORT) == 0, "CHistoryComboEditT", "ソート属性がついてしまっています。" );
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
		switch ( message )
		{
		case WM_DESTROY:
			if ( m_edit.GetSafeHwnd() != NULL )
			{
				m_edit.UnsubclassWindow();
			}
			break;
		case WM_CTLCOLOREDIT:
			if ( m_edit.GetSafeHwnd() == NULL )
			{
				m_edit.SubclassWindow(reinterpret_cast<HWND>(lParam));
			}
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	//　深さチェック
	void m_CheckDepth(void)
	{
		if ( m_depth > 1 )
		{
			while ( _super::GetCount() > m_depth )
			{
				_super::DeleteString(_super::GetCount() - 1);
			}
		}
	}
	int		m_depth;	/// <深さ
	EDT		m_edit;		///< EDIT
};



/**@ingroup MFCCONTROL
 * 履歴付き EDITコントロール
 *
 *		履歴付きのEDITコントロールを提供します。
 *
 *	@par使い方
 *		リソースエディタでコンボボックスコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CComboBox）を本クラスに置き換えて使用します。
 *
 *	@note ドロップダウンにし、ソートは false にします。
 *		
 *	@par必要ファイル
 *			TnbMfcHistoryComboEdit.h
 * 
 *	@date 09/06/30	新規作成
 */
typedef CHistoryComboEditT<> CHistoryComboEdit;



};
};