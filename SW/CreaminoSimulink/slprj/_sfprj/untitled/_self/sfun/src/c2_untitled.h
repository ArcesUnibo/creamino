#ifndef __c2_untitled_h__
#define __c2_untitled_h__

/* Include files */
#include "sf_runtime/sfc_sf.h"
#include "sf_runtime/sfc_mex.h"
#include "rtwtypes.h"
#include "multiword_types.h"

/* Type Definitions */
#ifndef typedef_SFc2_untitledInstanceStruct
#define typedef_SFc2_untitledInstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  int32_T c2_sfEvent;
  boolean_T c2_isStable;
  boolean_T c2_doneDoubleBufferReInit;
  uint8_T c2_is_active_c2_untitled;
  real32_T c2_inData[13651919];
  real32_T c2_u[13651919];
  real32_T c2_Vparc[13651919];
  real_T *c2_b_u;
  real_T *c2_y;
  real32_T (*c2_b_Vparc)[13651919];
  real_T *c2_AnoP;
  real_T *c2_BnoP;
} SFc2_untitledInstanceStruct;

#endif                                 /*typedef_SFc2_untitledInstanceStruct*/

/* Named Constants */

/* Variable Declarations */
extern struct SfDebugInstanceStruct *sfGlobalDebugInstanceStruct;

/* Variable Definitions */

/* Function Declarations */
extern const mxArray *sf_c2_untitled_get_eml_resolved_functions_info(void);

/* Function Definitions */
extern void sf_c2_untitled_get_check_sum(mxArray *plhs[]);
extern void c2_untitled_method_dispatcher(SimStruct *S, int_T method, void *data);

#endif
