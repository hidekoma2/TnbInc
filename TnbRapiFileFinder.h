#pragma once
/**
 *@file
 * RAPI �t�@�C�������֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbFileFinder.h"
#include "TnbStr.h"
#include "TnbRapi.h"



//TNB Library
namespace TNB
{



/**@ingroup RAPI FILE
 * RAPI �t�@�C�������N���X
 *
 *		�w��̃f�B���N�g������t�@�C�����������邽�߂̃N���X�ł��B
 *
 *	@note RAPI �̐ڑ���Ԃ� CRapi::IsConnected() �Ŋm�F�ł��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbRapiFileFinder.h
 *			\n Windows Mobile �� PocketPC �� SDK �� ./Activesync/inc/rapi.h �����K�v�ł��B
 *
 *	@date 09/04/23 �V�K
 */
class CRapiFileFinder : public CAbstractFileFinder
{
	DEFSUPER(CAbstractFileFinder);
public:

	/// �R���X�g���N�^
	CRapiFileFinder(void) : _super(), m_hFind(INVALID_HANDLE_VALUE)
	{
		CRapi::Startup();
	}

	/// �f�X�g���N�^
	virtual ~CRapiFileFinder(void)
	{
		Finish();
	}

protected:

	/**
	 * [�ʒm] �����J�n�ʒm
	 *	@param[out] _data ���������ꍇ�A������񂪊i�[����܂��B
	 *	@param[in] lpszName �����p�X�w��B���C���h�J�[�h�ɂ��w�肪�K�v�ł��B
	 *	@retval true ��ȏ㔭���B
	 *	@retval false �������B
	 */
	virtual bool OnStart(WIN32_FIND_DATA& _data, LPCTSTR lpszName)
	{
		if ( ! CRapi::IsSupported() ) { return false; }
		Finish();
		CE_FIND_DATA ceFd;
		m_hFind = ::CeFindFirstFile(CUnicode(lpszName), &ceFd);
		bool r = (m_hFind != INVALID_HANDLE_VALUE);
		if ( r )
		{
			m_CeFindDataToWin32(_data, ceFd);
		}
		return r;
	}

	/**
	 * [�ʒm] �������ʒm
	 *	@note Find() �� true �̏ꍇ�A���ɂ��̃��\�b�h���g�p���܂��B���̌�J��Ԃ���
	 *			�{���\�b�h�g�����Ƃŕ����̏��𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _data ���������ꍇ�A������񂪊i�[����܂��B
	 *	@retval true ��ȏ㔭���B
	 *	@retval false �������B
	 */
	virtual bool OnNext(WIN32_FIND_DATA& _data)
	{
		if ( ! CRapi::IsSupported() ) { return false; }
		if ( m_hFind != INVALID_HANDLE_VALUE )
		{
			CE_FIND_DATA ceFd;
			bool r = !! ::CeFindNextFile(m_hFind, &ceFd);
			if ( r )
			{
				m_CeFindDataToWin32(_data, ceFd);
			}
			return r;
		}
		return false;
	}

	/**
	 * [�ʒm] �����I���ʒm
	 *	@note �������̏ꍇ�A�������~���܂��i OnNext() ���\�b�h�� false ��Ԃ��悤�ɂȂ�܂��j�B
	 */
	virtual void OnFinish(void)
	{
		if ( CRapi::IsSupported() && m_hFind != INVALID_HANDLE_VALUE )
		{
			::CeFindClose(m_hFind);
			m_hFind = INVALID_HANDLE_VALUE;
		}
	}

private:
	HANDLE				m_hFind;	///< �����n���h��
	/// �\���̕ϊ�
	void m_CeFindDataToWin32(WIN32_FIND_DATA& _winFd, const CE_FIND_DATA& ceFd)
	{
		Zero(_winFd);
		_winFd.dwFileAttributes = ceFd.dwFileAttributes;
		_winFd.ftCreationTime = ceFd.ftCreationTime;
		_winFd.ftLastAccessTime = ceFd.ftLastAccessTime;
		_winFd.ftLastWriteTime = ceFd.ftLastWriteTime;
		_winFd.nFileSizeHigh = ceFd.nFileSizeHigh;
		_winFd.nFileSizeLow = ceFd.nFileSizeLow;
		CStr s = ceFd.cFileName;
		STRLIB::Copy(_winFd.cFileName, s);
	}
	friend class CCeFileFinderTest;
};



}; //TNB



