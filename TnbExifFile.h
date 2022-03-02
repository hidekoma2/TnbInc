#pragma once
/**
 *@file
 * EXIF�K�i�t�@�C���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbGdiPlus.h"
#include "TnbBitmapHandle.h"
#include "TnbVector.h"
#include "TnbMap.h"



//TNB Library
namespace TNB
{



/**@ingroup GDIPLUS BITMAP
 * EXIF�K�i�t�@�C���Ǘ��N���X
 *
 *		��g�̉摜����EXIF�^�O�����Ǘ����A�t�@�C�����o�͂��s�����Ƃ��ł��܂��B
 *
 *	@attention �� UNICODE�v���W�F�N�g�ł��A�{�N���X�̕������ UNICODE �Ŏw�肷��K�v������܂��B
 *				CUnicode ���A�g�p���Ă��������B
 *
 *	@todo Windows7 �ɂ����ă��[�U�[�R�����g���������A�N�Z�X�ł��Ȃ����۔������B
 *
 *	@par�g�p��
 *		\code
 *		CExifFile ef;
 *		ef.Attach(L"c:\\A.bmp");		// �Ώۃt�@�C���A�^�b�`
 *		//����
 *		SYSTEMTIME st;
 *		::GetLocalTime(&st);
 *		ef.SetTagInfo(PropertyTagExifDTOrig, st);// ���t�ݒ�
 *
 *		ef.Save(L"c:\\B.jpg");		// �ۑ�
 *		\endcode
 *
 *	@par�K�v�t�@�C��
 *			TnbExifFile.h
 * 
 *	@date 09/01/21 �V�K�쐬�B
 *	@date 09/02/02 Save�������P�B
 *	@date 15/01/07 ���[�U�[�R�����g���������A�N�Z�X�o���Ȃ����۔����B���Ή��B
 */
class CExifFile : CCopyImpossible
{
public:
	/// ���������L����
	struct TRational
	{
		DWORD	molecular;		///< ���q
		DWORD	denominator;	///< ����
	};
	/// �����t���L����
	struct TSRational
	{
		long	molecular;		///< ���q
		long	denominator;	///< ����
	};

	/// �f�[�^�^
	enum EType
	{
		ET_Byte			= PropertyTagTypeByte,		///< 8�r�b�g�������������B
		ET_Ascii		= PropertyTagTypeASCII,		///< ASCII������B
		ET_Short		= PropertyTagTypeShort,		///< 16�r�b�g�������������B
		ET_Long			= PropertyTagTypeLong,		///< 32�r�b�g�������������B
		ET_Rational		= PropertyTagTypeRational,	///< 32�r�b�g���������L�����i���� 2�B���q�A����j�B
		ET_Undefined	= PropertyTagTypeUndefined,	///< �C�ӂ� 8bit�f�[�^�B
		ET_SLong		= PropertyTagTypeSLONG,		///< 32�r�b�g�����t�������B
		ET_SRational	= PropertyTagTypeSRational,	///< 32�r�b�g�����t���L�����i���� 2�B���q�A����j�B
	};

	/**@ingroup GDIPLUS BITMAP
	 * �^�O���.
	 *	@note �^�O����̏��
	 */
	class CTagInfo
	{
	public:
		/// �R���X�g���N�^.
		CTagInfo(void) : m_type(ET_Undefined), m_length(0)
		{
		}
		/**
		 * [�m�F] �f�[�^�L���m�F
		 *	@retval true �f�[�^����
		 *	@retval false �Ȃ��B
		 */
		bool IsEmpty(void) const
		{
			return m_length == 0;
		}
		/**
		 * [�擾] �f�[�^�^�擾
		 *	@return �^
		 */
		EType GetType(void) const
		{
			return m_type;
		}
		/**
		 * [�擾] �f�[�^���擾.
		 *	@note BYTE���ł͂���܂���B
		 *	@return �f�[�^��
		 */
		size_t GetLength(void) const
		{
			return m_length;
		}
		/**
		 * [�擾] 8bit�������������擾.
		 *	@return �f�[�^�B
		 */
		CVectorT<BYTE> GetByte(void) const
		{
			return m_Get(BYTE(), ET_Byte);
		}
		/**
		 * [�ݒ�] 8bit�������������ݒ�.
		 *	@param len �f�[�^��
		 *	@param P �f�[�^
		 */
		void SetByte(size_t len, const BYTE* P)
		{
			m_Set(len, P, ET_Byte);
		}
		/**
		 * [�ݒ�] 8bit�������������ݒ�.
		 *	@param b �f�[�^
		 */
		void SetByte(BYTE b)
		{
			SetByte(1, &b);
		}
		/**
		 * [�擾] ASCII�擾.
		 *	@retval NULL �^��ASCII������ȊO���A�T�C�Y�O�D
		 *	@retval NULL�ȊO ������
		 */
		LPCSTR GetAscii(void) const
		{
			LPCSTR P = NULL;
			if ( m_type == ET_Ascii )
			{
				P = reinterpret_cast<LPCSTR>(m_data.ReferBuffer());
			}
			return P;
		}
		/**
		 * [�ݒ�] ASCII�ݒ�.
		 *	@param lpsz ������B
		 *	@param len NULL���܂ޕ����񒷁B�ȗ��\�B�w�肷��ꍇ�Alpsz �����̃T�C�Y�A�N�Z�X�\�ł���悤�ɂ��邱�ƁB
		 */
		void SetAscii(LPCSTR lpsz, size_t len = INVALID_SIZE)
		{
			if ( lpsz == NULL )
			{
				m_Set(0, lpsz, ET_Ascii);
			}
			else
			{
				if ( len == INVALID_SIZE )
				{
					len = STRLIB::GetLen(lpsz) + 1;
				}
				ASSERT( ! ::IsBadReadPtr(lpsz, len) );
				m_Set(len, lpsz, ET_Ascii);
			}
		}
		/**
		 * [�擾] 16bit�������������擾.
		 *	@return �f�[�^�B
		 */
		CVectorT<WORD> GetShort(void) const
		{
			return m_Get(WORD(), ET_Short);
		}
		/**
		 * [�ݒ�] 16bit�������������ݒ�.
		 *	@param len �f�[�^��
		 *	@param P �f�[�^
		 */
		void SetShort(size_t len, const WORD* P)
		{
			m_Set(len, P, ET_Short);
		}
		/**
		 * [�ݒ�] 16bit�������������ݒ�.
		 *	@param w �f�[�^
		 */
		void SetShort(WORD w)
		{
			SetShort(1, &w);
		}
		/**
		 * [�擾] 32bit�������������擾.
		 *	@return �f�[�^�B
		 */
		CVectorT<DWORD> GetLong(void) const
		{
			return m_Get(DWORD(), ET_Long);
		}
		/**
		 * [�ݒ�] 32bit�������������ݒ�.
		 *	@param len �f�[�^��
		 *	@param P �f�[�^
		 */
		void SetLong(size_t len, const DWORD* P)
		{
			m_Set(len, P, ET_Long);
		}
		/**
		 * [�ݒ�] 32bit�������������ݒ�.
		 *	@param d �f�[�^
		 */
		void SetLong(DWORD d)
		{
			SetLong(1, &d);
		}
		/**
		 * [�擾] 32bit���������L�����擾.
		 *	@return �f�[�^�B
		 */
		CVectorT<TRational> GetRational(void) const
		{
			return m_Get(TRational(), ET_Rational);
		}
		/**
		 * [�ݒ�] 32bit���������L�����ݒ�.
		 *	@param len �f�[�^��
		 *	@param P �f�[�^
		 */
		void SetRational(size_t len, const TRational* P)
		{
			m_Set(len, P, ET_Rational);
		}
		/**
		 * [�ݒ�] 32bit���������L�����ݒ�.
		 *	@param d1 ���q
		 *	@param d2 ����
		 */
		void SetRational(DWORD d1, DWORD d2)
		{
			TRational r = { d1, d2 };
			SetRational(1, &r);
		}
		/**
		 * [�擾] �C�ӂ� 8bit�f�[�^�擾.
		 *	@return �f�[�^�B
		 */
		CByteVector GetUndefined(void) const
		{
			return m_Get(BYTE(), ET_Undefined);
		}
		/**
		 * [�ݒ�] �C�ӂ� 8bit�f�[�^�ݒ�.
		 *	@param len �f�[�^��
		 *	@param P �f�[�^
		 */
		void SetUndefined(size_t len, const BYTE* P)
		{
			m_Set(len, P, ET_Undefined);
		}
		/**
		 * [�擾] 32bit�����t�������擾.
		 *	@return �f�[�^�B
		 */
		CVectorT<long> GetSLong(void) const
		{
			return m_Get(long(), ET_SLong);
		}
		/**
		 * [�ݒ�] 32bit�������������ݒ�.
		 *	@param len �f�[�^��
		 *	@param P �f�[�^
		 */
		void SetSLong(size_t len, const long* P)
		{
			m_Set(len, P, ET_SLong);
		}
		/**
		 * [�ݒ�] 32bit�������������ݒ�.
		 *	@param l �f�[�^
		 */
		void SetSLong(long l)
		{
			SetSLong(1, &l);
		}
		/**
		 * [�擾] 32bit�����t���L�����擾.
		 *	@return �f�[�^�B
		 */
		CVectorT<TSRational> GetSRational(void) const
		{
			return m_Get(TSRational(), ET_SRational);
		}
		/**
		 * [�ݒ�] 32bit�����t���L�����ݒ�.
		 *	@param len �f�[�^��
		 *	@param P �f�[�^
		 */
		void SetSRational(size_t len, const TSRational* P)
		{
			m_Set(len, P, ET_SRational);
		}
		/**
		 * [�ݒ�] 32bit�����t���L�����ݒ�.
		 *	@param d1 ���q
		 *	@param d2 ����
		 */
		void SetSRational(long d1, long d2)
		{
			TSRational r = { d1, d2 };
			SetSRational(1, &r);
		}
		/**
		 * [�擾] �v���p�e�B�A�C�e���擾
		 *	@note �{�C���X�^���X�̏��� PropertyItem�̌`�Ɏ擾���܂��B
		 *	@note _pi.id �͑��삵�܂���B
		 *	@attention _pi.value �̃A�h���X�́A�{�C���X�^���X���̃A�h���X�������Ă��܂��B
		 *				�g�p�I���܂Ŗ{�C���X�^���X�͔j�����Ȃ��悤�A�����ӁB
		 *	@param[out] _pi �A�C�e��
		 *	@retval true ����
		 *	@retval false ���s
		 */
		bool Get(Gdiplus::PropertyItem& _pi)
		{
			if ( m_length != 0 )
			{
				_pi.type = static_cast<WORD>(m_type);
				_pi.length = ToDword(m_data.GetSize());
				_pi.value = m_data.GetBuffer();
				m_data.ReleaseBuffer();
				return true;
			}
			return false;
		}
		/**
		 * [�ݒ�] �v���p�e�B�A�C�e���ݒ�
		 *	@note PropertyItem�̏���{�C���X�^���X�ɐݒ肵�܂��B
		 *	@param p �A�C�e��
		 *	@retval true ����
		 *	@retval false ���s
		 */
		bool Set(const Gdiplus::PropertyItem& p)
		{
			switch ( p.type )
			{
			case ET_Byte: // 8�r�b�g�������������B
				SetByte(p.length, static_cast<const BYTE*>(p.value));
				break;
			case ET_Ascii: // ASCII������B
				SetAscii(static_cast<LPCSTR>(p.value));
				break;
			case ET_Short: // 16�r�b�g�������������B
				SetShort(p.length / sizeof(WORD), static_cast<const WORD*>(p.value));
				break;
			case ET_Long: // 32�r�b�g�������������B
				SetLong(p.length / sizeof(DWORD), static_cast<const DWORD*>(p.value));
				break;
			case ET_Rational: // 32�r�b�g���������L�����i���� 2�B���q�A����j�B
				SetRational(p.length / sizeof(TRational), static_cast<const TRational*>(p.value));
				break;
			case ET_Undefined: // �C�ӂ� 8bit�f�[�^�B
				SetUndefined(p.length, static_cast<const BYTE*>(p.value));
				break;
			case ET_SLong: // 32�r�b�g�����t�������B
				SetSLong(p.length / sizeof(long), static_cast<const long*>(p.value));
				break;
			case ET_SRational: // 32�r�b�g�����t���L�����i���� 2�B���q�A����j�B
				SetSRational(p.length / sizeof(TSRational), static_cast<const TSRational*>(p.value));
				break;
			default:
				return false;
			}
			return true;
		}
		/**
		 * [�擾] ������
		 *	@note �{�C���X�^���X�̏��𕶎���ɂ��܂��B
		 *	@param mx �z��̍ő�ϊ����B�ȗ�����ƑS���B
		 *	@return ������
		 */
		CStr ToString(size_t mx = INVALID_SIZE) const
		{
			CStr s;
			size_t ln = min(mx, m_length);
			switch ( m_type )
			{
			case ET_Undefined: // �C�ӂ� 8bit�f�[�^�B
			case ET_Byte: // 8�r�b�g�������������B
				{
					const BYTE* P = m_data.ReferBuffer();
					loop ( i, ln )
					{
						s += CStr::Fmt(_T("%u "), P[i]);
					}
				}
				break;
			case ET_Ascii: // ASCII������B
				s = reinterpret_cast<LPCSTR>(m_data.ReferBuffer());
				break;
			case ET_Short: // 16�r�b�g�������������B
				{
					const WORD* P = reinterpret_cast<const WORD*>(m_data.ReferBuffer());
					loop ( i, ln )
					{
						s += CStr::Fmt(_T("%u "), P[i]);
					}
				}
				break;
			case ET_Long: // 32�r�b�g�������������B
				{
					const DWORD* P = reinterpret_cast<const DWORD*>(m_data.ReferBuffer());
					loop ( i, ln )
					{
						s += CStr::Fmt(_T("%u "), P[i]);
					}
				}
				break;
			case ET_Rational: // 32�r�b�g���������L�����i���� 2�B���q�A����j�B
				{
					const TRational* P = reinterpret_cast<const TRational*>(m_data.ReferBuffer());
					loop ( i, ln )
					{
						s += CStr::Fmt(_T("%u/%u "), P[i].molecular, P[i].denominator);
					}
				}
				break;
			case ET_SLong: // 32�r�b�g�����t�������B
				{
					const long* P = reinterpret_cast<const long*>(m_data.ReferBuffer());
					loop ( i, ln )
					{
						s += CStr::Fmt(_T("%d "), P[i]);
					}
				}
				break;
			case ET_SRational: // 32�r�b�g�����t���L�����i���� 2�B���q�A����j�B
				{
					const TSRational* P = reinterpret_cast<const TSRational*>(m_data.ReferBuffer());
					loop ( i, ln )
					{
						s += CStr::Fmt(_T("%d/%d "), P[i].molecular, P[i].denominator);
					}
				}
				break;
			default:
				break;
			}
			s += CStr::Fmt(_T("(l=%d)"), m_length);
			return s;
		}
	private:
		EType		m_type;		///< �^
		CByteVector	m_data;		///< �f�[�^
		size_t		m_length;	///< �f�[�^��(byte���ł͂Ȃ�)
		// �Q�b�^�[
		template<typename T>
		CVectorT<T> m_Get(T dmy, EType t) const
		{
			CVectorT<T> r;
			if ( m_type == t )
			{
				ASSERT( m_data.GetSize() / sizeof(T) == m_length );
				r.SetElements(m_length, reinterpret_cast<const T*>(m_data.ReferBuffer()));
			}
			return r;
		}
		// �Z�b�^�[
		template<typename T>
		void m_Set(size_t len, T* P, EType t)
		{
			m_type = t;
			m_length = len;
			m_data.SetElements(len * sizeof(T), reinterpret_cast<const BYTE*>(P));
		}
	};

	typedef CMapT<PROPID, CTagInfo>	CMapTagInfos;	///<�^�O���̃}�b�v


	//----


	/// �R���X�g���N�^
	CExifFile(void) : m_pBmp(NULL)
	{
	}

	/// �f�X�g���N�^
	~CExifFile(void)
	{
		Empty();
	}

	/**
	 * [�ݒ�] �G���v�e�B��
	 *	@note �ێ����Ă�����J�����܂��B
	 */
	void Empty(void)
	{
		if ( m_pBmp != NULL )
		{
			delete m_pBmp;
			m_pBmp = NULL;
		}
		m_tags.RemoveAll();
	}

	/**
	 * [�Ǎ�] �t�@�C���Ǎ���(Attach).
	 *		�w��̃t�@�C����ǂݍ��݁A EXIF �^�O�����ǂݍ��݁A�{�C���X�^���X�ɋL�����܂��B
	 *		�w��̃t�@�C���̓A�^�b�`���́A���b�N����܂��B
	 *	@param lpszFileName �摜�t�@�C�����B
	 *	@retval true �Ǎ��ݐ���.
	 *	@retval false ���s.
	 */
	bool Attach(LPCWSTR lpszFileName)
	{
		CGdiPlus::Startup();
		Empty();
		m_pBmp = Gdiplus::Bitmap::FromFile(lpszFileName);
		if ( m_pBmp !=NULL && m_pBmp->GetLastStatus() == Gdiplus::Ok )
		{
			UINT cnt = m_pBmp->GetPropertyCount();
			CWorkMemT<PROPID> propIDs(cnt);
			m_pBmp->GetPropertyIdList(cnt, propIDs);
			loop( i, cnt )
			{
				PROPID id = propIDs[i];
				UINT size = m_pBmp->GetPropertyItemSize(id);
				if ( size > 0 )
				{
					CWorkMem mem(size);
					Gdiplus::PropertyItem* P = reinterpret_cast<Gdiplus::PropertyItem*>(mem.Ref());
					m_pBmp->GetPropertyItem(id, size, P);
					m_tags[id].Set(*P);
				}
			}
			return true;
		}
		Empty();
		return false;
	}

	/**
	 * [�Ǎ�] �t�@�C���Ǎ���.
	 *		�w��̃t�@�C����ǂݍ��݁A EXIF �^�O�����ǂݍ��݁A�{�C���X�^���X�ɋL�����܂��B
	 *	@note ���̊֐��͉摜���𕡐����܂��B�㏑����ړI�Ƃ��Ă��Ȃ��ꍇ�A Attach() �̂ق��������ł��B
	 *	@param lpszFileName �摜�t�@�C�����B
	 *	@retval true �Ǎ��ݐ���.
	 *	@retval false ���s.
	 */
	bool Load(LPCWSTR lpszFileName)
	{
		if ( Attach(lpszFileName) )
		{
			if ( m_pBmp != NULL )
			{
				delete m_pBmp;
				m_pBmp = NULL;
			}
			HPALETTE hPal = NULL;
			CBitmapHandle b = CGdiPlus::LoadBitmap(lpszFileName);
			m_pBmp = Gdiplus::Bitmap::FromHBITMAP(b, hPal);
			if ( m_pBmp != NULL && m_pBmp->GetLastStatus() == Gdiplus::Ok )
			{
				return true;
			}
		}
		Empty();
		return false;
	}

	/**
	 * [�Ǎ�] HBITMAP�Ǎ���.
	 *		�w���HBITMAP��{�C���X�^���X�ɋL�����܂��B
	 *	@param hBmp �摜�B
	 *	@param hPal �p���b�g�B�ȗ��\�B
	 *	@retval true �Ǎ��ݐ���.
	 *	@retval false ���s.
	 */
	bool Load(HBITMAP hBmp, HPALETTE hPal = NULL)
	{
		CGdiPlus::Startup();
		Empty();
		m_pBmp = Gdiplus::Bitmap::FromHBITMAP(hBmp, hPal);
		if ( m_pBmp !=NULL && m_pBmp->GetLastStatus() == Gdiplus::Ok )
		{
			return true;
		}
		Empty();
		return false;
	}

	/**
	 * [�쐬] Jpeg�t�@�C���쐬.
	 *		�{�C���X�^���X�ŊǗ����Ă���摜�� EXIF�^�O���� jpeg�t�@�C���ɏ����o���܂��B
	 *	@note Attach() ���Ă���ꍇ�A�㏑���ł��܂���B
	 *	@param lpsz �t�@�C�����B
	 *	@param jpegQuality JPEG�̃N�I���e�B(0�`100)�B�ȗ������80�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SaveJpeg(LPCWSTR lpsz, LONG jpegQuality = 80)
	{
		return Save(lpsz, CGdiPlus::JPG, jpegQuality);
	}

	/**
	 * [�쐬] �摜�t�@�C���쐬.
	 *		�{�C���X�^���X�ŊǗ����Ă���摜�� EXIF�^�O���� BMP, JPEG �Ȃǂ̃t�@�C���ɏ����o���܂��B
	 *	@note JPEG �ȊO�̏ꍇ EXIF�^�O��񂪏����o����Ȃ��\��������܂��i�K�i�j�B
	 *	@note Attach() ���Ă���ꍇ�A�㏑���ł��܂���B
	 *	@param lpsz �t�@�C�����B
	 *	@param fmt �t�H�[�}�b�g�B�ȗ������BMP�ł��B
	 *	@param jpegQuality JPEG�̃N�I���e�B(0�`100)�B�ȗ������80�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Save(LPCWSTR lpsz, CGdiPlus::EBitmapFormat fmt = CGdiPlus::JPG, LONG jpegQuality = 80)
	{
		bool r = m_Save(lpsz, m_pBmp, fmt, jpegQuality);
		if ( ! r && m_pBmp != NULL )
		{
			TRACE0("Exif�G�摜�č\�z\n");
			Gdiplus::Color c;
			c.SetFromCOLORREF(RGB(0, 0, 0));
			HBITMAP hBmp = NULL;
			if ( m_pBmp->GetHBITMAP(c, &hBmp) == Gdiplus::Ok && hBmp != NULL )
			{
				HPALETTE h = NULL;
				Gdiplus::Bitmap bm(hBmp, h);
				r = m_Save(lpsz, &bm, fmt, jpegQuality);
				_DeleteObject(hBmp);
			}
		}
		return r;
	}

	/**
	 * [�擾] Bitmap�擾.
	 *	@note �ێ����Ă���摜��Ԃ��܂��B
	 *	@retval NULL �ێ����Ă��Ȃ�
	 *	@return NULL�ȊO �摜�B delete ���Ȃ��ł��������B
	 */
	const Gdiplus::Bitmap* ReferBitmap(void) const
	{
		return m_pBmp;
	}

	/**
	 * [�擾] Bitmap�擾.
	 *	@note �ێ����Ă���摜��Ԃ��܂��B
	 *	@return �摜
	 */
	CBitmapHandle GetBitmap(void) const
	{
		CBitmapHandle b;
		if ( m_pBmp != NULL )
		{
			Gdiplus::Color c;
			c.SetFromCOLORREF(RGB(0, 0, 0));
			HBITMAP hBmp = NULL;
			if ( m_pBmp->GetHBITMAP(c, &hBmp) == Gdiplus::Ok && hBmp != NULL )
			{
				b = hBmp;
			}
		}
		return b;
	}

	/**
	 * [�Q��] EXIF�^�O���ꗗ�Q��.
	 *	@return �SEXIF�^�O���
	 */
	const CMapTagInfos& GetTagInfoMap(void) const
	{
		#ifdef _DEBUG
			loop ( i, m_tags.GetSize() )
			{
				TRACE2("tag [%04X] = %s\n", m_tags.Get(i).first, m_tags.Get(i).second.ToString(16));
			}
		#endif
		return m_tags;
	}
	
	/**
	 * [�擾] EXIF�^�O���擾
	 *	@param id ID
	 *	@retval NULL �w���ID�̏��Ȃ�
	 *	@retval NULL�ȊO ���
	 */
	const CTagInfo* GetTagInfo(PROPID id) const
	{
		return m_tags.GetValuePtr(id);
	}

	/**
	 * [�擾] UserComment�^�O�擾.
	 *	@note PropertyTagExifUserComment �^�O��������擾���܂��B
	 *	@return ���[�U�R�����g
	 */
	CStr GetUserCommentTag(void) const
	{
		CStr s;
		const CTagInfo* P = GetTagInfo(PropertyTagExifUserComment);
		if ( P != NULL )
		{
			CByteVector vb = P->GetUndefined();
			if ( vb.GetSize() >= 8 )
			{
				vb.Add(0); //NULL�I�[�ǉ�
				vb.Add(0); //NULL�I�[�ǉ�
				LPCSTR lpsz = reinterpret_cast<LPCSTR>(vb.ReferBuffer());
				bool isUnicode = (STRLIB::Compare(lpsz, "UNICODE") == 0);
				if ( lpsz[0] == 0 && sizeof(TCHAR) == 2 )
				{
					isUnicode = true;
				}
				if ( isUnicode )
				{
					s = reinterpret_cast<LPCWSTR>(&lpsz[8]);
				}
				else
				{
					s = &lpsz[8];
				}
			}
		}
		return s;
	}

	/**
	 * [�擾] Copyright�^�O�擾.
	 *	@note PropertyTagCopyright �^�O��������擾���܂��B
	 *	@param[out] _s1 �B�e���쌠��
	 *	@param[out] _s2 �ҏW���쌠��
	 */
	void GetCopyrightTag(CStr& _s1, CStr& _s2) const
	{
		_s1.Empty();
		_s2.Empty();
		const CTagInfo* P = GetTagInfo(PropertyTagCopyright);
		if ( P != NULL )
		{
			size_t len = P->GetLength();
			LPCSTR lpsz = P->GetAscii();
			_s1 = lpsz;
			if ( _s1.GetLength() + 1 < len && lpsz != NULL )
			{
				_s2 = &lpsz[_s1.GetLength() + 1];
			}
		}
	}

	/**
	 * [�ݒ�] EXIF�^�O���폜.
	 *	@param id ID
	 */
	void RemoveTagInfo(PROPID id)
	{
		m_tags.RemoveKey(id);
	}
	
	/**
	 * [�ݒ�] EXIF�^�O���ݒ�.
	 *		�C�ӂ�ID�ɔC�ӂ̏���ݒ�ł��܂��B
	 *	@param id ID
	 *	@param ti ���
	 */
	void SetTagInfo(PROPID id, const CTagInfo& ti)
	{
		m_tags[id] = ti;
	}

	/**
	 * [�ݒ�] EXIF�^�O���ݒ�(ASCII������).
	 *		�C�ӂ�ID�ɕ�����̏���ݒ�ł��܂��B
	 *	@param id ID
	 *	@param lpsz ������
	 *	@param len NULL���܂ޕ����񒷁B�ȗ��\�B�w�肷��ꍇ�Alpsz �����̃T�C�Y�A�N�Z�X�\�ł���悤�ɂ��邱�ƁB
	 */
	void SetTagInfo(PROPID id, LPCSTR lpsz, size_t len = INVALID_SIZE)
	{
		m_tags[id].SetAscii(lpsz, len);
	}

	/**
	 * [�ݒ�] EXIF�^�O���ݒ�(����ASCII������).
	 *	@note �w��̎��Ԃ� ASCI 20�����Ŋi�[���܂��B
	 *	@param id ID�B�B�e���Ԃ̏ꍇ�A PropertyTagExifDTOrig ���w�肵�܂��B
	 *	@param st ����
	 */
	void SetTagInfo(PROPID id, const SYSTEMTIME& st)
	{
		char buf[64];//������
		STRLIB::PrintF(buf, 64, "%04d:%02d:%02d %02d:%02d:%02d"
			, st.wYear, st.wMonth, st.wDay
			,st.wHour, st.wMinute, st.wSecond);
		SetTagInfo(id, buf);
	}

	/**
	 * [�ݒ�] �o�[�W�����^�O�ݒ�.
	 *		�o�[�W�������Ɏw��̃o�[�W������ݒ�ł��܂��B
	 *	@note PropertyTagExifVer �^�O�ɏ���ݒ肵�܂��B
	 *	@param ver �o�[�W�����B�ȗ������ 0x0210 �����蓖�Ă��܂��B
	 */
	void SetVersionTag(DWORD ver = 0x0210)
	{
		CTagInfo ti;
		CAscii s;
		s.Format("%04X", ver);
		LPCSTR lpsz = s;
		ti.SetUndefined(4, reinterpret_cast<const BYTE*>(lpsz));
		SetTagInfo(PropertyTagExifVer, ti);
	}

	/**
	 * [�ݒ�] UserComment�^�O�ݒ�(ASCII/JIS).
	 *		���[�U�R�����g���Ɏw��̕������ݒ�ł��܂��B
	 *	@note PropertyTagExifUserComment �^�O�ɏ���ݒ肵�܂��B
	 *	@param lpsz �R�����g
	 */
	void SetUserCommentTag(LPCSTR lpsz)
	{
		size_t len = STRLIB::GetLen(lpsz);
		CWorkMemT<BYTE> mm(8 + len);
		if ( STRLIB::IsAsciiOnly(lpsz) )
		{
			const BYTE bb[8] = { 'A', 'S', 'C', 'I', 'I', 0, 0, 0 };
			TNB::MemCopy(&mm[0], bb, 8);
		}
		else
		{
			const BYTE bb[8] = { 'J', 'I', 'S', 0, 0, 0, 0, 0 };
			TNB::MemCopy(&mm[0], bb, 8);
		}
		TNB::MemCopy(&mm[8], lpsz, len);
		CTagInfo ti;
		ti.SetUndefined(8 + len, mm);
		SetTagInfo(PropertyTagExifUserComment, ti);
	}

	/**
	 * [�ݒ�] UserComment�^�O�ݒ�(UNICODE).
	 *		���[�U�R�����g���Ɏw��̕������ݒ�ł��܂��B
	 *	@note PropertyTagExifUserComment �^�O�ɏ���ݒ肵�܂��B
	 *	@param lpsz �R�����g
	 */
	void SetUserCommentTag(LPCWSTR lpsz)
	{
		size_t len = STRLIB::GetLen(lpsz) * sizeof(WCHAR);
		CWorkMemT<BYTE> mm(8 + len);
		const BYTE bb[8] = { 'U', 'N', 'I', 'C', 'O', 'D', 'E', 0 };
		TNB::MemCopy(&mm[0], bb, 8);
		TNB::MemCopy(&mm[8], lpsz, len);
		CTagInfo ti;
		ti.SetUndefined(8 + len, mm);
		SetTagInfo(PropertyTagExifUserComment, ti);
	}

	/**
	 * [�擾] Copyright�^�O�擾.
	 *	@note PropertyTagCopyright �^�O�ɏ���ݒ肵�܂��B
	 *	@param lpsz1 �B�e���쌠�ҁB
	 *	@param lpsz2 �ҏW���쌠�ҁB
	 */
	void SetCopyrightTag(LPCSTR lpsz1, LPCSTR lpsz2 = NULL)
	{
		CAscii s2 = lpsz2;
		if ( s2.IsEmpty() )
		{
			SetTagInfo(PropertyTagCopyright, lpsz1);
		}
		else
		{
			CAscii s1 = lpsz1;
			if ( s1.IsEmpty() )
			{
				s1 = " ";
			}
			CWorkMemT<char> mm(s1.GetLength() + s2.GetLength() + 2);
			STRLIB::Copy(&mm[0], s1);
			STRLIB::Copy(&mm[s1.GetLength() + 1], s2);
			SetTagInfo(PropertyTagCopyright, &mm[0], mm.GetSize());
		}
	}

	/**
	 * [�ݒ�] GPS�o�[�W�������ݒ�
	 *	@note PropertyTagGpsVer �^�O�ɏ���ݒ肵�܂��B
	 */
	void SetGpsVersionTag(void)
	{
		const BYTE a[4] = {2, 0, 0, 0};
		CTagInfo ti;
		ti.SetByte(4, a);
		SetTagInfo(PropertyTagGpsVer, ti);
	}

	/**
	 * [�ݒ�] �ܓx�ݒ�
	 *	@note PropertyTagGpsVer / PropertyTagGpsLatitudeRef / PropertyTagGpsLatitude �^�O�ɏ���ݒ肵�܂��B
	 *	@param isNorth true �Ȃ�k�܁A false �Ȃ���
	 *	@param dd D.
	 *	@param mm M.
	 *	@param ss S.
	 */
	void SetGpsLatitudeTag(bool isNorth, DWORD dd, DWORD mm, double ss)
	{
		SetGpsVersionTag();
		SetTagInfo(PropertyTagGpsLatitudeRef, isNorth ? "N" : "S");
		TRational r[3] = { dd, 1, mm, 1, static_cast<DWORD>(ss * 1000), 1000 };
		CTagInfo ti;
		ti.SetRational(3, r);
		SetTagInfo(PropertyTagGpsLatitude, ti);
	}

	/**
	 * [�ݒ�] �o�x�ݒ�
	 *	@note PropertyTagGpsVer / PropertyTagGpsLongitudeRef / PropertyTagGpsLongitude �^�O�ɏ���ݒ肵�܂��B
	 *	@param isEast true �Ȃ琼�o�A false �Ȃ瓌�o
	 *	@param dd D.
	 *	@param mm M.
	 *	@param ss S.
	 */
	void SetGpsLongitudeTag(bool isEast, DWORD dd, DWORD mm, double ss)
	{
		SetGpsVersionTag();
		SetTagInfo(PropertyTagGpsLongitudeRef, isEast ? "E" : "W");
		TRational r[3] = { dd, 1,  mm, 1,  static_cast<DWORD>(ss * 1000), 1000 };
		CTagInfo ti;
		ti.SetRational(3, r);
		SetTagInfo(PropertyTagGpsLongitude, ti);
	}

	/**
	 * [�ݒ�] ���x�ݒ�
	 *	@note PropertyTagGpsVer / PropertyTagGpsAltitudeRef / PropertyTagGpsAltitude �^�O�ɏ���ݒ肵�܂��B
	 *	@param d1 �l1�B
	 *	@param d2 �l2�B�ȗ�����ƂP�B
	 */
	void SetGpsAltitudeTag(DWORD d1, DWORD d2 = 1)
	{
		SetGpsVersionTag();
		CTagInfo ti;
		ti.SetByte(0);
		SetTagInfo(PropertyTagGpsAltitudeRef, ti);
		ti.SetRational(d1, d2);
		SetTagInfo(PropertyTagGpsAltitude, ti);
	}

private:
	// �摜�t�@�C���쐬.
	bool m_Save(LPCWSTR lpsz, Gdiplus::Bitmap* B, CGdiPlus::EBitmapFormat fmt = CGdiPlus::JPG, LONG jpegQuality = 80)
	{
		CGdiPlus::Startup();
		if ( B != NULL )
		{
			UINT cnt = B->GetPropertyCount();
			CWorkMemT<PROPID> propIDs(cnt);
			B->GetPropertyIdList(cnt, propIDs);
			loop( i, cnt )
			{
				PROPID id = propIDs[i];
				B->RemovePropertyItem(id);
			}
			Gdiplus::PropertyItem pi;
			loop ( i, m_tags.GetSize() )
			{
				pi.id = m_tags.Ref(i).first;
				if ( m_tags.Ref(i).second.Get(pi) )
				{
					if ( B->SetPropertyItem(&pi) != Gdiplus::Ok )
					{
						return false;
					}
				}
			}
			return CGdiPlus::SaveBitmap(lpsz, *B, fmt, jpegQuality);
		}
		return false;
	}

	Gdiplus::Bitmap*		m_pBmp;	///< �ێ����Ă���摜
	CMapTagInfos			m_tags;	///< �^�O���
};



}; // TNB

