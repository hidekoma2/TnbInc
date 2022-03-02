#pragma once
/**
 *@file
 * �J���p�ɁA�������̏�Ԃ𕶎���ɂ���֐�������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStrVector.h"
#include "TnbTinyStringFormater.h"



//TNB Library
namespace TNB
{


	
/**@ingroup ETC
 * [�쐬] �o�C�i���_���v�p�����z��쐬.
 *		�w��f�[�^��16�i�\�L������ɕϊ����܂��B
 *	@par�K�v�t�@�C��
 *			TnbDump.h
 *	@param size		Dump���钷��
 *	@param pData	Dump����f�[�^
 *	@param canAllDump �ȗ��� true �Ȃ�w��T�C�YDump����Bfalse �Ȃ�r�����Ȃ��B
 *	@param width ���B�ȗ�����ƂP�U�D
 *	@param isUnicode �ȗ��� false �Ȃ� S-JIS�A true �Ȃ� UNICODE �Ƃ��ĕ\�����܂��B
  *	@return �ϊ����ʂ��i�[���Ă��镶���z��
 */
//template<typename TYP>
inline CStrVector DumpData(size_t size, const void* pData, bool canAllDump = true, size_t width = 16, bool isUnicode = false)
{
	CStrVector sa;
	if ( width < 2 )
	{
		width = 2;
	}
	size_t bufferLength = (21 + width * 3 + width + 8);
	CWorkMemT<TCHAR> buffer(bufferLength);
	LPTSTR lpszBuffer = buffer.Ref();
	const BYTE* B = reinterpret_cast<const BYTE*>(pData);
	bool isLastSecondByte = false;
	for ( size_t j = 0; j < size; j += width )
	{
		CTinyStringFormater sf;
		sf.Initialize(lpszBuffer, bufferLength);
		sf.AddFormatHexInt(down_cast<DWORD>(j), 4);
		sf.AddLiteral(_T(" : "));
		loop ( i, width )
		{
			if ( i + j >= size )
			{
				sf.AddLiteral(_T("-- "));
			}
			else
			{
				BYTE c = B[i + j];
				sf.AddFormatHexInt(c, 2);
				sf.AddChar(' ');
			}
		}
		sf.AddLiteral(_T(" : "));
		if ( ! isUnicode )
		{
			//== S-JIS
			loop ( i, width )
			{
				if ( i + j >= size )
				{
					break;
				}
				TCHAR c = B[i + j];
				if ( isLastSecondByte )
				{
					isLastSecondByte = false;
					sf.AddChar('.');
				}
				else if ( c >= 0 && c < 0x20 )
				{
					sf.AddChar('.');
				}
				else if ( i == (width - 1) && STRLIB::GetCharSize(c) == 2 )
				{
					if ( (i + j + 1) < size )
					{
						sf.AddChar(c);
						sf.AddChar(B[i + j + 1]);
						isLastSecondByte = true;
					}
					else
					{
						sf.AddChar('.');
					}
				}
				else
				{
					sf.AddChar(c);
				}
			}
			sf.AddTerminateChar();
			sa.Add(CStr(sf));
		}
		else
		{
			//== UNICODE
			CUnicode uni;
			size_t i = width;
			if ( i + j >= size )
			{
				i = size - j;
			}
			uni.SetFromLeft(reinterpret_cast<LPCWSTR>(&B[j]), i / 2);
			sf.AddTerminateChar();
			sa.Add(CStr(CUnicode(sf) + uni));
		}
		//
		if ( ! canAllDump )
		{
			if ( j >= width * 4 )
			{
				isLastSecondByte = false;
				canAllDump = true;
				size_t k = (size - width * 4) & 0xFFFFFFF0;
				if ( j < k )
				{
					sa.Add(_T(" :                     :"));
					j = k;
				}
			}
		}
	}
	return sa;
}



/**@ingroup ETC
 * [�쐬] �o�C�i���_���v.
 *		�w��f�[�^��16�i�\�L������ɕϊ��o�͂��܂��B
 *	@note	�f�o�O�r���h���̂ݏ������܂��B
 *	@par�K�v�t�@�C��
 *			TnbDump.h
 *	@param size Dump���钷��
 *	@param pData	Dump����f�[�^
 *	@param canAllDump true �Ȃ�w��T�C�YDump����B�ȗ��� false �Ȃ�r�����Ȃ��B
 *	@param width ���B�ȗ�����ƂP�U�D
 *	@param isUnicode �ȗ��� false �Ȃ� S-JIS�A true �Ȃ� UNICODE �Ƃ��ĕ\�����܂��B
 */
inline void DebugDump(size_t size, const void* pData, bool canAllDump = false, size_t width = 16, bool isUnicode = false)
{
	bool r = true;
	#if defined(_DEBUG) && ! defined(_WIN32_WCE)
	#if ( _WIN32_WINNT >= 0x0400 ) || ( _WIN32_WINDOWS > 0x0400 )
		#define _DEBUGDUMP_ENABLE
		r = !! ::IsDebuggerPresent();
	#endif
	#endif
	#if defined(_DEBUG) && defined(_WIN32_WCE)
		#define _DEBUGDUMP_ENABLE
	#endif
	//
	#ifdef _DEBUGDUMP_ENABLE
	if ( r )
	{	
		::OutputDebugString(CStr::Fmt(_T(">Size = %u\n"), size));
		CStrVector sa = DumpData(size, pData, canAllDump, width, isUnicode);
		loop ( i, sa.GetSize() )
		{
			::OutputDebugString(_T("      ") + sa[i] + _T("\n"));
		}
	}
	#endif
	IgnoreUnusedValue(r);
}



}; // TNB

