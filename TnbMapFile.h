#pragma once
/**
 *@file
 * �}�b�v�t�@�C���֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMap.h"
#include "TnbStrVector.h"
#include "TnbTextFile.h"



//TNB Library
namespace TNB
{



/**@ingroup FILE
 * �}�b�v�t�@�C���N���X.
 *
 *		CMapT �̏��� csv�t�@�C���ŕۑ��A�Ǎ��݂��邽�߂̃N���X�ł��B
 *		�p���� KeyToString(), ValueToString(), StringToKey(), StringToValue() ���I�[�o�[���C�h����K�v������܂��B
 *
 *	@param	KEY	�L�[�Ƃ���^�B�N���X�̏ꍇ�R�s�[�@�\�A��r�@�\( IComparableT ���A�����Z)���K�{�B
 *	@param	VAL	�l�Ƃ���^�B�N���X�̏ꍇ�R�s�[�@�\���K�{�B
 *	@param	INK	�L�[�Ƃ���^�̓��͌^�B�ȗ��\�B
 *
 *	@par�K�v�t�@�C��
 *			TnbMapFile.h
 *
 *	@date 15/03/10	�b��V�K
 */
template<typename KEY, typename VAL, typename INK=KEY>
class CMapFileT
{
public:
	/**
	 * [�ۑ�] �t�@�C���ۑ�.
	 *		�w��� map ���t�@�C���ɕۑ����܂��B
	 *	@param lpszFileName �t�@�C����
	 *	@param map �}�b�v
	 *	@param comma �J���}
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Save(LPCTSTR lpszFileName, const CMapT<KEY, VAL, INK>& map, TCHAR comma = ',')
	{
		CStr s;
		CStrVector vs;
		loop ( i, map )
		{
			s = KeyToString(map.At(i).first);
			s += period;
			s += ValueToString(map.At(i).second);
			vs.Add(s);
		}
		return CTextFileWriter::NewFile(lpszFileName, vs);
	}

	/**
	 * [�Ǎ�] �t�@�C���Ǎ���.
	 *		�w��̃t�@�C���� map �ɓǍ��݂܂��B
	 *	@param[out] _map �}�b�v
	 *	@param[in] lpszFileName �t�@�C����
	 *	@param[in] comma �J���}
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Load(CMapT<KEY, VAL, INK>& _map, LPCTSTR lpszFileName, TCHAR comma = ',')
	{
		_map.RemoveAll();
		CTextFileReader r;
		if ( r.Read(lpszFileName) )
		{
			CConstStrVector vs;
			if ( r.GetContents(vs) )
			{
				CStrVector v;
				loop ( i, vs.GetLineCount() )
				{
					v = CStrOperator::SeparatePeriod(vs.GetString(i), period);
					if ( v.GetSize() >= 2 )
					{
						KEY key = StringToKey(v[0]);
						VAL val = StringToValue(v[1]);
						_map[key] = val;
					}
				}
				return true;
			}
		}
		return false;
	}

protected:
	/**
	 * [�ϊ�] �L�[�𕶎���.
	 *	@param key �L�[
	 *	@return ������
	 */
	virtual CStr KeyToString(const KEY& key) = 0;

	/**
	 * [�ϊ�] �l�𕶎���.
	 *	@param val �l
	 *	@return ������
	 */
	virtual CStr ValueToString(const VAL& val) = 0;

	/**
	 * [�ϊ�] ��������L�[��.
	 *	@param lpsz ������
	 *	@return �L�[
	 */
	virtual KEY StringToKey(LPCTSTR lpsz) = 0;

	/**
	 * [�ϊ�] �������l��.
	 *	@param lpsz ������
	 *	@return �l
	 */
	virtual VAL StringToValue(LPCTSTR lpsz) = 0;
};



};
