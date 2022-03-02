#pragma once
/**
 *@file
 * ���Q�Ǘ��̃V���A���C�U�[�A�_�v�^�֌W�̃w�b�_
 *
 *		���Q�Ǘ��̃V���A���C�U�[�A�_�v�^�N���X������܂��B
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbCollection.h"
#include "TnbCollectionAdapter.h"
#include "TnbSerializer.h"



//TNB Library
namespace TNB
{



/**@ingroup SERIALIZE COLLECT
 * �V���A���C�Y�A�_�v�^.
 *
 *		BYTE �^�̃R���N�V�����ɑ΂��A�f�[�^���V���A���C�Y���܂��B
 *
 *	@note	�V���A���C�Y���̌^�͕ۑ����Ă��܂���B
 *			�f�V���A���C�Y���A�V���A���C�Y�������Ԃƌ^����v������K�v���L��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbSerializeAdapter.h
 *
 *	@date 06/08/17 �V�K�쐬
 *	@date 07/03/15 size�Ǘ��� unsigned�ɕύX�B
 */
class CSerializeAdapter : public ISerializer
{
	ICollectionT<BYTE> *	m_pColl;	///< �Ώۂ̃R���N�V����

public:

	/**
	 * �R���X�g���N�^
	 *	@param in �i�[�Ώۂ�BYTE�R���N�V�����i�ǋL����܂��j�B
	 */
	CSerializeAdapter(ICollectionT<BYTE>* in) : m_pColl(in)
	{
	}

	/**
	 * [�ǉ�] �ǉ�.
	 *		�w��f�[�^��ǉ����܂��B
	 *	@param size �ǉ�����f�[�^�T�C�Y(�P��BYTE)
	 *	@param P �ǉ�����f�[�^
	 *	@throw CAppendFailureException �ǉ��Ɏ��s�������A�X���[����܂��B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void AddRaw(size_t size, LPCVOID P)
	{
		if ( m_pColl->AddElements(size, static_cast<const BYTE*>(P)) != size )
		{
			throw CAppendFailureException();
		}
	}
};



/**@ingroup SERIALIZE COLLECT
 * �f�V���A���C�Y�A�_�v�^.
 *
 *		BYTE�^�̃R���N�V��������A�f�V���A���C���܂��B
 *
 *	@note	�V���A���C�Y���̌^�͕ۑ����Ă��܂���B
 *			�f�V���A���C�Y���A�V���A���C�Y�������Ԃƌ^����v������K�v���L��܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbSerializeAdapter.h
 *
 *	@date 06/08/17 �V�K�쐬
 *	@date 07/03/15 size�Ǘ��� unsigned�ɕύX�B
 */
class CDeserializeAdapter : public IDeserializer
{
	const IConstCollectionT<BYTE> *	m_pColl;	///< �Ώۂ̃R���N�V����
	mutable INDEX					m_pos;		///< ���݂̃A�N�Z�X�|�W�V����

public:

	/**
	 * �R���X�g���N�^
	 *	@param in ���o���Ώۂ�BYTE�R���N�V�����B
	 */
	CDeserializeAdapter(const IConstCollectionT<BYTE>* in)
		: m_pColl(in), m_pos(0)
	{
	}

	/**
	 * [�擾] �擾.
	 *		�w��f�[�^���擾���܂��B
	 *	@param[in] size �擾����f�[�^�T�C�Y(�P��BYTE)
	 *	@param[out] _P �擾����f�[�^
	 *	@throw CShortOfDataException �ǂݏo���T�C�Y������Ȃ����A�X���[����܂��B
	 *	@throw CTnbException ���s���́A�C�ӂ̃X���[�̉\��������܂��B
	 */
	virtual void GetRaw(size_t size, LPVOID _P) const
	{
		CConstOffsetAdapterT<BYTE> ad(m_pColl, m_pos);
		size_t l = ad.GetElements(size, static_cast<BYTE*>(_P));
		if ( l != size )
		{
			throw CShortOfDataException();
		}
		m_pos += l;
	}
};



/**@ingroup COLLECT
 * �f���^�����R���N�V����������
 *	@note index0�̃f�[�^�͕ω������܂���B
 *	@param[in,out] _c ����������R���N�V�����B
 *	@throw CTnbException ���쎞�ɃG���[������ƁA�X���[�����\��������܂��B
 */
template<typename TYP>
inline void EncodeDeltaCollection(ICollectionT<TYP>& _c)
{
	size_t size = _c.GetSize();
	if ( size > 1 )
	{
		TYP last = _c.Get(0);
		for ( INDEX i = 1; i < size; i++ )
		{
			TYP t = _c.Get(i);
			_c.Set(i, t - last);
			last = t;
		}
	}
}



/**@ingroup COLLECT
 * �f���^�����R���N�V����������
 *	@note index0�̃f�[�^����_�ɕ������܂��B
 *	@param[in,out] _c ����������R���N�V�����B
 *	@throw CTnbException ���쎞�ɃG���[������ƁA�X���[�����\��������܂��B
 */
template<typename TYP>
inline void DecodeDeltaCollection(ICollectionT<TYP>& _c)
{
	size_t size = _c.GetSize();
	if ( size > 1 )
	{
		TYP last = _c.Get(0);
		for ( INDEX i = 1; i < size; i++ )
		{
			TYP t = _c.Get(i) + last;
			_c.Set(i, t);
			last = t;
		}
	}
}



}; // TNB

