#pragma once
/**
 *@file
 * ミキサーボリューム関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"
#include <mmsystem.h>



//TNB Library
namespace TNB
{



/**@ingroup AUDIO
 * ミキサーボリュームクラス
 *
 *		ボリューム値を取得、設定することが出来ます。
 *
 *	@note コールバックはウィンドウメッセージです。\n
 *			message = MM_MIXM_CONTROL_CHANGE , wParam = ミキサーデバイスハンドル , lParam = コントロール ID
 *
 *	@par必要ファイル
 *			TnbMixerVolume.h
 *
 *	@date 08/11/05 新規作成
 */
class CMixerVolume : CCopyImpossible
{
public:

	/// コンストラクタ
	CMixerVolume(void) : m_hMix(NULL), m_result(MMSYSERR_NOERROR)
	{
	}

	/// デストラクタ
	~CMixerVolume(void)
	{
		Close();
	}

	/**
	 * [設定] クローズ
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
	 * [設定] オープン
	 *	@param mxid ミキサーデバイスID。 ０～ GetDevicesNumber() -1 の間で指定します。
	 *	@param hCallbackWnd コールバック先のウィンドウハンドルを指定できます。
	 *	@retval true 成功
	 *	@retval false 失敗
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
	 * [取得] デバイス名取得.
	 *		オープンしたデバイス名を取得できます。
	 *	@return 名
	 */
	LPCTSTR GetDeviceName(void)
	{
		return (m_hMix == NULL) ? NULL : m_mixerLine.Target.szPname;
	}

	/**
	 * [取得] ライン数取得
	 *	@return ライン数
	 */
	size_t GetLinesNumber(void) const
	{
		return (m_hMix == NULL) ? 0 : m_mixerLine.cConnections;
	}

	/**@ingroup AUDIO
	 * ボリュームパラメータ型
	 *
	 *		CMixerVolume::GetLineVolume() , CMixerVolume::GetMasterVolume() , CMixerVolume::GetComponentTypeVolume() で使用されます。
	 *
	 *	@note コンポーネントタイプは MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC , MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT などです。
	 *
	 *	@note モノラルの場合、ボリューム値は left に入り、 right は DWORD_MAX になります。
	 *
	 *	@par必要ファイル
	 *			TnbMixerVolume.h
	 *
	 *	@date 08/11/05 新規作成
	 */
	struct TParam
	{
		TCHAR	szLineName[MIXER_LONG_NAME_CHARS];		///< ライン名
		DWORD	componentType;							///< コンポーネントタイプ
		TCHAR	szVolumeName[MIXER_LONG_NAME_CHARS];	///< ボリューム名
		DWORD	left;									///< 左ボリューム値
		DWORD	right;									///< 右ボリューム値
		DWORD	minimum;								///< ボリューム値最小
		DWORD	maximum;								///< ボリューム値最大
		TCHAR	szMuteName[MIXER_LONG_NAME_CHARS];		///< ミュート名
		bool	isMute;									///< ミュート
	};

	/**
	 * [取得] マスターボリューム取得.
	 *		マスターボリュームパラメータを取得できます。
	 *	@param[out] _prm 取得したパラメータが格納されます。
	 *	@retval true 取得成功。
	 *	@retval false 失敗。 詳細は GetLastError() で得られます。
	 */
	bool GetMasterVolume(TParam& _prm) const
	{
		return GetComponentTypeVolume(_prm, MIXERLINE_COMPONENTTYPE_DST_SPEAKERS);
	}

	/**
	 * [取得] ラインボリューム取得.
	 *		指定したラインのボリュームパラメータを取得できます。
	 *	@param[out] _prm 取得したパラメータが格納されます。
	 *	@param[in] line ライン。 ０～ GetLineNumber() -1 の間で指定します。
	 *	@retval true 取得成功。
	 *	@retval false 失敗。 詳細は GetLastError() で得られます。
	 */
	bool GetLineVolume(TParam& _prm, size_t line) const
	{
		if ( line == INVALID_INDEX ) { return GetMasterVolume(_prm); }
		Zero(_prm);
		return m_LineParam(_prm, line, 0, true);
	}

	/**
	 * [取得] コンポーネントタイプボリューム取得.
	 *		指定したコンポーネントタイプのボリュームパラメータを取得できます。
	 *	@param[out] _prm 取得したパラメータが格納されます。
	 *	@param[in] componentType コンポーネントタイプ。 MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC など。
	 *	@retval true 取得成功。
	 *	@retval false 失敗。 詳細は GetLastError() で得られます。
	 */
	bool GetComponentTypeVolume(TParam& _prm, DWORD componentType) const
	{
		Zero(_prm);
		return m_LineParam(_prm, 0, componentType, true);
	}

	/**
	 * [設定] マスターボリューム設定.
	 *		マスターボリュームパラメータの設定ができます。
	 *	@note モノラルの場合、 right の値は無視されます。
	 *	@param left 左のボリューム値。 GetMasterVolume() で得た、最小、最大内になるようにします。
	 *	@param right 右のボリューム値。 GetMasterVolume() で得た、最小、最大内になるようにします。
	 *	@param isMute ミュート状態。省略すると false になります。
	 *	@retval true 取得成功。
	 *	@retval false 失敗。 詳細は GetLastError() で得られます。
	 */
	bool SetMasterVolume(DWORD left, DWORD right, bool isMute = false)
	{
		return SetComponentTypeVolume(MIXERLINE_COMPONENTTYPE_DST_SPEAKERS, left, right, isMute);
	}

	/**
	 * [設定] ラインボリューム設定.
	 *		指定したラインのボリュームパラメータの設定ができます。
	 *	@note モノラルの場合、 right の値は無視されます。
	 *	@param line ライン。 ０～ GetLineNumber() -1 の間で指定します。
	 *	@param left 左のボリューム値。 GetLineVolume() で得た、最小、最大内になるようにします。
	 *	@param right 右のボリューム値。 GetLineVolume() で得た、最小、最大内になるようにします。
	 *	@param isMute ミュート状態。省略すると false になります。
	 *	@retval true 取得成功。
	 *	@retval false 失敗。 詳細は GetLastError() で得られます。
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
	 * [設定] コンポーネントタイプボリューム設定.
	 *		指定したコンポーネントタイプのボリュームパラメータの設定ができます。
	 *	@note モノラルの場合、 right の値は無視されます。
	 *	@param componentType コンポーネントタイプ。 MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC など。
	 *	@param left 左のボリューム値。 GetComponentTypeVolume() で得た、最小、最大内になるようにします。
	 *	@param right 右のボリューム値。 GetComponentTypeVolume() で得た、最小、最大内になるようにします。
	 *	@param isMute ミュート状態。省略すると false になります。
	 *	@retval true 取得成功。
	 *	@retval false 失敗。 詳細は GetLastError() で得られます。
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
	 * [取得] エラーコード取得.
	 *	@return MMRESULT
	 */
	MMRESULT GetLastError(void) const
	{
		return m_result;
	}
	
	/**
	 * [取得] デバイス数取得
	 *	@return デバイス数
	 */
	static size_t GetDevicesNumber(void)
	{
		return ::mixerGetNumDevs();
	}

private:

	/**
	 * パラメータ操作
	 *	@param[in,out] _prm パラメータ
	 *	@param[in] line ライン
	 *	@param[in] compType コンポーネントタイプ
	 *	@param[in] isGet true なら取得、 false なら設定をおこなう
	 *	@retval true 取得成功。
	 *	@retval false 失敗。 詳細は GetLastError() で得られます。
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
	/// リザルト処理
	bool m_res(MMRESULT r) const
	{
		m_result = r;
		return r == MMSYSERR_NOERROR;
	}
	HMIXEROBJ			m_hMix;			///< ミキサーハンドル
	mutable	MMRESULT	m_result;		///< ラストリザルト
	MIXERLINE			m_mixerLine;	///< ミキサー情報
};



}; // TNB

