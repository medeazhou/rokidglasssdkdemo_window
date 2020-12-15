#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "stdbool.h"

#ifdef IMPORT_DLL
#else
#define IMPORT_DLL  __declspec(dllimport)
#endif

typedef void* GlassHandle;

typedef enum SENSORID {
	ACCELEROMETER,
	MAGNETIC,
	GYROSCOPE,
};

typedef enum TOUCH_EVENT {
	IDLE = 0,
	PRESS,
	SHORT_PRESS,
	LONG_PRESS,
	FORWARD_SLIDE,
	BACKWARD_SLIDE,
	REPORTED
};

typedef struct  _GlassEvent{
	void (*onRotationVectorEvent)(unsigned long timeStamp,float *quaternion);
	void (*onGameRotationVectorEvent)(unsigned long timeStamp, float* quaternion);
	void (*onRawSensorEvent)(unsigned long timeStamp,int sensorID, float* data, int status);
	void (*onKeyEvent)(int keyCode, bool status);
	void (*onTouchEvent)(int event, int len);
	void (*onProximitySensorEvnet)(bool state);
	void (*onLightSensorEvent)(int value);
}GlassEvent;



 IMPORT_DLL GlassHandle GlassInitial(GlassEvent* callback);
 IMPORT_DLL bool GlassOpen(GlassHandle instance);
 IMPORT_DLL void GlassWaitEvent(GlassHandle instance);
 IMPORT_DLL void GlassClose(GlassHandle instance);

#ifdef __cplusplus
}
#endif
