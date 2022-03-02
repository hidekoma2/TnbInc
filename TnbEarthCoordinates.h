#pragma once
/**
 *@file
 * �n�����W(�ܓx,�o�x)�֌W�̃w�b�_
 *
 *	@see TnbDef.h ���p�����A����і��ۏ؋K��
 */



#include "TnbValueUnit.h"



//TNB Library
namespace TNB
{



/**@ingroup VALUEUNIT
 * �n�����W(�ܓx,�o�x)�Ǘ��N���X
 *
 *		��g�̈ܓx�ƌo�x���Ǘ����܂��B
 *
 *	@note �ܓx���}�C�i�X�̏ꍇ�A��܂�\���܂��B
 *	@note �o�x���}�C�i�X�̏ꍇ�A���o��\���܂��B
 *
 *	@par�K�v�t�@�C��
 *			TnbEarthCoordinates.h
 * 
 *	@date 09/01/27 �V�K�쐬�B
 */
class CEarthCoordinates
{
public:

	/// �R���X�g���N�^
	CEarthCoordinates(void) : m_latitude(0.0), m_longitude(0.0)
	{
	}

	/**
	 * ����R���X�g���N�^.
	 *	@note �����, MakeDegree() ���g���ƕ֗��ł��B
	 *	@param latitude �ܓx
	 *	@param longitude  �o�x
	 */
	CEarthCoordinates(const CValueDegree& latitude, const CValueDegree& longitude)
		: m_latitude(latitude), m_longitude(longitude)
	{
	}

	/**
	 * ����R���X�g���N�^.
	 *	@param latitude �ܓx(�P��;�x)
	 *	@param longitude  �o�x(�P��;�x)
	 */
	CEarthCoordinates(double latitude, double longitude)
		: m_latitude(CValueDegree(latitude)), m_longitude(CValueDegree(longitude))
	{
	}

	/**
	 * �R�s�[�R���X�g���N�^.
	 *	@param other �R�s�[��
	 */
	CEarthCoordinates(const CEarthCoordinates& other)
	{
		m_latitude = other.m_latitude;
		m_longitude = other.m_longitude;
	}

	/**
	 * �R�s�[�I�y���[�^.
	 *	@param other �R�s�[��
	 *	@return �����̎Q��
	 */
	CEarthCoordinates& operator=(const CEarthCoordinates& other)
	{
		m_latitude = other.m_latitude;
		m_longitude = other.m_longitude;
		return *this;
	}

	/**
	 * [�v�Z] ���ʊp�v�Z.
	 *		�{�C���X�^���X���ێ����Ă�����W�Ǝw��̍��W�̕��ʊp���v�Z�B
	 *	@param other �n�����W
	 *	@return ���ʊp
	 */
	CValueDegree CalcAzimuthal(const CEarthCoordinates& other) const
	{
		const CValueDegree& lat1 = m_latitude;
		const CValueDegree& lat2 = other.m_latitude;
		const CValueDegree& lon1 = m_longitude;
		const CValueDegree& lon2 = other.m_longitude;
		CRealNumber y = cos(lat2) * sin(lon2 - lon1);
		CRealNumber x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(lon2 - lon1);
		return CValueDegree(atan2(y, x), CValueDegree::RAD);
	}

	/**
	 * [�v�Z] ������.
	 *		�{�C���X�^���X���ێ����Ă�����W�Ǝw��̍��W�̋������v�Z�B
	 *	@param other �n�����W
	 *	@return ����(m)
	 */
	CValueDistance CalcDirectDistance(const CEarthCoordinates& other) const
	{
		return CValueDistance(m_CalcDirectDistance(other), CValueDistance::M);
	}

	/**
	 * [�v�Z] �n�\�ʋ���.
	 *		�{�C���X�^���X���ێ����Ă�����W�Ǝw��̍��W�̋������v�Z�B
	 *	@param other �n�����W
	 *	@return ����(m)
	 */
	CValueDistance CalcEarthSurfaceDistance(const CEarthCoordinates& other) const
	{
		const double er = 6378136.0;					// �ԓ����a(m)
		CRealNumber r = m_CalcDirectDistance(other);
		CRealNumber wr = asin(r / 2 / er);				// ���˒��p(rad)
		return CValueDistance(er * 2 * wr, CValueDistance::M);
	} 

	/**
	 * [�v�Z] ���W�ړ�.
	 *	@note �w��̊p�x�Ɏw��̋������ړ��������W���v�Z���܂��B
	 *	@param de ���ʊp�B�^�k���O�x�A�^�����X�O�x�Ƃ��܂��B
	 *	@param di �����B
	 *	@return �ړ���̍��W�B
	 */
	CEarthCoordinates Move(const CValueDegree& de, const CValueDistance& di) const
	{
		CValueDegree one(1.0, CValueDegree::DEG); //�P��
		CEarthCoordinates rr = *this;
		CValueDistance cx = sin(de) * di;	//�ړ�����(��������)
		CValueDistance cy = cos(de) * di;	//�ړ�����(��k����)
		//�o�x(����)
		CEarthCoordinates ecy(m_latitude + one, m_longitude);
		CValueDistance dy = CalcDirectDistance(ecy);	//�o�x��x�̒���
		CRealNumber ry = cy / dy;		//�ړ��o�x
		rr.m_latitude += CValueDegree(ry, CValueDegree::DEG);
		//�ܓx(��)
		CEarthCoordinates ecx(rr.m_latitude, m_longitude + one);
		CValueDistance dx = CalcDirectDistance(ecx);	//�ܓx��x�̒���
		CRealNumber rx = cx / dx;		//�ړ��ܓx
		rr.m_longitude += CValueDegree(rx, CValueDegree::DEG);
		return rr;
	}

	/**
	 * [�擾] �ܓx
	 *	@return �ܓx
	 */
	const CValueDegree& GetLatitude(void) const
	{
		return m_latitude;
	}

	/**
	 * [�擾] �o�x
	 *	@return �o�x
	 */
	const CValueDegree& GetLongitude(void) const
	{
		return m_longitude;
	}

	/**
	 * [�ϊ�] �ܓx������
	 *	@note ex) �k�� 12�x34��56.789�b
	 *	@return ������
	 */
	CStr LatitudeToString(void) const
	{
		double r = m_latitude.GetValue(CValueDegree::DEGS).ToDouble();
		return CStr::Fmt(_T("%s %s"), (r >= 0) ? _T("�k��") :  _T("���"), SecondToString(r));
	}

	/**
	 * [�ϊ�] �o�x������
	 *	@note ex) ���o 12�x34��56.789�b
	 *	@return ������
	 */
	CStr LongitudeToString(void) const
	{
		double r = m_longitude.GetValue(CValueDegree::DEGS).ToDouble();
		return CStr::Fmt(_T("%s %s"), (r >= 0) ? _T("���o") :  _T("���o"), SecondToString(r));
	}

	/**
	 * [�ϊ�] ������
	 *	@return ������
	 */
	CStr ToString(void) const
	{
		return CStr::Fmt(_T("%s / %s"), LatitudeToString(), LongitudeToString());
	}

	/**
	 * [�ϊ�] �p�x(�b)������
	 *	@note �}�C�i�X�\���͂���܂���B
	 *	@param deg �p�x(�b)
	 *	@return ������
	 */
	static CStr SecondToString(double deg)
	{
		int dd;
		int mm;
		double ss;
		Resolution(dd, mm, ss, deg);
		return CStr::Fmt(_T("%d�x%d��%.3f�b"), dd, mm, ss);
	}

	/**
	 * [�ϊ�] �p�x����.
	 *	@note �P�ʂ��b�̊p�x��x�A���A�b�ɕ������܂��B
	 *	@param[out] _dd �x
	 *	@param[out] _mm ��(0�ȏ� 60����)
	 *	@param[out] _ss �b(0�ȏ� 60����)
	 *	@param[in] deg �p�x(�b)�B
	 */
	static void Resolution(int& _dd, int& _mm, double& _ss, double deg)
	{
		int r = 1;
		if ( deg < 0 )
		{
			deg = -deg;
			r = -1;
		}
		_dd = static_cast<int>(deg / 60.0 / 60.0);
		_mm = int(deg / 60.0) % 60;
		_ss = deg - ((_dd * 60) + _mm) * 60;
		_dd *= r;
	}

private:

	/**
	 * [�v�Z] ������.
	 *	@param other �n�����W
	 *	@return ����(m)
	 */
	CRealNumber m_CalcDirectDistance(const CEarthCoordinates& other) const
	{
		const double er = 6378136.0;				// �ԓ����a(m)
		const double ecc = 0.006694470;				// �n���̗��S���̎���
		const CValueDegree& lat1 = m_latitude;
		const CValueDegree& lat2 = other.m_latitude;
		const CValueDegree& lon1 = m_longitude;
		const CValueDegree& lon2 = other.m_longitude;
		CRealNumber n1 = er / (sqrt(1.0 - ecc * sin(lat1) * sin(lat1)));//�ܓx�␳�����n���̔��a(m)
		CRealNumber x1 = n1 * cos(lat1) * cos(lon1);				//�Q�n�_�̒������W�l(m)
		CRealNumber y1 = n1 * cos(lat1) * sin(lon1);
		CRealNumber z1 = (n1 * (1.0 - ecc)) * sin(lat1);
		CRealNumber n2 = er / (sqrt(1.0 - ecc * sin(lat2) * sin(lat2)));//�ܓx�␳�����n���̔��a(m)
		CRealNumber x2 = n2 * cos(lat2) * cos(lon2);				//�Q�n�_�̒������W�l(m)
		CRealNumber y2 = n2 * cos(lat2) * sin(lon2);
		CRealNumber z2 = (n2 * (1.0 - ecc)) * sin(lat2);
		return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
	}

	CValueDegree	m_latitude;		///< �ܓx
	CValueDegree	m_longitude;	///< �o�x
	friend class CEarthCoordinatesTest;
};



}; // TNB


/*
 memo

�@2007�N�̎��k�ɂ� �k�� 84.1�x ���o123.7�x
�@�}�����X�^�W�A�� 
		�ܓx 35�x38��46.95�b(35.646375), �o�x 140�x1��52.928�b(140.031369)
�@�����^���[ 
		�ܓx 35�x39��31.075�b(35.658632), �o�x 139�x44��43.48�b(139.745411)
�@�����h�[��
		�ܓx 35�x42��20.045�b(35.705568), �o�x 139�x45��6.858�b(139.751905)
�@�D�y�h�[��
		�ܓx 43�x0��54.612�b(43.01517), �o�x 141�x24��35.154�b(141.409765)
�@�n���C
		�ܓx 19�x32��20.703�b(19.539084), �o�x -155�x40��34.57�b(-155.67627)
�@�X�g�b�v�z����
		�ܓx 59�x19��58.037�b(59.332788), �o�x 18�x3��52.157�b(18.064488)


*/