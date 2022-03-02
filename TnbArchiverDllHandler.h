#pragma once
/**
 *@file
 * �A�[�J�C�oDLL�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include "TnbDynamicFunc.h"



//TNB Library
namespace TNB
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
#ifndef ARC_DECSTRACT
	//[7ZIP]�̒񋟃w�b�_
	#include "other/7-zip32.h"
#endif
#endif //_TnbDOXYGEN



#pragma pack(push, 1)
/**@ingroup ARCHIVEFILE
 * �A�[�J�C�oDLL�p���\����
 *
 *	@par�K�v�t�@�C��
 *			TnbArchiverDllHandler.h
 *
 *	@date 06/12/14 �V�K�쐬
 */
struct TIndividualInfo
{
	DWORD	dwOriginalSize;			///< �t�@�C���̃T�C�Y
 	DWORD	dwCompressedSize;		///< ���k��̃T�C�Y
	DWORD	dwCRC;					///< �i�[�t�@�C���̃`�F�b�N�T��
	UINT	uFlag;					///< ��������
	UINT	uOSType;				///< ���ɍ쐬�Ɏg��ꂽ OS
	WORD	wRatio;					///< ���k��
	WORD	wDate;					///< �i�[�t�@�C���̓��t(DOS �`��)
	WORD 	wTime;					///< �i�[�t�@�C���̎���(�V)
	char	szFileName[FNAME_MAX32 + 1];	///< �i�[�t�@�C����
	char	dummy1[3];				///< �_�~�[
	char	szAttribute[8];			///< �i�[�t�@�C���̑���(���ɌŗL)
	char	szMode[8];				///< �i�[�t�@�C���̊i�[���[�h(�V)
	#ifndef _UNICODE
	/// �R���X�g���N�^
	TIndividualInfo(void)
	{
		ASSERT0( sizeof(TNB::TIndividualInfo) == sizeof(INDIVIDUALINFO), "TIndividualInfo()",
					"���[�J���Ő錾���Ă���^(TNB::TIndividualInfo)��\n"
					"�C���N���[�h���Ă���^(INDIVIDUALINFO)�ō��ق�����܂��B\n");
	}
	#endif
};
#pragma pack(pop)



/**@ingroup ARCHIVEFILE
 * �A�[�J�C�oDLL�Ǘ��N���X
 *
 *		�A�[�J�C�oDLL�� Load/Free�A�֐��R�[����e�Ղɂ��邽�߂̃N���X�ł��B
 *
 *	@throw CNotSupportException �T�|�[�g���Ă��Ȃ��֐������s����ƁA�X���[����܂��B
 *	@throw CNullPointerException �w���DLL���Ȃ��ꍇ�A�X���[����܂��B
 *
 *	@note UNICODE �v���W�F�N�g�ł� OEM�R�[�h�œ����܂��B
 *
 *	@see http://www.csdinc.co.jp/archiver/
 *
 *	@see http://www.geocities.jp/matsuhp/pc/archive.html
 *
 *	@par�K�v�t�@�C��
 *			TnbArchiverDllHandler.h
 *
 *	@date 06/12/12 �V�K�쐬
 *	@date 10/10/04 �\�����P
 */
class CArchiverDllHandler : CCopyImpossible
{
public:

	/// �R�}���h�� ID
	enum ECommand
	{
		EC_Delete,		///< �폜
		EC_Extract,		///< ���o
		EC_ExtractDir,	///< ���o(Directory�t��)
		EC_Attract,		///< �i�[
	};

	/**
	 * �R���X�g���N�^
	 *	@param lpszDllName �A�[�J�C�o��DLL���B
	 *	@param lpszPrifix �A�[�J�C�o��̃v���t�B�b�N�X�B
	 */
	CArchiverDllHandler(LPCSTR lpszDllName, LPCSTR lpszPrifix)
		: m_hModule(NULL), m_strPrifix(lpszPrifix), m_hWnd(NULL)
	{
		m_Init(lpszDllName);
	}

	/// �f�X�g���N�^
	virtual ~CArchiverDllHandler(void)
	{
		if ( m_hModule != NULL )
		{
			::FreeLibrary(m_hModule);
			m_hModule = NULL;
		}
	}

	/**
	 * [�ݒ�] �֘A Window�n���h��
	 *	@param hWnd	�A�[�J�C�odll ���Ăяo���A�v���̃E�B���h�E�n���h���B
	 *				�E�B���h�E�����݂��Ȃ��R���\�[���A�v���̏ꍇ��A
	 *				�w�肷��K�v�̂Ȃ��ꍇ�� NULL ��n���܂��B
	 */
	void SetParent(HWND hWnd)
	{
		m_hWnd = hWnd;
	}

	 /**
	 * [�m�F] DLL�����m�F
	 *	@retval true ����OK�B
	 *	@retval false DLL��������Ȃ������B���̃��\�b�h���g�p����� throw �����\��������܂��B
	 */
	bool IsReady(void) const
	{
		return m_hModule != NULL;
	}

	/**
	 * [���s] DLL�֐����s
	 *	@param[out] _r �߂�l
	 *	@param[in] lpszFuncName �֐���
	 *	@return r �̎Q��
	 *	@throw CTnbException �w��̊֐����g�p�ł��Ȃ����A�X���[����܂��B
	 */
	template<typename R>
	R Func(R& _r, LPCSTR lpszFuncName)
	{
		R (WINAPI *pFunc0)(void); 
		_r = m_GetFuncAdd(pFunc0, lpszFuncName)();
		return _r;
	}

	/**
	 * [���s] DLL�֐����s
	 *	@param[out] _r �߂�l
	 *	@param[in] lpszFuncName �֐���
	 *	@param[in] t1 ������
	 *	@return r �̎Q��
	 *	@throw CTnbException �w��̊֐����g�p�ł��Ȃ����A�X���[����܂��B
	 */
	template<typename R, typename T1>
	R Func(R& _r, LPCSTR lpszFuncName, T1 t1)
	{
		R (WINAPI *pFunc1)(T1 t1);
		_r = m_GetFuncAdd(pFunc1, lpszFuncName)(t1);
		return _r;
	}

	/**
	 * [���s] DLL�֐����s
	 *	@param[out] _r �߂�l
	 *	@param[in] lpszFuncName �֐���
	 *	@param[in] t1 ������
	 *	@param[in] t2 ������
	 *	@return r �̎Q��
	 *	@throw CTnbException �w��̊֐����g�p�ł��Ȃ����A�X���[����܂��B
	 */
	template<typename R, typename T1, typename T2>
	R Func(R& _r, LPCSTR lpszFuncName, T1 t1, T2 t2)
	{
		R (WINAPI *pFunc2)(T1, T2);
		_r = m_GetFuncAdd(pFunc2, lpszFuncName)(t1, t2);
		return _r;
	}

	/**
	 * [���s] DLL�֐����s
	 *	@param[out] _r �߂�l
	 *	@param[in] lpszFuncName �֐���
	 *	@param[in] t1 ������
	 *	@param[in] t2 ������
	 *	@param[in] t3 ��O����
	 *	@return r �̎Q��
	 *	@throw CTnbException �w��̊֐����g�p�ł��Ȃ����A�X���[����܂��B
	 */
	template<typename R, typename T1, typename T2, typename T3>
	R Func(R& _r, LPCSTR lpszFuncName, T1 t1, T2 t2, T3 t3)
	{
		R (WINAPI *pFunc3)(T1, T2, T3);
		_r = m_GetFuncAdd(pFunc3, lpszFuncName)(t1, t2, t3);
		return _r;
	}

	/**
	 * [���s] DLL�֐����s
	 *	@param[out] _r �߂�l
	 *	@param[in] lpszFuncName �֐���
	 *	@param[in] t1 ������
	 *	@param[in] t2 ������
	 *	@param[in] t3 ��O����
	 *	@param[in] t4 ��l����
	 *	@return r �̎Q��
	 *	@throw CTnbException �w��̊֐����g�p�ł��Ȃ����A�X���[����܂��B
	 */
	template<typename R, typename T1, typename T2, typename T3, typename T4>
	R Func(R& _r, LPCSTR lpszFuncName, T1 t1, T2 t2, T3 t3, T4 t4)
	{
		R (WINAPI *pFunc4)(T1, T2, T3, T4);
		_r = m_GetFuncAdd(pFunc4, lpszFuncName)(t1, t2, t3, t4);
		return _r;
	}

	/**
	 * [�m�F] ���쒆�m�F
	 *	@note	���� �A�[�J�C�oDLL �����쒆���ۂ��𓾂܂��B
	 *			�A�v���P�[�V�������͕K�����k�^�𓀓��̃t�@�C���A�N�Z�X�𔺂�API��
	 *			���s����O�ɂ́A����ɂ����s�\�����`�F�b�N����K�v������܂��B
	 *	@note API��������l�Ȑ��l = ISARC_GET_RUNNING(8)	
	 *	@retval true ���s���ł��B
	 *	@retval false ���s���ł͂Ȃ��i���s�\�j�B
	 */
	bool IsRunning(void)
	{
		BOOL r;
		return !! Func(r, "GetRunning");
	}

	/**
	 * [�擾] ����
	 *	@return ���ʂ̕�����B
	 */
	CAscii GetResultString(void)
	{
		m_strResult.ReleaseBuffer();
		return m_strResult;
	}

	/**
	 * [����] �R�}���h����.
	 *		���k�E�𓀓����s���܂��B
	 *	@note ���ʂ́A GetResultString() �œ��邱�Ƃ��o���܂��B
	 *	@note API��������l�Ȑ��l = ISARC(0)
	 *	@param lpszCmdLine �A�[�J�C�odll �ɓn���R�}���h������B
	 *	@param dwOutputSize �A�[�J�C�odll �����ʂ�Ԃ����߂̃T�C�Y�B
	 *						���ʂ̕�����́A GetResultString() �œ����܂�
	 *	@retval 0 ����I��
	 *	@retval 0�ȊO �G���[����
	 */
	int Command(LPCSTR lpszCmdLine, const DWORD dwOutputSize)
	{
		int r;
		return Func(r, "", m_hWnd, lpszCmdLine, m_strResult.GetBuffer(dwOutputSize), dwOutputSize);
	}

	/**
	 * [�擾] DLL�̃o�[�W�����擾.
	 *		���݂̃o�[�W�����̏�� 2 ���[�h�� 100 ���|�������̂��Ԃ��Ă��܂��B\n
	 *		    ��) 2.30 -> 230
	 *	@note API��������l�Ȑ��l = ISARC_GET_VERSION(1)	
	 *	@return �o�[�W�����B
	 */
	WORD GetVersion(void)
	{
		WORD r;
		return Func(r, "GetVersion");
	}

	/**
	 * [�擾] DLL�̃T�u�o�[�W�����擾.
	 *		���݂̃o�[�W�����̉��� 2 ���[�h�� 100 ���|�������̂��Ԃ��Ă��܂��B\n
	 *			��) 4.01 -> 401
	 *	@note �A�[�J�C�oDLL�ɂ���ăT�|�[�g���Ă��Ȃ��\��������܂��B
	 *	@note API��������l�Ȑ��l = 94	
	 *	@return �T�u�o�[�W�����B
	 */
	WORD GetSubVersion(void)
	{
		WORD r;
		return Func(r, "GetSubVersion");
	}

	/**
	 * [�擾] DLL�̃o�[�W�����擾.	
	 *		������œ��邱�Ƃ��o���܂��B\n
	 *			��) "4.01.00.01"
	 *	@return �o�[�W����������B
	 */
	CStr GetVersionString(void)
	{
		CStr s;
		WORD w = GetVersion();
		try
		{
			WORD v = GetSubVersion();
			s.Format(_T("%d.%02d.%02d.%02d"), w / 100, w % 100, v / 100, v % 100);
		}
		catch(CNotSupportException& e)
		{
			e.OnCatch();
			s.Format(_T("%d.%02d"), w / 100, w % 100);
		}
		return s;
	}

	/**
	 * [�ݒ�] �I�v�V�������
	 *	@note �𓀂̃I�v�V�����w��_�C�A���O�̕\���B
	 *	@note API��������l�Ȑ��l = ISARC_CONFIG_DIALOG(17)	
	 *	@param[out] _lpszOptionBuffer �A�[�J�C�u��ɂ��I�v�V�����B�ȗ������ NULL�B
	 *	@param[in] iMode �A�[�J�C�u��ɂ�郂�[�h�B�ȗ������ 0�B
	 *	@retval ERROR_ALREADY_RUNNING	�A�[�J�C�udll �����Ɏ��s���̎��B
	 *	@retval TRUE					�ݒ肪�����������B
	 *	@retval FALSE					�ݒ肪�o���Ȃ������i����Ȃ������j���B�܂��͎���I�����B
	 */
	DWORD ConfigDialog(LPSTR _lpszOptionBuffer = NULL, const int iMode = 0)
	{
		BOOL r;
		return Func(r, "ConfigDialog", m_hWnd, _lpszOptionBuffer, iMode);
	}

	/**
	 * [�m�F] �t�@�C���`���m�F
	 *	@note	�w��t�@�C�����T�|�[�g���Ă���A�[�J�C�u�t�@�C���Ƃ���
	 *			���������ǂ�����Ԃ��܂��B
	 *	@note API��������l�Ȑ��l = ISARC_CHECK_ARCHIVE(16)
	 *	@param lpszArcFile �`�F�b�N���ׂ��A�[�J�C�u�t�@�C���̃t�@�C�����B
	 *	@param iMode �A�[�J�C�u��ɂ��B�ȗ�����ƂO�B
	 *	@retval true �������A�[�J�C�u�t�@�C���̎��B
	 *	@retval false �t�@�C�����s���̎��B
	 */
	bool CheckArchive(LPCSTR lpszArcFile, const int iMode)
	{
		BOOL r;
		return !! Func(r, "CheckArchive", lpszArcFile, iMode);
	}

	/**
	 * [�m�F] �A�[�J�C�u���m�F
	 *	@note �w�肳�ꂽ���Ƀt�@�C���Ɋi�[����Ă���t�@�C�����𓾂܂��B
	 *	@note API��������l�Ȑ��l = ISARC_GET_FILE_COUNT(18)
	 *	@param lpszArcFile �i�[�t�@�C�����𓾂������Ƀt�@�C�����B
	 *	@retval -1 �G���[�B
	 *	@retval 0�ȏ� �i�[�t�@�C�����B
	 */
	int GetFileCount(LPCSTR lpszArcFile)
	{
		int r;
		return Func(r, "GetFileCount", lpszArcFile);
	}

	/**
	 * [�m�F] �t�@���N�V�����`�F�b�N.
	 *		�w�肳�ꂽ API �� 7-zip32.dll �Ŏg�p�\���ǂ����𓾂܂��B
	 *		�A�v���P�[�V�������͖ړI�̋@�\�iAPI�j���L���ł��邩�ǂ����A
	 *		���O�Ƀ`�F�b�N����悤�ɂ��Ă��������B
	 *	@note API��������l�Ȑ��l = ISARC_QUERY_FUNCTION_LIST(18)
	 *	@param iFunction API ��������ӂȐ��l�B
	 *			��̓I�Ȑ��l�ɂ��Ă� 7-zip32.h �ȂǃA�[�J�C�o�̃w�b�_���Q�Ƃ��ĉ������B
	 *	@retval true �g�p�\�B
	 *	@retval false �g�p�o���Ȃ��A�܂��� iFunction �͖����Ȓl�B
	 */
	bool QueryFunctionList(const int iFunction)
	{
		BOOL r;
		return !! Func(r, "QueryFunctionList", iFunction);
	}

	/**
	 * [����] �A�[�J�C�uOpen.
	 *		�w��t�@�C���� �A�[�J�C�odll �ɗL���ȏ��Ƀt�@�C�����ǂ������ׁA
	 *		�L���ł���΃I�[�v�����A��ӂ̐��l�i�n���h���j��Ԃ��܂��B
	 *		�Ȍ�̏��Ƀt�@�C���ւ̃A�N�Z�X�͂��̃n���h���ł����Ȃ��܂��B
	 *		�I���̍ۂ́A CloseArchive() �ŕ��Ă��������B
	 *	@note API��������l�Ȑ��l = ISARC_OPEN_ARCHIVE(23)
	 *	@param lpszArcFile �A�[�J�C�u�t�@�C�����B
	 *	@param dwMode		�A�[�J�C�o��ɂ��B�ȗ�����ƂO�B
	 *	@retval NULL �G���[�B
	 *	@retval NULL�ȊO �����B
	 */
	HARC OpenArchive(LPCSTR lpszArcFile, const DWORD dwMode = 0)
	{
		HARC r;
		return Func(r, "OpenArchive", m_hWnd, lpszArcFile, dwMode);
	}

	/**
	 * [����] �A�[�J�C�uClose.
	 *		OpenArchive() �Ŋ���t�����n���h�����������B
	 *	@note API��������l�Ȑ��l = ISARC_CLOSE_ARCHIVE(24)
	 *	@param hArc OpenArchive() �ŕԂ��ꂽ�n���h���B
	 *	@retval 0 ����
	 *	@retval -1 �ُ펞
	 */
	int CloseArchive(HARC hArc)
	{
		int r;
		return Func(r, "CloseArchive", hArc);
	}

	/**
	 * [�쐬] �R�}���h������쐬
	 *	@param cmd �R�}���h��B
	 *	@param lpszOption �ǉ��I�v�V�����i�ȗ��\�j�B
	 *	@return �R�}���h�� ��s ���R�܂񂾕�����BEmpty�Ȃ�G���[�B
	 */
	virtual CAscii MakeCommand(ECommand cmd, LPCSTR lpszOption = NULL)
	{
		CAscii strOpt = lpszOption;
		if ( ! strOpt.IsEmpty() )
		{
			strOpt += " ";
		}
		CAscii strResult;
		CAscii strCmd = MakeCommandOption(cmd);
		if ( strCmd.IsEmpty() )
		{
			return strResult;
		}
		strResult.Format("%s%s\042%%s\042 \042%%s\\\042 \042%%s\042", strCmd, strOpt);
		return strResult;
	}

	/**
	 * [�쐬] �R�}���h�I�v�V����������쐬.
	 *	@param cmd �R�}���h��B
	 *	@return �R�}���h��ɑΉ������I�v�V����������BEmpty�Ȃ�G���[�B
	 */
	virtual CAscii MakeCommandOption(ECommand cmd) = 0;

	/**
	 * [���s] �R�}���h���s
	 *	@note ���ʂ́A GetResultString() �œ��邱�Ƃ��o���܂��B
	 *	@param cmd �R�}���h��B�w��ł���͈̂ȉ��̂S�ł��B
	 *				EC_Delete		�폜	\n
	 *				EC_Extract		���o	\n
	 *				EC_ExtractDir	���o(Directory�t��)	\n
	 *				EC_Attract		�i�[	\n
	 *	@param lpszArcFile �ΏۃA�[�J�C�u���B
	 *	@param lpszPath �Ώۃp�X�B
	 *	@param lpszFilter �Ώۃt�@�C��(���C���h�J�[�h�\)�B
	 *	@param lpszOption �ǉ��I�v�V�����i�ȗ��\�j�B
	 *	@retval 0 ����I��
	 *	@retval 0�ȊO �G���[����
	 */
	int Execute(ECommand cmd, LPCSTR lpszArcFile, LPCSTR lpszPath, LPCSTR lpszFilter, LPCSTR lpszOption = NULL)
	{
		CAscii strCmd;
		switch ( cmd )
		{
		case EC_Delete:	// �폜
		case EC_Extract: // ���o
		case EC_ExtractDir: // ���o(Directory�t��)
		case EC_Attract: // �i�[
			strCmd = MakeCommand(cmd, lpszOption);
			break;
		default:
			ASSERT(false);
			break;
		}
		if ( strCmd.IsEmpty() )
		{
			m_strResult.ReleaseBuffer();
			m_strResult.Empty();
			return -1;
		}
		CAscii s = CAscii::Fmt(strCmd, lpszArcFile, lpszPath, lpszFilter);
		s.Replace("\\\\","\\");
		return Command(s, RESULTMAX);
	}

	/**
	 * [���s] �������ɒ��o
	 *	@note ���ʂ́A GetResultString() �œ��邱�Ƃ��o���܂��B
	 *	@param[in] lpszArcFile �ΏۃA�[�J�C�u�t�@�C�����B
	 *	@param[in] lpszName �Ώۃt�@�C���B
	 *	@param[out] _lpBuf ���o�������B
	 *	@param[in,out] dwSize lpBuf �̃T�C�Y�B ���s��͒��o�����T�C�Y���i�[����܂��B
	 *	@param[in] lpszOption �ǉ��I�v�V�����i�ȗ��\�j�B
	 *	@retval 0 ����I��
	 *	@retval 0�ȊO �G���[����
	 */
	int ExtractMem(LPCSTR lpszArcFile, LPCSTR lpszName, BYTE* _lpBuf, size_t& dwSize, LPCSTR lpszOption = NULL)
	{
		ASSERT( ! ::IsBadWritePtr(_lpBuf, dwSize) );
		CAscii strCmd = CAscii::Fmt("%s \042%s\042 \042%s\042", CAscii(lpszOption), lpszArcFile, lpszName);
		strCmd.TrimLeft();
		DWORD dwOriginalSize = 0;
		int (WINAPI* pFunc)(const HWND, LPCSTR, BYTE*, const DWORD, time_t*, WORD*, DWORD*);
		int r = m_GetFuncAdd(pFunc, "ExtractMem")
					(m_hWnd, strCmd, _lpBuf, static_cast<DWORD>(dwSize), NULL, NULL, &dwOriginalSize);
		dwSize = dwOriginalSize;
		return r;
	}

	/**
	 * [���s] ����������i�[
	 *	@note ���ʂ́A GetResultString() �œ��邱�Ƃ��o���܂��B
	 *	@param[in] lpszArcFile �ΏۃA�[�J�C�u�B
	 *	@param[in] lpszName �i�[�t�@�C�����B
	 *	@param[in] time �i�[���ԁB
	 *	@param[in] wAttr �i�[�����B
	 *	@param[in] lpBuf �i�[���e�B
	 *	@param[in,out] _dwSize lpBuf �̃T�C�Y�B������͈��k��̃T�C�Y���i�[����܂��B
	 *	@param[in] lpszOption �ǉ��I�v�V�����i�ȗ��\�j�B
	 *	@retval 0 ����I��
	 *	@retval 0�ȊO �G���[����
	 */
	int CompressMem(LPCSTR lpszArcFile, LPCSTR lpszName, time_t time, WORD wAttr,
										const BYTE* lpBuf, DWORD& _dwSize, LPCSTR lpszOption = NULL)
	{
		ASSERT( ! ::IsBadReadPtr(lpBuf, _dwSize) );
		CAscii strCmd = CAscii::Fmt("%s \042%s\042 \042%s\042", CAscii(lpszOption), lpszArcFile, lpszName);
		strCmd.TrimLeft();
		DWORD dwCompreSize = 0;
		int (WINAPI* pFunc)
					(const HWND, LPCSTR, const BYTE*, const DWORD, const time_t*, const WORD*, DWORD*);
		int r = m_GetFuncAdd(pFunc, "CompressMem")
					(m_hWnd, strCmd, lpBuf, _dwSize, &time, &wAttr, &dwCompreSize);
		_dwSize = dwCompreSize;
		return r;
	}

private:
	enum 
	{
		RESULTMAX = 1024 ///< ���ʎ󂯎��̃T�C�Y 
	};
	// DLL�̓ǂݍ���(�ŏ���1��ڂ̂�)�ƃ��[�N�G���A�̏��������s���܂��B
	bool m_Init(LPCSTR lpszDllName)
	{
		if ( m_hModule != NULL )
		{
			return true;
		}
		HINSTANCE h = ::LoadLibraryA(lpszDllName);
		if ( h == NULL )
		{
			return false;
		}
		// �G�N�X�|�[�g�֐��̊m�F
		bool r = true;
		WORD (WINAPI *pGetVersion)(void);
		r &= GetFuncAddress(pGetVersion, h, m_strPrifix + "GetVersion");
		if ( r )
		{
			m_hModule = h;
		}
		else 
		{
			::FreeLibrary(h);
		}
		return r;
	}
	/**
	 * �A�h���X�擾
	 *	@param[out] _t �擾�A�h���X�i�[
	 *	@param[in]	lpszFuncName �֐���
	 *	@return t �̎Q��
	 */
	template<typename T>
	T& m_GetFuncAdd(T& _t, LPCSTR lpszFuncName)
	{
		if ( m_hModule == NULL )
		{
			throw CNullPointerException();
		}
		if ( ! GetFuncAddress(_t, m_hModule, m_strPrifix + lpszFuncName) )
		{
			throw CNotSupportException(); 
		}
		return _t;
	}
	HMODULE		m_hModule;			///< DLL�̃n���h��
	CAscii		m_strPrifix;		///< �v���t�B�b�N�X
	CAscii		m_strResult;		///< ���ʊi�[�p
	HWND		m_hWnd;				///< �g�pHWND�B
};



/**@ingroup ARCHIVEFILE
 * UnZip32.dll �Ǘ��N���X
 *
 *		UnZip32.dll �� Load/Free�A�֐��R�[����e�Ղɂ��邽�߂̃N���X�ł��B
 *
 *	@note ���s����PC�� UnZip32.dll ���C���X�g�[������Ă���K�v������܂��i IsReady() �ɂĊm�F�\�j�B
 *
 *	@see http://www.csdinc.co.jp/archiver/
 *
 *	@todo ���݉𓀂̂ݑΉ��B
 *
 *	@par�K�v�t�@�C��
 *			TnbArchiverDllHandler.h
 *
 *	@date 18/12/25 �V�K�쐬
 *	@date 19/01/03 �N���X���ύX
 */
class CUnZipDllHandler : public CArchiverDllHandler
{
	DEFSUPER(CArchiverDllHandler);
public:

	/// �R���X�g���N�^
	CUnZipDllHandler(void) : _super("UnZip32.dll", "UnZip")
	{
	}

	/**
	 * [�쐬] �R�}���h�I�v�V����������쐬.
	 *	@param cmd �R�}���h��B
	 *	@return �R�}���h��ɑΉ������I�v�V����������BEmpty�Ȃ�G���[�B
	 */
	virtual CAscii MakeCommandOption(ECommand cmd)
	{
		CAscii strCmd;
		switch ( cmd )
		{
		case EC_Delete:	// �폜
			strCmd = "-d ";
			break;
		case EC_Extract: // ���o
			strCmd = " ";
			break;
		case EC_ExtractDir: // ���o(Directory�t��)
			strCmd = " ";
			break;
		case EC_Attract: // �i�[
			strCmd = "";
			break;
		default:
			ASSERT(false);
			break;
		}
		return strCmd;
	}
};



/**@ingroup ARCHIVEFILE
 * 7-Zip32.dll �Ǘ��N���X
 *
 *		7-Zip32.dll �� Load/Free�A�֐��R�[����e�Ղɂ��邽�߂̃N���X�ł��B
 *
 *	@note ���s����PC�� 7-Zip32.dll ���C���X�g�[������Ă���K�v������܂��i IsReady() �ɂĊm�F�\�j�B
 *
 *	@see http://www.csdinc.co.jp/archiver/
 *
 *	@par�K�v�t�@�C��
 *			TnbArchiverDllHandler.h
 *
 *	@date 06/12/12 �V�K�쐬
 */
class C7zDllHandler : public CArchiverDllHandler
{
	DEFSUPER(CArchiverDllHandler);
public:

	/// �R���X�g���N�^
	C7zDllHandler(void) : _super("7-zip32.dll", "SevenZip")
	{
	}

	/**
	 * [�m�F] �t�@�C���`���m�F
	 *	@param lpszArcFile �A�[�J�C�u�t�@�C����
	 *	@retval ARCHIVETYPE_ZIP		ZIP�`��
	 *	@retval ARCHIVETYPE_7Z		7Z�`��
	 *	@retval 0					7-zip32.dll �ŗ��p�ł��Ȃ��`��
	 *	@retval -1					�G���[
	 */
	int GetArchiveType(LPCSTR lpszArcFile)
	{
		int r;
		return Func(r, "GetArchiveType", lpszArcFile);
	}
	
	/**
	 * [�ݒ�] �g�p�����ݒ�
	 *	@note OEM(���{���� SJIS) �� UTF-8 �̐؂�ւ����\�ł��B
	 *	@param boIsUtf8 true ��ݒ肷��� UTF-8 ���g�p����܂��B
	 *					false ��ݒ肷��� OEM �R�[�h�Z�b�g���g�p����܂��B
	 *	@return ��� true �B
	 */
	bool SetUnicodeMode(bool boIsUtf8)
	{
		BOOL r = boIsUtf8;
		return !! Func(r, "SetUnicodeMode", boIsUtf8);
	}

	/**
	 * [�쐬] �R�}���h�I�v�V����������쐬
	 *	@param cmd �R�}���h��B
	 *	@return �R�}���h��ɑΉ������I�v�V����������BEmpty�Ȃ�G���[�B
	 */
	virtual CAscii MakeCommandOption(ECommand cmd)
	{
		CAscii strCmd;
		switch ( cmd )
		{
		case EC_Delete:	// �폜
			strCmd = "d ";
			break;
		case EC_Extract: // ���o
			strCmd = "e ";
			break;
		case EC_ExtractDir: // ���o(Directory�t��)
			strCmd = "x ";
			break;
		case EC_Attract: // �i�[
			strCmd = "a ";
			break;
		default:
			ASSERT(false);
			break;
		}
		return strCmd;
	}
};



/**@ingroup ARCHIVEFILE
 * UnLha32.dll �Ǘ��N���X
 *
 *		UnLha32.dll �� Load/Free�A�֐��R�[����e�Ղɂ��邽�߂̃N���X�ł��B
 *
 *	@note �g�p����ɂ́A�{�w�b�_������� UnLha32.h �� include ���Ă����K�v������܂��B
 *	@note ���s����PC�� UnLha32.dll ���C���X�g�[������Ă���K�v������܂��i IsReady() �ɂĊm�F�\�j�B
 *
 *	@see http://www.csdinc.co.jp/archiver/
 *
 *	@par�K�v�t�@�C��
 *			TnbArchiverDllHandler.h
 *
 *	@date 06/12/13 �V�K�쐬
 *	@date 10/10/04 �I�v�V�����C��
 */
class CUnLhaDllHandler : public CArchiverDllHandler
{
	DEFSUPER(CArchiverDllHandler);
public:

	/// �R���X�g���N�^
	CUnLhaDllHandler(void) : _super("UnLha32.dll", "Unlha")
	{
	}

	/**
	 * [�쐬] �R�}���h�I�v�V����������쐬
	 *	@param cmd �R�}���h��B
	 *	@return �R�}���h��ɑΉ������I�v�V����������BEmpty�Ȃ�G���[�B
	 */
	virtual CAscii MakeCommandOption(ECommand cmd)
	{
		CAscii strCmd;
		switch ( cmd )
		{
		case EC_Delete:	// �폜
			strCmd = "d ";
			break;
		case EC_Extract: // ���o
			strCmd = "e ";
			break;
		case EC_ExtractDir: // ���o(Directory�t��)
			strCmd = "x ";
			break;
		case EC_Attract: // �i�[
			strCmd = "a -r1 -x1 ";
			break;
		default:
			ASSERT(false);
			break;
		}
		return strCmd;
	}
};



/**@ingroup ARCHIVEFILE
 * Cab32.dll �Ǘ��N���X
 *
 *		Cab32.dll �� Load/Free�A�֐��R�[����e�Ղɂ��邽�߂̃N���X�ł��B
 *
 *	@note �g�p����ɂ́A�{�w�b�_������� UnLha32.h �� include ���Ă����K�v������܂��B
 *	@note ���s����PC�� Cab32.dll ���C���X�g�[������Ă���K�v������܂��i IsReady() �ɂĊm�F�\�j�B
 *
 *	@note �L���r�l�b�g�t�@�C���̎d�l�ł́A��̃t�H���_���i�[�ł��܂���B
 *
 *	@see http://www.csdinc.co.jp/archiver/
 *
 *	@par�K�v�t�@�C��
 *			TnbArchiverDllHandler.h
 *
 *	@date 06/12/13 �V�K�쐬
 *	@date 10/10/04 �I�v�V�����C��
 */
class CCabDllHandler : public CArchiverDllHandler
{
	DEFSUPER(CArchiverDllHandler);
public:

	/// �R���X�g���N�^
	CCabDllHandler(void) : _super("Cab32.dll", "Cab")
	{
	}

	/**
	 * [�쐬] �R�}���h�I�v�V����������쐬
	 *	@param cmd �R�}���h��B
	 *	@return �R�}���h��ɑΉ������I�v�V����������BEmpty�Ȃ�G���[�B
	 */
	virtual CAscii MakeCommandOption(ECommand cmd)
	{
		CAscii strCmd;
		switch ( cmd )
		{
		case EC_Delete:	// �폜		-- ���T�|�[�g
			break;
		case EC_Extract: // ���o
			strCmd = "-x -j";
			break;
		case EC_ExtractDir: // ���o(Directory�t��)
			strCmd = "-x ";
			break;
		case EC_Attract: // �i�[
			strCmd = "-a -r ";
			break;
		default:
			ASSERT(false);
			break;
		}
		return strCmd;
	}
};



/**@ingroup ARCHIVEFILE
 * Tar32.dll �Ǘ��N���X
 *
 *		Tar32.dll �� Load/Free�A�֐��R�[����e�Ղɂ��邽�߂̃N���X�ł��B
 *
 *	@note �g�p����ɂ́A�{�w�b�_������� UnLha32.h �� include ���Ă����K�v������܂��B
 *	@note ���s���� PC �� Tar32.dll ���C���X�g�[������Ă���K�v������܂��i IsReady() �ɂĊm�F�\�j�B
 *
 *	@see http://www.csdinc.co.jp/archiver/
 *
 *	@par�K�v�t�@�C��
 *			TnbArchiverDllHandler.h
 *
 *	@date 10/10/01 �V�K�쐬
 *	@date 10/10/04 �I�v�V�����C��
 */
class CTarDllHandler : public CArchiverDllHandler
{
	DEFSUPER(CArchiverDllHandler);
public:

	/// �R���X�g���N�^
	CTarDllHandler(void) : _super("Tar32.dll", "Tar")
	{
	}

	/**
	 * [�쐬] �R�}���h�I�v�V����������쐬
	 *	@param cmd �R�}���h��B
	 *	@return �R�}���h��ɑΉ������I�v�V����������BEmpty�Ȃ�G���[�B
	 */
	virtual CAscii MakeCommandOption(ECommand cmd)
	{
		CAscii strCmd;
		switch ( cmd )
		{
		case EC_Delete:	// �폜		-- ���T�|�[�g
			break;
		case EC_Extract: // ���o
			strCmd = "-x ";
			break;
		case EC_ExtractDir: // ���o(Directory�t��)
			strCmd = "-xf ";
			break;
		case EC_Attract: // �i�[
			strCmd = "-cf ";
			break;
		default:
			break;
		}
		return strCmd;
	}
};



}; // TNB

#if 0
�A�[�J�C�oDLL 	���k	N���k	�� 	F�� 	�X�V 	���� 	�ꗗ 	SFX 	�ڍ� 	�폜 	���� 	�w�� 
------------------------------------------------------------------------------------------------------------
7-Zip32.DLL 	a		**** 	e		x 		u		t	 	l	 	**** 	**** 	d 		**** 	**** 
UNLHA32.DLL 	a		u		e		x	 	f	 	t	 	l	 	s	 	v	 	d 		c		y 
CAB32.DLL 		a 		**** 	x 		x 		**** 	c,t 	l 		f 		**** 	**** 	**** 	**** 
TAR32.DLL 		-cvf 	**** 	-xvf 	-xvf 	**** 	**** 	-tvf 	**** 	**** 	**** 	**** 	**** 
------------------------------------------------------------------------------------------------------------
UNZIP32.DLL 	**** 	**** 	-x,-xv 	-x,-xv 	**** 	-t 		-l,-lv 	**** 	-v		-d		-z,-Z 	**** 
ZIP32J.DLL 		-r 		-r 		**** 	**** 	-f,-u 	**** 	**** 	-sfx 	**** 	-d 		-z 		**** 
UNARJ32J.DLL 	**** 	**** 	e,ev 	x,xv 	**** 	t 		l,lv 	**** 	v 		**** 	**** 	**** 
UNRAR32.DLL 	**** 	**** 	-e 		-x 		**** 	**** 	-l 		**** 	-v		**** 	**** 	**** 
BGA32.DLL 		a 		**** 	x 		x		**** 	t 		l,lv 	s 		**** 	d 		**** 	**** 
YZ1.DLL 		c 		**** 	x 		x	 	**** 	**** 	**** 	**** 	**** 	**** 	**** 	**** 
UNGCA32.DLL 	**** 	**** 	e,ex 	e,ex 	**** 	t 		l	 	**** 	**** 	**** 	**** 	**** 
UNIMP32.DLL 	**** 	**** 	**** 	-o 		**** 	**** 	**** 	**** 	**** 	**** 	**** 	**** 
UNBEL32.DLL 	�����R�}���h�͕s�v���� 
	���l�FN���k:�V�K�ǉ����k, F��:�t�H���_��, SFX:���ȉ𓀏��ɂ��쐬, �֕�:�w�b�_�ύX, **** �͖��Ή�

#endif