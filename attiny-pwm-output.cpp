{{#global}}
#include <FixedPoints.h>
#include <FixedPointsCommon.h>
{{/global}}

struct State {
  int configuredPort = -1;
};

{{ GENERATED_CODE }}

void evaluate(Context ctx) {
  State* state = getState(ctx);
  const int port = ::floorFixed(getValue<input_PORT>(ctx)).getInteger();
  if (port != state->configuredPort) {
    ::pinMode(port, OUTPUT);
    // Store configured port so to avoid repeating `pinMode` call if just
    // SIG is updated
    state->configuredPort = port;
  }
  
  //check bounds for duty cycle input

  auto duty = getValue<input_DUTY>(ctx);
  duty = duty >= Number(1, 0) ? Number(0.99)
                             : (duty < Number(0, 0)) ? Number(0.) : duty;
  
  //Pick off most significant byte of fixed point fraction and
  //send to analogWrite taking uchar
  
  auto val_convert = (SQ7x8)(duty);
  uint8_t val = val_convert.getFraction();
  ::analogWrite(port, val);
}
