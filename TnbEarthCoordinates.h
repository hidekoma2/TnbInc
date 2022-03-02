#pragma once
/**
 *@file
 * 地球座標(緯度,経度)関係のヘッダ
 *
 *	@see TnbDef.h 利用条件、および無保証規定
 */



#include "TnbValueUnit.h"



//TNB Library
namespace TNB
{



/**@ingroup VALUEUNIT
 * 地球座標(緯度,経度)管理クラス
 *
 *		一組の緯度と経度を管理します。
 *
 *	@note 緯度がマイナスの場合、南緯を表します。
 *	@note 経度がマイナスの場合、西経を表します。
 *
 *	@par必要ファイル
 *			TnbEarthCoordinates.h
 * 
 *	@date 09/01/27 新規作成。
 */
class CEarthCoordinates
{
public:

	/// コンストラクタ
	CEarthCoordinates(void) : m_latitude(0.0), m_longitude(0.0)
	{
	}

	/**
	 * 代入コンストラクタ.
	 *	@note 代入時, MakeDegree() を使うと便利です。
	 *	@param latitude 緯度
	 *	@param longitude  経度
	 */
	CEarthCoordinates(const CValueDegree& latitude, const CValueDegree& longitude)
		: m_latitude(latitude), m_longitude(longitude)
	{
	}

	/**
	 * 代入コンストラクタ.
	 *	@param latitude 緯度(単位;度)
	 *	@param longitude  経度(単位;度)
	 */
	CEarthCoordinates(double latitude, double longitude)
		: m_latitude(CValueDegree(latitude)), m_longitude(CValueDegree(longitude))
	{
	}

	/**
	 * コピーコンストラクタ.
	 *	@param other コピー元
	 */
	CEarthCoordinates(const CEarthCoordinates& other)
	{
		m_latitude = other.m_latitude;
		m_longitude = other.m_longitude;
	}

	/**
	 * コピーオペレータ.
	 *	@param other コピー元
	 *	@return 自分の参照
	 */
	CEarthCoordinates& operator=(const CEarthCoordinates& other)
	{
		m_latitude = other.m_latitude;
		m_longitude = other.m_longitude;
		return *this;
	}

	/**
	 * [計算] 方位角計算.
	 *		本インスタンスが保持している座標と指定の座標の方位角を計算。
	 *	@param other 地球座標
	 *	@return 方位角
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
	 * [計算] 直距離.
	 *		本インスタンスが保持している座標と指定の座標の距離を計算。
	 *	@param other 地球座標
	 *	@return 距離(m)
	 */
	CValueDistance CalcDirectDistance(const CEarthCoordinates& other) const
	{
		return CValueDistance(m_CalcDirectDistance(other), CValueDistance::M);
	}

	/**
	 * [計算] 地表面距離.
	 *		本インスタンスが保持している座標と指定の座標の距離を計算。
	 *	@param other 地球座標
	 *	@return 距離(m)
	 */
	CValueDistance CalcEarthSurfaceDistance(const CEarthCoordinates& other) const
	{
		const double er = 6378136.0;					// 赤道半径(m)
		CRealNumber r = m_CalcDirectDistance(other);
		CRealNumber wr = asin(r / 2 / er);				// 半射程角(rad)
		return CValueDistance(er * 2 * wr, CValueDistance::M);
	} 

	/**
	 * [計算] 座標移動.
	 *	@note 指定の角度に指定の距離を移動した座標を計算します。
	 *	@param de 方位角。真北を０度、真東を９０度とします。
	 *	@param di 距離。
	 *	@return 移動後の座標。
	 */
	CEarthCoordinates Move(const CValueDegree& de, const CValueDistance& di) const
	{
		CValueDegree one(1.0, CValueDegree::DEG); //１°
		CEarthCoordinates rr = *this;
		CValueDistance cx = sin(de) * di;	//移動距離(東西方向)
		CValueDistance cy = cos(de) * di;	//移動距離(南北方向)
		//経度(たて)
		CEarthCoordinates ecy(m_latitude + one, m_longitude);
		CValueDistance dy = CalcDirectDistance(ecy);	//経度一度の長さ
		CRealNumber ry = cy / dy;		//移動経度
		rr.m_latitude += CValueDegree(ry, CValueDegree::DEG);
		//緯度(横)
		CEarthCoordinates ecx(rr.m_latitude, m_longitude + one);
		CValueDistance dx = CalcDirectDistance(ecx);	//緯度一度の長さ
		CRealNumber rx = cx / dx;		//移動緯度
		rr.m_longitude += CValueDegree(rx, CValueDegree::DEG);
		return rr;
	}

	/**
	 * [取得] 緯度
	 *	@return 緯度
	 */
	const CValueDegree& GetLatitude(void) const
	{
		return m_latitude;
	}

	/**
	 * [取得] 経度
	 *	@return 経度
	 */
	const CValueDegree& GetLongitude(void) const
	{
		return m_longitude;
	}

	/**
	 * [変換] 緯度文字列化
	 *	@note ex) 北緯 12度34分56.789秒
	 *	@return 文字列
	 */
	CStr LatitudeToString(void) const
	{
		double r = m_latitude.GetValue(CValueDegree::DEGS).ToDouble();
		return CStr::Fmt(_T("%s %s"), (r >= 0) ? _T("北緯") :  _T("南緯"), SecondToString(r));
	}

	/**
	 * [変換] 経度文字列化
	 *	@note ex) 東経 12度34分56.789秒
	 *	@return 文字列
	 */
	CStr LongitudeToString(void) const
	{
		double r = m_longitude.GetValue(CValueDegree::DEGS).ToDouble();
		return CStr::Fmt(_T("%s %s"), (r >= 0) ? _T("東経") :  _T("西経"), SecondToString(r));
	}

	/**
	 * [変換] 文字列化
	 *	@return 文字列
	 */
	CStr ToString(void) const
	{
		return CStr::Fmt(_T("%s / %s"), LatitudeToString(), LongitudeToString());
	}

	/**
	 * [変換] 角度(秒)文字列化
	 *	@note マイナス表示はされません。
	 *	@param deg 角度(秒)
	 *	@return 文字列
	 */
	static CStr SecondToString(double deg)
	{
		int dd;
		int mm;
		double ss;
		Resolution(dd, mm, ss, deg);
		return CStr::Fmt(_T("%d度%d分%.3f秒"), dd, mm, ss);
	}

	/**
	 * [変換] 角度分解.
	 *	@note 単位が秒の角度を度、分、秒に分解します。
	 *	@param[out] _dd 度
	 *	@param[out] _mm 分(0以上 60未満)
	 *	@param[out] _ss 秒(0以上 60未満)
	 *	@param[in] deg 角度(秒)。
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
	 * [計算] 直距離.
	 *	@param other 地球座標
	 *	@return 距離(m)
	 */
	CRealNumber m_CalcDirectDistance(const CEarthCoordinates& other) const
	{
		const double er = 6378136.0;				// 赤道半径(m)
		const double ecc = 0.006694470;				// 地球の離心率の自乗
		const CValueDegree& lat1 = m_latitude;
		const CValueDegree& lat2 = other.m_latitude;
		const CValueDegree& lon1 = m_longitude;
		const CValueDegree& lon2 = other.m_longitude;
		CRealNumber n1 = er / (sqrt(1.0 - ecc * sin(lat1) * sin(lat1)));//緯度補正した地球の半径(m)
		CRealNumber x1 = n1 * cos(lat1) * cos(lon1);				//２地点の直交座標値(m)
		CRealNumber y1 = n1 * cos(lat1) * sin(lon1);
		CRealNumber z1 = (n1 * (1.0 - ecc)) * sin(lat1);
		CRealNumber n2 = er / (sqrt(1.0 - ecc * sin(lat2) * sin(lat2)));//緯度補正した地球の半径(m)
		CRealNumber x2 = n2 * cos(lat2) * cos(lon2);				//２地点の直交座標値(m)
		CRealNumber y2 = n2 * cos(lat2) * sin(lon2);
		CRealNumber z2 = (n2 * (1.0 - ecc)) * sin(lat2);
		return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
	}

	CValueDegree	m_latitude;		///< 緯度
	CValueDegree	m_longitude;	///< 経度
	friend class CEarthCoordinatesTest;
};



}; // TNB


/*
 memo

　2007年の磁北極は 北緯 84.1度 西経123.7度
　マリンスタジアム 
		緯度 35度38分46.95秒(35.646375), 経度 140度1分52.928秒(140.031369)
　東京タワー 
		緯度 35度39分31.075秒(35.658632), 経度 139度44分43.48秒(139.745411)
　東京ドーム
		緯度 35度42分20.045秒(35.705568), 経度 139度45分6.858秒(139.751905)
　札幌ドーム
		緯度 43度0分54.612秒(43.01517), 経度 141度24分35.154秒(141.409765)
　ハワイ
		緯度 19度32分20.703秒(19.539084), 経度 -155度40分34.57秒(-155.67627)
　ストップホルム
		緯度 59度19分58.037秒(59.332788), 経度 18度3分52.157秒(18.064488)


*/