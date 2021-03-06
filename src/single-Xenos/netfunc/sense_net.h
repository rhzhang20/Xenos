#ifndef SENSENET_SENSE_NET_H_
#define SENSENET_SENSE_NET_H_


#include "common.h"
#include "commondef.h"
extern "C"
{
#include <dsplib.h>
}

//分块策略：保持整行，对图像高度划分为7块，分别进行计算。
									// con              con             con        10(5x2)                 conv
const int img_height[20]=     {224,112,112,112,56 ,56 ,56 ,56 ,28 ,28 ,28 ,28 ,14 ,14 ,14 ,7   ,7   ,7   ,7   ,7   };
const int img_width[20]=      {224,112,112,112,56 ,56 ,56 ,56 ,28 ,28 ,28 ,28 ,14 ,14 ,14 ,7   ,7   ,7   ,7   ,7   };
const int input_channel[20]=  {3  ,32 ,32 ,64 ,64 ,128,128,128,128,256,256,256,256,512,512,512 ,1024,1024,1024,1024};
const int output_channel[20]= {32 ,32 ,64 ,64 ,128,128,128,128,256,256,256,256,512,512,512,1024,1024,512 ,512 ,1024};
//分块参数说明：数组第1个参数表示输入通道分组大小，第2个参数表示输入通道分组偏移量，第3个参数输出通道分组，第4个参数表示输出通道分组偏移量。第13行是neckfeature里面的conv1x1
//这个里面只包括卷积计算的分块，其他数学运算的分块在上面另行定义了
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
	{ { 0, 0, 64, 0},{ 0, 0, 64, 64 },{ 0, 0, 64, 128 },{ 0, 0, 64, 192 },{ 0, 0, 64, 256 },{ 0, 0, 64, 320 },{ 0, 0, 64, 384 },{ 0, 0, 64, 448 } },
	{ { 64, 0, 64, 0},{ 64, 64, 64, 64 },{ 64, 128, 64, 128 },{ 64, 192, 64, 192 },{ 64, 256, 64, 256 },{ 64, 320, 64, 320 },{ 64, 384, 64, 384 },{ 64, 448, 64, 448 } },
	{ { 64, 0, 0, 0},{ 64, 64, 0, 0 },{ 64, 128, 0, 0 },{ 64, 192, 0, 0 },{ 64, 256, 0, 0 },{ 64, 320, 0, 0 },{ 64, 384, 0, 0 },{ 64, 448, 0, 0 } },
	{ { 0, 0, 128, 0},{ 0, 0, 128, 128 },{ 0, 0, 128, 256 },{ 0, 0, 128, 384 },{ 0, 0, 128, 512 },{ 0, 0, 128, 640 },{ 0, 0, 128, 768 },{ 0, 0, 128, 896 } },
	{ { 128, 0, 0, 0},{ 128, 128, 0, 0 },{ 128, 256, 0, 0 },{ 128, 384, 0, 0 },{ 128, 512, 0, 0 },{ 128, 640, 0, 0 },{ 128, 768, 0, 0 },{ 128, 896, 0, 0 } },
	{ { 0, 0, 64, 0},{ 0, 0, 64, 64 },{ 0, 0, 64, 128 },{ 0, 0, 64, 192 },{ 0, 0, 64, 256 },{ 0, 0, 64, 320 },{ 0, 0, 64, 384 },{ 0, 0, 64, 448 } },
	{ { 0, 0, 64, 0},{ 0, 0, 64, 64 },{ 0, 0, 64, 128 },{ 0, 0, 64, 192 },{ 0, 0, 64, 256 },{ 0, 0, 64, 320 },{ 0, 0, 64, 384 },{ 0, 0, 64, 448 } },
//	{ { 0, 0, 64*2, 0},{ 0, 0, 64*2, 64*2 },{ 0, 0, 64*2, 128*2 },{ 0, 0, 64*2, 192*2 },{ 0, 0, 64*2, 256*2 },{ 0, 0, 64*2, 320*2 },{ 0, 0, 64*2, 384*2 },{ 0, 0, 64*2, 448*2 } },
//	{ { 0, 0, 64*2, 0},{ 0, 0, 64*2, 64*2 },{ 0, 0, 64*2, 128*2 },{ 0, 0, 64*2, 192*2 },{ 0, 0, 64*2, 256*2 },{ 0, 0, 64*2, 320*2 },{ 0, 0, 64*2, 384*2 },{ 0, 0, 64*2, 448*2 } },
	{ { 128, 0, 0, 0},{ 128, 128, 0, 0 },{ 128, 256, 0, 0 },{ 128, 384, 0, 0 },{ 128, 512, 0, 0 },{ 128, 640, 0, 0 },{ 128, 768, 0, 0 },{ 128, 896, 0, 0 } },
};

sensenet_t* sensenet_open(unsigned int core);

void concat(float* input1,float* input2,float* output);
void globalavg_pool_group(
	float* input,
	float* output,
	int layerno,
	int corenum);
void conv_3x3_outputgroup_L2(float* input,float* weight,float* bias,float* output,int layer_no,int inputgroupno,int stride);
void dw_conv_1x1_outputgroup_L2(float* input, float* weight, float* bias, float* output, int layer_no, int outputgroupno, int stride);
void dw_conv_3x3_inputgroup_L2(float* input, float* weight, float* bias, float* output, int layer_no, int inputgroupno,int k_size, int stride, int pad);

void dw_conv_3x3_inputgroup_L21(float* input, float* weight, float* bias, float* output, int layer_no, int inputgroupno,int k_size, int stride, int pad);

#endif
