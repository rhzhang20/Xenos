#ifndef SENSENET_SENSE_NET_H_
#define SENSENET_SENSE_NET_H_


#include "common.h"
#include "commondef.h"
extern "C"
{
#include <dsplib.h>
}

extern "C" {
#include "SRIO/SRIO_Test.h"
}

extern volatile Uint32 RecvNum;

//�ֿ���ԣ��������У���ͼ��߶Ȼ���Ϊ7�飬�ֱ���м��㡣
									// con              con             con        10(5x2)                 conv
const int img_height[20]=     {224,112,112,112,56 ,56 ,56 ,56 ,28 ,28 ,28 ,28 ,14 ,14 ,14 ,7   ,7   ,7   ,7   ,7   };
const int img_width[20]=      {224,112,112,112,56 ,56 ,56 ,56 ,28 ,28 ,28 ,28 ,14 ,14 ,14 ,7   ,7   ,7   ,7   ,7   };
const int input_channel[20]=  {3  ,32 ,32 ,64 ,64 ,128,128,128,128,256,256,256,256,512,512,512 ,1024,1024,1024,1024};
const int output_channel[20]= {32 ,32 ,64 ,64 ,128,128,128,128,256,256,256,256,512,512,512,1024,1024,512 ,512 ,1024};

//				//�ֿ���ԣ��������У���ͼ��߶Ȼ���Ϊ7�飬�ֱ���м��㡣
//													// con              con             con        10(5x2)                 conv
//				const int img_height[20]=     {224,112,112,112,56 ,56 ,56 ,56 ,28 ,28 ,28 ,28 ,14 ,14 ,14 ,7   ,7   ,7   ,7   ,7   };
//				const int img_width[20]=      {224,112,112,112,56 ,56 ,56 ,56 ,28 ,28 ,28 ,28 ,14 ,14 ,14 ,7   ,7   ,7   ,7   ,7   };
//				const int input_channel[20]=  {3  ,32 ,32 ,64 ,64 ,128,128,128,128,256,256,256,256,512,512,512 ,1024,1024,1024,1024};
//				const int output_channel[20]= {32 ,32 ,64 ,64 ,128,128,128,128,256,256,256,256,512,512,512,1024,1024,512 ,512 ,1024};
//                                              0   1   2   3   4   5  6   7    8   9   10  11  12  13  23  24   25   26  27    28

//�ֿ����˵���������1��������ʾͼ����ȣ���2��������ʾͼ��ĸ߶ȣ���3��������ʾͼ��Padingǰʵ�ʷֿ�λ�á�
const int img_tiling[28][4][3] =
{
    { {226, 58, 0}, {226, 58, 56}, {226, 58, 112}, { 226, 58, 168}},
    { {114, 30, 0}, {114, 30, 28}, {114, 30, 56}, { 114, 30, 84}},
    { {112, 28, 0}, {112, 28, 28}, {112, 28, 56}, { 112, 28, 84}},//1*1
    { {114, 30, 0}, {114, 30, 28}, {114, 30, 56}, { 114, 30, 84}},
    { {56 , 14, 0}, {56 , 14, 14}, {56 , 14, 28}, { 56 , 14, 42}},//1*1
    { {58 , 16, 0}, {58 , 16, 14}, {58 , 16, 28}, { 58 , 16, 42}},
    { {58 , 16, 0}, {58 , 16, 14}, {58 , 16, 28}, { 58 , 16, 42}},//1*1
    { {58 , 16, 0}, {58 , 16, 14}, {58 , 16, 28}, { 58 , 16, 42}},
    { {28 , 7 , 0}, {28 , 7 , 7}, {28 , 7 , 14}, { 28 , 7 , 21}},//1*1
    { {30 , 9 , 0}, {30 , 9 , 7}, {30 , 9 , 14}, { 30 , 9 , 21}},
    { {28 , 7 , 0}, {28 , 7 , 7}, {28 , 7 , 14}, { 28 , 7 , 21}},//1*1
    { {30 , 9 , 0}, {30 , 9 , 7}, {30 , 9 , 14}, { 30 , 9 , 21}},//������һ�����Ҫȫ������step == 11
    { {14 , 4 , 0}, {14 , 3 , 4}, {14 , 4 , 7}, { 14 , 3 , 11}},//1*1
	{ {16 , 6 , 0}, {16 , 5 , 4}, {16 , 6 , 7}, { 16 , 5 , 11}},
    { {14 , 4 , 0}, {14 , 3 , 4}, {14 , 4 , 7}, { 14 , 3 , 11}},//1*1
	{ {16 , 6 , 0}, {16 , 5 , 4}, {16 , 6 , 7}, { 16 , 5 , 11}},
    { {14 , 4 , 0}, {14 , 3 , 4}, {14 , 4 , 7}, { 14 , 3 , 11}},//1*1
	{ {16 , 6 , 0}, {16 , 5 , 4}, {16 , 6 , 7}, { 16 , 5 , 11}},
    { {14 , 4 , 0}, {14 , 3 , 4}, {14 , 4 , 7}, { 14 , 3 , 11}},//1*1
	{ {16 , 6 , 0}, {16 , 5 , 4}, {16 , 6 , 7}, { 16 , 5 , 11}},
    { {14 , 4 , 0}, {14 , 3 , 4}, {14 , 4 , 7}, { 14 , 3 , 11}},//1*1
	{ {16 , 6 , 0}, {16 , 5 , 4}, {16 , 6 , 7}, { 16 , 5 , 11}},
    { {14 , 4 , 0}, {14 , 3 , 4}, {14 , 4 , 7}, { 14 , 3 , 11}},//1*1   step=22

	{ {16 , 6 , 0}, {16 , 5 , 4}, {16 , 6 , 7}, { 16 , 5 , 11}},
    { {7 , 2 , 0}, {7 , 2 , 2}, {7 , 2 , 4}, { 7 , 1 , 6}},//1*1
	{ {15 , 7 , 0}, {15 , 4 , 2}, {15 , 4 , 4}, { 15 , 6 , 6}},
	{ {7 , 3 , 0}, {7 , 1 , 3}, {7 , 1 , 4}, { 7 , 2 , 5}},//1*1
	{ {7 , 3 , 0}, {7 , 1 , 3}, {7 , 1 , 4}, { 7 , 2 , 5}}//1*1
};

//const int channel_group[20][8][4] =
//{
//	{ { 4, 0, 0, 0 },{ 4, 4, 0, 0 },{ 4, 8, 0, 0 },{ 4, 12, 0, 0 },{ 4, 16, 0, 0 },{ 4, 20, 0, 0 },{ 4, 24, 0, 0 },{ 4, 28, 0, 0 } },
//	{ { 4, 0, 0, 0 },{ 4, 4, 0, 0 },{ 4, 8, 0, 0 },{ 4, 12, 0, 0 },{ 4, 16, 0, 0 },{ 4, 20, 0, 0 },{ 4, 24, 0, 0 },{ 4, 28, 0, 0 } },
//	{ { 0, 0, 8, 0},{ 0, 0, 8, 8 },{ 0, 0, 8, 16 },{ 0, 0, 8, 24 },{ 0, 0, 8, 32 },{ 0, 0, 8, 40 },{ 0, 0, 8, 48 },{ 0, 0, 8, 56 } },
//	{ { 8, 0, 0, 0},{ 8, 8, 0, 0 },{ 8, 16, 0, 0 },{ 8, 24, 0, 0 },{ 8, 32, 0, 0 },{ 8, 40, 0, 0 },{ 8, 48, 0, 0 },{ 8, 56, 0, 0 } },
//	{ { 0, 0, 16, 0},{ 0, 0, 16, 16 },{ 0, 0, 16, 32 },{ 0, 0, 16, 48 },{ 0, 0, 16, 64 },{ 0, 0, 16, 80 },{ 0, 0, 16, 96 },{ 0, 0, 16, 112 } },
//	{ { 16, 0, 0, 0},{ 16, 16, 0, 0 },{ 16, 32, 0, 0 },{ 16, 48, 0, 0 },{ 16, 64, 0, 0 },{ 16, 80, 0, 0 },{ 16, 96, 0, 0 },{ 16, 112, 0, 0 } },
//	{ { 0, 0, 16, 0},{ 0, 0, 16, 16 },{ 0, 0, 16, 32 },{ 0, 0, 16, 48 },{ 0, 0, 16, 64 },{ 0, 0, 16, 80 },{ 0, 0, 16, 96 },{ 0, 0, 16, 112 } },
//	{ { 16, 0, 0, 0},{ 16, 16, 0, 0 },{ 16, 32, 0, 0 },{ 16, 48, 0, 0 },{ 16, 64, 0, 0 },{ 16, 80, 0, 0 },{ 16, 96, 0, 0 },{ 16, 112, 0, 0 } },
//	{ { 0, 0, 32, 0},{ 0, 0, 32, 32 },{ 0, 0, 32, 64 },{ 0, 0, 32, 96 },{ 0, 0, 32, 128 },{ 0, 0, 32, 160 },{ 0, 0, 32, 192 },{ 0, 0, 32, 224 } },
//	{ { 32, 0, 0, 0},{ 32, 32, 0, 0 },{ 32, 64, 0, 0 },{ 32, 96 , 0, 0 },{ 32, 128, 0, 0 },{ 32, 160, 0, 0 },{32, 192, 0, 0 },{ 32, 224, 0, 0 } },
//	{ { 0, 0, 32, 0},{ 0, 0, 32, 32 },{ 0, 0, 32, 64 },{ 0, 0, 32, 96 },{ 0, 0, 32, 128 },{ 0, 0, 32, 160 },{ 0, 0, 32, 192 },{ 0, 0, 32, 224 } },
//	{ { 32, 0, 0, 0},{ 32, 32, 0, 0 },{ 32, 64, 0, 0 },{ 32, 96 , 0, 0 },{ 32, 128, 0, 0 },{ 32, 160, 0, 0 },{32, 192, 0, 0 },{ 32, 224, 0, 0 } },
//	{ { 0, 0, 64, 0},{ 0, 0, 64, 64 },{ 0, 0, 64, 128 },{ 0, 0, 64, 192 },{ 0, 0, 64, 256 },{ 0, 0, 64, 320 },{ 0, 0, 64, 384 },{ 0, 0, 64, 448 } },
//	{ { 64, 0, 64, 0},{ 64, 64, 64, 64 },{ 64, 128, 64, 128 },{ 64, 192, 64, 192 },{ 64, 256, 64, 256 },{ 64, 320, 64, 320 },{ 64, 384, 64, 384 },{ 64, 448, 64, 448 } },
//	{ { 64, 0, 0, 0},{ 64, 64, 0, 0 },{ 64, 128, 0, 0 },{ 64, 192, 0, 0 },{ 64, 256, 0, 0 },{ 64, 320, 0, 0 },{ 64, 384, 0, 0 },{ 64, 448, 0, 0 } },
//	{ { 0, 0, 128, 0},{ 0, 0, 128, 128 },{ 0, 0, 128, 256 },{ 0, 0, 128, 384 },{ 0, 0, 128, 512 },{ 0, 0, 128, 640 },{ 0, 0, 128, 768 },{ 0, 0, 128, 896 } },
//	{ { 128, 0, 0, 0},{ 128, 128, 0, 0 },{ 128, 256, 0, 0 },{ 128, 384, 0, 0 },{ 128, 512, 0, 0 },{ 128, 640, 0, 0 },{ 128, 768, 0, 0 },{ 128, 896, 0, 0 } },
//	{ { 0, 0, 64, 0},{ 0, 0, 64, 64 },{ 0, 0, 64, 128 },{ 0, 0, 64, 192 },{ 0, 0, 64, 256 },{ 0, 0, 64, 320 },{ 0, 0, 64, 384 },{ 0, 0, 64, 448 } },
//	{ { 0, 0, 64, 0},{ 0, 0, 64, 64 },{ 0, 0, 64, 128 },{ 0, 0, 64, 192 },{ 0, 0, 64, 256 },{ 0, 0, 64, 320 },{ 0, 0, 64, 384 },{ 0, 0, 64, 448 } },
////	{ { 0, 0, 64*2, 0},{ 0, 0, 64*2, 64*2 },{ 0, 0, 64*2, 128*2 },{ 0, 0, 64*2, 192*2 },{ 0, 0, 64*2, 256*2 },{ 0, 0, 64*2, 320*2 },{ 0, 0, 64*2, 384*2 },{ 0, 0, 64*2, 448*2 } },
////	{ { 0, 0, 64*2, 0},{ 0, 0, 64*2, 64*2 },{ 0, 0, 64*2, 128*2 },{ 0, 0, 64*2, 192*2 },{ 0, 0, 64*2, 256*2 },{ 0, 0, 64*2, 320*2 },{ 0, 0, 64*2, 384*2 },{ 0, 0, 64*2, 448*2 } },
//	{ { 128, 0, 0, 0},{ 128, 128, 0, 0 },{ 128, 256, 0, 0 },{ 128, 384, 0, 0 },{ 128, 512, 0, 0 },{ 128, 640, 0, 0 },{ 128, 768, 0, 0 },{ 128, 896, 0, 0 } },
//};

const int channel_group[20][8][4] =
{
	{ { 4, 0, 0, 0 },{ 4, 4, 0, 0 },{ 4, 8, 0, 0 },{ 4, 12, 0, 0 },{ 4, 16, 0, 0 },{ 4, 20, 0, 0 },{ 4, 24, 0, 0 },{ 4, 28, 0, 0 } },
	{ { 4, 0, 0, 0 },{ 4, 4, 0, 0 },{ 4, 8, 0, 0 },{ 4, 12, 0, 0 },{ 4, 16, 0, 0 },{ 4, 20, 0, 0 },{ 4, 24, 0, 0 },{ 4, 28, 0, 0 } },
	{ { 0, 0, 8, 0},{ 0, 0, 8, 8 },{ 0, 0, 8, 16 },{ 0, 0, 8, 24 },{ 0, 0, 8, 32 },{ 0, 0, 8, 40 },{ 0, 0, 8, 48 },{ 0, 0, 8, 56 } },
	{ { 8, 0, 0, 0},{ 8, 8, 0, 0 },{ 8, 16, 0, 0 },{ 8, 24, 0, 0 },{ 8, 32, 0, 0 },{ 8, 40, 0, 0 },{ 8, 48, 0, 0 },{ 8, 56, 0, 0 } },
	{ { 0, 0, 16, 0},{ 0, 0, 16, 16 },{ 0, 0, 16, 32 },{ 0, 0, 16, 48 },{ 0, 0, 16, 64 },{ 0, 0, 16, 80 },{ 0, 0, 16, 96 },{ 0, 0, 16, 112 } },
	{ { 16, 0, 0, 0},{ 16, 16, 0, 0 },{ 16, 32, 0, 0 },{ 16, 48, 0, 0 },{ 16, 64, 0, 0 },{ 16, 80, 0, 0 },{ 16, 96, 0, 0 },{ 16, 112, 0, 0 } },
	{ { 0, 0, 16, 0},{ 0, 0, 16, 16 },{ 0, 0, 16, 32 },{ 0, 0, 16, 48 },{ 0, 0, 16, 64 },{ 0, 0, 16, 80 },{ 0, 0, 16, 96 },{ 0, 0, 16, 112 } },
	{ { 16, 0, 0, 0},{ 16, 16, 0, 0 },{ 16, 32, 0, 0 },{ 16, 48, 0, 0 },{ 16, 64, 0, 0 },{ 16, 80, 0, 0 },{ 16, 96, 0, 0 },{ 16, 112, 0, 0 } },
	{ { 0, 0, 32, 0},{ 0, 0, 32, 32 },{ 0, 0, 32, 64 },{ 0, 0, 32, 96 },{ 0, 0, 32, 128 },{ 0, 0, 32, 160 },{ 0, 0, 32, 192 },{ 0, 0, 32, 224 } },
	{ { 32, 0, 0, 0},{ 32, 32, 0, 0 },{ 32, 64, 0, 0 },{ 32, 96 , 0, 0 },{ 32, 128, 0, 0 },{ 32, 160, 0, 0 },{32, 192, 0, 0 },{ 32, 224, 0, 0 } },
	{ { 0, 0, 32, 0},{ 0, 0, 32, 32 },{ 0, 0, 32, 64 },{ 0, 0, 32, 96 },{ 0, 0, 32, 128 },{ 0, 0, 32, 160 },{ 0, 0, 32, 192 },{ 0, 0, 32, 224 } },
	{ { 32, 0, 0, 0},{ 32, 32, 0, 0 },{ 32, 64, 0, 0 },{ 32, 96 , 0, 0 },{ 32, 128, 0, 0 },{ 32, 160, 0, 0 },{32, 192, 0, 0 },{ 32, 224, 0, 0 } },
	{ { 0, 0, 16, 0+128*DeviceId},{ 0, 0, 16, 16+128*DeviceId },{ 0, 0, 16, 32+128*DeviceId },{ 0, 0, 16, 48+128*DeviceId },{ 0, 0, 16, 64+128*DeviceId },{ 0, 0, 16, 80+128*DeviceId },{ 0, 0, 16, 96+128*DeviceId },{ 0, 0, 16, 112+128*DeviceId } },
	{ { 16, 0+128*DeviceId, 16, 0+128*DeviceId},{ 16, 16+128*DeviceId, 16, 16 +128*DeviceId},{ 16, 32+128*DeviceId, 16, 32+128*DeviceId},{ 16, 48+128*DeviceId, 16, 48+128*DeviceId },{ 16, 64+128*DeviceId, 16, 64+128*DeviceId },{ 16, 80+128*DeviceId, 16, 80+128*DeviceId },{ 16, 96+128*DeviceId, 16, 96+128*DeviceId },{ 16, 112+128*DeviceId, 16, 112+128*DeviceId } },
	{ { 16, 0+128*DeviceId, 0, 0},{ 16, 16+128*DeviceId, 0, 0 },{ 16, 32+128*DeviceId, 0, 0 },{ 16, 48+128*DeviceId, 0, 0 },{ 16, 64+128*DeviceId, 0, 0 },{ 16, 80+128*DeviceId, 0, 0 },{ 16, 96+128*DeviceId, 0, 0 },{ 16, 112+128*DeviceId, 0, 0 } },
	{ { 0, 0,  32, 0+256*DeviceId},{ 0, 0, 32, 32+256*DeviceId },{ 0, 0, 32, 64+256*DeviceId },{ 0, 0, 32, 96+256*DeviceId },{ 0, 0, 32, 128+256*DeviceId },{ 0, 0, 32, 160+256*DeviceId },{ 0, 0, 32, 192+256*DeviceId },{ 0, 0, 32, 224+256*DeviceId} },
	{ { 32, 0+256*DeviceId, 0, 0},{ 32, 32+256*DeviceId, 0, 0 },{ 32, 64+256*DeviceId, 0, 0 },{ 32, 96+256*DeviceId, 0, 0 },{ 32, 128+256*DeviceId, 0, 0 },{ 32, 160+256*DeviceId, 0, 0 },{ 32, 192+256*DeviceId, 0, 0 },{ 32, 224+256*DeviceId, 0, 0 } },
	{ { 0, 0, 16, 0+128*DeviceId},{ 0, 0, 16, 16+128*DeviceId },{ 0, 0, 16, 32+128*DeviceId },{ 0, 0, 16, 48+128*DeviceId },{ 0, 0, 16, 64+128*DeviceId },{ 0, 0, 16, 80+128*DeviceId },{ 0, 0, 16, 96+128*DeviceId },{ 0, 0, 16, 112+128*DeviceId } },
	{ { 0, 0, 16, 0+128*DeviceId},{ 0, 0, 16, 16+128*DeviceId },{ 0, 0, 16, 32+128*DeviceId },{ 0, 0, 16, 48+128*DeviceId },{ 0, 0, 16, 64+128*DeviceId },{ 0, 0, 16, 80+128*DeviceId },{ 0, 0, 16, 96+128*DeviceId },{ 0, 0, 16, 112+128*DeviceId } },
	{ { 32, 0+256*DeviceId, 0, 0},{ 32, 32+256*DeviceId, 0, 0 },{ 32, 64+256*DeviceId, 0, 0 },{ 32, 96+256*DeviceId, 0, 0 },{ 32, 128+256*DeviceId, 0, 0 },{ 32, 160+256*DeviceId, 0, 0 },{ 32, 192+256*DeviceId, 0, 0 },{ 32, 224+256*DeviceId, 0, 0 } }
};


//const int channel_group[20][8][4] =
//{
//	{ { 1, 0, 0, 0 },{ 1, 1, 0, 0 },{ 1, 2, 0, 0 },{ 1, 3, 0, 0 },{ 1, 4, 0, 0 },{ 1, 5, 0, 0 },{ 1, 6, 0, 0 },{ 1, 8, 0, 0 } },
//	{ { 1, 0, 0, 0 },{ 1, 1, 0, 0 },{ 1, 2, 0, 0 },{ 1, 3, 0, 0 },{ 1, 4, 0, 0 },{ 1, 5, 0, 0 },{ 1, 6, 0, 0 },{ 1, 8, 0, 0 } },
//	{ { 0, 0, 2, 0},{ 0, 0, 2, 2 },{ 0, 0, 2, 4 },{ 0, 0, 2, 6 },{ 0, 0, 2, 8 },{ 0, 0, 2, 10 },{ 0, 0, 2, 12 },{ 0, 0, 2, 14 } },
//	{ { 2, 0, 0, 0},{ 2, 2, 0, 0 },{ 2, 4, 0, 0 },{ 2, 6, 0, 0 },{ 2, 8, 0, 0 },{ 2, 10, 0, 0 },{ 2, 12, 0, 0 },{ 2, 14, 0, 0 } },
//	{ { 0, 0, 4, 0},{ 0, 0, 4, 4 },{ 0, 0, 4, 8 },{ 0, 0, 4, 12 },{ 0, 0, 4, 16 },{ 0, 0, 4, 20 },{ 0, 0, 4, 24 },{ 0, 0, 4, 28 } },
//	{ { 4, 0, 0, 0},{ 4, 4, 0, 0 },{ 4, 8, 0, 0 },{ 4, 12, 0, 0 },{ 4, 16, 0, 0 },{ 4, 20, 0, 0 },{ 4, 24, 0, 0 },{ 4, 28, 0, 0 } },
//	{ { 0, 0, 4, 0},{ 0, 0, 4, 4 },{ 0, 0, 4, 8 },{ 0, 0, 4, 12 },{ 0, 0, 4, 16 },{ 0, 0, 4, 20 },{ 0, 0, 4, 24 },{ 0, 0, 4, 28 } },
//	{ { 4, 0, 0, 0},{ 4, 4, 0, 0 },{ 4, 8, 0, 0 },{ 4, 12, 0, 0 },{ 4, 16, 0, 0 },{ 4, 20, 0, 0 },{ 4, 24, 0, 0 },{ 4, 28, 0, 0 } },
//	{ { 0, 0, 8, 0},{ 0, 0, 8, 8 },{ 0, 0, 8, 16 },{ 0, 0, 8, 24 },{ 0, 0, 8, 32 },{ 0, 0, 8, 40 },{ 0, 0, 8, 48 },{ 0, 0, 8, 56 } },
//	{ { 8, 0, 0, 0},{ 8, 8, 0, 0 },{ 8, 16, 0, 0 },{ 8, 24, 0, 0 },{ 8, 32, 0, 0 },{ 8, 40, 0, 0 },{ 8, 48, 0, 0 },{ 8, 56, 0, 0 } },
//	{ { 0, 0, 8, 0},{ 0, 0, 8, 8 },{ 0, 0, 8, 16 },{ 0, 0, 8, 24 },{ 0, 0, 8, 32 },{ 0, 0, 8, 40 },{ 0, 0, 8, 48 },{ 0, 0, 8, 56 } },
//	{ { 8, 0, 0, 0},{ 8, 8, 0, 0 },{ 8, 16, 0, 0 },{ 8, 24, 0, 0 },{ 8, 32, 0, 0 },{ 8, 40, 0, 0 },{ 8, 48, 0, 0 },{ 8, 56, 0, 0 } },
//	{ { 0, 0, 16, 0},{ 0, 0, 16, 16 },{ 0, 0, 16, 32 },{ 0, 0, 16, 48 },{ 0, 0, 16, 64 },{ 0, 0, 16, 80 },{ 0, 0, 16, 96 },{ 0, 0, 16, 112 } },
//	{ { 16, 0, 16, 0},{ 16, 16, 16, 16 },{ 16, 32, 16, 32 },{ 16, 48, 16, 48 },{ 16, 64, 16, 64 },{ 16, 80, 16, 80 },{ 16, 96, 16, 96 },{ 16, 112, 16, 112 } },
//	{ { 16, 0, 0, 0},{ 16, 16, 0, 0 },{ 16, 32, 0, 0 },{ 16, 48, 0, 0 },{ 16, 64, 0, 0 },{ 16, 80, 0, 0 },{ 16, 96, 0, 0 },{ 16, 112, 0, 0 } },
//	{ { 0, 0,  32, 0},{ 0, 0, 32, 32 },{ 0, 0, 32, 64 },{ 0, 0, 32, 96 },{ 0, 0, 32, 128 },{ 0, 0, 32, 160 },{ 0, 0, 32, 192 },{ 0, 0, 32, 224} },
//	{ { 32, 0, 0, 0},{ 32, 32, 0, 0 },{ 32, 64, 0, 0 },{ 32, 96, 0, 0 },{ 32, 128, 0, 0 },{ 32, 160, 0, 0 },{ 32, 192, 0, 0 },{ 32, 224, 0, 0 } },
//	{ { 0, 0, 16, 0},{ 0, 0, 16, 16 },{ 0, 0, 16, 32 },{ 0, 0, 16, 48 },{ 0, 0, 16, 64 },{ 0, 0, 16, 80 },{ 0, 0, 16, 96 },{ 0, 0, 16, 112 } },
//	{ { 0, 0, 16, 0},{ 0, 0, 16, 16 },{ 0, 0, 16, 32 },{ 0, 0, 16, 48 },{ 0, 0, 16, 64 },{ 0, 0, 16, 80 },{ 0, 0, 16, 96 },{ 0, 0, 16, 112 } },
//	{ { 32, 0, 0, 0},{ 32, 32, 0, 0 },{ 32, 64, 0, 0 },{ 32, 96, 0, 0 },{ 32, 128, 0, 0 },{ 32, 160, 0, 0 },{ 32, 192, 0, 0 },{ 32, 224, 0, 0 } },
//};


//const int channel_group[20][8][4] =
//{
//	{ { 1, 0+8*DeviceId, 0, 0 },{ 1, 1+8*DeviceId, 0, 0 },{ 1, 2+8*DeviceId, 0, 0 },{ 1, 3+8*DeviceId, 0, 0 },{ 1, 4+8*DeviceId, 0, 0 },{ 1, 5+8*DeviceId, 0, 0 },{ 1, 6+8*DeviceId, 0, 0 },{ 1, 8+8*DeviceId, 0, 0 } },
//	{ { 1, 0+8*DeviceId, 0, 0 },{ 1, 1+8*DeviceId, 0, 0 },{ 1, 2+8*DeviceId, 0, 0 },{ 1, 3+8*DeviceId, 0, 0 },{ 1, 4+8*DeviceId, 0, 0 },{ 1, 5+8*DeviceId, 0, 0 },{ 1, 6+8*DeviceId, 0, 0 },{ 1, 8+8*DeviceId, 0, 0 } },
//	{ { 0, 0, 2, 0+16*DeviceId},{ 0, 0, 2, 2+16*DeviceId },{ 0, 0, 2, 4+16*DeviceId },{ 0, 0, 2, 6+16*DeviceId },{ 0, 0, 2, 8+16*DeviceId },{ 0, 0, 2, 10+16*DeviceId },{ 0, 0, 2, 12+16*DeviceId },{ 0, 0, 2, 14+16*DeviceId } },
//	{ { 2, 0+16*DeviceId, 0, 0},{ 2, 2+16*DeviceId, 0, 0 },{ 2, 4+16*DeviceId, 0, 0 },{ 2, 6+16*DeviceId, 0, 0 },{ 2, 8+16*DeviceId, 0, 0 },{ 2, 10+16*DeviceId, 0, 0 },{ 2, 12+16*DeviceId, 0, 0 },{ 2, 14+16*DeviceId, 0, 0 } },
//	{ { 0, 0, 4, 0+32*DeviceId},{ 0, 0, 4, 4+32*DeviceId },{ 0, 0, 4, 8+32*DeviceId },{ 0, 0, 4, 12+32*DeviceId },{ 0, 0, 4, 16+32*DeviceId },{ 0, 0, 4, 20+32*DeviceId },{ 0, 0, 4, 24+32*DeviceId },{ 0, 0, 4, 28+32*DeviceId } },
//	{ { 4, 0+32*DeviceId, 0, 0},{ 4, 4+32*DeviceId, 0, 0 },{ 4, 8+32*DeviceId, 0, 0 },{ 4, 12+32*DeviceId, 0, 0 },{ 4, 16+32*DeviceId, 0, 0 },{ 4, 20+32*DeviceId, 0, 0 },{ 4, 24+32*DeviceId, 0, 0 },{ 4, 28+32*DeviceId, 0, 0 } },
//	{ { 0, 0, 4, 0+32*DeviceId},{ 0, 0, 4, 4+32*DeviceId },{ 0, 0, 4, 8+32*DeviceId },{ 0, 0, 4, 12+32*DeviceId },{ 0, 0, 4, 16+32*DeviceId },{ 0, 0, 4, 20+32*DeviceId },{ 0, 0, 4, 24+32*DeviceId },{ 0, 0, 4, 28+32*DeviceId } },
//	{ { 4, 0+32*DeviceId, 0, 0},{ 4, 4+32*DeviceId, 0, 0 },{ 4, 8+32*DeviceId, 0, 0 },{ 4, 12+32*DeviceId, 0, 0 },{ 4, 16+32*DeviceId, 0, 0 },{ 4, 20+32*DeviceId, 0, 0 },{ 4, 24+32*DeviceId, 0, 0 },{ 4, 28+32*DeviceId, 0, 0 } },
//	{ { 0, 0, 8, 0+64*DeviceId},{ 0, 0, 8, 8+64*DeviceId },{ 0, 0, 8, 16+64*DeviceId },{ 0, 0, 8, 24+64*DeviceId },{ 0, 0, 8, 32+64*DeviceId },{ 0, 0, 8, 40+64*DeviceId },{ 0, 0, 8, 48+64*DeviceId },{ 0, 0, 8, 56+64*DeviceId } },
//	{ { 8, 0+64*DeviceId, 0, 0},{ 8, 8+64*DeviceId, 0, 0 },{ 8, 16+64*DeviceId, 0, 0 },{ 8, 24+64*DeviceId, 0, 0 },{ 8, 32+64*DeviceId, 0, 0 },{ 8, 40+64*DeviceId, 0, 0 },{ 8, 48+64*DeviceId, 0, 0 },{ 8, 56+64*DeviceId, 0, 0 } },
//	{ { 0, 0, 8, 0+64*DeviceId},{ 0, 0, 8, 8+64*DeviceId },{ 0, 0, 8, 16+64*DeviceId },{ 0, 0, 8, 24+64*DeviceId },{ 0, 0, 8, 32+64*DeviceId },{ 0, 0, 8, 40+64*DeviceId },{ 0, 0, 8, 48+64*DeviceId },{ 0, 0, 8, 56+64*DeviceId } },
//	{ { 8, 0+64*DeviceId, 0, 0},{ 8, 8+64*DeviceId, 0, 0 },{ 8, 16+64*DeviceId, 0, 0 },{ 8, 24+64*DeviceId, 0, 0 },{ 8, 32+64*DeviceId, 0, 0 },{ 8, 40+64*DeviceId, 0, 0 },{ 8, 48+64*DeviceId, 0, 0 },{ 8, 56+64*DeviceId, 0, 0 } },
//	{ { 0, 0, 16, 0+128*DeviceId},{ 0, 0, 16, 16+128*DeviceId },{ 0, 0, 16, 32+128*DeviceId },{ 0, 0, 16, 48+128*DeviceId },{ 0, 0, 16, 64+128*DeviceId },{ 0, 0, 16, 80+128*DeviceId },{ 0, 0, 16, 96+128*DeviceId },{ 0, 0, 16, 112+128*DeviceId } },
//	{ { 16, 0+128*DeviceId, 16, 0+128*DeviceId},{ 16, 16+128*DeviceId, 16, 16 +128*DeviceId},{ 16, 32+128*DeviceId, 16, 32+128*DeviceId},{ 16, 48+128*DeviceId, 16, 48+128*DeviceId },{ 16, 64+128*DeviceId, 16, 64+128*DeviceId },{ 16, 80+128*DeviceId, 16, 80+128*DeviceId },{ 16, 96+128*DeviceId, 16, 96+128*DeviceId },{ 16, 112+128*DeviceId, 16, 112+128*DeviceId } },
//	{ { 16, 0+128*DeviceId, 0, 0},{ 16, 16+128*DeviceId, 0, 0 },{ 16, 32+128*DeviceId, 0, 0 },{ 16, 48+128*DeviceId, 0, 0 },{ 16, 64+128*DeviceId, 0, 0 },{ 16, 80+128*DeviceId, 0, 0 },{ 16, 96+128*DeviceId, 0, 0 },{ 16, 112+128*DeviceId, 0, 0 } },
//	{ { 0, 0,  32, 0+256*DeviceId},{ 0, 0, 32, 32+256*DeviceId },{ 0, 0, 32, 64+256*DeviceId },{ 0, 0, 32, 96+256*DeviceId },{ 0, 0, 32, 128+256*DeviceId },{ 0, 0, 32, 160+256*DeviceId },{ 0, 0, 32, 192+256*DeviceId },{ 0, 0, 32, 224+256*DeviceId} },
//	{ { 32, 0+256*DeviceId, 0, 0},{ 32, 32+256*DeviceId, 0, 0 },{ 32, 64+256*DeviceId, 0, 0 },{ 32, 96+256*DeviceId, 0, 0 },{ 32, 128+256*DeviceId, 0, 0 },{ 32, 160+256*DeviceId, 0, 0 },{ 32, 192+256*DeviceId, 0, 0 },{ 32, 224+256*DeviceId, 0, 0 } },
//	{ { 0, 0, 16, 0+128*DeviceId},{ 0, 0, 16, 16+128*DeviceId },{ 0, 0, 16, 32+128*DeviceId },{ 0, 0, 16, 48+128*DeviceId },{ 0, 0, 16, 64+128*DeviceId },{ 0, 0, 16, 80+128*DeviceId },{ 0, 0, 16, 96+128*DeviceId },{ 0, 0, 16, 112+128*DeviceId } },
//	{ { 0, 0, 16, 0+128*DeviceId},{ 0, 0, 16, 16+128*DeviceId },{ 0, 0, 16, 32+128*DeviceId },{ 0, 0, 16, 48+128*DeviceId },{ 0, 0, 16, 64+128*DeviceId },{ 0, 0, 16, 80+128*DeviceId },{ 0, 0, 16, 96+128*DeviceId },{ 0, 0, 16, 112+128*DeviceId } },
//	{ { 32, 0+256*DeviceId, 0, 0},{ 32, 32+256*DeviceId, 0, 0 },{ 32, 64+256*DeviceId, 0, 0 },{ 32, 96+256*DeviceId, 0, 0 },{ 32, 128+256*DeviceId, 0, 0 },{ 32, 160+256*DeviceId, 0, 0 },{ 32, 192+256*DeviceId, 0, 0 },{ 32, 224+256*DeviceId, 0, 0 } }
//};

sensenet_t* sensenet_open(unsigned int core);

void concat(float* input1,float* input2,float* output);
void globalavg_pool_group(
	float* input,
	float* output,
	int layerno,
	int corenum);
void conv_3x3_outputgroup_L2(float* input,float* weight,float* bias,float* output,int layer_no,int inputgroupno,int stride,int step);
void dw_conv_1x1_outputgroup_L2(float* input, float* weight, float* bias, float* output, int layer_no, int outputgroupno, int stride,int step);
void dw_conv_3x3_inputgroup_L2(float* input, float* weight, float* bias, float* output, int layer_no, int inputgroupno,int k_size, int stride, int pad,int step);

void dw_conv_3x3_inputgroup_L21(float* input, float* weight, float* bias, float* output, int layer_no, int inputgroupno,int k_size, int stride, int pad);
//int SrioSendData(uint32_t LocalDeviceID, uint32_t DstDeviceId, uint32_t LocalAddr, uint32_t DstAddr, uint32_t DataLen);


void tiling_input_0_from_tiled_sram(float* tiling_input_n_1, float* output_n, int rows, int weight, int height_n_1, int ch);
void tiling_input_1_n_from_tiled_sram_behind(float* tiling_input_n_1, float* output_n,int rows, int weight, int height_n_1, int ch);
void tiling_input_1_n_from_tiled_sram_infront( float* tiling_input_1_n, float* output_n, int rows, int weight, int height_1_n, int ch);
void writedata( float* tiling_input, float** tiling_buffer_read, float** tiling_buffer_write, int rows, int weight, int height_1_n, int ch);
void readdata( float* tiling_input, float** tiling_buffer_read, float* tiling_output, int rows, int weight, int height_1_n, int ch);
void well_aligned_data_sram(float* input, float** input_sram, float* output, int rows, int weight, int org_height, int ch);
void tiling_input_0(float* img, float* output, int offset0, int layer_no, int tiling_no);
void tiling_input_n(float* img, float* output, int offset, int layer_no, int tiling_no);
void tiling_input_1_n(float* img, float* output, int offset, int layer_no, int tiling_no);
void tiling_to_big(float* input, float* output, int layer_no, int tiling_no);
#endif