#pragma once
/**
 *@file
 * リソースデータ参照関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbCollection.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT WINAPI
 * リソースデータ参照クラス
 *
 *		リソースデータを直接参照するためのクラスです。
 *
 *	@note データは、単純なメモリ読み出ししか行いませんので、スレッドセーブになっています。
 *
 *	@par必要ファイル
 *			TnbConstResourceData.h
 *
 *	@date 09/09/09 新規作成
 */
class CConstResourceData : public IConstCollectionT<BYTE>, public ISequenceCollectionT<BYTE>
{
public:

	/**@ingroup COLLECT WINAPI
	 * リソース値設定補助クラス
	 *
	 *		CConstResourceData::Attach() の引数のサポートクラスです。
	 *		直接インスタンスを定義する必要はありません。
	 *
	 *	@par必要ファイル
	 *			TnbConstResourceData.h
	 *
	 *	@date 09/09/09 新規作成
	 */
	class CValue
	{
	public:

		/**
		 * コンストラクタ.
		 *	@param lpsz 文字列指定
		 */
		CValue(LPCTSTR lpsz) : m_lpsz(lpsz)
		{
		}

		/**
		 * コンストラクタ.
		 *	@param id ID 指定
		 */
		CValue(int id) : m_lpsz(MAKEINTRESOURCE(id))
		{
		}

		/**
		 * [取得] 情報取得.
		 *	@return 情報
		 */
		operator LPCTSTR() const
		{
			return m_lpsz;
		}

	private:
		LPCTSTR m_lpsz;
	};


	//----------------------------


	/// コンストラクタ
	CConstResourceData(void) : m_size(0), m_pData(NULL)
	{
	}

	/** 
	 * [排他] ロック
	 *	@note 本クラスでは何もしません。
	 *	@param dwTime ダミー。
	 *	@return 常に true
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const
	{
		return true;
	}

	/**
 	 * [排他] アンロック.
	 *	@note 本クラスでは何もしません。
	 */
	virtual void Unlock(void) const
	{
	}

	/**
	 * [取得] 要素数取得.
	 *	@return 要素数
	 */
	virtual size_t GetSize(void) const
	{
		return m_size;
	}

	/**
	 * [取得] 要素の参照取得.
	 *	@note	非スレッドセーブメソッドです。
	 *			複数のスレッドからアクセスがある場合、 Lock() / Unlock() を使用してください。
	 *	@param index インデックス
	 *	@return 要素の参照
	 *	@throw CIndexOutOfBoundsException インデックスが範囲外であることを示すためにスローされます。
	 */
	virtual const BYTE& At(INDEX index) const
	{
		if ( IsInRange(index) )
		{
			return m_pData[index];
		}
		throw CIndexOutOfBoundsException();
	}

	/**
	 * [取得] データアドレス取得.
	 *	@note	取得したアドレスは delete で開放してはいけません。
	 *	@note	取得したアドレスは Load Unload すると無効になります。
	 *	@return アドレス。確保していない場合、NULLが返ります。
	 */
	virtual const BYTE* ReferBuffer(void) const
	{
		return m_pData;
	}

	/**
	 * [設定] アタッチ.
	 *		指定のリソースのデータを、本インスタンスにアタッチします。
	 *	@param name リソース名を指定します。文字列でも ID (数値) でも指定可能です。
	 *	@param type リソースタイプを指定します。文字列でも ID (数値) でも指定可能です。
	 *			省略すると、 RT_RCDATA が適用されます。\n
	 *			リソースタイプは、以下の標準の値も使用できます。
	 *			<table>
	 *			<tr> <td>値</td>				<td>意味</td>										</tr>
	 *			<tr> <td>RT_ACCELERATOR</td>	<td>アクセラレータテーブル</td>						</tr>
	 *			<tr> <td>RT_ANICURSOR</td>		<td>アニメーションカーソル</td>						</tr>
	 *			<tr> <td>RT_ANIICON</td>		<td>アニメーションアイコン</td>						</tr>
	 *			<tr> <td>RT_BITMAP</td>			<td>ビットマップリソース</td>						</tr>
	 *			<tr> <td>RT_CURSOR</td>			<td>ハードウェア依存のカーソルリソース</td>			</tr>
	 *			<tr> <td>RT_DIALOG</td>			<td>ダイアログボックス</td>							</tr>
	 *			<tr> <td>RT_FONT</td>			<td>フォントリソース</td>							</tr>
	 *			<tr> <td>RT_FONTDIR</td>		<td>フォントディレクトリリソース</td>				</tr>
	 *			<tr> <td>RT_GROUP_CURSOR</td>	<td>ハードウェアに依存しないカーソルリソース</td>	</tr>
	 *			<tr> <td>RT_GROUP_ICON</td>		<td>ハードウェアに依存しないアイコンリソース</td>	</tr>
	 *			<tr> <td>RT_HTML</td>			<td>HTML ドキュメント</td>							</tr>
	 *			<tr> <td>RT_ICON</td>			<td>ハードウェア依存のアイコンリソース</td>			</tr>
	 *			<tr> <td>RT_MENU</td>			<td>メニューリソース</td>							</tr>
	 *			<tr> <td>RT_MESSAGETABLE</td>	<td>メッセージテーブルのエントリ</td>				</tr>
	 *			<tr> <td>RT_RCDATA</td>			<td>アプリケーション定義のリソース（ 生データ）</td></tr>
	 *			<tr> <td>RT_STRING</td>			<td>文字列テーブルのエントリ</td>					</tr>
	 *			<tr> <td>RT_VERSION</td>		<td>バージョンリソース</td>							</tr>
	 *			</table>
	 *	@param hModule インスタンスハンドルを指定します。省略すると、現在のプロセスのハンドルが使用されます。
	 *	@param wLanguage 言語。省略すると、現在のスレッドに関連付けられている言語が使用されます。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool Attach(CValue name, CValue type = RT_RCDATA, HMODULE hModule = NULL, WORD wLanguage = 0)
	{
		Detach();
		if ( hModule == NULL )
		{
			hModule = GetInstanceHandleByTnb();
		}
		HRSRC h = NULL;
		if ( wLanguage == 0 )
		{
			h = ::FindResource(hModule, name, type);
		}
		else
		{
			h = ::FindResourceEx(hModule, type, name, wLanguage);
		}
		if ( h != NULL )
		{
			HGLOBAL hGM = ::LoadResource(hModule, h);
			m_size = ::SizeofResource(hModule, h);
			m_pData = static_cast<const BYTE*>(::LockResource(hGM));
		}
		return ! IsEmpty();
	}

	/**
	 * [設定] デタッチ.
	 *		リソースデータをデタッチします。本インスタンスのサイズは０になります。
	 */
	void Detach(void)
	{
		m_size = 0;
		m_pData = NULL;
	}

private:
	size_t		m_size;		///< サイズ
	const BYTE*	m_pData;	///< データ
};



}; // TNB

