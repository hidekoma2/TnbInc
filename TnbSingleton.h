#pragma once
/**
 *@file
 * �V���O���g���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbSharedMemory.h"
#include "TnbStr.h"



//TNB Library
namespace TNB
{



/**@ingroup ETC
 * �V���O���g���쐬�}�N��
 *
 *		�V���O���g���ɂ������N���X�̃R���X�g���N�^�̐錾�����̃}�N���ɂ��܂��B
 *		��������ƁAnew �� �X�^�b�N��ł̃C���X�^���X���֎~���AGetInstance()���\�b�h��
 *		�ǉ�����܂��B
 *
 *	@par�g�p��
 *	\code
 *		���錾��
 *		//CFoo�̓V���O���g���ł����g�p�ł��Ȃ�
 *		class CFoo
 *		{
 *			CStr m_strData;
 *			SINGLETON_CONSTRUCTOR(CFoo)
 *			{
 *				m_strData = "ABC";
 *			}
 *		public:
 *			CStr GetData(void){ return n_strData; }			
 *		};
 *	\endcode
 *	\code
 *		���쐬��
 *		CFoo *pcFoo = CFoo::GetInstance();		
 *		printf( pcFoo->GetData() );
 *	\endcode
 *
 *	@note ���̃}�N�����������A�V���O���g���ɂ���N���X�ɂ́A
 *			���ɃR���X�g���N�^�����Ȃ��ł��������B
 *
 *	@param CLS �V���O���g�����������N���X�B
 *
 *	@par�K�v�t�@�C��
 *			TnbSingleton.h
 * 
 *	@date	06/04/26 �V�K�쐬
 */
#define SINGLETON_CONSTRUCTOR( CLS )				\
		public:										\
		static CLS * GetInstance(void)				\
		{											\
			return CSingletonT<CLS>::Get(_T(#CLS));	\
		}											\
		private:									\
		CCopyImpossible m_cCopyImpossible;			\
		friend class CSingletonT<CLS>;				\
		CLS(void)



/**@ingroup ETC
 * �V���O���g���e���v���[�g
 *
 *		���ڎg�p���邱�Ƃ͗L��܂���B SINGLETON_CONSTRUCTOR()�}�N������g�p����܂��B
 *		�C���X�^���X�͍쐬���܂���B
 *
 *	@see SINGLETON_CONSTRUCTOR
 *
 *	@date 06/04/26 �V�K�쐬
 *	@date 06/07/13 �����ɗv�������ƁA��肱�ڂ��\�����������B
 *	@date 11/04/01 �������t���O�̎��������C���B
 */
template<typename TYP>
class CSingletonT
{
	/**
	 * �R���X�g���N�^
	 *	@note �쐬�ł��Ȃ��悤��Private�錾���Ă��܂��B
	 */
	CSingletonT(void);
	/// ���Ǘ��^
	struct TInfo
	{
		LONG	isInit;		///< �������p
		LONG	lRef;		///< �Ǘ��p
		TYP*	pType;		///< �B��̎��̂̎Q��
	};
	/// ���̍폜��p�N���X
	class CDestory
	{
		TInfo*	m_pInfo;	///< ���̎Q��
	public:
		/// �R���X�g���N�^
		CDestory(TInfo* P) : m_pInfo(P) {}
		/// �f�X�g���N�^
		~CDestory(void)
		{
			if ( m_pInfo->pType != NULL )
			{
				delete m_pInfo->pType;
				m_pInfo->pType = NULL;
			}
		}
	};

public:

	/**
	 * [�擾] �C���X�^���X�擾
	 *	@param lpszName �N���X��
	 *	@return TYP�̃C���X�^���X�|�C���^
	 */
	static TYP* Get(LPCTSTR lpszName)
	{
		static CSharedMemoryManager s_cSharedMem(true);
		TInfo* pInfo = NULL;
		if ( ! s_cSharedMem.HasMemory() )
		{
			CStr str = CStr::Fmt(_T("_!Tnb1Ton_%08X_%s"), ::GetCurrentProcessId(), lpszName);
			int iRc = s_cSharedMem.Open(str, sizeof(TInfo));
			if ( iRc < 0 )
			{
				ASSERTLIB(false);
				return NULL;
			}
			pInfo = static_cast<TInfo*>(s_cSharedMem.Get());
			if ( iRc == 0 )
			{	//�V�K��
				pInfo->pType = new TYP;
				static CDestory s_cDest(pInfo);	//TYP�폜�p�I�u�W�F�N�g
				pInfo->lRef++;
				pInfo->isInit = 1;	//�������ς�
			}
			else
			{	//����
				while ( pInfo->isInit != 1 )
				{
					::Sleep(1);
				}
				pInfo->lRef++;
			}
		}
		else
		{
			pInfo = static_cast<TInfo*>(s_cSharedMem.Get());
			while ( pInfo->isInit != 1 )
			{
				::Sleep(1); 
			}
			pInfo->lRef++;
		}
		return pInfo->pType;
	}
};



};//TNB
