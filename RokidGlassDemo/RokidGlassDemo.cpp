// RokidGlassDemo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>

#include <glass.h>
#pragma comment(lib, "RokidGlassSDK.lib")

using namespace std;

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

typedef struct                       /**** BMP file header structure ****/
{
	unsigned int   bfSize;           /* Size of file */
	unsigned short bfReserved1;      /* Reserved */
	unsigned short bfReserved2;      /* ... */
	unsigned int   bfOffBits;        /* Offset to bitmap data */
} MyBITMAPFILEHEADER;

typedef struct                       /**** BMP file info structure ****/
{
	unsigned int   biSize;           /* Size of info header */
	int            biWidth;          /* Width of image */
	int            biHeight;         /* Height of image */
	unsigned short biPlanes;         /* Number of color planes */
	unsigned short biBitCount;       /* Number of bits per pixel */
	unsigned int   biCompression;    /* Type of compression to use */
	unsigned int   biSizeImage;      /* Size of image data */
	int            biXPelsPerMeter;  /* X pixels per meter */
	int            biYPelsPerMeter;  /* Y pixels per meter */
	unsigned int   biClrUsed;        /* Number of colors used */
	unsigned int   biClrImportant;   /* Number of important colors */
} MyBITMAPINFOHEADER;

void saveBmp(const char* filename, unsigned char* rgbbuf, int width, int height)
{
	MyBITMAPFILEHEADER bfh;
	MyBITMAPINFOHEADER bih;
	/* Magic number for file. It does not fit in the header structure due to alignment requirements, so put it outside */
	unsigned short bfType = 0x4d42;
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfSize = 2 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + width * height * 3;
	bfh.bfOffBits = 0x36;

	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = width;
	bih.biHeight = height;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = 0;
	bih.biSizeImage = 0;
	bih.biXPelsPerMeter = 5000;
	bih.biYPelsPerMeter = 5000;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	FILE* file = fopen(filename, "wb");
	if (!file)
	{
		printf("Could not write file\n");
		return;
	}

	/*Write headers*/
	fwrite(&bfType, sizeof(bfType), 1, file);
	fwrite(&bfh, sizeof(bfh), 1, file);
	fwrite(&bih, sizeof(bih), 1, file);

	fwrite(rgbbuf, width * height * 3, 1, file);
	fclose(file);
}

void savePicture(const char* filename, unsigned char* rgbbuf, int size) {
	ofstream out(filename);

	if (out.is_open()) {
		out.write((char *)rgbbuf, size);
	}

	out.close();
}



void onCaptureEvent(double timestamp, VideoFormat* format, BYTE* pBuf, long size) {

	const char* fileName;

	if (format->type == JPEG) {
		savePicture("cameraPreview.jpeg", pBuf, size);
	}
	else if (format->type == RGB) {
		//saveBmp("cameraPreview.bmp", pBuf, format->width, format->height);
		savePicture("cameraPreview.jpeg", pBuf, size);
	}
	else if (format->type == YUV) {
		savePicture("cameraPreview.jpeg", pBuf, size);
	}
	else {
		savePicture("unkown.yuv", pBuf, size);
	}

}

void onAudioEvent(BYTE* pBuf, long frames) {
	const char* fileName = "audio.wav";

	ofstream out(fileName);

	if (out.is_open()) {
		out.write((char*)pBuf, frames);
	}
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
	GlassWaitEvent(glass);

	GlassStopCapture(glass);
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
