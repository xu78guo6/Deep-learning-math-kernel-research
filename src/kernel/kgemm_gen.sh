#!/bin/bash

# Build gemm kernel instantiation
#

src_file=$1; dst_dir=$2; cc=$3

if [ ! -f $src_file ] || [ ! -d $dst_dir ]; then
  "Invalid src_file=$src_file or dst_dir=$dst_dir"
  exit -1
fi

__kgemm_generate_inst__() {
  ktype=$1; dtype=$2; V=$3; Vx=$4; I=$5; S=$6; F=$7;

  cat <<@ > $dst_dir/elk_${ktype}_otj_${dtype}_${V}_${Vx}_${I}_${S}_${F}.cpp
// _generated_kgemm_file_
//
#include "el_intrin.hpp"
#include "el_def.hpp"
#include "el_utils.hpp"
#include "el_stl.hpp"
#include "elx_conv.hpp"
#include "$src_file"

using namespace euler;

namespace euler {

#undef E
#define E(O, T) \\
  gemm_kernel_binder::gemm_ker_cls<conv_impl::$dtype, \\
      $V, $Vx, $I, $S, $F, O, T, 1>::$ktype
  gemm_kernel_binder::k${ktype}<conv_impl::$dtype>
      *gemm_kernel_binder::k${ktype}_${dtype}_${V}_${Vx}_${I}_${S}_${F}[8][32] =
  { // 8
    { // 32
      E(1, 1),
      E(1, 2),
      E(1, 3),
      E(1, 4),
      E(1, 5),
      E(1, 6),
      E(1, 7),
      E(1, 8),
      E(1, 9),
      E(1, 10),
      E(1, 11),
      E(1, 12),
      E(1, 13),
      E(1, 14),
      E(1, 15),
      E(1, 16),
      E(1, 17),
      E(1, 18),
      E(1, 19),
      E(1, 20),
      E(1, 21),
      E(1, 22),
      E(1, 23),
      E(1, 24),
      E(1, 25),
      E(1, 26),
      E(1, 27),
      E(1, 28),
      E(1, 29),
      E(1, 30),
      E(1, 31),
    },
    { // 32
      E(2, 1),
      E(2, 2),
      E(2, 3),
      E(2, 4),
      E(2, 5),
      E(2, 6),
      E(2, 7),
      E(2, 8),
      E(2, 9),
      E(2, 10),
      E(2, 11),
      E(2, 12),
      E(2, 13),
      E(2, 14),
    },
    { // 32
      E(3, 1),
      E(3, 2),
      E(3, 3),
      E(3, 4),
      E(3, 5),
      E(3, 6),
      E(3, 7),
      E(3, 8),
      E(3, 9),
      E(3, 10),
      E(3, 11),
      E(3, 12),
      E(3, 13),
      E(3, 14),
    },
    { // 32
      E(4, 1),
      E(4, 2),
      E(4, 3),
      E(4, 4),
      E(4, 5),
      E(4, 6),
      E(4, 7),
      E(4, 8),
      E(4, 9),
      E(4, 10),
      E(4, 11),
      E(4, 12),
      E(4, 13),
      E(4, 14),
    },
    { // 32
      E(5, 1),
      E(5, 2),
      E(5, 3),
      E(5, 4),
      E(5, 5),
    },
    { // 32
      E(6, 1),
      E(6, 2),
      E(6, 3),
      E(6, 4),
    },
    { // 32
      E(7, 1),
      E(7, 2),
      E(7, 3),
    },
    { // 32
      E(8, 1),
      E(8, 2),
      E(8, 3),
      E(8, 4),
      E(8, 5),
      E(8, 6),
      E(8, 7),
      E(8, 8),
    },
  };

} // namespace
@
}

__kconv_generate_inst__() {
  ktype=$1; dtype=$2; V=$3; Vx=$4; I=$5; S=$6; F=$7;

  cat <<@ > $dst_dir/elk_${ktype}_otj_${dtype}_${V}_${Vx}_${I}_${S}_${F}.cpp
// _generated_kernel_file_
//
#include "el_intrin.hpp"
#include "el_def.hpp"
#include "el_utils.hpp"
#include "el_stl.hpp"
#include "elx_conv.hpp"
#include "$src_file"

using namespace euler;

namespace euler {

#undef E
#define E(O, T, K) \\
  gemm_kernel_binder::gemm_ker_cls<conv_impl::$dtype, \\
      $V, $Vx, $I, $S, $F, O, T, K>::$ktype
  gemm_kernel_binder::k${ktype}<conv_impl::$dtype>
      *gemm_kernel_binder::k${ktype}_${dtype}_${V}_${Vx}_${I}_${S}_${F}[8][32][3] =
  { // 8
    { // 32
      { E(1, 1,  3), E(1, 1,  5), E(1, 1,  7) },
      { E(1, 2,  3), E(1, 2,  5), E(1, 2,  7) },
      { E(1, 3,  3), E(1, 3,  5), E(1, 3,  7) },
      { E(1, 4,  3), E(1, 4,  5), E(1, 4,  7) },
      { E(1, 5,  3), E(1, 5,  5), E(1, 5,  7) },
      { E(1, 6,  3), E(1, 6,  5), E(1, 6,  7) },
      { E(1, 7,  3), E(1, 7,  5), E(1, 7,  7) },
      { E(1, 8,  3), E(1, 8,  5), E(1, 8,  7) },
      { E(1, 9,  3), E(1, 9,  5), E(1, 9,  7) },
      { E(1, 10, 3), E(1, 10, 5), E(1, 10, 7) },
      { E(1, 11, 3), E(1, 11, 5), E(1, 11, 7) },
      { E(1, 12, 3), E(1, 12, 5), E(1, 12, 7) },
      { E(1, 13, 3), E(1, 13, 5), E(1, 13, 7) },
      { E(1, 14, 3), E(1, 14, 5), E(1, 14, 7) },
      { E(1, 15, 3), E(1, 15, 5), E(1, 15, 7) },
      { E(1, 16, 3), E(1, 16, 5), E(1, 16, 7) },
      { E(1, 17, 3), E(1, 17, 5), E(1, 17, 7) },
      { E(1, 18, 3), E(1, 18, 5), E(1, 18, 7) },
      { E(1, 19, 3), E(1, 19, 5), E(1, 19, 7) },
      { E(1, 20, 3), E(1, 20, 5), E(1, 20, 7) },
      { E(1, 21, 3), E(1, 21, 5), E(1, 21, 7) },
      { E(1, 22, 3), E(1, 22, 5), E(1, 22, 7) },
      { E(1, 23, 3), E(1, 23, 5), E(1, 23, 7) },
      { E(1, 24, 3), E(1, 24, 5), E(1, 24, 7) },
      { E(1, 25, 3), E(1, 25, 5), E(1, 25, 7) },
      { E(1, 26, 3), E(1, 26, 5), E(1, 26, 7) },
      { E(1, 27, 3), E(1, 27, 5), E(1, 27, 7) },
      { E(1, 28, 3), E(1, 28, 5), E(1, 28, 7) },
      { E(1, 29, 3), E(1, 29, 5), E(1, 29, 7) },
      { E(1, 30, 3), E(1, 30, 5), E(1, 30, 7) },
      { E(1, 31, 3), E(1, 31, 5), E(1, 31, 7) },
    },
    { // 32
      { E(2,  1, 3), E(2,  1, 5), E(2,  1, 7) },
      { E(2,  2, 3), E(2,  2, 5), E(2,  2, 7) },
      { E(2,  3, 3), E(2,  3, 5), E(2,  3, 7) },
      { E(2,  4, 3), E(2,  4, 5), E(2,  4, 7) },
      { E(2,  5, 3), E(2,  5, 5), E(2,  5, 7) },
      { E(2,  6, 3), E(2,  6, 5), E(2,  6, 7) },
      { E(2,  7, 3), E(2,  7, 5), E(2,  7, 7) },
      { E(2,  8, 3), E(2,  8, 5), E(2,  8, 7) },
      { E(2,  9, 3), E(2,  9, 5), E(2,  9, 7) },
      { E(2, 10, 3), E(2, 10, 5), E(2, 10, 7) },
      { E(2, 11, 3), E(2, 11, 5), E(2, 11, 7) },
      { E(2, 12, 3), E(2, 12, 5), E(2, 12, 7) },
      { E(2, 13, 3), E(2, 13, 5), E(2, 13, 7) },
      { E(2, 14, 3), E(2, 14, 5), E(2, 14, 7) },
    },
    { // 32
      { E(3,  1, 3), E(3,  1, 5), E(3,  1, 7) },
      { E(3,  2, 3), E(3,  2, 5), E(3,  2, 7) },
      { E(3,  3, 3), E(3,  3, 5), E(3,  3, 7) },
      { E(3,  4, 3), E(3,  4, 5), E(3,  4, 7) },
      { E(3,  5, 3), E(3,  5, 5), E(3,  5, 7) },
      { E(3,  6, 3), E(3,  6, 5), E(3,  6, 7) },
      { E(3,  7, 3), E(3,  7, 5), E(3,  7, 7) },
      { E(3,  8, 3), E(3,  8, 5), E(3,  8, 7) },
      { E(3,  9, 3), E(3,  9, 5), E(3,  9, 7) },
      { E(3, 10, 3), E(3, 10, 5), E(3, 10, 7) },
      { E(3, 11, 3), E(3, 11, 5), E(3, 11, 7) },
      { E(3, 12, 3), E(3, 12, 5), E(3, 12, 7) },
      { E(3, 13, 3), E(3, 13, 5), E(3, 13, 7) },
      { E(3, 14, 3), E(3, 14, 5), E(3, 14, 7) },
    },
#if 0 // No perf benefit
    { // 32
      { E(4,  1, 3), E(4,  1, 5), E(4,  1, 7) },
      { E(4,  2, 3), E(4,  2, 5), E(4,  2, 7) },
      { E(4,  3, 3), E(4,  3, 5), E(4,  3, 7) },
      { E(4,  4, 3), E(4,  4, 5), E(4,  4, 7) },
      { E(4,  5, 3), E(4,  5, 5), E(4,  5, 7) },
      { E(4,  6, 3), E(4,  6, 5), E(4,  6, 7) },
      { E(4,  7, 3), E(4,  7, 5), E(4,  7, 7) },
      { E(4,  8, 3), E(4,  8, 5), E(4,  8, 7) },
      { E(4,  9, 3), E(4,  9, 5), E(4,  9, 7) },
      { E(4, 10, 3), E(4, 10, 5), E(4, 10, 7) },
      { E(4, 11, 3), E(4, 11, 5), E(4, 11, 7) },
      { E(4, 12, 3), E(4, 12, 5), E(4, 12, 7) },
      { E(4, 13, 3), E(4, 13, 5), E(4, 13, 7) },
      { E(4, 14, 3), E(4, 14, 5), E(4, 14, 7) },
    },
    { // 32
      { E(5, 1, 3), E(5, 1, 5), E(5, 1, 7) },
      { E(5, 2, 3), E(5, 2, 5), E(5, 2, 7) },
      { E(5, 3, 3), E(5, 3, 5), E(5, 3, 7) },
      { E(5, 4, 3), E(5, 4, 5), E(5, 4, 7) },
      { E(5, 5, 3), E(5, 5, 5), E(5, 5, 7) },
    },
    { // 32
      { E(6, 1, 3), E(6, 1, 5), E(6, 1, 7) },
      { E(6, 2, 3), E(6, 2, 5), E(6, 2, 7) },
      { E(6, 3, 3), E(6, 3, 5), E(6, 3, 7) },
      { E(6, 4, 3), E(6, 4, 5), E(6, 4, 7) },
    },
    { // 32
      { E(7, 1, 3), E(7, 1, 5), E(7, 1, 7) },
      { E(7, 2, 3), E(7, 2, 5), E(7, 2, 7) },
      { E(7, 3, 3), E(7, 3, 5), E(7, 3, 7) },
    },
    { // 32
      { E(8, 1, 3), E(8, 1, 5), E(8, 1, 7) },
      { E(8, 2, 3), E(8, 2, 5), E(8, 2, 7) },
      { E(8, 3, 3), E(8, 3, 5), E(8, 3, 7) },
      { E(8, 4, 3), E(8, 4, 5), E(8, 4, 7) },
      { E(8, 5, 3), E(8, 5, 5), E(8, 5, 7) },
      { E(8, 6, 3), E(8, 6, 5), E(8, 6, 7) },
      { E(8, 7, 3), E(8, 7, 5), E(8, 7, 7) },
      { E(8, 8, 3), E(8, 8, 5), E(8, 8, 7) },
    },
#endif
  };

} // namespace
@
}

eval $($cc -DBUILD_OTJ_TBL -E $src_file 2>&1 | grep _generate_inst_)
