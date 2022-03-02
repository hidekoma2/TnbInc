#pragma once
/**
 *@file
 * �h���b�O&�h���b�v�����֌W�̃w�b�_
 *
 *		�R���g���[�����h���b�O���h���b�v���邽�߂̋@�\��񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"
#include "TnbBitmapImage.h"
#include "TnbDrawable.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �h���b�O�R���g���[���A�h�C���N���X
 *
 *		�{�N���X���g�p����ƁA�h���b�O���邱�Ƃ��\�ɂȂ�܂��B
 *
 *		�h���b�N�J�n���A�h���b�O���~���A�h���b�v���Ƀ��b�Z�[�W��e�ɒʒm���܂��B
 *		�ʒm���b�Z�[�W�͈ȉ��̂悤�ɂȂ�܂��B \n
 *			message = MSG (�e���v���[�g�̈������X�g�A�Q��)	\n
 *			WPARAM�̏��WORD = �O���[�vID�i SetDragGroupID() �Q�Ɓj�B\n
 *			WPARAM�̉���WORD = �ʒm��ʁi CDragAddinT::EDragNotifyCode �j �B	\n
 *			LPARAM�̏��WORD = ����R���g���[��ID �i�R�}���h��ʂ��h���b�N�J�n��L�����Z���̎��͂O�Œ�j�B	\n
 *			LPARAM�̉���WORD = �h���b�N�R���g���[��ID �B	\n
 *		���ʒm��� ISDROP �́A���ʂ̃R���g���[�������ǁA�h���b�v�Ώۂɂ������ꍇ�Ɏg�p���܂��B
 *
 *	@note ���WORD�� HIWORD() �}�N�����g���ƕ֗��ł��B
 *	@note ����WORD�� LOWORD() �}�N�����g���ƕ֗��ł��B
 *
 *	\par �|�C���g:
 *		�h���b�v�ΏۃR���g���[���ɂ��āB\n
 *		�h���b�v�\�̃R���g���[���́A���b�Z�[�W�ŉ����������̂��ΏۂɂȂ�܂��B\n
 *		�܂��A�{�N���X���A�h�C�������R���g���[�����h���b�O����ƁA�}�E�X���ɂ���R���g���[����
 *		GetDragNotifyMessage() �œ����郁�b�Z�[�W�� SendMessage ���܂��B���̍ہA WPARAM �̉���WORD�ɂ�
 *		CANDROP �A���WORD�ɂ� �O���[�vID�ALPARAM �� TDropParam �̃|�C���^�������Ă��܂��B
 *		�R���g���[���� 1 �ŉ�������ƁA�h���b�v�\�ȃR���g���[���Ɣ��f���܂��i TDropParam �ɂ́A
 *		�R���g���[���̗̈��ID�������Ă���A�ύX���\�j�B\n
 *		�܂��A�R���g���[���� 0�i���ہE���T�|�[�g�j��Ԃ����ꍇ�A�e�_�C�A���O�ɁA �ʒm��� = ISDROP ��
 *		�ʒm���b�Z�[�W�𑗂�܂��B����ɂ��W���̃R���g���[�����h���b�v�Ώۂɂ��邱�Ƃ��\�ł��B\n
 *		�Ȃ��A �{�N���X���A�h�C�������R���g���[���́A��L���b�Z�[�W�ɑΉ����Ă���A SetDropMode()
 *		�ŁA�h���b�v���A���ۂ̐ݒ�A SetDragMode() �Ńh���b�O���A���ۂ̐ݒ肪�\�ł��B\n
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *		//= �錾
 *		typedef CDragAddinT<CStatic> CDragStatic;
 *				;
 *		class CXXXX : public CDialog
 *		{
 *				;
 *		 	CDragStatic	m_dragText;		// <- ���X CStatic�����������o������������
 *				;
 *			afx_msg LRESULT OnDroped(WPARAM wParam, LPARAM lParam);
 *				;
 *
 *		//= �ݒ�(�O���[�o��)
 *		static UINT s_uDropMessage = CDragStatic::GetNotifyMessage();
 *		BEGIN_MESSAGE_MAP(CXXXX, CDialog)
 *			//{{AFX_MSG_MAP(CMfcTestAppDlg)
 *				;
 *			//}}AFX_MSG_MAP
 *			ON_REGISTERED_MESSAGE(s_uDropMessage, OnDroped)		//���ǉ�
 *		END_MESSAGE_MAP()
 *
 *		//= �����iOnDroped() ��)
 *		LRESULT CXXXX::OnDroped(WPARAM wParam, LPARAM lParam)
 *		{
 *			switch( LOWORD(wParam) )
 *			{
 *			case CDragStatic::START:		// �h���b�N�J�n
 *				TRACE1("ID = %d ���h���b�O�J�n", LOWORD(lParam));
 *				break;
 *			case CDragStatic::CANCEL:		// �L�����Z���i�h���b�N�I���j
 *				TRACE1("ID = %d �̃h���b�N�𒆎~", LOWORD(lParam));
 *				break;
 *			case CDragStatic::INVALID:		// �����i�h���b�N�I���j
 *				TRACE1("ID = %d �͋��ł��Ȃ��Ƃ���փh���b�v", LOWORD(lParam));
 *				break;
 *			case CDragStatic::DROP:			// �h���b�v�i�h���b�N�I���j
 *				TRACE2("ID = %d ���� %d �փh���b�O", LOWORD(lParam), HIWORD(lParam));
 *				break;
 *			case CDragStatic::HOVER:		// �z�o�[�i�h���b�N���j
 *				TRACE2("ID = %d��%d�̏�", LOWORD(lParam), HIWORD(lParam));
 *				break;
 *			case CDragStatic::LEAVE:		// �z�o�[�I���i�h���b�N���j
 *				TRACE2("ID = %d��%d�̏ォ��͂Ȃꂽ", LOWORD(lParam), HIWORD(lParam));
 *				break;
 *			case CDragStatic::ISDROP:		// �h���b�v�m�F
 *				TRACE2("ID = %d��%d�̏�ցBIDOK �Ȃ�h���b�v�\��", LOWORD(lParam), HIWORD(lParam));
 *				return HIWORD(lParam) == IDOK; 
 *			}
 *			return 0;
 *		}
 *		</pre></td></tr></table>
 *
 *	@param TYP �v�f�̌^�BCWnd �̃T�u�N���X�ł���K�{������܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcDrag.h
 * 
 *	@date 07/10/11 �V�K�쐬
 *	@date 07/10/16 ��p���b�Z�[�W�ɕύX�B
 *	@date 07/10/26 INVALID �ʒm��ǉ��B
 *	@date 07/10/31 �摜�擾���@��ύX�B
 *	@date 07/11/14 ���X�i�[�A�����B �h���b�v�\�̃A�C�e���Ƀ}�[�N��������悤�ɂ����B
 *	@date 07/12/06 �h���b�v�Ώۂ̓o�^���@��ύX�B
 *	@date 07/12/25 �h���b�v�}�[�N�w����@�A�ǉ��B Listener �p�~�B
 *	@date 08/03/04 Drag���� �j�����ɁADrag��Ԃ���������悤�ɏC���B
 *	@date 09/04/15 Leave���A�R���g���[���ĕ`�揈�����œK���B
 */
template<typename TYP = CStatic>
class CDragAddinT : public TYP
{
	DEFSUPER(TYP);
public:

	/**
	 * �h���b�O�ʒm��ʃR�[�h.
	 *		�ʒm���b�Z�[�W��WPARAM �̉���WORD�Ɋi�[����Ă��܂��B
	 *	@note �e�_�C�A���O�ɒʒm����܂��B
	 */
	enum EDragNotifyCode
	{
		START = 0,		///< �h���b�N�J�n.
		HOVER,			///< �z�o�[�i�h���b�N���j.
		LEAVE,			///< �z�o�[�I���i�h���b�N���j.
		CANCEL,			///< �L�����Z���i�h���b�N�I���j.
		INVALID,		///< �����h���b�v�i�h���b�N�I���j.
		DROP,			///< �L���h���b�v�i�h���b�N�I���j.
		ISDROP,			///< �h���b�v�m�F.
	};

	/**
	 * �h���b�v�R���g���[���R�[�h.
	 *		�ʒm���b�Z�[�W�� WPARAM �̉���WORD�Ɋi�[����Ă��܂��B
	 *	@note �������g�ɒʒm����܂��B
	 */
	enum EDropCtrlCode
	{
		CANDROP = 100,	/**<
						 *	�h���b�v�\���`�F�b�N���ɒʒm���܂��B
						 *	�ʒm���󂯂��R���g���[���� LRESULT �� true(�h���b�v�\),false(�s�\) ��Ԃ��܂��B
						 *	LPARAM �� TDropParam �̃|�C���^���i�[����܂��i�K�v�ɉ����ĕύX���܂��j�B
						 */
	};
	
	/// �h���b�v�R���g���[���p�����[�^
	struct TDropParam
	{
		int		dragID;	///< �h���b�O���̃R���g���[��ID�B�ύX�s�B
		POINT	point;	///< �}�E�X�J�[�\���ʒu�i�N���C�A���g���W�j�B�ύX�s�B
		HRGN	hRgn;	///< �N���C�A���g�̗̈�i�N���C�A���g���W�j�B�ύX�\�B
		WORD	ctrlID;	///< �R���g���[��ID�B�ύX�\�B
	};
	
	/**
	 * [�擾] �ʒm�p���b�Z�[�W�R�[�h�擾.
	 *	@return �ʒm�p���b�Z�[�W�R�[�h.
	 */
	static UINT GetDragNotifyMessage(void)
	{
		return ::RegisterWindowMessage(_T("TnbMfcDragAddin"));
	}

	/// �R���X�g���N�^
	CDragAddinT(void) 
		: m_targetMarkColor(::GetSysColor(COLOR_HIGHLIGHT)), m_targetMarkThick(2)
		, m_arrowIcon(NULL), m_noIcon(NULL)
		, m_wGroupID(0), m_boCanDragging(true), m_boCanDropping(true)
		, m_boIsDragging(false), m_nowDropItem(-1), m_nowDropItemRgn(NULL)
	{
		m_nMessageCode = GetDragNotifyMessage();
		m_nowDropItemRgn = ::CreateRectRgn(0, 0, 0, 0);
	}

	/// �f�X�g���N�^
	~CDragAddinT(void)
	{
		_DeleteObject(m_nowDropItemRgn);
	}

	/**
	 * [�ݒ�] �h���b�v�O���[�vID�ݒ�
	 *	@note ���̒l�͒ʒm���b�Z�[�W�� WPARAM �̏��WORD�Ɋi�[����܂��B
	 *			�O���[�v�l��ݒ肷�邱�ƂŁA�����ɕ����̃h���b�N�A���h�h���b�v�O���[�v��z�u�ł��܂��B
	 *	@param w �O���[�v�l�B
	 */
	void SetDragGroupID(WORD w)
	{
		m_wGroupID = w;
	}

	/**
	 * [�擾] �h���b�v�O���[�vID�擾
	 *	@return �O���[�v�l�B
	 */
	WORD GetDragGroupID(void) const
	{
		return m_wGroupID;
	}

	/**
	 * [�ݒ�] �h���b�N���r�b�g�}�b�v�ݒ�.
	 *		�h���b�N���̊G���w��ł��܂��B�w�肵�Ȃ��ꍇ�A�R���g���[���̑傫���ŃL���v�`�����������g���܂��B
	 *	@param bmp �r�b�g�}�b�v�B
	 */
	void SetDraggingBitmap(CBitmapHandle bmp)
	{
		m_bmpDragging = bmp;
	}

	/**
	 * [�ݒ�] �h���b�v��̃}�[�N�ݒ�.
	 *		�h���b�v�\�̃A�C�e���Ƀ}�[�N��\�����邱�Ƃ��o���܂��B
	 *	@note �ݒ肵�Ȃ��ƁA�I��F�̑����Q�ɂȂ��Ă��܂��B
	 *	@param other �R�s�[���B
	 */
	template<typename T>
	void SetDropTargetMark(const CDragAddinT<T>& other)
	{
		const IDrawable*	draw = NULL;
		COLORREF			color;
		int					thick;
		other.GetDropTargetMark(draw, color, thick);
		if ( draw != NULL )
		{
			m_targetMarkDrawer = draw->Clone();
		}
		m_targetMarkColor = color;
		m_targetMarkThick = thick;
	}

	/**
	 * [�ݒ�] �h���b�v��̃}�[�N�ݒ�.
	 *		�h���b�v�\�̃A�C�e���Ƀ}�[�N��\�����邱�Ƃ��o���܂��B
	 *	@note �ݒ肵�Ȃ��ƁA�I��F�̑����Q�ɂȂ��Ă��܂��B
	 *	@param draw �`����iResize() ���g�p���܂��j�B������ێ����܂��̂ŁA draw �͔j�����Ă��܂��܂���B
	 */
	void SetDropTargetMark(const IDrawable& draw)
	{
		m_targetMarkDrawer = draw.Clone();
	}

	/**
	 * [�ݒ�] �h���b�v��̃}�[�N�ݒ�.
	 *		�h���b�v�\�̃A�C�e���Ƀ}�[�N��\�����邱�Ƃ��o���܂��B
	 *	@note �ݒ肵�Ȃ��ƁA�I��F�̑����Q�ɂȂ��Ă��܂��B
	 *	@param color �J���[�B
	 *	@param thick �����B�O�Ȃ�ݒ�ύX�Ȃ��B
	 */
	void SetDropTargetMark(COLORREF color, int thick = 0)
	{
		m_targetMarkDrawer.Null();
		m_targetMarkColor = color;
		if ( thick > 0 )
		{
			m_targetMarkThick = thick;
		}
	}

	/**
	 * [�擾] �h���b�v��̃}�[�N�ݒ�擾.
	 *	@param[out] _draw �`����B
	 *	@param[out] _color �J���[�B
	 *	@param[out] _thick �����B
	 */
	void GetDropTargetMark(const IDrawable*& _draw, COLORREF& _color, int& _thick) const
	{
		_draw = m_targetMarkDrawer;
		_color = m_targetMarkColor;
		_thick = m_targetMarkThick;
	}

	/**
	 * [�ݒ�] �h���b�O���[�h�ݒ�.
	 *	@param can true �Ȃ�h���b�O�\�ɂȂ�܂��B false �Ȃ�h���b�O�o���Ȃ��Ȃ�܂��B
	 */
	void SetDragMode(bool can)
	{
		m_boCanDragging = can;
	}

	/**
	 * [�擾] �h���b�O���[�h�擾.
	 *	@retval true �h���b�O�\.
	 *	@retval false �h���b�O�s�\.
	 */
	bool CanDragging(void) const
	{
		return m_boCanDragging;
	}

	/**
	 * [�ݒ�] �h���b�v���[�h�ݒ�.
	 *	@param can true �Ȃ�h���b�v�������܂��B false �Ȃ狖���܂���B
	 */
	void SetDropMode(bool can)
	{
		m_boCanDropping = can;
	}

	/**
	 * [�擾] �h���b�v���[�h�擾.
	 *	@retval true �h���b�v�\.
	 *	@retval false �h���b�v�s�\.
	 */
	bool CanDropping(void) const
	{
		return m_boCanDropping;
	}

protected:

	/**
	 * [�ʒm] ���b�Z�[�W�ʒm.
	 *	@param pWnd �ʒm��B
	 *	@param wParam WPARAM�B
	 *	@param lParam LPARAM�B
	 *	@return �����B
	 */
	bool SendNofityMessage(CWnd* pWnd, WPARAM wParam, LPARAM lParam) const
	{
		return !! ::SendMessage(*pWnd, m_nMessageCode, wParam, lParam);
	}

	/**
	 * [�m�F] �ʒm���b�Z�[�W�m�F.
	 *	@param message ��r���b�Z�[�W�B
	 *	@retval true �ʒm���b�Z�[�W�B
	 *	@retval false �ہB
	 */
	bool IsNofityMessage(UINT message) const
	{
		return m_nMessageCode == message;
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
		if ( message == m_nMessageCode )
		{
			if ( wParam == static_cast<WPARAM>(MAKELONG(CANDROP, m_wGroupID)) )
			{
				return m_boCanDropping;
			}
		}
		else if ( message == WM_LBUTTONDOWN )
		{
			if ( m_BeginDrag() )
			{
				m_SendCommand(START, 0);
				return 0;
			}
		}
		else if ( message == WM_LBUTTONUP )
		{
			if ( m_EndDrag() )
			{
				if ( m_nowDropItem >= 0 )
				{
					m_SendCommand(DROP, m_nowDropItem);
				}
				else
				{
					m_SendCommand(INVALID, 0);
				}
				return 0;
			}
		}
		else if ( message == WM_CAPTURECHANGED || message == WM_RBUTTONDOWN )
		{
			if ( m_EndDrag() )
			{
				m_SendCommand(CANCEL, 0);
			}
		}
		else if ( message == WM_MOUSEMOVE )
		{
			if ( m_boIsDragging )
			{
				m_OnMouseMove();
			}
		}
		else if ( message == WM_DESTROY )
		{
			m_EndDrag();
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [�ʒm] �h���b�O�J�n.
	 *	@param[in,out] _rc �A�C�e���͈́Bin�� ::GetWindowRect() �����͈͂������Ă��܂��iWindow���W�j�B
	 *	@param[in] po �}�E�X�̃|�W�V�����iWindow���W�j�B
	 *	@retval 0 �h���b�N�o���Ȃ�.
	 *	@retval 1�ȏ� �R���g���[��ID
	 */
	virtual int OnBeginDrag(RECT& _rc, const POINT& po)
	{
		if ( ! ::DragDetect(m_hWnd, po) )	//����ȊO�őΉ�������
		{
			return 0;
		}
		return _super::GetDlgCtrlID();
	}	

	/**
	 * [�ʒm] �h���b�O�ʒm.
	 *	@note �e�ɒʒm������̂Ɠ������̂��ʒm����܂��B
	 *	@param cmd �R�}���h.
	 *	@param draggingItem �h���b�O���A�C�e��ID
	 *	@param dropedItem �h���b�v��A�C�e��ID
	 */
	virtual void OnDragNofity(UINT cmd, int draggingItem, int dropedItem)
	{
	}

private:
	//-�ݒ�
	IDrawable::Ptr		m_targetMarkDrawer;	///< �h���b�v�ΏۃA�C�e���}�[�N�`��
	COLORREF			m_targetMarkColor;	///< �h���b�v�ΏۃA�C�e���}�[�N�F
	int					m_targetMarkThick;	///< �h���b�v�ΏۃA�C�e���}�[�N����
	CBitmapHandle		m_bmpDragging;		///< �h���b�N���̊G
	WORD				m_wGroupID;			///< �O���[�vID
	bool				m_boCanDragging;	///< �h���b�O���邱�Ƃ��\�H
	bool				m_boCanDropping;	///< �h���b�v���邱�Ƃ��\�H
	//-���� Work
	UINT				m_nMessageCode;		///< ��p���b�Z�[�W
	int					m_draggingItemID;	///< �h���b�O���̃A�C�e��ID
	HICON				m_arrowIcon;		///< ���J�[�\��
	HICON				m_noIcon;			///< �֎~�J�[�\��
	CImageList			m_imglist;			///< �C���[�W���X�g�i�h���b�N���̕\���p�j
	bool				m_boIsDragging;		///< true �Ȃ�h���b�N��
	CPoint				m_dragOffset;		///< �h���b�N�J�n���̃}�E�X�J�[�\���ƃA�C�e���̂���
	//-MOVE��
	int					m_nowDropItem;		///< ���ɂ���A�C�e��
	HRGN				m_nowDropItemRgn;	///< ���ɂ���A�C�e���̌`
	CWnd*				m_nowDropWnd;		///< ���ɂ���A�C�e��
	/// �h���b�v�\�}�[�N�`��
	void m_DrawTargetMark(CDC* pDC, HRGN hRgn)
	{
		if ( m_targetMarkDrawer.IsNull() )
		{
			CBrush br;
			br.CreateSolidBrush(m_targetMarkColor);
			::FrameRgn(*pDC, m_nowDropItemRgn, br, m_targetMarkThick, m_targetMarkThick);
		}
		else
		{
			CRect rc;
			::GetRgnBox(hRgn, &rc);
			::SelectClipRgn(*pDC, hRgn);
			m_targetMarkDrawer->Resize(rc.Size());
			m_targetMarkDrawer->Draw(*pDC, rc.left, rc.top);
			::SelectClipRgn(*pDC, NULL);
		}
	}
	/// �}�E�X���[�u
	void m_OnMouseMove(void)
	{
		if ( m_arrowIcon == NULL ) { m_arrowIcon = ::GetCursor(); }
		CPoint pos;
		::GetCursorPos(&pos);
		m_imglist.DragMove(pos - m_dragOffset);
		//
		CWnd* pParent = _super::GetParent();
		if ( pParent == NULL )
		{
			return;
		}
		int backupDropItem = m_nowDropItem;
		CWnd* pBackupDropWnd = m_nowDropWnd;
		RECT backupDropRect;
		::GetRgnBox(m_nowDropItemRgn, &backupDropRect);
		m_nowDropItem = -1;
		HCURSOR hCursor = NULL;
		//
		CWnd* P = pParent->GetParentOwner();
		if ( P == NULL )
		{
			return;
		}
		CRect rc;
		P->GetClientRect(&rc);
		P->ClientToScreen(&rc);
		if ( rc.PtInRect(pos) )
		{
			pParent->ScreenToClient(&pos);
			CWnd* P = pParent->ChildWindowFromPoint(pos, CWP_SKIPINVISIBLE);
			if ( P != NULL )
			{
				TDropParam param;
				param.dragID = m_draggingItemID;
				param.ctrlID = static_cast<WORD>(P->GetDlgCtrlID());
				if ( param.ctrlID != 0 )
				{
					pParent->ClientToScreen(&pos);
					P->ScreenToClient(&pos);
					param.point = pos;
					RECT rect;
					P->GetClientRect(&rect);
					param.hRgn = ::CreateRectRgn(0, 0, rect.right, rect.bottom);
					P->GetWindowRgn(param.hRgn);
					if ( m_CanDropping(P, param) )
					{
						if ( param.ctrlID != m_draggingItemID )
						{
							_DeleteObject(m_nowDropItemRgn);
							m_nowDropItemRgn = param.hRgn;
							m_nowDropItem = param.ctrlID;
							m_nowDropWnd = P;
							hCursor = m_arrowIcon;
						}
						else
						{
							_DeleteObject(param.hRgn);
						}
					}
					else
					{
						_DeleteObject(param.hRgn);
					}
				}
			}
		}
		if ( hCursor == NULL )
		{
			if ( m_noIcon == NULL ) { m_noIcon = ::LoadCursor(NULL, IDC_NO); }
			hCursor = m_noIcon;
		}
		::SetCursor(hCursor);
		if ( backupDropItem != m_nowDropItem )
		{
			if ( backupDropItem >= 0 )
			{
				m_SendCommand(LEAVE, backupDropItem);
				m_imglist.DragShowNolock(false);
				CWnd* P = pBackupDropWnd->GetParent();
				if ( P != NULL )
				{
					pBackupDropWnd->ClientToScreen(&backupDropRect);
					P->ScreenToClient(&backupDropRect);
					P->RedrawWindow(&backupDropRect);
				}
				else
				{
					pBackupDropWnd->RedrawWindow(&backupDropRect);
				}
				m_imglist.DragShowNolock(true);
			}
			if ( m_nowDropItem >= 0 )
			{
				m_SendCommand(HOVER, m_nowDropItem);
				if ( IS_RGBVALUE(m_targetMarkColor) )
				{
					CDC* pDC = m_nowDropWnd->GetDC();
					m_imglist.DragShowNolock(false);
					m_DrawTargetMark(pDC, m_nowDropItemRgn);
					m_imglist.DragShowNolock(true);
					m_nowDropWnd->ReleaseDC(pDC);
				}
			}
		}
	}
	/// �\�Ȃ� true
	bool m_CanDropping(CWnd* P, TDropParam& _param)
	{
		ASSERTLIB( P != NULL );
		ASSERTLIB( _param.ctrlID != 0 );
		WPARAM wParam = MAKELONG(CANDROP, m_wGroupID);
		LPARAM lParam = reinterpret_cast<LPARAM>(&_param);
		if ( P->SendMessage(m_nMessageCode, wParam, lParam) )
		{
			return true;
		}
		if ( m_SendCommand(ISDROP, _param.ctrlID) )
		{
			return true;
		}
		return false;
	}
	//�@�A�C�e���̃h���b�O�J�n
	bool m_BeginDrag(void)
	{
		if ( m_boIsDragging )
		{
			m_EndDrag();
		}
		if ( ! m_boCanDragging ) { return false; }
		CPoint pos;
		::GetCursorPos(&pos);
		CRect rect;
		_super::GetWindowRect(rect);
		CRect winRect = rect;
		m_draggingItemID = OnBeginDrag(rect, pos);
		if ( m_draggingItemID <= 0 )
		{
			return false;
		}
		CPoint controlPoint = rect.TopLeft();
		m_dragOffset = pos - controlPoint;
		CBitmapHandle hh = m_bmpDragging;
		CSize size;
		if ( hh.IsNull() )
		{
			CBitmapImage bi;
			bi.SetFromWindow(m_hWnd, RGB(0, 0, 1));
			rect.OffsetRect(-winRect.TopLeft());
			hh = bi.Cut(rect).GetBitmapHandle();
			hh.GetSize(size);
		}
		else
		{
			VERIFY( hh.GetSize(size) );
		}
		m_imglist.Create(size.cx, size.cy, ILC_MASK, 0, 1);
		m_imglist.Add(CBitmap::FromHandle(hh), RGB(0, 0, 1));
		m_imglist.BeginDrag(0, CPoint(0, 0));
		m_imglist.DragEnter(NULL, controlPoint);
		m_imglist.DragMove(controlPoint);
		SetCapture();
		m_boIsDragging = true;
		return true;
	}
	// �h���b�O�̏I��
	bool m_EndDrag(void)
	{
		if ( ! m_boIsDragging ) { return false; }
		m_boIsDragging = false;
		m_imglist.EndDrag();
		m_imglist.DeleteImageList();
		if ( m_nowDropItem >= 0 && ::IsWindow(*m_nowDropWnd) )
		{
			m_nowDropWnd->Invalidate();
		}
		ReleaseCapture();
		return true;
	}
	/// �e�ɃR�}���h��ʒm����B
	bool m_SendCommand(UINT uCmd, int item)
	{
		WPARAM wParam = MAKELONG(uCmd, m_wGroupID);
		LPARAM lParam = MAKELONG(m_draggingItemID, item);
		LRESULT r = 0;
		CWnd* pWnd = _super::GetParent();
		if ( pWnd != NULL )
		{
			r = pWnd->SendMessage(m_nMessageCode, wParam, lParam);
		}
		OnDragNofity(uCmd, m_draggingItemID, item);
		return !! r;
	}
};



}; //MFC
}; //TNB



