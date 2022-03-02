#pragma once
/**
 *@file
 * TinyMacroLang 変数管理関係のヘッダ
 *	
 *		TMLの変数管理のクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbTmlValue.h"



//TNB Library
namespace TNB
{



/**@ingroup TML
 * TinyMacroLang 変数管理
 *
 *		レベルをもって変数名/配列名、および、内容を管理します。
 *	@note ローカルエリアでの宣言などで、生存期間の異なるものを管理するためにレベル管理しています。
 *
 *	@par必要ファイル
 *			TnbTmlVariableManager.h
 *
 *	@date 06/01/01 新規作成
 *	@date 09/08/18 改新
 */
class CTmlVariableManager
{
public:

	/// TinyMacroLang 変数名と内容の型
	struct TVarValue
	{
		CAscii		name;	///< 変数名
		CTmlValue	value;	///< 内容
	};

	/// 変数名と内容の配列型
	typedef CVectorT<TVarValue> CVaVaVector;

	/// 変数名と内容の二次元配列型
	typedef CVectorT<CVaVaVector> CVaVaVectors;

	/// TinyMacroLang 配列名と内容型
	struct TArrayValue
	{
		CAscii				name;	///< 配列名
		CTmlValuesVector	values;	///< 内容
	};

	/// 配列名と内容の配列型
	typedef CVectorT<TArrayValue> CArVaVector;

	/// 配列名と内容の二次元配列型
	typedef CVectorT<CArVaVector> CArVaVectors;


	//-----------------------------


	/// コンストラクタ
	CTmlVariableManager(void)
	{
	}

	/**
	 * コピーコンストラクタ.
	 *	@param other コピー元
	 */
	CTmlVariableManager(const CTmlVariableManager& other) : m_aVava(other.m_aVava), m_aArva(other.m_aArva)
	{
	}

	/**
	 * [取得] 変数情報参照.
	 *		変数名と内容の二次元配列の参照
	 *	@return 変数情報
 	 */
	CVaVaVectors& ReferVarArrays(void)
	{
		return m_aVava;
	}

	/**
	 * [取得] 配列情報参照.
	 *		配列名と内容の二次元配列の参照
	 *	@return 変数情報
	 */
	CArVaVectors& ReferArrayArrays(void)
	{
		return m_aArva;
	}

	/**
	 * [取得] 現在のレベル取得.
	 * @return 現在のレベル
	 */
	INT_PTR GetLevel(void) const
	{
		return m_aVava.GetSize();
	}

	/**
	 * [検索] 変数名の検索
	 *		変数名に対になる値管理のポインタを返します。
	 *	@param lpszName 変数名
	 *	@retval NULL 変数名なし。
	 *	@retval NULL以外 値管理のポインタ。
	 */
	CTmlValue* FindValue(LPCSTR lpszName)
	{
		INT_PTR iDim1 = m_aVava.GetSize();
		for( INT_PTR i = iDim1 - 1; i >= 0; i-- )
		{
			CVaVaVector& va = m_aVava.Ref(i);
			INT_PTR iDim2 = va.GetSize();
			for( INT_PTR j = 0; j < iDim2; j++ )
			{
				TVarValue& T = va.Ref(j);
				if ( T.name.Compare(lpszName) == 0 )
				{
					return &T.value;
				}
			}
		}
		return NULL;
	}

	/**
	 * [検索] 配列名の検索
	 *		配列名に対になる配列管理のポインタを返します。
	 *	@param lpszName 変数名
	 *	@retval NULL 変数名なし。
	 *	@retval NULL以外 配列管理のポインタ。
	 */
	CTmlValuesVector* FindArray(LPCSTR lpszName)
	{
		INT_PTR iDim1 = m_aArva.GetSize();
		for( INT_PTR i = iDim1 - 1; i >= 0; i-- )
		{
			CArVaVector& va = m_aArva.Ref(i);
			INT_PTR iDim2 = va.GetSize();
			for( INT_PTR j = 0; j < iDim2; j++ )
			{
				TArrayValue& T = va.Ref(j);
				if ( T.name.Compare(lpszName) == 0 )
				{
					return &T.values;
				}
			}
		}
		return NULL;
	}

	/** 
	 * [設定] Empty
	 */
	void Empty(void)
	{
		m_aVava.RemoveAll();
		m_aArva.RemoveAll();
	}

	/**
	 * [設定] レベル＋１.
	 */
	void IncLevel(void)
	{
		m_aVava.Add(CVaVaVector());
		m_aArva.Add(CArVaVector());
		//
		TTRACE1("CTmlVariableManager::レベル++ NowLvl=%d\n", GetLevel());
	}

	/**
	 * [設定] レベル－１.
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool DecLevel(void)
	{
		INT_PTR l = m_aVava.GetSize();
		if ( l <= 0 )
		{
			return false;
		}
		#ifdef _DEBUG
			CAscii s;
			{
				CVaVaVector& va = m_aVava.Ref(l - 1);
				INT_PTR iDim = va.GetSize();
				for( INT_PTR i = 0; i < iDim; i++ )
				{
					s.Format( "    del-> 変数 %s (=%s)\n",
									va[i].name,
									va[i].value.GetString() 
					);
					::OutputDebugStringA( s );
				}
			}
			//
			{
				CArVaVector& va = m_aArva.Ref(l - 1);
				INT_PTR iDim = va.GetSize();
				for( INT_PTR i = 0; i < iDim; i++ )
				{
					s.Format( "    del-> 配列 %s(%d)\n",
									va[i].name,
									va[i].values.GetSize() 
					);
					::OutputDebugStringA( s );
				}
			}

		#endif
		//
		m_aVava.SetSize(l - 1);
		m_aArva.SetSize(l - 1);
		TTRACE1( "CTmlVariableManager::レベル-- NowLvl=%d\n", GetLevel() );
		return true;
	}

	/**
	 * [設定] レベル指定.
	 *		レベルを指定の位置へ下げる。
	 *	@param level レベル。		
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool DecLevels(INT_PTR level)
	{
		while( level < GetLevel() )
		{
			if ( ! DecLevel() )
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * [検索] 現レベルに指定名あるか.
	 *	@note 変数名、配列名、両方検索します。
	 *	@param lpszName 検索名前。
	 *	@retval true あり。
	 *	@retval false なし。
	 */
	bool HasNameInNowLevel(LPCSTR lpszName) const
	{
		size_t l = m_aVava.GetSize();
		if ( l > 0 )
		{
			const CVaVaVector& va = m_aVava.At(l - 1);
			loop ( i, va.GetSize() )
			{
				if ( va[i].name.Compare(lpszName) == 0 )
				{
					return true;
				}
			}
			const CArVaVector& aa = m_aArva.At(l - 1);
			loop ( i, aa.GetSize() )
			{
				if ( aa[i].name.Compare(lpszName) == 0 )
				{
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * [追加] 変数情報追加.
	 *	@note 今のレベルに変数を追加します。
	 *	@param lpszName 変数名。
	 *	@param v 初期値。
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool AddValue(LPCSTR lpszName, const CTmlValue& v)
	{
		size_t l = m_aVava.GetSize();
		if ( l == 0 )
		{
			return false;
		}
		TVarValue vv;
		vv.name = lpszName;
		vv.value = v;
		m_aVava.Ref(l - 1).Add(vv);
		return true;
	}

	/**
	 * [追加] 配列情報追加.
	 *	@note 今のレベルに配列を追加します。
	 *	@param lpszName 変数名。
	 *	@param size 配列サイズ
	 *	@retval true 成功。
	 *	@retval false 失敗。
	 */
	bool AddArray(LPCSTR lpszName, size_t size)
	{
		size_t l = m_aArva.GetSize();
		if ( l == 0 || size == 0 )
		{
			return false;
		}
		TArrayValue d;
		d.name = lpszName;
		d.values.SetSize(size);
		m_aArva.Ref(l - 1).Add(d);
		return true;
	}

private:
	CVaVaVectors		m_aVava;		///< 変数名と内容の二次元配列
	CArVaVectors		m_aArva;		///< 配列名と内容の二次元配列
};



}; // TNB

