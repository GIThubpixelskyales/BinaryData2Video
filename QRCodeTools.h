#pragma once
#include "const.h"
#include <vector>
#include <iostream>
#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "const.h"
class QRCodeTools { //���ֹ��ߵĻ���
public:
	QRCodeTools(int v = 11); //v�Ƕ�ά��İ汾
	~QRCodeTools();
	void setDbg(bool flag);//���õ��Ա�־=1ÿ�����read/write��ʵ���������ꡣĬ��=0
	virtual void flush() = 0;
protected:
	int Ver, sz; //Ver ��ά��İ汾��sz ������ռ����
	bool dbg; //���Ա�־
	int mask(const int x, const int y)const; //����Ĥ��
	cv::Mat img;
	int checkPos(const int x, const int y)const; //��������Ƿ��ǿ�д��ĺϷ�����
	const int AreaCount = 11;
	const int QrLocaterSize = 8;
	const int QrSize = 4 * (Ver - 1) + 21;
	const int areapos[11][2][2] =//[2][2]��һά��Ϊ��ߣ��ڶ�ά��Ϊ���Ͻ�����
	{
	 {{QrLocaterSize - 2,3} ,{0,QrLocaterSize}},//֡���������֡���
	 {{QrLocaterSize - 2,2},{0,QrLocaterSize + 3}},//����֡��Ϣ����

	 {{QrLocaterSize - 2,QrSize - 5 - 2 * QrLocaterSize},{0,QrLocaterSize + 5}},//���������
	 {{QrSize - 2 * QrLocaterSize,QrLocaterSize - 2},{QrLocaterSize,0}},//�ϲ�������
	 {{QrSize - 1 - QrLocaterSize,QrSize - 1 - QrLocaterSize},{QrLocaterSize,QrLocaterSize}},//����������

	 {{1,QrSize - 5 - 2 * QrLocaterSize},{QrLocaterSize - 1,QrLocaterSize}},//�����У����
	 {{QrLocaterSize - 2,1},{0,QrSize - 1 - QrLocaterSize}},//�����У����
	 {{1,QrLocaterSize - 2},{QrSize - 1 - QrLocaterSize,0}},//�ϲ���У����
	 {{QrSize - 2 * QrLocaterSize,1},{QrLocaterSize,QrLocaterSize - 1}},//�ϲ���У����
	 {{1,QrSize - QrLocaterSize},{QrSize - 1,QrLocaterSize}},//������У����
	 {{QrSize - QrLocaterSize,1},{QrLocaterSize,QrSize - 1}}//������У����
	};
};
class QREncodeTools : public QRCodeTools {
public:
	QREncodeTools(int v = 11);
	void flush(); //!!д��һ��ͼƬ��Ҫˢ��
	int write(const int x, const int y, const int bit); //����Ϊx,y����0��ʼ��,bit=0д���ɫ��bit=1д���ɫ

	void display()const; //��stdout��ʱ�鿴����������Ҫ�ѿ���̨���п����
	cv::Size output(cv::OutputArray, int rate = 7); //�����OutputArray��Ĭ��rate=7,һ��ԭ����=7*7�����ص�Size��֡�Ŀ��
	void WriteFrame(bool datamatrix[][TSIZE], int flames);
private:
	cv::Mat src;
	void makeSrc();
	void drawLocator(const int x, const int y);
};
class QRDecodeTools : public QRCodeTools
{
public:
	QRDecodeTools(int v = 11, float e = 0.2f); //vĬ�ϣ��޲Σ����ɣ�sz=81*81��Ver=16����e�Ǽ����ֵ��Ĭ��0.2
	int loadQRCode(cv::InputArray in); //���ز����ͼƬ�������⵽��ά�뷵��1�����򷵻�0
	int detected(); //load֮�������ж��Ƿ��⵽��ά�룬��⵽����True�����򷵻�0
	int read(const int x, const int y)const; //��ȡ��Ӧ���꣨x,y���ı���ֵ������Խ���򷵻�-1��������ȡ������ֵ�ڣ��򷵻�0�������ɫ������1�������ɫ������������׳�uchar���쳣����Ҷ�ֵ��debug=1ʱ������ص�λ�á�
	void setThreshold(const float x); //�趨�����ֵ����Χ[0,0.5)��Ĭ��Ϊ0.2 ����read(x,y)ʱ�����ػҶ�[0,0.2*255]Ϊ��ɫ��[0.8*255,255]Ϊ��ɫ����Χ������Ϊģ�����׳��쳣
	void flush(); //!!loadQRCode(�µ�֡)֮ǰ��Ҫˢ��
	void ReadFrame(const char* str, bool datamatrix[][TSIZE]);
	~QRDecodeTools();
private:
	int gotQR;
	float Eps;
	cv::QRCodeDetector wheels;
	std::vector <cv::Point2f> Rect;
	cv::Point map(const int x, const int y) const; //read���굽���ص������ӳ��
	cv::Point2f rate(const int p, const cv::Point2f& a, const cv::Point2f& b)const;
};

using namespace cv;
void Demo(char str[]);
bool readFile(const char str[]);
void process(bool arr[][TSIZE], int flame, int len);
void readframes();
int BTD(bool arr[], int lens);
bool writeFile(bool arr[][TSIZE]);