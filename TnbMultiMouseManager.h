#pragma once
/**
 *@file
 * �}���`�}�E�X�Ǘ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDummyWindow.h"
#include "TnbPointerVector.h"
#include "TnbBitmapImage.h"
#include <math.h>



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * �}���`�}�E�X�Ď��C���^�[�t�F�[�X.
 *
 *		�����̃}�E�X�̃f�[�^���擾����C���^�[�t�F�[�X�ł��B
 *
 *	@attention XP �ȍ~�̂ݗL���� API ���g�p���Ă��邽�߁A�ŏ��� TnbHotWinver.h ���C���N���[�h���Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbMultiMouseManager.h
 *
 *	@date 10/06/17	CRawMouseWatcher �𒊏ۉ����č쐬�B
 */
struct IMultiMouseWatcher
{
public:

	/**@ingroup WINAPI
	 * �}���`�}�E�X�Ď��̃��X�i�[
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbMultiMouseManager.h
	 *
	 *	@date 10/06/17	CRawMouseWatcher �𒊏ۉ����č쐬�B
	 */
	struct IListener
	{
		/// �f�X�g���N�^
		virtual ~IListener(void) {}

		/// �f�o�C�X�ω�
		enum EDevice
		{
			Device_Connect,		///< �ڑ�
			Device_Reconnect,	///< �Đڑ�
			Device_Remove,		///< �폜
		};

		/**
		 * [�ʒm] �f�o�C�X�ω��ʒm.
		 *	@param index �}�E�X�C���f�b�N�X.
		 *	@param device �f�o�C�X�ω�
		 */
		virtual void OnMultiMouseDevice(INDEX index, EDevice device) = 0;

		/**
		 * [�ʒm] �C�x���g�ʒm.
		 *	@param index �}�E�X�C���f�b�N�X.
		 *	@param mouse ���
		 */
		virtual void OnMultiMouseEvent(INDEX index, const RAWMOUSE& mouse) = 0;
	};


	//--------------------------------


	/**
	 * [�o�^] ���X�i�[�o�^.
	 *	@param P ���X�i�[
	 */
	virtual void SetListener(IListener* P) = NULL;

	/**
	 * [�擾] �}�E�X���擾.
	 *	@return �}�E�X��.
	 */
	virtual size_t GetMouseCount(void) const = NULL;

	/**
	 * [�ݒ�] �Ď��J�n.
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool Start(void) = NULL;

	/**
	 * [�ݒ�] �Ď���~.
	 */
	virtual void Stop(void) = NULL;
};



/**@ingroup WINAPI
 * �}���`�}�E�X�Ǘ��N���X
 *
 *		�����̃}�E�X���Ǘ����܂��B�J�[�\���̕\���A�ړ��A�C�x���g�̒ʒm�����ꂼ��ŊǗ��ł��܂��B
 *
 *	@par�g�p��
 *		\code
 *		// �w�b�_
 *		class CFooDlg : public CDialog, CMultiMouseManager::IListener
 *		{
 *					;
 *			virtual void OnMultiMouseDevice(INDEX index, EDevice device);
 *			virtual void OnMultiMouseButton(INDEX index, EButton button, EEvent event, int x, int y);
 *			virtual void OnMultiMouseMove(INDEX index, BYTE buttonFlags, int x, int y);
 *			virtual void OnMultiMouseWheel(INDEX index, int d);
 *					;
 *			CMultiMouseManager		m_mmm;
 *			CRawMouseWatcher		m_rmw;
 *					;
 *
 *		// �\�[�X
 *		void CFooDlg::XX(void)
 *		{
 *			m_mmm.SetListener(this);
 *			m_mmm.SetDefaultCursorImage(6, 6, IDB_POINTER); //�J�[�\���`�w��
 *			m_mmm.EnableMouseCursor(false); //�V�X�e���J�[�\�����쐧��
 *			ShowCursor(FALSE);  //�K�v�ɉ����ăV�X�e���J�[�\�����\����
 *			m_mmm.Start(&m_rmw, GetSafeHwnd()); //�J�n
 *		}
 *		void CFooDlg::OnMultiMouseDevice(INDEX index, EDevice device)
 *		{
 *		}
 *		void CFooDlg::OnMultiMouseButton(INDEX index, EButton button, EEvent event, int x, int y)
 *		{
 *		}
 *		void CFooDlg::OnMultiMouseMove(INDEX index, BYTE buttonFlags, int x, int y)
 *		{
 *		}
 *		void CFooDlg::OnMultiMouseWheel(INDEX index, int d)
 *		{
 *		}
 *		\endcode
 *
 *	@attention XP �ȍ~�̂ݗL���� API ���g�p���Ă��邽�߁A�ŏ��� TnbHotWinver.h ���C���N���[�h���Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbMultiMouseManager.h
 *
 *	@date 10/04/20	�V�K�쐬
 *	@date 10/06/17	IMultiMouseWatcher ���g�p����悤�ɕύX�B
 *	@date 10/07/14	�X���b�h�Z�[�t���B
 *	@date 10/07/14	EnableMouseCursor() ��p�~(���z��)�B
 */
class CMultiMouseManager
{
public:
	
	/**@ingroup WINAPI
	 * �}���`�}�E�X�Ǘ��̃��X�i�[
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbMultiMouseManager.h
	 *
	 *	@date 10/04/20	�V�K�쐬
	 *	@date 10/06/17	IMultiMouseWatcher �̃T�u�N���X��
	 */
	struct IListener : IMultiMouseWatcher::IListener
	{
		/// �f�X�g���N�^
		virtual ~IListener(void) {}

		/// �{�^�����
		enum EButton
		{
			Button_Left,		///< ��
			Button_Right,		///< �E
			Button_Middle,		///< ����
		};

		/// �C�x���g
		enum EEvent
		{
			Event_Up,			///< �A�b�v
			Event_Down,			///< �_�E��
		};

		/**
		 * [�ʒm] �{�^���ʒm.
		 *	@param index �}�E�X�C���f�b�N�X.
		 *	@param button �{�^�����
		 *	@param event �C�x���g
		 *	@param x X ���W
		 *	@param y Y ���W
		 */
		virtual void OnMultiMouseButton(INDEX index, EButton button, EEvent event, int x, int y) = 0;

		/**
		 * [�ʒm] �ړ��ʒm.
		 *	@param index �}�E�X�C���f�b�N�X.
		 *	@param buttonFlags �L�[��ԁB _BIT(EButton) �� �� ���� �O�ȊO�Ȃ�v���X����Ă��܂��B
		 *	@param x X ���W
		 *	@param y Y ���W
		 */
		virtual void OnMultiMouseMove(INDEX index, BYTE buttonFlags, int x, int y) = 0;

		/**
		 * [�ʒm] �z�C�[���ʒm.
		 *	@param index �}�E�X�C���f�b�N�X.
		 *	@param d �z�C�[���ړ���
		 */
		virtual void OnMultiMouseWheel(INDEX index, int d) = 0;

		/**
		 * [�ʒm] �C�x���g�ʒm.
		 *	@param index �}�E�X�C���f�b�N�X.
		 *	@param mouse ���
		 */
		virtual void OnMultiMouseEvent(INDEX index, const RAWMOUSE& mouse) { }

		#ifdef _TnbDOXYGEN	//Document�쐬�p�V���{��

		/// �f�o�C�X�ω�
		enum EDevice
		{
			Device_Connect,		///< �ڑ�
			Device_Reconnect,	///< �Đڑ�
			Device_Remove,		///< �폜
		};

		/**
		 * [�ʒm] �f�o�C�X�ω��ʒm.
		 *	@param index �}�E�X�C���f�b�N�X.
		 *	@param device �f�o�C�X�ω�
		 */
		virtual void OnMultiMouseDevice(INDEX index, EDevice device) = 0;

		#endif

		/**
		 * [�쐬] �}�E�X�C�x���g�t���O�쐬.
		 *	@param button �{�^�����
		 *	@param event �C�x���g
		 *	@return �}�E�X�C�x���g�t���O.�@
		 */
		static DWORD MakeMouseEventFlags(EButton button, EEvent event)
		{
			if ( event == Event_Down )
			{
				switch ( button )
				{
				case Button_Left:	return MOUSEEVENTF_LEFTDOWN;
				case Button_Right:	return MOUSEEVENTF_RIGHTDOWN;
				case Button_Middle:	return MOUSEEVENTF_MIDDLEDOWN;
				}
			}
			else if ( event == Event_Up )
			{
				switch ( button )
				{
				case Button_Left:	return MOUSEEVENTF_LEFTUP;
				case Button_Right:	return MOUSEEVENTF_RIGHTUP;
				case Button_Middle:	return MOUSEEVENTF_MIDDLEUP;
				}
			}
			return 0;
		}
	};


	//-----------------------------------


	/// �R���X�g���N�^
	CMultiMouseManager(void) : m_pWatcher(NULL)
	{
		::SetRectEmpty(&m_in.m_corsorRect);
	}

	/**
	 * [�o�^] ���X�i�[�o�^.
	 *	@param P ���X�i�[
	 */
	void SetListener(IListener* P)
	{
		EXCLUSIVE( &m_in.m_sync );
		m_in.m_pListener = P;
	}

	/**
	 * [�ݒ�] �J�n.
	 *	@param pWatcher �}�E�X�E�H�b�`���[�̃C���X�^���X�B�J�n���Ă��Ȃ����̂��w�肵�܂��B
	 *	@param hParent �e�E�B���h�E�n���h���B�ȗ�����ƁA�O���[�o���œ��삵�܂��B
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	bool Start(IMultiMouseWatcher* pWatcher, HWND hParent = NULL)
	{
		EXCLUSIVE( &m_in.m_sync );
		Stop();
		m_pWatcher = pWatcher;
		m_pWatcher->SetListener(&m_in);
		if ( ! m_in.m_cursorImage.IsNull() )
		{
			m_in.m_hParent = hParent;
			if ( m_pWatcher->Start() )
			{
				size_t len = m_pWatcher->GetMouseCount();
				m_in.m_cursores.SetSize(len);
				loop ( i, len )
				{
					if ( ! m_in.CheckIndex(i) )
					{
						return false;
					}
				}
				return true;
			}
			Stop();
		}
		return false;
	}

	/**
	 * [�ݒ�] ��~.
	 *		�J�n���Ă��Ȃ��ꍇ�A�������܂���B
	 */
	void Stop(void)
	{
		EXCLUSIVE( &m_in.m_sync );
		if ( m_pWatcher != NULL )
		{
			m_pWatcher->Stop();
			m_in.m_cursores.RemoveAll();
			m_pWatcher = NULL;
		}
	}

	/**
	 * [�ݒ�] �f�t�H���g�J�[�\���C���[�W�ݒ�.
	 *	@param hotspotX �z�b�g�X�|�b�g X
	 *	@param hotspotY �z�b�g�X�|�b�g Y
	 *	@param bmp �摜.
	 *	@param color ���ߐF.�ȗ�����ƁA�摜�̍����̐F��K�p���܂��B
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	bool SetDefaultCursorImage(int hotspotX, int hotspotY, CBitmapHandle bmp, COLORREF color = CLR_AUTOSELECT)
	{
		EXCLUSIVE( &m_in.m_sync );
		m_in.m_cursorImage = bmp;
		m_in.m_cursorMaskColor = color;
		m_in.m_cursorHotspot.x = hotspotX;
		m_in.m_cursorHotspot.y = hotspotY;
		return ! m_in.m_cursorImage.IsNull();
	}

	/**
	 * [�ݒ�] �f�t�H���g�J�[�\���C���[�W�ݒ�.
	 *	@note �z�b�g�X�|�b�g�͍���ɂȂ�܂��B
	 *	@param bmp �摜.
	 *	@param color ���ߐF.�ȗ�����ƁA�摜�̍����̐F��K�p���܂��B
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	bool SetDefaultCursorImage(CBitmapHandle bmp, COLORREF color = CLR_AUTOSELECT)
	{
		return SetDefaultCursorImage(0, 0, bmp, color);
	}

	/**
	 * [�ݒ�] �f�t�H���g�J�[�\���C���[�W�ݒ�.
	 *	@note �A�j���[�V�����J�[�\���ɂ͑Ή����Ă��܂���B
	 *	@param hCursor �J�[�\��.
	 *	@param color �����p�̔w�i�F�B�J�[�\���Ɋ܂܂�Ȃ��F���w�肵�Ă��������B�ȗ�����ƃ}�[���^���w�肳���B
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	bool SetDefaultCursorImage(HCURSOR hCursor, COLORREF color = RGB(255, 0, 255))
	{
		EXCLUSIVE( &m_in.m_sync );
		CBitmapHandle bmp;
		POINT hotspot;
		if ( m_IconToBitmap(bmp, hotspot, hCursor, color) )
		{
			m_in.m_cursorImage = bmp;
			m_in.m_cursorMaskColor = color;
			m_in.m_cursorHotspot = hotspot;
			return ! m_in.m_cursorImage.IsNull();
		}
		return false;
	}

	/**
	 * [�擾] �}�E�X���擾.
	 *		���ݔF�����Ă���}�E�X�����擾�ł��܂��B
	 *	@return �}�E�X��.
	 */
	size_t GetMouseCount(void) const
	{
		EXCLUSIVE( &m_in.m_sync );
		return m_in.m_cursores.GetSize();
	}

	/**
	 * [�m�F] �}�E�X�L���m�F.
	 *	@param index �}�E�X�C���f�b�N�X
	 *	@retval true �L��.
	 *	@retval false ����.
	 */
	bool IsValid(INDEX index) const
	{
		EXCLUSIVE( &m_in.m_sync );
		return (m_RefCursorManager(index) != NULL);
	}

	/**
	 * [�ݒ�] �J�[�\���C���[�W�ݒ�.
	 *	@param index �}�E�X�C���f�b�N�X
	 *	@param hotspotX �z�b�g�X�|�b�g X
	 *	@param hotspotY �z�b�g�X�|�b�g Y
	 *	@param bmp �摜.
	 *	@param color ���ߐF�B�ȗ�����ƁA�摜�̍����̐F��K�p���܂��B
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	bool SetCursorImage(INDEX index, int hotspotX, int hotspotY, CBitmapHandle bmp, COLORREF color = CLR_AUTOSELECT)
	{
		EXCLUSIVE( &m_in.m_sync );
		CCursorManager* P = m_GetCursorManager(index);
		if ( P != NULL )
		{
			POINT po = { hotspotX, hotspotY };
			return P->SetImage(bmp, color, po);
		}
		return false;
	}

	/**
	 * [�ݒ�] �J�[�\���C���[�W�ݒ�.
	 *	@param index �}�E�X�C���f�b�N�X
	 *	@param bmp �摜.
	 *	@param color ���ߐF�B�ȗ�����ƁA�摜�̍����̐F��K�p���܂��B
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	bool SetCursorImage(INDEX index, CBitmapHandle bmp, COLORREF color = CLR_AUTOSELECT)
	{
		return SetCursorImage(index, 0, 0, bmp, color);
	}

	/**
	 * [�ݒ�] �J�[�\���C���[�W�ݒ�.
	 *	@note �A�j���[�V�����J�[�\���ɂ͑Ή����Ă��܂���B
	 *	@param index �}�E�X�C���f�b�N�X
	 *	@param hCursor �J�[�\��.
	 *	@param color �����p�̔w�i�F�B�J�[�\���Ɋ܂܂�Ȃ��F���w�肵�Ă��������B
	 *	@retval true ����.
	 *	@retval false ���s.
	 */
	bool SetCursorImage(INDEX index, HCURSOR hCursor, COLORREF color = RGB(255, 0, 255))
	{
		EXCLUSIVE( &m_in.m_sync );
		CCursorManager* P = m_GetCursorManager(index);
		if ( P != NULL )
		{
			CBitmapHandle bmp;
			POINT hotspot;
			if ( m_IconToBitmap(bmp, hotspot, hCursor, color) )
			{
				return P->SetImage(bmp, color, hotspot);
			}
		}
		return false;
	}

	/**
	 * [�\��] �J�[�\���\��.
	 *	@param index �}�E�X�C���f�b�N�X
	 *	@param isShow true �Ȃ�\���B false �Ȃ��\��
	 *	@retval true ����
	 *	@retval false ���s
	 */
 	bool ShowCursor(INDEX index, bool isShow)
	{
		EXCLUSIVE( &m_in.m_sync );
		CCursorManager* P = m_GetCursorManager(index);
		if ( P != NULL )
		{
			P->Show(isShow);
			return true;
		}
		return false;
	}

	/**
	 * [�ݒ�] �J�[�\���ғ��͈͐ݒ�.
	 *	@param index �}�E�X�C���f�b�N�X
	 *	@param rect �ғ��͈�
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool ClipCursor(INDEX index, const RECT& rect)
	{
		EXCLUSIVE( &m_in.m_sync );
		CCursorManager* P = m_GetCursorManager(index);
		if ( P != NULL )
		{
			P->Clip(rect);
			return true;
		}
		return false;
	}
	
	/**
	 * [�ݒ�] �J�[�\���ʒu�ݒ�.
	 *	@param index �}�E�X�C���f�b�N�X�B
	 *	@param x X ���W
	 *	@param y Y ���W
	 *	@retval true ����
	 *	@retval false ���s
	 */
 	bool SetCursorPos(INDEX index, int x, int y)
	{
		EXCLUSIVE( &m_in.m_sync );
		CCursorManager* P = m_GetCursorManager(index);
		if ( P != NULL )
		{
			return P->SetPos(x, y);
		}
		return false;
	}

	/**
	 * [�擾] �J�[�\���ʒu�擾.
	 *	@note �}�E�X�C���f�b�N�X���s���ȏꍇ�� 0, 0 ���Ԃ�܂��B
	 *	@param index �}�E�X�C���f�b�N�X
	 *	@return �ʒu
	 */
	POINT GetCursorPos(INDEX index) const
	{
		EXCLUSIVE( &m_in.m_sync );
		const CCursorManager* P = m_RefCursorManager(index);
		if ( P != NULL )
		{
			return P->GetPos();
		}
		POINT po = { 0, 0 };
		return po;
	}

	/**
	 * [�擾] �{�^����Ԏ擾.
	 *	@note �}�E�X�C���f�b�N�X���s���ȏꍇ�� 0 ���Ԃ�܂��B
	 *	@param index �}�E�X�C���f�b�N�X
	 *	@return �{�^�����
	 */
	BYTE GetButtonState(INDEX index) const
	{
		EXCLUSIVE( &m_in.m_sync );
		const CCursorManager* P = m_RefCursorManager(index);
		if ( P != NULL )
		{
			return P->GetButtonState();
		}
		return 0;
	}

private:

	/**
	 * �J�[�\���^�E�B���h�E�Ǘ�.
	 *		�J�[�\���̌`�������E�B���h�E���Ǘ����܂��B
	 */
	class CCursorWindow : public CDummyWindow::IListener
	{
	public:
		/// �R���X�g���N�^
		CCursorWindow(void) : m_rgn(NULL), m_isChild(false)
		{
		}
		/// �f�X�g���N�^
		~CCursorWindow(void)
		{
			m_DeleteRgn();
		}
		/// �C���[�W�������Ă��邩�H
		bool HasCursorImage(void) const
		{
			return ! m_image.IsEmpty();
		}
		/**
		 * �쐬.
		 *	@param hParent �e�E�B���h�E�n���h���B�ȗ�����ƁA�O���[�o���œ��삵�܂��B
		 */
		bool Create(HWND hParent)
		{
			if ( HasCursorImage() )
			{
				m_isChild = false;
				DWORD style = WS_POPUP;
				HWND hWnd = HWND_TOPMOST;
				if ( hParent != NULL )
				{
					style = WS_CHILD;
					hWnd = HWND_TOP;
					m_isChild = true;
				}
				HWND hFocus = ::GetFocus();
				CStr className = "TNBLIB:MultiMouseMan_";
				if ( m_win.Create(this, NULL, className, style, NULL, hParent) )
				{
					m_win.ModifyStyleEx(0, WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TOPMOST);
					if ( ! m_isChild )
					{
						m_win.ModifyStyleEx(0, WS_EX_LAYERED);
						::SetLayeredWindowAttributes(m_win, RGB(0, 0, 0), 0, 0);
					}
					::SetFocus(hFocus);
					const SIZE& sz = m_image.GetSize();
					::SetWindowPos(m_win, hWnd, 0, 0, sz.cx, sz.cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
					HRGN rgn = ::CreateRectRgn(0, 0, 0, 0);
					::CombineRgn(rgn, m_rgn, NULL, RGN_COPY);
					::SetWindowRgn(m_win, rgn, TRUE);
					Move(0, 0);
					return true;
				}
				::SetFocus(hFocus);
			}
			return false;
		}
		/// �ړ�
		void Move(int x, int y)
		{
			if ( ::IsWindow(m_win) )
			{
				HWND hWnd = HWND_TOPMOST;
				UINT flags = SWP_NOSIZE | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS;
				if ( m_isChild )
				{
					hWnd = NULL;
					flags |= SWP_NOZORDER;
				}
				::SetWindowPos(m_win, hWnd, x - m_hotspot.x, y - m_hotspot.y, 0, 0, flags);
			}
		}
		/// �C���[�W�ݒ�
		bool SetImage(CBitmapHandle bmp, COLORREF color, const POINT& hotspot)
		{
			if ( m_image.Set(bmp) )
			{
				m_DeleteRgn();
				m_rgn = m_image.CreateRgn(color);
				m_hotspot = hotspot;
				if ( ::IsWindow(m_win) )
				{
					const SIZE& sz = m_image.GetSize();
					::SetWindowPos(m_win, NULL, 0, 0, sz.cx, sz.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
					HRGN rgn = ::CreateRectRgn(0, 0, 0, 0);
					::CombineRgn(rgn, m_rgn, NULL, RGN_COPY);
					::SetWindowRgn(m_win, rgn, TRUE);
					::InvalidateRgn(m_win, m_rgn, TRUE);
				}
				return true;
			}
			return false;
		}

		void Show(bool isShow)
		{
			::ShowWindow(m_win, isShow ? SW_SHOW : SW_HIDE);
		}

	protected:
		virtual bool OnWindowMessage(LRESULT& _result, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			if ( message == WM_SETFOCUS )
			{
				::SetFocus(reinterpret_cast<HWND>(wParam));
				return true;
			}
			else if ( message == WM_PAINT )
			{
				PAINTSTRUCT ps;
				HDC dc = ::BeginPaint(hWnd, &ps);
				::SelectClipRgn(dc, m_rgn);
				m_image.Draw(dc, 0, 0);
				::EndPaint(hWnd, &ps);
				return true;
			}
			else if ( message == WM_DESTROY )
			{
				m_DeleteRgn();
			}
			return false;
		}
	private:
		void m_DeleteRgn(void)
		{
			if ( m_rgn != NULL )
			{
				_DeleteObject(m_rgn);
				m_rgn = NULL;
			}
		}
		CBitmapImage	m_image;	///< �C���[�W
		HRGN			m_rgn;		///< ���[�W����
		POINT			m_hotspot;	///< �z�b�g�X�|�b�g
		CDummyWindow	m_win;		///< �E�B���h�E�Ǘ�
		bool			m_isChild;	///< true �Ȃ�q�E�B���h�E
	};

	/**
	 * �J�[�\���Ǘ�.
	 *		��̃J�[�\�����Ǘ����܂��B
	 */
	class CCursorManager
	{
	public:
		/// �R���X�g���N�^
		CCursorManager(void) : m_button(0), m_hRgn(NULL)
		{
			Zero(m_pos);
		}
		/// �쐬
		bool Create(HWND hParent, CBitmapHandle bmp, COLORREF color, const POINT& hotspot)
		{
			m_button = 0;
			Zero(m_pos);
			if ( m_win.SetImage(bmp, color, hotspot) )
			{
				RECT rc = { 0 };
				if ( hParent != NULL )
				{
					// �e�E�B���h�E�w��L��
					::GetClientRect(hParent, &rc);
					Clip(rc);
				}
				else
				{
					// �f�X�N�g�b�v
					Clip(rc);
					::EnumDisplayMonitors(NULL, NULL, ms_MonitorEnumProc, reinterpret_cast<LPARAM>(this));
				}
				return m_win.Create(hParent);
			}
			return false;
		}
		/// �쐬
		bool SetImage(CBitmapHandle bmp, COLORREF color, const POINT& hotspot)
		{
			return m_win.SetImage(bmp, color, hotspot);
		}
		/// �\��
		void Show(bool isShow)
		{
			m_win.Show(isShow);
		}
		/// �͈�
		void Clip(const RECT& rect)
		{
			if ( m_hRgn != NULL )
			{
				_DeleteObject(m_hRgn);
			}
			m_hRgn = ::CreateRectRgnIndirect(&rect);
		}
		/// �|�W�V�����ݒ�
		bool SetPos(int x, int y)
		{
			if ( m_hRgn != NULL && ! ::PtInRegion(m_hRgn, x, y) )
			{
				// �����Ă��Ȃ�
				bool r = false;
				if ( ::PtInRegion(m_hRgn, m_pos.x, y) )
				{
					// X�ړ����Ȃ���Γ���
					int f = (m_pos.x > x) ? 1 : -1;
					loop ( i, abs(m_pos.x - x) )
					{
						x += f;
						if ( ::PtInRegion(m_hRgn, x, y) )
						{
							r = true;
							break;
						}
					}
				}
				else if ( ::PtInRegion(m_hRgn, x, m_pos.y) )
				{
					// Y�ړ����Ȃ���Γ���
					int f = (m_pos.y > y) ? 1 : -1;
					loop ( i, abs(m_pos.y - y) )
					{
						y += f;
						if ( ::PtInRegion(m_hRgn, x, y) )
						{
							r = true;
							break;
						}
					}
				}
				else
				{
					// Y�ړ��Ɉړ�
					int f = (m_pos.y > y) ? 1 : -1;
					loop ( i, abs(m_pos.y - y) )
					{
						y += f;
						if ( ::PtInRegion(m_hRgn, m_pos.x, y) )
						{
							break;
						}
					}
					// X�ړ��Ɉړ�
					f = (m_pos.x > x) ? 1 : -1;
					loop ( i, abs(m_pos.x - x) )
					{
						x += f;
						if ( ::PtInRegion(m_hRgn, x, y) )
						{
							r = true;
							break;
						}
					}
				}
				if ( ! r )
				{
					return false;
				}
			}
			if ( m_pos.x != x || m_pos.y != y )
			{
				m_pos.x = x;
				m_pos.y = y;
				m_win.Move(m_pos.x, m_pos.y);
				return true;
			}
			return false;
		}
		/// �|�W�V�����ړ�
		bool MovePos(int dx, int dy)
		{
			return SetPos(m_pos.x + dx, m_pos.y + dy);
		}
		/// �|�W�V�����擾
		const POINT& GetPos(void) const
		{
			return m_pos;
		}
		/// �{�^����Ԏ擾
		BYTE GetButtonState(void) const
		{
			return m_button;
		}
		/// �{�^����Ԑݒ�
		void SetButtonFlags(WORD buttonFlags)
		{
			loop ( i, 3/*�{�^����*/ * 2 )
			{
				if ( (buttonFlags & _BIT(i)) != 0 )
				{
					if ( (i & 1) == 0 )
					{
						m_button |= _BIT(i / 2);
					}
					else
					{
						m_button &= ~_BIT(i / 2);
					}
				}
			}
		}

	private:
		/// EnumDisplayMonitors() �p�R�[���o�b�N
		static BOOL CALLBACK ms_MonitorEnumProc(
			HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
		{
			CCursorManager* P = reinterpret_cast<CCursorManager*>(dwData);
			HRGN& r1 = P->m_hRgn;
			HRGN r2 = ::CreateRectRgnIndirect(lprcMonitor);
			VERIFY( ::CombineRgn(r1, r1, r2, RGN_OR) != ERROR );
			_DeleteObject(r2);
			return TRUE;
		}
		CCursorWindow	m_win;			///< �J�[�\��
		POINT			m_pos;			///< ���W
		HRGN			m_hRgn;			///< ����͈�
		BYTE			m_button;		///< _BIT(IListener::EKind) �B
	};

	/// �����p�N���X
	class CInner : public IMultiMouseWatcher::IListener
	{
		DEFPARENTLISTENER(CMultiMouseManager, IParentListener);
	public:
		/// �R���X�g���N�^
		CInner(void) : m_pListener(NULL)
		{
			CBitmapImage bi;
			bi.Set(1, 1);
			m_cursorImage = bi;
		}
		/// �`�F�b�N
		bool CheckIndex(INDEX index)
		{
			if ( ! m_cursores.IsInRange(index) )
			{
				m_cursores.SetSize(index + 1);
			}
			if ( m_cursores[index] == NULL )
			{
				CCursorManager* P = new CCursorManager;
				if ( ! P->Create(m_hParent, m_cursorImage, m_cursorMaskColor, m_cursorHotspot) )
				{
					_GetLastError("CreateWindow");
					delete P;
					return false;
				}
				if ( ! ::IsRectEmpty(&m_corsorRect) )
				{
					P->Clip(m_corsorRect);
				}
				m_cursores.Set(index, P);
			}
			return true;
		}
		/// [�ʒm] ��M���b�Z�[�W�ʒm.
		virtual void OnMultiMouseEvent(INDEX index, const RAWMOUSE& mouse)
		{
			EXCLUSIVE( &m_sync );
			if ( m_pListener != NULL )
			{
				m_pListener->OnMultiMouseEvent(index, mouse);
			}
			CCursorManager* P = m_cursores[index];
			if ( P == NULL )
			{
				ASSERT( P != NULL );
				return;
			}
			const POINT& po = P->GetPos();
			P->SetButtonFlags(mouse.usButtonFlags);
			if ( P->MovePos(mouse.lLastX, mouse.lLastY) )
			{
				//�ω��L��
				if ( m_pListener != NULL )
				{
					m_pListener->OnMultiMouseMove(index, P->GetButtonState(), po.x, po.y);
				}
			}
			if ( m_pListener != NULL )
			{
				if ( (mouse.usButtonFlags & RI_MOUSE_WHEEL) != 0 )
				{
					short d = mouse.usButtonData;
					m_pListener->OnMultiMouseWheel(index, d);
				}
				loop ( i, 3/*�{�^����*/ * 2 )
				{
					if ( (mouse.usButtonFlags & _BIT(i)) != 0 )
					{
						CMultiMouseManager::IListener::EEvent e = CMultiMouseManager::IListener::Event_Down;
						if ( (i & 1) != 0 )
						{
							e = CMultiMouseManager::IListener::Event_Up;
						}
						m_pListener->OnMultiMouseButton(index, CMultiMouseManager::IListener::EButton(i / 2), e, po.x, po.y);
					}
				}
			}
		}
		/// [�ʒm]
		virtual void OnMultiMouseDevice(INDEX index, EDevice device)
		{
			EXCLUSIVE( &m_sync );
			if ( device == Device_Remove )
			{
				if ( m_pListener != NULL )
				{
					m_pListener->OnMultiMouseDevice(index, device);
				}
				return;
			}
			if ( ! CheckIndex(index) )
			{
				return;
			}
			if ( m_pListener != NULL )
			{
				m_pListener->OnMultiMouseDevice(index, device);
			}
		}
		IParentListener*				m_pListener;		///< ���X�i�[
		CPointerVectorT<CCursorManager>	m_cursores;			///< �}�E�X�J�[�\���Ǘ�
		HWND							m_hParent;			///< �e�E�B���h�E�n���h��
		CBitmapHandle					m_cursorImage;		///< �J�[�\���C���[�W
		COLORREF						m_cursorMaskColor;	///< �J�[�\���C���[�W�}�X�N�F
		POINT							m_cursorHotspot;	///< �J�[�\���z�b�g�X�|�b�g
		RECT							m_corsorRect;		///< �J�[�\���ړ��͈�
		CSyncSection					m_sync;
	};	

	/// HCURSOR �� BMP �ɕϊ�
	bool m_IconToBitmap(CBitmapHandle& _bmp, POINT& _hotspot, HCURSOR hCursor, COLORREF color)
	{
		ICONINFO ii;
		if ( ::GetIconInfo(hCursor, &ii) )
		{
			HBITMAP h = ii.hbmColor;
			if ( h == NULL )
			{
				h = ii.hbmMask;
			}
			BITMAP bm;
			if ( ::GetObject(h, sizeof(BITMAP), &bm) > 0 )
			{
				CBitmapImage bi;
				bi.Set(bm.bmWidth, bm.bmHeight, color);
				HDC dc = bi.GetDC();
				::DrawIconEx(dc, 0, 0, hCursor, 0, 0, 0, NULL, DI_NORMAL);
				bi.ReleaseDC();
				_bmp = bi;
				_hotspot.x = ii.xHotspot;
				_hotspot.y = ii.yHotspot;
				return true;
			}
		}
		return false;
	}

	/// �w��̃}�E�X�C���f�b�N�X�̃}�l�[�W���擾
	const CCursorManager* m_RefCursorManager(INDEX index) const
	{
		if ( m_in.m_cursores.IsInRange(index) )
		{
			return m_in.m_cursores[index];
		}
		return NULL;
	}

	/// �w��̃}�E�X�C���f�b�N�X�̃}�l�[�W���擾
	CCursorManager* m_GetCursorManager(INDEX index)
	{
		m_in.CheckIndex(index);
		return m_in.m_cursores[index];
	}

	CInner				m_in;		///< �����Ǘ��N���X
	IMultiMouseWatcher*	m_pWatcher;	///< �}�E�X�Ď�
};



}; //TNB



