#pragma once
/**
 *@file
 * �A�C�R���Ǘ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMap.h"
#include "TnbStr.h"
#include <Shellapi.h>


//TNB Library
namespace TNB
{



/**@ingroup ICON
 * �A�C�R���Ǘ��N���X
 *
 *		�{�N���X���g�p���ăA�C�R�������[�h����ƁA�J�����ׂ��A�C�R���̊J���R���h���܂��B
 *		�܂��A���\�[�XID�ƃn���h�����֘A�t���ċL�����Ă������߁A�L���b�V���I�Ȏg�������\�ł��B
 *
 *		�J�[�\�����A�C�R���Ɠ��l�ɖ{�N���X�ŊǗ����邱�Ƃ��\�ł��B
 *
 * @note	�{�N���X�̃C���X�^���X�Ŏ擾���� �A�C�R���n���h���́A�g�p��A API�� DestroyIcon() ���g�p�����A
 *			Destroy() ���g�p���j�����܂�(�C���X�^���X���폜���Ă��n���h���͔j������܂�) �B
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *		
 *		CIconManager m_icon;
 *				;
 *		void foo1(){
 *		   HICON h1 = m_icon.LoadResorce(-IDI_MAIN);  //�������A�C�R���n���h��
 *		       ;	
 *		   HICON h2 = m_icon[-IDI_MAIN];   // ������Load�����n���h���������܂��B		
 *		       ;
 *		}
 *		</pre></td></tr></table>
 *
 *	@par�K�v�t�@�C��
 *			TnbIconManager.h
 *
 *	@date 06/06/14 �V�K�쐬
 *	@date 07/10/29 �\������
 */
class CIconManager : CCopyImpossible
{
	CMapT<int, HICON>		m_mapIdIcon;		///< �����K�v�̂���HICON
	CVectorT<HICON>			m_vIcon;			///< �����K�v�̂���HICON
	/// �n���h�����[�h
	HICON m_Load(int iIconResourceId, HINSTANCE hIns)
	{
		LPCTSTR lpsz = NULL;
		int cx = 0;
		int cy = 0;
		if ( iIconResourceId < 0 )
		{
			lpsz = MAKEINTRESOURCE(-iIconResourceId);
			cx = ::GetSystemMetrics(SM_CXSMICON);	// �X���[���A�C�R���̕�
			cy = ::GetSystemMetrics(SM_CYSMICON); 
		}
		else
		{
			lpsz = MAKEINTRESOURCE(iIconResourceId);
		}
		HANDLE r = ::LoadImage(hIns, lpsz, IMAGE_ICON, cx, cy, LR_DEFAULTCOLOR);
		if ( r == NULL )
		{
			r = ::LoadImage(hIns, lpsz, IMAGE_CURSOR, cx, cy, LR_DEFAULTCOLOR);
		}
		return static_cast<HICON>(r);
	}
protected:

	/**
	 * [�j��] �n���h���j��.
	 *	@note �n���h�����A�C�R�����J�[�\�����`�F�b�N���Ĕj�����܂��B
	 *	@param h �A�C�R�����J�[�\���̃n���h��
	 */
	void m_Destroy(HICON h)
	{
#ifndef _WIN32_WCE
		ICONINFO ii;
		if ( ::GetIconInfo(h, &ii) )
		{
			ii.fIcon ? ::DestroyIcon(h) : ::DestroyCursor(h);
		}
#else
		::DestroyIcon(h);
#endif
	}

public:

#ifndef _WIN32_WCE

	/** �V�X�e���̃A�C�R��ID�l
	 * @note LoadSystem() �Ŏg�p���܂��B */
	enum ESystemIconId
	{
		E_APPLICATION	= (INT_PTR)(IDI_APPLICATION),	///< ����̃A�v���P�[�V�����A�C�R�� 
		E_ASTERISK		= (INT_PTR)(IDI_ASTERISK),		///< E_INFORMATION �Ɠ����ł� 
		E_ERROR			= (INT_PTR)(IDI_ERROR),			///< �X�g�b�v�}�[�N�̃A�C�R���i�č��̕��s�җp�M���̐ԂƓ����ŁA����J���Đ��~���Ă���f�U�C���ł��j 
		E_EXCLAMATION	= (INT_PTR)(IDI_EXCLAMATION),	///< E_WARNING �Ɠ����ł� 
		E_HAND			= (INT_PTR)(IDI_HAND),			///< E_ERROR �Ɠ����ł� 
		E_INFORMATION	= (INT_PTR)(IDI_INFORMATION),	///< ���A�C�R���i�����o���̒��� i �̕����j 
		E_QUESTION		= (INT_PTR)(IDI_QUESTION),		///< �^�╄�̃A�C�R�� 
		E_WARNING		= (INT_PTR)(IDI_WARNING),		///< ���Q���̃A�C�R�� 
		E_WINLOGO		= (INT_PTR)(IDI_WINLOGO),		///< Windows ���S�̃A�C�R�� 
	};

#endif

	/// �R���X�g���N�^
	CIconManager(void) 
	{
	}

	/// �f�X�g���N�^
	virtual ~CIconManager(void)
	{
		DestroyAll();
	}

	/**
	 * [�J��] �S�n���h���J��.
	 *		�{�C���X�^���X�ŊǗ����Ă���n���h�������ׂĊJ�����܂��B
	 */
	void DestroyAll(void)
	{
		const CMapT<int, HICON>::CConstValuesAdapter& avu = m_mapIdIcon.GetValuesAdapter();
		loop ( i, avu.GetSize() )
		{
			m_Destroy(avu.At(i));
		}
		loop ( i, m_vIcon.GetSize() )
		{
			m_Destroy(m_vIcon[i]);
		}
		m_mapIdIcon.RemoveAll();
		m_vIcon.RemoveAll();
	}

	/**
	 * [�J��] �n���h���J��.
	 *	@param iIconResourceId �{�C���X�^���X�Ń��[�h�������\�[�XID�B
	 *	@retval true �����B
	 *	@retval false ���s�i�{�C���X�^���X�ŊǗ����Ă��Ȃ����\�[�XID���w�肳�ꂽ�j�B
	 */
	bool Destroy(int iIconResourceId)
	{
		if ( m_mapIdIcon.HasKey(iIconResourceId) )
		{
			m_Destroy(m_mapIdIcon[iIconResourceId]);
		}
		return m_mapIdIcon.RemoveKey(iIconResourceId);
	}

	/**
	 * [�J��] �n���h���J��.
	 *	@param hIcon �{�C���X�^���X�ŊǗ����Ă���A�C�R���n���h���B
	 *	@retval true �����B
	 *	@retval false ���s�i�{�C���X�^���X�ŊǗ����Ă��Ȃ��n���h�����w�肳�ꂽ�j�B
	 */
	bool Destroy(HICON hIcon)
	{
		const CMapT<int, HICON>::CConstValuesAdapter& av = m_mapIdIcon.GetValuesAdapter();
		INDEX i = av.Find(hIcon);
		if ( i != INVALID_INDEX )
		{
			m_mapIdIcon.Remove(i);
			m_Destroy(hIcon);
			return true;
		}
		i = m_vIcon.Find(hIcon);
		if ( i != INVALID_INDEX )
		{
			m_vIcon.Remove(i);
			m_Destroy(hIcon);
			return true;
		}
		return false;
	}

	/**
	 * [�擾] ���\�[�X����擾
	 *	@param iIconResourceId �A�C�R�����\�[�XID�B �~-1 �����l���w�肷�邱�ƂŁA ������ ICON �����[�h�ł��܂��B
	 *	@param hIns �C���X�^���X�B
	 *	@retval NULL�ȊO �A�C�R���n���h���B API�� DestroyIcon() �͎g�p���Ȃ��ł��������B
	 *	@retval NULL �G���[�i���݂��Ȃ��j
	 */
	HICON LoadResource(int iIconResourceId, HINSTANCE hIns)
	{
		HICON hIcon = NULL;
		if ( ! m_mapIdIcon.Lookup(iIconResourceId, hIcon) )
		{
			hIcon = m_Load(iIconResourceId, hIns);
			if ( hIcon != NULL)
			{
				m_mapIdIcon[iIconResourceId] = hIcon;
			}
		}
		return hIcon;
	}

	/**
	 * [�擾] ���\�[�X����擾
	 *	@note �C���X�^���X�n���h���́A GetInstanceHandle(EI_Icon) �̒l���g�p����܂��B
	 *	@param iIconResourceId �A�C�R�����\�[�XID�B �~-1 �����l���w�肷�邱�ƂŁA ������ ICON �����[�h�ł��܂��B
	 *	@retval NULL�ȊO �A�C�R���n���h���B API�� DestroyIcon() �͎g�p���Ȃ��ł��������B
	 *	@retval NULL �G���[�i���݂��Ȃ��j
	 */
	HICON LoadResource(int iIconResourceId)
	{
		return LoadResource(iIconResourceId, GetInstanceHandleByTnb(EI_Icon));
	}

	/**
	 * [�擾] ���\�[�X����擾
	 *	@note �C���X�^���X���w�肵�Ȃ� LoadResource() �Ɠ������������܂��B
	 *	@param iIconResourceId �A�C�R�����\�[�XID
	 *	@retval NULL�ȊO �A�C�R���n���h���B API�� DestroyIcon() �͎g�p���Ȃ��ł��������B
	 *	@retval NULL �G���[�i���݂��Ȃ��j
	 */
	HICON operator[](int iIconResourceId)
	{
		return LoadResource(iIconResourceId);
	}

#ifndef _WIN32_WCE

	/**
	 * [�擾] �V�X�e������擾
	 *	@param systemId �V�X�e���A�C�R��ID�B
	 *	@retval NULL�ȊO �A�C�R���n���h���B API�� DestroyIcon() �͎g�p���Ȃ��ł��������B
	 *	@retval NULL �G���[�i���݂��Ȃ��j
	 */
	HICON LoadSystem(ESystemIconId systemId)
	{
		return ::LoadIcon(NULL, reinterpret_cast<LPCTSTR>(systemId));
	}

	/**
	 * [�擾] �t�@�C�����璊�o�\��.
	 *	@note �w��̃t�@�C�����̃��\�[�X����A�C�R�����擾���܂��B
	 *	@note �C���X�^���X�n���h���́A GetInstanceHandle(EI_Process) �̒l���g�p����܂��B
	 *	@param lpszFile �t�@�C�����B�t�@�C���́A EXE DLL ICO �t�@�C�����w��ł��܂��B
	 *	@retval 1�ȏ� �t�@�C�����̃A�C�R�����B
	 *	@retval 0 �G���[�i���݂��Ȃ��j
	 */
	INT_PTR ExtractNumber(LPCTSTR lpszFile)
	{
		return reinterpret_cast<INT_PTR>(::ExtractIcon(GetInstanceHandleByTnb(EI_Process), lpszFile, UINT_MAX));
	}

	/**
	 * [�擾] �t�@�C�����璊�o.
	 *	@note �w��̃t�@�C�����̃��\�[�X����A�C�R�����擾���܂��B
	 *	@note �C���X�^���X�n���h���́A GetInstanceHandle(EI_Process) �̒l���g�p����܂��B
	 *	@param lpszFile �t�@�C�����B�t�@�C���́A EXE DLL ICO �t�@�C�����w��ł��܂��B
	 *	@param iconIndex �C���f�b�N�X�B ExtractNumber() �œ�����u�A�C�R�����v�������w��ł��܂��B
	 *	@retval NULL�ȊO �A�C�R���n���h���B API�� DestroyIcon() �͎g�p���Ȃ��ł��������B
	 *	@retval NULL �G���[�i���݂��Ȃ��j
	 */
	HICON Extract(LPCTSTR lpszFile, UINT iconIndex)
	{
		HICON hIcon = ::ExtractIcon(GetInstanceHandleByTnb(EI_Process), lpszFile, iconIndex);
		if ( hIcon != NULL )
		{
			m_vIcon.Add(hIcon);
		}
		return hIcon;
	}

#endif

	/**
	 * [�w��] �n���h���Ǘ�.
	 *		�w��̃n���h����{�C���X�^���X�ŊǗ�����悤�ɂ��܂��B
	 *	@note �{�N���X�ȊO�ŃA�C�R���n���h�����쐬���A�C���X�^���X�ŊǗ����������ꍇ�A�{���\�b�h���g���܂��B
	 *	@param hIcon �A�C�R���n���h���B �����Ŏw�肵���A�C�R���n���h���� API�� DestroyIcon() ���g�p���Ȃ��ł��������B
	 */
	void Attach(HICON hIcon)
	{
		const CMapT<int, HICON>::CConstValuesAdapter& av = m_mapIdIcon.GetValuesAdapter();
		if ( av.Find(hIcon) == INVALID_INDEX )
		{
			if ( m_vIcon.Find(hIcon) == INVALID_INDEX )
			{
				m_vIcon.Add(hIcon);
			}
		}
	}


#if 0
	/**
	 * [�擾] �A�C�R���n���h���擾.
	 *	@note �w��̃t�@�C�����̃��\�[�X����A�C�R�����擾���܂��B
	 *	@param lpszFile �t�@�C�����B�t�H���_���w��\�B
	 *	@param uAddFlags	�ǉ��t���O�B�ȉ��̃V���{����I���\�B<BR>
	 *						SHGFI_LARGEICON : �傫���T�C�Y�̃A�C�R��<BR>
	 *						SHGFI_SMALLICON : �������T�C�Y�̃A�C�R��<BR>
	 *						SHGFI_OPENICON  : �I�[�v����Ԃ̃A�C�R��<BR>
	 *	@retval NULL�ȊO �A�C�R���n���h���B API�� DestroyIcon() �͎g�p���Ȃ��ł��������B
	 *	@retval NULL �G���[�i���݂��Ȃ��j
	 */
	HICON LoadFileInfo(LPCTSTR lpszFile, UINT uAddFlags = SHGFI_LARGEICON)
	{
		SHFILEINFO info = { 0 };
		UINT uFlags = SHGFI_ICON | uAddFlags;
		::SHGetFileInfo(lpszFile, 0, &info, sizeof(SHFILEINFO), uFlags);
		if ( info.hIcon != NULL )
		{
			m_vIcon.Add(info.hIcon);
		}
		return info.hIcon;
	}
#endif
};



};///TNB

