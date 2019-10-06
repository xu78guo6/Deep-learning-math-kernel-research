#include "elx_int8_conv_wino_gemm.hpp"
#include "el_parallel.hpp"

namespace euler {

template <typename GarrayTypes, const int A, const int V, const int I>
void elx_int8_conv_wino_gemm_t<GarrayTypes, A, V, I>::setup(elx_conv_params_t *conv_xc)
{
  attr_ = A != 6 ? set_bit(0x0, AT_FMAOPT_MASK) : 0x0;
  xc    = conv_xc;
  mthr_ = xc->nthreads;

  bind_kernel_functions();
}

template <typename GarrayTypes, const int A, const int V, const int I>
void elx_int8_conv_wino_gemm_t<GarrayTypes, A, V, I>::bind_kernel_functions()
{
  u8s8_gemm_kernel_binder::bind<1, GKF_CCC>(xc->O, xc->T, &ker_u8s8_gemm_);
  u8s8_gemm_kernel_binder::bind<1, GKF_CCC>(xc->O, xc->Tr, &ker_u8s8_gemm0_);
}

// tweights:      O4 | O3, I3, A, A, O2, I2, V1, V, Vx
// tinputs:        t2 | A, A, I3, I2, T, V1, Vx
// toutput:   t2, O4 | A, A, O3, O2, T, V
// weights_scale  O4 | O3, O2, V
// facotr:        O4 | O3, A, A, O2, V
template <typename GarrayTypes, const int A, const int V, const int I>
void elx_int8_conv_wino_gemm_t<GarrayTypes, A, V, I>::execute(
    ToutputType *toutput, uint8_t *tinput, int8_t *tweights,
    TscaleType *src_scale, TscaleType *weights_scale, TscaleType *weights_factor,
    int _t2, int Tz, int _I4)
{
  auto ker_gemm = (_t2 == xc->t2 - 1) ? ker_u8s8_gemm0_ : ker_u8s8_gemm_;

  MD6(uint8_t, atinput, tinput, A, A, xc->I3, xc->I2, Tz, V);
  MD6(ToutputType, atoutput, toutput, A, A, xc->O3, xc->O2, Tz, V);
  MD5(int8_t, atweights, tweights, xc->O3, xc->I3, A, A,
      xc->O2 * xc->I2 * V * V);
  MD5(TscaleType, aweights_scale, weights_scale, xc->O3, A, A, xc->O2, V);
  MD5(TscaleType, aweights_factor, weights_factor, xc->O3, A, A, xc->O2, V);
  MD5(TscaleType, asrc_scale, src_scale, xc->I3,  A, A, 2, xc->T);

  iter_each (_hA, A) {
  iter_each (_wA, A) {
  iter_each (_I3, xc->I3) {
  iter_each (_O3, xc->O3) {
    int attr = _I3 == 0 && _I4 == 0 ?  set_bit(attr_, AT_CLEAR_OUTPUT_MASK) : attr_;
    if (_I4 == xc->I4 - 1 && _I3 == xc->I3 - 1) {
      attr = set_bit(attr, AT_RESTORE_OUTPUT_MASK);
      if (xc->Ir != V)
        attr = set_bit(attr, AT_Ir_MASK);
    }

    TscaleType *asrc_s = nullptr, *asrc_z = nullptr;
    if (xc->sampling_kind == COARSE) {
      asrc_s = &md5(asrc_scale, 0, 0, 0, 0, 0);
      asrc_z = &md5(asrc_scale, 0, 0, 0, 1, 0);
    } else if (xc->sampling_kind == FINE) {
      asrc_s = &md5(asrc_scale, _I3, _hA, _wA, 0, 0);
      asrc_z = &md5(asrc_scale, _I3, _hA, _wA, 1, 0);
    } else { // CALIBRATED
      // nothing to do.
      // asrc_s/asrc_z are folded into weights_scale/factor
    }

    ker_gemm(*(elx_conv_params_t *)xc,
        &md6(atoutput, _hA, _wA, _O3, 0, 0, 0),
        nullptr,
        &md6(atinput, _hA, _wA, _I3, 0, 0, 0),
        &md5(atweights, _O3, _I3, _hA, _wA, 0),
        nullptr, attr, asrc_s, asrc_z,
        &md5(aweights_scale, _O3, _hA, _wA, 0, 0),
        &md5(aweights_factor, _O3, _hA, _wA, 0, 0));
  }}}}
}

template <typename GarrayTypes, const int A, const int V, const int I>
void elx_int8_conv_wino_gemm_t<GarrayTypes, A, V, I>::execute_na(
    ToutputType *toutput, uint8_t *tinput, int8_t *tweights,
    TscaleType *src_scale, TscaleType *weights_scale, TscaleType *weights_factor,
    int _t2, int Tz, int _I4)
{
  auto ker_gemm = (_t2 == xc->t2 - 1) ? ker_u8s8_gemm0_ : ker_u8s8_gemm_;

  MD6(uint8_t, atinput, tinput, A, A, xc->I3, xc->I2, Tz, V);
  MD6(ToutputType, atoutput, toutput, A, A, xc->O3, xc->O2, Tz, V);
  MD5(int8_t, atweights, tweights, xc->O3, xc->I3, A, A,
      xc->O2 * xc->I2 * V * V);
  MD5(TscaleType, aweights_scale, weights_scale, xc->O3, A, A, xc->O2, V);
  MD5(TscaleType, aweights_factor, weights_factor, xc->O3, A, A, xc->O2, V);
  MD5(TscaleType, asrc_scale, src_scale, xc->I3, A, A, 2, Tz);

  bool scramble = (xc->T == xc->Tr) || (xc->t2 >= 2 * mthr_);
  if (scramble) {
    int it_start = estl::current_thread_index();
    iter_each(i, A * A) {
      int n = (it_start + i) % (A * A);
      int _wA = n % A;
      int _hA = n / A;
      iter_each(_I3, xc->I3) {
      iter_each(_O3, xc->O3) {
        auto attr = _I3 == 0 ? set_bit(attr_, AT_CLEAR_OUTPUT_MASK) : attr_;
        if (_I4 == xc->I4 - 1 && _I3 == xc->I3 - 1) {
          attr = set_bit(attr, AT_RESTORE_OUTPUT_MASK);
          if (xc->Ir != V)
            attr = set_bit(attr, AT_Ir_MASK);
        }

        TscaleType *asrc_s = nullptr, *asrc_z = nullptr;
        if (xc->sampling_kind == COARSE) {
          asrc_s = &md5(asrc_scale, 0, 0, 0, 0, 0);
          asrc_z = &md5(asrc_scale, 0, 0, 0, 1, 0);
        } else if (xc->sampling_kind == FINE) {
          asrc_s = &md5(asrc_scale, _I3, _hA, _wA, 0, 0);
          asrc_z = &md5(asrc_scale, _I3, _hA, _wA, 1, 0);
        } else { // CALIBRATED
          // nothing to do.
          // asrc_s/asrc_z are folded into weights_scale/factor
        }

        ker_gemm(*(elx_conv_params_t *)xc,
            &md6(atoutput, _hA, _wA, _O3, 0, 0, 0),
            nullptr,
            &md6(atinput, _hA, _wA, _I3, 0, 0, 0),
            &md5(atweights, _O3, _I3, _hA, _wA, 0),
            nullptr, attr, asrc_s, asrc_z,
            &md5(aweights_scale, _O3, _hA, _wA, 0, 0),
            &md5(aweights_factor, _O3, _hA, _wA, 0, 0));
      }}
    }
  } else {
    iter_each(_hA, A) {
    iter_each(_wA, A) {
    iter_each(_I3, xc->I3) {
    iter_each(_O3, xc->O3) {
      auto attr = _I3 == 0 ? set_bit(attr_, AT_CLEAR_OUTPUT_MASK) : attr_;
      if (_I4 == xc->I4 - 1 && _I3 == xc->I3 - 1) {
        attr = set_bit(attr, AT_RESTORE_OUTPUT_MASK);
        if (xc->Ir != V)
          attr = set_bit(attr, AT_Ir_MASK);
      }

      ker_gemm(*(elx_conv_params_t *)xc,
          &md6(atoutput, _hA, _wA, _O3, 0, 0, 0),
          nullptr,
          &md6(atinput, _hA, _wA, _I3, 0, 0, 0),
          &md5(atweights, _O3, _I3, _hA, _wA, 0),
          nullptr, attr,
          &md5(asrc_scale, _I3, _hA, _wA, 0, 0),
          &md5(asrc_scale, _I3, _hA, _wA, 1, 0),
          &md5(aweights_scale, _O3, _hA, _wA, 0, 0),
          &md5(aweights_factor, _O3, _hA, _wA, 0, 0));
    }}}}
  }
}

template <typename GarrayTypes, const int A, const int V, const int I>
void elx_int8_conv_wino_gemm_t<GarrayTypes, A, V, I>::execute_na(
    ToutputType *toutput, uint8_t *tinput, int8_t *tweights,
    TscaleType *src_scale, TscaleType *src_factor,
    TscaleType *weights_scale, TscaleType *weights_factor, int _I4)
{
  int ithr = estl::current_thread_index();
  THREAD_FOR2(5, 2, mthr_, ithr,
              [&](int _hA, int _wA, int _I3, int _O3, int _t2) {
    MD2(uint8_t, atinput2, tinput, xc->t2, A * A * xc->I3 * xc->I2 * xc->T * V);
    MD2(ToutputType, atoutput2, toutput, xc->t2, A * A * xc->O3 * xc->O2 * xc->T * V);
    MD5(int8_t, atweights, tweights, xc->O3, xc->I3, A, A, xc->O2 * xc->I2 * V * V);
    MD5(TscaleType, aweights_scale, weights_scale, xc->O3, A, A, xc->O2, V);
    MD5(TscaleType, aweights_factor, weights_factor, xc->O3, A, A, xc->O2, V);
    MD6(TscaleType, asrc_scale, src_scale, xc->t2, A, A, xc->I3, 2, xc->T);
    int Tz = _t2 == (xc->t2 - 1) ? xc->Tr : xc->T;
    MD6(uint8_t, atinput6, &md2(atinput2, _t2, 0), A, A, xc->I3, xc->I2, Tz, V);
    MD6(ToutputType, atoutput6, &md2(atoutput2, _t2, 0), A, A, xc->O3, xc->O2, Tz, V);
    auto ker_gemm = (_t2 == xc->t2 - 1) ? ker_u8s8_gemm0_ : ker_u8s8_gemm_;

    int attr = _I3 == 0 ?  set_bit(attr_, AT_CLEAR_OUTPUT_MASK) : attr_;
    if (_I3 == xc->I3 - 1) {
      attr = set_bit(attr, AT_RESTORE_OUTPUT_MASK);
      if (_I4 == xc->I4 - 1 && xc->Ir != V)
        attr = set_bit(attr, AT_Ir_MASK);
    }

    TscaleType *asrc_s = nullptr, *asrc_z = nullptr;
    if (xc->sampling_kind == COARSE) {
      asrc_s = &md6(asrc_scale, 0, 0, 0, 0, 0, 0);
      asrc_z = &md6(asrc_scale, 0, 0, 0, 0, 1, 0);
    } else if (xc->sampling_kind == FINE) {
      asrc_s = &md6(asrc_scale, _t2, _hA, _wA, _I3, 0, 0);
      asrc_z = &md6(asrc_scale, _t2, _hA, _wA, _I3, 1, 0);
    } else { // CALIBRATED
      // nothing to do.
      // asrc_s/asrc_z are folded into weights_scale/factor
    }

    ker_gemm(*xc, &md6(atoutput6, _hA, _wA, _O3, 0, 0, 0),
        nullptr,
        &md6(atinput6, _hA, _wA, _I3, 0, 0, 0),
        &md5(atweights, _O3, _I3, _hA, _wA, 0),
        nullptr, attr, asrc_s, asrc_z,
        &md5(aweights_scale, _O3, _hA, _wA, 0, 0),
        &md5(aweights_factor, _O3, _hA, _wA, 0, 0));
  }, A, A, xc->I3, xc->O3, xc->t2);
}

} // namespace euler
