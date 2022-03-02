#pragma once
/**
 *@file
 * �r�b�g�}�b�v�|�[�^�u���t�H�[�}�b�g�Ǘ��֌W�̃w�b�_.
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbBitmapImage.h"
#include "TnbBlockLinkVector.h"
#include "TnbFile.h"
#include "TnbStrOperator.h"



//TNB Library
namespace TNB
{



/**@ingroup BITMAP
 * �r�b�g�}�b�v�|�[�^�u���t�H�[�}�b�g�N���X.
 *		PNM (Portable aNyMap �`���ANetpbm�`��) �摜�`���̍쐬(��������)�A
 *		����щ��(�ǂݍ���)���T�|�[�g���Ă��܂��B
 *
 *	@todo 16bit�͖��Ή��B8bit�̂ݑΉ����Ă��܂��B
 *	@todo �ǂݍ��ݎ��A��؂蕶���̓X�y�[�X�����s�Ɍ��肳��Ă��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapPortableFormat.h
 *
 *	@date 15/05/01 �V�K�쐬�B
 *	@date 15/05/07 P4 �����BP1 �̔������t�������̂��C���B
 */
class CBitmapPortableFormat
{
public:

	/// ���
	enum EDescriptor
	{
		P1,				///< Portable BitMap ASCII (.PBM)
		P2,				///< Portable GrayMap ASCII (.PGM)
		P3,				///< Portable PixMap ASCII (.PPM)
		P4,				///< Portable BitMap Binary (.PBM)
		P5,				///< Portable GrayMap Binary (.PGM)
		P6,				///< Portable PixMap Binary (.PPM)
	};

	/**
	 * [�擾] �ŏI�ݒ��ގ擾.
	 *	@note �߂�l�� Set() / Load() ���サ���Ӗ��������܂���B
	 *	@return ���.
	 */
	EDescriptor GetLastDescriptor(void) const
	{
		return m_desc;
	}

	/**
	 * [�ݒ�] �ݒ�.
	 *		�w��̃f�[�^��ǂݍ��݁A�{�C���X�^���X��24bit�J���[�ŋL�����܂��B�o�C�i���A�e�L�X�g�Ƃ���OK�ł��B
	 *	@param size �f�[�^�T�C�Y
	 *	@param pData �f�[�^
	 *	@retval true �����B��ނ� GetLastDescriptor() �Œm�邱�Ƃ��o���܂��B
	 *	@retval false ���s
	 */
	bool Set(size_t size, LPCVOID pData)
	{
		CBlockLinkVectorT<TParam> params;
		const char* contents = static_cast<const char*>(pData);
		const char* lpszText = contents;
		INDEX ind = 0;
		CAscii pa;
		bool isFinishied = false;
		while ( ! isFinishied )
		{
			const char* p = lpszText;
			size_t len = 0;
			while ( true )
			{
				if ( *p == 0 )
				{
					if ( len == 0 )
					{
						isFinishied = true;
					}
					break;
				}
				else if ( *p == 0x0D || *p == 0x0A )
				{
					p++;
					if ( *p == 0x0A )
					{
						p++;
					}
					break;
				}
				int s = STRLIB::GetCharSize(*p);
				p += s;
				len += s;
			}
			if ( ! isFinishied )
			{
				char c = contents[ind];
				if ( c == '#' || c == 0 )
				{
					; //�L�����Ȃ�
				}
				else if ( c < 0x20 && c > 'P' )
				{
					break; //�o�C�i���f�[�^
				}
				else
				{
					params.Add(TParam(ind, len));
					if ( pa.IsEmpty() )
					{
						pa.SetFromLeft(&contents[params[0].index], params[0].size);
					}
					if ( pa == "P4" && params.GetSize() >= 2 )
					{
						break;
					}
					if ( (pa == "P5" || pa == "P6") && params.GetSize() >= 3 )
					{
						break;
					}
				}
				ind += p - lpszText;
				lpszText += p - lpszText;
			}
		}
		//
		if ( params.GetSize() < 2 )
		{
			return false; 
		}
		DWORD colorDips = 0;	// �J���[�[��
		SIZE sz;			// �摜�T�C�Y
		// �T�C�Y�`�F�b�N
		CAscii a;
		a.SetFromLeft(&contents[params[1].index], params[1].size);
		while ( a.Replace("\t", " ") != 0 );
		while ( a.Replace("  ", " ") != 0 );
		CAsciiVector va = CAsciiOperator::SeparatePeriod(a, ' ');
		if ( va.GetSize() != 2 )
		{
			return false;
		}
		sz.cx = va[0].ToInt();
		sz.cy = va[1].ToInt();
		if ( sz.cx == 0 || sz.cy == 0 )
		{
			return false;
		}
		//
		if ( pa == "P2" || pa == "P3" || pa == "P5" || pa == "P6" )
		{
			// P2, P3, P5, P6 �̓J���[�[��������
			if ( params.GetSize() < 3 )
			{
				return false; 
			}
			const char * pp = &contents[params[2].index];
			colorDips = STRLIB::ToInt(pp);
		}
		if ( pa == "P1" || pa == "P2" || pa == "P3" )
		{
			// P1, P2, P3
			int ex = (pa == "P1") ? 2 : 3;
			CVectorT<DWORD> bm;
			loop ( i, params.GetSize() - ex )
			{
				a.SetFromLeft(&contents[params[i + ex].index], params[i + ex].size);
				while ( a.Replace("\t", " ") != 0 );
				while ( a.Replace("  ", " ") != 0 );
				CAsciiVector va = CAsciiOperator::SeparatePeriod(a, ' ');
				loop ( j, va )
				{
					bm.Add(va[j].ToInt());
				}
			}
			if ( pa == "P1" )
			{
				// P1
				if ( size_t(sz.cx * sz.cy) > bm.GetSize() )
				{
					return false;
				}
				m_bmp.Set(sz.cx, sz.cy, RGB(0, 0, 0));
				m_bmp.ToHaveRgbData();
				loop ( y, sz.cy )
				{
					RGBQUAD* p = m_bmp.GetRgbDataPtr(y);
					int m = y * sz.cx;
					loop ( x, sz.cx )
					{
						if ( bm[m + x] == 0 )
						{
							p->rgbRed = 255;
							p->rgbGreen = 255;
							p->rgbBlue = 255;
						}
						p++;
					}
				}
				m_desc = P1;
				return true;
			}
			else if ( pa == "P2" )
			{
				// P2
				if ( size_t(sz.cx * sz.cy) > bm.GetSize() )
				{
					return false;
				}
				m_bmp.Set(sz.cx, sz.cy, RGB(0, 0, 0));
				m_bmp.ToHaveRgbData();
				loop ( y, sz.cy )
				{
					RGBQUAD* p = m_bmp.GetRgbDataPtr(y);
					int m = y * sz.cx;
					loop ( x, sz.cx )
					{
						BYTE c = static_cast<BYTE>(bm[m + x] * 255 / colorDips);
						p->rgbRed = c;
						p->rgbGreen = c;
						p->rgbBlue = c;
						p++;
					}
				}
				m_desc = P2;
				return true;
			}
			else // pa == "P3"
			{
				// P3
				if ( size_t(sz.cx * sz.cy * 3) > bm.GetSize() )
				{
					return false;
				}
				m_bmp.Set(sz.cx, sz.cy, RGB(0, 0, 0));
				m_bmp.ToHaveRgbData();
				loop ( y, sz.cy )
				{
					RGBQUAD* p = m_bmp.GetRgbDataPtr(y);
					int m = y * sz.cx * 3;
					loop ( x, sz.cx )
					{
						p->rgbRed = static_cast<BYTE>(bm[m + x * 3 + 0] * 255 / colorDips);
						p->rgbGreen = static_cast<BYTE>(bm[m + x * 3 + 1] * 255 / colorDips);
						p->rgbBlue = static_cast<BYTE>(bm[m + x * 3 + 2] * 255 / colorDips);
						p++;
					}
				}
				m_desc = P3;
				return true;
			}
		}
		else if ( pa == "P4" || pa == "P5" || pa == "P6" )
		{
			int ex = (pa == "P4") ? 1 : 2;
			const char* q0 = &contents[params[ex].index + params[ex].size];
			size_t len = size - (params[ex].index + params[ex].size);
			if ( *q0 == 0x0D || *q0 == 0x0A )
			{
				q0++;
				len--;
				//if ( *q0 == 0x0A )
				//{
				//	q0++;
				//	len--;
				//}
			}
			const BYTE* q = reinterpret_cast<const BYTE*>(q0);
			if ( pa == "P4" )
			{
				// P4
				if ( size_t(((sz.cx + 7 ) / 8) * sz.cy) > len )
				{
					return false;
				}
				m_bmp.Set(sz.cx, sz.cy, RGB(0, 0, 0));
				m_bmp.ToHaveRgbData();
				loop ( y, sz.cy )
				{
					RGBQUAD* p = m_bmp.GetRgbDataPtr(y);
					int m = y * ((sz.cx + 7) / 8);
					loop ( x, sz.cx )
					{
						if ( (q[m + (x / 8)] & _BIT(7 - (x & 7))) == 0 )
						{
							p->rgbRed = 255;
							p->rgbGreen = 255;
							p->rgbBlue = 255;
						}
						p++;
					}
				}
				m_desc = P4;
				return true;
			}
			else if ( pa == "P5" )
			{
				// P5
				if ( size_t(sz.cx * sz.cy) > len )
				{
					return false;
				}
				m_bmp.Set(sz.cx, sz.cy, RGB(0, 0, 0));
				m_bmp.ToHaveRgbData();
				loop ( y, sz.cy )
				{
					RGBQUAD* p = m_bmp.GetRgbDataPtr(y);
					int m = y * sz.cx;
					loop ( x, sz.cx )
					{
						BYTE c = static_cast<BYTE>(q[m + x] * 255 / colorDips);
						p->rgbRed = c;
						p->rgbGreen = c;
						p->rgbBlue = c;
						p++;
					}
				}
				m_desc = P5;
				return true;
			}
			else // pa == "P6"
			{
				// P6
				if ( size_t(sz.cx * sz.cy * 3) > len )
				{
					return false;
				}
				m_bmp.Set(sz.cx, sz.cy, RGB(0, 0, 0));
				m_bmp.ToHaveRgbData();
				loop ( y, sz.cy )
				{
					RGBQUAD* p = m_bmp.GetRgbDataPtr(y);
					int m = y * sz.cx * 3;
					loop ( x, sz.cx )
					{
						p->rgbRed = static_cast<BYTE>(q[m + x * 3 + 0] * 255 / colorDips);
						p->rgbGreen = static_cast<BYTE>(q[m + x * 3 + 1] * 255 / colorDips);
						p->rgbBlue = static_cast<BYTE>(q[m + x * 3 + 2] * 255 / colorDips);
						p++;
					}
				}
				m_desc = P6;
				return true;
			}
		}
		else
		{
			return false;
		}
		return true;
	}

	/**
	 * [�ݒ�] �t�@�C���Ǎ���.
	 *		�w��̃t�@�C����ǂݍ��݁A�{�C���X�^���X��24bit�J���[�ŋL�����܂��B
	 *	@param lpszFileName �t�@�C����
	 *	@retval true �����B��ނ� GetLastDescriptor() �Œm�邱�Ƃ��o���܂��B
	 *	@retval false ���s
	 */
	bool Load(LPCTSTR lpszFileName)
	{
		CFileReader rr;
		if ( rr.Open(lpszFileName) )
		{
			CByteVector vb = rr.ReadExactly();
			return Set(vb.GetSize(), vb.ReferBuffer());
		}
		return false;
	}

	/**
	 * [�ϊ�] �o�C�i���ϊ�.
	 *	@param desc ���
	 *	@param prm �p�����[�^�B ��ނ� P1,P4 �̏ꍇ�u�~���l�v�A���̑��̎�ނ̏ꍇ�u�F�X�P�[���̍ő�l�v���w�肵�܂��B
	 *	@return �ϊ���o�C�i��
	 */
	CByteVector ToVector(EDescriptor desc, DWORD prm, LPCTSTR lpszComment = NULL) const
	{
		CByteVector vb;
		if ( ! m_bmp.IsEmpty() )
		{
			CAscii a;
			CAscii asz;
			SIZE sz = m_bmp.GetSize();
			asz.Format("%d %d\n", sz.cx, sz.cy);
			if ( lpszComment != NULL )
			{
				asz.Format("#%s\n%d %d\n", CAscii(lpszComment), sz.cx, sz.cy);
			}
			else
			{
				asz.Format("%d %d\n", sz.cx, sz.cy);
			}
			switch ( desc )
			{
			case P1:
				{
					CBitmapImage bm = m_bmp.CreateGrayscaleBitmap();
					a = "P1\n" + asz;
					INDEX idx = 0;
					loop ( y, sz.cy )
					{
						const RGBQUAD* p = bm.GetRgbDataPtr(y);
						loop ( x, sz.cx )
						{
							if ( p->rgbRed < prm )
							{
								a += "1 "; 
							}
							else
							{
								a += "0 "; 
							}
							if ( idx++ >= 32 )
							{
								a.TrimRight(' ');
								a += "\n";
								idx = 0;
							}
							p++;
						}
					}
					vb.AddElements(a.GetLength(), reinterpret_cast<const BYTE*>(a.ReferBuffer()));
				}
				break;
			case P2:
				{
					CBitmapImage bm = m_bmp.CreateGrayscaleBitmap();
					a = "P2\n" + asz + CAscii::Fmt("%d\n", prm);
					INDEX idx = 0;
					loop ( y, sz.cy )
					{
						const RGBQUAD* p = bm.GetRgbDataPtr(y);
						loop ( x, sz.cx )
						{
							int q = p->rgbRed * prm / 255;
							a += CAscii::Fmt("%d ", q);
							if ( idx++ >= 32 )
							{
								a.TrimRight(' ');
								a += "\n";
								idx = 0;
							}
							p++;
						}
					}
					vb.AddElements(a.GetLength(), reinterpret_cast<const BYTE*>(a.ReferBuffer()));
				}
				break;
			case P3:
				{
					a = "P3\n" + asz + CAscii::Fmt("%d\n", prm);
					INDEX idx = 0;
					loop ( y, sz.cy )
					{
						const RGBQUAD* p = m_bmp.GetRgbDataPtr(y);
						loop ( x, sz.cx )
						{
							int r = p->rgbRed * prm / 255;
							int g = p->rgbGreen * prm / 255;
							int b = p->rgbBlue * prm / 255;
							a += CAscii::Fmt("%d %d %d ", r, g, b);
							if ( idx++ >= 32 )
							{
								a.TrimRight(' ');
								a += "\n";
								vb.AddElements(a.GetLength(), reinterpret_cast<const BYTE*>(a.ReferBuffer()));
								a.Empty();
								idx = 0;
							}
							p++;
						}
					}
					if ( a.GetLength() > 0 )
					{
						vb.AddElements(a.GetLength(), reinterpret_cast<const BYTE*>(a.ReferBuffer()));
					}
				}
				break;
			case P4:
				{
					CBitmapImage bm = m_bmp.CreateGrayscaleBitmap();
					a = "P4\n" + asz;
					vb.AddElements(a.GetLength(), reinterpret_cast<const BYTE*>(a.ReferBuffer()));
					CByteVector v;
					int yyy = (sz.cx + 7) / 8;
					v.SetSize(sz.cy * yyy);
					loop ( y, sz.cy )
					{
						const RGBQUAD* p = bm.GetRgbDataPtr(y);
						loop ( x, sz.cx )
						{
							BYTE& b = v.Ref((y * yyy) + (x / 8));
							if ( p->rgbRed < prm )
							{
								b |= _BIT(7 - (x & 7)); 
							}
							else
							{
								b  &= ~(_BIT(7 - (x & 7))); 
							}
							p++;
						}
					}
					vb += v;
				}
				break;
			case P5:
				{
					CBitmapImage bm = m_bmp.CreateGrayscaleBitmap();
					a = "P5\n" + asz + CAscii::Fmt("%d\n", prm);
					vb.AddElements(a.GetLength(), reinterpret_cast<const BYTE*>(a.ReferBuffer()));
					loop ( y, sz.cy )
					{
						const RGBQUAD* p = bm.GetRgbDataPtr(y);
						loop ( x, sz.cx )
						{
							int q = p->rgbRed * prm / 255;
							vb.Add(static_cast<BYTE>(q));
							p++;
						}
					}
				}
				break;
			case P6:
				{
					a = "P6\n" + asz + CAscii::Fmt("%d\n", prm);
					vb.AddElements(a.GetLength(), reinterpret_cast<const BYTE*>(a.ReferBuffer()));
					loop ( y, sz.cy )
					{
						const RGBQUAD* p = m_bmp.GetRgbDataPtr(y);
						loop ( x, sz.cx )
						{
							int r = p->rgbRed * prm / 255;
							int g = p->rgbGreen * prm / 255;
							int b = p->rgbBlue * prm / 255;
							vb.Add(static_cast<BYTE>(r));
							vb.Add(static_cast<BYTE>(g));
							vb.Add(static_cast<BYTE>(b));
							p++;
						}
					}
				}
				break;
			}
		}
		return vb;
	}

	/**
	 * [�ۑ�] �t�@�C����������.
	 *		�{�C���X�^���X�ɋL�����Ă���摜���w��̎�ނŎw��̃t�@�C���֏������݂܂��B
	 *	@param lpszFileName �t�@�C����
	 *	@param desc ���
	 *	@param prm �p�����[�^�B ��ނ� P1,P4 �̏ꍇ�u�~���l�v�A���̑��̎�ނ̏ꍇ�u�F�X�P�[���̍ő�l�v���w�肵�܂��B
	 *	@param lpszComment �R�����g�B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Save(LPCTSTR lpszFileName, EDescriptor desc, DWORD prm, LPCTSTR lpszComment = NULL) const
	{
		CByteVector vb = ToVector(desc, prm, lpszComment);
		if ( vb.GetSize() > 0 )
		{
			return CFileWriter::NewFile(lpszFileName, vb.GetSize(), vb.ReferBuffer());
		}
		return false;
	}

	/**
	 * [�Q��] �摜�Q��.
	 *	@return �摜�N���X
	 */
	CBitmapImage& Bitmap(void) 
	{
		return m_bmp;
	}

private:
	/// �p�����[�^
	struct TParam
	{
		INDEX	index;		///< �C���f�b�N�X
		size_t	size;		///< �T�C�Y
		// �R���X�g���N�^.
		TParam(INDEX i = 0, size_t s = 0) : index(i), size(s)
		{
		}
	};

	CBitmapImage	m_bmp;		///< �ێ��摜
	EDescriptor		m_desc;		///< �ێ��t�H�[�}�b�g
};



}; // TNB
