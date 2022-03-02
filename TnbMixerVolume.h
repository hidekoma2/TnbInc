#pragma once
/**
 *@file
 * �~�L�T�[�{�����[���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"
#include <mmsystem.h>



//TNB Library
namespace TNB
{



/**@ingroup AUDIO
 * �~�L�T�[�{�����[���N���X
 *
 *		�{�����[���l���擾�A�ݒ肷�邱�Ƃ��o���܂��B
 *
 *	@note �R�[���o�b�N�̓E�B���h�E���b�Z�[�W�ł��B\n
 *			message = MM_MIXM_CONTROL_CHANGE , wParam = �~�L�T�[�f�o�C�X�n���h�� , lParam = �R���g���[�� ID
 *
 *	@par�K�v�t�@�C��
 *			TnbMixerVolume.h
 *
 *	@date 08/11/05 �V�K�쐬
 */
class CMixerVolume : CCopyImpossible
{
public:

	/// �R���X�g���N�^
	CMixerVolume(void) : m_hMix(NULL), m_result(MMSYSERR_NOERROR)
	{
	}

	/// �f�X�g���N�^
	~CMixerVolume(void)
	{
		Close();
	}

	/**
	 * [�ݒ�] �N���[�Y
	 */
	void Close(void)
	{
		if ( m_hMix != NULL )
		{
			HMIXER h = reinterpret_cast<HMIXER>(m_hMix);
			::mixerClose(h);
			m_hMix = NULL;
		}
	}

	/**
	 * [�ݒ�] �I�[�v��
	 *	@param mxid �~�L�T�[�f�o�C�XID�B �O�` GetDevicesNumber() -1 �̊ԂŎw�肵�܂��B
	 *	@param hCallbackWnd �R�[���o�b�N��̃E�B���h�E�n���h�����w��ł��܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Open(UINT mxid = 0, HWND hCallbackWnd = NULL)
	{
		Close();
		HMIXER h = NULL;
		DWORD dwCallback = reinterpret_cast<DWORD>(hCallbackWnd);
		DWORD dwFlag = MIXER_OBJECTF_MIXER;
		if ( dwCallback != 0 )
		{
			dwFlag |= CALLBACK_WINDOW;
		}
		if ( m_res(::mixerOpen(&h, mxid, dwCallback, 0, dwFlag)) )
		{
			m_hMix = reinterpret_cast<HMIXEROBJ>(h);
			Zero(m_mixerLine);
			m_mixerLine.cbStruct = sizeof(MIXERLINE);
			if ( m_res(::mixerGetLineInfo(m_hMix, &m_mixerLine, MIXER_GETLINEINFOF_DESTINATION | MIXER_OBJECTF_HMIXER)) )
			{
				return true;
			}
		}
		Close();
		return false;
	}

	/**
	 * [�擾] �f�o�C�X���擾.
	 *		�I�[�v�������f�o�C�X�����擾�ł��܂��B
	 *	@return ��
	 */
	LPCTSTR GetDeviceName(void)
	{
		return (m_hMix == NULL) ? NULL : m_mixerLine.Target.szPname;
	}

	/**
	 * [�擾] ���C�����擾
	 *	@return ���C����
	 */
	size_t GetLinesNumber(void) const
	{
		return (m_hMix == NULL) ? 0 : m_mixerLine.cConnections;
	}

	/**@ingroup AUDIO
	 * �{�����[���p�����[�^�^
	 *
	 *		CMixerVolume::GetLineVolume() , CMixerVolume::GetMasterVolume() , CMixerVolume::GetComponentTypeVolume() �Ŏg�p����܂��B
	 *
	 *	@note �R���|�[�l���g�^�C�v�� MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC , MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT �Ȃǂł��B
	 *
	 *	@note ���m�����̏ꍇ�A�{�����[���l�� left �ɓ���A right �� DWORD_MAX �ɂȂ�܂��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbMixerVolume.h
	 *
	 *	@date 08/11/05 �V�K�쐬
	 */
	struct TParam
	{
		TCHAR	szLineName[MIXER_LONG_NAME_CHARS];		///< ���C����
		DWORD	componentType;							///< �R���|�[�l���g�^�C�v
		TCHAR	szVolumeName[MIXER_LONG_NAME_CHARS];	///< �{�����[����
		DWORD	left;									///< ���{�����[���l
		DWORD	right;									///< �E�{�����[���l
		DWORD	minimum;								///< �{�����[���l�ŏ�
		DWORD	maximum;								///< �{�����[���l�ő�
		TCHAR	szMuteName[MIXER_LONG_NAME_CHARS];		///< �~���[�g��
		bool	isMute;									///< �~���[�g
	};

	/**
	 * [�擾] �}�X�^�[�{�����[���擾.
	 *		�}�X�^�[�{�����[���p�����[�^���擾�ł��܂��B
	 *	@param[out] _prm �擾�����p�����[�^���i�[����܂��B
	 *	@retval true �擾�����B
	 *	@retval false ���s�B �ڍׂ� GetLastError() �œ����܂��B
	 */
	bool GetMasterVolume(TParam& _prm) const
	{
		return GetComponentTypeVolume(_prm, MIXERLINE_COMPONENTTYPE_DST_SPEAKERS);
	}

	/**
	 * [�擾] ���C���{�����[���擾.
	 *		�w�肵�����C���̃{�����[���p�����[�^���擾�ł��܂��B
	 *	@param[out] _prm �擾�����p�����[�^���i�[����܂��B
	 *	@param[in] line ���C���B �O�` GetLineNumber() -1 �̊ԂŎw�肵�܂��B
	 *	@retval true �擾�����B
	 *	@retval false ���s�B �ڍׂ� GetLastError() �œ����܂��B
	 */
	bool GetLineVolume(TParam& _prm, size_t line) const
	{
		if ( line == INVALID_INDEX ) { return GetMasterVolume(_prm); }
		Zero(_prm);
		return m_LineParam(_prm, line, 0, true);
	}

	/**
	 * [�擾] �R���|�[�l���g�^�C�v�{�����[���擾.
	 *		�w�肵���R���|�[�l���g�^�C�v�̃{�����[���p�����[�^���擾�ł��܂��B
	 *	@param[out] _prm �擾�����p�����[�^���i�[����܂��B
	 *	@param[in] componentType �R���|�[�l���g�^�C�v�B MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC �ȂǁB
	 *	@retval true �擾�����B
	 *	@retval false ���s�B �ڍׂ� GetLastError() �œ����܂��B
	 */
	bool GetComponentTypeVolume(TParam& _prm, DWORD componentType) const
	{
		Zero(_prm);
		return m_LineParam(_prm, 0, componentType, true);
	}

	/**
	 * [�ݒ�] �}�X�^�[�{�����[���ݒ�.
	 *		�}�X�^�[�{�����[���p�����[�^�̐ݒ肪�ł��܂��B
	 *	@note ���m�����̏ꍇ�A right �̒l�͖�������܂��B
	 *	@param left ���̃{�����[���l�B GetMasterVolume() �œ����A�ŏ��A�ő���ɂȂ�悤�ɂ��܂��B
	 *	@param right �E�̃{�����[���l�B GetMasterVolume() �œ����A�ŏ��A�ő���ɂȂ�悤�ɂ��܂��B
	 *	@param isMute �~���[�g��ԁB�ȗ������ false �ɂȂ�܂��B
	 *	@retval true �擾�����B
	 *	@retval false ���s�B �ڍׂ� GetLastError() �œ����܂��B
	 */
	bool SetMasterVolume(DWORD left, DWORD right, bool isMute = false)
	{
		return SetComponentTypeVolume(MIXERLINE_COMPONENTTYPE_DST_SPEAKERS, left, right, isMute);
	}

	/**
	 * [�ݒ�] ���C���{�����[���ݒ�.
	 *		�w�肵�����C���̃{�����[���p�����[�^�̐ݒ肪�ł��܂��B
	 *	@note ���m�����̏ꍇ�A right �̒l�͖�������܂��B
	 *	@param line ���C���B �O�` GetLineNumber() -1 �̊ԂŎw�肵�܂��B
	 *	@param left ���̃{�����[���l�B GetLineVolume() �œ����A�ŏ��A�ő���ɂȂ�悤�ɂ��܂��B
	 *	@param right �E�̃{�����[���l�B GetLineVolume() �œ����A�ŏ��A�ő���ɂȂ�悤�ɂ��܂��B
	 *	@param isMute �~���[�g��ԁB�ȗ������ false �ɂȂ�܂��B
	 *	@retval true �擾�����B
	 *	@retval false ���s�B �ڍׂ� GetLastError() �œ����܂��B
	 */
	bool SetLineVolume(size_t line, DWORD left, DWORD right, bool isMute = false)
	{
		if ( line == INVALID_INDEX ) { return SetMasterVolume(left, right, isMute); }
		TParam p = { 0 };
		p.left = left;
		p.right = right;
		p.isMute = isMute;
		return m_LineParam(p, line, 0, false);
	}

	/**
	 * [�ݒ�] �R���|�[�l���g�^�C�v�{�����[���ݒ�.
	 *		�w�肵���R���|�[�l���g�^�C�v�̃{�����[���p�����[�^�̐ݒ肪�ł��܂��B
	 *	@note ���m�����̏ꍇ�A right �̒l�͖�������܂��B
	 *	@param componentType �R���|�[�l���g�^�C�v�B MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC �ȂǁB
	 *	@param left ���̃{�����[���l�B GetComponentTypeVolume() �œ����A�ŏ��A�ő���ɂȂ�悤�ɂ��܂��B
	 *	@param right �E�̃{�����[���l�B GetComponentTypeVolume() �œ����A�ŏ��A�ő���ɂȂ�悤�ɂ��܂��B
	 *	@param isMute �~���[�g��ԁB�ȗ������ false �ɂȂ�܂��B
	 *	@retval true �擾�����B
	 *	@retval false ���s�B �ڍׂ� GetLastError() �œ����܂��B
	 */
	bool SetComponentTypeVolume(DWORD componentType, DWORD left, DWORD right, bool isMute = false)
	{
		TParam p = { 0 };
		p.left = left;
		p.right = right;
		p.isMute = isMute;
		return m_LineParam(p, 0, componentType, false);
	}

	/**
	 * [�擾] �G���[�R�[�h�擾.
	 *	@return MMRESULT
	 */
	MMRESULT GetLastError(void) const
	{
		return m_result;
	}
	
	/**
	 * [�擾] �f�o�C�X���擾
	 *	@return �f�o�C�X��
	 */
	static size_t GetDevicesNumber(void)
	{
		return ::mixerGetNumDevs();
	}

private:

	/**
	 * �p�����[�^����
	 *	@param[in,out] _prm �p�����[�^
	 *	@param[in] line ���C��
	 *	@param[in] compType �R���|�[�l���g�^�C�v
	 *	@param[in] isGet true �Ȃ�擾�A false �Ȃ�ݒ�������Ȃ�
	 *	@retval true �擾�����B
	 *	@retval false ���s�B �ڍׂ� GetLastError() �œ����܂��B
	 */
	bool m_LineParam(TParam& _prm, size_t line, DWORD compType, bool isGet) const
	{
		if ( m_hMix == NULL )
		{
			m_res(MMSYSERR_ERROR);
			return false;
		}
		MIXERLINE ml = { 0 };
		ml.cbStruct = sizeof(MIXERLINE);
		DWORD info = MIXER_OBJECTF_HMIXER;
		if ( compType != 0 )
		{
			ml.dwComponentType = compType;
			info |= MIXER_GETLINEINFOF_COMPONENTTYPE;
		}
		else
		{
			ml.dwSource = line;
			info |= MIXER_GETLINEINFOF_SOURCE;
		}
		if ( ! m_res(::mixerGetLineInfo(m_hMix, &ml, info)) )
		{
			return false;
		}
		MemCopy(_prm.szLineName, ml.szName, MIXER_LONG_NAME_CHARS);
		_prm.componentType = ml.dwComponentType;
		CWorkMemT<MIXERCONTROL> amc(ml.cControls);
		loop ( i, ml.cControls )
		{
			amc[i].cbStruct = sizeof(MIXERCONTROL);
		}
		MIXERLINECONTROLS mlc = { 0 };
		mlc.cbStruct = sizeof(mlc);
		mlc.dwLineID = ml.dwLineID;
		mlc.cControls = ml.cControls;
		mlc.cbmxctrl = sizeof(MIXERCONTROL);
		mlc.pamxctrl = amc;
		if ( ! m_res(::mixerGetLineControls(m_hMix, &mlc, MIXER_GETLINECONTROLSF_ALL | MIXER_OBJECTF_HMIXER)) )
		{
			return false;
		}
		loop ( i, ml.cControls )
		{
			MIXERCONTROL* P = &amc[i];
			MIXERCONTROLDETAILS mcd = { 0 };
			mcd.cMultipleItems = P->cMultipleItems;
			if ( (P->fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE) == 0 )
			{
				mcd.cMultipleItems = 0;
			}
			mcd.cChannels = ml.cChannels;
			if ( (P->fdwControl & MIXERCONTROL_CONTROLF_UNIFORM) != 0 )
			{
				mcd.cChannels = 1;
			}
			MIXERCONTROLDETAILS_BOOLEAN bn = { 0 };
			MIXERCONTROLDETAILS_UNSIGNED us[2] = { 0 };
			if ( P->dwControlType == MIXERCONTROL_CONTROLTYPE_VOLUME )
			{
				mcd.paDetails = us;
				mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
				us[0].dwValue = _prm.left;
				us[1].dwValue = _prm.right;
			}
			else if ( P->dwControlType == MIXERCONTROL_CONTROLTYPE_MUTE )
			{
				mcd.paDetails = &bn;
				mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
				mcd.cChannels = 1;
				bn.fValue = _prm.isMute;
			}
			else
			{
				continue;
			}
			mcd.dwControlID = P->dwControlID;
			mcd.cbStruct = sizeof(mcd);
			//
			if ( isGet )
			{
				if ( ! m_res(::mixerGetControlDetails(m_hMix, &mcd, MIXER_GETCONTROLDETAILSF_VALUE | MIXER_OBJECTF_HMIXER)) )
				{
					return false;
				}
				if ( P->dwControlType == MIXERCONTROL_CONTROLTYPE_VOLUME )
				{
					MemCopy(_prm.szVolumeName, P->szName, MIXER_LONG_NAME_CHARS);
					_prm.left = us[0].dwValue;
					_prm.right = us[1].dwValue;
					if ( ml.cChannels == 1 )
					{
						_prm.right = DWORD_MAX;
					}
					_prm.minimum = P->Bounds.dwMinimum;
					_prm.maximum = P->Bounds.dwMaximum;
				}
				else if ( P->dwControlType == MIXERCONTROL_CONTROLTYPE_MUTE )
				{
					MemCopy(_prm.szMuteName, P->szName, MIXER_LONG_NAME_CHARS);
					_prm.isMute = !! bn.fValue;
				}
			}
			else
			{
				if ( ! m_res(::mixerSetControlDetails(m_hMix, &mcd, MIXER_SETCONTROLDETAILSF_VALUE | MIXER_OBJECTF_HMIXER)) )
				{
					return false;
				}
			}
		}
		return true;
	}
	/// ���U���g����
	bool m_res(MMRESULT r) const
	{
		m_result = r;
		return r == MMSYSERR_NOERROR;
	}
	HMIXEROBJ			m_hMix;			///< �~�L�T�[�n���h��
	mutable	MMRESULT	m_result;		///< ���X�g���U���g
	MIXERLINE			m_mixerLine;	///< �~�L�T�[���
};



}; // TNB

