#include "windowRasterized.h"

namespace cf {

cf::WindowRasterized::WindowRasterized(int width, int height, const char* windowName, const cf::Color& startColor)
    : Window2D(width, height, windowName, startColor) {
    this->setInvertYAxis(false);
}

cf::WindowRasterized::WindowRasterized(const char* filePath) : Window2D(filePath) { this->setInvertYAxis(false); }
}
