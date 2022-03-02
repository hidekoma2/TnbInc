#pragma once
/**
 *@file
 * ���ԊǗ��֌W�̃w�b�_
 *
 *		��̎��Ԃ��y�A�ŊǗ�����N���X���L��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbRange.h"



//TNB Library
namespace TNB
{



/**@ingroup TIME
 * ���ԊǗ��N���X
 *
 *		��̎��Ԃ��y�A�ŊǗ�����N���X�ł��B
 *
 *	@note First�l,Last�l�� CFileTimeEx ���g�p���܂��B
 *
 *	@note Diff�l �� LONGLONG ���g�p���܂��B�P�ʂ̓~���b�ɂȂ�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbRangeTime.h
 *
 *	@date 06/10/13	�V�K�쐬
 */
typedef CRangeT<CFileTimeEx,LONGLONG> CRangeTime;



}; /// TNB

