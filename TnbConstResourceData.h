#pragma once
/**
 *@file
 * ���\�[�X�f�[�^�Q�Ɗ֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbCollection.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT WINAPI
 * ���\�[�X�f�[�^�Q�ƃN���X
 *
 *		���\�[�X�f�[�^�𒼐ڎQ�Ƃ��邽�߂̃N���X�ł��B
 *
 *	@note �f�[�^�́A�P���ȃ������ǂݏo�������s���܂���̂ŁA�X���b�h�Z�[�u�ɂȂ��Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbConstResourceData.h
 *
 *	@date 09/09/09 �V�K�쐬
 */
class CConstResourceData : public IConstCollectionT<BYTE>, public ISequenceCollectionT<BYTE>
{
public:

	/**@ingroup COLLECT WINAPI
	 * ���\�[�X�l�ݒ�⏕�N���X
	 *
	 *		CConstResourceData::Attach() �̈����̃T�|�[�g�N���X�ł��B
	 *		���ڃC���X�^���X���`����K�v�͂���܂���B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbConstResourceData.h
	 *
	 *	@date 09/09/09 �V�K�쐬
	 */
	class CValue
	{
	public:

		/**
		 * �R���X�g���N�^.
		 *	@param lpsz ������w��
		 */
		CValue(LPCTSTR lpsz) : m_lpsz(lpsz)
		{
		}

		/**
		 * �R���X�g���N�^.
		 *	@param id ID �w��
		 */
		CValue(int id) : m_lpsz(MAKEINTRESOURCE(id))
		{
		}

		/**
		 * [�擾] ���擾.
		 *	@return ���
		 */
		operator LPCTSTR() const
		{
			return m_lpsz;
		}

	private:
		LPCTSTR m_lpsz;
	};


	//----------------------------


	/// �R���X�g���N�^
	CConstResourceData(void) : m_size(0), m_pData(NULL)
	{
	}

	/** 
	 * [�r��] ���b�N
	 *	@note �{�N���X�ł͉������܂���B
	 *	@param dwTime �_�~�[�B
	 *	@return ��� true
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const
	{
		return true;
	}

	/**
 	 * [�r��] �A�����b�N.
	 *	@note �{�N���X�ł͉������܂���B
	 */
	virtual void Unlock(void) const
	{
	}

	/**
	 * [�擾] �v�f���擾.
	 *	@return �v�f��
	 */
	virtual size_t GetSize(void) const
	{
		return m_size;
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �v�f�̎Q��
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	virtual const BYTE& At(INDEX index) const
	{
		if ( IsInRange(index) )
		{
			return m_pData[index];
		}
		throw CIndexOutOfBoundsException();
	}

	/**
	 * [�擾] �f�[�^�A�h���X�擾.
	 *	@note	�擾�����A�h���X�� delete �ŊJ�����Ă͂����܂���B
	 *	@note	�擾�����A�h���X�� Load Unload ����Ɩ����ɂȂ�܂��B
	 *	@return �A�h���X�B�m�ۂ��Ă��Ȃ��ꍇ�ANULL���Ԃ�܂��B
	 */
	virtual const BYTE* ReferBuffer(void) const
	{
		return m_pData;
	}

	/**
	 * [�ݒ�] �A�^�b�`.
	 *		�w��̃��\�[�X�̃f�[�^���A�{�C���X�^���X�ɃA�^�b�`���܂��B
	 *	@param name ���\�[�X�����w�肵�܂��B������ł� ID (���l) �ł��w��\�ł��B
	 *	@param type ���\�[�X�^�C�v���w�肵�܂��B������ł� ID (���l) �ł��w��\�ł��B
	 *			�ȗ�����ƁA RT_RCDATA ���K�p����܂��B\n
	 *			���\�[�X�^�C�v�́A�ȉ��̕W���̒l���g�p�ł��܂��B
	 *			<table>
	 *			<tr> <td>�l</td>				<td>�Ӗ�</td>										</tr>
	 *			<tr> <td>RT_ACCELERATOR</td>	<td>�A�N�Z�����[�^�e�[�u��</td>						</tr>
	 *			<tr> <td>RT_ANICURSOR</td>		<td>�A�j���[�V�����J�[�\��</td>						</tr>
	 *			<tr> <td>RT_ANIICON</td>		<td>�A�j���[�V�����A�C�R��</td>						</tr>
	 *			<tr> <td>RT_BITMAP</td>			<td>�r�b�g�}�b�v���\�[�X</td>						</tr>
	 *			<tr> <td>RT_CURSOR</td>			<td>�n�[�h�E�F�A�ˑ��̃J�[�\�����\�[�X</td>			</tr>
	 *			<tr> <td>RT_DIALOG</td>			<td>�_�C�A���O�{�b�N�X</td>							</tr>
	 *			<tr> <td>RT_FONT</td>			<td>�t�H���g���\�[�X</td>							</tr>
	 *			<tr> <td>RT_FONTDIR</td>		<td>�t�H���g�f�B���N�g�����\�[�X</td>				</tr>
	 *			<tr> <td>RT_GROUP_CURSOR</td>	<td>�n�[�h�E�F�A�Ɉˑ����Ȃ��J�[�\�����\�[�X</td>	</tr>
	 *			<tr> <td>RT_GROUP_ICON</td>		<td>�n�[�h�E�F�A�Ɉˑ����Ȃ��A�C�R�����\�[�X</td>	</tr>
	 *			<tr> <td>RT_HTML</td>			<td>HTML �h�L�������g</td>							</tr>
	 *			<tr> <td>RT_ICON</td>			<td>�n�[�h�E�F�A�ˑ��̃A�C�R�����\�[�X</td>			</tr>
	 *			<tr> <td>RT_MENU</td>			<td>���j���[���\�[�X</td>							</tr>
	 *			<tr> <td>RT_MESSAGETABLE</td>	<td>���b�Z�[�W�e�[�u���̃G���g��</td>				</tr>
	 *			<tr> <td>RT_RCDATA</td>			<td>�A�v���P�[�V������`�̃��\�[�X�i ���f�[�^�j</td></tr>
	 *			<tr> <td>RT_STRING</td>			<td>������e�[�u���̃G���g��</td>					</tr>
	 *			<tr> <td>RT_VERSION</td>		<td>�o�[�W�������\�[�X</td>							</tr>
	 *			</table>
	 *	@param hModule �C���X�^���X�n���h�����w�肵�܂��B�ȗ�����ƁA���݂̃v���Z�X�̃n���h�����g�p����܂��B
	 *	@param wLanguage ����B�ȗ�����ƁA���݂̃X���b�h�Ɋ֘A�t�����Ă��錾�ꂪ�g�p����܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Attach(CValue name, CValue type = RT_RCDATA, HMODULE hModule = NULL, WORD wLanguage = 0)
	{
		Detach();
		if ( hModule == NULL )
		{
			hModule = GetInstanceHandleByTnb();
		}
		HRSRC h = NULL;
		if ( wLanguage == 0 )
		{
			h = ::FindResource(hModule, name, type);
		}
		else
		{
			h = ::FindResourceEx(hModule, type, name, wLanguage);
		}
		if ( h != NULL )
		{
			HGLOBAL hGM = ::LoadResource(hModule, h);
			m_size = ::SizeofResource(hModule, h);
			m_pData = static_cast<const BYTE*>(::LockResource(hGM));
		}
		return ! IsEmpty();
	}

	/**
	 * [�ݒ�] �f�^�b�`.
	 *		���\�[�X�f�[�^���f�^�b�`���܂��B�{�C���X�^���X�̃T�C�Y�͂O�ɂȂ�܂��B
	 */
	void Detach(void)
	{
		m_size = 0;
		m_pData = NULL;
	}

private:
	size_t		m_size;		///< �T�C�Y
	const BYTE*	m_pData;	///< �f�[�^
};



}; // TNB

