#pragma once
/**
 *@file
 * 情報相互アクセス関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbAccessor.h"
#include "TnbSerializer.h"



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR
 * 情報相互アクセスクラス
 *
 *		IAccessor を持つクラスのアクセス補助クラスです。
 *
 *		IAccessor へのアクセス（Query / Write）を行う変数と管理用のキーを
 *		一元に管理することが出来るようになります。
 *
 *	@par使用例
 *		<table><tr><td><pre>
 *		void CFoo::Init(void)
 *		{
 *			CInifileAccessor ini;
 *			CMutualAccessor a(&ini, CMutualAccessor::QUERY);
 *			BackupRestore(a);
 *		}
 *
 *		void CFoo::Exit(void)
 *		{
 *			CInifileAccessor ini;
 *			CMutualAccessor a(&ini, CMutualAccessor::WRITE);
 *			BackupRestore(a);
 *		}
 *		
 *		bool CFoo::BuckupRestore(CMutualAccessor& _a)
 *		{
 *			CMutualAccessor::CSubSection sub(&_a, _T("Settings"));
 *			//
 *			bool r = _a.Works("DeviceNumber", m_wDeviceNumber, 1);
 *			r &= _a.Works("ModelNo", m_wModelNo, 3);
 *			r &= _a.Works("HardVersion", m_wHardVersion, 3);
 *			//
 *			CStrVector vs;
 *			vs.Add("NoConnect");
 *			vs.Add("Connect");
 *			r &= _a.Works("Mode", m_dwMode, vs, 0);
 *			return r;
 *		}
 *
 *		</pre></td></tr></table>
 *
 *	@par必要ファイル
 *			TnbMutualAccessor.h
 *
 *	@date 08/04/01 新規作成
 *	@date 08/04/15 OnFalse() 追加。 CSubSectionクラス 追加。
 *	@date 10/01/15 構造更新。
 *	@date 11/02/11 double にも対応。
 */
class CMutualAccessor
{
public:

	/**@ingroup ACCESSOR
	 * 情報相互アクセスのワーカーインターフェース
	 *
	 *		CMutualAccessor のサポートインターフェースです。
	 *
	 *		このインターフェースを持っているオブジェクトを CMutualAccessor::Works() で処理することが出来ます。
	 *
	 *	@par必要ファイル
	 *			TnbMutualAccessor.h
	 *
	 *	@date 10/01/15 新規作成
	 */
	struct IWorker
	{
		/// デストラクタ
		virtual ~IWorker(void) {}

		/**
		 * [処理]
		 *	@param[in,out] sec セクション.
		 *	@param[in] lpszKey キー.
		 *	@param[in] isQuery true なら QUERYモード、false ならWRITEモード。
		 *	@retval true 成功。
		 *	@retval false 失敗。 WRITEモードの場合は書込みに失敗した時（QUERYモードは失敗はありません）。
		 */
		virtual bool MutualAccessWorks(IAccessor::CSection& sec, LPCTSTR lpszKey, bool isQuery) = 0;

		#ifndef _TnbDOXYGEN	//Document作成用シンボル
			virtual bool Works(IAccessor::CSection& sec, LPCTSTR lpszKey, bool isQuery)
			{
				return MutualAccessWorks(sec, lpszKey, isQuery);
			}
		#endif
	};

	/**@ingroup ACCESSOR
	 * 情報相互アクセスサブセクションクラス
	 *
	 *		CMutualAccessor のサポートクラスです。
	 *
	 *	@see CMutualAccessor
	 *
	 *	@par必要ファイル
	 *			TnbMutualAccessor.h
	 *
	 *	@date 08/04/15 新規作成
	 */
	class CSubSection
	{
	public:
		/**
		 * コンストラクタ
		 *	@param P CMutualAccessor のポインタ
		 *	@param lpszSub サブセクション名
		 */
		CSubSection(CMutualAccessor* P, LPCTSTR lpszSub) : m_pMu(P), m_success(false)
		{
			m_success = m_pMu->InSubSection(lpszSub);
		}
		/// デストラクタ
		~CSubSection(void)
		{
			if ( m_success )
			{
				m_pMu->PreviousSubSection();
			}
		}
	private:
		CMutualAccessor*	m_pMu;		///< 扱う CMutualAccessor
		bool				m_success;	///< InSubSectionが成功したか
	};


	//----------------------------------------------------------------


	/// モード
	enum EMode
	{
		QUERY,	///< QUERYモード
		WRITE,	///< WRITEモード
	};

	/**
	 * コンストラクタ
	 *	@param pAccessor IAccessor のポインタ。 NULL は指定できません。
	 *	@param mode 本インスタンスのモード。 省略か QUERY なら QUERYモード、 WRITE ならWRITEモードを指定。
	 */
	CMutualAccessor(IAccessor* pAccessor, EMode mode = QUERY) 
		: m_sec(IAccessor::CSection(pAccessor, _T(""))), m_isQuery(mode == QUERY)
	{
	}

	/**
	 * コンストラクタ
	 *	@param sec セクション。
	 *	@param mode 本インスタンスのモード。 省略か QUERY なら QUERYモード、 WRITE ならWRITEモードを指定。
	 */
	CMutualAccessor(IAccessor::CSection& sec, EMode mode = QUERY) 
		: m_sec(sec), m_isQuery(mode == QUERY)
	{
	}

	/**
	 * [設定] 対象セクション変更.
	 *		指定のサブセクションを追加し、取得対象をそのセクション内にします。
	 *	@param lpszSection セクション名
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool InSubSection(LPCTSTR lpszSection)
	{
		return m_Result(m_sec.InSubSection(lpszSection));
	}

	/**
	 * [設定] 対象セクション変更.
	 *		取得対象を一つ上のセクション内ににします。
	 *	@note IsSubSection() と対になるように使用します。
	 */
	void PreviousSubSection(void)
	{
		m_sec.PreviousSubSection();
	}

	/**
	 * [確認] モード確認
	 *	@retval true QUERYモード
	 *	@retval false WRITEモード
	 */
	bool IsQueryMode(void) const
	{
		return m_isQuery; 
	}

	/**
	 * [確認] モード確認
	 *	@retval false QUERYモード
	 *	@retval true WRITEモード
	 */
	bool IsWriteMode(void) const
	{
		return ! m_isQuery; 
	}

	/**
	 * [参照] CSection 参照.
	 *	@note 本クラスが使用している CSection を参照します。
	 *		Works() メソッドだけでは処理できない場合、モードを確認して本メソッドを使って、処理が可能です。
	 *	@return CSection 。
	 */
	IAccessor::CSection* operator->(void)
	{
		return &m_sec;
	}

	/**
	 * [処理] bool 情報処理.
	 *		bool 情報を Query / Write します。
	 *	@param[in] lpszKey キー。
	 *	@param[in,out] _boValue 値。 QUERYモードなら取得した値をこれに格納。WRITEモードならこの値を書込み。
	 *	@param[in] def QUERYモードのみ使用。指定のキーがない場合、この値が戻り値とします。
	 *	@retval true 成功。
	 *	@retval false 失敗。 QUERYモードの時は、値が不明文字列の場合( _boValue は def が入ります)。\n
	 *						WRITEモードの場合は書込みに失敗した時。
	 */
	bool Works(LPCTSTR lpszKey, bool& _boValue, bool def = false)
	{
		CStrVector vs;
		vs.Add(_T("false"));
		vs.Add(_T("true"));
		DWORD w = _boValue;
		bool r = Works(lpszKey, w, vs, def);
		_boValue = (w != 0);
		return m_Result(r);
	}

	/**
	 * [処理] BYTE 情報処理.
	 *		BYTE 情報を Query / Write します。
	 *	@param[in] lpszKey キー。
	 *	@param[in,out] _bValue 値。 QUERYモードなら取得した値をこれに格納。WRITEモードならこの値を書込み。
	 *	@param[in] def QUERYモードのみ使用。指定のキーがない場合、この値が戻り値とします。
	 *	@retval true 成功。
	 *	@retval false 失敗。 WRITEモードの場合は書込みに失敗した時（QUERYモードは失敗はありません）。
	 */
	bool Works(LPCTSTR lpszKey, BYTE& _bValue, BYTE def = 0)
	{
		if ( m_isQuery )
		{
			DWORD w = m_sec.QueryDword(lpszKey, def);
			_bValue = static_cast<BYTE>(w);
			return true;
		}
		return m_Result(m_sec.WriteDword(lpszKey, _bValue));
	}

	/**
	 * [処理] WORD 情報処理.
	 *		WORD 情報を Query / Write します。
	 *	@param[in] lpszKey キー。
	 *	@param[in,out] _wValue 値。 QUERYモードなら取得した値をこれに格納。WRITEモードならこの値を書込み。
	 *	@param[in] def QUERYモードのみ使用。指定のキーがない場合、この値が戻り値とします。
	 *	@retval true 成功。
	 *	@retval false 失敗。 WRITEモードの場合は書込みに失敗した時（QUERYモードは失敗はありません）。
	 */
	bool Works(LPCTSTR lpszKey, WORD& _wValue, WORD def = 0)
	{
		if ( m_isQuery )
		{
			DWORD w = m_sec.QueryDword(lpszKey, def);
			_wValue = static_cast<WORD>(w);
			return true;
		}
		return m_Result(m_sec.WriteDword(lpszKey, _wValue));
	}

	/**
	 * [処理] short 情報処理.
	 *		short 情報を Query / Write します。
	 *	@param[in] lpszKey キー。
	 *	@param[in,out] _value 値。 QUERYモードなら取得した値をこれに格納。WRITEモードならこの値を書込み。
	 *	@param[in] def QUERYモードのみ使用。指定のキーがない場合、この値が戻り値とします。
	 *	@retval true 成功。
	 *	@retval false 失敗。 WRITEモードの場合は書込みに失敗した時（QUERYモードは失敗はありません）。
	 */
	bool Works(LPCTSTR lpszKey, short& _value, short def = 0)
	{
		if ( m_isQuery )
		{
			DWORD w = m_sec.QueryDword(lpszKey, def);
			_value = static_cast<short>(w);
			return true;
		}
		return m_Result(m_sec.WriteDword(lpszKey, _value));
	}

	/**
	 * [処理] DWORD 情報処理.
	 *		DWORD 情報を Query / Write します。
	 *	@param[in] lpszKey キー。
	 *	@param[in,out] _dwValue 値。 QUERYモードなら取得した値をこれに格納。WRITEモードならこの値を書込み。
	 *	@param[in] def QUERYモードのみ使用。指定のキーがない場合、この値が戻り値とします。
	 *	@retval true 成功。
	 *	@retval false 失敗。 WRITEモードの場合は書込みに失敗した時（QUERYモードは失敗はありません）。
	 */
	bool Works(LPCTSTR lpszKey, DWORD& _dwValue, DWORD def = 0)
	{
		if ( m_isQuery )
		{
			_dwValue = m_sec.QueryDword(lpszKey, def);
			return true;
		}
		return m_Result(m_sec.WriteDword(lpszKey, _dwValue));
	}

	/**
	 * [処理] int 情報処理.
	 *		int 情報を Query / Write します。
	 *	@param[in] lpszKey キー。
	 *	@param[in,out] _iValue 値。 QUERYモードなら取得した値をこれに格納。WRITEモードならこの値を書込み。
	 *	@param[in] def QUERYモードのみ使用。指定のキーがない場合、この値が戻り値とします。
	 *	@retval true 成功。
	 *	@retval false 失敗。 WRITEモードの場合は書込みに失敗した時（QUERYモードは失敗はありません）。
	 */
	bool Works(LPCTSTR lpszKey, int& _iValue, int def = 0)
	{
		if ( m_isQuery )
		{
			LONGLONG l = m_sec.QueryLonglong(lpszKey, def);
			_iValue = static_cast<int>(l);
			return true;
		}
		return m_Result(m_sec.WriteLonglong(lpszKey, _iValue));
	}

	/**
	 * [処理] long 情報処理.
	 *		long 情報を Query / Write します。
	 *	@param[in] lpszKey キー。
	 *	@param[in,out] _lValue 値。 QUERYモードなら取得した値をこれに格納。WRITEモードならこの値を書込み。
	 *	@param[in] def QUERYモードのみ使用。指定のキーがない場合、この値が戻り値とします。
	 *	@retval true 成功。
	 *	@retval false 失敗。 WRITEモードの場合は書込みに失敗した時（QUERYモードは失敗はありません）。
	 */
	bool Works(LPCTSTR lpszKey, long& _lValue, long def = 0)
	{
		if ( m_isQuery )
		{
			LONGLONG l = m_sec.QueryLonglong(lpszKey, def);
			_lValue = static_cast<long>(l);
			return true;
		}
		return m_Result(m_sec.WriteLonglong(lpszKey, _lValue));
	}

	/**
	 * [処理] LONGLONG 情報処理.
	 *		LONGLONG 情報を Query / Write します。
	 *	@param[in] lpszKey キー。
	 *	@param[in,out] _llValue 値。 QUERYモードなら取得した値をこれに格納。WRITEモードならこの値を書込み。
	 *	@param[in] def QUERYモードのみ使用。指定のキーがない場合、この値が戻り値とします。
	 *	@retval true 成功。
	 *	@retval false 失敗。 WRITEモードの場合は書込みに失敗した時（QUERYモードは失敗はありません）。
	 */
	bool Works(LPCTSTR lpszKey, LONGLONG& _llValue, LONGLONG def = 0)
	{
		if ( m_isQuery )
		{
			_llValue = m_sec.QueryLonglong(lpszKey, def);
			return true;
		}
		return m_Result(m_sec.WriteLonglong(lpszKey, _llValue));
	}

	/**
	 * [処理] LONGLONG 情報処理.
	 *		LONGLONG 情報を Query / Write します。
	 *	@param[in] lpszKey キー。
	 *	@param[in,out] _value 値。 QUERYモードなら取得した値をこれに格納。WRITEモードならこの値を書込み。
	 *	@param[in] def QUERYモードのみ使用。指定のキーがない場合、この値が戻り値とします。
	 *	@retval true 成功。
	 *	@retval false 失敗。 WRITEモードの場合は書込みに失敗した時（QUERYモードは失敗はありません）。
	 */
	bool Works(LPCTSTR lpszKey, double& _value, double def = 0.0)
	{
		if ( m_isQuery )
		{
			CByteVector vb = m_sec.QueryData(lpszKey);
			_value = def;
			if ( vb.GetSize() == sizeof(double) )
			{
				_value = *reinterpret_cast<const double*>(vb.ReferBuffer());

			}
			return true;
		}
		return m_Result(m_sec.WriteData(lpszKey, CConstAdapterT<BYTE>(sizeof(double), &_value)));
	}

	/**
	 * [処理] 文字列情報処理.
	 *		文字列情報を Query / Write します。
	 *	@param[in] lpszKey キー。
	 *	@param[in,out] _strValue 値。 QUERYモードなら取得した値をこれに格納。WRITEモードならこの値を書込み。
	 *	@param[in] def QUERYモードのみ使用。指定のキーがない場合、この値が戻り値とします。
	 *	@retval true 成功。
	 *	@retval false 失敗。 WRITEモードの場合は書込みに失敗した時（QUERYモードは失敗はありません）。
	 */
	bool Works(LPCTSTR lpszKey, CStr& _strValue, LPCTSTR def = NULL)
	{
		if ( m_isQuery )
		{
			_strValue = m_sec.QueryString(lpszKey, def);
			return true;
		}
		return m_Result(m_sec.WriteString(lpszKey, _strValue));
	}

	/**
	 * [処理] レガシー構造体情報処理.
	 *		構造体を Query / Write します。
	 *	@param[in] lpszKey キー。
	 *	@param[in] size P のバッファサイズ。
	 *	@param[in,out] P 値。 QUERYモードなら取得した情報をこれに格納。WRITEモードならこの情報を書込み。
	 *	@retval true 成功。
	 *	@retval false 失敗。 WRITEモードの場合は書込みに失敗した時（QUERYモードは失敗はありません）。
	 */
	bool Works(LPCTSTR lpszKey, size_t size, LPVOID P)
	{
		if ( m_isQuery )
		{
			CByteVector vb = m_sec.QueryData(lpszKey);
			memset(P, 0, size);
			if ( vb.GetSize() == size )
			{
				vb.GetElements(size, static_cast<BYTE*>(P));
			}
			return true;
		}
		return m_Result(m_sec.WriteData(lpszKey, CConstAdapterT<BYTE>(size, P)));
	}

	/**
	 * [処理] コレクション情報処理.
	 *		コレクション情報を Query / Write します。
	 *	@param[in] lpszKey キー。
	 *	@param[in,out] _c 値。 QUERYモードなら取得した情報をこれに格納。WRITEモードならこの情報を書込み。
	 *	@retval true 成功。
	 *	@retval false 失敗。 WRITEモードの場合は書込みに失敗した時（QUERYモードは失敗はありません）。
	 */
	bool Works(LPCTSTR lpszKey, ICollectionT<BYTE>& _c)
	{
		if ( m_isQuery )
		{
			CByteVector vb = m_sec.QueryData(lpszKey);
			_c.Copy(vb);
			return true;
		}
		return m_Result(m_sec.WriteData(lpszKey, _c));
	}

	/**
	 * [処理] シリアライザブル情報処理.
	 *		シリアライザブル情報を Query / Write します。
	 *	@param[in] lpszKey キー。
	 *	@param[in,out] _s 値。 QUERYモードなら取得した情報をこれに格納。WRITEモードならこの情報を書込み。
	 *	@retval true 成功。
	 *	@retval false 失敗。 WRITEモードの場合は書込みに失敗した時（QUERYモードは失敗はありません）。
	 */
	bool Works(LPCTSTR lpszKey, ISerializable& _s)
	{
		ASSERTLIB( false ); // このメソッドは潜在的なバグのある可能性があります。
		CSubSection sub(this, lpszKey);
		bool r = false;
		try
		{
			CMySerializer sr(&m_sec);
			if ( m_isQuery )
			{
				sr >> _s;
			}
			else
			{
				m_sec.DeleteAllKeys();
				sr << _s;
			}
			r = true;
		}
		catch ( CNoSuchTypeException& e )
		{
			e.OnCatch();
		}
		return r;
	} 

	/**
	 * [処理] DWORD 情報処理.
	 *		DWORD 情報をQuery / Write します。その際、値と対になる文字列配列を使って、情報を管理します。
	 *	@param[in] lpszKey キー。
	 *	@param[in,out] _dwValue 値。QUERYモードなら取得した値をこれに格納。WRITEモードならこの値を書込み。
	 *	@param[in] vs 値を対になる文字列配列。
	 *	@param[in] def QUERYモードのみ使用。指定のキーがない場合、この値が戻り値とします。
	 *	@retval true 成功。
	 *	@retval false 失敗。 QUERYモードの時は、値が不明文字列の場合( _dwValue は def が入ります)。\n
	 *						WRITEモードの場合は書込みに失敗した時。
	 */
	bool Works(LPCTSTR lpszKey, DWORD& _dwValue, const CStrVector& vs, DWORD def = 0)
	{
		if ( m_isQuery )
		{
			_dwValue = def;
			if ( ! m_sec.HasKey(lpszKey) )
			{
				return true;
			}
			CStr s = m_sec.QueryString(lpszKey);
			INDEX r = vs.Find(s);
			if ( r != INVALID_INDEX )
			{
				_dwValue = r;
				return true;
			}
			return m_Result(false);
		}
		if ( vs.GetSize() > _dwValue )
		{
			return m_Result(m_sec.WriteString(lpszKey, vs[_dwValue]));
		}
		m_sec.DeleteKey(lpszKey);
		return m_Result(false);
	}

	/**
	 * [処理] ワーカー情報処理.
	 *		ワーカー情報をQuery / Write します。
	 *	@param[in] lpszKey キー。
	 *	@param[in,out] _worker ワーカー。
	 *	@retval true 成功。
	 *	@retval false 失敗。WRITEモードの場合は書込みに失敗した時。
	 */
	bool Works(LPCTSTR lpszKey, IWorker& _worker)
	{
		bool r = _worker.Works(m_sec, lpszKey, m_isQuery);
		return m_Result(r);
	}

#ifdef __AFX_H__
	/**
	 * [処理] 文字列情報処理.
	 *		文字列情報を Query / Write します。
	 *	@param[in] lpszKey キー。
	 *	@param[in,out] _strValue 値。 QUERYモードなら取得した値をこれに格納。WRITEモードならこの値を書込み。
	 *	@param[in] def QUERYモードのみ使用。指定のキーがない場合、この値が戻り値とします。
	 *	@retval true 成功。
	 *	@retval false 失敗。 WRITEモードの場合は書込みに失敗した時（QUERYモードは失敗はありません）。
	 */
	bool Works(LPCTSTR lpszKey, CString& _strValue, LPCTSTR def = NULL)
	{
		if ( m_isQuery )
		{
			_strValue = m_sec.QueryString(lpszKey, def);
			return true;
		}
		CStr s = _strValue;
		return m_Result(m_sec.WriteString(lpszKey, s));
	}

	/**
	 * [処理] ウィンドウテキスト情報処理.
	 *		ウィンドウテキスト情報を Query / Write します。
	 *	@param[in] lpszKey キー。
	 *	@param[in,out] hWnd ウィンドウハンドル。QUERYモードなら取得したテキストをこのウィンドウに設定。WRITEモードならこのウィンドウのテキストを書込み。
	 *	@param[in] def QUERYモードのみ使用。指定のキーがない場合、この値が戻り値とします。
	 *	@retval true 成功。
	 *	@retval false 失敗。 WRITEモードの場合は書込みに失敗した時（QUERYモードは失敗はありません）。
	 */
	bool Works(LPCTSTR lpszKey, HWND hWnd, LPCTSTR def = NULL)
	{
		if ( m_isQuery )
		{
			m_sec.QueryWindowText(lpszKey, hWnd, def);
			return true;
		}
		return m_Result(m_sec.WriteWindowText(lpszKey, hWnd));
	}

	/**
	 * [処理] コンボボックス情報処理.
	 *		コンボボックス情報を Query / Write します。
	 *	@param[in] lpszKey キー。
	 *	@param[in,out] comboBox コンボボックス。QUERYモードなら取得したインデックスでコンボボックスを選択。WRITEモードなら選択されているインデックスを書込み。
	 *	@param[in] def QUERYモードのみ使用。指定のキーがない場合、この値をインデックスとします。
	 *	@retval true 成功。
	 *	@retval false 失敗。 WRITEモードの場合は書込みに失敗した時（QUERYモードは失敗はありません）。
	 */
	bool Works(LPCTSTR lpszKey, CComboBox& comboBox, int def = 0)
	{
		if ( m_isQuery )
		{
			comboBox.SetCurSel(m_sec.QueryDword(lpszKey, def));
			return true;
		}
		return m_Result(m_sec.WriteDword(lpszKey, comboBox.GetCurSel()));
	}

	/**
	 * [処理] ボタン情報処理.
	 *		ボタン情報を Query / Write します。ボタンは通常「チェックボックス」のものを使います。
	 *	@param[in] lpszKey キー。
	 *	@param[in,out] button ボタン。QUERYモードなら取得した値でボタン状態を設定。WRITEモードならボタン状態をインデックスを書込み。
	 *	@param[in] def QUERYモードのみ使用。指定のキーがない場合、この値をボタン状態とします。
	 *	@retval true 成功。
	 *	@retval false 失敗。 WRITEモードの場合は書込みに失敗した時（QUERYモードは失敗はありません）。
	 */
	bool Works(LPCTSTR lpszKey, CButton& button, int def = BST_UNCHECKED)
	{
		if ( m_isQuery )
		{
			button.SetCheck(m_sec.QueryDword(lpszKey, def));
			return true;
		}
		return m_Result(m_sec.WriteDword(lpszKey, button.GetCheck()));
	}
#endif //__AFX_H__

protected:

	/**
	 * [通知] 各処理失敗通知.
	 *	@note 各処理が失敗した時に通知されます。このメソッドを継承して Throw するなど、処理を変更できます。
	 *	@note 本クラスでは、何もしません。
	 */
	virtual void OnFalse(void) const
	{
	}

private:

	bool					m_isQuery;		///< true なら QUERYモード、false ならWRITEモード。
	IAccessor::CSection		m_sec;			///< セッション
	///> 戻り値チェック
	bool m_Result(bool r) const
	{
		if ( ! r )
		{
			OnFalse();
		}
		return r;
	}

	// シリアライザー
	class CMySerializer : public ISerializer, public IDeserializer
	{
	public:
		/**
		 * コンストラクタ.
		 *	@param P セクション
		 */
		CMySerializer(IAccessor::CSection* P) : m_pSec(P), m_count(0)
		{
		}
		/**
		 * [追加] 追加.
		 *	@param size 追加するデータサイズ(単位BYTE)
		 *	@param P 追加するデータ
		 */
		virtual void AddRaw(size_t size, LPCVOID P)
		{
			m_pSec->WriteData(CStr::Fmt(_T("%04d"), m_count), CConstAdapterT<BYTE>(size, P));
			m_count++;
		}
		/**
		 * [取得] 取得.
		 *	@param[in] size 取得するデータサイズ(単位BYTE)
		 *	@param[out] _P 取得するデータ
		 */
		virtual void GetRaw(size_t size, LPVOID _P) const
		{
			CByteVector vb = m_pSec->QueryData(CStr::Fmt(_T("%04d"), m_count));
			if ( size != vb.GetSize() )
			{
				throw CNoSuchTypeException();
			}
			vb.GetElements(size, static_cast<BYTE*>(_P));
			m_count++;
		}
	private:
		mutable int				m_count;	///< カウンタ
		IAccessor::CSection*	m_pSec;		///< セクション
	};
};



}; // TNB
