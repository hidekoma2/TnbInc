#pragma once
/**
 *@file
 * �t�@�C���֌W�̃w�b�_
 *
 *		�t�@�C�����R���N�V�����Ƃ��đ��삷�邽�߂̃A�_�v�^�N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbFile.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE COLLECT ITERATOR
 * �t�@�C���ǂݍ��݃A�_�v�^
 *
 *		IReader �����I�u�W�F�N�g�i�t�@�C���Ȃǁj����̃R���N�V�����Ƃ��ăA�N�Z�X���邽�߂̃A�_�v�^�ł��B
 *
 *	@note ���ǂݍ��ݎ� CIoFailureException ����������\��������܂��B
 *
 *	@note �C���X�^���X�̕�������ƕ����������ׂẴC���X�^���X�œ����t�@�C���n���h����
 *		�g���悤�ɂȂ�܂��B���ׂẴC���X�^���X��Close���Ȃ��ƃt�@�C���n���h�����N���[�Y
 *		����Ȃ��Ȃ�܂��B
 *
 *	@see	IConstCollectionT<BYTE> ���������Ă��܂��B
 *
 *	@note IConstCollectionT �� TYP �� BYTE �ɂȂ�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbFileAdapter.h
 *
 *	@date 06/08/04	�V�K�쐬
 *	@date 06/08/13 �C�e���[�^�� STL �ɂ��킹�āA���C�B
 *	@date 06/12/20 IReader ���g�p����悤�ɁA���C�B
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 *	@date 09/09/08 �N���X���A CConstFileAdapter �� CReaderAdapter �ɕύX�B
 */
class CReaderAdapter : public IConstCollectionT<BYTE>
{
	DEFSUPER(IConstCollectionT<BYTE>);
public:

	/// �R���X�g���N�^
	CReaderAdapter(void) : _super(), m_pReader(NULL), m_iTempNo(0)
	{
	}

	/** 
	 * [�r��] ���b�N
	 *	@param dwTime �_�~�[�B�ȗ����Ă��������B
	 *	@return ���true�B
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const 
	{
		return true; 
	}

	/** [�r��] �A�����b�N */
	virtual void Unlock(void) const
	{
	}

	/**
	 * [����] �A�^�b�`.
	 *	@param pReader	�I�[�v�����Ă��� IReader�B �f�^�b�`���邩�A
	 *					�{�C���X�^���X���f�X�g���N�g����܂Ŕj�����Ȃ��ŉ������B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Attach(const IReader* pReader)
	{
		Detach();
		if ( ! pReader->CanRead() )
		{
			return false;
		}
		m_pReader = pReader;
		return true;
	}

	/**
	 * [����] �f�^�b�`.
	 */
	void Detach(void)
	{
		if ( m_pReader == NULL )
		{
			m_pReader = NULL;
		}
	}

	/**
	 * [�擾] �v�f���擾.
	 *	@return �v�f��
	 */
	virtual size_t GetSize(void) const
	{
		if ( m_pReader == NULL )
		{
			return 0;
		}
		return ToIndex(m_pReader->GetSize());
	}

	/**
	 * [�擾] �v�f�̎擾.
	 *	@param index �C���f�b�N�X
	 *	@return ���e
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 *	@throw CReadFailureException �t�@�C���ǂݍ��݂Ɏ��s�������Ƃ��������߂ɃX���[����܂��B
	 */
	virtual BYTE Get(INDEX index) const
	{
		if ( m_pReader == NULL || ! IsInRange(index) )
		{
			throw CIndexOutOfBoundsException();
		}
		if ( m_pReader->Seek(index) >= 0 )
		{
			CByteVector vb = m_pReader->ReadExactly(1);
			if ( vb.IsValid() )
			{
				return vb[0];
			}
		}
		throw CReadFailureException(ERROR_ACCESS_DENIED);
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@attention �Q�Ɛ�̓C���X�^���X���̃��[�N�̃A�h���X���w���Ă��܂��B
	 *			���� At() �ɂ���̓��e���ω�����\�����L��܂��̂ŁA
	 *			�K�v�ɉ����Ēl���R�s�[���Ă����悤�ɂ��Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �ꎞ�I�ȎQ��
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 *	@throw CReadFailureException �t�@�C���ǂݍ��݂Ɏ��s�������Ƃ��������߂ɃX���[����܂��B
	 */
	virtual const BYTE& At(INDEX index) const
	{
		m_iTempNo = (m_iTempNo + 1) & 0x0F;
		return m_abTemp[m_iTempNo] = Get(index);
	}

	/**
	 * [�擾] �v�f�̎擾.
	 *		Get() �Ɠ�������ł��B
	 *	@param index �C���f�b�N�X
	 *	@return ���e
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	BYTE operator[](INDEX index) const
	{
		return Get(index);
	}

private:
	const IReader*		m_pReader;		///< �t�@�C���ǂݍ��݃N���X	
	mutable BYTE		m_abTemp[16];	///< �Q�Ɨp
	mutable INDEX		m_iTempNo;		///< �Q�ƗpIndex
};



/**@ingroup FILE COLLECT ITERATOR
 * �t�@�C���ǂݏ����A�_�v�^
 *
 *		IWriter �����I�u�W�F�N�g�i�t�@�C���Ȃǁj����̃R���N�V�����Ƃ��ăA�N�Z�X���邽�߂̃A�_�v�^�ł��B
 *
 *	@note ���ǂݍ��ݎ� CReadFailureException ����������\��������܂��B
 *
 *	@note �C���X�^���X�̕�������ƕ����������ׂẴC���X�^���X�œ����t�@�C���n���h����
 *		�g���悤�ɂȂ�܂��B���ׂẴC���X�^���X��Close���Ȃ��ƃt�@�C���n���h�����N���[�Y
 *		����Ȃ��Ȃ�܂��B
 *
 *	@see	ICollectionT<BYTE> ���������Ă��܂��B
 *
 *	@note ICollectionT �� TYP �� BYTE �ɂȂ�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbFileAdapter.h
 *
 *	@date 06/08/04	�V�K�쐬
 *	@date 06/08/11 Element() �� At() �ɉ����BReferElementPtr() ��p�~�B
 *	@date 06/08/13 �C�e���[�^�� STL �ɂ��킹�āA���C�B
 *	@date 07/03/15 index,size�Ǘ��� unsigned�ɕύX�B
 */
class CWriterAdapter : public ICollectionT<BYTE>
{
	DEFSUPER(ICollectionT<BYTE>);
public:

	/**@ingroup FILE COLLECT
	 * �t�@�C���ǂݏ����̎Q�ƃN���X
	 *
	 *		�t�@�C���̈�o�C�g�𑀍삷�邽�߂̃N���X�ł��B
	 *
	 *	@note	CWriterAdapter �ō쐬�������̂�Z���Ŏg�p���邱�Ƃ�z�肵�Ă��邽�߁A
	 *			�R���X�g���N�^�� private �錾���Ă��܂��B
	 *	
	 *	@see CWriterAdapter::operator[]()
	 *
	 *	@par�g�p��
	 *		<table><tr><td><pre>
	 *		
	 *		CWriterAdapter fa;
	 *		CFileWriter fw;
	 *		fw.New("c:\\temp\\aaa.dat");
	 *		fa.Attach(&fw);
	 *		fa.SetSize(10);
	 *		    ;
	 *		BYTE bb = fa[0];    // �t�@�C���� �ŏ��̃f�[�^�� bb �ɓǂݍ��݂܂��B		
	 *		fa[3] = 7;          // �t�@�C���� 4Byte�ڂ� 7 ���������݂܂��B		 
	 *		fa[5] += 2;           // �t�@�C���� 6Byte�ڂ̃f�[�^�� +2 ���܂��B
	 *		</pre></td></tr></table>
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbFileAdapter.h
	 *
	 *	@date 06/08/11	�V�K�쐬
	 */
	class CRef
	{
		friend class CWriterAdapter;
		CWriterAdapter*	m_fa;
		INDEX			m_index;
		BYTE m_Set(BYTE b)
		{
			if ( ! m_fa->Set(m_index, b) )
			{
				throw CWriteFailureException(ERROR_ACCESS_DENIED);
			}
			return b;
		}
		BYTE m_Get(void) const
		{
			return m_fa->Get(m_index);
		}
		CRef(CWriterAdapter* P, INDEX i)
		{
			m_fa = P;
			m_index = i;
		}
	public:
		/**
		 * [�擾] �f�[�^�擾
		 *	@return �f�[�^�B
		 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
		 *	@throw CReadFailureException �t�@�C���ǂݍ��݂Ɏ��s�������Ƃ��������߂ɃX���[����܂��B
		 */
		operator BYTE(void) const
		{
			return m_Get();
		}
		/**
		 * [�ݒ�] �f�[�^�ݒ�
		 *	@param b �������ރf�[�^�B
		 *	@return �������񂾃f�[�^�B
		 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
		 *	@throw CWriteFailureException �t�@�C���������݂Ɏ��s�������Ƃ��������߂ɃX���[����܂��B
		 */
		BYTE operator=(BYTE b)
		{
			return m_Set(static_cast<BYTE>(b));
		}
		/**
		 * [�v�Z] �f�[�^���Z
		 *	@param b ���Z����f�[�^�B
		 *	@return �������񂾃f�[�^�B
		 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
		 *	@throw CIoFailureException �t�@�C���ǂݏ����Ɏ��s�������Ƃ��������߂ɃX���[����܂��B
		 */
		BYTE operator+=(BYTE b)
		{
			return m_Set(static_cast<BYTE>(m_Get() + b));
		}
		/**
		 * [�擾] �f�[�^���Z
		 *	@param b ���Z����f�[�^�B
		 *	@return �������񂾃f�[�^�B
		 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
		 *	@throw CIoFailureException �t�@�C���ǂݏ����Ɏ��s�������Ƃ��������߂ɃX���[����܂��B
		 */
		BYTE operator-=(BYTE b)
		{
			return m_Set(static_cast<BYTE>(m_Get() - b));
		}
	};


	//-----------------


	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
	// const_iterator�^�錾
	typedef CConstIteratorBaseT<CWriterAdapter, BYTE> const_iterator;
	// iterator�錾
	class iterator
	{
		_ITERATORCORE(iterator, CWriterAdapter)
	public:
		CRef operator*(void)
		{
			return (*m_V)[m_ind];
		}
		CRef operator[](int i)
		{
			return (*m_V)[m_ind+i];
		}
	};
	#endif //_TnbDOXYEM

	/**
	 * [����] �擪const_iterator.
	 *		�擪�v�f���w��{@link ITERATOR �����q}���擾
	 *	@return const_iterator
	 */
	const_iterator begin(void) const { return const_iterator(this, 0); }

	/**
	 * [����] �Ō�const_iterator.
	 *		�Ō�v�f�̎����w��{@link ITERATOR �����q}���擾
	 *	@return const_iterator
	 */
	const_iterator end(void) const { return const_iterator(this, GetSize()); }

	/**
	 * [����] �擪iterator.
	 *		�擪�v�f���w��{@link ITERATOR �����q}���擾
	 *	@return iterator
	 */
	iterator begin(void) { return iterator(this, 0); }

	/**
	 * [����] �Ō�iterator.
	 *		�Ō�v�f�̎����w��{@link ITERATOR �����q}���擾
	 *	@return iterator
	 */
	iterator end(void) { return iterator(this, GetSize()); }


	//-----------------


	/// �R���X�g���N�^
	CWriterAdapter(void) : _super(), m_pWriter(NULL), m_iTempNo(0)
	{
	}

	/** 
	 * [�r��] ���b�N
	 *	@param dwTime �_�~�[�B�ȗ����Ă��������B
	 *	@return ���true�B
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const 
	{
		return true; 
	}

	/** [�r��] �A�����b�N */
	virtual void Unlock(void) const 
	{
	}

	/**
	 * [����] �A�^�b�`.
	 *	@param pWriter	�I�[�v�����Ă��� IWriter �f�^�b�`���邩�A
	 *					�{�C���X�^���X���f�X�g���N�g����܂Ŕj�����Ȃ��ŉ������B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Attach(IWriter* pWriter)
	{
		Detach();
		if ( ! pWriter->CanWrite() )
		{
			return false;
		}
		m_pWriter = pWriter;
		return true;
	}

	/**
	 * [����] �f�^�b�`.
	 */
	void Detach(void)
	{
		if ( m_pWriter == NULL )
		{
			m_pWriter = NULL;
		}
	}

	/**
	 * [�擾] �v�f���擾.
	 *	@return �v�f��
	 */
	virtual size_t GetSize(void) const
	{
		if ( m_pWriter == NULL )
		{
			return 0;
		}
		return ToIndex(m_pWriter->GetSize());
	}

	/**
	 * [�ݒ�] �v�f���ݒ�.
	 *	@note �傫�����邱�Ƃ����������邱�Ƃ��o���܂��B
	 *	@note �傫���������A���̃f�[�^�� 0 �Ŗ��߂��܂��B
	 *	@param size �T�C�Y
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	virtual bool SetSize(size_t size)
	{
		if ( m_pWriter == NULL || m_pWriter->Seek(size) < 0 )
		{
			return false;
		}
		m_pWriter->SetEnd();
		return true;
	}

	/**
	 * [�擾] �v�f�̎擾.
	 *	@param index �C���f�b�N�X
	 *	@return ���e
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 *	@throw CReadFailureException �t�@�C���ǂݍ��݂Ɏ��s�������Ƃ��������߂ɃX���[����܂��B
	 */
	virtual BYTE Get(INDEX index) const
	{
		if ( m_pWriter == NULL || ! IsInRange(index) )
		{
			throw CIndexOutOfBoundsException(); 
		}
		if ( m_pWriter->Seek(index) >= 0 )
		{
			CByteVector vb = m_pWriter->ReadExactly(1);
			if ( vb.IsValid() )
			{
				return vb[0];
			}
		}
		throw CReadFailureException(ERROR_ACCESS_DENIED);
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@attention �Q�Ɛ�̓C���X�^���X���̃��[�N�̃A�h���X���w���Ă��܂��B
	 *			���� At() �ɂ���̓��e���ω�����\�����L��܂��̂ŁA
	 *			�K�v�ɉ����Ēl���R�s�[���Ă����悤�ɂ��Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �ꎞ�I�ȎQ��
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 *	@throw CReadFailureException �t�@�C���ǂݍ��݂Ɏ��s�������Ƃ��������߂ɃX���[����܂��B
	 */
	virtual const BYTE& At(INDEX index) const
	{
		m_iTempNo = (m_iTempNo + 1) & 0x0F;
		m_abTemp[m_iTempNo] = Get(index);
		return m_abTemp[m_iTempNo];
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@attention �T�|�[�g�ł��܂���B operator[] ���g�p���܂��B
	 *	@param index �C���f�b�N�X
	 *	@return -
	 *	@throw CNotSupportException �T�|�[�g���Ă��Ȃ����Ƃ��������߂ɃX���[����܂��B
	 */
	virtual BYTE& Ref(INDEX index)
	{
		throw CNotSupportException();
	}

	/**
	 * [�擾] �v�f�̎擾.
	 *		Get() �Ɠ�������ł��B
	 *	@param index �C���f�b�N�X
	 *	@return ���e
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	BYTE operator[](INDEX index) const
	{
		return Get(index);
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �Q�ƃI�u�W�F�N�g�B����𑀍삷��ƃt�@�C���ɉe�����L��܂��B
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	CRef operator[](INDEX index)
	{
		if ( m_pWriter == NULL || ! IsInRange(index) )
		{
			throw CIndexOutOfBoundsException(); 
		}
		return CRef(this, index);
	}

	/**
	 * [�ǉ�] �v�f��ǉ�.
	 *	@param t �v�f
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval ��L�ȊO �ǉ�����INDEX�B
	 */
	virtual INDEX Add(const BYTE& t)
	{
		if ( m_pWriter == NULL )
		{
			return INVALID_INDEX;
		}
		INDEX r = static_cast<INDEX>(m_pWriter->Seek(0, CFileWriter::END));
		if ( r != INVALID_INDEX )
		{
			m_pWriter->Write(1, &t);
		}
		return r; 
	}

	/**
	 * [�폜] �v�f��폜.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A RemoveEx() ���g�p���Ă��������B
	 *	@attention �T�|�[�g���܂���B
	 *	@param index �C���f�b�N�X
	 *	@return ��� false(���s)
	 */
	virtual bool Remove(INDEX index)
	{
		return false;
	}

	/**
	 * [�폜] �S�v�f�폜 .
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool RemoveAll(void)
	{
		if ( m_pWriter == NULL )
		{
			return false;
		}
		m_pWriter->Seek(0);
		m_pWriter->SetEnd();
		return true;
	}

	/**
	 * [�ݒ�] �v�f�̐ݒ�.
	 *	@param index �C���f�b�N�X
	 *	@param t ���e
	 *	@retval true	�ݒ萬��
	 *	@retval false	���s
	 */
	virtual bool Set(INDEX index, const BYTE& t)
	{
		if ( m_pWriter != NULL && IsInRange(index) && m_pWriter->Seek(index) >= 0 )
		{
			m_pWriter->Write(1, &t);
			return true;
		}
		return false;
	}

private:
	IWriter*		m_pWriter;		///< �t�@�C���������݃N���X
	mutable BYTE	m_abTemp[16];	///< �Q�Ɨp
	mutable INDEX	m_iTempNo;		///< �Q�ƗpIndex
};



}; // TNB
