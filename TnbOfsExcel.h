#pragma once
/**
 *@file
 * Office EXCEL�����֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbDef.h"
#include "TnbFilePathManager.h"
#include "TnbStr.h"

#include <comutil.h>
#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
//Excel�𑀍삷�邽�߂̃^�C�v���C�u������ǂ݂���(Excel2007�p)
//#import "C:\Program Files (x86)\Common Files\Microsoft Shared\OFFICE14\MSO.DLL"
//#import "C:\Program Files (x86)\Common Files\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB"
//Excel�𑀍삷�邽�߂̃^�C�v���C�u������ǂ݂���(Excel2019�p)
#import "C:\Program Files (x86)\Microsoft Office\Root\VFS\ProgramFilesCommonX86\Microsoft Shared\OFFICE16\MSO.DLL"
#import "C:\Program Files (x86)\Microsoft Office\Root\VFS\ProgramFilesCommonX86\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB"
#import "C:\Program Files (x86)\Microsoft Office\root\Office16\excel.exe" rename("DialogBox", "DialogBoxForExcel") rename( "RGB", "RGBForExcel") 
#endif // _TnbDOXYGEN



//TNB Library
namespace TNB
{



/**@ingroup WINAPI
 * Microsoft Office Excel �R���g���[���⏕
 *	@note �v���W�F�N�g�� ::CoInitialize(NULL); �����s���Ă����Ă��������B
 */
class CExcelControl
{
public:

	/// �f�X�g���N�^
	~CExcelControl(void)
	{
		Close(false);
	}

	/**
	 * [�ݒ�] �I�[�v��.
	 *	@param lpszXlsxFile xls�t�@�C�����B���΃p�X�̏ꍇEXE����̑��΃p�X�ɂȂ�܂��B
	 *	@retval true ����
	 *	@retval false ���s�B
	 */
	bool Open(LPCTSTR lpszXlsxFile)
	{
		CFilePathManager fpm;
		HRESULT r = m_pApp.CreateInstance( "Excel.Application");
		if ( m_pApp == NULL )
		{
			return false;
		}
		try
		{
			CUnicode fn = fpm.GetCanonicalize(lpszXlsxFile);
			m_pWorkbook = m_pApp->Workbooks->Open(fn.ReferBuffer());
		}
		catch( ... )
		{
			m_pApp->Quit();				//��\���̏ꍇ�A���̃^�C�~���O�ł̓C���X�^���X�͏����Ȃ��B
			// �C���X�^���X���J��
			m_pApp.Release();				//��\���̏ꍇ�A������excel.exe�C���X�^���X���Ȃ��Ȃ�B
			return false;
		}
		return true;
	}

	/**
	 * [�ݒ�] �N���[�Y.
	 *	@param isSave true �Ȃ�ۑ����ďI�����܂��B
	 *	@retval true ����
	 *	@retval false ���s�B
	 */
	bool Close(bool isSave = true)
	{
		if ( m_pApp != NULL )
		{
			try
			{
				if ( isSave )
				{
					m_pWorkbook->Save();
				}
				m_pWorkbook->Close();
				m_pRange.Release();
				m_pWorkbook.Release();
				m_pSheet.Release();
				m_pApp->Quit();				//��\���̏ꍇ�A���̃^�C�~���O�ł̓C���X�^���X�͏����Ȃ��B
				// �C���X�^���X���J��
				m_pApp.Release();				//��\���̏ꍇ�A������excel.exe�C���X�^���X���Ȃ��Ȃ�B
			}
			catch( ... )
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * [�ݒ�] �J�����g���[�N�V�[�g�I��.
	 *	@param lpszSheetName �V�[�g��
	 *	@retval true ����
	 *	@retval false ���s�B
	 */
	bool SelectWorksheet(LPCTSTR lpszSheetName)
	{
		try
		{
			m_pSheet = m_pWorkbook->Sheets->Item[CUnicode(lpszSheetName).ReferBuffer()];
			return m_pSheet != NULL;
		}
		catch( ... )
		{
			//m_pSheet.Release();
			return false;
		}
	}

	/**
	 * [�ݒ�] �J�����g�Z�����I��.
	 *	@param lpszCellName �Z����
	 *	@retval true ����
	 *	@retval false ���s�B
	 */
	bool SelectCell(LPCTSTR lpszCellName)
	{
		try
		{
			COleVariant varCell;
			varCell = CUnicode(lpszCellName).ReferBuffer();
			m_pRange = m_pSheet->GetRange( varCell);
		}
		catch( ... )
		{
			m_pRange.Release();
			return false;
		}
		return m_pRange != NULL;
	}

	/**
	 * [�擾] �����W�擾.
	 *	@return �����W
	 */
	Excel::RangePtr GetRange(void)
	{
		return m_pRange;
	}

	/**
	 * [�擾] �{�[�_�[�擾.
	 *	@return �{�[�_�[
	 */
	Excel::BorderPtr GetBorder(Excel::XlBordersIndex index)
	{
		return m_pRange->GetBorders()->GetItem(index);
	}

	/**
	 * [�쐬] �J������������.
	 *	@param idx �J�����ԍ�
	 *	@return �J��������
	 */
	static CStr ColunmChar(INDEX idx)
	{
		TCHAR buf[3] = {0};
		INDEX i = idx % 26;
		INDEX j = idx / 26;
		if ( j == 0 )
		{
			buf[0] = 'A' + i;
		}
		else
		{
			buf[0] = 'A' + (j - 1);
			buf[1] = 'A' + i;
		}
		return buf;
	}

private:
	Excel::_ApplicationPtr	m_pApp;			///< �A�v���C���X�^���X
	Excel::_WorkbookPtr		m_pWorkbook;	///< ���[�N�u�b�N
	Excel::_WorksheetPtr	m_pSheet;		///< �V�[�g
	Excel::RangePtr			m_pRange;		///< �͈̓|�C���^
public:
	void test(void)
	{
		// COM�̏�����
		::CoInitialize(NULL);

		////////////////////
		// Excel���N��
		Excel::_ApplicationPtr pApp;

		// �C���X�^���X���쐬
		pApp.CreateInstance( "Excel.Application");
		// Excel��\������
//		pApp->PutVisible( 0, TRUE);			//TRUE�ɂ���ƃC���X�^���X���c��

 
		// WorkBook��ǉ�����
//		pApp->Workbooks->Add();

        Excel::_WorkbookPtr pWorkbook;
		try
		{
			pWorkbook = pApp->Workbooks->Open(L"C:\\Home\\Temp\\DDDDD\\Test\\Debug\\test.xlsx");		//�t���p�X�œn�����ƁI
		}
		catch( ... )
		{
			pApp->Quit();				//��\���̏ꍇ�A���̃^�C�~���O�ł̓C���X�^���X�͏����Ȃ��B
			// �C���X�^���X���J��
			pApp.Release();				//��\���̏ꍇ�A������excel.exe�C���X�^���X���Ȃ��Ȃ�B
			::CoUninitialize();
			return;
		}
		// �A�N�e�B�u��Sheet���擾
//		Excel::_WorksheetPtr pSheet;
//		pSheet = pApp->GetActiveSheet();

		Excel::_WorksheetPtr pSheet = pWorkbook->Sheets->Item[L"Sheet2"];

		/////////////////////////
		// Cell�ɕ�������������
		COleVariant varCell;
		varCell = L"B2";
		Excel::RangePtr pRang;
		pRang = pSheet->GetRange( varCell);
		pRang->NumberFormat = "@";

		pRang->Value2 = "=A1+B1";


		Excel::BorderPtr border;
		border = pRang->GetBorders()->GetItem( Excel::xlEdgeTop );
		border->LineStyle = (long)Excel::xlContinuous;
		border = pRang->GetBorders()->GetItem( Excel::xlEdgeBottom);
		border->PutLineStyle((long)Excel::xlContinuous);
		border = pRang->GetBorders()->GetItem( Excel::xlEdgeLeft);
		border->LineStyle = (long)Excel::xlContinuous;
		border = pRang->GetBorders()->GetItem( Excel::xlEdgeRight);
		border->LineStyle = (long)Excel::xlContinuous;
		border->PutWeight((long)Excel::xlMedium);

		pRang->Interior->Color = (long)RGB( 255, 0, 0 );

		varCell = pRang->Value;				//�v�Z���ʂ��擾�ł���B���ł͂Ȃ��B
	

		//////////////////////////////////
		// Cell�Ƀn�C�p�[
		// �����N����������
		varCell = "C2";
//		pRang = pSheet->GetRange( varCell);
		pRang = pSheet->Range[varCell];
		_bstr_t bstrURL = "http://homepage1.nifty.com/goldfish";
		pSheet->Hyperlinks->Add( pRang, bstrURL);
		

		pWorkbook->Save();
		pWorkbook->Close();
	// �C���X�^���X���J��
		pRang.Release();
		pWorkbook.Release();
		pSheet.Release();

		pApp->Quit();				//��\���̏ꍇ�A���̃^�C�~���O�ł̓C���X�^���X�͏����Ȃ��B
		// �C���X�^���X���J��
		pApp.Release();				//��\���̏ꍇ�A������excel.exe�C���X�^���X���Ȃ��Ȃ�B

		::CoUninitialize();

	}

};



}; // TNB