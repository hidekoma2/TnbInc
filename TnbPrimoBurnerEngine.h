#pragma once
/**
 *@file
 * PrimoBurner�֌W�̃w�b�_
 *
 *		<a class="el" href="http://www.primoburner.com/"> PrimoBurner</a> ���g�p���܂��B
 *
 *	@note	��UNICODE �v���W�F�N�g�ł��A�N���X�����ł� UNICODE �œ��삳���Ă��܂��B
 *			����́AV1.99 �����{��̃t�@�C����/�f�B���N�g�������������������߂Ȃ����߂ł��B
 *
 *	@note	���݁AVer1.99 �� 2.10 �̂����ꂩ���g�p�\�ł��B
 *	@note	���݁AVer2.10 �ȍ~�͓��얢�m�F�ł��B
 *
 *	@todo	2.10���g�p�̏ꍇ�ł��A Blu-ray Disc �͖��Ή��ł��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbFileFinder.h"
#include "TnbFileName.h"
#include "TnbStrVector.h"
#include "TnbTickCount.h"



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

#if defined(___PRIMO_TCHAR_T) | defined(__PRIMO_CHAR_T)
 #error hpCDE.h will be included previously.
#endif
#define ___PRIMO_TCHAR_T
typedef wchar_t tchar_t;
#define ___PRIMO_CHAR_T
typedef wchar_t char_t;

// �ȉ���Path��ʂ�����
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
	// PrimoBurner 2.10�ȍ~���g�p
	#define _TnbPrimoBurner_Used_V2
#else
	// PrimoBurner 2.00�ȍ~���g�p
	#pragma comment(lib,"HfsU.lib")
#endif

#endif // _TnbDOXYGEN



//TNB Library
namespace TNB
{



/// namespace primo::burner �̕ʖ�
#define PRIBUR ::primo::burner
/// namespace primo::scsi �̕ʖ�
#ifdef _TnbPrimoBurner_Used_V2
 #define PRISCSI ::primo::scsi
#else
 #define PRISCSI ::primo::burner
#endif



/**@ingroup PRIMOBURNER
 * PrimoBurner �Ǘ��N���X
 *
 *	@par�K�v�t�@�C��
 *			TnbPrimoBurnerEngine.h
 *
 *	@date 07/01/29	�V�K�쐬
 *	@date 09/08/04	�N���X���A�\���ύX�B
 */
class CPrimoBurner : CCopyImpossible
{
public:

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

	// PointerHandleBase�p �ėp �|�C���^�����[�X�e���v���[�g�N���X
	template<typename T> struct TPhReleasePointerT
	{
		void operator()(T P) { P->Release(); }
	};

	#endif // _TnbDOXYGEN


	/**@ingroup PRIMOBURNER
	 * PrimoBurner �f�B�X�N�C���[�W���Ǘ��N���X
	 *
	 *		�f�B�X�N�C���[�W���Ǘ����܂��B�{�N���X�̓t�@�C���̎��͎̂������ɁA
	 *		�t�@�C�����A�f�B���N�g������ێ����Ă��܂��B
	 *
	 *	@note SetDefaultParametersForCd() ���g���ƁAJoliet CD �C���[�W���e�Ղɐݒ�ł��܂��B
	 *	@note SetDefaultParametersForDvd() ���g���ƁA UDF �C���[�W���e�Ղɐݒ�ł��܂��B
	 *	
	 *	@note �����\�ł��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbPrimoBurnerEngine.h
	 *
	 *	@date 07/01/29	�V�K�쐬
	 *	@date 09/08/04	�N���X���A�\���ύX�B
	 */
	class CDiscImage
	{
	public:

		/**@ingroup PRIMOBURNER
		 * PrimoBurner �f�B�X�N�C���[�W��񃊃X�i�[�C���^�[�t�F�[�X
		 *
		 *	@date 07/01/29	�V�K�쐬
		 */
		struct IListener
		{
			/// �f�X�g���N�^
			virtual ~IListener(void) {}

			/**
			 * [�ʒm] �t�@�C������
			 *	@note �C���[�W�쐬�����Ŏg�p���̃t�@�C������ʒm���܂��B
			 *	@param nFile		�ʂ��ԍ��B
			 *	@param lpszFileName	�t�@�C�����B
			 *	@param nPercent		�p�[�Z���g�B
			 */
			virtual void OnFileStatus(int nFile, LPCWSTR lpszFileName, int nPercent) = 0;
		};

		/// �R���X�g���N�^
		CDiscImage(void) : m_hpDataDisc(hpCDE::CreateDataDisc()), m_hpDataFile(hpCDE::CreateDataFile())
		{		 
			m_hpDataFile->SetDirectory(TRUE);		// Root��ݒ肵�Ă���
			m_hpDataFile->SetFilePath(L"\\");			
			m_hpDataFile->SetLongFilename(L"\\");
		}

		/// �f�X�g���N�^
		~CDiscImage(void)
		{
		}

		/**
		 * [�o�^] ���X�i�[�o�^
		 *	@param I ���X�i�[
		 */
		void SetListener(IListener* I)
		{
			m_callbacker.SetListener(I);
		}

		/**
		 * [�擾] IDataDisc �|�C���^�Q��
		 *	@return IDataDisc �|�C���^
		 */
		PRIBUR::IDataDisc* operator->(void)
		{
			return m_hpDataDisc;
		}

		/**
		 * [�擾] �G���[��擾
		 *	@note ������G���[��́A�V�X�e���̂��̂Ƃ͈قȂ�܂��B
		 *	@return <a class="el" href="http://www.primoburner.com/documentation/primo__burner__EDataDiscError.html"> EDataDiscError</a>
		 */
		DWORD GetLastError(void)
		{
			return m_hpDataDisc.IsNull() ? 0 : m_hpDataDisc->GetLastError();
		}

		/**
		 * [�ݒ�] CD�p��{�ݒ�
		 *	@note Joliet �ō쐬���邱�Ƃ��w��B
		 *	@param lpszVolume �{�����[����
		 *	@param eType �C���[�W�̃^�C�v���w��B�f�t�H���g�́A Joliet�B
		 *	@param dwFlags �C���[�W�^�C�v���̏ڍ׃t���O�Benum EImageConstraint �̑g�ݍ��킹���w��B
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
		 * [�ݒ�] DVD�p��{�ݒ�
		 *	@note UDF �ō쐬���邱�Ƃ��w��B
		 *	@param lpszVolume �{�����[����
		 *	@param eType �C���[�W�̃^�C�v���w��B�f�t�H���g�́A UDF�B
		*/
		void SetDefaultParametersForDvd(LPCTSTR lpszVolume, EImageType eType = IMG_TYPE_UDF)
		{
			SetDefaultParametersForCd(lpszVolume, eType);
		}

		/**
		 * [�ݒ�] Bootable�ݒ�
		 *	@param boIsBootableCd true �Ȃ瑼�̃p�����[�^���ݒ肷��K�v������܂��B
		 *	@param eEmu �G�~�����[�V�������[�h�ݒ�B�ȗ������ BMT_NO_EMULATION �B
		 *	@param lpszbootImageFile �u�[�g�C���[�W�t�@�C���B�ȗ������NULL�ɂȂ�܂��B
		 */
		void SetBootableParameters(
						bool boIsBootableCd, EBootEmulation eEmu = BMT_NO_EMULATION, LPCTSTR lpszbootImageFile = NULL)
		{
			m_hpDataDisc->SetBootable(boIsBootableCd);
			m_hpDataDisc->SetBootEmulation(eEmu);
			m_hpDataDisc->SetBootImageFile(CUnicode(lpszbootImageFile));
		}

		/**
		 * [�ݒ�] �f�B�X�N�C���[�W�N���A
		 *	@note Add �����t�@�C�����N���A���܂��BAdd���Ȃ����Ƃ��Ɏg�p�ł��܂��B
		 */
		void ResetFileImage(void)
		{
			m_SetLayout(false);
		}

		/**
		 * [�o�^] �t�@�C���ǉ�
		 *	@note ���̃t�@�C�����R�s�[�ΏۂɂȂ�܂��B
		 *	@param lpszName �o�^�ΏہB
		 *	@param lpszImagePath CD�C���[�W���Path�i�t���p�X�Ŏw��̕K�v����j�B�ȗ�����ƃ��[�g�ɂȂ�܂��B
		 *	@retval ture �ǉ������B
		 *	@retval false ���s�B�C���[�W�f�B�X�N�͋�ɂȂ�܂��B
		 */
		bool AddSource(LPCTSTR lpszName, LPCTSTR lpszImagePath = NULL)
		{
			CAppendFile a(lpszName);
			return m_SetLayout(a.Execute(m_hpDataFile, m_hpDataDisc->GetImageType(), lpszImagePath));
		}

		/**
		 * [�o�^] �t�@�C���Q�ǉ�
		 *	@note �f�B���N�g���ȉ����R�s�[�ΏۂɂȂ�܂��B	
		 *	@param lpszName �o�^�ΏہB
		 *	@param lpszImagePath CD�C���[�W���Path�i�t���p�X�Ŏw��̕K�v����j�B�ȗ�����ƃ��[�g�ɂȂ�܂��B
		 *	@retval ture �ǉ������B
		 *	@retval false ���s�B�C���[�W�f�B�X�N�͋�ɂȂ�܂��B
		 */
		bool AddSourceFromPath(LPCTSTR lpszName, LPCTSTR lpszImagePath = NULL)
		{
			CAppendPath a(lpszName);
			return m_SetLayout(a.Execute(m_hpDataFile, m_hpDataDisc->GetImageType(), lpszImagePath));
		}

		/**
		 * [�쐬] �C���[�W�t�@�C���쐬.
		 *	@note �o�^���ꂽ���e�ŃC���[�W�t�@�C�����쐬���܂��B
		 *	@param lpszFile �o�̓t�@�C�����B�ʏ�g���q�� iso ���w�肵�܂��B
		 *	@retval ture �����B
		 *	@retval false ���s�B
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

		#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

		/**
		 * ���������p PrimoBurner �R�[���o�b�N�N���X
		 */
		class CCallBacker : public PRIBUR::IDataDiscCallback14
		{
			IListener*	m_iListener;
		public:
			/// �R���X�g���N�^
			CCallBacker(void) : m_iListener(NULL) {}
			/// �f�X�g���N�^
			virtual ~CCallBacker(void) {}
			/// ���X�i�[�Z�b�g
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

		/// PRIBUR::IDataDisc* �p�n���h��
		typedef CPointerHandleBaseT<PRIBUR::IDataDisc*, TPhReleasePointerT<PRIBUR::IDataDisc*> > CDataDiscHandle;
		/// PRIBUR::IDataFile* �p�n���h��
		typedef CPointerHandleBaseT<PRIBUR::IDataFile*, TPhReleasePointerT<PRIBUR::IDataFile*> > CDataFileHandle;
		CDataDiscHandle			m_hpDataDisc;	///< PRIBUR::IDataDisc �C���X�^���X�Ǘ�
		CDataFileHandle			m_hpDataFile;	///< PRIBUR::IDataFile �C���X�^���X�Ǘ�
		CCallBacker				m_callbacker;	///< ���X�i�Ǘ�

		/// �o�^�p�t�@�C�����
		struct TFileInfo
		{
			CStr		strFileName;				///< �t�@�C����
			CStr		strAlternateFileName;		///< Short�t�@�C����
			bool		boHasHidden;				///< Hidden�t���O�̗L��
			CFileTimeEx	timeLastWrite;				///< �쐬����
			/// �R���X�g���N�^
			TFileInfo(void) {}
			TFileInfo(const WIN32_FIND_DATA& fd) {
				Set(fd);
			}
			/// �ݒ�
			void Set(const WIN32_FIND_DATA& fd) {
				strFileName				= fd.cFileName;
				strAlternateFileName	= fd.cAlternateFileName;
				boHasHidden				= (fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0;
				timeLastWrite			= fd.ftLastWriteTime;
			}
			/// �t�@�C�����擾
			CStr GetFileName(void) const {
				return strFileName;
			}
			/// �t�@�C�����擾
			CStr GetAlternateFileName(void) const {
				return strAlternateFileName.Left(12);
			}
			/// ���Ԏ擾
			FILETIME GetFileTime(void) const {
				return timeLastWrite;
			}
		};

		/// �ǉ��p�x�[�X�N���X
		class CAbstractAppender
		{
			/// �@�艺���i�ċN�Ăяo���j
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
			/// �N��
			virtual bool OnSetup(void) = 0;
			/// ����
			virtual bool OnFound(const CDataFileHandle& hpFile, DWORD dwImageType) = 0;
			/// ���s
			bool Execute(const CDataFileHandle& hpFile, DWORD dwImageType, LPCTSTR lpszImagePath)
			{
				if ( ! OnSetup() ) { return false; }
				CStr str = lpszImagePath;
				return m_Dig(hpFile, dwImageType, str);
			}
			/**
			 * IDataFile�ցA�o�^
			 *	@param[out] _pFile �ǉ���
			 *	@param[in] boDir true �Ȃ� �f�B���N�g���w��Bfalse �Ȃ�t�@�C���w��B
			 *	@param[in] lpszFilePath �t�@�C���w��̏ꍇ�A���ۑ��݂���t�@�C���̏ꏊ���w�肵�܂�(�ʏ�t���p�X�w��)�B
			 *	@param[in] dwImageType �C���[�W�^�C�v�B
			 *	@param[in] fd �t�@�C�����B
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

		/// �t�@�C���ǉ��N���X
		class CAppendFile : public CAbstractAppender
		{
			DEFSUPER(CAbstractAppender);
			CStr		m_strFullPath;
			TFileInfo	m_fileInfo;
		public:
			/// �R���X�g���N�^
			CAppendFile(LPCTSTR lpszFullPath) : m_strFullPath(lpszFullPath) {}
			/// �N��
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
			/// ����
			virtual bool OnFound(const CDataFileHandle& hpFile, DWORD dwImageType)
			{
				CDataFileHandle F = hpCDE::CreateDataFile(); 
				SetDataFile(F, false, m_strFullPath, dwImageType, m_fileInfo);
				hpFile->AddItem(F);
				return true;
			}
		};

		/// �p�X�ǉ��N���X
		class CAppendPath : public CAbstractAppender
		{
			DEFSUPER(CAbstractAppender);
			CStr		m_strPath;
			/// �ǉ�
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
			/// �R���X�g���N�^
			CAppendPath(LPCTSTR lpszPath) : m_strPath(lpszPath) {}
			/// �N��
			virtual bool OnSetup(void)
			{
				CFileFinder ff;
				return ff.Start(m_strPath, _T("\\*"));
			}
			/// ����
			virtual bool OnFound(const CDataFileHandle& hpFile, DWORD dwImageType)
			{
				return m_AddTree(hpFile, dwImageType, m_strPath);
			}
		};

		#endif //_TnbDOXYGEN

		/**
		 * m_hpDataFile �� m_hpDataDisc �ɃZ�b�g
		 *	@param boIsSet true �Ȃ�Z�b�g�B�@ false �Ȃ�N���A��Ԃ�
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
				m_hpDataFile->SetDirectory(TRUE);		//���[�g��ݒ肵�Ă���
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
	 * PrimoBurner SCSI�Ǘ��N���X
	 *
	 * @note CPrimoBurner::CDevice::CreateScsi() �ɂ��쐬����܂��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbPrimoBurnerEngine.h
	 *
	 *	@date 07/02/02	�V�K�쐬
	 *	@date 07/02/05	Vector���g�p�������\�b�h��ǉ��BModeSense��ǉ��B
	 *	@date 09/08/04	�N���X���A�\���ύX�B
	 */
	class CScsi
	{
	public:

		/**
		 * �R���X�g���N�^
		 *	@param P IScsiInterface �C���X�^���X�̃|�C���^�B�ȗ������ NULL�B
		 */
		CScsi(PRISCSI::IScsiInterface* P = NULL) : m_hpScsi(P)
		{
		}

		/// �f�X�g���N�^
		~CScsi(void)
		{
		}

		/**
		 * [�m�F] �L���m�F.
		 *		�{�C���X�^���X�� �L���iIScsiInterface �������Ă���j���A�m�F���܂��B
		 *	@retval true �L���B�e���\�b�h�͎g���܂��B
		 *	@retval false �����B
		 */
		bool IsValid(void) const
		{
			return ! m_hpScsi.IsNull();
		}

		/**
		 * [���M] CDB���M.
		 *	@param cdb CDB�B
		 *	@param dwTimeout �^�C���A�E�g���ԁims�j�B�ȗ�����ƂR�����w�肳��܂��B
		 *	@retval true ����
		 *	@retval false ���s�B GetLastError() �ŃG���[���e���m�F�ł��܂��B
		 */
		bool SendCommand(const ISequenceCollectionT<BYTE>& cdb, DWORD dwTimeout = SCSI_CMD_TIMEOUT_MS_LONG)
		{
			if ( m_hpScsi.IsNull() ) { return false; }
			return !! m_hpScsi->SendCommand(cdb.ReferBuffer(), cdb.GetSize(), SCSI_CMD_DIR_UNSPECIFIED, NULL, 0, dwTimeout);
		}

		/**
		 * [���M] CDB���M(�Ǎ��݃t�F�[�Y�t).
		 *	@param cdb CDB�B
		 *	@param iReadSize �ǂݍ��݃f�[�^��(CDB�ɐ������ǂݍ��݃f�[�^�T�C�Y���w�肵�Ă�������)�B
		 *	@param dwTimeout �^�C���A�E�g���ԁims�j�B�ȗ�����ƂR�����w�肳��܂��B
		 *	@retval Valid��� �����B�ǂݍ��񂾃f�[�^�������Ă��܂��B
		 *	@retval Invalid��� false ���s�B GetLastError() �ŃG���[���e���m�F�ł��܂��B
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
		 * [���M] CDB���M(�����݃t�F�[�Y�t).
		 *	@param cdb CDB�B
		 *	@param d �����݃f�[�^(CDB�ɐ������f�[�^�T�C�Y�����w�肵�Ă�������)�B
		 *	@param dwTimeout �^�C���A�E�g���ԁims�j�B�ȗ�����ƂR�����w�肳��܂��B
		 *	@retval true ����
		 *	@retval false ���s�B GetLastError() �ŃG���[���e���m�F�ł��܂��B
		 */
		bool SendCommandWithWritePhase(
				const ISequenceCollectionT<BYTE>& cdb, const ISequenceCollectionT<BYTE>& d, DWORD dwTimeout = SCSI_CMD_TIMEOUT_MS_LONG)
		{
			if ( m_hpScsi.IsNull() ) { return false; }
			return !! m_hpScsi->SendCommand(
						cdb.ReferBuffer(), cdb.GetSize(), SCSI_CMD_DIR_WRITE, const_cast<BYTE*>(d.ReferBuffer()), d.GetSize(), dwTimeout);
		}

		/**
		 * [���M] MODESENSE CDB���M.
		 *	@param pc �y�[�W�R�[�h�A����сA�y�[�W�R���g���[���l�B
		 *	@param boIsDbd true �Ȃ� DBD���ǂݍ��݂܂��B �ȗ��� false �Ȃ� �ǂݍ��݂܂���B
		 *	@param dwTimeout �^�C���A�E�g���ԁims�j�B�ȗ�����ƂR�����w�肳��܂��B
		 *	@retval Valid��� �����B�ǂݍ��񂾃f�[�^�������Ă��܂��B
		 *	@retval Invalid��� false ���s�B GetLastError() �ŃG���[���e���m�F�ł��܂��B
		 */
		CByteVector Send_ModeSense(BYTE pc, bool boIsDbd = false, DWORD dwTimeout = SCSI_CMD_TIMEOUT_MS_LONG)
		{
			CByteVector cdb;
			cdb.SetSize(6);
			cdb[0] = 0x1A;	//ModeSense
			cdb[1] = _BIT(3); //Block Descriptor �͓]���Ȃ��B
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
		 * [���M] INQUIRY CDB���M.
		 *	@param dwTimeout �^�C���A�E�g���ԁims�j�B�ȗ�����ƂR�����w�肳��܂��B
		 *	@retval Valid��� �����B�ǂݍ��񂾃f�[�^�������Ă��܂��B
		 *	@retval Invalid��� false ���s�B GetLastError() �ŃG���[���e���m�F�ł��܂��B
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
		 * [�擾] �G���[��擾
		 *	@note ������G���[��́A�V�X�e���̂��̂Ƃ͈قȂ�܂��B
		 *	@note Send*() ���\�b�h���s���ʂ� ���s���������A�{���\�b�h�����s���邱�Ƃŏڍׂ��킩��܂��B
		 *	@retval SCSIINTERFACE_SUCCESS			�����B
		 *	@retval SCSIINTERFACE_SYSTEM_ERROR		���s�̗��R�́A�V�X�e��API�ŃG���[���o�����߂ł��B 
		 *											::GetLastError() �ŃV�X�e���G���[�R�[�h���擾���Ă��������B
		 *	@retval SCSIINTERFACE_DEVICE_ERROR		�f�o�C�X�̃G���[�ł��B �f�o�C�X�� CPrimoBurner::CDevice::GetLastError() ��
		 *											�g���A�G���[�R�[�h���擾���Ă��������B
		 *	@retval SCSIINTERFACE_DEVICE_NOT_SET	�f�o�C�X�͐ݒ肳��Ă��܂���B
		 *	@retval SCSIINTERFACE_INVALID_PARAMETERS	�p�����[�^�Ɉُ킪����܂��B
		 *	@retval SCSIINTERFACE_CHECK_SENSE		�f�o�C�X���Z���X��Ԃł��B GetSense() �ŃZ���X�f�[�^���擾���Ă��������B
		 */
		DWORD GetLastError(void)
		{
			return m_hpScsi.IsNull() ? 0 : m_hpScsi->GetLastError();
		}

		/**
		 * [�擾] �V�X�e���G���[�R�[�h�擾
		 *	@note ���̃��\�b�h�́A GetLastError()�� SCSIINTERFACE_SYSTEM_ERROR ���ʒm���ꂽ���A�g�p���܂��B
		 *	@return �V�X�e���G���[�R�[�h�B
		 */
		DWORD GetLastSystemError(void)
		{
			return m_hpScsi.IsNull() ? 0 : m_hpScsi->GetLastSystemError();
		}

		/**
		 * [�擾] �Z���X�f�[�^�擾
		 *	@note ���̃��\�b�h�́A GetLastError()�� SCSIINTERFACE_CHECK_SENSE ���ʒm���ꂽ���A�g�p���܂��B
		 *	@param[out] _sense �擾�Z���X�f�[�^�A�i�[�B
		 *	@retval true �����B
		 *	@retval false ���s�B
		 */
		bool GetSense(PRISCSI::SCSI_COMMAND_SENSE& _sense)
		{
			if ( m_hpScsi.IsNull() ) { return false; }
			m_hpScsi->GetSense(&_sense);
			return true;
		}

	private:
		/// PRIBUR::IScsiInterface* �p�n���h��
		typedef CPointerHandleBaseT<PRISCSI::IScsiInterface*, TPhReleasePointerT<PRISCSI::IScsiInterface*> > CScsiHandle;
		CScsiHandle		m_hpScsi;		///< PRIBUR::IScsiInterface �C���X�^���X�Ǘ�
	};


	
	/**@ingroup PRIMOBURNER
	 * PrimoBurner �f�o�C�X���Ǘ��N���X
	 *
	 *	@note CPrimoBurner::CreateDevice() �ɂ��쐬����܂��B
	 *	@note IsMediaReady() �Ń��f�B�A�̑}����Ԃ��킩��܂��B
	 *	@note GetMediaProfile() �ő}������Ă��郁�f�B�A�킪�킩��܂��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbPrimoBurnerEngine.h
	 *
	 *	@date 07/01/29	�V�K�쐬
	 *	@date 09/08/04	�N���X���A�\���ύX�B
	 */
	class CDevice
	{
	public:

		/**@ingroup PRIMOBURNER
		 * PrimoBurner �f�o�C�X��񃊃X�i�[�C���^�[�t�F�[�X
		 *
		 *	@date 07/01/29	�V�K�쐬
		 */
		struct IListener
		{
			/// �i�����
			enum EType
			{
				BURN,			///< �Ă����ݒ�
				FORMAT,			///< Format,Erase��
				WRITELEADIN,	///< LeadIn�����ݒ�
				WRITELEADOUT,	///< LeadOut�����ݒ�
			};

			/// �f�X�g���N�^
			virtual ~IListener(void) {}

			/**
			 * [�ʒm] �i���ʒm
			 *	@note �Ă����݂�t�H�[�}�b�g�A�C���[�X���̐i����ʒm���܂��B
			 *	@param eType ��ށB
			 *	@param ullPos ���ݒl�B
			 *	@param ullAll �S�̒l�B
			*/
			virtual void OnProgress(EType eType, ULONGLONG ullPos, ULONGLONG ullAll) = 0;

			/**
			 * [�ʒm] ������Ԓʒm
			 *	@note �Ă����ݒ��̏�����Ԃ�ʒm���܂��B��Ԃ��ω�����ƒʒm����܂��B
			 *	@param eStatus ���݂̏�����ԁB�ȉ��̒l������܂��B\n
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
			 * [�ʒm] �t�@�C������
			 *	@note �C���[�W�쐬�����Ŏg�p���̃t�@�C������ʒm���܂��B
			 *	@param nFile		�ʂ��ԍ��B
			 *	@param lpszFileName	�t�@�C�����B
			 *	@param nPercent		�p�[�Z���g�B
			 */
			virtual void OnFileStatus(int nFile, LPCWSTR lpszFileName, int nPercent) = 0;

			/**
			 * [�m�F] �p���m�F
			 *	@note �������A�K���ȃ^�C�~���O�ł��̃��\�b�h���R�[������܂��B�L�����Z���������ꍇ�A false ��Ԃ��܂��B
			 *	@retval true �p���B
			 *	@retval false ���~�˗��B
			*/
			virtual bool OnContinueBurn() = 0;
		};

		/**
		 * �R���X�g���N�^
		 *	@param P IDevice�C���X�^���X�̃|�C���^�B�ȗ������ NULL�B
		 */
		CDevice(PRIBUR::IDevice* P = NULL) 
				: m_hpDevice(P), m_dwWriteSpeed(0), m_boIsEject(true)
		{
		}

		/// �f�X�g���N�^
		~CDevice(void)
		{
		}

		/**
		 * [�o�^] ���X�i�[�o�^
		 *	@param I ���X�i�[
		 */
		void SetListener(IListener* I)
		{
			m_callbacker.SetListener(I);
		}

		/**
		 * [�o�^] ������Eject�ݒ�.
		 *	@note �{���\�b�h�Őݒ肵�Ȃ��ƁA������C�W�F�N�g���܂��B
		 *	@param boIsEject true �Ȃ� �������݁A������A���f�B�A��Eject���܂��B
		 */
		void SetEjectMode(bool boIsEject)
		{
			m_boIsEject = boIsEject;
		}

		/**
		 * [����] �N���[�Y
		 *	@note	�{�C���X�^���X�ŊǗ����Ă��� IDevie����܂��i�������Ă���ꍇ�A
		 *			���ׂĂ̕������N���[�Y/�f�X�g���N�g����܂Ő����܂��j�B
		 */
		void Close(void)
		{
			m_hpDevice.Null();
		}

		/**
		 * [�m�F] �L���m�F.
		 *		�{�C���X�^���X�� �L���iIDevice�������Ă���j���A�m�F���܂��B
		 *	@retval true �L���B�e���\�b�h�͎g���܂��B
		 *	@retval false �����B
		 */
		bool IsValid(void) const
		{
			return ! m_hpDevice.IsNull();
		}

		/**
		 * [�擾] �G���[�R�[�h�擾.
		 *	@return �G���[���B���l�̓V�X�e���G���[�R�[�h�ł�( SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
		 */
		DWORD GetLastError(void)
		{
			return m_hpDevice.IsNull() ? 0 : m_hpDevice->GetLastError();
		}

		/**
		 * [�擾] IDevice�Q��.
		 *		IDevice�̃|�C���^�𓾂邱�Ƃ��o���܂��B���ڑ���ł��܂��B
		 *	@return IDevice �̃|�C���^�B
		 */
		PRIBUR::IDevice* operator->(void)
		{
			return m_hpDevice;
		}

		/**
		 * [�擾] SCSI�C���^�[�t�F�[�X�擾.
		 *		SCSI����p�̃N���X�𓾂邱�Ƃ��o���܂��B
		 *	@return CScsi�B
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
		 * [�m�F] ���f�B�A�m�F
		 *	@param dwTime �^�C���A�E�g����(�P��ms)�B�ȗ�����ƂOms�ł��B
		 *	@retval ture ����OK
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
		 * [�擾] ���f�B�A�󂫗e��
		 *	@note BLOCK �P�ʂ̏ꍇ�A IDevice.GetMediaFreeSpace() ���g���܂��B
		 *	@note �{���\�b�h�́A��L�֐��̌��ʂ� CDROM_BLOCKSIZE(2048)�������Ă��邾���ł��B
		 *	@note IsMediaReady() �� true �ɂȂ��Ă���g�p���܂��B
		 *	@return �e�ʁiByte�j�B
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
		 * [�m�F] �����݉\�m�F
		 *	@note �����݉\�̃��f�B�A�������Ă��邩�m�F���܂��B
		 *	@note IsMediaReady() �� true �ɂȂ��Ă���g�p���܂��B
		 *	@retval true �\�B Burn �n ���\�b�h���g�p�\�ł��B
		 *	@retval false �����B
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
		 * [�m�F] �ď����݉\�m�F
		 *	@note �ď����݉\�̃��f�B�A(CD-RW/DVD�}RW)�������Ă��邩�m�F���܂��B
		 *	@note IsMediaReady() �� true �ɂȂ��Ă���g�p���܂��B
		 *	@retval true �\�B�t�H�[�}�b�g��C���[�X���\�ł��B
		 *	@retval false �����B
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
		 * [�m�F] ���f�B�A��m�F
		 *	@note ���ݑ}������Ă��郁�f�B�A���Ԃ��܂��B 
		 *	@note IsMediaReady() �� true �ɂȂ��Ă���g�p���܂��B
		 *	@note MP_BD�` �̃V���{���́A PrimoBurner 2.0�ȍ~���g�p���Ă���ꍇ�̂ݎg�p�ł��܂��B
		 *	@retval MP_UNKNOWN �G���[�B���邢�̓��f�B�A�Ȃ��B���邢�͒m��Ȃ����f�B�A�B
		 *	@retval MP_CD_ROM				CD-ROM -- Read only CD�B
		 *	@retval MP_CD_R					CD-R -- Write once CD�B
		 *	@retval MP_CD_RW				CD-RW -- Re-writable CD�B
		 *	@retval MP_DVD_ROM				DVD-ROM -- Read only DVD�B 
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
		 * [�擾] ���f�B�A�햼�擾
		 *	@param e ���f�B�A��R�[�h�B
		 *	@return ���f�B�A�햼�B
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
		 * [�擾] �����ݑ��x�ꗗ.
		 *		���̃f�o�C�X�ŉ\�� BYTE�P�ʂ̑��x�̈ꗗ���擾�ł��܂��B
		 *	@return ���x�z��B
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
		 * [�擾] �ŏI�g���b�N�擾.
		 *	@note IsMediaReady() �� true �ɂȂ��Ă���g�p���܂��B
		 *	@retval 0�ȏ� �ŏI�g���b�N�B
		 *	@retval -1 �G���[�B
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
		 * [�擾] �P�~���x�擾
		 *	@param boIsDvd true �Ȃ� DVD�Afalse�Ȃ�CD-R�ł�1�{���̏����ݑ��x��Ԃ��܂��B
		 *	@return BYTE�P�ʂ̑��x�B
		 */
		static double Get1xSpeed(bool boIsDvd)
		{
			return boIsDvd ? SPEED_DVD_1X_KB : SPEED_CD_1X_KB;
		}
		
		/**
		 * [�擾] ���x�擾
		 *	@note SetWriteSpeed() �Őݒ肵���l�i�ݒ肵�Ă��Ȃ��ꍇ�A�f�o�C�X�̍ō����j��Ԃ��܂��B
		 *	@note IsMediaReady() �� true �ɂȂ��Ă���g�p���܂��B
		 *	@return BYTE�P�ʂ̑��x�B
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
		 * [�ݒ�] �����ݑ��x�ݒ�
		 *	@note �{�N���X�̃��\�b�h�݂̂ɗL���ł��B
		 *	@param dw BYTE�P�ʂ̑��x���w��B
		 */
		void SetWriteSpeed(DWORD dw)
		{
			m_dwWriteSpeed = dw;
		}

		/**
		 * [����] �C���[�X
		 *	@note �C���[�X�ł���̂́ACD-RW���f�B�A�� DVD-RW���f�B�A�����ł��B
		 *	@param eType �C���[�X�^�C�v�B�ȉ�����w�肵�܂��B�ȗ�����ƁAERASE_DISC �ɂȂ�܂��B\n
		 *					ERASE_DISC					Erase the entire disc	\n
		 *					ERASE_MINIMAL				Erase the PMA, 1st session TOC, pregap	\n
		 *					ERASE_LAST_TRACK			Erase last incomplete track	\n
		 *					ERASE_UNRESERVE_LAST_TRACK	Unreserve the last reserved track	\n
		 *					ERASE_UNCLOSE_LAST_TRACK	Erase the tail of a track - re-opens a complete track	\n
		 *					ERASE_UNCLOSE_LAST_SESSION	Reopen the last session	\n
		 *					ERASE_LAST_SESSION			Erase the last session
		 *	@retval ture ����
		 *	@retval false ���s
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
		 * [����] �t�H�[�}�b�g
		 *	@note �t�H�[�}�b�g�ł���̂́ACD-RW DVD-RW DVD+RW ���f�B�A�����ł��B
		 *	@param boIsQuick true �Ȃ�QuickFormat�B�ȗ��� false �Ȃ犮�SFormat�B
		 *						���̐ݒ�́ADVD+RW ���f�B�A�̏ꍇ�A��������܂��B
		 *	@retval ture ����
		 *	@retval false ���s
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
		 * [����] ������(�C���[�W�t�@�C���w��)
		 *	@param lpszFile �C���[�W�t�@�C����
		 *	@retval true ����
		 *	@retval false ���s
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
		 * [����] ������(CDiscImage �w��)
		 *	@param dd CDiscImage �̃C���X�^���X
		 *	@retval true ����
		 *	@retval false ���s
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
		 * ���������p PrimoBurner �R�[���o�b�N�N���X
		 */
		class CCallBacker : public PRIBUR::IDeviceCallback13, public PRIBUR::IDeviceCallback15, public PRIBUR::IDataDiscCallback14
		{
			IListener*	m_iListener;
		public:
			/// �R���X�g���N�^
			CCallBacker(void) : m_iListener(NULL) {}
			/// �f�X�g���N�^
			virtual ~CCallBacker(void) {}
			/// ���X�i�[�Z�b�g
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
			/// Format�i���ʒm
			virtual void OnFormatProgress(double doProgress)
			{
				TRACE1("OnFormatProgress(%f%%)\n", doProgress);
				if ( m_iListener != NULL )
				{
					m_iListener->OnProgress(IListener::FORMAT, static_cast<DWORD>(doProgress) * 100, 10000);
				}
			}
			/// Erase�i���ʒm
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

		/// PRIBUR::IDevice* �p�n���h��
		typedef CPointerHandleBaseT<PRIBUR::IDevice*, TPhReleasePointerT<PRIBUR::IDevice*> > CDeviceHandle;
		CDeviceHandle		m_hpDevice;		///< PRIBUR::IDevice �C���X�^���X�Ǘ�
		DWORD				m_dwWriteSpeed;	///< �����ݑ��x
		bool				m_boIsEject;	///< ���s��AEject����H
		CCallBacker			m_callbacker;	///< ���X�i�Ǘ�

		/**
		 * [����] �C���[�W�t�@�C����������
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

	
	/// �R���X�g���N�^
	CPrimoBurner(void) : m_pEngine(NULL), m_pEnumerator(NULL), m_iDeviceNumber(0)
	{
	}

	/// �f�X�g���N�^
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
	 * [�ݒ�] ������
	 *	@note	�����������܂��B�܂��ACD-R/RW,DVD�}R/RW �f�o�C�X���X�g���쐬���܂��B
	 *			�����f�o�C�X�̏�ԁi���j���ς������A�ēx������R�[������K�v������܂��B
	 *	@param boIsWritableOnly true ���ȗ��Ȃ珑���݉\�f�o�C�X�Bfalse �Ȃ�S�āB
	 *	@retval �}�C�i�X PrimoBurner �̏��������s
	 *	@retval 0 �f�o�C�X���ڑ�����Ă��Ȃ�
	 *	@retval 1�ȏ� �ڑ����Ă��� �f�o�C�X�̐��B
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
	 * [�擾] �f�o�C�X���擾
	 *	@note Initialize() �ő��������f�o�C�X�̐���Ԃ��܂��B
	 *	@return �f�o�C�X��
	 */
	int GetFoundDeviceNumber(void) const
	{
		return m_iDeviceNumber;
	}

	/**
	 * [�擾] �f�o�C�X���擾
	 *	@param iIndex �C���f�b�N�X�B0�ȏ�A GetFoundDeviceNumber() �̖߂�l�����Ŏw�肵�܂��B
	 *	@return �f�o�C�X���B�h���C�u���^�[���擪�ɕt������܂��B
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
	 * [�I��] �Ώۃf�o�C�X�I��
	 *	@param tchLetter �h���C�u���^�[('a'�`'z')
	 *	@retval true �I��
	 *	@retval false ���s
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
	/// Engine�N���X������
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
	/// �ꗗ�N���X�A�N���[�Y
	void m_CloseEnumerator(void)
	{
		if ( m_pEnumerator != NULL) 
		{
			m_pEnumerator->Release();
			m_pEnumerator = NULL;
		}
	}
	PRIBUR::IEngine*			m_pEngine;			///< �G���W���N���X�Ǘ�
	PRIBUR::IDeviceCollection*	m_pEnumerator;		///< �f�o�C�X�ꗗ�N���X�Ǘ�
	CStrVector					m_vDeviceNames;		///< �f�o�C�X���ꗗ
	int							m_iDeviceNumber;	///< �g�p�\�ȃf�o�C�X��
};



}; //TNB



