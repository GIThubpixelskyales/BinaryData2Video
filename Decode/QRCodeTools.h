#pragma once
#include "const.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "opencv2\opencv.hpp"
#include <opencv2\core\core.hpp>
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

using namespace cv;
using namespace std;

class QRCodeTools { //两种工具的基类
public:
	QRCodeTools(int v = VERSION); //v是二维码的版本
	~QRCodeTools();
	void setDbg(bool flag);//设置调试标志=1每次输出read/write的实际像素坐标。默认=0
	virtual void flush() = 0;
protected:
	int Ver, sz, maskID; //Ver 二维码的版本，sz 长宽所占像素
	bool dbg; //调试标志
	int mask(const int x, const int y)const; //上掩膜。
	cv::Mat img;
	cv::QRCodeDetector wheels;
	std::vector <cv::Point2f> Rect;
	int checkPos(const int x, const int y)const; //检查坐标是否是可写入的合法坐标
	const int AreaCount = 11;
	const int QrLocaterSize = 8;
	const int QrSize = 4 * (Ver - 1) + 21;
	const int areapos[11][2][2] =//[2][2]第一维度为宽高，第二维度为左上角坐标
	{
	 {{QrLocaterSize - 2,3} ,{0,QrLocaterSize}},//帧编码与结束帧标记
	 {{QrLocaterSize - 2,2},{0,QrLocaterSize + 3}},//结束帧信息长度

	 {{QrLocaterSize - 2,QrSize - 5 - 2 * QrLocaterSize},{0,QrLocaterSize + 5}},//左侧数据区
	 {{QrSize - 2 * QrLocaterSize,QrLocaterSize - 2},{QrLocaterSize,0}},//上侧数据区
	 {{QrSize - 1 - QrLocaterSize,QrSize - 1 - QrLocaterSize},{QrLocaterSize,QrLocaterSize}},//中央数据区

	 {{1,QrSize - 5 - 2 * QrLocaterSize},{QrLocaterSize - 1,QrLocaterSize}},//左侧行校验码
	 {{QrLocaterSize - 2,1},{0,QrSize - 1 - QrLocaterSize}},//左侧列校验码
	 {{1,QrLocaterSize - 2},{QrSize - 1 - QrLocaterSize,0}},//上侧行校验码
	 {{QrSize - 2 * QrLocaterSize,1},{QrLocaterSize,QrLocaterSize - 1}},//上侧列校验码
	 {{1,QrSize - QrLocaterSize},{QrSize - 1,QrLocaterSize}},//中央行校验码
	 {{QrSize - QrLocaterSize,1},{QrLocaterSize,QrSize - 1}}//中央列校验码
	};
};
class QRDecodeTools : public QRCodeTools
{
public:
	QRDecodeTools(int v = VERSION, float e = 0.2745f, int r = 1); //v默认（无参）即可（sz=61*61，Ver=11），e是检测阈值，默认0.2
	int loadQRCode(cv::InputArray in); //加载并检测图片，如果检测到二维码返回1，否则返回0
	int detected(); //load之后用来判断是否检测到二维码，检测到返回True，否则返回0
	int read(const int x, const int y); //获取对应坐标（x,y）的编码值，坐标越界则返回-1，正常读取（在阈值内）则返回0（代表黑色），或1（代表白色），不清楚则抛出uchar的异常代表灰度值。debug=1时输出像素点位置。
	void setThreshold(const float x); //设定检测阈值，范围[0,0.5)。默认为0.2 即：read(x,y)时，像素灰度[0,0.2*255]为白色，[0.8*255,255]为黑色，范围外则认为模糊，抛出异常
	void flush(); //!!loadQRCode(新的帧)之前都要刷新
	void ReadFrame(const char* str, int datamatrix[][TSIZE]);
	~QRDecodeTools();
private:
	int gotQR, rounD;
	float Eps;
	cv::Point map(const int x, const int y) const; //read坐标到像素点坐标的映射
	cv::Point2f rate(const int p, const cv::Point2f& a, const cv::Point2f& b)const;
	int readM(const int x, const int y) const;
};

int BTD(bool arr[], int lens);
void readframes(char* str, char* err, int k);
int Video_To_Image_Separate(string filename);
bool writeFile(int arr[][TSIZE], FILE* fp, FILE* fw);
