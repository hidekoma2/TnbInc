#pragma once
/**
 *@file
 * テンポラリ情報アクセス関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbAccessor.h"
#include "TnbMap.h"



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR
 * テンポラリ情報アクセスクラス
 *
 *		セクションで区切られた中に複数のキーと値を持つ形の情報をメモリで管理するクラスです。
 *		プロセスが終了したら、情報は破棄されてしまいます（残りません）。
 *
 *		コンストラクタや SetBase() で情報のベースを指定し、
 *		operator[]() で セクション情報アクセスクラス({@link IAccessor::CSection CSection})を取得し、
 *		Query～() 、 Write～() などでキー情報にアクセスします。
 *
 *	@note	本インターフェースにも QueryValue() , WriteValue() など、
 *			セクションを指定しキー情報を操作するメソッドもありますが、
 *			基本的に {@link IAccessor::CSection CSection} を使用してください。
 *
 *	@see	IAccessor を実装しています。
 *
 *	@par必要ファイル
 *			TnbTemporaryAccessor.h
 *
 *	@date 06/06/09 新規作成
 *	@date 06/10/31 IInformation の改修に対応。
 *	@date 07/05/10 クラス名変更。
 */
class CTemporaryAccessor : public CAbstractAccessor
{
	DEFSUPER(CAbstractAccessor);
	typedef CMapT<CStr, CValue, LPCTSTR> CKeyMap;		///< KEYのマップ型名宣言

	mutable CStr			m_strSecName;				///< アクティブのセクション名		
	mutable CKeyMap*		m_pmapKey;					///< アクティブのセクションマップの参照

protected:

	mutable CMapT<CStr, CKeyMap, LPCTSTR> m_mapSec;	///< セクションマップ

private:

	/**
	 * 参照リセット
	 *	@note セクションマップを操作した時コールします。
	 */
	void m_Reset(void) const
	{
		m_pmapKey = NULL;
	}

	/**
	 * キーマップのアドレス取得
	 *	@retval NULL アクティブのセクションはない
	 *	@retval NULL以外 キーマップのアドレス
	 */
	CKeyMap* m_CheckKeyMapP(void) const
	{
		if ( m_pmapKey == NULL && m_mapSec.HasKey(m_strSecName) )
		{
			m_pmapKey = &(m_mapSec[m_strSecName]);
		}
		return m_pmapKey;
	}

	/**
	 * キーマップのアドレス取得
	 *	@note ない場合、拡張します。
	 *	@return キーマップのアドレス
	 */
	CKeyMap* m_GetKeyMapP(void)
	{
		if ( m_pmapKey == NULL )
		{
			m_pmapKey = &(m_mapSec[m_strSecName]);
		}
		return m_pmapKey;
	}

	/**
	 * セクション名セット
	 */
	void m_SetSectionName(LPCTSTR lpszSectionName) const
	{
		if ( m_strSecName != lpszSectionName )
		{
			m_Reset();
			m_strSecName = lpszSectionName;
		}
	}

	/**
	 * 指定キー値設定
	 *	@param lpszKey キー
	 *	@param v 値
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool m_Write(LPCTSTR lpszKey, const CValue& v)
	{
		CKeyMap* P = m_GetKeyMapP();
		if ( P != NULL )
		{
			(*P)[lpszKey] = v;
			return true;
		}
		return false;
	}

	/**
	 * [削除] 指定キー削除
	 *	@param lpszKey キー名。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool m_Delete(LPCTSTR lpszKey)
	{
		CKeyMap* P = m_CheckKeyMapP();
		return (P != NULL) ? P->RemoveKey(lpszKey) : false;
	}	

public:
	
	/// コンストラクタ
	CTemporaryAccessor(void) : _super(), m_pmapKey(NULL)
	{
	}

	/**
	 * [取得] タイプ名取得
	 *	@return タイプ名
	 */
	virtual CStr GetTypeName(void) const
	{
		return _T("Temp");
	}

	/**
	 * [操作] フラッシュ.
	 *	@note 本クラスでは何もしません。
	 *	@return true 成功
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
		CStrVector vs = m_mapSec.EnumKeys();
		CStr ss = lpszSectionName;
		size_t len = ss.GetLength();
		if ( ! ss.IsEmpty() )
		{
			loop ( i, vs.GetSize() )
			{
				CStr s;
				if ( STRLIB::Compare(vs[i], ss, len, NORM_IGNORECASE) == 0 )
				{
					s = vs[i].Mid(len);
					if ( ! s.IsEmpty() && s[0] == '\\' )
					{
						s = s.Mid(1);
					}
					else
					{
						s.Empty();
					}
				}
				vs[i] = s;
			}
		}
		CStrVector vstrSecNames;
		loop ( i, vs.GetSize() )
		{
			if ( ! vs[i].IsEmpty() )
			{
				CStr s = vs[i];
				INDEX d = s.Find(_T('\\'));
				if ( d != INVALID_INDEX )
				{
					s = s.Left(d);
				}
				if ( vstrSecNames.Find(s) == INVALID_INDEX )
				{
					vstrSecNames.Add(s);
				}
			}
		}
		return vstrSecNames;
	}

	/**
	 * [削除] 指定セクション削除
	 *	@param lpszSectionName セクション名。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool DeleteSection(LPCTSTR lpszSectionName)
	{
		m_Reset();
		CStrVector vs = m_mapSec.EnumKeys();
		CStr ss = lpszSectionName;
		size_t len = ss.GetLength();
		loop ( i, vs.GetSize() )
		{
			if ( STRLIB::Compare(vs[i], ss, len, NORM_IGNORECASE) == 0 )
			{
				if ( ! m_mapSec.RemoveKey(vs[i]) )
				{
					return false;
				}
			}
		}
		return true;
	}

	/**
	 * [取得] キー名一覧取得
	 *	@param lpszSectionName セクション名
	 *	@return キー名一覧。
	 */
	virtual CStrVector EnumKeyNames(LPCTSTR lpszSectionName) const
	{
		m_SetSectionName(lpszSectionName);
		CKeyMap* P = m_CheckKeyMapP();
		return (P == NULL) ? CStrVector() : P->EnumKeys();
	}

	/**
	 * [取得] 情報取種取得
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@return 情報種。ない場合、EK_Nothing になります。
	 */
	virtual EKind GetKeyKind(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		m_SetSectionName(lpszSectionName);
		CKeyMap* P = m_CheckKeyMapP();
		if ( P == NULL )
		{
			return EK_Nothing;
		}
		CValue v;
		return (P->Lookup(lpszKey, v)) ? v.GetKind() : EK_Nothing;
	}

	/**
	 * [取得] 情報取得
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@return キーの値
	 */
	virtual CValue QueryValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const
	{
		m_SetSectionName(lpszSectionName);
		CKeyMap* P = m_CheckKeyMapP();
		CValue v;
		if ( P != NULL )
		{
			P->Lookup(lpszKey, v);
		}
		return v;
	}

	/**
	 * [設定] 情報設定
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@param value 設定する値
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool WriteValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey, const IAccessor::CValue& value)
	{
		m_SetSectionName(lpszSectionName);
		if ( value.GetKind() == EK_Nothing )
		{
			return m_Delete(lpszKey); 
		}
		return m_Write(lpszKey, value);
	}
};



}; // TNB
