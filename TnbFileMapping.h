#pragma once
/**
 *@file
 * �t�@�C���}�b�s���O�����֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbFile.h"
#include "TnbTemporaryFile.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT FILE 
 * �t�@�C���}�b�s���O�e���v���[�g�N���X
 *
 *		���̂��t�@�C����Ɏ��R���N�V�����N���X�ł��B���̂��߁A TYP �Ɏw��ł���
 *		�^����������܂����A 4G �𒴂�����ʂ��R���N�V�����ł��܂��i�������A�C���f�b�N�X�͂O�`4G �܂Łj�B
 *
 *	@attention �}���A�폜�����́A�t�@�C���ɑ΂��ăA�N�Z�X���������邽�߁A���x�I�ɂ͂悭�L��܂���B
 *
 *	@attention Open() ���\�b�h�Ńt�@�C�������w�肵���ꍇ�A�{�C���X�^���X���Ȃ��Ȃ��Ă��A�t�@�C���͎c��܂��B
 *
 *	@note �R���N�V�����C���^�[�t�F�[�X�������Ă��܂����A�C���X�^���X�̃R�s�[�͔F�߂Ă��܂���B
 *
 *	@param TYP	�ێ�����^�B�������C���[�W�����̂܂܃t�@�C���ɂ��邽�߁A��{�f�[�^�^�A����ъ�{�f�[�^�^�݂̂̍\���̂Ɍ���܂��B
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *	@see	ICollectionT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbFileMapping.h
 *
 *	@date 06/05/08 �V�K�쐬
 *	@date 11/08/24 �\������V�B ISequenceCollectionT �C���^�[�t�F�[�X�����A�p�~�B
 */
template<typename TYP>
class CFileMappingT : public ICollectionT<TYP>, CCopyImpossible
{
	DEFSUPER(ICollectionT<TYP>);
public:

	/**
	 * �R���X�g���N�^.
	 *	@param multi �}�b�s���O����T�C�Y�̔{�����w��B�ʏ�A�Q�D
	 */
	CFileMappingT(DWORD multi = 2) : _super(), m_pType(NULL), m_hMapping(NULL), m_nowPage(-1), m_isSeeked(false), m_offset(0)
	{
		SYSTEM_INFO si;
		::GetSystemInfo(&si);
		m_pageSize = si.dwAllocationGranularity * multi;
	}

	/**
	 * �f�X�g���N�^.
	 *		�A�^�b�`�����t�@�C�����J�����܂��B
	 *	@note Open() �̍ہA�w�肵���t�@�C���͂��̂܂܂ł��B�K�v�ɉ����č폜���Ă��������B
	 *		Open() �̍ہA�t�@�C�������ȗ������ꍇ�A�e���|�����t�@�C���͍폜����܂��B
	 */
	~CFileMappingT(void)
	{
		Close();
	}

	/** 
	 * [�r��] ���b�N
	 *	@param dwTime �_�~�[�B�ȗ����Ă��������B
	 *	@return ���true�B
	 */
	virtual bool Lock(DWORD dwTime = INFINITE) const 
	{
		return m_sync.Lock(); 
	}

	/** [�r��] �A�����b�N */
	virtual void Unlock(void) const 
	{
		m_sync.Unlock();
	}

	/**
	 * [�m�F] �I�[�v���m�F.
	 *	@retval true �I�[�v���ς�
	 *	@retval false ���I�[�v��
	 */
	bool IsOpened(void) const
	{
		return m_writer.CanWrite();
	}

	/**
	 * [����] �I�[�v��.
	 *	@param lpszName �f�[�^�L���p�Ɏg���t�@�C�����B�ȗ�����ƃe���|�����t�H���_�ɔC�ӂ̖��O�ō쐬���܂��B
	 *	@param offset �R���N�V�����J�n�ʒu�B�t�@�C���̐擪�Ƀw�b�_�Ȃǂ�����A����ȍ~��z��Ƃ���ꍇ�ɗL���ł��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Open(LPCTSTR lpszName = NULL, LONGLONG offset = 0)
	{
		m_offset = offset;
		CStr n = lpszName;
		if ( n.IsEmpty() )
		{
			n = m_tempFileName.Create();
		}
		return m_writer.Open(n, false, true);
	}

	/**
	 * [����] �N���[�Y.
	 *		�I�[�v�������t�@�C������܂��B
	 *	@note Open() �̍ہA�w�肵���t�@�C���͂��̂܂܂ł��B�K�v�ɉ����č폜���Ă��������B
	 *		Open() �̍ہA�t�@�C�������ȗ������ꍇ�A�e���|�����t�@�C���͍폜����܂��B
	 */
	void Close(void)
	{
		m_Unmapping();
		m_writer.Close();
		m_tempFileName.Free();
	}

	/**
	 * [�擾] �v�f���擾.
	 *	@return �v�f��
	 */
	virtual size_t GetSize(void) const
	{
		return ToIndex((m_writer.GetSize() - m_offset) / sizeof(TYP));
	}

	/**
	 * [�ݒ�] �v�f���ݒ�.
	 *	@note �傫�����邱�Ƃ����������邱�Ƃ��o���܂��B
	 *	@param size �T�C�Y
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	virtual bool SetSize(size_t size)
	{
		m_Unmapping();
		if ( m_writer.Seek(m_IndexToFilePos(size)) < 0 )
		{
			return false;
		}
		m_writer.SetEnd();
		m_isSeeked = true;
		return true;
	}

	/**
	 * [�擾] �v�f�̎擾.
	 *	@param index �C���f�b�N�X
	 *	@return ���e
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 *	@throw CReadFailureException �t�@�C���ǂݍ��݂Ɏ��s�������Ƃ��������߂ɃX���[����܂��B
	 */
	virtual TYP Get(INDEX index) const
	{
		return At(index);
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
	virtual const TYP& At(INDEX index) const
	{
		const TYP* P = m_Mapping(index);
		if ( P != NULL )
		{
			return *P;
		}
		throw CIndexOutOfBoundsException();
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@param index �C���f�b�N�X
	 *	@return 
	 */
	virtual TYP& Ref(INDEX index)
	{
		TYP* P = m_Mapping(index);
		if ( P != NULL )
		{
			return *P;
		}
		throw CIndexOutOfBoundsException();
	}

	/**
	 * [�擾] �v�f�̎擾.
	 *		Get() �Ɠ�������ł��B
	 *	@param index �C���f�b�N�X
	 *	@return ���e
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	const TYP& operator[](INDEX index) const
	{
		return At(index);
	}

	/**
	 * [�擾] �v�f�̎Q�Ǝ擾.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A Lock() / Unlock() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@return �Q�ƃI�u�W�F�N�g�B����𑀍삷��ƃt�@�C���ɉe�����L��܂��B
	 *	@throw CIndexOutOfBoundsException �C���f�b�N�X���͈͊O�ł��邱�Ƃ��������߂ɃX���[����܂��B
	 */
	TYP& operator[](INDEX index)
	{
		return Ref(index);
	}

	/**
	 * [�ǉ�] �v�f��ǉ�.
	 *	@param t �v�f
	 *	@retval INVALID_INDEX ���s�B
	 *	@retval ��L�ȊO �ǉ�����INDEX�B
	 */
	virtual INDEX Add(const TYP& t)
	{
		m_Unmapping();
		m_writer.Write(sizeof(TYP), &t);
		return true; 
	}

	/**
	 * [�폜] �v�f��폜.
	 *	@note	��X���b�h�Z�[�u���\�b�h�ł��B
	 *			�����̃X���b�h����A�N�Z�X������ꍇ�A RemoveEx() ���g�p���Ă��������B
	 *	@param index �C���f�b�N�X
	 *	@retval false	�G���[�B
	 *	@retval true	�����B
	 */
	virtual bool Remove(INDEX index)
	{
		if ( IsInRange(index) )
		{
			size_t nl = GetSize();
			loop ( i, (nl - index) - 1 )
			{
				INDEX d = i + index;
				Set(d, Ref(d + 1));
			}
			return SetSize(nl - 1);
		}
		return false;
	}

	/**
	 * [�폜] �S�v�f�폜 .
	 *	@retval true	����
	 *	@retval false	���s
	 */
	virtual bool RemoveAll(void)
	{
		m_Unmapping();
		if ( ! m_writer.CanWrite() )
		{
			return false;
		}
		m_writer.Seek(m_offset);
		m_writer.SetEnd();
		return true;
	}

	/**
	 * [�ݒ�] �v�f�̐ݒ�.
	 *	@param index �C���f�b�N�X
	 *	@param t ���e
	 *	@retval true	�ݒ萬��
	 *	@retval false	���s
	 */
	virtual bool Set(INDEX index, const TYP& t)
	{
		if ( IsInRange(index) )
		{
			Ref(index) = t;
			return true;
		}
		return false;
	}

private:
	/// �C���f�b�N�X���t�@�C���|�C���^
	LONGLONG m_IndexToFilePos(INDEX index) const
	{
		LONGLONG l = index;
		return l * sizeof(TYP) + m_offset;
	}
	/// �A���}�b�s���O
	void m_Unmapping(void)
	{
		if ( m_pType != NULL )
		{
			::UnmapViewOfFile(m_pType);
			m_pType = NULL;
			m_nowPage = -1;
		}
		if ( m_hMapping != NULL )
		{
			::CloseHandle(m_hMapping);
			m_hMapping = NULL;
		}
		if ( m_isSeeked )
		{
			m_isSeeked = false;
			m_writer.Seek(0, CFileWriter::END);
		}
	}
	/// �}�b�s���O
	TYP* m_Mapping(INDEX index) const
	{
		if ( ! IsInRange(index) )
		{
			return NULL;
		}
		if ( m_hMapping == NULL )
		{
			m_hMapping = ::CreateFileMapping(m_writer, NULL, PAGE_READWRITE, 0, 0, NULL);
			if ( m_hMapping == NULL )
			{
				return NULL;
			}
		}
		LONGLONG fl = m_IndexToFilePos(index);
		LONGLONG pl = fl / m_pageSize * m_pageSize;
		if ( pl != m_nowPage )
		{
			if ( m_pType != NULL )
			{
				::UnmapViewOfFile(m_pType);
				m_pType = NULL;
			}
			m_nowPage = pl;
			LONGLONG ns = min(m_pageSize * 2, m_writer.GetSize() - pl);
			m_pType = static_cast<BYTE*>(::MapViewOfFile(m_hMapping, FILE_MAP_ALL_ACCESS, static_cast<DWORD>(pl >> 32), static_cast<DWORD>(pl), down_cast<DWORD>(ns)));
			if ( m_pType == NULL )
			{
				_GetLastError("MapViewOfFile");
				::CloseHandle(m_hMapping);
				m_hMapping = NULL;
				return NULL;
			}
		}
		return reinterpret_cast<TYP*>(&m_pType[fl - pl]);
	}

	CFileWriter			m_writer;		///< �t�@�C���������݃N���X
	CTemporaryFile		m_tempFileName;	///< �e���|�����t�@�C����
	DWORD				m_pageSize;		///< �y�[�W�T�C�Y
	CSyncSection		m_sync;			///< �����I�u�W�F�N�g
	bool				m_isSeeked;		///< �V�[�N���K�v�H
	LONGLONG			m_offset;		///< �R���N�V�����J�n�ʒu
	mutable HANDLE		m_hMapping;		///< �}�b�s���O�n���h��
	mutable BYTE*		m_pType;		///< �}�b�s���O�A�h���X
	mutable LONGLONG	m_nowPage;		///< ���݂̃}�b�s���O�y�[�W
};



/**@ingroup FILE
 * �t�@�C���}�b�s���O�N���X
 *
 *	@note	TYP �� BYTE �� CFileMappingT �ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbFileMapping.h
 *
 *	@date 06/10/25 �V�K�쐬
 */
typedef CFileMappingT<BYTE> CFileMapping;



}; // TNB
