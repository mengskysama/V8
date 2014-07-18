#define SELF_TEN_FRAME   //使用自定义10帧每秒 每100毫秒取一帧图像
//#define SELF_EIGHT_FRAME   //使用8帧每秒
//#define SELF_FIVE_FRAME    //使用5帧每秒
// #define SELF_TWELVE_FRAME  //使用12帧每秒

#define SELF_CONTROL_VIDEO //是否自己控制视频帧数 把帧数调整到30帧每秒
//#define SELF_TEST_NO_AUDIO //不发送语音
#define IGNOR_CRC_CHECK  1

#ifndef _DEBUG
#define IGNOR_INT_3
#endif

