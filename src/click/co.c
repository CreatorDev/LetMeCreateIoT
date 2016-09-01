/************************************************************************************************************************
 Copyright (c) 2016, Imagination Technologies Limited and/or its affiliated group companies.
 All rights reserved.
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 following conditions are met:
 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the
 following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 following disclaimer in the documentation and/or other materials provided with the distribution.
 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 products derived from this software without specific prior written permission.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ************************************************************************************************************************/
#include <letmecreate/letmecreate.h>
#include <math.h>

int rl_values[2] = {5000, 5000};

uint8_t co_click_init(uint8_t mikrobus_index, int rl) {

  if (rl > 10000) {
    fprintf(stderr, "co: Invalid resistance value. Maximum value is 10 000 Ohms");
    return -1;
  }
  switch (mikrobus_index) {
    case MIKROBUS_1:
        rl_values[MIKROBUS_1_INT] = rl;
        break;
#ifndef CONTIKI
    case MIKROBUS_2:
        rl_values[MIKROBUS_2_INT] = rl;
        break;
#endif
    default:
        fprintf(stderr, "co: Invalid mikrobus index.\n");
        return -1;
    }
}

uint8_t co_click_read_ppm(uint8_t mikrobus_index, float* value) {

    switch (mikrobus_index) {
    case MIKROBUS_1:
        break;
#ifndef CONTIKI
    case MIKROBUS_2:
        break;
#endif
    default:
        fprintf(stderr, "co: Invalid mikrobus index.\n");
        return -1;
    }
      
    float Vrl;
    if (adc_get_value(mikrobus_index, &Vrl) < 0) {
        return -1;
    }

    const double Rl = 5000.0;
    double Rs = Rl * (5 - Vrl) / Vrl;
    double ratio = Rs / Rl;
    double lgPPM = (log10(ratio) * -3.7) + 0.9948;
    double ppm = pow(10, lgPPM);
    *value = (float)ppm;
    return 0;
}


