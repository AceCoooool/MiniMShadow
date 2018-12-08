#ifndef MINIMSHADOW_BASE_H
#define MINIMSHADOW_BASE_H

#include <cstdlib>
#include <iostream>

namespace MiniMShadow {
    /* --------------------一些特殊(特定意义)宏和别名--------------------- */
    // 强制内敛宏
#define MSHADOW_FORCE_INLINE inline __attribute__((always_inline))
#define MSHADOW_XINLINE MSHADOW_FORCE_INLINE

    // 别名
    using index_t = int64_t;


    /* -----------------------"运行"设备---------------------- */
    /* CPU */
    struct cpu {
        // 标记: 是否为cpu
        static const bool kDevCPU = true;
        // 标记: 辨别哪种设备
        static const int kDevMask = 1 << 0;
    };

    /* GPU */
    struct gpu {
        // 标记: 是否为cpu
        static const bool kDevCPU = false;
        // 标记: 辨别哪种设备
        static const int kDevMask = 1 << 1;
    };


}

#endif //MINIMSHADOW_BASE_H
