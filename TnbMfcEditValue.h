#pragma once
/**
 *@file
 * ���l��p����Edit�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
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
 * ���l��p Edit �R���g���[���e���v���[�g
 *
 *		�e���v���[�g�N���X�ł��B
 *		���l�œ��o�͂ł��܂��B
 *		�͈͂��w��ł��܂��B
 *		�X�e�b�v�����w��ł��܂��i��������_�͂O�j�B�����l�͂P�ł��B
 *		�X�s���R���g���[�������邱�Ƃ��o���܂��B
 *	
 *	@par�g����
 *		���\�[�X�G�f�B�^�ŃG�f�B�g�{�b�N�X�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCEdit�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@note (CE�ŗL);�X�s���t���̏�Ԃŏ㉺�Ő��l�̉����A���E�ŃG�f�B�g�{�b�N�X�R���g���[�����̈ړ��A
 *			����сA�t�H�[�J�X�̈ړ����s���܂��i�X�s�����Ȃ����́A�㉺�Ńt�H�[�J�X�ړ����܂��j�B
 *
 *	@todo �X�s���t���̏�ԂŃR���g���[���̃T�C�Y��ύX����ƁA�X�s���̕\�������������Ȃ�܂��B
 *
 *	@param TYP �^
 *	@param TMIN �ŏ��l
 *	@param TMAX �ő�l
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	�V�K�쐬
 *	@date 06/11/17	CStr �̎g�p���� CString �ɐ؂�ւ��B
 *	@date 08/01/02	�X�s���� Edit�̈ʒu�ɒǏ]����悤�ɏC���B 
 *	@date 08/01/11	WM_PRINT �ɃX�s���{�^�����������悤�ɑΉ��B
 *	@date 08/01/18	�X�s���{�^���������ɁA�t�H�[�J�X������悤�ɏC���B
 *	@date 08/01/23	�\�����œK���B
 *	@date 08/03/13	KillFocus���A�͈͂�������Ă��鎞�AStep�l�O�ȂǕ␳��
 *					���������� BEEP ����悤�ɕύX�B
 *	@date 08/04/15	SetValue() ���ɔ͈̓`�F�b�N��ǉ��B
 *	@date 09/03/25	EM_SETREADONLY �ŃX�s���{�^���̏�Ԃ��ω�����悤�ɂ����B
 *	@date 09/09/16	CE �΍�B
 *	@date 10/03/17	GetValue() �̈���A�ǉ��B
 *	@date 11/06/02	�X�s���{�^����Disable �\���ɂȂ��Ă��܂������C���B
 *	@date 11/07/22	OnChangeRange() ��ǉ��B
 *	@date 14/12/05	ReadOnly�ł��z�C�[���Ő��l���ύX�ł��錏���C���B
 *	@date 15/04/26	'-' ���͂����P�B
 *	@date 19/04/11	OnEditEnd()�ŕύX���Ȃ��̂ɕ������ݒ肵�Ă��������ŁA
 *					�L���t�H�[�J�X����EN_CHANGE���o�Ă����s����C���B
 *	@date 19/06/17	GetValue() ���� GetText() ���g��Ȃ��悤�ɑΉ��B
 */
template<typename TYP, LONGLONG TMIN, LONGLONG TMAX>
class CEditValueT : public CEditAscii
{
	DEFSUPER(CEditAscii);
public:

	typedef TYP TYPE;	///< �^��`

	/// �R���X�g���N�^
	CEditValueT(void) 
		: _super(_T("-0123456789")), m_boOnPaint(false), m_iZeroPlace(0)
		, m_xMin(TMIN), m_xMax(TMAX), m_step(1)
	{
	}

	/// �f�X�g���N�^
	virtual ~CEditValueT(void)
	{
		if ( ::IsWindow(*this) )
		{
			_super::DestroyWindow();
		}
	}

	/**
	 * [�ϊ�] �l������
	 *	@param xValue �ϊ�����l
	 *	@return �ϊ���̕�����
	 */
	virtual CString ValueToString(TYPE xValue) const
	{
		LONGLONG l = AdjustValue(static_cast<LONGLONG>(xValue));
		return ToString(l);
	}

	/**
	 * [�ݒ�] �l�ݒ�
	 *	@param xValue �ݒ肷��l
	 */
	virtual void SetValue(TYPE xValue)
	{
		_super::SetText(ValueToString(xValue));
	}

	/**
	 * [�擾] �l�擾
	 *	@param[out] _xValue �ݒ肳��Ă���l
	 *	@retval true �l�L��B
	 *	@return false Empty�B
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
	 * [�擾] �l�擾
	 *	@note �l���Ȃ��ꍇ�A TYPE() ���Ԃ�܂��B
	 *	@return �l�B
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
	 * [�擾] �͈͎擾
	 *	@param _xMin �ŏ��l
	 *	@param _xMax �ő�l
	 *	@param _step �X�e�b�v���B
	 */
	void GetRange(TYPE& _xMin, TYPE& _xMax, TYPE& _step)
	{
		_xMin = static_cast<TYPE>(m_xMin);
		_xMax = static_cast<TYPE>(m_xMax);
		_step = static_cast<TYPE>(m_step);
	}

	/**
	 * [�ݒ�] �͈͎w��
	 *	@param xMin �ŏ��l
	 *	@param xMax �ő�l
	 *	@param step �X�e�b�v���B�ȗ�����ƂP�ɂȂ�܂��B
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
	 * [�ݒ�] �[���p�f�B���O�����ݒ�
	 *	@param ii �[���p�f�B���O����
	 */
	void SetPlace(int ii)
	{
		m_iZeroPlace = ii;
	}

	/**
	 * [�ݒ�] �X�s���{�^���ǉ�
	 *	@param dummy Dummy�B�ȗ��\�B
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
				{ 0,  1},	// 1�Â�
				{ 2,  5},	// 2�b�ォ���5�Â�
				{ 4, 10},	// 4�b�ォ���10�Â�
				{ 6, 50},	// 6�b�ォ���50�Â�
			};
			m_spin.SetAccel(4, udacl);
			m_spin.SetRange(10000, -10000);
			m_spin.SetPos(0);
			m_AdjustSpinAndEdit();
		}
	}

protected:

	/**
	 * [�ʒm] for notifications from parent
	 *	@note	CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *			���b�Z�[�W��M������R�[������܂��B
	 *			WM_NOTIFY�̏������t�b�N���Ă��܂��B
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@param[out] _pResult ���U���g
	 *	@retval TRUE �����ρB
	 *	@retval FALSE �������B
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
	 * [�ʒm] for notifications from parent
	 *	@note	CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *			���b�Z�[�W��M������R�[������܂��B
	 *			WM_NOTIFY�̏������t�b�N���Ă��܂��B
	 *	@param[in] message ���b�Z�[�W
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@param[out] _pResult ���U���g
	 *	@retval TRUE �����ρB
	 *	@retval FALSE �������B
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
	 * [�ʒm] for processing Windows messages.
	 *		���b�Z�[�W��M������R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *	@param message ���b�Z�[�W
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return ���U���g�B
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
	 * [�m�F] ���͋��`�F�b�N
	 *	@param[in,out] _nChar WM_CHAR �� WPARAM
	 *	@param[in,out] _lFlags WM_CHAR �� LPARAM
	 *	@param[in] strNowText ���͍ς݂̕�����
	 *	@retval true ���͂�F�߂�B
	 *	@retval false ���͂�F�߂Ȃ��B
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
	 * [�ʒm] ���͎��ɃR�[��
	 *	@note	�͈̓`�F�b�N���A�����Ă�����ۂ߂��Ⴄ
	 */
	virtual void OnUpdateChar(void)
	{
		m_lastText = _super::GetText();		//�ύX��Ԃ��L�����Ă���
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
				::MessageBeep(MB_ICONASTERISK); //���͐��������e���ω�������
			}
		}
	}

	/**
	 * [�ʒm] �t�H�[�J�X�����������ɃR�[��
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
				::MessageBeep(MB_ICONASTERISK); //���͐��������e���ω�������
			}
		}
	}

	/**
	 * [�ʒm] �����W���ς�����Ƃ��ɃR�[��
	 */
	virtual void OnChangeRange(void)
	{
	}
	
	/** 
	 * [�ϊ�] ���l���當����쐬.
	 *	@note �����p�̃��\�b�h�ł��B
	 *	@param ll ���l
	 *	@return ������
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
	 * [�ϊ�] �����񂩂琔�l�쐬.
	 *	@note �����p�̃��\�b�h�ł��B
	 *	@param s ������
	 *	@return ���l
	 */
	virtual LONGLONG ToInteger(LPCTSTR s) const
	{
		return _ttoi64(s);
	}

	/** 
	 * [�ϊ�] ���l�ۂ�.
	 *	@note �����p�̃��\�b�h�ł��B
	 *	@param l �l
	 *	@return �ۂ߂��l
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
	 * [�Q��] �X�s���{�^���Q��.
	 *	@return �X�s���{�^��.
	 */
	CSpinButtonCtrl& ReferSpinButton(void)
	{
		return m_spin;			///< �X�s���R���g���[��
	}

	LONGLONG		m_xMin;			///< ���͉\�ŏ��l
	LONGLONG		m_xMax;			///< ���͉\�ő�l
	int				m_iZeroPlace;	///< �[���p�f�B���O
	int				m_step;			///< �X�e�b�v��
	CString			m_lastText;		///< �ŏI������

private:
	enum
	{
		TIMERID_SPIN_REPOS = 1 
	};
	#ifndef _WIN32_WCE
		CSpinButtonCtrl		m_spin;	///< �X�s���R���g���[��
	#else
		CSpinButtonCtrlDg	m_spin;///< �X�s���R���g���[��
	#endif
	int				m_spinHeight;	///< �X�s���R���g���[���̍���
	bool			m_boOnPaint;	///< Paint���t���O
	// �f���^
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
	// �X�s���{�^������
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
 * ULONGLONG �^���l Edit �R���g���[��
 *
 *	0�`9��9999��9999��9999��9999�܂œ��͉\�B
 *	@note Spin�͎g�p�ł��܂���B
 *	@par�K�v�t�@�C��
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	�V�K�쐬
 */
class CEditULongLong : public CEditValueT<ULONGLONG, 0, 99999999999999999>
{
public:
	/// �R���X�g���N�^
	CEditULongLong(void)
	{}
};



/**@ingroup MFCCONTROL
 * DWORD �^���l Edit �R���g���[��
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	�V�K�쐬
 */
class CEditDword : public CEditValueT<DWORD, 0, 0xFFFFFFFF>
{
public:
	/// �R���X�g���N�^
	CEditDword(void)
	{}
};



/**@ingroup MFCCONTROL
 * Int �^���l Edit �R���g���[��
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	�V�K�쐬
 */
class CEditInt : public CEditValueT<int, -(0x7FFFFFFF), 0x7FFFFFFE>
{
public:
	/// �R���X�g���N�^
	CEditInt(void)
	{}
};



/**@ingroup MFCCONTROL
 * WORD �^���l Edit �R���g���[��
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	�V�K�쐬
 */
class CEditWord : public CEditValueT<WORD, 0, 65535>
{
public:
	/// �R���X�g���N�^
	CEditWord(void)
	{}
};



/**@ingroup MFCCONTROL
 * short �^���l Edit �R���g���[��
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEditValue.h
 * 
 *	@date 09/09/03	�V�K�쐬
 */
class CEditShort : public CEditValueT<short, -32768, 32767>
{
public:
	/// �R���X�g���N�^
	CEditShort(void)
	{}
};



/**@ingroup MFCCONTROL
 * BYTE �^���l Edit �R���g���[��
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	�V�K�쐬
 */
class CEditByte : public CEditValueT<BYTE, 0, 255>
{
public:
	/// �R���X�g���N�^
	CEditByte(void)
	{}
};



/**@ingroup MFCCONTROL
 * char �^���l Edit �R���g���[��
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	�V�K�쐬
 */
class CEditChar : public CEditValueT<char, -128, 127>
{
public:
	/// �R���X�g���N�^
	CEditChar(void)
	{}
};



/**@ingroup MFCCONTROL
 * HEX ���� Edit�R���g���[���e���v���[�g
 *
 *		0�`9�AA�`F�܂ł̂ݓ��͂��\�ł��B
 *		��������a�`f�͎����I�ɑ啶���ɕϊ�����܂��B
 *		���͎� '0x'�͂���܂���i���͂ł��܂���j�B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	�V�K�쐬
 *	@date 10/08/04	�e���v���[�g��
 *	@date 15/04/26	ToInteger() ���� _tcstoul ���g��Ȃ��悤�ɏC���B
 */
template<typename TYP, size_t WIDTH>
class CEditHexValueT : public TYP
{
	DEFSUPER(TYP);
public:

	/// �R���X�g���N�^
	CEditHexValueT(void) : _super()
	{
		_super::SetValidChars(_T("1234567890") _T("abcdef") _T("ABCDEF"));
	}

	/**
	 * [�ݒ�] �X�s���{�^���ǉ�
	 *	@param dummy Dummy�B�ȗ��\�B
	 */
	void AddSpinButton(int dummy = 0)
	{
		_super::AddSpinButton(dummy);
		UDACCEL udacl[] = 
		{
			{ 0, 0x01},	// 1�Â�
			{ 2, 0x08},	// 2�b�ォ���0x08�Â�
			{ 4, 0x10},	// 4�b�ォ���0x10�Â�
			{ 6, 0x20},	// 6�b�ォ���0x20�Â�
		};
		ReferSpinButton().SetAccel(4, udacl);
	}

protected:

	/** 
	 * [�ϊ�] ���l���當����쐬
	 *	@param ll ���l
	 *	@return ������
	 */
	virtual CString ToString(LONGLONG ll) const
	{
		CString s;
		s.Format(_T("%08I64X"), ll);
		return s.Right(WIDTH);
	}
	
	/** 
	 * [�ϊ�] �����񂩂琔�l�쐬
	 *	@param s ������
	 *	@return ���l
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
 * DWORD �^���l HEX���� Edit �R���g���[��
 *
 *		0�`9�AA�`F�܂ł̂ݓ��͂��\�ł��B
 *		��������a�`f�͎����I�ɑ啶���ɕϊ�����܂��B
 *		���͎� '0x'�͂���܂���i���͂ł��܂���j�B
 *		�l�� 0x00000000�`0xFFFFFFFF�܂łł��B
 *
 *	@see CEditHexValueT, CEditDword
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEditValue.h
 * 
 *	@date 10/08/04	�V�K�쐬
 */
class CEditHexDword : public CEditHexValueT<CEditDword, 8>
{
public:
	/// �R���X�g���N�^
	CEditHexDword(void)
	{}
};



/**@ingroup MFCCONTROL
 * WORD �^���l HEX���� Edit �R���g���[��
 *
 *		0�`9�AA�`F�܂ł̂ݓ��͂��\�ł��B
 *		��������a�`f�͎����I�ɑ啶���ɕϊ�����܂��B
 *		���͎� '0x'�͂���܂���i���͂ł��܂���j�B
 *		�l�� 0x0000�`0xFFFF�܂łł��B
 *
 *	@see CEditHexValueT, CEditWord
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	�V�K�쐬
 *	@date 10/08/04	�e���v���[�g��
 */
class CEditHexWord : public CEditHexValueT<CEditWord, 4>
{
public:
	/// �R���X�g���N�^
	CEditHexWord(void)
	{}
};



/**@ingroup MFCCONTROL
 * BYTE �^���l HEX���� Edit �R���g���[��
 *
 *		0�`9�AA�`F�܂ł̂ݓ��͂��\�ł��B
 *		��������a�`f�͎����I�ɑ啶���ɕϊ�����܂��B
 *		���͎� '0x'�͂���܂���i���͂ł��܂���j�B
 *		�l�� 0x00�`0xFF�܂łł��B
 *
 *	@see CEditHexValueT, CEditByte
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEditValue.h
 * 
 *	@date 06/01/01	�V�K�쐬
 *	@date 10/08/04	�e���v���[�g��
 */
class CEditHexByte : public CEditHexValueT<CEditByte, 2>
{
public:
	/// �R���X�g���N�^
	CEditHexByte(void)
	{}
};



/**@ingroup MFCCONTROL
 * ���Ԗ��� ���l���� Edit�R���g���[���e���v���[�g
 *
 *		����ۏ�Ԃ̂Ȃ����l���� Edit�ɂ��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcEditValue.h
 * 
 *	@date 10/09/15	�V�K�쐬
 *	@date 11/07/22	SetRange() ���ɁA�ŏ��l�̒l���R���g���[���ɐݒ肷��悤�ɂ����B
 */
template<typename TYP>
class CNonEmptyEditValueT : public TYP
{
	DEFSUPER(TYP);
protected:

	/**
	 * [�ʒm] �����W���ς�����Ƃ��ɃR�[��
	 */
	virtual void OnChangeRange(void)
	{
		if ( _super::GetText().IsEmpty() )
		{
			_super::SetValue(static_cast<TYPE>(_super::m_xMin));
		}
	}

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::SetText(ToString(_super::m_xMin));
		_super::PreSubclassWindow();
	}

	/**
	 * [�ʒm] �t�H�[�J�X�����������ɃR�[��
	 *	@note ����ۂ�������A�ŏ��l�����Ă܂��B
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
