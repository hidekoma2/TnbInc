#pragma once
/**
 *@file
 * PrimoBurner関係のヘッダ
 *
 *		<a class="el" href="http://www.primoburner.com/"> PrimoBurner</a> を使用します。
 *
 *	@note	非UNICODE プロジェクトでも、クラス内部では UNICODE で動作させています。
 *			これは、V1.99 が日本語のファイル名/ディレクトリ名が正しく書き込めないためです。
 *
 *	@note	現在、Ver1.99 と 2.10 のいずれかが使用可能です。
 *	@note	現在、Ver2.10 以降は動作未確認です。
 *
 *	@todo	2.10を使用の場合でも、 Blu-ray Disc は未対応です。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbFileFinder.h"
#include "TnbFileName.h"
#include "TnbStrVector.h"
#include "TnbTickCount.h"



#ifndef _TnbDOXYGEN	//Document作成用シンボル

#if defined(___PRIMO_TCHAR_T) | defined(__PRIMO_CHAR_T)
 #error hpCDE.h will be included previously.
#endif
#define ___PRIMO_TCHAR_T
typedef wchar_t tchar_t;
#define ___PRIMO_CHAR_T
typedef wchar_t char_t;

// 以下のPathを通すこと
#ifdef __MSVCPP6__
	#define deprecated dllexport
	#include "hpCDE.h"
	#undef deprecated
#else
	#include "hpCDE.h"
#endif
#pragma comment(lib,"hpCDEU.lib")
#pragma comment(lib,"DataCDU.lib")

#ifdef ___HPSCSI_H___
	// PrimoBurner 2.10以降を使用
	#define _TnbPrimoBurner_Used_V2
#else
	// PrimoBurner 2.00以降を使用
	#pragma comment(lib,"HfsU.lib")
#endif

#endif // _TnbDOXYGEN



//TNB Library
namespace TNB
{



/// namespace primo::burner の別名
#define PRIBUR ::primo::burner
/// namespace primo::scsi の別名
#ifdef _TnbPrimoBurner_Used_V2
 #define PRISCSI ::primo::scsi
#else
 #define PRISCSI ::primo::burner
#endif



/**@ingroup PRIMOBURNER
 * PrimoBurner 管理クラス
 *
 *	@par必要ファイル
 *			TnbPrimoBurnerEngine.h
 *
 *	@date 07/01/29	新規作成
 *	@date 09/08/04	クラス名、構成変更。
 */
class CPrimoBurner : CCopyImpossible
{
public:

	#ifndef _TnbDOXYGEN	//Document作成用シンボル

	// PointerHandleBase用 汎用 ポインタリリーステンプレートクラス
	template<typename T> struct TPhReleasePointerT
	{
		void operator()(T P) { P->Release(); }
	};

	#endif // _TnbDOXYGEN


	/**@ingroup PRIMOBURNER
	 * PrimoBurner ディスクイメージ情報管理クラス
	 *
	 *		ディスクイメージを管理します。本クラスはファイルの実体は持たずに、
	 *		ファイル名、ディレクトリ名を保持しています。
	 *
	 *	@note SetDefaultParametersForCd() を使うと、Joliet CD イメージが容易に設定できます。
	 *	@note SetDefaultParametersForDvd() を使うと、 UDF イメージが容易に設定できます。
	 *	
	 *	@note 複製可能です。
	 *
	 *	@par必要ファイル
	 *			TnbPrimoBurnerEngine.h
	 *
	 *	@date 07/01/29	新規作成
	 *	@date 09/08/04	クラス名、構成変更。
	 */
	class CDiscImage
	{
	public:

		/**@ingroup PRIMOBURNER
		 * PrimoBurner ディスクイメージ情報リスナーインターフェース
		 *
		 *	@date 07/01/29	新規作成
		 */
		struct IListener
		{
			/// デストラクタ
			virtual ~IListener(void) {}

			/**
			 * [通知] ファイル処理
			 *	@note イメージ作成処理で使用中のファイル名を通知します。
			 *	@param nFile		通し番号。
			 *	@param lpszFileName	ファイル名。
			 *	@param nPercent		パーセント。
			 */
			virtual void OnFileStatus(int nFile, LPCWSTR lpszFileName, int nPercent) = 0;
		};

		/// コンストラクタ
		CDiscImage(void) : m_hpDataDisc(hpCDE::CreateDataDisc()), m_hpDataFile(hpCDE::CreateDataFile())
		{		 
			m_hpDataFile->SetDirectory(TRUE);		// Rootを設定しておく
			m_hpDataFile->SetFilePath(L"\\");			
			m_hpDataFile->SetLongFilename(L"\\");
		}

		/// デストラクタ
		~CDiscImage(void)
		{
		}

		/**
		 * [登録] リスナー登録
		 *	@param I リスナー
		 */
		void SetListener(IListener* I)
		{
			m_callbacker.SetListener(I);
		}

		/**
		 * [取得] IDataDisc ポインタ参照
		 *	@return IDataDisc ポインタ
		 */
		PRIBUR::IDataDisc* operator->(void)
		{
			return m_hpDataDisc;
		}

		/**
		 * [取得] エラー種取得
		 *	@note 得られるエラー種は、システムのものとは異なります。
		 *	@return <a class="el" href="http://www.primoburner.com/documentation/primo__burner__EDataDiscError.html"> EDataDiscError</a>
		 */
		DWORD GetLastError(void)
		{
			return m_hpDataDisc.IsNull() ? 0 : m_hpDataDisc->GetLastError();
		}

		/**
		 * [設定] CD用基本設定
		 *	@note Joliet で作成することを指定。
		 *	@param lpszVolume ボリューム名
		 *	@param eType イメージのタイプを指定。デフォルトは、 Joliet。
		 *	@param dwFlags イメージタイプ内の詳細フラグ。enum EImageConstraint の組み合わせを指定。
		 */
		void SetDefaultParametersForCd(LPCTSTR lpszVolume, EImageType eType = IMG_TYPE_JOLIET, DWORD dwFlags = IC_NONE)
		{
			m_hpDataDisc->SetVolumeLabel(CUnicode(lpszVolume));
			m_hpDataDisc->SetImageType(eType);
			m_hpDataDisc->SetImageConstraints(dwFlags);
			m_hpDataDisc->SetTranslateFilenames(false);
			SetBootableParameters(false);
			m_hpDataDisc->SetSessionStartAddress(0);
		}

		/**
		 * [設定] DVD用基本設定
		 *	@note UDF で作成することを指定。
		 *	@param lpszVolume ボリューム名
		 *	@param eType イメージのタイプを指定。デフォルトは、 UDF。
		*/
		void SetDefaultParametersForDvd(LPCTSTR lpszVolume, EImageType eType = IMG_TYPE_UDF)
		{
			SetDefaultParametersForCd(lpszVolume, eType);
		}

		/**
		 * [設定] Bootable設定
		 *	@param boIsBootableCd true なら他のパラメータも設定する必要があります。
		 *	@param eEmu エミュレーションモード設定。省略すると BMT_NO_EMULATION 。
		 *	@param lpszbootImageFile ブートイメージファイル。省略するとNULLになります。
		 */
		void SetBootableParameters(
						bool boIsBootableCd, EBootEmulation eEmu = BMT_NO_EMULATION, LPCTSTR lpszbootImageFile = NULL)
		{
			m_hpDataDisc->SetBootable(boIsBootableCd);
			m_hpDataDisc->SetBootEmulation(eEmu);
			m_hpDataDisc->SetBootImageFile(CUnicode(lpszbootImageFile));
		}

		/**
		 * [設定] ディスクイメージクリア
		 *	@note Add したファイルをクリアします。Addしなおすときに使用できます。
		 */
		void ResetFileImage(void)
		{
			m_SetLayout(false);
		}

		/**
		 * [登録] ファイル追加
		 *	@note そのファイルがコピー対象になります。
		 *	@param lpszName 登録対象。
		 *	@param lpszImagePath CDイメージ上のPath（フルパスで指定の必要あり）。省略するとルートになります。
		 *	@retval ture 追加成功。
		 *	@retval false 失敗。イメージディスクは空になります。
		 */
		bool AddSource(LPCTSTR lpszName, LPCTSTR lpszImagePath = NULL)
		{
			CAppendFile a(lpszName);
			return m_SetLayout(a.Execute(m_hpDataFile, m_hpDataDisc->GetImageType(), lpszImagePath));
		}

		/**
		 * [登録] ファイル群追加
		 *	@note ディレクトリ以下がコピー対象になります。	
		 *	@param lpszName 登録対象。
		 *	@param lpszImagePath CDイメージ上のPath（フルパスで指定の必要あり）。省略するとルートになります。
		 *	@retval ture 追加成功。
		 *	@retval false 失敗。イメージディスクは空になります。
		 */
		bool AddSourceFromPath(LPCTSTR lpszName, LPCTSTR lpszImagePath = NULL)
		{
			CAppendPath a(lpszName);
			return m_SetLayout(a.Execute(m_hpDataFile, m_hpDataDisc->GetImageType(), lpszImagePath));
		}

		/**
		 * [作成] イメージファイル作成.
		 *	@note 登録された内容でイメージファイルを作成します。
		 *	@param lpszFile 出力ファイル名。通常拡張子は iso を指定します。
		 *	@retval ture 成功。
		 *	@retval false 失敗。
		 */
		bool CreateImageFile(LPCTSTR lpszFile)
		{
			m_hpDataDisc->SetCallback14(&m_callbacker);
			CUnicode s = CUnicode(lpszFile);
			s.Replace(L"\\\\", L"\\");
			bool r = !! m_hpDataDisc->CreateImageFile(s);
			if ( ! r )
			{
				TRACE1("err - %d\n", m_hpDataDisc->GetLastError() - SCSI_SENSE_ERROR_BASE );
				TRACE1("err - %d\n", GetLastError() );
			}
			m_SetLayout(true);
			return r;
		}

	private:

		#ifndef _TnbDOXYGEN	//Document作成用シンボル

		/**
		 * 内部処理用 PrimoBurner コールバッククラス
		 */
		class CCallBacker : public PRIBUR::IDataDiscCallback14
		{
			IListener*	m_iListener;
		public:
			/// コンストラクタ
			CCallBacker(void) : m_iListener(NULL) {}
			/// デストラクタ
			virtual ~CCallBacker(void) {}
			/// リスナーセット
			void SetListener(IListener*	I)
			{
				m_iListener = I;
			}
			/// The OnFileStatus method is called by a IDataDisc instance to report the file that is currently being recorded.
			virtual void OnFileStatus(int nFile, const tchar_t* pFileName, int nPercent) 
			{
				nPercent = (nPercent > 100) ? 100 : nPercent;
				TRACE3("No.%d %d%% [%s] \n", nFile, nPercent, CStr(pFileName));
				if ( m_iListener != NULL )
				{
					m_iListener->OnFileStatus(nFile, pFileName, nPercent);
				}
			}
			/// dummy
			virtual bool_t OnContinueBurn(void) { return true; }
			/// dummy
			virtual void OnProgress(uint64_t ddwPos, uint64_t ddwAll) {}
			/// dummy
			virtual void OnStatus(EDataDiscStatus eStatus) {}
		};

		/// PRIBUR::IDataDisc* 用ハンドル
		typedef CPointerHandleBaseT<PRIBUR::IDataDisc*, TPhReleasePointerT<PRIBUR::IDataDisc*> > CDataDiscHandle;
		/// PRIBUR::IDataFile* 用ハンドル
		typedef CPointerHandleBaseT<PRIBUR::IDataFile*, TPhReleasePointerT<PRIBUR::IDataFile*> > CDataFileHandle;
		CDataDiscHandle			m_hpDataDisc;	///< PRIBUR::IDataDisc インスタンス管理
		CDataFileHandle			m_hpDataFile;	///< PRIBUR::IDataFile インスタンス管理
		CCallBacker				m_callbacker;	///< リスナ管理

		/// 登録用ファイル情報
		struct TFileInfo
		{
			CStr		strFileName;				///< ファイル名
			CStr		strAlternateFileName;		///< Shortファイル名
			bool		boHasHidden;				///< Hiddenフラグの有無
			CFileTimeEx	timeLastWrite;				///< 作成時間
			/// コンストラクタ
			TFileInfo(void) {}
			TFileInfo(const WIN32_FIND_DATA& fd) {
				Set(fd);
			}
			/// 設定
			void Set(const WIN32_FIND_DATA& fd) {
				strFileName				= fd.cFileName;
				strAlternateFileName	= fd.cAlternateFileName;
				boHasHidden				= (fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0;
				timeLastWrite			= fd.ftLastWriteTime;
			}
			/// ファイル名取得
			CStr GetFileName(void) const {
				return strFileName;
			}
			/// ファイル名取得
			CStr GetAlternateFileName(void) const {
				return strAlternateFileName.Left(12);
			}
			/// 時間取得
			FILETIME GetFileTime(void) const {
				return timeLastWrite;
			}
		};

		/// 追加用ベースクラス
		class CAbstractAppender
		{
			/// 掘り下げ（再起呼び出し）
			bool m_Dig(const CDataFileHandle& hpFile, DWORD dwImageType, const CStr& strImagePath)
			{
				if ( strImagePath.IsEmpty() )
				{
					return OnFound(hpFile, dwImageType);
				}
				CStr s1 = strImagePath;
				CStr s2;
				int f = strImagePath.Find(_T('\\'));
				if ( f < 0 ) { f = strImagePath.Find(_T('/')); }
				if ( f >= 0 )
				{
					s1 = strImagePath.Left(f);
					s2 = strImagePath.Mid(f + 1);
					if ( s1.IsEmpty() )
					{
						return m_Dig(hpFile, dwImageType, s2);
					}
				}
				CDataFileHandle F = hpFile->Find(CUnicode(_T("/") + s1));
				if ( F.IsNull() )
				{
					F = hpCDE::CreateDataFile(); 
					TFileInfo fd;
					fd.boHasHidden			= false;
					fd.strAlternateFileName	= s1;
					fd.strFileName			= s1;
					fd.timeLastWrite		= CFileTimeEx::GetCurrent();
					SetDataFile(F, true, s1, dwImageType, fd);
					if ( ! m_Dig(F, dwImageType, s2) )
					{
						return false;
					}
					hpFile->AddItem(F);
				}
				else
				{
					if ( ! m_Dig(F, dwImageType, s2) )
					{
						return false;
					}
				}
				return true;
			}
		public:
			/// 起動
			virtual bool OnSetup(void) = 0;
			/// 発見
			virtual bool OnFound(const CDataFileHandle& hpFile, DWORD dwImageType) = 0;
			/// 実行
			bool Execute(const CDataFileHandle& hpFile, DWORD dwImageType, LPCTSTR lpszImagePath)
			{
				if ( ! OnSetup() ) { return false; }
				CStr str = lpszImagePath;
				return m_Dig(hpFile, dwImageType, str);
			}
			/**
			 * IDataFileへ、登録
			 *	@param[out] _pFile 追加先
			 *	@param[in] boDir true なら ディレクトリ指定。false ならファイル指定。
			 *	@param[in] lpszFilePath ファイル指定の場合、実際存在するファイルの場所を指定します(通常フルパス指定)。
			 *	@param[in] dwImageType イメージタイプ。
			 *	@param[in] fd ファイル情報。
			 */
			static void SetDataFile(
					PRIBUR::IDataFile* _pFile, bool boDir, LPCTSTR lpszFilePath, DWORD dwImageType, const TFileInfo& fd)
			{
				_pFile->SetDirectory(boDir);
				_pFile->SetFilePath(CUnicode(lpszFilePath));			
				_pFile->SetLongFilename(CUnicode(fd.GetFileName()));
				_pFile->SetShortFilename(CUnicode(fd.GetAlternateFileName()));
				_pFile->SetHiddenAttribute(static_cast<EImageType>(dwImageType), fd.boHasHidden);
				#ifdef _TnbPrimoBurner_Used_V2
					FILETIME ft = fd.GetFileTime();
					_pFile->SetCreationTime(&ft);
				#else
					_pFile->SetFileTime(&fd.GetFileTime());
				#endif
			}
		};

		/// ファイル追加クラス
		class CAppendFile : public CAbstractAppender
		{
			DEFSUPER(CAbstractAppender);
			CStr		m_strFullPath;
			TFileInfo	m_fileInfo;
		public:
			/// コンストラクタ
			CAppendFile(LPCTSTR lpszFullPath) : m_strFullPath(lpszFullPath) {}
			/// 起動
			virtual bool OnSetup(void)
			{
				CFileName fn(m_strFullPath);
				if ( ! fn.IsExist() || fn.IsDirectory() )
				{
					return false;
				}
				m_fileInfo.boHasHidden			= fn.IsHidden();
				m_fileInfo.timeLastWrite		= fn.GetLastWriteTime();
				m_fileInfo.strAlternateFileName = fn.GetShortName();
				m_fileInfo.strFileName			= fn.GetFileName();
				m_strFullPath = fn;
				return true;
			}
			/// 発見
			virtual bool OnFound(const CDataFileHandle& hpFile, DWORD dwImageType)
			{
				CDataFileHandle F = hpCDE::CreateDataFile(); 
				SetDataFile(F, false, m_strFullPath, dwImageType, m_fileInfo);
				hpFile->AddItem(F);
				return true;
			}
		};

		/// パス追加クラス
		class CAppendPath : public CAbstractAppender
		{
			DEFSUPER(CAbstractAppender);
			CStr		m_strPath;
			/// 追加
			bool m_AddTree(PRIBUR::IDataFile* pFile, DWORD dwImageType, LPCTSTR lpszPath)
			{
				CFileFinder ff;
				if ( ff.Start(lpszPath, _T("\\*")) )
				{
					do
					{
						CStr strFileName = ff->cFileName; 
						CStr strFullPath;
						strFullPath.Format(_T("%s\\%s"), lpszPath, strFileName);
						if ( (ff->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
						{
							// Dir
							CDataFileHandle F = pFile->Find(CUnicode(_T("/") + strFileName));
							if ( F.IsNull() )
							{
								F = hpCDE::CreateDataFile(); 
								_super::SetDataFile(F, true, strFileName, dwImageType, TFileInfo(ff.Get()));
								if ( ! m_AddTree(F, dwImageType, strFullPath) )
								{
									return false;
								}
								pFile->AddItem(F);
							}
							else
							{
								if ( ! m_AddTree(F, dwImageType, strFullPath) )
								{
									return false;
								}
							}
						} 
						else
						{
							// File
							CDataFileHandle F = hpCDE::CreateDataFile(); 
							_super::SetDataFile(F, false, strFullPath, dwImageType, TFileInfo(ff.Get()));
							pFile->AddItem(F);
						}
					} while ( ff.Next() );
				}
				return true;
			}
		public:
			/// コンストラクタ
			CAppendPath(LPCTSTR lpszPath) : m_strPath(lpszPath) {}
			/// 起動
			virtual bool OnSetup(void)
			{
				CFileFinder ff;
				return ff.Start(m_strPath, _T("\\*"));
			}
			/// 発見
			virtual bool OnFound(const CDataFileHandle& hpFile, DWORD dwImageType)
			{
				return m_AddTree(hpFile, dwImageType, m_strPath);
			}
		};

		#endif //_TnbDOXYGEN

		/**
		 * m_hpDataFile を m_hpDataDisc にセット
		 *	@param boIsSet true ならセット。　 false ならクリア状態へ
		 */
		bool m_SetLayout(bool boIsSet)
		{
			try
			{
				if ( boIsSet && m_hpDataDisc->SetImageLayout11(m_hpDataFile) )
				{
					return true;
				}
				m_hpDataFile = hpCDE::CreateDataFile(); 
				m_hpDataFile->SetDirectory(TRUE);		//ルートを設定しておく
				m_hpDataFile->SetFilePath(L"\\");			
				m_hpDataFile->SetLongFilename(L"\\");
				m_hpDataDisc->SetImageLayout11(m_hpDataFile);
			}
			catch(PRIBUR::EDataDiscError& error)
			{
				IgnoreUnusedValue(error);
			}
			return false;
		}
	};



	/**@ingroup PRIMOBURNER
	 * PrimoBurner SCSI管理クラス
	 *
	 * @note CPrimoBurner::CDevice::CreateScsi() により作成されます。
	 *
	 *	@par必要ファイル
	 *			TnbPrimoBurnerEngine.h
	 *
	 *	@date 07/02/02	新規作成
	 *	@date 07/02/05	Vectorを使用したメソッドを追加。ModeSenseを追加。
	 *	@date 09/08/04	クラス名、構成変更。
	 */
	class CScsi
	{
	public:

		/**
		 * コンストラクタ
		 *	@param P IScsiInterface インスタンスのポインタ。省略すると NULL。
		 */
		CScsi(PRISCSI::IScsiInterface* P = NULL) : m_hpScsi(P)
		{
		}

		/// デストラクタ
		~CScsi(void)
		{
		}

		/**
		 * [確認] 有効確認.
		 *		本インスタンスが 有効（IScsiInterface を持っている）か、確認します。
		 *	@retval true 有効。各メソッドは使えます。
		 *	@retval false 無効。
		 */
		bool IsValid(void) const
		{
			return ! m_hpScsi.IsNull();
		}

		/**
		 * [送信] CDB送信.
		 *	@param cdb CDB。
		 *	@param dwTimeout タイムアウト時間（ms）。省略すると３分が指定されます。
		 *	@retval true 成功
		 *	@retval false 失敗。 GetLastError() でエラー内容を確認できます。
		 */
		bool SendCommand(const ISequenceCollectionT<BYTE>& cdb, DWORD dwTimeout = SCSI_CMD_TIMEOUT_MS_LONG)
		{
			if ( m_hpScsi.IsNull() ) { return false; }
			return !! m_hpScsi->SendCommand(cdb.ReferBuffer(), cdb.GetSize(), SCSI_CMD_DIR_UNSPECIFIED, NULL, 0, dwTimeout);
		}

		/**
		 * [送信] CDB送信(読込みフェーズ付).
		 *	@param cdb CDB。
		 *	@param iReadSize 読み込みデータ長(CDBに正しく読み込みデータサイズを指定しておくこと)。
		 *	@param dwTimeout タイムアウト時間（ms）。省略すると３分が指定されます。
		 *	@retval Valid状態 成功。読み込んだデータを持っています。
		 *	@retval Invalid状態 false 失敗。 GetLastError() でエラー内容を確認できます。
		 */
		CByteVector SendCommandWithReadPhase(
						const ISequenceCollectionT<BYTE>& cdb, int iReadSize, DWORD dwTimeout = SCSI_CMD_TIMEOUT_MS_LONG)
		{
			CByteVector vb;
			if ( ! m_hpScsi.IsNull() ) 
			{
				bool r = !! m_hpScsi->SendCommand(
							cdb.ReferBuffer(), cdb.GetSize(), SCSI_CMD_DIR_READ, vb.GetBuffer(iReadSize), iReadSize, dwTimeout);
				vb.ReleaseBuffer();
				if ( r )
				{
					return vb;
				}
			}	
			vb.Invalid();
			return vb; 
		}

		/**
		 * [送信] CDB送信(書込みフェーズ付).
		 *	@param cdb CDB。
		 *	@param d 書込みデータ(CDBに正しくデータサイズ分を指定しておくこと)。
		 *	@param dwTimeout タイムアウト時間（ms）。省略すると３分が指定されます。
		 *	@retval true 成功
		 *	@retval false 失敗。 GetLastError() でエラー内容を確認できます。
		 */
		bool SendCommandWithWritePhase(
				const ISequenceCollectionT<BYTE>& cdb, const ISequenceCollectionT<BYTE>& d, DWORD dwTimeout = SCSI_CMD_TIMEOUT_MS_LONG)
		{
			if ( m_hpScsi.IsNull() ) { return false; }
			return !! m_hpScsi->SendCommand(
						cdb.ReferBuffer(), cdb.GetSize(), SCSI_CMD_DIR_WRITE, const_cast<BYTE*>(d.ReferBuffer()), d.GetSize(), dwTimeout);
		}

		/**
		 * [送信] MODESENSE CDB送信.
		 *	@param pc ページコード、および、ページコントロール値。
		 *	@param boIsDbd true なら DBDも読み込みます。 省略か false なら 読み込みません。
		 *	@param dwTimeout タイムアウト時間（ms）。省略すると３分が指定されます。
		 *	@retval Valid状態 成功。読み込んだデータを持っています。
		 *	@retval Invalid状態 false 失敗。 GetLastError() でエラー内容を確認できます。
		 */
		CByteVector Send_ModeSense(BYTE pc, bool boIsDbd = false, DWORD dwTimeout = SCSI_CMD_TIMEOUT_MS_LONG)
		{
			CByteVector cdb;
			cdb.SetSize(6);
			cdb[0] = 0x1A;	//ModeSense
			cdb[1] = _BIT(3); //Block Descriptor は転送なし。
			cdb[2] = pc;
			cdb[4] = 4;
			CByteVector r = SendCommandWithReadPhase(cdb, cdb[4], dwTimeout);
			if ( r.GetSize() != 4 )
			{
				return r;
			}
			cdb[1] = static_cast<BYTE>((boIsDbd && r[3] == 8) ? 0 : _BIT(3));
			cdb[4] = static_cast<BYTE>(4 + r[3] + r[0]);
			return SendCommandWithReadPhase(cdb, cdb[4], dwTimeout);
		}

		/**
		 * [送信] INQUIRY CDB送信.
		 *	@param dwTimeout タイムアウト時間（ms）。省略すると３分が指定されます。
		 *	@retval Valid状態 成功。読み込んだデータを持っています。
		 *	@retval Invalid状態 false 失敗。 GetLastError() でエラー内容を確認できます。
		 */
		CByteVector Send_Inquiry(DWORD dwTimeout = SCSI_CMD_TIMEOUT_MS_LONG)
		{
			CByteVector cdb;
			cdb.SetSize(6);
			cdb[0] = 0x12;	//Inquiry
			cdb[4] = 8;
			CByteVector r = SendCommandWithReadPhase(cdb, cdb[4], dwTimeout);
			if ( r.GetSize() != 8 )
			{
				return r;
			}
			cdb[4] = static_cast<BYTE>(5 + r[4]);
			return SendCommandWithReadPhase(cdb, cdb[4], dwTimeout);
		}

		/**
		 * [取得] エラー種取得
		 *	@note 得られるエラー種は、システムのものとは異なります。
		 *	@note Send*() メソッド実行結果が 失敗だった時、本メソッドを実行することで詳細がわかります。
		 *	@retval SCSIINTERFACE_SUCCESS			成功。
		 *	@retval SCSIINTERFACE_SYSTEM_ERROR		失敗の理由は、システムAPIでエラーが出たためです。 
		 *											::GetLastError() でシステムエラーコードを取得してください。
		 *	@retval SCSIINTERFACE_DEVICE_ERROR		デバイスのエラーです。 デバイスの CPrimoBurner::CDevice::GetLastError() を
		 *											使い、エラーコードを取得してください。
		 *	@retval SCSIINTERFACE_DEVICE_NOT_SET	デバイスは設定されていません。
		 *	@retval SCSIINTERFACE_INVALID_PARAMETERS	パラメータに異常があります。
		 *	@retval SCSIINTERFACE_CHECK_SENSE		デバイスがセンス状態です。 GetSense() でセンスデータを取得してください。
		 */
		DWORD GetLastError(void)
		{
			return m_hpScsi.IsNull() ? 0 : m_hpScsi->GetLastError();
		}

		/**
		 * [取得] システムエラーコード取得
		 *	@note このメソッドは、 GetLastError()で SCSIINTERFACE_SYSTEM_ERROR が通知された時、使用します。
		 *	@return システムエラーコード。
		 */
		DWORD GetLastSystemError(void)
		{
			return m_hpScsi.IsNull() ? 0 : m_hpScsi->GetLastSystemError();
		}

		/**
		 * [取得] センスデータ取得
		 *	@note このメソッドは、 GetLastError()で SCSIINTERFACE_CHECK_SENSE が通知された時、使用します。
		 *	@param[out] _sense 取得センスデータ、格納。
		 *	@retval true 成功。
		 *	@retval false 失敗。
		 */
		bool GetSense(PRISCSI::SCSI_COMMAND_SENSE& _sense)
		{
			if ( m_hpScsi.IsNull() ) { return false; }
			m_hpScsi->GetSense(&_sense);
			return true;
		}

	private:
		/// PRIBUR::IScsiInterface* 用ハンドル
		typedef CPointerHandleBaseT<PRISCSI::IScsiInterface*, TPhReleasePointerT<PRISCSI::IScsiInterface*> > CScsiHandle;
		CScsiHandle		m_hpScsi;		///< PRIBUR::IScsiInterface インスタンス管理
	};


	
	/**@ingroup PRIMOBURNER
	 * PrimoBurner デバイス情報管理クラス
	 *
	 *	@note CPrimoBurner::CreateDevice() により作成されます。
	 *	@note IsMediaReady() でメディアの挿入状態がわかります。
	 *	@note GetMediaProfile() で挿入されているメディア種がわかります。
	 *
	 *	@par必要ファイル
	 *			TnbPrimoBurnerEngine.h
	 *
	 *	@date 07/01/29	新規作成
	 *	@date 09/08/04	クラス名、構成変更。
	 */
	class CDevice
	{
	public:

		/**@ingroup PRIMOBURNER
		 * PrimoBurner デバイス情報リスナーインターフェース
		 *
		 *	@date 07/01/29	新規作成
		 */
		struct IListener
		{
			/// 進捗種類
			enum EType
			{
				BURN,			///< 焼き込み中
				FORMAT,			///< Format,Erase中
				WRITELEADIN,	///< LeadIn書込み中
				WRITELEADOUT,	///< LeadOut書込み中
			};

			/// デストラクタ
			virtual ~IListener(void) {}

			/**
			 * [通知] 進捗通知
			 *	@note 焼き込みやフォーマット、イレース中の進捗を通知します。
			 *	@param eType 種類。
			 *	@param ullPos 現在値。
			 *	@param ullAll 全体値。
			*/
			virtual void OnProgress(EType eType, ULONGLONG ullPos, ULONGLONG ullAll) = 0;

			/**
			 * [通知] 処理状態通知
			 *	@note 焼き込み中の処理状態を通知します。状態が変化すると通知されます。
			 *	@param eStatus 現在の処理状態。以下の値があります。\n
			 *				DATACD_STATUS_BUILDING_IMAGE_HEADER		Building image header.	\n
			 *				DATACD_STATUS_WRITING_IMAGE_HEADER		Writing image header.	\n
			 *				DATACD_STATUS_WRITING_IMAGE				Writing image.	\n
			 *				DATACD_STATUS_CACHING_SMALL_FILES		Caching small files.	\n
			 *				DATACD_STATUS_CACHING_NETWORK_FILES		Caching network files.	\n
			 *				DATACD_STATUS_CACHING_CDROM_FILES		Caching CDROM files.	\n
			 *				DATACD_STATUS_INITIALIZING				Initializing.	\n
			 *				DATACD_STATUS_WRITING					Writing.	\n
			 *				DATACD_STATUS_WRITING_LEAD_OUT			Writing lead-out and flushing cache.
			 */
			virtual void OnStatus(PRIBUR::EDataDiscStatus eStatus) = 0;
		
			/**
			 * [通知] ファイル処理
			 *	@note イメージ作成処理で使用中のファイル名を通知します。
			 *	@param nFile		通し番号。
			 *	@param lpszFileName	ファイル名。
			 *	@param nPercent		パーセント。
			 */
			virtual void OnFileStatus(int nFile, LPCWSTR lpszFileName, int nPercent) = 0;

			/**
			 * [確認] 継続確認
			 *	@note 処理中、適当なタイミングでこのメソッドがコールされます。キャンセルしたい場合、 false を返します。
			 *	@retval true 継続。
			 *	@retval false 中止依頼。
			*/
			virtual bool OnContinueBurn() = 0;
		};

		/**
		 * コンストラクタ
		 *	@param P IDeviceインスタンスのポインタ。省略すると NULL。
		 */
		CDevice(PRIBUR::IDevice* P = NULL) 
				: m_hpDevice(P), m_dwWriteSpeed(0), m_boIsEject(true)
		{
		}

		/// デストラクタ
		~CDevice(void)
		{
		}

		/**
		 * [登録] リスナー登録
		 *	@param I リスナー
		 */
		void SetListener(IListener* I)
		{
			m_callbacker.SetListener(I);
		}

		/**
		 * [登録] 処理後Eject設定.
		 *	@note 本メソッドで設定しないと、処理後イジェクトします。
		 *	@param boIsEject true なら 書き込み、消去後、メディアをEjectします。
		 */
		void SetEjectMode(bool boIsEject)
		{
			m_boIsEject = boIsEject;
		}

		/**
		 * [処理] クローズ
		 *	@note	本インスタンスで管理している IDevieを閉じます（複製している場合、
		 *			すべての複製がクローズ/デストラクトするまで生きます）。
		 */
		void Close(void)
		{
			m_hpDevice.Null();
		}

		/**
		 * [確認] 有効確認.
		 *		本インスタンスが 有効（IDeviceを持っている）か、確認します。
		 *	@retval true 有効。各メソッドは使えます。
		 *	@retval false 無効。
		 */
		bool IsValid(void) const
		{
			return ! m_hpDevice.IsNull();
		}

		/**
		 * [取得] エラーコード取得.
		 *	@return エラー情報。数値はシステムエラーコードです( SystemErrorToMessageText() で文字列化できます)。
		 */
		DWORD GetLastError(void)
		{
			return m_hpDevice.IsNull() ? 0 : m_hpDevice->GetLastError();
		}

		/**
		 * [取得] IDevice参照.
		 *		IDeviceのポインタを得ることが出来ます。直接操作できます。
		 *	@return IDevice のポインタ。
		 */
		PRIBUR::IDevice* operator->(void)
		{
			return m_hpDevice;
		}

		/**
		 * [取得] SCSIインターフェース取得.
		 *		SCSI操作用のクラスを得ることが出来ます。
		 *	@return CScsi。
		 */
		CScsi CreateScsi(void)
		{
			if ( m_hpDevice.IsNull() )
			{
				return CScsi();
			}
			return CScsi(m_hpDevice->GetScsiInterface());
		}

		/**
		 * [確認] メディア確認
		 *	@param dwTime タイムアウト時間(単位ms)。省略すると０msです。
		 *	@retval ture 準備OK
		 *	@retval false NG
		 */
		bool IsMediaReady(DWORD dwTime = 0)
		{
			if ( m_hpDevice.IsNull() )
			{
				return false;
			}
			if ( dwTime == 0 )
			{
				return m_hpDevice->GetUnitReady() == DEVICE_ERROR_SUCCESS;
			}
			CTickCount tick;
			while ( ! tick.IsPassed(dwTime) )
			{
				if ( m_hpDevice->GetUnitReady() == DEVICE_ERROR_SUCCESS )
				{
					return true;
				}
				::Sleep(100);
			}
			return false;
		}
		
		/**
		 * [取得] メディア空き容量
		 *	@note BLOCK 単位の場合、 IDevice.GetMediaFreeSpace() を使います。
		 *	@note 本メソッドは、上記関数の結果に CDROM_BLOCKSIZE(2048)をかけているだけです。
		 *	@note IsMediaReady() で true になってから使用します。
		 *	@return 容量（Byte）。
		 */
		DWORD GetMediaFreeByte(void)
		{
			if ( m_hpDevice.IsNull() )
			{
				return 0;
			}
			return m_hpDevice->GetMediaFreeSpace() * CDROM_BLOCKSIZE;
		}

		/**
		 * [確認] 書込み可能確認
		 *	@note 書込み可能のメディアを持っているか確認します。
		 *	@note IsMediaReady() で true になってから使用します。
		 *	@retval true 可能。 Burn 系 メソッドが使用可能です。
		 *	@retval false 無効。
		 */
		bool HasWritableMedia(void)
		{
			if ( m_hpDevice.IsNull() )
			{
				return false;
			}
			TRACE3("HasWritableMedia()  %d,%d,%d\n", 
					m_hpDevice->GetMediaFreeSpace(), m_hpDevice->GetReWritePossible(), m_hpDevice->GetMediaIsReWritable());
			if ( m_hpDevice->GetMediaFreeSpace() == 0 )
			{
				return false;
			}
			return m_hpDevice->GetReWritePossible() && m_hpDevice->GetMediaIsReWritable();
		}

		/**
		 * [確認] 再書込み可能確認
		 *	@note 再書込み可能のメディア(CD-RW/DVD±RW)を持っているか確認します。
		 *	@note IsMediaReady() で true になってから使用します。
		 *	@retval true 可能。フォーマットやイレースが可能です。
		 *	@retval false 無効。
		 */
		bool HasRewritableMedia(void)
		{
			if ( m_hpDevice.IsNull() )
			{
				return false;
			}
			TRACE2("HasRewritableMedia()  %d,%d\n", m_hpDevice->GetReWritePossible(), m_hpDevice->GetMediaIsReWritable());
			return m_hpDevice->GetReWritePossible() && m_hpDevice->GetMediaIsReWritable();
		}

		/**
		 * [確認] メディア種確認
		 *	@note 現在挿入されているメディア種を返します。 
		 *	@note IsMediaReady() で true になってから使用します。
		 *	@note MP_BD～ のシンボルは、 PrimoBurner 2.0以降を使用している場合のみ使用できます。
		 *	@retval MP_UNKNOWN エラー。あるいはメディアなし。あるいは知らないメディア。
		 *	@retval MP_CD_ROM				CD-ROM -- Read only CD。
		 *	@retval MP_CD_R					CD-R -- Write once CD。
		 *	@retval MP_CD_RW				CD-RW -- Re-writable CD。
		 *	@retval MP_DVD_ROM				DVD-ROM -- Read only DVD。 
		 *	@retval MP_DVD_MINUS_R_SEQ		DVD-R Sequential Recording -- Write once DVD that can be recorded only sequentially.
		 *	@retval MP_DVD_RAM_RW			DVD-RAM -- Random Access Re-writable DVD
		 *	@retval MP_DVD_MINUS_RW_RO		DVD-RW Restricted Overwrite -- Re-writable DVD formatted for restricted overwrite recording.
		 *	@retval MP_DVD_MINUS_RW_SEQ		DVD-RW Sequential Recording -- Re-writable DVD that can be recorded only sequentially.
		 *	@retval MP_DVD_MINUS_R_DL_SEQ	DVD-R DL -- Write once DVD-R Double Layer 8.5GB that can be recorded sequentially.
		 *	@retval MP_DVD_MINUS_R_DL_JUMP	DVD-R DL -- Write once DVD-R Double Layer 8.5GB that can be recorded using layer jumping.
		 *	@retval MP_DVD_PLUS_RW			DVD+RW -- Re-Writable DVD
		 *	@retval MP_DVD_PLUS_R			DVD+R -- Write Once DVD+R Single Layer
		 *	@retval MP_DVD_PLUS_R_DL		DVD+R DL -- Write Once DVD+R Double Layer 8.5GB 
		 *	@retval MP_BD_R_SRM				BD-R in Sequential Recording Mode (SRM) -- Write once BD that can be recorded sequentially.
		 *	@retval	MP_BD_R_SRM_POW			BD-R Sequential Recording Mode (SRM) formatted for Pseudo Overwrite (POW) -- Write once BD that can be recorded only sequentially.
		 *									This profile is returned by the engine when the media is POW formatted BD-R SRM.
		 *	@retval MP_BD_R_RRM				BD-R in Random Recording Mode (RRM) -- Write once BD which is formatted for random recording.
		 *	@retval MP_BD_RE				BD-RE -- Re-writable Blue-ray Disc. 
		 */
		PRIBUR::EMediaProfile GetMediaProfile(void)
		{
			if ( m_hpDevice.IsNull() )
			{
				return MP_UNKNOWN;
			}
			return m_hpDevice->GetMediaProfile();
		}

		/**
		 * [取得] メディア種名取得
		 *	@param e メディア種コード。
		 *	@return メディア種名。
		 */
		static CStr ToString(PRIBUR::EMediaProfile e)
		{
			CStr s;
			switch ( e )
			{
			default:
			case MP_UNKNOWN:	break;
			case MP_CD_ROM:				s = _T("CD-ROM");	break;
			case MP_CD_R:				s = _T("CD-R");		break;
			case MP_CD_RW:				s = _T("CD-RW");	break;
			case MP_DVD_ROM:			s = _T("DVD-ROM");	break;
			case MP_DVD_MINUS_R_SEQ:	s = _T("DVD-R");	break;
			case MP_DVD_RAM_RW:			s = _T("DVD-RAM");	break;
			case MP_DVD_MINUS_RW_RO: //DVD-RW Restricted Overwrite -- Re-writable DVD formatted for restricted overwrite recording.
			case MP_DVD_MINUS_RW_SEQ: //DVD-RW Sequential Recording -- Re-writable DVD that can be recorded only sequentially.
										s = _T("DVD-RW");	break;
			case MP_DVD_MINUS_R_DL_SEQ: //DVD-R DL -- Write once DVD-R Double Layer 8.5GB that can be recorded sequentially.
			case MP_DVD_MINUS_R_DL_JUMP: //DVD-R DL -- Write once DVD-R Double Layer 8.5GB that can be recorded using layer jumping.
										s = _T("DVD-R DL"); break;
			case MP_DVD_PLUS_RW:		s = _T("DVD+RW"); break;
			case MP_DVD_PLUS_R:			s = _T("DVD+R"); break;
			case MP_DVD_PLUS_R_DL:		s = _T("DVD+R DL"); break;
			#ifdef _TnbPrimoBurner_Used_V2
			case MP_BD_R_SRM:
			case MP_BD_R_SRM_POW:
			case MP_BD_R_RRM:			s = _T("BD-R"); break;
			case MP_BD_RE:				s = _T("BD-RE"); break;
			#endif
			}
			return s;
		}

		/**
		 * [取得] 書込み速度一覧.
		 *		そのデバイスで可能な BYTE単位の速度の一覧を取得できます。
		 *	@return 速度配列。
		 */
		CDwordVector GetSpeeds(void)
		{
			CDwordVector vdwSpeeds;
			if ( ! m_hpDevice.IsNull() )
			{
				PRIBUR::ISpeedCollection* pSpeeds = m_hpDevice->GetSupportedWriteSpeeds();
				loop ( i, pSpeeds->GetCount() )
				{
					PRIBUR::ISpeedDescriptor* pSpeed = pSpeeds->GetItem(i);
					vdwSpeeds.Add(pSpeed->GetTransferRateKB());
					pSpeed->Release();
				}
				pSpeeds->Release();
			}
			return vdwSpeeds;
		}

		/**
		 * [取得] 最終トラック取得.
		 *	@note IsMediaReady() で true になってから使用します。
		 *	@retval 0以上 最終トラック。
		 *	@retval -1 エラー。
		 */
		int GetLastCompleteTrack(void)
		{
			if ( m_hpDevice.IsNull() )
			{
				return -1;
			}
			int nLastTrack = 0;
			PRIBUR::EMediaProfile mp = m_hpDevice->GetMediaProfile();
			if ( MP_DVD_PLUS_RW == mp || MP_DVD_MINUS_RW_RO == mp )
			{
				// DVD+RW and DVD-RW RO has only one session with one track
				if ( m_hpDevice->GetMediaFreeSpace() > 0 )
				{
					nLastTrack = 1;	
				}
			}		
			else
			{
				PRIBUR::READDISKINFO di;
				if ( m_hpDevice->ReadDiskInfo(&di) )
				{
					nLastTrack = di.wLastTrack;
					if ( DISK_STATUS_OPEN == di.eDiskStatus || DISK_STATUS_EMPTY == di.eDiskStatus )
					{
						nLastTrack--;
					}
				}
			}
			return nLastTrack;
		}

		/**
		 * [取得] １×速度取得
		 *	@param boIsDvd true なら DVD、falseならCD-Rでの1倍速の書込み速度を返します。
		 *	@return BYTE単位の速度。
		 */
		static double Get1xSpeed(bool boIsDvd)
		{
			return boIsDvd ? SPEED_DVD_1X_KB : SPEED_CD_1X_KB;
		}
		
		/**
		 * [取得] 速度取得
		 *	@note SetWriteSpeed() で設定した値（設定していない場合、デバイスの最高速）を返します。
		 *	@note IsMediaReady() で true になってから使用します。
		 *	@return BYTE単位の速度。
		 */
		DWORD GetWriteSpeed(void)
		{
			if ( m_hpDevice.IsNull() )
			{
				return 0;
			}
			if ( m_dwWriteSpeed == 0 )
			{
				return m_hpDevice->GetMaxWriteSpeedKB();
			}
			return m_dwWriteSpeed;
		}

		/**
		 * [設定] 書込み速度設定
		 *	@note 本クラスのメソッドのみに有効です。
		 *	@param dw BYTE単位の速度を指定。
		 */
		void SetWriteSpeed(DWORD dw)
		{
			m_dwWriteSpeed = dw;
		}

		/**
		 * [消去] イレース
		 *	@note イレースできるのは、CD-RWメディアと DVD-RWメディアだけです。
		 *	@param eType イレースタイプ。以下から指定します。省略すると、ERASE_DISC になります。\n
		 *					ERASE_DISC					Erase the entire disc	\n
		 *					ERASE_MINIMAL				Erase the PMA, 1st session TOC, pregap	\n
		 *					ERASE_LAST_TRACK			Erase last incomplete track	\n
		 *					ERASE_UNRESERVE_LAST_TRACK	Unreserve the last reserved track	\n
		 *					ERASE_UNCLOSE_LAST_TRACK	Erase the tail of a track - re-opens a complete track	\n
		 *					ERASE_UNCLOSE_LAST_SESSION	Reopen the last session	\n
		 *					ERASE_LAST_SESSION			Erase the last session
		 *	@retval ture 成功
		 *	@retval false 失敗
		 */
		bool Erase(EEraseType eType = ERASE_DISC)
		{
			if ( m_hpDevice.IsNull() )
			{
				return false;
			}
			PRIBUR::EMediaProfile mp = m_hpDevice->GetMediaProfile();
			if ( MP_DVD_MINUS_RW_SEQ != mp && MP_DVD_MINUS_RW_RO != mp && MP_CD_RW != mp )
			{
				//AfxMessageBox(_T("Erasing is supported on CD-RW and DVD-RW media only."), MB_OK);
				return false;
			}
			m_hpDevice->SetCallback13(&m_callbacker);
			m_hpDevice->SetCallback15(&m_callbacker);
			bool r = !! m_hpDevice->Erase13(eType);
			if ( m_boIsEject )
			{
				m_hpDevice->Eject(TRUE);
			}
			return r;
		}

		/**
		 * [消去] フォーマット
		 *	@note フォーマットできるのは、CD-RW DVD-RW DVD+RW メディアだけです。
		 *	@param boIsQuick true ならQuickFormat。省略か false なら完全Format。
		 *						この設定は、DVD+RW メディアの場合、無視されます。
		 *	@retval ture 成功
		 *	@retval false 失敗
		 */
		bool Format(bool boIsQuick = false)
		{
			if ( m_hpDevice.IsNull() )
			{
				return false;
			}
			PRIBUR::EMediaProfile mp = m_hpDevice->GetMediaProfile();
			m_hpDevice->SetCallback13(&m_callbacker);
			m_hpDevice->SetCallback15(&m_callbacker);
			BOOL bRes = TRUE;
			switch ( mp )
			{
			case MP_CD_RW:
				bRes = !! m_hpDevice->Erase13(boIsQuick ? ERASE_MINIMAL : ERASE_DISC);
				break;
			case MP_DVD_MINUS_RW_RO:
			case MP_DVD_MINUS_RW_SEQ:
				bRes = m_hpDevice->Format(boIsQuick ? FT_DVD_MINUS_RW_QUICK : FT_DVD_MINUS_RW_FULL);
				break;
			case MP_DVD_PLUS_RW:
				switch ( m_hpDevice->GetBgFormatStatus() )
				{
				case BFS_NOT_FORMATTED:
					bRes = m_hpDevice->Format(FT_DVD_PLUS_RW_FULL, 0, false);
					break;
				case BFS_PARTIAL:
					bRes = m_hpDevice->Format(FT_DVD_PLUS_RW_RESTART, 0, false);
					break;
				default:
					return false;
					break;
				}
				break;
			default:
				return false;
				break;
			}
			if ( m_boIsEject )
			{
				m_hpDevice->Eject(TRUE);
			}
			return !! bRes;
		}

		/**
		 * [書込] 書込み(イメージファイル指定)
		 *	@param lpszFile イメージファイル名
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		bool BurnImageFile(LPCTSTR lpszFile)
		{
			CDiscImage hpDataDisc;
			if ( ! m_PreBurn(hpDataDisc.operator->()) )
			{
				return false;
			}
			m_hpDevice->SetCallback13(&m_callbacker);
			m_hpDevice->SetCallback15(&m_callbacker);
			hpDataDisc->SetCallback14(&m_callbacker);
			bool r = !! hpDataDisc->WriteImageToDisc(CUnicode(lpszFile));
			if ( m_boIsEject )
			{
				m_hpDevice->Eject(TRUE);
			}
			return r;
		}

		/**
		 * [書込] 書込み(CDiscImage 指定)
		 *	@param dd CDiscImage のインスタンス
		 *	@retval true 成功
		 *	@retval false 失敗
		 */
		bool Burn(CDiscImage& dd) 
		{
			PRIBUR::IDataDisc* pDataDisc = dd.operator->(); 
			if ( ! m_PreBurn(pDataDisc) )
			{
				return false;
			}
			m_hpDevice->SetCallback13(&m_callbacker);
			m_hpDevice->SetCallback15(&m_callbacker);
			pDataDisc->SetCallback14(&m_callbacker);
			bool r = true;
			while ( true )
			{
				if ( ! pDataDisc->WriteToDisc(TRUE) )
				{
					if ( DATACD_ERROR_CANNOT_LOAD_IMAGE_LAYOUT == pDataDisc->GetLastError() )
					{
						pDataDisc->SetLoadTrackLayout(0);
						continue;
					}
					else
					{
						r = false;
					}
				}
				break;
			}
			pDataDisc->SetCallback14(NULL);
			if ( m_boIsEject )
			{
				m_hpDevice->Eject(TRUE);
			}
			return r;
		}

	private:

		#ifndef _TnbDOXYGEN

		/**
		 * 内部処理用 PrimoBurner コールバッククラス
		 */
		class CCallBacker : public PRIBUR::IDeviceCallback13, public PRIBUR::IDeviceCallback15, public PRIBUR::IDataDiscCallback14
		{
			IListener*	m_iListener;
		public:
			/// コンストラクタ
			CCallBacker(void) : m_iListener(NULL) {}
			/// デストラクタ
			virtual ~CCallBacker(void) {}
			/// リスナーセット
			void SetListener(IListener*	I)
			{
				m_iListener = I;
			}
			/// The OnProgress method is called by IDataDisc to report the number of bytes that have been written to the CD.
			virtual void OnProgress(uint64_t ddwPos, uint64_t ddwAll) 
			{
				TRACE3("OnProgress(%I64u,%I64u)  %f%%\n", ddwPos, ddwAll, (double)(LONGLONG)((ddwPos * 100) / ddwAll) );
				if ( m_iListener != NULL )
				{
					m_iListener->OnProgress(IListener::BURN, ddwPos, ddwAll);
				}
			}
			/// The OnStatus method is called by a IDataDisc instance to report the status of the burn process.
			virtual void OnStatus(PRIBUR::EDataDiscStatus eStatus) 
			{
				TRACE1("OnStatus(%d)\n", eStatus);
				if ( m_iListener != NULL )
				{
					m_iListener->OnStatus(eStatus);
				}
			}
			/// The OnFileStatus method is called by a IDataDisc instance to report the file that is currently being recorded.
			virtual void OnFileStatus(int nFile, const tchar_t* pFileName, int nPercent) 
			{
				nPercent = nPercent > 100 ? 100 : nPercent;
				TRACE3("No.%d %d%% [%s] \n", nFile, nPercent, CStr(pFileName));
				if ( m_iListener != NULL )
				{
					m_iListener->OnFileStatus(nFile, pFileName, nPercent);
				}
			}
			/// The OnContinueBurn method is called by a IDataDisc instance to check if the burning should continue. 
			virtual bool_t OnContinueBurn() 
			{
				TRACE0("OnContinueBurn()\n");
				if ( m_iListener != NULL )
				{
					return m_iListener->OnContinueBurn();
				}
				return true; 
			}
			/// Format進捗通知
			virtual void OnFormatProgress(double doProgress)
			{
				TRACE1("OnFormatProgress(%f%%)\n", doProgress);
				if ( m_iListener != NULL )
				{
					m_iListener->OnProgress(IListener::FORMAT, static_cast<DWORD>(doProgress) * 100, 10000);
				}
			}
			/// Erase進捗通知
			virtual void OnEraseProgress(double doProgress) 
			{
				TRACE1("OnEraseProgress(%f%%)\n", doProgress);
				if ( m_iListener != NULL )
				{
					m_iListener->OnProgress(IListener::FORMAT, static_cast<DWORD>(doProgress * 100), 10000);
				}
			}
			/// The OnWriteLeadIn method is called to report the progress during the recording of the lead in area of a session.
			virtual void OnWriteLeadIn(uint32_t dwBlocks, uint32_t dwAllBlocks) 
			{
				TRACE2("OnWriteLeadIn(%u,%u)\n", dwBlocks, dwAllBlocks);
				if ( m_iListener != NULL )
				{
					m_iListener->OnProgress(IListener::WRITELEADIN, dwBlocks, dwAllBlocks);
				}
			}
			/// The OnWriteLeadOut method is called to report the progress during the recording of the lead out area of a session.
			virtual void OnWriteLeadOut(uint32_t dwBlocks, uint32_t dwAllBlocks) 
			{
				TRACE2("OnWriteLeadOut(%u,%u)\n", dwBlocks, dwAllBlocks);
				if ( m_iListener != NULL )
				{
					m_iListener->OnProgress(IListener::WRITELEADOUT, dwBlocks, dwAllBlocks);
				}
			}
		};

		#endif _TnbDOXYGEN

		/// PRIBUR::IDevice* 用ハンドル
		typedef CPointerHandleBaseT<PRIBUR::IDevice*, TPhReleasePointerT<PRIBUR::IDevice*> > CDeviceHandle;
		CDeviceHandle		m_hpDevice;		///< PRIBUR::IDevice インスタンス管理
		DWORD				m_dwWriteSpeed;	///< 書込み速度
		bool				m_boIsEject;	///< 実行後、Ejectする？
		CCallBacker			m_callbacker;	///< リスナ管理

		/**
		 * [書込] イメージファイル書込準備
		 *
		 */
		bool m_PreBurn(PRIBUR::IDataDisc * pDataDisc) 
		{
			if ( pDataDisc == NULL || m_hpDevice.IsNull() )
			{
				return false;
			}
			m_hpDevice->SetWriteSpeedKB(GetWriteSpeed());
			pDataDisc->SetDevice(m_hpDevice);
			EMediaProfile mp = m_hpDevice->GetMediaProfile();
			if ( mp == MP_DVD_PLUS_RW )
			{
				switch ( m_hpDevice->GetBgFormatStatus() )
				{
				case BFS_NOT_FORMATTED:
					m_hpDevice->Format(FT_DVD_PLUS_RW_FULL);
					break;
				case BFS_PARTIAL:
					m_hpDevice->Format(FT_DVD_PLUS_RW_RESTART);
					break;
				}
			}
			pDataDisc->SetCallback14(&m_callbacker);
			if ( mp == MP_CD_ROM || mp == MP_CD_R || mp == MP_CD_RW )
			{
				pDataDisc->SetSessionStartAddress(m_hpDevice->GetNewSessionStartAddress());
				pDataDisc->SetCacheSmallFiles(FALSE);
	//			pDataDisc->SetCacheSmallFiles(m_ctx.bCacheSmallFiles);
	//			if (m_ctx.bCacheSmallFiles) 
	//			{
	//				pDataDisc->SetSmallFileSizeThreshold(m_ctx.dwSmallFileSize);
	//				pDataDisc->SetSmallFilesCacheSize(m_ctx.dwSmallFilesCacheLimit);
	//			}
				// Set write mode
				pDataDisc->SetWriteMethod(WRITE_METHOD_TAO);
	//			if (!m_ctx.bRaw && (0 == m_ctx.nMode || 1 == m_ctx.nMode))
	//				// Session-At-Once (also called Disc-At-Once)
	//				pDataDisc->SetWriteMethod(WRITE_METHOD_SAO);
	//			else if (m_ctx.bRaw)
	//				// RAW Disc-At-Once
	//				pDataDisc->SetWriteMethod(WRITE_METHOD_RAW_DAO);
	//			else if (2 == m_ctx.nMode)
	//				// Track-At-Once
	//				pDataDisc->SetWriteMethod(WRITE_METHOD_TAO);
	//			else
	//				// Packet
	//				pDataDisc->SetWriteMethod(WRITE_METHOD_PACKET);
			}
			else
			{
				pDataDisc->SetCacheSmallFiles(FALSE);
				// Set write mode
				pDataDisc->SetWriteMethod(WRITE_METHOD_DVD_INCR);
	//			if (0 == m_ctx.nMode)
	//				// DVD Disc-At-Once
	//				pDataDisc->SetWriteMethod(WRITE_METHOD_DVD_DAO);
	//			else if (1 == m_ctx.nMode)
	//				// DVD Incremental
	//				pDataDisc->SetWriteMethod(WRITE_METHOD_DVD_INCR);
			}
	//		pDataDisc->SetSimulateBurn(m_ctx.bSimulate);
			pDataDisc->SetSimulateBurn(FALSE);
	//		pDataDisc->SetCloseDisk(m_ctx.bCloseDisk);
			pDataDisc->SetCloseDisk(TRUE);
			return true;
		}
	};
	

	//-----------------------------------------

	
	/// コンストラクタ
	CPrimoBurner(void) : m_pEngine(NULL), m_pEnumerator(NULL), m_iDeviceNumber(0)
	{
	}

	/// デストラクタ
	~CPrimoBurner(void)
	{
		m_CloseEnumerator();
		if ( m_pEngine != NULL )
		{
			m_pEngine->Shutdown();
			m_pEngine->Release();
			m_pEngine = NULL;
		}
	}

	/**
	 * [設定] 初期化
	 *	@note	初期化をします。また、CD-R/RW,DVD±R/RW デバイスリストも作成します。
	 *			もしデバイスの状態（数）が変わったら、再度これをコールする必要があります。
	 *	@param boIsWritableOnly true か省略なら書込み可能デバイス。false なら全て。
	 *	@retval マイナス PrimoBurner の初期化失敗
	 *	@retval 0 デバイスが接続されていない
	 *	@retval 1以上 接続している デバイスの数。
	 */
	int Initialize(bool boIsWritableOnly = true)
	{
		if ( ! m_InitEngine() )
		{
			return -1;
		}
		m_vDeviceNames.RemoveAll();
		m_CloseEnumerator();
		m_pEnumerator = m_pEngine->GetDevices();
		int iCount = m_pEnumerator->GetCount();
		if ( iCount <= 0 ) 
		{
			m_CloseEnumerator();
			return -1;
		}
		m_vDeviceNames.SetSize(iCount);
		int r = 0;
		for ( int i = 0; i < iCount; i++ ) 
		{
			PRIBUR::IDevice* pDevice = m_pEnumerator->GetItem(i);
			if ( pDevice != NULL )
			{
				if ( ! boIsWritableOnly || pDevice->GetWritePossible() )
				{
					CUnicode strDesc;
					LPWSTR P = strDesc.GetBuffer(256);
					memset(P, 0, 256 * sizeof(WCHAR));
					pDevice->GetDescription(P, 256);
					strDesc.ReleaseBuffer();
					int iLetter = pDevice->GetDriveLetter();
					m_vDeviceNames[i].Format(_T("%c:%s"), iLetter & 0xDF, CStr(strDesc));
					TRACE2("%d %s\n", i, m_vDeviceNames[i]);
					r++;
				}
				pDevice->Release();
			}
		}
		m_iDeviceNumber = r;
		return r;
	}

	/**
	 * [取得] デバイス数取得
	 *	@note Initialize() で走査したデバイスの数を返します。
	 *	@return デバイス数
	 */
	int GetFoundDeviceNumber(void) const
	{
		return m_iDeviceNumber;
	}

	/**
	 * [取得] デバイス名取得
	 *	@param iIndex インデックス。0以上、 GetFoundDeviceNumber() の戻り値未満で指定します。
	 *	@return デバイス名。ドライブレターが先頭に付加されます。
	 */
	CStr GetFoundDeviceName(int iIndex) const
	{
		int c = 0;
		loop ( i, m_vDeviceNames.GetSize() )
		{
			const CStr& s = m_vDeviceNames[i];
			if ( ! s.IsEmpty() )
			{
				if ( c == iIndex )
				{
					return s;
				}
				c++;
			}
		}
		return CStr();
	}

	/**
	 * [選択] 対象デバイス選択
	 *	@param tchLetter ドライブレター('a'～'z')
	 *	@retval true 選択
	 *	@retval false 失敗
	 */
	CDevice CreateDevice(TCHAR tchLetter)
	{
		if ( m_pEnumerator == NULL )
		{
			Initialize();
		}
		loop ( i, m_vDeviceNames.GetSize() )
		{
			const CStr& s = m_vDeviceNames[i];
			if ( ! s.IsEmpty() && s[0] == (tchLetter & 0xDF) )
			{
				CDevice r(m_pEnumerator->GetItem(i));
				return r;
			}
		}
		return CDevice();
	}

private:
	/// Engineクラス初期化
	bool m_InitEngine(void)
	{
		if ( m_pEngine == NULL )
		{
			m_pEngine = hpCDE::CreateEngine();
			if ( ! m_pEngine->Initialize() && ! m_pEngine->Initialize(TRUE) )
			{
				return false;
			}
		}
		return true;
	}
	/// 一覧クラス、クローズ
	void m_CloseEnumerator(void)
	{
		if ( m_pEnumerator != NULL) 
		{
			m_pEnumerator->Release();
			m_pEnumerator = NULL;
		}
	}
	PRIBUR::IEngine*			m_pEngine;			///< エンジンクラス管理
	PRIBUR::IDeviceCollection*	m_pEnumerator;		///< デバイス一覧クラス管理
	CStrVector					m_vDeviceNames;		///< デバイス名一覧
	int							m_iDeviceNumber;	///< 使用可能なデバイス数
};



}; //TNB



