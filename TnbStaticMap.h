#pragma once
/**
 *@file
 * �ÓI�ȈՃ}�b�v�Ǘ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"



//TNB Library
namespace TNB
{



/**@ingroup COLLECT
 * �ÓI�ȈՃ}�b�v�N���X.
 *
 *		�ÓI�ȃ}�b�v���쐬���A�L�[�Œl���������邽�߂̃t���[���ł��B
 *		(�N���X�I�Ȏg�����ɂ͂��܂���j
 *
 *	@par�g�p��
 *		<table><tr><td><pre>
 *		
 *		const CStaticMapT<int,LPCTSTR> tMap[] = 
 *		{
 *			0,		_T(""),
 *			1,		_T("def��������"),
 *			2,		_T("dim��������"),
 *			3,		_T("func��������"),
 *		};
 *		CString s = CStaticMapT<int,LPCTSTR>::Find(2, tMap);			
 *		TRACE1("[%s]\n", s);
 *		</pre></td></tr></table>
 *
 *	@param TYP �L�[�̌^�B�^�͔�r�@�\���K�{�ł��B
 *	@param VAL �l�̌^�B�^�̓R�s�[�@�\���K�{�ł��B�ȗ�����ƁA LPCTSTR �ɂȂ�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbStaticMap.h
 *
 *	@date 07/06/29	�V�K
 *	@date 08/09/01	�f�t�H���g�̒l���e���v���[�g�ł͂Ȃ��A���\�b�h�Ŏw�肷��悤�ɕύX�B
 */
template<typename TYP, typename VAL = LPCTSTR>
class CStaticMapT
{
public:

	TYP			key;	///< �L�[
	VAL			val;	///< �l

	/**
	 * [�擾] �擾
	 *	@param[out] _key �擾�����L�[
	 *	@param[out] _val �擾�����l
	 *	@param[in] index �C���f�b�N�X
	 *	@param ts �����Ώۂ̃}�b�v�e�[�u���B�{�N���X�̔z��ł���K�v������܂��B
	 *	@retval true �擾�����B
	 *	@retval false ���s�B
	 */
	template<typename TYPS>
	static bool Get(TYP& _key, VAL& _val, INDEX index, const TYPS& ts)
	{
		const CStaticMapT* P = ts;		//������ ts ���w�肵�Ă��Ȃ��Ƃ����ŃG���[�B
		IgnoreUnusedValue(P);
		if ( index < sizeof(ts) / sizeof(CStaticMapT) )
		{
			_key = ts[index].key;
			_val = ts[index].val;
			return true;
		}
		return false;
	}
	
	/**
	 * [����] �l����
	 *	@param key ��������L�[
	 *	@param ts �����Ώۂ̃}�b�v�e�[�u���B�{�N���X�̔z��ł���K�v������܂��B
	 *	@param def �Ȃ������Ƃ��̒l�ɂȂ�܂��B�ȗ������ VAL() �ɂȂ�܂��B
	 *	@retval def�̒l �����炸�B
	 *	@retval ��L�ȊO key �ɑ΂���l�B
	 */
	template<typename TYPS>
	static VAL Find(const TYP& key, const TYPS& ts, const VAL& def = VAL())
	{
		const CStaticMapT* P = ts;		//������ ts ���w�肵�Ă��Ȃ��Ƃ����ŃG���[�B
		IgnoreUnusedValue(P);
		loop ( i, sizeof(ts) / sizeof(CStaticMapT) )
		{
			if ( ts[i].key == key )
			{
				return ts[i].val;
			}
		}
		return def;
	}
	
	/**
	 * [����] �l����
	 *	@param key ��������L�[
	 *	@param ts �����Ώۂ̃}�b�v�e�[�u���B�{�N���X�̔z��ł���K�v������܂��B
	 *	@param def �Ȃ������Ƃ��̒l�ɂȂ�܂��B�ȗ������ VAL() �ɂȂ�܂��B
	 *	@retval def�̒l �����炸�B
	 *	@retval ��L�ȊO key �ɑ΂���l�B
	 */
	template<typename TYPS>
	static VAL FindString(const TYP& key, const TYPS& ts, const VAL& def = VAL())
	{
		const CStaticMapT* P = ts;		//������ ts ���w�肵�Ă��Ȃ��Ƃ����ŃG���[�B
		loop ( i, sizeof(ts) / sizeof(CStaticMapT) )
		{
			if ( STRLIB::Compare(ts[i].key, key) == 0 )
			{
				return ts[i].val;
			}
		}
		return def;
	}

	/**
	 * [����] �L�[����
	 *	@param val ��������l
	 *	@param ts �����Ώۂ̃}�b�v�e�[�u���B�{�N���X�̔z��ł���K�v������܂��B
	 *	@param def �Ȃ������Ƃ��̒l�ɂȂ�܂��B�ȗ������ TYP() �ɂȂ�܂��B
	 *	@retval def�̒l �����炸�B
	 *	@retval ��L�ȊO val �ɑ΂���L�[�B
	 */
	template<typename TYPS>
	static TYP FindKey(const VAL& val, const TYPS& ts, const TYP& def = TYP())
	{
		const CStaticMapT* P = ts;		//������ ts ���w�肵�Ă��Ȃ��Ƃ����ŃG���[�B
		IgnoreUnusedValue(P);
		loop ( i, sizeof(ts) / sizeof(CStaticMapT) )
		{
			if ( ts[i].val == val )
			{
				return ts[i].key;
			}
		}
		return def;
	}
};



}; // TNB
