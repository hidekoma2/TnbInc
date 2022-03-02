#pragma once
/**
 *@file
 * PlacementNew/Delete�֌W�̃w�b�_
 *
 *	@note	TnbDef.h �̓C���N���[�h���Ă��܂���B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#pragma warning( disable : 4100 )



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

/*
 * PLACEMENT NEW
 *	@param P �|�C���^ 
 *	@return P�Ɠ����l
 */
template<typename T>
inline LPVOID __cdecl operator new(size_t, T* P)
{
	return (P); 
}

/*
 * PLACEMENT DELETE
 *	@param V �_�~�[ 
 *	@param P �_�~�[
 */
template<typename T>
inline void __cdecl operator delete(LPVOID V, T* P)
{
	return; 
}

#endif	//Document�쐬�p�V���{��



//TNB Library
namespace TNB
{



/**@ingroup NEWDEL
 * PLACEMENT NEW.
 *		�w��^�̃|�C���^�ɑ΂��A�R���X�g���N�^�����s���܂��B
 *	@note	�{�֐����g���ƁA���łɊm�ۂ��ꂽ�������ɑ΂��āA�R���X�g���N�^�����s���邱�Ƃ��o���܂��B
 *			���I�ɃT�C�Y�̕ω�����R���N�V�����ł̎g�p��A
 *			����ȕ��@�Ŋm�ۂ����������ɑ΂��Ďg�p�̎g�p��z�肵�Ă��܂��B
 *	@attention	�������͊m�ۂ��܂���B���łɊm�ۂ��ꂽ�������ɑ΂��������܂��B
 *	@attention	�{�֐����s�����������́A����O�ɕK���A PlacementDelete() �����s�����܂��B
 *	@see PlacementDelete
 *	@par�g�p��
 *		<table><tr><td><pre>
 *		
 *		CStr* ps = (CStr*)malloc(sizeof(CStr) * 2); //new�ȊO�Ŋm��
 *		PlacementNew(&ps[0]);                     //�R���X�g���N�^���s
 *		PlacementNew(&ps[1]);
 *		ps[0] = "ABCD";	                          //���ʂ̃N���X�C���X�^���X�Ƃ��Ďg�p�\		
 *		ps[1] = ps[0] + "ABCD";	
 *			;
 *		PlacementDelete(&ps[0]);                  //�f�X�g���N�^���s
 *		PlacementDelete(&ps[1]);
 *		free(ps);                                 //���������
 *		</pre></td></tr></table>
 *
 *	@param P �w��^�̃|�C���^
 *	@par�K�v�t�@�C��
 *			TnbNewPlacement.h
 *	@date 06/08/07 �V�K
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
 *		�w��^�̔z��ɑ΂��A�R���X�g���N�^�����s���܂��B
 *	@note	�{�֐����g���ƁA���łɊm�ۂ��ꂽ�������ɑ΂��āA�R���X�g���N�^�����s���邱�Ƃ��o���܂��B
 *			���I�ɃT�C�Y�̕ω�����R���N�V�����ł̎g�p��A
 *			����ȕ��@�Ŋm�ۂ����������ɑ΂��Ďg�p�̎g�p��z�肵�Ă��܂��B
 *	@attention	�������͊m�ۂ��܂���B���łɊm�ۂ��ꂽ�������ɑ΂��������܂��B
 *	@attention	�{�֐����s�����������́A����O�ɕK���A PlacementDelete() �����s�����܂��B
 *	@see PlacementDelete
 *	@par�g�p��
 *		<table><tr><td><pre>
 *		
 *		CStr* ps = (CStr*)malloc(sizeof(CStr) * 5); //new�ȊO�Ŋm��
 *		PlacementNew(5, ps);                     //�R���X�g���N�^���s
 *		ps[0] = "ABCD";	                          //���ʂ̃N���X�C���X�^���X�Ƃ��Ďg�p�\		
 *		ps[1] = ps[0] + "ABCD";	
 *			;
 *		PlacementDelete(5, ps);
 *		free(ps);                                 //���������
 *		</pre></td></tr></table>
 *
 *	@param iLen �z��
 *	@param P �w��^�̔z��
 *	@par�K�v�t�@�C��
 *			TnbNewPlacement.h
 *	@date 06/08/30 �V�K
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
 *		�w��^�̃|�C���^�ɑ΂��A�f�X�g���N�^�����s���܂��B
 *	@attention �������͉�����܂���B
 *	@see PlacementNew
 *	@param P �w��^�̃|�C���^ 
 *	@par�K�v�t�@�C��
 *			TnbNewPlacement.h
 *	@date 06/08/07 �V�K
 */
template<typename TYP>
inline void PlacementDelete(TYP* P)
{
	P->~TYP();
}



/**@ingroup NEWDEL
 * PLACEMENT DELETE.
 *		�w��^�̔z��ɑ΂��A�f�X�g���N�^�����s���܂��B
 *	@attention �������͉�����܂���B
 *	@see PlacementNew
 *	@param iLen �z��
 *	@param P �w��^�̔z��
 *	@par�K�v�t�@�C��
 *			TnbNewPlacement.h
 *	@date 06/08/30 �V�K
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
