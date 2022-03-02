#pragma once
/**
 *@file
 * ファイルネーム関係のヘッダ
 *
 *		ファイル名管理のクラスがあります。
 *		ファイル名から、有無、属性、サイズ、日付などが取得できます。
 *		また、属性、日付の変更が出来ます。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbFile.h"
#include "TnbStrEx.h"
#include "TnbDntStr.h"
#ifndef _WIN32_WCE
 #include <SHLWAPI.h>
 #include <shellapi.h>
 #pragma comment(lib,"SHLWAPI.lib")
 #include <ShObjIdl.h>
 #include <ShlGuid.h>
#endif



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN
 #ifndef FOF_NOERRORUI
  #define FOF_NOERRORUI 			 0x0400  // don't put up error UI
 #endif
#endif



/**@ingroup FILE
 * ファイル名管理クラス
 *
 *		ファイル名から、有無、属性、サイズ、日付などが取得できます。
 *		また、属性、日付の変更が出来ます。
 *
 *	@par必要ファイル
 *			TnbFileName.h
 *
 *	@date 06/05/01	新規作成。
 *	@date 10/10/05	構成変更。 CopyEx() , InsertAtEndOfName() 新規。
 *	@date 11/09/16	IsShortcut(), MakeShortcut() , ResolveShortcut() 新規。
 *	@date 12/01/24	InsertAtEndOfName() の仕様、改良。
 *	@date 12/05/11	MakeDirectory() で無限再起呼び出しの可能性があった。修正。
 *	@date 13/09/02	GetSizeOnDisk(), GetBytesOfCluster() 新規。
 */
class CFileName
{
public:

	/**
	 * コンストラクタ
	 *	@note 保持する名前はカレントパス名になります。
	 */
	CFileName(void) : m_strName(TNB::GetProcessPath()), m_boIsExists(false)
	{
		IsExist();
	}

	/**
	 * 代入コンストラクタ
	 *	@param lpszName ファイル名
	 */
	CFileName(LPCTSTR lpszName) : m_boIsExists(false)
	{
		operator=(lpszName);
	}

	/**
	 * 代入コンストラクタ.
	 *		指定した文字列を加工せず記憶します。
	 *	@param lpszName ファイル名
	 *	@param dummy ダミー。
	 */
	CFileName(LPCTSTR lpszName, bool dummy) : m_boIsExists(false)
	{
		m_strName = lpszName;
		m_strName.TrimRight('\\');
		IsExist();
	}

	/**
	 * [設定] 代入オペレーション
	 *	@param lpszName ファイル名
	 *	@return 自分の参照
	 */
	CFileName& operator=(LPCTSTR lpszName)
	{
#ifndef _WIN32_WCE
		CStr s1, s2;
		bool r = !! ::PathSearchAndQualify(lpszName, s1.GetBuffer(MAX_PATH), MAX_PATH);
		s1.ReleaseBuffer();
		if ( r )
		{
			r = (::GetLongPathName(s1, s2.GetBuffer(MAX_PATH), MAX_PATH) != 0);
			s2.ReleaseBuffer();
		}
		if ( r )
		{
			m_strName = s2;
		}
		else
#endif
		{
			m_strName = lpszName;
		}
		m_strName.TrimRight('\\');
		IsExist();
		return *this;
	}

	/**
	 * [設定] 代入
	 *	@note この関数は、ファイルの存在と情報を確認ずみの情報を格納する時に使用します。
	 *	@param lpszName ファイル名
	 *	@param tFileAttr ファイル情報
	 */
	void Set(LPCTSTR lpszName, const WIN32_FILE_ATTRIBUTE_DATA& tFileAttr)
	{
		m_strName = lpszName;
		m_strName.TrimRight('\\');
		m_tInfo = tFileAttr;
		m_boIsExists = true;
	}

	/**
	 * [設定] 代入
	 *	@note この関数は、ファイルの存在と情報を確認ずみの情報を格納する時に使用します。
	 *	@param lpszPath パス名
	 *	@param t ファイル情報
	 */
	void Set(LPCTSTR lpszPath, const WIN32_FIND_DATA& t)
	{
		m_strName.Format(_T("%s\\%s"), lpszPath, t.cFileName);
		m_strName.Replace(_T("\\\\"), _T("\\"));
		m_strName.TrimRight('\\');
		m_tInfo.dwFileAttributes	= t.dwFileAttributes;
		m_tInfo.ftCreationTime		= t.ftCreationTime;
		m_tInfo.ftLastAccessTime	= t.ftLastAccessTime;
		m_tInfo.ftLastWriteTime		= t.ftLastWriteTime;
		m_tInfo.nFileSizeHigh		= t.nFileSizeHigh;
		m_tInfo.nFileSizeLow		= t.nFileSizeLow;
		m_boIsExists = true;
	}

	/**
	 * [確認] 比較.
	 *		同じファイルを指しているか、調べることが出来ます。
	 *	@param lpszName ファイル名
	 *	@retval true 同じ
	 *	@retval false 異なる
	 */
	bool IsSameFile(LPCTSTR lpszName) const
	{
		CFileName N(lpszName);
		CStr str = N.GetFullShortName();
		return str.CompareNoCase(GetFullShortName()) == 0;
	}

	/**
	 * [取得] フルName名取得
	 *	@return フルName名
	 */
	operator LPCTSTR(void) const
	{
		return m_strName;
	}

	/**
	 * [取得] フルName名取得
	 *	@return フルName名
	 */
	CStr GetFullName(void) const
	{
		return m_strName;
	}

	/**
	 * [取得] FullShortName名取得
	 *	@return Name名
	 */
	CStr GetFullShortName(void) const
	{
		#ifndef _WIN32_WCE
			CStr str;
			::GetShortPathName(m_strName, str.GetBuffer(MAX_PATH), MAX_PATH);
			str.ReleaseBuffer();
			return str;
		#else
			return m_strName;
		#endif
	}

	/**
	 * [取得] ファイル名取得
	 *	@return ファイル名
	 */
	CStr GetFileName(void) const
	{
		INT_PTR p = m_strName.ReverseFindOneOf(_T("\\/"));
		if ( p >= 0 )
		{
			return m_strName.Mid(p + 1);
		}
		return m_strName;
	}

	/**
	 * [取得] ファイルタイトル取得
	 *	@note ファイル名から拡張子を除いたものを取得できます。
	 *	@return ファイルタイトル
	 */
	CStr GetFileTitle(void) const
	{
		CStr s = GetFileName();
		INDEX idx = s.Find(_T('.'));
		if ( idx != INVALID_INDEX )
		{
			s = s.Left(idx);
		}
		return s;
	}

	/**
	 * [取得] ShortName名取得
	 *	@return Name名
	 */
	CStr GetShortName(void) const
	{
		CStr s = GetFullShortName();
		INT_PTR p = s.ReverseFindOneOf(_T("\\/"));
		if ( p >= 0 )
		{
			return s.Mid(p + 1);
		}
		return s;
	}

	/**
	 * [取得] 拡張子取得
	 *	@return 拡張子
	 */
	CStr GetExtName(void) const
	{
		INT_PTR p = m_strName.ReverseFind('.');
		INT_PTR p1 = m_strName.ReverseFindOneOf(_T("\\/"));
		if ( p >= 0 && p1 < p )
		{
			return m_strName.Mid(p + 1);
		}
		return _T("");
	}

	/**
	 * [取得] パス取得
	 *	@return パス
	 */
	CStr GetPathName(void) const
	{
		INT_PTR p = m_strName.ReverseFindOneOf(_T("\\/"));
		if ( p >= 0 )
		{
			return m_strName.Left(p);
		}
		return m_strName;
	}

	/**
	 * [取得] ファイルサイズ取得
	 *	@note 有効なファイル名を指定している時のみ成功します。
	 *	@retval ０以上 ファイルサイズ
	 *	@retval マイナス　失敗
	 */
	LONGLONG GetSize(void) const
	{
		LONGLONG r = -1;
		if ( m_boIsExists )
		{
			LARGE_INTEGER i;
			i.HighPart = m_tInfo.nFileSizeHigh;
			i.LowPart = m_tInfo.nFileSizeLow;
			r = i.QuadPart;
		}
		return r;
	}

#ifndef _WIN32_WCE
	/**
	 * [取得] ディスク上のサイズ取得
	 *	@note ディスク上で占有しているサイズを返します。
	 *	@retval ０以上 ファイルサイズ
	 *	@retval マイナス 取得失敗
	 */
	LONGLONG GetSizeOnDisk(void) const
	{
		LONGLONG r = -1;
		::SetLastError(NO_ERROR);
		CStr fn = GetFullName();
		WIN32_FILE_ATTRIBUTE_DATA t;
		if ( ::GetFileAttributesEx(fn, GetFileExInfoStandard, &t) )
		{
			LARGE_INTEGER l;
			if ( (t.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
			{
				// DIR の場合
				l.HighPart = t.nFileSizeHigh;
				l.LowPart = t.nFileSizeLow;
				r = l.QuadPart;
			}
			else
			{
				// FILE の場合
				DWORD h;
				l.LowPart = ::GetCompressedFileSize(fn, &h);
				if ( ::GetLastError() == 0 )
				{
					l.HighPart = h;
					r = l.QuadPart;
					// クラスタサイズ、セクターサイズ
					DWORD sectorsPerCluster;
					DWORD bytesPerSector;
					DWORD numberOfFreeClusters;
					DWORD totalNumberOfClusters;
					if ( ::GetDiskFreeSpace(GetPathName(), &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters) )
					{
						DWORD d = sectorsPerCluster * bytesPerSector; // セクター byte数
						r = (r + d - 1) / d * d;
					}
					else
					{
						r = -1;
					}
				}
			}
		}
		return r;
	}

	/**
	 * [取得] クラスタサイズ取得.
	 *	@note ディスク上で占有しているサイズを返します。
	 *	@retval ０以外 クラスタバイト数
	 *	@retval ０ 取得失敗
	 */
	DWORD GetBytesOfCluster(void) const
	{
		if ( m_boIsExists )
		{
			CStr fn = GetFullName();
			if ( ! IsDirectory() )
			{
				fn = GetPathName();
			}
			// クラスタサイズ、セクターサイズ
			DWORD sectorsPerCluster;
			DWORD bytesPerSector;
			DWORD numberOfFreeClusters;
			DWORD totalNumberOfClusters;
			if ( ::GetDiskFreeSpace(GetPathName(), &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters) )
			{
				return sectorsPerCluster * bytesPerSector; // セクター byte数
			}
		}
		return 0;
	}
#endif

	/**
	 * [取得] ファイル情報取得
	 *	@note 有効なファイル名を指定している時のみ成功します。
	 *	@param[out] P ファイル情報
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool GetAttributeData(LPWIN32_FILE_ATTRIBUTE_DATA P) const
	{
		bool r = IsExist();
		if ( r && P != NULL )
		{
			*P = m_tInfo;
		}
		return r;
	}

	/**
	 * [取得] ファイル情報取得
	 *	@note 有効なファイル名を指定している時のみ成功します。
	 *	@param[out] P ファイル情報
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool GetFindData(LPWIN32_FIND_DATA P) const
	{
		bool r = IsExist();
		if ( r && P != NULL )
		{
			STRLIB::Copy(P->cFileName, GetFileName());
			P->dwFileAttributes	= m_tInfo.dwFileAttributes;
			P->ftCreationTime	= m_tInfo.ftCreationTime;
			P->ftLastAccessTime	= m_tInfo.ftLastAccessTime;
			P->ftLastWriteTime	= m_tInfo.ftLastWriteTime;
			P->nFileSizeHigh	= m_tInfo.nFileSizeHigh;
			P->nFileSizeLow	= m_tInfo.nFileSizeLow;
			#if !defined(_WIN32_WCE)
				STRLIB::Copy(P->cAlternateFileName, GetShortName());
			#endif
		}
		return r;
	}

	/**
	 * [取得] ファイル名末文字列追加.
	 *		ファイル名の末に文字列を追加したファイル名を返します。
	 *	@note 本インスタンスの管理するファイル名は変更しません。
	 *	@param lpsz 追加する文字列。
	 *	@param lpszExt 変更する拡張子。省略、 NULL なら、変更しません。
	 *	@return 加工したファイルパス。
	 */
	CStr InsertAtEndOfName(LPCTSTR lpsz, LPCTSTR lpszExt = NULL) const
	{
		CStr s = GetFullName();
		INT_PTR p = s.ReverseFind('.');
		INT_PTR p1 = s.ReverseFindOneOf(_T("\\/"));
		if ( p >= 0 && p1 < p )
		{
			if ( lpszExt != NULL )
			{
				return s.Left(p) + lpsz + _T(".") + lpszExt;
			}
			return s.Left(p) + lpsz + s.Mid(p);
		}
		if ( lpszExt != NULL )
		{
			return s + lpsz + _T(".") + lpszExt;
		}
		return s + lpsz;
	}
	
	/**
	 * [確認] 有無チェック
	 *	@retval true 存在
	 *	@retval false ない
	 */
	bool IsExist(void) const
	{
		m_boIsExists = ms_GetInfo(m_strName, &m_tInfo);
		return m_boIsExists;
	}

	/**
	 * [確認] ReadOnly属性チェック
	 *	@retval true あり
	 *	@retval false なし
	 */
	bool IsReadOnly(void) const { return m_IsAttr(FILE_ATTRIBUTE_READONLY); }

	/**
	 * [確認] Directory属性チェック
	 *	@retval true あり
	 *	@retval false なし
	 */
	bool IsDirectory(void) const { return m_IsAttr(FILE_ATTRIBUTE_DIRECTORY); }

	/**
	 * [確認] Compressed属性チェック
	 *	@retval true あり
	 *	@retval false なし
	 */
	bool IsCompressed(void) const { return m_IsAttr(FILE_ATTRIBUTE_COMPRESSED); }

	/**
	 * [確認] System属性チェック
	 *	@retval true あり
	 *	@retval false なし
	 */
	bool IsSystem(void) const { return m_IsAttr(FILE_ATTRIBUTE_SYSTEM); }

	/**
	 * [確認] Hidden属性チェック
	 *	@retval true あり
	 *	@retval false なし
	 */
	bool IsHidden(void) const { return m_IsAttr(FILE_ATTRIBUTE_HIDDEN); }

	/**
	 * [確認] Temporary属性チェック
	 *	@retval true あり
	 *	@retval false なし
	 */
	bool IsTemporary(void) const { return m_IsAttr(FILE_ATTRIBUTE_TEMPORARY); }

	/**
	 * [確認] Normal属性チェック
	 *	@retval true あり
	 *	@retval false なし
	 */
	bool IsNormal(void) const { return m_IsAttr(FILE_ATTRIBUTE_NORMAL); }

	/**
	 * [確認] Archived属性チェック
	 *	@retval true あり
	 *	@retval false なし
	 */
	bool IsArchived(void) const { return m_IsAttr(FILE_ATTRIBUTE_ARCHIVE); }

	/**
	 * [取得] 最終書込み日時取得
	 *	@return 日時。失敗するとEmpty状態の CFileTimeEx になる。
	 */
	CFileTimeEx GetLastWriteTime(void) const
	{
		CFileTimeEx ft;
		if ( m_boIsExists ){ ft = m_tInfo.ftLastWriteTime; }
		return ft;
	}

	/**
	 * [取得] 最終アクセス日時取得
	 *	@return 日時。失敗するとEmpty状態の CFileTimeEx になる。
	 */
	CFileTimeEx GetLastAccessTime(void) const
	{
		CFileTimeEx ft;
		if ( m_boIsExists ){ ft = m_tInfo.ftLastAccessTime; }
		return ft;
	}

	/**
	 * [取得] 作成日時取得
	 *	@return 日時。失敗するとEmpty状態の CFileTimeEx になる。
	 */
	CFileTimeEx GetCreationTime(void) const
	{
		CFileTimeEx ft;
		if ( m_boIsExists ){ ft = m_tInfo.ftCreationTime; }
		return ft;
	}

	/**
	 * [設定] ReadOnly属性設定
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetReadOnly(void){ return m_SetAttr(FILE_ATTRIBUTE_READONLY); }

	/**
	 * [設定] Hidden属性設定
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetHidden(void){ return m_SetAttr(FILE_ATTRIBUTE_HIDDEN); }

	/**
	 * [設定] Normal属性設定
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool SetNormal(void){ return m_SetAttr(FILE_ATTRIBUTE_NORMAL); }

	/**
	 * [設定] ファイル日付設定
	 *	@param timeCreation 作成日付
	 *	@param timeLastAccess 最終アクセス日付
	 *	@param timeLastWrite 書込み日付
	 *	@retval 0 成功
	 *	@retval 0以外 エラー。数値はシステムエラーコードです( SystemErrorToMessageText() で文字列化できます)。
	 */
	DWORD SetTime(const CFileTimeEx& timeCreation, const CFileTimeEx& timeLastAccess, const CFileTimeEx& timeLastWrite)
	{
		CFileWriter file;
		if ( file.Open(m_strName) )
		{
			::SetLastError(0);
			file.SetTime(timeCreation, timeLastAccess, timeLastWrite);
		}
		return ::GetLastError();
	}

	/**
	 * [作成] ファイル一覧作成
	 *	@note 保持名がDirectoryの場合、以下のファイル/フォルダを検索します。
	 *	@param lpszOpt 検索用のオプション。省略すると ￥*.* が指定されます。
	 *	@return フルパスファイル名一覧
	 */
	CStrVector GetFiles(LPCTSTR lpszOpt = _T("\\*.*"))
	{
		CStrVector vstrResult;
		if ( IsDirectory() )
		{
			WIN32_FIND_DATA t;
			HANDLE h = ::FindFirstFile(m_strName + lpszOpt, &t);
			if ( h != INVALID_HANDLE_VALUE )
			{
				while ( true )
				{
					if ( t.cFileName[0] != '.' )
					{
						vstrResult.Add(CStr::Fmt(_T("%s\\%s"), m_strName, t.cFileName));
					}
					if ( ! ::FindNextFile(h, &t) )
					{
						::FindClose(h);
						break;
					}
				}
			}
		}
		return vstrResult;
	}

	/**
	 * [作成] フォルダ作成.
	 *	@note 階層が深い場合、すべて作成します。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool MakeDirectory(void)
	{
		if ( ! ::CreateDirectory(m_strName, NULL) )
		{
			DWORD e = ::GetLastError();
			if ( e == ERROR_NOT_READY )
			{
				return false;
			}
			if ( e != ERROR_ALREADY_EXISTS )
			{
				CStr n = GetPathName();
				if ( m_strName == n )
				{
					return false;
				}
				CFileName(n).MakeDirectory();
				if ( ! ::CreateDirectory(m_strName, NULL) )
				{
					return false;
				}
			}
		}
		IsExist();
		return true;
	}

	/**
	 * [操作] リネーム.
	 *	@note ファイルでもディレクトリでもリネーム可能です。
	 *	@note	成功すると、本オブジェクトで記憶しているファイル名もリネーム後のものになります。
	 *	@param	lpszNew			リネーム後名。
	 *	@param	boIsAllowUndo	trueならUNDOに残るようにします。
	 *							false or省略するとUNDOに残しません。
	 *	@param	boIsSilent		falseならUIを出し、ユーザに確認させます。
	 *							true or省略なら UIを一切出しません。
	 *	@retval true 成功
	 *	@retval false 失敗（キャンセルも含む）
	 */
	bool Rename(LPCTSTR lpszNew, bool boIsAllowUndo = false, bool boIsSilent = true)
	{
		if ( IsExist() )
		{
			CFileName fnNew = m_FullPath(lpszNew);
			if ( m_ShFileOp(FO_RENAME, fnNew, boIsAllowUndo, boIsSilent) )
			{
				//成功
				*this = fnNew;
				return true;
			}
		}
		return false;
	}

	/**
	 * [操作] 削除.
	 *	@note ファイルでもディレクトリでも削除可能です。
	 *	@param	boIsAllowUndo	trueならUNDOに残るようにします。
	 *							false or省略するとUNDOに残しません。
	 *	@param	boIsSilent		falseならUIを出し、ユーザに確認させます。
	 *							true or省略なら UIを一切出しません。
	 *	@retval true 成功
	 *	@retval false 失敗（キャンセルも含む）
	 */
	bool Remove(bool boIsAllowUndo = false, bool boIsSilent = true)
	{
		if ( IsExist() && m_ShFileOp(FO_DELETE, NULL, boIsAllowUndo, boIsSilent) )
		{
			//成功
			IsExist();
			return true;
		}
		return false;
	}

	/**
	 * [操作] コピー.
	 *	@note ファイルでもディレクトリでもコピー可能です。
	 *	@param	lpszNew			コピー先名。
	 *	@param	boIsAllowUndo	trueならUNDOに残るようにします。
	 *							false or省略するとUNDOに残しません。
	 *	@param	boIsSilent		falseならUIを出し、ユーザに確認させます。
	 *							true or省略なら UIを一切出しません。
	 *	@retval true 成功
	 *	@retval false 失敗（キャンセルも含む）
	 */
	bool CopyTo(LPCTSTR lpszNew, bool boIsAllowUndo = false, bool boIsSilent = true)
	{
		if ( IsExist() && m_ShFileOp(FO_COPY, m_FullPath(lpszNew), boIsAllowUndo, boIsSilent) )
		{
			return true;
		}
		return false;
	}

	/**
	 * [操作] ムーブ.
	 *	@note ファイルでもディレクトリでもムーブ可能です。
	 *	@note	成功すると、本オブジェクトで記憶しているファイル名もムーブ後のものになります。
	 *	@param	lpszNew			ムーブ先名。
	 *	@param	boIsAllowUndo	trueならUNDOに残るようにします。
	 *							false or省略するとUNDOに残しません。
	 *	@param	boIsSilent		falseならUIを出し、ユーザに確認させます。
	 *							true or省略なら UIを一切出しません。
	 *	@retval true 成功
	 *	@retval false 失敗（キャンセルも含む）
	 */
	bool MoveTo(LPCTSTR lpszNew, bool boIsAllowUndo = false, bool boIsSilent = true)
	{
		if ( IsExist() )
		{
			CFileName fnNew = m_FullPath(lpszNew);
			if ( m_ShFileOp(FO_MOVE, fnNew, boIsAllowUndo, boIsSilent) )
			{
				//成功
				*this = fnNew;
				return true;
			}
		}
		return false;
	}

	/**
	 * [操作] コピー.
	 *	@note ディレクトリはコピーできません。
	 *	@param	lpszNew			コピー先名。
	 *	@param	boIsAllowUndo	trueならUNDOに残るようにします。
	 *							false or省略するとUNDOに残しません。
	 *	@retval empty 失敗
	 *	@retval 上記以外 成功。文字列は、実際にコピーしたファイル名。
	 */
	CStr CopyEx(LPCTSTR lpszNew, bool boIsAllowUndo = false)
	{
		if ( IsExist() && ! IsDirectory() )
		{
			CStr s = lpszNew;
			if ( IsExist(s) )
			{
				CStr p1 = s;
				CStr p2;
				INT_PTR f1 = s.ReverseFind('.');
				INT_PTR f2 = s.ReverseFindOneOf(_T("\\/"));
				if ( f1 >= 0 && f2 < f1 )
				{
					p1 = s.Left(f1);
					p2 = s.Mid(f1);
				}
				loop ( i, 100000 )
				{
					s.Format(_T("%s(%d)%s"), p1, i + 1, p2);
					if ( ! IsExist(s) )
					{
						if ( CopyTo(s, boIsAllowUndo, true) )
						{
							return s;
						}
						break;
					}
				}
			}
			else
			{
				if ( CopyTo(s, boIsAllowUndo, true) )
				{
					return s;
				}
			}
		}
		return _T("");
	}

	/**
	 * [作成] ショートカット作成.
	 *		本インスタンスが保持しているファイル名のショートカットを作成します。
	 *	@attention 本メソッドは COM を使用しています。先に CoInitialize() 等、初期化しておいてください。
	 *	@param lpszLinkName ショートカットファイル名。拡張子は lnk にするようにしてください。
	 *	@param lpszDescription 説明文。 CE では無視されます。
	 *	@param lpszParameter パラメータ。省略可能。 CE では無視されます。
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool MakeShortcut(LPCTSTR lpszLinkName, LPCTSTR lpszDescription, LPCTSTR lpszParameter = NULL) const
	{
#ifndef _WIN32_WCE
		// ShellLink 作成
		IShellLink* pSL = NULL;
		HRESULT hr = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
						IID_IShellLink, reinterpret_cast<LPVOID*>(&pSL));
		if ( SUCCEEDED(hr) )
		{ 
			pSL->SetPath(m_strName);
			pSL->SetWorkingDirectory(GetPathName());
			pSL->SetDescription(lpszDescription);
			if ( lpszParameter != NULL )
			{
				pSL->SetArguments(lpszParameter);
			}
			// ShellLink から PersistFile を得る
			IPersistFile* pPF = NULL; 
			hr = pSL->QueryInterface(IID_IPersistFile, reinterpret_cast<LPVOID*>(&pPF));
			if ( SUCCEEDED(hr) ) 
			{ 
				hr = pPF->Save(CUnicode(lpszLinkName), TRUE);
				pPF->Release();
				pPF = NULL;
			} 
			pSL->Release();
			pSL = NULL;
		}
		return SUCCEEDED(hr);
#else
		CStr ln = lpszLinkName;
		CStr fn = m_strName;
		return !! ::SHCreateShortcut(ln.GetBuffer(MAX_PATH), fn.GetBuffer(MAX_PATH));
#endif
	}
	
	/**
	 * [確認] Shortcutチェック
	 *	@attention 本メソッドは COM を使用しています。先に CoInitialize() 等、初期化しておいてください。
	 *	@retval true ショートカットファイル
	 *	@retval false 否
	 */
	bool IsShortcut(void) const
	{
#ifndef _WIN32_WCE
		IShellLink* pSL = NULL;
		HRESULT hr = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
								IID_IShellLink, reinterpret_cast<LPVOID*>(&pSL)); 
		if ( SUCCEEDED(hr) ) 
		{ 
			IPersistFile* pPF = NULL; 
			hr = pSL->QueryInterface(IID_IPersistFile, reinterpret_cast<LPVOID*>(&pPF));
			if ( SUCCEEDED(hr) ) 
			{ 
				hr = pPF->Load(CUnicode(m_strName), STGM_READ);
				pPF->Release();
				pPF = NULL;
			} 
			pSL->Release(); 
			pSL = NULL;
		} 
		return SUCCEEDED(hr);
#else
		CStr s;
		if ( ::SHGetShortcutTarget(m_strName, s.GetBuffer(MAX_PATH), MAX_PATH) )
		{
			s.ReleaseBuffer();
			return true;
		}
		return false;
#endif
	}

	/**
	 * [設定] ショートカット解決.
	 *		本インスタンスが保持しているファイル名がショートカットの場合、リンク先のファイル名に変換します。
	 *	@attention 本メソッドは COM を使用しています。先に CoInitialize() 等、初期化しておいてください。
	 *	@param[out] _description ショートカットの説明文が格納されます。 CE では取得されません。
	 *	@param[in] hWnd 有効なウィンドウハンドルを指定すると、ショートカットがないときに探す UI が出ます。 CE では機能しません。
	 *	@retval true 解決に成功し、本インスタンスで記憶しました。
	 *	@retval false 失敗。ショートカットファイルでないか、参照先のファイルが無い
	 */
	bool ResolveShortcut(CStr& _description, HWND hWnd = NULL)
	{
	#ifndef _WIN32_WCE
		IShellLink* pSL = NULL;
		HRESULT hr = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
								IID_IShellLink, reinterpret_cast<LPVOID*>(&pSL)); 
		if ( SUCCEEDED(hr) ) 
		{ 
			IPersistFile* pPF = NULL; 
			hr = pSL->QueryInterface(IID_IPersistFile, reinterpret_cast<LPVOID*>(&pPF));
			if ( SUCCEEDED(hr) ) 
			{ 
				hr = pPF->Load(CUnicode(m_strName), STGM_READ);
				if ( hWnd != NULL && SUCCEEDED(hr) ) 
				{ 
					hr = pSL->Resolve(hWnd, 0); 
				}
				WIN32_FIND_DATA wfd; 
				CStr path;
				if ( SUCCEEDED(hr) ) 
				{ 
					hr = pSL->GetPath(path.GetBuffer(MAX_PATH), MAX_PATH, &wfd, SLGP_SHORTPATH); 
					path.ReleaseBuffer();
				}
				if ( SUCCEEDED(hr) ) 
				{ 
					// Get the description of the target. 
					hr = pSL->GetDescription(_description.GetBuffer(MAX_PATH), MAX_PATH); 
					_description.ReleaseBuffer();
				}
				if ( SUCCEEDED(hr) ) 
				{
					operator=(path);
				}
				pPF->Release();
				pPF = NULL;
			} 
			pSL->Release(); 
			pSL = NULL;
		} 
		return SUCCEEDED(hr);
#else
		CStr s;
		if ( ::SHGetShortcutTarget(m_strName, s.GetBuffer(MAX_PATH), MAX_PATH) )
		{
			s.ReleaseBuffer();
			operator=(s);
			_description.Empty();
			return true;
		}
		return false;
#endif
	}

	/**
	 * [設定] ショートカット解決.
	 *		本インスタンスが保持しているファイル名がショートカットの場合、リンク先のファイル名に変換します。
	 *	@attention 本メソッドは COM を使用しています。先に CoInitialize() 等、初期化しておいてください。
	 *	@retval true 解決に成功し、本インスタンスで記憶しました。
	 *	@retval false 失敗。ショートカットファイルでないか、参照先のファイルが無い
	 */
	bool ResolveShortcut(void)
	{
		CStr s;
		return ResolveShortcut(s, NULL);
	}
	
	/**
	 * [確認] 有無チェック.
	 *	@param lpszPath 確認対処のパス.
	 *	@retval true 存在
	 *	@retval false ない
	 */
	static bool IsExist(LPCTSTR lpszPath)
	{
		return CFileName(lpszPath).IsExist();
	}

private:
	/// 指定の属性がtrueかチェック
	bool m_IsAttr(DWORD dwCheck) const
	{
		if ( m_boIsExists )
		{
			return (m_tInfo.dwFileAttributes & dwCheck) != 0;
		}
		return false;
	}
	/// 指定の属性を設定
	bool m_SetAttr(DWORD dwAttr)
	{
		bool r = !! ::SetFileAttributes(m_strName, dwAttr);
		m_boIsExists = ms_GetInfo(m_strName, &m_tInfo);
		return r;
	}
	/// ファイル情報取得
	static bool ms_GetInfo(LPCTSTR lpszName, LPWIN32_FILE_ATTRIBUTE_DATA P)
	{
		WIN32_FIND_DATA t;
		HANDLE h = ::FindFirstFile(lpszName, &t);
		if ( h != INVALID_HANDLE_VALUE )
		{
			::FindClose(h);
			P->dwFileAttributes	= t.dwFileAttributes;
			P->ftCreationTime	= t.ftCreationTime;
			P->ftLastAccessTime	= t.ftLastAccessTime;
			P->ftLastWriteTime	= t.ftLastWriteTime;
			P->nFileSizeHigh	= t.nFileSizeHigh;
			P->nFileSizeLow		= t.nFileSizeLow;
			return true;
		}
		return false;
	}
	/**
	 * フルパス化
	 *	@param lpszName ファイル名
	 *	@return フルパス
	 */
	CStr m_FullPath(LPCTSTR lpszName)
	{
		#ifndef _WIN32_WCE
			CStr str;
			if ( ::PathIsRelative(lpszName) )
			{
				str = CStr::Fmt(_T("%s\\%s"), GetPathName(), lpszName);
			}
			else
			{
				str = lpszName;
			}
			return str;
		#else
			return lpszName;
		#endif
	}
	/**
	 * SHFileOperationの実行
	 *	@param[in]	wFunc			FO_DELETE,FO_COPY など動作を指定します。
	 *	@param[in]	lpszTo			FO_DELETE以外は処理対象先を指定する必要が有ります。
	 *	@param[in]	boIsAllowUndo	trueならUNDOに残るようにします。
	 *	@param[in]	boIsSilent		falseならUIを出し、ユーザに確認させます
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool m_ShFileOp(WORD wFunc, LPCTSTR lpszTo, bool boIsAllowUndo, bool boIsSilent)
	{
		CDoubleNullTerminateStr strdntFrom(m_strName);
		return m_ShFileOp(wFunc, strdntFrom, lpszTo, boIsAllowUndo, boIsSilent);
	}
	/**
	 * SHFileOperationの実行
	 *	@param[in]	wFunc			FO_DELETE,FO_COPY など動作を指定します。
	 *	@param[in]	strdntFrom		処理対象元を指定します。
	 *	@param[in]	lpszTo			FO_DELETE以外は処理対象先を指定する必要が有ります。
	 *	@param[in]	boIsAllowUndo	trueならUNDOに残るようにします。
	 *	@param[in]	boIsSilent		falseならUIを出し、ユーザに確認させます
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool m_ShFileOp(WORD wFunc, const CDoubleNullTerminateStr& strdntFrom, LPCTSTR lpszTo, 
				bool boIsAllowUndo, bool boIsSilent)
	{
		SHFILEOPSTRUCT tFileOp;
		CDoubleNullTerminateStr strdntTo(lpszTo);
		tFileOp.hwnd	= NULL;							// ウィンドウハンドル
		tFileOp.wFunc	= wFunc;						// 実行する操作
		tFileOp.pFrom	= strdntFrom;					// 対象ファイル名
		tFileOp.pTo		= NULL;							// 目的ファイル名
		if ( lpszTo != NULL )
		{
			tFileOp.pTo = strdntTo;
		}
		tFileOp.fFlags	= 0;							// Flag
		if ( boIsSilent )
		{
			tFileOp.fFlags |= FOF_SILENT		// 経過ダイアログ無し
							| FOF_NOCONFIRMATION// 確認で「すべて」指定
							| FOF_NOERRORUI;	// エラー時、ダイアログを出さない
		}
		if ( boIsAllowUndo )
		{
			tFileOp.fFlags |= FOF_ALLOWUNDO;	//UNDO
		}
		tFileOp.fAnyOperationsAborted = FALSE;			// [out]結果
		tFileOp.hNameMappings = NULL;					// [out]ファイル名マッピングオブジェクト
		tFileOp.lpszProgressTitle = _T("");
		return ::SHFileOperation(&tFileOp) == 0;
	}
	CStr								m_strName;		///< 保持するファイル名
	mutable bool						m_boIsExists;	///< ファイルが存在するか？
	mutable	WIN32_FILE_ATTRIBUTE_DATA	m_tInfo;		///< ファイル情報
};



}; // TNB

