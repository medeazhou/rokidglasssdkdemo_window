// RokidGlassDemo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>

#include <glass.h>
#pragma comment(lib, "RokidGlassSDK.lib")

using namespace std;


static int frameSize;


void SavePicture(float timestamp, BYTE* buf, long len) {
	char path[128] = { 0 };
	FILE* fp;
	sprintf_s(path, "%f.jpg", timestamp);
	fopen_s(&fp, path, "wb");
	if (fp) {
		fwrite(buf, 1, len, fp);
		fclose(fp);
	}
}


void SaveVideo(float timestamp, BYTE* buf, long len) {
	static FILE* fp = NULL;
	if (!fp)
		fopen_s(&fp, "cap.mjpg", "wb");
	if (fp) {
		fwrite(buf, 1, len, fp);
		fflush(fp);
	}
}

void SaveWav(BYTE* buf, long frames) {
	static FILE* fp = NULL;

	if (!fp) {
		fopen_s(&fp, "mic1.raw", "wb");
	}

	if (fp) {
		fwrite(buf, 1, frames * frameSize, fp);
		fflush(fp);
	}
}

/*Glass 上报事件的回调函数*/
void onRotationVectorEvent(unsigned long timeStamp, float* quaternion) {
	printf("Rotation Vector:%.08f %.08f %.08f %.08f\r\n", quaternion[0], quaternion[1],
		quaternion[2], quaternion[3]);
}

void onGameRotationVectorEvent(unsigned long timeStamp, float* quaternion) {
	printf("GameRotation Vector:%.08f %.08f %.08f %.08f\r\n", quaternion[0], quaternion[1],
		quaternion[2], quaternion[3]);
}

void onRawSensorEvent(unsigned long timeStamp, int sensorID, float* data, int status) {
	if (sensorID == MAGNETIC)
		printf("Magnetic", "%08f	%08f	%08f	%d", data[0], data[1], data[2], status);
	else if (sensorID == ACCELEROMETER)
		printf("Accelemeter", "%08f	%08f	%08f	%d", data[0], data[1], data[2], status);
	else if (sensorID == GYROSCOPE)
		printf("Gyroscope", "%08f	%08f	%08f	%d", data[0], data[1], data[2], status);
}

void onKeyEvent(int keyCode, bool status) {
	printf("Key Code", "%02x	%s", keyCode, status ? "Press" : "Release");
}

void onTouchEvent(int eventType, int len) {
	printf("Touch Event", "%02x	%d", eventType, len);
}

void onProximitySensorEvent(bool state) {
	printf("Proximity Sensor", "%s", state ? "true" : "false");
}

void onLightSensorEvent(int value) {
	printf("Light Sensor", "%d", value);
}


void onCaptureEvent(double timestamp, VideoFormat* format, BYTE* pBuf, long size) {

	printf("Camera Data: ts:%f	w=%ld	h=%ld	len:%d	data:%02x", timestamp, format->width, format->height, size, pBuf[4096]);
	SavePicture(timestamp, pBuf, size);
}

void onAudioEvent(BYTE* pBuf, long frames) {
	printf("Audio Data: w=%ld  d=%04x", frames, *(UINT16*)pBuf);
	SaveWav(pBuf, frames * frameSize);
}


int main()
{
	static GlassEvent EventCallback = { 0 };
	GlassHandle glass;

	EventCallback.onRotationVectorEvent = onRotationVectorEvent;
	EventCallback.onGameRotationVectorEvent = onGameRotationVectorEvent;
	EventCallback.onRawSensorEvent = onRawSensorEvent;
	EventCallback.onKeyEvent = onKeyEvent;
	EventCallback.onTouchEvent = onTouchEvent;
	EventCallback.onProximitySensorEvent = onProximitySensorEvent;
	EventCallback.onLightSensorEvent = onLightSensorEvent;
	EventCallback.onCaptureEvent = onCaptureEvent;
	EventCallback.onAudioEvent = onAudioEvent;

	glass = GlassInitial(&EventCallback);
	if (!glass) {
		printf("Unable Initial glass!");
		return -1;
	}
	while (true) {
		printf("try to open glass...");
		if (GlassOpen(glass) == true)
			break;
		Sleep(1000);
	}
	printf("open glass sucess!\n");

	printf("open glass sucess: version=%d sn=%s\n", GlassGetFirmwareVersion(glass), GlassGetSerialNumber(glass));

	GlassStartCapture(glass);
	GlassStartAudio(glass);
	frameSize = GlassGetAudioFrameSize(glass);
	GlassWaitEvent(glass);

	GlassStopCapture(glass);
	GlassStopAudio(glass);
	while (true) {
		Sleep(1000);
	}

	GlassClose(glass);
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
