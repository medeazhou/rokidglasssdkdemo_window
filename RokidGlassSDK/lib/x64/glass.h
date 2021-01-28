/***************************************************************
*  Copyright 2021, All Rights Reserved.
* 
 * @file       Rokid Glass Hardward
 * @brief      Rokid Glass��Ӳ���豸�㡢���Ի�ȡRokidӲ���豸��IMU��Ԫ���������� raw data��touch�¼���Camera previewͼƬ��Audioý������Ӳ����Ϣ��
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

/** �۾��豸��� */
typedef void* GlassHandle;

/** ���������� */
typedef enum SENSORID {
	/** ���ٶ� */
	ACCELEROMETER,
	/** ������ */
	MAGNETIC,
	/** ������ */
	GYROSCOPE,
};

/** �������� */
typedef enum KEY_CODE {
	/** ���ؼ� */
	BACK_KEY = 4,
	/** Ϩ���� */
	POWER_KEY = 10
};

/** �����¼����� */
typedef enum TOUCH_EVENT {
	/** ����״̬ */
	IDLE = 0,
	/** ѹ�� */
	PRESS,
	/** �̰� */
	SHORT_PRESS,
	/** ���� */
	LONG_PRESS,
	/** ��ǰ���� */
	FORWARD_SLIDE,
	/** ��󻬶� */
	BACKWARD_SLIDE,
	REPORTED
};

/** Camera ͼƬ���ݸ�ʽ */
typedef enum VIDEO_TYPE {
	YUV = 0,
	RGB,
	JPEG
};

/** Camera ͼƬ������Ϣ */
typedef struct _VidoFormat {
	/** Camera ͼƬ���� */
	int type;
	/** Camera ͼƬ�߶� */
	long height;
	/** Camera ͼƬ��� */
	long wigth;
}VidoFormat;

/** Glass�豸�����ص������ṹ�� */
typedef struct  _GlassEvent{
	/** ����IMU������������ںϵ�4Ԫ�����ݣ�timeStamp����ʱ�����quaternion����4Ԫ������ */
	void (*onRotationVectorEvent)(unsigned long timeStamp,float *quaternion);
	/** ����IMU������������ںϵ�4Ԫ�����ݣ�timeStamp����ʱ�����quaternion����4Ԫ������ */
	void (*onGameRotationVectorEvent)(unsigned long timeStamp, float* quaternion);
	/** ���ش�������ԭʼ���ݣ�timeStamp����ʱ�����sensorID����ö�ٱ���SENSORID�����ݣ�data����ǰָ��sensor���ص�ԭʼ���ݣ�status��ʾ��ǰ��������״̬ */
	void (*onRawSensorEvent)(unsigned long timeStamp,int sensorID, float* data, int status);
	/** ���ذ����¼���keyCode �豸�����Ĵ������ݣ�status true��ʾ���£�false��ʾ�ͷ�*/
	void (*onKeyEvent)(int keyCode, bool status);
	/** ���ش����¼���event �������ͣ�len ����ʱ��*/
	void (*onTouchEvent)(int event, int len);
	/** ���ؾ��봫�����¼���state true������; false��Զ�� */
	void (*onProximitySensorEvnet)(bool state);
	/** ���ع��ߴ������¼���value ����ǿ�ȣ���λlux */
	void (*onLightSensorEvent)(int value);
	/** ����CameraͼƬ���ݣ�timestamp �¼����� format ͼƬ��Ϣ  pBuf ͼƬ���� size ͼƬ��������ĳ��� */
	void (*onCaptuerEvent)(double timestamp, VidoFormat *format, BYTE* pBuf, long size);
	/** ����Audioý�������ݣ� pBuf ý�������ݣ� frames ���鳤�� */
	void (*onAudioEvent)(BYTE* pBuf, long frames);
}GlassEvent;

/***************************************************************
  *  @brief     ͨ��USBЭ���ʼ��Rokid Glass�豸���������ûص�����
  *  @param     callback Ӳ���豸����ͨ���ص��������ظ�������
  *  @return    GlassHandle ��ʼ��Rokid Glass�豸���ص��豸���
  *  @note      Rokid Glass���е�Ӳ����������ͨ��GlassEvent�Ļص��������ظ������ߣ���Ҫ��ע��ЩӲ����Ϣ�������߿�������������Ҫ�����Ļص�����
  *  @Sample usage:     nop
 **************************************************************/
 IMPORT_DLL GlassHandle GlassInitial(GlassEvent* callback);

 /***************************************************************
  *  @brief     ��Rokid Glass �豸
  *  @param     instance glass�������
  *  @return    bool true: ��ʾ�򿪳ɹ���flase�������ʧ�ܡ�
  *  @Sample usage:     nop
 **************************************************************/
 IMPORT_DLL bool GlassOpen(GlassHandle instance);

 /***************************************************************
  *  @brief     ��ȡRokid Glass����Ĺ̼��汾
  *  @param     instance glass�������
  *  @return     short ��ǰ�۾�����Ĺ̼��汾
  *  @Sample usage:     nop
 **************************************************************/
 IMPORT_DLL	short GlassGetFirmwareVersion(GlassHandle instance);

 /***************************************************************
  *  @brief     ��ȡRokid Glass�����Ψһ����
  *  @param     instance glass�������
  *  @return    wchar_t �����豸Ψһ���ţ���������һ��unicode�ַ������
  *  @Sample usage:     nop
 **************************************************************/
 IMPORT_DLL	wchar_t *GlassGetSerialNumber(GlassHandle instance);

 /***************************************************************
  *  @brief     �ȴ��豸ʱ��
  *  @param     instance glass�������
  *  @return    nop
  *  @note      �������̺߳������ȴ�Rokid GlassӲ���豸�¼��ص���
  *  @Sample usage:     nop
 **************************************************************/
 IMPORT_DLL void GlassWaitEvent(GlassHandle instance);

 /***************************************************************
  *  @brief     �ر�Rokid Glass�豸�����ӡ�
  *  @param     instance glass�������
  *  @return    nop
  *  @Sample usage:     nop
 **************************************************************/
 IMPORT_DLL void GlassClose(GlassHandle instance);

 /***************************************************************
  *  @brief     ����Rokid Glass�豸�е�Camera preview
  *  @param     instance glass�������
  *  @return    GlassHandle ��ʼ��Rokid Glass�豸���ص��豸���
  *  @note      ��ʼ����������Camera��Camera��preview����ͨ���ص�����onCaptuerEvnet��ͼ�����ݷ��ظ������ߡ�
  *  @Sample usage:     nop
 **************************************************************/
 IMPORT_DLL void GlassStartCapture(GlassHandle instance);

 /***************************************************************
  *  @brief     ֹͣRokid Glass�豸�е�Camera preview
  *  @param     instance glass�������
  *  @return    GlassHandle ��ʼ��Rokid Glass�豸���ص��豸���
  *  @note      �ر�Camera�豸
  *  @Sample usage:     nop
 **************************************************************/
 IMPORT_DLL void GlassStopCapture(GlassHandle instance);
#ifdef __cplusplus
}
#endif
