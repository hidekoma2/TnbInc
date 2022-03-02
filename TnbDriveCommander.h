#pragma once
/**
 *@file
 * ドライブコマンダー関係のヘッダ
 *
 *		SCSIコマンドにてドライブを操作することができます。
 *
 *	@note Windows NT系の SPTI (SCSI PASS THROUGH INTERFACE) を使用しています。そのため、 Win9x系 OSでは動きません。
 *
 *	@note 本機能を使用するには、Windows SDK が必要になります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include <ntddscsi.h>	// SDK
#include <WinIoCtl.h>	// SDK
#include "TnbPointerHandle.h"
#include "TnbHandleHandle.h"
#include "TnbCollection.h"
#include "TnbWriter.h"



//TNB Library
namespace TNB
{



/**@ingroup ETC
 * ドライブコマンダークラス
 *	
 *		IWriter インターフェースを持っています。また、 SCSIコマンドにてデバイスを操作することができます。
 *
 *	@note	指定したメモリを直接使わず、一度内部で確保したメモリを使用しています。
 *			そのため、指定するメモリに制限はありませんが、オーバーヘッドがあります。
 *
 *	@note Windows NT系の SPTI (SCSI PASS THROUGH INTERFACE) を使用しています。そのため、 Win9x系 OSでは動きません。
 *
 *	@note CDB の LUN、タグは無視されます。
 *
 *	@par必要ファイル
 *			TnbDriveCommander.h
 *
 *	@date 07/02/15	新規作成
 *	@date 15/01/21	物理ドライブ番号によるオープン追加。
 *	@date 15/01/28	クラス名変更。 IWriter 実装。
 *	@date 15/06/03	IsOpened() の判定バグ修正。
 */
class CDriveCommander : public IWriter
{
public:
	
	/**@ingroup ETC
	 * SCSI センスデータ型
	 *	@note GetSense() の戻り値で使用されています。
	 */
	struct TSenseData
	{
		BYTE KEY;		///< SCSI Sense Key
		BYTE ASC;		///< SCSI Additional Sense Code
		BYTE ASCQ;		///< SCSI Additional Sense Code Qualifier
		/// [消去] 全情報クリア
		void Clear(void) {
			KEY = ASC = ASCQ = 0;
		}
	};


	//-----


	/// コンストラクタ
	CDriveCommander(void) : m_timeout(2)
	{
		m_sense.Clear();
	}

	/**
	 * [確認] オープン済み？
	 *	@retval true オープン済み
	 *	@retval false 否
	 */
	bool IsOpened(void) const
	{
		return ! m_hhDrive.IsNull();
	}

	/// [操作] クローズ
	virtual void Close(void)
	{
		m_hhDrive.Null();
	}

	/**
	 * [操作] オープン.
	 *	@note 本インスタンスで使用したいドライブを指定します。 
	 *	@param driveNo ドライブ番号
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool OpenDrive(DWORD driveNo)
	{
		Close();
		m_hhDrive = ::CreateFile(CStr::Fmt(_T("\\\\.\\PhysicalDrive%d"), driveNo),
								GENERIC_READ | GENERIC_WRITE,
								FILE_SHARE_READ | FILE_SHARE_WRITE,
								NULL,
								OPEN_EXISTING,
								0, NULL);
		return ! m_hhDrive.IsNull();
	}

	/**
	 * [操作] オープン.
	 *	@note 本インスタンスで使用したいドライブを指定します。 
	 *	@param driveLetter ドライブレター。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool OpenLetter(TCHAR driveLetter)
	{
		Close();
		m_hhDrive = ::CreateFile(CStr::Fmt(_T("\\\\.\\%c:"), driveLetter),
								GENERIC_READ | GENERIC_WRITE,
								FILE_SHARE_READ,
								NULL,
								OPEN_EXISTING,
								0, NULL);
		return ! m_hhDrive.IsNull();
	}

	/**
	 * [取得] センスデータ取得
	 *	@note このメソッドは、 Send～()で 戻り値が２(CheckCodition)の時、使用します。
	 *	@return センスデータ。
	 */
	const TSenseData& GetSense(void) const
	{
		return m_sense;
	}

	/**
	 * [設定] アップデート.
	 *	@retval true 成功
	 *	@retval false 否
	 */
	bool Update()
	{
		DWORD d;
		return !! ::DeviceIoControl(m_hhDrive, IOCTL_DISK_UPDATE_PROPERTIES, NULL, 0, NULL, 0, &d, NULL);
	}

	/**
	 * [設定] Dismount Volume.
	 *	@retval true 成功
	 *	@retval false 否
	 */
	bool DismountVolume()
	{
		DWORD d;
		return !! ::DeviceIoControl(m_hhDrive, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &d, NULL);
	}

	/**
	 * [設定] Lock Volume.
	 *	@retval true 成功
	 *	@retval false 否
	 */
	bool LockVolume(void)
	{
		DWORD d;
		return !! ::DeviceIoControl(m_hhDrive, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &d, NULL);
	}

	/**
	 * [設定] Unlock Volume.
	 *	@retval true 成功
	 *	@retval false 否
	 */
	bool UnlockVolume(void)
	{
		DWORD d;
		return !! ::DeviceIoControl(m_hhDrive, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &d, NULL);
	}

	/**
	 * [送信] CDB送信.
	 *	@param cdb CDB。 LUN エリアは無視されます。
	 *	@retval 0 成功
	 *	@retval 1以上 SCSIステータス。 ２ならCheck Condition( GetSense() でセンス情報が得られます)。８ならBusy。
	 *	@retval -1 失敗。 ::GetLastError() でエラー内容を確認できます。
	 */
	int SendCommand(const ISequenceCollectionT<BYTE>& cdb)
	{
		if ( m_hhDrive.IsNull() ) 
		{
			::SetLastError(ERROR_ACCESS_DENIED);
			return -1; 
		}
		CCdbInfo tmp(0, SCSI_IOCTL_DATA_UNSPECIFIED); //SCSI_IOCTL_DATA_IN
		tmp.SetCdb(cdb, m_timeout);
		return m_SendCdb(tmp);
	}

	/**
	 * [送信] CDB送信(書込みフェーズ付).
	 *	@param cdb CDB。 LUN エリアは無視されます。
	 *	@param data 書込みデータ(CDBに正しくデータサイズ分を指定しておくこと)。
	 *	@retval 0 成功
	 *	@retval 1以上 SCSIステータス。 ２ならCheck Condition( GetSense() でセンス情報が得られます)。８ならBusy。
	 *	@retval -1 失敗。 ::GetLastError() でエラー内容を確認できます。
	 */
	int SendCommandWithWritePhase(
			const ISequenceCollectionT<BYTE>& cdb, const ISequenceCollectionT<BYTE>& data)
	{
		if ( m_hhDrive.IsNull() ) 
		{
			::SetLastError(ERROR_ACCESS_DENIED);
			return -1; 
		}
		size_t l = data.GetSize();
		CCdbInfo tmp(l, static_cast<BYTE>((l > 0) ? SCSI_IOCTL_DATA_OUT : SCSI_IOCTL_DATA_UNSPECIFIED));
		tmp.SetCdb(cdb, m_timeout);
		if ( l > 0 )
		{
			MemCopy(tmp.RefData(), data.ReferBuffer(), l);
		}
		return m_SendCdb(tmp);
	}

	/**
	 * [送信] CDB送信(読込みフェーズ付).
	 *	@param[out] _d 読み込みデータ。
	 *	@param[in] cdb CDB。 LUN エリアは無視されます。
	 *	@param[in] readSize 読み込みデータ長(CDBに正しく読み込みデータサイズを指定しておくこと)。
	 *	@retval 0 成功
	 *	@retval 1以上 SCSIステータス。 ２ならCheck Condition( GetSense() でセンス情報が得られます)。８ならBusy。
	 *	@retval -1 失敗。 ::GetLastError() でエラー内容を確認できます。
	 */
	int SendCommandWithReadPhase(ICollectionT<BYTE>& _d,
					const ISequenceCollectionT<BYTE>& cdb, int readSize)
	{
		if ( m_hhDrive.IsNull() ) 
		{
			::SetLastError(ERROR_ACCESS_DENIED);
			return -1; 
		}
		CCdbInfo tmp(readSize, SCSI_IOCTL_DATA_IN);
		tmp.SetCdb(cdb, m_timeout);
		int r = m_SendCdb(tmp);
		if ( r == 0 )
		{
			_d.CopyElements(readSize, tmp.RefData());
		}
		return r;
	}

	/**
	 * [送信] MODESENSE CDB送信.
	 *	@note Mode Parameter Header の 2byte目が Media ID、3Byte目の最上位Bitが WriteProtect 状態です（1なら WP）。
	 *	@param[out] _d 読み込みデータ。
	 *	@param[in] pc ページコード、および、ページコントロール値。
	 *	@param[in] isWithBd true なら Block Descriptor も読み込みます。 省略か false なら 読み込みません。
	 *	@retval 0 成功
	 *	@retval 1以上 SCSIステータス。 ２ならCheck Condition( GetSense() でセンス情報が得られます)。８ならBusy。
	 *	@retval -1 失敗。 ::GetLastError() でエラー内容を確認できます。
	 */
	int Send_ModeSense(ICollectionT<BYTE>& _d, BYTE pc, bool isWithBd = false)
	{
		BYTE cdb[6] = {0x1A/*ModeSense*/, _BIT(3)/*Block Descriptor 転送なし*/, pc, 0, 4, 0};
		int r = SendCommandWithReadPhase(_d, CArrayAdapterT<BYTE>(cdb), cdb[4]);
		if ( r == 0 && _d.GetSize() >= 4 )
		{
			cdb[1] = static_cast<BYTE>((isWithBd && _d.At(3) == 8) ? 0 : _BIT(3));
			cdb[4] = static_cast<BYTE>(4 + _d.At(3) + _d.At(0));
			r = SendCommandWithReadPhase(_d, CArrayAdapterT<BYTE>(cdb), cdb[4]);
		}
		return r;
	}

	/**
	 * [送信] INQUIRY CDB送信.
	 *	@param[out] _d 読み込みデータ。
	 *	@retval 0 成功
	 *	@retval 1以上 SCSIステータス。 ２ならCheck Condition( GetSense() でセンス情報が得られます)。８ならBusy。
	 *	@retval -1 失敗。 ::GetLastError() でエラー内容を確認できます。
	 */
	int Send_Inquiry(ICollectionT<BYTE>& _d)
	{
		BYTE cdb[6] = {0x12/*Inquiry*/, 0, 0, 0, 8, 0};
		int r = SendCommandWithReadPhase(_d, CArrayAdapterT<BYTE>(cdb), cdb[4]);
		if ( r == 0 )
		{
			cdb[4] = static_cast<BYTE>(5 + _d.At(4));
			r = SendCommandWithReadPhase(_d, CArrayAdapterT<BYTE>(cdb), cdb[4]);
		}
		return r;
	}

	/**
	 * [送信] TEST UNIT CDB送信.
	 *	@retval 0 成功
	 *	@retval 1以上 SCSIステータス。 ２ならCheck Condition( GetSense() でセンス情報が得られます)。８ならBusy。
	 *	@retval -1 失敗。 ::GetLastError() でエラー内容を確認できます。
	 */
	int Send_TestUnit(void)
	{
		BYTE cdb[6] = {0/*TestUnit*/, 0, 0, 0, 0, 0};
		return SendCommand(CArrayAdapterT<BYTE>(cdb));
	}

	/**
	 * [送信] STARTSTOP CDB送信.
	 *	@param b フラグ。 2 なら Eject、0 なら Load します。
	 *	@retval 0 成功
	 *	@retval 1以上 SCSIステータス。 ２ならCheck Condition( GetSense() でセンス情報が得られます)。８ならBusy。
	 *	@retval -1 失敗。 ::GetLastError() でエラー内容を確認できます。
	 */
	int Send_StartStop(BYTE b = 2)
	{
		BYTE cdb[6] = {0x1B/*StartStop*/, 1, 0, 0, b, 0};
		return SendCommand(CArrayAdapterT<BYTE>(cdb));
	}

	/**
	 * [送信] Read Capacity CDB送信.
	 *	@param[out] _blockAddress 成功時、最後のブロックアドレスが格納されます。
	 *	@param[out] _blockLength 成功時、ブロック長が格納されます。
	 *	@retval 0 成功
	 *	@retval 1以上 SCSIステータス。 ２ならCheck Condition( GetSense() でセンス情報が得られます)。８ならBusy。
	 *	@retval -1 失敗。 ::GetLastError() でエラー内容を確認できます。
	 */
	int Send_ReadCapacity(DWORD& _blockAddress, DWORD& _blockLength)
	{
		BYTE cdb[10] = {0x25/*ReadCapacity*/, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		CByteVector vb;
		int r = SendCommandWithReadPhase(vb, CArrayAdapterT<BYTE>(cdb), 8);
		if ( r == 0 )
		{
			BYTE* p = reinterpret_cast<BYTE*>(vb.GetBuffer());
			_blockAddress = p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
			_blockLength = p[4] << 24 | p[5] << 16 | p[6] << 8 | p[7];
		}
		return r;
	}

	/**
	 * [送信] Write(10) CDB送信.
	 *	@param ba ブロックアドレス
	 *	@param d データ。長さは div の倍数である必要があります。
	 *	@param div ブロック長。通常 512 です。
	 *	@retval 0 成功
	 *	@retval 1以上 SCSIステータス。 ２ならCheck Condition( GetSense() でセンス情報が得られます)。８ならBusy。
	 *	@retval -1 失敗。 ::GetLastError() でエラー内容を確認できます。
	 */
	int Send_Write10(DWORD ba, const ISequenceCollectionT<BYTE>& d, DWORD div = 512)
	{
		BYTE cdb[10] = {0x2A/*Write10*/, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		cdb[2] = static_cast<BYTE>(ba >> 24);
		cdb[3] = static_cast<BYTE>(ba >> 16);
		cdb[4] = static_cast<BYTE>(ba >> 8);
		cdb[5] = static_cast<BYTE>(ba);
		DWORD l = d.GetSize() / div;
		cdb[7] = static_cast<BYTE>(l >> 8);
		cdb[8] = static_cast<BYTE>(l);
		int r = SendCommandWithWritePhase(CArrayAdapterT<BYTE>(cdb), d);
		return r;
	}

	/**
	 * [取得] ハンドル取得.
	 *	@return ハンドル
	 */ 
	HANDLE GetSafeHandle(void) const
	{
		return m_hhDrive;
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
		CDriveCommander* p = const_cast<CDriveCommander*>(this);
		CByteVector vb;
		p->Send_TestUnit();	// 空読込み
		if ( p->Send_Inquiry(vb) == 0 && vb.GetSize() >= 36 )
		{
			const char* p = reinterpret_cast<const char*>(vb.ReferBuffer());
			CAscii a1;
			a1.SetFromLeft(&p[8], 8);
			_id = a1;
			a1.SetFromLeft(&p[16], 16);
			_id += a1;
			if ( withRevision )
			{
				a1.SetFromLeft(&p[32], 4);
				_id += a1;
			}
			return true;
		}
		return false;
	}

	/**
	 * [取得] オープン名取得.
	 *		ベンダIDが返されます。
	 *	@return ベンダID
	 */
	virtual LPCTSTR GetOpenName(void) const
	{
		m_vendorId.Empty();
		GetVendorId(m_vendorId);
		return m_vendorId;
	}

	/**
	 * [確認] 読み込み可能か？
	 *	@note TESTUNIT で状態を確認しています。
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	virtual bool CanRead(void) const
	{
		CDriveCommander* p = const_cast<CDriveCommander*>(this);
		return (p->Send_TestUnit()) == 0;
	}

	/**
	 * [確認] 書込み可能か？
	 *	@note MODESENSE で WP のビットを確認しています。
	 *	@retval true 可能
	 *	@retval false 不可能
	 */
	virtual bool CanWrite(void) const
	{
		CDriveCommander* p = const_cast<CDriveCommander*>(this);
		CByteVector vb;
		if ( CanRead() && p->Send_ModeSense(vb, 0x3F) == 0 && vb.GetSize() >= 4 )
		{
			//TNB::DebugDump(vb.GetSize(), vb.ReferBuffer());
			// vb[1]; //Media ID
			// vb[2]; // 3Byte目の最上位Bitが WriteProtect 状態です（1なら WP）。
			return (vb[2] & _BIT(7)) == 0;
		}
		return false;
	}

	/**
	 * [取得] サイズ取得.
	 *	@note IOCTL_DISK_GET_DRIVE_GEOMETRY_EX を使い、サイズを得ています。
	 *	@retval ０以上 サイズ
	 *	@retval マイナス 取得失敗
	 */
	virtual LONGLONG GetSize(void) const 
	{
		CWorkMem w(sizeof(DISK_GEOMETRY_EX) + sizeof(DISK_PARTITION_INFO) + sizeof(DISK_DETECTION_INFO));
		w.Fill(0);
		DWORD returnedBytes = 0;
		BOOL r = ::DeviceIoControl(
				GetSafeHandle(), IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, NULL, 0, w.Ref(), w.GetSize(), &returnedBytes, NULL);
		DISK_GEOMETRY_EX* pGeometry = reinterpret_cast<DISK_GEOMETRY_EX*>(w.Ref());
		//DISK_PARTITION_INFO* pPartition= DiskGeometryGetPartition(pGeometry);
		//DISK_DETECTION_INFO* pDetection = DiskGeometryGetDetect(pGeometry);
		LONGLONG ll = pGeometry->DiskSize.QuadPart;
		return r ? ll : -1;
	}

	/**
	 * [取得] GEOMETRY 取得.
	 *	@note IOCTL_DISK_GET_DRIVE_GEOMETRY_EX を使い、サイズを得ています。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool GetGeometry(DISK_GEOMETRY& _g) const 
	{
		CWorkMem w(sizeof(DISK_GEOMETRY_EX) + sizeof(DISK_PARTITION_INFO) + sizeof(DISK_DETECTION_INFO));
		w.Fill(0);
		DWORD returnedBytes = 0;
		BOOL r = ::DeviceIoControl(
				GetSafeHandle(), IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, NULL, 0, w.Ref(), w.GetSize(), &returnedBytes, NULL);
		DISK_GEOMETRY_EX* pGeometry = reinterpret_cast<DISK_GEOMETRY_EX*>(w.Ref());
		_g = pGeometry->Geometry;
		return !! r;
	}

	/**
	 * [操作] シーク.
	 *		アクセスポインタを移動します。
	 *	@attention 512 bytes 単位での移動をするようにしてください。
	 *	@param llOffset アクセスポインタの移動数。
	 *	@param seek 移動開始アクセスポインタ位置。省略すると TOP になります。
	 *	@retval ０以上	シーク成功。数値は、現在のファイルポインタ
	 *	@retval マイナス シーク失敗
	 */
	virtual LONGLONG Seek(LONGLONG llOffset, ESeekMode seek = TOP) const 
	{
		if ( seek == END )
		{
			llOffset += GetSize();
			seek = TOP;
		}
		LARGE_INTEGER r;
		r.QuadPart = llOffset;
		r.LowPart = ::SetFilePointer(GetSafeHandle(), r.LowPart, &r.HighPart, seek);
		if ( r.LowPart == INVALID_SET_FILE_POINTER )
		{
			if ( ::GetLastError() != NO_ERROR )
			{
				r.QuadPart = -1;
			}
		}
		return r.QuadPart;
	}

	/**
	 * [取得] 読み込み
	 *	@attention 512 bytes 単位でのアクセスをするようにしてください。
	 *	@param[in] size 読み込みサイズ。
	 *	@param[out] _P 読み込みアドレス。 size 分書き込める領域である必要が有ります。
	 *	@return 読み込めたサイズ。
	 *	@throw CReadFailureException 読み込みに失敗すると、スローされます。
	 */
	virtual size_t Read(size_t size, LPVOID _P) const
	{
		DWORD dwRead;
		BOOL boRc = ::ReadFile(GetSafeHandle(), _P, ToDword(size), &dwRead, NULL);
		if ( ! boRc )
		{
			throw CReadFailureException(::GetLastError());
		}
		return dwRead;
	}

	/**
	 * [保存] 書き込み
	 *	@attention 512 bytes 単位でのアクセスをするようにしてください。
	 *	@param size 書き込みサイズ。
	 *	@param P 書き込みデータ。
	 *	@throw CWriteFailureException 書込みに失敗すると、スローされます。
	 */
	virtual void Write(size_t size, LPCVOID P)
	{
		if ( size >= ULONG_MAX )
		{
			throw CWriteFailureException(ERROR_BAD_LENGTH);
		}
		DWORD dwWrite;
		bool boRc = !! ::WriteFile(GetSafeHandle(), P, ToDword(size), &dwWrite, NULL);
		DWORD dwError = boRc ? 0 : ::GetLastError();
		if ( ! boRc || size != dwWrite )
		{
			throw CWriteFailureException(dwError);
		}
	}
	
	/**
	 * [設定] EOFを指定.
	 *		必ず失敗します。。
	 *	@throw CNotSupportException 失敗
	 */
	virtual void SetEnd(void)
	{
		throw CNotSupportException();
	}

private:

	/// IOCTL_SCSI_PASS_THROUGH のパラメータ型
	struct TScsiPassThroughEx
	{
		SCSI_PASS_THROUGH	spt;		///< 基本パラメータ型
		DWORD				boundary;	///< バウンダリ
		BYTE				abSense[32];///< センス情報用
	};

	/// CDB 情報管理クラス
	class CCdbInfo
	{
		CWorkMem	m_work;			///< ワークメモリ
		size_t		m_size;			///< サイズ
	public:
		/// コンストラクタ
		CCdbInfo(size_t size, BYTE bMode) : m_work(0)
		{
			m_size = sizeof(TScsiPassThroughEx) + size;
			m_work.Resize(m_size);
			TScsiPassThroughEx* P = reinterpret_cast<TScsiPassThroughEx*>(m_work.Ref());
			::ZeroMemory(P, m_size);
			P->spt.Length				= sizeof(SCSI_PASS_THROUGH);
//			P->spt.SenseInfoLength		= 18;
			P->spt.SenseInfoLength		= 32;
			P->spt.DataIn				= bMode;
			P->spt.DataTransferLength	= static_cast<ULONG>(size);
			P->spt.TimeOutValue			= 2;
			if ( size != 0 )
			{
				P->spt.DataBufferOffset	= sizeof(TScsiPassThroughEx);
			}
			P->spt.SenseInfoOffset		= offsetof(TScsiPassThroughEx, abSense);
		}
		/// セット
		void SetCdb(const ISequenceCollectionT<BYTE>& cdb, DWORD timeout)
		{
			TScsiPassThroughEx* P = reinterpret_cast<TScsiPassThroughEx*>(m_work.Ref());
			P->spt.CdbLength = static_cast<BYTE>(cdb.GetSize());
			MemCopy(P->spt.Cdb, cdb.ReferBuffer(), cdb.GetSize());
			P->spt.TimeOutValue = timeout;
		}
		/// 参照
		TScsiPassThroughEx* Ref(void)
		{
			return reinterpret_cast<TScsiPassThroughEx*>(m_work.Ref());
		}
		/// データ参照
		BYTE* RefData(void)
		{
			return m_work.Ref() + sizeof(TScsiPassThroughEx);
		}
		/// CDBサイズ
		size_t GetLength(void)
		{
			return m_size;
		}
	};

	/// アダプタクラス
	template<typename TYP>
	class CArrayAdapterT : public ISequenceCollectionT<TYP>
	{
		DEFSUPER(ISequenceCollectionT<TYP>);
		const TYP*			m_pcType;			///< 保持するデータのポインタ
		size_t				m_size;				///< 保持するデータの長さ
	public:
		/// コンストラクタ
		template<typename T> CArrayAdapterT(const T& t)
			: _super(), m_pcType(static_cast<const TYP*>(t)), m_size(sizeof(T) / sizeof(TYP)) {}
		/// 要素数取得
		virtual size_t GetSize(void) const { return m_size; }
		/// データアドレス取得.
		virtual const TYP* ReferBuffer(void) const { return m_pcType; }
	};

	/**
	 * [送信] CDB送信.
	 *	@param cdb CDB Info。
	 *	@retval 0 成功
	 *	@retval 1以上 SCSIステータス。 ２ならCheck Condition( GetSense() でセンス情報が得られます)。８ならBusy。
	 *	@retval -1 失敗。 ::GetLastError() でエラー内容を確認できます。
	 */
	int m_SendCdb(CCdbInfo& cdb)
	{
		DWORD dwResult = 0;
		TScsiPassThroughEx* P = cdb.Ref();
		BOOL r = ::DeviceIoControl(m_hhDrive,
								IOCTL_SCSI_PASS_THROUGH,
								P,
								cdb.GetLength(),
								P,
								cdb.GetLength(),
								&dwResult,
								NULL);
		m_sense.Clear();
		if ( r && P->spt.ScsiStatus != 0 )
		{
			m_sense.KEY = static_cast<BYTE>(P->abSense[2] & 0x0F);
			if ( P->abSense[7] >= 0x0A )
			{
				m_sense.ASC = P->abSense[12];
				m_sense.ASCQ = P->abSense[13];
			}
			return P->spt.ScsiStatus;
		}
		return r ? 0 : -1;
	}

	CHandleHandle	m_hhDrive;		///< ドライブハンドル
	TSenseData		m_sense;		///< Rastセンス情報
	DWORD			m_timeout;		///< タイムアウト時間
	mutable CStr	m_vendorId;		///< ベンダID
};



}; // TNB



#if 0

// メモ
 http://support.microsoft.com/kb/871134/ja
 http://support.microsoft.com/kb/137247/ja

#endif