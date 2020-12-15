// RokidGlassDemo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <glass.h>
#pragma comment(lib, "RokidGlassSDK.lib")
/*Glass 上报事件的回调函数*/
void onRotationVectorEvent(unsigned long timeStamp, float* quaternion) {
	printf("Rotation Vector:%.08f %.08f %.08f %.08f\r\n", quaternion[0], quaternion[1],
		quaternion[2], quaternion[3]);
}
int main()
{
	std::cout << "Hello World!\n";
	static GlassEvent EventCallback = { 0 };
	GlassHandle glass;
	EventCallback.onRotationVectorEvent = onRotationVectorEvent;
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
	GlassWaitEvent(glass);
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
