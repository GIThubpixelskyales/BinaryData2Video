#define _CRT_SECURE_NO_WARNING
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <Windows.h>

using namespace std;
using namespace cv;

Mat src; Mat src_gray;

void seperate(const char* pathMp4, const char* pathPng)//seperate("test.mp4", "x%d.png");//ʾ��
{
	string strFfmpegPath = "E:\\ffmpeg-4.2.2-win64-static\\bin\\ffmpeg";//ffmpeg.exe���ڵ�λ��
	string strPathMp4 = pathMp4;//��ת����mp4
	string strPathPng = pathPng;//���ɵ�png
	string strCmdContent = "/c" + strFfmpegPath + " -i " + strPathMp4 + " " + strPathPng;
	ShellExecute(NULL, "open", "cmd", strCmdContent.c_str(), NULL, SW_HIDE);
}

RNG rng(12345);
//Scalar colorful = CV_RGB(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));

//��ȡ���������ĵ�
Point Center_cal(vector<vector<Point> > contours, int i)
{
	int centerx = 0, centery = 0, n = contours[i].size();
	//����ȡ��С�����εı߽���ÿ���ܳ���������ȡһ��������꣬
	//������ȡ�ĸ����ƽ�����꣨��ΪС�����εĴ������ģ�
	centerx = (contours[i][n / 4].x + contours[i][n * 2 / 4].x + contours[i][3 * n / 4].x + contours[i][n - 1].x) / 4;
	centery = (contours[i][n / 4].y + contours[i][n * 2 / 4].y + contours[i][3 * n / 4].y + contours[i][n - 1].y) / 4;
	Point point1 = Point(centerx, centery);
	return point1;
}

void cvtColor_src(Mat& src, Mat& src_gray)
{
	//  ת����ͨ��
	if (src.channels() == 4)
	{
		cv::cvtColor(src, src_gray, COLOR_BGRA2GRAY);
	}
	else if (src.channels() == 3)
	{
		cv::cvtColor(src, src_gray, COLOR_BGR2GRAY);
	}
	else if (src.channels() == 2)
	{
		cv::cvtColor(src, src_gray, COLOR_BGR5652GRAY);
	}
	else if (src.channels() == 1)
	{
		// ��ͨ����ͼƬֱ�ӾͲ���Ҫ����
		src_gray = src;
	}
	else
	{ // ����,˵��ͼ������ ֱ�ӷ���   
		src_gray = src;
	}

}

int main(int argc, char** argv[])
{
	src = imread("core.png", 1);
	Mat src_all = src.clone();
	cout << src.channels()<<"  ";

	//��ɫͼת�Ҷ�ͼ
	cvtColor_src(src, src_gray);
	//��ͼ�����ƽ������
	blur(src_gray, src_gray, Size(1,1));
	//ʹ�Ҷ�ͼ��ֱ��ͼ���⻯
	equalizeHist(src_gray, src_gray);
	namedWindow("src_gray");
	imshow("src_gray", src_gray);


	Scalar color = Scalar(1, 1, 255);
	Mat threshold_output;
	vector<vector<Point> > contours, contours2;
	vector<Vec4i> hierarchy;
	Mat drawing = Mat::zeros(src.size(), CV_8UC3);
	Mat drawing2 = Mat::zeros(src.size(), CV_8UC3);
	Mat drawingAllContours = Mat::zeros(src.size(), CV_8UC3);

	//ָ��112��ֵ���ж�ֵ��
	threshold(src_gray, threshold_output, 112, 255, THRESH_BINARY);

	namedWindow("Threshold_output");
	imshow("Threshold_output", threshold_output);

	findContours(threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0));

	int c = 0, ic = 0, k = 0, area = 0;

	//ͨ����ɫ��λ����Ϊ�����������������������ص㣬ɸѡ��������λ��
	int parentIdx = -1;
	for (int i = 0; i < contours.size(); i++)
	{
		//������������ͼ
		drawContours(drawingAllContours, contours, parentIdx, CV_RGB(255, 255, 255), 1, 8);
		if (hierarchy[i][2] != -1 && ic == 0)
		{
			parentIdx = i;
			ic++;
		}
		else if (hierarchy[i][2] != -1)
		{
			ic++;
		}
		else if (hierarchy[i][2] == -1)
		{
			ic = 0;
			parentIdx = -1;
		}

		//������������
		if (ic >= 2)
		{
			//�����ҵ���������ɫ��λ��
			contours2.push_back(contours[parentIdx]);
			//����������ɫ��λ�ǵ�����
			drawContours(drawing, contours, parentIdx, CV_RGB(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 1, 8);
			ic = 0;
			parentIdx = -1;
		}
	}

	//���ķ�ʽ����������ɫ��λ�ǵ�����
	for (int i = 0; i < contours2.size(); i++)
		drawContours(drawing2, contours2, i, CV_RGB(rng.uniform(100, 255), rng.uniform(100, 255), rng.uniform(100, 255)), -1, 4, hierarchy[k][2], 0, Point());

	//��ȡ������λ�ǵ���������
	Point point[3];
	for (int i = 0; i < contours2.size(); i++)
	{
		point[i] = Center_cal(contours2, i);
	}

	//������������������㶨λ�ǵ�������Ӷ�������߳�
	area = contourArea(contours2[1]);
	int area_side = cvRound(sqrt(double(area)));
	for (int i = 0; i < contours2.size(); i++)
	{
		//����������λ�ǵ���������  
		line(drawing2, point[i % contours2.size()], point[(i + 1) % contours2.size()], color, area_side / 8);
	}

	namedWindow("DrawingAllContours");
	imshow("DrawingAllContours", drawingAllContours);

	namedWindow("Drawing2");
	imshow("Drawing2", drawing2);

	namedWindow("Drawing");
	imshow("Drawing", drawing);
	
	//������Ҫ�����������ά��
	Mat gray_all, threshold_output_all;
	vector<vector<Point> > contours_all;
	vector<Vec4i> hierarchy_all;
	cvtColor(drawing2, gray_all, COLOR_BGR2GRAY);


	threshold(gray_all, threshold_output_all, 45, 255, THRESH_BINARY);
	findContours(threshold_output_all, contours_all, hierarchy_all, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));//RETR_EXTERNAL��ʾֻѰ�����������

	Point2f fourPoint2f[4];
	//����С��Χ����
	RotatedRect rectPoint = minAreaRect(contours_all[0]);

	//��rectPoint�����д洢������ֵ�ŵ� fourPoint��������
	rectPoint.points(fourPoint2f);


	for (int i = 0; i < 4; i++)
	{
		line(src_all, fourPoint2f[i % 4], fourPoint2f[(i + 1) % 4], Scalar(20, 21, 237), 3);
	}

	namedWindow("Src_all");
	imshow("Src_all", src_all);

	//�����ά��󣬾Ϳ�����ȡ����ά�룬Ȼ��ʹ�ý����zxing����������Ϣ��
	//�����о���ά����Ų������Լ�д���벿��

	waitKey(0);
	return(0);
}

