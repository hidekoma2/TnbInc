#pragma once
/**
 *@file
 * SMART (Self Monitoring, Analysis and Reporting Technology)関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#include "TnbSimpleVector.h"
#include <WinIoCtl.h>



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * SMART (Self Monitoring, Analysis and Reporting Technology)アクセスクラス.
 *
 *	\code
 *		CSmartAccessor sa;
 *		loop ( d, 26 )
 *		{
 *			if ( sa.Collect(d) )
 *			{
 *				TRACE1("Found Disk %d\n", d);
 *				TRACE3("    温度=%d℃, 電源投入回数=%d回, 使用時間=%d時間\n"
 *						, sa.GetTemperature(), sa.GetPowerCycleCount(), sa.GetPowerOnHoursCount() );
 *				loop ( i, sa.GetAttributeCount() )
 *				{
 *					const CSmartAccessor::TAttribute* a = sa.GetAttribute(i);
 *					ASSERT( a != NULL );
 *					TRACE3("    %02X[%-38s] %s\n", a->bAttrID, a->GetAttributeIdString(), a->ToString());
 *				}
 *			}
 *		}
 *	\endcode
 *
 *
 *	@par必要ファイル 
 *			TnbSmartAccessor.h
 *
 *	@date 12/02/14 新規作成
 */
class CSmartAccessor : CCopyImpossible
{
public:
	
	/**@ingroup FILE
	 * SMART バージョン情報
	 */
	struct TVersion
	{
		BYTE	bVersion;		///< Binary driver version.
		BYTE	bRevision;		///< Binary driver revision.
		BYTE	bReserved;		///< Not used.
		BYTE	bIDEDeviceMap;	///< Bit map of IDE devices.
		DWORD	fCapabilities;	///< Bit mask of driver capabilities.
		DWORD	dwReserved[4];	///< For future use.
	};

	/**@ingroup FILE
	 * SMART 属性型
	 *	@note DRIVEATTRIBUTE 構造体がベースです
	 */
	struct TAttribute
	{
		#pragma pack(push,1)
		BYTE	bAttrID;		///< Identifies which attribute
		WORD	wStatusFlags;	///< see bit definitions below
		BYTE	bAttrValue;		///< Current normalized value
		BYTE	bWorstValue;	///< How bad has it ever been?
		WORD	bRawValue[3];	///< Un-normalized value
		BYTE	bReserved;		///< ...
		#pragma	pack(pop)
		/**
		 * [取得] Un-normalized value取得
		 *		bRawValue[3] を 48bit 値として返します。
		 *	@return 値
		 */
		ULONGLONG GetValue(void) const
		{
			ULONGLONG ll = bRawValue[2];
			ll <<= 16;	ll += bRawValue[1];
			ll <<= 16;	ll += bRawValue[0];
			return ll;
		}
		/**
		 * [取得] 文字列化
		 *		現在の値 ワーストの値 生データx6 の文字列を生成します。
		 *	@return 文字列。
		 */
		CStr ToString(void) const
		{
			CStr s;
			s.Format(_T("%03d %03d  %012I64X"), bAttrValue, bWorstValue, GetValue());
			return s;
		}
		/**
		 * [取得] 属性ＩＤ文字列取得
		 *	@param isEnglish true なら英語、 false なら日本語
		 *	@return 文字列
		 */
		CStr GetAttributeIdString(bool isEnglish = true) const
		{
			return GetAttributeIdString(bAttrID, isEnglish);
		}
		/**
		 * [取得] 属性ＩＤ文字列取得
		 *	@param attributeId 属性ＩＤ
		 *	@param isEnglish true なら英語、 false なら日本語
		 *	@return 文字列
		 */
		static CStr GetAttributeIdString(BYTE attributeId, bool isEnglish = true)
		{
			CStr r;
			switch ( attributeId )
			{
			case 0x01:	r = isEnglish ? _T("Raw read error rate") : _T("読み込みエラー率");
				break;
			case 0x02:	r = isEnglish ? _T("Throughput performance") : _T("スループット");
				break;
			case 0x03:	r = isEnglish ? _T("Spinup time") : _T("スピンアップ時間");
				break;
			case 0x04:	r = isEnglish ? _T("Start/Stop count") : _T("回転／停止数");
				break;
			case 0x05:	r = isEnglish ? _T("Reallocated sector count") : _T("再割り当てセクター数");
				break;
			case 0x06:	r = isEnglish ? _T("Read channel margin") : _T("リード・チャンネル・マージン");
				break;
			case 0x07:	r = isEnglish ? _T("Seek error rate") : _T("シークエラー率");
				break;
			case 0x08:	r = isEnglish ? _T("Seek timer performance") : _T("シークタイム性能");
				break;
			case 0x09:	r = isEnglish ? _T("Power-on hours count") : _T("合計利用時間");
				break;
			case 0x0A:	r = isEnglish ? _T("Spinup retry count") : _T("スピンアップ再試行回数");
				break;
			case 0x0B:	r = isEnglish ? _T("Calibration retry count") : _T("キャリブレーション再試行回数");
				break;
			case 0x0C:	r = isEnglish ? _T("Power cycle count") : _T("電源投入回数");
				break;
			case 0x0D:
			case 0xC9:	r = isEnglish ? _T("Soft read error rate") : _T("論理読み込みエラー率");
				break;
			case 0xBB:
			case 0xBD:
			case 0xBE:	r = isEnglish ? _T("vendor-specific") : _T("ベンダー独自情報");
				break;
			case 0xBF:	r = isEnglish ? _T("G-sense error rate") : _T("加速度センサー検出エラー率");
				break;
			case 0xC0:	r = isEnglish ? _T("Power-off retract count") : _T("電源切断回避数");
				break;
			case 0xC1:	r = isEnglish ? _T("Load/Unload cycle count") : _T("ロード／アンロード・サイクル数");
				break;
			case 0xC2:	r = isEnglish ? _T("HDA temperature") : _T("温度");
				break;
			case 0xC3:	r = isEnglish ? _T("Hardware ECC recovered") : _T("ハードウエアECC復旧");
				break;
			case 0xC4:	r = isEnglish ? _T("Reallocation count") : _T("再割り当て数");
				break;
			case 0xC5:	r = isEnglish ? _T("Current pending sector count") : _T("不安定セクター数");
				break;
			case 0xC6:	r = isEnglish ? _T("Offline scan uncorrectable count") : _T("未訂正エラー数");
				break;
			case 0xC7:	r = isEnglish ? _T("UDMA CRC error rate") : _T("UltraDMA CRCエラー");
				break;
			case 0xC8:	r = isEnglish ? _T("Write error rate") : _T("書き込みエラー率");
				break;
			//case	0xC9:
			case 0xCA:	r = isEnglish ? _T("Data Address Mark errors") : _T("DAM(Data Address Mark)エラー");
				break;
			case 0xCB:	r = isEnglish ? _T("Run out cancel") : _T("ECCエラー");
				break;
			case 0xCC:	r = isEnglish ? _T("Soft ECC correction") : _T("論理ECC訂正");
				break;
			case 0xCD:	r = isEnglish ? _T("Thermal asperity rate(TAR)") : _T("熱エラー率（TAR）");
				break;
			case 0xCE:	r = isEnglish ? _T("Flying height") : _T("ヘッド高");
				break;
			case 0xCF:	r = isEnglish ? _T("Spin high current") : _T("最大回転電流");
				break;
			case 0xD0:	r = isEnglish ? _T("Spin buzz") : _T("ヘッド制御");
				break;
			case 0xD1:	r = isEnglish ? _T("Offline seek performance") : _T("オフライン・シーク性能");
				break;
			case 0xDC:	r = isEnglish ? _T("Disk shift") : _T("ディスク交換");
				break;
			case 0xDD:	r = isEnglish ? _T("G-sense error rate") : _T("加速度センサー検出エラー率");
				break;
			case 0xDE:	r = isEnglish ? _T("Loaded hours") : _T("利用時間");
				break;
			case 0xDF:	r = isEnglish ? _T("Load/unload retry count") : _T("ロード／アンロード再試行回数");
				break;
			case 0xE0:	r = isEnglish ? _T("Load friction") : _T("ロード抵抗");
				break;
			case 0xE1:	r = isEnglish ? _T("Load/Unload cycle count") : _T("ロード／アンロード・サイクル数");
				break;
			case 0xE2:	r = isEnglish ? _T("Load-in time") : _T("ロードイン時間");
				break;
			case 0xE3:	r = isEnglish ? _T("Torque amplification count") : _T("トルク増幅数");
				break;
			case 0xE4:	r = isEnglish ? _T("Power-off retract count") : _T("電源切断退避数");
				break;
			case 0xE6:	r = isEnglish ? _T("GMR head amplitude") : _T("GMRヘッド振幅");
				break;
			case 0xE7:	r = isEnglish ? _T("Temperature") : _T("温度");
				break;
			case 0xF0:	r = isEnglish ? _T("Head flying hours") : _T("シーク移動時間");
				break;
			case 0xFA:	r = isEnglish ? _T("Read error retry rate") : _T("読み込みエラー再試行数");
				break;
			default:
				r = isEnglish ? _T("Unknown") : _T("不明");
				break;
			}
			return	r;
		}
	};


	//---------------------------------------------------------


	/// コンストラクタ
	CSmartAccessor(void) : m_isValidVersion(false)
	{
	}

	/**
	 * [取得] 収集.
	 *		指定のドライブNo から SMART 情報を収集します。
	 *	@param deviceNo ドライブＮｏ
	 *	@retval true 成功。本インスタンス内に情報を記憶します。
	 *	@retval false 失敗。
	 */
	bool Collect(int deviceNo)
	{
		m_isValidVersion = false;
		return m_Collect(deviceNo);
	}

	/**
	 * [取得] バージョン情報取得.
	 *	@retval NULL以外 成功。ポインタ先はバージョン情報。
	 *	@retval NULL 失敗。 Collect() が成功していないと失敗します。
	 */
	const TVersion* GetVersion(void) const
	{
		return m_isValidVersion ? &m_version : NULL;
	}

	/**
	 * [取得] 属性数取得.
	 *	@retval ０以上 成功。数値は属性数。
	 *	@retval マイナス 失敗。 Collect() が成功していないと失敗します。
	 */
	size_t GetAttributeCount(void) const
	{
		return m_infos.GetSize();
	}

	/**
	 * [取得] 属性情報取得.
	 *	@param index インデックス。 0以上、 GetAttributeCount() 未満が指定できます。
	 *	@retval NULL以外 成功。ポインタ先は属性情報。
	 *	@retval NULL 失敗。 Collect() が成功していないと失敗します。
	 */
	const TAttribute* GetAttribute(INDEX index) const
	{
		if ( m_infos.IsInRange(index) )
		{
			return m_infos[index];
		}
		return NULL;
	}

	/**
	 * [取得] 属性情報取得.
	 *	@param id 属性ＩＤ。
	 *	@retval NULL以外 成功。ポインタ先は属性ＩＤの属性情報。
	 *	@retval NULL 未発見。
	 */
	const TAttribute* FindAttributeId(BYTE id) const
	{
		loop ( i, m_infos.GetSize() )
		{
			if ( m_infos[i]->bAttrID == id )
			{
				return m_infos[i];
			}
		}
		return NULL;
	}

	/**
	 * [取得] 温度情報取得.
	 *	@retval ０以上 成功。数値は温度。
	 *	@retval マイナス 失敗。情報がない可能性もあります。
	 */
	int GetTemperature(void) const
	{
		const TAttribute* p = FindAttributeId(0xC2/*温度*/);
		if ( p != NULL )
		{
			return p->bRawValue[0];
		}
		return -1;
	}

	/**
	 * [取得] 合計利用時間取得.
	 *	@retval ０以上 成功。数値は合計利用時間。
	 *	@retval マイナス 失敗。情報がない可能性もあります。
	 */
	int GetPowerOnHoursCount(void) const
	{
		const TAttribute* p = FindAttributeId(0x09/*合計利用時間*/);
		if ( p != NULL )
		{
			return ToInt(p->GetValue());
		}
		return -1;
	}

	/**
	 * [取得] 電源投入回数取得.
	 *	@retval ０以上 成功。数値は電源投入回数。
	 *	@retval マイナス 失敗。情報がない可能性もあります。
	 */
	int GetPowerCycleCount(void) const
	{
		const TAttribute* p = FindAttributeId(0x0C/*電源投入回数*/);
		if ( p != NULL )
		{
			return ToInt(p->GetValue());
		}
		return -1;
	}

private:
	enum
	{
		DFP_GET_VERSION				= 0x00074080,
		DFP_RECEIVE_DRIVE_DATA		= 0x0007c088,
	};
	#pragma pack(push,1)
	struct TReadAttrData : SENDCMDOUTPARAMS
	{
		BYTE	bb[READ_ATTRIBUTE_BUFFER_SIZE - 1];
	};
	#pragma	pack(pop)
	// 収集
	bool m_Collect(int deviceNo)
	{
		CStr dev;
		dev.Format(_T("\\\\.\\PhysicalDrive%d"), deviceNo);
		HANDLE h = ::CreateFile(dev, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if ( h == INVALID_HANDLE_VALUE )
		{
			return false;
		}
		DWORD dwReturned;
		//== Version
		bool r1 = !! ::DeviceIoControl(h, DFP_GET_VERSION, NULL, 0, &m_version, sizeof(m_version), &dwReturned, NULL);
		if ( ! r1 )
		{
			::CloseHandle(h);
			return false;
		}
		m_isValidVersion = true;
		//== 属性取得
		SENDCMDINPARAMS	sc;	//sSendCmd;
		TReadAttrData	ra;	//sSendCmdOutParam;
		Zero(sc);
		Zero(ra);
		sc.irDriveRegs.bFeaturesReg		= READ_ATTRIBUTES;
		sc.irDriveRegs.bCylLowReg		= SMART_CYL_LOW;
		sc.irDriveRegs.bCylHighReg		= SMART_CYL_HI;
		sc.irDriveRegs.bDriveHeadReg	= static_cast<BYTE>(0xA0 | ((deviceNo & 1) << 4));	//ドライブ番号
		sc.irDriveRegs.bCommandReg		= SMART_CMD;			//SMART READ DATAコマンド番号
		sc.cBufferSize					= READ_ATTRIBUTE_BUFFER_SIZE;
		sc.bDriveNumber					= static_cast<BYTE>(deviceNo);			//ドライブ番号
		bool r2 = !! ::DeviceIoControl(h, DFP_RECEIVE_DRIVE_DATA, &sc, sizeof(sc), &ra, sizeof(ra), &dwReturned, NULL);
		::CloseHandle(h);
		if ( r2 && dwReturned == sizeof(TReadAttrData) )
		{
			return m_SetRowData(READ_ATTRIBUTE_BUFFER_SIZE, ra.bBuffer);
		}
		return	false;
	}
	/// 生データセット
	bool m_SetRowData(size_t size, const void* pData)
	{
		m_infos.RemoveAll();
		if ( size != READ_ATTRIBUTE_BUFFER_SIZE)
		{
			return	false;
		}
		m_rowData.Reset(size, static_cast<const BYTE*>(pData));
		TAttribute* pAttribute = reinterpret_cast<TAttribute*>(&m_rowData[2]);
		loop ( i, 30 )
		{
			if ( pAttribute->bAttrID != 0 )
			{
				m_infos.Add(pAttribute);
			}
			pAttribute++;
		}
		return	true;
	}

	CWorkMem					m_rowData;			///< 生情報
	CSimpleVectorT<TAttribute*>	m_infos;			///< m_rowData のポインタテーブル
	TVersion					m_version;			///< バージョン情報
	bool						m_isValidVersion;	///< バージョン情報は有効？
};



}; // TNB



