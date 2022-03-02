#pragma once
/**
 *@file
 * 数値専用入力Edit関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMfcEditAscii.h"
#ifdef _WIN32_WCE
 #include "TnbCeMfcGesture.h"
#endif

#ifdef _WIN32_WCE
 #define _TnbEDIT_Spin_Horz
#endif




//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * 数値専用 Edit コントロールテンプレート
 *
 *		テンプレートクラスです。
 *		数値で入出力できます。
 *		範囲を指定できます。
 *		ステップ数を指定できます（ただし基点は０）。初期値は１です。
 *		スピンコントロールをつけることが出来ます。
 *	
 *	@par使い方
 *		リソースエディタでエディトボックスコントロールを張り、コントロール変数に割り当て後、
 *		そのコントロール変数の型（CEdit）を本クラスに置き換えて使用します。
 *
 *	@note (CE固有);スピン付きの状態で上下で数値の加減、左右でエディトボックスコントロール内の移動、
 *			および、フォーカスの移動を行います（スピンがない時は、上下でフォーカス移動します）。
 *
 *	@todo スピン付きの状態でコントロールのサイズを変更すると、スピンの表示がおかしくなります。
 *
 *	@param TYP 型
 *	@param TMIN 最小値
 *	@param TMAX 最大値
 *
 *	@par必要ファイル
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	新規作成
 *	@date 06/11/17	CStr の使用から CString に切り替え。
 *	@date 08/01/02	スピンが Editの位置に追従するように修正。 
 *	@date 08/01/11	WM_PRINT にスピンボタンも書かれるように対応。
 *	@date 08/01/18	スピンボタン押下時に、フォーカスが来るように修正。
 *	@date 08/01/23	表示を最適化。
 *	@date 08/03/13	KillFocus時、範囲を下回っている時、Step値外など補正が
 *					入った時も BEEP するように変更。
 *	@date 08/04/15	SetValue() 時に範囲チェックを追加。
 *	@date 09/03/25	EM_SETREADONLY でスピンボタンの状態も変化するようにした。
 *	@date 09/09/16	CE 対策。
 *	@date 10/03/17	GetValue() の亜種、追加。
 *	@date 11/06/02	スピンボタンがDisable 表示になってしまう件を修正。
 *	@date 11/07/22	OnChangeRange() を追加。
 *	@date 14/12/05	ReadOnlyでもホイールで数値が変更できる件を修正。
 *	@date 15/04/26	'-' 入力を改善。
 *	@date 19/04/11	OnEditEnd()で変更もないのに文字列を設定していたせいで、
 *					キルフォーカス時にEN_CHANGEが出ていた不具合を修正。
 *	@date 19/06/17	GetValue() 時に GetText() を使わないように対応。
 */
template<typename TYP, LONGLONG TMIN, LONGLONG TMAX>
class CEditValueT : public CEditAscii
{
	DEFSUPER(CEditAscii);
public:

	typedef TYP TYPE;	///< 型定義

	/// コンストラクタ
	CEditValueT(void) 
		: _super(_T("-0123456789")), m_boOnPaint(false), m_iZeroPlace(0)
		, m_xMin(TMIN), m_xMax(TMAX), m_step(1)
	{
	}

	/// デストラクタ
	virtual ~CEditValueT(void)
	{
		if ( ::IsWindow(*this) )
		{
			_super::DestroyWindow();
		}
	}

	/**
	 * [変換] 値文字列化
	 *	@param xValue 変換する値
	 *	@return 変換後の文字列
	 */
	virtual CString ValueToString(TYPE xValue) const
	{
		LONGLONG l = AdjustValue(static_cast<LONGLONG>(xValue));
		return ToString(l);
	}

	/**
	 * [設定] 値設定
	 *	@param xValue 設定する値
	 */
	virtual void SetValue(TYPE xValue)
	{
		_super::SetText(ValueToString(xValue));
	}

	/**
	 * [取得] 値取得
	 *	@param[out] _xValue 設定されている値
	 *	@retval true 値有り。
	 *	@return false Empty。
	 */
	virtual bool GetValue(TYPE& _xValue) const
	{
		CString s = m_lastText;//_super::GetText();
		if ( s.IsEmpty() )
		{
			return false;
		}
		_xValue = static_cast<TYPE>(AdjustValue(ToInteger(s)));
		return true;
	}

	/**
	 * [取得] 値取得
	 *	@note 値がない場合、 TYPE() が返ります。
	 *	@return 値。
	 */
	TYPE GetValue(void) const
	{
		TYPE v;
		if ( GetValue(v) )
		{
			return v;
		}
		return TYPE();
	}

	/**
	 * [取得] 範囲取得
	 *	@param _xMin 最小値
	 *	@param _xMax 最大値
	 *	@param _step ステップ数。
	 */
	void GetRange(TYPE& _xMin, TYPE& _xMax, TYPE& _step)
	{
		_xMin = static_cast<TYPE>(m_xMin);
		_xMax = static_cast<TYPE>(m_xMax);
		_step = static_cast<TYPE>(m_step);
	}

	/**
	 * [設定] 範囲指定
	 *	@param xMin 最小値
	 *	@param xMax 最大値
	 *	@param step ステップ数。省略すると１になります。
	 */
	virtual void SetRange(TYPE xMin, TYPE xMax, TYPE step = 0)
	{
		m_xMin = static_cast<LONGLONG>(xMin);
		m_xMax = static_cast<LONGLONG>(xMax);
		if ( m_xMin > m_xMax )
		{
			Swap(m_xMin, m_xMax);
		}
		if ( TMAX < m_xMax )
		{
			m_xMax = TMAX;
		}
		if ( TMIN > m_xMin )
		{
			m_xMin = TMIN;
		}
		if ( step <= 0 )
		{
			step = 1;
		}
		m_step = ToInt(step);
		OnChangeRange();
	}

	/**
	 * [設定] ゼロパディング桁数設定
	 *	@param ii ゼロパディング桁数
	 */
	void SetPlace(int ii)
	{
		m_iZeroPlace = ii;
	}

	/**
	 * [設定] スピンボタン追加
	 *	@param dummy Dummy。省略可能。
	 */
	void AddSpinButton(int dummy = 0)
	{
		if ( ! ::IsWindow(m_spin) )
		{
			CRect rect;
			_super::GetClientRect(&rect);
			m_spinHeight = rect.Height();
			DWORD style = UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_NOTHOUSANDS;
			#ifdef _TnbEDIT_Spin_Horz
				style |= UDS_HORZ;
			#endif
			m_spin.Create(style, rect, this, 1000);
			UDACCEL udacl[] = 
			{
				{ 0,  1},	// 1づつ
				{ 2,  5},	// 2秒後からは5づつ
				{ 4, 10},	// 4秒後からは10づつ
				{ 6, 50},	// 6秒後からは50づつ
			};
			m_spin.SetAccel(4, udacl);
			m_spin.SetRange(10000, -10000);
			m_spin.SetPos(0);
			m_AdjustSpinAndEdit();
		}
	}

protected:

	/**
	 * [通知] for notifications from parent
	 *	@note	CWndのメソッドをオーバーライドしています。
	 *			メッセージ受信したらコールされます。
	 *			WM_NOTIFYの処理をフックしています。
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@param[out] _pResult リザルト
	 *	@retval TRUE 処理済。
	 *	@retval FALSE 未処理。
	 */
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* _pResult)
	{
		NMUPDOWN* pNmUpDown = reinterpret_cast<NMUPDOWN *>(lParam);
		if ( pNmUpDown->hdr.code == UDN_DELTAPOS )
		{
			if ( pNmUpDown->iDelta != 0 )
			{
				m_SetDelta(pNmUpDown->iDelta);
			}
			m_spin.SetPos(0);
			return TRUE;
		}
		return _super::OnNotify(wParam, lParam, _pResult);
	}

	#ifndef _WIN32_WCE

	/**
	 * [通知] for notifications from parent
	 *	@note	CWndのメソッドをオーバーライドしています。
	 *			メッセージ受信したらコールされます。
	 *			WM_NOTIFYの処理をフックしています。
	 *	@param[in] message メッセージ
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@param[out] _pResult リザルト
	 *	@retval TRUE 処理済。
	 *	@retval FALSE 未処理。
	 */
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* _pResult)
	{
		if ( message == WM_CTLCOLORSTATIC || message == WM_CTLCOLOREDIT )
		{
			if ( ::IsWindow(m_spin) && m_spin.IsWindowVisible() )
			{
				HDC dc = reinterpret_cast<HDC>(wParam);
				CPoint winPos(0, 0);
				if ( ! m_boOnPaint )
				{
					_super::ClientToScreen(&winPos);
				}
				else
				{
					m_boOnPaint = false;
					CRect winRect;
					_super::GetWindowRect(&winRect);
					winPos = winRect.TopLeft();
				}
				CRect spinRect;
				m_spin.GetWindowRect(&spinRect);
				CPoint po = spinRect.TopLeft() - winPos;
				HRGN rgn = ::CreateRectRgnIndirect(CRect(po, spinRect.Size()));
				::ExtSelectClipRgn(dc, rgn, RGN_DIFF);
				_DeleteObject(rgn);
			}
		}
		return _super::OnChildNotify(message, wParam, lParam, _pResult);
	}

	#endif //_WIN32_WCE

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
		if ( ::IsWindow(m_spin) )
		{
			if ( message == WM_TIMER )
			{
				if ( lParam == TIMERID_SPIN_REPOS )
				{
					KillTimer(TIMERID_SPIN_REPOS);
					m_AdjustSpinAndEdit();
				}
			}
			else if ( message == 0x031A/*WM_THEMECHANGED*/ )
			{
				SetTimer(TIMERID_SPIN_REPOS, 1, NULL);
			}
			else if ( message == WM_MOUSEWHEEL )
			{
				short d = HIWORD(wParam);
				d /= 120;
				if ( d != 0 )
				{
					m_SetDelta(-d);
				}
				return 0;
			}
			else if ( message == WM_KEYDOWN || message == WM_KEYUP )
			{
				if ( wParam == VK_DOWN || wParam == VK_UP )
				{
					UINT msg = (message == WM_KEYDOWN) ? WM_LBUTTONDOWN : WM_LBUTTONUP;
					#ifndef _TnbEDIT_Spin_Horz
						int x = 0;
						int y = (wParam == VK_DOWN) ? m_spinHeight - 1 : 0;
					#else
						int x = (wParam == VK_DOWN) ? m_spinHeight + 1 : 0;
						int y = 0;
					#endif
					m_spin.SendMessage(msg, 0, MAKELONG(x, y));
					return 0;
				}
				#ifdef _WIN32_WCE
					if ( message == WM_KEYDOWN && (wParam == VK_LEFT || wParam == VK_RIGHT) )
					{
						DWORD s1 = GetSel();
						LRESULT r = _super::WindowProc(message, wParam, lParam);
						CWnd* pPrev = GetParent();
						if ( s1 == GetSel() && pPrev != NULL )
						{
							CWnd* P = pPrev->GetNextDlgTabItem(this, (wParam == VK_LEFT));
							if ( P != NULL )
							{
								LRESULT cd = P->SendMessage(WM_GETDLGCODE);
								if ( (cd & DLGC_HASSETSEL) != 0 )
								{
									P->SendMessage(EM_SETSEL, 0, 0x7FFFFFFE);
								}
								P->SetFocus();
							}
						}
						return r;
					}
				#endif //_WIN32_WCE
			}
			else if ( message == WM_PAINT || message == WM_PRINT )
			{
				m_boOnPaint = true;
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				m_spin.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
				return r;
			}
			else if ( message == WM_DESTROY )
			{
				m_spin.SetParent(GetParent());
			}
			else if ( message == WM_PRINTCLIENT )
			{
				#ifndef _WIN32_WCE
					LRESULT r = _super::WindowProc(message, wParam, lParam);
					if ( m_spin.IsWindowVisible() )
					{
						HDC dc = reinterpret_cast<HDC>(wParam);
						CPoint winPos(0, 0);
						_super::ClientToScreen(&winPos);
						CPoint clientPos(0, 0);
						m_spin.ClientToScreen(&clientPos);
						CPoint orgOrg;
						::GetWindowOrgEx(dc, &orgOrg);
						CPoint po = orgOrg - (clientPos - winPos);
						::SetWindowOrgEx(dc, po.x, po.y, NULL);
						m_spin.SendMessage(WM_PRINT, wParam, lParam);
						::SetWindowOrgEx(dc, orgOrg.x, orgOrg.y, NULL);
					}
					return r;
				#endif //_WIN32_WCE
			}
			else if ( message == EM_SETREADONLY )
			{
				m_spin.EnableWindow(wParam == 0);
			}
			else if ( message == WM_ENABLE )
			{
				m_spin.EnableWindow(wParam != 0);
			}
			else if ( message == WM_GETDLGCODE )
			{
				#ifdef _WIN32_WCE
					return _super::WindowProc(message, wParam, lParam) | DLGC_WANTARROWS;
				#endif
			}
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [確認] 入力許可チェック
	 *	@param[in,out] _nChar WM_CHAR の WPARAM
	 *	@param[in,out] _lFlags WM_CHAR の LPARAM
	 *	@param[in] strNowText 入力済みの文字列
	 *	@retval true 入力を認める。
	 *	@retval false 入力を認めない。
	 */
	virtual bool IsValidChar(UINT& _nChar, LONG& _lFlags, const CString& strNowText)
	{
		if ( _nChar == '-' )
		{
			if ( m_xMin >= 0 )
			{
				return false;
			}
			DWORD g =_super::GetSel();
			if ( strNowText.Find('-') >= 0 )
			{
				if ( LOWORD(g) == 0 && HIWORD(g) > 0 )
				{
					return true;
				}
				return false;
			}
			return LOWORD(g) == 0;
		}
		return _super::IsValidChar(_nChar, _lFlags, strNowText);
	}

	/**
	 * [通知] 入力時にコール
	 *	@note	範囲チェックし、超えていたら丸めちゃう
	 */
	virtual void OnUpdateChar(void)
	{
		m_lastText = _super::GetText();		//変更状態を記憶しておく
		LONGLONG i = ToInteger(m_lastText);
		LONGLONG ii = i;
		if ( i > m_xMax )
		{
			i = m_xMax;
			if ( m_step != 1 )
			{
				i -= (i % m_step);
			}
			_super::SetText(ToString(i), true);
			if ( i != ii )
			{
				::MessageBeep(MB_ICONASTERISK); //入力制限より内容が変化した時
			}
		}
	}

	/**
	 * [通知] フォーカスを失った時にコール
	 */
	virtual void OnEditEnd(void)
	{
		CString s = _super::GetText();
		if ( ! s.IsEmpty() )
		{
			LONGLONG i = ToInteger(s);
			LONGLONG ii = AdjustValue(i);
			CString s = ToString(ii);
			if ( _super::GetText() != s )
			{
				_super::SetText(s);
			}
			if ( i != ii )
			{
				::MessageBeep(MB_ICONASTERISK); //入力制限より内容が変化した時
			}
		}
	}

	/**
	 * [通知] レンジが変わったときにコール
	 */
	virtual void OnChangeRange(void)
	{
	}
	
	/** 
	 * [変換] 数値から文字列作成.
	 *	@note 内部用のメソッドです。
	 *	@param ll 数値
	 *	@return 文字列
	 */
	virtual CString ToString(LONGLONG ll) const
	{
		CString str;
		if ( m_iZeroPlace > 0 )
		{
			CString s;
			s.Format(_T("%%0%dI64d"), (ll >= 0) ? m_iZeroPlace : (m_iZeroPlace + 1)); 
			str.Format(s, ll);
		}
		else
		{
			str.Format(_T("%I64d"), ll);
		}
		return str;
	}
	
	/** 
	 * [変換] 文字列から数値作成.
	 *	@note 内部用のメソッドです。
	 *	@param s 文字列
	 *	@return 数値
	 */
	virtual LONGLONG ToInteger(LPCTSTR s) const
	{
		return _ttoi64(s);
	}

	/** 
	 * [変換] 数値丸め.
	 *	@note 内部用のメソッドです。
	 *	@param l 値
	 *	@return 丸めた値
	 */
	virtual LONGLONG AdjustValue(LONGLONG l) const
	{
		if ( l > m_xMax )
		{
			l = m_xMax;
		}
		if ( l < m_xMin )
		{
			l = m_xMin;
		}
		if ( m_step != 1 )
		{
			if ( m_xMin > 0 )
			{
				l -= m_xMin;
				l -= (l % m_step);
				l += m_xMin;
			}
			else
			{
				l -= (l % m_step);
			}
		}
		return l;
	}

	/**
	 * [参照] スピンボタン参照.
	 *	@return スピンボタン.
	 */
	CSpinButtonCtrl& ReferSpinButton(void)
	{
		return m_spin;			///< スピンコントロール
	}

	LONGLONG		m_xMin;			///< 入力可能最小値
	LONGLONG		m_xMax;			///< 入力可能最大値
	int				m_iZeroPlace;	///< ゼロパディング
	int				m_step;			///< ステップ数
	CString			m_lastText;		///< 最終文字列

private:
	enum
	{
		TIMERID_SPIN_REPOS = 1 
	};
	#ifndef _WIN32_WCE
		CSpinButtonCtrl		m_spin;	///< スピンコントロール
	#else
		CSpinButtonCtrlDg	m_spin;///< スピンコントロール
	#endif
	int				m_spinHeight;	///< スピンコントロールの高さ
	bool			m_boOnPaint;	///< Paint中フラグ
	// デルタ
	void m_SetDelta(int delta)
	{
		if ( (GetStyle() & ES_READONLY) != 0 )
		{
			return;
		}
		ASSERTLIB( delta != 0 );
		LONGLONG d = delta;
		LONGLONG d0 = (d < 0) ? -d : d;
		LONGLONG r = ToInteger(GetText());
		LONGLONG r0 = r;
		LONGLONG step = (m_step == 1) ? d0 : m_step;
		d0 = m_step * d0;
		if ( delta > 0 )
		{
			d0 = -d0;
		}
		d0 -= (r % step);
		r += d0;
		if ( r > m_xMax )
		{
			r = m_xMax;
		}
		else if ( r < m_xMin )
		{
			r = m_xMin;
		}
		if ( r0 != r )
		{
			SetFocus();
			SetText(ToString(r), true);
			SetSel(0, 0);
		}
	}
	// スピンボタン調整
	void m_AdjustSpinAndEdit(void)
	{
		CRect rect;
		_super::GetClientRect(&rect);
		CRect rc;
		m_spin.GetWindowRect(&rc);
		UINT h = rc.Width();
		ASSERT( rc.Height() == m_spinHeight );
		#ifdef _TnbEDIT_Spin_Horz
			h = rc.Height() * 2;
			MFCLIB::ChangeClientSize(&m_spin, h, -1);
		#endif
		_super::SetMargins(3, 3 + h);
		MFCLIB::SetControlPos(&m_spin, rect.Width() - h, -1);
		m_spin.ShowWindow(SW_SHOW);
	}
};



/**@ingroup MFCCONTROL
 * ULONGLONG 型数値 Edit コントロール
 *
 *	0～9京9999兆9999億9999万9999まで入力可能。
 *	@note Spinは使用できません。
 *	@par必要ファイル
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	新規作成
 */
class CEditULongLong : public CEditValueT<ULONGLONG, 0, 99999999999999999>
{
public:
	/// コンストラクタ
	CEditULongLong(void)
	{}
};



/**@ingroup MFCCONTROL
 * DWORD 型数値 Edit コントロール
 *
 *	@par必要ファイル
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	新規作成
 */
class CEditDword : public CEditValueT<DWORD, 0, 0xFFFFFFFF>
{
public:
	/// コンストラクタ
	CEditDword(void)
	{}
};



/**@ingroup MFCCONTROL
 * Int 型数値 Edit コントロール
 *
 *	@par必要ファイル
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	新規作成
 */
class CEditInt : public CEditValueT<int, -(0x7FFFFFFF), 0x7FFFFFFE>
{
public:
	/// コンストラクタ
	CEditInt(void)
	{}
};



/**@ingroup MFCCONTROL
 * WORD 型数値 Edit コントロール
 *
 *	@par必要ファイル
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	新規作成
 */
class CEditWord : public CEditValueT<WORD, 0, 65535>
{
public:
	/// コンストラクタ
	CEditWord(void)
	{}
};



/**@ingroup MFCCONTROL
 * short 型数値 Edit コントロール
 *
 *	@par必要ファイル
 *			TnbMfcEditValue.h
 * 
 *	@date 09/09/03	新規作成
 */
class CEditShort : public CEditValueT<short, -32768, 32767>
{
public:
	/// コンストラクタ
	CEditShort(void)
	{}
};



/**@ingroup MFCCONTROL
 * BYTE 型数値 Edit コントロール
 *
 *	@par必要ファイル
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	新規作成
 */
class CEditByte : public CEditValueT<BYTE, 0, 255>
{
public:
	/// コンストラクタ
	CEditByte(void)
	{}
};



/**@ingroup MFCCONTROL
 * char 型数値 Edit コントロール
 *
 *	@par必要ファイル
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	新規作成
 */
class CEditChar : public CEditValueT<char, -128, 127>
{
public:
	/// コンストラクタ
	CEditChar(void)
	{}
};



/**@ingroup MFCCONTROL
 * HEX 入力 Editコントロールテンプレート
 *
 *		0～9、A～Fまでのみ入力が可能です。
 *		小文字のa～fは自動的に大文字に変換されます。
 *		入力時 '0x'はいりません（入力できません）。
 *
 *	@par必要ファイル
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	新規作成
 *	@date 10/08/04	テンプレート化
 *	@date 15/04/26	ToInteger() 内で _tcstoul を使わないように修正。
 */
template<typename TYP, size_t WIDTH>
class CEditHexValueT : public TYP
{
	DEFSUPER(TYP);
public:

	/// コンストラクタ
	CEditHexValueT(void) : _super()
	{
		_super::SetValidChars(_T("1234567890") _T("abcdef") _T("ABCDEF"));
	}

	/**
	 * [設定] スピンボタン追加
	 *	@param dummy Dummy。省略可能。
	 */
	void AddSpinButton(int dummy = 0)
	{
		_super::AddSpinButton(dummy);
		UDACCEL udacl[] = 
		{
			{ 0, 0x01},	// 1づつ
			{ 2, 0x08},	// 2秒後からは0x08づつ
			{ 4, 0x10},	// 4秒後からは0x10づつ
			{ 6, 0x20},	// 6秒後からは0x20づつ
		};
		ReferSpinButton().SetAccel(4, udacl);
	}

protected:

	/** 
	 * [変換] 数値から文字列作成
	 *	@param ll 数値
	 *	@return 文字列
	 */
	virtual CString ToString(LONGLONG ll) const
	{
		CString s;
		s.Format(_T("%08I64X"), ll);
		return s.Right(WIDTH);
	}
	
	/** 
	 * [変換] 文字列から数値作成
	 *	@param s 文字列
	 *	@return 数値
	 */
	virtual LONGLONG ToInteger(LPCTSTR s) const
	{
//		return _tcstoul(s, NULL, 16);
		LONGLONG r = 0;
		while ( true )
		{
			int c = *s++;
			if ( c >= 'a' && c <= 'f' )
			{
				c = (c - 'a') + 10;
			}
			else if ( c >= 'A' && c <= 'F' )
			{
				c = (c - 'A') + 10;
			}
			else if ( c >= '0' && c <= '9' )
			{
				c = c - '0';
			}
			else
			{
				break;
			}
			r *= 16;
			r += c;
		}
		return r;
	}
};



/**@ingroup MFCCONTROL
 * DWORD 型数値 HEX入力 Edit コントロール
 *
 *		0～9、A～Fまでのみ入力が可能です。
 *		小文字のa～fは自動的に大文字に変換されます。
 *		入力時 '0x'はいりません（入力できません）。
 *		値は 0x00000000～0xFFFFFFFFまでです。
 *
 *	@see CEditHexValueT, CEditDword
 *
 *	@par必要ファイル
 *			TnbMfcEditValue.h
 * 
 *	@date 10/08/04	新規作成
 */
class CEditHexDword : public CEditHexValueT<CEditDword, 8>
{
public:
	/// コンストラクタ
	CEditHexDword(void)
	{}
};



/**@ingroup MFCCONTROL
 * WORD 型数値 HEX入力 Edit コントロール
 *
 *		0～9、A～Fまでのみ入力が可能です。
 *		小文字のa～fは自動的に大文字に変換されます。
 *		入力時 '0x'はいりません（入力できません）。
 *		値は 0x0000～0xFFFFまでです。
 *
 *	@see CEditHexValueT, CEditWord
 *
 *	@par必要ファイル
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	新規作成
 *	@date 10/08/04	テンプレート化
 */
class CEditHexWord : public CEditHexValueT<CEditWord, 4>
{
public:
	/// コンストラクタ
	CEditHexWord(void)
	{}
};



/**@ingroup MFCCONTROL
 * BYTE 型数値 HEX入力 Edit コントロール
 *
 *		0～9、A～Fまでのみ入力が可能です。
 *		小文字のa～fは自動的に大文字に変換されます。
 *		入力時 '0x'はいりません（入力できません）。
 *		値は 0x00～0xFFまでです。
 *
 *	@see CEditHexValueT, CEditByte
 *
 *	@par必要ファイル
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	新規作成
 *	@date 10/08/04	テンプレート化
 */
class CEditHexByte : public CEditHexValueT<CEditByte, 2>
{
public:
	/// コンストラクタ
	CEditHexByte(void)
	{}
};



/**@ingroup MFCCONTROL
 * 空状態無し 数値入力 Editコントロールテンプレート
 *
 *		空っぽ状態のない数値入力 Editにします。
 *
 *	@par必要ファイル
 *			TnbMfcEditValue.h
 * 
 *	@date 10/09/15	新規作成
 *	@date 11/07/22	SetRange() 時に、最小値の値をコントロールに設定するようにした。
 */
template<typename TYP>
class CNonEmptyEditValueT : public TYP
{
	DEFSUPER(TYP);
protected:

	/**
	 * [通知] レンジが変わったときにコール
	 */
	virtual void OnChangeRange(void)
	{
		if ( _super::GetText().IsEmpty() )
		{
			_super::SetValue(static_cast<TYPE>(_super::m_xMin));
		}
	}

	/**
	 * [通知] subclassing/unsubclassing functions.
	 *		サブクラス化する時コールされます。
	 *	@note CWndのメソッドをオーバーライドしています。
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::SetText(ToString(_super::m_xMin));
		_super::PreSubclassWindow();
	}

	/**
	 * [通知] フォーカスを失った時にコール
	 *	@note 空っぽだったら、最小値を入れてます。
	 */
	virtual void OnEditEnd(void)
	{
		if ( _super::GetText().IsEmpty() )
		{
			_super::SetValue(static_cast<TYPE>(_super::m_xMin));
		}
		_super::OnEditEnd();
	}
};	



}; //MFC
}; //TNB
