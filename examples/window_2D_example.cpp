#include "windowRasterized.h"

int main(void) {
    cf::WindowRasterized w;

    for (int y = 0; y < w.getHeight(); ++y){
        for (int x = 0; x < w.getWidth() / 2;  ++x)
            w.setColor(x, y, cf::Color(255, 255, 255));

        for (int x = w.getWidth() / 2; x < w.getWidth();  ++x)
            w.setColor(x, y, cf::Color::BLUE);
    }
    w.show();
	w.waitKey();

    const int step_count = 100;
    while(w.getColor(0, 0) == cf::Color::WHITE){
        for (int i = 0; i < step_count; ++i){
            printf("Processing: %d%% \r", int(float(i) / float(step_count) * 100));
            fflush(stdout);

			// this algorithm might be slow (debug version) 
			// so we might want to parallize this by using openmp
			// by default openmp uses number of processor cores, however you may specify this yourself
            #pragma omp parallel for
            for (int y = 0; y < w.getHeight()   ; ++y){
            for (int x = 0; x < w.getWidth() - 1; ++x){
				cf::Color col = w.getColor(x + 1, y);
                w.setColor(x, y, col);

                // alternative (and much faster) version
                // NOTE: by using this version you have to deal with y-axis inverting and invervall changing yourself
				//		(additional you might have to know:
				//			cv::Vec3b[0]		// blue  value
				//			cv::Vec3b[1]		// green value
				//			cv::Vec3b[2]		// red	 value
                //
                //cv::Mat& img = w.getImage();
                //img.at<cv::Vec3b>(y, x) = img.at<cv::Vec3b>(y, x + 1);	// copy color
				//
				//img.at<cv::Vec3b>(y, x)[0] = img.at<cv::Vec3b>(y, x + 1)[0];	// copy blue
				//img.at<cv::Vec3b>(y, x)[1] = img.at<cv::Vec3b>(y, x + 1)[1];	// copy green
				//img.at<cv::Vec3b>(y, x)[2] = img.at<cv::Vec3b>(y, x + 1)[2];	// copy red
            }
            }
        }
        w.show();
        w.waitKey();
    }
    return 0;
}
