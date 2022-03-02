#pragma once
/**
 *@file
 * �L���b�V�������ݏ����֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbWriter.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * �L���b�V���������݃N���X
 *
 *	@note �ǂ݂��݂̓L���b�V�����܂���B
 *
 *	\par �g����:
 *	@code
 *		CStr s;
 *		CFileWriter fw;
 *		fw.New("NewFile.dat");
 *		CTickCount t1;
 *		try
 *		{
 *			CCacheWriter cw;
 *			cw.SetWriter(&fw, 1024 * 1024);	//�L���b�V���T�C�Y�� 1MB
 *			loop ( i, GetTextSize() )
 *			{
 *				s = GetText(i);
 *				fw.Write(s.GetLength(), s.operator LPCTSTR());
 *			}
 *		}
 *		catch ( CWriteFailureException& e )
 *		{
 *			e.OnCatch();
 *		}
 *	@endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbCacheWriter.h
 *
 *	@date 11/08/17 �V�K�쐬
 */
class CCacheWriter : public IWriter, CCopyImpossible
{
public:

	/// �R���X�g���N�^ 
	CCacheWriter(void) : m_pWriter(NULL)
	{
	}

	/// �f�X�g���N�^
	~CCacheWriter(void)
	{
		m_Push();
	}

	/**
	 * [�ݒ�] ���C�^�[�ݒ�
	 *	@param pfw �I�[�v�����Ă���Writer���w�肵�܂��B
	 *				Open���Ă��Ȃ��I�u�W�F�N�g��n���Ǝ��s���܂��B
	 *				pfw �͎Q�Ƃ��܂��̂ŁA�g�p���͔j�����Ȃ��ŉ������B
	 *	@param size �L���b�V���T�C�Y
	 *	@retval true �ݒ萬��
	 *	@retval false ���s
	 */
	bool SetWriter(IWriter* pfw, size_t size = 10000)
	{
		if ( ! pfw->CanWrite() )
		{
			return false;
		}
		m_pWriter = pfw;
		m_cache.Resize(size);
		m_cacheSize = 0;
		return true;
	}

	/**
	 * [�擾] �I�[�v�����擾
	 *	@return ���O
	 */
	virtual LPCTSTR GetOpenName(void) const
	{
		m_Check();
		return m_pWriter->GetOpenName();
	}

	/**
	 * [����] �N���[�Y
	 */
	virtual void Close(void)
	{
		m_CheckAndPush();
		m_pWriter->Close();
	}

	/**
	 * [�m�F] �ǂݍ��݉\��
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	virtual bool CanRead(void) const
	{
		m_Check();
		return m_pWriter->CanRead();
	}

	/**
	 * [�擾] �T�C�Y�擾
	 *	@retval �O�ȏ� �T�C�Y
	 *	@retval �}�C�i�X �擾���s
	 */
	virtual LONGLONG GetSize(void) const
	{
		m_Check();
		return m_pWriter->GetSize();
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
		m_CheckAndPush();
		return m_pWriter->Seek(llOffset, eSeek);
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
		m_CheckAndPush();
		return m_pWriter->Read(size, _P);
	}

	/**
	 * [�m�F] �����݉\��
	 *	@retval true �\
	 *	@retval false �s�\
	 */
	virtual bool CanWrite(void) const
	{
		m_Check();
		return m_pWriter->CanWrite();
	}

	/**
	 * [�ݒ�] EOF���w��.
	 *		���݂̃t�@�C���|�C���^�̈ʒu��EOF�ɂ��܂��B
	 *	@throw CTnbException �����݂Ɏ��s����ƁA�X���[����܂��B
	 */
	virtual void SetEnd(void)
	{
		m_CheckAndPush();
		m_pWriter->SetEnd();
	}

	/**
	 * [�ۑ�] ��������
	 *	@param size �������݃T�C�Y�B
	 *	@param P �������݃f�[�^�B
	 *	@throw CWriteFailureException �����݂Ɏ��s����ƁA�X���[����܂��B
	 */
	virtual void Write(size_t size, LPCVOID P)
	{
		m_Check();
		if ( m_cacheSize + size >= m_cache.GetSize() )
		{
			// �L���b�V�������ӂ��
			m_Push();
			ASSERT( m_cacheSize == 0 );
			if ( size >= m_cache.GetSize() )
			{
				// �L���b�V���ɓ���Ȃ�
				m_pWriter->Write(size, P);
			}
			else
			{
				memcpy(m_cache, P, size);
				m_cacheSize = size;
			}
		}
		else
		{
			memcpy(&m_cache[m_cacheSize], P, size);
			m_cacheSize += size;
		}
	}

private:
	// NULL �`�F�b�N
	void m_Check(void) const
	{
		if ( m_pWriter == NULL )
		{
			throw CNullPointerException();
		}
	}
	// �ǂ��o��
	void m_Push(void) const
	{
		if ( m_cacheSize > 0 )
		{
			m_pWriter->Write(m_cacheSize, m_cache);
			m_cacheSize = 0;
		}
	}
	// NULL �`�F�b�N���ǂ��o��
	void m_CheckAndPush(void) const
	{
		m_Check();
		m_Push();
	}
	mutable size_t		m_cacheSize;		///< �L���b�V�����Ă��鐔
	mutable CWorkMem	m_cache;			///< �L���b�V��
	mutable IWriter*	m_pWriter;			///< �t�@�C���N���X
};



}; // TNB
