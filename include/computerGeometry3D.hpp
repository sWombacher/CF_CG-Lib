#ifndef COMPUTER_GEOMETRY_H_H
#define COMPUTER_GEOMETRY_H_H

#include "utils.h"
#include <Eigen/Core>

namespace cf {

template <typename _ValueType = double> class Vec4 {
    typedef Vec4<_ValueType> MY_TYPE;

  public:
    template <typename _vt0, typename _vt1, typename _vt2>
    decltype(_vt0(0) + _vt1(0) + _vt2(0) - (_vt0(0) * _vt1(0) * _vt2(0)))
        extendedCrossproduct(const Vec4<_vt0>& v0, const Vec4<_vt1>& v1, const Vec4<_vt2>& v2) {
        typedef decltype(_vt0(0) + _vt1(0) + _vt2(0) - (_vt0(0) * _vt1(0) * _vt2(0))) vt;
        Vec4<vt> result;
        Eigen::Matrix<vt, 3, 3> mat;
        auto calculateDeterminate = [&mat, &v0, &v1, &v2](size_t ignore_idx) -> vt {
            for (size_t i = 0; i < 4; ++i) {
                if (i == ignore_idx)
                    continue;

                // clang-format off
                mat[0][0] = v0[1]; mat[0][1] = v0[2]; mat[0][2] = v0[3];
                mat[1][0] = v1[1]; mat[1][1] = v1[2]; mat[1][2] = v1[3];
                mat[2][0] = v2[1]; mat[2][1] = v2[2]; mat[2][2] = v2[3];
                // clang-format on
            }
            return mat.determinate();
        };
        for (size_t i = 0; i < 4; ++i)
            result[i] = calculateDeterminate(i);
        return result;
    }

    _ValueType& operator[](int idx) { return this->m_Data[idx]; }
    const _ValueType& operator[](int idx) const { return this->m_Data[idx]; }

  private:
    Eigen::Vector4d m_Data;
};

template <typename _VType> std::ostream& operator<<(std::ostream& os, const cf::Vec4<_VType>& rhs) { return os; }
}

#endif
