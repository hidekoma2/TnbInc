#pragma once
/**
 *@file
 * MCI �֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include <mmsystem.h>
#include <digitalv.h>
#pragma comment(lib, "winmm.lib")



//TNB Library
namespace TNB
{



/**@ingroup AUDIO ANIMATE
 * MCI �v���C���[�N���X.
 *
 *		MCI ���g�p���āA MP3 �t�@�C���� MIDI �t�@�C�����Đ����܂��B
 *
 *	@todo ����� Windows7 64bit�̊��ɂ����āACLOSE ���� imjptip.dll �� Access Violation ����������B�����s���B
 *
 *	@par�K�v�t�@�C��
 *			TnbMciPlayer.h
 *
 *	@date 10/05/25	�V�K�쐬
 *	@date 10/06/25	MIDI �����łȂ��A MCI �֌W�Ȃ�Đ��ł���悤�ɕύX�B
 *	@date 13/01/16	�w�b�_�Ɨ��B����t�@�C���ɂ��b��Ή��B
 */
class CMciPlayer : public CCopyImpossible
{
public:

	/// �R���X�g���N�^
	CMciPlayer(void) : m_isPlayed(false), m_lastError(0), m_hParent(NULL), m_wDeviceId(0), m_hViewWnd(NULL), m_isPaused(false)
	{
	}

	/// �f�X�g���N�^
	~CMciPlayer(void)
	{
		Close();
	}

	/**
	 * [�ݒ�] �e�E�B���h�E�ݒ�.
	 *		�e�E�B���h�E���w�肷��ƁA���̃E�B���h�E�� MM_MCINOTIFY ���ʒm����܂��B
	 *	@param hWnd �e�E�B���h�E�B NULL ���w�肷��Ɖ����B
	 */
	void SetParentWnd(HWND hWnd)
	{
		m_hParent = hWnd;
	}

	/**
	 * [�擾] �f�o�C�X ID �擾.
	 *	@retval 0 �擾���s�B���I�[�v���B
	 *	@retval 0�ȊO �擾�����B�l�̓f�o�C�X ID�B
	 */
	MCIDEVICEID GetDeviceId() const
	{
		return m_wDeviceId;
	}

	/**
	 * [�ݒ�] MIDI �t�@�C���I�[�v��.
	 *	@param lpszFileName �t�@�C����.
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool OpenMidi(LPCTSTR lpszFileName)
	{
		LPCTSTR lpszDeviceType = reinterpret_cast<LPCTSTR>(MCI_DEVTYPE_SEQUENCER);
		return Open(lpszFileName, lpszDeviceType, true);
	}

	/**
	 * [�ݒ�] MP3 �t�@�C���I�[�v��.
	 *	@param lpszFileName �t�@�C����.
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool OpenMp3(LPCTSTR lpszFileName)
	{
		LPCTSTR lpszDeviceType = _T("MPEGVideo");
		return Open(lpszFileName, lpszDeviceType, false);
	}

	/**
	 * [�ݒ�] WAVE �t�@�C���I�[�v��.
	 *	@param lpszFileName �t�@�C����.
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool OpenWave(LPCTSTR lpszFileName)
	{
		LPCTSTR lpszDeviceType = reinterpret_cast<LPCTSTR>(MCI_DEVTYPE_WAVEFORM_AUDIO);
		return Open(lpszFileName, lpszDeviceType, true);
	}

	/**
	 * [�ݒ�] AVI �t�@�C���I�[�v��.
	 *	@note mpeg �t�@�C�����Ή��B
	 *	@param lpszFileName �t�@�C����.
	 *	@param hViewWnd �\���E�B���h�E�n���h���B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool OpenAvi(LPCTSTR lpszFileName, HWND hViewWnd)
	{
		LPCTSTR lpszDeviceType = _T("MPEGVideo");	// _T("avivideo");
		return Open(lpszFileName, lpszDeviceType, false, hViewWnd);
	}

	/**
	 * [�ݒ�] �N���[�Y.
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool Close(void)
	{
		bool r = true;
		if ( m_wDeviceId != 0 )
		{
			r = m_Cmd(MCI_CLOSE);
			if ( r )
			{
				m_isPaused = false;
				m_isPlayed = false;
				m_wDeviceId = 0;
				m_hViewWnd = NULL;
			}
		}
		return r;
	}

	/**
	 * [�擾] �I�[�v���m�F.
	 *	@retval true �I�[�v���ς�.
	 *	@retval false ���I�[�v��.
	 */
	bool IsOpened(void) const
	{
		return GetDeviceId() != 0;
	}

	/**
	 * [�擾] �S���t����.
	 *		�I�[�v�������Ȃ̑S���t���Ԃ�Ԃ��܂��B
	 *	@param _ms �擾��������(ms)���i�[����܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool GetLength(DWORD& _ms) const
	{
		return m_GetStatus(_ms, MCI_STATUS_LENGTH);
	}

	/**
	 * [�擾] ���[�h�擾.
	 *	@retval MCI_MODE_NOT_READY	�G���[�B
	 *	@retval MCI_MODE_STOP		��~���B
	 *	@retval MCI_MODE_PLAY		���t���B
	 *	@retval MCI_MODE_PAUSE		�ꎞ��~���B
	 *	@retval MCI_MODE_SEEK		�V�[�N���B
	 */
	DWORD GetMode(void) const
	{
		DWORD mode;
		if ( m_GetStatus(mode, MCI_STATUS_MODE) )
		{
			return mode;
		}
		return MCI_MODE_NOT_READY;
	}

	/**
	 * [�ݒ�] ���t�J�n.
	 *	@note ���t�J�n�ʒu��ς������ꍇ�A Seek() ���g���܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool Play(void)
	{
		MCI_PLAY_PARMS mciPlay = { 0 };
		bool r = m_Cmd(MCI_PLAY, 0, mciPlay);
		if ( r )
		{
			m_isPlayed = true;
			m_isPaused = false;
		}
		return r;
	}

	/**
	 * [�ݒ�] ���t��~.
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool Stop(void)
	{
		bool r = m_Cmd(MCI_STOP);
		if ( r )
		{
			m_isPlayed = false;
		}
		return r;
	}
	
	/**
	 * [�ݒ�] ���t���f�E�ĊJ.
	 *	@note ���t���Ă��Ȃ����́A�������܂��񂪁A�����ŕԂ�܂��B
	 *	@param isPause true �Ȃ璆�f false �Ȃ�ĊJ���܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool Pause(bool isPause = true)
	{
		bool r = true;
		if ( m_isPlayed )
		{
			r = m_Cmd(isPause ? MCI_PAUSE : MCI_PLAY);
			if ( r )
			{
				m_isPaused = isPause;
			}
		}
		return r;
	}

	/**
	 * [�擾] �ꎞ��~���H
	 *	@retval true �ꎞ��~���B
	 *	@retval false �ہB
	 */
	bool IsPaused(void) const
	{
		if ( m_isPlayed && GetMode() == MCI_MODE_PAUSE )
		{
			return true;
		}
		return false;
	}

	/**
	 * [�ݒ�] �V�[�N.
	 *	@param pos ������̎���(ms).
	 *	@param isResume true �Ȃ�V�[�N��A�Đ����ĊJ���܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool Seek(DWORD pos = 0, bool isResume = true)
	{
		if ( pos == 0 )
		{
			return m_Cmd(MCI_SEEK, MCI_SEEK_TO_START);
		}
		MCI_SEEK_PARMS mciSeek = { 0 };
		mciSeek.dwTo = pos;
		bool r = m_Cmd(MCI_SEEK, MCI_TO, mciSeek);
		if ( r && isResume )
		{
			r = m_Cmd(MCI_PLAY);
		}
		return r;
	}

	/**
	 * [�ݒ�] ���ʐݒ�.
	 *	@param vol ���� 0�`1000
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool SetVolume(DWORD vol)
	{
		MCI_DGV_SETAUDIO_PARMS parm = { 0 };
		parm.dwItem = MCI_DGV_SETAUDIO_VOLUME;
		parm.dwValue = vol;
		return m_Cmd(MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, parm);
	}

	/**
	 * [�擾] ���t�ʒu�擾.
	 *		���t���̈ʒu��Ԃ��܂��B
	 *	@param _ms �擾��������(ms)���i�[����܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool GetPlayingPosition(DWORD& _ms) const
	{
		return m_GetStatus(_ms, MCI_STATUS_POSITION);
	}

	/**
	 * [�ݒ�] �\��RECT�ݒ�.
	 *	@note ����̏ꍇ�̂ݎg�p�\�ł��B
	 *	@param rect �\���͈�
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool SetViewRect(const RECT& rect)
	{
		MCI_DGV_PUT_PARMS parms = { 0 };
		parms.rc = rect;
		return m_Cmd(MCI_PUT, MCI_DGV_RECT | MCI_DGV_PUT_DESTINATION, parms);
	}

	/**
	 * [�擾] �摜�傫���擾.
	 *	@note ����̏ꍇ�̂ݎg�p�\�ł��B
	 *	@param[out] _size ��������ƁA�傫�����i�[����܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool GetSourceSize(SIZE& _size) const
	{
		MCI_ANIM_RECT_PARMS parms = { 0 };
		if ( m_Cmd(MCI_WHERE, MCI_ANIM_WHERE_SOURCE, parms) )
		{
			_size.cx = parms.rc.right - parms.rc.left;
			_size.cy = parms.rc.bottom - parms.rc.top;
			return true;
		}
		return false;
	}

	/**
	 * [�擾] ���X�g�G���[�R�[�h�擾.
	 *	@return �G���[�R�[�h. GetErrorString() �ŕ�����ɕϊ����邱�Ƃ��\�ł��B
	 */
	MCIERROR GetLastError(void) const
	{
		return m_lastError;
	}

	/**
	 * [�擾] �G���[������擾.
	 *	@param e �G���[�R�[�h. 
	 *	@return ������B
	 */
	static CStr GetErrorString(MCIERROR e)
	{
		CStr s;
		if ( e != MMSYSERR_NOERROR )
		{
			::mciGetErrorString(e, s.GetBuffer(1000), 1000);
			s.ReleaseBuffer();
		}
		return s;
	}

protected:

	/**
	 * [�ݒ�] �I�[�v��.
	 *	@param lpszFileName �t�@�C����.
	 *	@param lpszDeviceType �f�o�C�X�^�C�v��.
	 *	@param isDeviceTypeId �f�o�C�X�^�C�v��ID�̏ꍇ true ���w�肵�܂��B
	 *	@param hViewWnd �\���E�B���h�E�n���h���B
	 *	@retval true �����B
	 *	@retval false ���s�B�ڍׂ� GetLastError() �œ����܂��B
	 */
	bool Open(LPCTSTR lpszFileName, LPCTSTR lpszDeviceType, bool isDeviceTypeId, HWND hViewWnd = NULL)
	{
		Stop();
		if ( IsOpened() )
		{
			Close();
		}
		DWORD p1 = MCI_OPEN_TYPE | MCI_OPEN_ELEMENT | (isDeviceTypeId ? MCI_OPEN_TYPE_ID : 0);
		DWORD p2 = 0;
		if ( hViewWnd == NULL )
		{
			Zero(m_openParms);
			m_openParms.lpstrDeviceType = lpszDeviceType;
			m_openParms.lpstrElementName = lpszFileName;
			p2 = reinterpret_cast<DWORD>(&m_openParms);
		}
		else
		{
			Zero(m_animOpenParms);
			m_animOpenParms.hWndParent = hViewWnd;
			m_animOpenParms.dwStyle = WS_CHILD;
			m_animOpenParms.lpstrDeviceType = lpszDeviceType;
			m_animOpenParms.lpstrElementName = lpszFileName;
			p1 |= (MCI_ANIM_OPEN_PARENT | MCI_ANIM_OPEN_WS);
			p2 = reinterpret_cast<DWORD>(&m_animOpenParms);
		}
		bool r = m_Rst(::mciSendCommand(NULL, MCI_OPEN, p1, p2));
		if ( r )
		{
			m_hViewWnd = hViewWnd;
			if ( hViewWnd == NULL )
			{
				m_wDeviceId = m_openParms.wDeviceID;
			}
			else
			{
				m_wDeviceId = m_animOpenParms.wDeviceID;
			}
		}
		if ( r )
		{
			MCI_SET_PARMS mciSet = { 0 };
			mciSet.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
			r = m_Cmd(MCI_SET, MCI_SET_TIME_FORMAT, mciSet);
		}
		if ( r && m_hViewWnd != NULL )
		{
			RECT rc;
			::GetClientRect(m_hViewWnd, &rc);
			MCI_DGV_PUT_PARMS parms = { 0 };
			parms.rc = rc;
			r = m_Cmd(MCI_PUT, MCI_DGV_RECT | MCI_DGV_PUT_DESTINATION, parms);
		}
		return r;
	}

	/**
	 * [�ݒ�] ���X�g�G���[�R�[�h�ݒ�.
	 *	@param e �G���[�R�[�h. 
	 */
	void SetLastError(MCIERROR e)
	{
		m_lastError = e;
	}

private:
	/// �߂�l�`�F�b�N
	bool m_RstFx(MCIERROR r) const
	{
		m_lastError = r;
		return r == 0;
	}
	/// �߂�l�`�F�b�N
	bool m_Rst(MCIERROR r) const
	{
		m_lastError = r;
		if ( r != 0 )
		{
			TRACE2("MCIERROR = %d[%s]\n", r, GetErrorString(r));
		}
		return r == 0;
	}
	/// �R�}���h���s
	template<typename TYP>
	bool m_Cmd(UINT msg, DWORD param, TYP& typ) const
	{
		if ( IsOpened() )
		{
			if ( m_hParent != NULL )
			{
				typ.dwCallback = reinterpret_cast<DWORD>(m_hParent);
				param |= MCI_NOTIFY;
			}
			return m_Rst(::mciSendCommand(GetDeviceId(), msg, param, reinterpret_cast<DWORD>(&typ)));
		}
		return m_RstFx(MCIERR_DEVICE_NOT_READY);
	}
	/// �R�}���h���s
	bool m_Cmd(UINT msg, DWORD param = 0) const
	{
		MCI_GENERIC_PARMS p = { 0 };
		return m_Cmd(msg, param, p);
	}
	/// ��Ԏ擾
	bool m_GetStatus(DWORD& _status, DWORD item) const
	{
		MCI_STATUS_PARMS mciStatus = { 0 };
		mciStatus.dwItem = item;
		bool r = m_Cmd(MCI_STATUS, MCI_STATUS_ITEM, mciStatus);
		if ( r )
		{
			_status = mciStatus.dwReturn;
		}
		return r;
	}
	MCI_OPEN_PARMS			m_openParms;		///< �I�[�f�B�I�I�[�v���p�����[�^
	MCI_ANIM_OPEN_PARMS		m_animOpenParms;	///< �r�f�I�I�[�v���p�����[�^
	MCIDEVICEID				m_wDeviceId;		///< �I�[�v�����̃f�o�C�XID @note 0 �Ȃ疢�I�[�v��
	HWND					m_hViewWnd;			///< �\���E�B���h�E�n���h��
	HWND					m_hParent;			///< Notify ��E�B���h�E
	bool					m_isPlayed;			///< ���t���H
	bool					m_isPaused;			///< �ꎞ��~��
	mutable MCIERROR		m_lastError;		///< �G���[�R�[�h
};



}; //TNB
