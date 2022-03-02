#pragma once
/**
 *@file
 * iniファイル形式テキスト情報アクセス関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbTemporaryAccessor.h"



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR
 * iniファイル形式テキスト情報アクセスクラス
 *
 *		セクションで区切られた中に複数のキーと値を持つ形の情報をメモリで管理し、
 *		GetText() でテキスト化し、 SetText() で情報を復元することが出来るクラスです。
 *
 *		operator[]() で セクション情報アクセスクラス({@link IAccessor::CSection CSection})を取得し、
 *		Query～() 、 Write～() などでキー情報にアクセスします。
 *
 *	@note	本インターフェースにも QueryValue() , WriteValue() など、
 *			セクションを指定しキー情報を操作するメソッドもありますが、
 *			基本的に {@link IAccessor::CSection CSection} を使用してください。
 *
 *	@see	IIAccessor を実装しています。
 *
 *	@par必要ファイル
 *			TnbInifileTextAccessor.h
 *
 *	@date 12/04/26 新規作成
 */
class CInifileTextAccessor : public CTemporaryAccessor
{
	DEFSUPER(CTemporaryAccessor);
public:

	/**
	 * [設定] テキスト設定.
	 *	@param lpszText テキスト
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetText(LPCTSTR lpszText)
	{
		_super::DeleteAllSection();
		CStrVector vs = CStrOperator::SeparateLine(lpszText);
		//
		CStr sec;
		loop ( i, vs.GetSize() )
		{
			const CStr& a = vs[i];
			if ( a.IsEmpty() )
			{
				continue;
			}
			if ( a[0] == _T('[') )
			{
				int ofs = a.Find(_T(']'), 1);
				if ( ofs >= 0 )
				{
					sec = a.Mid(1, ofs - 1);
				}
				continue;
			}
			int ofs = a.Find('=');
			if ( ofs >= 0 )
			{
				CStr val;
				CStr key = a.FindCut(_T('='), &val);
				IAccessor::CValue vv(val);
				if ( ! _super::WriteValue(sec, key, vv) )
				{
					return false;
				}
			}
		}
		return true;
	}

	/**
	 * [取得] テキスト取得.
	 *	@return テキスト
	 */
	CStr GetText(void) const
	{
		CStr data;
		CStrVector sections = _super::EnumSectionNames();
		loop ( i, sections.GetSize() )
		{
			const CStr& sec = sections[i];
			data += CStr::Fmt(_T("[%s]\r\n"), sec);
			CStrVector keys = _super::EnumKeyNames(sec);
			loop ( i, keys.GetSize() )
			{
				const CStr& key = keys[i];
				CValue v = _super::QueryValue(sec, key);
				if ( ! v.IsNull() )
				{
					data += CStr::Fmt(_T("%s=%s\r\n"), key, v.QueryString());
				}
			}
		}
		return data;
	}

	/**
	 * [取得] タイプ名取得
	 *	@return タイプ名
	 */
	virtual CStr GetTypeName(void) const
	{
		return _T("Inifile");
	}

	/**
	 * [操作] フラッシュ.
	 *	@note 本クラスではなにもしません。
	 *	@return 常に true
	 */
	virtual bool Flush(void)
	{
		return true;
	}
};



};//TNB

