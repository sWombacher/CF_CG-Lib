#ifndef COMPUTER_GEOMETRY_H_H
#define COMPUTER_GEOMETRY_H_H

#pragma once
#include "utils.h"
#include <algorithm>

namespace cf {

// declarations
template <typename _VType> struct MultiVector;
typedef MultiVector<long double> ldMultiVector;
typedef MultiVector<double> dMultiVector;
typedef MultiVector<float> fMultiVector;
typedef MultiVector<double> Vec;

namespace literals {
static cf::ldMultiVector operator"" _e1(long double value);
static cf::ldMultiVector operator"" _e2(long double value);
static cf::ldMultiVector operator"" _e3(long double value);
static cf::ldMultiVector operator"" _e0(long double value);
static cf::ldMultiVector operator"" _einf(long double value);

static cf::ldMultiVector operator"" _E(long double value);
static cf::ldMultiVector operator"" _value(long double value);
}

template <typename _ValueType> struct MultiVector {
    MultiVector() = default;

    template <typename _VType> MultiVector(const MultiVector<_VType>& vec) {
        this->m_Data.clear();
        for (const auto& e : vec.m_Data) {
            Blade blade(typename Blade::TYPE(int(e.type)), _ValueType(e.factor));
            for (const auto& t : e.outerProduct)
                blade.outerProduct.push_back(typename Blade::TYPE(t));
            this->m_Data.push_back(blade);
        }
        this->_createConsistentData();
    }

    template <typename... _Blades> MultiVector(const _Blades&... blades) {
        this->_initBlades(blades...);
        this->_createConsistentData();
    }

    struct Blade {
        enum class TYPE { E1 = 1, E2, E3, EINF, E0, VALUE = std::numeric_limits<int16_t>::max() };
        Blade() = default;
        template <typename _RHS_Blade> Blade(const _RHS_Blade& rhs) {
            this->type = Blade::TYPE(int(rhs.type));
            this->factor = _ValueType(rhs.factor);
            for (const auto& t : rhs.outerProduct)
                this->outerProduct.push_back(Blade::TYPE(int(t)));
        }
        Blade(TYPE t, const _ValueType& f) : type(t), factor(f) {}
        TYPE type;
        _ValueType factor;
        std::vector<TYPE> outerProduct;

        bool sameType(const Blade& rhs) const { return this->type2int() == rhs.type2int(); }
        int type2int() const {
            int res = int(this->type);
            for (const auto& e : this->outerProduct) {
                res <<= 3; // we've 5 possible types, next possible power of 2 is 8 -> shift of 3
                res += int(e);
            }
            return res;
        }

        static std::string TYPE_TO_STRING(const TYPE& type) {
            switch (type) {
            case TYPE::E1:
                return "e1";
            case TYPE::E2:
                return "e2";
            case TYPE::E3:
                return "e3";
            case TYPE::E0:
                return "e0";
            case TYPE::EINF:
                return "einf";
            default:
                return "";
            }
        }

        std::string getCompleteType() const {
            std::string str = Blade::TYPE_TO_STRING(this->type);
            for (const auto& t : this->outerProduct)
                str += '^' + Blade::TYPE_TO_STRING(t);
            return str;
        }

        template <typename _RHS_Blade> Blade& operator=(const _RHS_Blade& rhs) {
            this->type = Blade::TYPE(int(rhs.type));
            this->factor = _ValueType(rhs.factor);
            for (const auto& t : rhs.outerProduct)
                this->outerProduct.push_back(Blade::TYPE(int(t)));
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const Blade& blade) {
            os << blade.factor << (blade.type != Blade::TYPE::VALUE ? '*' : ' ') << blade.getCompleteType();
            return os;
        }
    };

    void setData(const std::vector<Blade>& data) { this->m_Data = data; }
    const std::vector<Blade>& getData() const { return this->m_Data; }

    friend std::ostream& operator<<(std::ostream& os, const MultiVector<_ValueType>& vec) {
        bool first = true;
        for (const auto& e : vec.m_Data) {
            os << (first ? "" : " + ") << e;
            first = false;
        }
        os << std::endl;
        return os;
    }

    // assigment operator
    template <typename _VType> MultiVector<_ValueType>& operator=(const MultiVector<_VType>& rhs) {
        this->m_Data.clear();
        for (const auto& e : rhs.m_Data) {
            Blade blade(typename Blade::TYPE(int(e.type)), _ValueType(e.factor));
            for (const auto& t : e.outerProduct)
                blade.outerProduct.push_back(typename Blade::TYPE(t));
            this->m_Data.push_back(blade);
        }
        return *this;
    }

    // plus/minus operator
    MultiVector<_ValueType>& operator+() const { return *this; }
    MultiVector<_ValueType>& operator-() {
        for (auto& e : this->m_Data)
            e.factor *= _ValueType(-1.0);
        return *this;
    }

    // duality operator
    MultiVector<_ValueType> operator*() const {
        using namespace literals;
        static const MultiVector<_ValueType> minus_I = -1.0_e1 ^ 1.0_e2 ^ 1.0_e3 ^ 1.0_einf ^ 1.0_e0;
        return *this * minus_I;
    }

    // reversor operator
    MultiVector<_ValueType> operator~() const {
        using Vec = MultiVector<_ValueType>;
        Vec result;
        for (const auto& e : this->m_Data) {
            if (e.outerProduct.empty())
                result.m_Data.push_back(e);
            else {
                result.m_Data.emplace_back(e.outerProduct.back(), e.factor);
                auto& blade = result.m_Data.back();
                for (size_t i = e.outerProduct.size() - 2; i < e.outerProduct.size(); --i)
                    blade.outerProduct.push_back(e.outerProduct[i]);
                blade.outerProduct.push_back(e.type);
            }
        }
        return result;
    }

    // addition/subtraction
    template <typename _VType>
    MultiVector<decltype(_ValueType(1) + _VType(1))> operator+(const MultiVector<_VType>& rhs) const {
        using vt = decltype(_ValueType(1) + _VType(1));

        MultiVector<vt> result = *this;
        result.m_Data.reserve(rhs.m_Data.size());
        for (const auto& e : rhs.m_Data)
            result.m_Data.push_back(e);

        result._createConsistentData(); // sort & add & remove stuff will be handled anyways
        return result;
    }
    template <typename _VType> MultiVector<_ValueType>& operator+=(const MultiVector<_VType>& rhs) {
        *this = *this + rhs;
        return *this;
    }

    template <typename _VType>
    MultiVector<decltype(_ValueType(1) * _VType(1))> operator-(const MultiVector<_VType>& rhs) const {
        using vt = decltype(_ValueType(1) * _VType(1));
        MultiVector<vt> result = rhs;
        for (auto& e : result.m_Data)
            e.factor *= vt(-1.0);
        return *this + result;
    }
    template <typename _VType> MultiVector<_ValueType>& operator-=(const MultiVector<_VType>& rhs) {
        *this = *this - rhs;
        return *this;
    }

    // multiply with constant
    MultiVector<_ValueType>& operator*=(const _ValueType& rhs) {
        for (auto& e : this->m_Data)
            e.factor *= rhs;
        this->_createConsistentData();
        return *this;
    }
    MultiVector<_ValueType> operator*(const _ValueType& rhs) const {
        MultiVector<_ValueType> res = *this;
        res *= rhs;
        return res;
    }

    // innerproduct
    template <typename _VType>
    MultiVector<decltype(_ValueType(1) * _VType(1))> operator*(const MultiVector<_VType>& rhs) const {
        using vt = decltype(_ValueType(1) * _VType(1));
        using MVEC = MultiVector<vt>;

        MVEC result;
        for (const auto& e_lhs : this->m_Data) {
            for (const auto& e_rhs : rhs.m_Data) {
                /// TODO
                const size_t sumSize = e_lhs.outerProduct.size() + e_rhs.outerProduct.size();
                if (sumSize == 0) {
                    // simple case
                    const int t0 = int(e_lhs.type);
                    const int t1 = int(e_lhs.type);
                    vt value = 0.0;
                    if (t0 == t1 && t0 >= int(Blade::TYPE::E1) && t0 <= int(Blade::TYPE::E3))
                        value = 1.0;
                    else if (t0 == int(Blade::TYPE::E0) && t1 == int(MultiVector<_VType>::Blade::TYPE::EINF))
                        value = 1.0; /// TODO check
                    else if (t0 == int(Blade::TYPE::EINF) && t1 == int(MultiVector<_VType>::Blade::TYPE::E0))
                        value = -1.0; /// TODO check

                    result.m_Data.emplace_back(MVEC::Blade::TYPE::VALUE, value);
                } else if (sumSize == 1) {
                    // advanced case
                    // we can use
                    // a . (b ^ c) = (a . b) * c - (a . c) * b
                    // (b ^ c) . a = - a . (b ^ c)
                    // where . = inner product, ^ = outer product, * = geometry product
                    MVEC a, b, c;
                    if (e_lhs.outerProduct.size()) {
                        /// TODO
                    } else {
                        /// TODO
                    }
                    return ((a * b) & c) - ((a * c) & b);

                } else {
                    // hard case
                }
            }
        }
        result._createConsistentData();
        return result;
    }
    template <typename _VType> MultiVector<_ValueType>& operator*=(const MultiVector<_VType>& rhs) const {
        *this = *this * rhs;
        return *this;
    }

    // outer product
    template <typename _VType>
    MultiVector<decltype(_ValueType(1) * _VType(1))> operator%(const MultiVector<_VType>& rhs) const {
        using vt = decltype(_ValueType(1) * _VType(1));

        MultiVector<vt> result;
        for (const auto& e_lhs : this->m_Data) {
            for (const auto& e_rhs : rhs.m_Data) {
                /// TODO
            }
        }
        result._createConsistentData();
        return result;
    }
    template <typename _VType> MultiVector<_ValueType>& operator%=(const MultiVector<_VType>& rhs) const {
        *this = *this % rhs;
        return *this;
    }
    template <typename _VType>
    MultiVector<decltype(_ValueType(1) * _VType(1))> operator^(const MultiVector<_VType>& rhs) const {
        return *this % rhs;
    }
    template <typename _VType> MultiVector<_ValueType>& operator^=(const MultiVector<_VType>& rhs) const {
        return *this %= rhs;
    }

    // geometry product
    template <typename _VType>
    MultiVector<decltype(_ValueType(1) * _VType(1))> operator&(const MultiVector<_VType>& rhs) const {
        const auto& lhs = *this;
        return (lhs % rhs) + (lhs * rhs);
    }
    template <typename _VType> MultiVector<decltype(_ValueType(1) * _VType(1))> operator&=(const MultiVector<_VType>& rhs) {
        *this = *this & rhs;
        return *this;
    }

  private:
    template <typename _VType> friend struct MultiVector;

    template <typename _Blade> void _initBlades(const _Blade& blade) { this->m_Data.push_back(blade); }
    template <typename _First, typename... _Blades> void _initBlades(const _First& first, const _Blades&... blades) {
        this->m_Data.push_back(first);
        this->_initBlades(blades...);
    }

    inline void _createConsistentData() {
        if (this->m_Data.empty())
            return;

        // sort outerProducts by TYPE
        for (auto& e : this->m_Data) {
            if (e.outerProduct.empty())
                continue;

            decltype(e.outerProduct) blade = {e.type};
            blade.insert(blade.end(), e.outerProduct.begin(), e.outerProduct.end());
            const auto cpy = blade;
            std::sort(blade.begin(), blade.end());

            // calc swap distance
            size_t swapDist = 0;
            for (size_t i = 0; i < cpy.size(); ++i) {
                const auto range = std::equal_range(blade.begin(), blade.end(), cpy[i]);
                if (std::distance(range.first, range.second) != 1)
                    throw std::runtime_error("Error: Incorrect range!");

                swapDist += size_t(std::abs(int(i) - std::distance(blade.begin(), range.first)));
            }
            if (swapDist) {
                if (swapDist & 1) // test for odd number of swaps
                    e.factor *= _ValueType(-1.0);

                e.type = blade.front();
                e.outerProduct.clear();
                e.outerProduct.insert(e.outerProduct.end(), std::next(blade.begin()), blade.end());
            }
        }

        // sort blades
        std::sort(this->m_Data.begin(), this->m_Data.end(),
                  [](const Blade& lhs, const Blade& rhs) { return lhs.type2int() < rhs.type2int(); });

        // combine blades
        for (int id_front = this->m_Data.front().type2int(), i = 1; i < int(this->m_Data.size()); ++i) {
            const int id_back = this->m_Data[size_t(i)].type2int();
            if (id_front == id_back) {
                this->m_Data[size_t(i - 1)].factor += this->m_Data[size_t(i)].factor;
                this->m_Data[size_t(i)].factor = _ValueType(0.0);
            }
            id_front = id_back;
        }

        // remove blades with factor of 0
        for (size_t i = 0; i < this->m_Data.size(); ++i) {
            if (MultiVector<_ValueType>::_CmpZero(this->m_Data[i].factor))
                this->m_Data.erase(this->m_Data.begin() + int(i--));
        }
    }

    template <typename T> static bool _CmpZero(const T& value) { return std::abs(value) < T(0.000001); }

    std::vector<Blade> m_Data;
};

namespace literals {
static cf::ldMultiVector operator"" _e1(long double value) {
    using Blade = cf::ldMultiVector::Blade;
    return cf::ldMultiVector(Blade(Blade::TYPE::E1, value));
}
static cf::ldMultiVector operator"" _e2(long double value) {
    using Blade = cf::ldMultiVector::Blade;
    return cf::ldMultiVector(Blade(Blade::TYPE::E2, value));
}
static cf::ldMultiVector operator"" _e3(long double value) {
    using Blade = cf::ldMultiVector::Blade;
    return cf::ldMultiVector(Blade(Blade::TYPE::E3, value));
}
static cf::ldMultiVector operator"" _e0(long double value) {
    using Blade = cf::ldMultiVector::Blade;
    return cf::ldMultiVector(Blade(Blade::TYPE::E0, value));
}
static cf::ldMultiVector operator"" _einf(long double value) {
    using Blade = cf::ldMultiVector::Blade;
    return cf::ldMultiVector(Blade(Blade::TYPE::EINF, value));
}

static cf::ldMultiVector operator"" _E(long double value) {
    using Blade = cf::ldMultiVector::Blade;
    Blade tmp(Blade::TYPE::EINF, value);
    tmp.outerProduct.push_back(Blade::TYPE::E0);
    return cf::ldMultiVector(tmp);
}
static cf::ldMultiVector operator"" _value(long double value) {
    using Blade = cf::ldMultiVector::Blade;
    Blade tmp(Blade::TYPE::VALUE, value);
    return cf::ldMultiVector(tmp);
}
}
}

#endif
