#pragma once
#include "opencv2/opencv.hpp"
#include <iostream>


namespace DataMosh_impl
{
	typedef unsigned char UCHAR;
#define CASE(a) break;case(a):
#define END break;

	class Exeption
	{

	};

	//Singleton
	class Datamosh
	{
	public:
		enum STATUS
		{
			LIVE,
			NONE,
			VIDEO
		};

		static Datamosh & instanse()
		{
			static Datamosh res;
			return res;
		}

		void SetEngine(const STATUS & status)
		{
			this->status = status;
		}

		//+-+-+-+-+-+-+ kof_mosh - level of pixels mosh
		//+-+-+-+-+-+-+ sensetive - kof switch frames
		void SetSettings(int sensetive = 25,
			int pixel_diff = 50)
		{
			this->Sensetive = sensetive;
			this->pixel_diff = pixel_diff;
		}

		void Start()
		{
			try
			{
				switch (status)
				{
					CASE(NONE)
						throw Exeption();
					CASE(LIVE)
						Live_CV();
					CASE(VIDEO)

						try
					{
						if (PATH == "")throw false;
					}
					catch (...)
					{

					}


					END
				}
			}
			catch (...)
			{
				// полимофизм ошибок
			}
		}

		void SetPATH(const std::string PATH)
		{
			this->PATH = PATH;
			for (int i = 0; i < PATH.size(); i++)
			{
				if (PATH[i] == '\\') this->PATH[i] = '/';
			}
		}

		Datamosh(const Datamosh &) = delete;
		Datamosh & operator=(const Datamosh &) = delete;
	private:

		void Live_CV();

		//void Video_CV();
		void ProgressBar(const int &);

		std::string PATH = "";

		static STATUS status;
		Datamosh() = default;

		int frames_complete = 0;

		int Sensetive = 25;
		int pixel_diff = 50;

	};

	struct Pixel
	{
		UCHAR R, G, B;
		friend int operator-(const Pixel &, const Pixel &);
	};

	class DataFrame
	{

	public:

		//+-+-+-+-+-+-+-+-+-+-+-+-+- Defalut Settings +-+-+-+-+-+-+-+-+-+-+-+-+

		enum STATUS
		{
			FULL = 1
		};
		DataFrame() = default;
		DataFrame(const DataFrame & data);



		//+-+-+-+-+-+-+-+-+-+-+-+-+- Constructors +-+-+-+-+-+-+-+-+-+-+-+-+

		DataFrame(const cv::Mat &, const STATUS &);

		DataFrame(const cv::Mat &, const int &, bool);

		//+-+-+-+-+-+-+-+-+-+-+-+-+- Operators +-+-+-+-+-+-+-+-+-+-+-+-+

		friend double operator^ (const DataFrame &, const DataFrame &);
		friend void operator+(cv::Mat &, const DataFrame &);

	private:

		short HEIGHT;
		short WIDTH;
		std::vector < std::vector <Pixel> > PIXELS;
		bool is_compete = false;

	};

}

namespace IDataMosh
{
	// add settings for Interf 
	using DataMosh_impl::Datamosh;
}
