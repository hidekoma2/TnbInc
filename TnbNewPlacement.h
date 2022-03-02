#pragma once
/**
 *@file
 * PlacementNew/Delete関係のヘッダ
 *
 *	@note	TnbDef.h はインクルードしていません。
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#pragma warning( disable : 4100 )



#ifndef _TnbDOXYGEN	//Document作成用シンボル

/*
 * PLACEMENT NEW
 *	@param P ポインタ 
 *	@return Pと同じ値
 */
template<typename T>
inline LPVOID __cdecl operator new(size_t, T* P)
{
	return (P); 
}

/*
 * PLACEMENT DELETE
 *	@param V ダミー 
 *	@param P ダミー
 */
template<typename T>
inline void __cdecl operator delete(LPVOID V, T* P)
{
	return; 
}

#endif	//Document作成用シンボル



//TNB Library
namespace TNB
{



/**@ingroup NEWDEL
 * PLACEMENT NEW.
 *		指定型のポインタに対し、コンストラクタを実行します。
 *	@note	本関数を使うと、すでに確保されたメモリに対して、コンストラクタを実行することが出来ます。
 *			動的にサイズの変化するコレクションでの使用や、
 *			特殊な方法で確保したメモリに対して使用の使用を想定しています。
 *	@attention	メモリは確保しません。すでに確保されたメモリに対し処理します。
 *	@attention	本関数実行したメモリは、解放前に必ず、 PlacementDelete() を実行させます。
 *	@see PlacementDelete
 *	@par使用例
 *		<table><tr><td><pre>
 *		
 *		CStr* ps = (CStr*)malloc(sizeof(CStr) * 2); //new以外で確保
 *		PlacementNew(&ps[0]);                     //コンストラクタ実行
 *		PlacementNew(&ps[1]);
 *		ps[0] = "ABCD";	                          //普通のクラスインスタンスとして使用可能		
 *		ps[1] = ps[0] + "ABCD";	
 *			;
 *		PlacementDelete(&ps[0]);                  //デストラクタ実行
 *		PlacementDelete(&ps[1]);
 *		free(ps);                                 //メモリ解放
 *		</pre></td></tr></table>
 *
 *	@param P 指定型のポインタ
 *	@par必要ファイル
 *			TnbNewPlacement.h
 *	@date 06/08/07 新規
 */
template<typename TYP>
inline void PlacementNew(TYP* P)
{
	#ifndef _DEBUG
		new(P) TYP;
	#else
		VERIFY( (new(P) TYP) == P );
	#endif
}



/**@ingroup NEWDEL
 * PLACEMENT NEW.
 *		指定型の配列に対し、コンストラクタを実行します。
 *	@note	本関数を使うと、すでに確保されたメモリに対して、コンストラクタを実行することが出来ます。
 *			動的にサイズの変化するコレクションでの使用や、
 *			特殊な方法で確保したメモリに対して使用の使用を想定しています。
 *	@attention	メモリは確保しません。すでに確保されたメモリに対し処理します。
 *	@attention	本関数実行したメモリは、解放前に必ず、 PlacementDelete() を実行させます。
 *	@see PlacementDelete
 *	@par使用例
 *		<table><tr><td><pre>
 *		
 *		CStr* ps = (CStr*)malloc(sizeof(CStr) * 5); //new以外で確保
 *		PlacementNew(5, ps);                     //コンストラクタ実行
 *		ps[0] = "ABCD";	                          //普通のクラスインスタンスとして使用可能		
 *		ps[1] = ps[0] + "ABCD";	
 *			;
 *		PlacementDelete(5, ps);
 *		free(ps);                                 //メモリ解放
 *		</pre></td></tr></table>
 *
 *	@param iLen 配列数
 *	@param P 指定型の配列
 *	@par必要ファイル
 *			TnbNewPlacement.h
 *	@date 06/08/30 新規
 */
template<typename TYP>
inline void PlacementNew(int iLen, TYP* P)
{
	for ( int i = 0; i < iLen; i++ )
	{
		#ifndef _DEBUG
			new(P) TYP;
		#else
			VERIFY( (new(P) TYP) == P );
		#endif
		P++;
	}
}



/**@ingroup NEWDEL
 * PLACEMENT DELETE.
 *		指定型のポインタに対し、デストラクタを実行します。
 *	@attention メモリは解放しません。
 *	@see PlacementNew
 *	@param P 指定型のポインタ 
 *	@par必要ファイル
 *			TnbNewPlacement.h
 *	@date 06/08/07 新規
 */
template<typename TYP>
inline void PlacementDelete(TYP* P)
{
	P->~TYP();
}



/**@ingroup NEWDEL
 * PLACEMENT DELETE.
 *		指定型の配列に対し、デストラクタを実行します。
 *	@attention メモリは解放しません。
 *	@see PlacementNew
 *	@param iLen 配列数
 *	@param P 指定型の配列
 *	@par必要ファイル
 *			TnbNewPlacement.h
 *	@date 06/08/30 新規
 */
template<typename TYP>
inline void PlacementDelete(int iLen, TYP* P)
{
	for ( int i = 0; i < iLen; i++ )
	{
		P->~TYP();
		P++;
	}
}



};//TNB



#ifndef _TnbUSINGNAMESPACE_DISABLE
 using namespace TNB;
#endif
