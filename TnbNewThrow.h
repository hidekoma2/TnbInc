#pragma once
/**
 *@file
 *@ingroup NEWDEL
 * �������m�ۗ�O�֌W�̃w�b�_.
 *
 *	@attention �{�w�b�_�́A�C���N���[�h���邾���ŉe��������܂��B
 *
 *		�{�w�b�_���C���N���[�h����ƁAnew / malloc() �ł̃������m�ێ��s���ɁA
 *		std::bad_alloc ��O�X���[����悤�ɂȂ�܂��B
 *
 *	@note	�f�o�b�O�r���h���A�}�N�� "_TnbNEW_Log_ON" ����`����Ă���ƁAnew / delete �̃��O���\������܂��B
 *	
 *	@note	TnbDef.h �̓C���N���[�h���Ă��܂���B
 *
 *	@note placement new �́u PlacementNew() �v���g���܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

#ifdef _DEBUG

//=== �f�o�b�O�r���h��

#include "TnbNewDebug.h"


#else

//=== �����[�X�r���h��

#include <new.h>
#include <new>

namespace TNB{
namespace NEW
{

class CThrowNew
{
	/// �������̊��蓖�ĂɎ��s�����ꍇ�ɌĂяo����O�����֐�
	static int HandleProgramMemoryDepletion(size_t)
	{
		throw std::bad_alloc();
	}
public:
	CThrowNew(void)
	{
		::_set_new_handler(HandleProgramMemoryDepletion);
		::_set_new_mode(1);
	}
};

namespace
{
	CThrowNew s_cThrowNew;
};

}; //NEW
}; //TNB


#endif // _DEBUG

#endif //_TnbDOXYGEN
