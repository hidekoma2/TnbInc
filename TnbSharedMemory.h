#pragma once
/**
 *@file
 * 共有メモリ関係のヘッダ
 *
 *		異なるプロジェクトで情報を共有するためのものです。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



/**@ingroup MEMORY
 * 簡易共有メモリ管理クラス
 *
 *		一つの共有メモリを保持します。
 *
 *	@par必要ファイル
 *			TnbSharedMemory.h
 * 
 *	@date 06/01/01 新規
 *	@date 10/12/01 コンストラクタの引数の説明文修正。デストラクタで MAP は開放するように修正。
 */
class CSharedMemoryManager : CCopyImpossible
{
//protected:
	HANDLE	m_hFileMap;			///< 共有メモリのハンドル
	LPVOID	m_lpvMapAdds;		///< マップしたアドレス
	bool	m_boCanClose;		///< デストラクタ時の動作
public:

	/**
	 * コンストラクタ
	 *	@note この段階では共有メモリは保持していません。
	 *	@param boCanClose false を指定すると、デストラクタ時、共有メモリをクローズしません。
	 */
	explicit CSharedMemoryManager(bool boCanClose = false)
	{
		m_lpvMapAdds = NULL;
		m_hFileMap = NULL;
		m_boCanClose = boCanClose;
	}

	/** デストラクタ */
	virtual ~CSharedMemoryManager(void)
	{
		if ( m_boCanClose )
		{
			Close();
		}
		else
		{
			if ( m_lpvMapAdds != NULL )
			{
				::UnmapViewOfFile(m_lpvMapAdds);
			}
		}
	};

	/**
	 * [確認] 共有メモリの有無
	 *	@retval true 持っている
	 *	@retval false 持っていない(Openしていない)
	 */
	bool HasMemory(void)
	{
		return Get() != NULL;
	}

	/**
	 * [取得] アドレス参照.
	 *		本クラスで管理している共有メモリのアドレスを返します。
	 *	@retval	NULL		Openしていない
	 *	@retval NULL以外	メモリのアドレス
	 */
	LPVOID Get(void)
	{
		if ( m_lpvMapAdds != NULL )
		{
			// このクラスの共有メモリはすでにオープンしている
			return m_lpvMapAdds;
		}
		return NULL;
	}

	/**
	 * [操作] オープン
	 *	@note すでにオープンしていたらCloseしてからオープンします
	 *	@param lpszMemName 共有メモリ名。インスタンス内のみ有効な共有メモリになります。
	 *	@param uMemSize 共有メモリサイズ。
	 *	@retval	-1		Open失敗
	 *	@retval 0		Open成功（初めての作成）
	 *	@retval 1		Open成功（既存メモリに接続。uMemSizeは無視されました）
	 */
	int Open(LPCTSTR lpszMemName, UINT uMemSize)
	{
		if ( m_lpvMapAdds != NULL )
		{
			// このクラスの共有メモリはすでにオープンしている
			Close();
		}
		// 共有メモリハンドルオープン
		m_hFileMap = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, uMemSize, lpszMemName);
		if ( m_hFileMap != NULL )
		{
			// 共有メモリオープン成功！
			DWORD dwError = ::GetLastError();
			//　マッピングする。
			m_lpvMapAdds = ::MapViewOfFile(m_hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if ( m_lpvMapAdds != NULL )
			{
				return (dwError == ERROR_ALREADY_EXISTS) ? 1/*既存*/ : 0/*新規*/;
			}
			// マッピング失敗
			_GetLastError("MapViewOfFile");
			::CloseHandle(m_hFileMap);		// ハンドルを閉じなくては
			m_lpvMapAdds = NULL;
		}
		else
		{
			// 共有メモリオープン失敗
			_GetLastError("CreateFileMapping");
			m_lpvMapAdds = NULL;
		}
		return -1;
	}

	/**
	 * [操作] クローズ.
	 *		本クラスで管理している共有メモリをクローズします
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	bool Close(void)
	{
		if ( m_lpvMapAdds == NULL )
		{
			// 開いていなかった（未使用だった）
			return true;
		}
		if ( ! ::UnmapViewOfFile(m_lpvMapAdds) )
		{
			//アンマップド失敗
			return false;
		}
		m_lpvMapAdds = NULL;
		if ( m_hFileMap != 0 )	//念のためチェック
		{
			if ( ! ::CloseHandle(m_hFileMap) )
			{
				//ハンドルがクローズできなかった			
				return false;
			}
		}
		return true;
	}
};



/**@ingroup MEMORY
 * 共有構造体管理テンプレート
 *
 *		一つの指定の構造体型の共有メモリを保持します。
 *
 *		オープン時リスナー( IListener )を指定することで、初めてのオープンの時だけ
 *		共有メモリの初期化ができます。
 *		
 *		Openしたインスタンスは指定した型のポインタとして操作が可能です。
 *
 *	@note	共有メモリにおくので、コンストラクタが動かなくても問題のない型を
 *			指定してください。
 *	
 *	@param TYP 共有メモリにおく構造体（ 'operator→' を使っているので intなどは使えません）
 *
 *	@par使用例
 *	<table><tr><td><pre>
 *
 *		struct ISM : CSharedMemoryT<Bool>::IListener
 *		{
 *		    virtual void OnAction(Bool* P)
 *		    {
 *		        *P = false;
 *		    }
 *		} ism;
 *		CSharedMemoryT<Bool> ll1;
 *		CSharedMemoryT<Bool> ll2;
 *		ll1.Open( _T("AAA"), &ism );
 *		ll2.Open( _T("AAA"), &ism ); //同じ共有メモリ名なのでリスナーは呼ばれない
 *		ASSERT( ll2->IsFalse() ); //（TYPのポインタと同じ記述が可能）
 *		(*ll1) = true;
 *		ASSERT( ll2->IsTrue() ); //同じメモリを指しているので影響がある
 *	</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbSharedMemory.h
 * 
 *	@date 06/01/01 新規作成
 *	@date 06/04/20 リスナー追加
 *	@date 06/04/26 テンプレート名変更。最後の一つは消さないオプション追加。
 *	@date 06/09/12 constを考慮。
 */
template<typename TYP>
class CSharedStructT : CCopyImpossible
{
	CSharedMemoryManager m_cSharedMem;	///< 共有メモリ管理
	TYP*	m_pType;					///< 対象のメモリアドレス
	bool	m_boCanClose;				///< trueなら Close時に共有メモリをClose

public:

	/**@ingroup MEMORY
	 * 共有構造体管理のリスナーインターフェース
	 *
	 *		初めてのOpen,最後のClose時の通知を行います。
	 *
	 *	@see CSharedStructT::Open()
	 *	@see CSharedStructT::Close()
	 *
	 *	@par必要ファイル
	 *			TnbSharedMemory.h
	 * 
	 *	@date 06/04/20 新規作成
	 */
	struct IListener
	{
		/** デストラクタ */
		virtual ~IListener(void) {}

		/**
		 * [通知] アクション発生
		 *	@note Openで使用した場合、初めて作成された時にコールされる。
		 *	@note Closeで使用した場合、共有メモリを破棄する直前にコールされる。
		 *	@param P 確保したメモリのポインタ
		 */
		virtual void OnAction(TYP* P) = 0;
	};

	/**
	 * コンストラクタ
	 *	@note この段階では共有メモリは保持していません。
	 *	\par重要:
	 *			共有メモリは、すべての参照がクローズされると、破棄されます。
	 *			しかし、boCanClose にfalseを指定しておくと、本クラスで一度オープンした
	 *			共有メモリは、プロセスが終了するまでクローズされなくなります。		<BR>
	 *			システムとしてグローバルに情報を保持し続ける必要がある場合に、
	 *			このオプションを使用するか、本インスタンスの一つをグローバルに
	 *			持ち続けてください。
	 *	@param boCanClose false ならクローズ時共有メモリを閉じない。trueか省略なら閉じる。
	 */
	explicit CSharedStructT(bool boCanClose = true) : m_cSharedMem(boCanClose)
	{
		m_boCanClose = boCanClose;
		m_pType = NULL;
	}

	/** デストラクタ */
	virtual ~CSharedStructT(void)
	{
		Close();
	}

	/**
	 * [確認] 共有メモリの有無
	 *	@retval true 持っている
	 *	@retval false 持っていない(Openしていない)
	 */
	bool HasMemory(void) const
	{
		return m_pType != NULL;
	}

	/**
	 * [取得] メモリの参照.
	 *	@note OpenしていないとNULLが返ります。
	 *	@return アドレス
	 */
	operator TYP*(void)
	{
		return m_pType;
	}

	/**
	 * [取得] メモリの参照.
	 *	@note OpenしていないとNULLが返ります。
	 *	@return アドレス
	 */
	operator const TYP*(void) const
	{
		return m_pType;
	}

	/**
	 * [取得] メモリの参照.
	 *	@note OpenしていないとNULLが返ります。
	 *	@return アドレス
	 */
	TYP* operator->(void)
	{
		return m_pType;
	}

	/**
	 * [取得] メモリの参照.
	 *	@note OpenしていないとNULLが返ります。
	 *	@return アドレス
	 */
	const TYP* operator->(void) const
	{
		return m_pType;
	}

	/**
	 * [操作] オープン
	 *	@note	最初にオープンするとリスナーがコールされます。リスナー実行中は、
	 *			二つ目以降のオープンはブロックされます。そのため、リスナーによる初期化は
	 *			保障されます。
	 *	@note	本クラスですでにオープンしていたらCloseしてからオープンします。
	 *	@param lpMemName 共有メモリ名。
	 *			省略するとインスタンス内のみ有効な共有メモリになります。
	 *			他のTYPと同じ名前にならないようにしてください。
	 *	@param piListener リスナー。省略可能。指定すると初めてOpenした時に OnAction がコールされます。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool Open(LPCTSTR lpMemName = NULL, IListener* piListener = NULL)
	{
		int iRc = m_cSharedMem.Open(lpMemName, sizeof(LONG) * 2 + sizeof(TYP));
		if ( iRc < 0 )
		{
			return false;
		}
		LONG* P = static_cast<LONG*>(m_cSharedMem.Get());
		m_pType = reinterpret_cast<TYP*>(&P[2]);
		if ( iRc == 0 )
		{
			//==新規
			if ( piListener != NULL )
			{
				piListener->OnAction(m_pType);
			}
			P[1]++;	//参照カウンタ
			P[0] = 1;	//初期化済み
		}
		else
		{
			//==既存
			while ( P[0] != 1 )
			{
				::Sleep(1); 
			}
			P[1]++;	//参照カウンタ
		}
		return true;
	}

	/** 
	 * [操作] クローズ
	 *	@param piListener リスナー。省略可能。指定すると破棄直前にOnActionがコールされます。
	 *					ただし、コンストラクタで boCanClose に false を指定していると、最後の
	 *					クローズはしないので、OnAction()は絶対コールされません。
	 */
	void Close(IListener* piListener = NULL)
	{
		LONG* P = static_cast<LONG*>(m_cSharedMem.Get());
		if ( P == NULL ){ return; }
		while ( true )
		{
			while ( P[0] != 1 )
			{
				::Sleep(1); 
			}
			if ( ::InterlockedDecrement(&P[0]) == 0 )
			{
				break;
			}
		}
		if ( ::InterlockedDecrement(&P[1]) <= 0 )
		{
			//最後の一つ
			if ( piListener != NULL )
			{
				piListener->OnAction(m_pType);
			}
			if ( ! m_boCanClose )
			{
				//閉じない、だからカウンタは１のままに
				P[1] = 1;
			}
			P[0] = 1;
			if ( m_boCanClose )
			{
				//閉じる
				m_cSharedMem.Close();
			}
		}
		else
		{
			P[0] = 1;
			m_cSharedMem.Close();
		}
		m_pType = NULL;
	}
};



}; // TNB
