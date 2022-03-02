#pragma once
/**
 *@file
 * DirectX�֌W�̃w�b�_
 *
 *	DirectX 8 �ȏ�� SDK ���K�{�ł��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
 #define DIRECTINPUT_VERSION 0x0800
#endif
#include <atlbase.h>
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")



//TNB Library
namespace TNB{
namespace DX
{



/**@ingroup DIRECTX
 * DirectInput�f�o�C�X�Ǘ��N���X
 *
 *		�w��̃f�o�C�X(GUID)�� DirectInput �f�o�C�X���m�ہ��Ǘ����܂��B
 *
 *	@see CDirectInputMouse �� CDirectInputJoystick �Ŏg�p����Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDxInput.h
 *			\n DirectX 8 �ȏ�� SDK ���K�v�ł��B
 *
 *	@date 06/06/23	�V�K�쐬
 *	@date 09/09/01	����
 */
class CDirectInputDevice : CCopyImpossible
{
public:

	/// �R���X�g���N�^
	CDirectInputDevice(void) : m_deviceSize(0), m_hResult(E_FAIL)
	{
	}

	/// �f�X�g���N�^	
	~CDirectInputDevice(void)
	{
		Destroy(); 
	}

	/**
	 * [�j��] �f�o�C�X�j��.
	 *		�ێ����Ă���I�u�W�F�N�g�ƃf�o�C�X���J�����܂��B
	 */
	void Destroy(void)
	{
		m_pDiObject = NULL;
		loop ( i, m_deviceSize )
		{
			m_apDiDevice[i] = NULL;
		}
		m_deviceSize = 0;
		m_hResult = E_FAIL;
	}

	/**
	 * [�쐬] �f�o�C�X�쐬.
	 *		DirectInput�I�u�W�F�N�g�Ǝw��̃f�o�C�X���쐬���܂��B
	 *	@param dwDevType �f�o�C�X�^�C�v�B\n
	 *					DI8DEVCLASS_POINTER		Mouse		\n
	 *					DI8DEVCLASS_KEYBOARD	Keyboard	\n
	 *					DI8DEVCLASS_GAMECTRL	Joystick	\n
	 *	@param hInst �C���X�^���X�n���h���B
	 *				�ȗ������ TNB::GetInstanceHandle(EI_Process) �œ�����n���h�����g�p���܂��B
	 *	@retval true ����	
	 *	@retval false ���s
	 */
	bool Create(DWORD dwDevType, HINSTANCE hInst = NULL)
	{
		Destroy();
		if ( hInst == NULL )
		{
			hInst = GetInstanceHandleByTnb(EI_Process);
		}
		//DirectInput�I�u�W�F�N�g�̐���
		m_hResult = ::DirectInput8Create(hInst, DIRECTINPUT_VERSION,
						IID_IDirectInput8, reinterpret_cast<LPVOID*>(&m_pDiObject), NULL);
		if ( FAILED(m_hResult) )
		{
			return false;
		}
		//DirectInput�f�o�C�X�I�u�W�F�N�g�B�̐���
		if ( m_pDiObject->EnumDevices(dwDevType, ms_Callback, this, DIEDFL_ATTACHEDONLY) != DI_OK )
		{
			Destroy();
			return false;
		}
		return true;
	}

	/**
	 * [�쐬] �f�o�C�X�쐬.
	 *		DirectInput�I�u�W�F�N�g�Ǝw��̃f�o�C�X���쐬���܂��B
	 *	@param guid �f�o�C�X�B
	 *	@param hInst �C���X�^���X�n���h���B
	 *				�ȗ������ TNB::GetInstanceHandle(EI_Process) �œ�����n���h�����g�p���܂��B
	 *	@retval true ����	
	 *	@retval false ���s
	 */
	bool Create(const GUID& guid, HINSTANCE hInst = NULL)
	{
		Destroy();
		if ( hInst == NULL )
		{
			hInst = GetInstanceHandleByTnb(EI_Process);
		}
		//DirectInput�I�u�W�F�N�g�̐���
		m_hResult = ::DirectInput8Create(hInst, DIRECTINPUT_VERSION,
						IID_IDirectInput8, reinterpret_cast<LPVOID*>(&m_pDiObject), NULL);
		if ( FAILED(m_hResult) )
		{
			return false;
		}
		// �f�o�C�X�E�I�u�W�F�N�g���쐬
		m_hResult = m_pDiObject->CreateDevice(guid, &m_apDiDevice[0], NULL); 
		if ( FAILED(m_hResult) )
		{
			m_pDiObject = NULL;
			return false;
		}
		m_deviceSize = 1;
		return true;
	}

	/**
	 * [�Q��] �I�u�W�F�N�g�Q��.
	 *	@return IDirectInput8 �̃|�C���^�BNULL�̉\�����L��܂��B
	 */
	const IDirectInput8* ReferObject(void) const
	{
		return m_pDiObject;
	}

	/**
	 * [�Q��] �f�o�C�X���擾.
	 *	@return �擾�����f�o�C�X�̐��B
	 */
	size_t GetDeviceNum(void) const
	{
		return 	m_deviceSize;
	}

	/**
	 * [�Q��] �f�o�C�X�Q��.
	 *	@param index �C���f�b�N�X�B 0�ȏ�A GetDeviceNum() �����̒l���w�肵�܂��B
	 *	@return IDirectInputDevice8 �̃|�C���^�BNULL�̉\�����L��܂��B	
	 */
	IDirectInputDevice8* operator[](INDEX index) const
	{
		if ( index < MAX )
		{
			return m_apDiDevice[index]; 
		}
		return NULL;
	}

private:
	enum { MAX = 16 };									// �ő�Device��
	CComPtr<IDirectInput8>			m_pDiObject;		///< DirectInput�I�u�W�F�N�g
	size_t							m_deviceSize;		///< DirectInput�f�o�C�X�̐�
	CComPtr<IDirectInputDevice8>	m_apDiDevice[MAX];	///< DirectInput�f�o�C�X
	HRESULT							m_hResult;			///< ���U���g
	/// EnumDevices �̃R�[���o�b�N
	static BOOL CALLBACK ms_Callback(const DIDEVICEINSTANCE* pInst, VOID* pContext)
	{
		CDirectInputDevice* P = static_cast<CDirectInputDevice*>(pContext);
		size_t l = P->m_deviceSize;
		if ( l < MAX - 1 )
		{
			HRESULT hr = P->m_pDiObject->CreateDevice(pInst->guidInstance, &(P->m_apDiDevice[l]), NULL);
			if ( SUCCEEDED(hr) ) 
			{
				(P->m_deviceSize)++;
				return DIENUM_CONTINUE;
			}
		}
		return DIENUM_STOP;
	}
};



/**@ingroup DIRECTX
 * DirectInput�Ǘ��x�[�X�e���v���[�g�N���X
 *
 *	@see CDirectInputMouse �� CDirectInputJoystick �Ŏg�p����Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDxInput.h
 *			\n DirectX 8 �ȏ�� SDK ���K�v�ł��B
 *
 *	@date 06/06/23	�V�K�쐬
 *	@date 09/09/01	����
 */
template<typename TYP>
class CDirectInputT
{
public:

	/**
	 * �R���X�g���N�^
	 *	@param hInst �C���X�^���X�n���h���B
	 *				�ȗ������ GetInstanceForTnb() �œ�����n���h�����g�p���܂��B
	 */
	CDirectInputT(HINSTANCE hInst = NULL) : m_hInst(hInst), m_boIsInit(false)
	{
		Zero(m_state);
	}

	/**
	 * [�ݒ�] ������.
	 *	@note	Acquire() ����O�ɕK����x���s���Ă��������B
	 *	@param hWnd �E�B���h�E�n���h���B�ȗ��ł��܂��B
	 *	@param dwMode ���[�h�ݒ�B
	 *				�ȗ������ �uDISCL_NONEXCLUSIVE | DISCL_BACKGROUND�v���w�肳��܂��B\n
	 *					DISCL_BACKGROUND	�o�b�N�O���E���h\n
	 *					DISCL_FOREGROUND	�t�H�A�O���E���h\n
	 *					DISCL_NONEXCLUSIVE	��r�����[�h	\n
	 *					DISCL_EXCLUSIVE		�r�����[�h		\n
	 *	@retval true ����	
	 *	@retval false ���s
	 */
	bool Initialize(HWND hWnd = NULL, DWORD dwMode = DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)
	{
		if ( ! OnCreate(m_dx, m_hInst) )
		{
			return false; 
		}
		loop ( i, m_dx.GetDeviceNum() )
		{
			// �f�[�^�`����ݒ�
			if ( ! OnSetDataFormat(m_dx[i]) )
			{
				return false;
			}
			//���[�h��ݒ�
			if ( hWnd != NULL && FAILED(m_dx[i]->SetCooperativeLevel(hWnd, dwMode)) )
			{
				return false;
			}
			//�f�o�C�X�̐ݒ�
			if ( ! OnSetProperty(m_dx[i]) )
			{
				return false; 
			}
		}
		m_boIsInit = true;
		return true;
	}

	/**
	 * [�ݒ�] ���s.
	 *	@note Get() ����O�ɕK����x���s���Ă��������B
	 *	@retval true ����	
	 *	@retval false ���s
	 */
	bool Acquire(void)
	{
		if ( ! m_boIsInit )
		{
			return false;
		}
		loop ( i, m_dx.GetDeviceNum() )
		{
			if ( FAILED(m_dx[i]->Acquire()) )
			{
				return false; 
			}
		}
		return true;
	}
	
	/**
	 * [�ݒ�] ��~.
	 *	@retval true ����	
	 *	@retval false ���s
	 */
	bool Unacquire(void)
	{
		if ( ! m_boIsInit )
		{
			return false;
		}
		loop ( i, m_dx.GetDeviceNum() )
		{
			if ( FAILED(m_dx[i]->Unacquire()) )
			{
				return false; 
			}
		}
		return true;
	}

	/**
	 * [�ݒ�] �f�o�C�X��ԍX�V.
	 */
	void Poll(void)
	{
		if ( m_boIsInit )
		{
			loop ( i, m_dx.GetDeviceNum() )
			{
				m_dx[i]->Poll();
			}
		}
	}

	/**
	 * [�Q��] �f�o�C�X���擾.
	 *	@return �擾�����f�o�C�X�̐��B	
	 */
	size_t GetDeviceNum(void) const
	{
		return m_dx.GetDeviceNum();
	}

	/**
	 * [�擾] ��Ԏ擾.
	 *	@note Acquire() ���Ă���K�v���L��܂��B
	 *	@param index �C���f�b�N�X�B 0�ȏ�A GetDeviceNum() �����̒l���w�肵�܂��B
	 *	@retval NULL	���s�B	
	 *	@retval NULL�ȊO ���|�C���^�i�C���X�^���X�ŕێ����Ă��郁�����A�h���X�ł��j�B
	 */
	const TYP* Get(INDEX index = 0) const
	{
		if ( m_boIsInit && IsInRange(index, GetDeviceNum()) )
		{
			IDirectInputDevice8* px = m_dx[index];
			if ( px != NULL && SUCCEEDED(px->GetDeviceState(sizeof(TYP), &m_state)) )
			{
				return &m_state;
			}
		}
		return NULL;
	}

protected:

	/**
	 * [�ʒm] �쐬��.
	 *	@note dx�ɑ΂��쐬��v�����܂��B
	 *	@param[in,out] _dx �f�o�C�X�N���X�̎Q��
	 *	@param[in] hInst �C���X�^���X
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool OnCreate(CDirectInputDevice& _dx, HINSTANCE hInst) = 0;

	/**
	 * [�ʒm] �t�H�[�}�b�g�ݒ莞.
	 *	@note �f�o�C�X�̃t�H�[�}�b�g���w�肵�܂��B
	 *	@param[in,out] _px �f�o�C�X
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool OnSetDataFormat(IDirectInputDevice8* _px) = 0;

	/**
	 * [�ʒm] �v���p�e�B�ݒ莞.
	 *	@note �f�o�C�X�̃v���p�e�B���w�肵�܂��B
	 *	@param[in,out] _px �f�o�C�X
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool OnSetProperty(IDirectInputDevice8* _px) = 0;

private:
	CDirectInputDevice	m_dx;		///< DirectInput�Ǘ�
	bool				m_boIsInit;	///< true �������ς�
	HINSTANCE			m_hInst;	///< �C���X�^���X�n���h��
	mutable TYP			m_state;	///< ��M�f�[�^
};



/**@ingroup DIRECTX
 * DirectInput�}�E�X�Ǘ��N���X
 *
 *	@par�K�v�t�@�C��
 *			TnbDxInput.h
 *			\n DirectX 8 �ȏ�� SDK ���K�v�ł��B
 *
 *	@date 06/06/23	�V�K�쐬
 *	@date 09/09/01	����
 */
class CDirectInputMouse : public CDirectInputT<DIMOUSESTATE2>
{
	DEFSUPER(CDirectInputT<DIMOUSESTATE2>);
public:

	/**
	 * [�擾] ��Ԏ擾.
	 *	@note Acquire() ���Ă���K�v���L��܂��B
	 *	@param index �C���f�b�N�X�B 0�ȏ�A GetDeviceNum() �����̒l���w�肵�܂��B
	 *	@retval NULL	���s�B	
	 *	@retval NULL�ȊO ���\���̃|�C���^�i�C���X�^���X�ŕێ����Ă��郁�����A�h���X�ł��j�B	\n
	 *				lX;�}�E�X��X�����̈ړ��ʁB						\n
	 *				lY;�}�E�X��Y�����̈ړ��ʁB						\n
	 *				lZ;Wheel�̈ړ��ʁB								\n
	 *				rgbButtons[0];���{�^����ԁB0�ȊO�Ȃ�Pushed�B	\n
	 *				rgbButtons[1];�E�{�^����ԁB0�ȊO�Ȃ�Pushed�B	\n
	 *				rgbButtons[2];Wheel�{�^����ԁB0�ȊO�Ȃ�Pushed�B\n
	 */
	const DIMOUSESTATE2* Get(INDEX index = 0) const
	{
		return _super::Get(index);
	}	

protected:

	/**
	 * [�ʒm] �쐬��.
	 *	@note dx�ɑ΂� GUID_SysMouse ���쐬���Ă��܂��B
	 *	@param[in,out] _dx �f�o�C�X�N���X�̎Q��
	 *	@param[in] hInst �C���X�^���X
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool OnCreate(CDirectInputDevice& _dx, HINSTANCE hInst)
	{
		return _dx.Create(GUID_SysMouse, hInst);
	}

	/**
	 * [�ʒm] �t�H�[�}�b�g�ݒ莞.
	 *	@note c_dfDIMouse2�t�H�[�}�b�g���w�肵�Ă��܂��B
	 *	@param[in,out] _px �f�o�C�X
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool OnSetDataFormat(IDirectInputDevice8* _px)
	{
		return !! SUCCEEDED(_px->SetDataFormat(&c_dfDIMouse2));
	}

	/**
	 * [�ʒm] �v���p�e�B�ݒ莞.
	 *	@note ���Βl���[�h���w�肵�Ă��܂��B
	 *	@param[in,out] _px �f�o�C�X
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool OnSetProperty(IDirectInputDevice8* _px)
	{
		// �����[�h��ݒ�
		DIPROPDWORD diprop		= {sizeof(DIPROPDWORD)};
		diprop.diph.dwHeaderSize = sizeof(diprop.diph); 
		diprop.diph.dwHow		= DIPH_DEVICE;
		diprop.dwData			= DIPROPAXISMODE_REL;	//���Βl���[�h
		return !! SUCCEEDED(_px->SetProperty(DIPROP_AXISMODE, &diprop.diph));
	}
};



/**@ingroup DIRECTX
 * DirectInput�W���C�X�e�B�b�N�Ǘ��N���X
 *
 *	@par�K�v�t�@�C��
 *			TnbDxInput.h
 *			\n DirectX 8 �ȏ�� SDK ���K�v�ł��B
 *
 *	@date 06/06/23	�V�K�쐬
 *	@date 09/09/01	����
 */
class CDirectInputJoystick : public CDirectInputT<DIJOYSTATE2>
{
	DEFSUPER(CDirectInputT<DIJOYSTATE2>);
public:

	/**
	 * [�擾] ��Ԏ擾.
	 *	@note Acquire() ���Ă���K�v���L��܂��B
	 *	@param index �C���f�b�N�X�B 0�ȏ�A GetDeviceNum() �����̒l���w�肵�܂��B
	 *	@retval NULL	���s�B	
	 *	@retval NULL�ȊO ���\���̃|�C���^�i�C���X�^���X�ŕێ����Ă��郁�����A�h���X�ł��j�B	\n
	 *				lX;Stick��X�����̈ړ��ʁB						\n
	 *				lY;Stick��Y�����̈ړ��ʁB						\n
	 *				rgbButtons[X];�e�{�^����ԁB0�ȊO�Ȃ�Pushed�B	\n
	 *				�ȂǁB
	 */
	const DIJOYSTATE2* Get(INDEX index = 0) const
	{
		return _super::Get(index);
	}	

	/**
	 * [�擾] Stick�̌X���ő�l.
	 *		�{���\�b�h�ł� 1000 ��Ԃ��܂��B
	 *	@return �X���ő�l.
	 */
	virtual LONG GetStickRange(void) const
	{
		return 1000;
	}

protected:

	/**
	 * [�ʒm] �쐬��.
	 *	@note dx�ɑ΂� DI8DEVCLASS_GAMECTRL ���쐬���Ă��܂��B
	 *	@param[in,out] _dx �f�o�C�X�N���X�̎Q��
	 *	@param[in] hInst �C���X�^���X
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool OnCreate(CDirectInputDevice& _dx, HINSTANCE hInst)
	{
		return _dx.Create(DI8DEVCLASS_GAMECTRL, hInst);
	}

	/**
	 * [�ʒm] �t�H�[�}�b�g�ݒ莞.
	 *	@note c_dfDIJoystick2 �t�H�[�}�b�g���w�肵�Ă��܂��B
	 *	@param[in,out] _px �f�o�C�X
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool OnSetDataFormat(IDirectInputDevice8* _px)
	{
		return !! SUCCEEDED(_px->SetDataFormat(&c_dfDIJoystick2));
	}

	/**
	 * [�ʒm] �v���p�e�B�ݒ莞.
	 *	@note �����W���Ȃǎw�肵�Ă��܂��B
	 *	@param[in,out] _px �f�o�C�X
	 *	@retval true ����
	 *	@retval false ���s
	 */
	virtual bool OnSetProperty(IDirectInputDevice8* _px)
	{
		TParam p = { _px, this };
		return (_px->EnumObjects(ms_EnumObjectsCallback, &p, DIDFT_ALL)) == DI_OK;
	}

private:

	/// �R�[���o�b�N�p�^
	struct TParam
	{
		IDirectInputDevice8*	pDev;
		CDirectInputJoystick*	pJoystick;
	};
	/// EnumObject�̃R�[���o�b�N
	static BOOL CALLBACK ms_EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, LPVOID pContext)
	{
		TParam* P = static_cast<TParam*>(pContext);
		if ( (pdidoi->dwType & DIDFT_AXIS) != 0 )
		{
			DIPROPRANGE diprg; 
			diprg.diph.dwSize		= sizeof(DIPROPRANGE); 
			diprg.diph.dwHeaderSize	= sizeof(DIPROPHEADER); 
			diprg.diph.dwHow		= DIPH_BYID; 
			diprg.diph.dwObj		= pdidoi->dwType; // Specify the enumerated axis
			diprg.lMin				= P->pJoystick->GetStickRange() * -1;
			diprg.lMax				= P->pJoystick->GetStickRange();
			// Set the range for the axis
			if ( FAILED(P->pDev->SetProperty(DIPROP_RANGE, &diprg.diph)) )
			{
				return false;
			}
		}
		return true;
	}
};



};//DX
};//TNB
