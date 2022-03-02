#pragma once
/**
 *@file
 * CAB�t�@�C���A�[�J�C�o�[�֌W�̃w�b�_
 *
 *		CAB�t�@�C���̍쐬�A�𓀂��邽�߂̋@�\������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDeepFileFinder.h"
#include "TnbSet.h"
#include "TnbMap.h"
#include "TnbFileName.h"
#include "TnbProcessHandle.h"
#include "TnbStrAdder.h"
#include "TnbTemporaryDirectory.h"
#include "TnbFilePathManager.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * CAB�t�@�C���A�[�J�C�o�[
 *
 *	@par�g�p��
 *	@code
 *		CCabfileArchiver cab;
 *		cab.AddSourceFile(_T("C:\\Home\\temp\\TT\\a.dat"), _T(""));
 *		cab.AddSourceDirectory(_T("C:\\Home\\temp\\TT"), _T("\\GG"));
 *		cab.Attract(_T("C:\\Home\\temp\\ABC.cab"));
 *
 *		CCabfileArchiver::Extract(_T("C:\\Home\\temp\\BB"), _T("C:\\Home\\temp\\ABC.cab"));
 *	@endcode
 *	@par�K�v�t�@�C��
 *			TnbCabfileArchiver.h
 *
 *	@date 12/08/15 �V�K�쐬
 */
class CCabfileArchiver
{
public:

	/**
	 * [�ݒ�] �\�[�X�f�B���N�g���ǉ�.
	 *		�w��̃f�B���N�g���ȉ������k�Ώۂɂ��܂��B
	 *	@param lpszPath �ǉ�����p�X��
	 *	@param lpszCabPath CAB���̃p�X���w�肵�܂��B�w�肵�Ȃ��ƃ��[�g�ɂȂ�܂��B�@
	 */
	void AddSourceDirectory(LPCTSTR lpszPath, LPCTSTR lpszCabPath = NULL)
	{
		CMyFinder f(&m_map, lpszCabPath);
		f.Execute(lpszPath);
	}

	/**
	 * [�ݒ�] �\�[�X�t�@�C���ǉ�.
	 *		�w��̃t�@�C�������k�Ώۂɂ��܂��B
	 *	@param lpszFileName �ǉ�����t�@�C����
	 *	@param lpszCabPath CAB���̃p�X���w�肵�܂��B�w�肵�Ȃ��ƃ��[�g�ɂȂ�܂��B�@
	 *	@retval true ����
	 *	@retval false ���s�B�t�@�C�����Ȃ��Ǝ��s���܂��B
	 */
	bool AddSourceFile(LPCTSTR lpszFileName, LPCTSTR lpszCabPath = NULL)
	{
		CFileName fn = lpszFileName;
		if ( fn.IsExist() )
		{
			CStr s = lpszCabPath;
			m_map[s].Insert(fn.GetFullName());
			return true;
		}
		return false;
	}

	/**
	 * [�폜] �폜.
	 *		AddSourceDirectory() , AddSourceFile() �������̂�j�����܂��B�t�@�C���ɂ͉e������܂���B
	 */
	void RemoveAll(void)
	{
		m_map.RemoveAll();
	}

	/**
	 * [�쐬] �i�[.
	 *		�V���� cab�t�@�C�����쐬���܂��B
	 *	@param lpszCabFileName CAB�t�@�C����
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Attract(LPCTSTR lpszCabFileName)
	{
		CStrAdder sa;
		loop ( i, m_map )
		{
			CStr cabPath = m_map.At(i).first;
			const CSingleSetT<CStr>& v = m_map.At(i).second;
			if ( v.GetSize() > 0 )
			{
				sa.AddFormat(_T(".Set DestinationDir=\"%s\"\r\n"), cabPath);
				loop ( j, v )
				{
					sa.AddFormat(_T("\"%s\"\r\n"), v[j]);
				}
			}
		}
		CAscii a = CStr(sa);
		LPCSTR lpsz = a;
		CTemporaryFile temp;
		CTemporaryDirectory tempDir;
		CStr pt = tempDir.Create();
		CStr fn = temp.Create();
		if ( CFileWriter::NewFile(fn, a.GetLength(), lpsz) )
		{
			CFilePathManager fpm;
			::SetCurrentDirectory(pt);
			CStr cl;
			cl.Format(_T("makecab /F \"%s\""), fn);
			CProcessHandle ph;
			if ( ph.Create(NULL, cl, false) )
			{
				ph.WaitForExit();
				CFileName fn = pt + _T("\\disk1\\1.cab");
				return fn.CopyTo(lpszCabFileName);
			}
		}
		return false;
	}

	/**
	 * [�쐬] ��.
	 *		cab�t�@�C�����𓀂��܂��B
	 *	@param lpszPath �𓀐�B
	 *	@param lpszCabFileName �𓀂���CAB�t�@�C�����B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	static bool Extract(LPCTSTR lpszPath, LPCTSTR lpszCabFileName)
	{
		CFileName fn = lpszPath;
		fn.MakeDirectory();
		CStr cl;
		cl.Format(_T("expand \"%s\" -F:* \"%s\""), lpszCabFileName, lpszPath);
		CProcessHandle ph;
		if ( ph.Create(NULL, cl, false) )
		{
			ph.WaitForExit();
			return ph.GetExitCode() == 0;
		}
		return false;
	}

private:
	/// �����N���X
	class CMyFinder : public CDeepFileFinder
	{
		CMapT< CStr, CSingleSetT<CStr> >*	m_pMap;
		CStr								m_cabPath;
	public:
		CMyFinder(CMapT< CStr, CSingleSetT<CStr> >*	p, LPCTSTR lpszCabPath) : m_pMap(p), m_cabPath(lpszCabPath)
		{
		}
	protected:
		// [�ʒm] �t�@�C�������ʒm.
		virtual void OnFoundFile(LPCTSTR lpszFoundName, const WIN32_FIND_DATA& data, int depth)
		{
			CFileName fn = lpszFoundName;
			CFileName path = fn.GetPathName();
			CStr cp = m_cabPath;
			if ( depth > 1 )
			{
				CStr ex;
				loop ( i, depth - 1 )
				{
					if ( ex.IsEmpty() )
					{
						ex = path.GetFileName();
					}
					else
					{
						ex = path.GetFileName() + _T("\\") + ex;
					}
					path = path.GetPathName();
				}
				if ( cp.IsEmpty() )
				{
					cp = ex;
				}
				else
				{
					cp = cp + _T("\\") + ex;
				}
			}
			(*m_pMap)[cp].Insert(fn.GetFullName());
		}
	};
	CMapT< CStr, CSingleSetT<CStr> >	m_map;	///< ���
};



}; // TNB
