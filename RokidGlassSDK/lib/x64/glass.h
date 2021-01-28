/***************************************************************
*  Copyright 2021, All Rights Reserved.
* 
 * @file       Rokid Glass Hardward
 * @brief      Rokid Glass的硬件设备层、可以获取Rokid硬件设备中IMU四元数、传感器 raw data、touch事件、Camera preview图片、Audio媒体流的硬件信息。
 * @author     medea
 * @version    1.0
 * @date       2021.1.21
 **************************************************************/
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "stdbool.h"

#ifdef IMPORT_DLL
#else
#define IMPORT_DLL  __declspec(dllimport)
#endif

/** 眼镜设备句柄 */
typedef void* GlassHandle;

/** 传感器类型 */
typedef enum SENSORID {
	/** 加速度 */
	ACCELEROMETER,
	/** 磁力计 */
	MAGNETIC,
	/** 陀螺仪 */
	GYROSCOPE,
};

/** 按键类型 */
typedef enum KEY_CODE {
	/** 返回键 */
	BACK_KEY = 4,
	/** 熄屏键 */
	POWER_KEY = 10
};

/** 触摸事件类型 */
typedef enum TOUCH_EVENT {
	/** 空置状态 */
	IDLE = 0,
	/** 压下 */
	PRESS,
	/** 短按 */
	SHORT_PRESS,
	/** 长按 */
	LONG_PRESS,
	/** 向前滑动 */
	FORWARD_SLIDE,
	/** 向后滑动 */
	BACKWARD_SLIDE,
	REPORTED
};

/** Camera 图片数据格式 */
typedef enum VIDEO_TYPE {
	YUV = 0,
	RGB,
	JPEG
};

/** Camera 图片基本信息 */
typedef struct _VidoFormat {
	/** Camera 图片类型 */
	int type;
	/** Camera 图片高度 */
	long height;
	/** Camera 图片宽度 */
	long wigth;
}VidoFormat;

/** Glass设备能力回调函数结构体 */
typedef struct  _GlassEvent{
	/** 返回IMU九轴绝对坐标融合的4元素数据，timeStamp代表时间戳，quaternion代表4元素数组 */
	void (*onRotationVectorEvent)(unsigned long timeStamp,float *quaternion);
	/** 返回IMU六轴相对坐标融合的4元素数据，timeStamp代表时间戳，quaternion代表4元素数组 */
	void (*onGameRotationVectorEvent)(unsigned long timeStamp, float* quaternion);
	/** 返回传感器的原始数据，timeStamp代表时间戳，sensorID代表枚举变量SENSORID的数据，data代表当前指定sensor返回的原始数据，status表示当前传感器的状态 */
	void (*onRawSensorEvent)(unsigned long timeStamp,int sensorID, float* data, int status);
	/** 返回按键事件，keyCode 设备按键的代号数据，status true表示按下，false表示释放*/
	void (*onKeyEvent)(int keyCode, bool status);
	/** 返回触摸事件，event 触摸类型，len 触摸时长*/
	void (*onTouchEvent)(int event, int len);
	/** 返回距离传感器事件，state true，靠近; false，远离 */
	void (*onProximitySensorEvnet)(bool state);
	/** 返回光线传感器事件，value 光线强度，单位lux */
	void (*onLightSensorEvent)(int value);
	/** 返回Camera图片数据，timestamp 事件戳， format 图片信息  pBuf 图片数据 size 图片数据数组的长度 */
	void (*onCaptuerEvent)(double timestamp, VidoFormat *format, BYTE* pBuf, long size);
	/** 返回Audio媒体流数据， pBuf 媒体流数据， frames 数组长度 */
	void (*onAudioEvent)(BYTE* pBuf, long frames);
}GlassEvent;

/***************************************************************
  *  @brief     通过USB协议初始化Rokid Glass设备，并且设置回调函数
  *  @param     callback 硬件设备数据通过回调函数返回给调用者
  *  @return    GlassHandle 初始化Rokid Glass设备返回的设备句柄
  *  @note      Rokid Glass所有的硬件能力都将通过GlassEvent的回调函数返回给调用者，需要关注那些硬件信息，调用者可以自行设置需要监听的回调函数
  *  @Sample usage:     nop
 **************************************************************/
 IMPORT_DLL GlassHandle GlassInitial(GlassEvent* callback);

 /***************************************************************
  *  @brief     打开Rokid Glass 设备
  *  @param     instance glass句柄对象
  *  @return    bool true: 表示打开成功；flase：代表打开失败。
  *  @Sample usage:     nop
 **************************************************************/
 IMPORT_DLL bool GlassOpen(GlassHandle instance);

 /***************************************************************
  *  @brief     获取Rokid Glass外设的固件版本
  *  @param     instance glass句柄对象
  *  @return     short 当前眼镜外设的固件版本
  *  @Sample usage:     nop
 **************************************************************/
 IMPORT_DLL	short GlassGetFirmwareVersion(GlassHandle instance);

 /***************************************************************
  *  @brief     获取Rokid Glass外设的唯一串号
  *  @param     instance glass句柄对象
  *  @return    wchar_t 返回设备唯一串号，串号是有一组unicode字符串组成
  *  @Sample usage:     nop
 **************************************************************/
 IMPORT_DLL	wchar_t *GlassGetSerialNumber(GlassHandle instance);

 /***************************************************************
  *  @brief     等待设备时间
  *  @param     instance glass句柄对象
  *  @return    nop
  *  @note      堵塞主线程函数，等待Rokid Glass硬件设备事件回调。
  *  @Sample usage:     nop
 **************************************************************/
 IMPORT_DLL void GlassWaitEvent(GlassHandle instance);

 /***************************************************************
  *  @brief     关闭Rokid Glass设备的连接。
  *  @param     instance glass句柄对象
  *  @return    nop
  *  @Sample usage:     nop
 **************************************************************/
 IMPORT_DLL void GlassClose(GlassHandle instance);

 /***************************************************************
  *  @brief     启动Rokid Glass设备中的Camera preview
  *  @param     instance glass句柄对象
  *  @return    GlassHandle 初始化Rokid Glass设备返回的设备句柄
  *  @note      初始化并且启动Camera，Camera的preview数据通过回调函数onCaptuerEvnet将图像数据返回给调用者。
  *  @Sample usage:     nop
 **************************************************************/
 IMPORT_DLL void GlassStartCapture(GlassHandle instance);

 /***************************************************************
  *  @brief     停止Rokid Glass设备中的Camera preview
  *  @param     instance glass句柄对象
  *  @return    GlassHandle 初始化Rokid Glass设备返回的设备句柄
  *  @note      关闭Camera设备
  *  @Sample usage:     nop
 **************************************************************/
 IMPORT_DLL void GlassStopCapture(GlassHandle instance);
#ifdef __cplusplus
}
#endif
