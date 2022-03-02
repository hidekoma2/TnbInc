#pragma once
/**
 *@file
 * TinyMacroLang ���ʊ֌W�̃w�b�_
 *
 *		TML�̌��ʊǗ��̃N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include "TnbStaticMap.h"
#include "TnbGrammarAnalyzer.h"



//TNB Library
namespace TNB
{



/**@ingroup TML
 * TinyMacroLang ���͉��
 *
 *	@par�K�v�t�@�C��
 *			TnbTmlResult.h
 *
 *	@date 06/01/01 �V�K�쐬
 *	@date 09/08/18 ���V
 */
typedef CGrammarAnalyzerT<char> CTmlGrammar;



/**@ingroup TML
 * TinyMacroLang �R�[�h�Ǘ�
 *
 *	@par�K�v�t�@�C��
 *			TnbTmlResult.h
 *
 *	@date 06/01/01 �V�K�쐬
 *	@date 09/08/18 ���V
 */
class CTml
{
public:

	/// �V�[���R�[�h
	enum EScene
	{
		Scene_Non,			///< ����`
		Scene_Val,			///< val��������
		Scene_Array,		///< array��������
		Scene_Func,			///< func��������
		Scene_If,			///< if��������
		Scene_Else,			///< else��������
		Scene_Switch,		///< switch��������
		Scene_Case,			///< case��������
		Scene_Default,		///< default��������
		Scene_While,		///< while��������
		Scene_Break,		///< break��������
		Scene_Return,		///< return��������
		Scene_Include,		///< include��������
		Scene_End,			///< end��������
	};

	/// �I�y���[�V�����R�[�h
	enum EOperation
	{
		Operation_Non,				///< ����`
		Operation_Text,				///< �����񏈗���
		Operation_Text_Esc,			///< ������ ��? ������
		Operation_Text_EscHex,		///< ������ ��x ������
	//	Operation_Text_EscInsert,	///< ������ ��i() ������
		Operation_Calc,				///< �v�Z������
		Operation_CheckFunc,		///< �֐��`�F�b�N��
		Operation_CheckStatement,	///< �X�e�[�g�����g�`�F�b�N��
		Operation_FindingFunc,		///< �֐�������
		Operation_FileAccess,		///< File�A�N�Z�X��
	};

	/// ���U���g�R�[�h
	enum EResult
	{
		Result_Success = 1,					///< ����
		Result_Nop = 0,						///< �����Ȃ�
		Result_UnknownError = -10000,		///< ���̑��̃G���[
		Result_InvalidEscSeq,				///< �G�X�P�[�v�V�[�P���X���ُ�
		Result_InvalidHexNotation,			///< HEX�\�L���ُ�
		Result_NotFoundParentheses,			///< ���ʂ��K�v�ȂƂ���ɂȂ�
		Result_NotCloseParentheses,			///< ���ʂ����Ă��Ȃ�
		Result_InvalidParenthesesPosition,	///< ���ʂ̈ʒu����������
		Result_UnfinishedExpression,		///< �v�Z�����r���ŏI����Ă���
		Result_ZeroDivide,					///< �O�Ŋ��낤�Ƃ��Ă܂�
		Result_UnknownKeyword,				///< �m��Ȃ��L�[���[�h
		Result_InvalidWordPosition,			///< �P��ʒu���ُ�ł��i�����ɂ͑��݂��Ȃ��͂��j
		Result_NotName,						///< ���O������܂���
		Result_AlreadyExistName,			///< ���ɂ��閼�O�ł�
		Result_UnnecessaryParam,			///< �����͕s�K�v�ł�
		Result_ManyParam,					///< �������������܂�
		Result_NotenoughParam,				///< ����������܂���
		Result_OutOfRange,					///< �͈͊O
		Result_InvalidMacroContentsLevel,	///< �}�N���R���e���c���x�����ُ�ł�
		Result_OpenFailure,					///< Open���s
		Result_ReadFailure,					///< Read���s
		Result_SendFailure,					///< Send���s
		Result_InvalidState,				///< ��ԃG���[�i���A���s�ł��܂���)
		Result_NotFoundFunction,			///< �K�v�Ȋ֐�������܂���
		Result_UserInterruption,			///< ���[�U�ɂ�钆�f
		Result_ProgramableInterruption,		///< �v���O�����ɂ�钆�f
	};
};



/**@ingroup TML
 * TinyMacroLang ���ʏ�ԊǗ�
 *
 * ���ʂ��Ǘ�����N���X
 *		�}�C�i�X�́A�G���[�R�[�h�B�v���X�͔C�ӂ̒l��ۊǂł��܂��B
 *		�I�y���[�^int ������̂ŁA�ʏ��int �Ƃ��ăA�N�Z�X�ł��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbTmlResult.h
 *
 *	@date 06/01/01 �V�K�쐬
 *	@date 09/08/18 ���V
 */
class CTmlResult : public CTml
{
public:
	
	/**
	 * �R���X�g���N�^.
	 *		Success �������܂��B
	 */
	CTmlResult(void) : m_eResult(Result_Success), m_eScene(Scene_Non), m_eOperation(Operation_Non), m_iFileLine(0)
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^
	 *	@param other �R�s�[��
	 */
	CTmlResult(const CTmlResult& other) 
		: m_eResult(other.m_eResult), m_eScene(other.m_eScene), m_eOperation(other.m_eOperation), m_iFileLine(other.m_iFileLine)
		, m_strFuncName(other.m_strFuncName), m_strToken(other.m_strToken), m_strFileName(other.m_strFileName)
	{
	}

	/**
	 * ����R���X�g���N�^.
	 *		�I�y���[�V���������ʑ���B�V�[���R�[�h�v�u�G���[�\�[�X�ʒu�v�u�G���[���e�v�u�����֐����v�͖����ł��B
	 *	@param eOperation	�I�y���[�V�����R�[�h
	 *	@param eResult		���U���g�R�[�h
	 */
	CTmlResult(EOperation eOperation, EResult eResult)
		: m_eResult(eResult), m_eScene(Scene_Non), m_eOperation(eOperation), m_iFileLine(0)
	{
	}

	/** 
	 * ����R���X�g���N�^.
	 *		�V�[�����I�y���[�V���������ʑ���B�u�G���[�\�[�X�ʒu�v�u�G���[���e�v�u�����֐����v�͖����ł��B
	 *	@param eScene		�V�[���R�[�h
	 *	@param eOperation	�I�y���[�V�����R�[�h
	 *	@param eResult		���U���g�R�[�h
	 */
	CTmlResult(EScene eScene, EOperation eOperation, EResult eResult)
		: m_eResult(eResult), m_eScene(eScene), m_eOperation(eOperation), m_iFileLine(0)
	{
	}

	/**
	 * ����R���X�g���N�^.
	 *		���ʑ���B�u�I�y���[�V�����R�[�h�v�u�V�[���R�[�h�v
	 *		�u�G���[�\�[�X�ʒu�v�u�G���[���e�v�u�����֐����v�͖����ł��B
	 *	@param i ���ʂ� int ���A���U���g�R�[�h( EResult )
	 */
	CTmlResult(EResult i) : m_eResult(i), m_eScene(Scene_Non), m_eOperation(Operation_Non), m_iFileLine(0)
	{
	}

	/**
	 * �R�s�[�I�y���[�^
	 *	@param other �R�s�[��
	 *	@return �����̎Q��.
	 */
	CTmlResult& operator=(const CTmlResult& other) 
	{
		m_eResult = other.m_eResult;
		m_eScene = other.m_eScene;
		m_eOperation = other.m_eOperation;
		m_iFileLine = other.m_iFileLine;
		m_strFuncName = other.m_strFuncName;
		m_strToken = other.m_strToken;
		m_strFileName = other.m_strFileName;
		return *this;
	}

	/**
	 * [���] int�^�l���.
	 *		���ʑ���B
	 *		�u�I�y���[�V�����R�[�h�v�u�V�[���R�[�h�v
	 *		�u�G���[�\�[�X�ʒu�v�u�G���[���e�v�u�����֐����v�͖����ł��B
	 *	@param i ���ʂ� int ���A���U���g�R�[�h( EResult )
	 *	@return �����̎Q��
	 */
	CTmlResult& operator=(EResult i)
	{
		*this = CTmlResult();
		m_eResult = i;
		return *this;
	}

	/**
	 * [�ݒ�] NOP�ݒ�.
	 *		���ʂ� NOP�ɂ��܂��B
	 */
	void SetNop(void)
	{
		*this = CTmlResult();
		m_eResult = Result_Nop;
	}

	/**
	 * [�ݒ�] SUCCESS�ݒ�.
	 *		���ʂ� SUCCESS�ɂ��܂��B
	 */
	void SetSuccess(void)
	{
		*this = CTmlResult();
		m_eResult = Result_Success;
	}

	/**
	 * [�m�F] SUCCESS �m�F.
	 *	@retval true Success
	 *	@retval false Nop �� �G���[�B
	 */
	bool IsSuccess(void) const
	{
		return m_eResult == Result_Success;
	}

	/**
	 * [�m�F] ERROR �m�F.
	 *	@retval true �G���[
	 *	@retval false Nop �� Success�B
	 */
	bool IsError(void) const
	{
		return m_eResult < 0;
	}

	/**
	 * [�m�F] NOP �m�F.
	 *	@retval true Nop
	 *	@retval false Success �� �G���[�B
	 */
	bool IsNop(void) const
	{
		return m_eResult == Result_Nop;
	}

	/**
	 * [�ݒ�] �V�[���R�[�h�ݒ�
	 *	@param eScene �V�[���R�[�h
	 */
	void SetScene(EScene eScene)
	{
		m_eScene = eScene;
	}
	
	/**
	 * [�ݒ�] �I�y���[�V�����R�[�h�ݒ�
	 *	@param eOperation �I�y���[�V�����R�[�h
	 */
	void SetOperation(EOperation eOperation)
	{
		m_eOperation = eOperation;
	}

	/**
	 * [�ݒ�] �G���[�����֐����ݒ�
	 *	@param lpszFunc �G���[�����֐���
	 */
	void SetFuncName(LPCSTR lpszFunc)
	{
		m_strFuncName = lpszFunc;
	}

	/**
	 * [�ݒ�]�@�G���[���e�ݒ�
	 *	@param lpszString �G���[���e
	 */
	void SetErrorString(LPCSTR lpszString)
	{
		m_strToken = lpszString;
	}

	/**
	 * [�ݒ�] �G���[�\�[�X�t�@�C�����A�s���ݒ�
	 *	@param lpszFileName �G���[�\�[�X�t�@�C����
	 *	@param iFileLine �G���[�����s�ԍ�
	 *	@return �����̎Q�ƁB
	 */
	CTmlResult& SetLine(LPCSTR lpszFileName, INT_PTR iFileLine)
	{
		m_iFileLine		= iFileLine;
		m_strFileName	= lpszFileName;
		return *this;
	}
	
	/**
	 * [�ݒ�] �G���[�\�[�X�t�@�C�����A�s���ݒ�
	 *	@param gram �������̓N���X
	 *	@return �����̎Q�ƁB
	 */
	CTmlResult& SetLine(const CTmlGrammar& gram)
	{
		m_iFileLine		= gram.GetLine();
		m_strFileName	= gram.GetName();
		return *this;
	}

	/**
	 * [�擾] ���U���g�R�[�h�擾.
	 *	@return ���U���g�R�[�h
	 */
	EResult GetResult(void) const
	{
		return m_eResult;
	}

	/**
	 * [�擾] �V�[���R�[�h�擾
	 *	@return �V�[���R�[�h
	 */
	EScene GetScene(void) const
	{
		return m_eScene;
	}

	/**
	 * [�擾] �I�y���[�V�����R�[�h�擾
	 *	@return �I�y���[�V�����R�[�h
	 */
	EOperation GetOperation(void) const
	{
		return m_eOperation;
	}

	/**
	 * [�擾] �G���[�����֐����擾
	 *	@return �G���[�����֐���
	 */
	LPCSTR GetFuncName(void) const
	{
		return m_strFuncName;
	}

	/**
	 * [�擾] �G���[���e�擾
	 *	@return �G���[���e
	 */
	LPCSTR GetErrorString(void) const
	{
		return m_strToken;
	}

	/**
	 * ��擾� �G���[�\�[�X�s�ԍ��擾
	 *	@return �G���[�\�[�X�s�ԍ�
	 */
	INT_PTR GetFileLine(void) const 
	{
		return m_iFileLine;
	}

	/**
	 * [�擾] �G���[�\�[�X�t�@�C�����擾
	 *	@return �G���[�\�[�X�t�@�C����
	 */
	LPCSTR GetFileName(void) const
	{
		return m_strFileName;
	}

	/**
	 * [�擾] ������擾.
	 *	@note �{�C���X�^���X���ێ����Ă�����e����A������𐶐����܂��B
	 *			�������G���[�R�[�h��0�ȏ�̏ꍇ�A"" ��Ԃ��܂��B
	 *	@return ������  \n
	 *		ex)
	 *			"�G���[�\�[�X��(�s): �֐��� �V�[�� �I�y���[�V���� ���U���g �G���[���e"
	 */
	CAscii ToString(void)
	{
		if ( ! IsError() )
		{
			return "";
		}
		//
		CAscii strFile;
		if ( ! m_strFileName.IsEmpty() )
		{
			strFile.Format("%s(%d):", m_strFileName, m_iFileLine);
		}
		//
		CAscii strFunc = GetFuncName();
		if ( ! strFunc.IsEmpty() )
		{
			strFunc = strFunc.Sandwich(" FuncName=[", "]");
		}
		//
		CAscii strScene = SceneToString(m_eScene);
		if ( ! strScene.IsEmpty() )
		{
			strScene = strScene.Sandwich(" Scene=[", "]");
		}
		//
		CAscii strOperation = OperationToString(m_eOperation);
		if ( ! strOperation.IsEmpty() )
		{
			strOperation = strOperation.Sandwich(" Operation=[", "]");
		}
		//
		CAscii strResult = ResultToString(m_eResult);
		if ( ! strResult.IsEmpty() )
		{
			strResult = strResult.Sandwich(" Result=[", "]");
		}
		//
		CAscii strError = GetErrorString();
		if ( ! strError.IsEmpty() )
		{
			strError = strError.Sandwich(" / [", "]");
		}
		//
		return strFile + strFunc + strScene + strOperation + strResult + strError;
	}

	/**
	 * [�m�F] �p�����[�^�̐��ƁA���҂̐��̃`�F�b�N.
	 *	@param iParamNum ���ۂ̃p�����[�^�̐��B
	 *	@param iExpectNum ���҂���p�����[�^��
	 *	@return ���ʁB
	 */
	static CTmlResult CheckParamNum(INT_PTR iParamNum, INT_PTR iExpectNum)
	{
		CTmlResult r;
		if ( iExpectNum == 0 && iParamNum > 0 )
		{
			//�����͕s�K�v�ł��B
			r = CTmlResult(Operation_CheckFunc, Result_UnnecessaryParam);
		}
		else if ( iParamNum < iExpectNum )
		{
			//����������܂���B
			r = CTmlResult(Operation_CheckFunc, Result_NotenoughParam);
		}
		else if ( iParamNum > iExpectNum )
		{
			//�������������܂��B
			r = CTmlResult(Operation_CheckFunc, Result_ManyParam);
		}
		return r;
	}

	/**
	 * [�ϊ�] �V�[���R�[�h��������
	 *	@param eScene �V�[���R�[�h
	 *	@return ������
	 */
	static LPCSTR SceneToString(EScene eScene)
	{
		const static CStaticMapT<EScene, LPCSTR> t[] =
		{
			Scene_Non,			"",
			Scene_Val,			"val��������",
			Scene_Array,		"array��������",
			Scene_Func,			"func��������",
			Scene_If,			"if��������",
			Scene_Else,			"else��������",
			Scene_Switch,		"switch��������",
			Scene_Case,			"case��������",
			Scene_Default,		"default��������",
			Scene_While,		"while��������",
			Scene_Break,		"break��������",
			Scene_Return,		"return��������",
			Scene_Include,		"include��������",
			Scene_End,			"end��������",
		};
		return CStaticMapT<EScene, LPCSTR>::Find(eScene, t);
	}

	/**
	 * [�ϊ�] �I�y���[�V�����R�[�h��������
	 *	@param eOperation �I�y���[�V�����R�[�h
	 *	@return ������
	 */
	static LPCSTR OperationToString(EOperation eOperation)
	{
		const static CStaticMapT<EOperation, LPCSTR> t[] =
		{
			Operation_Non,				"",
			Operation_Text,				"�����񏈗���",
			Operation_Text_Esc,			"������ \\? ������",
			Operation_Text_EscHex,		"������ \\x ������",
//			Operation_Text_EscInsert,	"������ \\i() ������",
			Operation_Calc,				"�v�Z������",
			Operation_CheckFunc,		"�֐��`�F�b�N��",
			Operation_CheckStatement,	"�X�e�[�g�����g�`�F�b�N��",
			Operation_FindingFunc,		"�֐�������",
			Operation_FileAccess,		"File�A�N�Z�X��",
		};
		return CStaticMapT<EOperation, LPCSTR>::Find(eOperation, t);
	}
	
	/**
	 * [�ϊ�] ���U���g�R�[�h��������
	 *	@param result ���U���g�R�[�h
	 *	@return ������
	 */
	static LPCSTR ResultToString(INT_PTR result)
	{
		const static CStaticMapT<INT_PTR, LPCSTR> t[] =
		{
			Result_Success,						"",
			Result_UnknownError,				"���̑��̃G���[",
			Result_InvalidEscSeq,				"�G�X�P�[�v�V�[�P���X���ُ�",
			Result_InvalidHexNotation,			"HEX�\�L���ُ�",
			Result_NotFoundParentheses,			"���ʂ��K�v�ȂƂ���ɂȂ�",
			Result_NotCloseParentheses,			"���ʂ����Ă��Ȃ�",
			Result_InvalidParenthesesPosition,	"���ʂ̈ʒu����������",
			Result_UnfinishedExpression,		"�v�Z�����r���ŏI����Ă���",
			Result_ZeroDivide,					"�O�Ŋ��낤�Ƃ��Ă܂�",
			Result_UnknownKeyword,				"�m��Ȃ��L�[���[�h",
			Result_InvalidWordPosition,			"�P��ʒu���ُ�ł��i�����ɂ͑��݂��Ȃ��͂��j",
			Result_NotName,						"���O������܂���",
			Result_AlreadyExistName,			"���ɂ��閼�O�ł�",
			Result_UnnecessaryParam,			"�����͕s�K�v�ł�",
			Result_ManyParam,					"�������������܂�",
			Result_NotenoughParam,				"����������܂���",
			Result_OutOfRange,					"�͈͊O�ł�",
			Result_InvalidMacroContentsLevel,	"�}�N���R���e���c���x�����ُ�",
			Result_OpenFailure,					"Open���s",
			Result_ReadFailure,					"Read���s",
			Result_SendFailure,					"Send���s",
			Result_InvalidState,				"��ԃG���[�i���A���s�ł��܂���j",
			Result_NotFoundFunction,			"�K�v�Ȋ֐�������܂���",
			Result_UserInterruption,			"���f",
			Result_ProgramableInterruption,		"�I��",
		};
		return CStaticMapT<INT_PTR, LPCSTR>::Find(result, t);
	}

private:
	EResult		m_eResult;		///< ���U���g�R�[�h
	EScene		m_eScene;		///< �G���[�̃V�[��
	EOperation	m_eOperation;	///< �G���[�̃I�y���[�V����
	CAscii		m_strFuncName;	///< �G���[�����֐���
	CAscii		m_strToken;		///< �G���[���e
	CAscii		m_strFileName;	///< �G���[�\�[�X�t�@�C���� (�G���[�\�[�X�ʒu)
	INT_PTR		m_iFileLine;	///< �G���[�\�[�X�s�ԍ� (�G���[�\�[�X�ʒu)
};



}; // TNB
