#pragma once
/**
 *@file
 * コンパレータ関係のヘッダ
 *
 *		比較するためのインターフェースがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStr.h"
#include "TnbComparable.h"



//TNB Library
namespace TNB
{



/**@ingroup VALUE
 * コンパレータインターフェース.
 *
 *		比較機能( IComparableT のサブクラスか基本型)を持たない型同士を比較するための、
 *		インターフェースです。
 *		比較条件を CompareTo() に実装します。 
 *
 *		比較機能を持っている型でも、このインターフェースのサブクラスを作成し、
 *		指定可能なメソッドに委譲することで、比較方法を変更することが出来ます。
 *
 *	@note 型に対するデフォルトのコンパレータを作成することも出来ます( GetDefault() )。
 *
 *	@param TYP 要素の型。
 *
 *	@par必要ファイル
 *			TnbComparator.h
 *
 *	@date 06/07/04 IConstCollectionT から分離。
 *	@date 06/09/13 IsEqualTo() 追加。
 */
template<typename TYP>
struct IComparatorT
{
	/// デストラクタ
	virtual ~IComparatorT(void) {}

	/**
	 * [確認] 比較
	 *	@param t1 比較対象１
	 *	@param t2 比較対象２
	 *	@retval ０		t1 は t2 と同じ
	 *	@retval １以上	t1 は t2 より大きい
	 *	@retval ０未満	t1 は t2 より小さい
	 */
	virtual INT_PTR CompareTo(const TYP& t1, const TYP& t2) const = 0;

	/**
	 * [確認] 比較
	 *	@param t1 比較対象１
	 *	@param t2 比較対象２
	 *	@retval true t1 は t2 と同じ
	 *	@retval false t1 は t2 と異なる
	 */
	virtual bool IsEqualTo(const TYP& t1, const TYP& t2) const
	{
		return CompareTo(t1, t2) == 0;
	}

	/**
	 * [作成] 汎用コンパレータ取得.
	 *		TYP 用のコンパレータを作成します。
	 *	@note	基本型、 IComparatorT 実装型、 CStr系型以外の場合、
	 *			比較しても「同じ」を返すコンパレータが作成されてしまいます。
	 *			その場合、本メソッドを使わず、 IComparatorT を実装して作成してください。
	 *	@return 汎用コンパレータ
	 */
	static IComparatorT& GetDefault(void);
};



#ifndef _TnbDOXYGEN	//Document作成用シンボル



/**@ingroup VALUE
 * 情報群管理インターフェースのコンパレーター自動作成クラス.
 */
template<typename TYP>
class CAutoComparatorT
{
	// IComparableTインターフェース実装型用
	template<typename T>
	struct TMyCompCompareT : IComparatorT<TYP>
	{
		INT_PTR CompareTo(const TYP& t1, const TYP& t2) const
		{
			//ポイント;Ｃ型キャストを用いることで型に応じたキャストが行われる。
			return ((const IComparableT<T>*)&t1)->Compare(t2);
		}
		bool IsEqualTo(const TYP& t1, const TYP& t2) const
		{
			//ポイント;Ｃ型キャストを用いることで型に応じたキャストが行われる。
			return ((const IComparableT<T>*)&t1)->IsEqual(t2);
		}
	};

	// その他の型用
	struct TMyCompElse : IComparatorT<TYP>
	{
		// SubSub ポインタ比較
		template<typename T>
		INT_PTR m_SubSub(const T& t, LPCVOID t1, LPCVOID t2) const
		{
			return static_cast<const BYTE *>(t1) - static_cast<const BYTE *>(t2);
		}

		// SubSub 比較できない
		template<typename T>
		INT_PTR m_SubSub(const T& t, ...) const
		{
 #ifndef _WIN32_WCE
			ASSERT1(
				false,
				"オートComparator内",
				"比較機能のない[%s]型を IComparator指定無しで比較しました。結果は常に EVEN になります。",
				typeid(T).name()
			);
 #else
			ASSERT0(
				false,
				"オートComparator内",
				"比較機能のない型を IComparator指定無しで比較しました。結果は常に EVEN になります。"
			);
 #endif // _WIN32_WCE
			return 0;
		}

		// <Sub> その他の型。 SubSubに依頼
		template<typename T>
		INT_PTR m_Sub(const T& t1, const T& t2) const
		{
			return m_SubSub(t1, t1, t2);
		}
		
		// <Sub> CAscii型だ
		template<> INT_PTR m_Sub(const CAscii& i1, const CAscii& i2) const
		{
			return i1.Compare(i2);
		}

		// <Sub> CUnicode型だ
		template<> INT_PTR m_Sub(const CUnicode& i1, const CUnicode& i2) const
		{
			return i1.Compare(i2);
		}

		// <Sub> 基本型だ
		#define _COLLECTCOMP(TY)											\
			template<> INT_PTR m_Sub(const TY& i1, const TY& i2) const {	\
				if ( i1 == i2 ){ return 0; }								\
				return (i1 > i2) ? 1 : -1;									\
			}
		_COLLECTCOMP(bool)
		_COLLECTCOMP(BYTE)
		_COLLECTCOMP(char)
		_COLLECTCOMP(WORD)
		_COLLECTCOMP(short)
		_COLLECTCOMP(DWORD)
		_COLLECTCOMP(long)
		_COLLECTCOMP(UINT)
		_COLLECTCOMP(int)
		_COLLECTCOMP(ULONGLONG)
		_COLLECTCOMP(LONGLONG)
		_COLLECTCOMP(long double)
		_COLLECTCOMP(double)
		_COLLECTCOMP(float)
		#ifdef __AFX_H__
			_COLLECTCOMP(CString)
		#endif
		#undef _COLLECTCOMP

		// 比較関数
		INT_PTR CompareTo(const TYP& t1, const TYP& t2) const
		{
			return m_Sub(t1, t2);
		}
	};

public:

	/**
	 * [作成] IComparableT用コンパレーター自動作成.
	 *	@param P ダミー
	 *	@return コンパレーター
	 */
	template<typename T>
	static IComparatorT<TYP>& MakeAutoComp(const IComparableT<T>* P)
	{
		static TMyCompCompareT<T> s_comp;
		return s_comp;
	}

	/**
	 * [作成] 汎用コンパレーター自動作成.
	 *	@param P ダミー
	 *	@return コンパレーター
	 */
	static IComparatorT<TYP>& MakeAutoComp(LPCVOID P)
	{
		static TMyCompElse s_comp;
		return s_comp;
	}
};

#endif	



template<typename TYP>
inline IComparatorT<TYP>& IComparatorT<TYP>::GetDefault(void)
{
	return CAutoComparatorT<TYP>::MakeAutoComp(static_cast<TYP*>(NULL));
};



};