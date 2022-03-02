#pragma once
/**
 *@file
 * ���}�E�X�Ď��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMultiMouseManager.h"
#include "TnbDummyWindow.h"
#include "TnbSimpleVector.h"
#include "TnbStr.h"
#include "TnbVector.h"
#include "TnbWindowsVersion.h"
#include <dbt.h>



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
_SELECTANY HHOOK s_hTnbRmwLowMouse = NULL;
#endif



/**@ingroup WINAPI
 * ���}�E�X�Ď��N���X
 *
 *		���̃}�E�X�̃f�[�^���擾���邱�Ƃ��o���܂��B�����̃}�E�X��ڑ����Ă���ꍇ�A���ꂼ��̃f�[�^���擾�ł��܂��B
 *
 *	@par�g�p��
 *		\code
 *		// �w�b�_
 *		class CFooDlg : public CDialog, CRawMouseWatcher::IListener
 *		{
 *					;
 *			virtual void OnMultiMouseEvent(INDEX index, const RAWMOUSE& mouse);
 *			virtual void OnMultiMouseDevice(INDEX index, EDevice device);
 *					;
 *			CRawMouseWatcher	m_mouseWatcher;
 *					;
 *
 *		// �\�[�X
 *		void CFooDlg::XX(void)
 *		{
 *			m_mouseWatcher.SetListener(this);
 *			m_mouseWatcher.Start();
 *		}
 *		void CFooDlg::OnMultiMouseEvent(INDEX index, const RAWMOUSE& mouse)
 *		{
 *			int x = mouse.lLastX; // X �ړ���
 *			int y = mouse.lLastY; // Y �ړ���
 *			if ( (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) != 0 )
 *			{
 *				//���{�^������
 *			}
 *			if ( (mouse.usButtonFlags & RI_MOUSE_WHEEL) != 0 )
 *			{
 *				//�z�C�[��
 *				short d = mouse.usButtonData;
 *			}
 *		}
 *		void CFooDlg::OnMultiMouseDevice(INDEX index, EDevice device)
 *		{
 *		}
 *		\endcode
 *
 *	@attention XP �ȍ~�̂ݗL���� API ���g�p���Ă��邽�߁A�ŏ��� TnbHotWinver.h ���C���N���[�h���Ă��������B
 *
 *	@par�K�v�t�@�C��
 *			TnbRawMouseWatcher.h
 *
 *	@date 10/04/20	�V�K�쐬
 *	@date 10/06/17	IMultiMouseWatcher �̃T�u�N���X��
 *	@date 10/07/14	EnableMouseCursor() ��ǉ��B
 *	@date 10/07/23	HorizontalWheelEvent() ��ǉ��B
 */
class CRawMouseWatcher : public IMultiMouseWatcher
{
	DEFSUPER(IMultiMouseWatcher);
public:

	/// �R���X�g���N�^
	CRawMouseWatcher(void)
	{
		Refresh();
	}

	/**
	 * [�o�^] ���X�i�[�o�^.
	 *	@param P ���X�i�[
	 */
	virtual void SetListener(IMultiMouseWatcher::IListener* P)
	{
		m_in.m_pListener = P;
	}

	/**
	 * [�擾] �}�E�X���擾.
	 *	@return �}�E�X��.
	 */
	virtual size_t GetMouseCount(void) const
	{
		return m_in.m_devsList.GetSize();
	}

	/**
	 * [�ݒ�] �Ď��J�n.
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool Start(void)
	{
		Stop();
		if ( m_win.Create(&m_in) )
		{
			RAWINPUTDEVICE rid;
			rid.usUsagePage = 1/*HID_USAGE_PAGE_GENERIC*/; 
			rid.usUsage = 2/*HID_USAGE_GENERIC_MOUSE*/; 
			rid.dwFlags = RIDEV_INPUTSINK;
			rid.hwndTarget = m_win;
			if ( ::RegisterRawInputDevices(&rid, 1, sizeof(rid)) )
			{
				return true;
			}
		}
		Stop();
		return false;
	}

	/**
	 * [�ݒ�] �Ď���~.
	 */
	virtual void Stop(void)
	{
		m_win.Destroy();
	}

	/**
	 * [�擾] �}�E�X�X�y�b�N�擾.
	 *	@param[out] _spec �X�y�b�N���������܂��B
	 *	@param[in] index �}�E�X�C���f�b�N�X�B �O�ȏ�A GetMouseCount() �����̒l�ł��B
	 *	@retval true �擾����
	 *	@retval false ���s�iindex �̒l�����������j
	 */
	bool GetMouseSpecified(RID_DEVICE_INFO_MOUSE& _spec, INDEX index) const
	{
		if ( GetMouseCount() > index )
		{
			_spec = m_in.m_devsList[index].specified;
			return true;
		}
		return false;
	}

	/**
	 * [�擾] �}�E�X�f�o�C�X���擾.
	 *	@param[in] index �}�E�X�C���f�b�N�X�B �O�ȏ�A GetMouseCount() �����̒l�ł��B
	 *	@return �}�E�X�f�o�C�X��
	 */
	CStr GetMouseName(INDEX index) const
	{
		if ( GetMouseCount() > index )
		{
			return m_in.m_devsList[index].name;
		}
		return _T("");
	}

	/**
	 * [�ݒ�] �Č���.
	 *	@note �f�o�C�X�̏�Ԃ��ς�����Ƃ��A�{�C���X�^���X�͎����I�ɒǏ]���܂��B
	 *		�{���\�b�h���R�[������ƁA�}�E�X�C���f�b�N�X������������܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Refresh(void)
	{
		return m_in.Refresh();
	}

	/**
	 * [�ݒ�] �V�X�e���}�E�X�L�������ݒ�.
	 *		�}�E�X�𓮂����Ă��A�V�X�e���}�E�X�𓮂��Ȃ����邱�Ƃ��o���܂��B
	 *		���̏�Ԃ� MouseEvent() �ɂ�鑀��w���̂ݗL���ɂȂ�܂��B
	 *	@note �{���\�b�h�̐ݒ�́A�v���Z�X���I�������猳�ɖ߂�܂��B
	 *	@param isEnable false �Ȃ�}�E�X�f�o�C�X�Ń}�E�X�J�[�\���͓����Ȃ��悤���܂��B MouseEvent() �ł͓��������Ƃ��o���܂��B\n
	 *				true �Ȃ�ʏ�ʂ�ł��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	static bool EnableMouseCursor(bool isEnable)
	{
		if ( ! isEnable )
		{
			//������
			if ( s_hTnbRmwLowMouse == NULL )
			{
				s_hTnbRmwLowMouse = ::SetWindowsHookEx(WH_MOUSE_LL, ms_OnLowLevelMouseProc, GetInstanceHandleByTnb(), 0);
			}
			return s_hTnbRmwLowMouse != NULL;
		}
		// �L����
		if ( s_hTnbRmwLowMouse != NULL ) 
		{
			::UnhookWindowsHookEx(s_hTnbRmwLowMouse);
			s_hTnbRmwLowMouse = NULL;
		}
		return s_hTnbRmwLowMouse == NULL;
	}

	/**
	 * [�ݒ�] �V�X�e���}�E�X�C�x���g.
	 *		�V�X�e���}�E�X�C�x���g�𔭍s���܂��B 
	 *	@note EnableMouseCorsor �� false ���w�肵�Ă��Ă��A�@�\���܂��B
	 *	@note ������ ::mouse_event() �Ɠ����ł��B
	 *	@note �����z�C�[���́A HorizontalWheelEvent() ���g���܂��B
	 *	@param flags �t���O
	 *	@param dx X
	 *	@param dy Y
	 *	@param data �f�[�^
	 */
	static void MouseEvent(DWORD flags, DWORD dx = 0,DWORD dy = 0, DWORD data = 0)
	{
		::mouse_event(flags, dx, dy, data, COOKIE);
	}

	/**
	 * [�ݒ�] �V�X�e�������z�C�[���C�x���g.
	 *		�����z�C�[�����������܂��B 
	 *	@note EnableMouseCorsor �� false ���w�肵�Ă��Ă��A�@�\���܂��B
	 *	@note VISTA�ȍ~�ł̓}�E�X�C�x���g�A�ȑO�ł̓X�N���[�����b�Z�[�W�őΉ����܂��B
	 *	@param delta �ړ��ʁB WHEEL_DELTA �̔{���Ŏw�肵�܂��B
	 *	@todo ���� Opera �œ��삵�Ȃ����Ƃ��m�F�B
	 *	@todo ���� Excel �̕�����Ԃł̓t�H�[�J�X�ʂ蓮�삵�Ȃ����Ƃ��m�F�B
	 */
	static void HorizontalWheelEvent(int delta)
	{
		//CWindowsVersion wv;	@todo VISTA �ł� HWHEEL ���b�Z�[�W�ɑΉ����Ă��Ȃ��E�B���h�E�������E�E�E�B
		//if ( wv.GetMajorVersion() >= 6 )
		//{
		//	MouseEvent(0x1000/*MOUSEEVENTF_HWHEEL*/, 0, 0, delta);
		//	return;
		//}
		// ���݂̃t�H�A�O���E���h�̃E�B���h�E�擾
		HWND hWnd = ::GetForegroundWindow();
		TTRACE1("GetForegroundWindow = %08X\n", hWnd);
		// �A�N�e�B�u�ȃE�C���h�E�̃X���b�hID���擾����
		DWORD dwCurrentThreadId = ::GetCurrentThreadId();
		DWORD dwActiveProcessId;
		DWORD dwActiveThreadId = ::GetWindowThreadProcessId(hWnd, &dwActiveProcessId);
		if ( ::AttachThreadInput(dwCurrentThreadId, dwActiveThreadId, TRUE) )
		{
			hWnd = ::GetFocus();
			if ( hWnd != NULL )
			{	
				LONG style = ::GetWindowLong(hWnd, GWL_STYLE);
				bool hasScrollBar = (style & WS_HSCROLL) != 0;
				#ifdef _DEBUG
					CStr s;
					::GetClassName(hWnd, s.GetBuffer(100), 100);
					s.ReleaseBuffer();
					TTRACE3(" GetFocus = %08X %d, [%s]\n", hWnd, hasScrollBar, s);
				#endif
				bool isDisp = false;
				if ( ! hasScrollBar )
				{
					// memo; WORD �͌Z��� ScrollBar ������B
					isDisp = ms_SubWheel(hWnd, delta);
					if ( ! isDisp )
					{
						// memo; EXCEL �͈���� ScrollBar ������B
						HWND h = ::GetWindow(hWnd, GW_CHILD);
						isDisp = ms_SubWheel(h, delta);
					}
				}
				if ( ! isDisp )
				{
					ms_PostWheel(hWnd, NULL, delta);
				}
			}
			// �J������
			::AttachThreadInput(dwCurrentThreadId, dwActiveThreadId, FALSE);
		}
	}

private:

	enum
	{
		COOKIE = 'TbMm'	///< ��p�N�b�L�[
	};

	/// ���
	struct TInfo : TNB::IComparableT<TInfo>
	{
		HANDLE					handle;		///< �n���h���B INVALID_HANDLE_VALUE �Ȃ獡�͖��ڑ����
		CStr					name;		///< ���O�B
		RID_DEVICE_INFO_MOUSE	specified;	///< �X�y�b�N�B
		TInfo(void) : handle(INVALID_HANDLE_VALUE)
		{
		}
		virtual INT_PTR Compare(const TInfo& t) const
		{
			return name.Compare(t.name);
		}
	};
	/// �����p�N���X
	class CInner : public CDummyWindow::IListener
	{
		DEFPARENTLISTENER(IMultiMouseWatcher, IParentListener);
	public:
		/// �R���X�g���N�^
		CInner(void) : m_pListener(NULL), m_lastHandle(INVALID_HANDLE_VALUE)
		{
		}
		/**
		 * [�ʒm] ��M���b�Z�[�W�ʒm.
		 *	@retval true ������(DefWindowProc()���Ăяo���Ȃ��Ȃ�܂�)
		 *	@retval false ������
		 */
		virtual bool OnWindowMessage(LRESULT& _result, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			if ( m_pListener != NULL )
			{
				if ( message == WM_DEVICECHANGE && wParam == DBT_DEVNODES_CHANGED )
				{
					CVectorT<TInfo> newInf;
					if ( Collect(newInf) )
					{
						loop ( i, m_devsList )
						{
							INDEX idx = newInf.Find(m_devsList[i]);
							if ( idx != INVALID_INDEX )
							{
								// �Ĕ������ꂽ
								if ( m_devsList[i].handle == INVALID_HANDLE_VALUE )
								{
									m_devsList[i] = newInf[idx];
									m_pListener->OnMultiMouseDevice(i, _super::IListener::Device_Reconnect);
								}
							}
							else
							{
								// �����Ȃ���
								if ( m_devsList[i].handle != INVALID_HANDLE_VALUE )
								{
									m_devsList[i].handle = INVALID_HANDLE_VALUE;
									m_pListener->OnMultiMouseDevice(i, _super::IListener::Device_Remove);
								}
							}
						}
						loop ( i, newInf )
						{
							INDEX idx = m_devsList.Find(newInf[i]);
							if ( idx == INVALID_INDEX )
							{
								// �V�K����
								int x = m_devsList.Add(newInf[i]);
								m_pListener->OnMultiMouseDevice(x, _super::IListener::Device_Connect);
							}
						}
					}
				}
				else if ( message == WM_INPUT )
				{
					UINT size = 40;
					CWorkMem w(size);
					::GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, w, &size, sizeof(RAWINPUTHEADER));
					RAWINPUT* P = reinterpret_cast<RAWINPUT*>(w.Ref());
					if ( P->header.dwType == RIM_TYPEMOUSE )
					{
						INDEX idx = INVALID_INDEX;
						if ( m_lastHandle == P->header.hDevice )
						{
							idx = m_lastIndex;
						}
						else
						{
							loop ( i, m_devsList.GetSize() )
							{
								if ( m_devsList[i].handle == P->header.hDevice )
								{
									idx = i;
									m_lastHandle = P->header.hDevice;
									m_lastIndex = idx;
									break;
								}
							}
						}
						if ( idx != INVALID_INDEX )
						{
							m_pListener->OnMultiMouseEvent(idx, P->data.mouse);
						}
					} 
				}
			}
			return false;
		}
		/// �Č���
		bool Refresh(void)
		{
			m_lastHandle = INVALID_HANDLE_VALUE;
			return Collect(m_devsList);
		}
		/// �����W
		bool Collect(CVectorT<TInfo>& _info)
		{
			_info.RemoveAll();
			UINT count = 0;
			UINT rr = ::GetRawInputDeviceList(NULL, &count, sizeof(RAWINPUTDEVICELIST));
			if ( rr != 0 )
			{
				return false;
			}
			UINT d1 = count;
			CWorkMemT<RAWINPUTDEVICELIST> devs(count);
			UINT d2 = ::GetRawInputDeviceList(devs, &count, sizeof(RAWINPUTDEVICELIST));
			if ( d1 != d2 )
			{
				return false;
			}
			loop ( i, devs.GetSize() )
			{
				if ( devs[i].dwType != RIM_TYPEMOUSE )
				{
					continue;
				}
				TInfo info;
				info.handle = devs[i].hDevice;
				// �X�y�b�N�擾
				RID_DEVICE_INFO dev;
				UINT size = sizeof(RID_DEVICE_INFO);
				if ( ::GetRawInputDeviceInfo(info.handle, RIDI_DEVICEINFO, &dev, &size) > 0 )
				{
					info.specified = dev.mouse;
					// �f�o�C�X���̒����擾
					if ( ::GetRawInputDeviceInfo(info.handle, RIDI_DEVICENAME, NULL, &size) == 0 )
					{
						// �f�o�C�X���擾
						CWorkMemT<TCHAR> name(size + 1);
						int r = ::GetRawInputDeviceInfo(info.handle, RIDI_DEVICENAME, name, &size);
						if ( r > 0 )
						{
							info.name = name.Ref();
						}
					}
					_info.Add(info);
				}
			}
			return true;
		}
		IParentListener*		m_pListener;	///< ���X�i�[
		CVectorT<TInfo>			m_devsList;		///< �f�o�C�X���X�g
		mutable HANDLE			m_lastHandle;	///< ���X�g�n���h��(cache)
		mutable INDEX			m_lastIndex;	///< ���X�g�C���f�b�N�X(cache)
	};
	/**
	 * [�ʒm] ���[���x���}�E�X PROC.
	 *		�}�E�X������t�b�N.
	 */
	static LRESULT CALLBACK ms_OnLowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if ( nCode == HC_ACTION )
		{
			MSLLHOOKSTRUCT* P = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
			TTRACE3("OnLowLevelMouseProc msg=0x%X, x=%d, y=%d\n", wParam, P->pt.x, P->pt.y);
			TTRACE3("         data=%d, flags=0x%X, ex-info=%d\n", P->mouseData, P->flags, P->dwExtraInfo);
			if ( P->dwExtraInfo != COOKIE )
			{
				return 1;
			}
		} 
		return ::CallNextHookEx(s_hTnbRmwLowMouse, nCode, wParam, lParam);
	}
	// CLASS ���`�F�b�N
	static bool ms_IsScrollBar(LPCTSTR P)
	{
		const TCHAR src[] = _T("ScrollBar");
		loop ( i, sizeof(src) )
		{
			if ( src[i] != P[i] )
			{
				return false;
			}
		}
		return true;
	}
	// ���M
	static void ms_PostWheel(HWND hTargetWnd, HWND hCtrlWnd, int delta)
	{
		UINT msg = WM_HSCROLL;
		WPARAM wp = (delta < 0) ? SB_LINELEFT : SB_LINERIGHT;
		LPARAM lp = reinterpret_cast<LPARAM>(hCtrlWnd);
		int l = ((delta < 0) ? -delta : delta) / WHEEL_DELTA;
		loop ( i, l )
		{
			::PostMessage(hTargetWnd, msg, wp, lp);
		}
		::PostMessage(hTargetWnd, msg, SB_ENDSCROLL, lp);
	}
	// �T�u
	static bool ms_SubWheel(HWND hWnd, int delta)
	{
		CWorkMemT<TCHAR> buf(100);
		HWND h = ::GetWindow(hWnd, GW_HWNDNEXT);
		while ( h != NULL )
		{
			::GetClassName(h, buf, 100);
			LONG style = ::GetWindowLong(h, GWL_STYLE);
			bool isVisible = (style & WS_VISIBLE) != 0;
			bool isHorz = (style & (SBS_VERT | SBS_SIZEBOX)) == 0;
			if ( ms_IsScrollBar(buf) && isVisible && isHorz )
			{
				TTRACE2(" Found ScrollBar = %08X, [%s]\n", h, buf.Ref());
				HWND h2 = ::GetParent(h);
				if ( h2 != NULL )
				{
					// �e�ɒʒm
					ms_PostWheel(h2, h, delta);
					return true;
				}
			}
			h = ::GetWindow(h, GW_HWNDNEXT);
		}
		return false;
	}
	CInner			m_in;						///< �����N���X
	CDummyWindow	m_win;						///< �C�x���g��M�p�E�B���h�E
};



}; //TNB



