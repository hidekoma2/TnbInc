#pragma once
/**
 *@file
 * リカバリーファイルズ関係のヘッダ
 *
 *		状況に応じて、ファイルを元の状態に戻すことが出来るクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbFileName.h"
#include "TnbStrVector.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * リカバリーファイルズクラス
 *
 *		状況に応じて、ファイルを元の状態に戻すことが出来るクラスがあります。
 *
 *		
 *
 *	@par必要ファイル
 *			TnbRecoveryFiles.h
 *
 *	@date 09/03/03	新規作成
 */
class CRecoveryFiles
{
public:

	/**
	 * コンストラクタ
	 *	@param isDefaultRecoveryMode デストラクタ時の動作。 true;リカバリー / 省略 or false;アンレジ。
	 */
	CRecoveryFiles(bool isDefaultRecoveryMode = false) : m_isDefaultRecoveryMode(isDefaultRecoveryMode)
	{
	}

	/// デストラクタ
	~CRecoveryFiles(void)
	{
		if ( m_isDefaultRecoveryMode )
		{
			RecoverAll();
		}
		else
		{
			UnregisterAll();
		}
	}

	/**
	 * [設定] 登録.
	 *		指定ファイルを複製し、ファイル名を記憶します。
	 *	@note 複数登録可能です。
	 *	@param lpszFile ファイル名
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Register(LPCTSTR lpszFile)
	{
		CFileName fn(lpszFile);
		if ( fn.IsExist() )
		{
			CStr bak;
			loop ( i, 0x10000 )
			{
				bak = fn.GetFullName() + CStr::Fmt(_T(".%04X.bak"), i);
				if ( ! m_Exist(bak) )
				{
					break;
				}
			}
			if ( fn.CopyTo(bak) )
			{
				TParam p(fn.GetFullName(), bak);
				p.backHandle = ::CreateFile(bak, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
				m_names.Add(p);
				return true;
			}
		}
		return false;
	}

	/**
	 * [設定] 登録解除.
	 *	@note 登録時複製したバックアップファイルを削除し、クラスの管理下から外します。
	 *	@retval true バックアップファイルの掃除に成功。
	 *	@retval false いくつか消せないバックアップファイルがあった。
	 */
	bool UnregisterAll(void)
	{
		bool r = true;
		loop ( i, m_names.GetSize() )
		{
			m_names[i].backHandle.Null();
			r &= !! ::DeleteFile(m_names[i].backName);
		}
		m_names.RemoveAll();
		return r;
	}

	/**
	 * [設定] リカバー.
	 *	@note 登録した時の状態にファイルを戻します。
	 *	@note クラスの管理下から外します。
	 *	@retval true 全部元に戻せた
	 *	@retval false いくつか戻せないものがあった（オリジナルは bak ファイルとして残っている）
	 */
	bool RecoverAll(void)
	{
		bool r = true;
		//== 先にtmpファイル作成
		loop ( i, m_names.GetSize() )
		{
			CStr reg = m_names[i].registName;
			if ( m_Exist(reg) )
			{
				CStr tmp;
				loop ( ii, 0x10000 )
				{
					tmp = reg + CStr::Fmt(_T(".%04X.tmp"), ii);
					if ( ! m_Exist(tmp) )
					{
						break;
					}
				}
				if ( ! ::MoveFile(reg, tmp) )
				{
					_GetLastError("MoveFile");
					m_names[i].tempName.Empty();
					r = false;
				}
				else
				{
					m_names[i].tempName = tmp;
				}
			}
		}
		//== tmpファイルが出来たものはbakをオリジナルにリネームし、tmpを削除。
		loop ( i, m_names.GetSize() )
		{
			m_names[i].backHandle.Null();
			if ( ::MoveFile(m_names[i].backName, m_names[i].registName) )
			{
				if ( ! m_names[i].tempName.IsEmpty() )
				{
					VERIFY( ::DeleteFile(m_names[i].tempName) );
				}
			}
			else
			{
				// 意図的に bak ファイルを消した場合など.
				r = false;
			}
		}
		m_names.RemoveAll();
		return r;
	}

private:
	/// ファイルは存在？
	bool m_Exist(LPCTSTR lpszName) const
	{
		WIN32_FIND_DATA t;
		HANDLE h = ::FindFirstFile(lpszName, &t);
		if ( h != INVALID_HANDLE_VALUE )
		{
			::FindClose(h);
			return true;
		}
		return false;
	}
	/// パラメータ
	struct TParam
	{
		CStr			registName;	///< 登録名
		CStr			backName;	///< バックアップ名
		CHandleHandle	backHandle;	///< バックアップファイルのハンドル
		CStr			tempName;	///< テンポラリ名
		TParam(LPCTSTR reg = NULL, LPCTSTR tmp = NULL) : registName(reg), backName(tmp)
		{
		}
	};
	CVectorT<TParam>	m_names;					///< 名前群
	bool				m_isDefaultRecoveryMode;	///< デストラクタ時の動作。 true;リカバリー false;アンレジ
	friend class CRecoveryFilesTest;
};



}; // TNB

