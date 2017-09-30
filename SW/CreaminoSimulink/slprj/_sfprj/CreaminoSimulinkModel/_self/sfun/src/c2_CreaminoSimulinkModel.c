/* Include files */

#include <stddef.h>
#include "blas.h"
#include "CreaminoSimulinkModel_sfun.h"
#include "c2_CreaminoSimulinkModel.h"
#include "mwmathutil.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance->chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance->instanceNumber)
#include "CreaminoSimulinkModel_sfun_debug_macros.h"
#define _SF_MEX_LISTEN_FOR_CTRL_C(S)   sf_mex_listen_for_ctrl_c(sfGlobalDebugInstanceStruct,S);

/* Type Definitions */

/* Named Constants */
#define CALL_EVENT                     (-1)

/* Variable Declarations */

/* Variable Definitions */
static real_T _sfTime_;
static const char * c2_debug_family_names[14] = { "ynop", "yF", "indice", "i",
  "kk", "j", "u", "nargin", "nargout", "labels", "indiceGrigU2", "AnoP", "BnoP",
  "yP" };

/* Function Declarations */
static void initialize_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance);
static void initialize_params_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance);
static void enable_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance);
static void disable_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance);
static void c2_update_debugger_state_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance);
static const mxArray *get_sim_state_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance);
static void set_sim_state_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance, const mxArray *c2_st);
static void finalize_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance);
static void sf_gateway_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance);
static void mdl_start_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance);
static void c2_chartstep_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance);
static void initSimStructsc2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance);
static void init_script_number_translation(uint32_T c2_machineNumber, uint32_T
  c2_chartNumber, uint32_T c2_instanceNumber);
static const mxArray *c2_sf_marshallOut(void *chartInstanceVoid, void
  *c2_e_inData);
static void c2_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_b_yP, const char_T *c2_identifier, real_T
  c2_c_y[6144]);
static void c2_b_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_h_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_c_y[6144]);
static void c2_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_b_sf_marshallOut(void *chartInstanceVoid, void
  *c2_e_inData);
static const mxArray *c2_c_sf_marshallOut(void *chartInstanceVoid, void
  *c2_e_inData);
static const mxArray *c2_d_sf_marshallOut(void *chartInstanceVoid, void
  *c2_e_inData);
static const mxArray *c2_e_sf_marshallOut(void *chartInstanceVoid, void
  *c2_e_inData);
static real_T c2_c_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_h_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_f_sf_marshallOut(void *chartInstanceVoid, void
  *c2_e_inData);
static void c2_d_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_h_u, const emlrtMsgIdentifier *c2_parentId,
  real32_T c2_c_y[16777216]);
static void c2_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static real32_T c2_e_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_h_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_g_sf_marshallOut(void *chartInstanceVoid, void
  *c2_e_inData);
static void c2_f_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_h_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_c_y[5587904]);
static void c2_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_h_sf_marshallOut(void *chartInstanceVoid, real32_T
  c2_b_inData_data[], int32_T c2_inData_sizes[2]);
static void c2_g_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_h_u, const emlrtMsgIdentifier *c2_parentId,
  real32_T c2_b_y_data[], int32_T c2_y_sizes[2]);
static void c2_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, real32_T c2_outData_data[],
  int32_T c2_outData_sizes[2]);
static void c2_info_helper(const mxArray **c2_info);
static const mxArray *c2_emlrt_marshallOut(const char * c2_h_u);
static const mxArray *c2_b_emlrt_marshallOut(const uint32_T c2_h_u);
static real32_T c2_norm(SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance,
  real32_T c2_x[3]);
static real32_T c2_eml_xnrm2(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, real32_T c2_x[3]);
static void c2_below_threshold(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance);
static const mxArray *c2_i_sf_marshallOut(void *chartInstanceVoid, void
  *c2_e_inData);
static int32_T c2_h_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_h_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_g_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static uint8_T c2_i_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_b_is_active_c2_CreaminoSimulinkModel, const
  char_T *c2_identifier);
static uint8_T c2_j_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_h_u, const emlrtMsgIdentifier *c2_parentId);
static void init_dsm_address_info(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance);
static void init_simulink_io_address(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance);

/* Function Definitions */
static void initialize_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance)
{
  chartInstance->c2_sfEvent = CALL_EVENT;
  _sfTime_ = sf_get_time(chartInstance->S);
  chartInstance->c2_is_active_c2_CreaminoSimulinkModel = 0U;
}

static void initialize_params_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void enable_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void disable_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void c2_update_debugger_state_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static const mxArray *get_sim_state_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance)
{
  const mxArray *c2_st;
  const mxArray *c2_c_y = NULL;
  int32_T c2_i0;
  real_T c2_h_u[6144];
  const mxArray *c2_d_y = NULL;
  uint8_T c2_hoistedGlobal;
  uint8_T c2_i_u;
  const mxArray *c2_e_y = NULL;
  c2_st = NULL;
  c2_st = NULL;
  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_createcellmatrix(2, 1), false);
  for (c2_i0 = 0; c2_i0 < 6144; c2_i0++) {
    c2_h_u[c2_i0] = (*chartInstance->c2_yP)[c2_i0];
  }

  c2_d_y = NULL;
  sf_mex_assign(&c2_d_y, sf_mex_create("y", c2_h_u, 0, 0U, 1U, 0U, 2, 64, 96),
                false);
  sf_mex_setcell(c2_c_y, 0, c2_d_y);
  c2_hoistedGlobal = chartInstance->c2_is_active_c2_CreaminoSimulinkModel;
  c2_i_u = c2_hoistedGlobal;
  c2_e_y = NULL;
  sf_mex_assign(&c2_e_y, sf_mex_create("y", &c2_i_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_c_y, 1, c2_e_y);
  sf_mex_assign(&c2_st, c2_c_y, false);
  return c2_st;
}

static void set_sim_state_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance, const mxArray *c2_st)
{
  const mxArray *c2_h_u;
  real_T c2_dv0[6144];
  int32_T c2_i1;
  chartInstance->c2_doneDoubleBufferReInit = true;
  c2_h_u = sf_mex_dup(c2_st);
  c2_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c2_h_u, 0)), "yP",
                      c2_dv0);
  for (c2_i1 = 0; c2_i1 < 6144; c2_i1++) {
    (*chartInstance->c2_yP)[c2_i1] = c2_dv0[c2_i1];
  }

  chartInstance->c2_is_active_c2_CreaminoSimulinkModel = c2_i_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell(c2_h_u, 1)),
     "is_active_c2_CreaminoSimulinkModel");
  sf_mex_destroy(&c2_h_u);
  c2_update_debugger_state_c2_CreaminoSimulinkModel(chartInstance);
  sf_mex_destroy(&c2_st);
}

static void finalize_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void sf_gateway_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance)
{
  int32_T c2_i2;
  int32_T c2_i3;
  int32_T c2_i4;
  int32_T c2_i5;
  _SFD_SYMBOL_SCOPE_PUSH(0U, 0U);
  _sfTime_ = sf_get_time(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 0U, chartInstance->c2_sfEvent);
  for (c2_i2 = 0; c2_i2 < 16777216; c2_i2++) {
    _SFD_DATA_RANGE_CHECK((real_T)(*chartInstance->c2_g_u)[c2_i2], 0U);
  }

  for (c2_i3 = 0; c2_i3 < 87311; c2_i3++) {
    _SFD_DATA_RANGE_CHECK((real_T)(*chartInstance->c2_b_labels)[c2_i3], 1U);
  }

  for (c2_i4 = 0; c2_i4 < 87311; c2_i4++) {
    _SFD_DATA_RANGE_CHECK((real_T)(*chartInstance->c2_b_indiceGrigU2)[c2_i4], 2U);
  }

  chartInstance->c2_sfEvent = CALL_EVENT;
  c2_chartstep_c2_CreaminoSimulinkModel(chartInstance);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_CreaminoSimulinkModelMachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
  for (c2_i5 = 0; c2_i5 < 6144; c2_i5++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c2_yP)[c2_i5], 3U);
  }

  _SFD_DATA_RANGE_CHECK((real_T)*chartInstance->c2_AnoP, 4U);
  _SFD_DATA_RANGE_CHECK((real_T)*chartInstance->c2_BnoP, 5U);
}

static void mdl_start_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void c2_chartstep_c2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance)
{
  real32_T c2_hoistedGlobal;
  real32_T c2_b_hoistedGlobal;
  int32_T c2_i6;
  int32_T c2_i7;
  int32_T c2_i8;
  real32_T c2_b_AnoP;
  real32_T c2_b_BnoP;
  uint32_T c2_debug_family_var_map[14];
  int32_T c2_ynop_sizes[2];
  real_T c2_indice;
  real_T c2_i;
  real32_T c2_kk;
  real_T c2_j;
  real_T c2_nargin = 5.0;
  real_T c2_nargout = 1.0;
  real_T c2_b_yP[6144];
  int32_T c2_i9;
  int32_T c2_i10;
  int32_T c2_i11;
  int32_T c2_i12;
  boolean_T c2_b0;
  boolean_T c2_b1;
  boolean_T c2_b2;
  int32_T c2_i13;
  boolean_T c2_b3;
  boolean_T c2_b4;
  boolean_T c2_b5;
  int32_T c2_i14;
  int32_T c2_loop_ub;
  int32_T c2_i15;
  int32_T c2_b_loop_ub;
  int32_T c2_i16;
  int32_T c2_i17;
  int32_T c2_b_i;
  real32_T c2_c_BnoP;
  int32_T c2_i18;
  int32_T c2_b_kk;
  real32_T c2_ynop[3];
  int32_T c2_i19;
  int32_T c2_c_i;
  int32_T c2_b_j;
  int32_T c2_i20;
  int32_T c2_i21;
  boolean_T c2_b[87311];
  int32_T c2_i22;
  real_T c2_c_y[64];
  int32_T c2_i23;
  int32_T c2_i24;
  int32_T c2_c_j;
  int32_T c2_i25;
  int32_T c2_i26;
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 0U, chartInstance->c2_sfEvent);
  c2_hoistedGlobal = *chartInstance->c2_AnoP;
  c2_b_hoistedGlobal = *chartInstance->c2_BnoP;
  for (c2_i6 = 0; c2_i6 < 16777216; c2_i6++) {
    chartInstance->c2_u[c2_i6] = (*chartInstance->c2_g_u)[c2_i6];
  }

  for (c2_i7 = 0; c2_i7 < 87311; c2_i7++) {
    chartInstance->c2_labels[c2_i7] = (*chartInstance->c2_b_labels)[c2_i7];
  }

  for (c2_i8 = 0; c2_i8 < 87311; c2_i8++) {
    chartInstance->c2_indiceGrigU2[c2_i8] = (*chartInstance->c2_b_indiceGrigU2)
      [c2_i8];
  }

  c2_b_AnoP = c2_hoistedGlobal;
  c2_b_BnoP = c2_b_hoistedGlobal;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 14U, 15U, c2_debug_family_names,
    c2_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_DYN_IMPORTABLE(chartInstance->c2_ynop_data, (const
    int32_T *)&c2_ynop_sizes, NULL, 0, 0, (void *)c2_h_sf_marshallOut, (void *)
    c2_f_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(chartInstance->c2_yF, 1U,
    c2_g_sf_marshallOut, c2_e_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_indice, 2U, c2_e_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_i, 3U, c2_e_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_kk, 4U, c2_b_sf_marshallOut,
    c2_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_j, 5U, c2_e_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(chartInstance->c2_b_u, MAX_uint32_T,
    c2_f_sf_marshallOut, c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargin, 7U, c2_e_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargout, 8U, c2_e_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(chartInstance->c2_u, 6U, c2_d_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(chartInstance->c2_labels, 9U, c2_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(chartInstance->c2_indiceGrigU2, 10U,
    c2_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_b_AnoP, 11U, c2_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_b_BnoP, 12U, c2_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_b_yP, 13U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 3);
  c2_i9 = 0;
  for (c2_i10 = 0; c2_i10 < 64; c2_i10++) {
    c2_i11 = 0;
    for (c2_i12 = 0; c2_i12 < 262144; c2_i12++) {
      chartInstance->c2_b_u[c2_i12 + c2_i9] = chartInstance->c2_u[c2_i11 +
        c2_i10];
      c2_i11 += 64;
    }

    c2_i9 += 262144;
  }

  _SFD_SYMBOL_SWITCH(6U, 6U);
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 4);
  c2_b0 = (1.0F > c2_b_AnoP);
  c2_b1 = c2_b0;
  c2_b2 = c2_b1;
  if (c2_b2) {
    c2_i13 = 0;
  } else {
    c2_i13 = (int32_T)(real32_T)_SFD_EML_ARRAY_BOUNDS_CHECK("u", (int32_T)
      (real32_T)_SFD_INTEGER_CHECK("AnoP", (real_T)c2_b_AnoP), 1, 262144, 0, 0);
  }

  c2_b3 = (1.0F > c2_b_BnoP);
  c2_b4 = c2_b3;
  c2_b5 = c2_b4;
  if (c2_b5) {
    c2_i14 = 0;
  } else {
    c2_i14 = (int32_T)(real32_T)_SFD_EML_ARRAY_BOUNDS_CHECK("u", (int32_T)
      (real32_T)_SFD_INTEGER_CHECK("BnoP", (real_T)c2_b_BnoP), 1, 64, 0, 0);
  }

  c2_ynop_sizes[0] = c2_i13;
  c2_ynop_sizes[1] = c2_i14;
  c2_loop_ub = c2_i14 - 1;
  for (c2_i15 = 0; c2_i15 <= c2_loop_ub; c2_i15++) {
    c2_b_loop_ub = c2_i13 - 1;
    for (c2_i16 = 0; c2_i16 <= c2_b_loop_ub; c2_i16++) {
      chartInstance->c2_ynop_data[c2_i16 + c2_ynop_sizes[0] * c2_i15] =
        chartInstance->c2_b_u[c2_i16 + (c2_i15 << 18)];
    }
  }

  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 5);
  for (c2_i17 = 0; c2_i17 < 5587904; c2_i17++) {
    chartInstance->c2_yF[c2_i17] = 0.0;
  }

  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 6);
  c2_indice = 1.0;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 8);
  c2_i = 1.0;
  c2_b_i = 0;
  while (c2_b_i < 87311) {
    c2_i = 1.0 + (real_T)c2_b_i;
    CV_EML_FOR(0, 1, 0, 1);
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 9);
    c2_c_BnoP = c2_b_BnoP;
    c2_i18 = (int32_T)c2_c_BnoP;
    _SFD_FOR_LOOP_VECTOR_CHECK(1.0, 1.0, (real_T)c2_c_BnoP, mxSINGLE_CLASS,
      c2_i18);
    c2_kk = 1.0F;
    c2_b_kk = 0;
    while (c2_b_kk <= c2_i18 - 1) {
      c2_kk = 1.0F + (real32_T)c2_b_kk;
      CV_EML_FOR(0, 1, 1, 1);
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 10);
      c2_ynop[0] = chartInstance->c2_ynop_data[(_SFD_EML_ARRAY_BOUNDS_CHECK(
        "ynop", (int32_T)c2_indice, 1, c2_ynop_sizes[0], 1, 0) + c2_ynop_sizes[0]
        * ((int32_T)(real32_T)_SFD_EML_ARRAY_BOUNDS_CHECK("ynop", (int32_T)c2_kk,
        1, c2_ynop_sizes[1], 2, 0) - 1)) - 1];
      c2_ynop[1] = chartInstance->c2_ynop_data[(_SFD_EML_ARRAY_BOUNDS_CHECK(
        "ynop", (int32_T)(c2_indice + 1.0), 1, c2_ynop_sizes[0], 1, 0) +
        c2_ynop_sizes[0] * ((int32_T)(real32_T)_SFD_EML_ARRAY_BOUNDS_CHECK(
        "ynop", (int32_T)c2_kk, 1, c2_ynop_sizes[1], 2, 0) - 1)) - 1];
      c2_ynop[2] = chartInstance->c2_ynop_data[(_SFD_EML_ARRAY_BOUNDS_CHECK(
        "ynop", (int32_T)(c2_indice + 2.0), 1, c2_ynop_sizes[0], 1, 0) +
        c2_ynop_sizes[0] * ((int32_T)(real32_T)_SFD_EML_ARRAY_BOUNDS_CHECK(
        "ynop", (int32_T)c2_kk, 1, c2_ynop_sizes[1], 2, 0) - 1)) - 1];
      chartInstance->c2_yF[((int32_T)(real32_T)_SFD_EML_ARRAY_BOUNDS_CHECK("yF",
                             (int32_T)c2_kk, 1, 64, 1, 0) + (((int32_T)c2_i - 1)
        << 6)) - 1] = c2_norm(chartInstance, c2_ynop);
      c2_b_kk++;
      _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
    }

    CV_EML_FOR(0, 1, 1, 0);
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 12);
    c2_indice += 3.0;
    c2_b_i++;
    _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
  }

  CV_EML_FOR(0, 1, 0, 0);
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 15);
  for (c2_i19 = 0; c2_i19 < 6144; c2_i19++) {
    c2_b_yP[c2_i19] = 0.0;
  }

  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 17);
  c2_i = 1.0;
  c2_c_i = 0;
  while (c2_c_i < 64) {
    c2_i = 1.0 + (real_T)c2_c_i;
    CV_EML_FOR(0, 1, 2, 1);
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 18);
    c2_j = 1.0;
    c2_b_j = 0;
    while (c2_b_j < 96) {
      c2_j = 1.0 + (real_T)c2_b_j;
      CV_EML_FOR(0, 1, 3, 1);
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 19);
      for (c2_i20 = 0; c2_i20 < 5587904; c2_i20++) {
        chartInstance->c2_a[c2_i20] = chartInstance->c2_yF[c2_i20];
      }

      for (c2_i21 = 0; c2_i21 < 87311; c2_i21++) {
        c2_b[c2_i21] = ((real_T)chartInstance->c2_labels[c2_i21] == c2_i);
      }

      for (c2_i22 = 0; c2_i22 < 64; c2_i22++) {
        c2_c_y[c2_i22] = 0.0;
        c2_i23 = 0;
        for (c2_i24 = 0; c2_i24 < 87311; c2_i24++) {
          c2_c_y[c2_i22] += chartInstance->c2_a[c2_i23 + c2_i22] * (real_T)
            c2_b[c2_i24];
          c2_i23 += 64;
        }
      }

      c2_c_j = (int32_T)c2_j - 1;
      for (c2_i25 = 0; c2_i25 < 64; c2_i25++) {
        c2_b_yP[c2_i25 + (c2_c_j << 6)] = c2_c_y[c2_i25];
      }

      c2_b_j++;
      _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
    }

    CV_EML_FOR(0, 1, 3, 0);
    c2_c_i++;
    _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
  }

  CV_EML_FOR(0, 1, 2, 0);
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, -19);
  _SFD_SYMBOL_SCOPE_POP();
  for (c2_i26 = 0; c2_i26 < 6144; c2_i26++) {
    (*chartInstance->c2_yP)[c2_i26] = c2_b_yP[c2_i26];
  }

  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 0U, chartInstance->c2_sfEvent);
}

static void initSimStructsc2_CreaminoSimulinkModel
  (SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void init_script_number_translation(uint32_T c2_machineNumber, uint32_T
  c2_chartNumber, uint32_T c2_instanceNumber)
{
  (void)c2_machineNumber;
  (void)c2_chartNumber;
  (void)c2_instanceNumber;
}

static const mxArray *c2_sf_marshallOut(void *chartInstanceVoid, void
  *c2_e_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  int32_T c2_i27;
  int32_T c2_i28;
  int32_T c2_i29;
  real_T c2_f_inData[6144];
  int32_T c2_i30;
  int32_T c2_i31;
  int32_T c2_i32;
  real_T c2_h_u[6144];
  const mxArray *c2_c_y = NULL;
  SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance;
  chartInstance = (SFc2_CreaminoSimulinkModelInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_i27 = 0;
  for (c2_i28 = 0; c2_i28 < 96; c2_i28++) {
    for (c2_i29 = 0; c2_i29 < 64; c2_i29++) {
      c2_f_inData[c2_i29 + c2_i27] = (*(real_T (*)[6144])c2_e_inData)[c2_i29 +
        c2_i27];
    }

    c2_i27 += 64;
  }

  c2_i30 = 0;
  for (c2_i31 = 0; c2_i31 < 96; c2_i31++) {
    for (c2_i32 = 0; c2_i32 < 64; c2_i32++) {
      c2_h_u[c2_i32 + c2_i30] = c2_f_inData[c2_i32 + c2_i30];
    }

    c2_i30 += 64;
  }

  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", c2_h_u, 0, 0U, 1U, 0U, 2, 64, 96),
                false);
  sf_mex_assign(&c2_mxArrayOutData, c2_c_y, false);
  return c2_mxArrayOutData;
}

static void c2_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_b_yP, const char_T *c2_identifier, real_T
  c2_c_y[6144])
{
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_yP), &c2_thisId, c2_c_y);
  sf_mex_destroy(&c2_b_yP);
}

static void c2_b_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_h_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_c_y[6144])
{
  real_T c2_dv1[6144];
  int32_T c2_i33;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_h_u), c2_dv1, 1, 0, 0U, 1, 0U, 2, 64,
                96);
  for (c2_i33 = 0; c2_i33 < 6144; c2_i33++) {
    c2_c_y[c2_i33] = c2_dv1[c2_i33];
  }

  sf_mex_destroy(&c2_h_u);
}

static void c2_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_yP;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_c_y[6144];
  int32_T c2_i34;
  int32_T c2_i35;
  int32_T c2_i36;
  SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance;
  chartInstance = (SFc2_CreaminoSimulinkModelInstanceStruct *)chartInstanceVoid;
  c2_b_yP = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_yP), &c2_thisId, c2_c_y);
  sf_mex_destroy(&c2_b_yP);
  c2_i34 = 0;
  for (c2_i35 = 0; c2_i35 < 96; c2_i35++) {
    for (c2_i36 = 0; c2_i36 < 64; c2_i36++) {
      (*(real_T (*)[6144])c2_outData)[c2_i36 + c2_i34] = c2_c_y[c2_i36 + c2_i34];
    }

    c2_i34 += 64;
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_b_sf_marshallOut(void *chartInstanceVoid, void
  *c2_e_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  real32_T c2_h_u;
  const mxArray *c2_c_y = NULL;
  SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance;
  chartInstance = (SFc2_CreaminoSimulinkModelInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_h_u = *(real32_T *)c2_e_inData;
  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", &c2_h_u, 1, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_c_y, false);
  return c2_mxArrayOutData;
}

static const mxArray *c2_c_sf_marshallOut(void *chartInstanceVoid, void
  *c2_e_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  int32_T c2_i37;
  int32_T c2_i38;
  const mxArray *c2_c_y = NULL;
  SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance;
  chartInstance = (SFc2_CreaminoSimulinkModelInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  for (c2_i37 = 0; c2_i37 < 87311; c2_i37++) {
    chartInstance->c2_d_inData[c2_i37] = (*(real32_T (*)[87311])c2_e_inData)
      [c2_i37];
  }

  for (c2_i38 = 0; c2_i38 < 87311; c2_i38++) {
    chartInstance->c2_f_u[c2_i38] = chartInstance->c2_d_inData[c2_i38];
  }

  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", chartInstance->c2_f_u, 1, 0U, 1U, 0U,
    2, 1, 87311), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_c_y, false);
  return c2_mxArrayOutData;
}

static const mxArray *c2_d_sf_marshallOut(void *chartInstanceVoid, void
  *c2_e_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  int32_T c2_i39;
  int32_T c2_i40;
  int32_T c2_i41;
  int32_T c2_i42;
  int32_T c2_i43;
  int32_T c2_i44;
  const mxArray *c2_c_y = NULL;
  SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance;
  chartInstance = (SFc2_CreaminoSimulinkModelInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_i39 = 0;
  for (c2_i40 = 0; c2_i40 < 262144; c2_i40++) {
    for (c2_i41 = 0; c2_i41 < 64; c2_i41++) {
      chartInstance->c2_b_inData[c2_i41 + c2_i39] = (*(real32_T (*)[16777216])
        c2_e_inData)[c2_i41 + c2_i39];
    }

    c2_i39 += 64;
  }

  c2_i42 = 0;
  for (c2_i43 = 0; c2_i43 < 262144; c2_i43++) {
    for (c2_i44 = 0; c2_i44 < 64; c2_i44++) {
      chartInstance->c2_d_u[c2_i44 + c2_i42] = chartInstance->c2_b_inData[c2_i44
        + c2_i42];
    }

    c2_i42 += 64;
  }

  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", chartInstance->c2_d_u, 1, 0U, 1U, 0U,
    2, 64, 262144), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_c_y, false);
  return c2_mxArrayOutData;
}

static const mxArray *c2_e_sf_marshallOut(void *chartInstanceVoid, void
  *c2_e_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  real_T c2_h_u;
  const mxArray *c2_c_y = NULL;
  SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance;
  chartInstance = (SFc2_CreaminoSimulinkModelInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_h_u = *(real_T *)c2_e_inData;
  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", &c2_h_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_c_y, false);
  return c2_mxArrayOutData;
}

static real_T c2_c_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_h_u, const emlrtMsgIdentifier *c2_parentId)
{
  real_T c2_c_y;
  real_T c2_d0;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_h_u), &c2_d0, 1, 0, 0U, 0, 0U, 0);
  c2_c_y = c2_d0;
  sf_mex_destroy(&c2_h_u);
  return c2_c_y;
}

static void c2_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_nargout;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_c_y;
  SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance;
  chartInstance = (SFc2_CreaminoSimulinkModelInstanceStruct *)chartInstanceVoid;
  c2_nargout = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_c_y = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_nargout),
    &c2_thisId);
  sf_mex_destroy(&c2_nargout);
  *(real_T *)c2_outData = c2_c_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_f_sf_marshallOut(void *chartInstanceVoid, void
  *c2_e_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  int32_T c2_i45;
  int32_T c2_i46;
  int32_T c2_i47;
  int32_T c2_i48;
  int32_T c2_i49;
  int32_T c2_i50;
  const mxArray *c2_c_y = NULL;
  SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance;
  chartInstance = (SFc2_CreaminoSimulinkModelInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_i45 = 0;
  for (c2_i46 = 0; c2_i46 < 64; c2_i46++) {
    for (c2_i47 = 0; c2_i47 < 262144; c2_i47++) {
      chartInstance->c2_inData[c2_i47 + c2_i45] = (*(real32_T (*)[16777216])
        c2_e_inData)[c2_i47 + c2_i45];
    }

    c2_i45 += 262144;
  }

  c2_i48 = 0;
  for (c2_i49 = 0; c2_i49 < 64; c2_i49++) {
    for (c2_i50 = 0; c2_i50 < 262144; c2_i50++) {
      chartInstance->c2_c_u[c2_i50 + c2_i48] = chartInstance->c2_inData[c2_i50 +
        c2_i48];
    }

    c2_i48 += 262144;
  }

  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", chartInstance->c2_c_u, 1, 0U, 1U, 0U,
    2, 262144, 64), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_c_y, false);
  return c2_mxArrayOutData;
}

static void c2_d_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_h_u, const emlrtMsgIdentifier *c2_parentId,
  real32_T c2_c_y[16777216])
{
  int32_T c2_i51;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_h_u), chartInstance->c2_fv0, 1, 1, 0U,
                1, 0U, 2, 262144, 64);
  for (c2_i51 = 0; c2_i51 < 16777216; c2_i51++) {
    c2_c_y[c2_i51] = chartInstance->c2_fv0[c2_i51];
  }

  sf_mex_destroy(&c2_h_u);
}

static void c2_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_h_u;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  int32_T c2_i52;
  int32_T c2_i53;
  int32_T c2_i54;
  SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance;
  chartInstance = (SFc2_CreaminoSimulinkModelInstanceStruct *)chartInstanceVoid;
  c2_h_u = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_h_u), &c2_thisId,
                        chartInstance->c2_y);
  sf_mex_destroy(&c2_h_u);
  c2_i52 = 0;
  for (c2_i53 = 0; c2_i53 < 64; c2_i53++) {
    for (c2_i54 = 0; c2_i54 < 262144; c2_i54++) {
      (*(real32_T (*)[16777216])c2_outData)[c2_i54 + c2_i52] =
        chartInstance->c2_y[c2_i54 + c2_i52];
    }

    c2_i52 += 262144;
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static real32_T c2_e_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_h_u, const emlrtMsgIdentifier *c2_parentId)
{
  real32_T c2_c_y;
  real32_T c2_f0;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_h_u), &c2_f0, 1, 1, 0U, 0, 0U, 0);
  c2_c_y = c2_f0;
  sf_mex_destroy(&c2_h_u);
  return c2_c_y;
}

static void c2_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_kk;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real32_T c2_c_y;
  SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance;
  chartInstance = (SFc2_CreaminoSimulinkModelInstanceStruct *)chartInstanceVoid;
  c2_kk = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_c_y = c2_e_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_kk), &c2_thisId);
  sf_mex_destroy(&c2_kk);
  *(real32_T *)c2_outData = c2_c_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_g_sf_marshallOut(void *chartInstanceVoid, void
  *c2_e_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  int32_T c2_i55;
  int32_T c2_i56;
  int32_T c2_i57;
  int32_T c2_i58;
  int32_T c2_i59;
  int32_T c2_i60;
  const mxArray *c2_c_y = NULL;
  SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance;
  chartInstance = (SFc2_CreaminoSimulinkModelInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_i55 = 0;
  for (c2_i56 = 0; c2_i56 < 87311; c2_i56++) {
    for (c2_i57 = 0; c2_i57 < 64; c2_i57++) {
      chartInstance->c2_c_inData[c2_i57 + c2_i55] = (*(real_T (*)[5587904])
        c2_e_inData)[c2_i57 + c2_i55];
    }

    c2_i55 += 64;
  }

  c2_i58 = 0;
  for (c2_i59 = 0; c2_i59 < 87311; c2_i59++) {
    for (c2_i60 = 0; c2_i60 < 64; c2_i60++) {
      chartInstance->c2_e_u[c2_i60 + c2_i58] = chartInstance->c2_c_inData[c2_i60
        + c2_i58];
    }

    c2_i58 += 64;
  }

  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", chartInstance->c2_e_u, 0, 0U, 1U, 0U,
    2, 64, 87311), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_c_y, false);
  return c2_mxArrayOutData;
}

static void c2_f_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_h_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_c_y[5587904])
{
  int32_T c2_i61;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_h_u), chartInstance->c2_dv2, 1, 0, 0U,
                1, 0U, 2, 64, 87311);
  for (c2_i61 = 0; c2_i61 < 5587904; c2_i61++) {
    c2_c_y[c2_i61] = chartInstance->c2_dv2[c2_i61];
  }

  sf_mex_destroy(&c2_h_u);
}

static void c2_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_yF;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  int32_T c2_i62;
  int32_T c2_i63;
  int32_T c2_i64;
  SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance;
  chartInstance = (SFc2_CreaminoSimulinkModelInstanceStruct *)chartInstanceVoid;
  c2_b_yF = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_f_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_yF), &c2_thisId,
                        chartInstance->c2_b_y);
  sf_mex_destroy(&c2_b_yF);
  c2_i62 = 0;
  for (c2_i63 = 0; c2_i63 < 87311; c2_i63++) {
    for (c2_i64 = 0; c2_i64 < 64; c2_i64++) {
      (*(real_T (*)[5587904])c2_outData)[c2_i64 + c2_i62] =
        chartInstance->c2_b_y[c2_i64 + c2_i62];
    }

    c2_i62 += 64;
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_h_sf_marshallOut(void *chartInstanceVoid, real32_T
  c2_b_inData_data[], int32_T c2_inData_sizes[2])
{
  const mxArray *c2_mxArrayOutData = NULL;
  int32_T c2_u_sizes[2];
  int32_T c2_h_u;
  int32_T c2_i_u;
  int32_T c2_e_inData;
  int32_T c2_f_inData;
  int32_T c2_b_inData_sizes;
  int32_T c2_loop_ub;
  int32_T c2_i65;
  int32_T c2_b_loop_ub;
  int32_T c2_i66;
  const mxArray *c2_c_y = NULL;
  SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance;
  chartInstance = (SFc2_CreaminoSimulinkModelInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_u_sizes[0] = c2_inData_sizes[0];
  c2_u_sizes[1] = c2_inData_sizes[1];
  c2_h_u = c2_u_sizes[0];
  c2_i_u = c2_u_sizes[1];
  c2_e_inData = c2_inData_sizes[0];
  c2_f_inData = c2_inData_sizes[1];
  c2_b_inData_sizes = c2_e_inData * c2_f_inData;
  c2_loop_ub = c2_e_inData * c2_f_inData - 1;
  for (c2_i65 = 0; c2_i65 <= c2_loop_ub; c2_i65++) {
    chartInstance->c2_inData_data[c2_i65] = c2_b_inData_data[c2_i65];
  }

  c2_b_loop_ub = c2_b_inData_sizes - 1;
  for (c2_i66 = 0; c2_i66 <= c2_b_loop_ub; c2_i66++) {
    chartInstance->c2_u_data[c2_i66] = chartInstance->c2_inData_data[c2_i66];
  }

  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", chartInstance->c2_u_data, 1, 0U, 1U,
    0U, 2, c2_u_sizes[0], c2_u_sizes[1]), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_c_y, false);
  return c2_mxArrayOutData;
}

static void c2_g_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_h_u, const emlrtMsgIdentifier *c2_parentId,
  real32_T c2_b_y_data[], int32_T c2_y_sizes[2])
{
  int32_T c2_i67;
  uint32_T c2_uv0[2];
  int32_T c2_i68;
  boolean_T c2_bv0[2];
  int32_T c2_tmp_sizes[2];
  int32_T c2_c_y;
  int32_T c2_d_y;
  int32_T c2_loop_ub;
  int32_T c2_i69;
  for (c2_i67 = 0; c2_i67 < 2; c2_i67++) {
    c2_uv0[c2_i67] = 262144U + (uint32_T)(-262080 * c2_i67);
  }

  for (c2_i68 = 0; c2_i68 < 2; c2_i68++) {
    c2_bv0[c2_i68] = true;
  }

  sf_mex_import_vs(c2_parentId, sf_mex_dup(c2_h_u), chartInstance->c2_tmp_data,
                   1, 1, 0U, 1, 0U, 2, c2_bv0, c2_uv0, c2_tmp_sizes);
  c2_y_sizes[0] = c2_tmp_sizes[0];
  c2_y_sizes[1] = c2_tmp_sizes[1];
  c2_c_y = c2_y_sizes[0];
  c2_d_y = c2_y_sizes[1];
  c2_loop_ub = c2_tmp_sizes[0] * c2_tmp_sizes[1] - 1;
  for (c2_i69 = 0; c2_i69 <= c2_loop_ub; c2_i69++) {
    c2_b_y_data[c2_i69] = chartInstance->c2_tmp_data[c2_i69];
  }

  sf_mex_destroy(&c2_h_u);
}

static void c2_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, real32_T c2_outData_data[],
  int32_T c2_outData_sizes[2])
{
  const mxArray *c2_ynop;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  int32_T c2_y_sizes[2];
  int32_T c2_loop_ub;
  int32_T c2_i70;
  int32_T c2_b_loop_ub;
  int32_T c2_i71;
  SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance;
  chartInstance = (SFc2_CreaminoSimulinkModelInstanceStruct *)chartInstanceVoid;
  c2_ynop = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_g_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_ynop), &c2_thisId,
                        chartInstance->c2_y_data, c2_y_sizes);
  sf_mex_destroy(&c2_ynop);
  c2_outData_sizes[0] = c2_y_sizes[0];
  c2_outData_sizes[1] = c2_y_sizes[1];
  c2_loop_ub = c2_y_sizes[1] - 1;
  for (c2_i70 = 0; c2_i70 <= c2_loop_ub; c2_i70++) {
    c2_b_loop_ub = c2_y_sizes[0] - 1;
    for (c2_i71 = 0; c2_i71 <= c2_b_loop_ub; c2_i71++) {
      c2_outData_data[c2_i71 + c2_outData_sizes[0] * c2_i70] =
        chartInstance->c2_y_data[c2_i71 + c2_y_sizes[0] * c2_i70];
    }
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

const mxArray *sf_c2_CreaminoSimulinkModel_get_eml_resolved_functions_info(void)
{
  const mxArray *c2_nameCaptureInfo = NULL;
  c2_nameCaptureInfo = NULL;
  sf_mex_assign(&c2_nameCaptureInfo, sf_mex_createstruct("structure", 2, 30, 1),
                false);
  c2_info_helper(&c2_nameCaptureInfo);
  sf_mex_emlrtNameCapturePostProcessR2012a(&c2_nameCaptureInfo);
  return c2_nameCaptureInfo;
}

static void c2_info_helper(const mxArray **c2_info)
{
  const mxArray *c2_rhs0 = NULL;
  const mxArray *c2_lhs0 = NULL;
  const mxArray *c2_rhs1 = NULL;
  const mxArray *c2_lhs1 = NULL;
  const mxArray *c2_rhs2 = NULL;
  const mxArray *c2_lhs2 = NULL;
  const mxArray *c2_rhs3 = NULL;
  const mxArray *c2_lhs3 = NULL;
  const mxArray *c2_rhs4 = NULL;
  const mxArray *c2_lhs4 = NULL;
  const mxArray *c2_rhs5 = NULL;
  const mxArray *c2_lhs5 = NULL;
  const mxArray *c2_rhs6 = NULL;
  const mxArray *c2_lhs6 = NULL;
  const mxArray *c2_rhs7 = NULL;
  const mxArray *c2_lhs7 = NULL;
  const mxArray *c2_rhs8 = NULL;
  const mxArray *c2_lhs8 = NULL;
  const mxArray *c2_rhs9 = NULL;
  const mxArray *c2_lhs9 = NULL;
  const mxArray *c2_rhs10 = NULL;
  const mxArray *c2_lhs10 = NULL;
  const mxArray *c2_rhs11 = NULL;
  const mxArray *c2_lhs11 = NULL;
  const mxArray *c2_rhs12 = NULL;
  const mxArray *c2_lhs12 = NULL;
  const mxArray *c2_rhs13 = NULL;
  const mxArray *c2_lhs13 = NULL;
  const mxArray *c2_rhs14 = NULL;
  const mxArray *c2_lhs14 = NULL;
  const mxArray *c2_rhs15 = NULL;
  const mxArray *c2_lhs15 = NULL;
  const mxArray *c2_rhs16 = NULL;
  const mxArray *c2_lhs16 = NULL;
  const mxArray *c2_rhs17 = NULL;
  const mxArray *c2_lhs17 = NULL;
  const mxArray *c2_rhs18 = NULL;
  const mxArray *c2_lhs18 = NULL;
  const mxArray *c2_rhs19 = NULL;
  const mxArray *c2_lhs19 = NULL;
  const mxArray *c2_rhs20 = NULL;
  const mxArray *c2_lhs20 = NULL;
  const mxArray *c2_rhs21 = NULL;
  const mxArray *c2_lhs21 = NULL;
  const mxArray *c2_rhs22 = NULL;
  const mxArray *c2_lhs22 = NULL;
  const mxArray *c2_rhs23 = NULL;
  const mxArray *c2_lhs23 = NULL;
  const mxArray *c2_rhs24 = NULL;
  const mxArray *c2_lhs24 = NULL;
  const mxArray *c2_rhs25 = NULL;
  const mxArray *c2_lhs25 = NULL;
  const mxArray *c2_rhs26 = NULL;
  const mxArray *c2_lhs26 = NULL;
  const mxArray *c2_rhs27 = NULL;
  const mxArray *c2_lhs27 = NULL;
  const mxArray *c2_rhs28 = NULL;
  const mxArray *c2_lhs28 = NULL;
  const mxArray *c2_rhs29 = NULL;
  const mxArray *c2_lhs29 = NULL;
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(""), "context", "context", 0);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("length"), "name", "name", 0);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("single"), "dominantType",
                  "dominantType", 0);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/length.m"), "resolved",
                  "resolved", 0);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1303146206U), "fileTimeLo",
                  "fileTimeLo", 0);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 0);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 0);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 0);
  sf_mex_assign(&c2_rhs0, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs0, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs0), "rhs", "rhs", 0);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs0), "lhs", "lhs", 0);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(""), "context", "context", 1);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("norm"), "name", "name", 1);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("single"), "dominantType",
                  "dominantType", 1);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/norm.m"), "resolved",
                  "resolved", 1);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1363713868U), "fileTimeLo",
                  "fileTimeLo", 1);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 1);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 1);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 1);
  sf_mex_assign(&c2_rhs1, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs1, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs1), "rhs", "rhs", 1);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs1), "lhs", "lhs", 1);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/norm.m!genpnorm"),
                  "context", "context", 2);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_index_class"), "name",
                  "name", 2);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 2);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m"),
                  "resolved", "resolved", 2);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1323170578U), "fileTimeLo",
                  "fileTimeLo", 2);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 2);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 2);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 2);
  sf_mex_assign(&c2_rhs2, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs2, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs2), "rhs", "rhs", 2);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs2), "lhs", "lhs", 2);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/norm.m!genpnorm"),
                  "context", "context", 3);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 3);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 3);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 3);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1395931856U), "fileTimeLo",
                  "fileTimeLo", 3);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 3);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 3);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 3);
  sf_mex_assign(&c2_rhs3, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs3, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs3), "rhs", "rhs", 3);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs3), "lhs", "lhs", 3);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/norm.m!genpnorm"),
                  "context", "context", 4);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_xnrm2"), "name", "name", 4);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("single"), "dominantType",
                  "dominantType", 4);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xnrm2.m"),
                  "resolved", "resolved", 4);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1375980692U), "fileTimeLo",
                  "fileTimeLo", 4);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 4);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 4);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 4);
  sf_mex_assign(&c2_rhs4, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs4, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs4), "rhs", "rhs", 4);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs4), "lhs", "lhs", 4);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xnrm2.m"), "context",
                  "context", 5);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("coder.internal.blas.inline"),
                  "name", "name", 5);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 5);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/inline.p"),
                  "resolved", "resolved", 5);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1410807772U), "fileTimeLo",
                  "fileTimeLo", 5);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 5);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 5);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 5);
  sf_mex_assign(&c2_rhs5, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs5, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs5), "rhs", "rhs", 5);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs5), "lhs", "lhs", 5);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xnrm2.m"), "context",
                  "context", 6);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("coder.internal.blas.xnrm2"),
                  "name", "name", 6);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("single"), "dominantType",
                  "dominantType", 6);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/xnrm2.p"),
                  "resolved", "resolved", 6);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1410807770U), "fileTimeLo",
                  "fileTimeLo", 6);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 6);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 6);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 6);
  sf_mex_assign(&c2_rhs6, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs6, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs6), "rhs", "rhs", 6);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs6), "lhs", "lhs", 6);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/xnrm2.p"),
                  "context", "context", 7);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "coder.internal.blas.use_refblas"), "name", "name", 7);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 7);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/use_refblas.p"),
                  "resolved", "resolved", 7);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1410807770U), "fileTimeLo",
                  "fileTimeLo", 7);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 7);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 7);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 7);
  sf_mex_assign(&c2_rhs7, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs7, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs7), "rhs", "rhs", 7);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs7), "lhs", "lhs", 7);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/xnrm2.p!below_threshold"),
                  "context", "context", 8);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("coder.internal.blas.threshold"),
                  "name", "name", 8);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 8);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/threshold.p"),
                  "resolved", "resolved", 8);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1410807772U), "fileTimeLo",
                  "fileTimeLo", 8);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 8);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 8);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 8);
  sf_mex_assign(&c2_rhs8, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs8, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs8), "rhs", "rhs", 8);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs8), "lhs", "lhs", 8);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/threshold.p"),
                  "context", "context", 9);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_switch_helper"), "name",
                  "name", 9);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 9);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_switch_helper.m"),
                  "resolved", "resolved", 9);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1393330858U), "fileTimeLo",
                  "fileTimeLo", 9);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 9);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 9);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 9);
  sf_mex_assign(&c2_rhs9, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs9, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs9), "rhs", "rhs", 9);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs9), "lhs", "lhs", 9);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/xnrm2.p"),
                  "context", "context", 10);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("coder.internal.refblas.xnrm2"),
                  "name", "name", 10);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("single"), "dominantType",
                  "dominantType", 10);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+refblas/xnrm2.p"),
                  "resolved", "resolved", 10);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1410807772U), "fileTimeLo",
                  "fileTimeLo", 10);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 10);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 10);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 10);
  sf_mex_assign(&c2_rhs10, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs10, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs10), "rhs", "rhs",
                  10);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs10), "lhs", "lhs",
                  10);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+refblas/xnrm2.p"),
                  "context", "context", 11);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("realmin"), "name", "name", 11);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 11);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/realmin.m"), "resolved",
                  "resolved", 11);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1307651242U), "fileTimeLo",
                  "fileTimeLo", 11);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 11);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 11);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 11);
  sf_mex_assign(&c2_rhs11, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs11, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs11), "rhs", "rhs",
                  11);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs11), "lhs", "lhs",
                  11);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/realmin.m"), "context",
                  "context", 12);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_realmin"), "name", "name",
                  12);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 12);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_realmin.m"), "resolved",
                  "resolved", 12);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1307651244U), "fileTimeLo",
                  "fileTimeLo", 12);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 12);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 12);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 12);
  sf_mex_assign(&c2_rhs12, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs12, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs12), "rhs", "rhs",
                  12);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs12), "lhs", "lhs",
                  12);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_realmin.m"), "context",
                  "context", 13);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_float_model"), "name",
                  "name", 13);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 13);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_float_model.m"),
                  "resolved", "resolved", 13);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1326727996U), "fileTimeLo",
                  "fileTimeLo", 13);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 13);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 13);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 13);
  sf_mex_assign(&c2_rhs13, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs13, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs13), "rhs", "rhs",
                  13);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs13), "lhs", "lhs",
                  13);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+refblas/xnrm2.p"),
                  "context", "context", 14);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("coder.internal.indexMinus"),
                  "name", "name", 14);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 14);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/indexMinus.m"),
                  "resolved", "resolved", 14);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1372583160U), "fileTimeLo",
                  "fileTimeLo", 14);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 14);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 14);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 14);
  sf_mex_assign(&c2_rhs14, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs14, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs14), "rhs", "rhs",
                  14);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs14), "lhs", "lhs",
                  14);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+refblas/xnrm2.p"),
                  "context", "context", 15);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("coder.internal.indexTimes"),
                  "name", "name", 15);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("coder.internal.indexInt"),
                  "dominantType", "dominantType", 15);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/indexTimes.m"),
                  "resolved", "resolved", 15);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1372583160U), "fileTimeLo",
                  "fileTimeLo", 15);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 15);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 15);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 15);
  sf_mex_assign(&c2_rhs15, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs15, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs15), "rhs", "rhs",
                  15);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs15), "lhs", "lhs",
                  15);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+refblas/xnrm2.p"),
                  "context", "context", 16);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("coder.internal.indexPlus"),
                  "name", "name", 16);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("coder.internal.indexInt"),
                  "dominantType", "dominantType", 16);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/indexPlus.m"),
                  "resolved", "resolved", 16);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1372583160U), "fileTimeLo",
                  "fileTimeLo", 16);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 16);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 16);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 16);
  sf_mex_assign(&c2_rhs16, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs16, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs16), "rhs", "rhs",
                  16);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs16), "lhs", "lhs",
                  16);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+refblas/xnrm2.p"),
                  "context", "context", 17);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "eml_int_forloop_overflow_check"), "name", "name", 17);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 17);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m"),
                  "resolved", "resolved", 17);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1397257422U), "fileTimeLo",
                  "fileTimeLo", 17);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 17);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 17);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 17);
  sf_mex_assign(&c2_rhs17, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs17, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs17), "rhs", "rhs",
                  17);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs17), "lhs", "lhs",
                  17);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m!eml_int_forloop_overflow_check_helper"),
                  "context", "context", 18);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("isfi"), "name", "name", 18);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("coder.internal.indexInt"),
                  "dominantType", "dominantType", 18);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/fixedpoint/isfi.m"), "resolved",
                  "resolved", 18);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1346510358U), "fileTimeLo",
                  "fileTimeLo", 18);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 18);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 18);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 18);
  sf_mex_assign(&c2_rhs18, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs18, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs18), "rhs", "rhs",
                  18);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs18), "lhs", "lhs",
                  18);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/fixedpoint/isfi.m"), "context",
                  "context", 19);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("isnumerictype"), "name",
                  "name", 19);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 19);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/fixedpoint/isnumerictype.m"), "resolved",
                  "resolved", 19);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1398875598U), "fileTimeLo",
                  "fileTimeLo", 19);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 19);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 19);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 19);
  sf_mex_assign(&c2_rhs19, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs19, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs19), "rhs", "rhs",
                  19);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs19), "lhs", "lhs",
                  19);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m!eml_int_forloop_overflow_check_helper"),
                  "context", "context", 20);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("intmax"), "name", "name", 20);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 20);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/intmax.m"), "resolved",
                  "resolved", 20);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1362261882U), "fileTimeLo",
                  "fileTimeLo", 20);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 20);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 20);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 20);
  sf_mex_assign(&c2_rhs20, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs20, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs20), "rhs", "rhs",
                  20);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs20), "lhs", "lhs",
                  20);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/intmax.m"), "context",
                  "context", 21);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_switch_helper"), "name",
                  "name", 21);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 21);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_switch_helper.m"),
                  "resolved", "resolved", 21);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1393330858U), "fileTimeLo",
                  "fileTimeLo", 21);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 21);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 21);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 21);
  sf_mex_assign(&c2_rhs21, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs21, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs21), "rhs", "rhs",
                  21);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs21), "lhs", "lhs",
                  21);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m!eml_int_forloop_overflow_check_helper"),
                  "context", "context", 22);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("intmin"), "name", "name", 22);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 22);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/intmin.m"), "resolved",
                  "resolved", 22);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1362261882U), "fileTimeLo",
                  "fileTimeLo", 22);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 22);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 22);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 22);
  sf_mex_assign(&c2_rhs22, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs22, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs22), "rhs", "rhs",
                  22);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs22), "lhs", "lhs",
                  22);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/intmin.m"), "context",
                  "context", 23);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_switch_helper"), "name",
                  "name", 23);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 23);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_switch_helper.m"),
                  "resolved", "resolved", 23);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1393330858U), "fileTimeLo",
                  "fileTimeLo", 23);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 23);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 23);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 23);
  sf_mex_assign(&c2_rhs23, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs23, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs23), "rhs", "rhs",
                  23);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs23), "lhs", "lhs",
                  23);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+refblas/xnrm2.p"),
                  "context", "context", 24);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("abs"), "name", "name", 24);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("single"), "dominantType",
                  "dominantType", 24);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m"), "resolved",
                  "resolved", 24);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1363713852U), "fileTimeLo",
                  "fileTimeLo", 24);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 24);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 24);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 24);
  sf_mex_assign(&c2_rhs24, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs24, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs24), "rhs", "rhs",
                  24);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs24), "lhs", "lhs",
                  24);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m"), "context",
                  "context", 25);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 25);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("single"), "dominantType",
                  "dominantType", 25);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 25);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1395931856U), "fileTimeLo",
                  "fileTimeLo", 25);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 25);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 25);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 25);
  sf_mex_assign(&c2_rhs25, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs25, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs25), "rhs", "rhs",
                  25);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs25), "lhs", "lhs",
                  25);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m"), "context",
                  "context", 26);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_scalar_abs"), "name",
                  "name", 26);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("single"), "dominantType",
                  "dominantType", 26);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_abs.m"),
                  "resolved", "resolved", 26);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1286818712U), "fileTimeLo",
                  "fileTimeLo", 26);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 26);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 26);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 26);
  sf_mex_assign(&c2_rhs26, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs26, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs26), "rhs", "rhs",
                  26);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs26), "lhs", "lhs",
                  26);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(""), "context", "context", 27);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_mtimes_helper"), "name",
                  "name", 27);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 27);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/eml_mtimes_helper.m"),
                  "resolved", "resolved", 27);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1383877294U), "fileTimeLo",
                  "fileTimeLo", 27);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 27);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 27);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 27);
  sf_mex_assign(&c2_rhs27, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs27, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs27), "rhs", "rhs",
                  27);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs27), "lhs", "lhs",
                  27);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/eml_mtimes_helper.m!common_checks"),
                  "context", "context", 28);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 28);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 28);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 28);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1395931856U), "fileTimeLo",
                  "fileTimeLo", 28);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 28);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 28);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 28);
  sf_mex_assign(&c2_rhs28, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs28, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs28), "rhs", "rhs",
                  28);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs28), "lhs", "lhs",
                  28);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/eml_mtimes_helper.m!common_checks"),
                  "context", "context", 29);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 29);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("logical"), "dominantType",
                  "dominantType", 29);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 29);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1395931856U), "fileTimeLo",
                  "fileTimeLo", 29);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 29);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 29);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 29);
  sf_mex_assign(&c2_rhs29, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs29, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs29), "rhs", "rhs",
                  29);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs29), "lhs", "lhs",
                  29);
  sf_mex_destroy(&c2_rhs0);
  sf_mex_destroy(&c2_lhs0);
  sf_mex_destroy(&c2_rhs1);
  sf_mex_destroy(&c2_lhs1);
  sf_mex_destroy(&c2_rhs2);
  sf_mex_destroy(&c2_lhs2);
  sf_mex_destroy(&c2_rhs3);
  sf_mex_destroy(&c2_lhs3);
  sf_mex_destroy(&c2_rhs4);
  sf_mex_destroy(&c2_lhs4);
  sf_mex_destroy(&c2_rhs5);
  sf_mex_destroy(&c2_lhs5);
  sf_mex_destroy(&c2_rhs6);
  sf_mex_destroy(&c2_lhs6);
  sf_mex_destroy(&c2_rhs7);
  sf_mex_destroy(&c2_lhs7);
  sf_mex_destroy(&c2_rhs8);
  sf_mex_destroy(&c2_lhs8);
  sf_mex_destroy(&c2_rhs9);
  sf_mex_destroy(&c2_lhs9);
  sf_mex_destroy(&c2_rhs10);
  sf_mex_destroy(&c2_lhs10);
  sf_mex_destroy(&c2_rhs11);
  sf_mex_destroy(&c2_lhs11);
  sf_mex_destroy(&c2_rhs12);
  sf_mex_destroy(&c2_lhs12);
  sf_mex_destroy(&c2_rhs13);
  sf_mex_destroy(&c2_lhs13);
  sf_mex_destroy(&c2_rhs14);
  sf_mex_destroy(&c2_lhs14);
  sf_mex_destroy(&c2_rhs15);
  sf_mex_destroy(&c2_lhs15);
  sf_mex_destroy(&c2_rhs16);
  sf_mex_destroy(&c2_lhs16);
  sf_mex_destroy(&c2_rhs17);
  sf_mex_destroy(&c2_lhs17);
  sf_mex_destroy(&c2_rhs18);
  sf_mex_destroy(&c2_lhs18);
  sf_mex_destroy(&c2_rhs19);
  sf_mex_destroy(&c2_lhs19);
  sf_mex_destroy(&c2_rhs20);
  sf_mex_destroy(&c2_lhs20);
  sf_mex_destroy(&c2_rhs21);
  sf_mex_destroy(&c2_lhs21);
  sf_mex_destroy(&c2_rhs22);
  sf_mex_destroy(&c2_lhs22);
  sf_mex_destroy(&c2_rhs23);
  sf_mex_destroy(&c2_lhs23);
  sf_mex_destroy(&c2_rhs24);
  sf_mex_destroy(&c2_lhs24);
  sf_mex_destroy(&c2_rhs25);
  sf_mex_destroy(&c2_lhs25);
  sf_mex_destroy(&c2_rhs26);
  sf_mex_destroy(&c2_lhs26);
  sf_mex_destroy(&c2_rhs27);
  sf_mex_destroy(&c2_lhs27);
  sf_mex_destroy(&c2_rhs28);
  sf_mex_destroy(&c2_lhs28);
  sf_mex_destroy(&c2_rhs29);
  sf_mex_destroy(&c2_lhs29);
}

static const mxArray *c2_emlrt_marshallOut(const char * c2_h_u)
{
  const mxArray *c2_c_y = NULL;
  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", c2_h_u, 15, 0U, 0U, 0U, 2, 1, strlen
                 (c2_h_u)), false);
  return c2_c_y;
}

static const mxArray *c2_b_emlrt_marshallOut(const uint32_T c2_h_u)
{
  const mxArray *c2_c_y = NULL;
  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", &c2_h_u, 7, 0U, 0U, 0U, 0), false);
  return c2_c_y;
}

static real32_T c2_norm(SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance,
  real32_T c2_x[3])
{
  int32_T c2_i72;
  real32_T c2_b_x[3];
  for (c2_i72 = 0; c2_i72 < 3; c2_i72++) {
    c2_b_x[c2_i72] = c2_x[c2_i72];
  }

  return c2_eml_xnrm2(chartInstance, c2_b_x);
}

static real32_T c2_eml_xnrm2(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, real32_T c2_x[3])
{
  real32_T c2_c_y;
  real32_T c2_scale;
  int32_T c2_k;
  int32_T c2_b_k;
  real32_T c2_b_x;
  real32_T c2_c_x;
  real32_T c2_absxk;
  real32_T c2_t;
  c2_below_threshold(chartInstance);
  c2_c_y = 0.0F;
  c2_scale = 1.17549435E-38F;
  for (c2_k = 1; c2_k < 4; c2_k++) {
    c2_b_k = c2_k - 1;
    c2_b_x = c2_x[c2_b_k];
    c2_c_x = c2_b_x;
    c2_absxk = muSingleScalarAbs(c2_c_x);
    if (c2_absxk > c2_scale) {
      c2_t = c2_scale / c2_absxk;
      c2_c_y = 1.0F + c2_c_y * c2_t * c2_t;
      c2_scale = c2_absxk;
    } else {
      c2_t = c2_absxk / c2_scale;
      c2_c_y += c2_t * c2_t;
    }
  }

  return c2_scale * muSingleScalarSqrt(c2_c_y);
}

static void c2_below_threshold(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static const mxArray *c2_i_sf_marshallOut(void *chartInstanceVoid, void
  *c2_e_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  int32_T c2_h_u;
  const mxArray *c2_c_y = NULL;
  SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance;
  chartInstance = (SFc2_CreaminoSimulinkModelInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_h_u = *(int32_T *)c2_e_inData;
  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", &c2_h_u, 6, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_c_y, false);
  return c2_mxArrayOutData;
}

static int32_T c2_h_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_h_u, const emlrtMsgIdentifier *c2_parentId)
{
  int32_T c2_c_y;
  int32_T c2_i73;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_h_u), &c2_i73, 1, 6, 0U, 0, 0U, 0);
  c2_c_y = c2_i73;
  sf_mex_destroy(&c2_h_u);
  return c2_c_y;
}

static void c2_g_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_sfEvent;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  int32_T c2_c_y;
  SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance;
  chartInstance = (SFc2_CreaminoSimulinkModelInstanceStruct *)chartInstanceVoid;
  c2_b_sfEvent = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_c_y = c2_h_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_sfEvent),
    &c2_thisId);
  sf_mex_destroy(&c2_b_sfEvent);
  *(int32_T *)c2_outData = c2_c_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static uint8_T c2_i_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_b_is_active_c2_CreaminoSimulinkModel, const
  char_T *c2_identifier)
{
  uint8_T c2_c_y;
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_c_y = c2_j_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c2_b_is_active_c2_CreaminoSimulinkModel), &c2_thisId);
  sf_mex_destroy(&c2_b_is_active_c2_CreaminoSimulinkModel);
  return c2_c_y;
}

static uint8_T c2_j_emlrt_marshallIn(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance, const mxArray *c2_h_u, const emlrtMsgIdentifier *c2_parentId)
{
  uint8_T c2_c_y;
  uint8_T c2_u0;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_h_u), &c2_u0, 1, 3, 0U, 0, 0U, 0);
  c2_c_y = c2_u0;
  sf_mex_destroy(&c2_h_u);
  return c2_c_y;
}

static void init_dsm_address_info(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static void init_simulink_io_address(SFc2_CreaminoSimulinkModelInstanceStruct
  *chartInstance)
{
  chartInstance->c2_g_u = (real32_T (*)[16777216])ssGetInputPortSignal_wrapper
    (chartInstance->S, 0);
  chartInstance->c2_b_labels = (real32_T (*)[87311])ssGetInputPortSignal_wrapper
    (chartInstance->S, 1);
  chartInstance->c2_b_indiceGrigU2 = (real32_T (*)[87311])
    ssGetInputPortSignal_wrapper(chartInstance->S, 2);
  chartInstance->c2_yP = (real_T (*)[6144])ssGetOutputPortSignal_wrapper
    (chartInstance->S, 1);
  chartInstance->c2_AnoP = (real32_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 3);
  chartInstance->c2_BnoP = (real32_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 4);
}

/* SFunction Glue Code */
#ifdef utFree
#undef utFree
#endif

#ifdef utMalloc
#undef utMalloc
#endif

#ifdef __cplusplus

extern "C" void *utMalloc(size_t size);
extern "C" void utFree(void*);

#else

extern void *utMalloc(size_t size);
extern void utFree(void*);

#endif

void sf_c2_CreaminoSimulinkModel_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(2470822246U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(239372095U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(1378144027U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(3739137644U);
}

mxArray* sf_c2_CreaminoSimulinkModel_get_post_codegen_info(void);
mxArray *sf_c2_CreaminoSimulinkModel_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals", "postCodegenInfo" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1, 1, sizeof
    (autoinheritanceFields)/sizeof(autoinheritanceFields[0]),
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("oj8GhLasLo0ZZLCHidFUh");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,5,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(64);
      pr[1] = (double)(262144);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(9));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(87311);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(9));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(87311);
      mxSetField(mxData,2,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(9));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,2,"type",mxType);
    }

    mxSetField(mxData,2,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,3,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(9));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,3,"type",mxType);
    }

    mxSetField(mxData,3,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,4,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(9));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,4,"type",mxType);
    }

    mxSetField(mxData,4,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"inputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"parameters",mxCreateDoubleMatrix(0,0,
                mxREAL));
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,1,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(64);
      pr[1] = (double)(96);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"outputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"locals",mxCreateDoubleMatrix(0,0,mxREAL));
  }

  {
    mxArray* mxPostCodegenInfo =
      sf_c2_CreaminoSimulinkModel_get_post_codegen_info();
    mxSetField(mxAutoinheritanceInfo,0,"postCodegenInfo",mxPostCodegenInfo);
  }

  return(mxAutoinheritanceInfo);
}

mxArray *sf_c2_CreaminoSimulinkModel_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

mxArray *sf_c2_CreaminoSimulinkModel_jit_fallback_info(void)
{
  const char *infoFields[] = { "fallbackType", "fallbackReason",
    "incompatibleSymbol", };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 3, infoFields);
  mxArray *fallbackReason = mxCreateString("feature_off");
  mxArray *incompatibleSymbol = mxCreateString("");
  mxArray *fallbackType = mxCreateString("early");
  mxSetField(mxInfo, 0, infoFields[0], fallbackType);
  mxSetField(mxInfo, 0, infoFields[1], fallbackReason);
  mxSetField(mxInfo, 0, infoFields[2], incompatibleSymbol);
  return mxInfo;
}

mxArray *sf_c2_CreaminoSimulinkModel_updateBuildInfo_args_info(void)
{
  mxArray *mxBIArgs = mxCreateCellMatrix(1,0);
  return mxBIArgs;
}

mxArray* sf_c2_CreaminoSimulinkModel_get_post_codegen_info(void)
{
  const char* fieldNames[] = { "exportedFunctionsUsedByThisChart",
    "exportedFunctionsChecksum" };

  mwSize dims[2] = { 1, 1 };

  mxArray* mxPostCodegenInfo = mxCreateStructArray(2, dims, sizeof(fieldNames)/
    sizeof(fieldNames[0]), fieldNames);

  {
    mxArray* mxExportedFunctionsChecksum = mxCreateString("");
    mwSize exp_dims[2] = { 0, 1 };

    mxArray* mxExportedFunctionsUsedByThisChart = mxCreateCellArray(2, exp_dims);
    mxSetField(mxPostCodegenInfo, 0, "exportedFunctionsUsedByThisChart",
               mxExportedFunctionsUsedByThisChart);
    mxSetField(mxPostCodegenInfo, 0, "exportedFunctionsChecksum",
               mxExportedFunctionsChecksum);
  }

  return mxPostCodegenInfo;
}

static const mxArray *sf_get_sim_state_info_c2_CreaminoSimulinkModel(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x2'type','srcId','name','auxInfo'{{M[1],M[5],T\"yP\",},{M[8],M[0],T\"is_active_c2_CreaminoSimulinkModel\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 2, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c2_CreaminoSimulinkModel_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance;
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
    chartInstance = (SFc2_CreaminoSimulinkModelInstanceStruct *)
      chartInfo->chartInstance;
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _CreaminoSimulinkModelMachineNumber_,
           2,
           1,
           1,
           0,
           6,
           0,
           0,
           0,
           0,
           0,
           &(chartInstance->chartNumber),
           &(chartInstance->instanceNumber),
           (void *)S);

        /* Each instance must initialize its own list of scripts */
        init_script_number_translation(_CreaminoSimulinkModelMachineNumber_,
          chartInstance->chartNumber,chartInstance->instanceNumber);
        if (chartAlreadyPresent==0) {
          /* this is the first instance */
          sf_debug_set_chart_disable_implicit_casting
            (sfGlobalDebugInstanceStruct,_CreaminoSimulinkModelMachineNumber_,
             chartInstance->chartNumber,1);
          sf_debug_set_chart_event_thresholds(sfGlobalDebugInstanceStruct,
            _CreaminoSimulinkModelMachineNumber_,
            chartInstance->chartNumber,
            0,
            0,
            0);
          _SFD_SET_DATA_PROPS(0,1,1,0,"u");
          _SFD_SET_DATA_PROPS(1,1,1,0,"labels");
          _SFD_SET_DATA_PROPS(2,1,1,0,"indiceGrigU2");
          _SFD_SET_DATA_PROPS(3,2,0,1,"yP");
          _SFD_SET_DATA_PROPS(4,1,1,0,"AnoP");
          _SFD_SET_DATA_PROPS(5,1,1,0,"BnoP");
          _SFD_STATE_INFO(0,0,2);
          _SFD_CH_SUBSTATE_COUNT(0);
          _SFD_CH_SUBSTATE_DECOMP(0);
        }

        _SFD_CV_INIT_CHART(0,0,0,0);

        {
          _SFD_CV_INIT_STATE(0,0,0,0,0,0,NULL,NULL);
        }

        _SFD_CV_INIT_TRANS(0,0,NULL,NULL,0,NULL);

        /* Initialization of MATLAB Function Model Coverage */
        _SFD_CV_INIT_EML(0,1,1,0,0,0,0,4,0,0,0);
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,424);
        _SFD_CV_INIT_EML_FOR(0,1,0,127,156,302);
        _SFD_CV_INIT_EML_FOR(0,1,1,164,178,269);
        _SFD_CV_INIT_EML_FOR(0,1,2,338,349,424);
        _SFD_CV_INIT_EML_FOR(0,1,3,357,368,416);

        {
          unsigned int dimVector[2];
          dimVector[0]= 64;
          dimVector[1]= 262144;
          _SFD_SET_DATA_COMPILED_PROPS(0,SF_SINGLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_d_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 1;
          dimVector[1]= 87311;
          _SFD_SET_DATA_COMPILED_PROPS(1,SF_SINGLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_c_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 1;
          dimVector[1]= 87311;
          _SFD_SET_DATA_COMPILED_PROPS(2,SF_SINGLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_c_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 64;
          dimVector[1]= 96;
          _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)
            c2_sf_marshallIn);
        }

        _SFD_SET_DATA_COMPILED_PROPS(4,SF_SINGLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(5,SF_SINGLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_VALUE_PTR(0U, *chartInstance->c2_g_u);
        _SFD_SET_DATA_VALUE_PTR(1U, *chartInstance->c2_b_labels);
        _SFD_SET_DATA_VALUE_PTR(2U, *chartInstance->c2_b_indiceGrigU2);
        _SFD_SET_DATA_VALUE_PTR(3U, *chartInstance->c2_yP);
        _SFD_SET_DATA_VALUE_PTR(4U, chartInstance->c2_AnoP);
        _SFD_SET_DATA_VALUE_PTR(5U, chartInstance->c2_BnoP);
      }
    } else {
      sf_debug_reset_current_state_configuration(sfGlobalDebugInstanceStruct,
        _CreaminoSimulinkModelMachineNumber_,chartInstance->chartNumber,
        chartInstance->instanceNumber);
    }
  }
}

static const char* sf_get_instance_specialization(void)
{
  return "PRml3fFe3Q7EM9YDM6gDQC";
}

static void sf_opaque_initialize_c2_CreaminoSimulinkModel(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc2_CreaminoSimulinkModelInstanceStruct*)
    chartInstanceVar)->S,0);
  initialize_params_c2_CreaminoSimulinkModel
    ((SFc2_CreaminoSimulinkModelInstanceStruct*) chartInstanceVar);
  initialize_c2_CreaminoSimulinkModel((SFc2_CreaminoSimulinkModelInstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_enable_c2_CreaminoSimulinkModel(void *chartInstanceVar)
{
  enable_c2_CreaminoSimulinkModel((SFc2_CreaminoSimulinkModelInstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_disable_c2_CreaminoSimulinkModel(void *chartInstanceVar)
{
  disable_c2_CreaminoSimulinkModel((SFc2_CreaminoSimulinkModelInstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_gateway_c2_CreaminoSimulinkModel(void *chartInstanceVar)
{
  sf_gateway_c2_CreaminoSimulinkModel((SFc2_CreaminoSimulinkModelInstanceStruct*)
    chartInstanceVar);
}

static const mxArray* sf_opaque_get_sim_state_c2_CreaminoSimulinkModel(SimStruct*
  S)
{
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
  ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
  return get_sim_state_c2_CreaminoSimulinkModel
    ((SFc2_CreaminoSimulinkModelInstanceStruct*)chartInfo->chartInstance);/* raw sim ctx */
}

static void sf_opaque_set_sim_state_c2_CreaminoSimulinkModel(SimStruct* S, const
  mxArray *st)
{
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
  ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
  set_sim_state_c2_CreaminoSimulinkModel
    ((SFc2_CreaminoSimulinkModelInstanceStruct*)chartInfo->chartInstance, st);
}

static void sf_opaque_terminate_c2_CreaminoSimulinkModel(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc2_CreaminoSimulinkModelInstanceStruct*) chartInstanceVar)
      ->S;
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_CreaminoSimulinkModel_optimization_info();
    }

    finalize_c2_CreaminoSimulinkModel((SFc2_CreaminoSimulinkModelInstanceStruct*)
      chartInstanceVar);
    utFree(chartInstanceVar);
    if (crtInfo != NULL) {
      utFree(crtInfo);
    }

    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc2_CreaminoSimulinkModel
    ((SFc2_CreaminoSimulinkModelInstanceStruct*) chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c2_CreaminoSimulinkModel(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
    initialize_params_c2_CreaminoSimulinkModel
      ((SFc2_CreaminoSimulinkModelInstanceStruct*)(chartInfo->chartInstance));
  }
}

static void mdlSetWorkWidths_c2_CreaminoSimulinkModel(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_CreaminoSimulinkModel_optimization_info();
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(sf_get_instance_specialization(),infoStruct,2);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,sf_rtw_info_uint_prop(sf_get_instance_specialization(),
                infoStruct,2,"RTWCG"));
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop
      (sf_get_instance_specialization(),infoStruct,2,
       "gatewayCannotBeInlinedMultipleTimes"));
    sf_update_buildInfo(sf_get_instance_specialization(),infoStruct,2);
    if (chartIsInlinable) {
      ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 1, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 2, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 3, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 4, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,2,5);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,2,1);
    }

    {
      unsigned int outPortIdx;
      for (outPortIdx=1; outPortIdx<=1; ++outPortIdx) {
        ssSetOutputPortOptimizeInIR(S, outPortIdx, 1U);
      }
    }

    {
      unsigned int inPortIdx;
      for (inPortIdx=0; inPortIdx < 5; ++inPortIdx) {
        ssSetInputPortOptimizeInIR(S, inPortIdx, 1U);
      }
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,2);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(2423719198U));
  ssSetChecksum1(S,(3122352849U));
  ssSetChecksum2(S,(1342266898U));
  ssSetChecksum3(S,(3822448398U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c2_CreaminoSimulinkModel(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c2_CreaminoSimulinkModel(SimStruct *S)
{
  SFc2_CreaminoSimulinkModelInstanceStruct *chartInstance;
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)utMalloc(sizeof
    (ChartRunTimeInfo));
  chartInstance = (SFc2_CreaminoSimulinkModelInstanceStruct *)utMalloc(sizeof
    (SFc2_CreaminoSimulinkModelInstanceStruct));
  memset(chartInstance, 0, sizeof(SFc2_CreaminoSimulinkModelInstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway =
    sf_opaque_gateway_c2_CreaminoSimulinkModel;
  chartInstance->chartInfo.initializeChart =
    sf_opaque_initialize_c2_CreaminoSimulinkModel;
  chartInstance->chartInfo.terminateChart =
    sf_opaque_terminate_c2_CreaminoSimulinkModel;
  chartInstance->chartInfo.enableChart =
    sf_opaque_enable_c2_CreaminoSimulinkModel;
  chartInstance->chartInfo.disableChart =
    sf_opaque_disable_c2_CreaminoSimulinkModel;
  chartInstance->chartInfo.getSimState =
    sf_opaque_get_sim_state_c2_CreaminoSimulinkModel;
  chartInstance->chartInfo.setSimState =
    sf_opaque_set_sim_state_c2_CreaminoSimulinkModel;
  chartInstance->chartInfo.getSimStateInfo =
    sf_get_sim_state_info_c2_CreaminoSimulinkModel;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c2_CreaminoSimulinkModel;
  chartInstance->chartInfo.mdlStart = mdlStart_c2_CreaminoSimulinkModel;
  chartInstance->chartInfo.mdlSetWorkWidths =
    mdlSetWorkWidths_c2_CreaminoSimulinkModel;
  chartInstance->chartInfo.extModeExec = NULL;
  chartInstance->chartInfo.restoreLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.restoreBeforeLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.storeCurrentConfiguration = NULL;
  chartInstance->chartInfo.callAtomicSubchartUserFcn = NULL;
  chartInstance->chartInfo.callAtomicSubchartAutoFcn = NULL;
  chartInstance->chartInfo.debugInstance = sfGlobalDebugInstanceStruct;
  chartInstance->S = S;
  crtInfo->checksum = SF_RUNTIME_INFO_CHECKSUM;
  crtInfo->instanceInfo = (&(chartInstance->chartInfo));
  crtInfo->isJITEnabled = false;
  crtInfo->compiledInfo = NULL;
  ssSetUserData(S,(void *)(crtInfo));  /* register the chart instance with simstruct */
  init_dsm_address_info(chartInstance);
  init_simulink_io_address(chartInstance);
  if (!sim_mode_is_rtw_gen(S)) {
  }

  sf_opaque_init_subchart_simstructs(chartInstance->chartInfo.chartInstance);
  chart_debug_initialization(S,1);
}

void c2_CreaminoSimulinkModel_method_dispatcher(SimStruct *S, int_T method, void
  *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c2_CreaminoSimulinkModel(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c2_CreaminoSimulinkModel(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c2_CreaminoSimulinkModel(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c2_CreaminoSimulinkModel_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
