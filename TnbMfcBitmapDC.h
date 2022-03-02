#pragma once
/**
 *@file
 * �r�b�g�}�b�vDC�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbBitmapImage.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup WINAPI BITMAP
 * �r�b�g�}�b�v�f�o�C�X�R���e�L�X�g�N���X
 *
 *		CBitmapImage ��ێ����A CDC �N���X�����ɑ���ł���悤�ɂ��郉�b�p�N���X�ł��B
 *
 *	@note �\�z���� CBitmapImage::GetDC() �֐������s���A�j������ CBitmapImage::ReleaseDC() �֐������s���܂��B 
 *		�{�C���X�^���X�Ŏg�p���� CBitmapImage �I�u�W�F�N�g�́A��{�I�ɑ��삵�Ȃ��悤�ɂ��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcBitmapDC.h
 * 
 *	@date 07/10/19 �V�K�쐬
 *	@date 07/11/13 �A�^�b�`�A�f�^�b�`�A�ǉ�
 *	@date 07/11/25 Draw() �ǉ�
 *	@date 11/05/19 �f�t�H���g�R���X�g���N�^�A�p�ӁB
 */
class CBitmapDC : public CDC
{
	DEFSUPER(CDC);
public:

	/**
	 * �R���X�g���N�^
	 *	@param pBI �������C���[�W������ CBitmapImage �I�u�W�F�N�g�ւ̃|�C���^�B
	 *	@throw CResourceException �s���� CBitmapImage ��n�������A�ʒm����܂��B
	 */
	explicit CBitmapDC(CBitmapImage* pBI) : m_pBitImg(pBI)
	{
		if ( m_pBitImg != NULL && _super::Attach(m_pBitImg->GetDC()) )
		{
		}
		else
		{
			TRACE0(" CBitmapDC �L���łȂ� CBItmapImage ���w�肵�܂����B\n" );
			AfxThrowResourceException();
		}
	}

	/**
	 * �R���X�g���N�^.
	 *	@note DC ���A�^�b�`����Ă��Ȃ��C���X�^���X�ɂȂ�܂��̂ŁA�K���A Attach() ���ŏ��Ɏg���Ă��������B
	 */
	explicit CBitmapDC(void) : m_pBitImg(NULL)
	{
	}

	/// �f�X�g���N�^
	virtual ~CBitmapDC(void)
	{
		Detach();
	}

	/**
	 * [����] �C���[�W�`��.
	 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��̈ʒu�ɁA�{�C���X�^���X�Ŏ����Ă���C���[�W��`�悵�܂��B
	 *	@param dc �`��Ώۂ̃f�o�C�X�R���e�L�X�g�B
	 *	@param x �`��X�ʒu(����0)�B�ȗ�����ƂO�B
	 *	@param y �`��Y�ʒu(�オ0)�B�ȗ�����ƂO�B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool Draw(HDC dc, int x = 0, int y = 0) const
	{
		if ( m_pBitImg != NULL )
		{	
			const SIZE& size = m_pBitImg->GetSize();
			return !! ::BitBlt(dc, x, y, size.cx, size.cy, *this, 0, 0, SRCCOPY);
		}
		return false;
	}

	/**
	 * [�ݒ�] �A�^�b�`
	 *	@param pBI �������C���[�W������ CBitmapImage �I�u�W�F�N�g�ւ̃|�C���^�B
	 *			�ʏ�A Detach() �����|�C���^���w�肵�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B�s���� CBitmapImage ��n�������A�ʒm����܂��B
	 */
	bool Attach(CBitmapImage* pBI)
	{
		Detach();
		if ( pBI != NULL && _super::Attach(pBI->GetDC()) )
		{
			m_pBitImg = pBI;
			return true;
		}
		return false;
	}

	/**
	 * [�ݒ�] �f�^�b�`.
	 *	@note �f�^�b�`�����炻�̃|�C���^�� Attach() ���܂��B
	 *	@retval NULL�ȊO �����BCBitmapImage �̃|�C���^�ł��B�j�����Ă͂����܂���B
	 *	@retval NULL ���s�B�s���� CBitmapImage ���n����Ă��܂���ł����B
	 */
	CBitmapImage* Detach(void)
	{
		CBitmapImage* P = m_pBitImg;
		_super::Detach();
		if ( m_pBitImg != NULL )
		{
			m_pBitImg->ReleaseDC();
		}
		m_pBitImg = NULL;
		return P;
	}

private:
	CBitmapImage*	m_pBitImg;	///< �Ǘ����� BitmapImage 
};



}; //MFC
}; //TNB


