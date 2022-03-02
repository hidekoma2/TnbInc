#pragma once
/**
 *@file
 * �t�@�C���֌W�̃w�b�_
 *
 *		�L���b�V���t���A�ǂݍ��݃N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbReader.h"
#include "TnbQueue.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * �L���b�V���ǂݍ��݃N���X
 *
 *	@note �w��� IReader ����ǂݍ��݂܂��B���̍ہA��s�ǂݏo���A���ǂݏo���̃T�|�[�g���s���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbCacheReader.h
 *
 *	@date 10/02/03 �V�K�쐬
 */
class CCacheReader : public IReader, CCopyImpposible
{
public:

	CCacheReader(void) : m_pReader(NULL), m_minimum(100)
	{
	}

	void SetEnvironmrent(IReport* pRep, size_t minimum = 100)
	{
		m_pReader = pRep;
		m_minimum = minimum;
	}

	/**
	 * [�擾] �I�[�v�����擾
	 *	@return ���O
	 */
	virtual LPCTSTR GetOpenName(void) const
	{
		return (m_pReader != NULL) ? m_pReport->GetOpenName() : _T("");
	}

	/**
	 * [����] �N���[�Y
	 */
	virtual void Close(void)
	{
		if ( m_pReader != NULL )
		{
			m_pReader->Close();
		}
	}

	/**
	 * [�m�F] �ǂݍ��݉\��
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	virtual bool CanRead(void) const
	{
		return (m_pReader != NULL) ? m_pReport->CanRead() : false;
	}

	/**
	 * [�擾] �T�C�Y�擾
	 *	@retval �O�ȏ� �T�C�Y
	 *	@retval �}�C�i�X �擾���s
	 */
	virtual LONGLONG GetSize(void) const
	{
		return (m_pReader != NULL) ? m_pReport->GetSize() : 0;
	}

	/**
	 * [����] �V�[�N.
	 *		�A�N�Z�X�|�C���^���ړ����܂��B
	 *	@param llOffset �A�N�Z�X�|�C���^�̈ړ����B
	 *	@param eSeek �ړ��J�n�A�N�Z�X�|�C���^�ʒu�B�ȗ������ TOP �ɂȂ�܂��B
	 *	@retval �O�ȏ�	�V�[�N�����B���l�́A���݂̃t�@�C���|�C���^
	 *	@retval �}�C�i�X �V�[�N���s
	 */
	virtual LONGLONG Seek(LONGLONG llOffset, ESeekMode eSeek = TOP) const
	{
		return (m_pReader != NULL) ? m_pReport->Seek(llOffset, eSeek) : -1;
	}

	/**
	 * [�擾] �ǂݍ���
	 *	@param[in] size �ǂݍ��݃T�C�Y�B
	 *	@param[out] _P �ǂݍ��݃A�h���X�B size ���������߂�̈�ł���K�v���L��܂��B
	 *	@return �ǂݍ��߂��T�C�Y�B
	 *	@throw CReadFailureException �ǂݍ��݂Ɏ��s����ƁA�X���[����܂��B
	 */
	virtual size_t Read(size_t size, LPVOID _P) const
	{
		return 0;
	}

	/**
	 * [�擾] ���߂��ǂ�
	 *	@note �t�@�C���|�C���^�͕ω����܂���B
	 *	@param size �ǂݍ��݃T�C�Y�B
	 *	@param offset �ǂݍ��݊J�n�A�N�Z�X�|�C���^�ʒu
	 *	@return ���߂��ǂ݂����f�[�^�B
	 *	@throw CReadFailureException �ǂݍ��݂Ɏ��s����ƁA�X���[����܂��B
	 */
	const IConstCollectionT<BYTE>& Peek(size_t size = 0, LONGLONG offset = 0) const
	{
	}





private:
	size_t							m_minimum;	///< �ǂݍ��ݍŏ�����
	IReader*						m_pReader;	///< �Ώۂ̃��[�_�[
	LONGLONG						m_pointer;	///< ��ǂ݃o�b�t�@�̃t�@�C���|�C���^
	mutable CVectorT<BYTE>			m_buffer;	///< ��ǂ݂̃o�b�t�@
};

