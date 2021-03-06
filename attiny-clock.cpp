{{ #global }}
#include <FixedPoints.h>
#include <FixedPointsCommon.h>
{{ / global }}

struct State {
  TimeMs nextTrig;
};

{{ GENERATED_CODE }}

void evaluate(Context ctx) {
  State* state = getState(ctx);
  TimeMs tNow = transactionTime();
  TimeMs dt = (double)(getValue<input_IVAL>(ctx).getInteger()) * 1000 +
              (getValue<input_IVAL>(ctx).getFraction() /
               (double)(Number(0, ~0).getInternal() + 1)) *
                  1000;

  TimeMs tNext = tNow + dt;

  if (isInputDirty<input_RST>(ctx) || isInputDirty<input_EN>(ctx)) {
    // Handle enable/disable/reset
    if (dt <= 0 || !getValue<input_EN>(ctx)) {
      // Disable timeout loop on zero IVAL or explicit false on EN
      state->nextTrig = 0;
      clearTimeout(ctx);
    } else if (state->nextTrig < tNow || state->nextTrig > tNext) {
      // Start timeout from scratch
      state->nextTrig = tNext;
      setTimeout(ctx, dt);
    }
  }

  if (isTimedOut(ctx)) {
    emitValue<output_TICK>(ctx, 1);
    state->nextTrig = tNext;
    setTimeout(ctx, dt);
  }
}
