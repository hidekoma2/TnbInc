#pragma once
/**
 *@file
 * �{�^�����ۊ֌W�̃w�b�_
 *
 *		�{�^�����쐬���邽�߂̃t���[����񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
#define BN_PUSHED_T			0x100
#define BN_UNPUSHED_T		0x101
#endif



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �{�^���R���g���[�����ۃN���X
 *
 *		�{�N���X���p�����Ď��R�ȃ{�^���R���g���[�����쐬�ł��܂��B
 *
 *	@note �}�E�X����ɏ�������� BN_HILITE �A���ꂽ���� BN_UNHILITE ���A�e�ɒʒm���Ă��܂��B
 *	@par�g�p��
 *		\code
 *		//= �錾
 *		BEGIN_MESSAGE_MAP(CXxxDlg, CDialog)
 *		ON_BN_CLICK(IDC_BMP_KAGE2, OnButtonClick)		// �N���b�N������
 *		ON_CONTROL(BN_HILITE, IDC_BMP_KAGE2, OnButtonHover) // ��Ƀ}�E�X��������		
 *			:
 *		\endcode
 *
 *	@note �v���X���[�h�̎��A�v���X�������A BN_PUSHED_T �A�����[�X���� BN_UNPUSHED_T ���A�e�ɒʒm���Ă��܂��B
 *	@par�g�p��
 *		\code
 *		//= �錾
 *		BEGIN_MESSAGE_MAP(CXxxDlg, CDialog)
 *		ON_CONTROL(BN_PUSHED_T, IDC_BMP_KAGE2, OnButtonPushed) // �v���X������
 *		ON_CONTROL(BN_UNPUSHED_T, IDC_BMP_KAGE2, OnButtonUnpushed) // �����[�X������
 *			:
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcAbstractButton.h
 * 
 *	@date 07/10/04 �V�K�쐬
 *	@date 07/10/11 Enter�L�[�ɑΉ��i�e���_�C�A���O�x�[�X�ȊO�͑ΏۊO�j�B
 *	@date 07/10/17 �V���[�g�J�b�g�ɑΉ��B
 *	@date 07/10/19 �e�ɒʒm����O�ɏ�����������悤�ɑΉ��B
 *	@date 07/10/22 �}�E�X�z�o�[�Őe�ɒʒm����悤�ɂ����BWM_NCPAINT,WM_ENABLE �̏����A���P
 *	@date 07/10/26 WM_SETFONT,WM_GETFONT �Ή��B���\�b�h���A�����B
 *	@date 07/10/31 WM_PRINTCLIENT �ɑΉ��B
 *	@date 07/11/07 ��ԕω��ʒm�ǉ��B
 *	@date 07/11/12 WM_COMMAND �� LPARAM ���e�C���B
 *	@date 07/11/20 Window�j����̃��b�Z�[�W��M�����̑Ή�(�C��)�B
 *	@date 07/12/20 ���\�[�X�G�f�B�^�Ƀ{�^����K�p���������݂��o��̂��C���B
 *	@date 07/12/27 BM_CLICK �C�x���g���������P�B OnClickButton() �̒ʒm�^�C�~���O�����P�B
 *	@date 08/02/06 SetDefaultButtonStyle() ��ǉ��B
 *	@date 08/02/07 �z�o�[��Ԃ𐮗��B BN_DOUBLECLICKED �T�|�[�g�B
 *	@date 08/02/18 �v���X���� KillFocus �̏��������P�B
 *	@date 08/02/22 �t�H�[�J�X�t���O,�p�~
 *	@date 08/03/03 EP_Message ���A���e�ɉ����ʒm�����̂��C���B
 *	@date 08/03/10 ���������m�F�������B
 *	@date 09/09/25 �\���ύX�B
 *	@date 10/11/09 SetPressedMode() ��ǉ��B�����A�����̃C�x���g��V�K
 *	@date 14/05/20 EPressMode ��ǉ��B SetPressedMode(EPressMode) ��ǉ��B
 */
class CAbstractButton : public CWnd
{
	DEFSUPER(CWnd);
public:

	/// �R���X�g���N�^
	CAbstractButton(void) 
		: m_shortcutKey(0), m_pushState(EP_Non), m_boHasHover(false)
		, m_nowState(ES_Normal), m_lastState(EState(-1)), m_hFont(NULL), m_boInBmClick(false)
		, m_boIsOnClickDisp(false), m_canSendDblClk(false), m_pressedMode(PM_Unused)
	{
	}

	/**
	 * �R�s�[�I�y���[�^.
	 *	@note �t�H���g�n���h���A�V���[�g�J�b�g�L�[���̂ݕ������܂��B
	 *	@param other �R�s�[���B
	 *	@return �����̎Q��
	 */
	CAbstractButton& operator=(const CAbstractButton& other)
	{
		m_shortcutKey = other.m_shortcutKey;
		m_hFont = other.m_hFont;
		return *this;
	}

	/// �f�X�g���N�^
	virtual ~CAbstractButton(void)
	{
	}

	/**
	 * [�ݒ�] �V���[�g�J�b�g�L�[�ݒ�.
	 *		�t�H�[�J�X�������Ƃ��ł��L�[�{�[�h�� ALT + �ŉ�����V���[�g�J�b�g�L�[�ɑΉ�����L�[���w�肵�܂��B
	 *		�w�肵�Ȃ��ꍇ�A�V���[�g�J�b�g�͏o���܂���B
	 *	@param key �L�[�B 'A' �` 'Z' ���w�肵�܂��B
	 */
	void SetShortcutKey(TCHAR key)
	{
		m_shortcutKey = key;
	}

	/**
	 * [�ݒ�] �f�t�H���g�{�^���X�^�C���ݒ�.
	 *		�{�{�^���Ƀf�t�H���g�{�^���X�^�C�������܂��B
	 *	@note ���̃{�^���Ƀt�H�[�J�X���Ȃ����ɁA���^�[������������Ɠ��삷��̂��A�f�t�H���g�{�^���ł��B
	 */
	void SetDefaultButtonStyle(void)
	{
		CWnd* P = _super::GetParent();
		if ( P != NULL ) 
		{
			P->SendMessage(DM_SETDEFID, _super::GetDlgCtrlID(), 0);
		}
	}

	/**
	 * [�擾] ��Ԏ擾.
	 *		�I�v�V���� �{�^���܂��̓`�F�b�N �{�b�N�X�̏�Ԃ��擾���܂��B
	 *	@return ��ԁB���̃}�X�N�l���g�p�ł��܂��B
	 *		<table>
	 *		<tr><td>�}�X�N</td>	<td>����</td></tr>
	 *					<tr><td>
	 *		0x0003		</td><td>
	 *				�I���A�I�t��Ԃ𒲂ׂ܂� (�I�v�V���� �{�^���ƃ`�F�b�N �{�b�N�X�̏ꍇ����)�B
	 *				0 �̂Ƃ��́A�{�^���̓I�t�ł��B1 �̂Ƃ��́A�{�^���̓I���ł��B�I�v�V���� �{�^���̓`�F�b�N �}�[�N (?) ���t���Ă����Ԃ��I���ł��B�`�F�b�N �{�b�N�X�� X �}�[�N���t���Ă����Ԃ��I���ł��B2 �́A�`�F�b�N��Ԃ��s�m��ł��邱�Ƃ������܂� (3 �X�e�[�g �`�F�b�N �{�b�N�X�̂�)�B
	 *				3 �X�e�[�g �`�F�b�N �{�b�N�X�͒W�F�\����Ԃ̂Ƃ��s�m���Ԃ������܂��B</td>
	 *					</tr><tr><td>
	 *		0x0004		</td><td>
	 *				�����\����Ԃ𒲂ׂ܂��B0 �ȊO�̒l�̂Ƃ��́A�{�^���������\����Ԃł��B
	 *				�}�E�X�̍��{�^�����N���b�N�����܂܂ɂ���ƁA�{�^���͋����\����ԂɂȂ�܂��B
	 *				�}�E�X �{�^���𗣂��ƁA�����\����Ԃ͉�������܂��B</td>
	 *					</tr><tr><td>
	 *		0x0008		</td><td>
	 *				�t�H�[�J�X��Ԃ𒲂ׂ܂��B0 �ȊO�̒l�̂Ƃ��́A�{�^���Ƀt�H�[�J�X�����邱�Ƃ������܂��B</td>
	 *					</tr>
	 *		</table>
	 */
	UINT GetState(void) const
	{
		ASSERT(::IsWindow(m_hWnd));
		return static_cast<UINT>(::SendMessage(m_hWnd, BM_GETSTATE, 0, 0));
	} 

	/// �v���X���[�h��
	enum EPressMode
	{
		PM_Unused,		///< �ʏ�{�^��
		PM_Message,		///< �v���X���b�Z�[�W�Ή�
		PM_Lockable,	///< �v���X���b�N�@�\���L��
	};

	/**
	 * [�ݒ�] �v���X���[�h�ݒ�.
	 *		�v���X���b�N���[�h�t���̃{�^���ɂ��邱�Ƃ��o���܂��B
	 *		�v���X���[�h�ɂ���ƁA�v���X����Ɛe�E�B���h�E�� BN_PUSHED_T ���A
	 *		�����[�X����� BN_UNPUSHED_T ���ʒm����܂��B
	 *	@param isEnable true �Ȃ�v���X���[�h�B false �Ȃ�ʏ�{�^���ɂ��܂��B
	 */
	void SetPressedMode(bool isEnable)
	{
		m_pressedMode = isEnable ? PM_Lockable : PM_Unused;
	}	
	
	/**
	 * [�ݒ�] �v���X���[�h�ݒ�.
	 *		�v���X���b�Z�[�W�A�v���X���b�N���[�h�t���̃{�^���ɂ��邱�Ƃ��o���܂��B
	 *		�v���X���[�h�ɂ���ƁA�v���X����Ɛe�E�B���h�E�� BN_PUSHED_T ���A
	 *		�����[�X����� BN_UNPUSHED_T ���ʒm����܂��B
	 *	@param pm �v���X���[�h
	 */
	void SetPressedMode(EPressMode pm)
	{
		m_pressedMode = pm;
	}

protected:
	
	/// ���
	enum EState 
	{
		ES_Normal,				///< �W�����
		ES_Hover,				///< �z�o�[���
		ES_Pushed,				///< �������
		ES_Disable,				///< �������
	};

	/**
	 * [�擾] �{�^����Ԏ擾.
	 *	@note ���݂̏�Ԃ��擾���܂�
	 *	@return ��ԁB
	 */
	EState GetButtonState(void) const
	{
		return m_nowState;
	}

	/**
	 * [�ݒ�] �{�^����Ԑݒ�.
	 *	@note �����������������邾���ł��B�ĕ\���͂��܂���B
	 *	@param s ��ԁB
	 */
	void SetButtonState(EState s)
	{
		m_nowState = s;
	}

	/**
	 * [�ݒ�] �_�u���N���b�N�C�x���g�T�|�[�g
	 *	@note �f�t�H���g�ł� false �ł��B
	 *	@param canSendDblClk true �Ȃ�A BN_DOUBLECLICKED ��e�ɒʒm����悤�ɂȂ�܂��B 
	 */
	void SetSendDblClkMode(bool canSendDblClk)
	{
		m_canSendDblClk = canSendDblClk;
	}

	/**
	 * [����] �`��.
	 *	@param pDC �f�o�C�X�R���e�L�X�g�B
	 */
	virtual void OnDrawButton(CDC* pDC) = 0;

	/**
	 * [�m�F] �|�C���g���{�^����ɂ��邩.
	 *	@param po �X�N���[�����W�B
	 *	@retval true ��ɂ���B
	 *	@retval false �Ȃ��B
	 */
	virtual bool IsOnButton(const POINT& po) const
	{
		return ::WindowFromPoint(po) == m_hWnd;
	}

	/**
	 * [�ʒm] ���݂̏�Ԋm�F�ʒm
	 *	@param st ���݂̏��
	 *	@return �V�������
	 */
	virtual EState OnCheckState(EState st)
	{
		if ( m_IsAlive() )
		{
			if ( ! _super::IsWindowEnabled() )
			{
				st = ES_Disable;
			}
			else if ( m_pushState == EP_Mouse )
			{
				st = ES_Pushed;
				CPoint po = m_curpos;
				ClientToScreen(&po);
				if ( ! IsOnButton(po) )
				{
					st = ES_Normal;
				}
			}
			else if ( m_pushState != EP_Non )
			{
				st = ES_Pushed;
			}
			else if ( m_boHasHover )
			{
				st = ES_Hover;
			}
			else
			{
				st = ES_Normal;
			}
		}
		return st;
	}

	/**
	 * [�m�F] �t�H�[�J�X�̗L��
	 *	@retval true �t�H�[�J�X����B
	 *	@retval false �t�H�[�J�X�Ȃ��B
	 */
	virtual bool HasFocus(void) const
	{
		return ::GetFocus() == m_hWnd;
	}

	/**
	 * [�ʒm] �N���b�N.
	 *		BN_CLICKED ��e�ɒʒm����O�ɃR�[������܂��B
	 */
	virtual void OnClickButton(void)
	{
	}

	/**
	 * [�ʒm] ��ԕω��ʒm.
	 *	@note ���݂̏�Ԃ�ʒm���܂��B�t�H�[�J�X���ω����������ʒm���܂��istate�͓����l�ł��j�B
	 *	@param state ��ԁB
	 */
	virtual void OnChangeState(EState state)
	{
	}

	/**
	 * [�ʒm] for notifications from parent
	 *	@note
	 *		CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *		���b�Z�[�W��M������R�[������܂��B
	 *	@param[in] message ���b�Z�[�W
	 *	@param[in] wParam WPARAM
	 *	@param[in] lParam LPARAM
	 *	@param[out] _pResult ���U���g
	 *	@retval TRUE	�����ρB
	 *	@retval FALSE	�������B
	 */
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* _pResult)
	{
		if ( message == WM_COMMAND && lParam == reinterpret_cast<LPARAM>(m_hWnd) )
		{
			ASSERTLIB( (wParam & 0xFFFF) == static_cast<WPARAM>(GetDlgCtrlID()) );
			ASSERTLIB( (BN_CLICKED * 0x10000) == 0 );
			if ( (wParam & 0xFFFF0000) == 0 )
			{
				if ( ! m_boIsOnClickDisp )
				{
					OnClickButton();
				}
				else
				{
					m_boIsOnClickDisp = false;
				}
			}
		}
		return _super::OnChildNotify(message, wParam, lParam, _pResult);
	}
	
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
		switch ( message )
		{
		case 0x0128/*WM_UPDATEUISTATE*/:
			TTRACE0("WM_UPDATEUISTATE\n");
			return 0;
		case WM_ERASEBKGND:
			TTRACE0("WM_ERASEBKGND\n");
			return 0;
		case WM_PRINTCLIENT:
			TTRACE0("WM_PRINTCLIENT\n");
			{
				CDC* pDC = CDC::FromHandle(reinterpret_cast<HDC>(wParam));
				if ( pDC != NULL )
				{
					OnDrawButton(pDC);
				}
			}
			return 0;
		case WM_PAINT:
			TTRACE0("WM_PAINT\n");
			{
				CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
				OnDrawButton(&dc);
			}
			return 0;
		case 0x031A/*WM_THEMECHANGED*/:
			OnChangeState(m_nowState);
			break;
		case WM_SETFOCUS:
			TTRACE0("WM_SETFOCUS\n");
			OnChangeState(m_nowState);
			m_RedrawWindow();
			return 0;
		case WM_KILLFOCUS:
			TTRACE0("WM_KILLFOCUS\n");
			if ( m_pushState != EP_Non && ! m_IsPressedLockMode() )
			{
				m_pushState = EP_Non;
				::ReleaseCapture();
				m_HoverOff(false);
				m_CheckNowState(false);
				m_RedrawWindow();
			}
			else
			{
				OnChangeState(m_nowState);
			}
			m_RedrawWindow();
			return 0;
		case WM_GETDLGCODE:
			TTRACE0("WM_GETDLGCODE\n");
			if ( HasFocus() )
			{
				return DLGC_DEFPUSHBUTTON | DLGC_BUTTON;
			}
			return DLGC_UNDEFPUSHBUTTON | DLGC_BUTTON;
		case WM_GETTEXTLENGTH:
			TTRACE0("WM_GETTEXTLENGTH\n");
			if ( m_shortcutKey != 0 )
			{
				return 2 * sizeof(TCHAR);
			}
			break;
		case WM_GETTEXT:
			TTRACE0("WM_GETTEXT\n");
			if ( m_shortcutKey != 0 && wParam >= 2 )
			{
				TCHAR* P = reinterpret_cast<TCHAR*>(lParam);
				*P++ = '&';
				*P++ = m_shortcutKey;
				return 2;
			}
			break;
		case WM_KEYDOWN:
			TTRACE2("WM_KEYDOWN 0x%04X,0x%04X\n",wParam,lParam);
			if ( wParam == ' ' )
			{
				m_Push(EP_Key);
			}
			return 0;
		case WM_KEYUP:
			TTRACE2("WM_KEYUP 0x%04X,0x%04X\n",wParam,lParam);
			if ( wParam == ' ' )
			{
				m_Unpush(EP_Key);
				if ( m_IsPressedLockMode() )
				{
					m_Unpush(EP_Mouse);
				}
			}
			return 0;
		case WM_LBUTTONDOWN:
			TTRACE0("WM_LBUTTONDOWN\n");
//			m_curpos.x = LOWORD(lParam);
//			m_curpos.y = HIWORD(lParam);
			m_curpos = CPoint(lParam);
			m_Push(m_boInBmClick ? EP_Key : EP_Mouse);
			return 0;
		case WM_LBUTTONUP:
			TTRACE0("WM_LBUTTONUP\n");
			m_Unpush(m_boInBmClick ? EP_Key : EP_Mouse);
			return 0;
		case WM_LBUTTONDBLCLK:
			TTRACE0("WM_LBUTTONDBLCLK\n");
			if ( ! m_canSendDblClk )
			{
				m_Push(EP_Mouse);
			}
			else
			{
				m_SendCommand(BN_DBLCLK/*BN_DOUBLECLICKED*/);
			}
			return 0;
		case WM_MOUSEMOVE:
			TTRACE1("WM_MOUSEMOVE %X\n", lParam);
			m_curpos = CPoint(lParam);
			if ( ! m_boHasHover && m_pushState == EP_Non )
			{
				m_HoverOn(true);
			}
			else
			{
				m_CheckNowState(true);
			}
			break;
		case WM_ENABLE:
			TTRACE0("WM_ENABLE\n");
			m_CheckNowState(true);
			return 0;
		case WM_GETFONT:
			TTRACE0("WM_GETFONT\n");
			{
				HFONT f = m_hFont;
				if ( f == NULL )
				{
					f = reinterpret_cast<HFONT>(::SendMessage(::GetParent(m_hWnd), WM_GETFONT, 0, 0));
				}
				if ( f == NULL )
				{
					f = reinterpret_cast<HFONT>(::GetStockObject(SYSTEM_FONT));
				}
				return reinterpret_cast<LRESULT>(f);
			}
			break;
		case WM_SETFONT:
			TTRACE0("WM_SETFONT\n");
			m_hFont = reinterpret_cast<HFONT>(wParam);
			if ( lParam != 0 )
			{
				m_RedrawWindow();
			}
			return 0;
		case WM_DESTROY:
			TTRACE0("WM_DESTROY\n");
			m_pushState = EP_Non;
			m_boHasHover = false;
			SetButtonState(ES_Normal);
			m_lastState = static_cast<EState>(-1);
			m_hFont = NULL;
			m_boInBmClick = false;
			m_boIsOnClickDisp = false;
			break;
		case BM_CLICK:
			TTRACE0("BM_CLICK\n");
			{
				m_boInBmClick = true;
				LRESULT r = _super::WindowProc(message, wParam, lParam);
				m_boInBmClick = false;
				return r;
			}
			break;
		case BM_GETSTATE:
			TTRACE0("BM_GETSTATE\n");
			{
				UINT r = 0;
				r |= (m_nowState == ES_Pushed) ? BST_PUSHED : 0;//����
				r |= HasFocus() ? BST_FOCUS : 0;//�t�H�[�J�X
				return r;
			}
			break;
		case BM_SETSTATE:
			TTRACE0("BM_SETSTATE\n");
			if ( wParam == 0 )
			{
				m_Unpush(EP_Message);
			}
			else
			{
				m_Push(EP_Message);
			}
			return 0;
		case 0x02A1/*WM_MOUSEHOVER*/:
			TTRACE0("WM_MOUSEHOVER\n");
			m_HoverOn(true);
			break;
		case 0x02A3/*WM_MOUSELEAVE*/:
			TTRACE0("WM_MOUSELEAVE\n");
			if ( m_boHasHover )
			{
				m_HoverOff(true);
			}
			else
			{
				m_CheckNowState(true);
			}
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::ModifyStyle(0, SS_NOTIFY);
		_super::PreSubclassWindow();
		m_CheckNowState(false);
	}

private:
	/// �������
	enum EPush
	{
		EP_Non,		///< �������Ė���
		EP_Mouse,	///< �}�E�X�ɂ�鉟��
		EP_Key,		///< �L�[�ɂ�鉟��
		EP_Message	///< ���b�Z�[�W�ɂ�鉟��
	};
	EPush		m_pushState;			///< �������
	bool		m_boHasHover;			///< true �z�o�[��
	bool		m_boInBmClick;			///< BM_CLICK �������t���O
	bool		m_boIsOnClickDisp;		///< OnClickButton �R�[���ς�;
	TCHAR		m_shortcutKey;			///< �V���[�g�J�b�g�L�[�R�[�h�iALT+ �ŉ����ꂽ���ƂɂȂ�j
	HFONT		m_hFont;				///< �t�H���g
	bool		m_canSendDblClk;		///< BN_DOUBLECLICKED ���M�T�|�[�g
	EState		m_nowState;				///< ���݂̏��
	EState		m_lastState;			///< �Ō�ɕ`�悵�� State
	CPoint		m_curpos;				///< ���݂̃|�W�V����
	EPressMode	m_pressedMode;			///< ��������������Ԃ����H
	bool m_WithPressedMessage(void) const
	{
		return m_pressedMode != PM_Unused;
	}
	bool m_IsPressedLockMode(void) const
	{
		return m_pressedMode == PM_Lockable;
	}
	/// �����m�F
	bool m_IsAlive(void) const
	{
		return !! ::IsWindow(GetSafeHwnd());
	}
	/// �e�ɃR�}���h��Post����B
	void m_SendCommand(UINT uCmd)
	{
		if ( m_IsAlive() )
		{
			MFCLIB::SendCommandMessage(this, uCmd);
		}
	}
	/// ���h���[
	void m_RedrawWindow(void)
	{
		if ( m_IsAlive() )
		{
			_super::RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
		}
	}
	/// ���݂̃C���X�^���X�̏�ԁA�E�B���h�E�̏�Ԃ��`�F�b�N���A nowState ��ݒ�
	void m_CheckNowState(bool boRedraw)
	{
		if ( m_IsAlive() )
		{
			SetButtonState(OnCheckState(m_nowState));
			if ( m_nowState != m_lastState )
			{
				m_lastState = m_nowState;
				OnChangeState(m_nowState);
				if ( boRedraw )
				{
					m_RedrawWindow();
				}
			}
		}
	}
	/// Hover On
	void m_HoverOn(bool boRedraw)
	{
		#ifndef _WIN32_WCE
		if ( ! m_boHasHover )
		{
			m_boHasHover = true;
			m_CheckNowState(boRedraw);
			m_SendCommand(BN_HILITE);
			m_HoverSub();
		}
		#endif
	}
	/// Hover OFF
	void m_HoverOff(bool boRedraw)
	{
		#ifndef _WIN32_WCE
		if ( m_boHasHover )
		{
			m_boHasHover = false;
			m_CheckNowState(boRedraw);
			m_SendCommand(BN_UNHILITE);
		}
		#endif
	}
	/// �������� 
	void m_Push(EPush ep)
	{
		if ( ! m_IsAlive() ) { return; }
		if ( m_pushState == EP_Non )
		{
			m_pushState = ep;
			if ( ! m_IsPressedLockMode() )
			{
				_super::SetCapture();
				if ( m_WithPressedMessage() )
				{
					m_SendCommand(BN_PUSHED_T);
				}
			}
			else
			{
				m_SendCommand(BN_PUSHED_T);
			}
			m_CheckNowState(true);
		}
		_super::SetFocus();
	}
	/// �����[�X���� 
	void m_Unpush(EPush ep)
	{
		if ( ! m_IsAlive() ) { return; }
		if ( m_pushState != ep ) { return; }
		m_pushState = EP_Non;
		::ReleaseCapture();
		if ( ep != EP_Message ) 
		{
			CPoint po = m_curpos;
			ClientToScreen(&po);
			bool onButton = IsOnButton(po);
			if ( ep == EP_Key || onButton )
			{
				m_boIsOnClickDisp = true;
				OnClickButton();
				if ( ! m_IsAlive() ) { return; }
				#ifdef _WIN32_WCE
					m_CheckNowState(false);
					m_RedrawWindow();
					if ( m_IsPressedLockMode() || m_WithPressedMessageMode() )
					{
						m_SendCommand(BN_UNPUSHED_T);
					}
					m_SendCommand(BN_CLICKED);
					return;
				#endif
				if ( m_IsPressedLockMode() || m_WithPressedMessage() )
				{
					m_SendCommand(BN_UNPUSHED_T);
				}
				m_SendCommand(BN_CLICKED);
			}
			if ( ! onButton )
			{
				m_HoverOff(false);
			}
		}
		m_CheckNowState(false);
		m_RedrawWindow();
	}
	/// ���̃z�o�[�C�x���g�����炤���߂̏���
	void m_HoverSub(void)
	{
		#ifndef _WIN32_WCE
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof (TRACKMOUSEEVENT);
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = HOVER_DEFAULT;
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		::_TrackMouseEvent(&tme);
		#endif
	}
};



}; //MFC
}; //TNB



