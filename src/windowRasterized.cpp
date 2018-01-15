#include "windowRasterized.h"

namespace cf {

cf::WindowRasterized::WindowRasterized(int width, int height, const std::string& windowName, const cf::Color& startColor)
    : Window2D(width, height, windowName, startColor) {
    this->setInvertYAxis(false);
}

cf::WindowRasterized::WindowRasterized(const std::string& filePath) : Window2D(filePath) { this->setInvertYAxis(false); }
} // namespace cf
