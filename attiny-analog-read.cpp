{
  { #global }
}
#include <FixedPoints.h>
#include <FixedPointsCommon.h>
{
  { / global }
}

struct State {
  int configuredPort = -1;
};

{
  { GENERATED_CODE }
}

void evaluate(Context ctx) {
  if (!isInputDirty<input_UPD>(ctx)) return;

  State* state = getState(ctx);
  const int port = (int)getValue<input_PORT>(ctx).getInteger();
  if (port != state->configuredPort) {
    ::pinMode(port, INPUT);
    // Store configured port so to avoid repeating `pinMode` on
    // subsequent requests
    state->configuredPort = port;
  }

  getValue<input_REF>(ctx) ? analogReference(INTERNAL)
                           : analogReference(DEFAULT);

  emitValue<output_VAL>(ctx, Number(::analogRead(port) / 1023.));
}
