#pragma once
/**
 *@file
 * アイコン管理関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbMap.h"
#include "TnbStr.h"
#include <Shellapi.h>


//TNB Library
namespace TNB
{



/**@ingroup ICON
 * アイコン管理クラス
 *
 *		本クラスを使用してアイコンをロードすると、開放すべきアイコンの開放漏れを防ぎます。
 *		また、リソースIDとハンドルを関連付けて記憶しておくため、キャッシュ的な使い方が可能です。
 *
 *		カーソルもアイコンと同様に本クラスで管理することが可能です。
 *
 * @note	本クラスのインスタンスで取得した アイコンハンドルは、使用後、 APIの DestroyIcon() を使用せず、
 *			Destroy() を使用し破棄します(インスタンスを削除してもハンドルは破棄されます) 。
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *		
 *		CIconManager m_icon;
 *				;
 *		void foo1(){
 *		   HICON h1 = m_icon.LoadResorce(-IDI_MAIN);  //小さいアイコンハンドル
 *		       ;	
 *		   HICON h2 = m_icon[-IDI_MAIN];   // さっきLoadしたハンドルが得られます。		
 *		       ;
 *		}
 *		</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbIconManager.h
 *
 *	@date 06/06/14 新規作成
 *	@date 07/10/29 構造整理
 */
class CIconManager : CCopyImpossible
{
	CMapT<int, HICON>		m_mapIdIcon;		///< 消す必要のあるHICON
	CVectorT<HICON>			m_vIcon;			///< 消す必要のあるHICON
	/// ハンドルロード
	HICON m_Load(int iIconResourceId, HINSTANCE hIns)
	{
		LPCTSTR lpsz = NULL;
		int cx = 0;
		int cy = 0;
		if ( iIconResourceId < 0 )
		{
			lpsz = MAKEINTRESOURCE(-iIconResourceId);
			cx = ::GetSystemMetrics(SM_CXSMICON);	// スモールアイコンの幅
			cy = ::GetSystemMetrics(SM_CYSMICON); 
		}
		else
		{
			lpsz = MAKEINTRESOURCE(iIconResourceId);
		}
		HANDLE r = ::LoadImage(hIns, lpsz, IMAGE_ICON, cx, cy, LR_DEFAULTCOLOR);
		if ( r == NULL )
		{
			r = ::LoadImage(hIns, lpsz, IMAGE_CURSOR, cx, cy, LR_DEFAULTCOLOR);
		}
		return static_cast<HICON>(r);
	}
protected:

	/**
	 * [破棄] ハンドル破棄.
	 *	@note ハンドルがアイコンかカーソルかチェックして破棄します。
	 *	@param h アイコンかカーソルのハンドル
	 */
	void m_Destroy(HICON h)
	{
#ifndef _WIN32_WCE
		ICONINFO ii;
		if ( ::GetIconInfo(h, &ii) )
		{
			ii.fIcon ? ::DestroyIcon(h) : ::DestroyCursor(h);
		}
#else
		::DestroyIcon(h);
#endif
	}

public:

#ifndef _WIN32_WCE

	/** システムのアイコンID値
	 * @note LoadSystem() で使用します。 */
	enum ESystemIconId
	{
		E_APPLICATION	= (INT_PTR)(IDI_APPLICATION),	///< 既定のアプリケーションアイコン 
		E_ASTERISK		= (INT_PTR)(IDI_ASTERISK),		///< E_INFORMATION と同じです 
		E_ERROR			= (INT_PTR)(IDI_ERROR),			///< ストップマークのアイコン（米国の歩行者用信号の赤と同じで、手を開いて制止しているデザインです） 
		E_EXCLAMATION	= (INT_PTR)(IDI_EXCLAMATION),	///< E_WARNING と同じです 
		E_HAND			= (INT_PTR)(IDI_HAND),			///< E_ERROR と同じです 
		E_INFORMATION	= (INT_PTR)(IDI_INFORMATION),	///< 情報アイコン（吹き出しの中に i の文字） 
		E_QUESTION		= (INT_PTR)(IDI_QUESTION),		///< 疑問符のアイコン 
		E_WARNING		= (INT_PTR)(IDI_WARNING),		///< 感嘆符のアイコン 
		E_WINLOGO		= (INT_PTR)(IDI_WINLOGO),		///< Windows ロゴのアイコン 
	};

#endif

	/// コンストラクタ
	CIconManager(void) 
	{
	}

	/// デストラクタ
	virtual ~CIconManager(void)
	{
		DestroyAll();
	}

	/**
	 * [開放] 全ハンドル開放.
	 *		本インスタンスで管理しているハンドルをすべて開放します。
	 */
	void DestroyAll(void)
	{
		const CMapT<int, HICON>::CConstValuesAdapter& avu = m_mapIdIcon.GetValuesAdapter();
		loop ( i, avu.GetSize() )
		{
			m_Destroy(avu.At(i));
		}
		loop ( i, m_vIcon.GetSize() )
		{
			m_Destroy(m_vIcon[i]);
		}
		m_mapIdIcon.RemoveAll();
		m_vIcon.RemoveAll();
	}

	/**
	 * [開放] ハンドル開放.
	 *	@param iIconResourceId 本インスタンスでロードしたリソースID。
	 *	@retval true 成功。
	 *	@retval false 失敗（本インスタンスで管理していないリソースIDが指定された）。
	 */
	bool Destroy(int iIconResourceId)
	{
		if ( m_mapIdIcon.HasKey(iIconResourceId) )
		{
			m_Destroy(m_mapIdIcon[iIconResourceId]);
		}
		return m_mapIdIcon.RemoveKey(iIconResourceId);
	}

	/**
	 * [開放] ハンドル開放.
	 *	@param hIcon 本インスタンスで管理しているアイコンハンドル。
	 *	@retval true 成功。
	 *	@retval false 失敗（本インスタンスで管理していないハンドルが指定された）。
	 */
	bool Destroy(HICON hIcon)
	{
		const CMapT<int, HICON>::CConstValuesAdapter& av = m_mapIdIcon.GetValuesAdapter();
		INDEX i = av.Find(hIcon);
		if ( i != INVALID_INDEX )
		{
			m_mapIdIcon.Remove(i);
			m_Destroy(hIcon);
			return true;
		}
		i = m_vIcon.Find(hIcon);
		if ( i != INVALID_INDEX )
		{
			m_vIcon.Remove(i);
			m_Destroy(hIcon);
			return true;
		}
		return false;
	}

	/**
	 * [取得] リソースから取得
	 *	@param iIconResourceId アイコンリソースID。 ×-1 した値を指定することで、 小さい ICON をロードできます。
	 *	@param hIns インスタンス。
	 *	@retval NULL以外 アイコンハンドル。 APIの DestroyIcon() は使用しないでください。
	 *	@retval NULL エラー（存在しない）
	 */
	HICON LoadResource(int iIconResourceId, HINSTANCE hIns)
	{
		HICON hIcon = NULL;
		if ( ! m_mapIdIcon.Lookup(iIconResourceId, hIcon) )
		{
			hIcon = m_Load(iIconResourceId, hIns);
			if ( hIcon != NULL)
			{
				m_mapIdIcon[iIconResourceId] = hIcon;
			}
		}
		return hIcon;
	}

	/**
	 * [取得] リソースから取得
	 *	@note インスタンスハンドルは、 GetInstanceHandle(EI_Icon) の値が使用されます。
	 *	@param iIconResourceId アイコンリソースID。 ×-1 した値を指定することで、 小さい ICON をロードできます。
	 *	@retval NULL以外 アイコンハンドル。 APIの DestroyIcon() は使用しないでください。
	 *	@retval NULL エラー（存在しない）
	 */
	HICON LoadResource(int iIconResourceId)
	{
		return LoadResource(iIconResourceId, GetInstanceHandleByTnb(EI_Icon));
	}

	/**
	 * [取得] リソースから取得
	 *	@note インスタンスを指定しない LoadResource() と同じ動きをします。
	 *	@param iIconResourceId アイコンリソースID
	 *	@retval NULL以外 アイコンハンドル。 APIの DestroyIcon() は使用しないでください。
	 *	@retval NULL エラー（存在しない）
	 */
	HICON operator[](int iIconResourceId)
	{
		return LoadResource(iIconResourceId);
	}

#ifndef _WIN32_WCE

	/**
	 * [取得] システムから取得
	 *	@param systemId システムアイコンID。
	 *	@retval NULL以外 アイコンハンドル。 APIの DestroyIcon() は使用しないでください。
	 *	@retval NULL エラー（存在しない）
	 */
	HICON LoadSystem(ESystemIconId systemId)
	{
		return ::LoadIcon(NULL, reinterpret_cast<LPCTSTR>(systemId));
	}

	/**
	 * [取得] ファイルから抽出可能数.
	 *	@note 指定のファイル内のリソースからアイコンを取得します。
	 *	@note インスタンスハンドルは、 GetInstanceHandle(EI_Process) の値が使用されます。
	 *	@param lpszFile ファイル名。ファイルは、 EXE DLL ICO ファイルが指定できます。
	 *	@retval 1以上 ファイル内のアイコン数。
	 *	@retval 0 エラー（存在しない）
	 */
	INT_PTR ExtractNumber(LPCTSTR lpszFile)
	{
		return reinterpret_cast<INT_PTR>(::ExtractIcon(GetInstanceHandleByTnb(EI_Process), lpszFile, UINT_MAX));
	}

	/**
	 * [取得] ファイルから抽出.
	 *	@note 指定のファイル内のリソースからアイコンを取得します。
	 *	@note インスタンスハンドルは、 GetInstanceHandle(EI_Process) の値が使用されます。
	 *	@param lpszFile ファイル名。ファイルは、 EXE DLL ICO ファイルが指定できます。
	 *	@param iconIndex インデックス。 ExtractNumber() で得られる「アイコン数」未満を指定できます。
	 *	@retval NULL以外 アイコンハンドル。 APIの DestroyIcon() は使用しないでください。
	 *	@retval NULL エラー（存在しない）
	 */
	HICON Extract(LPCTSTR lpszFile, UINT iconIndex)
	{
		HICON hIcon = ::ExtractIcon(GetInstanceHandleByTnb(EI_Process), lpszFile, iconIndex);
		if ( hIcon != NULL )
		{
			m_vIcon.Add(hIcon);
		}
		return hIcon;
	}

#endif

	/**
	 * [指定] ハンドル管理.
	 *		指定のハンドルを本インスタンスで管理するようにします。
	 *	@note 本クラス以外でアイコンハンドルを作成し、インスタンスで管理させたい場合、本メソッドを使います。
	 *	@param hIcon アイコンハンドル。 ここで指定したアイコンハンドルは APIの DestroyIcon() を使用しないでください。
	 */
	void Attach(HICON hIcon)
	{
		const CMapT<int, HICON>::CConstValuesAdapter& av = m_mapIdIcon.GetValuesAdapter();
		if ( av.Find(hIcon) == INVALID_INDEX )
		{
			if ( m_vIcon.Find(hIcon) == INVALID_INDEX )
			{
				m_vIcon.Add(hIcon);
			}
		}
	}


#if 0
	/**
	 * [取得] アイコンハンドル取得.
	 *	@note 指定のファイル内のリソースからアイコンを取得します。
	 *	@param lpszFile ファイル名。フォルダも指定可能。
	 *	@param uAddFlags	追加フラグ。以下のシンボルを選択可能。<BR>
	 *						SHGFI_LARGEICON : 大きいサイズのアイコン<BR>
	 *						SHGFI_SMALLICON : 小さいサイズのアイコン<BR>
	 *						SHGFI_OPENICON  : オープン状態のアイコン<BR>
	 *	@retval NULL以外 アイコンハンドル。 APIの DestroyIcon() は使用しないでください。
	 *	@retval NULL エラー（存在しない）
	 */
	HICON LoadFileInfo(LPCTSTR lpszFile, UINT uAddFlags = SHGFI_LARGEICON)
	{
		SHFILEINFO info = { 0 };
		UINT uFlags = SHGFI_ICON | uAddFlags;
		::SHGetFileInfo(lpszFile, 0, &info, sizeof(SHFILEINFO), uFlags);
		if ( info.hIcon != NULL )
		{
			m_vIcon.Add(info.hIcon);
		}
		return info.hIcon;
	}
#endif
};



};///TNB

