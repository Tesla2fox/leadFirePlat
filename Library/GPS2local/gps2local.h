#ifndef _GPS_TO_LOCAL_H
#define _GPS_TO_LOCAL_H

#if defined(LIBGPS2LOCAL_EXPORTS)
#if defined(_MSC_VER)
#define LIBGPS2LOCAL_API __declspec(dllexport) 
#else 
#define LIBGPS2LOCAL_API __attribute__((__visibility__("default")))
#endif
#elif defined(_MSC_VER)
#define LIBGPS2LOCAL_API __declspec(dllimport) 
#else
#define LIBGPS2LOCAL_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 坐标转换函数 GPS2Local/Local2GPS
 * 功能: GPS坐标 与 局部坐标之间的转换
 *		 局部坐标系的原点, x方向在北东地坐标系中的朝向角 可通过GPSSetAnchor/GPSGetAnchor设置/读取
 *       (默认x即是北, y即是东)
 *		 
 * GPS坐标: lat 维度, lon 经度, th_gps 朝向角(角度, 北为90, 东为0, 即按东北天坐标系)
 *      例: lat = 41.761128182994, lon = 123.441580684147, heading = 45
 * 局部坐标: x (米), y (米), th_local 朝向角(角度, x方向为0, y方向为90)
 *
 * 当只需要进行坐标转换时, 调用 GPS2Local(lat, lon, 0, &x, &y, NULL)
 *                              Local2GPS(x, y, 0, &lat, &lon, NULL) 即可
 */
LIBGPS2LOCAL_API void GPS2Local(double lat, double lon, double th_gps, double* x, double* y, double* th_local);
LIBGPS2LOCAL_API void Local2GPS(double x, double y, double th_local, double* lat, double* lon, double* th_gps);

/*
 * 坐标系转换配置函数
 * 功能: 设置/读取 局部坐标系的原点 与 朝向角
 */
LIBGPS2LOCAL_API void GPSSetAnchor(double lat, double lon, double th_gps);
LIBGPS2LOCAL_API void GPSGetAnchor(double* lat, double* lon, double* th_gps);

/*
 * 从配置文件中读取GPS的设置
 * 配置文件示意:

 ref_lat 39.962953093632
 ref_lon 116.304291427993
 ref_th  0

 */
LIBGPS2LOCAL_API void GPSLoadConfig(const char* config_file);

#ifdef __cplusplus
}      // extern "C"
#endif  

#endif // _GPS_TO_LOCAL_H
