#include <opencv2/opencv.hpp>
#define MAXFILE 7100
#define SIZE_WIGTH 1024
#define SIZE_HEGTH 768

// при сжатии изображения использовать интерполяцию CV_INTER_NEAREST (простая итерполяция)
// при расширении изображения использовать интерполяцию CV_INTER_CUBIC

// перспективное преобразование
cv::Mat perspektivnoe_preobrazovanie(const cv::Mat &src)
{
	
	cv::Point2f  srcQuad[4], dstQuad[4];

	cv::Mat dst = src.clone();

	dst.data = 0;

	srcQuad[0].x = 0; // исходный левый верхний угол
	srcQuad[0].y = 0; 
	srcQuad[1].x = (float)src.rows; // Исходный правый верхний угол
	srcQuad[1].y = 0;
	srcQuad[2].x = 0; // исходный левый нижний угол
	srcQuad[2].y = (float)src.cols; 
	srcQuad[3].x = (float)src.rows; // исходный нижний правый угол
	srcQuad[3].y = (float)src.cols;

	dstQuad[0].x = src.rows * 0.07f; // конечный верхний левый угол
	dstQuad[0].y = src.cols * 0.002f;
	dstQuad[1].x = src.rows * 0.9f; // конечный верхний правый угол
	dstQuad[1].y = src.cols * 0.002f;
	dstQuad[2].x = src.rows * 0.45f; // конечный нижний левый угол
	dstQuad[2].y = src.cols * 0.7f;
	dstQuad[3].x = src.rows * 0.8f; // конечный нижний правый угол
	dstQuad[3].y = src.cols * 0.9f;

	cv::Mat perspektiv_transform = cv::getPerspectiveTransform(srcQuad, dstQuad);
	
	
	cv::warpPerspective(src, dst, perspektiv_transform, dst.size());
	

	return dst;

}

void image_resize(cv::Mat &src)
{
	
	if (src.cols < SIZE_WIGTH && src.rows < SIZE_HEGTH)
		// расширяем изображение
		cv::resize(src, src, cv::Size(SIZE_WIGTH, SIZE_HEGTH), 0, 0, cv::INTER_CUBIC);
	else
		cv::resize(src, src, cv::Size(SIZE_WIGTH, SIZE_HEGTH), 0, 0, cv::INTER_NEAREST);
	
	
	
}

void file_write(std::stringstream &stream, cv::Mat &image)
{
	std::string stroka = "./tmp\\"+stream.str()+".jpg";
	

	cv::imwrite(stroka, image);

}

int open_file(int &num, cv::Mat &src)
{
	
	std::stringstream str_stream; 
	str_stream << num;
	
	
	try
	{
		std::string stroka = str_stream.str() + ".jpeg";
		src = cv::imread(stroka, CV_LOAD_IMAGE_GRAYSCALE);

		if (!src.data)
		{
			stroka =  str_stream.str() + ".jpg";
			src = cv::imread(stroka, CV_LOAD_IMAGE_GRAYSCALE);
		}
		else
		{
			std::cout << "Open: " << stroka << std::endl;
			return 1;
		}

		if (!src.data)
		{
			stroka =  str_stream.str() + ".JPG";
			src = cv::imread(stroka, CV_LOAD_IMAGE_GRAYSCALE);
		}
		else
		{
			std::cout << "Open: " << stroka << std::endl;
			return 1;
		}
		
	}
	catch (cv::Exception c)
	{
		std::cout << "error open file!" << std::endl;
		int a;
		std::cin >> a;
		return -1;
	}
	



}

int main() 
{
	size_t n = 1;
	
#pragma omp parallel for firstprivate(n) schedule(dynamic)
		for(int i = 1; i <= MAXFILE; i++) 
		{
			cv::Mat tmp;

			if (!open_file(i, tmp))
				continue;


			image_resize(tmp);

			std::stringstream str_stream2;
			str_stream2 << i;

			file_write(str_stream2, tmp);
/*
			cv::Mat perspektiva = perspektivnoe_preobrazovanie(tmp);
			// очистка буфера stringstream пустой строкой
			str_stream2.str(std::string());
			str_stream2 << (n+1);

			file_write(str_stream2, perspektiva);
	*/		
			n += 2;

		}
	
	
	
	return 0; 
} 
