#pragma once
/**
 *@file
 * �t�@�C�������֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbFileFinder.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * �f�B�[�v�t�@�C�������N���X
 *
 *		�f�B���N�g����[���@�荞��Ō�������N���X�ł��B
 *		�{�N���X���p�����A OnStartFindFolder() , OnFoundFolder() , OnFoundFile() �ɏ������������܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDeepFileFinder.h
 *
 *	@date 09/07/16 �V�K�쐬
 *	@date 09/09/29 OndemandStop() �Ŏw�肵���l�� Execute() �̖߂�l�ɂȂ�Ȃ��o�O���C���B 
 */
template<typename TYP = CFileFinder>
class CDeepFileFinderT
{
public:

	/// �R���X�g���N�^
	CDeepFileFinderT(void) : m_depth(0), m_continue(true), m_result(0)
	{
		#ifdef _DEBUG
			TYP ff;
			CAbstractFileFinder* P = &ff;		// �����ŃG���[�o��ꍇ�A TYP �� CAbstractFileFinder �̃T�u�N���X���w�肵�Ė����\��������܂��B
			IgnoreUnusedValue(P);
		#endif
	}

	/**
	 * [���s] �����J�n.
	 *	@param lpszPath �������J�n����p�X���w�肵�܂��B
	 *	@return ���U���g( OndemandStop() �Ŏw�肵���l)�B
	 */
	DWORD Execute(LPCTSTR lpszPath)
	{
		m_depth = 0;
		m_continue = true;
		m_result = 0;
		m_Find(lpszPath);
		return m_result;
	}

protected:

	/**
	 * [�˗�] ��~�˗�.
	 *		�����̒�~���w�����܂��B
	 *	@note OnStartFindFolder(), OnFoundFolder() , OnFoundFile() ������A�g�p���܂��B
	 *	@param result ���U���g�B Execute() �̖߂�l���w��ł��܂��B
 	 */
	void OndemandStop(DWORD result)
	{
		m_result = result;
		m_continue = false;
	}

	/**
	 * [�ʒm] �t�H���_�����J�n�ʒm.
	 *		�t�H���_�̌����J�n��ʒm���܂��B
	 *	@note �������~�������ꍇ�A OndemandStop() ���Ăяo���Ă��������B
	 *	@param[in,out] finder �����Ɏg�p����t�@�C���_�[�B�K�v�ɉ����Đݒ�����Ă��������B
	 *	@param[in] lpszTargetPath �Ώۂ̃t�H���_��
	 *	@param[in] depth �[���B�ŏ��̃t�H���_�w���P�ŁA�ȉ��[���Ȃ邲�ƂɁ{�P����܂��B
	 */
	virtual void OnStartFindFolder(TYP& finder, LPCTSTR lpszTargetPath, int depth)
	{
	}

	/**
	 * [�ʒm] �t�H���_�����ʒm.
	 *		�t�H���_�𔭌�����ƒʒm���܂��B
	 *	@note �������~�������ꍇ�A OndemandStop() ���Ăяo���Ă��������B
	 *	@param lpszFoundFolder ���������t�H���_��
	 *	@param data ���������t�H���_�̏��
	 *	@param depth �[���B�ŏ��̃t�H���_�w���P�ŁA�ȉ��[���Ȃ邲�ƂɁ{�P����܂��B
	 *	@retval true ���̃t�H���_���̌������s���B
	 *	@retval false ���̃t�H���_���̌����͍s��Ȃ��B
	 */
	virtual bool OnFoundFolder(LPCTSTR lpszFoundFolder, const WIN32_FIND_DATA& data, int depth)
	{
		::OutputDebugString(_T("OnFoundFolder("));
		::OutputDebugString(lpszFoundFolder);
		::OutputDebugString(_T(")\n"));
		return true;
	}

	/**
	 * [�ʒm] �t�@�C�������ʒm.
	 *		�t�@�C���𔭌�����ƒʒm���܂��B
	 *	@note �������~�������ꍇ�A OndemandStop() ���Ăяo���Ă��������B
	 *	@param lpszFoundName ���������t�@�C����
	 *	@param data ���������t�@�C���̏��
	 *	@param depth �[���B�ŏ��̃t�H���_�w���P�ŁA�ȉ��[���Ȃ邲�ƂɁ{�P����܂��B
	 */
	virtual void OnFoundFile(LPCTSTR lpszFoundName, const WIN32_FIND_DATA& data, int depth)
	{
		::OutputDebugString(_T("OnFoundFile("));
		::OutputDebugString(lpszFoundName);
		::OutputDebugString(_T(")\n"));
	}

private:
	int		m_depth;	///< �[��
	bool	m_continue;	///< ���s�t���O
	DWORD	m_result;	///< ���U���g
	// ��������
	void m_Find(LPCTSTR lpszPath)
	{
		m_depth++;
		if ( m_continue )
		{
			TYP ff;
			OnStartFindFolder(ff, lpszPath, m_depth);
			if ( m_continue && ff.Start(lpszPath, _T("\\*")) )
			{
				do
				{
					CStr fileName = ff->cFileName; 
					CStr fullPath;
					fullPath.Format(_T("%s\\%s"), lpszPath, fileName);
					if ( (ff->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
					{
						// Dir
						if ( OnFoundFolder(fullPath, ff.Get(), m_depth) )
						{
							m_Find(fullPath);
						}
					} 
					else
					{
						OnFoundFile(fullPath, ff.Get(), m_depth);
					}
				} 
				while ( m_continue && ff.Next() );
			}
		}
		m_depth--;
	}
};



/**@ingroup FILE
 * �f�B�[�v�t�@�C�������N���X
 *
 *		�f�B���N�g����[���@�荞��Ō�������N���X�ł��B
 *		�{�N���X���p�����AOnNewFiner() , OnFoundFolder() , OnFoundFile() �ɏ������������܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbDeepFileFinder.h
 *
 *	@date 09/07/16 �V�K�쐬
 */
typedef CDeepFileFinderT<> CDeepFileFinder;



}; // TNB
