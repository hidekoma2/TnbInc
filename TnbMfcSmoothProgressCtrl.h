#pragma once
/**
 *@file
 * �X���[�X�i���R���g���[���֌W�̃w�b�_
 *
 *		�X���[�X�ȃo�[�̓�����񋟂���i���R���g���[����񋟂��܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbMfcCommon.h"



//TNB Library
namespace TNB {
namespace MFC {



/**@ingroup MFCCONTROL
 * �X���[�X�i���R���g���[���N���X
 *
 *		�X���[�X�ȃo�[�̓�����񋟂���i���R���g���[���ł��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ńv���O���X�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCProgressCtrl�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@param PGC �v���O���X�R���g���[���̃N���X�B�ȗ������ CProgressCtrl
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcSmoothProgressCtrl.h
 * 
 *	@date 10/04/05 �V�K�쐬
 */
template<typename PGC = CProgressCtrl>
class CSmoothProgressCtrlT : public PGC
{
	DEFSUPER(PGC);
public:

	/// �R���X�g���N�^
	CSmoothProgressCtrlT(void) : m_step(1), m_currentPos(0), m_maxDelta(-1.0)
	{
	}

	/**
	 * [�ݒ�] �o�[�̐F�w��.
	 *	@param color �o�[�̐F�B�ȗ�����ƃf�t�H���g�̐F�ɂȂ�܂��B
	 *	@return �ݒ�O�̐F�B
	 */
	COLORREF SetBarColor(COLORREF color = CLR_DEFAULT) 
	{
		return _super::SendMessage(PBM_SETBARCOLOR, 0, color);
	}

	/**
	 * [�ݒ�] �w�i�F�w��.
	 *	@param color �w�i�F�B�ȗ�����ƃf�t�H���g�̐F�ɂȂ�܂��B
	 *	@return �ݒ�O�̐F�B
	 */
	COLORREF SetBkColor(COLORREF color = CLR_DEFAULT)
	{
		return _super::SendMessage(PBM_SETBKCOLOR, 0, color);
	}

	/**
	 * [�ݒ�] �͈͐ݒ�.
	 *	@param nLower �����B�f�t�H���g�� 0�D
	 *	@param nUpper ����B�f�t�H���g�� 100.
	 */
	void SetRange(short nLower, short nUpper)
	{
		SetRange32(nLower, nUpper);
	}

	/**
	 * [�ݒ�] �͈͐ݒ�.
	 *	@param nLower �����B�f�t�H���g�� 0�D
	 *	@param nUpper ����B�f�t�H���g�� 100.
	 */
	void SetRange32(int nLower, int nUpper)
	{
		_super::SetRange32(nLower, nUpper);
		m_lower = nLower;
		m_upper = nUpper;
	}

	/**
	 * [�擾] �͈͐ݒ�擾.
	 *	@param[out] _nLower �������i�[����܂��B
	 *	@param[out] _nUpper ������i�[����܂��B
	 */
	void GetRange(int& _nLower, int& _nUpper)
	{
		_super::GetRange(_nLower, _nUpper);
	}

	/**
	 * [�擾] �|�W�V�����擾.
	 *	@return �|�W�V����.
	 */
	int GetPos(void)
	{
		return m_currentPos;
	}

	/**
	 * [�ݒ�] �|�W�V�����ݒ�.
	 *	@note �X���[�X�Ɉړ������A�|�W�V�������ړ������܂��B
	 *	@param nPos �|�W�V����.
	 *	@return �ݒ�O�̃|�W�V����.
	 */
	int SetPos(int nPos)
	{
		m_currentPos = nPos;
		m_viewPos = nPos;
		m_viewDelta = 0.0;
		return _super::SetPos(nPos);
	}

	/**
	 * [�ݒ�] �X�e�b�v�l�ݒ�.
	 *		StepIt() �ŁA�ړ�����|�W�V�����������w�肵�܂��B
	 *	@param nStep ����.
	 *	@return �ݒ�O�̑���.
	 */
	int SetStep(int nStep)
	{
		Swap(m_step, nStep);
		return nStep;
	}

	/**
	 * [�ݒ�] �|�W�V�����ړ�.
	 *		SetStep() �Ŏw�肵�����������A�|�W�V�������ړ������܂��B
	 *	@return �ړ��O�̃|�W�V����.
	 */
	int StepIt(void)
	{
		return OffsetPos(m_step);
	}

	/**
	 * [�ݒ�] �|�W�V�����ړ�.
	 *	@param nPos �����B���̕������A�|�W�V�������ړ������܂��B
	 *	@return �ړ��O�̃|�W�V����.
	 */
	int OffsetPos(int nPos)
	{
		int r = m_currentPos;
		m_currentPos += nPos;
		_super::SetTimer(TIMERID_MOVE, 10, NULL);
		return  r;
	}

	/**
	 * [�ݒ�] �ő�ړ��l�ݒ�.
	 *		�w��̃|�W�V�����܂ňړ����鎞�́A���̈ړ��l�̍ő���w�肷�邱�Ƃ��o���܂��B
	 *	@param d �ő�ړ��l�B�ȗ�����ƁA�ő�l���������܂��B
	 */
	void SetMaxDelta(double d = -1.0)
	{
		m_maxDelta = d;			///< �ړ��l�̍ő�
	}

protected:

	/**
	 * [�ʒm] subclassing/unsubclassing functions.
	 *		�T�u�N���X�����鎞�R�[������܂��B
	 *	@note CWnd�̃��\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	virtual void PreSubclassWindow(void)
	{
		_super::GetRange(m_lower, m_upper);
		SetPos(0);
		_super::PreSubclassWindow();
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
		if ( message == WM_TIMER && wParam == TIMERID_MOVE )
		{
			OnMoveTimer(wParam);
			return 0;
		}
		return _super::WindowProc(message, wParam, lParam);
	}

	/**
	 * [�ʒm] WM_TIMER �C�x���g.
	 *	@param nIDEvent �^�C�}�[�C�x���g
	 */
	void OnMoveTimer(UINT nIDEvent)
	{
		ASSERT( nIDEvent == TIMERID_MOVE );
		if ( m_currentPos != ToInt(m_viewPos + 0.005) )
		{
			// �\���ʒu�ƁA�����ʒu�ɂ��ꂪ����B
			m_viewDelta = m_currentPos - m_viewPos;
			if ( m_upper != m_currentPos )
			{
				m_viewDelta /= 20.0; //20���
			}
			else
			{
				m_viewDelta /= 5.0; //5���
			}
		}
		else
		{
			m_viewDelta = 0.0;
//			_super::SetPos(m_currentPos);
			KillTimer(TIMERID_MOVE);
		}
		if ( m_viewDelta != 0.0 )
		{
			//== �f���^������B
			//TRACE1("delta = %.4f\n", m_viewDelta);
			if ( m_maxDelta > 0.0 )
			{
				if ( ::fabs(m_viewDelta) > ::fabs(m_maxDelta) )
				{
					bool m = (m_viewDelta > 0.0);
					m_viewDelta = m ? m_maxDelta : -m_maxDelta;
				}
			}
			m_viewPos += m_viewDelta;
			if ( m_viewPos < m_lower )
			{
				m_viewPos = m_lower;
			}
			if ( m_viewPos > m_upper )
			{
				m_viewPos = m_upper;
			}
			_super::SetPos(ToInt(m_viewPos + 0.005));
		}
	}

private:
	enum { TIMERID_MOVE = 1 };
	double	m_viewPos;			///< �\�����̃|�W�V����
	double	m_viewDelta;		///< �ړ��l
	double	m_maxDelta;			///< �ړ��l�̍ő�
	int		m_currentPos;		///< ���݂̃|�W�V����(�ݒ肳��Ă���l)
	int		m_step;				///< StepIt �̃X�e�b�v�l
	int		m_lower;			///< ����
	int		m_upper;			///< ���
};



/**@ingroup MFCCONTROL
 * �X���[�X�i���R���g���[���N���X
 *
 *		�X���[�X�ȃo�[�̓�����񋟂���i���R���g���[���ł��B
 *
 *	@par�g����
 *		���\�[�X�G�f�B�^�Ńv���O���X�R���g���[���𒣂�A�R���g���[���ϐ��Ɋ��蓖�Č�A
 *		���̃R���g���[���ϐ��̌^�iCProgressCtrl�j��{�N���X�ɒu�������Ďg�p���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbMfcSmoothProgressCtrl.h
 * 
 *	@date 10/04/05 �V�K�쐬
 */
typedef CSmoothProgressCtrlT<> CSmoothProgressCtrl;



};
};
