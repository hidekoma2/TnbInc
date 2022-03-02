#pragma once
/**
 *@file
 * DEVICE �̌����֌W�̃w�b�_
 *
 *	@note �{�@�\���g�p����ɂ́AWindows SDK ���K�v�ɂȂ�܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"
#include <SetupApi.h>
#pragma comment(lib, "setupapi.lib")



//TNB Library
namespace TNB
{



/**@ingroup USB
 * DEVICE �����N���X
 *
 *		�ڑ�����Ă��� DEVICE �̌������s���A���ۃN���X�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbUsbGuidFinder.h
 * 
 *	@date 08/07/07 CUsbHidFinder �𒊏ۉ����ĐV�K�쐬
 */
class CAbstractUsbGuidFinder : CCopyImpossible
{
	GUID			m_guid;				///< �����Ώ� GUID
	HDEVINFO		m_hDevInfo;			///< �f�o�C�X�C���t�H�n���h��
	DWORD			m_dwIndex;			///< ����Index
	DWORD			m_dwLastError;		///< �G���[�R�[�h (��������)

	/**
	 * �w��INDEX�̃f�o�C�X��Capability���擾���N���X���\�b�h�Ɋi�[
	 *	@param index �C���f�b�N�X�B
	 *	@retval true �����B
	 *	@retval false ���s�B���R�� m_dwLastError �Ɋi�[����܂��B ERROR_NO_MORE_ITEMS(0x103) �͂���ȏ�f�[�^�Ȃ��A�Ƃ����Ӗ��B
	 */
	bool m_GetDevInfo(DWORD index)
	{
		ASSERTLIB( m_hDevInfo != NULL );
		SP_DEVICE_INTERFACE_DATA tDevIfData;
		tDevIfData.cbSize = sizeof(tDevIfData);	// �T�C�Y��ݒ�
		// �f�o�C�X���Z�b�g���̊e�f�o�C�X �C���^�[�t�F�[�X���
		if ( ! ::SetupDiEnumDeviceInterfaces(
					m_hDevInfo,		// �C���^�[�t�F�C�X�����擾����f�o�C�X���܂܂�Ă���f�o�C�X���Z�b�g�ւ̃|�C���^
					NULL,			// ���g�p
					&m_guid,		// �v������C���^�[�t�F�C�X�̃f�o�C�X�C���^�[�t�F�C�X�N���X���w�肷��A1��GUID�ւ̃|�C���^
					index,			// �f�o�C�X���Z�b�g���̃C���^�[�t�F�C�X���X�g�ɑ΂��āA0�Ŏn�܂�C���f�b�N�X�ԍ�
					&tDevIfData		// �֐������������ꍇ�ɁA�������݂̊�������SP_DEVICE_INTERFACE_DATA�\���̂�ێ�����o�b�t�@�ւ̃|�C���^
			)
		)
		{
			m_dwLastError = ::GetLastError();
		}
		else
		{
			// �w�肳�ꂽ�f�o�C�X�C���^�[�t�F�C�X�Ɋւ���ڍ׏���Ԃ��܂�
			// �ڍ׃f�[�^�\���̃T�C�Y���擾
			DWORD dwReqSize;
			SP_DEVINFO_DATA tDevInfoData;
			tDevInfoData.cbSize = sizeof(tDevInfoData);
			::SetupDiGetDeviceInterfaceDetail(
					m_hDevInfo,		// �C���^�[�t�F�C�X�Ƃ��̊�ɂȂ�f�o�C�X���܂܂��f�o�C�X���Z�b�g�ւ̃|�C���^
					&tDevIfData,	// �C���^�[�t�F�C�X�����ʂ���1��SP_DEVICE_INTERFACE_DATA�\���̂ւ̃|�C���^
					NULL,			// �T�C�Y�擾���邽�߁ANULL	�i�w�肳�ꂽ�C���^�[�t�F�C�X�Ɋւ�������󂯎��1��SP_DEVICE_INTERFACE_DETAIL_DATA�\���̂ւ̃|�C���^�j
					0,				// �T�C�Y�擾���邽�߁A0	�iDeviceInterfaceDetailData���w���o�b�t�@�̃T�C�Y�j
					&dwReqSize,		// DeviceInterfaceDetailData���w���o�b�t�@���K�v�Ƃ���T�C�Y���󂯎��A1�̕ϐ��ւ̃|�C���^
					&tDevInfoData	// �v�����ꂽ�C���^�[�t�F�C�X�����J���Ă���f�o�C�X�Ɋւ�������󂯎��A1��SP_DEVINFO_DATA�\���̂ւ̃|�C���^
				);
			CWorkMem work(dwReqSize);
			PSP_DEVICE_INTERFACE_DETAIL_DATA pDevIfDetailData
						= reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(work.Ref());
			pDevIfDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			if ( ! ::SetupDiGetDeviceInterfaceDetail(
					m_hDevInfo,			// �C���^�[�t�F�C�X�Ƃ��̊�ɂȂ�f�o�C�X���܂܂��f�o�C�X���Z�b�g�ւ̃|�C���^
					&tDevIfData,		// �C���^�[�t�F�C�X�����ʂ���1��SP_DEVICE_INTERFACE_DATA�\���̂ւ̃|�C���^
					pDevIfDetailData,	// �w�肳�ꂽ�C���^�[�t�F�C�X�Ɋւ�������󂯎��1��SP_DEVICE_INTERFACE_DETAIL_DATA�\���̂ւ̃|�C���^
					dwReqSize,			// DeviceInterfaceDetailData���w���o�b�t�@�̃T�C�Y
					&dwReqSize,			// DeviceInterfaceDetailData���w���o�b�t�@���K�v�Ƃ���T�C�Y���󂯎��A1�̕ϐ��ւ̃|�C���^
					&tDevInfoData		// �v�����ꂽ�C���^�[�t�F�C�X�����J���Ă���f�o�C�X�Ɋւ�������󂯎��A1��SP_DEVINFO_DATA�\���̂ւ̃|�C���^
				)
			)
			{
				m_dwLastError = ::GetLastError();
			}
			else
			{
				m_dwLastError = OnFound(pDevIfDetailData->DevicePath);
			}
		}
		return m_dwLastError == ERROR_SUCCESS;
	}
	
	/**
	 * �����o�̃n���h���� GUID ���g������
	 *	@retval true �����i�N���X���\�b�h�ɏ�񂠂�j
	 *	@retval false �������^�G���[�����i GetLastError() �ŏ��擾�\�j
	 */
	bool m_Find(void)
	{
		Close();
		// �w�肳�ꂽ�N���X�ɏ������邷�ׂẴf�o�C�X���܂܂�Ă���1�̃f�o�C�X���Z�b�g��Ԃ��܂�
		m_hDevInfo = ::SetupDiGetClassDevs(
				&m_guid,								// �Z�b�g�A�b�v�N���X�̃N���XGUID�ւ̃|�C���^
				NULL,									// �V�X�e���̂��ׂẴf�o�C�X�C���X�^���X�Ɋւ���f�o�C�X�����擾
				0,										// ���̃Z�b�g�̃����o�Ɋ֘A���邷�ׂẴ��[�U�[�C���^�[�t�F�C�X�����p����A�g�b�v���x���E�B���h�E�̃n���h�����w��
				(DIGCF_PRESENT | DIGCF_DEVICEINTERFACE)	// ���ݑ��݂���f�o�C�X | ClassGuid�Ŏw�肳�ꂽ�C���^�[�t�F�C�X�N���X�ɏ�������C���^�[�t�F�C�X�����J���Ă���f�o�C�X
		);
		m_dwIndex = 0;
		m_dwLastError = 0;
		if ( m_hDevInfo != NULL )
		{
			m_GetDevInfo(m_dwIndex);
			if ( m_dwLastError != ERROR_NO_MORE_ITEMS )
			{
				m_dwIndex++;
				return true;
			}
			Close();
		}
		else
		{
			m_dwLastError = ::GetLastError();
		}
		return false;
	}
protected:

	/**
	 * [�ʒm] ����.
	 *	@note �����ɍ����f�o�C�X�������������ɒʒm����܂��B
	 *	@param lpszDevicePath �f�o�C�X�p�X���B CreateFile() ���Ŏg�p���邱�Ƃ��o���܂��B
	 *	@return �G���[�R�[�h�B
	 */
	virtual DWORD OnFound(LPCTSTR lpszDevicePath) = 0;

	/**
	 * [�ʒm] �N���[�Y.
	 *	@note �������N���[�Y�������ɒʒm����܂��B
	 */
	virtual void OnClosed(void) = 0;

public:

	/// �R���X�g���N�^
	CAbstractUsbGuidFinder(void) : m_hDevInfo(NULL), m_dwIndex(0), m_dwLastError(0)
	{
		Zero(m_guid);
	}

	/**
	 * [�ݒ�] �N���[�Y.
	 *	@note �������̏ꍇ�A�������~���܂��i Next() ���\�b�h�� false ��Ԃ��悤�ɂȂ�܂��j�B
	 */
	void Close(void)
	{
		if ( m_hDevInfo != NULL )
		{
			::SetupDiDestroyDeviceInfoList(m_hDevInfo);
			m_hDevInfo = NULL;
			OnClosed();
		}
	}

	/**
	 * [�m�F] �������m�F
	 *	@retval true �������B���������Ă���B
	 *	@retval false ���Ă��Ȃ��B
	 */
	bool IsFinding(void) const
	{
		return m_hDevInfo != NULL;
	}
	
	/**
	 * [����] �����J�n.
	 *	@param g �����Ώۂ� GUID�B
	 *	@retval true �����i�N���X���\�b�h�ɏ�񂠂�j
	 *	@retval false �������^�G���[�����i GetLastError() �ŏ��擾�\�j
	 */
	bool Find(const GUID& g)
	{
		m_guid = g;
		return m_Find();
	}

	/**
	 * [����] ������
	 *	@note Find() �� true �̏ꍇ�A���ɂ��̃��\�b�h���g�p���܂��B���̌�J��Ԃ���
	 *			�{���\�b�h�g�����Ƃŕ����̏��𓾂邱�Ƃ��o���܂��B
	 *	@retval true �����i�N���X���\�b�h�ɏ�񂠂�j
	 *	@retval false �������^�G���[�����i GetLastError() �ŏ��擾�\�j
	 */
	bool Next(void)
	{
		m_GetDevInfo(m_dwIndex);
		if ( m_dwLastError == ERROR_NO_MORE_ITEMS )
		{
			Close();
			return false;
		}
		m_dwIndex++;
		return true;
	}

	/**
	 * [�擾] �G���[���擾
	 *	@note Find() Next() �� false �̏ꍇ�A�{���\�b�h�ŃG���[������I�������f�ł��܂��B
	 *	@retval ERROR_SUCCESS		�G���[���Ȃ��B
	 *	@retval ERROR_SUCCESS�ȊO �G���[�B���l�̓V�X�e���G���[�R�[�h�ł�( SystemErrorToMessageText() �ŕ����񉻂ł��܂�)�B
	 */
	DWORD GetLastError(void) const
	{
		return m_dwLastError;
	}
};



/**@ingroup USB
 * DEVICE�����N���X
 *
 *		�ڑ�����Ă��� DEVICE �̌������s���N���X�ł��B
 *
 *	@par�K�v�t�@�C��
 *			TnbUsbGuidFinder.h
 * 
 *	@date 08/07/07 CUsbHidFinder �𒊏ۉ����ĐV�K�쐬
 */
class CUsbGuidFinder : public CAbstractUsbGuidFinder
{
	DEFSUPER(CAbstractUsbGuidFinder);
	CSimpleStr	m_devicePathName;	///< �����f�o�C�X��
protected:
	/**
	 * [�ʒm] ����.
	 *	@note �����ɍ����f�o�C�X�������������ɒʒm����܂��B
	 *	@param lpszDevicePath �f�o�C�X�p�X���B CreateFile() ���Ŏg�p���邱�Ƃ��o���܂��B
	 *	@return �G���[�R�[�h�B
	 */
	virtual DWORD OnFound(LPCTSTR lpszDevicePath)
	{
		m_devicePathName = lpszDevicePath;
		return 0;
	}

	/**
	 * [�ʒm] �N���[�Y.
	 *	@note �������N���[�Y�������ɒʒm����܂��B
	 */
	virtual void OnClosed(void)
	{
		m_devicePathName.Empty();
	}

public:
	/// �R���X�g���N�^
	CUsbGuidFinder(void)
	{
	}

	/**
	 * [�擾] �����f�o�C�X�p�X���擾.
	 *	@return �f�o�C�X�p�X��
	 */
	LPCTSTR GetDevicePathName(void) const
	{
		return m_devicePathName;
	}
};



}; // TNB
