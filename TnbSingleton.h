#pragma once
/**
 *@file
 * シングルトン関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbSharedMemory.h"
#include "TnbStr.h"



//TNB Library
namespace TNB
{



/**@ingroup ETC
 * シングルトン作成マクロ
 *
 *		シングルトンにしたいクラスのコンストラクタの宣言をこのマクロにします。
 *		そうすると、new や スタック上でのインスタンスを禁止し、GetInstance()メソッドが
 *		追加されます。
 *
 *	@par使用例
 *	\code
 *		○宣言時
 *		//CFooはシングルトンでしか使用できない
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
 *		○作成時
 *		CFoo *pcFoo = CFoo::GetInstance();		
 *		printf( pcFoo->GetData() );
 *	\endcode
 *
 *	@note このマクロを実装し、シングルトンにするクラスには、
 *			他にコンストラクタを作らないでください。
 *
 *	@param CLS シングルトン化したいクラス。
 *
 *	@par必要ファイル
 *			TnbSingleton.h
 * 
 *	@date	06/04/26 新規作成
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
 * シングルトンテンプレート
 *
 *		直接使用することは有りません。 SINGLETON_CONSTRUCTOR()マクロから使用されます。
 *		インスタンスは作成しません。
 *
 *	@see SINGLETON_CONSTRUCTOR
 *
 *	@date 06/04/26 新規作成
 *	@date 06/07/13 同時に要求されると、取りこぼす可能性があった。
 *	@date 11/04/01 初期化フラグの持ち方を修正。
 */
template<typename TYP>
class CSingletonT
{
	/**
	 * コンストラクタ
	 *	@note 作成できないようにPrivate宣言しています。
	 */
	CSingletonT(void);
	/// 情報管理型
	struct TInfo
	{
		LONG	isInit;		///< 初期化用
		LONG	lRef;		///< 管理用
		TYP*	pType;		///< 唯一の実体の参照
	};
	/// 実体削除専用クラス
	class CDestory
	{
		TInfo*	m_pInfo;	///< 情報の参照
	public:
		/// コンストラクタ
		CDestory(TInfo* P) : m_pInfo(P) {}
		/// デストラクタ
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
	 * [取得] インスタンス取得
	 *	@param lpszName クラス名
	 *	@return TYPのインスタンスポインタ
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
			{	//新規だ
				pInfo->pType = new TYP;
				static CDestory s_cDest(pInfo);	//TYP削除用オブジェクト
				pInfo->lRef++;
				pInfo->isInit = 1;	//初期化済み
			}
			else
			{	//既存
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
