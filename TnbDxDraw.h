#pragma once
/**
 *@file
 * DirectX�֌W�̃w�b�_
 *
 *	DirectX 7 �ȏ�� SDK ���K�{�ł��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbPointerHandle.h"



#ifndef INITGUID
 #define INITGUID
#endif
#include <ddraw.h> // SDK
#pragma comment(lib,"ddraw.lib")
#pragma comment(lib,"dxguid.lib")



//TNB Library
namespace TNB{
namespace DX
{



#ifndef _TnbDOXYGEN	//Document�쐬�p�V���{��
/// PointerHandleBase�p DIRECTDRAWSURFACE7 �I�u�W�F�N�g Release�N���X
struct TPhReleaseSurface
{
	void operator()(LPDIRECTDRAWSURFACE7 P) { P->Release(); }
};
typedef TNB::CPointerHandleBaseT<LPDIRECTDRAWSURFACE7, TPhReleaseSurface, NULL> CSurfaceHandle;
#endif



/**@ingroup DIRECTX
 * DirectDraw �Ǘ�.
 *
 *
 *	@par�K�v�t�@�C��
 *			TnbDxDraw.h
 *			\n DirectX 7 �ȏ�� SDK ���K�v�ł��B
 *
 *	@date 09/07/23	�V�K�쐬
 *	@date 10/02/16	SetOffset() �ŁAX Y �ʁX�̔{�����w��ł���悤�ɂ����B
 */
class CDirectDraw : CCopyImpossible
{
public:

	/**
	 * �o�b�t�@�Ǘ��^.
	 *	@see GetBuffer()
	 */
	struct TBuffer
	{
		SIZE	size;				///< �o�b�t�@�T�C�Y
		int		pitch;				///< ���̍s�̐擪�܂ł̋���
		int		bitsPixel;			///< ��s�N�Z���� bit���B 8, 16, 32�B
		/// �摜�̍���̃|�C���^
		union
		{
			BYTE*		pB8;	///< bitsPixel ���W�̎�
			WORD*		pB16;	///< bitsPixel ��16�̎�
			RGBTRIPLE*	pB24;	///< bitsPixel ��24�̎�
			RGBQUAD*	pB32;	///< bitsPixel ��32�̎�
		};
		/**
		 * [�擾] �|�W�V�����v�Z.
		 *	@param x X���W
		 *	@param y Y���W
		 *	@return �|�W�V����
		 */
		int Pos(int x, int y) const
		{
			return x + y * pitch;
		}
	};

	/**@ingroup DIRECTX
	 * DirectDraw �T�[�t�F�X�Ǘ�.
	 *	
	 * 
	 *	@note �������\�ł����A�R�s�[��������T�[�t�F�X���Q�Ƃ�����̂ɂȂ�܂��B
	 *
	 *	@par�K�v�t�@�C��
	 *			TnbDxDraw.h
	 *			\n DirectX 7 �ȏ�� SDK ���K�v�ł��B
	 *
	 *	@date 09/07/23	�V�K�쐬
	 */
	class CSurface
	{
	public:

		/// �f�X�g���N�^
		~CSurface()
		{
			m_ReleaseRent();
		}

		/// �R���X�g���N�^.
		CSurface(void) : m_isRentBuffer(false), m_hDC(NULL)
		{
			m_CheckSpec();
		}

		/**
		 * �R���X�g���N�^.
		 *	@note �쐬�����T�[�t�F�X���w�肷�邱�Ƃ�,�{�N���X�ŊǗ����邱�Ƃ��o���܂��B
		 *	@param pSurface �T�[�t�F�X�|�C���^�B���̃T�[�t�F�X�͖{�N���X���Ǘ�����̂ŁA Release���Ȃ��ł��������B
		 */
		CSurface(LPDIRECTDRAWSURFACE7 pSurface) : m_hpSurface(pSurface)
		{
			m_CheckSpec();
		}

		/**
		 * �R�s�[�R���X�g���N�^.
		 *	@attention const �I�u�W�F�N�g�̓R�s�[���Ɏg���܂���B
		 *	@param other �R�s�[��
		 */
		CSurface(CSurface& other)
		{
			operator=(other);
		}

		/**
		 * �R�s�[�I�y���[�^.
		 *	@attention const �I�u�W�F�N�g�̓R�s�[���Ɏg���܂���B
		 *	@param other �R�s�[��
		 *	@return �����̎Q��.
		 */
		CSurface& operator=(CSurface& other)
		{
			other.m_ReleaseRent();
			m_hpSurface = other.m_hpSurface;
			m_primarySize = other.m_primarySize;
			m_bitsPixel = other.m_bitsPixel;
			m_isRentBuffer = false;
			m_hDC = NULL;
			return *this;
		}

		/**
		 * [�o�^] �A�^�b�`.
		 *	@note ��ɕێ����Ă���T�[�t�F�X������΁A�j�����Ă���A�^�b�`���܂��B
		 *	@note �쐬�����T�[�t�F�X���w�肷�邱�Ƃ�,�{�N���X�ŊǗ����邱�Ƃ��o���܂��B
		 *	@param pSurface �T�[�t�F�X�|�C���^�B���̃T�[�t�F�X�͖{�N���X���Ǘ�����̂ŁA Release���Ȃ��ł��������B
		 */
		void Attach(LPDIRECTDRAWSURFACE7 pSurface)
		{
			m_ReleaseRent();
			m_hpSurface = pSurface;
			m_CheckSpec();
		}

		/**
		 * [�쐬] �쐬.
		 *	@note ��ɕێ����Ă���T�[�t�F�X������΁A�j�����Ă���쐬���܂��B
		 *	@param pDraw �L���� DirectDraw �̃|�C���^
		 *	@param pDdsd �T�[�t�F�X�쐬�p���
		 *	@retval DD_OK �����B�쐬�����T�[�t�F�X��ێ����܂��B
		 *	@retval ��L�ȊO ���s�B
		 */
		HRESULT Create(IDirectDraw7* pDraw, LPDDSURFACEDESC2 pDdsd)
		{
			Destroy();
			LPDIRECTDRAWSURFACE7 P;
			HRESULT r = pDraw->CreateSurface(pDdsd, &P, NULL);
			if ( r == DD_OK )
			{
				m_hpSurface = P;
				m_CheckSpec();
			}
			return r;
		}

		/**
		 * [�j��] �j��.
		 *		�ێ����Ă���T�[�t�F�X��j�����܂��B
		 *		�ێ����Ă��Ȃ���Ή������܂���B�@
		 */
		void Destroy(void)
		{
			m_ReleaseRent();
			m_hpSurface = NULL;
		}

		/**
		 * [�m�F] �L���m�F.
		 *	@retval true �L���B�T�[�t�F�X��ێ����Ă��܂��B
		 *	@retval false �����B�T�[�t�F�X��ێ����Ă��܂���B
		 */
		bool IsValid(void) const
		{
			return ! m_hpSurface.IsNull();
		}

		/**
		 * [�擾] �T�C�Y�擾.
		 *		�ێ����Ă���T�[�t�F�X�̃T�C�Y��Ԃ��܂��B
		 *	@return �T�C�Y
		 */
		const SIZE& GetSize(void) const
		{
			return m_primarySize;
		}

		/**
		 * [�Q��] �Q��.
		 *	@note DirectDraw �� API �̈����ɓn�����߂ɃT�[�t�F�X�̃|�C���^��Ԃ��܂��B
		 *	@return �T�[�t�F�X�|�C���^
		 */
		operator LPDIRECTDRAWSURFACE7(void)
		{
			DEBUG_ONLY( m_ReleaseRent() );
			return m_hpSurface;
		}

		/**
		 * [�Q��] �Q��.
		 *	@note DirectDraw �� IDirectDrawSurface �̃��\�b�h�����s���邽�߂ɃT�[�t�F�X�̃|�C���^��Ԃ��܂��B
		 *	@return �T�[�t�F�X�|�C���^
		 */
		LPDIRECTDRAWSURFACE7 operator->(void)
		{
			DEBUG_ONLY( m_ReleaseRent() );
			return m_hpSurface;
		}

		/**
		 * [�`��] BltFast.
		 *	@param x		�]����X���W
		 *	@param y		�]����Y���W 
		 *	@param lpDdsSrc	�]�����T�[�t�F�X
		 *	@param srcRect	�]�����͈̔�
		 *	@param dwTrans	�]���t���O
		 *	@retval DD_OK �����B
		 *	@retval ��L�ȊO ���s�B
		 */
		HRESULT BltFast(int x, int y, LPDIRECTDRAWSURFACE7 lpDdsSrc, const RECT& srcRect, DWORD dwTrans = 0)
		{
			CRect rc = srcRect;
			HRESULT hr = m_hpSurface->BltFast(x, y, lpDdsSrc, &rc, DDBLTFAST_WAIT | dwTrans);
			if ( hr == DDERR_SURFACELOST )
			{
				m_hpSurface->Restore();
				hr = m_hpSurface->BltFast(x, y, lpDdsSrc, &rc, DDBLTFAST_WAIT | dwTrans);
			}
			return hr;
		}

		/**
		 * [�`��] �N���b�s���O BltFast.
		 *		�N���b�s���O�����t���� BltFase �ł��B
		 *	@param x		�]����X���W
		 *	@param y		�]����Y���W 
		 *	@param lpDdsSrc	�]�����T�[�t�F�X
		 *	@param srcRect	�]�����͈̔�
		 *	@param dwTrans	�]���t���O
		 *	@retval DD_OK �����B
		 *	@retval ��L�ȊO ���s�B
		 */
		HRESULT BltFastClip(int x, int y, LPDIRECTDRAWSURFACE7 lpDdsSrc, const RECT& srcRect, DWORD dwTrans = 0)
		{
			RECT rc = srcRect;
			int width = rc.right - rc.left;
			int height = rc.bottom - rc.top;
			// �w��͈͂𒴂����摜��؂���
			if ( x < 0 )
			{
				rc.left += (-x);
				x = 0;
			}
			else if ( m_primarySize.cx < x + width )
			{
				rc.right -= (x + width - m_primarySize.cx);
			}
			if ( y < 0)
			{
				rc.top += (-y);
				y = 0;
			}
			else if ( m_primarySize.cy < y + height)
			{
				rc.bottom -= (y + height - m_primarySize.cy);
			}
			//�`��
			return BltFast(x, y, lpDdsSrc, rc, dwTrans);
		}

		/**
		 * [�`��] Blt.
		 *	@param dstRect	�]����͈�
		 *	@param lpDdsSrc	�]�����T�[�t�F�X
		 *	@param srcRect	�]�����͈̔�
		 *	@param dwTrans	�]���t���O
		 *	@retval DD_OK �����B
		 *	@retval ��L�ȊO ���s�B
		 */
		HRESULT Blt(const RECT& dstRect, LPDIRECTDRAWSURFACE7 lpDdsSrc, const RECT& srcRect, DWORD dwTrans = 0)
		{
			CRect rc1 = dstRect;
			CRect rc2 = srcRect;
			HRESULT hr = m_hpSurface->Blt(&rc1, lpDdsSrc, &rc2, DDBLT_WAIT | dwTrans, NULL);
			if ( hr == DDERR_SURFACELOST )
			{
				m_hpSurface->Restore();
				hr = m_hpSurface->Blt(&rc1, lpDdsSrc, &rc2, DDBLT_WAIT | dwTrans, NULL);
			}
			return hr;
		}

		/**
		 * [�`��] �N���b�s���O Blt.
		 *		�N���b�s���O�����t���� Blt �ł��B
		 *	@param dstRect	�]����͈�
		 *	@param lpDdsSrc	�]�����T�[�t�F�X
		 *	@param srcRect	�]�����͈̔�
		 *	@param dwTrans	�]���t���O
		 *	@retval DD_OK �����B
		 *	@retval ��L�ȊO ���s�B
		 */
		HRESULT BltClip(const RECT& dstRect, LPDIRECTDRAWSURFACE7 lpDdsSrc, const RECT& srcRect, DWORD dwTrans = 0)
		{
			RECT dstRc = dstRect;
			RECT srcRc = srcRect;
			SIZE dstSz = { dstRc.right - dstRc.left, dstRc.bottom - dstRc.top };
			SIZE srcSz = { srcRc.right - srcRc.left, srcRc.bottom - srcRc.top };
			const LONG par = 1024;
			LONG parHeight = dstSz.cx * par / srcSz.cx;
			LONG parWidth = dstSz.cy * par / srcSz.cy;
			// �w��͈͂𒴂����摜��؂���
			int w1 = dstRc.right - m_clipRect.right;
			if ( w1 > 0 )
			{
				srcRc.right -= w1 * par / parWidth;
				dstRc.right -= w1;
			}
			int w2 = dstRc.left - m_clipRect.left;
			if ( w2 < 0)
			{
				srcRc.left -= w2 * par / parWidth;
				dstRc.left -= w2;
			}
			int h1 = dstRc.bottom - m_clipRect.bottom;
			if ( h1 > 0 )
			{
				srcRc.bottom -= h1 * par / parHeight;
				dstRc.bottom -= h1;
			}
			int h2 = dstRc.top - m_clipRect.top;
			if ( h2 < 0)
			{
				srcRc.top -= h2 * par / parHeight;
				dstRc.top -= h2;
			}
			//
			return Blt(dstRc, lpDdsSrc, srcRc, dwTrans);
		}

		/**
		 * [�擾] HDC �擾.
		 *		�T�[�t�F�X�� HDC ���擾�ł��܂��B
		 *	@note  ReleaseBuffer() �����s����܂��̂ŁA GetBuffer() �̖߂�l�������ɂȂ�܂��B
		 *	@attention GetDC() �� ReleaseDC() �̊Ԃ͋ɗ͏�����Z���Ȃ�悤�ɐS�����Ă��������B
		 *	@retval NULL ���s�B��� GetDC() ��A ReleaseDC() ���Ă��Ȃ��ꍇ�����s���܂��B
		 *	@retval NULL�ȊO �����B�l�� HDC�B�g�p��A�j�������A ReleaseDC() ���R�[�����Ă��������B
		 */
		HDC GetDC(void)
		{
			if ( IsValid() && m_hDC == NULL )
			{
				ReleaseBuffer();
				if ( m_hpSurface->GetDC(&m_hDC) == DD_OK )
				{
					return m_hDC;
				}
			}
			return NULL;
		}

		/**
		 * [�擾] HDC �ԋp.
		 *		GetDC() �œ��� HDC �͎g���Ȃ��Ȃ�܂��B
		 */
		void ReleaseDC(void)
		{
			if ( IsValid() && m_hDC != NULL )
			{
				m_hpSurface->ReleaseDC(m_hDC);
				m_hDC = NULL;
			}
		}

		/**
		 * [�擾] �o�b�t�@�擾.
		 *		�T�[�t�F�X�̃o�b�t�@���擾�ł��܂��B
		 *	@note  ReleaseDC() �����s����܂��̂ŁA GetDC() �œ��� HDC �͖����ɂȂ�܂��B
		 *	@attention GetBuffer() �� ReleaseBuffer() �̊Ԃ͋ɗ͏�����Z���Ȃ�悤�ɐS�����Ă��������B
		 *	@retval NULL ���s�B��� GetBuffer() ��A ReleaseDC() ���Ă��Ȃ��ꍇ�����s���܂��B
		 *	@retval NULL�ȊO �����B�l�̓o�b�t�@�Ǘ��̃|�C���^�ł��B�g�p��A�j�������A ReleaseBuffer() ���R�[�����Ă��������B
		 */
		const TBuffer* GetBuffer(void)
		{
			if ( IsValid() && ! m_isRentBuffer )
			{
				ReleaseDC();
				DDSURFACEDESC2 ddsd = { sizeof(DDSURFACEDESC2) };
				if ( m_hpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) == DD_OK )
				{
					m_buffer.size = m_primarySize;
					m_buffer.pitch = ddsd.lPitch / m_bitsPixel * 8;
					m_buffer.bitsPixel = m_bitsPixel;
					m_buffer.pB8 = static_cast<BYTE*>(ddsd.lpSurface);
					m_isRentBuffer = true;
					return &m_buffer;
				}
			}
			return NULL;
		}

		/**
		 * [�擾] �o�b�t�@�ԋp.
		 *		GetBuffer() �œ����o�b�t�@���̃|�C���^�͎g���Ȃ��Ȃ�܂��B
		 */
		void ReleaseBuffer(void)
		{
			if ( IsValid() && m_isRentBuffer )
			{
				m_hpSurface->Unlock(NULL);
				m_isRentBuffer = false;
			}
		}

		/**
		 * [�ݒ�] �N���b�s���O�͈͐ݒ�.
		 *		�N���b�s���O�͈͂�ݒ肵�܂��B
		 *	@note �ʏ�g���܂���B
		 *	@param rect �͈�
		 */
		void SetClipRect(const RECT& rect)
		{
			m_clipRect = rect;
		}

	private:

		/// �T�[�t�F�X�X�y�b�N�擾
		void m_CheckSpec(void)
		{
			if ( IsValid() )
			{
				DDSURFACEDESC2 ddsd = { sizeof(DDSURFACEDESC2) };
				DDPIXELFORMAT ddpf = { sizeof(DDPIXELFORMAT) };
				if ( m_hpSurface->GetSurfaceDesc(&ddsd) == DD_OK && m_hpSurface->GetPixelFormat(&ddpf) == DD_OK )
				{
					DWORD f = DDSD_WIDTH | DDSD_HEIGHT;
					if ( (ddsd.dwFlags & f) == f )
					{
						m_primarySize.cx = ddsd.dwWidth;
						m_primarySize.cy = ddsd.dwHeight;
						::SetRect(&m_clipRect, 0, 0, m_primarySize.cx, m_primarySize.cy);
						m_bitsPixel = ddpf.dwRGBBitCount;
						return;
					}
				}
			}
			m_primarySize.cx = 0;
			m_primarySize.cy = 0;
			::SetRectEmpty(&m_clipRect);
			m_bitsPixel = 0;
		}
		/// �݂��o�����̂��J��
		void m_ReleaseRent(void)
		{
			ReleaseDC();
			ReleaseBuffer();
		}
		//
		CSurfaceHandle	m_hpSurface;	///< �I�u�W�F�N�g
		SIZE			m_primarySize;			///< �T�C�Y
		int				m_bitsPixel;	///< �s�N�Z���r�b�g��
		//
		RECT			m_clipRect;		///< �N���b�v�͈�
		//
		bool			m_isRentBuffer;	///< �݂��o�����t���O
		HDC				m_hDC;			///< DC �n���h��
		TBuffer			m_buffer;		///< GetBuffer�p���U���g
	};


	//------------------------
		
	
	/// �R���X�g���N�^
	CDirectDraw(void) : m_pDraw(NULL), m_isFull(false), m_pClip(NULL), m_hDC(NULL), m_pGetBuffSurface(NULL)
	{
		m_primarySize.cx = 0;
		m_primarySize.cy = 0;
		m_backSize.cx = 0;
		m_backSize.cy = 0;
		m_offset.x = 0;
		m_offset.y = 0;
		m_magnific.x = 1;
		m_magnific.y = 1;
	}

	/// �f�X�g���N�^
	~CDirectDraw(void)
	{
		Destroy();
	}

	/**
	 * [�ݒ�] �t���X�N���[�����[�h�ݒ�.
	 *		�t���X�N���[�����[�h�ƃE�B���h�E���[�h��ؑւ��܂��B
	 *	@note ������Ԃł̓E�B���h�E���[�h�ł��B
	 *	@note Create() �O�ł���ł��@�\���܂��B
	 *	@param isFull true ���w�肷��ƃt���X�N���[�����[�h�B false ���w�肷��ƃE�B���h�E���[�h�ɂȂ�܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetFullScreenMode(bool isFull)
	{
		if ( m_isFull != isFull )
		{
			if ( m_pDraw != NULL )
			{
				m_DestroySurface();
				m_isFull = isFull;
				return m_CreateSurface();
			}
			m_isFull = isFull;
		}
		return true;
	}

	/**
	 * [�ݒ�] �T�C�Y�ݒ�.
	 *		�v���C�}���T�[�t�F�X�ƃo�b�N�o�b�t�@�T�[�t�F�X�̃T�C�Y��ݒ肵�܂��B
	 *	@note Create() ��ł��@�\���܂��B
	 *	@param size �v���C�}���T�[�t�F�X�ƃo�b�N�o�b�t�@�T�[�t�F�X�̃T�C�Y�B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetSize(const SIZE& size)
	{
		return SetSize(size, size);
	}

	/**
	 * [�ݒ�] �T�C�Y�ݒ�.
	 *		�v���C�}���T�[�t�F�X�ƃo�b�N�o�b�t�@�T�[�t�F�X�̃T�C�Y��ݒ肵�܂��B
	 *	@note Create() ��ł��@�\���܂��B
	 *	@param primarySize �v���C�}���T�[�t�F�X�̃T�C�Y
	 *	@param backSize �o�b�N�o�b�t�@�T�[�t�F�X�̃T�C�Y�B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetSize(const SIZE& primarySize, const SIZE& backSize)
	{
		m_primarySize = primarySize;
		m_backSize = backSize;
		if ( m_pDraw != NULL )
		{
			m_DestroySurface();
			return m_CreateSurface();
		}
		return true;
	}

	/**
	 * [�쐬] �쐬.
	 *		DirectDraw ���쐬���܂��B
	 *	@note SetSize() �ɂāA��ɃT�C�Y���w�肵�Ă����K�v������܂��B
	 *	@param hWnd �֘A����E�B���h�E�n���h��.
	 *	@param bitsPixel �P�s�N�Z���̃r�b�g���B�ȗ�����ƁA���݂̃f�B�X�v���C�̐ݒ肪�g���܂�( GetBitsPixel() �œ��邱�Ƃ��\ )�B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Create(HWND hWnd, int bitsPixel = -1)
	{
		Destroy();
		m_hWnd = hWnd;
		if ( bitsPixel < 0 )
		{
			DEVMODE dm = { sizeof(DEVMODE) };
			if ( ! ::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm) )
			{
				return false;
			}
			bitsPixel = dm.dmBitsPerPel;
		}
		m_bitsPixel = bitsPixel;
		C_ASSERT( DD_OK == 0 );
		if ( ::DirectDrawCreateEx(NULL, reinterpret_cast<void**>(&m_pDraw), IID_IDirectDraw7, NULL) != DD_OK )
		{
			return false;
		}
		if ( ! m_CreateSurface() )
		{
			Destroy();
			return false;
		}
		return true;
	}

	/**
	 * [�j��] �j��.
	 *		DirectDraw �I�u�W�F�N�g��j�����܂��B
	 *	@note �쐬���Ă��Ȃ���Ή������܂���B
	 */
	void Destroy(void)
	{
		if ( m_backbufferSurface.IsValid() && m_hDC != NULL )
		{
			m_backbufferSurface->ReleaseDC(m_hDC);
			m_hDC = NULL;
		}
		m_DestroySurface();
		m_Release(m_pDraw);
	}

	/**
	 * [�擾] �v���C�}���T�[�t�F�X�̃T�C�Y�擾.
	 *	@return �T�C�Y
	 */
	const SIZE& GetPrimarySize(void) const
	{
		return m_primarySize;
	}

	/**
	 * [�擾] �o�b�N�o�b�t�@�T�[�t�F�X�̃T�C�Y�擾.
	 *	@return �T�C�Y
	 */
	const SIZE& GetBackbufferSize(void) const
	{
		return m_backSize;
	}

	/**
	 * [�擾] �s�N�Z�� BIT ���擾.
	 *	@note Create() �O�́A�V�X�e���̒l�A Create() ��́A DirectDraw ����l�𓾂Ă��܂��B
	 *	@retval �}�C�i�X �G���[�B
	 *	@retval �O�ȏ� �����B�l�́A�P�s�N�Z���� BIT ���ł��B
	 */
	int GetBitsPixel(void) const
	{
		if ( m_pDraw != NULL )
		{
			DDSURFACEDESC2 ddsd = { sizeof(DDSURFACEDESC2) };
			if ( m_pDraw->GetDisplayMode(&ddsd) == DD_OK )
			{
				return (ddsd.lPitch / ddsd.dwWidth) * 8;
			}
		}
		else
		{
			DEVMODE dm = { sizeof(DEVMODE) };
			if ( ::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm) )
			{
				return dm.dmBitsPerPel;
			}
		}
		return -1;
	}

	/**
	 * [�ݒ�] �I�t�Z�b�g�ݒ�.
	 *		�E�B���h�E���[�h���ɁA�\������I�t�Z�b�g���w��ł��܂��B
	 *	@note �I�t�Z�b�g�ݒ��A�ĕ\�����܂��B
	 *	@param x X �I�t�Z�b�g�B
	 *	@param y Y �I�t�Z�b�g�B
	 *	@param magnificX X �{���B
	 *	@param magnificY Y �{���B
	 */
	void SetOffset(int x, int y, int magnificX, int magnificY)
	{
		m_offset.x = x;
		m_offset.y = y;
		m_magnific.x = magnificX;
		if ( m_magnific.x < 1 )
		{
			m_magnific.x = 1;
		}
		m_magnific.y = magnificY;
		if ( m_magnific.y < 1 )
		{
			m_magnific.y = 1;
		}
		Redraw();
	}

	/**
	 * [�ݒ�] �I�t�Z�b�g�ݒ�.
	 *		�E�B���h�E���[�h���ɁA�\������I�t�Z�b�g���w��ł��܂��B
	 *	@note �I�t�Z�b�g�ݒ��A�ĕ\�����܂��B
	 *	@param x X �I�t�Z�b�g�B
	 *	@param y Y �I�t�Z�b�g�B
	 *	@param magnific �{���B
	 */
	void SetOffset(int x = 0, int y = 0, int magnific = 1)
	{
		m_offset.x = x;
		m_offset.y = y;
		if ( magnific < 1 )
		{
			magnific = 1;
		}
		m_magnific.x = magnific;
		m_magnific.y = magnific;
		Redraw();
	}

	/**
	 * [�\��] �ĕ\��.
	 *	@note �E�B���h�E���[�h�̏ꍇ�̂݁A�ĕ\�����s���܂��B
	 *		�ʏ�A WM_PAINT ���ɃR�[�����܂��B
	 *	@note �t���X�N���[�����[�h�̏ꍇ�A�������܂���B
	 *	@retval true ����
	 *	@retval false ���s�B�t���X�N���[�����[�h�̏ꍇ�� false ���Ԃ�܂��B
	 */
	bool Redraw(void)
	{
		if ( m_primarySurface.IsValid() && ! m_isFull )
		{
			POINT po = { 0, 0 };
			::ClientToScreen(m_hWnd, &po);
			RECT r2 = { 0, 0, m_primarySize.cx, m_primarySize.cy };
			RECT r1 = r2;
			::OffsetRect(&r1, po.x, po.y);
			r2.right /= m_magnific.x;
			r2.bottom /= m_magnific.y;
			::OffsetRect(&r2, m_offset.x, m_offset.y);
			m_pDraw->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0); //�����҂�
//			return m_primarySurface->Blt(&r1, m_benchSurface, &r2, DDBLT_WAIT, NULL) == DD_OK; //�������̂ق�����������ʊO�ɂ����ƁA�������ȂƂ���ɕ\������Ă��܂�
			return m_primarySurface.BltClip(r1, m_benchSurface, r2) == DD_OK;			
		}
		return false;
	}

	/**
	 * [����] �t�B���b�v.
	 *	@note �t���X�N���[�����[�h�ƃE�B���h�E���[�h�œ��삪�قȂ�܂��B\n
	 *		�t���X�N���[�����[�h�̏ꍇ�A�o�b�N�T�[�t�F�X�ƃv���C�}���T�[�t�F�X�����ւ��܂��B\n
	 *		�E�B���h�E���[�h�̏ꍇ�A�o�b�N�T�[�t�F�X�̉摜���v���C�}���T�[�t�F�X�ɃR�s�[���܂��B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Flip(void)
	{
		bool r = false;
		if ( m_primarySurface.IsValid() )
		{
			if ( m_isFull )
			{
				//== �t�����[�h
				r = m_primarySurface->Flip(0, DDFLIP_WAIT) == DD_OK;
			}
			else
			{
				//== �E�B�����[�h
				r = m_benchSurface->BltFast(0, 0, m_backbufferSurface, NULL, DDBLTFAST_WAIT) == DD_OK;
				if ( r )
				{
					r = Redraw();
				}
			}
		}
		return r;
	}

	/**
	 * [����] ���������҂�.
	 *
	 */
	void WaitForVerticalBlank(void)
	{
		if ( m_pDraw != NULL )
		{
			m_pDraw->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0); //�����҂�
		}
	}

	/**
	 * [�`��] �o�b�N�o�b�t�@�N���A
	 *	@note �^�����ɂ��܂��B
	 *	@retval true ����
	 *	@retval false ���s
 	 */
	bool ClearBackbufferSurface(void)
	{
		if ( m_primarySurface.IsValid() )
		{
			return m_backbufferSurface->BltFast(0, 0, m_baseSurface, NULL, DDBLTFAST_WAIT) == DD_OK;
		}
		return false;
	}

	/**
	 * [�擾] �o�b�N�o�b�t�@�擾.
	 *	@return �o�b�N�o�b�t�@�̎Q��.
	 */
	CSurface& GetBackbufferSurface(void)
	{
		return m_backbufferSurface;
	}

	CSurface CreateWorkSurfase(int height, int width)
	{
		CSurface surf;
		DDSURFACEDESC2 ddsd = { sizeof(DDSURFACEDESC2) };
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT ;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY; 
		ddsd.dwWidth = (height > 0) ? height : m_backSize.cx;
		ddsd.dwHeight = (width > 0) ? width : m_backSize.cy;
		surf.Create(m_pDraw, &ddsd);
		return surf;
	}

private:

	/// �S�T�[�t�F�X�j��
	void m_DestroySurface(void)
	{
		m_Release(m_baseSurface);
		m_Release(m_backbufferSurface);
		m_Release(m_benchSurface);
		m_Release(m_primarySurface);
		m_Release(m_pClip);
	}

	// �T�[�t�F�X�쐬
	bool m_CreateSurface(void)
	{
		m_DestroySurface();
		ASSERT( m_pDraw != NULL );
		DDSURFACEDESC2 ddsd = { sizeof(DDSURFACEDESC2) };
		HRESULT res = DD_OK;
		if ( m_isFull )
		{
			//== �t��
			//�������x���ݒ�
			res |= m_pDraw->SetCooperativeLevel(m_hWnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
			//��ʐݒ�
			res |= m_pDraw->SetDisplayMode(m_primarySize.cx, m_primarySize.cy, m_bitsPixel, 0, 0);
			//�v���C�}���T�[�t�F�C�X�쐬
			ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
			ddsd.ddsCaps.dwCaps =  DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
			ddsd.dwBackBufferCount = 1;
			res |= m_primarySurface.Create(m_pDraw, &ddsd);
			if ( res == DD_OK )
			{
				//�o�b�N�o�b�t�@�T�[�t�F�X�쐬
				DDSCAPS2 ddscaps = { 0 };
				ddscaps.dwCaps=DDSCAPS_BACKBUFFER;
				LPDIRECTDRAWSURFACE7 pSur = NULL;
				res |= m_primarySurface->GetAttachedSurface(&ddscaps, &pSur);
				m_backbufferSurface.Attach(pSur);
			}
			ddsd.dwWidth = m_primarySize.cx;
			ddsd.dwHeight = m_primarySize.cy;
		}
		else
		{
			//== �E�B��
			//�������x���ݒ�
			m_pDraw->SetCooperativeLevel(m_hWnd, DDSCL_NORMAL);
			//��ʐݒ�
			DEVMODE dm = { sizeof(DEVMODE) };
			if ( ::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm) )
			{
				dm.dmBitsPerPel = m_bitsPixel;
				dm.dmFields = DM_BITSPERPEL;
				if ( ::ChangeDisplaySettings(&dm, CDS_FULLSCREEN/*�ꎞ�I�̈Ӗ��Ŏg��*/) != DISP_CHANGE_SUCCESSFUL )
				{
					res = DD_FALSE;
				}
			}
			else
			{
				res = DD_FALSE;
			}
			//�N���b�p�[�쐬
			res |= m_pDraw->CreateClipper(0, &m_pClip, NULL);
			if ( res == DD_OK )
			{
				res |= m_pClip->SetHWnd(0, m_hWnd);
				//�v���C�}���T�[�t�F�X�̍쐬
				ddsd.dwFlags = DDSD_CAPS;
				ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
				res |= m_primarySurface.Create(m_pDraw, &ddsd);
			}
			if ( res == DD_OK )
			{
				res |= m_primarySurface->SetClipper(m_pClip);
				//�N���b�s���O�g�X�V
				RECT nowClipRect;
				::GetClipCursor(&nowClipRect);
				::ClipCursor(NULL);
				RECT allClipRect;
				::GetClipCursor(&allClipRect);
				::ClipCursor(&nowClipRect);
				m_primarySurface.SetClipRect(allClipRect);
				//�o�b�N�T�[�t�F�X�̍쐬
				ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
				ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
				ddsd.dwWidth = m_backSize.cx;
				ddsd.dwHeight = m_backSize.cy;
				res |= m_backbufferSurface.Create(m_pDraw, &ddsd);
				//�x���`�T�[�t�F�X�̍쐬
				res |= m_benchSurface.Create(m_pDraw, &ddsd);
			}
		}
		//
		if ( res == DD_OK )
		{
			//�x�[�X�T�[�t�F�X
			ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY; 
			res |= m_baseSurface.Create(m_pDraw, &ddsd);
		}
		//
		if ( res != DD_OK )
		{
			m_DestroySurface();
			return false;
		}
		return true;
	}

	/// �|�C���^�����[�X
	template<typename TYP>
	void m_Release(TYP*& P)
	{
		if ( P != NULL )
		{
			P->Release();
			P = NULL;
		}
	}

	/// �|�C���^�����[�X
	void m_Release(CSurface& P)
	{
		P.Destroy();
	}

	HWND					m_hWnd;					///< �E�B���h�E�n���h��
	SIZE					m_primarySize;			///< �v���C�}���T�C�Y
	SIZE					m_backSize;				///< �o�b�N�o�b�t�@���T�C�Y
	int						m_bitsPixel;			///< �F�[��
	bool					m_isFull;				///< �t���X�N���[�����H
	POINT					m_offset;				///< �E�B���h�E���[�h�p(�I�t�Z�b�g)
	POINT					m_magnific;				///< �E�B���h�E���[�h�p(�{��)
	LPDIRECTDRAW7			m_pDraw;				///< DirectDraw
	CSurface				m_primarySurface;		///< �v���C�}���T�[�t�F�X
	CSurface				m_backbufferSurface;	///< �o�b�N�o�b�t�@�T�[�t�F�X(�ҏW�p)
	CSurface				m_benchSurface;			///< �x���`�T�[�t�F�X(�E�B���h�E���[�h�p)
													///< @note �E�B���h�E�̏ꍇ�A�ĕ`�悪��������̂Ńv���C�}���ɕ\�����Ă���摜�Ɠ������̂��A
													///<	���̃x���`�ɓ���Ă����悤�ɂ��Ă���B
	CSurface				m_baseSurface;			///< �x�[�X�T�[�t�F�X(�N���A�p)
													///< @note Flip������A�o�b�N�o�b�t�@���N���A���邽�߂Ɏg�p����B
													///<	�ʏ�A�^�����B
	LPDIRECTDRAWCLIPPER		m_pClip;				///< �N���b�p�[
	//
	HDC						m_hDC;					///< NULL�Ȃ�GetDC�͉\
	TBuffer					m_buffer;				///< GetBuffer�p���U���g
	LPDIRECTDRAWSURFACE7	m_pGetBuffSurface;		///< �݂��o�����̃T�[�t�F�X
};



};//DX
};//TNB


/**
  
	x2�@�ȏ�̎��A��ʊO�ɕ\���G���A������ƁA���������Ȃ�B


 */


/*
	��ʂƂ��āA���A����Ȋ����B
	
	�\�����Ă���A�u�v���C�}���T�[�t�F�X�v
	�ҏW�p�́A�u�o�b�N�o�b�t�@�T�[�t�F�X�v
	�E�B���h�E���[�h��p�ɁA�\�����e�̍T���A�u�x���`�T�[�t�F�X�v


	�o�b�N�o�b�t�@���N���A���邽�߂́A�u�x�[�X�T�[�t�F�X�v

	��`�p�^�[���������Ă����A�u�p�^�[���T�[�t�F�X�v�@�����H

	�w�i�p�^�[������ׂ�A�u�o�b�N�O���E���h�T�[�t�F�X�v�@�����H
		�X�N���[���ɑΉ��H�I






	�E�B���h�E���[�h�̏ꍇ�A�I�t�Z�b�g�\�����\�B



*/
