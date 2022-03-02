#pragma once
/**
 *@file
 * �O���t�`��R���g���[���֌W�̃w�b�_
 *	
 *		�O���t��`�悷��R���g���[���N���X������܂��B
 */



#include "TnbBitmapImage.h"
#include "TnbMfcBitmapDC.h"
#include "TnbMap.h"
#include "TnbTextDrawer.h"
#include <float.h>
#include <math.h>



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �܂���O���t�R���g���[��.
 *
 *		�ŉ��� �` 0(�) �` �ŏ�� �̏c�����������܂���O���t��\������R���g���[���ł��B
 *		�\���̏���Ɖ������w�肷�邱�Ƃ��o���܂��B
 *		���̕\���̏���������R���g���[���̏㕔�����Ƀ}�b�s���O����ĕ\������܂��B
 *		�� SetViewValueRange()
 *
 *		�����̓R���g���[���̃N���C�A���g���ȏ���w��\�ł��B
 *		���̏ꍇ�A���X�N���[���o�[���\������܂��B
 *		�� SetBackgroundWidth()
 *
 *		�܂��A�X�N���[������Ɛe�E�B���h�E�ɁA WM_HSCROLL ���b�Z�[�W����т܂��B���̍ہA
 *		WPARAM �̉��ʃ��[�h�� SB_THUMBPOSITION �Œ�A��ʃ��[�h�͌��݂̐����\���ʒu�������Ă��܂��B 
 *		LPARAM �͖{�R���g���[���̃E�B���h�E�n���h���ł��iMFC �̃n���h���̏ꍇ�A OnHScroll() �̑�O������
 *		CScrollBar* �ł����A ���̃R���g���[���̃|�C���^�������Ă܂��̂ň����ɂ͒��ӂ��Ă�������)�B
 *
 *		�����C���f�b�N�X�P�����s�N�Z���ɂ��邩�w�肷�邱�Ƃ��o���܂��B�f�t�H���g�͂P�ł��B
 *		�� SetHorizontalIndexPixel()
 *
 *		�X�P�[������\�����邱�Ƃ��o���܂��i���C�����ƃT�u���j�B���ꂼ��A�X�^�C���A�F���w��\�ł��B
 *		�� SetScaleLine() , SetSubScaleLine()
 *
 *		�C�ӂ̕������A�O���t���ɕ\�����邱�Ƃ��o���܂��i�W���̈ʒu�j�B���ꂼ��t�H���g�A�F�̎w�肪�\�B
 *		�����̕����͉��X�N���[���ɂ͉e������܂���B
 *		�� SetOverlayText()
 *
 *		���́A�F���ɊǗ�����A�A�������|�C���g����Őڑ����邱�Ƃ��\�ł��B
 *		
 *		�����C���f�b�N�X�O�́A�ʏ퍶�ł����A�E�ɂ��邱�Ƃ��\�ł��i�\�������E���]����j�B
 *		�� SetHorizontalIndexPixel()
 *
 *		�ÓI�ȃO���t�̑��A�S�̂��X�N���[�������ĘA���I�ɐV��������\�����Ă������Ƃ��\�ł��i�ŐV���ŉE���ō��ɌŒ�j�B
 *		�� Shift()
 *
 *		�܂��ADC���擾���A���R�ɕ`�悷�邱�Ƃ��\�ł��B
 *		�� GetImageDC() , ReleaseImageDC()
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�ŃX�^�e�B�b�N�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCStatic�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@par�g�p��(�Œ�̒l��\������O���t)
 *		\code
 *		// *.h
 *		class CFooDlg : public CDialog {
 *			;
 *		CLineGraphViewCtrl m_graph;
 *
 *		// *.cpp
 *		void CFooDlg::OnInitDialog(void) {
 *				;
 *			m_graph.SetHorizontalIndexPixel(5); //�����C���f�b�N�X�P�̕��� 5�s�N�Z���Ƃ���
 *			m_graph.SetBackgroundWidth(RGB(0, 0, 0), 1000); //�ŐV����ԉE �w�i�͍��B���� 1000�s�N�Z���B�����C���f�b�N�X�ł� 200�ƂȂ�B
 *			m_graph.SetPointMakertSize(1); //�|�C���g�}�[�J�[�̑傫���� 1�s�N�Z���Ƃ���B
 *			m_graph.SetViewValueRange(5000, -5000); //�c���l�̕\���͈͂� 5000 �` -5000 �Ƃ���
 *			m_graph.SetScaleLine(RGB(255, 255, 255), m_graph.LineStyle_Bold); //����͔��̑����Ƃ���B
 *			m_graph.SetSubScaleLine(1, m_graph.LineStyle_Dot, RGB(50, 50, 50), 1000); //�����P�� 1000���ɃO���[�̔j���Ƃ���B
 *			m_graph.SetSubScaleLine(2, m_graph.LineStyle_EdgeDot, RGB(50, 50, 50), 100); //�����Q�� 100���ɍ��E�ɖڈ�Ƃ���B
 *			m_graph.SetOverlayText(7, RGB(255, 255, 255), _T("�^�C�g��")); // ����ɕ����\��
 *				;
 *			loop ( i, 200 ) {
 *				int d = this->GetData(i);
 *				if ( (i % 10) == 0 ){
 *					m_graph.SetVerticalLine(RGB(100, 20, 20), i, 20); //�������ɂ������Ⴂ�c��
 *				}
 *				m_graph.SetPoint(RGB(255, 0, 0), d, i);
 *			}
 *			m_graph.Settle();
 *		}
 *		\endcode
 *	
 *	@par�g�p��(���I�ɕω���������O���t)
 *		\code
 *		// *.h
 *		class CFooDlg : public CDialog {
 *			;
 *		CLineGraphViewCtrl m_graph;
 *
 *		// *.cpp
 *		void CFooDlg::OnInitDialog(void) {
 *				;
 *			m_graph.SetHorizontalIndexPixel(5, true); //�����C���f�b�N�X�P�̕��� 5�s�N�Z���Ƃ���@�ŐV����ԉE 
 *			m_graph.SetBackgroundWidth(RGB(0, 0, 0)); //�w�i�͍�
 *			m_graph.SetPointMakertSize(3); //�|�C���g�}�[�J�[�̑傫���� 3�s�N�Z���Ƃ���B
 *			m_graph.SetViewValueRange(5000, -10); //�c���l�̕\���͈͂� 5000 �` -10 �Ƃ���B
 *			m_graph.SetScaleLine(RGB(255, 255, 255), m_graph.LineStyle_Bold); //����͔��̑����Ƃ���B
 *			m_graph.SetSubScaleLine(1, m_graph.LineStyle_Dot, RGB(50, 50, 50), 100); //������ 100���ɃO���[�̔j���Ƃ���B
 *			m_graph.SetOverlayText(7, RGB(255, 255, 255), _T("�^�C�g��")); // ����ɕ����\���B
 *				;
 *
 *		// �f�[�^��M
 *		void CFooDlg::OnReceivedData(int data) {
 *			// ��M�����f�[�^���O���t�̉E�[�ɒǉ����Ă����B���ɍs���ɂ�A�Â��f�[�^�ɂȂ�B
 *			m_graph.Shift(); // �܂��A�O���t���E�Ɉ�V�t�g�B
 *			m_graph.SetPoint(RGB(255, 0, 0), data);  //��ԉE�Ƀ|�C���g�}�[�J�[��ł�
 *			m_graph.SetOverlayText(1, RGB(155, 155, 155), CStr::Fmt(_T("%d"), data); // �����ɕ����\��
 *			m_graph.Settle(); // �m��
 *		}
 *		\endcode
 *
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcLineGraphViewCtrl.h
 * 
 *	@date 11/05/23 �V�K�쐬
 *	@date 11/06/15 ��ʂ��v���X���Ĉړ����鏈�����C���B
 *	@date 11/08/02 ClearPoint() �ǉ��B
 */
class CLineGraphViewCtrl : public CWnd, CCopyImpossible
{
	DEFSUPER(CWnd);
public:

	/// �萔
	enum
	{
		EDGE_VALUE = 10000,				///< �f�t�H���g�̏㉺�[�̃|�C���g�l
		INTERNALLY_COLOR = RGB(1, 1, 1)	///< ���������p�J���[
	};

	/// ���C���X�^�C��
	enum ELineStyle
	{
		LineStyle_Non,		///< ��\��
		LineStyle_Solid,	///< ����
		LineStyle_Dot,		///< �j��
		LineStyle_Bold,		///< ����
		LineStyle_EdgeDot,	///< ���[�̂�
	};

	/// �R���X�g���N�^.
	CLineGraphViewCtrl(void) : m_isReverseIndexMode(false), m_viewHorizontalPos(0), m_markerWidth(6), m_scrollOneStep(1)
		, m_viewTopValue(EDGE_VALUE), m_viewbottomValue(-EDGE_VALUE), m_scrollBarHeight(::GetSystemMetrics(SM_CYHSCROLL))
		, m_backColor(RGB(0, 0, 0)), m_isGraphImageDcLeading(false), m_isOndemandRemakeViewImage(false), m_horzIndexPixel(1)
		, m_isMouseButtonPressed(false), m_isScrollBarUsed(true)
	{
		m_viewHeight = m_viewTopValue - m_viewbottomValue + 0.0; //�\���͈�
	}

	/**
	 * [�ݒ�] �O���t�w�i���ݒ�.
	 *	@note SetPoint() �Ń|�C���g��`���A�ł��̃��\�b�h���R�[������ƁA�|�C���g���N���A����܂��B
	 *	@param backgroundColor �w�i�F
	 *	@param graphWidth �O���t�̃s�N�Z�������w�肵�܂��B�N���C�A���g�����傫���ꍇ�A�X�N���[���o�[���\������܂��B
	 *					�ȗ����邩0���w�肷��ƁA�N���C�A���g���Ɠ����ɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetBackgroundWidth(COLORREF backgroundColor, size_t graphWidth = 0)
	{
		if ( ::IsWindow(GetSafeHwnd()) )
		{
			CRect rc;
			GetClientRect(&rc);
			CSize sz = rc.Size();
			m_backColor = backgroundColor;
			if ( graphWidth == 0 )
			{
				graphWidth = sz.cx;
			}
			if ( sz.cx == 0 || sz.cy == 0 )
			{
				return false;
			}
			m_Resize(sz, graphWidth);
			SetViewHorizontalPos(0);
			return true;
		}
		return false;
	}

	/**
	 * [�ݒ�] �C���f�b�N�X�s�N�Z���ݒ�.
	 *		�����C���f�b�N�X�P�����s�N�Z�������w��ł��܂��B�ʏ�P�ł����A�Ⴆ�΁A�|�C���g�}�[�J�[�� 3 �s�N�Z�����ɂ�����A
	 *		������ 3 �s�N�Z���ȏ゠���Ȃ��ƃ}�[�J�[�����Ԃ��Ă��܂��܂��B
	 *		�����ł��̃K�C�h�ݒ�����邱�ƂŁA�����C���f�b�N�X�w�莞�Ɉӎ����邱�ƂȂ��{�P���邾���ł����悤�ɂȂ�܂��B
	 *		SetPoint(), SetVerticalLine() , Shift() , SetViewHorizontalPos() �ɉe��������܂��B
	 *	\code
	 *		m_graph.SetPointMakertSize(3); //�|�C���g�}�[�J�[�̑傫���� 3�s�N�Z���Ƃ���B
	 *		m_graph.SetHorizontalIndexPixel(5); //�����C���f�b�N�X�P�̕��� 5�s�N�Z���Ƃ���
	 *				;
	 *		m_graph.SetPoint(color, 100, 0);	//���̓�_�͉����C���f�b�N�X���P�������Ȃ����A
	 *		m_graph.SetPoint(color, 100, 1);	//�@�Ԃ� 5 �s�N�Z���ɂȂ�B
	 *	\endcode
	 *	@note SetPoint() �Ń|�C���g��`���A�ł��̃��\�b�h���R�[������ƁA�|�C���g���N���A����܂��B
	 *	@param pixel �����C���f�b�N�X�P�����s�N�Z�������w��ł��܂��B
	 *	@param isReverseIndexMode �ȗ��� false �Ȃ牡���C���f�b�N�X�O����ԍ��A true �Ȃ��ԉE�ɂȂ�܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetHorizontalIndexPixel(int pixel = 1, bool isReverseIndexMode = false)
	{
		if ( ::IsWindow(GetSafeHwnd()) )
		{
			m_horzIndexPixel = pixel;
			if ( m_horzIndexPixel <= 0 )
			{
				m_horzIndexPixel = 1;
			}
			m_isReverseIndexMode = isReverseIndexMode;
			m_Resize();
			return true;
		}
		return false;
	}
	
	/**
	 * [�擾] �C���f�b�N�X�s�N�Z���擾.
	 *	@note SetHorizontalIndexPixel() �ݒ肵���l���Ԃ�܂��B
	 *	@return �C���f�b�N�X�s�N�Z��
	 */
	int GetHorizontalIndexPixel(void) const
	{
		return m_horzIndexPixel;
	}

	/**
	 * [�ݒ�] �|�C���g�}�[�J�[�T�C�Y�ݒ�.
	 *	@note SetPoint() �̒��O�ɕς��Ă��Ή��ł��܂��B
	 *	@param width ��
	 *	@return �ݒ�O�̒l
	 */
	size_t SetPointMakertSize(size_t width)
	{
		size_t r = m_markerWidth;
		m_markerWidth = width;
		return r;
	}

	/**
	 * [�擾] �����\���͈͎擾.
	 *	@param[out] _topValue �ŏ�̏c���l
	 *	@param[out] _bottomValue �ŉ��̏c���l
	 */
	void GetViewValueRange(double& _topValue, double& _bottomValue) const
	{
		_topValue = m_viewTopValue;
		_bottomValue = m_viewbottomValue;
	}

	/**
	 * [�ݒ�] �����\���͈͐ݒ�.
	 *		�\���͈͂��w�肵�܂��B�w�肪�����ꍇ�A�ŏオ 10000�A�ŉ��� -10000 �ɂȂ�܂��B
	 *	@note SetPoint() �Ń|�C���g��`���A�ł��̃��\�b�h���R�[������ƁA�|�C���g���N���A����܂��B
	 *	@note �c���̏�������́uint �ŕ\�����Ƃ̏o����l / �N���C�A���g�̍���(pixel)�v�ɂȂ�܂��B
	 *	@param topValue �ŏ�̏c���l
	 *	@param bottomValue �ŉ��̏c���l
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetViewValueRange(double topValue, double bottomValue)
	{
		if ( ::IsWindow(GetSafeHwnd()) )
		{
			if ( topValue < bottomValue )
			{
				Swap(topValue, bottomValue);
			}
			if ( m_viewTopValue == topValue && m_viewbottomValue == bottomValue )
			{
				return true;
			}
			m_viewTopValue = topValue;
			m_viewbottomValue = bottomValue;
			m_viewHeight = m_viewTopValue - m_viewbottomValue + 0.0; //�\���͈�
			//
			const SIZE& sz = m_graphImage.GetSize();
			if ( m_graphImageDc.GetSafeHdc() != NULL )
			{
				m_graphImageDc.FillSolidRect(0, 0, sz.cx, sz.cy, INTERNALLY_COLOR);
			}
			m_pointMap.RemoveAll();
			return true;
		}
		return false;
	}
	
	/**
	 * [�ݒ�] �X�N���[���o�[����.
	 *	@note SetPoint() �Ń|�C���g��`���A�ł��̃��\�b�h���R�[������ƁA�|�C���g���N���A����܂��B
	 *	@param height �����i�P�ʃs�N�Z���j�B�O�Ȃ�V�X�e���̍����ɂȂ�܂��B�}�C�i�X�Ȃ�A�X�N���[���o�[���o���Ȃ��悤�ɂ��܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetScrollBarHeight(int height = 0)
	{
		if ( ::IsWindow(GetSafeHwnd()) )
		{
			m_scrollBarHeight = height;
			if ( height <= 0 )
			{
				m_scrollBarHeight = ::GetSystemMetrics(SM_CYHSCROLL);
			}
			m_isScrollBarUsed = (height >= 0);
			m_Resize();
			return true;
		}
		return false;
	}


	/**
	 * [�ݒ�] �X�N���[���o�[�P�X�e�b�v��.
	 *		�f�t�H���g�ł� 1 �ł��B
	 *	@param os ��
	 */
	void SetScrollOneStep(size_t os = 1)
	{
		m_scrollOneStep = os;
	}

	/**
	 * [�ݒ�] �X�P�[�����ݒ�.
	 *		�c���l�O�̈ʒu�ɕ\���������̐ݒ�����܂��B
	 *	@param color �F
	 *	@param style �X�^�C��
	 */
	void SetScaleLine(COLORREF color, ELineStyle style = LineStyle_Solid)
	{
		m_scaleLine.Set(style, color, 0.0);
	}

	/**
	 * [�ݒ�] �T�u�X�P�[�����ݒ�.
	 *		�w��̃X�e�b�v���Ƃɕ\���������̐ݒ�����܂��B�ő�R�w��ł��܂��B
	 *	@param number �T�u�X�P�[���ԍ� 1�`3 ���w��ł��܂��B
	 *	@param style �X�^�C��
	 *	@param color �F
	 *	@param step �X�e�b�v�B �\���̏c���l�� -10000 �` 10000 �̏ꍇ�A1000 ���w�肷��ƃX�P�[�����̏㉺�ɂ��ꂼ��A10�{������܂��B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetSubScaleLine(INDEX number, ELineStyle style = LineStyle_Non, COLORREF color = 0, double step = 0.0)
	{
		if ( number >= 1 && number <= 3 )
		{
			TScaleLineInfo& I = m_subScaleLines[number - 1];
			if ( step <= 0.0 )
			{
				style = LineStyle_Non;
			}
			I.Set(style, color, step);
			return true;
		}
		return false;
	}

	/**
	 * [�ݒ�] ������ݒ�.
	 *		�O���t�̏�ɕ\������镶����ݒ肵�܂��B��x�ݒ肵����A�������܂ŕ\������܂��B
	 *	@param pos �ʒu������ 1 �` 9�i�e���L�[�̕��сj�B
	 *	@param color �����̐F�B�ȗ�����ƁA�\���������ɂȂ�܂��B
	 *	@param lpsz �����B
	 *	@param hFont �t�H���g�B�{�N���X�Ŏg�p���I���܂ŁA�j�����Ȃ��ł��������B
	 *			 �ȗ�����ƁA�R���g���[���ɐݒ肳��Ă���t�H���g���g�p�����B
	 *	@retval true �����B
	 *	@retval false ���s�B
	 */
	bool SetOverlayText(INDEX pos, COLORREF color = RGB(0, 0, 0), LPCTSTR lpsz = NULL, HFONT hFont = NULL)
	{
		if ( pos > 0 && pos < 10 )
		{
			TTextInfo& t = m_testInfo[pos];
			t.hFont =hFont;
			t.text = lpsz;
			t.color = color;
			return true;
		}
		return false;
	}

	/**
	 * [�m�F] �t�C���f�b�N�X���[�h�H.
	 *		���C���f�b�N�X���[�h�͍ō��������C���f�b�N�X�O�B�t�C���f�b�N�X���[�h�͉����C���f�b�N�X�O���ŉE�ł��B
	 *	@retval true �t�C���f�b�N�X���[�h
	 *	@retval false ���C���f�b�N�X���[�h
	 */
	bool IsReverseHorizontalIndexMode(void) const
	{
		return m_isReverseIndexMode;
	}

	/**
	 * [�m�F] �X�N���[���o�[����H.
	 *	@note SetBackgroundWidth() �̃O���t�̕��ɂ��A�X�N���[���o�[������܂��B���̊m�F���o���܂��B 
	 *	@retval true ����
	 *	@retval false �Ȃ�
	 */
	bool HasScrollBar(void) const
	{
		return !! ::IsWindow(m_scrollBar);
	}

	/**
	 * [�擾] �O���t�C���[�W DC �擾.
	 *		�O���t�̏�����Ă���C���[�W�� DC ���擾�ł��܂��B
	 *		DC �̃T�C�Y�� GetImageSize() �œ����܂��B
	 *		�`���́A ReleaseImageDC() �����s����K�v������܂��B
	 *	@retval NULL ���s.
	 *	@retval NULL�ȊO �����B�l�� CDC �̃|�C���^.
	 */
	CDC* GetImageDC(void)
	{
		if ( m_isGraphImageDcLeading || m_graphImageDc.GetSafeHdc() == NULL )
		{
			return NULL;
		}
		m_isGraphImageDcLeading = true;
		return &m_graphImageDc;
	}

	/**
	 * [�ݒ�] �O���t�C���[�W DC �ԋp.
	 *		GetImageDC() �Ŏ擾���� DC ��ԋp���܂��B
	 */
	void ReleaseImageDC(void)
	{
		if ( m_isGraphImageDcLeading )
		{
			m_isGraphImageDcLeading = false;
		}
	}

	/**
	 * [�擾] �O���t�C���[�W�T�C�Y�擾.
	 *		�O���t�̏�����Ă���C���[�W�̃T�C�Y���擾�ł��܂��B
	 *	@return �T�C�Y
	 */
	const SIZE& GetImageSize(void) const
	{
		return m_graphImage.GetSize();
	}

	/**
	 * [�擾] �����C���f�b�N�X to X���W.
	 *		�����C���f�b�N�X�� GetImageDC() �ŕ`�悷�邽�߂� X ���W�ɕϊ����܂��B
	 *	@param horizontalIndex �����C���f�b�N�X.
	 *	@return X ���W
	 */
	int HorizontalIndexToX(INDEX horizontalIndex)
	{
		int xx = ToInt(horizontalIndex * m_horzIndexPixel + m_horzIndexPixel / 2);
		if ( m_isReverseIndexMode )
		{
			xx = (m_graphImage.GetSize().cx - 1) - xx;
		}
		return xx;
	}

	/**
	 * [�ړ�] �V�t�g.
	 *		�w�萔�����A�O���t���V�t�g���܂��B
	 *	@note �O���t�S�̂��V�t�g���܂��B
	 *		�i���C���f�b�N�X���[�h�Ȃ�A�E�ɃV�t�g���܂��j
	 *	@param count �V�t�g���B�ȗ�����ƂP�ł��B
	 */
	void Shift(int count = 1)
	{
		if ( m_graphImageDc.GetSafeHdc() != NULL )
		{
			const SIZE& sz = m_graphImage.GetSize();
			count *= m_horzIndexPixel;
			if ( m_isReverseIndexMode )
			{
				count = -count;
			}
			m_graphImageDc.ScrollDC(count, 0, NULL, NULL, NULL, NULL);
			if ( count > 0 )
			{
				m_graphImageDc.FillSolidRect(0, 0, count, sz.cy, INTERNALLY_COLOR);
			}
			else if ( count < 0 )
			{
				m_graphImageDc.FillSolidRect(sz.cx + count, 0, -count, sz.cy, INTERNALLY_COLOR);
			}
			loop ( i, m_pointMap )
			{
				m_pointMap.Ref(i).second->point.x += count;
			}
		}
	}

	/**
	 * [�擾] �����ő�C���f�b�N�X�擾.
	 *		SetPoint() �� SetVerticalLine() �Ɏw��ł��鉡���C���f�b�N�X�̍ő���擾�ł��܂��B
	 *	@note GetViewHorizontalPosLimit() �Ƃ͒l���قȂ�܂��B
	 *	@return �ő�C���f�b�N�X.
	 */
	INDEX GetHorizontalIndexLimit(void) const
	{
		return m_graphImage.GetSize().cx / m_horzIndexPixel;
	}

	/**
	 * [�ݒ�] �|�C���g�N���A.
	 *		SetPoint() �Ŏw�肵���|�C���g�}�[�J�[��S�ăN���A���܂��B
	 */
	void ClearPoint(void)
	{
		if ( m_graphImageDc.GetSafeHdc() != NULL )
		{
			const SIZE& sz = m_graphImage.GetSize();
			m_graphImageDc.FillSolidRect(0, 0, sz.cx, sz.cy, INTERNALLY_COLOR);
			m_pointMap.RemoveAll();
		}
	}

	/**
	 * [�ݒ�] �|�C���g�ݒ�.
	 *		�w��̉����C���f�b�N�X�A�c���l�̈ʒu�Ɏw��̐F�Ń|�C���g�}�[�J�[���`����܂��B�܂��A��O�̃|�C���g�ƘA������������܂��B
	 *	@note �F���L�[�ƂȂ�A�A������������܂��B
	 *	@note �F�� INTERNALLY_COLOR �͎w�肵�Ȃ��ł��������B
	 *	@param color �F�B
	 *	@param value �c���l�B 
	 *	@param horizontalIndex �����C���f�b�N�X�B���C���f�b�N�X���[�h�Ȃ� 0�@����ԍ��ł��B
	 *	@param withContinuousLine true �Ȃ�A�����������܂��B false �Ȃ�����܂���B
	 */
	void SetPoint(COLORREF color, double value, INDEX horizontalIndex = 0, bool withContinuousLine = true)
	{
		if ( m_graphImageDc.GetSafeHdc() != NULL )
		{
			int xx = HorizontalIndexToX(horizontalIndex);
			m_SetMaker(m_graphImageDc, xx, value, color, withContinuousLine);
		}
	}

	/**
	 * [�ݒ�] �c�X�P�[���ݒ�.
	 *		�w��� index �Ɏw��̐F�ŏc���̃X�P�[���������܂��B
	 *	@note �F�� INTERNALLY_COLOR �͎w�肵�Ȃ��ł��������B
	 *	@param color �F�B
	 *	@param horizontalIndex �����C���f�b�N�X�B���C���f�b�N�X���[�h�Ȃ� 0�@����ԍ��ł��B
	 *	@param length ���S������̏㉺���ꂼ��̒���(pixel)�B-1 �Ȃ�ォ�牺�܂ŏ�����܂��B
	 */
	void SetVerticalLine(COLORREF color, INDEX horizontalIndex, int length = -1)
	{
		if ( m_graphImageDc.GetSafeHdc() != NULL )
		{
			int xx = HorizontalIndexToX(horizontalIndex);
			CPen pen((length != -2)  ? PS_SOLID : PS_DASH, 1, color);
			CPen* pOldPen = m_graphImageDc.SelectObject(&pen);
			if ( length <= 0 )
			{
				m_graphImageDc.MoveTo(xx, 0);
				m_graphImageDc.LineTo(xx, m_clientSize.cy);
			}
			else
			{
				int yy = m_CalcDrawY(0.0);
				m_graphImageDc.MoveTo(xx, yy - length);
				m_graphImageDc.LineTo(xx, yy + length);
			}
			m_graphImageDc.SelectObject(pOldPen);
		}
	}

	/**
	 * [�ݒ�] �m��.
	 *		�ݒ�����m�肵�A��ʂɔ��f�����܂��B
	 *	@note SetPoint() , SetVerticalLine() �� SetOverlayText() �Ȃǎ��s��A������R�[�����Ȃ��ƁA��ʂɔ��f����܂���B
	 *	@param isForce true ���w�肷��ƁA�����ɕ`��p�C���[�W���쐬���܂��B�ʏ�A �ȗ��� false ���w�肵�܂��i�K�v�Ȏ��ɍ쐬����j�B
	 */
	void Settle(bool isForce = false)
	{
		if ( isForce )
		{
			m_MakeViewImage();
		}
		else
		{
			OndemandRemakeViewImage();
		}
		RedrawWindow();
	}

	/**
	 * [�擾] �����\���ő�ʒu�擾.
	 *		SetViewHorizontalPos() �ɓn����ő�l���擾�ł��܂��B
	 *	@return �ő�l 
	 */
	size_t GetViewHorizontalPosLimit(void)
	{
		return m_scrollBar.GetScrollLimit();
	}

	/**
	 * [�擾] �����\���ʒu�擾.
	 *	@return �\���ʒu.
	 */
	INDEX GetViewHorizontalPos(void) const
	{
		return m_viewHorizontalPos / m_horzIndexPixel;
	}

	/**
	 * [�ݒ�] �����\���ʒu�ݒ�.
	 *	@note ���[�ɕ\�������������C���f�b�N�X���w�肵�܂��i�t�C���f�b�N�X���[�h�Ȃ�E�[�j�B
	 *	@param pos �\���ʒu�B
	 */
	void SetViewHorizontalPos(INDEX pos)
	{
		if ( ::IsWindow(m_scrollBar) )
		{
			INDEX xx = pos;
			if ( m_isReverseIndexMode )
			{
				xx = m_scrollBar.GetScrollLimit() - xx;
			}
			m_scrollBar.SetScrollPos(ToInt(xx));
			xx = m_scrollBar.GetScrollPos();
			if ( m_isReverseIndexMode )
			{
				xx = m_scrollBar.GetScrollLimit() - xx;
			}
			m_viewHorizontalPos = xx * m_horzIndexPixel;
			OndemandRemakeViewImage();
			RedrawWindow();
		}
	}

protected:

	/**
	 * [�ʒm] �č\�z�v���ʒm.
	 */
	void OndemandRemakeViewImage(void)
	{
		m_isOndemandRemakeViewImage = true;
	}

	/**
	 * [�ʒm] �\���p�C���[�W�쐬�ʒm.
	 *		�\���p�̃C���[�W���쐬��ɒʒm����܂��B
	 *		�n���ꂽ pDC �ɕ`�悷�邱�Ƃ��\�ł��B
	 *	@param pDC �f�o�C�X�R���e�L�X�g
	 *	@param sz �f�o�C�X�R���e�L�X�g�̑傫��
	 */
	virtual void OnMadeViewImage(CDC* pDC, const SIZE& sz)
	{
	}

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::PreSubclassWindow();
		CRect rc;
		GetClientRect(&rc);
		m_clientSize = rc.Size();
		const SIZE& sz = m_clientSize;
		m_viewImage.Set(sz.cx, sz.cy, m_backColor);
		m_graphImageDc.Detach();
		m_graphImage.Set(sz.cx, sz.cy, INTERNALLY_COLOR);
		m_graphImageDc.Attach(&m_graphImage);
	}

	/**
	 * [�ʒm] for processing Windows messages.
	 *		���b�Z�[�W��M������R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 *	@param message ���b�Z�[�W
	 *	@param wParam WPARAM
	 *	@param lParam LPARAM
	 *	@return ���U���g�B
	 */
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch ( message )
		{
		case WM_ERASEBKGND:
			return 0;
		case WM_PRINTCLIENT:
			if ( m_isOndemandRemakeViewImage )
			{
				m_isOndemandRemakeViewImage = false;
				m_MakeViewImage();
			}
			m_viewImage.Draw(reinterpret_cast<HDC>(wParam));
			return 0;
		case WM_PAINT:
			if ( m_isOndemandRemakeViewImage )
			{
				m_isOndemandRemakeViewImage = false;
				m_MakeViewImage();
			}
			m_viewImage.Draw(CPaintDC(this));
			return 0;
		case WM_ENABLE:
			return 0;
		case WM_SIZE:
			{
				CSize sz(LOWORD(lParam), HIWORD(lParam));
				if ( sz != m_clientSize )
				{
					CSize gsz = m_graphImage.GetSize();
					m_Resize(sz, gsz.cx);
					OndemandRemakeViewImage();
				}
			}
			break;
		case WM_HSCROLL:
			if ( reinterpret_cast<HWND>(lParam) == m_scrollBar )
			{
				int sbCode = LOWORD(wParam);
				int nPos = HIWORD(wParam);
				switch ( sbCode )
				{
				case SB_THUMBPOSITION: // ��Έʒu�փX�N���[���B���݈ʒu�̓p�����[�^ nPos �Ŏw�肳��܂��B
				case SB_THUMBTRACK: // �X�N���[�� �{�b�N�X���w��ʒu�փh���b�O�B���݈ʒu�̓p�����[�^ nPos �Ŏw�肳��܂��B
					{
						SCROLLINFO si;
						if ( m_scrollBar.GetScrollInfo(&si, SIF_TRACKPOS) )
						{
							nPos = si.nTrackPos;
						}
					}
					break;
				}
				m_OnScrollBar(sbCode, nPos);
			}
			break;
		case WM_LBUTTONDOWN:
			if ( ::IsWindow(m_scrollBar) )
			{
				m_isMouseButtonPressed = true;
				SetCapture();
				m_pressedPos = CPoint(lParam);
			}
			#ifdef _WIN32_WCE
				return _super::DefWindowProc(message, wParam, lParam);
			#endif
			break;
		case WM_LBUTTONUP:
			if ( m_isMouseButtonPressed )
			{
				m_isMouseButtonPressed = false;
				ReleaseCapture();
			}
			break;
		case WM_MOUSEMOVE:
			if ( m_isMouseButtonPressed )
			{
				if ( GetCapture() != this )
				{
					m_isMouseButtonPressed = false;
				}
				else
				{
					CPoint pos(lParam);
					int d = (pos.x - m_pressedPos.x) / m_horzIndexPixel;
					if ( d != 0 )
					{
						m_OnScrollBar(SB_THUMBPOSITION, m_scrollBar.GetScrollPos() - d);
						m_pressedPos = pos;
						Invalidate();
						UpdateWindow();
					}
				}
			}
			break;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

private:
	/// �e�L�X�g���
	struct TTextInfo
	{
		HFONT		hFont;	///< �t�H���g
		CStr		text;	///< ������
		COLORREF	color;	///< �F
		//�R���X�g���N�^
		TTextInfo(void) : hFont(NULL)
		{
		}
	};
	/// �����
	struct TLineInfo : CCopyImpossible
	{
		ELineStyle	style;		///< �����A�j���A�����A�E�̂݁A���̂�
		CPen		colorPen;	///< �F�̃y��
		//�R���X�g���N�^
		TLineInfo(void) : style(LineStyle_Non)
		{
		}
		void Set(ELineStyle	ls, COLORREF color)
		{
			style = ls;
			if ( colorPen.GetSafeHandle() != NULL )
			{
				colorPen.DeleteObject();
			}
			CPen* pPen = NULL;
			switch ( style )
			{
			case LineStyle_Solid:	// ����
			case LineStyle_EdgeDot: // ���[�̂�
				pPen = new CPen(PS_SOLID, 1, color);
				break;
			case LineStyle_Bold:	// ����
				pPen = new CPen(PS_SOLID, 3, color);
				break;
			case LineStyle_Dot:	// �j��
				pPen = new CPen(PS_DOT, 1, color);
				break;
			}
			if ( pPen != NULL )
			{
				colorPen.Attach(pPen->Detach());
				delete pPen;
			}
		}
	};
	/// �X�P�[�������
	struct TScaleLineInfo : TLineInfo
	{
		double			step;	///< �X�e�b�v(�O�𒆐S�Ƃ��Ă��������ɐ����������B�}�C�i�X�Ȃ�Ȃ�)�B
		void Set(ELineStyle	ls, COLORREF color, double sp)
		{
			step = sp;
			TLineInfo::Set(ls, color);
		}
	};
	/// �X�N���[���o�[
	class CMyScrollBar : public CScrollBar
	{
		DEFSUPER(CScrollBar);
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
		{
			if ( message == WM_LBUTTONDOWN )
			{
				return _super::DefWindowProc(message, wParam, lParam);
			}
			return _super::WindowProc(message, wParam, lParam);
		}
	};

	int m_CalcDrawY(double y) const
	{	
		double yy = (m_viewTopValue - y);
		yy *= (m_clientSize.cy - 1);
		yy /= m_viewHeight;
		return down_cast<int>(yy);
	}

	/// �X�N���[�����b�Z�[�W����
	void m_OnScrollBar(int sbCode, int nPos)
	{
		INT_PTR np = GetViewHorizontalPos();
		INT_PTR maxPos = m_scrollBar.GetScrollLimit();
		INT_PTR p = m_scrollBar.GetScrollPos();
		switch ( sbCode )
		{
		case SB_LEFT: // ���[�փX�N���[���B
			p = 0;
			break;
		case SB_ENDSCROLL: // �X�N���[���I���B
			return;
		case SB_LINELEFT: // ���փX�N���[���B
			p -= m_scrollOneStep;
			break;
		case SB_LINERIGHT: // �E�փX�N���[���B
			p += m_scrollOneStep;
			break;
		case SB_PAGELEFT: // 1 �y�[�W���փX�N���[���B
			p -= m_clientSize.cx / m_horzIndexPixel;
			break;
		case SB_PAGERIGHT: // 1 �y�[�W�E�փX�N���[���B
			p += m_clientSize.cx / m_horzIndexPixel;
			break;
		case SB_RIGHT: // �E�[�փX�N���[���B
			p = maxPos;
			break;
		case SB_THUMBPOSITION: // ��Έʒu�փX�N���[���B���݈ʒu�̓p�����[�^ nPos �Ŏw�肳��܂��B
		case SB_THUMBTRACK: // �X�N���[�� �{�b�N�X���w��ʒu�փh���b�O�B���݈ʒu�̓p�����[�^ nPos �Ŏw�肳��܂��B
			p = nPos;
			break;
		default:
			ASSERT( false );
			break;
		}
		m_scrollBar.SetScrollPos(ToInt(p));
		p = m_scrollBar.GetScrollPos();
		if ( m_isReverseIndexMode )
		{
			p = m_scrollBar.GetScrollLimit() - p;
		}
		if ( np != p )
		{
			SetViewHorizontalPos(p);
			CWnd* pParent = GetParent();
			if ( pParent != NULL )
			{
				pParent->SendMessage(WM_HSCROLL, MAKELONG(SB_THUMBPOSITION, p), reinterpret_cast<LPARAM>(GetSafeHwnd()));
			}
		}
	}
	/**
	 * �T�C�Y�ύX.
	 *	@param size �R���g���[���̃N���C�A���g�̈�T�C�Y
	 *	@param graphWidht �O���t�̃T�C�Y�B�N���C�A���g���傫���Ă� OK�B
	 */
	void m_Resize(const SIZE& sz, size_t graphWidth)
	{
		if ( sz.cx < 0 || sz.cx > 0x7FFF || sz.cy < 0 || sz.cy > 0x7FFF )
		{
			return;
		}
		m_clientSize = sz;
		//== �X�N���[���o�[������
		if ( ::IsWindow(m_scrollBar) )
		{
			m_scrollBar.DestroyWindow();
		}
		if ( ToInt(graphWidth) > m_clientSize.cx )
		{
			//== �X�N���[���o�[�p��
			DWORD style = SBS_HORZ | WS_CHILD;
			if ( m_isScrollBarUsed )
			{
				m_clientSize.cy -= ToInt(m_scrollBarHeight);
				style |= WS_VISIBLE;
			}
			m_scrollBar.Create(style, CRect(0, m_clientSize.cy, m_clientSize.cx, ToInt(m_clientSize.cy + m_scrollBarHeight)), this, 1000);
			SCROLLINFO si;
			m_scrollBar.GetScrollInfo(&si);
			si.nPage = m_clientSize.cx / m_horzIndexPixel;
			si.nMin = 0;
			si.nMax = ToInt(graphWidth / m_horzIndexPixel);
			if ( m_isReverseIndexMode )
			{
				si.nPos = si.nMax;
			}
			m_scrollBar.SetScrollInfo(&si);
		}
		m_viewImage.Set(sz.cx, sz.cy, m_backColor);
		m_graphImageDc.Detach();
		if ( sz.cx > 0 && sz.cy > 0 && m_clientSize.cy > 0 )
		{
			VERIFY( m_graphImage.Set(ToInt(graphWidth), m_clientSize.cy, INTERNALLY_COLOR) );
			m_graphImageDc.Attach(&m_graphImage);
		}
		m_pointMap.RemoveAll();
	}
	void m_Resize(void)
	{
		const CSize& gsz = m_graphImage.GetSize();
		CRect rc;
		GetClientRect(&rc);
		m_Resize(rc.Size(), gsz.cx);
	}
	/**
	 * �}�[�J�[�`��
	 *	@param dc DC
	 *	@param xx �����W�B
	 *	@param y �c���l
	 *	@param color �F
	 *	@param withContinuousLine �A�������ꏏ�H
	 */
	void m_SetMaker(CDC& dc, int xx, double y, COLORREF color, bool withContinuousLine)
	{
		if ( ! IS_RGBVALUE(color) )
		{
			return;
		}
		if ( ! ::_finite(y) )
		{
			return;
		}
		int yy = m_CalcDrawY(y);
		bool hasColorKey = m_pointMap.HasKey(color);
		//== �u���V�A�y���A�쐬
		if ( ! hasColorKey )
		{
			m_pointMap[color] = new TPointInfo(color);
		}
		TPointInfo* P = m_pointMap[color];
		ASSERT( P != NULL );
		CPen* pOldPen = dc.SelectObject(&P->pen);
		{
			//== �A�����`��
			if ( withContinuousLine && hasColorKey )
			{
				dc.MoveTo(P->point);
				dc.LineTo(xx, yy);
			}
			//== �}�[�J�[�`��
			if ( m_markerWidth <= 1 )
			{
				dc.SetPixel(xx, yy, color);
			}
			else if ( m_markerWidth <= 3 )
			{
				int mw = ToInt(m_markerWidth);
				int d1 = mw / 2;
				dc.FillSolidRect(xx - d1, yy - d1, mw, mw, color);
			}
			else
			{
				CBrush* pOldBrush = dc.SelectObject(&P->brush);
				int d1 = ToInt(m_markerWidth / 2);
				int d2 = ToInt(m_markerWidth - d1);
				dc.Ellipse(xx - d1, yy - d1, xx + d2, yy + d2);
				dc.SelectObject(pOldBrush);
			}
		}
		dc.SelectObject(pOldPen);
		//== �L��
		P->point.x = xx;
		P->point.y = yy;
	}
	/**
	 * [�v�Z] �e���L�[�ʒu����DrawText�ʒu������
	 *	@param �e���L�[�ʒu
	 *	@return DrawText�ʒu
	 */
	DWORD m_ToDtStyle(INDEX idx) const
	{
		DWORD r = 0;
		switch ( idx )
		{
		case 1:	r = DT_LEFT		| DT_BOTTOM;	break;
		case 2:	r = DT_CENTER	| DT_BOTTOM;	break;
		case 3:	r = DT_RIGHT	| DT_BOTTOM;	break;
		case 4:	r = DT_LEFT		| DT_VCENTER;	break;
		case 5:	r = DT_CENTER	| DT_VCENTER;	break;
		case 6:	r = DT_RIGHT	| DT_VCENTER;	break;
		case 7:	r = DT_LEFT		| DT_TOP;		break;
		case 8:	r = DT_CENTER	| DT_TOP;		break;
		case 9:	r = DT_RIGHT	| DT_TOP;		break;
		}
		return r;
	}
	// �č\�z
	void m_MakeViewImage(void)
	{
		const SIZE& sz = m_clientSize;
		CBitmapDC dc(&m_viewImage);
		dc.FillSolidRect(0, 0, sz.cx, sz.cy, m_backColor);
		//== Scale�`��
		m_DrawScaleLine(dc, m_subScaleLines[2], false);
		m_DrawScaleLine(dc, m_subScaleLines[1], false);
		m_DrawScaleLine(dc, m_subScaleLines[0], false);
		m_DrawScaleLine(dc, m_scaleLine, true);
		//== �O���t�`��i�d�ˍ��킹�j
		if ( ! m_graphImage.IsEmpty() )
		{
			const SIZE& sz = m_graphImage.GetSize();
			INT_PTR x = -down_cast<INT_PTR>(m_viewHorizontalPos);
			if ( m_isReverseIndexMode )
			{
				x = m_clientSize.cx - sz.cx + m_viewHorizontalPos;
			}
			::TransparentBlt(dc, ToInt(x), 0, sz.cx, sz.cy, m_graphImageDc, 0, 0, sz.cx, sz.cy, INTERNALLY_COLOR);
		}
		//== �����`��
		{
			CRect rc(2, 2, sz.cx - 2, sz.cy - 2);
			CWnd* pParent = GetParent();
			CFont* pParentFont = GetFont();
			if ( pParent != NULL )
			{
				pParentFont = pParent->GetFont();
			}
			CFont* pOldFont = dc.SelectObject(GetFont());
			dc.SetBkMode(TRANSPARENT);
			loop ( i, 9 )
			{
				INDEX idx = i + 1;
				const TTextInfo& t = m_testInfo[idx];
				if ( ! t.text.IsEmpty() )
				{
					if ( t.hFont == NULL  )
					{
						dc.SelectObject(pParentFont);
					}
					else
					{
						::SelectObject(dc, t.hFont);
					}
					CTextDrawer::DrawTextRect(dc, rc, m_ToDtStyle(idx), t.color, t.text);
				}
			}
			dc.SelectObject(pOldFont);
		}
		//==
		OnMadeViewImage(&dc, sz);
		//==
		dc.Detach();
	}
	// �w��X�^�C���������`��
	void m_DrawStyleHorzLine(CDC& dc, bool isEdgeDotMode, int yy)
	{
		dc.MoveTo(0, yy);
		if ( ! isEdgeDotMode )
		{
			// ��
			dc.LineTo(m_clientSize.cx, yy);
		}
		else
		{
			// ���[�̂�
			dc.LineTo(4, yy);
			dc.MoveTo(m_clientSize.cx, yy);
			dc.LineTo(m_clientSize.cx - 4, yy);
		}
	}
	// �X�P�[�����`��
	void m_DrawScaleLine(CDC& dc, TScaleLineInfo& sli, bool isMaster)
	{
		if ( sli.colorPen.GetSafeHandle() == NULL )
		{
			return;
		}
		bool isEdgeDotMode = (sli.style == LineStyle_EdgeDot);
		dc.SetBkColor(m_backColor);
		CPen* pOldPen = dc.SelectObject(&sli.colorPen);
		if ( isMaster )
		{
			m_DrawStyleHorzLine(dc, isEdgeDotMode, m_CalcDrawY(0));
		}
		else
		{
			double ss = sli.step;
			double hh = max(::fabs(m_viewTopValue), ::fabs(m_viewbottomValue));
			while ( ss <= hh )
			{
				m_DrawStyleHorzLine(dc, isEdgeDotMode, m_CalcDrawY(ss));
				m_DrawStyleHorzLine(dc, isEdgeDotMode, m_CalcDrawY(-ss));
				ss += sli.step;
			}
		}
		dc.SelectObject(pOldPen);
	}
	// �|�C���g�Ǘ��^
	struct TPointInfo
	{
		CPoint	point;
		CBrush	brush;
		CPen	pen;
		TPointInfo(COLORREF c) : brush(c), pen(PS_SOLID, 1, c)
		{
		}
	};
	// �|�C���g�Ǘ��}�b�v�N���X
	typedef CMapT<COLORREF, CPointerHandleT<TPointInfo> >  CMyPointMap;

	bool				m_isScrollBarUsed;			///< �X�N���[���o�[�g�p����H
	CMyScrollBar		m_scrollBar;				///< �X�N���[���o�[�R���g���[��
	size_t				m_scrollBarHeight;			///< �X�N���[���o�[����
	size_t				m_scrollOneStep;			///< �X�N���[�������X�e�b�v��
	TTextInfo			m_testInfo[10];				///< �e�L�X�g���. [1]�`[9] �����ꂼ��e���L�[�ł����㉺���E�̈ʒu
	CMyPointMap			m_pointMap;					///< �F���̍ŏI�|�C���g���W
	TScaleLineInfo		m_scaleLine;				///< �X�P�[�������
	TScaleLineInfo		m_subScaleLines[3];			///< �T�u�X�P�[�������
	COLORREF			m_backColor;				///< �w�i�F
	size_t				m_markerWidth;				///< �|�C���g�}�[�J�[��
	double				m_viewTopValue;				///< �\������B
	double				m_viewbottomValue;			///< �\�������B
	double				m_viewHeight;				///< �\���͈�(m_viewTopValue - m_viewbottomValue + 1)
	bool				m_isReverseIndexMode;		///< �t�C���f�b�N�X���[�h�H
	CSize				m_clientSize;				///< �N���C�A���g�T�C�Y
	CBitmapDC			m_graphImageDc;				///< �O���t�C���[�W�� DC
	CBitmapImage		m_graphImage;				///< �\���C���[�W�i�܂���O���t�̂݁B�����̓N���C�A���g�̈�Ɠ����j
	CBitmapImage		m_viewImage;				///< �\���C���[�W�i������X�P�[��������B�傫���̓N���C�A���g�̈�Ɠ����j
	bool				m_isGraphImageDcLeading;	///< true �Ȃ�DC�݂��o����
	INDEX				m_viewHorizontalPos;		///< �\�������ʒu
	bool				m_isOndemandRemakeViewImage;///< �\���C���[�W�č쐬�v��
	int					m_horzIndexPixel;			///< �����C���f�b�N�X�P�����s�N�Z����
	bool				m_isMouseButtonPressed;		///< �v���X���H
	CPoint				m_pressedPos;				///< �v���X���ꂽ���W
};



};//MFC
};//TNB




