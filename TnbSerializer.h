#pragma once
/**
 *@file
 * シリアライザー関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbException.h"



//T-TestCaseコードカバレッジDisable
#pragma comment(user,"T-Coverage Disable")



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document作成用シンボル
//前方宣言
struct ISerializer;
struct IDeserializer;
#endif



/**@ingroup SERIALIZE
 * シリアライザブルインターフェース.
 *
 *		シリアライズ機能を実装するためのインターフェースです。
 *		このインターフェースが実装されていたら ISerializer , IDeserializer のサブクラスで
 *		シリアライズ/デシリアライズが可能です（コレクタの要素に使用しても可能）。
 *
 *	@par必要ファイル
 *			TnbSerializer.h
 *
 *	@date 06/08/17 新規作成
 */
struct ISerializable
{
	/// デストラクタ
	virtual ~ISerializable(void) {}
	/**
	 * [通知] シリアライズ
	 *	@param[out] _sr シリアライザー。 これに対しメンバの内容を追加してください。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void Serialize(ISerializer& _sr) const = 0;
	/**
	 * [通知] デシリアライズ
	 *	@param ds デシリアライザー。これからメンバの内容を取り出してください。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void Deserialize(const IDeserializer& ds) = 0;
};



/**@ingroup SERIALIZE
 * シリアライザーインターフェースクラス.
 *
 *		基本型、および、 ISerializable を実装したクラス
 *		( ICollectionT もこれを実装しています)、
 *		および、 CStrT 系 はシリアライズ可能になっています。
 *
 *		シリアライズしたデータは、同じ型、同じ順番でデシリアライズする必要があります。
 *		
 *	@par使用例
 *	<table><tr><td><pre>
 *
 *		void Foo(ISerializer& _sr)
 *		{
 *			int i;
 *			CStr str;
 *			_sr << i << str;			// int と文字列をシリアライズする。			
 *		}
 *	</pre></td></tr>
 *
 *	@note その他の型(MFCクラスやユーザクラスなど)は以下のようなオペレータを用意することでシリアライズに対応可能です。
 *	@par
 *	<table><tr><td><pre>
 *
 *		struct TMyType {	//ユーザクラス
 *		{
 *			CStr strKey;
 *			int  iValue;
 *		}
 *
 *		inline ISerializer& operator<<(ISerializer& _sr, const TMyType& t)		
 *		{
 *			_sr << t.strKey << t.iValue;
 *			return _sr;
 *		}
 *	</pre></td></tr>
 *
 *	@par必要ファイル
 *			TnbSerializer.h
 *
 *	@date 06/08/17 新規作成
 *	@date 08/04/16 機能整理
 *	@date 14/08/13 仮想デストラクタ実装
 */
struct ISerializer
{
protected:

	#ifndef _TnbDOXYGEN	//Document作成用シンボル

	template<typename T> void m_AddSub(const ISerializable* P, const T& t)
	{
		P->Serialize(*this);
	}

	template<typename T> void m_AddSub(LPCVOID P, const T& t)
	{
		TRACE1A("NotSupport-Serialize[%s]\n", typeid(T).name());
		throw CNotSupportException();
	}

	#define _SERIDEF(TY) virtual void m_AddSub(const TY* P, const TY& ty) { AddRaw(sizeof(TY), &ty); }
	_SERIDEF(bool)
	_SERIDEF(BYTE)
	_SERIDEF(char)
	_SERIDEF(WORD)
	_SERIDEF(short)
	_SERIDEF(DWORD)
	_SERIDEF(long)
	_SERIDEF(UINT)
	_SERIDEF(int)
	_SERIDEF(ULONGLONG)
	_SERIDEF(LONGLONG)
	_SERIDEF(long double)
	_SERIDEF(double)
	_SERIDEF(float)
	#undef _SERIDEF

	#endif //_TnbDOXYGEN

public:

	/// デストラクタ
	virtual ~ISerializer(void) 
	{
	}
	
	/**
	 * [追加] 追加.
	 *		指定データをシリアライズして追加します。
	 *	@param t データ
	 *	@return 自分の参照
 	 *	@throw CNotSupportException シリアライズをサポートしていない型が指定されると、スローされます。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	template<typename T> ISerializer& operator<<(const T& t)
	{
		TRACE1A("Serialize[%s]\n", typeid(T).name());
		m_AddSub(&t, t);
		return *this;
	}

	/**
	 * [追加] 追加.
	 *		指定データを追加します。
	 *	@note pure関数です。サブクラスでは本メソッドを実装する必要があります。
	 *	@param size 追加するデータサイズ(単位BYTE)
	 *	@param P 追加するデータ
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void AddRaw(size_t size, LPCVOID P) = 0;
};



/**@ingroup SERIALIZE
 * デシリアライザーインターフェースクラス.
 *
 *		基本型、および、 ISerializable を実装したクラス
 *		( ICollectionT もこれを実装しています)、
 *		および、 CStrT 系 はデシリアライズ可能になっています。
 *
 *		シリアライズしたデータは、同じ型、同じ順番でデシリアライズする必要があります。
 *
 *	@par使用例
 *	<table><tr><td><pre>
 *
 *		void Foo(IDeserializer& ds)
 *		{
 *			int i;
 *			CStr str;
 *			ds >> i >> str;			// int と文字列を取り出す。		
 *		}
 *	</pre></td></tr>
 *
 *	@note その他の型(MFCクラスやユーザクラスなど)は以下のようなオペレータを用意することでデシリアライズに対応可能です。
 *	@par
 *	<table><tr><td><pre>
 *
 *		struct TMyType {	//ユーザクラス
 *		{
 *			CStr strKey;
 *			int  iValue;
 *		}
 *
 *		inline const IDeserializer& operator>>(const IDeserializer& ds, TMyType& t)		
 *		{
 *			ds >> t.strKey >> t.iValue;
 *			return sr;
 *		}
 *	</pre></td></tr>
 *
 *	@par必要ファイル
 *			TnbSerializer.h
 *
 *	@date 06/08/17 新規作成
 *	@date 08/04/16 機能整理
 *	@date 14/08/13 仮想デストラクタ実装
 */
struct IDeserializer
{
private:

	#ifndef _TnbDOXYGEN	//Document作成用シンボル

	template<typename T>
	void m_AssaySub(ISerializable* P, T& t) const
	{
		P->Deserialize(*this);
	}

	template<typename T> void m_AssaySub(LPVOID P, T& t) const
	{
		TRACE1A("NotSupport-Deserialize[%s]\n", typeid(T).name());
		throw CNotSupportException();
	}

	#define _SERIDEF(TY) virtual void m_AssaySub(TY* P, TY& ty) const { GetRaw(sizeof(TY), &ty); }
	_SERIDEF(bool)
	_SERIDEF(BYTE)
	_SERIDEF(char)
	_SERIDEF(WORD)
	_SERIDEF(short)
	_SERIDEF(DWORD)
	_SERIDEF(long)
	_SERIDEF(UINT)
	_SERIDEF(int)
	_SERIDEF(ULONGLONG)
	_SERIDEF(LONGLONG)
	_SERIDEF(long double)
	_SERIDEF(double)
	_SERIDEF(float)
	#undef _SERIDEF

	#endif //_TnbDOXYGEN

public:

	/// デストラクタ
	virtual ~IDeserializer(void) 
	{
	}

	/**
	 * [取得] 取得.
	 *		指定データをデシリアライズして取得します。
	 *	@param[out] _t データ
	 *	@return 自分の参照
 	 *	@throw CNotSupportException デシリアライズをサポートしていない型が指定されると、スローされます。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	template<typename T> const IDeserializer& operator>>(T& _t) const
	{
		TRACE1A("Deserialize[%s]\n", typeid(T).name());
		m_AssaySub(&_t, _t);
		return *this;
	}

	/**
	 * [取得] 取得.
	 *		指定データを取得します。
	 *	@note pure関数です。サブクラスでは本メソッドを実装する必要があります。
	 *	@param[in] size 取得するデータサイズ(単位BYTE)
	 *	@param[out] _P 取得するデータ
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void GetRaw(size_t size, LPVOID _P) const = 0;
};



/**@ingroup SERIALIZE
 * シリアライズタグクラス.
 *
 *		シリアライズ機能のサポートクラスです。
 *		シリアライズ時に、タグ（情報名）を持った本クラスのインスタンスを入れることで、
 *		デシリアライズ時に、タグ（情報名）チェックを行うことで誤ったデータのデシリアライズをガードできます
 *		（情報名に差異があると、 CInvalidParamException がスローされる）。
 *		
 *	@par使用例
 *	<table><tr><td><pre>
 *
 *		void CFoo::Serialize(ISerializer& _sr)
 *		{
 *			int i;
 *			CStr str;
 *			_sr << CSerializeTag(_T("CFooData"));	//情報名を追加
 *			_sr << i << str;			// int と文字列をシリアライズする。			
 *		}
 *
 *		void CFoo::Deserialize(IDeserializer& ds)
 *		{
 *			int i;
 *			CStr str;
 *			ds >> CSerializeTag(_T("CFooData"));	//情報名をチェック
 *			ds >> i >> str;			// int と文字列を取り出す。		
 *		}
 *
 *	</pre></td></tr>
 *
 *	@par必要ファイル
 *			TnbSerializer.h
 *
 *	@date 08/04/16 新規作成
 */
class CSerializeTag : public ISerializable
{
	CArrayPtrHandleT<TCHAR>	m_hpTagName;
public:
	/** 
	 * コンストラクタ
	 *	@param tagName タグネーム
	 */
	CSerializeTag(LPCTSTR tagName)
	{
		size_t size = STRLIB::GetLen(tagName);
		m_hpTagName = new TCHAR[size + 1];
		STRLIB::Copy(m_hpTagName, tagName);
	}
	/**
	 * [通知] シリアライズ
	 *	@param[out] _sr シリアライザー。 これに対しメンバの内容を追加してください。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void Serialize(ISerializer& _sr) const
	{
		LPCTSTR P = m_hpTagName;
		size_t l = STRLIB::GetLen(m_hpTagName);
		_sr.AddRaw(l * sizeof(TCHAR), P);
	}
	/**
	 * [通知] デシリアライズ
	 *	@param ds デシリアライザー。これからメンバの内容を取り出してください。
	 *	@throw CInvalidParamException タグ名が異なる時、スローされます。
	 *	@throw CTnbException 失敗時は、任意のスローの可能性があります。
	 */
	virtual void Deserialize(const IDeserializer& ds)
	{
		size_t l = STRLIB::GetLen(m_hpTagName);
		CWorkMemT<TCHAR> mem(l);
		ds.GetRaw(l * sizeof(TCHAR), mem);
		if ( STRLIB::Compare(mem, m_hpTagName, l) != 0 )
		{
			throw CInvalidParamException();
		}
	}
};



}; // TNB



//T-TestCaseコードカバレッジEnable
#pragma comment(user,"T-Coverage Enable")
