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

  const bool val = getValue<input_SIG>(ctx);
  ::digitalWrite(port, val);
}
