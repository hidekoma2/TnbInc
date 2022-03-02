#pragma once
/**
 *@file
 * �r�b�g�}�b�v�`��֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbBitmapImage.h"
#include "TnbDrawable.h"



//TNB Library
namespace TNB
{



/**@ingroup BITMAP DRAWABLE
 * �r�b�g�}�b�v�`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�R���X�g���N�^���Ɏw�肳�ꂽ�r�b�g�}�b�v��`�悵�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapDrawer.h
 *
 *	@date 07/11/01 �V�K�쐬
 */
class CBitmapDrawer : public IDrawable
{
	DEFSUPER(IDrawable);
protected:

	CBitmapHandle m_bmp;	///< �r�b�g�}�b�v
public:

	/**
	 * �R���X�g���N�^
	 *	@param bmp �r�b�g�}�b�v�B
	 */
	CBitmapDrawer(CBitmapHandle bmp) : m_bmp(bmp)
	{
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CBitmapDrawer(m_bmp);
	}

	/**
	 * [�擾] �T�C�Y�擾.
	 *		�{�I�u�W�F�N�g�̕`�悷��̈�̊O�ڂ���l�p�̃T�C�Y�𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _size �T�C�Y�擾�B
	 *	@retval true �T�C�Y�擾�����B 
	 *	@retval false �f�[�^���Ȃ��B
	 */
	virtual bool GetSize(SIZE& _size) const
	{
		return m_bmp.GetSize(_size);
	}

	/**
	 * [�ݒ�] �T�C�Y�ݒ�.
	 *		�{�I�u�W�F�N�g�̕`�悷��̈�̊O�ڂ���l�p�̃T�C�Y��ݒ肷�邱�Ƃ��o���܂��B
	 *	@note �{�N���X�̓T�C�Y�ύX���T�|�[�g���܂���B
	 *	@param size �T�C�Y�B
	 *	@retval true �T�C�Y�ύX�����B 
	 *	@retval false ���s�B�T�C�Y�ύX�̓T�|�[�g���܂���B
	 */
	virtual bool Resize(const SIZE& size)
	{
		return false;
	}

	/**
	 * [�`��] �`��.
	 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��̍��W�ɕ`�悵�܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g
	 *	@param x �w���W
	 *	@param y �x���W
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const
	{
#ifndef _WIN32_WCE
		HBITMAP h = m_bmp;
		::DrawState(dc, NULL, NULL, reinterpret_cast<LPARAM>(h), 0, x, y, 0, 0, DST_BITMAP);
#else
		HBITMAP h = m_bmp;
		CBitmapHandle::Draw(dc, h, x, y);
#endif
	}

	/**
	 * [�쐬] �r�b�g�}�b�v�쐬.
	 *		�`���񂩂�A�r�b�g�}�b�v���쐬���܂��B
	 *	@param draw �`����
	 *	@param color �w�i�F���w�肵�܂��B�ȗ�����ƕs��i�h��Ԃ����Ȃ�) �ɂȂ�܂��B
	 *				�S�G���A�`�悷��`����̏ꍇ�A�w�i�F���w�肷��K�v�͂���܂���B
	 *	@return �r�b�g�}�b�v�B�G���[�̏ꍇ�A NULL��Ԃ̃n���h�����Ԃ�܂��B
	 */
	static CBitmapHandle ToBitmap(const IDrawable& draw, COLORREF color = CLR_INVALID)
	{
		CBitmapImage bi;
		SIZE size;
		if ( draw.GetSize(size) )
		{
			if ( bi.Set(size.cx, size.cy, color) )
			{
				HDC dc = bi.GetDC();
				draw.Draw(dc);
				bi.ReleaseDC();
			}
		}
		return bi;
	}
};



#ifndef _WIN32_WCE
/**@ingroup BITMAP DRAWABLE
 * �r�b�g�}�b�v�g���`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�R���X�g���N�^���Ɏw�肳�ꂽ�r�b�g�}�b�v��`�悵�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapDrawer.h
 *
 *	@date 07/11/01 �V�K�쐬
 */
class CBitmapExDrawer : public CBitmapDrawer
{
	DEFSUPER(CBitmapDrawer);
	UINT		m_uExFlag;		///< �t���O
public:

	/**
	 * �R���X�g���N�^
	 *	@param bmp �r�b�g�}�b�v�n���h���B
	 *	@param uExFlag �}���t���O�B�ȉ���3��A�g�p�ł��܂��B\n
	 *				�ȗ��� DSS_NORMAL - - - �C���[�W�����̂܂ܕ`�悵�܂��B \n
	 *				DSS_DISABLED - - - �C���[�W�𕂂��オ�点�܂��i�G���{�X�j�B\n
	 *				DSS_UNION - - - �C���[�W���f�B�U�����O���܂��B 
	 */
	CBitmapExDrawer(CBitmapHandle bmp, UINT uExFlag = DSS_NORMAL)
		: _super(bmp), m_uExFlag(uExFlag)
	{
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CBitmapExDrawer(m_bmp, m_uExFlag);
	}

	/**
	 * [�`��] �`��.
	 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��̍��W�ɕ`�悵�܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g
	 *	@param x �w���W
	 *	@param y �x���W
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const
	{
		HBITMAP h = m_bmp;
		::DrawState(dc, NULL, NULL, reinterpret_cast<LPARAM>(h), 0, x, y, 0, 0, DST_BITMAP | m_uExFlag);
	}
};
#endif //_WIN32_WCE



/**@ingroup BITMAP DRAWABLE
 * �r�b�g�}�b�v���ߕ`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�R���X�g���N�^���Ɏw�肳�ꂽ�r�b�g�}�b�v�𓧉�,�����ߏ������s���Ȃ���`�悵�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapDrawer.h
 *
 *	@date 07/11/01 �V�K�쐬
 */
class CSemitransparentBitmapDrawer : public CBitmapDrawer
{
	DEFSUPER(CBitmapDrawer);
	int			m_parsent;		///< �Z��
	COLORREF	m_color;		///< ���ߐF
public:

	/**
	 * �R���X�g���N�^
	 *	@param bmp �r�b�g�}�b�v�n���h���B
	 *	@param par �`��摜�̔Z��(�P��;%) �B -1 ���w�肷��ƁA �ێ����Ă���C���[�W�̃s�N�Z�����̃A���t�@�`���l�����g�p���܂��B
	 *	@param color ���߂���F�B CLR_INVALID ���w�肷��ƁA���ߐF�͂Ȃ��B
	 */
	CSemitransparentBitmapDrawer(CBitmapHandle bmp, int par = 100, COLORREF color = CLR_INVALID)
		: _super(bmp), m_parsent(par), m_color(color)
	{
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CSemitransparentBitmapDrawer(m_bmp, m_parsent, m_color);
	}

	/**
	 * [�`��] �`��.
	 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��̍��W�ɕ`�悵�܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g
	 *	@param x �w���W
	 *	@param y �x���W
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const
	{
		CBitmapImage bi(m_bmp);
		bi.SemitransparectBlt(dc, x, y, m_parsent, m_color);
	}
};



/**@ingroup BITMAP DRAWABLE
 * �^�C���`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�R���X�g���N�^���Ɏw�肳�ꂽ�r�b�g�}�b�v,�`������w��̑傫���ɕ~���l�ߕ`�悵�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapDrawer.h
 *
 *	@date 07/11/14 �V�K�쐬
 */
class CTileDrawer : public CBitmapDrawer
{
	DEFSUPER(CBitmapDrawer);
	SIZE		m_size;		///< �T�C�Y
public:

	/**
	 * �R���X�g���N�^
	 *	@param size �T�C�Y�B
	 *	@param bmp �r�b�g�}�b�v�B
	 */
	CTileDrawer(const SIZE& size, CBitmapHandle bmp) : _super(bmp), m_size(size)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@param size �T�C�Y�B
	 *	@param draw �`����B���͖{�C���X�^���X�ŕێ����܂��̂ŁA�j�����Ă����܂��܂���B
	 *	@param color �w�i�F���w�肵�܂��B�ȗ�����ƕs��i�h��Ԃ����Ȃ�) �ɂȂ�܂��B
	 *				�S�G���A�`�悷��`����̏ꍇ�A�w�i�F���w�肷��K�v�͂���܂���B
	 */
	CTileDrawer(const SIZE& size, const IDrawable& draw, COLORREF color = CLR_INVALID) 
		: _super(CBitmapDrawer::ToBitmap(draw, color)), m_size(size)
	{
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CTileDrawer(m_size, m_bmp);
	}

	/**
	 * [�擾] �T�C�Y�擾.
	 *		�{�I�u�W�F�N�g�̕`�悷��̈�̊O�ڂ���l�p�̃T�C�Y�𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _size �T�C�Y�擾�B
	 *	@retval true �T�C�Y�擾�����B 
	 *	@retval false �f�[�^���Ȃ��B
	 */
	virtual bool GetSize(SIZE& _size) const
	{
		_size = m_size;
		return ! m_bmp.IsNull();
	}

	/**
	 * [�`��] �`��.
	 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��̍��W�ɕ`�悵�܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g
	 *	@param x �w���W
	 *	@param y �x���W
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const
	{
		CBitmapImage bi(m_bmp);
		RECT rect = { x, y, x + m_size.cx, y + m_size.cy };
		bi.DrawTile(dc, rect);	
	}
};



/**@ingroup DRAWABLE
 * ���ߓh��Ԃ��`��N���X
 *
 *		IDrawable �C���^�[�t�F�[�X�������Ă��܂��B
 *
 *		�R���X�g���N�^���Ɏw�肳�ꂽ�F�𔼓��ߏ������s���Ȃ���`�悵�܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbBitmapDrawer.h
 *
 *	@date 07/12/13 �V�K�쐬
 *	@date 08/03/23 �R���X�g���N�^��ǉ��B
 */
class CSemitransparentBoxFillDrawer : public CNullDrawer
{
	DEFSUPER(CNullDrawer);
	COLORREF	m_color;		///< �F
	int			m_parsent;		///< �Z��
public:

	/**
	 * �R���X�g���N�^
	 *	@param size �T�C�Y�B
	 *	@param color �F
	 *	@param par �`��̔Z��(�P��;%) 
	 */
	CSemitransparentBoxFillDrawer(const SIZE& size, COLORREF color, int par = 100)
		: _super(size), m_color(color), m_parsent(par)
	{
	}

	/**
	 * �R���X�g���N�^
	 *	@note �T�C�Y�� 0, 0 �ɂȂ�܂��B
	 *	@param color �F
	 *	@param par �`��̔Z��(�P��;%) 
	 */
	CSemitransparentBoxFillDrawer(COLORREF color, int par = 100)
		: _super(), m_color(color), m_parsent(par)
	{
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual IDrawable* Clone(void) const
	{
		return new CSemitransparentBoxFillDrawer(m_size, m_color, m_parsent);
	}

	/**
	 * [�`��] �`��.
	 *		�w��̃f�o�C�X�R���e�L�X�g�̎w��̍��W�ɕ`�悵�܂��B
	 *	@param dc �f�o�C�X�R���e�L�X�g
	 *	@param x �w���W
	 *	@param y �x���W
	 */
	virtual void Draw(HDC dc, int x = 0, int y = 0) const
	{
		CBitmapImage bi;
		bi.Set(m_size.cx, m_size.cy, m_color);
		bi.AlphaBlend(dc, x, y, m_parsent);
	}
};



};///TNB

