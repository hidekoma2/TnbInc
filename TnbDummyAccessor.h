#pragma once
/**
 *@file
 * ダミー情報アクセス関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbAccessor.h"



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR
 * ダミー情報アクセスクラス
 *
 *		情報を一切もたないアクセス管理です。
 *		Query時は指定のデフォルトの値を返し、Writeは何もしませんが成功を返します。
 *
 *		IAccessor が必要で、何もする必要がない場合、デフォルトを使いたい場合などに使用できます。
 *
 *	@see	IAccessor を実装しています。
 *
 *	@par必要ファイル
 *			TnbDummyAccessor.h
 *
 *	@date 08/08/25 新規作成
 */
class CDummyAccessor : public CAbstractAccessor
{
	DEFSUPER(CAbstractAccessor);
public:
	/**
	 * [取得] タイプ名取得
	 *	@return タイプ名
	 */
	virtual CStr GetTypeName(void) const
	{
		return _T("Dummy");
	}

	/**
	 * [操作] フラッシュ.
	 *	@note Writeした情報を確定させます（Subクラスにより動作がことなります。）。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool Flush(void)
	{
		return true;
	}

	/**
	 * [取得] セクション名一覧取得
	 *	@param lpszSectionName セクション名。 NULLを指定すると、ROOTのセクション一覧が得られます。
	 *	@return セクション名一覧
	 */
	virtual CStrVector EnumSectionNames(LPCTSTR lpszSectionName = NULL) const
	{
		return CStrVector();
	}

	/**
	 * [削除] 指定セクション削除
	 *	@param lpszSectionName セクション名。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool DeleteSection(LPCTSTR lpszSectionName)
	{
		return true;
	}

	/**
	 * [取得] キー名一覧取得
	 *	@param lpszSectionName セクション名
	 *	@return 名前一覧。無効状態なら未対応。
	 */
	virtual CStrVector EnumKeyNames(LPCTSTR lpszSectionName) const
	{
		return CStrVector();
	}

	/**
	 * [取得] 情報取種取得
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@return 情報種。キーが存在しない場合、EK_Nothing になります。
	 */
	virtual EKind GetKeyKind(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		return EK_Nothing;
	}

	/**
	 * [取得] 情報取得
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@return キーの値
	 */
	virtual CValue QueryValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		return lpszKey;
	}

	/**
	 * [設定] 情報設定
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@param value 設定する値。EK_Nothing の場合、キーを削除します。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool WriteValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey, const IAccessor::CValue& value)
	{
		return true;
	}
};



}; //TNB
