#pragma once
/**
 *@file
 * TinyMacroLang �h�L�������g�Ǘ��֌W�̃w�b�_
 *	
 *		TML�̕ϐ��Ǘ��̃N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbTmlValue.h"
#include "TnbTmlCalculator.h"
#include "TnbTmlResult.h"
#include "TnbStrVector.h"



//TNB Library
namespace TNB
{



/**@ingroup TML
 * TinyMacroLang �h�L�������g�Ǘ�
 *
 * �R���e���c�Ǘ����܂�<BR>
 *	�R���e���c(�t�@�C���̓��e�Ȃ�)���A���x���Ǘ����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbTmlDocuments.h
 *
 *	@date 06/01/01 �V�K�쐬
 *	@date 07/07/20 ���P
 */
class CTmlDocuments : public CTml, CCopyImpossible
{
public:

	/**
	 * �t�@�C�����.
	 *	@see FindFileName
	 */
	struct TFileInfo
	{
		/// �֐����*
		struct TFunc
		{
			CAscii			strFuncName;	///< �֐���
			CAsciiVector	strParamNames;	///< ������
			INT_PTR			iOffset;		///< �֐��J�n��Offset(�o�̎��̈ʒu���w���Ă���)
			INT_PTR			iLine;			///< �֐��J�n��Line(�o�̎��̈ʒu���w���Ă���)
			/// �R���X�g���N�^
			TFunc(void) : iOffset(0), iLine(0){}
			/// �R�s�[�R���X�g���N�^
			TFunc(const TFunc& other)
				: strFuncName(other.strFuncName), strParamNames(other.strParamNames), iOffset(other.iOffset), iLine(other.iLine)
			{
			}
			/// �R�s�[�I�y���[�^
			TFunc& operator=(const TFunc& other)
			{
				strFuncName = other.strFuncName;
				strParamNames = other.strParamNames;
				iOffset = other.iOffset;
				iLine = other.iLine;
				return *this;
			}
		};
		CAscii			strName;	///< include�����t�@�C����
		CAscii			abContents;	///< �t�@�C�����e�{NULL�I�[
		CVectorT<TFunc>	atFunc;		///< �֐����
		/// �R���X�g���N�^
		TFileInfo(void) {}
		/// �R�s�[�R���X�g���N�^
		TFileInfo(const TFileInfo& other) : strName(other.strName), abContents(other.abContents), atFunc(other.atFunc)
		{
		}
		/// �R�s�[�I�y���[�^
		TFileInfo& operator=(const TFileInfo& other)
		{
			strName = other.strName;
			abContents = other.abContents;
			atFunc = other.atFunc;
			return *this;
		}
	};


	/// �R���X�g���N�^
	CTmlDocuments(void)
	{
	}

	/// Empty
	void Empty(void)
	{
		m_aaFileInfo.RemoveAll();
		TTRACE0( "TML::CDocuments::������\n" );
	}

	/**
	 * [�擾] ���݃��x���擾
	 *	@return ���x���B
	 */
	INT_PTR GetLevel(void) const
	{
		return m_aaFileInfo.GetSize();
	}

	/**@ingroup TML
	 * TinyMacroLang �������ʊǗ�
	 */
	struct TFuncInfoRes
	{
		LPCSTR	lpszFuncAdds;					///< �֐��̂���A�h���X(�֐��̓��� { �̒���)
		CAscii	strFileName;					///< �t�@�C����	
		INT_PTR	iFuncLine;						///< �֐��̂���t�@�C�s
		const CAsciiVector*	pastrParamNames;	///< �֐��̈�����(Array�̎Q��)
		/// �R���X�g���N�^
		TFuncInfoRes(void) : lpszFuncAdds(NULL), iFuncLine(0), pastrParamNames(NULL)
		{
		}
		/**
		 * [�m�F] �f�[�^�������Ă��邩�H
		 *	@retval true �f�[�^����B
		 *	@retval false �Ȃ��B
		 */
		bool HasData(void) const
		{
			return lpszFuncAdds != NULL;
		}
	};

	/**
	 * [����] �֐�����.
	 *		���x���̍���(��ɒǉ����ꂽ)�R���e���c���珇�Ɍ������܂��B
	 *	@param lpszFunc �֐���
	 *	@return �֐����B���� HasData()�̌��ʂ�false�Ȃ疢�����B
	 */
	TFuncInfoRes FindFunc(LPCSTR lpszFunc) const
	{
		INT_PTR iDim1 = m_aaFileInfo.GetSize();
		for( INT_PTR i = iDim1 - 1; i >= 0; i-- )
		{
			const CFileInfoArray& fiv = m_aaFileInfo.At(i);
			INT_PTR iDim2 = fiv.GetSize();
			for( INT_PTR j = 0; j < iDim2; j++ )
			{
				const TFileInfo& T = fiv.At(j);
				INT_PTR iDimF = T.atFunc.GetSize();
				for( INT_PTR f = 0; f < iDimF; f++ )
				{
					const TFileInfo::TFunc& X = T.atFunc[f];
					if ( X.strFuncName.Compare(lpszFunc) == 0 )
					{
						//������
						TFuncInfoRes t;
						t.strFileName		= T.strName;
						t.lpszFuncAdds		= T.abContents.begin();
						t.lpszFuncAdds		+= X.iOffset;
						t.iFuncLine			= X.iLine;
						t.pastrParamNames	= &X.strParamNames;
						return t;
					}
				}
			}
		}
		TFuncInfoRes t;
		return t;
	}

	/**
	 * [����] �t�@�C������.
	 *		�ێ����Ă���t�@�C���ɁA�w��̃t�@�C���������邩�`�F�b�N���܂��B�B
	 *	@param lpszFile �t�@�C����
	 *	@retval NULL �������B
	 *	@retval NULL�ȊO �t�@�C�����̎Q�ƃ|�C���^�B
	 */
	const TFileInfo* FindFileName(LPCSTR lpszFile) const
	{
		INT_PTR iDim1 = m_aaFileInfo.GetSize();
		for( INT_PTR i = 0; i < iDim1; i++ )
		{
			const CFileInfoArray& fiv = m_aaFileInfo.At(i);
			INT_PTR iDim2 = fiv.GetSize();
			for( INT_PTR j = 0; j < iDim2; j++ )
			{
				const TFileInfo &T = fiv.At(j);
				if ( T.strName.Compare(lpszFile) == 0 )
				{
					return &T;
				}
			}
		}
		return NULL;
	}

	/**
	 * [����] ���x���{�P
	 */
	void IncLevel(void)
	{
		CFileInfoArray A;
		m_aaFileInfo.Add(A);
		TTRACE1( "TML::CDocuments::���x��++ NowLvl=%d\n", GetLevel() );
	}

	/**
	 * [����] ���x���|�P
	 *	@retval ture �����B
	 *	@retval false ���s�B
	 */
	bool DecLevel(void)
	{
		INT_PTR l = m_aaFileInfo.GetSize();
		if ( l <= 0 )
		{
			return false;
		}
		//
		m_aaFileInfo.SetSize( l - 1 );
		TTRACE1( "TML::CDocuments::���x��-- NowLvl=%d\n", GetLevel() );
		return true;
	}

	/**
	 * [�ݒ�] ���x���w��.
	 *		���x�����w��̈ʒu�։�����B
	 *	@param level ���x���B		
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool DecLevels(INT_PTR level)
	{
		while( level < GetLevel() )
		{
			if ( ! DecLevel() )
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * [�ǉ�] �t�@�C�����ǉ�.
	 *		���̃��x���Ƀt�@�C����ǉ�����
	 *	@param[out] _tFuncInfo �ǉ���́A���s���ׂ����B
	 *	@param[in] lpszName	�t�@�C����
	 *	@param[in] lpszData �f�[�^���e
	 *	@return ���ʁB
	 */
	CTmlResult Add(TFuncInfoRes& _tFuncInfo, LPCSTR lpszName, LPCSTR lpszData)
	{
		size_t l = m_aaFileInfo.GetSize();
		if ( l == 0 )
		{
			//�}�N���R���e���c���x�����ُ�ł��B
			CTmlResult r(Operation_Non, Result_InvalidMacroContentsLevel);
			return r;
		}
		if ( FindFileName(lpszName) != NULL )
		{
			//���ɓo�^����Ă���i��񓯂��C���N���[�h�t�@�C���w�莩�̂̓G���[�ł͂Ȃ��j�B
			TTRACE1( "[%s]�͊��ɓo�^����Ă��܂��B\n", lpszName );
			return Result_Nop;
		}
		// ���\�z
		TFileInfo tFileInfo;
		tFileInfo.strName = lpszName;
		tFileInfo.abContents = lpszData;
		//���e�`�F�b�N
		CTmlResult r = m_AnalyzeFile(tFileInfo);
		if ( r.IsError() )
		{
			return r;
		}
		CFileInfoArray &A = m_aaFileInfo.Ref(l - 1);
		A.Add(tFileInfo);
		_tFuncInfo.strFileName		= lpszName;
		_tFuncInfo.lpszFuncAdds		= tFileInfo.abContents;
		_tFuncInfo.iFuncLine		= 1;
		_tFuncInfo.pastrParamNames	= NULL;
		return Result_Success;
	}

private:
	typedef CVectorT<TFileInfo> CFileInfoArray;			///< ��̃t�@�C���̔z��^
	typedef CVectorT<CFileInfoArray> CFileInfoArrays;	///< ��̃t�@�C���̓񎟌��z��^
	CFileInfoArrays m_aaFileInfo;						///< ��̃t�@�C���̔z��

	/**
	 * �t�@�C�����e����͂��܂��B
	 *	@param[in,out] _tFileInfo IN�Ńt�@�C�����e Out�Ŋ֐����
	 *	@return ���ʁB
	*/
	CTmlResult m_AnalyzeFile(TFileInfo& _tFileInfo)
	{
		CTmlResult res;
		CTmlGrammar gram(_tFileInfo.abContents.begin(), _tFileInfo.abContents.end(), 1);
		CStr strToken;
		CTmlGrammar::CParts parts;
		//
		while(true)
		{
			parts = gram.GetNextParts();
			if ( parts.GetKind() == CTmlGrammar::FINAL )
			{
				break;// �I���
			}
			if ( parts.GetKind() == CTmlGrammar::ERROR_DEPTH )
			{
				res = Result_InvalidParenthesesPosition;// ���ʃG���[
				break;
			}
			if ( parts.GetKind() == CTmlGrammar::OPEN_CHAR )
			{
				if ( ! gram.SkipoutBlock() )
				{
					res = Result_NotCloseParentheses;//���ĂȂ�
					break;
				}
				continue;
			}
			if ( parts.GetKind() == CTmlGrammar::STRING )
			{
				continue;
			}
			if ( parts.GetKind() == CTmlGrammar::CLOSE_CHAR )
			{
				ASSERTLIB( false );
			}
			CAscii s = parts.GetString();
			if ( s == ";" )
			{
				continue;
			}
			if ( s == "}" )
			{
				//�ˑR����
				ASSERTLIB( false );
				continue;
			}
			if ( s.Compare("func") != 0 )
			{
				continue;
			}
			//
			parts = gram.GetNextParts();
			if ( parts.GetKind() != CTmlGrammar::TOKEN )
			{
				res = Result_NotName;
				break;
			}
			CAscii funcName = parts.GetString();
			parts = gram.GetNextParts();
			if ( parts.GetAt() != '(' )
			{
				res = Result_NotFoundParentheses;//�֐����̌�� ( ���Ȃ�
				break;
			}
			//
			TFileInfo::TFunc tFunc;
			tFunc.strFuncName = funcName;
			while(true)
			{
				parts = gram.PeekNextParts();
				if ( parts.GetKind() != CTmlGrammar::TOKEN )
				{
					break;
				}
				gram.GetNextParts();
				CAscii s = parts.GetString();
				tFunc.strParamNames.Add(s);
				parts = gram.PeekNextParts();
				if ( parts.GetAt() != ',' )
				{
					break;
				}
				gram.GetNextParts();
			}	
			if ( ! gram.SkipoutBlock() )
			{
				res = Result_NotCloseParentheses;//���ĂȂ�
				break;
			}
			parts = gram.GetNextParts();
			if ( parts.GetAt() != '{' )
			{
				res = Result_NotFoundParentheses;// { ���Ȃ�
				break;
			}
			tFunc.iOffset	= gram.GetPos();
			tFunc.iLine		= gram.GetLine();
			if ( ! gram.SkipoutBlock() )
			{
				res = Result_NotCloseParentheses;//���ĂȂ�
				break;
			}
			_tFileInfo.atFunc.Add(tFunc);
		}
		if ( res.IsError() )
		{
			CTmlResult r(Scene_Func, Operation_FindingFunc, res.GetResult());
			r.SetLine(_tFileInfo.strName, parts.GetLine());
			res = r;
		}
		return res;
	}

	friend class CTmlDocumentsTest;
};



}; // TNB
