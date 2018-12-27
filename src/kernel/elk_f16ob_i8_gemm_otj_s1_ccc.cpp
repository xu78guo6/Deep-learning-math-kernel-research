#include "el_intrin.hpp"
#include "el_def.hpp"
#include "el_utils.hpp"
#include "el_stl.hpp"
#include "elx_conv.hpp"
#include "elk_gemm_otj.hxx"

using namespace euler;

namespace euler {

#undef E
#define E(O, T, r)                                                            \
  gemm_kernel_binder::gemm_ker_cls<conv_impl::INT8_F16ob, 16, 4,              \
      ISA_SKX_AVX512, 1, GKF_CCC, O, T, r>::execute
  gemm_kernel_binder::ker<conv_impl::INT8_F16ob>
    *gemm_kernel_binder::ker_i8_f16ob_s1_ccc[8][32][2] =
  { // 8
    { // 32
      { E(1, 1,  0), E(1, 1, 1), },
      { E(1, 2,  0), E(1, 2, 1), },
      { E(1, 3,  0), E(1, 3, 1), },
      { E(1, 4,  0), E(1, 4, 1), },
      { E(1, 5,  0), E(1, 5, 1), },
      { E(1, 6,  0), E(1, 6, 1), },
      { E(1, 7,  0), E(1, 7, 1), },
      { E(1, 8,  0), E(1, 8, 1), },
      { E(1, 9,  0), E(1, 9, 1), },
      { E(1, 10, 0), E(1, 10, 1), },
      { E(1, 11, 0), E(1, 11, 1), },
      { E(1, 12, 0), E(1, 12, 1), },
      { E(1, 13, 0), E(1, 13, 1), },
      { E(1, 14, 0), E(1, 14, 1), },
      { E(1, 15, 0), E(1, 15, 1), },
      { E(1, 16, 0), E(1, 16, 1), },
      { E(1, 17, 0), E(1, 17, 1), },
      { E(1, 18, 0), E(1, 18, 1), },
      { E(1, 19, 0), E(1, 19, 1), },
      { E(1, 20, 0), E(1, 20, 1), },
      { E(1, 21, 0), E(1, 21, 1), },
      { E(1, 22, 0), E(1, 22, 1), },
      { E(1, 23, 0), E(1, 23, 1), },
      { E(1, 24, 0), E(1, 24, 1), },
      { E(1, 25, 0), E(1, 25, 1), },
      { E(1, 26, 0), E(1, 26, 1), },
      { E(1, 27, 0), E(1, 27, 1), },
      { E(1, 28, 0), E(1, 28, 1), },
      { E(1, 29, 0), E(1, 29, 1), },
      { E(1, 30, 0), E(1, 30, 1), },
      { E(1, 31, 0), E(1, 31, 1), },
    },
    { // 32
      { E(2, 1, 0),  E(2, 1, 1), },
      { E(2, 2, 0),  E(2, 2, 1), },
      { E(2, 3, 0),  E(2, 3, 1), },
      { E(2, 4, 0),  E(2, 4, 1), },
      { E(2, 5, 0),  E(2, 5, 1), },
      { E(2, 6, 0),  E(2, 6, 1), },
      { E(2, 7, 0),  E(2, 7, 1), },
      { E(2, 8, 0),  E(2, 8, 1), },
      { E(2, 9, 0),  E(2, 9, 1), },
      { E(2, 10, 0), E(2, 10, 1), },
      { E(2, 11, 0), E(2, 11, 1), },
      { E(2, 12, 0), E(2, 12, 1), },
      { E(2, 13, 0), E(2, 13, 1), },
      { E(2, 14, 0), E(2, 14, 1), },
    },
    { // 32
      { E(3, 1, 0),  E(3, 1, 1), },
      { E(3, 2, 0),  E(3, 2, 1), },
      { E(3, 3, 0),  E(3, 3, 1), },
      { E(3, 4, 0),  E(3, 4, 1), },
      { E(3, 5, 0),  E(3, 5, 1), },
      { E(3, 6, 0),  E(3, 6, 1), },
      { E(3, 7, 0),  E(3, 7, 1), },
      { E(3, 8, 0),  E(3, 8, 1), },
      { E(3, 9, 0),  E(3, 9, 1), },
      { E(3, 10, 0), E(3, 10, 1), },
      { E(3, 11, 0), E(3, 11, 1), },
      { E(3, 12, 0), E(3, 12, 1), },
      { E(3, 13, 0), E(3, 13, 1), },
      { E(3, 14, 0), E(3, 14, 1), },
    },
    { // 32
      { E(4, 1, 0),  E(4, 1, 1), },
      { E(4, 2, 0),  E(4, 2, 1), },
      { E(4, 3, 0),  E(4, 3, 1), },
      { E(4, 4, 0),  E(4, 4, 1), },
      { E(4, 5, 0),  E(4, 5, 1), },
      { E(4, 6, 0),  E(4, 6, 1), },
      { E(4, 7, 0),  E(4, 7, 1), },
      { E(4, 8, 0),  E(4, 8, 1), },
      { E(4, 9, 0),  E(4, 9, 1), },
      { E(4, 10, 0), E(4, 10, 1), },
      { E(4, 11, 0), E(4, 11, 1), },
      { E(4, 12, 0), E(4, 12, 1), },
      { E(4, 13, 0), E(4, 13, 1), },
      { E(4, 14, 0), E(4, 14, 1), },
    },
    { // 32
      { E(5, 1, 0),  E(5, 1, 1), },
      { E(5, 2, 0),  E(5, 2, 1), },
      { E(5, 3, 0),  E(5, 3, 1), },
      { E(5, 4, 0),  E(5, 4, 1), },
      { E(5, 5, 0),  E(5, 5, 1), },
    },
    { // 32
      { E(6, 1, 0),  E(6, 1, 1), },
      { E(6, 2, 0),  E(6, 2, 1), },
      { E(6, 3, 0),  E(6, 3, 1), },
      { E(6, 4, 0),  E(6, 4, 1), },
    },
    { // 32
      { E(7, 1, 0),  E(7, 1, 1), },
      { E(7, 2, 0),  E(7, 2, 1), },
      { E(7, 3, 0),  E(7, 3, 1), },
    },
    { // 32
      { E(8, 1, 0),  E(8, 1, 1), },
      { E(8, 2, 0),  E(8, 2, 1), },
      { E(8, 3, 0),  E(8, 3, 1), },
      { E(8, 4, 0),  E(8, 4, 1), },
      { E(8, 5, 0),  E(8, 5, 1), },
      { E(8, 6, 0),  E(8, 6, 1), },
      { E(8, 7, 0),  E(8, 7, 1), },
      { E(8, 8, 0),  E(8, 8, 1), },
    },
  };

} // namespace
