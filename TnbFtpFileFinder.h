#pragma once
/**
 *@file
 * FTP�t�@�C�������֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbFtpFile.h"
#include "TnbFileFinder.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE INTERNET
 * FTP�t�@�C�������N���X
 *
 *		�w��̃f�B���N�g������t�@�C�����������邽�߂̃N���X�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbFtpFileFinder.h
 *
 *	@date 06/11/30 �V�K
 */
class CFtpFileFinder : public CAbstractFileFinder
{
	DEFSUPER(CAbstractFileFinder);
public:

	/// �R���X�g���N�^
	CFtpFileFinder(void) : _super()
	{
	}

	/**
	 * [�ݒ�] ������
	 *	@note �C���X�^���X�쐬��A�����̑O�Ɉ�x�͂��̃��\�b�h�����s���܂��B
	 *	@param sec			�I�[�v�������Z�b�V�����n���h���B
	 *	@param dwFlags	�I�[�v���̎�ʂ��w�肷��r�b�g�}�X�N�l�B��q�̕\���Q�ƁB
	 *					<BR>		INTERNET_FLAG_HYPERLINK		�ēǂݍ��݂��ׂ����ǂ����̔��f���ɗL�������E�ŏI�X�V�������T�[�o����Ԃ���Ȃ��ꍇ�����I�ɍēǂݍ��݂���B
	 *					<BR>		INTERNET_FLAG_NEED_FILE		�t�@�C�����L���b�V���ł��Ȃ��ꍇ�Ɉꎞ�t�@�C�����쐬����B
	 *					<BR>		INTERNET_FLAG_RELOAD		�v�������t�@�C���A�I�u�W�F�N�g�A�f�B���N�g�����X�g���L���b�V������擾����̂łȂ��A�����I�ɃT�[�o����ēǂݍ��݂���B
	 *					<BR>		INTERNET_FLAG_RESYNCHRONIZE	FTP���\�[�X���T�[�o����ēǂݍ��݂���B
	 *					<BR>		INTERNET_FLAG_NO_CACHE_WRITE �L���b�V���ɏ������܂Ȃ��B
	 */
	void Initialize(CFtpSession& sec, DWORD dwFlags = 0)
	{
		Finish();
		m_hInet = sec;
		m_dwFlags = dwFlags;
	}

	/// �f�X�g���N�^
	virtual ~CFtpFileFinder(void)
	{
		OnFinish();
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
		Finish();
		if ( m_hInet.IsValid() )
		{
			m_hFind = m_hInet.FindFirstFile(lpszName, &_data, m_dwFlags, 0);
		}
		return !! m_hFind.IsValid();
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
		if ( m_hFind.IsValid() )
		{
			return !! m_hFind.FindNextFile(&_data);
		}
		return false;
	}

	/**
	 * [�ʒm] �����I���ʒm
	 *	@note �������̏ꍇ�A�������~���܂��i OnNext() ���\�b�h�� false ��Ԃ��悤�ɂȂ�܂��j�B
	 */
	virtual void OnFinish(void)
	{
		m_hFind.Close();
	}

private:
	CInternetHandle		m_hInet;		///< �C���^�[�l�b�g�n���h��
	CInternetHandle		m_hFind;		///< �����n���h��
	DWORD				m_dwFlags;		///< �����t���O
	friend class CFtpFileFinderTest;
};



}; //TNB



