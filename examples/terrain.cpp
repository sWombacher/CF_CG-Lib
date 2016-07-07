#include "window3D.h"
#include <opencv2/opencv.hpp>

class MyWindow : public cf::Window3D{
public:
    MyWindow(int* argc, char** argv, cv::Mat heightMap) : cf::Window3D(argc, argv), m_HeightMap(heightMap){ }
    virtual void draw() override{
        this->clear();
        const float minHeight = 0.1f;
        const float maxHeight = 10.f;
        const float cubeSize  = 0.5f;

        for (int y = 0; y < this->m_HeightMap.rows; ++y){
        for (int x = 0; x < this->m_HeightMap.cols; ++x){
            glPushMatrix();
            {
                float tmp = this->m_HeightMap.at<uchar>(y, x);
                tmp /= 255.f;
                tmp *= maxHeight - minHeight;
                tmp += minHeight;

                glColor3f(tmp / maxHeight, tmp / maxHeight, tmp / maxHeight);
                glTranslatef((x - this->m_HeightMap.cols / 2) * cubeSize, (y - this->m_HeightMap.rows / 2) * cubeSize, 1.f);
                glScalef(1.f, 1.f, tmp);
                glutSolidCube(cubeSize);
            }
            glPopMatrix();
        }
        }
    }
private:
    cv::Mat m_HeightMap;
};

int main(int argc, char** argv){
    // read command line parameter
    // (image file of any format opencv does support, tested formats: jpeg, png, bmp)
    if (argc < 2){
        std::cout << "Please provide a heightmap";
        getchar();
        return -1;
    }

    cv::Mat img = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    cv::resize(img, img, cv::Size(255, 255));
    cv::imshow("HeightMap", img);
    cv::waitKey(1000);

    MyWindow::showWindowUsage();
    MyWindow window(&argc, argv, img);
    window.setCamera(MyWindow::CameraType::ROTATION);
    return window.startDrawing();
}
