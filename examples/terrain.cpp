
#include "window3D.h"
#include <opencv2/opencv.hpp>
#include <random>


std::mt19937 randomGen(std::random_device().operator()());

class MyWindow : public cf::Window3D{
public:
    MyWindow(int* argc, char** argv, cv::Mat heightMap) : cf::Window3D(argc, argv), m_HeightMap(heightMap){
        this->m_CameraAdjustment = 10.f;
        this->m_AngleAdjustment = 10.f;
    }
    virtual void draw() override{
        this->clear();
        const float minHeight =  0.1f;
        const float maxHeight = 10.f;
        const float cubeSize  =  0.5f;
        std::uniform_real_distribution<float> dis(0.f, 1.f);

        for (int y = 0; y < this->m_HeightMap.rows; ++y){
        for (int x = 0; x < this->m_HeightMap.cols; ++x){

            glPushMatrix();
            {
                float tmp = this->m_HeightMap.at<uchar>(y, x);
                tmp /= 255.f;
                tmp *= maxHeight - minHeight;
                tmp += minHeight;

                //glColor3f(dis(randomGen), dis(randomGen), dis(randomGen)); // random color
                glColor3f(1.f, 1.f, 1.f);
                glTranslatef((x - this->m_HeightMap.cols / 2) * cubeSize, (y - this->m_HeightMap.rows / 2) * cubeSize, 1.f);
                glScalef(1.f, 1.f, tmp);
                glutWireCube(cubeSize);
            }
            glPopMatrix();
        }
        }
    }
private:
    cv::Mat m_HeightMap;
};



int main(int argc, char** argv){
    if (argc < 2){
        std::cout << "Please provide a heightmap";
        return -1;
    }
    cv::Mat img = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    cv::resize(img, img, cv::Size(300, 200));
    cv::imshow("HeightMap", img);
    cv::waitKey(1000);

    MyWindow::showWindowUsage();
    MyWindow window(&argc, argv, img);
    window.setCamera(MyWindow::CameraType::ROTATION);
    return window.startDrawing();
}
