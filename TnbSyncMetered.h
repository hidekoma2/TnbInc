#pragma once
/**
 *@file
 * 同期処理関係のヘッダ
 *
 *		MeteredSectionを使った同期を実現するクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSync.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル
	namespace NMetered
	{
		//Mr.Dan Chou の排他Obj
		#include "other/MeteredSection.h"
	};
#endif



/**@ingroup SYNC
 * Metered排他管理クラス
 *
 *		MeteredSectionを使った同期を実現します。
 *
 *	@note 別プロセスでも別インスタンスでも同じ名前なら同期が取れます。
 *	@note 同スレッドではロックできます（デッドロックに注意）。
 *	@note カウンタを指定できます。
 *
 *	@see http://msdn.microsoft.com/library/en-us/dndllpro/html/msdn_metrsect.asp
 *
 *	@par必要ファイル 
 *			TnbSyncMetered.h
 *
 *	@date 06/04/14	新規作成
 *	@date 06/11/15 作成失敗したら スローするように変更。
 */
class CSyncMetered : public ISynchronized, CCopyImpossible
{
	NMetered::LPMETERED_SECTION	m_pms;		///< 排他ハンドル
	mutable LONG	m_lLockCount;			///< 本インスタンスでのロック数
public:

	/** 
	 * コンストラクタ
	 *	@param lpszName 同期用の名前を指定します。
	 *					省略すると別インスタンス同士の同期は取れません。
	 *	@param iMaximumCount 同時に実行できる数を指定します。省略すると１です。
	 *					同じ名前がすでに作成されている場合、この値は無視されます。
	 *	@throw CInvalidParamException 名前不正など、同期オブジェクトが作成できなかった時、スローされます。
	 */
	explicit CSyncMetered(LPCTSTR lpszName = NULL, int iMaximumCount = 1)
	{
		ASSERTLIB( lpszName == NULL || STRLIB::GetLen(lpszName) < MAX_METSECT_NAMELEN );
		m_pms = NMetered::CreateMeteredSection(iMaximumCount, iMaximumCount, lpszName);
		m_lLockCount = 0;
		ASSERT0(
			m_pms != NULL,
			"CSyncMetered::CSyncMetered()",
			"MeteredSectionが初期化できませんした。名前に問題がある可能性が有ります。"
		);
		if ( m_pms == NULL )
		{
			throw CInvalidParamException();
		}
	}

	/** 
	 * デストラクタ
	 *	@note ロックした数をアンロックしてます。
	 */
	virtual ~CSyncMetered(void)
	{
		if ( m_pms != NULL )
		{
			if ( m_lLockCount > 0 )
			{
				NMetered::LeaveMeteredSection(m_pms, m_lLockCount, NULL);
			}
			NMetered::CloseMeteredSection(m_pms);
			m_pms = NULL;
		}
	}

	/** 
	 * [排他] ロック
	 *	@param dwTime ロックにトライし続けるﾐﾘ秒。省略すると無限に待ちます。
	 *	@retval true	成功
	 *	@retval false	失敗
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const
	{
		if ( m_pms != NULL )
		{
			DWORD dwRc = NMetered::EnterMeteredSection(m_pms, dwTime);
			if ( dwRc == WAIT_OBJECT_0 )
			{
				::InterlockedIncrement(&m_lLockCount);
				return true;
			}
		}
		return false;
	}

	/** [排他] アンロック */
	virtual void Unlock(void) const
	{
		if ( m_pms != NULL && m_lLockCount > 0 )
		{
			NMetered::LeaveMeteredSection(m_pms, 1, NULL);
			::InterlockedDecrement(&m_lLockCount);
		}
	}
};



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")
