#pragma once
/**
 *@file
 * CE専用 INI ファイル情報アクセス関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#ifndef _WIN32_WCE
	#error TnbCeInifileAccessor.h is only supported on Windows CE platforms.
#endif // _WIN32_WCE



#include "TnbInifileTextAccessor.h"
#include "TnbFileName.h"
#include "TnbFile.h"



//TNB Library
namespace TNB
{



/**@ingroup ACCESSOR WINCE
 * iniファイル情報アクセスクラス(CE専用)
 *
 *		セクションで区切られた中に複数のキーと値を持つ形の情報をメモリで管理し、
 *		任意のタイミングで、ファイルに保存しておき、次回情報を復元することが
 *		出来るクラスです。
 *
 *		コンストラクタや SetBase() で情報のベースを指定し、
 *		operator[]() で セクション情報アクセスクラス({@link IAccessor::CSection CSection})を取得し、
 *		Query～() 、 Write～() などでキー情報にアクセスします。
 *
 *	@attention Write～() で情報を変更しても、 Flush() をコールしない限りファイルに書き出しません。
 *
 *	@todo 現在サブセクション処理が不正です。
 *
 *	@note	本インターフェースにも QueryValue() , WriteValue() など、
 *			セクションを指定しキー情報を操作するメソッドもありますが、
 *			基本的に {@link IAccessor::CSection CSection} を使用してください。
 *
 *	@see	IIAccessor を実装しています。
 *
 *	@par必要ファイル
 *			TnbCeInifileAccessor.h
 *
 *	@date 08/07/02 新規作成
 *	@date 12/04/26 親クラス変更
 */
class CCeInifileAccessor : public CInifileTextAccessor
{
	DEFSUPER(CInifileTextAccessor);
public:
	
	/** 
	 * コンストラクタ
	 *	@param lpszFile ファイル名 。省略すると、 MakeInifilePath() の結果が使われます。	 
	 */
	explicit CCeInifileAccessor(LPCTSTR lpszFile = NULL) : _super()
	{
		SetBase(lpszFile);
	}

	/**
	 * [設定] 対象ファイル指定
	 *	@param lpszFile ファイル名
	 */
	void SetBase(LPCTSTR lpszFile = NULL)
	{
		m_strFile = (lpszFile == NULL) ? MakeInifilePath() : lpszFile;
		m_Read(m_strFile);
	}

	/**
	 * [作成] iniファイルパス作成.
	 *		実行ファイル名の .exeを .iniに変えたものを作成します。
	 *	@return iniファイルのFullPath
	 */
	static CStr MakeInifilePath(void)
	{
		CStr str = _super::MakeDefineFilePath();
		str += _T(".ini");
		return str;
	}

	/**
	 * [取得] 対象ファイル取得
	 *	@return ファイル名
	 */
	CStr GetBaseFileName(void) const
	{
		return m_strFile;
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
	 *	@note コンストラクタや SetBase で指定したファイルに変更を書き出します。
	 *		このメソッドをコールしない限り、ファイルは変化しません。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	virtual bool Flush(void)
	{
		CFileName fn = m_strFile;
		try
		{
			m_Write(fn);
		}
		catch(CTnbException& e)
		{
			e.OnCatch();
			return false;
		}
		catch(...)
		{
			return false;
		}
		return true;
	}

private:
	/**
	 * [設定] テキスト設定.
	 *	@param lpszText テキスト
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetText(LPCTSTR lpszText);

	/**
	 * [取得] テキスト取得.
	 *	@return テキスト
	 */
	CStr GetText(void) const;

	/**
	 * ファイル読み込み
	 *	@param lpszFile ファイル名
	 */
	bool m_Read(LPCTSTR lpszFile)
	{
		_super::DeleteAllSection();
		CFileName name = lpszFile;
		if ( ! name.IsExist() )
		{
			return false;
		}
		CFileReader f;
		if ( ! f.Open(name) )
		{
			return false;
		}
		CByteVector vb = f.ReadInto();
		if ( vb.IsEmpty() )
		{
			return false;
		}
		vb.Add(0);
		CStr str = reinterpret_cast<LPCSTR>(vb.ReferBuffer());
		return _super::SetText(str);
	}

	/**
	 * ファイル書き出し
	 *	@param lpszFile ファイル名
	 */
	void m_Write(LPCTSTR lpszFile)
	{
		CFileWriter f;
		if ( ! f.New(lpszFile) )
		{
			return;
		}
		CAscii d = _super::GetText();
		LPCSTR lpsz = d;
		f.Write(d.GetLength(), lpsz);
	}

	CStr			m_strFile;		///< ファイル名
};



/**@ingroup ACCESSOR
 * iniファイル情報アクセスクラス
 *
 *	@note CEでも 同じように CInifileAccessor を使えるように宣言。
 */
typedef CCeInifileAccessor CInifileAccessor;



};//TNB

