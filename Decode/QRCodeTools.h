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

class QRCodeTools { //���ֹ��ߵĻ���
public:
	QRCodeTools(int v = VERSION); //v�Ƕ�ά��İ汾
	~QRCodeTools();
	void setDbg(bool flag);//���õ��Ա�־=1ÿ�����read/write��ʵ���������ꡣĬ��=0
	virtual void flush() = 0;
protected:
	int Ver, sz, maskID; //Ver ��ά��İ汾��sz ������ռ����
	bool dbg; //���Ա�־
	int mask(const int x, const int y)const; //����Ĥ��
	cv::Mat img;
	cv::QRCodeDetector wheels;
	std::vector <cv::Point2f> Rect;
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
class QRDecodeTools : public QRCodeTools
{
public:
	QRDecodeTools(int v = VERSION, float e = 0.2745f, int r = 1); //vĬ�ϣ��޲Σ����ɣ�sz=61*61��Ver=11����e�Ǽ����ֵ��Ĭ��0.2
	int loadQRCode(cv::InputArray in); //���ز����ͼƬ�������⵽��ά�뷵��1�����򷵻�0
	int detected(); //load֮�������ж��Ƿ��⵽��ά�룬��⵽����True�����򷵻�0
	int read(const int x, const int y); //��ȡ��Ӧ���꣨x,y���ı���ֵ������Խ���򷵻�-1��������ȡ������ֵ�ڣ��򷵻�0�������ɫ������1�������ɫ������������׳�uchar���쳣����Ҷ�ֵ��debug=1ʱ������ص�λ�á�
	void setThreshold(const float x); //�趨�����ֵ����Χ[0,0.5)��Ĭ��Ϊ0.2 ����read(x,y)ʱ�����ػҶ�[0,0.2*255]Ϊ��ɫ��[0.8*255,255]Ϊ��ɫ����Χ������Ϊģ�����׳��쳣
	void flush(); //!!loadQRCode(�µ�֡)֮ǰ��Ҫˢ��
	void ReadFrame(const char* str, int datamatrix[][TSIZE]);
	~QRDecodeTools();
private:
	int gotQR, rounD;
	float Eps;
	cv::Point map(const int x, const int y) const; //read���굽���ص������ӳ��
	cv::Point2f rate(const int p, const cv::Point2f& a, const cv::Point2f& b)const;
	int readM(const int x, const int y) const;
};

int BTD(bool arr[], int lens);
void readframes(char* str, char* err, int k);
int Video_To_Image_Separate(string filename);
bool writeFile(int arr[][TSIZE], FILE* fp, FILE* fw);
