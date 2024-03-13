#ifndef CustomEspFilter_H
#define CustomEspFilter_H

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstddef>
#endif

#include "LMS/LMS.h"
#include "NLMS/NLMS.h"

const int FILTERORDER = 50;
const float FILTERPARAM = 0.001;
NLMS lms(FILTERORDER, FILTERPARAM);
// const float FILTERPARAM = 0.000001;
// LMS lms(FILTERORDER, FILTERPARAM);

void setupFilter()
{
}

float useFilter(float refInp, float errorInp)
{
  float output = lms.getOutput(refInp, errorInp);
  lms.updateWeights();
  Serial.print(">OUTPUT:");
  Serial.println(output);
  Serial.print(">ERROR:");
  Serial.println(errorInp);
  return output;
}

// float useFilterSingleInput(int16_t *inputs, int length) {
//   int16_t prevOutput = 0;
//   const int delay = 1;
//   for(int i=delay; i<length; i++) {
//     prevOutput = useFilter(inputs[i], inputs[i-delay] - prevOutput);
//   }
// }

#endif