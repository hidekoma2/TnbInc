#pragma once
/**
 *@file
 * DirectX関係のヘッダ
 *
 *	DirectX 8 以上の SDK が必須です。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



#ifndef _TnbDOXYGEN	//Document作成用シンボル
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
 * DirectInputデバイス管理クラス
 *
 *		指定のデバイス(GUID)の DirectInput デバイスを確保＆管理します。
 *
 *	@see CDirectInputMouse や CDirectInputJoystick で使用されています。
 *
 *	@par必要ファイル
 *			TnbDxInput.h
 *			\n DirectX 8 以上の SDK も必要です。
 *
 *	@date 06/06/23	新規作成
 *	@date 09/09/01	整理
 */
class CDirectInputDevice : CCopyImpossible
{
public:

	/// コンストラクタ
	CDirectInputDevice(void) : m_deviceSize(0), m_hResult(E_FAIL)
	{
	}

	/// デストラクタ	
	~CDirectInputDevice(void)
	{
		Destroy(); 
	}

	/**
	 * [破棄] デバイス破棄.
	 *		保持しているオブジェクトとデバイスを開放します。
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
	 * [作成] デバイス作成.
	 *		DirectInputオブジェクトと指定のデバイスを作成します。
	 *	@param dwDevType デバイスタイプ。\n
	 *					DI8DEVCLASS_POINTER		Mouse		\n
	 *					DI8DEVCLASS_KEYBOARD	Keyboard	\n
	 *					DI8DEVCLASS_GAMECTRL	Joystick	\n
	 *	@param hInst インスタンスハンドル。
	 *				省略すると TNB::GetInstanceHandle(EI_Process) で得られるハンドルを使用します。
	 *	@retval true 成功	
	 *	@retval false 失敗
	 */
	bool Create(DWORD dwDevType, HINSTANCE hInst = NULL)
	{
		Destroy();
		if ( hInst == NULL )
		{
			hInst = GetInstanceHandleByTnb(EI_Process);
		}
		//DirectInputオブジェクトの生成
		m_hResult = ::DirectInput8Create(hInst, DIRECTINPUT_VERSION,
						IID_IDirectInput8, reinterpret_cast<LPVOID*>(&m_pDiObject), NULL);
		if ( FAILED(m_hResult) )
		{
			return false;
		}
		//DirectInputデバイスオブジェクト達の生成
		if ( m_pDiObject->EnumDevices(dwDevType, ms_Callback, this, DIEDFL_ATTACHEDONLY) != DI_OK )
		{
			Destroy();
			return false;
		}
		return true;
	}

	/**
	 * [作成] デバイス作成.
	 *		DirectInputオブジェクトと指定のデバイスを作成します。
	 *	@param guid デバイス。
	 *	@param hInst インスタンスハンドル。
	 *				省略すると TNB::GetInstanceHandle(EI_Process) で得られるハンドルを使用します。
	 *	@retval true 成功	
	 *	@retval false 失敗
	 */
	bool Create(const GUID& guid, HINSTANCE hInst = NULL)
	{
		Destroy();
		if ( hInst == NULL )
		{
			hInst = GetInstanceHandleByTnb(EI_Process);
		}
		//DirectInputオブジェクトの生成
		m_hResult = ::DirectInput8Create(hInst, DIRECTINPUT_VERSION,
						IID_IDirectInput8, reinterpret_cast<LPVOID*>(&m_pDiObject), NULL);
		if ( FAILED(m_hResult) )
		{
			return false;
		}
		// デバイス・オブジェクトを作成
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
	 * [参照] オブジェクト参照.
	 *	@return IDirectInput8 のポインタ。NULLの可能性も有ります。
	 */
	const IDirectInput8* ReferObject(void) const
	{
		return m_pDiObject;
	}

	/**
	 * [参照] デバイス数取得.
	 *	@return 取得したデバイスの数。
	 */
	size_t GetDeviceNum(void) const
	{
		return 	m_deviceSize;
	}

	/**
	 * [参照] デバイス参照.
	 *	@param index インデックス。 0以上、 GetDeviceNum() 未満の値を指定します。
	 *	@return IDirectInputDevice8 のポインタ。NULLの可能性も有ります。	
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
	enum { MAX = 16 };									// 最大Device数
	CComPtr<IDirectInput8>			m_pDiObject;		///< DirectInputオブジェクト
	size_t							m_deviceSize;		///< DirectInputデバイスの数
	CComPtr<IDirectInputDevice8>	m_apDiDevice[MAX];	///< DirectInputデバイス
	HRESULT							m_hResult;			///< リザルト
	/// EnumDevices のコールバック
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
 * DirectInput管理ベーステンプレートクラス
 *
 *	@see CDirectInputMouse や CDirectInputJoystick で使用されています。
 *
 *	@par必要ファイル
 *			TnbDxInput.h
 *			\n DirectX 8 以上の SDK も必要です。
 *
 *	@date 06/06/23	新規作成
 *	@date 09/09/01	整理
 */
template<typename TYP>
class CDirectInputT
{
public:

	/**
	 * コンストラクタ
	 *	@param hInst インスタンスハンドル。
	 *				省略すると GetInstanceForTnb() で得られるハンドルを使用します。
	 */
	CDirectInputT(HINSTANCE hInst = NULL) : m_hInst(hInst), m_boIsInit(false)
	{
		Zero(m_state);
	}

	/**
	 * [設定] 初期化.
	 *	@note	Acquire() する前に必ず一度実行してください。
	 *	@param hWnd ウィンドウハンドル。省略できます。
	 *	@param dwMode モード設定。
	 *				省略すると 「DISCL_NONEXCLUSIVE | DISCL_BACKGROUND」が指定されます。\n
	 *					DISCL_BACKGROUND	バックグラウンド\n
	 *					DISCL_FOREGROUND	フォアグラウンド\n
	 *					DISCL_NONEXCLUSIVE	非排他モード	\n
	 *					DISCL_EXCLUSIVE		排他モード		\n
	 *	@retval true 成功	
	 *	@retval false 失敗
	 */
	bool Initialize(HWND hWnd = NULL, DWORD dwMode = DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)
	{
		if ( ! OnCreate(m_dx, m_hInst) )
		{
			return false; 
		}
		loop ( i, m_dx.GetDeviceNum() )
		{
			// データ形式を設定
			if ( ! OnSetDataFormat(m_dx[i]) )
			{
				return false;
			}
			//モードを設定
			if ( hWnd != NULL && FAILED(m_dx[i]->SetCooperativeLevel(hWnd, dwMode)) )
			{
				return false;
			}
			//デバイスの設定
			if ( ! OnSetProperty(m_dx[i]) )
			{
				return false; 
			}
		}
		m_boIsInit = true;
		return true;
	}

	/**
	 * [設定] 実行.
	 *	@note Get() する前に必ず一度実行してください。
	 *	@retval true 成功	
	 *	@retval false 失敗
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
	 * [設定] 停止.
	 *	@retval true 成功	
	 *	@retval false 失敗
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
	 * [設定] デバイス状態更新.
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
	 * [参照] デバイス数取得.
	 *	@return 取得したデバイスの数。	
	 */
	size_t GetDeviceNum(void) const
	{
		return m_dx.GetDeviceNum();
	}

	/**
	 * [取得] 状態取得.
	 *	@note Acquire() している必要が有ります。
	 *	@param index インデックス。 0以上、 GetDeviceNum() 未満の値を指定します。
	 *	@retval NULL	失敗。	
	 *	@retval NULL以外 情報ポインタ（インスタンスで保持しているメモリアドレスです）。
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
	 * [通知] 作成時.
	 *	@note dxに対し作成を要求します。
	 *	@param[in,out] _dx デバイスクラスの参照
	 *	@param[in] hInst インスタンス
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool OnCreate(CDirectInputDevice& _dx, HINSTANCE hInst) = 0;

	/**
	 * [通知] フォーマット設定時.
	 *	@note デバイスのフォーマットを指定します。
	 *	@param[in,out] _px デバイス
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool OnSetDataFormat(IDirectInputDevice8* _px) = 0;

	/**
	 * [通知] プロパティ設定時.
	 *	@note デバイスのプロパティを指定します。
	 *	@param[in,out] _px デバイス
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool OnSetProperty(IDirectInputDevice8* _px) = 0;

private:
	CDirectInputDevice	m_dx;		///< DirectInput管理
	bool				m_boIsInit;	///< true 初期化済み
	HINSTANCE			m_hInst;	///< インスタンスハンドル
	mutable TYP			m_state;	///< 受信データ
};



/**@ingroup DIRECTX
 * DirectInputマウス管理クラス
 *
 *	@par必要ファイル
 *			TnbDxInput.h
 *			\n DirectX 8 以上の SDK も必要です。
 *
 *	@date 06/06/23	新規作成
 *	@date 09/09/01	整理
 */
class CDirectInputMouse : public CDirectInputT<DIMOUSESTATE2>
{
	DEFSUPER(CDirectInputT<DIMOUSESTATE2>);
public:

	/**
	 * [取得] 状態取得.
	 *	@note Acquire() している必要が有ります。
	 *	@param index インデックス。 0以上、 GetDeviceNum() 未満の値を指定します。
	 *	@retval NULL	失敗。	
	 *	@retval NULL以外 情報構造体ポインタ（インスタンスで保持しているメモリアドレスです）。	\n
	 *				lX;マウスのX方向の移動量。						\n
	 *				lY;マウスのY方向の移動量。						\n
	 *				lZ;Wheelの移動量。								\n
	 *				rgbButtons[0];左ボタン状態。0以外ならPushed。	\n
	 *				rgbButtons[1];右ボタン状態。0以外ならPushed。	\n
	 *				rgbButtons[2];Wheelボタン状態。0以外ならPushed。\n
	 */
	const DIMOUSESTATE2* Get(INDEX index = 0) const
	{
		return _super::Get(index);
	}	

protected:

	/**
	 * [通知] 作成時.
	 *	@note dxに対し GUID_SysMouse を作成しています。
	 *	@param[in,out] _dx デバイスクラスの参照
	 *	@param[in] hInst インスタンス
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool OnCreate(CDirectInputDevice& _dx, HINSTANCE hInst)
	{
		return _dx.Create(GUID_SysMouse, hInst);
	}

	/**
	 * [通知] フォーマット設定時.
	 *	@note c_dfDIMouse2フォーマットを指定しています。
	 *	@param[in,out] _px デバイス
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool OnSetDataFormat(IDirectInputDevice8* _px)
	{
		return !! SUCCEEDED(_px->SetDataFormat(&c_dfDIMouse2));
	}

	/**
	 * [通知] プロパティ設定時.
	 *	@note 相対値モードを指定しています。
	 *	@param[in,out] _px デバイス
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool OnSetProperty(IDirectInputDevice8* _px)
	{
		// 軸モードを設定
		DIPROPDWORD diprop		= {sizeof(DIPROPDWORD)};
		diprop.diph.dwHeaderSize = sizeof(diprop.diph); 
		diprop.diph.dwHow		= DIPH_DEVICE;
		diprop.dwData			= DIPROPAXISMODE_REL;	//相対値モード
		return !! SUCCEEDED(_px->SetProperty(DIPROP_AXISMODE, &diprop.diph));
	}
};



/**@ingroup DIRECTX
 * DirectInputジョイスティック管理クラス
 *
 *	@par必要ファイル
 *			TnbDxInput.h
 *			\n DirectX 8 以上の SDK も必要です。
 *
 *	@date 06/06/23	新規作成
 *	@date 09/09/01	整理
 */
class CDirectInputJoystick : public CDirectInputT<DIJOYSTATE2>
{
	DEFSUPER(CDirectInputT<DIJOYSTATE2>);
public:

	/**
	 * [取得] 状態取得.
	 *	@note Acquire() している必要が有ります。
	 *	@param index インデックス。 0以上、 GetDeviceNum() 未満の値を指定します。
	 *	@retval NULL	失敗。	
	 *	@retval NULL以外 情報構造体ポインタ（インスタンスで保持しているメモリアドレスです）。	\n
	 *				lX;StickのX方向の移動量。						\n
	 *				lY;StickのY方向の移動量。						\n
	 *				rgbButtons[X];各ボタン状態。0以外ならPushed。	\n
	 *				など。
	 */
	const DIJOYSTATE2* Get(INDEX index = 0) const
	{
		return _super::Get(index);
	}	

	/**
	 * [取得] Stickの傾き最大値.
	 *		本メソッドでは 1000 を返します。
	 *	@return 傾き最大値.
	 */
	virtual LONG GetStickRange(void) const
	{
		return 1000;
	}

protected:

	/**
	 * [通知] 作成時.
	 *	@note dxに対し DI8DEVCLASS_GAMECTRL を作成しています。
	 *	@param[in,out] _dx デバイスクラスの参照
	 *	@param[in] hInst インスタンス
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool OnCreate(CDirectInputDevice& _dx, HINSTANCE hInst)
	{
		return _dx.Create(DI8DEVCLASS_GAMECTRL, hInst);
	}

	/**
	 * [通知] フォーマット設定時.
	 *	@note c_dfDIJoystick2 フォーマットを指定しています。
	 *	@param[in,out] _px デバイス
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool OnSetDataFormat(IDirectInputDevice8* _px)
	{
		return !! SUCCEEDED(_px->SetDataFormat(&c_dfDIJoystick2));
	}

	/**
	 * [通知] プロパティ設定時.
	 *	@note レンジ幅など指定しています。
	 *	@param[in,out] _px デバイス
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool OnSetProperty(IDirectInputDevice8* _px)
	{
		TParam p = { _px, this };
		return (_px->EnumObjects(ms_EnumObjectsCallback, &p, DIDFT_ALL)) == DI_OK;
	}

private:

	/// コールバック用型
	struct TParam
	{
		IDirectInputDevice8*	pDev;
		CDirectInputJoystick*	pJoystick;
	};
	/// EnumObjectのコールバック
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
