/* Include files */

#include <stddef.h>
#include "blas.h"
#include "untitled_sfun.h"
#include "c2_untitled.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance->chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance->instanceNumber)
#include "untitled_sfun_debug_macros.h"
#define _SF_MEX_LISTEN_FOR_CTRL_C(S)   sf_mex_listen_for_ctrl_c(sfGlobalDebugInstanceStruct,S);

/* Type Definitions */

/* Named Constants */
#define CALL_EVENT                     (-1)

/* Variable Declarations */

/* Variable Definitions */
static real_T _sfTime_;
static const char * c2_debug_family_names[7] = { "nargin", "nargout", "u",
  "Vparc", "AnoP", "BnoP", "y" };

/* Function Declarations */
static void initialize_c2_untitled(SFc2_untitledInstanceStruct *chartInstance);
static void initialize_params_c2_untitled(SFc2_untitledInstanceStruct
  *chartInstance);
static void enable_c2_untitled(SFc2_untitledInstanceStruct *chartInstance);
static void disable_c2_untitled(SFc2_untitledInstanceStruct *chartInstance);
static void c2_update_debugger_state_c2_untitled(SFc2_untitledInstanceStruct
  *chartInstance);
static const mxArray *get_sim_state_c2_untitled(SFc2_untitledInstanceStruct
  *chartInstance);
static void set_sim_state_c2_untitled(SFc2_untitledInstanceStruct *chartInstance,
  const mxArray *c2_st);
static void finalize_c2_untitled(SFc2_untitledInstanceStruct *chartInstance);
static void sf_gateway_c2_untitled(SFc2_untitledInstanceStruct *chartInstance);
static void mdl_start_c2_untitled(SFc2_untitledInstanceStruct *chartInstance);
static void initSimStructsc2_untitled(SFc2_untitledInstanceStruct *chartInstance);
static void init_script_number_translation(uint32_T c2_machineNumber, uint32_T
  c2_chartNumber, uint32_T c2_instanceNumber);
static const mxArray *c2_sf_marshallOut(void *chartInstanceVoid, void
  *c2_b_inData);
static real_T c2_emlrt_marshallIn(SFc2_untitledInstanceStruct *chartInstance,
  const mxArray *c2_b_y, const char_T *c2_identifier);
static real_T c2_b_emlrt_marshallIn(SFc2_untitledInstanceStruct *chartInstance,
  const mxArray *c2_c_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_b_sf_marshallOut(void *chartInstanceVoid, void
  *c2_b_inData);
static const mxArray *c2_c_sf_marshallOut(void *chartInstanceVoid, void
  *c2_b_inData);
static int32_T c2_c_emlrt_marshallIn(SFc2_untitledInstanceStruct *chartInstance,
  const mxArray *c2_c_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static uint8_T c2_d_emlrt_marshallIn(SFc2_untitledInstanceStruct *chartInstance,
  const mxArray *c2_b_is_active_c2_untitled, const char_T *c2_identifier);
static uint8_T c2_e_emlrt_marshallIn(SFc2_untitledInstanceStruct *chartInstance,
  const mxArray *c2_c_u, const emlrtMsgIdentifier *c2_parentId);
static void init_dsm_address_info(SFc2_untitledInstanceStruct *chartInstance);
static void init_simulink_io_address(SFc2_untitledInstanceStruct *chartInstance);

/* Function Definitions */
static void initialize_c2_untitled(SFc2_untitledInstanceStruct *chartInstance)
{
  chartInstance->c2_sfEvent = CALL_EVENT;
  _sfTime_ = sf_get_time(chartInstance->S);
  chartInstance->c2_is_active_c2_untitled = 0U;
}

static void initialize_params_c2_untitled(SFc2_untitledInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static void enable_c2_untitled(SFc2_untitledInstanceStruct *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void disable_c2_untitled(SFc2_untitledInstanceStruct *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void c2_update_debugger_state_c2_untitled(SFc2_untitledInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static const mxArray *get_sim_state_c2_untitled(SFc2_untitledInstanceStruct
  *chartInstance)
{
  const mxArray *c2_st;
  const mxArray *c2_b_y = NULL;
  real_T c2_hoistedGlobal;
  real_T c2_c_u;
  const mxArray *c2_c_y = NULL;
  uint8_T c2_b_hoistedGlobal;
  uint8_T c2_d_u;
  const mxArray *c2_d_y = NULL;
  c2_st = NULL;
  c2_st = NULL;
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_createcellmatrix(2, 1), false);
  c2_hoistedGlobal = *chartInstance->c2_y;
  c2_c_u = c2_hoistedGlobal;
  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", &c2_c_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_b_y, 0, c2_c_y);
  c2_b_hoistedGlobal = chartInstance->c2_is_active_c2_untitled;
  c2_d_u = c2_b_hoistedGlobal;
  c2_d_y = NULL;
  sf_mex_assign(&c2_d_y, sf_mex_create("y", &c2_d_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_b_y, 1, c2_d_y);
  sf_mex_assign(&c2_st, c2_b_y, false);
  return c2_st;
}

static void set_sim_state_c2_untitled(SFc2_untitledInstanceStruct *chartInstance,
  const mxArray *c2_st)
{
  const mxArray *c2_c_u;
  chartInstance->c2_doneDoubleBufferReInit = true;
  c2_c_u = sf_mex_dup(c2_st);
  *chartInstance->c2_y = c2_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_c_u, 0)), "y");
  chartInstance->c2_is_active_c2_untitled = c2_d_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_c_u, 1)), "is_active_c2_untitled");
  sf_mex_destroy(&c2_c_u);
  c2_update_debugger_state_c2_untitled(chartInstance);
  sf_mex_destroy(&c2_st);
}

static void finalize_c2_untitled(SFc2_untitledInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void sf_gateway_c2_untitled(SFc2_untitledInstanceStruct *chartInstance)
{
  real_T c2_hoistedGlobal;
  real_T c2_b_hoistedGlobal;
  real_T c2_c_hoistedGlobal;
  real_T c2_c_u;
  int32_T c2_i0;
  real_T c2_b_AnoP;
  real_T c2_b_BnoP;
  uint32_T c2_debug_family_var_map[7];
  real_T c2_nargin = 4.0;
  real_T c2_nargout = 1.0;
  real_T c2_b_y;
  int32_T c2_i1;
  _SFD_SYMBOL_SCOPE_PUSH(0U, 0U);
  _sfTime_ = sf_get_time(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 0U, chartInstance->c2_sfEvent);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c2_b_u, 0U);
  chartInstance->c2_sfEvent = CALL_EVENT;
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 0U, chartInstance->c2_sfEvent);
  c2_hoistedGlobal = *chartInstance->c2_b_u;
  c2_b_hoistedGlobal = *chartInstance->c2_AnoP;
  c2_c_hoistedGlobal = *chartInstance->c2_BnoP;
  c2_c_u = c2_hoistedGlobal;
  for (c2_i0 = 0; c2_i0 < 13651919; c2_i0++) {
    chartInstance->c2_Vparc[c2_i0] = (*chartInstance->c2_b_Vparc)[c2_i0];
  }

  c2_b_AnoP = c2_b_hoistedGlobal;
  c2_b_BnoP = c2_c_hoistedGlobal;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 7U, 7U, c2_debug_family_names,
    c2_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargin, 0U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargout, 1U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_c_u, 2U, c2_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(chartInstance->c2_Vparc, 3U, c2_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_b_AnoP, 4U, c2_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_b_BnoP, 5U, c2_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_y, 6U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 8);
  c2_b_y = c2_c_u;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, -8);
  _SFD_SYMBOL_SCOPE_POP();
  *chartInstance->c2_y = c2_b_y;
  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 0U, chartInstance->c2_sfEvent);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_untitledMachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c2_y, 1U);
  for (c2_i1 = 0; c2_i1 < 13651919; c2_i1++) {
    _SFD_DATA_RANGE_CHECK((real_T)(*chartInstance->c2_b_Vparc)[c2_i1], 2U);
  }

  _SFD_DATA_RANGE_CHECK(*chartInstance->c2_AnoP, 3U);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c2_BnoP, 4U);
}

static void mdl_start_c2_untitled(SFc2_untitledInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void initSimStructsc2_untitled(SFc2_untitledInstanceStruct *chartInstance)
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
  *c2_b_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  real_T c2_c_u;
  const mxArray *c2_b_y = NULL;
  SFc2_untitledInstanceStruct *chartInstance;
  chartInstance = (SFc2_untitledInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_c_u = *(real_T *)c2_b_inData;
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", &c2_c_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static real_T c2_emlrt_marshallIn(SFc2_untitledInstanceStruct *chartInstance,
  const mxArray *c2_b_y, const char_T *c2_identifier)
{
  real_T c2_c_y;
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_c_y = c2_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_y), &c2_thisId);
  sf_mex_destroy(&c2_b_y);
  return c2_c_y;
}

static real_T c2_b_emlrt_marshallIn(SFc2_untitledInstanceStruct *chartInstance,
  const mxArray *c2_c_u, const emlrtMsgIdentifier *c2_parentId)
{
  real_T c2_b_y;
  real_T c2_d0;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_c_u), &c2_d0, 1, 0, 0U, 0, 0U, 0);
  c2_b_y = c2_d0;
  sf_mex_destroy(&c2_c_u);
  return c2_b_y;
}

static void c2_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_y;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_c_y;
  SFc2_untitledInstanceStruct *chartInstance;
  chartInstance = (SFc2_untitledInstanceStruct *)chartInstanceVoid;
  c2_b_y = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_c_y = c2_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_y), &c2_thisId);
  sf_mex_destroy(&c2_b_y);
  *(real_T *)c2_outData = c2_c_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_b_sf_marshallOut(void *chartInstanceVoid, void
  *c2_b_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  int32_T c2_i2;
  int32_T c2_i3;
  int32_T c2_i4;
  int32_T c2_i5;
  int32_T c2_i6;
  int32_T c2_i7;
  int32_T c2_i8;
  int32_T c2_i9;
  int32_T c2_i10;
  int32_T c2_i11;
  const mxArray *c2_b_y = NULL;
  SFc2_untitledInstanceStruct *chartInstance;
  chartInstance = (SFc2_untitledInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_i2 = 0;
  for (c2_i3 = 0; c2_i3 < 239; c2_i3++) {
    c2_i4 = 0;
    for (c2_i5 = 0; c2_i5 < 239; c2_i5++) {
      for (c2_i6 = 0; c2_i6 < 239; c2_i6++) {
        chartInstance->c2_inData[(c2_i6 + c2_i4) + c2_i2] = (*(real32_T (*)
          [13651919])c2_b_inData)[(c2_i6 + c2_i4) + c2_i2];
      }

      c2_i4 += 239;
    }

    c2_i2 += 57121;
  }

  c2_i7 = 0;
  for (c2_i8 = 0; c2_i8 < 239; c2_i8++) {
    c2_i9 = 0;
    for (c2_i10 = 0; c2_i10 < 239; c2_i10++) {
      for (c2_i11 = 0; c2_i11 < 239; c2_i11++) {
        chartInstance->c2_u[(c2_i11 + c2_i9) + c2_i7] = chartInstance->
          c2_inData[(c2_i11 + c2_i9) + c2_i7];
      }

      c2_i9 += 239;
    }

    c2_i7 += 57121;
  }

  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", chartInstance->c2_u, 1, 0U, 1U, 0U,
    3, 239, 239, 239), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

const mxArray *sf_c2_untitled_get_eml_resolved_functions_info(void)
{
  const mxArray *c2_nameCaptureInfo = NULL;
  c2_nameCaptureInfo = NULL;
  sf_mex_assign(&c2_nameCaptureInfo, sf_mex_create("nameCaptureInfo", NULL, 0,
    0U, 1U, 0U, 2, 0, 1), false);
  return c2_nameCaptureInfo;
}

static const mxArray *c2_c_sf_marshallOut(void *chartInstanceVoid, void
  *c2_b_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  int32_T c2_c_u;
  const mxArray *c2_b_y = NULL;
  SFc2_untitledInstanceStruct *chartInstance;
  chartInstance = (SFc2_untitledInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_c_u = *(int32_T *)c2_b_inData;
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", &c2_c_u, 6, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static int32_T c2_c_emlrt_marshallIn(SFc2_untitledInstanceStruct *chartInstance,
  const mxArray *c2_c_u, const emlrtMsgIdentifier *c2_parentId)
{
  int32_T c2_b_y;
  int32_T c2_i12;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_c_u), &c2_i12, 1, 6, 0U, 0, 0U, 0);
  c2_b_y = c2_i12;
  sf_mex_destroy(&c2_c_u);
  return c2_b_y;
}

static void c2_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_sfEvent;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  int32_T c2_b_y;
  SFc2_untitledInstanceStruct *chartInstance;
  chartInstance = (SFc2_untitledInstanceStruct *)chartInstanceVoid;
  c2_b_sfEvent = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_b_y = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_sfEvent),
    &c2_thisId);
  sf_mex_destroy(&c2_b_sfEvent);
  *(int32_T *)c2_outData = c2_b_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static uint8_T c2_d_emlrt_marshallIn(SFc2_untitledInstanceStruct *chartInstance,
  const mxArray *c2_b_is_active_c2_untitled, const char_T *c2_identifier)
{
  uint8_T c2_b_y;
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_b_y = c2_e_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c2_b_is_active_c2_untitled), &c2_thisId);
  sf_mex_destroy(&c2_b_is_active_c2_untitled);
  return c2_b_y;
}

static uint8_T c2_e_emlrt_marshallIn(SFc2_untitledInstanceStruct *chartInstance,
  const mxArray *c2_c_u, const emlrtMsgIdentifier *c2_parentId)
{
  uint8_T c2_b_y;
  uint8_T c2_u0;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_c_u), &c2_u0, 1, 3, 0U, 0, 0U, 0);
  c2_b_y = c2_u0;
  sf_mex_destroy(&c2_c_u);
  return c2_b_y;
}

static void init_dsm_address_info(SFc2_untitledInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void init_simulink_io_address(SFc2_untitledInstanceStruct *chartInstance)
{
  chartInstance->c2_b_u = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 0);
  chartInstance->c2_y = (real_T *)ssGetOutputPortSignal_wrapper(chartInstance->S,
    1);
  chartInstance->c2_b_Vparc = (real32_T (*)[13651919])
    ssGetInputPortSignal_wrapper(chartInstance->S, 1);
  chartInstance->c2_AnoP = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 2);
  chartInstance->c2_BnoP = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 3);
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

void sf_c2_untitled_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(707844760U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(490344420U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(288515954U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(1409641154U);
}

mxArray* sf_c2_untitled_get_post_codegen_info(void);
mxArray *sf_c2_untitled_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals", "postCodegenInfo" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1, 1, sizeof
    (autoinheritanceFields)/sizeof(autoinheritanceFields[0]),
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("Y7suIlZfCE9hACSEVTNfgD");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,4,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
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

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,3,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(239);
      pr[1] = (double)(239);
      pr[2] = (double)(239);
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
      pr[1] = (double)(1);
      mxSetField(mxData,2,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
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
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,3,"type",mxType);
    }

    mxSetField(mxData,3,"complexity",mxCreateDoubleScalar(0));
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
      pr[0] = (double)(1);
      pr[1] = (double)(1);
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
    mxArray* mxPostCodegenInfo = sf_c2_untitled_get_post_codegen_info();
    mxSetField(mxAutoinheritanceInfo,0,"postCodegenInfo",mxPostCodegenInfo);
  }

  return(mxAutoinheritanceInfo);
}

mxArray *sf_c2_untitled_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

mxArray *sf_c2_untitled_jit_fallback_info(void)
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

mxArray *sf_c2_untitled_updateBuildInfo_args_info(void)
{
  mxArray *mxBIArgs = mxCreateCellMatrix(1,0);
  return mxBIArgs;
}

mxArray* sf_c2_untitled_get_post_codegen_info(void)
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

static const mxArray *sf_get_sim_state_info_c2_untitled(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x2'type','srcId','name','auxInfo'{{M[1],M[5],T\"y\",},{M[8],M[0],T\"is_active_c2_untitled\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 2, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c2_untitled_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc2_untitledInstanceStruct *chartInstance;
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
    chartInstance = (SFc2_untitledInstanceStruct *) chartInfo->chartInstance;
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _untitledMachineNumber_,
           2,
           1,
           1,
           0,
           5,
           0,
           0,
           0,
           0,
           0,
           &(chartInstance->chartNumber),
           &(chartInstance->instanceNumber),
           (void *)S);

        /* Each instance must initialize its own list of scripts */
        init_script_number_translation(_untitledMachineNumber_,
          chartInstance->chartNumber,chartInstance->instanceNumber);
        if (chartAlreadyPresent==0) {
          /* this is the first instance */
          sf_debug_set_chart_disable_implicit_casting
            (sfGlobalDebugInstanceStruct,_untitledMachineNumber_,
             chartInstance->chartNumber,1);
          sf_debug_set_chart_event_thresholds(sfGlobalDebugInstanceStruct,
            _untitledMachineNumber_,
            chartInstance->chartNumber,
            0,
            0,
            0);
          _SFD_SET_DATA_PROPS(0,1,1,0,"u");
          _SFD_SET_DATA_PROPS(1,2,0,1,"y");
          _SFD_SET_DATA_PROPS(2,1,1,0,"Vparc");
          _SFD_SET_DATA_PROPS(3,1,1,0,"AnoP");
          _SFD_SET_DATA_PROPS(4,1,1,0,"BnoP");
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
        _SFD_CV_INIT_EML(0,1,1,0,0,0,0,0,0,0,0);
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,148);
        _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);

        {
          unsigned int dimVector[3];
          dimVector[0]= 239;
          dimVector[1]= 239;
          dimVector[2]= 239;
          _SFD_SET_DATA_COMPILED_PROPS(2,SF_SINGLE,3,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_b_sf_marshallOut,(MexInFcnForType)NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_VALUE_PTR(0U, chartInstance->c2_b_u);
        _SFD_SET_DATA_VALUE_PTR(1U, chartInstance->c2_y);
        _SFD_SET_DATA_VALUE_PTR(2U, *chartInstance->c2_b_Vparc);
        _SFD_SET_DATA_VALUE_PTR(3U, chartInstance->c2_AnoP);
        _SFD_SET_DATA_VALUE_PTR(4U, chartInstance->c2_BnoP);
      }
    } else {
      sf_debug_reset_current_state_configuration(sfGlobalDebugInstanceStruct,
        _untitledMachineNumber_,chartInstance->chartNumber,
        chartInstance->instanceNumber);
    }
  }
}

static const char* sf_get_instance_specialization(void)
{
  return "t36LqlGcwUhlLpZD7BMNpD";
}

static void sf_opaque_initialize_c2_untitled(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc2_untitledInstanceStruct*) chartInstanceVar)
    ->S,0);
  initialize_params_c2_untitled((SFc2_untitledInstanceStruct*) chartInstanceVar);
  initialize_c2_untitled((SFc2_untitledInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_enable_c2_untitled(void *chartInstanceVar)
{
  enable_c2_untitled((SFc2_untitledInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_disable_c2_untitled(void *chartInstanceVar)
{
  disable_c2_untitled((SFc2_untitledInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_gateway_c2_untitled(void *chartInstanceVar)
{
  sf_gateway_c2_untitled((SFc2_untitledInstanceStruct*) chartInstanceVar);
}

static const mxArray* sf_opaque_get_sim_state_c2_untitled(SimStruct* S)
{
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
  ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
  return get_sim_state_c2_untitled((SFc2_untitledInstanceStruct*)
    chartInfo->chartInstance);         /* raw sim ctx */
}

static void sf_opaque_set_sim_state_c2_untitled(SimStruct* S, const mxArray *st)
{
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
  ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
  set_sim_state_c2_untitled((SFc2_untitledInstanceStruct*)
    chartInfo->chartInstance, st);
}

static void sf_opaque_terminate_c2_untitled(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc2_untitledInstanceStruct*) chartInstanceVar)->S;
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_untitled_optimization_info();
    }

    finalize_c2_untitled((SFc2_untitledInstanceStruct*) chartInstanceVar);
    utFree(chartInstanceVar);
    if (crtInfo != NULL) {
      utFree(crtInfo);
    }

    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc2_untitled((SFc2_untitledInstanceStruct*) chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c2_untitled(SimStruct *S)
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
    initialize_params_c2_untitled((SFc2_untitledInstanceStruct*)
      (chartInfo->chartInstance));
  }
}

static void mdlSetWorkWidths_c2_untitled(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_untitled_optimization_info();
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
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,2,4);
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
      for (inPortIdx=0; inPortIdx < 4; ++inPortIdx) {
        ssSetInputPortOptimizeInIR(S, inPortIdx, 1U);
      }
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,2);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(2713897778U));
  ssSetChecksum1(S,(3731100214U));
  ssSetChecksum2(S,(2666443666U));
  ssSetChecksum3(S,(3967464178U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c2_untitled(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c2_untitled(SimStruct *S)
{
  SFc2_untitledInstanceStruct *chartInstance;
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)utMalloc(sizeof
    (ChartRunTimeInfo));
  chartInstance = (SFc2_untitledInstanceStruct *)utMalloc(sizeof
    (SFc2_untitledInstanceStruct));
  memset(chartInstance, 0, sizeof(SFc2_untitledInstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway = sf_opaque_gateway_c2_untitled;
  chartInstance->chartInfo.initializeChart = sf_opaque_initialize_c2_untitled;
  chartInstance->chartInfo.terminateChart = sf_opaque_terminate_c2_untitled;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c2_untitled;
  chartInstance->chartInfo.disableChart = sf_opaque_disable_c2_untitled;
  chartInstance->chartInfo.getSimState = sf_opaque_get_sim_state_c2_untitled;
  chartInstance->chartInfo.setSimState = sf_opaque_set_sim_state_c2_untitled;
  chartInstance->chartInfo.getSimStateInfo = sf_get_sim_state_info_c2_untitled;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c2_untitled;
  chartInstance->chartInfo.mdlStart = mdlStart_c2_untitled;
  chartInstance->chartInfo.mdlSetWorkWidths = mdlSetWorkWidths_c2_untitled;
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

void c2_untitled_method_dispatcher(SimStruct *S, int_T method, void *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c2_untitled(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c2_untitled(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c2_untitled(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c2_untitled_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
