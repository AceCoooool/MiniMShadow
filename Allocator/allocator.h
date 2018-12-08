#ifndef MINIMSHADOW_ALLOCATOR_H
#define MINIMSHADOW_ALLOCATOR_H

#include "../Basic/logging.h"
#include "../Basic/base.h"

namespace MiniMShadow {
    /*------------------------内存空间辅助函数----------------------------*/
    /**
     * 分配"对齐"的空间大小: num_line * lspace cells
     * \param out_pitch 返回参数, 每行实际分配的大小
     * \param lspace: 单行需要的内存数
     * \param num_line： 行数
     */
    inline void *AlignedMallocPitch(size_t *out_pitch, size_t lspace, size_t num_line) {
        const int64_t bits = 4;
        const int64_t mask = (1 << bits) - 1;  // 15
        size_t pitch = ((lspace + mask) >> bits) << bits;  // 按16对齐
        *out_pitch = pitch;
        void *res;
        int ret = posix_memalign(&res, 1 << bits, pitch * num_line);
        CHECK_EQ(ret, 0) << "AlignedMallocPitch failed";
        if (res == nullptr) {
            LOG(FATAL) << "AlignedMallocPitch failed";
        }
        return res;
    }

    /**
     * 释放指针指向的空间
     * \param ptr: 指向待释放空间的指针
     */
    inline void AlignedFree(void *ptr) {
        free(ptr);
    }

    /*------------------------内存空间函数 cpu----------------------------*/
    template<typename xpu>
    inline void *AllocHost_(size_t size);
    template<typename xpu>
    inline void FreeHost_(void *dptr);

    template<>
    inline void *AllocHost_<cpu>(size_t size) {
        size_t pitch;
        return AlignedMallocPitch(&pitch, size, 1);
    }
    template<>
    inline void FreeHost_<cpu>(void *dptr) {
        AlignedFree(dptr);
    }



}

#endif //MINIMSHADOW_ALLOCATOR_H
