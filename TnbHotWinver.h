#pragma once
/**
 *@file
 * �ŐV WINVER �֌W�̃w�b�_.
 *
 *	XP �ȍ~�̂ݗL���� API ���g�p���Ă���N���X���g�p����ꍇ�A�A
 *		�ȉ��̂悤�ɁA����Include����� TnbHotWinver.h ���C���N���[�h���Ă��������B
 *		\code
 *		// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
 *		//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
 *		//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
 *			: 
 *			:
 *		#if _MSC_VER > 1000
 *		#pragma once
 *		#endif // _MSC_VER > 1000
 *
 *		#define VC_EXTRALEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂��B
 *
 *		#include <TnbHotWinver.h>		//�������� �����ɒǉ�����B
 *
 *		#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
 *		#include <afxext.h>         // MFC �̊g������
 *			:
 *			:
 *		\endcode
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��

#if _MSC_VER < 1300
 #define CFileDialog CFileDialog2
#endif

#if defined(WINVER)
 #if WINVER < 0x0501
  #undef WINVER
 #endif
#endif

#if ! defined(WINVER)
 #define WINVER 0x0501
#endif
#define _WIN32_WINNT 0x0501

#endif


