#pragma once
/**
 *@file
 * ディスクベンチマーク関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbTemporaryFile.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * ベンチマーククラス
 *
 *	@todo 作成中。
 *
 *	@par必要ファイル
 *			TnbDiskBenchMark.h
 *
 *	@date 08/03/26 新規作成
 */
class CDiskBenchMark : CCopyImpossible
{
	CTemporaryFile	m_tempFile;	///> テンポラリファイル

public:

	/// コンストラクタ
	CDiskBenchMark(void)
	{
	}

	/**
	 * [設定] ターゲット設定
	 *	@param drv ドライブ 'A'～'Z'
	 *	@param size テスト用ファイルサイズ
	 *	@retval 0 成功
	 *	@retval 0以外 失敗
	 */
	DWORD SetTarget(TCHAR drv, size_t size)
	{
		try
		{
			CStr s;
			s.Format(_T("%c:"), drv);
			m_tempFile.Free();
			m_tempFile.SetTempraryFolder(s);
			m_tempFile.Create(size);
		}
		catch ( CIoFailureException& e )
		{
			return e.GetParam();
		}
		return 0;
	}

	/**
	 * [処理] 読込みテスト
	 *	@param time 実施時間(ms)。1ms以上を指定します。
	 *	@param one 一パケットサイズ。
	 *	@retval DWORD_MAX エラー。
	 *	@retval 上記以外 読み込み速度(byte/second)。
	 */
	DWORD ReadTest(DWORD time, size_t one = 1024) const
	{
		if ( time == 0 ) { return DWORD_MAX; }
		if ( ! m_tempFile.HasThat() )
		{
			return DWORD_MAX;
		}
		CStr file = m_tempFile.GetName();
		HANDLE h = ::CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if ( h == INVALID_HANDLE_VALUE )
		{
			return DWORD_MAX;
		}
		ULONGLONG total = 0;
		DWORD startTick = ::GetTickCount();
		DWORD dwRead;
		CWorkMem work(one);
		BYTE* B = work.Ref();
		while ( true )
		{
			dwRead = 0;
			if ( ! ::ReadFile(h, B, ToDword(one), &dwRead, NULL) )
			{
				total = DWORD_MAX;
				break;
			}
			DWORD nowTick = ::GetTickCount();
			DWORD d = nowTick - startTick;
			total += dwRead;
			if ( d >= time )
			{
//				TRACE2("read %dKB / %dms\n", (DWORD)(total / 1024), d);
				total *= 1000;
				total /= d;
				break;
			}
			if ( dwRead != one )
			{
				::SetFilePointer(h, 0, NULL, FILE_BEGIN);
			}
		}
		::CloseHandle(h);
		return static_cast<DWORD>(total);
	}






private :
	friend class CDiskBenchMarkTest;
};



}; // TNB
