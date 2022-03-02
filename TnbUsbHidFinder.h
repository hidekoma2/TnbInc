#pragma once
/**
 *@file
 * USB HIDの検索関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbUsbGuidFinder.h"
#include "TnbUsbHidPathName.h"
#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup USB
 * USB-HID検索クラス
 *
 *		接続されている USB-HID の検索、および、通信サイズを取得します。
 *		また、指定VID,PID の検索も可能。
 *
 *	@par必要ファイル
 *			TnbUsbHidFinder.h
 * 
 *	@date 07/04/24 新規作成
 *	@date 08/07/07 抽象化したクラスと分離
 */
class CUsbHidFinder : public CAbstractUsbGuidFinder
{
	DEFSUPER(CAbstractUsbGuidFinder);
	CUsbHidPathName	m_hidName;			///< HID情報 (検索結果)
protected:
	/**
	 * [通知] 発見.
	 *	@note 条件に合うデバイスが見つかった時に通知されます。
	 *	@param lpszDevicePath デバイスパス名。 CreateFile() 等で使用することが出来ます。
	 *	@return エラーコード。
	 */
	virtual DWORD OnFound(LPCTSTR lpszDevicePath)
	{
		return m_hidName.SetPathName(lpszDevicePath);
	}
	/**
	 * [通知] クローズ.
	 *	@note 検索をクローズした時に通知されます。
	 */
	virtual void OnClosed(void)
	{
		m_hidName.Empty();
	}

public:

	/// コンストラクタ
	CUsbHidFinder(void) : m_hidName()
	{
	}

	/**
	 * [検索] 検索開始
	 *	@retval true 発見（クラスメソッドに情報あり）
	 *	@retval false 未発見／エラー発生（ GetLastError() で情報取得可能）
	 */
	bool Find(void)
	{
		GUID g;
		CUsbHidApi().HidD_GetHidGuid(&g);
		return _super::Find(g);
	}

	/**
	 * [検索] 検索開始（Mouse指定）
	 *	@retval true 発見（クラスメソッドに情報あり）
	 *	@retval false 未発見／エラー発生（ GetLastError() で情報取得可能）
	 */
	bool FindMouseHid(void)
	{
		GUID g = { 0x378de44c, 0x56ef, 0x11d1, 0xbc, 0x8c, 0x00, 0xa0, 0xc9, 0x14, 0x05, 0xdd };
		return _super::Find(g);
	}

	/**
	 * [取得] 検索結果情報取得
	 *	@note Find() Next() で true の場合、情報が得られます。
	 *	@return パスネーム情報の参照。
	 *	@throw CEmptyException 未検索、あるいは未発見時に本メソッドをコールすると、スローされます。
	 */
	const CUsbHidPathName& Get(void) const
	{
		if ( ! _super::IsFinding() )
		{
			throw CEmptyException();
		}
		return m_hidName;
	}

	/**
	 * [取得] USB-HID パスネーム情報一覧.
	 *		接続されている USB-HID パスネーム情報一覧を作成します。
	 *	@return 発見したパスネーム情報の配列
	 */
	static CVectorT<CUsbHidPathName> EnumPathNames(void)
	{
		CVectorT<CUsbHidPathName> v;
		CUsbHidFinder fh;
		if ( fh.Find() )
		{
			do
			{
				v.Add(fh.Get());
			} while ( fh.Next() );
		}
		return v;
	}

	/**
	 * [取得] 指定 USB-HID パスネーム情報一覧.
	 *		指定の ID のパスネーム情報を集めます。
	 *	@param iVendorId ベンダID.
	 *	@param iProductId プロダクトID.
	 *	@return 発見したパスネーム情報の配列。
	 */
	static CVectorT<CUsbHidPathName> FindPathNames(int iVendorId, int iProductId)
	{
		CVectorT<CUsbHidPathName> v;
		CUsbHidFinder fh;
		if ( fh.Find() )
		{
			do
			{
				if ( fh.Get().GetVendorId() == iVendorId && fh.Get().GetProductId() == iProductId )
				{
					v.Add(fh.Get());
				}
			} while ( fh.Next() );
		}
		return v;
	}
};



}; // TNB
