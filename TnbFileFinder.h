#pragma once
/**
 *@file
 * �t�@�C�������֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include "TnbException.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * �t�@�C���������ۃN���X
 *
 *		�w��̃f�B���N�g������t�@�C�����������邽�߂̃N���X�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbFileFinder.h
 *
 *	@date 06/11/30 �V�K
 */
class CAbstractFileFinder : CCopyImpossible
{
public:
	/// �����^�C�v
	enum EFindType
	{
		ALL,			///< �S���Ώ�
		FILE_ONLY,		///< �t�@�C���̂�
		DIR_ONLY,		///< �f�B���N�g���̂�
	};

	#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
	/// �C�e���[�^
	class iterator
	{
		CAbstractFileFinder*	m_pff;
	public:
		iterator(CAbstractFileFinder* pff) : m_pff(pff)
		{
			if ( m_pff != NULL && ! m_pff->IsFinding() )
			{
				m_pff = NULL;
			}
		}
		const WIN32_FIND_DATA* operator->(void)
		{
			if ( m_pff == NULL ) { return NULL; }
			return &(m_pff->Get());
		}
		const WIN32_FIND_DATA& operator*(void)
		{
			if ( m_pff == NULL ) { throw CEmptyException(); }
			return m_pff->Get();
		}
		iterator& operator++(void)
		{
			if ( m_pff != NULL && ! m_pff->Next() ) { m_pff = NULL; }
			return *this;
		}
		iterator& operator++(int)
		{
			return operator++();
		}
		bool operator==(const iterator& i) const
		{
			return m_pff == i.m_pff;
		}
		bool operator!=(const iterator& i) const
		{
			return ! operator==(i);
		}
	};
	#endif //_TnbDOXYGEN

	/**
	 * [����] �擪iterator.
	 *		�擪�v�f���w��{@link ITERATOR �����q}���擾
	 *	@note Find() �ŁA�������Ă���C�e���[�^���擾���܂��B
	 *	@return iterator
	 */
	iterator begin(void)
	{
		return iterator(this);
	}

	/**
	 * [����] �Ō�iterator.
	 *		�Ō�v�f�̎����w��{@link ITERATOR �����q}���擾
	 *	@return iterator
	 */
	iterator end(void)
	{
		return iterator(NULL);
	}


	//-------------------------


	/// �R���X�g���N�^
	CAbstractFileFinder(void) : m_isFinding(false)
	{
	}

	/// �f�X�g���N�^
	virtual ~CAbstractFileFinder(void) {}

	/**
	 * [�ݒ�] �����I��.
	 *	@note �������̏ꍇ�A�������~���܂��i Next() ���\�b�h�� false ��Ԃ��悤�ɂȂ�܂��j�B
	 */
	void Finish(void)
	{
		m_isFinding = false;
		m_strPath.Empty();
		m_strMask.Empty();
		OnFinish();
	}
	
	/**
	 * [����] �����J�n
	 *	@note �����͕K�����̃��\�b�h����J�n���܂��B
	 *	@param lpszPath �����p�X�w��B
	 *	@param lpszMask �����}�X�N�w��B���C���h�J�[�h�ɂ��w�肪�K�v�ł��B�ȗ������ *.* ���w�肳��܂��B
	 *	@param type �����^�C�v�B
	 *	@retval true ��ȏ㔭���B�����������e�� Get() ���\�b�h���Ŏ擾�\�B
	 *	@retval false �������B
	 */
	bool Start(LPCTSTR lpszPath, LPCTSTR lpszMask = _T("*.*"), EFindType type = ALL)
	{
		Finish();
		CStr s = lpszPath;
		if ( lpszMask != NULL )
		{
			if ( lpszMask[0] != '\\' && lpszMask[0] != '/' )
			{
				s += _T("\\");
			}
			s += lpszMask;
			s.Replace(_T("\\\\"), _T("\\"));
		}
		if ( OnStart(m_data, s) )
		{
			m_isFinding = true;
			m_strPath = lpszPath;
			m_strMask = lpszMask;
			m_type = type;
			if ( m_IsTarget(m_data, m_type) )
			{
				return true;
			}
			return Next();
		}
		Finish();
		return false;
	}

	/**
	 * [����] ������
	 *	@note Find() �� true �̏ꍇ�A���ɂ��̃��\�b�h���g�p���܂��B���̌�J��Ԃ���
	 *			�{���\�b�h�g�����Ƃŕ����̏��𓾂邱�Ƃ��o���܂��B
	 *	@retval true ��ȏ㔭���B�����������e��Get���\�b�h�Ŏ擾�\�B
	 *	@retval false �������B
	 */
	bool Next(void)
	{
		bool r = false;
		if ( IsFinding() )
		{
			while ( true )
			{
				r = OnNext(m_data);
				if ( ! r )
				{
					Finish();
					break;
				}
				else if ( m_IsTarget(m_data, m_type) )
				{
					break;
				}
			}
		}
		return r;
	}

	/**
	 * [�m�F] �������m�F
	 *	@retval true �������B
	 *	@retval false ���Ă��Ȃ��B
	 */
	bool IsFinding(void) const
	{
		return m_isFinding;
	}

	/**
	 * [�擾] �������擾
	 *	@return �t�@�C�����̎Q�ƁB
	 *	@throw CEmptyException �������A���邢�͖��������ɖ{���\�b�h���R�[������ƁA�X���[����܂��B
	 */
	const WIN32_FIND_DATA& Get(void) const
	{
		if ( ! IsFinding() ) { throw CEmptyException(); }
		return m_data;
	}

	/**
	 * [�擾] �������擾
	 *	@return �t�@�C�����̎Q�ƁB
	 *	@throw CEmptyException �������A���邢�͖��������ɖ{���\�b�h���R�[������ƁA�X���[����܂��B
	 */
	const WIN32_FIND_DATA* operator->(void) const
	{
		return &Get();
	}

	/**
	 * [�擾] �����t�@�C�����擾
	 *	@return �t�@�C����
	 */
	CStr GetFoundName(void) const
	{
		if ( ! IsFinding() ) { return CStr(); }
		CStr s = m_strPath;
		s += _T("\\");
		s += Get().cFileName;
		s.Replace(_T("\\\\"), _T("\\"));
		s.TrimLeft('\\');
		return s;
	}

	/**
	 * [�m�F] �f�B���N�g�����H
	 *	@note �������̎��A����������񂪃f�B���N�g�����ۂ����ׂ邱�Ƃ��o���܂��B
	 *	@retval true �f�B���N�g��
	 *	@retval false �t�@�C�� or ������
	 */
	bool IsDirectory(void) const
	{
		if ( IsFinding() && (Get().dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
		{
			return true;
		}
		return false;
	}
#if 0
	/**
	 * [����] �����f�B���N�g��������
	 *	@note ����������񂪃f�B���N�g���̏ꍇ�A���̒��̌����Ɉڍs���܂��B
	 *	@retval NULL ���s�B
	 *	@retval NULL�ȊO �����B�߂�l�́A Pop() �Ɏg�p���܂��B
	 */
	virtual HFINDPUSH Push(void) { return NULL; };

	/**
	 * [����] �����f�B���N�g���������I��
	 *	@note Push() �����Ƃ��̏�Ԃɖ߂�܂��B
	 *	@param h Push() �̖߂�l�B
	 */
	virtual void Pop(HFINDPUSH h) {};
#endif

protected:

	/**
	 * [�ʒm] �����J�n�ʒm
	 *	@param[out] _data ���������ꍇ�A������񂪊i�[����܂��B
	 *	@param[in] lpszName �����p�X�w��B���C���h�J�[�h�ɂ��w�肪�K�v�ł��B
	 *	@retval true ��ȏ㔭���B
	 *	@retval false �������B
	 */
	virtual bool OnStart(WIN32_FIND_DATA& _data, LPCTSTR lpszName) = 0;

	/**
	 * [�ʒm] �������ʒm
	 *	@note Find() �� true �̏ꍇ�A���ɂ��̃��\�b�h���g�p���܂��B���̌�J��Ԃ���
	 *			�{���\�b�h�g�����Ƃŕ����̏��𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _data ���������ꍇ�A������񂪊i�[����܂��B
	 *	@retval true ��ȏ㔭���B
	 *	@retval false �������B
	 */
	virtual bool OnNext(WIN32_FIND_DATA& _data) = 0;

	/**
	 * [�ʒm] �����I���ʒm
	 *	@note �������̏ꍇ�A�������~���܂��i OnNext() ���\�b�h�� false ��Ԃ��悤�ɂȂ�܂��j�B
	 */
	virtual void OnFinish(void) = 0;

private:

	bool				m_isFinding;///< ������
	CStr				m_strPath;	///< �����p�X
	CStr				m_strMask;	///< �����}�X�N
	EFindType			m_type;		///< �������@
	WIN32_FIND_DATA		m_data;		///< �������� 

	/**
	 * [�m�F] �Ώۃ`�F�b�N
	 *	@note �����Ŏg�p����`�F�b�N���\�b�h�ł��B
	 *	@param data �t�@�C�����
	 *	@param type �`�F�b�N���@
	 *	@retval true �Ώ�
	 *	@retval false �ΏۊO
	 */
	bool m_IsTarget(const WIN32_FIND_DATA& data, EFindType type) const
	{
		if ( STRLIB::Compare(data.cFileName, _T("..")) == 0 )
		{
			return false;
		}
		if ( STRLIB::Compare(data.cFileName, _T(".")) == 0 )
		{
			return false;
		}
		switch ( type )
		{
		case FILE_ONLY:
			if ( (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
			{
				return false;
			}
			break;
		case DIR_ONLY:
			if ( (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 )
			{
				return false;
			}
			break;
		default:
			break;
		}
		return true;
	}
};



/**@ingroup FILE
 * �t�@�C�������N���X
 *
 *		�w��̃f�B���N�g������t�@�C�����������邽�߂̃N���X�ł��B
 *		
 *	@note �������A�f�B���N�g�����A[.][..]�́A���O����܂��B
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *		CFileFinder ff;
 *		if ( ff.Start(_T(".\\*.*")) )
 *		{
 *			do
 *			{
 *				TRACE1("  found = %s\n", ff->cFileName);
 *			} while ( ff.Next() );
 *		}
 *		</pre></td></tr></table>
 *
 *	@par�g�p��(iterator�g�p)
 *		<table><tr><td><pre>
 *		CFileFinder ff;
 *		if ( ff.Start(_T(".\\*.*")) )
 *		{
 *			for ( CFileFinder::iterator i = ff.begin(); i != ff.end(); i++ )
 *			{
 *				TRACE1("  found = %s\n", i->cFileName);
 *			}
 *		}
 *		</pre></td></tr></table>
 *	@note iterator ���쎞�A���� CFileFinder ���g�p����܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbFileFinder.h
 *
 *	@date 06/11/30 �V�K
 *	@date 08/03/23 �\���ύX�B
 */
class CFileFinder : public CAbstractFileFinder
{
	DEFSUPER(CAbstractFileFinder);
public:

	/// �R���X�g���N�^
	CFileFinder(void) : _super(), m_hFind(INVALID_HANDLE_VALUE)
	{
	}

	/// �f�X�g���N�^
	virtual ~CFileFinder(void)
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
		Finish();
		m_hFind = ::FindFirstFile(lpszName, &_data);
		return (m_hFind != INVALID_HANDLE_VALUE);
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
		if ( m_hFind != INVALID_HANDLE_VALUE )
		{
			return !! ::FindNextFile(m_hFind, &_data);
		}
		return false;
	}

	/**
	 * [�ʒm] �����I���ʒm
	 *	@note �������̏ꍇ�A�������~���܂��i OnNext() ���\�b�h�� false ��Ԃ��悤�ɂȂ�܂��j�B
	 */
	virtual void OnFinish(void)
	{
		if ( m_hFind != INVALID_HANDLE_VALUE )
		{
			::FindClose(m_hFind);
			m_hFind = INVALID_HANDLE_VALUE;
		}
	}
#if 0
	/**
	 * [����] �����f�B���N�g��������
	 *	@note ����������񂪃f�B���N�g���̏ꍇ�A���̒��̌����Ɉڍs���܂��B
	 *	@retval NULL ���s�B
	 *	@retval NULL�ȊO �����B�߂�l�́A Pop() �Ɏg�p���܂��B
	 */
	virtual HFINDPUSH Push(void)
	{
		if ( IsDirectory() )
		{
			TPushParam* P = new TPushParam;
			P->hFind = m_hFind;
			P->strPath = m_strPath;
			P->data = m_data;
			//
			m_strPath += _T("\\");
			m_strPath += m_data.cFileName;
			m_hFind = ::FindFirstFile(m_strPath, &m_data);
			if ( m_hFind != INVALID_HANDLE_VALUE )
			{
				if ( ! IsTarget(m_data, m_type) )
				{
					if ( Next() )
					{
						return P;
					}
				}
			}
			m_hFind = P->hFind;
			m_strPath = P->strPath;
			m_data = P->data;
			delete P;
		}
		return NULL;
	}

	/**
	 * [����] �����f�B���N�g���������I��
	 *	@note Push() �����Ƃ��̏�Ԃɖ߂�܂��B
	 *	@param h Push() �̖߂�l�B
	 */
	virtual void Pop(HFINDPUSH h) 
	{
		if ( h != NULL )
		{
			TPushParam* P = reinterpret_cast<TPushParam*>(h);
			m_hFind = P->hFind;
			m_strPath = P->strPath;
			m_data = P->data;
			delete P;
		}
	};
#endif

private:
	HANDLE				m_hFind;	///< �����n���h��
	friend class CFileFinderTest;
};



}; //TNB

#if 0
struct WIN32_FIND_DATA
{
	DWORD dwFileAttributes;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	DWORD nFileSizeHigh;
	DWORD nFileSizeLow;
	DWORD dwReserved0;
	DWORD dwReserved1;
	TCHAR cFileName[ MAX_PATH ];
	TCHAR cAlternateFileName[ 14 ];
} WIN32_FIND_DATA;

enum
{
	FILE_ATTRIBUTE_ARCHIVE				= 0x00000020, ///< �A�[�J�C�u�t�@�C���܂��̓A�[�J�C�u�f�B���N�g���ł��B�A�v���P�[�V�����͂��̑������A�t�@�C���̃o�b�N�A�b�v��폜�̂��߂̃}�[�N�Ƃ��Ďg���܂��B
	FILE_ATTRIBUTE_COMPRESSED			= 0x00000800, ///< �w�肳�ꂽ�t�@�C���܂��̓f�B���N�g���͈��k����Ă��܂��B�t�@�C���̏ꍇ�A�t�@�C�����̑S�f�[�^�����k����Ă��邱�Ƃ��Ӗ����܂��B�f�B���N�g���̏ꍇ�A���̃f�B���N�g�����ɐV�����쐬�����t�@�C���܂��̓T�u�f�B���N�g�����A����ň��k��ԂɂȂ邱�Ƃ��Ӗ����܂��B
	FILE_ATTRIBUTE_DEVICE				= 0x00000040, ///< �\��ς݁B�g��Ȃ��ł��������B
	FILE_ATTRIBUTE_DIRECTORY			= 0x00000010, ///< �w�肳�ꂽ�n���h���́A�f�B���N�g���Ɋ֘A���Ă��܂��B
	FILE_ATTRIBUTE_ENCRYPTED			= 0x00004000, ///< �w�肳�ꂽ�t�@�C���܂��̓f�B���N�g���͈Í�������Ă��܂��B�t�@�C���̏ꍇ�A�t�@�C�����̑S�f�[�^�X�g���[�����Í�������Ă��邱�Ƃ��Ӗ����܂��B�f�B���N�g���̏ꍇ�A���̃f�B���N�g�����ɐV�����쐬�����t�@�C���܂��̓T�u�f�B���N�g�����A����ňÍ�����ԂɂȂ邱�Ƃ��Ӗ����܂��B
	FILE_ATTRIBUTE_HIDDEN				= 0x00000002, ///< �B���t�@�C���܂��͉B���f�B���N�g���ł��B�ʏ�̃f�B���N�g�����X�e�B���O�ł͕\������܂���B
	FILE_ATTRIBUTE_NORMAL				= 0x00000080, ///< �w�肳�ꂽ�t�@�C���܂��̓f�B���N�g���ɂ́A���ɑ����͂���܂���B�P�ƂŕԂ����ꍇ�ɂ̂݁A���̑����͗L���ł��B
	FILE_ATTRIBUTE_NOT_CONTENT_INDEXED	= 0x00002000, ///< Windows 2000�F���̃t�@�C���́A�u�C���f�b�N�X�T�[�r�X�v�̑ΏۂɂȂ��Ă��܂���B
	FILE_ATTRIBUTE_OFFLINE				= 0x00001000, ///< Windows 2000�F���̃t�@�C���̃f�[�^�́A�����ɂ͗��p�ł��܂���B���̑����́A�t�@�C���̃f�[�^���I�t���C���̋L�����u�֕����I�Ɉړ����ꂽ���Ƃ������܂��B���̑����́AWindows 2000 �̊K�w�L���Ǘ��\�t�g�E�F�A�ł���u�����[�g�L����v�����p������̂ł��B�A�v���P�[�V�����́A�C�ӂɂ��̑�����ύX����ׂ��ł͂���܂���B
	FILE_ATTRIBUTE_READONLY				= 0x00000001, ///< ���̃t�@�C���܂��̓f�B���N�g���͓ǂݎ���p�ł��B�A�v���P�[�V�����͂��̃t�@�C���̓ǂݎ����s���܂����A�������݂�폜�͂ł��܂���B�f�B���N�g���̏ꍇ�A�A�v���P�[�V�����͍폜���s���܂���B
	FILE_ATTRIBUTE_REPARSE_POINT		= 0x00000400, ///< ���̃t�@�C���ɂ́A�ĉ�̓|�C���g���֘A�t�����Ă��܂��B
	FILE_ATTRIBUTE_SPARSE_FILE			= 0x00000200, ///< ���̃t�@�C���́A�X�p�[�X�t�@�C���i�a�ȃt�@�C���A���g�p�̗̈悪�����A�܂��͓����l�����������t�@�C���j�ł��B
	FILE_ATTRIBUTE_SYSTEM				= 0x00000004, ///< ���̃t�@�C���܂��̓f�B���N�g���́A�I�y���[�e�B���O�V�X�e���̈ꕔ�A�܂��̓I�y���[�e�B���O�V�X�e����p�ł��B
	FILE_ATTRIBUTE_TEMPORARY			= 0x00000100, ///< ���̃t�@�C���́A�ꎞ�t�@�C���Ƃ��Ďg���Ă��܂��B�t�@�C���V�X�e���́A�f�[�^���n�[�h�f�B�X�N�̂悤�ȑ�e�ʋL�����u�֏������ޑ���ɁA�����ȃA�N�Z�X���s����悤�A���ׂẴf�[�^�����������Ɉێ����邱�Ƃ����݂܂��B�A�v���P�[�V�����́A�K�v���Ȃ��Ȃ����i�K�ňꎞ�t�@�C���������ɍ폜����ׂ��ł��B	
};
#endif