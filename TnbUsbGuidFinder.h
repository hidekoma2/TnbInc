#pragma once
/**
 *@file
 * DEVICE の検索関係のヘッダ
 *
 *	@note 本機能を使用するには、Windows SDK が必要になります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"
#include <SetupApi.h>
#pragma comment(lib, "setupapi.lib")



//TNB Library
namespace TNB
{



/**@ingroup USB
 * DEVICE 検索クラス
 *
 *		接続されている DEVICE の検索を行う、抽象クラスです。
 *
 *	@par必要ファイル
 *			TnbUsbGuidFinder.h
 * 
 *	@date 08/07/07 CUsbHidFinder を抽象化して新規作成
 */
class CAbstractUsbGuidFinder : CCopyImpossible
{
	GUID			m_guid;				///< 検索対象 GUID
	HDEVINFO		m_hDevInfo;			///< デバイスインフォハンドル
	DWORD			m_dwIndex;			///< 検索Index
	DWORD			m_dwLastError;		///< エラーコード (検索結果)

	/**
	 * 指定INDEXのデバイスのCapabilityを取得しクラスメソッドに格納
	 *	@param index インデックス。
	 *	@retval true 成功。
	 *	@retval false 失敗。理由は m_dwLastError に格納されます。 ERROR_NO_MORE_ITEMS(0x103) はこれ以上データなし、という意味。
	 */
	bool m_GetDevInfo(DWORD index)
	{
		ASSERTLIB( m_hDevInfo != NULL );
		SP_DEVICE_INTERFACE_DATA tDevIfData;
		tDevIfData.cbSize = sizeof(tDevIfData);	// サイズを設定
		// デバイス情報セット内の各デバイス インターフェースを列挙
		if ( ! ::SetupDiEnumDeviceInterfaces(
					m_hDevInfo,		// インターフェイス情報を取得するデバイスが含まれているデバイス情報セットへのポインタ
					NULL,			// 未使用
					&m_guid,		// 要求するインターフェイスのデバイスインターフェイスクラスを指定する、1つのGUIDへのポインタ
					index,			// デバイス情報セット内のインターフェイスリストに対して、0で始まるインデックス番号
					&tDevIfData		// 関数が成功した場合に、書き込みの完了したSP_DEVICE_INTERFACE_DATA構造体を保持するバッファへのポインタ
			)
		)
		{
			m_dwLastError = ::GetLastError();
		}
		else
		{
			// 指定されたデバイスインターフェイスに関する詳細情報を返します
			// 詳細データ構造体サイズを取得
			DWORD dwReqSize;
			SP_DEVINFO_DATA tDevInfoData;
			tDevInfoData.cbSize = sizeof(tDevInfoData);
			::SetupDiGetDeviceInterfaceDetail(
					m_hDevInfo,		// インターフェイスとその基になるデバイスが含まれるデバイス情報セットへのポインタ
					&tDevIfData,	// インターフェイスを識別する1つのSP_DEVICE_INTERFACE_DATA構造体へのポインタ
					NULL,			// サイズ取得するため、NULL	（指定されたインターフェイスに関する情報を受け取る1つのSP_DEVICE_INTERFACE_DETAIL_DATA構造体へのポインタ）
					0,				// サイズ取得するため、0	（DeviceInterfaceDetailDataが指すバッファのサイズ）
					&dwReqSize,		// DeviceInterfaceDetailDataが指すバッファが必要とするサイズを受け取る、1つの変数へのポインタ
					&tDevInfoData	// 要求されたインターフェイスを公開しているデバイスに関する情報を受け取る、1つのSP_DEVINFO_DATA構造体へのポインタ
				);
			CWorkMem work(dwReqSize);
			PSP_DEVICE_INTERFACE_DETAIL_DATA pDevIfDetailData
						= reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(work.Ref());
			pDevIfDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			if ( ! ::SetupDiGetDeviceInterfaceDetail(
					m_hDevInfo,			// インターフェイスとその基になるデバイスが含まれるデバイス情報セットへのポインタ
					&tDevIfData,		// インターフェイスを識別する1つのSP_DEVICE_INTERFACE_DATA構造体へのポインタ
					pDevIfDetailData,	// 指定されたインターフェイスに関する情報を受け取る1つのSP_DEVICE_INTERFACE_DETAIL_DATA構造体へのポインタ
					dwReqSize,			// DeviceInterfaceDetailDataが指すバッファのサイズ
					&dwReqSize,			// DeviceInterfaceDetailDataが指すバッファが必要とするサイズを受け取る、1つの変数へのポインタ
					&tDevInfoData		// 要求されたインターフェイスを公開しているデバイスに関する情報を受け取る、1つのSP_DEVINFO_DATA構造体へのポインタ
				)
			)
			{
				m_dwLastError = ::GetLastError();
			}
			else
			{
				m_dwLastError = OnFound(pDevIfDetailData->DevicePath);
			}
		}
		return m_dwLastError == ERROR_SUCCESS;
	}
	
	/**
	 * メンバのハンドルや GUID を使い検索
	 *	@retval true 発見（クラスメソッドに情報あり）
	 *	@retval false 未発見／エラー発生（ GetLastError() で情報取得可能）
	 */
	bool m_Find(void)
	{
		Close();
		// 指定されたクラスに所属するすべてのデバイスが含まれている1つのデバイス情報セットを返します
		m_hDevInfo = ::SetupDiGetClassDevs(
				&m_guid,								// セットアップクラスのクラスGUIDへのポインタ
				NULL,									// システムのすべてのデバイスインスタンスに関するデバイス情報を取得
				0,										// このセットのメンバに関連するすべてのユーザーインターフェイスが利用する、トップレベルウィンドウのハンドルを指定
				(DIGCF_PRESENT | DIGCF_DEVICEINTERFACE)	// 現在存在するデバイス | ClassGuidで指定されたインターフェイスクラスに所属するインターフェイスを公開しているデバイス
		);
		m_dwIndex = 0;
		m_dwLastError = 0;
		if ( m_hDevInfo != NULL )
		{
			m_GetDevInfo(m_dwIndex);
			if ( m_dwLastError != ERROR_NO_MORE_ITEMS )
			{
				m_dwIndex++;
				return true;
			}
			Close();
		}
		else
		{
			m_dwLastError = ::GetLastError();
		}
		return false;
	}
protected:

	/**
	 * [通知] 発見.
	 *	@note 条件に合うデバイスが見つかった時に通知されます。
	 *	@param lpszDevicePath デバイスパス名。 CreateFile() 等で使用することが出来ます。
	 *	@return エラーコード。
	 */
	virtual DWORD OnFound(LPCTSTR lpszDevicePath) = 0;

	/**
	 * [通知] クローズ.
	 *	@note 検索をクローズした時に通知されます。
	 */
	virtual void OnClosed(void) = 0;

public:

	/// コンストラクタ
	CAbstractUsbGuidFinder(void) : m_hDevInfo(NULL), m_dwIndex(0), m_dwLastError(0)
	{
		Zero(m_guid);
	}

	/**
	 * [設定] クローズ.
	 *	@note 検索中の場合、検索を停止します（ Next() メソッドは false を返すようになります）。
	 */
	void Close(void)
	{
		if ( m_hDevInfo != NULL )
		{
			::SetupDiDestroyDeviceInfoList(m_hDevInfo);
			m_hDevInfo = NULL;
			OnClosed();
		}
	}

	/**
	 * [確認] 検索中確認
	 *	@retval true 検索中。情報を持っている。
	 *	@retval false していない。
	 */
	bool IsFinding(void) const
	{
		return m_hDevInfo != NULL;
	}
	
	/**
	 * [検索] 検索開始.
	 *	@param g 検索対象の GUID。
	 *	@retval true 発見（クラスメソッドに情報あり）
	 *	@retval false 未発見／エラー発生（ GetLastError() で情報取得可能）
	 */
	bool Find(const GUID& g)
	{
		m_guid = g;
		return m_Find();
	}

	/**
	 * [検索] 次検索
	 *	@note Find() で true の場合、次にこのメソッドを使用します。その後繰り返して
	 *			本メソッド使うことで複数の情報を得ることが出来ます。
	 *	@retval true 発見（クラスメソッドに情報あり）
	 *	@retval false 未発見／エラー発生（ GetLastError() で情報取得可能）
	 */
	bool Next(void)
	{
		m_GetDevInfo(m_dwIndex);
		if ( m_dwLastError == ERROR_NO_MORE_ITEMS )
		{
			Close();
			return false;
		}
		m_dwIndex++;
		return true;
	}

	/**
	 * [取得] エラー情報取得
	 *	@note Find() Next() で false の場合、本メソッドでエラーか正常終了か判断できます。
	 *	@retval ERROR_SUCCESS		エラー情報なし。
	 *	@retval ERROR_SUCCESS以外 エラー。数値はシステムエラーコードです( SystemErrorToMessageText() で文字列化できます)。
	 */
	DWORD GetLastError(void) const
	{
		return m_dwLastError;
	}
};



/**@ingroup USB
 * DEVICE検索クラス
 *
 *		接続されている DEVICE の検索を行うクラスです。
 *
 *	@par必要ファイル
 *			TnbUsbGuidFinder.h
 * 
 *	@date 08/07/07 CUsbHidFinder を抽象化して新規作成
 */
class CUsbGuidFinder : public CAbstractUsbGuidFinder
{
	DEFSUPER(CAbstractUsbGuidFinder);
	CSimpleStr	m_devicePathName;	///< 発見デバイス名
protected:
	/**
	 * [通知] 発見.
	 *	@note 条件に合うデバイスが見つかった時に通知されます。
	 *	@param lpszDevicePath デバイスパス名。 CreateFile() 等で使用することが出来ます。
	 *	@return エラーコード。
	 */
	virtual DWORD OnFound(LPCTSTR lpszDevicePath)
	{
		m_devicePathName = lpszDevicePath;
		return 0;
	}

	/**
	 * [通知] クローズ.
	 *	@note 検索をクローズした時に通知されます。
	 */
	virtual void OnClosed(void)
	{
		m_devicePathName.Empty();
	}

public:
	/// コンストラクタ
	CUsbGuidFinder(void)
	{
	}

	/**
	 * [取得] 発見デバイスパス名取得.
	 *	@return デバイスパス名
	 */
	LPCTSTR GetDevicePathName(void) const
	{
		return m_devicePathName;
	}
};



}; // TNB
