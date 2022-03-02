#pragma once
/**
 *@file
 * 範囲管理関係のヘッダ
 *
 *		ある値とある値をペアで管理するクラスが有ります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSerializer.h"
#include "TnbComparable.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



/**@ingroup VALUE
 * 範囲管理テンプレートクラス
 *
 *		ある値とある値をペアで管理するテンプレートクラスです。
 *
 *	@param TYP 比較機能のある型を指定できます。
 *	@param DIF 「TYP-TYP=DIF」「TYP+DIF=TYP」になるような型を指定します。 
 *
 *	@par必要ファイル
 *			TnbRange.h
 *
 *	@date 06/10/13	新規作成
 */
template<typename TYP, typename DIF=TYP>
class CRangeT : public IComparableT< TNB::CRangeT<TYP,DIF> >, public ISerializable 
{
	TYP		m_first;	///< 値の開始(m_lastと同じか小さい)
	TYP		m_last;		///< 値の終了(m_firstと同じか大きい)

	/// 保持している値内容を比較し、大小
	void m_CheckRange(void)
	{
		if ( m_first > m_last )
		{
			Swap(m_first, m_last);
			ASSERTLIB(m_first <= m_last);
		}
	}

public:

	/**
	 * コンストラクタ
	 *	@param t 値
	 */
	CRangeT(const TYP& t = TYP()) : m_first(t), m_last(t) 
	{
	}

	/**
	 * コンストラクタ
	 *	@note First,Lastは逆でも構いません( 内部的に管理を入れ替えます)。
	 *	@param t1 値１
	 *	@param t2 値２
	 */
	CRangeT(const TYP& t1, const TYP& t2) : m_first(t1), m_last(t2)
	{
		m_CheckRange();
	}

	/**
	 * コピーコンストラクタ
	 *	@param other コピー元。
	 */
	CRangeT(const CRangeT& other) : m_first(other.m_first), m_last(other.m_last)
	{
		ASSERTLIB(m_first <= m_last);
	}

	/**
	 * [代入] コピーオペレータ
	 *	@param other コピー元。
	 *	@return 自分の参照
	 */
	CRangeT& operator=(const CRangeT& other)
	{
		m_first = other.m_first;
		m_last	= other.m_last;
		ASSERTLIB(m_first <= m_last);
		return *this;
	}

	/**
	 * [代入] 代入
	 *	@note First,Lastは逆でも構いません( 内部的に管理を入れ替えます)。
	 *	@param t1 値１
	 *	@param t2 値２
	 */
	void Set(const TYP& t1, const TYP& t2)
	{
		m_first = t1;
		m_last	= t2;
		m_CheckRange();
	}

	/**
	 * [代入] First値代入
	 *	@attention Last値より大きい値を設定すると、内部的に管理を入れ替えます。
	 *	@param t 値
	 */
	void SetFirst(const TYP& t)
	{
		m_first = t;
		m_CheckRange();
	}

	/**
	 * [代入] Last値代入
	 *	@attention First値より小さい値を設定すると、内部的に管理を入れ替えます。
	 *	@param t 値
	 */
	void SetLast(const TYP& t)
	{
		m_first = t;
		m_CheckRange();
	}

	/**
	 * [代入] Diff値代入
	 *	@note Diff値を指定することで、First値を基点に、Last値を設定します。
	 *			(Last値 = First値 + Diff値)
	 *	@param d 値
	 */
	void SetRange(const DIF& d)
	{
		m_last = m_first + d;
		m_CheckRange();
	}

	/**
	 * [取得] First値取得
	 *	@return 値
	 */
	TYP GetFirst(void)
	{
		return m_first;
	}

	/**
	 * [取得] First値取得
	 *	@return 値
	 */
	const TYP& GetFirst(void) const
	{
		return m_first;
	}

	/**
	 * [取得] Last値取得
	 *	@return 値
	 */
	TYP GetLast(void)
	{
		return m_last;
	}

	/**
	 * [取得] Last値取得
	 *	@return 値
	 */
	const TYP& GetLast(void) const
	{
		return m_last;
	}

	/**
	 * [計算] Diff値取得
	 *	@note Last値 - First値が返ります。
	 *	@return Diff値
	 */
	DIF Difference(void) const
	{
		return m_last - m_first;
	}

	/**
	 * [取得] 中央計算
	 *	@attention DIF の型が割り算をサポートしていることが必要です。
	 *	@return 中央を指す値
	 */
	TYP Center(void) const
	{
		TYP r = m_first;
		return r += (Difference() / 2);
	}

	/**
	 * [計算] 加算
	 *	@note First値 Last値に指定Diff値を加えます。
	 *	@param dif Diff値
	 *	@return 自分の参照
	 */
	CRangeT& operator+=(const DIF& dif)
	{
		m_first += dif;
		m_last	+= dif;
		m_CheckRange();
		return *this;
	}

	/**
	 * [取得] 加算
	 *	@note First値 Last値に指定Diff値を加えた CRangeT を作成します。
	 *	@param rng 対象
	 *	@param dif Diff値
	 *	@return 結果
	 */
	inline friend CRangeT operator+(const CRangeT& rng, const DIF& dif)
	{
		CRangeT r = rng;
		return r += dif;
	}

	/**
	 * [計算] 減算
	 *	@note First値 Last値に指定Diff値を引きます。
	 *	@param dif Diff値
	 *	@return 自分の参照
	 */
	CRangeT& operator-=(const DIF& dif)
	{
		m_first -= dif;
		m_last	-= dif;
		m_CheckRange();
		return *this;
	}

	/**
	 * [取得] 減算
	 *	@note First値 Last値に指定Diff値を引いた CRangeT を作成します。
	 *	@param rng 対象
	 *	@param dif Diff値
	 *	@return 結果
	 */
	inline friend CRangeT operator-(const CRangeT& rng, const DIF& dif)
	{
		CRangeT r = rng;
		return r -= dif;
	}

	/**
	 * [確認] 範囲内確認
	 *	@param t 値
	 *	@retval true 範囲内
	 *	@retval false 範囲外
	 */
	bool IsInRange(const TYP& t) const
	{
		return m_first <= t && m_last >= t;
	}

	/**
	 * [確認] 範囲内確認
	 *	@note 対象が本インスタンスの範囲に全て含まれるか確認します。
	 *	@param rng 対象
	 *	@retval true 範囲内
	 *	@retval false 範囲外
	 */
	bool IsInRange(const CRangeT& rng) const
	{
		return m_first <= rng.m_first && m_last >= rng.m_last;
	}

	/**
	 * [確認] 範囲内確認
	 *	@param t 値
	 *	@retval true 範囲内
	 *	@retval false 範囲外
	 */
	bool InRange(const TYP& t) const
	{
		return IsInRange(t);
	}

	/**
	 * [確認] 範囲内確認
	 *	@note 対象が本インスタンスの範囲に全て含まれるか確認します。
	 *	@param rng 対象
	 *	@retval true 範囲内
	 *	@retval false 範囲外
	 */
	bool InRange(const CRangeT& rng) const
	{
		return IsInRange(rng);
	}

	/// 関係タイプ
	enum ERelationType
	{
		UNRELATED,	///< 無関係
		TOUCHED,	///< 一部接触
		WARPPED		///< 内包
	};

	/**
	 * [確認] 関係確認
	 *	@note 自分と対象の関係を調べます。
	 *	@param rng 対象
	 *	@retval UNRELATED	無関係。重なっている部分はありません。
	 *	@retval WARPPED		内包。片方が全て含んでいます。
	 *	@retval TOUCHED		一部接触。一部重なっている部分があります( Intersect() で取り出すことが出来ます)。
	 */
	ERelationType GetRelation(const CRangeT& rng) const
	{
		CRangeT r1 = *this;
		CRangeT r2 = rng;
		if ( *this > rng )
		{
			r1 = rng;
			r2 = *this;
		}
		if ( r1.m_last >= r2.m_last )
		{
			return WARPPED;
		}
		if ( r1.m_last >= r2.m_first )
		{
			if ( r1.m_first == r2.m_first )
			{
				return WARPPED;
			}
			return TOUCHED;
		}
		return UNRELATED;
	}

	/**
	 * [取得] 重複範囲取得
	 *	@note 自分と対象で重複している範囲をもつ CRangeT が得られます。
	 *	@param rng 対象
	 *	@return 結果
	 *	@throw CEmptyException 重複範囲がない場合、スローされます。
	 */
	CRangeT Intersect(const CRangeT& rng) const
	{
		CRangeT r1 = *this;
		CRangeT r2 = rng;
		if ( *this > rng )
		{
			r1 = rng;
			r2 = *this;
		}
		ERelationType rt = r1.GetRelation(r2);
		if ( rt == UNRELATED )
		{
			throw CEmptyException(); //無関係。重なっている部分はありません。
		}
		else if ( rt == WARPPED )
		{
			//内包。片方が全て含んでいます。
			if ( r1.m_first == r2.m_first )
			{
				return r1;
			}
			return r2;
		}
		return CRangeT(r2.m_first, r1.m_last);
	}

	/**
	 * [取得] 内包範囲取得
	 *	@note 自分と対象が両方内包される CRangeT が得られます。
	 *	@param rng 対象
	 *	@return 結果
	 */
	CRangeT Union(const CRangeT& rng) const
	{
		TYP t1 = min(m_first, rng.m_first);
		TYP t2 = max(m_last, rng.m_last);
		return CRangeT(t1, t2);
	}

	/**
	 * [確認] 比較
	 *	@note First値が同じ場合、Last値を比較する。
	 *	@param rng 比較対象
	 *	@retval ０		自分は rng と同じ
	 *	@retval １以上	自分は rng より大きい
	 *	@retval ０未満	自分は rng より小さい
	 */
	virtual INT_PTR Compare(const CRangeT& rng) const
	{
		INT_PTR r = IComparatorT<TYP>::GetDefault().CompareTo(m_first, rng.m_first);
		return (r != 0) ? r : IComparatorT<TYP>::GetDefault().CompareTo(m_last, rng.m_last);
	}

	/**
	 * [通知] シリアライズ
	 *	@param[out] _sr シリアライザー。 これに対しメンバの内容を追加してください。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		_sr << m_first << m_last;
	}

	/**
	 * [通知] デシリアライズ
	 *	@param ds デシリアライザー。これからメンバの内容を取り出してください。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		ds >> m_first >> m_last;
	}
};



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")
