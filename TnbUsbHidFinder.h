#pragma once
/**
 *@file
 * USB HID�̌����֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbUsbGuidFinder.h"
#include "TnbUsbHidPathName.h"
#include "TnbVector.h"



//TNB Library
namespace TNB
{



/**@ingroup USB
 * USB-HID�����N���X
 *
 *		�ڑ�����Ă��� USB-HID �̌����A����сA�ʐM�T�C�Y���擾���܂��B
 *		�܂��A�w��VID,PID �̌������\�B
 *
 *	@par�K�v�t�@�C��
 *			TnbUsbHidFinder.h
 * 
 *	@date 07/04/24 �V�K�쐬
 *	@date 08/07/07 ���ۉ������N���X�ƕ���
 */
class CUsbHidFinder : public CAbstractUsbGuidFinder
{
	DEFSUPER(CAbstractUsbGuidFinder);
	CUsbHidPathName	m_hidName;			///< HID��� (��������)
protected:
	/**
	 * [�ʒm] ����.
	 *	@note �����ɍ����f�o�C�X�������������ɒʒm����܂��B
	 *	@param lpszDevicePath �f�o�C�X�p�X���B CreateFile() ���Ŏg�p���邱�Ƃ��o���܂��B
	 *	@return �G���[�R�[�h�B
	 */
	virtual DWORD OnFound(LPCTSTR lpszDevicePath)
	{
		return m_hidName.SetPathName(lpszDevicePath);
	}
	/**
	 * [�ʒm] �N���[�Y.
	 *	@note �������N���[�Y�������ɒʒm����܂��B
	 */
	virtual void OnClosed(void)
	{
		m_hidName.Empty();
	}

public:

	/// �R���X�g���N�^
	CUsbHidFinder(void) : m_hidName()
	{
	}

	/**
	 * [����] �����J�n
	 *	@retval true �����i�N���X���\�b�h�ɏ�񂠂�j
	 *	@retval false �������^�G���[�����i GetLastError() �ŏ��擾�\�j
	 */
	bool Find(void)
	{
		GUID g;
		CUsbHidApi().HidD_GetHidGuid(&g);
		return _super::Find(g);
	}

	/**
	 * [����] �����J�n�iMouse�w��j
	 *	@retval true �����i�N���X���\�b�h�ɏ�񂠂�j
	 *	@retval false �������^�G���[�����i GetLastError() �ŏ��擾�\�j
	 */
	bool FindMouseHid(void)
	{
		GUID g = { 0x378de44c, 0x56ef, 0x11d1, 0xbc, 0x8c, 0x00, 0xa0, 0xc9, 0x14, 0x05, 0xdd };
		return _super::Find(g);
	}

	/**
	 * [�擾] �������ʏ��擾
	 *	@note Find() Next() �� true �̏ꍇ�A��񂪓����܂��B
	 *	@return �p�X�l�[�����̎Q�ƁB
	 *	@throw CEmptyException �������A���邢�͖��������ɖ{���\�b�h���R�[������ƁA�X���[����܂��B
	 */
	const CUsbHidPathName& Get(void) const
	{
		if ( ! _super::IsFinding() )
		{
			throw CEmptyException();
		}
		return m_hidName;
	}

	/**
	 * [�擾] USB-HID �p�X�l�[�����ꗗ.
	 *		�ڑ�����Ă��� USB-HID �p�X�l�[�����ꗗ���쐬���܂��B
	 *	@return ���������p�X�l�[�����̔z��
	 */
	static CVectorT<CUsbHidPathName> EnumPathNames(void)
	{
		CVectorT<CUsbHidPathName> v;
		CUsbHidFinder fh;
		if ( fh.Find() )
		{
			do
			{
				v.Add(fh.Get());
			} while ( fh.Next() );
		}
		return v;
	}

	/**
	 * [�擾] �w�� USB-HID �p�X�l�[�����ꗗ.
	 *		�w��� ID �̃p�X�l�[�������W�߂܂��B
	 *	@param iVendorId �x���_ID.
	 *	@param iProductId �v���_�N�gID.
	 *	@return ���������p�X�l�[�����̔z��B
	 */
	static CVectorT<CUsbHidPathName> FindPathNames(int iVendorId, int iProductId)
	{
		CVectorT<CUsbHidPathName> v;
		CUsbHidFinder fh;
		if ( fh.Find() )
		{
			do
			{
				if ( fh.Get().GetVendorId() == iVendorId && fh.Get().GetProductId() == iProductId )
				{
					v.Add(fh.Get());
				}
			} while ( fh.Next() );
		}
		return v;
	}
};



}; // TNB
