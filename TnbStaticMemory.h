#pragma once
/**
 *@file
 * 静的メモリ管理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSingleton.h"



//TNB Library
namespace TNB
{



/**@ingroup MEMORY
 * 静的メモリ管理クラス
 *
 *		サイズの異なるバンクを最大256個作成し、指定のバンクから指定のサイズずつ分配して
 *		使用することが出来ます。 new/delete を繰り返すことによるメモリの分断や、
 *		メモリ確保速度が気になる場合に使用することが出来ます。
 *
 *		インスタンスは、 Allocate() することで領域を確保できます。
 *		あるいは確保しているインスタンスをコピーすることでも確保できます
 *		（この場合、同じ領域を参照している）。
 *		確保している状態で Lock() をすることで実際のメモリアドレスが得られます。
 *
 *		不必要になれば、 Free() 、あるいはインスタンスをデストラクトします。
 *
 *		なお、 Lock() を Unlock() しないと、 ResizeBank() や DeleteBank() が出来なくなります。
 *		逆にこれらを使わないのであれば、 Unlock() を使う必要は有りません。
 *
 *	@note	領域を確保したしたインスタンスをコピーすると共有されます（二つから参照されている状態）。
 *			そして、デストラクタや別のインスタンスのコピー、 Free()などで
 *			参照ががなくなれば、そのメモリは自動的に解放されます。
 *
 *		おまけ(!)で、32BITのパラメータを持つことが出来ます。本クラスを関数の戻り値などに
 *		使用する場合に、有効だと思います。
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *		
 *		CStaticMemory::CreateBank(0, 1000, 10);// Bank0に 1000x10バイトのメモリ確保
 *		     ;
 *		CStaticMemory h1;
 *		h1.Allocate(0, 1000);                 // Bank0から 1000byte確保
 *		CStaticMemory h2 = h1;               // h1 と h2 は同じメモリを参照
 *		LPVOID P = h2.Lock();                // 実際のメモリアドレス取得&Lock
 *		     ;
 *		h2.Unlock();                         // 使用後 Unlock(Bankを変更しないなら不必要)		
 *		</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbStaticMemory.h
 *
 *	@date 06/07/21 新規作成
 *	@date 06/07/23 デフラグできるように仕様修正。
 */
class CStaticMemory
{
public:

	/// リザルト
	enum EResult
	{
		ER_Success,			///< 成功
		ER_ExistBank,		///< すでにバンクがあります。
		ER_NotFoundBank,	///< バンクが見つかりません。
		ER_OutOfMemory,		///< メモリが足りません。
		ER_InvalidTell,		///< 不正なTELLです。
		ER_InvalidParam,	///< 不正なパラメータです。
		ER_Locked,			///< ロックされています。
	};

private:

	/// 静的メモリ管理クラス用型宣言
	typedef DWORD TELL;

	/// 静的メモリ管理クラス用エラー値宣言
	#define INVALID_TELL (ULONG_MAX)

	/// CStaticMemory管理クラス
	class CManage
	{
		CSyncSection m_syncFunc;	///< 関数排他
		/**
		 * 情報管理クラス
		 */
		class CInfo
		{
			DWORD	m_dwLockCount;			///< ロックカウンタ
			DWORD	m_dwSectorSize;			///< セクタサイズ
			WORD	m_wSectorCount;			///< セクタ数
			WORD	m_wCacheIndex;			///< 次の検索Index
			WORD	m_wCacheSecNo;			///< 次の検索セクタNo
			WORD	m_wUsedSectors;			///< 使用しているセクタ数
			BYTE*	m_pContent;				///< プール
			/// Index->SecNo変換テーブルのアドレス
			WORD *index(void) { return reinterpret_cast<WORD*>(m_pContent); }
			/// フラグテーブルのアドレス
			WORD *flag(void)
			{
				return (m_pContent == NULL)
						? NULL
						: reinterpret_cast<WORD*>(&m_pContent[sizeof(WORD) * m_wSectorCount]); 
			}
			/// データの先頭アドレス
			BYTE* data(WORD wSecNo)
			{
				return (m_pContent == NULL)
						? NULL
						: &m_pContent[	sizeof(WORD) * m_wSectorCount + 
										sizeof(WORD) * m_wSectorCount + 
										m_dwSectorSize * wSecNo]; 
			}
			/// Index(TELLの下位WORD) → SecNo
			WORD m_IndexToSecNo(WORD i)
			{
				WORD* W = index();
				ASSERTLIB(W[i] != 0);
				return ToWord(W[i] & 0x7FFF);
			}
			/**
			 * 確保
			 *	@param wStart 検索開始セクタ番号
			 *	@param wEnd 検索終了セクタ番号+1
			 *	@param wLen 確保したいセクタ数
			 *	@retval INVALID_TELL	確保できず。
			 *	@retval 上記以外	確保した先頭セクタ番号（成功）
			 */
			DWORD m_AllocSub(WORD wStart, WORD wEnd, WORD wLen)
			{
				WORD wNext;
				WORD* F = flag();
				while ( wStart < wEnd )
				{
					wNext = static_cast<WORD>(F[wStart] & 0x7FFF);
					if ( wNext == 0 )
					{
						wNext = m_wSectorCount;
					}
					if ( (F[wStart] & 0x8000) == 0 && (wNext - wStart) >= wLen )
					{
						//期待セクタ数の空きがあった
						WORD w = static_cast<WORD>(wStart + wLen);
						if ( w >= m_wSectorCount )
						{ 
							w = 0;
						}
						for ( WORD i = 0; i < wLen; i++ )
						{
							F[wStart + i] = static_cast<WORD>(0x8000 | w);
						}
						m_wCacheSecNo = w;
						return wStart;
					}
					wStart = wNext;
				}
				return INVALID_TELL;
			}
		public:
			CSyncSection syncBank;		///< 排他
			/// コンストラクタ
			CInfo(void)
			{
				m_wCacheIndex	= 0;
				m_wCacheSecNo	= 0;
				m_wUsedSectors	= 0;
				m_dwLockCount	= 0;
				m_dwSectorSize	= 0;
				m_wSectorCount	= 0;
				m_pContent		= NULL;		///< プール
			}
			/// デストラクタ
			~CInfo(void)
			{
				if ( m_pContent != NULL )
				{
					delete[] m_pContent;
					m_pContent = NULL;
				}
			}
			/// セクタ数取得
			WORD GetSectorCount(void) { return m_wSectorCount; }
			/// 内容確保済み確認
			bool IsValid(void) { return m_pContent != NULL; }
			/**
			 * プール作成
			 *	@param dwSectorSize セクタサイズ。16以上
			 *	@param wSectorCount セクタ数。10以上 32768未満
			 *	@return EResult
			 */
			EResult Create(DWORD dwSectorSize, WORD wSectorCount)
			{
				EXCLUSIVE(&syncBank);
				if ( m_pContent != NULL ) { return ER_ExistBank; }
				if ( dwSectorSize < 16 ) { return ER_InvalidParam; }
				if ( wSectorCount < 10 || wSectorCount >= 32768) { return ER_InvalidParam; }
				m_pContent = new BYTE[sizeof(WORD) * wSectorCount + (sizeof(WORD) + dwSectorSize) * wSectorCount];
				if ( m_pContent == NULL ){ return ER_OutOfMemory; }
				memset(m_pContent, 0, sizeof(WORD) * wSectorCount + sizeof(WORD) * wSectorCount);
				m_dwSectorSize = dwSectorSize;
				m_wSectorCount= wSectorCount;
				return ER_Success;
			}
			/// プール破棄
			void Delete(void)	
			{
				EXCLUSIVE(&syncBank);
				#ifdef _DEBUG
					if ( m_dwLockCount != 0 )
					{
						TRACE0("CStaticMemory::CManage::Delete() ; Lock/Unlockの回数が合っていません。\n");
					}
				#endif
				if ( m_pContent != NULL )
				{
					delete[] m_pContent;
					m_pContent = NULL;
				}
				m_wCacheSecNo = 0;
				m_wCacheIndex = 0;
				m_wUsedSectors = 0;
				m_dwLockCount = 0;
			}
			/**
			 * プールサイズ変更
			 *	@note 小さくすることは禁止されています。
			 *	@param wSectorCount セクタ数。10以上 32768未満
			 *	@return EResult
			 */
			EResult Resize(WORD wSectorCount)
			{
				EXCLUSIVE(&syncBank);
				if ( wSectorCount == 0 )
				{
					wSectorCount = m_wSectorCount;
				}
				else if ( m_wUsedSectors > wSectorCount )
				{
					return ER_OutOfMemory;	//足りなくなる
				}
				else if ( m_wSectorCount > wSectorCount )
				{
					return ER_InvalidParam;	//小さくなる
				}
				if ( m_pContent == NULL ){ return ER_NotFoundBank; }
				CInfo infoNew;
				EResult rc = infoNew.Create(m_dwSectorSize, wSectorCount);
				if ( rc != ER_Success ){ return rc; }
				//
				WORD* W = index();
				WORD* newW = infoNew.index();
				WORD* F = flag();
				WORD wPos;
				for ( WORD i = 0; i < m_wSectorCount; i++ )
				{
					wPos = W[i];
					if ( wPos != 0 )
					{
						wPos &= 0x7FFF;
						WORD wLen = static_cast<WORD>((F[wPos] & 0x7FFF) - wPos);
						DWORD r = infoNew.m_AllocSub(infoNew.m_wCacheSecNo, wSectorCount, wLen);
						if ( r == INVALID_TELL && infoNew.m_wCacheSecNo != 0 )
						{
							r = infoNew.m_AllocSub(0, infoNew.m_wCacheSecNo, wLen);
						}
						ASSERTLIB(r != INVALID_TELL);
						BYTE* P = infoNew.data(ToWord(r));
						ASSERTLIB(P != NULL);
						MemCopy(P, data(wPos), wLen * m_dwSectorSize);
						newW[i] = ToWord(0x8000 | r);
					}
				}
				m_wSectorCount= wSectorCount;
				m_wCacheSecNo = 0;
				m_wCacheIndex = 0;
				delete[] m_pContent;
				m_pContent = infoNew.m_pContent;
				infoNew.m_pContent = NULL;
				return ER_Success;
			}
			/**
			 * 使用セクタ数
			 */
			WORD GetUsedSectors(void)
			{
				return m_wUsedSectors;
			}
			/**
			 * 確保
			 *	@param size 確保したいByte数
			 *	@retval INVALID_TELL	確保できず。
			 *	@retval 上記以外	確保したIndex(成功）
			 */
			DWORD Allocate(size_t size)
			{
				EXCLUSIVE(&syncBank);
				if ( m_pContent == NULL ){ return INVALID_TELL; }
				WORD wLen = ToWord((ToDword(size) + m_dwSectorSize - 1) / m_dwSectorSize);
				if ( wLen + m_wUsedSectors > m_wSectorCount ){ return INVALID_TELL; }
				DWORD r = m_AllocSub(m_wCacheSecNo, m_wSectorCount, wLen);
				if ( r == INVALID_TELL && m_wCacheSecNo != 0 )
				{
					r = m_AllocSub(0, m_wCacheSecNo, wLen);
				}
				if ( r == INVALID_TELL )
				{
					return r;
				}
				//
				m_wUsedSectors = static_cast<WORD>(m_wUsedSectors + wLen);
				WORD* W = index();
				for ( WORD i = m_wCacheIndex; i < m_wSectorCount; i++ )
				{
					if ( W[i] == 0 )
					{
						W[i] = ToWord(0x8000 | r);
						m_wCacheIndex = i;
						return i;
					}
				}
				for ( WORD i = 0; i < m_wCacheIndex; i++ )
				{
					if ( W[i] == 0 )
					{
						W[i] = ToWord(0x8000 | r);
						m_wCacheIndex = i;
						return i;
					}
				}
				ASSERTLIB(false);
				return INVALID_TELL;
			}
			/**
			 * 解放
			 *	@param wIndex Index(TELLの下位WORD)
			 *	@retval true 成功
			 *	@retval false 失敗
			 */
			bool Free(WORD wIndex)
			{
				EXCLUSIVE(&syncBank);
				if ( m_pContent == NULL ){ return false; } // プールなし
				if ( wIndex >= m_wSectorCount ){ return false; } // 番号異常
				WORD wSecNo = m_IndexToSecNo(wIndex);
				ASSERTLIB(wSecNo < m_wSectorCount);
				WORD* F = flag();
				if ( (F[wSecNo] & 0x8000) == 0 ){ return false; } // 確保していない
				m_wCacheSecNo = wSecNo;
				WORD w;
				WORD wNext = static_cast<WORD>(F[wSecNo] & 0x7FFF);
				if ( wNext == 0 )
				{
					//= 最後。全部０にしちゃう
					wNext = m_wSectorCount;
					w = 0;
				}
				else
				{
					//= 次のブロック確認
					if ( (F[wNext] & 0x8000) != 0 )
					{
						//確保している.次のブロックはNext
						w = wNext;
					}
					else
					{
						//確保していない。次のブロックの次にする。
						w = F[wNext];
					}
				}
				for ( WORD i = wSecNo; i < wNext; i++ )
				{
					F[i] = w;
				}
				index()[wIndex] = 0;
				m_wCacheIndex = wIndex;
				m_wUsedSectors = static_cast<WORD>(m_wUsedSectors - (wNext - wSecNo));
				return true;
			}
			/**
			 * 指定セクタの確保Byte数
			 *	@note 確保した先頭のセクタでない場合のエラーになります。
			 *	@note 実際に確保要求のサイズではなく、セクタサイズの公倍数にパディングされます。
			 *	@param wIndex Index(TELLの下位WORD)
			 *	@retval INVALID_TELL エラー
			 *	@retval INVALID_TELL以外 確保Byte数
			 */
			size_t GetSize(WORD wIndex)
			{
				EXCLUSIVE(&syncBank);
				if ( wIndex >= m_wSectorCount ){ return INVALID_TELL; } // 番号異常
				WORD wSecNo = m_IndexToSecNo(wIndex);
				ASSERTLIB(wSecNo < m_wSectorCount);
				WORD* F = flag();
				if ( (F[wSecNo] & 0x8000) == 0 ){ return INVALID_TELL; } //確保してない
				if ( wSecNo != 0 )
				{
					if ( F[wSecNo - 1] == F[wSecNo] ){ return INVALID_TELL; }//一つ前と同じのはずがない
				}
				return ((F[wSecNo] & 0x7FFF) - wSecNo) * m_dwSectorSize;
			}
			/**
			 * ロックカウンタ取得
			 *	@return ロックカウンタ
			 */
			DWORD GetLockCount(void) { return m_dwLockCount; }
			/**
			 * ロック
			 *	@note ロックカウンタを＋１し、アドレスを返します。
			 *	@param wIndex Index(TELLの下位WORD)
			 *	@retval NULL エラー（カウンタは＋１しません）
			 *	@retval NULL以外 アドレス
			 */
			LPVOID Lock(WORD wIndex)
			{
				EXCLUSIVE(&syncBank);
				if ( GetSize(wIndex) == INVALID_TELL ){ return NULL; }
				WORD wSecNo = m_IndexToSecNo(wIndex);
				ASSERTLIB(wSecNo < m_wSectorCount);
				ASSERT0(data(wSecNo) != NULL, "CStaticMemory::CManage::CInfo::Lock()", "確保していません");
				m_dwLockCount++;
				return data(wSecNo);
			}

			/**
			 * アンロック
			 *	@note ロックカウンタをー１します。
			 */
			void Unlock(void) 
			{
				m_dwLockCount--;
			}
		private:
			friend class CStaticMemoryTest;
		}; // CINfo

		//--------------------

		CInfo*	m_pInfo;			///< インフォ情報
		/// コンストラクタ
		SINGLETON_CONSTRUCTOR(CManage)
		{
			m_pInfo = new CInfo[256];
			ASSERT( m_pInfo != NULL );
		}
		/**
		 * TELL内容の整合チェック
		 *	@retval true 正しい
		 *	@retval false 異常。
		 */
		bool m_CheckTell(TELL& tell, BYTE& bank)
		{
			bank = static_cast<BYTE>(tell >> 24);
			tell &= 0x0000FFFF;
			if ( ! m_pInfo[bank].IsValid() ){ return false; }
			return m_pInfo[bank].GetSectorCount() > tell;
		}
	public:
		/// デストラクタ
		~CManage(void)
		{
			if ( m_pInfo != NULL )
			{
				delete[] m_pInfo;
				m_pInfo = NULL;
			}
		}
		/**
		 * [設定] バンク作成.
		 *		指定バンクの確保サイズを設定
		 *	@param bank バンク（０～２５５）。
		 *	@param dwSectorSize セクタサイズ。16以上
		 *	@param wSectorCount セクタ数。10以上 32768未満
		 *	@return EResult
		 */
		EResult CreateBank(BYTE bank, DWORD dwSectorSize, WORD wSectorCount)
		{
			EXCLUSIVE(&m_syncFunc);
			return m_pInfo[bank].Create(dwSectorSize, wSectorCount);
		}
		/**
		 * [設定] バンク削除.
		 *		指定バンクの開放
		 *	@param bank バンク（０～２５５）。
		 *	@param boIsForce 省略すると、 Lockチェックを行います。<BR>
		 *					true を指定すると、LockされていてもBankをDeleteします。
		 *	@return EResult
		 */
		EResult DeleteBank(BYTE bank, bool boIsForce = false)
		{
			EXCLUSIVE2(&m_syncFunc,&m_pInfo[bank].syncBank);
			if ( ! m_pInfo[bank].IsValid() ){ return ER_NotFoundBank; }
			if ( ! boIsForce && m_pInfo[bank].GetLockCount() != 0 ){ return ER_Locked; }
			m_pInfo[bank].Delete();
			return ER_Success; 
		}
		/**
		 * [設定] バンクのりサイズ.
		 *	@note 小さくすることは禁止されています。
		 *	@note ロックしていると失敗します。
		 *	@note セクタ数を省略することで、ガベージコレクションになります。
		 *	@param bank バンク（０～２５５）。
		 *	@param wSectorCount 変更したいセクタ数
		 *	@return EResult
		 */
		EResult ResizeBank(BYTE bank, WORD wSectorCount = 0)
		{
			EXCLUSIVE2(&m_syncFunc,&m_pInfo[bank].syncBank);
			if ( ! m_pInfo[bank].IsValid() ){ return ER_NotFoundBank; }
			if ( m_pInfo[bank].GetLockCount() != 0 ){ return ER_Locked; }
			return m_pInfo[bank].Resize(wSectorCount);
		}
		/**
		 * [取得] 使用セクタ数取得.
		 *	@note 使用セクタ数が確保セクタ数より小さくても、確保できるとは限りません
		 *			（ ResizeBank() をすると使用できるはずです）。
		 *	@param bank バンク（０～２５５）。
		 *	@return 使用セクタ数
		 */
		WORD GetUsedSectors(BYTE bank)
		{
			return static_cast<WORD>(m_pInfo[bank].IsValid() ? m_pInfo[bank].GetUsedSectors() : 0);
		}
		/**
		 * [設定] 確保.
		 *		指定Bankから指定サイズを確保します。
		 *	@param bank バンク（０～２５５）。
		 *	@param size サイズ
		 *	@retval INVALID_TELL 失敗
		 *	@retval 上記以外 TELL
		 */
		TELL Allocate(BYTE bank, size_t size)
		{
			if ( size == 0 || ! m_pInfo[bank].IsValid() ){ return INVALID_TELL; }
			DWORD dw = m_pInfo[bank].Allocate(size);
			return (dw == INVALID_TELL) ? INVALID_TELL : ((bank << 24) | dw); 
		}
		/**
		 * [設定] 解放.
		 *	@param tell Allocate() で取得した TELL
		 *	@retval true	成功
		 *	@retval false	不正なTELLです。
		 */
		bool Free(TELL tell)
		{
			BYTE bank;
			return m_CheckTell(tell, bank) ? m_pInfo[bank].Free(ToWord(tell)) : false;
		}
		/**
		 * [設定] 取得.
		 *	@note 実際に確保要求のサイズではなく、セクタサイズの公倍数にパディングされます。
		 *	@param tell Allocate() で取得した TELL
		 *	@retval 0		不正なTELLです。
		 *	@retval 0以上	確保Byte数
		 */
		size_t GetSize(TELL tell)
		{
			BYTE bank;
			return m_CheckTell(tell, bank) ? m_pInfo[bank].GetSize(ToWord(tell)) : 0;
		}
		/**
		 * [取得] メモリロック.
		 *		TELLからアドレスを得ます。
		 *	@note ロックしていると、ResizeBank() や DeleteBank() が失敗します。
		 *	@param tell Allocate() で取得した TELL
		 *	@retval NULL以外	アドレス
		 *	@retval NULL	不正なTELLです。
		 */
		LPVOID Lock(TELL tell)
		{
			BYTE bank;
			return m_CheckTell(tell, bank) ? m_pInfo[bank].Lock(ToWord(tell)) : NULL;
		}

		/**
		 * [取得] メモリアンロック.
		 *	@param tell Allocate() で取得した TELL
		 *	@retval true	成功
		 *	@retval false	不正なTELLです。
		 */
		bool Unlock(TELL tell)
		{
			BYTE bank;
			if ( ! m_CheckTell(tell, bank) ){ return false; }
			m_pInfo[bank].Unlock();
			return true;
		}

	private:
		friend class CStaticMemoryTest;
	}; // CManage

	//--------------------

	CManage*	m_pMan;			///< CManageのインスタンス
	TELL		m_tell;			///< 管理している TELL
	DWORD		m_dwParam;		///< パラメータ
	/// 初期化
	void m_Init(void)
	{
		m_dwParam = 0;
		m_pMan = CManage::GetInstance();
		m_tell = INVALID_TELL;
	}

public:

	/**
	 * [設定] バンク作成.
	 *		指定バンクの確保サイズを設定
	 *	@param bank バンク
	 *	@param dwSectorSize セクタサイズ。16以上
	 *	@param wSectorCount セクタ数。10以上 32768未満
	 *	@retval ER_Success		成功
	 *	@retval ER_ExistBank	すでにバンクがあります。
	 *	@retval ER_OutOfMemory	メモリが足りません。
	 *	@retval ER_InvalidTell	不正なTELLです。
	 *	@retval ER_InvalidParam	不正なパラメータです。
	 */
	static EResult CreateBank(BYTE bank, DWORD dwSectorSize, WORD wSectorCount)
	{
		return CManage::GetInstance()->CreateBank(bank, dwSectorSize, wSectorCount);
	}

	/**
	 * [設定] バンク削除.
	 *		指定バンクの開放
	 *	@param bank バンク
	 *	@param boIsForce 省略すると、 Lockチェックを行います。<BR>
	 *					true を指定すると、LockされていてもBankをDeleteします。
	 *	@retval ER_Success		成功
	 *	@retval ER_NotFoundBank	バンクが見つかりません。
	 *	@retval ER_Locked		ロックされています。
	 */
	static EResult DeleteBank(BYTE bank, bool boIsForce = false)
	{
		return CManage::GetInstance()->DeleteBank(bank, boIsForce);
	}

	/**
	 * [設定] バンクのりサイズ.
	 *	@note 小さくすることは禁止されています。
	 *	@note ロックしていると失敗します。
	 *	@note リサイズ時使用メモリを先頭に集めます（デフラグ）。
	 *	@note セクタ数を省略することで、数を変更せずデフラグのみ行います。
	 *	@param bank バンク
	 *	@param wSectorCount 変更したいセクタ数。省略可能。
	 *	@retval ER_Success		成功
	 *	@retval ER_NotFoundBank	バンクが見つかりません。
	 *	@retval ER_OutOfMemory	メモリが足りません。
	 *	@retval ER_InvalidParam	不正なパラメータです。
	 *	@retval ER_Locked		ロックされています。
	 */
	static EResult ResizeBank(BYTE bank, WORD wSectorCount = 0)
	{
		return CManage::GetInstance()->ResizeBank(bank, wSectorCount);
	}

	/// デストラクタ
	~CStaticMemory(void)
	{
		Free();
	}
	
	/// コンストラクタ
	CStaticMemory(void)
	{
		m_Init();
	}

	/**
	 * コピーコンストラクタ
	 *	@param h コピー元。
	 */
	CStaticMemory(const CStaticMemory& h)
	{
		m_Init();
		operator=(h);
	}

	/**
	 * [代入] コピーオペレータ
	 *	@param other コピー元。
	 *	@return 自分の参照
	 */
	CStaticMemory& operator=(const CStaticMemory& other)
	{
		Free();
		if ( other.m_tell != INVALID_TELL )
		{
			LONG* B = static_cast<LONG*>(m_pMan->Lock(other.m_tell));
			::InterlockedIncrement(B);
			m_pMan->Unlock(other.m_tell);
			m_tell = other.m_tell;
		}
		m_dwParam = other.m_dwParam;
		return *this;
	}

	/**
	 * [取得] パラメータ取得
	 *	@return パラメータ
	 */
	DWORD GetParam(void)
	{
		return m_dwParam;
	}

	/**
	 * [代入] パラメータ設定
	 *	@param dwParam パラメータ
	 */
	void SetParam(DWORD dwParam)
	{
		m_dwParam = dwParam;
	}

	/**
	 * [取得] 共有数取得.
	 *		インスタンスをコピーすると、共有数が＋１されます。
	 *			1の時、Free(）かデストラクトすると、メモリが解放されます。
	 *	@note 開発用のメソッドです。通常直接使いません。
	 *	@return 共有数
	 */
	LONG GetJointlyCount(void)
	{
		if ( m_tell == INVALID_TELL){ return 0; }
		LONG* B = static_cast<LONG*>(m_pMan->Lock(m_tell));
		LONG r = *B;
		m_pMan->Unlock(m_tell);
		return r;
	}

	/**
	 * [確認] メモリ保持確認
	 *	@retval true 持っている。
	 *	@retval false 持っていない。
	 */
	bool HasMemory(void)
	{
		return m_tell!=INVALID_TELL;
	}

	/**
	 * [取得] メモリ確保
	 *	@note 確保の前に、本インスタンスがメモリを持っていたら参照をやめます
	 *		（参照がなくなればメモリは解放されます）。
	 *	@param bank バンク
	 *	@param size サイズ
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Allocate(BYTE bank, size_t size)
	{
		Free();
		TELL t = m_pMan->Allocate(bank, size + sizeof(LONG));//カウンタ分多く
		if ( t == INVALID_TELL )
		{
			return false;
		}
		LONG* P = static_cast<LONG*>(m_pMan->Lock(t));
		if ( P == NULL )
		{
			m_pMan->Free(t);
			return false;
		}
		*P = 1;	//カウンタ初期化
		m_pMan->Unlock(t);
		m_tell = t;
		return true;
	}

	/**
	 * [取得] メモリ解放
	 *	@note 本インスタンスがメモリを持っていたら参照をやめます
	 *		（参照がなくなればメモリは解放されます）。
	 */
	void Free(void)
	{
		if ( m_tell == INVALID_TELL) { return; }
		LONG* B = static_cast<LONG*>(m_pMan->Lock(m_tell));
		if ( ::InterlockedDecrement(B) == 0 )
		{
			m_pMan->Unlock(m_tell);
			m_pMan->Free(m_tell);
		}
		else
		{
			m_pMan->Unlock(m_tell);
		}
		m_tell=INVALID_TELL;
	}

	/**
	 * [取得] 確保サイズ取得
	 *	@note 実際に確保要求のサイズではなく、セクタサイズの公倍数-4にパディングされます。
	 *	@retval 0		確保していない。
	 *	@retval 0以上	確保Byte数
	 */
	size_t GetSize(void)
	{
		if ( m_tell == INVALID_TELL ){ return 0; }
		size_t l = m_pMan->GetSize(m_tell);
		if ( l != 0 )
		{
			l -= sizeof(LONG); 
		}
		return l;
	}

	/**
	 * [取得] アドレス取得＆ロック.
	 *	@note ロックカウンタを＋１し、アドレスを返します。
	 *	@retval NULL エラー（カウンタは＋１しません）
	 *	@retval NULL以外 アドレス
	 */
	LPVOID Lock(void)
	{
		if ( m_tell == INVALID_TELL ) { return NULL; }
		LONG* B = static_cast<LONG*>(m_pMan->Lock(m_tell));
		return &B[1];
	}

	/**
	 * [取得] アンロック
	 *	@note ロックカウンタをー１します。
	 */
	void Unlock(void)
	{
		if ( m_tell != INVALID_TELL )
		{
			m_pMan->Unlock(m_tell);
		}
	}

private:
	friend class CStaticMemoryTest;
};



};//TNB



#ifndef _TnbDOXYGEN	//Document作成用シンボル
#if 0
	//m_pContent の確保内容
	[Indexテーブル][Flagテーブル][内容]という並び。
	//Indexテーブル
	並びはIndex順。
	TELLの下位WORD(Index)とセクタNoを関連付けるテーブル。最上位BIT
	が立っていたら使用（下位15BITがSecNo）
	Resize、デフラグしても並びは変換しない。
	// Flagテーブル＆内容
	いずれも並びはSecNo順。
	Flag用DWORDをセクタ数確保。
	セクタサイズのメモリをセクタ数確保。
	Flag用は、次の空きセクタ番号を保持。０なら最後を指す。
	つまり起動時はすべて０。
	確保すると、複数の確保したセクタすべて最上位ビットが立ち、
	次のブロックの先頭セクタ番号が入る。
	ex)
			 <0>    <1>    <2>    <3>    <4>    <5>    <6>
		起動時
			0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		2ブロック確保
			0x8002,0x8002,0x0000,0x0000,0x0000,0x0000,0x0000,
		さらに３ブロック確保
			0x8002,0x8002,0x8005,0x8005,0x8005,0x0000,0x0000,
		最初の2ブロック開放
			0x0002,0x0002,0x8005,0x8005,0x8005,0x0000,0x0000,
		1ブロック確保
			0x8001,0x0002,0x8005,0x8005,0x8005,0x0000,0x0000,

	つまりチェーンと使用フラグとブロック長がわかるようになる。
#endif
#endif
