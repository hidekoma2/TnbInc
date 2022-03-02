#pragma once
/**
 *@file
 * BIT���̃t���O�Ǘ��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbStr.h"
#include "TnbStaticMap.h"



//TNB Library
namespace TNB 
{



/**@ingroup VALUE
 * BIT�t���O�Ǘ����ۃN���X
 *
 *		BIT���g�����t���O�����Ǘ����邽�߂̒��ۃN���X�ł��B
 *		BIT�̃`�F�b�N���ȈՉ����܂��B
 *		Table() ���������邱�ƂŁA�����񉻂��T�|�[�g���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBitFlag.h
 * 
 *	@date 10/04/15	�V�K�쐬
 */
class CAbstructBitFlag
{
public:

	/**
	 * �R���X�g���N�^.
	 *	@param flag �����l�B�ȗ�����ƂO���w�肳��܂��B
	 */
	CAbstructBitFlag(DWORD flag = 0) : m_flag(flag)
	{
	}

	/**
	 * [�擾] �t���O�擾.
	 *	@return �t���O
	 */
	operator DWORD(void) const
	{
		return m_flag;
	}

	/**
	 * [�m�F] �m�F.
	 *	@note BIT�������o���Ă���ꍇ�A���ׂė����Ă��Ȃ��ƗL���ɂȂ�܂���B
	 *	@param fg �t���O
	 *	@retval true �L��
	 *	@retval false ����
	 */
	bool IsValid(DWORD fg) const
	{
		return ((m_flag & fg) == fg);
	}

	/**
	 * [�m�F] �m�F.
	 *	@param bn �r�b�gNo
	 *	@retval true �L��
	 *	@retval false ����
	 */
	bool IsValidByBitNo(DWORD bn) const
	{
		return (m_flag & _BIT(bn)) != 0;
	}

	/**
	 * [�ݒ�] �L��.
	 *	@param fg �t���O
	 */
	void Valid(DWORD fg)
	{
		m_flag |= fg;
	}

	/**
	 * [�ݒ�] ����.
	 *	@param fg �t���O
	 */
	void Invalid(DWORD fg)
	{
		m_flag &= ~fg;
	}

	/**
	 * [�擾] ������.
	 *		�{�C���X�^���X���ێ����Ă���t���O�� Table() ���g���ĕ����񉻂��܂��B
	 *	@return ������
	 */
	CStr ToString(void) const
	{
		CStr s;
		DWORD flag = m_flag;
		DWORD bf;
		CStr name;
		INDEX index = 0;
		while ( Table(bf, name, index) )
		{
			if ( (flag & bf) == bf )
			{
				s += _T("(") + name + _T(")");
				flag &= ~bf;
			}
			index++;
		}
		return s;
	}

protected:

	/**
	 * [�擾] �e�[�u���擾.
	 *	@param[out] _bf �t���O�l���擾����܂��B
	 *	@param[out] _name �t���O�����擾����܂��B
	 *	@param[in] index �C���f�b�N�X
	 *	@retval true �擾����
	 *	@retval false �C���f�b�N�X����
	 */
	virtual bool Table(DWORD& _bf, CStr& _name, INDEX index) const = 0;

private:
	DWORD	m_flag;	///< �t���O�l

};



/**@ingroup VALUE
 * Window�X�^�C���Ǘ��N���X
 *
 *	@par�K�v�t�@�C��
 *			TnbBitFlag.h
 * 
 *	@date 10/04/15	�V�K�쐬
 */
class CWindowStyleBitFlag : public CAbstructBitFlag
{
	DEFSUPER(CAbstructBitFlag);
public:

	/**
	 * �R���X�g���N�^.
	 *	@param flag �����l�B�ȗ�����ƂO���w�肳��܂��B
	 */
	CWindowStyleBitFlag(DWORD flag = 0) : _super(flag)
	{
	}

protected:

	/**
	 * [�擾] �e�[�u���擾.
	 *	@param[out] _bf �t���O�l���擾����܂��B
	 *	@param[out] _name �t���O�����擾����܂��B
	 *	@param[in] index �C���f�b�N�X
	 *	@retval true �擾����
	 *	@retval false �C���f�b�N�X����
	 */
	virtual bool Table(DWORD& _bf, CStr& _name, INDEX index) const
	{
		const static CStaticMapT<DWORD ,LPCTSTR> maps[] = 
		{
			#ifndef _TnbDOXYGEN
			#define _X(XX) {XX, _T(#XX)}
			_X(WS_POPUP),
			_X(WS_CHILD),
			_X(WS_MINIMIZE),
			_X(WS_VISIBLE),
			_X(WS_DISABLED),
			_X(WS_CLIPSIBLINGS),
			_X(WS_CLIPCHILDREN),
			_X(WS_MAXIMIZE),
			_X(WS_CAPTION),
			_X(WS_BORDER),
			_X(WS_DLGFRAME),
			_X(WS_VSCROLL),
			_X(WS_HSCROLL),
			_X(WS_SYSMENU),
			_X(WS_THICKFRAME),
			_X(WS_MINIMIZEBOX),
			_X(WS_MAXIMIZEBOX),
			#undef _X
			#endif //_TnbDOXYGEN
		};
		LPCTSTR lpsz = NULL;
		if ( CStaticMapT<DWORD, LPCTSTR>::Get(_bf, lpsz, index, maps) )
		{
			_name = lpsz;
			return true;
		}
		return false;
	}
};



}; //TNB
