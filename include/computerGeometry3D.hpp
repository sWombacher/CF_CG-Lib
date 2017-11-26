#ifndef COMPUTER_GEOMETRY_H_H
#define COMPUTER_GEOMETRY_H_H

#pragma once
#include "utils.h"
#include <algorithm>
#include <type_traits>

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

/// TODO
/// operators and value in front

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
            this->sortBladeTypes();
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
        void sortBladeTypes() {
            if (this->outerProduct.empty())
                return;

            // temporary add type to vector
            this->outerProduct.insert(this->outerProduct.begin(), this->type);

            // use insertion sort approach for sorting
            size_t numSwaps = 0;
            for (size_t i = 1; i < this->outerProduct.size(); ++i) {
                for (size_t k = i; k > 0; --k) {
                    if (int(this->outerProduct[k]) < int(this->outerProduct[k - 1])) {
                        ++numSwaps;
                        std::swap(this->outerProduct[k], this->outerProduct[k - 1]);
                    } else
                        break;
                }
            }
            if (numSwaps & 1)
                this->factor *= _ValueType(-1.0);

            // remove first element
            this->type = this->outerProduct.front();
            this->outerProduct.erase(this->outerProduct.begin());

            // error checking, TYPE::VALUE may only be standalone
            // always the last element (after sorting)
            // outerProduct.size() > 1 is implicit, see function begin
            if (/* this->outerProduct.size() > 1 && */ this->outerProduct.back() == TYPE::VALUE)
                throw std::runtime_error("Error: Blade::TYPE::VALUE in outerProducts!");
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
        if (first)
            os << '0';
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

    // conversion operator to _ValueType
    template<typename _VType>
    explicit operator _VType () const{
        static_assert(std::is_arithmetic<_VType>::value);
        if (this->m_Data.empty())
            return _ValueType(0);

        if (this->m_Data.size() > 1 || this->m_Data.front().type != Blade::TYPE::VALUE)
            throw std::runtime_error("Error: Conversion from MultiVector to value not possible");

        return this->m_Data.front().factor;
    }

    // divison operator for values
    template<typename _VType>
    MultiVector<decltype(_ValueType(1) / _VType(1))> operator/ (const _VType& value) const {
        static_assert(std::is_arithmetic<_VType>::value);
        MultiVector<decltype(_ValueType(1) / _VType(1))> res = *this;
        for (auto& e : res.m_Data)
            e.factor /= value;

        res._createConsistentData();
        return res;
    }
    template<typename _VType>
    MultiVector<decltype(_ValueType(1) / _VType(1))>& operator/=(const _VType& value) {
        *this = *this / value;
        return *this;
    }

    // addition/subtraction multivector with constant
    template<typename _VType>
    MultiVector<_ValueType> operator+(const _VType& value) const{
        static_assert(std::is_arithmetic<_VType>::value);
        MultiVector<_ValueType> result = *this;
        bool valueFound = false;
        for (auto& e : result.m_Data){
            if (e.type == Blade::TYPE::VALUE){
                e.factor += value;
                valueFound = true;
            }
        }
        if (!valueFound)
            result.m_Data.emplace_back(Blade::TYPE::VALUE, value);

        result._createConsistentData();
        return result;
    }
    template<typename _VType>
    MultiVector<_ValueType>& operator+=(const _VType& value) {
        *this = *this + value;
        return *this;
    }
    template<typename _VType>
    MultiVector<_ValueType> operator-(const _VType& value) const{
        return *this + (-value);
    }
    template<typename _VType>
    MultiVector<_ValueType>& operator-=(const _VType& value) {
        *this = *this - value;
        return *this;
    }

    // addition/subtraction two multivectors
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
        using BLADE = typename MVEC::Blade;

        MVEC result;
        for (const auto& e_lhs : this->m_Data) {
            for (const auto& e_rhs : rhs.m_Data) {
                const size_t sumSize = e_lhs.outerProduct.size() + e_rhs.outerProduct.size();
                if (sumSize == 0) {
                    // simple case
                    const int t0 = int(e_lhs.type);
                    const int t1 = int(e_rhs.type);

                    if (t0 == int(Blade::TYPE::VALUE)) {
                        result.m_Data.emplace_back(typename MVEC::Blade::TYPE(int(e_lhs.type)), e_lhs.factor * e_rhs.factor);
                        continue;
                    }
                    if (t1 == int(Blade::TYPE::VALUE)) {
                        result.m_Data.emplace_back(typename MVEC::Blade::TYPE(int(e_rhs.type)), e_lhs.factor * e_rhs.factor);
                        continue;
                    }
                    vt value;
                    if (t0 == t1 && t0 >= int(Blade::TYPE::E1) && t0 <= int(Blade::TYPE::E3))
                        value = 1.0;
                    else if (t0 == int(Blade::TYPE::E0) && t1 == int(MultiVector<_VType>::Blade::TYPE::EINF))
                        value = -1.0;
                    else if (t0 == int(Blade::TYPE::EINF) && t1 == int(MultiVector<_VType>::Blade::TYPE::E0))
                        value = -1.0;
                    else
                        continue;

                    result.m_Data.emplace_back(MVEC::Blade::TYPE::VALUE, value * e_lhs.factor * e_rhs.factor);
                } else if (sumSize == 1) {
                    // advanced case
                    // we can use
                    // a . (b ^ c) = (a . b) * c - (a . c) * b
                    // (b ^ c) . a = - a . (b ^ c)
                    // where . = inner product, ^ = outer product, * = geometry product
                    BLADE a, b, c;
                    vt factor;
                    if (e_lhs.outerProduct.empty()) {
                        a = e_lhs;
                        b = e_rhs;
                        factor = vt(1.0);
                    } else {
                        a = e_rhs;
                        b = e_lhs;
                        factor = vt(-1.0);
                    }
                    c.factor = factor;
                    c.type = b.outerProduct.front();
                    b.outerProduct.clear();
                    const MVEC mvec_a(a), mvec_b(b), mvec_c(c);
                    const MVEC res = ((mvec_a * mvec_b) & mvec_c) - ((mvec_a * mvec_c) & mvec_b);

					/// TEST
					MVEC t0 = mvec_a * mvec_b;
					MVEC t1 = t0 & mvec_c;
					MVEC r1 = (mvec_a * mvec_b) & mvec_c;
					MVEC r2 = (mvec_a * mvec_c) & mvec_b;
					MVEC r12 = r1 - r2;
					/// END TEST
                    result.m_Data.insert(result.m_Data.end(), res.m_Data.begin(), res.m_Data.end());
                } else {
                    // hard case
                    // here we use
                    // (a1 ^ b1 ^ ... ^ n1) . (a2 ^ b2 ^ ... ^ m2) = (a1 ^ b1 ^ ... ^ n1) . (a2 ^ (b2 ^ ... ^ m2) )
                    // to be able to use the same equations from above (advanced case), with
                    //
                    // (a1 ^ b1 ^ ... ^ n1) = a
                    //  a2 = b
                    // (b2 ^ ... ^ m2) = c
                    //
                    // NOTE:
                    //  hard and advanced case are very very similar
                    //  hence they CAN be merged, however
                    //  it is not recommended due to mathmatical understanding
                    BLADE a, b, c;
                    vt factor;
                    if (e_lhs.outerProduct.empty()) {
                        a = e_lhs;
                        b = e_rhs;
                        factor = 1.0;
                    } else {
                        a = e_rhs;
                        b = e_lhs;
                        factor = -1.0;
                    }
                    c = b;
                    c.factor = factor;
                    b.outerProduct.clear();
                    c.type = c.outerProduct.front();
                    c.outerProduct.erase(c.outerProduct.begin());
                    const MVEC mvec_a(a), mvec_b(b), mvec_c(c);
                    const MVEC res = ((mvec_a * mvec_b) & mvec_c) - ((mvec_a * mvec_c) & mvec_b);
                    result.m_Data.insert(result.m_Data.end(), res.m_Data.begin(), res.m_Data.end());
                }
            }
        }
        result._createConsistentData();
        return result;
    }
    template <typename _VType> MultiVector<_ValueType>& operator*=(const MultiVector<_VType>& rhs) {
        *this = *this * rhs;
        return *this;
    }

    // outer product
    template <typename _VType>
    MultiVector<decltype(_ValueType(1) * _VType(1))> operator%(const MultiVector<_VType>& rhs) const {
        using vt = decltype(_ValueType(1) * _VType(1));
        using MVEC = MultiVector<vt>;
        using BLADE = typename MVEC::Blade;

        MVEC result;
        for (const auto& e_lhs : this->m_Data) {
            for (const auto& e_rhs : rhs.m_Data) {
                if (int(e_lhs.type) == int(BLADE::TYPE::VALUE)) {
                    BLADE blade = e_rhs;
                    blade.factor *= vt(e_lhs.factor);
                    result.m_Data.push_back(std::move(blade));
                    continue;
                }
                if (int(e_rhs.type) == int(BLADE::TYPE::VALUE)) {
                    BLADE blade = e_lhs;
                    blade.factor *= vt(e_rhs.factor);
                    result.m_Data.push_back(std::move(blade));
                    continue;
                }
                auto findType = [](const BLADE& blade, typename BLADE::TYPE type) -> bool {
                    if (blade.type == type)
                        return true;
                    for (const auto& e : blade.outerProduct) {
                        if (e == type)
                            return true;
                    }
                    return false;
                };

                const BLADE& blade(e_lhs);
                bool found = findType(blade, typename BLADE::TYPE(int(e_rhs.type)));
                for (const auto& e : e_rhs.outerProduct)
                    found |= findType(blade, typename BLADE::TYPE(int(e)));

                if (found) {
                    // simple case
                    // result is 0
                    // we don't have to do anything
                } else {
                    // advanced case
                    // add all blades & sort blades, beaware of
                    //   +e1^e2 = -e2^e1
                    // while sorting
                    BLADE blade2insert(e_lhs);
                    blade2insert.factor *= vt(e_rhs.factor);
                    blade2insert.outerProduct.push_back(typename BLADE::TYPE(int(e_rhs.type)));
                    for (const auto& e : e_rhs.outerProduct)
                        blade2insert.outerProduct.push_back(typename BLADE::TYPE(int(e)));

                    // blade sorting will be done in '_createConsistentData' function at the end
                    result.m_Data.push_back(std::move(blade2insert));
                }
            }
        }
        result._createConsistentData();
        return result;
    }
    template <typename _VType> MultiVector<_ValueType>& operator%=(const MultiVector<_VType>& rhs) {
        *this = *this % rhs;
        return *this;
    }
    template <typename _VType>
    MultiVector<decltype(_ValueType(1) * _VType(1))> operator^(const MultiVector<_VType>& rhs) const {
        return *this % rhs;
    }
    template <typename _VType> MultiVector<_ValueType>& operator^=(const MultiVector<_VType>& rhs) {
        return *this %= rhs;
    }

    // geometry product
    template <typename _VType>
    MultiVector<decltype(_ValueType(1) * _VType(1))> operator&(const MultiVector<_VType>& rhs) const {
        const auto& lhs = *this;
        MultiVector<_VType> rhs_noValue;
        MultiVector<_ValueType> lhs_noValue;

        decltype(MultiVector<_ValueType>::operator&(rhs)) res;
        auto foo = [&res](const auto& original, auto& cpy, const auto& other){
            for (const auto& data : original.m_Data){
                if (data.type != std::decay<decltype(original)>::type::Blade::TYPE::VALUE)
                    cpy.m_Data.emplace_back(data.type, data.factor);
                else{
                    typename std::decay<decltype(res)>::type tmp;
                    tmp.m_Data.emplace_back(std::decay<decltype(tmp.m_Data.front())>::type::Blade::TYPE::VALUE, data.factor);
                    res += tmp % other;
                }
            }
        };
        foo(lhs, lhs_noValue, rhs);
        foo(rhs, rhs_noValue, lhs_noValue);
        res += (lhs_noValue % rhs_noValue) + (lhs_noValue * rhs_noValue);
        res._createConsistentData();
        return res;
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
		if (this->m_Data.empty()) {
			// if we dont have data set value to 0
			this->m_Data.emplace_back(Blade::TYPE::VALUE, 0.0);
			return;
		}

        // sort outerProducts by TYPE
        for (auto& e : this->m_Data)
            e.sortBladeTypes();

        // sort blades
        std::sort(this->m_Data.begin(), this->m_Data.end(),
                  [](const Blade& lhs, const Blade& rhs) { return lhs.type2int() < rhs.type2int(); });

        // combine blades
        for (int id_front = this->m_Data.front().type2int(), i = 1; i < int(this->m_Data.size()); ++i) {
            const int id_back = this->m_Data[size_t(i)].type2int();
            if (id_front == id_back) {
                this->m_Data[size_t(i - 1)].factor += this->m_Data[size_t(i)].factor;
                this->m_Data.erase(this->m_Data.begin() + i--);
            }
            id_front = id_back;
        }

        // remove blades with factor of 0
        for (size_t i = 0; i < this->m_Data.size(); ++i) {
            if (MultiVector<_ValueType>::_CmpZero(this->m_Data[i].factor))
                this->m_Data.erase(this->m_Data.begin() + int(i--));
        }

		// if we dont have data set value to 0
		if (this->m_Data.empty()) 
			this->m_Data.emplace_back(Blade::TYPE::VALUE, 0.0);
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
