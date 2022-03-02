#pragma once
/**
 *@file
 * ドライブ関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDriveCommander.h"
#include "TnbDump.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * ドライブ情報クラス
 *
 *	@par必要ファイル
 *			TnbDriveInformation.h
 *
 *	@date 07/02/19	新規作成
 *	@date 15/01/21	物理ドライブ番号取得メソッド追加
 */
class CDriveInformation
{
public:

	/// コンストラクタ
	CDriveInformation(void) : m_cLetter(0)
	{
	}

	/**
	 * [設定] デタッチ.
	 *		本インスタンスで管理するドライブをなくします。
	 */
	void Detatch(void)
	{
		m_cLetter = 0;
	}

	/**
	 * [設定] アタッチ.
	 *		本インスタンスで管理するドライブを設定します。
	 *	@param cLetter ドライブレター 'A'～'Z'
	 *	@retval true 成功
	 *	@retval false ない
	 */
	bool Atatch(TCHAR cLetter)
	{
		Detatch();
		if ( ExistDrive(cLetter) )
		{
			m_cLetter = cLetter;
			return true;
		}
		return false;
	}

	/**
	 * [確認] 有効か？.
	 *		本インスタンスで管理をしているか否か、確認します。
	 *	@retval true 管理中。
	 *	@retval false 否。
	 */
	bool IsValid(void) const
	{
		return m_cLetter != 0;
	}

	/**
	 * [確認] 種類確認.
	 *		本インスタンスで管理するドライブの種類を確認します。
	 *	@retval DRIVE_UNKNOWN ドライブの種類を判別できませんでした。 
	 *	@retval DRIVE_NO_ROOT_DIR 指定のルートディレクトリが存在しません。たとえば、パスにボリュームがマウントされていません（未フォーマットや、メディアが挿入されていないなど）。 
	 *	@retval DRIVE_REMOVABLE このディスクは、ドライブから取り出せます。 
	 *	@retval DRIVE_FIXED このディスクは、ドライブから取り出せません。 
	 *	@retval DRIVE_REMOTE このドライブは、リモート（ネットワーク）ドライブです。 
	 *	@retval DRIVE_CDROM このドライブは、CD-ROM ドライブです。 
	 *	@retval DRIVE_RAMDISK このドライブは、RAM ディスクです。 
	 */
	DWORD GetType(void) const
	{
		TCHAR a[] = { m_cLetter, ':', '\\', 0 };
		return ::GetDriveType(a);
	}

	/**
	 * [確認] 物理ドライブ番号取得.
	 *		本インスタンスで管理するドライブの物理ドライブ番号を確認します(一つとは限らない)。
	 *	@param[out] _pdn 物理ドライブ番号
	 *	@retval true 取得成功
	 *	@retval false 失敗
	 */
	bool GetPhysicalDriveNumber(CVectorT<int>& _pdn) const
	{
		TCHAR a[] = { m_cLetter, ':', '\\', 0 };
		return GetPhysicalDriveNumber(_pdn, a);
	}

	/**
	 * [確認] 取り外し可能か？.
	 *		本インスタンスで管理するドライブがリムーバルか確認します。
	 *	@retval true メディア取り外し可能。
	 *	@retval false 不可能。
	 */
	bool IsRemoval(void) const
	{
		switch ( GetType() )
		{
		case DRIVE_REMOVABLE:	// このディスクは、ドライブから取り出せます。 
		case DRIVE_CDROM:	// このドライブは、CD-ROM ドライブです。 
			return true;
		}
		return false;
	}

	/**
	 * [取得] ベンダID 取得.
	 *	@param[out] _id 取得したベンダIDが格納されます。
	 *	@param[in] withRevision true なら版数も含むIDを返します。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool GetVendorId(CStr& _id, bool withRevision = false) const
	{
		CDriveCommander scsi;	
		if ( scsi.OpenLetter(m_cLetter) )
		{
			return scsi.GetVendorId(_id, withRevision);
		}
		return false;
	}

	/**
	 * [作成] SCSIコマンダー作成.
	 *		管理ドライブの SCSIコマンダーを作成します。
	 *	@return コマンダー。無効状態ならエラーです。
	 */
	CDriveCommander CreateCommander(void)
	{
		CDriveCommander scsi;	
		scsi.OpenLetter(m_cLetter);
		return scsi;
	}

	/**
	 * [確認] アクセス可能か？.
	 *		本インスタンスで管理するドライブがアクセス可能か確認します。
	 *	@retval true 有効
	 *	@retval false 無効
	 */
	bool IsReady(void) const
	{
		if ( GetType() == DRIVE_FIXED )
		{
			return true;
		}
		CDriveCommander scsi;	
		if ( scsi.OpenLetter(m_cLetter) )
		{
			scsi.Send_TestUnit();
			return scsi.Send_TestUnit() == 0;
		}
		return false;
	}
	
	/**
	 * [確認] 書込み可能か？.
	 *		本インスタンスで管理するドライブが書込み可能か確認します。
	 *	@retval true メディア取り外し可能。
	 *	@retval false 不可能。
	 */
	bool CanWrite(void) const
	{
		if ( GetType() == DRIVE_FIXED )
		{
			return true;
		}
		CDriveCommander scsi;	
		if ( scsi.OpenLetter(m_cLetter) )
		{
			return scsi.CanWrite(); //ライトプロテクト
		}
		return false;	// 書き込めない
	}

	/**
	 * [処理] フォーマットダイアログ表示
	 *	@note http://support.microsoft.com/kb/173688/ja
	 *	@param hWnd 親となるウィンドウハンドル
	 *	@param hasCheckDlg true なら中止、再試行、無視を問い合わせる、重大エラーのダイアログ ボックスを表示します。\n
	 *						false なら表示しません。
	 *	@retval IDOK 成功
	 *	@retval 0xFFFFFFFFL フォーマットに失敗
	 *	@retval 0xFFFFFFFEL フォーマットは中止された
	 *	@retval 0xFFFFFFFDL ドライブはフォーマットされない	
	 */
	DWORD FormatDlg(HWND hWnd, bool hasCheckDlg = false) const
	{
		DWORD r = 0xFFFFFFFEL;
		if ( IsValid() )
		{
			HMODULE hInst = ::LoadLibraryA("shell32.dll");
			if ( hInst != NULL )
			{
				DWORD (WINAPI *pShFormatDrive)(HWND hWnd, UINT drive, UINT fmdID, UINT options);
				TNB::ForceSet(pShFormatDrive, ::GetProcAddress(hInst, "SHFormatDrive"));
				if ( pShFormatDrive != NULL )
				{
					UINT nowMode = ::SetErrorMode(0);
					if ( hasCheckDlg )
					{
						::SetErrorMode(nowMode | SEM_FAILCRITICALERRORS);
					}
					else
					{
						::SetErrorMode(nowMode & ~SEM_FAILCRITICALERRORS);
					}
					int d = (m_cLetter | 0x20) - 'a';
					r = (pShFormatDrive)(hWnd, d, 0xFFFF, 0x0);
					::SetErrorMode(nowMode);
				}
				::FreeLibrary(hInst);
			}
		}
		return r;
	}

	/**
	 * [確認] ドライブ存在確認.
	 *		指定のドライブが存在するか確認
	 *	@param cLetter ドライブレター 'A'～'Z'
	 *	@retval true 存在
	 *	@retval false ない
	 */
	static bool ExistDrive(TCHAR cLetter)
	{
		int r = (cLetter | 0x20) - 'a';
		if ( r >= 0 && r <= 25 )
		{
			DWORD d = ::GetLogicalDrives();
			return (d & _BIT(r)) != 0;
		}
		return false;
	}

	/**
	 * [取得] ドライブレター一覧.
	 *	@return 存在するドライブレター一覧
	 */
	static CVectorT<TCHAR> EnumDrive(void)
	{
		CVectorT<TCHAR> r;
		DWORD d = ::GetLogicalDrives();
		loop ( i, 26 )
		{
			if ( (d & _BIT(i)) != 0 )
			{
				r.Add(static_cast<TCHAR>(_T('A') + i));
			}
		}
		return r;
	}

	/**
	 * [確認] 物理ドライブ番号取得.
	 *		指定パスの物理ドライブ番号を確認します(一つとは限らない)。
	 *	@param[out] _pdn 物理ドライブ番号
	 *	@param[in] lpszPath パス
	 *	@retval true 取得成功
	 *	@retval false 失敗
	 */
	static bool GetPhysicalDriveNumber(CVectorT<int>& _pdn, LPCTSTR lpszPath)
	{
		_pdn.RemoveAll();
		CStr s1;
		if ( ! ::GetVolumePathName(lpszPath, s1.GetBuffer(1024), 1024) )
		{
			return false;
		}
		s1.ReleaseBuffer();
		CStr vol;
		if ( ! ::GetVolumeNameForVolumeMountPoint(s1, vol.GetBuffer(1024), 1024) )
		{
			return false;
		}
		vol.ReleaseBuffer();
		vol.TrimRight('\\');
		HANDLE h = ::CreateFile(vol, FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		VOLUME_DISK_EXTENTS buf;
		DWORD returnedSize = 0;
		BOOL r = ::DeviceIoControl(
				h, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0, &buf, sizeof(buf), &returnedSize, NULL);
		DWORD le = _GetLastError("DeviceIoControl");
		if ( r && buf.NumberOfDiskExtents == 1 )
		{
			_pdn.Add(buf.Extents[0].DiskNumber);
			::CloseHandle(h);
			return true;
		}
		if ( le != ERROR_MORE_DATA ) 
		{
			::CloseHandle(h);
			return false;
		}
		size_t bs = sizeof(buf.NumberOfDiskExtents) + sizeof(buf.Extents) * buf.NumberOfDiskExtents;
		CWorkMem w(bs);
		VOLUME_DISK_EXTENTS* p = reinterpret_cast<VOLUME_DISK_EXTENTS *>(w.Ref());
		r = ::DeviceIoControl(
				h, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0, p, bs, &returnedSize, NULL);
		le = _GetLastError("DeviceIoControl");
		if ( r )
		{
			loop ( i, p->NumberOfDiskExtents )
			{
				_pdn.Add(p->Extents[i].DiskNumber);
			}
		}
		::CloseHandle(h);
		return !! r;
	}

private:
	TCHAR		m_cLetter;	///< 管理ドライブレター。０なら未管理。
};



}; // TNB
