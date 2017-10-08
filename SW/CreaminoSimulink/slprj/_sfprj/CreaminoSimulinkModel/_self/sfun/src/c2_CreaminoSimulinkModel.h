#ifndef __c2_CreaminoSimulinkModel_h__
#define __c2_CreaminoSimulinkModel_h__

/* Include files */
#include "sf_runtime/sfc_sf.h"
#include "sf_runtime/sfc_mex.h"
#include "rtwtypes.h"
#include "multiword_types.h"

/* Type Definitions */
#ifndef typedef_SFc2_CreaminoSimulinkModelInstanceStruct
#define typedef_SFc2_CreaminoSimulinkModelInstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  int32_T c2_sfEvent;
  boolean_T c2_isStable;
  boolean_T c2_doneDoubleBufferReInit;
  uint8_T c2_is_active_c2_CreaminoSimulinkModel;
  real32_T c2_u[16777216];
  real32_T c2_ynop_data[16777216];
  real32_T c2_b_u[16777216];
  real_T c2_yF[5587904];
  real_T c2_a[5587904];
  real32_T c2_labels[87311];
  real32_T c2_indiceGrigU2[87311];
  real32_T c2_y_data[16777216];
  real32_T c2_tmp_data[16777216];
  real32_T c2_inData_data[16777216];
  real32_T c2_u_data[16777216];
  real32_T c2_y[16777216];
  real32_T c2_fv0[16777216];
  real32_T c2_inData[16777216];
  real32_T c2_c_u[16777216];
  real32_T c2_b_inData[16777216];
  real32_T c2_d_u[16777216];
  real_T c2_b_y[5587904];
  real_T c2_dv2[5587904];
  real_T c2_c_inData[5587904];
  real_T c2_e_u[5587904];
  real32_T c2_d_inData[87311];
  real32_T c2_f_u[87311];
  real32_T (*c2_g_u)[16777216];
  real32_T (*c2_b_labels)[87311];
  real32_T (*c2_b_indiceGrigU2)[87311];
  real_T (*c2_yP)[6144];
  real32_T *c2_AnoP;
  real32_T *c2_BnoP;
} SFc2_CreaminoSimulinkModelInstanceStruct;

#endif                                 /*typedef_SFc2_CreaminoSimulinkModelInstanceStruct*/

/* Named Constants */

/* Variable Declarations */
extern struct SfDebugInstanceStruct *sfGlobalDebugInstanceStruct;

/* Variable Definitions */

/* Function Declarations */
extern const mxArray
  *sf_c2_CreaminoSimulinkModel_get_eml_resolved_functions_info(void);

/* Function Definitions */
extern void sf_c2_CreaminoSimulinkModel_get_check_sum(mxArray *plhs[]);
extern void c2_CreaminoSimulinkModel_method_dispatcher(SimStruct *S, int_T
  method, void *data);

#endif
