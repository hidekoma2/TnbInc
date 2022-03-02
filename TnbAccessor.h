#pragma once
/**
 *@file
 * 情報アクセス関係のヘッダ
 *
 *	@note	マクロ "_TnbACCESSOR_Tree_ENABLE" が定義されていると、 IAccessor::CSection::GetKeyMap() , 
 *			IAccessor::CSection::GetKeyTree() , CAbstractAccessor::GetKeyTree()
 *			を使用可能にします（TnbMap.h TnbTree.h も必要になります）。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbStrVector.h"
#include "TnbCollection.h"
#include "TnbStrOperator.h"
#include "TnbSerializeAdapter.h"
#ifdef _TnbDOXYGEN	//Document作成用シンボル
 #define _TnbACCESSOR_Tree_ENABLE ///< 　
#endif
#ifdef _TnbACCESSOR_Tree_ENABLE
 #include "TnbTree.h"
 #include "TnbMap.h"
#endif



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR
 * 情報アクセスインターフェース.
 *
 *		セクションで区切られた中に複数のキーと値を持つ形の情報を管理するインターフェースです。
 *
 *		コンストラクタなどで情報のベースを指定し、
 *		operator[]() で セクション情報アクセスクラス({@link IAccessor::CSection CSection})を取得し、
 *		Query～() 、 Write～() などでキー情報にアクセスします。
 *
 *	@note	本インターフェースにも QueryValue() , WriteValue() など、
 *			セクションを指定しキー情報を操作するメソッドもありますが、
 *			基本的に {@link IAccessor::CSection CSection} を使用してください。
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *		
 *		CInifileAccessor ini;
 *		CInifileAccessor::CSection sec1 = ini["SEC1"];
 *		CInifileAccessor::CSection sec2 = ini["SEC2"];
 *		sec1.WriteString("KEY1", "VALUE1"); //セクション[SEC1]のキー[KEY1]に"VALUE1"を代入		
 *		sec1.WriteString("KEY2", "VALUE2"); //同セクションにキー[KEY1]に"VALUE1"を代入
 *		sec2.WriteString("KEY9", "VALUE9"); //セクション[SEC2]のキー[KEY9]に"VALUE9"を代入	
 *				;
 * 		CStrVector vs = sec1.EnumKeyNames(); //セクション[SEC1]下のキー名を全取得
 *		</pre></td></tr></table>
 *
 *	@see	SetExceptionMode() により、情報がない時や情報の型が異なる時、 
 *			CNoSuchTypeException や CEmptyException をスローさせることが出来ます。
 *			設定していない時はスローしません。
 *
 *	@par必要ファイル
 *			TnbAccessor.h
 *
 *	@date 06/05/12 新規作成
 *	@date 06/06/08 メンバ名変更。 CSection クラス、追加。
 *	@date 06/10/31 メンバ改修。キーアクセス関係を CSection クラスに移動。
 *	@date 08/04/04 数値に変換時、EMPTY文字なら Defaultを返すようにした。
 */
struct IAccessor
{
	/**
	 * 値型の種類.
	 *	IAccessor と IAccessor::CValue で使う型の種類です。
	 */
	enum EKind
	{
		EK_Nothing		= 'n',	///< 存在しない
		EK_String		= 's',	///< 文字列
		EK_PluralStrings= 'p',	///< 複数の文字列群
		EK_Binary		= 'b',	///< バイナリ
		EK_Dword		= 'd',	///< 32Bit Unsigned Integer
		EK_Longlong		= 'i',	///< 64Bit Signed Integer
	};

	/**@ingroup ACCESSOR
	 * 情報アクセスの汎用値保持クラス.
	 *
	 *	{@link IAccessor::EKind EKind}で定義される型と値を保持できます。
	 *
	 *	@par必要ファイル
	 *			TnbAccessor.h
	 *
	 *	@date 06/05/12 新規作成
	 */
	class CValue
	{
		EKind		m_eKind;		///< 保持情報種
		CStr		m_strData;		///< 文字列,複数の文字列群情報
		LONGLONG	m_llData;		///< Integer 情報
		CByteVector	m_vbData;		///< バイナリ情報
	public:
	
		/**
		 * コンストラクタ 
		 *	@note 情報無しオブジェクトになります。
		 */
		CValue(void) : m_eKind(EK_Nothing) 
		{
		}

		/**
		 * コンストラクタ 
		 *	@param lpsz 文字列。この文字列を持ったオブジェクトになります。
		 */
		CValue(LPCTSTR lpsz) : m_eKind(EK_String), m_strData(lpsz)
		{
		}

		/**
		 * コンストラクタ 
		 *	@param vstr 文字列群。この複数の文字列群を持ったオブジェクトになります。
		 */
		CValue(const CStrVector& vstr) : m_eKind(EK_PluralStrings), m_strData(StrVectorToStrEx(vstr)) 
		{
		}

		/**
		 * コンストラクタ 
		 *	@param c データ。このバイナリを持ったオブジェクトになります。
		 */
		CValue(const IConstCollectionT<BYTE>& c) : m_eKind(EK_Binary)
		{
			m_vbData.Copy(c);
		}

		/**
		 * コンストラクタ 
		 *		指定のバイナリを持ったオブジェクトになります。
		 *	@param len データサイズ.
		 *	@param P データ
		 */
		CValue(size_t len, LPCVOID P) : m_eKind(EK_Binary)
		{
			m_vbData.CopyElements(len, static_cast<const BYTE*>(P));
		}

		/**
		 * コンストラクタ 
		 *	@param dw データ。この32Bit Unsigned Integerを持ったオブジェクトになります。
		 */
		CValue(DWORD dw) : m_eKind(EK_Dword), m_llData(dw) 
		{
		}

		/**
		 * コンストラクタ 
		 *	@param ll データ。この64Bit Signed Integerを持ったオブジェクトになります。
		 */
		CValue(LONGLONG ll) : m_eKind(EK_Longlong), m_llData(ll)
		{
		}

		/**
		 * [取得] 情報取種取得
		 *	@return 情報種
		 */
		EKind GetKind(void) const { return m_eKind; }

		/**
		 * [確認] 情報有無確認
		 *	@retval true 情報有り
		 *	@retval false 情報無し
		 */
		bool IsNull(void) const { return m_eKind == EK_Nothing; }

		/**
		 * [取得] 文字列情報取得
		 *	@note 型が異なっても変換して取り出します
		 *	@return 値
		 */
		CStr QueryString(void) const
		{
			CStr s;
			switch ( m_eKind )
			{
			case EK_String:			// 文字列
			case EK_PluralStrings:	// 複数の文字列群
				s = m_strData;
				break;
			case EK_Binary:			// バイナリ
				s = CStrOperator::BinaryToHexString(m_vbData);
				break;
			case EK_Dword:			// 32Bit
				s = CStr::Fmt(_T("%u"), static_cast<DWORD>(m_llData));
				break;
			case EK_Longlong:		// 64Bit
				s = CStr::Fmt(_T("%I64d"), m_llData);
				break;
			case EK_Nothing:
				break;
			}	
			return s;
		}

		/**
		 * [取得] 文字列群情報取得
		 *	@note 型が異なっても変換して取り出します
		 *	@return 値
		 */
		CStrVector QueryPluralStrings(void) const
		{
			CStrVector vstr;
			if ( m_eKind == EK_PluralStrings || m_eKind == EK_String )
			{
				vstr = StrToStrVectorEx(m_strData);
			}
			return vstr;
		}

		/**
		 * [取得] バイナリ情報取得
		 *	@note 型が異なっても変換して取り出します
		 *	@return 値
		 */
		CByteVector QueryData(void) const
		{
			if ( m_eKind != EK_Binary )
			{
				return CStrOperator::HexStringToBinary(QueryString());
			}
			return m_vbData;
		}

		/**
		 * [取得] 数値情報取得
		 *	@note 型が異なっても変換して取り出します
		 *	@return 値
		 */
		DWORD QueryDword(void) const
		{
			return static_cast<DWORD>(QueryLonglong()); 
		}

		/**
		 * [取得] 数値情報取得
		 *	@note 型が異なっても変換して取り出します
		 *	@return 値
		 */
		LONGLONG QueryLonglong(void) const
		{
			if ( m_eKind == EK_Longlong || m_eKind == EK_Dword )
			{
				return m_llData;
			}
			return QueryString().ToLonglong(); 
		}
	};

	#ifdef _TnbACCESSOR_Tree_ENABLE

	/// キーと値のマップ型宣言
	typedef CMapT<CStr, CValue, LPCTSTR> CKeyMap;

	/// キーと値のツリー型宣言
	typedef CTreeT<CStr, CValue, LPCTSTR> CKeyTree;

	#endif
	
	/**@ingroup ACCESSOR
	 * セクション情報アクセスクラス
	 *
	 *		IAccessor のサブクラスにより作成されるクラスです。
	 *		本インスタンスで IAccessor のサブクラスの指定のセクションの情報にアクセスします。
	 *
	 *	@see IAccessor
	 *
	 *	@attention 本クラスは作成元の IAccessor のサブクラスのインスタンスの参照を持っています。
	 *				必ず本クラスの有効の間は、作成元のインスタンスは破棄しないようにしてください。
	 *
	 *	@par必要ファイル
	 *			TnbAccessor.h
	 *
	 *	@date 06/06/08 新規作成
	 *	@date 06/10/31 メンバ追加
	 *	@date 09/04/02 HWND にも対応
	 *	@date 09/11/12 WriteData() を追加。
	 *	@date 10/05/17 シリアライザーを追加。
	 */
	class CSection
	{
		IAccessor*		m_piAccessor;	///< 情報クラスの参照
		CStr			m_strSection;	///< 扱うセクション名
		/**
		 * CValueチェック
		 *	@retval ture NULL か 文字列からっぽ
		 *	@retval false 値あり
		 */
		bool m_IsEmpty(const CValue& v) const
		{
			bool r = v.IsNull();
			if ( ! r && v.GetKind() == EK_String )
			{
				r = v.QueryString().IsEmpty();
			}
			return r;
		}
		/** コンストラクタ */
		CSection(void);
	public:

		/**
		 * 代入コンストラクタ
		 *	@param I IAccessor を持つインスタンスオブジェクト
		 *	@param lpszSectionName 本クラスで扱うセクション名
		 *	@throw CNullPointerException IAccessorクラスを指定しないとスローされます。
		 */
		CSection(const IAccessor* I, LPCTSTR lpszSectionName) 
			: m_piAccessor(const_cast<IAccessor *>(I)), m_strSection(lpszSectionName)
		{
			if ( I == NULL )
			{
				ASSERT0( false, "CSection::CSection()", "IAccessor が指定されていません。" );
				throw CNullPointerException();
			}
		}

		/**
		 * [取得] セクション名取得
		 *	@return セクション名
		 */
		CStr GetSectionName(void) const
		{
			return m_strSection;
		}

		/**
		 * [確認] 参照可能？
		 *		そのセクションが存在し読み込める、確認できます。
		 *	@retval true 存在。
		 *	@retval false ない。
		 */
		bool CanQuery(void) const
		{
			CStr s = m_strSection;
			INT_PTR d = s.ReverseFind('\\');
			if ( d >= 0 )
			{
				s = s.Left(d);
			}
			CStrVector vs = m_piAccessor->EnumSectionNames();
			return vs.Find(s) != INVALID_INDEX;
		}

		/**
		 * [設定] 対象セクション変更.
		 *		指定のサブセクションを追加し、取得対象をそのセクション内にします。
		 *	@param lpszSubName サブセクション名。
		 *	@retval true 成功
		 *	@retval false 失敗（名前に￥が含まれている、など）
		 */
		bool InSubSection(LPCTSTR lpszSubName)
		{
			if ( STRLIB::GetLen(lpszSubName) > 0 && STRLIB::IndexOf(lpszSubName, '\\') < 0 )
			{
				if ( ! m_strSection.IsEmpty() )
				{
					m_strSection += _T('\\');
				}
				m_strSection += lpszSubName;
				return true;
			}
			return false;
		}

		/**
		 * [設定] 対象セクション変更.
		 *		取得対象を一つ上のセクション内ににします。
		 *	@note IsSubSection() と対になるように使用します。
		 */
		void PreviousSubSection(void)
		{
			INT_PTR d = m_strSection.ReverseFind('\\');
			if ( d >= 0 )
			{
				m_strSection = m_strSection.Left(d);
			}
			else
			{
				m_strSection.Empty();
			}
		}

		/**
		 * [参照] Accessorインターフェース参照
		 *	@return IAccessor
		 */
		const IAccessor* ReferAccessor(void) const
		{
			return m_piAccessor;
		}

		/**
		 * [取得] セクション名一覧取得
		 *	@return セクション名一覧
		 */
		CStrVector EnumSectionNames(void) const
		{
			return m_piAccessor->EnumSectionNames(m_strSection);
		}

		/**
		 * [取得] キー名一覧取得
		 *	@return キー名一覧
		 */
		CStrVector EnumKeyNames(void) const
		{
			return m_piAccessor->EnumKeyNames(m_strSection);
		}

		/**
		 * [削除] 全キー削除
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		bool DeleteAllKeys(void)
		{
			return m_piAccessor->DeleteSection(m_strSection);
		}

		/**
		 * [取得] 情報取種取得
		 *	@param lpszKey キー名
		 *	@return 情報種。ない場合、EK_Nothing になります。
		 */
		EKind GetKeyKind(LPCTSTR lpszKey) const
		{
			return m_piAccessor->GetKeyKind(m_strSection, lpszKey);
		}

		/**
		 * [確認] 情報存在確認
		 *	@param lpszKey キー名
		 *	@retval true 情報あり。
		 *	@retval false 情報無し。
		 */
		bool HasKey(LPCTSTR lpszKey) const
		{
			return GetKeyKind(lpszKey) != EK_Nothing;
		}

		/**
		 * [削除] 指定キー削除
		 *	@param lpszKey キー名。
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		bool DeleteKey(LPCTSTR lpszKey)
		{
			return m_piAccessor->WriteValue(m_strSection, lpszKey, CValue());
		}

		/**
		 * [追加] 情報記録
		 *	@param lpszKey キー名
		 *	@param value キーの値
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		bool WriteValue(LPCTSTR lpszKey, const CValue& value)
		{
			return m_piAccessor->WriteValue(m_strSection, lpszKey, value);
		}

		/**
		 * [取得] 情報取得
		 *	@param lpszKey キー名
		 *	@return キーの値
		 */
		CValue QueryValue(LPCTSTR lpszKey) const
		{
			return m_piAccessor->QueryValue(m_strSection, lpszKey);
		}
		
		/**
		 * [追加] 文字列情報記録
		 *	@param lpszKey キー名
		 *	@param lpszValue キーの値
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		bool WriteString(LPCTSTR lpszKey, LPCTSTR lpszValue)
		{
			return WriteValue(lpszKey, CValue(lpszValue));
		}

		/**
		 * [取得] 文字列情報取得
		 *	@param lpszKey キー名
		 *	@param lpszDefault キーがない時この値を返します。省略すると Empty文字列です。
		 *	@return キーの値
		 */
		CStr QueryString(LPCTSTR lpszKey, LPCTSTR lpszDefault = NULL) const
		{
			const CValue& v = QueryValue(lpszKey);
			return v.IsNull() ? lpszDefault : v.QueryString();
		}

		/**
		 * [追加] 文字列群情報記録
		 *	@param lpszKey キー名
		 *	@param vs キーの値
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		bool WritePluralStrings(LPCTSTR lpszKey, const CStrVector& vs)
		{
			return WriteValue(lpszKey, CValue(vs));
		}

		/**
		 * [取得] 文字列群情報取得
		 *	@param lpszKey キー名
		 *	@return キーの値
		 */
		CStrVector QueryPluralStrings(LPCTSTR lpszKey) const
		{
			return QueryValue(lpszKey).QueryPluralStrings();
		}

		/**
		 * [追加] バイナリ情報記録
		 *	@param lpszKey キー名
		 *	@param c キーの値
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		bool WriteData(LPCTSTR lpszKey, const IConstCollectionT<BYTE>& c)
		{
			return WriteValue(lpszKey, CValue(c));
		}

		/**
		 * [追加] バイナリ情報記録
		 *	@param lpszKey キー名
		 *	@param len キーの値(データサイズ)
		 *	@param P キーの値(データ)
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		bool WriteData(LPCTSTR lpszKey, size_t len, LPCVOID P)
		{
			return WriteValue(lpszKey, CValue(len, P));
		}

		/**
		 * [取得] バイナリ情報取得
		 *	@param lpszKey キー名
		 *	@return キーの値
		 */
		CByteVector QueryData(LPCTSTR lpszKey) const
		{
			return QueryValue(lpszKey).QueryData();
		}

		/**
		 * [追加] 数値情報記録
		 *	@param lpszKey キー名
		 *	@param dwValue キーの値
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		bool WriteDword(LPCTSTR lpszKey, DWORD dwValue)
		{
			return WriteValue(lpszKey, CValue(dwValue));
		}

		/**
		 * [取得] 数値情報取得
		 *	@param lpszKey キー名
		 *	@param dwDefault キーがない時この値を返します。省略すると０です。
		 *	@return キーの値
		 */
		DWORD QueryDword(LPCTSTR lpszKey, DWORD dwDefault = 0) const
		{
			const CValue& v = QueryValue(lpszKey);
			return m_IsEmpty(v) ? dwDefault : v.QueryDword();
		}

		/**
		 * [追加] 数値情報記録
		 *	@param lpszKey キー名
		 *	@param llValue キーの値
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		bool WriteLonglong(LPCTSTR lpszKey, LONGLONG llValue)
		{
			return WriteValue(lpszKey, CValue(llValue));
		}

		/**
		 * [取得] 数値情報取得
		 *	@param lpszKey キー名
		 *	@param llDefault キーがない時この値を返します。省略すると０です。
		 *	@return キーの値
		 */
		LONGLONG QueryLonglong(LPCTSTR lpszKey, LONGLONG llDefault = 0) const
		{
			const CValue& v = QueryValue(lpszKey);
			return m_IsEmpty(v) ? llDefault : v.QueryLonglong();
		}

		/**
		 * [追加] バイナリ情報記録
		 *	@note 構造体のメモリイメージを記録します。
		 *	@attention 構造体は、 Plain Old Data (POD) 型である必要があります。
		 *	@param lpszKey キー名
		 *	@param t キーの値
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		template<typename TYP>
		bool WriteStruct(LPCTSTR lpszKey, const TYP& t)
		{
			CByteVector vb;
			vb.SetElements(sizeof(TYP), reinterpret_cast<const BYTE*>(&t));
			return WriteData(lpszKey, vb);
		}

		/**
		 * [取得] バイナリ情報取得
		 *	@note 構造体のメモリイメージを取得します。
		 *	@attention 構造体は、 Plain Old Data (POD) 型である必要があります。
		 *	@param[in] lpszKey キー名
		 *	@param[out] _t キーの値
		 *	@retval true 成功。
		 *	@retval false 失敗。情報サイズが異なる。
		 */
		template<typename TYP>
		bool QueryStruct(LPCTSTR lpszKey, TYP& _t) const
		{
			const CByteVector& v = QueryData(lpszKey);
			if ( v.GetSize() != sizeof(TYP) )
			{
				return false;
			}
			BYTE* B = reinterpret_cast<BYTE*>(&_t);
			MemCopy(B, v.ReferBuffer(), sizeof(TYP));
			return true;
		}

		/**
		 * [追加] 配列値記録
		 *	@note 配列を記録します。セクション以下を一度消去し、配列数と値を記録します。
		 *	@param vv 配列
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		bool WriteVector(const CVectorT<CValue>& vv)
		{
			bool r = true;
			r &= m_piAccessor->DeleteSection(m_strSection);
			size_t len = vv.GetSize();
			r &= WriteLonglong(_T("size"), len);
			loop ( i, len )
			{
				r &= WriteValue(CStr::Fmt(_T("%d"), i + 1), vv[i]);
			}
			return r;
		}

		/**
		 * [取得] 配列値取得
		 *	@note WriteVector() による書き込んだ配列値を取得します。
		 *	@return 配列
		 */
		CVectorT<CValue> QueryVector(void) const
		{
			CVectorT<CValue> vv;
			size_t len = QueryDword(_T("size"), 0);
			vv.SetSize(len);
			loop ( i, len )
			{
				vv[i] = QueryValue(CStr::Fmt(_T("%d"), i + 1));
			}
			return vv;
		}

		/**
		 * [追加] シリアライザブルデータ情報記録
		 *	@param lpszKey キー名
		 *	@param s シリアライズ可能のデータ
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		bool WriteSerializableData(LPCTSTR lpszKey, const ISerializable& s)
		{
			CByteVector vb;
			try
			{
				CSerializeAdapter sa(&vb);
				sa << s;
				return WriteData(lpszKey, vb);
			}
			catch ( ... )
			{
				return false;
			}
			return false;
		}

		/**
		 * [取得] シリアライザブルデータ情報取得
		 *	@param[in] lpszKey キー名
		 *	@param[out] _d デシリアライズした結果が格納されます。
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		bool QuerySerializableData(LPCTSTR lpszKey, ISerializable& _d) const
		{
			CByteVector vb = QueryData(lpszKey);
			if ( ! vb.IsEmpty() )
			{
				try
				{
					CDeserializeAdapter da(&vb);
					da >> _d;
					return true;
				}
				catch ( ... )
				{
					return false;
				}
			}
			return false;
		}

		#ifdef __AFX_H__
		
		/**
		 * [追加] ウィンドウテキスト情報記録
		 *	@param lpszKey キー名
		 *	@param hWnd ウィンドウハンドル
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		bool WriteWindowText(LPCTSTR lpszKey, HWND hWnd)
		{
			CStr s;
			int l = ::GetWindowTextLength(hWnd);
			if ( l > 0 )
			{
				::GetWindowText(hWnd, s.GetBuffer(l + 2), l + 1);
				s.ReleaseBuffer();
				return WriteString(lpszKey, s);
			}
			return DeleteKey(lpszKey);
		}

		/**
		 * [取得] ウィンドウテキスト情報取得
		 *	@param lpszKey キー名
		 *	@param hWnd ウィンドウハンドル
		 *	@param lpszDefault キーがない時この値を返します。省略すると Empty文字列です。
		 *	@retval true 成功。
		 *	@retval false 失敗。
		 */
		bool QueryWindowText(LPCTSTR lpszKey, HWND hWnd, LPCTSTR lpszDefault = NULL) const
		{
			CStr s = QueryString(lpszKey, lpszDefault);
			return !! ::SetWindowText(hWnd, s);
		}

		#endif

		#ifdef _TnbACCESSOR_Tree_ENABLE

		/**
		 * [取得] 全値取得
		 *	@note 全キーと値をマップに格納します。
		 *	@return マップ
		 */
		CKeyMap GetKeyMap(void) const
		{
			CKeyMap map;
			const CStrVector& vstr = EnumKeyNames();
			loop ( i, vstr.GetSize() )
			{
				map[vstr[i]] = QueryValue(vstr[i]);
			}
			return map;
		}

		/**
		 * [取得] 全値取得
		 *	@note 全キーと値、さらにサブセクション以下もTreeに格納します。
		 *	@param[out] _tree ツリー
		 */
		void GetKeyTree(CKeyTree& _tree) const
		{
			const CStrVector& vs = EnumSectionNames();
			loop ( i, vs.GetSize() )
			{
				CSection s = *this;
				s.InSubSection(vs[i]);
				s.GetKeyTree(_tree.Refer(vs[i]));
				s.PreviousSubSection();
			}
			const CKeyMap& mapKey = GetKeyMap();
			CStr key;
			CValue value;
			loop ( i, mapKey.GetSize() )
			{
				mapKey.Get(i, key, value);
				if ( key.IsEmpty() )
				{
					_tree.AtSelf() = value;
				}
				else
				{
					_tree.Add(key, value);
				}
			}
		}

		/**
		 * [取得] 全値取得
		 *	@note 全キーと値、さらにサブセクション以下もTreeに格納します。
		 *			値はすべて文字列情報に変換します。
		 *	@param[out] _tree ツリー
		 */
		void GetKeyTree(CStrsTree& _tree) const
		{
			const CStrVector& vs = EnumSectionNames();
			loop ( i, vs.GetSize() )
			{
				CSection s = *this;
				s.InSubSection(vs[i]);
				s.GetKeyTree(_tree.Refer(vs[i]));
				s.PreviousSubSection();
			}
			const CKeyMap& mapKey = GetKeyMap();
			CStr key;
			CValue value;
			loop ( i, mapKey.GetSize() )
			{
				mapKey.Get(i, key, value);
				if ( key.IsEmpty() )
				{
					_tree.AtSelf() = value.QueryString();
				}
				else
				{
					_tree.Add(key, value.QueryString());
				}
			}
		}

		#endif
	};

	/// デストラクタ
	virtual ~IAccessor(void)
	{
	}

	/**
	 * [取得] タイプ名取得
	 *	@return タイプ名
	 */
	virtual CStr GetTypeName(void) const = 0;

	/**
	 * [操作] フラッシュ.
	 *	@note Writeした情報を確定させます（Subクラスにより動作がことなります。）。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool Flush(void) = 0;

	/**
	 * [取得] セクション名一覧取得
	 *	@param lpszSectionName セクション名。 NULLを指定すると、ROOTのセクション一覧が得られます。
	 *	@return セクション名一覧
	 */
	virtual CStrVector EnumSectionNames(LPCTSTR lpszSectionName = NULL) const = 0;

	/**
	 * [削除] 指定セクション削除
	 *	@param lpszSectionName セクション名。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool DeleteSection(LPCTSTR lpszSectionName) = 0;

	/**
	 * [取得] キー名一覧取得
	 *	@param lpszSectionName セクション名
	 *	@return 名前一覧。無効状態なら未対応。
	 */
	virtual CStrVector EnumKeyNames(LPCTSTR lpszSectionName) const = 0;

	/**
	 * [取得] 情報取種取得
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@return 情報種。キーが存在しない場合、EK_Nothing になります。
	 */
	virtual EKind GetKeyKind(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const = 0;

	/**
	 * [取得] 情報取得
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@return キーの値
	 */
	virtual CValue QueryValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey) const = 0;

	/**
	 * [設定] 情報設定
	 *	@param lpszSectionName セクション名
	 *	@param lpszKey キー名
	 *	@param value 設定する値。EK_Nothing の場合、キーを削除します。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool WriteValue(LPCTSTR lpszSectionName, LPCTSTR lpszKey, const IAccessor::CValue& value) = 0;
	
	/**
	 * [変換] 配列変換.
	 *		IAccessor::CValue の配列に変換します。
	 *	@param vt 配列
	 *	@return IAccessor::CValue の配列
	 */
	template<typename TYP>
	static CVectorT<IAccessor::CValue> ToValueVector(const CVectorT<TYP>& vt)
	{
		CVectorT<IAccessor::CValue> vv;
		vv.SetSize(vt.GetSize());
		loop ( i, vt.GetSize() )
		{
			vv[i] = CValue(vt[i]);
		}
		return vv;
	}
};



/**@ingroup ACCESSOR
 * 情報アクセス抽象クラス.
 *
 *		セクションで区切られた中に複数のキーと値を持つ形の情報を管理する抽象クラスです。
 *
 *	@see IAccessor
 *
 *	@par必要ファイル
 *			TnbAccessor.h
 *
 *	@date 08/04/16 IAccessor から分離。
 */
class CAbstractAccessor : public IAccessor
{
	DEFSUPER(IAccessor);
public:

	/**
	 * [取得] CSection取得
	 *	@param lpszSectionName セクション名
	 *	@return 指定のセクションをアクセスする CSection
	 */
	CSection operator[](LPCTSTR lpszSectionName)
	{
		return CSection(this, lpszSectionName);
	}

	/**
	 * [取得] CSection取得
	 *	@param lpszSectionName セクション名
	 *	@return 指定のセクションをアクセスする CSection
	 */
	const CSection operator[](LPCTSTR lpszSectionName) const
	{
		return CSection(this, lpszSectionName);
	}

	/**
	 * [操作] フラッシュ.
	 *	@note 常に成功します。本クラスでは何もしません。
	 *	@return true 成功
	 */
	virtual bool Flush(void)
	{
		return true; 
	}

	/**
	 * [削除] 全セクション削除
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool DeleteAllSection(void)
	{
		bool r = true;
		CStrVector vs = EnumSectionNames(NULL);
		loop ( i, vs.GetSize() )
		{
			r &= DeleteSection(vs[i]);
		}
		return r;
	}

	#ifdef _TnbACCESSOR_Tree_ENABLE

	/**
	 * [取得] 全値取得
	 *	@note 全キーと値、さらにサブセクション以下もTreeに格納します。
	 *	@param[out] _tree ツリー
	 */
	void GetKeyTree(CKeyTree& _tree) const
	{
		const CStrVector& vstr = EnumSectionNames();
		CSection(this, _T("")).GetKeyTree(_tree);
		loop ( i, vstr.GetSize() )
		{
			CSection(this, vstr[i]).GetKeyTree(_tree.Refer(vstr[i]));
		}
	}

	/**
	 * [取得] 全値取得
	 *	@note 全キーと値、さらにサブセクション以下もTreeに格納します。
	 *			値はすべて文字列情報に変換します。
	 *	@param[out] _tree ツリー
	 */
	void GetKeyTree(CStrsTree& _tree) const
	{
		const CStrVector& vstr = EnumSectionNames();
		CSection(this, _T("")).GetKeyTree(_tree);
		loop ( i, vstr.GetSize() )
		{
			CSection(this, vstr[i]).GetKeyTree(_tree.Refer(vstr[i]));
		}
	}

	#endif

	/**
	 * [作成] 定義ファイル名作成.
	 *		実行ファイル名の .exeを省いたフルパスを返します。。
	 *	@return フルパス
	 */
	static CStr MakeDefineFilePath(void)
	{
		CStr str;
		DWORD dwRc = ::GetModuleFileName(NULL, str.GetBuffer(MAX_PATH), MAX_PATH);
		str.ReleaseBuffer();
		ASSERTLIB(dwRc != 0);
		IgnoreUnusedValue(dwRc);
		//
		CStr strWork;
		strWork = str.Left(str.ReverseFind('.'));
		#if defined(_DEBUG) && !defined(_WIN32_WCE)
			strWork += _T(".");
			CStr s;
			DWORD dwLength = MAX_COMPUTERNAME_LENGTH + 1;
			::GetComputerName(s.GetBuffer(dwLength), &dwLength);
			s.ReleaseBuffer();
			strWork += s;
		#endif
		return strWork;
	}
};



/**@ingroup ACCESSOR
 * ディープ検索クラス
 *
 *		セクションをを深く掘り込んで検索するクラスです。
 *		本クラスを継承し、 OnFindSection() に処理を実装します。
 *
 *	@par必要ファイル
 *			TnbAccessor.h
 *
 *	@date 10/02/17 新規作成
 */
class CDeepAccessFinder
{
public:

	/// コンストラクタ
	CDeepAccessFinder(void) : m_depth(0)
	{
	}

	/**
	 * [実行] 検索開始.
	 *	@param section 検索するセクションを指定.
	 */
	void Execute(IAccessor::CSection& section)
	{
		IAccessor::CSection sec = section;
		m_depth = 0;
		m_Find(sec);
	}

protected:

	/// 結果
	enum EResult
	{
		Next,		///< 次検索
		Previous,	///< 一つ上の階層へ
		Stop,		///< 検索停止
	};

	/**
	 * [通知] セクション発見.
	 *		セクションを発見した時に通知されます。
	 *	@note 本メソッドは Next を返します。
	 *	@param lpszSection 発見セクション名.
	 *	@param sec 発見セクションアクセスクラス.
	 *	@param depth 深さ
	 *	@return 結果
	 */
	virtual EResult OnFindSection(LPCTSTR lpszSection, IAccessor::CSection& sec, int depth)
	{
		return Next;
	}

private:
	/// 検索
	EResult m_Find(IAccessor::CSection& sec)
	{
		m_depth++;
		CStrVector vs = sec.EnumSectionNames();
		loop ( i, vs )
		{
			IAccessor::CSection s = sec;
			if ( s.InSubSection(vs[i]) )
			{
				EResult r = OnFindSection(vs[i], s, m_depth);
				if ( r == Next )
				{
					r = m_Find(s);
				}
				if ( r == Previous )
				{
					break;
				}
				if ( r != Next )
				{
					m_depth--;
					return Stop;
				}
			}
		}
		m_depth--;
		return Next;
	}
	int		m_depth;		///< 深さ
};



#ifndef _WIN32_WCE

/**@ingroup ACCESSOR
 * ウィンドウの状態を保存.
 *		指定の CSection に状態を保存します。
 *		(現在はサイズと位置のみ対応)
 *
 *	@note	読み出しは RestoreWindowPlacement を使います。
 *
 *	@par必要ファイル
 *			TnbAccessor.h
 *
 *	@param[out] _sec IAccessor のサブクラスから得た CSection の参照。
 *	@param[in] hWnd 状態を保存する対象。
 *	@retval true 成功
 *	@retval false 失敗
 */
inline bool StoreWindowPlacement(IAccessor::CSection& _sec, HWND hWnd)
{
	bool boRc = true;
	WINDOWPLACEMENT t = { 0 };
	t.length = sizeof(WINDOWPLACEMENT);
	if ( ! ::GetWindowPlacement(hWnd, &t) ){ return false; }
	boRc &= _sec.WriteDword(_T("Valid"), 1);
	boRc &= _sec.WriteLonglong(_T("SX"), t.rcNormalPosition.left);
	boRc &= _sec.WriteLonglong(_T("SY"), t.rcNormalPosition.top);
	boRc &= _sec.WriteLonglong(_T("EX"), t.rcNormalPosition.right);
	boRc &= _sec.WriteLonglong(_T("EY"), t.rcNormalPosition.bottom);
	boRc &= _sec.WriteDword(_T("Show"), ::IsWindowVisible(hWnd));
	boRc &= _sec.WriteDword(_T("Maximize"), ::IsZoomed(hWnd));
	return boRc;
}



/**@ingroup ACCESSOR
 * ウィンドウの状態を復元.
 *		指定の CSection から状態を復元します。
 *		(現在はサイズと位置のみ対応)
 *
 *	@note	書き出しは StoreWindowPlacement を使います。
 *	@note	情報がない場合、何もしませんので、本関数をコールの前にhWnd に対し
 *			デフォルトの設定を行っておくようにします。
 *
 *	@par必要ファイル
 *			TnbAccessor.h
 *
 *	@param[in] sec IAccessor のサブクラスから得た CSection の参照。
 *	@param hWnd 状態を復元する対象。
 *	@param withWindowSize true なら WindowSizeもリストア対象にする。
 *	@param withShowHide true なら Show/Hide もリストア対称にする。
 *	@retval true 設定
 *	@retval false 未処理
 */
inline bool RestoreWindowPlacement(const IAccessor::CSection& sec, HWND hWnd, bool withWindowSize = true, bool withShowHide = false)
{
	if ( sec.QueryDword(_T("Valid"), 0) == 0 )
	{
		return false;
	}
	WINDOWPLACEMENT t = { 0 };
	t.length = sizeof(WINDOWPLACEMENT);
	if ( ! ::GetWindowPlacement(hWnd, &t) )
	{
		return false;
	}
	int width = t.rcNormalPosition.right - t.rcNormalPosition.left;
	int height = t.rcNormalPosition.bottom	- t.rcNormalPosition.top;
	t.rcNormalPosition.left		= static_cast<LONG>(sec.QueryLonglong(_T("SX"), 0));
	t.rcNormalPosition.top		= static_cast<LONG>(sec.QueryLonglong(_T("SY"), 0));
	if ( withWindowSize )
	{
		t.rcNormalPosition.right	= static_cast<LONG>(sec.QueryLonglong(_T("EX"), 100));
		t.rcNormalPosition.bottom	= static_cast<LONG>(sec.QueryLonglong(_T("EY"), 100));
	}
	else
	{
		t.rcNormalPosition.right = t.rcNormalPosition.left + width;
		t.rcNormalPosition.bottom = t.rcNormalPosition.top + height;
	}
	//
	DWORD cx = t.rcNormalPosition.right - t.rcNormalPosition.left;
	DWORD cy = t.rcNormalPosition.bottom - t.rcNormalPosition.top;
	if ( cx > 4000 || cy > 4000 )
	{
		return false;
	}
	t.flags		= WPF_SETMINPOSITION;
	t.showCmd	= SW_HIDE;
	::SetWindowPlacement(hWnd, &t);
	if ( withShowHide )
	{
		DWORD s = sec.QueryDword(_T("Show"), TRUE);
		::ShowWindow(hWnd, (s != 0) ? SW_SHOW : SW_HIDE);
	}
	if ( !! sec.QueryDword(_T("Maximize"), FALSE) )
	{
		::ShowWindow(hWnd, SW_MAXIMIZE);
	}
	return true;
}

#endif //_WIN32_WCE



}; // TNB
