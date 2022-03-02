#pragma once
/**
 *@file
 * TinyMacroLang 値関係のヘッダ
 *	
 *		TMLの値管理のクラスがあります。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbAttributedVector.h"
#include "TnbDummyCollection.h"
#include "TnbStr.h"



//TNB Library
namespace TNB
{



/**@ingroup TML
 * TinyMacroLang 値管理
 *
 *		データ(文字列)、実数、整数、Empty の４種類の型で情報管理が可能です。
 *
 *	@note データ(文字列)の場合、同時にワイルドカード情報も管理できます。
 *
 *	@note 型には強さがあり、データ(文字列)、実数、整数、Empty の順番となっています。
 *		　２項演算の際、強いほうにあわせて処理をして、結果もその型になるようになっています。
 *			たとえば、 整数 + データ なら、整数をデータ(文字列)に変換してから処理をして、結果はデータになります。
 *
 *	@par必要ファイル
 *			TnbTmlValue.h
 *
 *	@date 06/01/01 新規作成
 *	@date 09/08/18 改新
 */
class CTmlValue : public IComparableT<CTmlValue>
{
public:

	/// 種類
	enum EKind
	{
		EMPTY	= 0,	///< なし
		INTEGER	= 1,	///< 整数値
		FLOAT	= 2,	///< 実数値
		DATA	= 3		///< データ
	};

	/**
	 * コピーコンストラクタ
	 *	@param other
	 */
	CTmlValue(const CTmlValue& other) : m_param(other.m_param)
	{
	}

	/// コンストラクタ
	CTmlValue(void) : m_param()
	{
	}

	/**
	 * コンストラクタ
	 *	@param i 整数
	 */
	CTmlValue(int i) : m_param()
	{
		m_param.Set(i);
	}

	/**
	 * コンストラクタ
	 *	@param i 実数
	 */
	CTmlValue(double i) : m_param()
	{
		m_param.Set(i);
	}

	/**
	 * [代入] コピーオペレータ.
	 *	@param v コピー元
	 *	@return 自分の参照
	 */
	CTmlValue& operator=(const CTmlValue& v)
	{
		m_param = v.m_param;
		return *this;
	}

	/**
	 * [代入] データ代入.
	 *		文字列指定型
	 *	@param lpszData 文字列
	 *	@return 自分の参照
	 */
	CTmlValue& SetString(LPCSTR lpszData)
	{
		m_param.Set(STRLIB::GetLen(lpszData), lpszData);
		return *this;
	}

	/**
	 * [代入] データ代入.
	 *		データ指定型
	 *	@param v データ
	 *	@return 自分の参照
	 */
	CTmlValue& operator=(const CByteVector& v)
	{
		m_param.Set(v.GetSize(), v.ReferBuffer());
		return *this;
	}

	/**
	 * [代入] データ代入.
	 *		データ指定型
	 *	@param len データサイズ
	 *	@param P データ
	 *	@return 自分の参照
	 */
	CTmlValue& SetData(size_t len, LPCVOID P)
	{
		m_param.Set(len, P);
		return *this;
	}

	/**
	 * [設定] ワイルドカードポイント追加.
	 *	@param index ワイルドカードポイントのインデックス.
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool AddWildPoint(INDEX index)
	{
		return HasData() && m_param.dataBytes.SetAttribute(index, WILDCARD);
	}

	/**
	 * [比較] 比較.
	 *		Value の比較が行われます。データ同時の場合、ワイルドカードが考慮されます。
	 *		ワイルドカードの文字が取り出せます。
	 *	@param _vvbWild [out]WildCardだったところにあった DATA
	 *	@param other 比較相手。
	 *	@retval ０		自分は other と同じ
	 *	@retval １以上	自分は other より大きい
	 *	@retval ０未満	自分は other より小さい
	 */
	INT_PTR Compare(ICollectionT<CByteVector>& _vvbWild, const CTmlValue& other) const
	{
		_vvbWild.RemoveAll();
		EKind kind = (m_param.eKind >= other.m_param.eKind) ? m_param.eKind : other.m_param.eKind;
		if ( kind != DATA )
		{
			return m_CompareNum(m_param, other.m_param);
		}
		const CDataBytes& leftData = m_param.GetData();
		const CDataBytes& rightData = other.m_param.GetData();
		//
		size_t srcPos = 0;
		size_t wildPos = 0;
		size_t srcPosMax = leftData.GetSize();
		size_t wildPosMax = rightData.GetSize();
		BYTE B;
		while ( true )
		{
			if ( wildPos >= wildPosMax )
			{
				return 1;
			}
			if ( rightData.GetAttribute(wildPos) == WILDCARD )
			{
				//ワイルド発見！
				wildPos++;
				while ( wildPos < wildPosMax )
				{
					if ( rightData.GetAttribute(wildPos) != WILDCARD )
					{
						break;
					}
					wildPos++;
				}
				CByteVector vb;
				if ( wildPos >= wildPosMax )
				{
					//ワイルドが最後。
					while ( true )
					{
						if ( srcPos >= srcPosMax )
						{
							_vvbWild.Add(vb);
							break;
						}
						B = leftData.Get(srcPos);
						vb.Add(B);
						srcPos++;
					}
					return 0;
				}
				while ( true )
				{
					B = leftData.Get(srcPos);
					if ( B == rightData.Get(wildPos) )
					{
						//ワイルドの次の文字を発見
						_vvbWild.Add(vb);
						break;
					}
					vb.Add(B);
					srcPos++;
					if ( srcPos >= srcPosMax )
					{
						return -1;
					}
				}
			}
			if ( srcPos >= srcPosMax )
			{
				return 1;
			}
			INT_PTR r = leftData.Get(srcPos);
			r -= rightData.Get(wildPos);
			if ( r != 0 )
			{
				return r;
			}
			srcPos++;
			wildPos++;
			if ( wildPos >= wildPosMax && srcPos >= srcPosMax )
			{
				break;
			}
		}
		return 0;
	}

	/**
	 * [確認] 比較.
	 *	@param other 比較対象
	 *	@retval ０		自分は other と同じ
	 *	@retval １以上	自分は other より大きい
	 *	@retval ０未満	自分は other より小さい
	 */
	virtual INT_PTR Compare(const CTmlValue& other) const
	{
		CDummyCollectionT<CByteVector> vv;
		return Compare(vv, other);
	}
 
	/**
	 * [計算] 加算.
	 *	@param other 足す相手
	 *	@return 足した結果
	 */
	CTmlValue operator+(const CTmlValue& other) const
	{
		CTmlValue v;
		EKind kind = (m_param.eKind >= other.m_param.eKind) ? m_param.eKind : other.m_param.eKind;
		switch ( kind )
		{
		case INTEGER:
			v = m_param.GetInteger() + other.m_param.GetInteger();
			break;
		case FLOAT:
			v = m_param.GetFloat() + other.m_param.GetFloat();
			break;
		case DATA:
			v.m_param.Set(m_param.GetData() + other.m_param.GetData());
			break;
		case EMPTY:
		default:
			break;
		}
		return v;
	}

	/**
	 * [計算] 引き算.
	 *	@param other 引く相手
	 *	@return 足した結果
	 */
	CTmlValue operator-(const CTmlValue& other) const
	{
		CTmlValue v;
		EKind kind = (m_param.eKind >= other.m_param.eKind) ? m_param.eKind : other.m_param.eKind;
		switch ( kind )
		{
		case INTEGER:
			v = m_param.GetInteger() - other.m_param.GetInteger();
			break;
		case FLOAT:
			v = m_param.GetFloat() - other.m_param.GetFloat();
			break;
		case DATA:
			v = *this;
			break;
		case EMPTY:
		default:
			break;
		}
		return v;
	}
 
	/**
	 * [計算] 掛け算.
	 *	@note DATA と INTEGER を掛けると、DATAを INTEGER分、繰り返したデータになります。
	 *	@param other 掛ける相手
	 *	@return 掛けた結果
	 */
	CTmlValue operator*(const CTmlValue& other) const
	{
		CTmlValue v;
		EKind kind = (m_param.eKind >= other.m_param.eKind) ? m_param.eKind : other.m_param.eKind;
		switch ( kind )
		{
		case INTEGER:
			v = m_param.GetInteger() * other.m_param.GetInteger();
			break;
		case FLOAT:
			v = m_param.GetFloat() * other.m_param.GetFloat();
			break;
		case DATA:
			v = *this;
			if ( m_param.eKind == DATA )
			{
				v.m_param.dataBytes.RemoveAll();
				loop ( i, other.m_param.GetInteger() )
				{
					v.m_param.dataBytes += m_param.dataBytes;
				}
			}
			break;
		case EMPTY:
		default:
			break;
		}
		return v;
	}
 
	/**
	 * [計算] 割り算.
	 *	@param other 割る相手
	 *	@return 割った果
	 */
	CTmlValue operator/(const CTmlValue& other) const
	{
		CTmlValue v;
		EKind kind = (m_param.eKind >= other.m_param.eKind) ? m_param.eKind : other.m_param.eKind;
		switch ( kind )
		{
		case INTEGER:
			v = m_param.GetInteger() / other.m_param.GetInteger();
			break;
		case FLOAT:
			v = m_param.GetFloat() / other.m_param.GetFloat();
			break;
		case DATA:
			v = *this;
			break;
		case EMPTY:
		default:
			break;
		}
		return v;
	}
 
	/**
	 * [取得] WildCard情報取得.
	 *	@return ワイルドカードのIndexが格納
	 */
	CVectorT<INDEX> GetWildCardPositions(void) const
	{
		CVectorT<INDEX> vid;
		if ( HasData() )
		{
			loop ( i, m_param.dataBytes.GetSize() )
			{
				if ( m_param.dataBytes.GetAttribute(i) == WILDCARD )
				{
					vid.Add(i);
				}
			}
		}
		return vid;
	}

	/**
	 * [確認] データ型保持確認.
	 *	@retval true データ保持
	 *	@retval false それ以外
	 */
	bool HasData(void) const
	{
		return m_param.eKind == DATA;
	}

	/**
	 * [確認] 保持型確認.
	 *		保持しているデータの型を返します。
	 *	@return 型。
	 */
	EKind Getkind(void) const
	{
		return m_param.eKind;
	}

	/**
	 * [取得] 数字取得.
	 *		保持しているのがデータなら数値に変換して返します。
	 *	@return 値
	 */
	int GetInteger(void) const
	{
		return HasData() ? atoi(m_param.GetString()) : m_param.GetInteger();
	}

	/**
	 * [取得] 数字取得.
	 *		保持しているのがデータなら数値に変換して返します。
	 *	@return 値
	 */
	double GetFloat(void) const
	{
		return HasData() ? atof(m_param.GetString()) : m_param.GetFloat();
	}

	/**
	 * [取得] 文字列取得.
	 *		データを文字列として返します。データ途中にNILがあると文字列はそこで終わります。
	 *		保持しているのが数値型なら、文字列に変換して返ります。
	 *	@return 文字列。
	 */
	CAscii GetString(void) const
	{
		return m_param.GetString();
	}

	/**
	 * [取得] データ取得.
	 *		保持しているのが数値型なら、データに変換して返ります。
	 *	@return データ
	 */
	CByteVector GetData(void) const
	{
//		return m_param.GetData().Refer();
		return m_param.GetData();
	}

	/**
	 * [取得] データ長取得.
	 *		保持しているのが数値型なら、データに変換して返ります。
	 *	@return 値
	 */
	size_t GetDataSize(void) const
	{
		return m_param.GetData().GetSize();
	}

private:

	///バイナリの属性
	enum EAttr
	{
		NORMAL = 0,		///< ノーマル
		WILDCARD = 1,	///< ワイルドカード
	};

	/// データ型宣言
	typedef CAttributedVectorT<BYTE, BYTE> CDataBytes;

	/// パラメータ
	struct TParam
	{
		EKind		eKind;			///< 保持している情報の種類
		CDataBytes	dataBytes;		///< 保持しているバイナリデータ
		int			iValue;			///< 保持している整数
		double		doValue;		///< 保持している実数
		/// コンストラクタ
		TParam(void) : iValue(0), doValue(0.0), eKind(EMPTY)
		{
		}
		/// コピーコンストラクタ
		TParam(const TParam& p) : eKind(p.eKind), dataBytes(p.dataBytes), iValue(p.iValue), doValue(p.doValue)
		{
		}
		/// コピーオペレータ
		TParam& operator=(const TParam& p)
		{
			eKind = p.eKind;
			dataBytes = p.dataBytes;
			iValue = p.iValue;
			doValue = p.doValue;
			return *this;
		}
		/// セッター
		void Set(EKind e, int i = 0, double d = 0.0, const CDataBytes* D = NULL)
		{
			eKind = e;
			if ( D == NULL )
			{
				dataBytes.RemoveAll();
			}
			else
			{
				dataBytes = *D;
			}
			iValue = i;
			doValue = d;
		}
		/// クリア
		void Empty(void) 
		{
			Set(EMPTY); 
		}
		/// セット
		void Set(int i)
		{
			Set(INTEGER, i, i);
		}
		/// セット
		void Set(double i) 
		{
			Set(FLOAT, ToInt(i), i); 
		}
		/// セット
		void Set(const CDataBytes& d)
		{
			Set(DATA, 0, 0.0, &d); 
		}
		/// セット
		void Set(size_t l, LPCVOID P)
		{
			CDataBytes d;
			d.SetElements(l, static_cast<const BYTE*>(P));
			Set(d);
		}
		/// INT取得
		int GetInteger(void) const
		{
			return iValue;
		}
		/// FLOAT取得
		double GetFloat(void) const
		{
			return (eKind <= INTEGER) ? iValue : doValue;
		}
		/// 文字列取得
		CAscii GetString(void) const
		{
			CAscii a;
			switch ( eKind )
			{
			case INTEGER:
				a.Format("%d", iValue);
				break;
			case FLOAT:
				a.Format("%.0f", doValue);
				break; 
			case DATA:
				{
					size_t l = dataBytes.GetSize();
					if ( l > 0 )
					{
						LPSTR P = a.GetBuffer(l);
						dataBytes.GetElements(l, reinterpret_cast<BYTE*>(P));
						P[l] = 0;
						a.ReleaseBuffer();
					}
				}
				break;
			default:
				break;
			}
			return a;
		}
		/// データ取得
		CDataBytes GetData(void) const
		{
			CDataBytes d;
			switch ( eKind )
			{
			case DATA:
				d = dataBytes;
				break;
			case INTEGER:
			case FLOAT:
				{
					CAscii a = GetString();
					d.SetElements(a.GetLength(), reinterpret_cast<const BYTE*>(LPCSTR(a)));
				}
				break;
			default:
				break;
			}
			return d;
		}
	};

	TParam m_param;	///< パラメータ

	/// 両方が DATA 以外の場合のみ使用可能
	INT_PTR m_CompareNum(const TParam& p1, const TParam& p2) const
	{
		if ( p1.eKind == DATA || p2.eKind == DATA )
		{
			ASSERT(false);
			return 0;
		}
		if ( p1.eKind <= INTEGER && p2.eKind <= INTEGER )
		{
			return IComparatorT<int>::GetDefault().CompareTo(p1.iValue, p2.iValue);
		}
		return IComparatorT<double>::GetDefault().CompareTo(p1.doValue, p2.doValue);
	}

	friend class CTmlValueTest;
};



/**@ingroup TML
 * TinyMacroLang 値配列型.
 *	
 *	@par必要ファイル
 *			TnbTmlValue.h
 *
 *	@date 06/01/01 新規作成
 *	@date 09/08/18 改新
 */
typedef CVectorT<CTmlValue> CTmlValuesVector;



/**@ingroup TML
 * TinyMacroLang 値参照管理
 *
 *		値の計算に使用される数値クラス。
 *		TmlValue の実体か参照のいずれかを管理します。
 *		参照の場合、値を変更したら参照先の値も変更されます。
 *
 *	@par必要ファイル
 *			TnbTmlValue.h
 *
 *	@date 06/01/01 新規作成
 *	@date 09/08/18 改新
 */
class CTmlValueCarrier
{
public:

	/// コンストラクタ
	CTmlValueCarrier(void) : m_pValueRef(NULL), m_valueReal(), m_strExName()
	{
	}

	/**
	 * コピーコンストラクタ
	 *	@param other コピー元
	 */
	CTmlValueCarrier(const CTmlValueCarrier& other) 
		: m_pValueRef(other.m_pValueRef), m_valueReal(other.m_valueReal), m_strExName(other.m_strExName)
	{
	}

	/**
	 * コピーオペレータ
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CTmlValueCarrier& operator=(const CTmlValueCarrier& other) 
	{
		m_pValueRef = other.m_pValueRef;
		m_valueReal = other.m_valueReal;
		m_strExName = other.m_strExName;
		return *this;
	}

	/**
	 * [代入] int代入.
	 *	@note 拡張ネームと参照はOFFになります
	 *	@param i 値
	 *	@return 自分の参照
	 */
	CTmlValueCarrier& operator=(int i)
	{
		m_strExName.Empty();
		m_valueReal = i;
		m_pValueRef = NULL;
		return *this;
	}

	/**
	 * [代入] double代入.
	 *	@note 拡張ネームと参照はOFFになります
	 *	@param i 値
	 *	@return 自分の参照
	 */
	CTmlValueCarrier& operator=(double i)
	{
		m_strExName.Empty();
		m_valueReal = i;
		m_pValueRef = NULL;
		return *this;
	}

	/**
	 * [代入] Valueを代入.
	 *	@note 拡張ネームと参照はOFFになります
	 *	@param v 値
	 *	@return 自分の参照
	 */
	CTmlValueCarrier& operator=(const CTmlValue& v)
	{
		m_strExName.Empty();
		m_valueReal = v;
		m_pValueRef = NULL;
		return *this;
	}

	/**
	 * [代入] Value参照指定.
	 *	@note 指定のValueの参照を記憶します。
	 *	@param v 値
	 */
	void SetRef(CTmlValue& v)
	{
		m_pValueRef = &v;
	}

	/**
	 * [代入] 拡張ネーム指定.
	 *	@param lpsz 拡張ネーム
	 */
	void Set(LPCSTR lpsz)
	{
		m_strExName = lpsz;
	}

	/**
	 * [設定] 空。.
	 *	@note 拡張ネームと参照はOFFになります
	 */
	void Empty(void)
	{
		m_strExName.Empty();
		m_valueReal = 0;
		m_pValueRef = NULL;
	}

	/**
	 * [取得] 拡張ネーム取得
	 *	@return 拡張ネーム
	 */
	LPCSTR GetExName(void) const
	{
		return m_strExName;
	}

	/**
	 * [取得] 値参照
	 *	@return 値の参照。
	 */
	const CTmlValue& operator()(void) const
	{
		return (m_pValueRef == NULL) ? m_valueReal : *m_pValueRef;
	}

	/**
	 * [取得] 値参照
	 *	@note 保持しているValueの参照を返します。参照なので、代入OKです.
	 *	@return 値の参照。
	 */
	CTmlValue& operator()(void)
	{
		return (m_pValueRef == NULL) ? m_valueReal : *m_pValueRef;
	}

private:
	CTmlValue*		m_pValueRef;	///< 本クラスが持つ値の参照
	CTmlValue		m_valueReal;	///< 本クラスが持つ値の実体
	CAscii			m_strExName;	///< 拡張ネーム
};



}; // TNB

