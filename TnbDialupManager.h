#pragma once
/**
 *@file
 * ダイアルアップ関係のヘッダ
 *
 *		ダイアルアップ後、切断までの監視を行うこともできます。
 *		ダイアルアップのアタッチができます。
 *		エントリの一覧を取得できます。
 *
 *	@note Windowsの RAS－API を使用しています。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStrVector.h"
#include "TnbStrEx.h"
#include "TnbMap.h"
#include "TnbThread.h"
#include "TnbStaticMap.h"
#include <Ras.h>
#include <RasError.h>
#ifndef _WIN32_WCE
 #include "TnbRegistryAccessor.h"
 #pragma comment(lib,"RasApi32.lib")
 #if ( WINVER >= 0x500 )
   #ifndef RASEAPINFO
	#error 
	// WINVER 0x0500 以上の場合、VC6++ 付属の RAS.h は使用できません（バグ）
	// SDK のRAS.h をインクルードするようにしてください。
	//（ｵﾌﾟｼｮﾝ→ﾃﾞｨﾚｸﾄﾘで SDKのヘッダのほうが先になるようにする、など）。
   #endif
 #endif
#endif



//TNB Library
namespace TNB
{



/// 本ヘッダ専用 TRACE マクロ
#define _DIALUP_TRACE(FUNC, VAL) \
	if ( (VAL) != 0 ) { TRACE3("%s failed: Error %u[%s]\n", _T(#FUNC), VAL, GetErrorString(VAL)); }



/**@ingroup RAS
 * ダイアルアップ管理クラス
 *
 *		インスタンス一つにつき、一つのダイアルアップを管理、監視することができます。
 *		すでに接続しているダイアルアップをアタッチすることも可能です。
 *
 *	@note リスナー内で SendMessage を使う場合、 ReferThreadStatus() で{@link CThreadStatus::SetOwnerHwnd() 関連ウィンドウ}を指定する
 *				必要があります。 CThread::SetOwnerHwnd() を参照してください。
 *
 *	@par必要ファイル
 *			TnbDialupManager.h
 *
 *	@date 06/04/01	新規作成
 *	@date 08/07/08	CEでの調整
 *	@date 10/03/19	ReferThreadStatus() 追加。
 */
class CDialupManager : TNB::CThread::IRunner, CCopyImpossible
{
public:

	/**@ingroup RAS
	 * ダイアルアップのリスナーインターフェース
	 *
	 *		CDialupManager の通知用インターフェースです。
	 *
	 *	@see CDialupManager::Attach()
	 *	@see CDialupManager::Dial()
	 *
	 *	@par必要ファイル
	 *			TnbDialupManager.h
	 *
	 *	@date 06/04/01	新規作成
	 */
	struct IListener
	{
		/** デストラクタ */
		virtual ~IListener(void) {}

		/** 
		 * [通知] エラー発生.
		 *	@note これを受信したら、接続に失敗/切断になっています。以後、リスナーに報告は有りません。
		 *	@param dwError エラーコード。
		 */
		virtual void OnDialupError(DWORD dwError) = 0;

		/** 
		 * [通知] 状態変化発生
		 *	@param eState 状態コード。									<BR>
		 *			「RASCS_Connected」を受信したら接続成功です。		<BR>
		 *			「RASCS_Disconnected」を受信したら切断です。以後リスナーに報告は有りません。<BR>
		 *			それ以外は状態変化の通知になります。				<BR>
		 */
		virtual void OnDialupChangeState(RASCONNSTATE eState) = 0;

	private:
		friend class CDialupManager;		///< フレンドクラス宣言
	};

private:

	CThread				m_threadWatch;	///< スレッド管理
	IListener*			m_piListener;	///< リスナー
	HRASCONN			m_hRasConn;		///< RASハンドル
	CSyncSection		m_syncFunc;		///< 排他
	enum
	{
		E_ConnectCheckInterval = 100,	///< 接続中のチェック間隔
		E_DisconnectCheckInterval = 500,///< 切断確認のチェック間隔
	};

	/** 
	 *	RASCONN構造体
	 *
	 *		 RASCONNのVer4.01以降の型を用意しています 
	 */
	struct TRasConn
	{
		DWORD		dwSize;									///< sizeof(TRasConn)
		HRASCONN	hRasConn;								///< RASのハンドル
		TCHAR		szEntryName[RAS_MaxEntryName + 1];		///< エントリ名
		#ifndef _WIN32_WCE
		TCHAR		szDeviceType[RAS_MaxDeviceType + 1];	///< デバイス種
		TCHAR		szDeviceName[RAS_MaxDeviceName + 1];	///< デバイス名
		TCHAR		szPhonebook[MAX_PATH];					///< 電話帳名
		DWORD		dwSubEntry;								///< サブエントリ
		#endif
		/**
		 * RASCONNのポインタ取得
		 *	@return RASCONNのポインタ
		 */
		operator LPRASCONN(){ return reinterpret_cast<LPRASCONN>(this); }
		/**
		 * 電話帳比較
		 *	@param s 電話帳
		 *	@retval true 同じ電話帳。
		 *	@retval false 異なる。
		 */
		bool IsPhoneBookEqual(CStr& s) const
		{
			#ifndef _WIN32_WCE
				return (s.IsEmpty() || STRLIB::Compare(szPhonebook, s) == 0);
			#else
				return s.IsEmpty();
			#endif
		}

	};

	/// TRasConnの配列宣言
	typedef CVectorT<TRasConn> CRasConnVector;

	/**
	 * 接続中のTRasConn全部取得
	 *	@return TRasConnの配列。無効ならAPIエラー発生。
	 */
	static CRasConnVector ms_GetConnectingEnties(void)
	{
		CRasConnVector aRasConn;
		CByteVector ba;
		TRasConn* P = reinterpret_cast<TRasConn*>(ba.GetBuffer(sizeof(TRasConn)));
		P->dwSize = sizeof(TRasConn);
		DWORD dwBufferSize = sizeof(TRasConn);
		DWORD dwEntries = 0;
		DWORD dwRc = ::RasEnumConnections(*P, &dwBufferSize, &dwEntries);
		if ( dwRc == ERROR_BUFFER_TOO_SMALL )
		{
			P = reinterpret_cast<TRasConn*>(ba.GetBuffer(dwBufferSize));
			P->dwSize = sizeof(TRasConn);
			dwRc = ::RasEnumConnections(*P, &dwBufferSize, &dwEntries);
		}
		if ( dwRc != ERROR_SUCCESS )
		{
			_DIALUP_TRACE(RasEnumConnections, dwRc);
			aRasConn.Invalid();
			return aRasConn;
		}
		else
		{
			loop ( i, dwEntries )
			{
				aRasConn.Add(P[i]);
			}
		}
		return aRasConn;
	}

	/// ハンドルとクラスのマップ
	typedef CMapT<HRASCONN, CDialupManager*> CHndThisMap;

	/**
	 * ハンドルのクリア.
	 *		メンバの m_hRasConnをクリアし、マップから削除します。
	 */
	void m_ClearHandleRasConn(void)
	{
		if ( m_hRasConn != NULL )
		{
			ms_ElementMap().RemoveKey(m_hRasConn);
			m_hRasConn = NULL;
		}
	}

	/**
	 * ハンドルとクラスのマップ参照
	 *	@return マップの参照
	 */
	static CHndThisMap& ms_ElementMap(void)
	{
		static CHndThisMap s_mapHndThis;
		return s_mapHndThis;
	}

	/**
	 * RasDial()のコールバック.
	 *		エラーを処理します。
	 *	@param h			発生したRASのハンドル
	 *	@param uMsg			発生したイベントのタイプ
	 *	@param state		これから入ろうとしている接続状態
	 *	@param dwError		発生したエラーコード
	 *	@param dwExError	発生したエラーの拡張コード)
	 */
	static VOID WINAPI 
	ms_RasDialFunc1(HRASCONN h, UINT uMsg, RASCONNSTATE state, DWORD dwError, DWORD dwExError)
	{
		if ( dwError != 0 )
		{
			TRACE3("RasDialFunc s=%d e=%u[%s]\n", state, dwError, GetErrorString(dwError));
			CDialupManager* P;
			if ( ms_ElementMap().Lookup(h, P) != 0 )
			{
				//発見
				if ( (reinterpret_cast<ULONG_PTR>(P) & 0x80000000) == 0 )
				{
					if ( P->m_piListener != NULL )
					{
						if ( P->m_threadWatch.IsAlive() )
						{
							P->m_piListener->OnDialupError(dwError);
						}
					}
					P->HangUp();
				}
			}
			else
			{
				ms_ElementMap()[h] = reinterpret_cast<CDialupManager*>(static_cast<size_t>(dwError | 0x80000000));
			}
		}
	}

	/** 
	 * スレッド処理本体
	 *	@retval 0（固定）。
	 */
	virtual DWORD Run(void)
	{
		DWORD dwRc;
		RASCONNSTATUS tStatus;
		RASCONNSTATE eNowState = static_cast<RASCONNSTATE>(0xFFFF);
		//
		m_piListener->OnDialupChangeState(RASCS_OpenPort);
		//
		bool boIsFirst = true;
		while ( IsRunnable() )
		{
			::Sleep(E_ConnectCheckInterval);
			dwRc = GetConnectStatus(tStatus);
			if ( dwRc != NULL )
			{
				//エラー処理はms_RasDialFunc1()で行っています。
				if ( dwRc == ERROR_INVALID_HANDLE )
				{
					::Sleep(500);
					continue;
				}
				break;
			}
			if ( tStatus.dwError != 0 )
			{
				//エラー処理はms_RasDialFunc1()で行っています。
				break;
			}
			//
			if ( boIsFirst )
			{
				boIsFirst = false;
				eNowState = tStatus.rasconnstate;
				TRACE1("ChangeState =[%s]\n", StateToString(eNowState));
				m_piListener->OnDialupChangeState(eNowState);
			}
			else if ( eNowState != tStatus.rasconnstate )
			{
				eNowState = tStatus.rasconnstate;
				TRACE1("ChangeState =[%s]\n", StateToString(eNowState));
				m_piListener->OnDialupChangeState(eNowState);
			}
			//
			if ( eNowState == RASCS_Connected )
			{
				while ( IsRunnable() )
				{
					dwRc = GetConnectStatus(tStatus);
					if ( dwRc != NULL || tStatus.rasconnstate == RASCS_Disconnected )
					{
						//切断された
						TRACE1("ChangeState =[%s]\n", StateToString(tStatus.rasconnstate));
						m_ClearHandleRasConn();
						m_piListener->OnDialupChangeState(RASCS_Disconnected);
						break;
					}
					::Sleep(E_DisconnectCheckInterval);
				}
				break;
			}
			if ( eNowState == RASCS_Disconnected )
			{
				m_ClearHandleRasConn();
				break;
			}
		}
		return 0;
	}

	/**
	 * 文字列サイズ０のポインタはNULLにする
	 *	@param[in,out] _lpszPhoneBook 電話帳名のポインタ
	 */
	static void ms_CheckPhoneBook(LPCTSTR& _lpszPhoneBook)
	{
		if ( _lpszPhoneBook != NULL && _lpszPhoneBook[0] == '\0' )
		{
			_lpszPhoneBook = NULL;
		}
	}

	#ifndef _WIN32_WCE
		static LPCTSTR NcStr(LPCTSTR lpsz) { return lpsz; } 
	#else
		class NcStr
		{
			CStr m_str;
			bool m_isNull;
		public:
			NcStr(LPCTSTR lpsz) : m_str(lpsz), m_isNull(lpsz == NULL) {}
			operator LPTSTR(void) { return m_isNull ? NULL : m_str.GetBuffer(256); }
		};
	#endif //_WIN32_WCE

public:

	/**@ingroup RAS
	 * ダイアルパラメータ型
	 *
	 *		ダイアルするための情報を管理してます
	 */
	struct TDialParams
	{
		CStr strPhoneBook;		///< 電話帳
		CStr strEntryName;		///< エントリ名
		CStr strUserName;		///< ユーザ名
		CStr strPassword;		///< パスワード
		CStr strPhoneNumber;	///< 電話番号
		CStr strDeviceName;		///< デバイス名
		#ifndef _WIN32_WCE
		/**
		 * 電話帳名取得
		 *	@return 電話帳。未設定ならNULL。
		 */
		LPCTSTR GetPhoneBook(void) const
		{
			if ( strPhoneBook.IsEmpty() )
			{
				return NULL;
			}
			return strPhoneBook;
		}
		#else
		LPWSTR GetPhoneBook(void) const { return NULL; }
		#endif
		/**
		 *	本情報は有効か否か
		 *	@retval true	有効。
		 *	@retval false	無効。
		 */
		bool IsInvalid(void) const
		{
			return strEntryName.IsEmpty();
		}
	};

	/**
	 * コンストラクタ 
	 */
	CDialupManager(void) : m_piListener(NULL), m_hRasConn(NULL)
	{
		m_threadWatch.SetRunner(this);
	}

	/**
	 * デストラクタ 
	 *	@note アタッチしていても HangUp() をコールしない限り自動的に切断することはありません。
	 */
	virtual ~CDialupManager(void)
	{
		m_ClearHandleRasConn();
	}

	/**
	 * [確認] アタッチしているか.
	 *	@note Dialした場合もアタッチ常態になります。
	 *	@retval true アタッチしている
	 *	@retval false アタッチしていない
	 */
	bool IsAttach(void) const
	{
		return m_hRasConn != NULL;
	}

	/**
	 * [設定] アタッチ.
	 *		アタッチ後、接続を監視します。
	 *	@note 指定のエントリ名が接続されているかのチェックにもなります。
	 *	@param lpszPhoneBook 電話帳名。NULL指定可能。
	 *	@param lpszEntryName エントリ名。
	 *	@param piDialupListener リスナー。NULLを指定すると切断を監視しません。
	 *	@retval true	アタッチ成功
	 *	@retval false	失敗（コネクトされていない/本インスタンスが処理中/他のプロセスで接続中）
	 */
	bool Attach(LPCTSTR lpszPhoneBook, LPCTSTR lpszEntryName, IListener* piDialupListener = NULL)
	{
		EXCLUSIVE(&m_syncFunc);
		ms_CheckPhoneBook(lpszPhoneBook);
		Detach();
		HRASCONN hRasConn = GetRasConn(lpszPhoneBook, lpszEntryName);
		if ( hRasConn != NULL )
		{
			//発見
			m_hRasConn = hRasConn;
			ms_ElementMap()[m_hRasConn] = this;
			m_piListener = piDialupListener;
			if ( piDialupListener != NULL )
			{
				#ifdef _DEBUG
					m_threadWatch.Start(CStr(lpszEntryName) + _T("Dialup"));
				#else
					m_threadWatch.Start();
				#endif
			}
			return true;
		}
		return false;
	}

	/**
	 * [操作] ダイアル.
	 *		接続後、接続を監視します。
	 *	@note 既につながっている場合、パラメータが異なっていても切断しません。
	 *	@note パラメータのデバイス名は、EnumDeviceNames()で得られた名前か、
	 *			Emptyを指定してください(Emptyの場合Deviceはデフォルトのまま)。
	 *	@param D ダイアルパラメータ
	 *	@param piDialupListener リスナー。NULLを指定すると接続成功するまでリターンしません。また切断を監視しません。
	 *	@retval 0		成功(非同期の場合、後で失敗が通知されることあり)
	 *	@retval 0以外	エラー。 GetErrorString()で文字列化できます。
	 */
	DWORD Dial(const TDialParams& D, IListener* piDialupListener = NULL)
	{
		EXCLUSIVE(&m_syncFunc);
		if ( D.IsInvalid() )
		{
			TRACE0("Dial パラメータ異常\n");
			return ERROR_INVALID_PARAMETER;
		}
		//==アタッチしていたらデタッチしておく
		Detach();
		//==デバイス指定
		DWORD dwSize = 0;
		::RasGetEntryProperties(D.GetPhoneBook(), NcStr(D.strEntryName), NULL, &dwSize, NULL, NULL);
		CByteVector ba;
		LPRASENTRY ptEntry = reinterpret_cast<LPRASENTRY>(ba.GetBuffer(dwSize));
		ptEntry->dwSize = dwSize;
		DWORD dwRc = ::RasGetEntryProperties(D.GetPhoneBook(), NcStr(D.strEntryName), ptEntry, &dwSize, NULL, NULL);
		if ( dwRc != 0 )
		{
			_DIALUP_TRACE(RasGetEntryProperties, dwRc);
			return dwRc;
		}
		if ( ! D.strDeviceName.IsEmpty() )
		{
			if ( D.strDeviceName.Compare(ptEntry->szDeviceName) != 0 )
			{
				STRLIB::Copy(ptEntry->szDeviceName, D.strDeviceName);
				dwRc = ::RasSetEntryProperties(D.GetPhoneBook(), NcStr(D.strEntryName), ptEntry, dwSize, NULL, NULL);
				if ( dwRc != 0 )
				{
					_DIALUP_TRACE(RasSetEntryProperties, dwRc);
					return dwRc;
				}
			}
		}
		//
		RASDIALPARAMS t;
		t.dwSize = sizeof(RASDIALPARAMS);
		STRLIB::Copy(t.szEntryName, D.strEntryName);
		STRLIB::Copy(t.szUserName, D.strUserName);
		STRLIB::Copy(t.szPassword, D.strPassword);
		STRLIB::Copy(t.szPhoneNumber, D.strPhoneNumber);
		t.szCallbackNumber[0] = '\0';
		t.szDomain[0] = '\0';
		//
		HRASCONN hRasConn = NULL;
		if ( piDialupListener == NULL )
		{
			//===同期
			dwRc = ::RasDial(NULL, D.GetPhoneBook(), &t, 0, NULL, &hRasConn);
		}
		else
		{
			//===非同期
			dwRc = ::RasDial(NULL, D.GetPhoneBook(), &t, 1, ms_RasDialFunc1, &hRasConn);
		}
		if ( dwRc != 0 )
		{
			_DIALUP_TRACE(RasDial, dwRc);
			return dwRc;
		}
		//
		dwRc = ::RasSetEntryDialParams(D.GetPhoneBook(), &t, false);
		if ( dwRc != 0 )
		{
			_DIALUP_TRACE(RasSetEntryDialParams, dwRc);
			HangUp();
			return dwRc;
		}
		//
		m_hRasConn = hRasConn;
		//
		CDialupManager* pMan;
		if ( ms_ElementMap().Lookup(hRasConn, pMan) != 0 )
		{
			DWORD dwError = static_cast<DWORD>(reinterpret_cast<size_t>(pMan));
			ASSERTLIB((dwError & 0x80000000) != 0);
			dwError &= 0x7FFFFFFF;
			_DIALUP_TRACE(RasDial, dwError);
			HangUp();
			return dwError;
		}
		//
		ms_ElementMap()[hRasConn] = this;
		if ( piDialupListener != NULL )
		{
			m_piListener = piDialupListener;
			#ifdef _DEBUG
				m_threadWatch.Start(D.strEntryName + _T("Dialup"));
			#else
				m_threadWatch.Start();
			#endif
		}
		return 0;
	}

	/**
	 * [設定] デタッチ.
	 *	@note Attach/Dialしていないときコールしても差し支えありません（何もしません）
	 */
	void Detach(void)
	{
		EXCLUSIVE(&m_syncFunc);
		if ( m_hRasConn == NULL )
		{
			return;
		}
		m_threadWatch.Stop();
		m_piListener = NULL;
		m_ClearHandleRasConn();
		return;
	}

	/**
	 * [操作] 切断.
	 *		本インスタンスで、Dial か Attachした接続を、切断します。
	 *	@retval 0		成功。(接続していない場合も成功になります)
	 *	@retval 0以外	エラー。 GetErrorString()で文字列化できます。
	 */
	DWORD HangUp(void)
	{
		EXCLUSIVE(&m_syncFunc);
		if ( m_hRasConn == NULL )
		{
			m_piListener = NULL;
			return 0;
		}
		//
		m_threadWatch.Stop();
		DWORD dwRc = ::RasHangUp(m_hRasConn);
		_DIALUP_TRACE(RasHangUp, dwRc);
		//
		RASCONNSTATUS tStatus;
		while ( true )
		{
			if ( GetConnectStatus(tStatus) == ERROR_INVALID_HANDLE )
			{
				break;
			}
			::Sleep(10);
		}
		if ( dwRc == ERROR_NO_CONNECTION )
		{
			dwRc = 0;
		}
		m_piListener = NULL;
		m_ClearHandleRasConn();
		return dwRc;
	}

	/**
	 * [取得] 接続状態取得
	 *	@param[out]	_tStatus 状態
	 *	@retval 0		成功
	 *	@retval 0以外	エラー。 GetErrorString()で文字列化できます。
	 */
	DWORD GetConnectStatus(RASCONNSTATUS& _tStatus) const
	{
		_tStatus.dwSize = sizeof(RASCONNSTATUS);
		DWORD dwRc = ::RasGetConnectStatus(m_hRasConn, &_tStatus);
		_DIALUP_TRACE(RasGetConnectStatus, dwRc);
		return dwRc;
	}

	/**
	 * [取得] デバイス名取得.
	 *		本インスタンスで扱っている接続のデバイス名が得られます。
	 *	@return デバイス名。アタッチ(接続)していない場合Emptyが返ります。
	 */
	CStr GetConnectedDeviceName(void) const
	{
		CStr str;
		RASCONNSTATUS tStatus;
		DWORD dwRc = GetConnectStatus(tStatus);
		if ( dwRc == 0 )
		{
			str = tStatus.szDeviceName;
		}
		return str;
	}

	/**
	 * [取得] IPアドレス取得.
	 *		本インスタンスで扱っている接続のIPアドレスが得られます。
	 *	@return IPアドレス。アタッチ(接続)していない場合Emptyが返ります。
	 */
	CStr GetIpAddress(void) const
	{
		return GetIpAddress(m_hRasConn);
	}

	/**
	 * [取得] RASハンドル取得.
	 *		本インスタンスで扱っている接続のRASハンドルが得られます。
	 *	@return ハンドル。アタッチ(接続)していない場合NULLになります。
	 */
	operator HRASCONN()
	{
		return m_hRasConn;
	}
	
	/**
	 * [参照] 監視スレッド状態参照.
	 *	@retval スレッド状態の参照.
	 */
	CThreadStatus& ReferThreadStatus(void)
	{
		return m_threadWatch;
	}


	//============================================================================


	/**
	 * [取得] 接続のIPアドレス取得
	 *	@param hRasConn RASのハンドル
	 *	@return IPアドレス。未接続ならEmptyが返ります。
	 */
	static CStr GetIpAddress(HRASCONN hRasConn)
	{
		RASPPPIP tRasPppIp = { sizeof(RASPPPIP) };
		DWORD dwLen = sizeof(RASPPPIP);
		DWORD dwRc = ::RasGetProjectionInfo(hRasConn, RASP_PppIp, &tRasPppIp, &dwLen);
		_DIALUP_TRACE(RasGetProjectionInfo, dwRc);
		if ( dwRc == 0 )
		{	
			return tRasPppIp.szIpAddress;
		}
		return _T("");
	}

	/**
	 * [確認] 指定Entry接続完了状態チェック
	 *	@param lpszPhoneBook 電話帳名。NULL指定可能。
	 *	@param lpszEntryname エントリ名。
	 *	@return RASのハンドル。NULLなら未接続(接続処理中も含む)
	 */
	static HRASCONN GetRasConn(LPCTSTR lpszPhoneBook, LPCTSTR lpszEntryname)
	{
		CStr strPb = lpszPhoneBook;
		CRasConnVector aRass = ms_GetConnectingEnties();
		loop ( i, aRass.GetSize() )
		{
			const TRasConn &T = aRass.At(i);
			if ( STRLIB::Compare(T.szEntryName, lpszEntryname) == 0 )
			{
				if ( T.IsPhoneBookEqual(strPb) )
				{
					//発見
					HRASCONN hRasComm = T.hRasConn;
					RASCONNSTATUS tStatus = { sizeof(RASCONNSTATUS) };
					DWORD dwRc = ::RasGetConnectStatus(hRasComm, &tStatus);
					_DIALUP_TRACE(RasGetConnectStatus, dwRc);
					if ( dwRc != 0 || tStatus.rasconnstate != RASCS_Connected )
					{
						return NULL;
					}
					return hRasComm;
				}
			}
		}
		return NULL;
	}

	/**
	 * [取得] エントリ名一覧
	 *	@param	lpszPhoneBook	電話帳名。NULL指定可能。
	 *	@return エントリ名一覧。無効ならAPIエラー発生。
	 */
	static CStrVector EnumEntryNames(LPCTSTR lpszPhoneBook)
	{
		ms_CheckPhoneBook(lpszPhoneBook);
		CStrVector vstrNames;
		CByteVector ba;
		LPRASENTRYNAME P = reinterpret_cast<LPRASENTRYNAME>(ba.GetBuffer(sizeof(RASENTRYNAME)));
		P->dwSize = sizeof(RASENTRYNAME);
		DWORD dwBufferSize = sizeof(RASENTRYNAME);
		DWORD dwEntries = 0;
		DWORD dwRc = ::RasEnumEntries(NULL, NcStr(lpszPhoneBook), P, &dwBufferSize, &dwEntries);
		if ( dwRc == ERROR_BUFFER_TOO_SMALL )
		{
			P = reinterpret_cast<LPRASENTRYNAME>(ba.GetBuffer(dwBufferSize));
			P->dwSize = sizeof(RASENTRYNAME);
			dwRc = ::RasEnumEntries(NULL, NcStr(lpszPhoneBook), P, &dwBufferSize, &dwEntries);
		}
		if ( dwRc != ERROR_SUCCESS )
		{
			_DIALUP_TRACE(RasEnumEntries, dwRc);
			vstrNames.Invalid();
			return vstrNames;
		}
		else
		{
			vstrNames.SetIncrementSize(dwEntries);
			loop ( i, dwEntries )
			{
				vstrNames.Add(P->szEntryName);
				P++;
			}
		}
		return vstrNames;
	}

	/**
	 * [取得] 接続中エントリ名一覧
	 *	@return エントリ名一覧。無効ならAPIエラー発生。
	 */
	static CStrVector EnumConnectedEntryNames(void)
	{
		CStrVector vstrEntryNames;
		//
		CRasConnVector aRass = ms_GetConnectingEnties();
		if ( ! aRass.IsValid() )
		{
			vstrEntryNames.Invalid();
			return vstrEntryNames;
		}
		size_t iLen = aRass.GetSize();
		vstrEntryNames.SetIncrementSize(iLen);
		loop ( i, iLen )
		{
			vstrEntryNames.Add(aRass.At(i).szEntryName);
		}
		//
		return vstrEntryNames;
	}

	/**
	 * [取得] エントリのダイアルパラメータ取得
	 *	@param lpszPhoneBook 電話帳名。NULL指定可能。
	 *	@param lpszEntryName エントリ名。
	 *	@return パラメータ。エラーの場合、無効なパラメータが返ります。
	 */
	static TDialParams GetDialParams(LPCTSTR lpszPhoneBook, LPCTSTR lpszEntryName)
	{
		ms_CheckPhoneBook(lpszPhoneBook);
		TDialParams tRc;
		//
		RASDIALPARAMS tParams = { sizeof(RASDIALPARAMS) };
		STRLIB::Copy(tParams.szEntryName, lpszEntryName);
		//
		BOOL boHasPassword = TRUE;
		DWORD dwParamsRc = ::RasGetEntryDialParams(NcStr(lpszPhoneBook), &tParams, &boHasPassword);
		if ( dwParamsRc != 0 )
		{
			_DIALUP_TRACE(RasGetEntryDialParams, dwParamsRc);
			return tRc;
		}
		//
		DWORD dwSize = 0;
		::RasGetEntryProperties(NcStr(lpszPhoneBook), NcStr(lpszEntryName), NULL, &dwSize, NULL, NULL);
		CByteVector ba;
		LPRASENTRY ptEntry = reinterpret_cast<LPRASENTRY>(ba.GetBuffer(dwSize));
		ptEntry->dwSize = dwSize;
		DWORD dwPropertyRc = ::RasGetEntryProperties(NcStr(lpszPhoneBook), NcStr(lpszEntryName), ptEntry, &dwSize, NULL, NULL);
		if ( dwPropertyRc != 0 )
		{
			_DIALUP_TRACE(RasGetEntryProperties, dwPropertyRc);
			return tRc;
		}
		//
		tRc.strEntryName	= lpszEntryName;
		tRc.strUserName		= tParams.szUserName;
		if ( boHasPassword )
		{
			tRc.strPassword	= tParams.szPassword;
		}
		tRc.strPhoneNumber	= ptEntry->szLocalPhoneNumber;
		tRc.strDeviceName	= ptEntry->szDeviceName;
		tRc.strPhoneBook	= lpszPhoneBook;
		//
		return tRc;
	}

	/**
	 * [取得] 接続されているデバイス名一覧
	 *	@note	一覧作成にRAS-APIを使用しています。
	 *	@param boIsModemOnly trueならMODEMのみ。省略するとRAS管理のデバイスすべて。
	 *	@return デバイスの名前の一覧。無効ならAPIエラー発生。
	 */
	static CStrVector EnumDeviceNames(bool boIsModemOnly = true)
	{
		CStrVector vstrNames;
		CByteVector ba;
		LPRASDEVINFO P = reinterpret_cast<LPRASDEVINFO>(ba.GetBuffer(sizeof(RASDEVINFO)));
		P->dwSize = sizeof(RASDEVINFO);
		DWORD dwBufferSize = sizeof(RASDEVINFO);
		DWORD dwEntries = 0;
		DWORD dwRc = ::RasEnumDevices(P, &dwBufferSize, &dwEntries);
		if ( dwRc == ERROR_BUFFER_TOO_SMALL )
		{
			P = reinterpret_cast<LPRASDEVINFO>(ba.GetBuffer(dwBufferSize));
			P->dwSize = sizeof(RASDEVINFO);
			dwRc = ::RasEnumDevices(P, &dwBufferSize, &dwEntries);
		}
		if ( dwRc != ERROR_SUCCESS)
		{
			_DIALUP_TRACE(RasEnumDevices, dwRc);
			vstrNames.Invalid();
			return vstrNames;
		}
		else
		{
			vstrNames.SetIncrementSize(dwEntries);
			loop ( i, dwEntries )
			{
				if ( ! boIsModemOnly || STRLIB::Compare(P->szDeviceType, RASDT_Modem) == 0)
				{
					vstrNames.Add(P->szDeviceName);
				}
				P++;
			}
		}
		return vstrNames;
	}

	#ifndef _WIN32_WCE
	/**
	 * [取得] デバイス(モデム)名一覧.
	 *			COMxに対するデバイス名が得られます。
	 *	@note	CStrVector のIndex 1が「COM1」のデバイス名、Index 2が「COM2」のデバイス名となります。
	 *			Emptyならデバイス無しです。
	 *	@note	一覧作成にレジストリを参照しています。
	 *	@param[out] _vstrModemNames Sizeは257になっており、COMポートがIndexで、それぞれに存在すればモデム名が格納される。
	 *	@return 発見数(配列数ではない)
	 */
	static int EnumModemPorts(CStrVector& _vstrModemNames)
	{
		OSVERSIONINFO tOsVerInfo = { sizeof(OSVERSIONINFO) };
		::GetVersionEx(&tOsVerInfo);
		//ＯＳにより取得するレジストリパスが違う
		CStr strModemBase;
		if ( tOsVerInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
		{
			//Windows95/98系
			strModemBase = "SYSTEM\\CurrentControlSet\\Services\\Class\\Modem";
		}
		else
		{
			//WindowsNT系
			strModemBase = "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E96D-E325-11CE-BFC1-08002BE10318}";
		}
		//
		int iFoundItemNumber = 0;
		_vstrModemNames.RemoveAll();
		_vstrModemNames.SetSize(257);
//		CRegistryAccessor regbase(CRegistryAccessor(HKEY_LOCAL_MACHINE)[strModemBase]);
		CRegistryAccessor regbase(CRegistryAccessor(HKEY_LOCAL_MACHINE).GetHandle(strModemBase));
		CStrVector strSecNames = regbase.EnumSectionNames();
		loop ( i, strSecNames.GetSize() )
		{
			CRegistryAccessor::CSection reg =regbase[strSecNames[i]];
			//モデム名取得		//"FriendlyName"のキーだとデバイスマネージャと同じ表記
			CStr strModemName = reg.QueryString(_T("Model"));
			if ( strModemName.IsEmpty() )
			{
				continue;
			}
			//COMポートを取得する
			CStr strPort = reg.QueryString(_T("AttachedTo"));
			if ( strPort.GetLength() < 4 )
			{
				continue;
			}
			int iPort = strPort.ToInt(3);
			if ( iPort < 0 || iPort > 256 )
			{
				continue;
			}
			_vstrModemNames.Set(iPort, strModemName);
			iFoundItemNumber++;
		}
		return iFoundItemNumber;
	}

	/**
	 * [取得] デバイス初期化コマンド設定.
	 *			指定デバイス(モデム)の「追加の初期化コマンド」を設定します。
	 *	@note	レジストリを操作しています。
	 *	@param lpszModemName デバイス(モデム)名。
	 *	@param lpszCommand 追加の初期化コマンド。NULLを指定すると、クリアします。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	static int SetUserInitCommand(LPCTSTR lpszModemName, LPCTSTR lpszCommand)
	{
		OSVERSIONINFO tOsVerInfo = { sizeof(OSVERSIONINFO) };
		::GetVersionEx(&tOsVerInfo);
		//ＯＳにより取得するレジストリパスが違う
		CStr strModemBase;
		if ( tOsVerInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
		{
			//Windows95/98系
			strModemBase = "SYSTEM\\CurrentControlSet\\Services\\Class\\Modem";
		}
		else
		{
			//WindowsNT系
			strModemBase = "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E96D-E325-11CE-BFC1-08002BE10318}";
		}
		//
		CRegistryAccessor regbase(CRegistryAccessor(HKEY_LOCAL_MACHINE).GetHandle(strModemBase));
		CStrVector strSecNames = regbase.EnumSectionNames();
		loop ( i, strSecNames.GetSize() )
		{
			CRegistryAccessor::CSection reg =regbase[strSecNames[i]];
			//モデム名取得		//"FriendlyName"のキーだとデバイスマネージャと同じ表記
			CStr strModemName = reg.QueryString(_T("Model"));
			if ( strModemName.IsEqual(lpszModemName) )
			{
				bool r;
				if ( lpszCommand == NULL || lpszCommand[0] == 0 )
				{
					r = reg.DeleteKey(_T("UserInit"));
				}
				else
				{
					r = reg.WriteString(_T("UserInit"), lpszCommand);
				}
				return r;
			}
		}
		return false;
	}
	#endif //_WIN32_WCE

	/**
	 * [作成] 電話エントリ作成
	 *	@param lpszPhoneBook	電話帳名。NULL指定可能。
	 *	@param lpszEntryName	エントリ名。
	 *	@param lpszDeviceName	デバイス名。
	 *	@param lpszAreaCode		エリアコード（NULL指定可能）
	 *	@param lpszPhoneNumber	電話番号。
	 *	@param lpszUserName		ユーザ名。
	 *	@param lpszPassword		パスワード。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	static bool CreatePhoneBookEntry(
				LPCTSTR lpszPhoneBook,
				LPCTSTR lpszEntryName,
				LPCTSTR lpszDeviceName,
				LPCTSTR lpszAreaCode,
				LPCTSTR lpszPhoneNumber,
				LPCTSTR lpszUserName,
				LPCTSTR lpszPassword )
	{
		if ( lpszPhoneBook != NULL && lpszPhoneBook[0] == '\0' )
		{
			lpszPhoneBook = NULL;
		}
		// RASENTRY 構造体のサイズ
		RASENTRY rasEntry = { sizeof(RASENTRY) };
		rasEntry.dwfOptions =
				// DNSを使用する
				RASEO_SpecificNameServers
				//　リモートデフォルトゲートウェイ
				| RASEO_RemoteDefaultGateway;
		//
		CStr strAreaCode = lpszAreaCode;
		if ( ! strAreaCode.IsEmpty() )
		{
			// 市外ダイアルを使用する
			rasEntry.dwfOptions |= RASEO_UseCountryAndAreaCodes;
		}
		rasEntry.dwfOptions = 0x38010118;	//なんでしたっけ？
		//
		rasEntry.dwCountryID			= 81;
		rasEntry.dwCountryCode			= 81;
		rasEntry.dwfNetProtocols		= RASNP_Ip;
		rasEntry.dwFramingProtocol		= RASFP_Ppp;
		_tcsncpy(rasEntry.szAreaCode, strAreaCode, sizeof(rasEntry.szAreaCode) - 1);
		_tcsncpy(rasEntry.szLocalPhoneNumber, lpszPhoneNumber, sizeof(rasEntry.szLocalPhoneNumber) - 1);
		_tcsncpy(rasEntry.szDeviceType, RASDT_Modem, sizeof(rasEntry.szDeviceType) - 1);
		_tcsncpy(rasEntry.szDeviceName, lpszDeviceName, sizeof(rasEntry.szDeviceName) - 1);
		DWORD dwError;
		dwError = ::RasSetEntryProperties (NcStr(lpszPhoneBook), NcStr(lpszEntryName), &rasEntry, sizeof(RASENTRY), NULL, NULL);
		if ( dwError != 0 )
		{
			// エラー処理
			return false;
		}
		RASDIALPARAMS	dialParm = {sizeof(RASDIALPARAMS)};
		dialParm.dwSize = sizeof (RASDIALPARAMS);
		_tcsncpy(dialParm.szEntryName, lpszEntryName, sizeof(dialParm.szEntryName) - 1);
		_tcsncpy(dialParm.szUserName, lpszUserName, sizeof(dialParm.szUserName) - 1);
		_tcsncpy(dialParm.szPassword, lpszPassword, sizeof(dialParm.szPassword) - 1);
		//
		dwError = ::RasSetEntryDialParams(NcStr(lpszPhoneBook), &dialParm, FALSE);
		if ( dwError != 0 )
		{
			// エラー処理
			return false;
		}
		return true;
	}

	/**
	 * [変換] エラーコードを文字列に変換
	 *	@param dwError エラーコード
	 *	@return 文字列
	 */
	static CStr GetErrorString(DWORD dwError)
	{
		CStr str;
		#ifndef _WIN32_WCE
			::RasGetErrorString(dwError, str.GetBuffer(256), 256 * sizeof(TCHAR));
			str.ReleaseBuffer();
		#else
			if ( dwError >= 600 && dwError <= 752 )
			{
				str.Format(_T("RASエラー(%d)"), dwError);
			}
		#endif
		if ( str.IsEmpty() )
		{
			str = SystemErrorToMessageText(dwError);
		}
		str.Remove('\n');
		str.Remove('\r');
		return str;
	}

	/**
	 * [変換] ステータスを文字列に変換
	 *	@param eState ステータスコード
	 *	@return 文字列
	 */
	static CStr StateToString(RASCONNSTATE eState)
	{
		const CStaticMapT<RASCONNSTATE, LPCTSTR> map[] =
		{
			{	RASCS_OpenPort				,_T("ポートを開いています")			},
			{	RASCS_PortOpened			,_T("ポートを開きました")			},
			{	RASCS_ConnectDevice			,_T("機器を接続しています")			},
			{	RASCS_DeviceConnected		,_T("機器を接続しました")			},
			{	RASCS_AllDevicesConnected	,_T("すべての機器を接続しました")	},
			{	RASCS_Authenticate			,_T("認証中です")					},
			{	RASCS_AuthNotify			,_T("認証を終了しました")			},
			{	RASCS_AuthRetry				,_T("認証をリトライ中です")			},
			{	RASCS_AuthCallback			,_T("認証をコールバック中です")		},
			{	RASCS_AuthChangePassword	,_T("認証パスワードを変更しています")},
			{	RASCS_AuthProject			,_T("投影の認証中です")				},
			{	RASCS_AuthLinkSpeed			,_T("接続速度を認証中です")			},
			{	RASCS_AuthAck				,_T("ACK認証中です")				},
			{	RASCS_ReAuthenticate		,_T("再認証中です")					},
			{	RASCS_Authenticated			,_T("認証が終了しました")			},
			{	RASCS_PrepareForCallback	,_T("コールバック準備中です")		},
			{	RASCS_WaitForModemReset		,_T("モデムのリセット待ちです")		},
			{	RASCS_WaitForCallback		,_T("コールバックの待機中です")		},
			{	RASCS_Projected				,_T("投影終了しました")				},
		#ifndef _WIN32_WCE
		#if ( WINVER >= 0x400 )
			{	RASCS_StartAuthentication	,_T("認証を開始します")				},
			{	RASCS_CallbackComplete		,_T("コールバックが完了しました")	},
			{	RASCS_LogonNetwork			,_T("ネットワークにログオンしました")},
		#endif
			{	RASCS_SubEntryConnected		,_T("SubEntryConnected")			},
 			{	RASCS_SubEntryDisconnected	,_T("SubEntryDisconnected")			},
			{	RASCS_Interactive			,_T("Interactive")					},
			{	RASCS_RetryAuthentication	,_T("RetryAuthentication")			},
			{	RASCS_CallbackSetByCaller	,_T("CallbackSetByCaller")			},
			{	RASCS_PasswordExpired		,_T("PasswordExpired")				},
		#if ( WINVER >= 0x500 )
			{	RASCS_InvokeEapUI			,_T("InvokeEapUI")					},
		#endif
		#endif //_WIN32_WCE
			{	RASCS_Connected				,_T("接続完了")						},
			{	RASCS_Disconnected			,_T("切断完了")						},
		};
		LPCTSTR P = CStaticMapT<RASCONNSTATE, LPCTSTR>::Find(eState, map);
		return (P != NULL) ? CStr(P) : CStr::Fmt(_T("Unknown State Code [%d]"), eState);
	}
};



#undef _DIALUP_TRACE



}; // TNB



#ifdef _TnbDOXYGEN	//Document作成用シンボル

/**@ingroup RAS
 * RAS状態コード
 */
enum RASCONNSTATE
{
	RASCS_OpenPort				,///< ポートを開いています
	RASCS_PortOpened			,///< ポートを開きました
	RASCS_ConnectDevice			,///< 機器を接続しています
	RASCS_DeviceConnected		,///< 機器を接続しました
	RASCS_AllDevicesConnected	,///< すべての機器を接続しました
	RASCS_Authenticate			,///< 認証中です
	RASCS_AuthNotify			,///< 認証を終了しました
	RASCS_AuthRetry				,///< 認証をリトライ中です
	RASCS_AuthCallback			,///< 認証をコールバック中です
	RASCS_AuthChangePassword	,///< 認証パスワードを変更しています
	RASCS_AuthProject			,///< 投影の認証中です
	RASCS_AuthLinkSpeed			,///< 接続速度を認証中です
	RASCS_AuthAck				,///< ACK認証中です
	RASCS_ReAuthenticate		,///< 再認証中です
	RASCS_Authenticated			,///< 認証が終了しました
	RASCS_PrepareForCallback	,///< コールバック準備中です
	RASCS_WaitForModemReset		,///< モデムのリセット待ちです
	RASCS_WaitForCallback		,///< コールバックの待機中です
	RASCS_Projected				,///< 投影終了しました
	RASCS_StartAuthentication	,///< 認証を開始します
	RASCS_CallbackComplete		,///< コールバックが完了しました
	RASCS_LogonNetwork			,///< ネットワークにログオンしました
	RASCS_SubEntryConnected		,///< SubEntryConnected
 	RASCS_SubEntryDisconnected	,///< SubEntryDisconnected
	RASCS_Interactive			,///< Interactive
	RASCS_RetryAuthentication	,///< RetryAuthentication
	RASCS_CallbackSetByCaller	,///< CallbackSetByCaller
	RASCS_PasswordExpired		,///< PasswordExpired
	RASCS_InvokeEapUI			,///< InvokeEapUI
	RASCS_Connected				,///< 接続完了
	RASCS_Disconnected			,///< 切断完了
};

#endif //_TnbDOXYGEN
