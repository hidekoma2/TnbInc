#pragma once
/**
 *@file
 * ���C�A�E�g�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbRatioLayout.h"



//TNB Library
namespace TNB
{



/**@ingroup LAYOUT
 * ���C�h���C�A�E�g�A�C�e���N���X.
 *
 *	�X�N���[���̏�Ԃɂ��A�Q�̃��C�A�E�g�A�C�e���̕��т��c�����ɂ��邱�Ƃ��o���܂��B
 *	Windows Moblie �ŃR���g���[���̔z�u��ړI�Ƃ������̂ł��B
 *			\code
 *			���|�|�|�|�|�|�|�|�|��
 *			������������������������ʂ��c���̎�
 *			�����@�P�@���@�Q�@����
 *			����������������������
 *			���|�|�|�|�|�|�|�|�|��
 *			���e���C�A�E�g�A�C�e���̕��́A�S�̂̕�����A�䗦�w��ł��܂��B
 *			���e���C�A�E�g�A�C�e���̍����́A�S�̂̍����ɂȂ�܂��B
 *			���|�|�|�|�|��
 *			����������������ʂ������̎�
 *			�����@�P�@����
 *			��������������
 *			�����@�Q�@����
 *			��������������
 *			���|�|�|�|�|��
 *			���e���C�A�E�g�A�C�e���̍����́A�S�̂̍�������A�䗦�w��ł��܂��B
 *			���e���C�A�E�g�A�C�e���̕��́A�S�̂̕��ɂȂ�܂��B
 *			\endcode
 *
 *	@note �~���́A�c��ʂ̏ꍇ�Ɖ���ʂ̏ꍇ�ŕʁX�Ɏw��ł��܂�( SetCenter() )�B
 *
 *	@note �X�N���[����ԂƏc�����т̊֌W�́A SetReverseMode() �ŕύX�ł��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbWideLayout.h
 *
 *	@date 09/09/15 �V�K�쐬
 */
class CWideLayout : public CRatioLayout
{
	DEFSUPER(CRatioLayout);
public:

	/// �R���X�g���N�^
	CWideLayout(void) : m_isReverse(false)
	{
		m_center[0] = 50;
		m_center[1] = 50;
	}

	/**
	 * [�ݒ�] �~���ύX.
	 *	@note �ݒ肵�Ă��Ȃ��ꍇ 50�� �ł��B
	 *	@param isWide true �Ȃ牡��ʂ̏ꍇ�̐ݒ�B false �Ȃ�c��ʂ̏ꍇ�̐ݒ�B
	 *	@param parsent �ォ���̃��C�A�E�g�̃p�[�Z���g�B
	 */
	void SetCenter(bool isWide, DWORD parsent = 50)
	{
		m_center[isWide ? 1 : 0] = parsent;
	}

	/**
	 * [�ݒ�] ���o�[�X�ݒ�.
	 *	@param isReverse true ���w�肷��ƁA�c��ʂ̏ꍇ���E�A����ʂ̏ꍇ�A�㉺�ɔz�u����悤�ɂȂ�܂��B
	 */
	void SetReverseMode(bool isReverse = false)
	{
		m_isReverse = isReverse;
	}

	/**
	 * [�쐬] �N���[���쐬.
	 *		�����Ɠ����������s���N���X���쐬���܂��B
	 *	@return �|�C���^�B�g�p��� delete ����K�v������܂��B
	 */
	virtual ILayout* Clone(void) const
	{
		CWideLayout* P = new CWideLayout();
		*P = *this;
		return P;
	}

	/**
	 * [�擾] �ŏ��T�C�Y�擾.
	 *		�ŏ��̃T�C�Y�𓾂邱�Ƃ��o���܂��B
	 *	@param[out] _size �T�C�Y�擾�B
	 *	@retval true �T�C�Y�擾�����B 
	 *	@retval false �f�[�^���Ȃ��B
	 */
	virtual bool GetMinimumSize(SIZE& _size) const
	{
		m_SetParameter();
		return _super::GetMinimumSize(_size);
	}
	
	/**
	 * [����] ����.
	 *	@note �I�u�W�F�N�g���Ǘ����Ă���T�C�Y�Ǝw��̃T�C�Y�Ƀ��C�A�E�g���܂��B
	 *	@param x  X���W�B-1�Ȃ�w��Ȃ��B
	 *	@param y  Y���W�B-1�Ȃ�w��Ȃ��B
	 *	@param wParam W �p�����[�^�B�{�N���X�ł͎g�p���܂���B �{�N���X�ɓo�^�������C�A�E�g�ɂ���Ă͎g�p�����\��������܂��B
	 *	@param lParam L �p�����[�^�B�{�N���X�ł͎g�p���܂���B �{�N���X�ɓo�^�������C�A�E�g�ɂ���Ă͎g�p�����\��������܂��B
	 */
	virtual void Decide(int x, int y, WPARAM wParam, LPARAM lParam)
	{
		m_SetParameter();
		_super::Decide(x, y, wParam, lParam);
	}

private:
	// �p�����^�ݒ�
	void SetParameter(bool isHorizental, DWORD parsent)
	{
		_super::SetParameter(isHorizental, parsent);
	}
	// �p�����^�ݒ�
	void m_SetParameter(void) const
	{
		bool r = m_IsWideMode();
		CWideLayout* P = const_cast<CWideLayout*>(this); //const�͂���
		P->SetParameter(r, m_center[r ? 1 : 0]);
	}
	// true �Ȃ� �����
	bool m_IsWideMode(void) const
	{
		int nWidth = ::GetSystemMetrics(SM_CXSCREEN);
		int nHeight = ::GetSystemMetrics(SM_CYSCREEN);
		return (nHeight < nWidth) ^ (m_isReverse);
	}
	size_t	m_center[2];	///< �����̊��� [0];�c��ʂ̏ꍇ [1];�����
	bool	m_isReverse;	///< true �Ȃ�@���C�A�E�g�������]
};



}; // TNB
