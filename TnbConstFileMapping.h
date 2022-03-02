#pragma once
/**
 *@file
 * �t�@�C���}�b�s���O�����֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbFileName.h"
#include "TnbCollection.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT FILE
 * �R���X�g�t�@�C���}�b�s���O�e���v���[�g�N���X
 *
 *		���̂��t�@�C����Ɏ��R���N�V�����N���X�ł��B���̂��߁A TYP �Ɏw��ł���
 *		�^����������܂����A 4G �𒴂�����ʂ��R���N�V�����ł��܂��i�������A�C���f�b�N�X�͂O�`4G �܂Łj�B
 *
 *	@param TYP	�ێ�����^�B�������C���[�W�����̂܂܃t�@�C���ɂ��邽�߁A��{�f�[�^�^�A����ъ�{�f�[�^�^�݂̂̍\���̂Ɍ���܂��B
 *
 *	@see	IConstCollectionT ���������Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbConstFileMapping.h
 *
 *	@date 09/03/18	�V�K�쐬
 *	@date 11/08/24	�\������V�B ISequenceCollectionT �C���^�[�t�F�[�X�����A�p�~�B
 *	@date 11/08/30	�}�b�s���O�T�C�Y���w��\�ɁB
 */
template<typename TYP>
class CConstFileMappingT : public IConstCollectionT<TYP>, CCopyImpossible
{
	DEFSUPER(IConstCollectionT<TYP>);
public:

	/**
	 * �R���X�g���N�^.
	 *	@param multi �}�b�s���O����T�C�Y�̔{�����w��B�ʏ�A�Q�D
	 */
	CConstFileMappingT(DWORD multi = 2) : _super(), m_pType(NULL), m_hMapping(NULL), m_nowPage(-1), m_offset(0), m_size(0)
	{
		SYSTEM_INFO si;
		::GetSystemInfo(&si);
		m_pageSize = si.dwAllocationGranularity * multi;
	}

	/**
	 * �f�X�g���N�^.
	 */
	~CConstFileMappingT(void)
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
	 *	@note �I�[�v�����Ă��Ȃ��ƁAGet,Refer�n���\�b�h�͖��Ӗ��Ȓl��Ԃ��܂��B
	 *	@retval true �I�[�v���ς�
	 *	@retval false ���I�[�v���B
	 */
	bool IsOpened(void) const
	{
		return m_reader.CanRead();
	}

	/**
	 * [����] �I�[�v��.
	 *	@param lpszName �ǂݎ��Ώۂ̃t�@�C�����B
	 *	@param offset �R���N�V�����J�n�̃t�@�C���|�C���^�B�t�@�C���̐擪�Ƀw�b�_�Ȃǂ�����A����ȍ~��z��Ƃ���ꍇ�ɗL���ł��B
	 *	@param size �R���N�V�������B�ȗ�����ƁA�t�@�C���T�C�Y���画�f���܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Open(LPCTSTR lpszName, LONGLONG offset = 0, size_t size = INVALID_SIZE)
	{
		Close();
		m_offset = offset;
		if ( m_reader.Open(lpszName, true) )
		{
			m_size = ToIndex((m_reader.GetSize() - m_offset) / sizeof(TYP));
			if ( size != INVALID_SIZE )
			{
				if ( m_size > size )
				{
					m_size = size;
				}
			}
			return true;
		}
		return false;
	}

	/**
	 * [����] �N���[�Y.
	 *		�I�[�v�������t�@�C�����J�����܂��B
	 */
	void Close(void)
	{
		m_Unmapping();
		m_reader.Close();
		m_size = 0;
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
			m_hMapping = ::CreateFileMapping(m_reader, NULL, PAGE_READONLY, 0, 0, NULL);
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
			LONGLONG ns = min(m_pageSize * 2, m_reader.GetSize() - pl);
			m_pType = static_cast<BYTE*>(::MapViewOfFile(m_hMapping, FILE_MAP_READ, static_cast<DWORD>(pl >> 32), static_cast<DWORD>(pl), down_cast<DWORD>(ns)));
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

	CFileReader			m_reader;		///< �t�@�C���ǂ݂��݃N���X
	DWORD				m_pageSize;		///< �y�[�W�T�C�Y
	CSyncSection		m_sync;			///< �����I�u�W�F�N�g
	LONGLONG			m_offset;		///< �I�t�Z�b�g
	size_t				m_size;			///< �f�[�^��
	mutable HANDLE		m_hMapping;		///< �}�b�s���O�n���h��
	mutable BYTE*		m_pType;		///< �}�b�s���O�A�h���X
	mutable LONGLONG	m_nowPage;		///< ���݂̃}�b�s���O�y�[�W
	friend class CConstFileMappingTest;
};



/**@ingroup FILE
 * Const�t�@�C���}�b�s���O�N���X
 *
 *	@note	TYP �� BYTE �� CConstFileMappingT �ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbConstFileMapping.h
 *
 *	@date 09/03/18 �V�K�쐬
 */
typedef CConstFileMappingT<BYTE> CConstFileMapping;



}; // TNB
