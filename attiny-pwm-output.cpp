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
  State* state = getState(ctx);
  const int port = ::floorFixed(getValue<input_PORT>(ctx)).getInteger();
  if (port != state->configuredPort) {
    ::pinMode(port, OUTPUT);
    // Store configured port so to avoid repeating `pinMode` call if just
    // SIG is updated
    state->configuredPort = port;
  }

  auto duty = getValue<input_DUTY>(ctx);
  duty = duty > Number(1, 0) ? Number(1, 0)
                             : (duty < Number(0, 0)) ? Number(0, 0) : duty;

  Number frac =
      Number(0, (duty.getFraction() / (double)(Number(0, ~0).getInternal())));

  uint8_t val =
      frac.getFraction() &
      (~0ULL & (~0ULL << ((sizeof(unsigned long long) * CHAR_BIT) - CHAR_BIT)));

  ::analogWrite(port, val);
}
