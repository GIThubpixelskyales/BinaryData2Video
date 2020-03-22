#pragma once
#include "const.h"
#include <vector>
#include <iostream>
#include "opencv2\opencv.hpp"
#include <opencv2/core/core.hpp>
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
class QREncodeTools : public QRCodeTools {
public:
	QREncodeTools(int v = VERSION);
	void flush(); //!!д��һ��ͼƬ��Ҫˢ��
	int write(const int x, const int y, const int bit); //����Ϊx,y����0��ʼ��,bit=0д���ɫ��bit=1д���ɫ

	void display()const; //��stdout��ʱ�鿴����������Ҫ�ѿ���̨���п����
	cv::Size output(cv::OutputArray, int rate = 7); //�����OutputArray��Ĭ��rate=7,һ��ԭ����=7*7�����ص�Size��֡�Ŀ��
	void WriteFrame(bool datamatrix[][TSIZE], int flames);
private:
	cv::Mat src, tmp;
	void makeSrc();
	void drawLocator(const int x, const int y);
	int checkValid(const int rate);
	int regenerate(const int rate);
	inline int epseq(float x, float y)const;
};

int readFile(const char str[]);
void process(bool arr[][TSIZE], int flame, int len);

void Image_to_Video_Generate(int num, int frame_width, int frame_height, float video_time, string video_name);