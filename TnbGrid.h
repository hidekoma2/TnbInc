#pragma once
/**
 *@file
 * グリッド情報管理関係のヘッダ
 *
 *		格子状に情報を管理します。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStrVector.h"



//TNB Library
namespace TNB
{



/**@ingroup DBASE
 * グリッド管理クラス.
 *
 *		1つ以上の任意の型のフィールドと、複数のレコードからなる情報を扱います。
 *
 *	@note dBase のデータ種をサポートしています。
 *	@note メモフィールドはサポートしていません。
 *
 */
class CGrid
{
public:
	
	/// フィールド種類
	enum EFieldKind
	{
		EK_String,		///< 文字列
		EK_Integer,		///< 数値
		EK_Float,		///< 実数
		EK_Date,		///< 日付
		EK_Invalid,		///< 不正
	};

	/// フィールドタイプ
	enum EFieldType
	{
		ET_Character	= 'C',		///< 文字列
		ET_Date			= 'D',		///< 年月日
		ET_Numeric		= 'N',		///< 数値
		ET_Floating		= 'F',		///< 浮動小数点数値
		ET_Logical		= 'L',		///< 論理(?,Y,N,T,F)
//		ET_Memo			= 'M',		///< メモ
	};

	/// フィールド情報
	struct TFieldInfo
	{
		CStr		name;		///< 名前
		size_t		width;		///< 幅
		size_t		decimals;	///< 小数点部幅
		EFieldType	type;		///< タイプ
		/// コンストラクタ
		TFieldInfo(LPCTSTR lpsz = NULL, size_t w = 0, size_t d = 0, EFieldType t = ET_Character)
			: name(lpsz), width(w), decimals(d), type(t)
		{
		}
		/// デシリアライズ
		virtual void Deserialize (const IDeserializer& ds)
		{
			ds >> name >> width >> decimals >> type;
		}
		/// シリアライズ
		virtual void Serialize (ISerializer& _sr) const
		{
			_sr << name << width << decimals << type;
		}
	};

	/// 日付情報
	struct TDate
	{
		WORD year;		///< 年
		BYTE month;		///< 月
		BYTE day;		///< 日
		/**
		 * コンストラクタ
		 *	@param y 年
		 *	@param m 月
		 *	@param d 日
		 */
		TDate(WORD y = 2000, BYTE m = 1, BYTE d = 1) : year(y), month(m), day(d)
		{
		}
		/**
		 * [取得] INT型取得
		 *	@return INT
		 */
		int Get(void) const
		{
			return year * 10000 + month * 100 + day;
		}
		/**
		 * [設定] INT型設定
		 *	@param i INT
		 */
		void Set(int i)
		{
			year = down_cast<WORD>(i / 10000);
			month = down_cast<BYTE>((i / 100) % 100);
			day = down_cast<BYTE>(i % 100);
		}
	};


	//---------------------------


	/// コンストラクタ
	CGrid(void)
	{
	}

	/**
	 * [削除] 全削除.
	 *	@note レコードも全削除します。
	 */
	void AllDelete(void)
	{
		m_records.RemoveAll();
		m_fields.RemoveAll();
	}

	/**
	 * [取得] フィールド数取得
	 *	@return フィールド数
	 */
	size_t GetFieldCount(void) const
	{
		return m_fields.GetSize();
	}

	/**
	 * [取得] レコード数取得
	 *	@return レコード数
	 */
	size_t GetRecordCount(void) const
	{
		return m_records.GetSize();
	}

	/**
	 * [検索] フィールド名検索
	 *	@param lpszFieldName フィールド名
	 *	@retval INVALID_INDEX以外 発見。値はフィールドインデックス。
	 *	@retval INVALID_INDEX 見つからない
	 */
	INDEX FindFieldName(LPCTSTR lpszFieldName) const
	{
		loop ( i, m_fields.GetSize() )
		{
			if ( m_fields[i].name.IsEqual(lpszFieldName) )
			{
				return i;
			}
		}
		return INVALID_INDEX;
	}

	/**
	 * [取得] フィールド情報取得
	 *	@param[out] _fi フィールド情報
	 *	@param[in] field フィードインデックス
	 *	@retval EK_Invalid以外 取得成功。値は種類。
	 *	@retval EK_Invalid 失敗（インデックスが範囲外）。
	 */
	EFieldKind GetFieldInfo(TFieldInfo& _fi, INDEX field) const
	{
		if ( m_fields.IsInRange(field) )
		{
			_fi = m_fields[field];
			return m_ToKind(_fi);
		}
		return EK_Invalid;
	}

	/**
	 * [追加] フィールド追加.
	 *	@note フィールド名が既存の場合、失敗します。
	 *	@note レコードがある場合、失敗します。
	 *	@param fi フィールド追加
	 *	@retval INVALID_INDEX以外 追加成功。値は追加したフィールドインデックス。
	 *	@retval INVALID_INDEX フィールド追加失敗
	 */
	INDEX AddField(const TFieldInfo& fi)
	{
		if ( GetRecordCount() == 0 && ! fi.name.IsEmpty() && FindFieldName(fi.name) == INVALID_INDEX && fi.width >= 1 && fi.width <= 254 )
		{
			return m_fields.Add(fi);
		}
		return INVALID_INDEX;
	}

	/**
	 * [追加] 文字フィールド追加.
	 *	@note フィールド名が既存の場合、失敗します。
	 *	@note レコードがある場合、失敗します。
	 *	@param lpszFieldName フィールド名
	 *	@param width フィールド幅
	 *	@retval INVALID_INDEX以外 追加成功。値は追加したフィールドインデックス。
	 *	@retval INVALID_INDEX フィールド追加失敗
	 */
	INDEX AddCharacterField(LPCTSTR lpszFieldName, size_t width)
	{
		TFieldInfo fi(lpszFieldName, width, 0, ET_Character);
		return AddField(fi);
	}

	/**
	 * [追加] 数値フィールド追加.
	 *	@note フィールド名が既存の場合、失敗します。
	 *	@note レコードがある場合、失敗します。
	 *　@note 小数形式で管理します。そのため、数値の範囲によりフィールド幅、小数点部幅を考慮する必要があります。
	 *	@param lpszFieldName フィールド名
	 *	@param width フィールド幅
	 *	@param dec 小数点部幅。省略すると０になります。
	 *	@retval INVALID_INDEX以外 追加成功。値は追加したフィールドインデックス。
	 *	@retval INVALID_INDEX フィールド追加失敗
	 */
	INDEX AddNumericField(LPCTSTR lpszFieldName, size_t width, size_t dec = 0)
	{
		TFieldInfo fi(lpszFieldName, width, dec, ET_Numeric);
		if ( width >= dec + 1 )
		{
			return AddField(fi);
		}
		return INVALID_INDEX;
	}

	/**
	 * [追加] 数値フィールド追加.
	 *	@note フィールド名が既存の場合、失敗します。
	 *	@note レコードがある場合、失敗します。
	 *　@note 指数形式で管理します。
	 *	@param lpszFieldName フィールド名
	 *	@param width フィールド幅
	 *	@param dec 小数点部幅。省略すると０になります。
	 *	@retval INVALID_INDEX以外 追加成功。値は追加したフィールドインデックス。
	 *	@retval INVALID_INDEX フィールド追加失敗
	 */
	INDEX AddFloatingField(LPCTSTR lpszFieldName, size_t width, size_t dec = 0)
	{
		TFieldInfo fi(lpszFieldName, width, dec, ET_Floating);
		if ( width >= dec + 1 )
		{
			return AddField(fi);
		}
		return INVALID_INDEX;
	}

	/**
	 * [追加] 日付フィールド追加.
	 *	@note フィールド名が既存の場合、失敗します。
	 *	@note レコードがある場合、失敗します。
	 *	@param lpszFieldName フィールド名
	 *	@retval INVALID_INDEX以外 追加成功。値は追加したフィールドインデックス。
	 *	@retval INVALID_INDEX フィールド追加失敗
	 */
	INDEX AddDateField(LPCTSTR lpszFieldName)
	{
		TFieldInfo fi(lpszFieldName, 8, 0, ET_Date);
		return AddField(fi);
	}

	/**
	 * [削除] フィールド削除.
	 *	@note レコードがある場合、失敗します。
	 *	@param field フィールドインデックス
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool DeleteField(INDEX field)
	{
		if ( GetRecordCount() == 0 )
		{
			return m_fields.Remove(field);
		}
		return false;
	}

	/**
	 * [削除] レコード全削除.
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool AllDeleteRecord(void)
	{
		return m_records.RemoveAll();
	}

	/**
	 * [追加] レコード追加.
	 *	@note 追加したレコードの全フィールドはNULLになっています。
	 *	@retval INVALID_INDEX以外 追加成功。値は追加したレコードインデックス。
	 *	@retval INVALID_INDEX レコード追加失敗
	 */
	INDEX AddRecord(void)
	{
		size_t len = GetFieldCount();
		if ( len != 0 )
		{
			CDataVector dv;
			dv.SetSize(len);
			return m_records.Add(dv);
		}
		return INVALID_INDEX;
	}

	/**
	 * [挿入] レコード挿入.
	 *	@note 挿入追加したレコードの全フィールドはNULLになっています。
	 *	@param record レコードインデックス。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool InsertRecord(INDEX record)
	{
		size_t len = GetFieldCount();
		if ( len != 0 )
		{
			CDataVector dv;
			dv.SetSize(len);
			return m_records.Insert(record, dv);
		}
		return false;
	}

	/**
	 * [削除] レコード削除.
	 *	@param record レコードインデックス
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool DeleteRecord(INDEX record)
	{
		return m_records.Remove(record);
	}

	/**
	 * [設定] レコードNULL設定.
	 *	@param record レコードインデックス
	 *	@param field フィールドインデックス
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetNullRecord(INDEX record, INDEX field)
	{
		TData* D = m_Ref(record, field);
		if ( D != NULL )
		{
			D->SetString(_T(""));
			return true;
		}
		return false;
	}

	/**
	 * [設定] レコード文字列設定.
	 *	@note 文字列のフィールド以外にも設定可能です。
	 *	@param record レコードインデックス
	 *	@param field フィールドインデックス
	 *	@param lpszString 文字列
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetStringRecord(INDEX record, INDEX field, LPCTSTR lpszString)
	{
		TData* D = m_Ref(record, field);
		if ( D != NULL )
		{
			D->SetString(lpszString);
			return true;
		}
		return false;
	}

	/**
	 * [設定] レコード数値設定.
	 *	@note 数値のフィールド以外には設定できません。
	 *	@param record レコードインデックス
	 *	@param field フィールドインデックス
	 *	@param value 値
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetIntegerRecord(INDEX record, INDEX field, int value)
	{
		if ( m_ToKind(field) == EK_Integer )
		{
			TData* D = m_Ref(record, field);
			if ( D != NULL )
			{
				D->SetFloat(value);
				return true;
			}
		}
		return false;
	}

	/**
	 * [設定] レコード実数設定.
	 *	@note 実数のフィールド以外には設定できません。
	 *	@param record レコードインデックス
	 *	@param field フィールドインデックス
	 *	@param value 値
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetFloatRecord(INDEX record, INDEX field, double value)
	{
		if ( m_ToKind(field) == EK_Float )
		{
			TData* D = m_Ref(record, field);
			if ( D != NULL )
			{
				D->SetFloat(value);
				return true;
			}
		}
		return false;
	}

	/**
	 * [設定] レコード日付設定.
	 *	@note 日付のフィールド以外には設定できません。
	 *	@param record レコードインデックス
	 *	@param field フィールドインデックス
	 *	@param date 日付
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetDateRecord(INDEX record, INDEX field, const TDate& date)
	{
		if ( m_ToKind(field) == EK_Date )
		{
			TData* D = m_Ref(record, field);
			if ( D != NULL )
			{
				D->SetFloat(date.Get());
				return true;
			}
		}
		return false;
	}

	/**
	 * [確認] レコードNULL確認
	 *	@param record レコードインデックス
	 *	@param field フィールドインデックス
	 *	@retval true NULL。フィールドインデックス、レコードインデックスが無効の場合も true が返ります。
	 *	@retval false 失敗
	 */
	bool IsNullRecord(INDEX record, INDEX field) const
	{
		const TData* D = m_Ref(record, field);
		if ( D != NULL )
		{
			return (D->hasString && D->string.IsEmpty());
		}
		return true;
	}

	/**
	 * [取得] レコード文字列取得.
	 *	@param record レコードインデックス
	 *	@param field フィールドインデックス
	 *	@param isAdjust true ならフィールドの幅になるようにスペースでパディング、調整します。
	 *	@return 文字列
	 */
	CStr GetStringRecord(INDEX record, INDEX field, bool isAdjust = false) const
	{
		CStr str;
		const TData* D = m_Ref(record, field);
		if ( D != NULL )
		{
			const TFieldInfo& F = m_fields[field];
			EFieldKind k = m_ToKind(F);
			if ( D->hasString )
			{
				// データが文字列なのでそのまま
				str = D->string;
			}
			else if ( k == EK_Float )
			{
				ASSERT( F.decimals > 0 );
				if ( F.type == ET_Floating )
				{
					CStr f;
					f.Format(_T("%%.%de"), F.decimals);
					str.Format(f, D->value);
				}
				else if ( F.type == ET_Numeric )
				{
					CStr f;
					f.Format(_T("%%.%df"), F.decimals);
					str.Format(f, D->value);
				}
				else
				{
					ASSERTLIB( false );
				}
			}
			else
			{
				int v = ToInt(D->value);
				str.Format(_T("%d"), v);
			}
			if ( isAdjust )
			{
				size_t l = str.GetLength();
				if ( l < F.width )
				{
					CStr sp = CStr::Lineup(' ', F.width - l);
					if ( k == EK_String || k == EK_Date )
					{
						str += sp;
					}
					else
					{
						str = sp + str;
					}
				}
				if ( str.GetLength() > F.width )
				{
					if ( k == EK_String || k == EK_Date )
					{
						str = str.Left(F.width);
					}
					else
					{
						str = str.Right(F.width);
					}
				}
			}
		}
		return str;
	}

	/**
	 * [取得] レコード数値取得.
	 *	@param record レコードインデックス
	 *	@param field フィールドインデックス
	 *	@return 数値
	 */
	int GetIntegerRecord(INDEX record, INDEX field) const
	{
		int r = 0;
		const TData* D = m_Ref(record, field);
		if ( D != NULL )
		{
			if ( D->hasString )
			{
				r = D->string.ToInt();
			}
			else
			{
				r = ToInt(D->value);
			}
		}
		return r;
	}

	/**
	 * [取得] レコード実数取得.
	 *	@param record レコードインデックス
	 *	@param field フィールドインデックス
	 *	@return 実数
	 */
	double GetFloatRecord(INDEX record, INDEX field) const
	{
		double r = 0;
		const TData* D = m_Ref(record, field);
		if ( D != NULL )
		{
			if ( D->hasString )
			{
				r = D->string.ToDouble();
			}
			else
			{
				r = D->value;
			}
		}
		return r;
	}

	/**
	 * [取得] レコード日付取得.
	 *	@param record レコードインデックス
	 *	@param field フィールドインデックス
	 *	@return 日付
	 */
	TDate GetDateRecord(INDEX record, INDEX field) const
	{
		TDate d;
		d.Set(GetIntegerRecord(record, field));
		return d;
	}

protected:
	/// Traceダンプ
	void Dump(void)
	{
	#ifdef _DEBUG
		TFieldInfo fi;
		size_t fc = GetFieldCount();
		CWorkMemT<size_t> wd(fc); 
		// フィールド
		TRACE0("    |");
		loop ( i, fc )
		{
			GetFieldInfo(fi, i);
			wd[i] = max(fi.name.GetLength(), fi.width);
			TRACE1("%s", CStr::Fmt(CStr::Fmt(_T("%%-%ds|"), wd[i]), fi.name));
		}
		TRACE0("\n");
		::Sleep(10);
		// フィールド(型)
		TRACE0("    |");
		loop ( i, fc )
		{
			GetFieldInfo(fi, i);
			CStr s = CStr::Fmt(_T("%C,%d,%d"), fi.type, fi.width, fi.decimals);
			TRACE1("%s", CStr::Fmt(CStr::Fmt(_T("%%-%ds|"), wd[i]), s));
		}
		TRACE0("\n");
		::Sleep(10);
		// セパレータ
		TRACE0("----+");
		loop ( i, fc )
		{
			TRACE1("%s", CStr::Lineup('-', wd[i]) + "+");
		}
		TRACE0("\n");
		::Sleep(10);
		// レコード
		loop ( j, GetRecordCount() )
		{
			TRACE1("%4d|", j + 1);
			loop ( i, fc )
			{
				TRACE1("%s", CStr::Fmt(CStr::Fmt(_T("%%-%ds|"), wd[i]), GetStringRecord(j, i, true)));
			}
			TRACE0("\n");
			::Sleep(10);
		}
	#endif // _DEBUG
	}

private:
	/// フィールド情報から種類を
	EFieldKind m_ToKind(const TFieldInfo& info) const
	{
		switch ( info.type )
		{
		case ET_Character:	// 文字列
		case ET_Logical:	// 論理(?,Y,N,T,F)
//		case ET_Memo:		// メモ
			return EK_String;
		case ET_Date:		// 年月日
			return EK_Date;
		case ET_Numeric:	// 数値
		case ET_Floating:
			if ( info.decimals == 0 )
			{
				return EK_Integer;
			}
			return EK_Float;
		default:
			break;
		}
		return EK_String;
//		return EK_Invalid;
	}
	// フィールドインデックスから種類を
	EFieldKind m_ToKind(INDEX field) const
	{
		if ( m_fields.IsInRange(field) )
		{
			return m_ToKind(m_fields[field]);
		}
		return EK_Invalid;
	}
	/// データ型
	struct TData
	{
		bool	hasString;	///< trueなら文字列 falseなら数値
		CStr	string;		///< 文字列
		double	value;		///< 数値
		/// コンストラクタ
		TData(void) : hasString(true)
		{
		}
		/// 文字列設定
		void SetString(LPCTSTR lpsz)
		{
			hasString = true;
			string = lpsz;
		}
		/// 数値
		void SetFloat(double v)
		{
			hasString = false;
			value = v;
		}
		virtual void Deserialize (const IDeserializer& ds)
		{
			ds >> hasString >> string >> value;
		}
		virtual void Serialize (ISerializer& _sr) const
		{
			_sr << hasString << string << value;
		}
	};
	/// データ参照
	const TData* m_Ref(INDEX record, INDEX field) const
	{
		if ( m_fields.IsInRange(field) && m_records.IsInRange(record) )
		{
			const CDataVector& v = m_records[record];
			ASSERT( v.IsInRange(field) );
			return &v[field];
		}
		return NULL;
	}
	/// データ参照
	TData* m_Ref(INDEX record, INDEX field)
	{
		if ( m_fields.IsInRange(field) && m_records.IsInRange(record) )
		{
			CDataVector& v = m_records[record];
			ASSERT( v.IsInRange(field) );
			return &v[field];
		}
		return NULL;
	}

	CVectorT<TFieldInfo>	m_fields;		///< フィールド情報
	typedef CVectorT<TData> CDataVector;	///< 一レコード型
	CVectorT<CDataVector>	m_records;		///< 全レコード情報
	friend class CGridTest;
};



}; // TNB

