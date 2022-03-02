#pragma once
/**
 *@file
 * TinyMacroLang �ϐ��Ǘ��֌W�̃w�b�_
 *	
 *		TML�̕ϐ��Ǘ��̃N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbFilePathManager.h"
#include "TnbTmlCalculator.h"
#include "TnbTmlDocuments.h"
#include "TnbTmlVariableManager.h"



//TNB Library
namespace TNB
{



/**@defgroup TML	TinyMacroLang
 *
 *	\par
 *		�}�N������������N���X������܂��B
 *
 *		�֐���`���g�����邽�߂ɁA ITmlExpander �����������N���X�쐬���A
 *		CTmlParser �ɓn���܂�( CTmlParser::SetExpander() )�B
 *		���s�i���́A CTmlParser::IListener �����������N���X���쐬���A
 *		CTmlParser �ɓn���܂�( CTmlParser::SetListener() )�B
 */



/**@ingroup TML
 * TinyMacroLang �p�[�T�[
 *	
 *	@par�K�v�t�@�C��
 *			TnbTmlParser.h
 *
 *	@date 06/01/01 �V�K�쐬
 *	@date 09/08/18 ���V
 */
class CTmlParser : public CTml, ITmlExpander
{
public:

	/**@ingroup TML
	 * TinyMacroLang ���X�i�[
	 *
	 *	@see SetListener()
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbTmlParser.h
	 *
	 *	@date 06/01/01 �V�K�쐬
	 *	@date 09/08/18 ���V
	 */
	struct IListener
	{
		/// �f�X�g���N�^
		virtual ~IListener(void) {}

		/**
		 * [�ʒm] �p���m�F�ʒm.
		 *		�����𑱂��Ă悢���₢���킹�̒ʒm�ł��B
		 *	@note �����ɉ�����Ԃ��Ȃ����ƂŁAPause�I�ȓ����������ł��܂��B
		 *		���~�Ȃ珈���̌��ʂ� ETR_UserInterruption �ɂȂ�܂��B
		 *	@retval true �������s�B
		 *	@retval false ���f�v���B
		 */
		virtual bool IsExecutionContinued(void) = 0;

		/**
		 * [�ʒm] ���O�o�͒ʒm.
		 *		LOG�ɏo�͂�����e��ʒm���܂��B
		 *	@param lpszLog ���e
		 *	@param boHasErrMsg �G���[���b�Z�[�W�Ȃ�true
		 */
		virtual void OutputLog(LPCTSTR lpszLog, bool boHasErrMsg) = 0;
	};

	/**
	 * LOG�\�����x��.
	 *	@see SetLogLevel
	 */
	enum ELogLevel
	{
		ELL_ABSOLUTE,		///< ���
		ELL_FUNC,			///< �֐�/����
		ELL_BRANCH,			///< ����
		ELL_DETAILS,		///< �ڍ�
	};


	//--------------------------------


	/// �R���X�g���N�^ 
	CTmlParser(void) : m_piListener(NULL), m_piExpander(NULL), m_eLogLevel(ELL_ABSOLUTE), m_iLogLastLine(0)
	{
		m_calc.SetExpander(this);
		m_variable.IncLevel();
		m_documents.IncLevel();
	}

	/// �f�X�g���N�^
	~CTmlParser(void)
	{
	}

	/** 
	 * [�ݒ�] �e�X�g���[�h�ݒ�/����.
	 *	@note �e�X�g���[�h�����@�`�F�b�N���[�h�ł��B���̃��[�h�Ŏ��s����ƁA
	 *			�S�X�e�[�g�����g��true�Ƃ��Ď��s����܂��B�܂�A�S�������A
	 *			���s���A���@�̃`�F�b�N���s���܂��B
	 *	@param boIsTestMode true�Ȃ�e�X�g���[�h�B
	 */
	void SetTestMode(bool boIsTestMode)
	{
		m_calc.SetTestMode(boIsTestMode);
	}

	/**
	 * [�擾] �e�X�g���[�h��Ԏ擾
	 *	@retval true �e�X�g���[�h���B
	 *	@retval false �ʏ�B
	 */
	bool IsTestMode(void) const
	{
		return m_calc.IsTestMode();
	}

	/**
	 * [�ݒ�] �N���A.
	 *		�����Ă���R���e���c�A�ϐ����e���N���A���A���x�����P�ɂ��܂��B
	 */
	void Empty(void)
	{
//		::PlaySound(NULL, NULL, 0);
		m_documents.Empty();
		m_documents.IncLevel();
		m_variable.Empty();
		m_variable.IncLevel();
		m_cInterruptInfo.Empty();
		m_strLogLastName.Empty();
		m_iLogLastLine = 0;
	}

	/**
	 * [�ݒ�] ���X�i�[�p�C���^�[�t�F�C�X�ݒ�.
	 *	@param piListener ���X�i�[�̎Q�ƁB�ȗ�����Ɖ����ɂȂ�܂��B
	 */
	void SetListener(IListener* piListener = NULL)
	{
		m_piListener = piListener;
	}

	/**
	 * [�ݒ�] �g���֐��̃C���^�[�t�F�C�X�ݒ�.
	 *	@param piExpander �g���I�u�W�F�N�g�̎Q�ƁB�ȗ�����Ɖ����ɂȂ�܂��B
	 */
	void SetExpander(ITmlExpander* piExpander = NULL)
	{
		m_piExpander = piExpander;
	}

	/** 
	 * [�ݒ�] ���O���x���A�ݒ�.
	 *	@param eLevel ���x���B
	 */
	void SetLogLevel(ELogLevel eLevel)
	{
		m_eLogLevel = eLevel;
	}

	/**
	 * [�ǉ�] �}�N���t�@�C���̒ǉ�.
	 *		�ϐ����x���Ƥ�R���e���c���x���́A�ύX�����g�p����܂��B
	 *	@param lpszName �t�@�C����
	 *	@param lpszPath �p�X���i�ȗ��Ȃ�A�v���Z�X�p�X����̑��΂ɂȂ�܂��j
	 *	@return ���ʁB
	 */
	CTmlResult AddMacroFile(LPCTSTR lpszName, LPCTSTR lpszPath = NULL)
	{
		CTmlResult trRet;
		//
		CFilePathManager pathMan;
		CFileReader reader = pathMan.Open(lpszName, lpszPath);
		if ( reader.CanRead() )
		{
			try
			{
				CByteVector vc = reader.ReadExactly();
				vc.Add(0);
				return AddMacroContents(reader.GetOpenName(), reinterpret_cast<LPCSTR>(vc.ReferBuffer()));
			}
			catch ( CTnbException& e )
			{
				e;
				trRet = CTmlResult(Operation_FileAccess, Result_ReadFailure);
			}
		}
		else
		{
			trRet = CTmlResult(Operation_FileAccess, Result_OpenFailure);
		}
		m_MessageBug( "�t�@�C��'%s'���ǂݍ��߂܂���B", CAscii(lpszName));
		reader.Close();
		return trRet;
	}
	
	/**
	 * [�ǉ�] �}�N���R���e���c�̒ǉ�.
	 *		�ϐ����x���Ƥ�R���e���c���x���́A�ύX�����g�p����܂��B
	 *	@param lpszName �t�@�C����(�o�^�p)
	 *	@param lpszData �R���e���c���e
	 *	@return ���ʁB
	 */
	CTmlResult AddMacroContents(LPCTSTR lpszName, LPCSTR lpszData)
	{
		CTmlDocuments::TFuncInfoRes tFunc;
		CTmlResult res = m_documents.Add(tFunc, CAscii(lpszName), lpszData);
		if ( res.IsError() )
		{
			return res;
		}
		else if ( res.IsNop() )
		{
			return Result_Success;	//�����t�@�C����ǂݍ���ł����B
		}
		//
		if ( ! tFunc.HasData() )
		{
			ASSERTLIB( false );	//���������A���肦�Ȃ��H�I
			return Result_Nop;
		}
		//������
		CTmlGrammar gram(tFunc.lpszFuncAdds, NULL, tFunc.iFuncLine);
		gram.SetName(CStr(tFunc.strFileName));
		//	
		CTmlValue tvResult;
		return ExecMacro(gram, tvResult);
	}

	/**
	 * [�ݒ�] �}�N�����x���{�P.
	 *		AddMacro�`()�Œǉ����郌�x�����w�肵�܂��B�}�N���ǉ��̑O�Ɂ{�P���Ă����ƁA
	 *		Dec���邾���ŁA�ǉ����ꂽ�}�N����j�����邱�Ƃ��ł��܂��B
	 *		�}�N�����s���ɂ͎��s���Ȃ��ł��������B
	 */
	void IncMacroLevel(void)
	{
		m_documents.IncLevel();
		m_variable.IncLevel();
	}

	/**
	 * [�ݒ�] �}�N�����x���|�P.
	 *		AddMacro�`()�Œǉ����郌�x�����w�肵�܂��B�}�N���ǉ��̑O�Ɂ{�P���Ă����ƁA
	 *		Dec���邾���ŁA�ǉ����ꂽ�}�N����j�����邱�Ƃ��ł��܂��B
	 *		�}�N�����s���ɂ͎��s���Ȃ��ł��������B
	 */
	void DecMacroLevel(void)
	{
		m_variable.DecLevel();
		m_documents.DecLevel();
	}

	/**
	 * [�m�F] �w��֐��̗L���`�F�b�N.
	 *		�w��̊֐����o�^����Ă��邩�B
	 *	@param lpszFuncName ��������֐���
	 *	@retval true �w��̊֐�������B
	 *	@retval false �����B
	 */
	bool ExistFunction(LPCSTR lpszFuncName) const
	{
		CTmlDocuments::TFuncInfoRes tFunc;
		tFunc = m_documents.FindFunc(lpszFuncName);
		return tFunc.HasData();
	}

	/**
	 * [����] �w��֐��̎��s.
	 *		�e�L�X�g�ʒu����ޔ����Ď��s���A�I���㌳�ɖ߂��Ă��܂��B
	 *	@param[out] _answer ���ʂ̒l�B
	 *	@param lpszFuncName �֐���
	 *	@return ����
	 */
	CTmlResult ExecFunction(CTmlValue& _answer, LPCSTR lpszFuncName)
	{
		CTmlValuesVector aValues;
		return ExecFunction(_answer, lpszFuncName, aValues);
	}

	/**
	 * [����] �w��֐��̎��s.
	 *		�e�L�X�g�ʒu����ޔ����Ď��s���A�I���㌳�ɖ߂��Ă��܂��B
	 *	@param[out] _answer ���ʂ̒l�B
	 *	@param lpszFuncName �֐���
	 *	@param aValues ����
	 *	@return ����
	 */
	CTmlResult ExecFunction(CTmlValue& _answer, LPCSTR lpszFuncName, const CTmlValuesVector& aValues)
	{
		CTmlDocuments::TFuncInfoRes tFunc;
		tFunc = m_documents.FindFunc(lpszFuncName);
		if ( ! tFunc.HasData() )
		{
			return Result_Nop;
		}
		//������
		m_variable.IncLevel();
		if ( tFunc.pastrParamNames == NULL )
		{
			//�p�����[�^�Ȃ�
			if ( aValues.GetSize()!=0 )
			{
				//����������̂ŃG���[
				m_variable.DecLevel();
				m_MessageBug("�����̂Ȃ� func '%s'�Ɉ�����n���Ă��܂��B", lpszFuncName);
				return CTmlResult(Operation_CheckFunc, Result_UnnecessaryParam);
			}
		}
		else
		{
			size_t dim = tFunc.pastrParamNames->GetSize();
			if ( dim < aValues.GetSize() )
			{
				//������������
				m_MessageBug("func '%s'(������ %d��)�ɁA%d�̂Ɉ�����n���Ă��܂��B",
							lpszFuncName,
							dim,
							aValues.GetSize()

				);
				m_variable.DecLevel();
				return CTmlResult(Operation_CheckFunc, Result_ManyParam);
			}
			//�����ϐ��A�ǉ�
			for ( size_t i = 0; i < dim; i++ )
			{
				CTmlValue value;
				if ( aValues.GetSize() > i )
				{
					value = aValues.At(i);
				}
				m_variable.AddValue(tFunc.pastrParamNames->At(i), value);
			}
		}
		//
		CTmlGrammar gram(tFunc.lpszFuncAdds, NULL, tFunc.iFuncLine);
		gram.SetName(CStr(tFunc.strFileName));
		CTmlResult res = ExecMacro(gram, _answer);
		m_variable.DecLevel();
		//
		if ( ! res.IsSuccess() )
		{
			if ( CAscii(res.GetFuncName()).IsEmpty() )
			{
				//�Ȃ���ΐݒ�
				res.SetFuncName(lpszFuncName);
			}
		}
		return res;
	}

	/**
	 * [����] �}�N�����s.
	 *		����́����s�B �u���b�N�I�������� NIL�����܂ŏ������܂��B
	 *	@param[out] _answer ���ʂ̒l�B
	 *	@param[in,out] _gram �}�N�����B
	 *	@return ������������ �}�C�i�X�Ȃ�G���[(ETmCTmlResult)
	 */
	CTmlResult ExecMacro(CTmlGrammar& _gram, CTmlValue& _answer)
	{
		LPCSTR pp = _gram.GetPointer();
		IgnoreUnusedValue(pp);
		CTmlGrammar backGram = _gram;
		CTmlGrammar::CParts parts;
		//
		CTmlResult res;
		_answer = 0;
		//
		CNestInfo cNest;
		INT_PTR iStartVarLevel = m_variable.GetLevel();
		INT_PTR iStartContentsLevel = m_documents.GetLevel();
		//
		while ( true )
		{
			backGram = _gram;
			//���荞�݃`�F�b�N
			{
				CTmlResult r = m_CheckInterrupt();
				if ( ! r.IsSuccess() )
				{
					res = r;
					break;
				}
			}
			//
			parts = _gram.PeekNextParts();
			CTmlGrammar::EPartsKind kind = parts.GetKind();
			CAscii token = parts.GetString();
			if ( kind == CTmlGrammar::ERROR_STRING )
			{
				//�G���[
				res = Result_NotCloseParentheses;
				res.SetOperation(Operation_CheckStatement);
				res.SetLine(backGram);
				m_MessageBug(res, "������\�L�ɃG���[������܂��B");
				break;
			}
			else if ( token == ";" )
			{
				_gram.GetNextParts();
				continue;
			}
			else if ( token == "{" )
			{
				_gram.GetNextParts();
				cNest.Inc(cNest.EK_Non);
				m_variable.IncLevel();
				m_documents.IncLevel();
				continue;
			}
			else if ( token == "}" || kind == CTmlGrammar::FINAL )
			{
				if ( cNest.IsTopDepth() )
				{
					m_MessageLog(backGram, ELL_DETAILS, "�[���O�ŕ�����������܂����B�֐����I���܂��B");
					break;
				} 
				else if ( cNest.GetNowKind() == cNest.EK_While )
				{
					m_MessageLog(backGram, ELL_BRANCH, "while�l�X�g��'}'������܂����Bwhile���֖߂�܂��B");
					_gram = cNest.GetWhileInfo();
					cNest.Dec();
					m_variable.DecLevel();
					m_documents.DecLevel();
				}
				else
				{
					_gram.GetNextParts();
					cNest.Dec();
					m_variable.DecLevel();
					m_documents.DecLevel();
				}
				continue;
			}
			//
			backGram = _gram;
			EStatementResultType stateResType;
			CTmlResult r = m_SearchAndExecCommand(_gram, cNest, _answer, stateResType);
			//
			if ( r.IsSuccess() )
			{
				//	�X�e�[�g�����g�����A����
				if ( stateResType == ESRT_Return/*return��*/ )
				{
					TTRACE0(_T("return���Ȃ̂Ŋ֐��I��\n"));
					break;
				}
				else if ( stateResType == ESRT_Function/*func��*/ )
				{
					if ( cNest.IsTopDepth() )
					{
						m_MessageLog(_gram, ELL_DETAILS, "�[���O��'func'������܂����B�֐����I���܂��B");
						break;
					}
					else
					{
						res = Result_InvalidWordPosition;
						res.SetScene(Scene_Func);
						res.SetOperation(Operation_CheckStatement);
						res.SetLine(backGram);
						m_MessageBug(res, "func������������܂����B�����ɂ� func�錾�͋L�q�ł��܂���B");
						break;
					}
				}
			}
			else if ( r.IsError() )
			{
				//�G���[
				res = r;
				break;
			}
			else
			{
				//�X�e�[�g�����g�ł͂Ȃ������̂ŁA�����H
				CTmlValue v;
				r = m_calc.TextToValue(v, _gram);
				if ( r.IsSuccess() )
				{
					/*����������*/
				}
				else if ( r.IsError() )
				{
					//�G���[
					res = r;
					break;
				}
				else
				{
					/*��*/
					CAscii str = backGram.PeekNextParts().GetString();
					CTmlResult r(Scene_Non, Operation_CheckStatement, Result_UnknownKeyword);
					r.SetErrorString(str);
					r.SetLine(backGram);
					m_MessageBug(r, "��̕� '%s...'  ������܂��B", str.Left(32));
					return r;
				}
			}
		}
		//
		// �ϐ����x�����֐��J�n���ɖ߂�
		m_variable.DecLevels(iStartVarLevel);
		m_documents.DecLevels(iStartContentsLevel);
		//
		if ( res.IsError() )
		{
			if ( res.GetFileLine()==0 )
			{
				//�G���[�s�t�@�C���������ݒ�Ȃ�A
				res.SetLine(_gram);
			}
			return res;
		}
		return Result_Success;
	}

private:

	/**
	 * TinyMacroLang �����֐����Ǘ�.
	 *		�z��ŁA�������ԂƊ֐�����ێ����Ă��܂��B
	 */
	class CInterruptFunc
	{
	public:
		/// �R���X�g���N�^
		CInterruptFunc(void) : m_iLastAccessId(0)
		{
		}
		/// ������
		void Empty(void)
		{
			m_iLastAccessId = 0;
			m_astrFuncInfos.RemoveAll();
		}
		/**
		 * �֐��o�^.
		 *		�֐��̗L���̓`�F�b�N���Ă���o�^���Ă��������B
		 *	@param lpszFuncName �o�^����֐���
		 *	@param dwTick	ms��ɓ��삷�邩�H
		 *	@return InterruptId�BKILL���Ɏg�p���܂��B
		 */
		int Set(LPCSTR lpszFuncName, DWORD dwTick)
		{
			ASSERT( lpszFuncName != NULL && lpszFuncName[0]!=0 );
			INT_PTR iFindingId = -1;
			TFuncInfo T;
			T.dwTick = ::GetTickCount() + dwTick;
			T.strName = lpszFuncName;
			size_t len = m_astrFuncInfos.GetSize();
			for( size_t i = m_iLastAccessId; i < len; i++ )
			{
				if ( m_astrFuncInfos[i].strName.IsEmpty() )
				{
					m_astrFuncInfos[i] = T;
					iFindingId = i;
					break;
				}
			}
			if ( iFindingId < 0 )
			{
				for ( size_t i = 0; i < m_iLastAccessId && i < len; i++ )
				{
					if ( m_astrFuncInfos[i].strName.IsEmpty() )
					{
						m_astrFuncInfos[i] = T;
						iFindingId = i;
						break;
					}
				}
			}
			if ( iFindingId < 0 )
			{
				iFindingId = m_astrFuncInfos.Add(T);
			}
			TTRACE3( "TML::CInterruptFunc::Set() %dms��Ɋ֐�[%s]�����s�\�� ID=%d\n", dwTick, lpszFuncName, iFindingId );
			m_iLastAccessId = iFindingId + 1;
			return ToInt(iFindingId);
		}
		/**
		 * ����
		 *	@param iInterruptId	Set()�̖߂�l���w�肵�܂��B
		 */
		void Kill(int iInterruptId)
		{
			if ( ToInt(m_astrFuncInfos.GetSize()) > iInterruptId )
			{
				if ( ! m_astrFuncInfos[iInterruptId].strName.IsEmpty() )
				{
					m_astrFuncInfos[iInterruptId].strName.Empty();
					m_iLastAccessId = iInterruptId + 1;
					TTRACE1("CInterruptFunc::Kill() ID=%d�̊��荞�ݒ�~\n", iInterruptId );
				}
			}
		}
		/**
		 * [�m�F] �`�F�b�N.
		 *		���ԂɂȂ����֐����X�g��Ԃ��܂��B
		 *		�o�^�͉�������܂��B
		 *	@return ���ԂɂȂ����֐����Q
		 */
		CAsciiVector Check(void)
		{
			CAsciiVector vs;
			DWORD dwNow = ::GetTickCount();
			loop ( i, m_astrFuncInfos.GetSize() )
			{
				if ( m_astrFuncInfos[i].strName.IsEmpty() )
				{
					continue;
				}
				int r = static_cast<int>(dwNow - m_astrFuncInfos[i].dwTick);
				if ( r >= 0 )
				{
					TTRACE2( "TML::CInterruptFunc::Check() ID=%d�̊������s Func=[%s]\n", i,
							m_astrFuncInfos[i].strName 
					);
					vs.Add(m_astrFuncInfos[i].strName);
					m_astrFuncInfos[i].strName.Empty();
					m_iLastAccessId = i + 1;
				}
			}
			return vs;
		}
	private:
		/// �֐����
		struct TFuncInfo
		{
			DWORD dwTick;	///< ��������(�J�n�܂ł�ms�ł͂Ȃ��A�J�nTickCount�l)
			CAscii strName;	///< �J�n�֐���
		};
		CVectorT<TFuncInfo> m_astrFuncInfos;	///< �֐����
		size_t					m_iLastAccessId;	///< �ŏI�A�N�Z�XID
	};

	/**
	 * TinyMacroLang �l�X�g���Ǘ�.
	 *	 { } �̃l�X�g�������Ǘ��B�l�X�e�B���O���T�|�[�g�B
	 */
	class CNestInfo
	{
	public:
		/// �l�X�g���
		enum EKind
		{
			EK_Non = 0,		///< �Ȃ�ł��Ȃ��l�X�g
			EK_IfEqu,		///< if��(�^)�ɂ��l�X�g
			EK_IfNeq,		///< if��(�U)�ɂ��l�X�g
			EK_Switch,		///< switch�̃l�X�g
			EK_While,		///< while�̃l�X�g
		};

		/// �R���X�g���N�^
		CNestInfo(void)
		{
			m_iDepth = 0;
			m_aKind.Add(EK_Non/*���ł��Ȃ��l�X*/);
			TTRACE1("�l�X�g���x��=0 NowLvl=%d\n", m_iDepth);
		}
		/** ���A�g�b�v�H */
		bool IsTopDepth()
		{
			return m_iDepth == 0;
		}
		/** ���݂̎�� */
		EKind GetNowKind(void)
		{
			if ( ToInt(m_aKind.GetSize()) <= m_iDepth )
			{
				return EK_Non;
			}
			return m_aKind.Ref(m_iDepth);
		}
		/** ��O�̎�� */
		EKind GetLastKind(void)
		{
			if ( ToInt(m_aKind.GetSize()) <= (m_iDepth + 1) )
			{
				return EK_Non;
			}
			return m_aKind.Ref(m_iDepth + 1);
		}
		/** �l�X�g���ЂƂ[������ */
		void Inc(EKind eKind)
		{
			m_iDepth++;
			m_aKind.SetSize(m_iDepth);
			m_aKind.Add(eKind);
			TTRACE1("�l�X�g���x��++ NowLvl=%d\n", m_iDepth);
		}
		/** �l�X�g���ЂƂ[������(While��p) */
		void IncWhile(const CTmlGrammar& gram)
		{
			Inc(EK_While);
			TTRACE0("  ��while�l�X�g�ł�\n");
			m_aWhileStartInfo.Insert(0, gram);
		}
		/** ����while�Ȃ����Ԃ� */
		CTmlGrammar GetWhileInfo(void)
		{
			if ( GetNowKind() == EK_While )
			{
				return m_aWhileStartInfo[0];
			}
			return CTmlGrammar();
		}
		/** �l�X�g���ЂƂ󂭂��� */
		void Dec(void)
		{
			if ( m_iDepth > 0 )
			{
				if ( GetNowKind() == EK_While )
				{
					m_aWhileStartInfo.Remove(0);
				}
				m_iDepth--;
				//�Q�Ƃ̉\��������̂ŁA�����Ȃ�
				TTRACE1("�l�X�g���x��-- NowLvl=%d\n", m_iDepth);
			}
		}
	private:
		int						m_iDepth;			///< �l�X�g�[��
		CVectorT<EKind>			m_aKind;			///< �l�X�g�[���̗��R
		CVectorT<CTmlGrammar>	m_aWhileStartInfo;	///< while�J�n���
	};

	CTmlDocuments			m_documents;		///< �h�L�������g�Ǘ�
	CTmlVariableManager		m_variable;			///< �ϐ��Ǘ�
	CTmlCalculator				m_calc;				///< �����p�̌v�Z�N���X
	CInterruptFunc			m_cInterruptInfo;	///< ���荞��
	IListener*				m_piListener;		///< ���X�i�[�p�C���^�[�t�F�C�X
	ITmlExpander*				m_piExpander;		///< �g���֐��̃C���^�[�t�F�C�X
	ELogLevel				m_eLogLevel;		///< ���݂�LOG�̕\�����x��
	CAscii					m_strLogLastName;	///< ���X�gLOG�\���l�[��
	INT_PTR					m_iLogLastLine;		///< ���X�gLOG�\�����C��

	/**
	 * ���荞�݃`�F�b�N.
	 *		���[�U����̊��荞��(��~)�Ɗ��荞�ݎw��̏������s��
	 */
	CTmlResult m_CheckInterrupt(void)
	{
		//���X�i�[�`�F�b�N
		if ( m_piListener != NULL )
		{
			if ( ! m_piListener->IsExecutionContinued() )
			{
				m_MessageBug("���f���܂��B");
				return Result_UserInterruption;
			}
		}
		//�����݃`�F�b�N
		CAsciiVector astrFuncs = m_cInterruptInfo.Check();
		if ( astrFuncs.GetSize()>0 )
		{
			//�����݂���
			CTmlValuesVector tva;
			CTmlValue tvObject;
			loop ( i, astrFuncs.GetSize() )
			{
				TTRACE1( "���Ԃ������̂� [%s]�����s\n", astrFuncs[i] );
				CTmlResult res = ExecFunction(tvObject, astrFuncs[i], tva);
				if ( res.IsError() )
				{
					return res;
				}
			}
		}
		return Result_Success;
	}

	/**
	 * �G���[���b�Z�[�W�ʒm.
	 *		�G���[�������������A�����𕶎���Œʒm���邽�߂Ɏg���܂��B
	 *		���U���g���e�ɂ���Ă͏o�͂��܂���B
	 *	@param lpszMessage �G���[���e
	 *	@param ... �p�����[�^
	 */
	void m_MessageBug(LPCSTR lpszMessage, ...)
	{
		CAscii str;
		va_list args;
		va_start(args, lpszMessage);
		str.FormatV(lpszMessage, args);
		va_end(args);
		m_MessageBug(m_strLogLastName, m_iLogLastLine, str);
	}

	/**
	 * �G���[���b�Z�[�W�ʒm.
	 *		�G���[�������������A�����𕶎���Œʒm���邽�߂Ɏg���܂��B
	 *		���U���g���e�ɂ���Ă͏o�͂��܂���B
	 *	@param res ���ʁB
	 *	@param lpszMessage �G���[���e
	 *	@param ... �p�����[�^
	 */
	void m_MessageBug(const CTmlResult& res, LPCSTR lpszMessage, ...)
	{
		if ( res.GetResult() == Result_UserInterruption || res.GetResult() == Result_ProgramableInterruption )
		{
			return;
		}
		CAscii str;
		va_list args;
		va_start(args, lpszMessage);
		str.FormatV(lpszMessage, args);
		va_end(args);
		m_MessageBug(res.GetFileName(), res.GetFileLine(), str);
	}

	/**
	 * �G���[���b�Z�[�W�ʒm.
	 *		�G���[�������������A�����𕶎���Œʒm���邽�߂Ɏg���܂��B
	 *		���U���g���e�ɂ���Ă͏o�͂��܂���B
	 *	@param lpszName �����t�@�C�����B
	 *	@param iLine �����s�B
	 *	@param lpszMessage �G���[���e
	 *	@param ... �p�����[�^
	 */
	void m_MessageBug(LPCSTR lpszName, INT_PTR iLine, LPCSTR lpszMessage)
	{
		CAscii str;
		if ( iLine > 0 )
		{
			str.Format("%s(%d): Error:%s", lpszName, iLine, lpszMessage);
		}
		else
		{
			str.Format("%s", lpszMessage);
		}
		m_strLogLastName = lpszName;
		m_iLogLastLine = iLine;
		#ifdef _DEBUG
			::OutputDebugStringA(str + "\n");
		#endif
		if ( m_piListener != NULL )
		{
			m_piListener->OutputLog(CStr(str), true);
		}
	}

	/**
	 * ���O���b�Z�[�W�ʒm
	 *		�����̗����ʒm���邽�߂Ɏg���܂��B�}�N���̃f�o�O�p�Ɏg�p���܂��B
	 *		���O���x���ɂ���Ă͏o�͂��܂���B
	 *	@param elLevel ���O���x���B
	 *	@param lpszMessage �G���[���e
	 *	@param ... �p�����[�^
	 */
	void m_MessageLog(ELogLevel elLevel, LPCSTR lpszMessage, ...)
	{
		CAscii str;
		va_list args;
		va_start(args, lpszMessage);
		str.FormatV(lpszMessage, args);
		va_end(args);
		m_MessageLog("", 0, elLevel, str);
	}

	/**
	 * ���O���b�Z�[�W�ʒm.
	 *		�����̗����ʒm���邽�߂Ɏg���܂��B�}�N���̃f�o�O�p�Ɏg�p���܂��B
	 *		���O���x���ɂ���Ă͏o�͂��܂���B
	 *	@param gram �����ʒu�B
	 *	@param elLevel ���O���x���B
	 *	@param lpszMessage �G���[���e
	 *	@param ... �p�����[�^
	 */
	void m_MessageLog(const CTmlGrammar& gram, ELogLevel elLevel, LPCSTR lpszMessage , ...)
	{
		CAscii str;
		va_list args;
		va_start(args, lpszMessage);
		str.FormatV(lpszMessage, args);
		va_end(args);
		m_MessageLog(CAscii(gram.GetName()), gram.GetLine(), elLevel, str);
	}

	/**
	 * ���O���b�Z�[�W�ʒm
	 *		�����̗����ʒm���邽�߂Ɏg���܂��B�}�N���̃f�o�O�p�Ɏg�p���܂��B
	 *		���O���x���ɂ���Ă͏o�͂��܂���B
	 *	@param lpszName �����t�@�C�����B
	 *	@param iLine �����s�B
	 *	@param elLevel ���O���x���B
	 *	@param lpszMessage �G���[���e
	 *	@param ... �p�����[�^
	 */
	void m_MessageLog(LPCSTR lpszName, INT_PTR iLine, ELogLevel elLevel, LPCSTR lpszMessage)
	{
		#ifndef _DEBUG
			if ( m_eLogLevel<elLevel )
			{
				return;
			}
		#endif
		CAscii strLevel;
		switch(elLevel)
		{
		case ELL_DETAILS://�ڍ�
			strLevel = "Info.";
			break;
		case ELL_BRANCH://����
			strLevel = "Branch";
			break;
		case ELL_FUNC:	//�֐�
			strLevel = "Func.";
			break;
		case ELL_ABSOLUTE://���
		default:
			strLevel = "Info.";
			break;
		}
		CAscii str;
		if ( iLine > 0 )
		{
			str.Format("%s(%d): %s:%s", lpszName, iLine, strLevel, lpszMessage);
		}
		else
		{
			str.Format(" %s:%s", strLevel, lpszMessage);
		}
		#ifdef _DEBUG
			::OutputDebugStringA(str + "\n");
			if ( m_eLogLevel < elLevel )
			{
				return;
			}
		#endif
		if ( m_piListener != NULL )
		{
			m_piListener->OutputLog(CStr(str), false);
		}
	}

	/**
	 * �ϐ��ꗗ�A���e�\��.
	 *	�ϐ��̈ꗗ��ʒm���邽�߂Ɏg���܂��B�}�N���̃f�o�O�p�Ɏg�p���܂��B
	 */
	void m_DumpVarDim(void)
	{
		CTmlVariableManager::CVaVaVectors& aaVava = m_variable.ReferVarArrays();
		CTmlVariableManager::CArVaVectors& aaDmva = m_variable.ReferArrayArrays();
		//
		INT_PTR iDim1 = aaVava.GetSize();
		for( INT_PTR i = iDim1 - 1; i >= 0; i-- )
		{
			m_MessageLog(ELL_ABSOLUTE, "---�ϐ����e(lvl=%d)", i);
			CTmlVariableManager::CVaVaVector& aa = aaVava.Ref(i);
			INT_PTR iDim2 = aa.GetSize();
			for( INT_PTR j = 0; j < iDim2; j++ )
			{
				CTmlVariableManager::TVarValue &T = aa.Ref(j);
				if ( T.value.HasData() )
				{
					//Data�^
					m_MessageLog(ELL_ABSOLUTE, "%s = [%s](len=%d)",
								T.name,
								T.value.GetString(),
								T.value.GetInteger()
					);
				}
				else
				{
					//���l�^
					m_MessageLog(ELL_ABSOLUTE, "%s = %d", T.name, T.value.GetInteger());
				}
			}
			CTmlVariableManager::CArVaVector& va = aaDmva.Ref(i);
			iDim2 = va.GetSize();
			for( INT_PTR j = 0; j < iDim2; j++ )
			{
				CTmlVariableManager::TArrayValue &T = va.Ref(j);
				INT_PTR iDim3 = T.values.GetSize();
				for( INT_PTR k = 0; k < iDim3; k++ )
				{
					if ( T.values[k].HasData() )
					{
						//Data�^
						m_MessageLog(ELL_ABSOLUTE, "%s(%d) = [%s](len=%d)",
									T.name,
									k,
									T.values[k].GetString(),
									T.values[k].GetInteger()
						);
					}
					else
					{
						//���l�^
						m_MessageLog(ELL_ABSOLUTE,"%s(%d) = %d",
									T.name, k, T.values[k].GetInteger());
					}
				}
			}
		}
	}

	/** 
	 * [����] �g���ϐ��A�֐��̏���.
	 *	@note ITmlExpander �̎����B
	 *	@param[out] _answer �����������̒l
	 *	@param[in] lpszToken ���������g�[�N��(�g�[�N��������NULL�I�[)
	 *	@param[in] aValues ���������g�[�N���̈����A����ѓ��e�B
	 *	@param[in] boIsTestMode true�Ȃ�e�X�g���[�h�i���@�`�F�b�N�̂ݍs���j�B
	 *	@return ���ʁB
	 */
	CTmlResult ExpandVariable(CTmlValueCarrier& _answer, LPCSTR lpszToken, const CTmlValuesVector &aValues, bool boIsTestMode)
	{
		if ( lpszToken == NULL )
		{
			//�e�X�g�p��LOG�o��
			ASSERT( boIsTestMode );
			m_MessageBug(_answer().GetString());
			return Result_Success;
		}
		//�{�N���X���o���Ă�ϐ����`�F�b�N
		CTmlValue* V = m_variable.FindValue(lpszToken);
		if ( V != NULL )
		{
			_answer.SetRef(*V);
			if ( aValues.GetSize() > 0 )
			{
				//�p�����[�^������
				return CTmlResult(Operation_CheckFunc, Result_UnnecessaryParam);
			}
			#ifdef _DEBUG
				_answer.Set(lpszToken);
			#endif
			return Result_Success;
		}
		//
		//�{�N���X���o���Ă�z����`�F�b�N
		CTmlValuesVector* A = m_variable.FindArray(lpszToken);
		if ( A != NULL )
		{
			if ( aValues.GetSize()==0 )
			{
				_answer = ToInt(A->GetSize());
				return Result_Success;
			}
			else if ( aValues.GetSize() == 1 )
			{
				int i = aValues[0].GetInteger();
				if ( i < 0 || i >= ToInt(A->GetSize()) )
				{
					return CTmlResult(Operation_CheckFunc, Result_OutOfRange);
				}
				_answer.SetRef(A->Ref(i));
				#ifdef _DEBUG
					CAscii s;
					s.Format("%s(%d)", lpszToken, i);
					_answer.Set(s);
				#endif
				return Result_Success;
			}
			return CTmlResult::CheckParamNum(aValues.GetSize(), 1);
		}
		//
		if ( m_piExpander != NULL )
		{
			CTmlResult r = m_piExpander->ExpandVariable(_answer, lpszToken, aValues, boIsTestMode);
			if ( ! r.IsNop() )
			{
				return r;
			}
		}
		//�{�N���X���m���Ă���֐����`�F�b�N
		CTmlValue value;
		CTmlResult res = ExecFunction(value, lpszToken, aValues);
		if ( res.IsError() )
		{
			return res;
		}
		else if ( res.IsSuccess() )
		{
			_answer = value;
			return res;
		}
		//
		if ( STRLIB::Compare(lpszToken, "pause") == 0 )
		{
			//�ėp�O���[�o���֐� �u�w�莞�Ԓ�~�v
			TTRACE1( "found System Func [%s]\n", lpszToken );
			if ( aValues.GetSize() == 1 )
			{
				if ( boIsTestMode ) { return Result_Success; }
				//
				int i = aValues[0].GetInteger();
				//	::Sleep( i );
				CTmlResult r;
				DWORD dwStart;
				DWORD dwNow;
				if ( i < 500 )
				{
					dwStart = ::GetTickCount();
					while ( true )
					{
						dwNow = ::GetTickCount();
						DWORD dw = i;
						if ( (dwNow - dwStart) >= dw ) { break; }
						r = m_CheckInterrupt();
						if ( ! r.IsSuccess() ) { return r; }
					}
				}
				else
				{
					dwStart = ::GetTickCount();
					while ( true )
					{
						dwNow = ::GetTickCount();
						DWORD dw = i;
						if ( (dwNow - dwStart) >= dw ) { break; }
						r = m_CheckInterrupt();
						if ( ! r.IsSuccess() ){ return r; }
						::Sleep(10);
					}
				}
				return Result_Success;
			}
			return CTmlResult::CheckParamNum(aValues.GetSize(), 1);
		}
		else if ( STRLIB::Compare(lpszToken, "setinterrupt") == 0 )
		{
			//�ėp�O���[�o���֐� �u���荞�ݐݒ�v
			TTRACE1( "found System Func [%s]\n", lpszToken );
			if ( aValues.GetSize() == 2 )
			{
				if ( boIsTestMode ){ return Result_Success; }
				//
				CAscii strFuncName = aValues[0].GetString();
				//
				CTmlDocuments::TFuncInfoRes tFunc;
				tFunc = m_documents.FindFunc(strFuncName);
				if ( ! tFunc.HasData() )
				{
					return CTmlResult(Operation_CheckFunc, Result_NotFoundFunction);
				}
				_answer = m_cInterruptInfo.Set(strFuncName, aValues[1].GetInteger());
				return Result_Success;
			}
			return CTmlResult::CheckParamNum(aValues.GetSize(), 2);

		}
		else if ( STRLIB::Compare(lpszToken, "killinterrupt") == 0 )
		{
			//�ėp�O���[�o���֐� �u���荞�ݒ�~�v
			TTRACE1( "found System Func [%s]\n", lpszToken );
			if ( aValues.GetSize() == 0 )
			{
				if ( boIsTestMode ) { return Result_Success; }
				m_cInterruptInfo.Empty();
				return Result_Success;
			}
			else if ( aValues.GetSize() == 1 )
			{
				if ( boIsTestMode ){ return Result_Success; }
				m_cInterruptInfo.Kill(aValues[0].GetInteger());
				return Result_Success;
			}
			return CTmlResult::CheckParamNum(aValues.GetSize(), 1);
		}
#if 0
		else if ( STRLIB::Compare((lpszToken, "playSound" ) == 0 || STRLIB::Compare(lpszToken, "loopSound") == 0 )
		{
			//�ėp�O���[�o���֐� �u���v
			TTRACE1( "found System Func [%s]\n", lpszToken );
			if ( aValues.GetSize() == 1 )
			{
				if ( boIsTestMode ) { return ETR_Success; }
				CCurrentDirMan cDirMan;
				cDirMan.SetDirOfFile( m_cText.GetFileName() );
				//
				CString strWaveName = aValues[0].GetString();
				DWORD dwSound = SND_FILENAME | SND_NODEFAULT | SND_ASYNC;
				if ( lpszToken[0]=='l' )
				{
					dwSound |= SND_LOOP;
				}
				::PlaySound(strWaveName, NULL, dwSound);
				//
				return 1;
			}
			return CTmlResult::CheckParamNum(aValues.GetSize(), 1);
		}
		else if ( _tcscmp( lpszToken,"stopsound" )==0 )
		{
			//�ėp�O���[�o���֐� �u���v
			TTRACE1("found System Func [%s]\n", lpszToken);
			if ( aValues.GetSize()==0 )
			{
				if ( boIsTestMode ){return ETR_Success;}
				//
				::PlaySound(NULL, NULL, 0);
				return 1;
			}
			return CTmlResult::CheckParamNum(aValues.GetSize(), 0);

		}
#endif
		//�������
		return Result_Nop;
	}

	/** 
	 * [����] �ύX(���)�R�[���o�b�N�B
	 *	@note IExpander �̎����B
	 *	@param lpszName ������ꂽ���O
	 *	@param value ������ꂽ�l
	 *	@return ���ʁB
	 */
	CTmlResult OnChangeValue(LPCSTR lpszName, const CTmlValue& value)
	{
		//�������
		if ( m_piExpander != NULL )
		{
			return m_piExpander->OnChangeValue(lpszName, value);
		}
		return Result_Nop;
	}

	/// �X�e�[�g�����g���ʃ^�C�v
	enum EStatementResultType
	{
		ESRT_Normal,	///< �m�[�}��
		ESRT_Return,	///< ���^�[��
		ESRT_Function,	///< �t�@���N�V����
	};

	/**
	 * �X�e�[�g�����g���������s
	 *	@param[in,out] _gram	���@
	 *	@param[in,out] _nest	�o�A�p�̃l�X�g�J�E���^
	 *	@param[out] _answer	�����̒l�B
	 *	@param[out] _stateResType	�X�e�[�g�����g����
	 *	@return ���ʁB
	 */
	CTmlResult m_SearchAndExecCommand(CTmlGrammar& _gram, CNestInfo& _nest, CTmlValue& _answer, EStatementResultType& _stateResType)
	{
		LPCSTR pp = _gram.GetPointer();
		IgnoreUnusedValue(pp);
		CTmlGrammar backGram = _gram;
		CTmlGrammar::CParts parts;
		//
		bool boIsTestMode = m_calc.IsTestMode();
		//
		_stateResType = ESRT_Normal;
		//
		CTmlResult res;
		//
		parts = _gram.GetNextParts();
		if ( parts.GetKind() != CTmlGrammar::TOKEN )
		{
			/*�g�[�N�����Ȃ��I�I*/
			_gram = backGram;
			return Result_Nop;
		}
		CAscii token = parts.GetString();
		if ( token.IsEqual("func") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'�����m�F", token);
			_stateResType = ESRT_Function/*func*/;
		}
		else if ( token.IsEqual("return") || token.IsEqual("end") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'�����m�F", token);
			parts = _gram.PeekNextParts();
			if ( strchr(";{}/", parts.GetString()[0]) != NULL )
			{
				;
			}
			else
			{
				CTmlValue v;
				res = m_calc.TextToValue(v, _gram);
				if ( res.IsError() )
				{
					res.SetScene((token[0] == 'r') ? Scene_Return : Scene_End);
					m_MessageBug(res, "%s ���̈������ŃG���[���������܂����B", token);
					return res;
				}
				_answer = v;
			}
			//
			if ( ! boIsTestMode )
			{
				//�{�ԃ��[�h
				if ( token[0] == 'r' )
				{
					//return
					_stateResType = ESRT_Return/*return*/;
				}
				else
				{
					//end
					return CTmlResult(Scene_End, Operation_Non, Result_ProgramableInterruption);
				}
			}
			else
			{
				//�e�X�g���[�h
				_answer = 1;
			}
		}
		else if ( token.IsEqual("val") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'�����m�F", token);
			//
			parts = _gram.GetNextParts();
			if ( parts.GetKind() != CTmlGrammar::TOKEN )
			{
				res = CTmlResult(Scene_Val, Operation_Non, Result_NotName).SetLine(backGram);
				m_MessageBug(res, "val�̌�ɁA�ϐ����w�肪����܂���B");
				return res;
			}
			CAscii valName = parts.GetString();
			CTmlValue firstValue;
			parts = _gram.PeekNextParts();
			if ( CAscii(parts.GetString()) == "=" )
			{
				_gram.GetNextParts();
				res = m_calc.TextToValue(firstValue, _gram);
				if ( res.IsError() )
				{
					res.SetScene(Scene_Val);
					res.SetLine(backGram);
					m_MessageBug(res, "val�̕ϐ��������̎��ŃG���[���������܂����B");
					return res;
				}
			} 
			else 
			{
				firstValue = 0;
			}
			//
			CTmlValueCarrier a;
			if ( m_variable.HasNameInNowLevel(valName) )
			{
				//�������x���ɓ������O������B
				res = CTmlResult(Scene_Val, Operation_Non, Result_AlreadyExistName);
				res.SetErrorString(valName);
				if ( ! m_calc.IsTestMode() )
				{
					return res;
				}
				//LOG��
				res.SetLine(_gram);
				m_MessageBug(res.ToString());
			}
			else
			{
				if ( ! m_variable.AddValue(valName, firstValue) )
				{
					res = CTmlResult(Scene_Val, Operation_Non, Result_UnknownError).SetLine(backGram);
					m_MessageBug(res, "val�̕ϐ��o�^�łȂ��̃G���[���������܂����B");
					return res;
				}
				m_MessageLog(backGram, ELL_DETAILS, "�ϐ��錾 `%s' = [%s](%d)", valName, firstValue.GetString(), firstValue.GetInteger());
			}
		}
		else if ( token.IsEqual("array") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'�����m�F", token);
			//
			parts = _gram.GetNextParts();
			if ( parts.GetKind() != CTmlGrammar::TOKEN )
			{
				res = CTmlResult(Scene_Array, Operation_Non, Result_NotName).SetLine(backGram);
				m_MessageBug(res, "array�̌�ɁA�ϐ����w�肪����܂���B");
				return res;
			}
			CAscii arrayName = parts.GetString();
			parts = _gram.PeekNextParts();
			if ( ! parts.IsEqualString("(") )
			{
				res = CTmlResult(Scene_Array, Operation_Non, Result_NotFoundParentheses).SetLine(backGram);
				m_MessageBug(res, "array�̔z�񖼂̌�Ɋ��ʂ�����܂���B");
				return res;
			}
			else
			{
				CTmlValue sizeValue;
				res = m_calc.TextToValue(sizeValue, _gram);
				if ( res.IsError() )
				{
					res.SetScene(Scene_Array);
					res.SetLine(backGram);
					m_MessageBug(res, "array�̔z��T�C�Y�̎��ŃG���[���������܂����B");
					return res;
				}
				CTmlValueCarrier a;
				if ( m_variable.HasNameInNowLevel(arrayName) )
				{
					//�������x���ɓ������O������B
					res = CTmlResult(Scene_Array, Operation_Non, Result_AlreadyExistName);
					res.SetErrorString(arrayName);
					if ( m_calc.IsTestMode() )
					{
						//LOG��
//						res.SetLine(CAscii(_gram.GetName()), _gram.GetLine());
						res.SetLine(_gram);
						m_MessageBug(res.ToString());
					}
					else
					{
						return res;
					}
				}
				else
				{
					if ( ! m_variable.AddArray(arrayName, sizeValue.GetInteger()) )
					{
						res = CTmlResult(Scene_Array, Operation_Non, Result_OutOfRange).SetLine(backGram);
						m_MessageBug(res, "array�̔z��T�C�Y���ُ�l�ł��B");
						return res;
					}
					m_MessageLog(backGram, ELL_DETAILS, "�z��錾 `%s(%d)", arrayName, sizeValue.GetInteger());
				}
			}
		}
		else if ( token.IsEqual("if") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'�����m�F", token);
			//
			CTmlValue boolValue;
			res = m_calc.TextToValue(boolValue, _gram);
			if ( res.IsError() )
			{
				res.SetScene(Scene_If);
				res.SetLine(backGram);
				m_MessageBug(res, "if���̏������ŃG���[���������܂����B");
				return res;
			}
			parts = _gram.GetNextParts();
			if ( ! parts.IsEqualString("{") )
			{
				res = CTmlResult(Scene_If, Operation_CheckStatement, Result_NotFoundParentheses).SetLine(backGram);
				m_MessageBug(res, "if����`{'������܂���B");
				return res;
			}
			//
			if ( ! boIsTestMode )
			{
				//�{�ԃ��[�h
				if ( boolValue.GetInteger() != 0 )
				{
					m_MessageLog(ELL_BRANCH, "�������ɍ����܂����̂ňȉ������s���܂�");
					_nest.Inc(CNestInfo::EK_IfEqu);
					m_variable.IncLevel();
					m_documents.IncLevel();
				}
				else
				{
					m_MessageLog(ELL_BRANCH, "�������ɍ����܂���ł���");
					_nest.Inc( CNestInfo::EK_IfNeq );
					_nest.Dec();
					_gram.SkipoutBlock();	//`}'�̎���
				}
			}
			else
			{
				//�e�X�g���[�h
				m_MessageLog(ELL_BRANCH, "���e�X�g���[�h�Ȃ̂ňȉ������s���܂�");
				_nest.Inc(CNestInfo::EK_IfNeq);
				m_variable.IncLevel();
				m_documents.IncLevel();
			}
		} 
		else if ( token.IsEqual("else") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'�����m�F", token);
			//
			parts = _gram.GetNextParts();
			if ( parts.IsEqualString("if") )
			{
				m_MessageLog(ELL_DETAILS, "else���ɑ�����if��������܂����B");
				switch( _nest.GetLastKind() )
				{
				case CNestInfo::EK_IfEqu:
					m_MessageLog(ELL_BRANCH, "�������ɍ����܂����̂�else�������s���܂���");
					{
						//if����{�܂ŃX�L�b�v
						CTmlValue boolValue;
						res = m_calc.TextToValue(boolValue, _gram);
						if ( res.IsError() )
						{
							res.SetScene(Scene_If);
							m_MessageBug(res, "if���̏������ŃG���[���������܂����B");
							return res;
						}
						parts = _gram.GetNextParts();
						if ( ! parts.IsEqualString("{") )
						{
							res = CTmlResult(Scene_If, Operation_CheckStatement, Result_NotFoundParentheses).SetLine(backGram);
							m_MessageBug(res, "if����`{'������܂���B");
							return res;
						}
						_gram.SkipoutBlock();	//`}'�̎���
					}
					break;
				case CNestInfo::EK_IfNeq:
					m_MessageLog(ELL_BRANCH, "�������ɍ���Ȃ��̂�else �̌�� if�������s���܂�");
					break;
				default:
					res = CTmlResult(Scene_Else, Operation_CheckStatement, Result_InvalidWordPosition).SetLine(backGram);
					m_MessageBug(res, "if�����Ȃ��̂�else��������܂��B");
					return res;
				}
			}
			else if ( ! parts.IsEqualString("{") )
			{
				res = CTmlResult(Scene_Else, Operation_CheckStatement, Result_NotFoundParentheses).SetLine(backGram);
				m_MessageBug(res, "else����`{'������܂���B");
				return res;
			}
			else
			{
				switch( _nest.GetLastKind() )
				{
				case CNestInfo::EK_IfEqu:
					m_MessageLog(ELL_BRANCH, "�������ɍ����܂����̂�else�������s���܂���");
					_gram.SkipoutBlock();	//`}'�̎���
					break;
				case CNestInfo::EK_IfNeq:
					m_MessageLog(ELL_BRANCH,"�������ɍ���Ȃ��̂�else�������s���܂�");
					_nest.Inc(CNestInfo::EK_Non);
					m_variable.IncLevel();
					m_documents.IncLevel();
					break;
				default:
					res = CTmlResult(Scene_Else, Operation_CheckStatement, Result_InvalidWordPosition).SetLine(backGram);
					m_MessageBug(res, "if�����Ȃ��̂�else��������܂��B");
					return res;
				}
			}
		}
		else if ( token.IsEqual("case") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'�����m�F", token);
			//
			CTmlValue dummyValue;
			res = m_calc.TextToValue(dummyValue, _gram);
			if ( res.IsError() )
			{
				res.SetScene(Scene_Case);
				m_MessageBug(res, "case���̎��ŃG���[���������܂����B");
				return res;
			}
			parts = _gram.GetNextParts();
			if ( ! parts.IsEqualString(":") )
			{
				res = CTmlResult(Scene_Case, Operation_CheckStatement, Result_NotFoundParentheses).SetLine(backGram);
				m_MessageBug(res, "case����`:'������܂���B");
				return res;
			}
		} 
		else if ( token.IsEqual("default") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'�����m�F", token);
			//
			parts = _gram.GetNextParts();
			if ( ! parts.IsEqualString(":") )
			{
				res = CTmlResult(Scene_Default, Operation_CheckStatement, Result_NotFoundParentheses).SetLine(backGram);
				m_MessageBug(res, "default����`:'������܂���B");
				return res;
			}
		}
		else if ( token.IsEqual("switch") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'�����m�F", token);
			//
			CTmlValue caseValue;
			res = m_calc.TextToValue(caseValue, _gram);
			if ( res.IsError() )
			{
				res.SetScene(Scene_Switch);
				m_MessageBug(res, "switch���̏������ŃG���[���������܂����B");
				return res;
			}
			m_MessageLog(ELL_DETAILS, "switch���̈�����[%s](%d)�ł��B", caseValue.GetString(), caseValue.GetInteger());
			//
			parts = _gram.GetNextParts();
			if ( ! parts.IsEqualString("{") )
			{
				res = CTmlResult(Scene_Switch, Operation_CheckStatement, Result_NotFoundParentheses).SetLine(backGram);
				m_MessageBug(res, "switch����`{'������܂���B");
				return res;
			}
			//switch()����̈ʒu���L��
			CTmlGrammar backGram = _gram;
			//default�̈ʒu
			CTmlGrammar defaultGram;
			//-- case or default �܂ŃX�L�b�v
			bool boMatchCase = false;
			while ( true )
			{
				parts = _gram.GetNextParts();
				CAscii token = parts.GetString();
				if ( parts.GetKind() == CTmlGrammar::OPEN_CHAR )
				{
					//�u���b�N�J�n����
					_gram.SkipoutBlock();
					continue;
				}
				else if ( token.IsEqual("}") )
				{
					break;
				}
				switch ( parts.GetKind() )
				{
				case CTmlGrammar::CLOSE_CHAR:		// �u���b�N�I�[����
				case CTmlGrammar::FINAL:			// �I���
				case CTmlGrammar::ERROR_STRING:	// ������G���[
				case CTmlGrammar::ERROR_DEPTH:		// �[���G���[
					res = CTmlResult(Scene_Switch, Operation_CheckStatement, Result_NotCloseParentheses).SetLine(backGram);
					m_MessageBug(res, "`{'���Ԃ��Ă܂���B");
					return res;
				}
				if ( token.IsEqual("default") )
				{
					m_MessageLog(_gram, ELL_DETAILS, "`default��'���m�F");
					parts = _gram.GetNextParts();
					if ( ! parts.IsEqualString(":") )
					{
						res = CTmlResult(Scene_Switch, Operation_CheckStatement, Result_NotFoundParentheses).SetLine(backGram);
						m_MessageBug(res, "switch����`:'������܂���B");
						return res;
					}
					defaultGram = _gram;	//�L���I
					continue;
				}
				else if ( token.IsEqual("case") )
				{
					m_MessageLog(_gram, ELL_DETAILS, "`case��'���m�F");
					//
					CTmlValue v;
					res = m_calc.TextToValue(v, _gram);
					if ( res.IsError() )
					{
						res.SetScene(Scene_Case);
						m_MessageBug(res, "case���̎��ŃG���[���������܂����B");
						return res;
					}
					parts = _gram.GetNextParts();
					if ( ! parts.IsEqualString(":") )
					{
						res = CTmlResult(Scene_Case, Operation_CheckStatement, Result_NotFoundParentheses).SetLine(backGram);
						m_MessageBug(res, "case����`:'������܂���B");
						return res;
					}
					if ( ! boIsTestMode && caseValue.Compare(m_calc.ReferElementWildResult(), v) )
					{
						//�{�ԃ��[�h & ��v
						m_MessageLog(ELL_BRANCH, "��v����case��������܂���");
						boMatchCase = true;
						break;
					}
				}
			}
			//
			if ( ! boIsTestMode )
			{
				//�{�ԃ��[�h
				if ( ! boMatchCase )
				{
					if ( defaultGram.GetPointer() != NULL )
					{
						//default���g��
						m_MessageLog(ELL_BRANCH, "��v����case�����Ȃ��̂�default���ȉ������s���܂�");
						_gram = defaultGram;
						boMatchCase = true;
					}
					else
					{
						m_MessageLog(ELL_BRANCH,"switch�\�����Ɉ�v����`case'`default'��������܂���ł���");
						_gram.SkipoutBlock();	//`}'�̎���
					}
				}
				if ( boMatchCase )
				{
					_nest.Inc(CNestInfo::EK_Switch);
					m_variable.IncLevel();
					m_documents.IncLevel();
				}
			}
			else
			{
				//�e�X�g���[�h
				_gram = backGram;
				_nest.Inc( CNestInfo::EK_Switch );
				m_variable.IncLevel();
				m_documents.IncLevel();
			}
		}
		else if ( token.IsEqual("while") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'�����m�F", token);
			//
			CTmlValue boolValue;
			res = m_calc.TextToValue(boolValue, _gram);
			if ( res.IsError() )
			{
				res.SetScene(Scene_While);
				m_MessageBug(res, "while���̏������ŃG���[���������܂����B");
				return res;
			}
			//
			parts = _gram.GetNextParts();
			if ( ! parts.IsEqualString("{") )
			{
				res = CTmlResult(Scene_While, Operation_CheckStatement, Result_NotFoundParentheses).SetLine(backGram);
				m_MessageBug(res, "while����`{'������܂���B");
				return res;
			}
			if ( ! boIsTestMode )
			{
				//�{�ԃ��[�h
				if ( boolValue.GetInteger() != 0 )
				{
					m_MessageLog(ELL_BRANCH, "�������ɍ����܂����̂ňȉ������s���܂�");
					_nest.IncWhile(_gram);
					m_variable.IncLevel();
					m_documents.IncLevel();
				}
				else
				{
					m_MessageLog(ELL_BRANCH, "�������ɍ����܂���ł���");
					_gram.SkipoutBlock();	//`}'�̎���
				}
			}
			else
			{
				//�e�X�g���[�h
				_nest.Inc(CNestInfo::EK_Switch);	//�e�X�g�ł́Aswitch�Ƃ��Ă���
				m_variable.IncLevel();
				m_documents.IncLevel();
			}
		}
		else if ( token.IsEqual("break") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'�����m�F", token);
			//
			if ( _nest.IsTopDepth() )
			{
				res = CTmlResult(Scene_Break, Operation_CheckStatement, Result_InvalidWordPosition).SetLine(backGram);
				m_MessageBug(res, "`switch'��`while'�����Ȃ��̂�`break'��������܂��B");
				return res;
			}
			//
			CNestInfo cNestTmp = _nest;
			//
			CTmlGrammar bg = _gram;
			while ( true )
			{
				if ( cNestTmp.GetNowKind() == CNestInfo::EK_Switch || cNestTmp.GetNowKind() == CNestInfo::EK_While )
				{
					if ( ! _gram.SkipoutBlock() )
					{
						_gram = bg;
						res = CTmlResult(Scene_Break, Operation_CheckStatement, Result_NotCloseParentheses).SetLine(backGram);
						m_MessageBug(res, "'{'�����Ă��܂���B");
						return res;
					}
					cNestTmp.Dec();
					if ( ! boIsTestMode )
					{
						//�{�ԃ��[�h
						_nest = cNestTmp;
						m_variable.DecLevel();
						m_documents.DecLevel();
					}
					else
					{
						//�e�X�g���[�h
						_gram = bg;
					}
					break;
				}
				else
				{
					if ( ! _gram.SkipoutBlock() )
					{
						_gram = bg;
						res = CTmlResult(Scene_Break, Operation_CheckStatement, Result_NotCloseParentheses).SetLine(backGram);
						m_MessageBug(res, "'{'�����Ă��܂���B");
						return res;
					}
					cNestTmp.Dec();
					if ( ! boIsTestMode )
					{
						//�{�ԃ��[�h
						m_variable.DecLevel();
						m_documents.DecLevel();
					}
					if ( cNestTmp.IsTopDepth() )
					{
						_gram = bg;
						res = CTmlResult(Scene_Break, Operation_CheckStatement, Result_InvalidWordPosition).SetLine(backGram);
						m_MessageBug(res, "`switch'��`while'�����Ȃ��̂�`break'��������܂��B");
						return res;
					}
				}
			}
		}
		else if ( token.IsEqual("include") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'�����m�F", token);
			//
			CTmlValue v;
			res = m_calc.TextToValue(v, _gram);
			if ( res.IsError() )
			{
				res.SetScene(Scene_Include);
				m_MessageBug(res, "include���̏������ŃG���[���������܂����B");
				return res;
			}
			//���݂̎��s�s��Path
			CStr str = CFilePathManager::GetPathName(CStr(_gram.GetName()));
			res = AddMacroFile(CStr(v.GetString()), str);
			if ( ! res.IsSuccess() )
			{
				res.SetScene(Scene_Include);
				m_MessageBug(res, "include���̏������ŃG���[���������܂����B");
				return res;
			}
			_answer = v;
		}
		else if ( token.IsEqual("vardump") )
		{
			m_MessageLog(backGram, ELL_FUNC, "`%s'�����m�F", token);
			//**�J���p�ł�
			if ( ! boIsTestMode )
			{
				//�{�ԃ��[�h
				m_DumpVarDim();
			}
		}
		else
		{
			//���Ȃ�
			_gram = backGram;
			return Result_Nop;
		}
		//
//		cBackupLine.Destroy();
		return Result_Success;
	}

	friend class CTmlMacroTest;
};



}; // TNB
