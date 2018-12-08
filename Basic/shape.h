#ifndef MINIMSHADOW_SHAPE_H
#define MINIMSHADOW_SHAPE_H

#include "base.h"

namespace MiniMShadow {
    /**
     * tensor的shape
     * \tparam tensor的维度数目
     */
    template<int dimension>
    struct Shape {
        /* ---------------------------成员变量------------------------ */
        // 当前维度数目
        static const int kDimension = dimension;
        // 当前维度数目-1
        static const int kSubdim = dimension - 1;
        // 维度信息
        index_t shape_[kDimension];

        /* ---------------------------构造函数----------------------- */
        // 默认构造函数
        inline Shape(void) {}
        // 构造函数
        MSHADOW_XINLINE Shape(const Shape<kDimension> &s) {
            for (int i = 0; i < kDimension; ++i) {
                this->shape_[i] = s[i];
            }
        }

        /* ---------------------------运算符重载----------------------- */
        // 返回第idx维的大小
        MSHADOW_XINLINE int64_t &operator[](index_t idx) {
            return shape_[idx];
        }
        // 返回第idx维的大小(const)
        MSHADOW_XINLINE const index_t &operator[](index_t idx) const {
            return shape_[idx];
        }
        // 判读是否与Shape相等
        MSHADOW_XINLINE bool operator==(const Shape<kDimension> &s) const {
            for (int i = 0; i < kDimension; ++i) {
                if (s.shape_[i] != this->shape_[i]) return false;
            }
            return true;
        }
        // 判读是否与Shape不相等
        MSHADOW_XINLINE bool operator!=(const Shape<kDimension> &s) const {
            return !(*this == s);
        }

        /* ---------------------------实用操作----------------------- */
        // "完全铺平": 比如[2, 3, 4] -> [24]
        MSHADOW_XINLINE Shape<1> FlatTo1D(void) const {
            Shape<1> s;
            s[0] = this->Size();
            return s;
        }
        // "铺平"(保留2维): 比如[2, 3, 4] -> [6, 4]
        MSHADOW_XINLINE Shape<2> FlatTo2D(void) const {
            Shape<2> s;
            s.shape_[1] = this->shape_[kDimension - 1];
            index_t ymax = 1;
            for (int i = 0; i < kDimension - 1; ++i) {
                ymax *= this->shape_[i];
            }
            s.shape_[0] = ymax;
            return s;
        }
        // 返回元素个数: 比如[2, 3, 4], 则为24
        MSHADOW_XINLINE index_t Size(void) const {
            index_t size = this->shape_[0];
            for (int i = 1; i < kDimension; ++i) {
                size *= this->shape_[i];
            }
            return size;
        }
        // 返回shape的乘积: 比如[2, 3, 4, 5]的ProdShape(1, 2)=3x4=12
        MSHADOW_XINLINE index_t ProdShape(int dimstart, int dimend) const {
            index_t num = 1;
            for (int i = dimstart; i < dimend; ++i) {
                num *= this->shape_[i];
            }
            return num;
        }
        // 返回子空间, 此处子空间指的是去除第1维: 比如[2, 3, 4]->[3, 4]
        MSHADOW_XINLINE Shape<kSubdim> SubShape(void) const {
            Shape<kSubdim> s;
            for (int i = 0; i < kSubdim; ++i) {
                s.shape_[i] = this->shape_[i + 1];
            }
            return s;
        }
        // 指定范围的空间: 比如[2, 3, 4, 5]的Slice<1, 3> -> [3, 4]
        template<int dimstart, int dimend>
        MSHADOW_XINLINE Shape<dimend - dimstart> Slice(void) const {
            Shape<dimend - dimstart> s;
            for (int i = dimstart; i < dimend; ++i) {
                s[i - dimstart] = this->shape_[i];
            }
            return s;
        }

        /* -------------------------友元函数-------------------------- */
        // 输出信息
        template<int dim>
        friend std::ostream &operator<<(std::ostream &os, const Shape<dim> &shape);
    };

    template<int ndim>
    inline std::ostream &operator<<(std::ostream &os, const Shape<ndim> &shape) {
        os << '(';
        for (int i = 0; i < ndim; ++i) {
            if (i != 0) os << ',';
            os << shape[i];
        }
        // python style tuple
        if (ndim == 1) os << ',';
        os << ')';
        return os;
    }
}

#endif //MINIMSHADOW_SHAPE_H
