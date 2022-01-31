#include "Datamosh.h"
#include <windows.h>
#include <fstream>
#include <thread>
#include <conio.h>
using namespace DataMosh_impl;

Datamosh::STATUS Datamosh::status = NONE;
int kof_mosh = 50;



	void Datamosh::Live_CV()
	{

		cv::VideoCapture video(0);

		try {
			if (!video.isOpened()) throw 5;
		}
		catch (...)
		{
			std::cout << "Error with video or PATH";

		}

		cv::Mat frame;
		cv::Mat frame_before;

		std::vector <cv::Mat> frame_v;

		DataFrame data;
		DataFrame mosh_frame;

		bool mosh_active = false;
		bool key = false;
		double diff;



		//INPUT
		while (!_kbhit()) {
			video.read(frame);
			cv::Mat frame_d = frame.clone();
			frame_v.push_back(frame_d);
			cv::imshow("Preview", frame);
			if (cv::waitKey(25) >= 0) break;

		}
		cv::destroyAllWindows();
		video.release();
		//RENDERING
		for (int i = 0; i < frame_v.size(); i++) {

			DataFrame(frame_v[i], 1, 1);
			DataFrame a(frame_v[i], 10, 0);
			if (key)
			{

				diff = data ^ a;
				if (diff > 27)
				{
					kof_mosh = pixel_diff;
					mosh_frame = DataFrame(frame_before, 1, 0);
					mosh_active = true;
				}
				if (mosh_active) {
					if (kof_mosh > 2) kof_mosh -= 3; frame_v[i] + mosh_frame;
				}

			}
			else key = true;


			data = a;
			if (cv::waitKey(25) >= 0) break;
			frame_before = frame_v[i];

			cv::imshow("Rendering", frame_v[i]);

			ProgressBar(frame_v.size());

		}

		//COMPLETE

		cv::destroyAllWindows();
		while (!_kbhit())
		{
			for (int i = 0; i < frame_v.size(); i++)
			{
				frame = frame_v[i];
				Sleep(20);
				cv::imshow("Result", frame);
				if (cv::waitKey(25) >= 0) break;
				
			}
		}


		cv::destroyAllWindows();
	}

	void Datamosh::ProgressBar(const int & all)
	{
		system("cls");
		std::cout << frames_complete / (24 * 60) << "m. " << (frames_complete / 24) % 60 << "s." << '\n' << "frame: " << frames_complete << '\n';
		int data = frames_complete;
		std::cout << std::endl << '|';
		for (double i = 1; i <= all / 10; i += 1)
		{
			data -= all / 10;
			if (data > 0) std::cout << '#'; else std::cout << " ";
		}
		std::cout << "|\n";
		std::cout << int((double(frames_complete) / all) * 100) << "%\n" << std::endl;

		Sleep(0.1);
		frames_complete++;
	}

	DataFrame::DataFrame(const cv::Mat & frame, const STATUS & status)
	{
		switch (status)
		{
			CASE(FULL)
				DataFrame(frame, 1, 0);
			END
		}
	}
	DataFrame::DataFrame(const cv::Mat & frame, const int & kof = 1, bool mode = 0)
	{
		try
		{
			if (kof < 1) throw 5;
		}
		catch (...)
		{

		}
		uint8_t* pixelPtr = (uint8_t*)frame.data;
		int cn = frame.channels();
		int index;

		UCHAR  R, G, B;
		int HEIGHT = frame.rows;
		int WIDTH = frame.cols;

		this->HEIGHT = HEIGHT / kof;
		this->WIDTH = WIDTH / kof;

		std::vector < std::vector <Pixel> > Data((HEIGHT / kof), std::vector <Pixel>(WIDTH / kof));


		if (!mode) {
			for (int i = 0; i < HEIGHT; i += kof)
			{

				for (int j = 0; j < WIDTH; j += kof)
				{
					index = i * WIDTH * cn + j * cn;
					// Export from frame PIXELS to OUR BGR matrix
					Data[i / kof][j / kof].B = pixelPtr[index];
					Data[i / kof][j / kof].G = pixelPtr[index + 1];
					Data[i / kof][j / kof].R = pixelPtr[index + 2];

				}

			}
			PIXELS = std::move(Data);
			is_compete = true;
		}
		else
		{
			for (int i = 0; i < HEIGHT; i += kof)
			{

				for (int j = 0; j < WIDTH; j += kof)
				{
					index = i * WIDTH * cn + j * cn;
					// setnew value
					Data[i / kof][j / kof].B = pixelPtr[index] += 10;
					Data[i / kof][j / kof].G = pixelPtr[index + 1] -= 10;
					Data[i / kof][j / kof].R = pixelPtr[index + 2] += 10;

				}

			}
		}

	}
	DataFrame::DataFrame(const DataFrame & data)
	{
		this->HEIGHT = data.HEIGHT;
		this->WIDTH = data.WIDTH;
		this->PIXELS = data.PIXELS;
		this->is_compete = data.is_compete;
	}

	void  DataMosh_impl::operator+(cv::Mat & frame_now, const DataFrame & frame_mosh)
	{

		uint8_t* pixelPtr = (uint8_t*)frame_now.data;
		int cn = frame_now.channels();

		UCHAR  R, G, B;

		int HEIGHT = frame_now.rows;
		int WIDTH = frame_now.cols;
		int index;
		for (int i = 0; i < HEIGHT; i += 1)
		{

			for (int j = 0; j < WIDTH; j += 1)
			{

				index = i * WIDTH*cn + j * cn;
				// Export from frame PIXELS to OUR BGR matrix
				if (abs(pixelPtr[index] - frame_mosh.PIXELS[i][j].R) < kof_mosh
					|| abs(pixelPtr[index + 1] - frame_mosh.PIXELS[i][j].R) < kof_mosh
					|| abs(pixelPtr[index + 2] - frame_mosh.PIXELS[i][j].R) < kof_mosh)
				{
					pixelPtr[index] = frame_mosh.PIXELS[i][j].B;
					pixelPtr[index + 1] = frame_mosh.PIXELS[i][j].G;
					pixelPtr[index + 2] = frame_mosh.PIXELS[i][j].R;
				}
			}
		}
	}

	int  DataMosh_impl::operator-(const Pixel & px1, const Pixel & px2)
	{
		return (abs(px1.R - px2.R) + abs(px1.G - px2.G) + abs(px1.B - px2.B)) / 3;
	}
	double  DataMosh_impl::operator^ (const DataFrame & data1, const DataFrame & data2)
	{
		int diff = 0, sum = 0;
		short Height = data1.HEIGHT;
		short Width = data1.WIDTH;

		for (int i = 0; i < Height; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				diff = abs(data1.PIXELS[i][j] - data2.PIXELS[i][j]);
				sum += diff;
			}
		}
		return sum /= (Height*Width);
	}
