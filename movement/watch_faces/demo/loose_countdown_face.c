mkdir -p movement/watch_faces/demo
cat > movement/watch_faces/demo/loose_countdown_face.c <<'C'
/*
 * Loose 50-year countdown face for Sensor Watch
 * - Ignores leap days (intentionally “loose”)
 * - Shows “YYy DDDd” (e.g., 48y 210d)
 * - Persists remaining days; decrements once every 86,400 ticks
 * - Swallows MODE so you can't leave the face
 */

#include "movement.h"
#include "watch.h"
#include <stdlib.h>
#include <stdio.h>

#define TOTAL_DAYS    18250UL   /* 50 * 365 */
#define TICKS_PER_DAY 86400UL   /* 1 tick/sec, 86400 sec/day */

typedef struct {
    uint32_t days_remaining;
    uint32_t tick_accum;            /* seconds since last day decrement */
} loose_countdown_state_t;

/* persistence helpers */
static void _save(persistent_storage_t *ps, loose_countdown_state_t *st) {
    movement_store_single(ps, 0, (uint8_t *)st, sizeof(*st));
}
static bool _load(persistent_storage_t *ps, loose_countdown_state_t *st) {
    size_t sz = sizeof(*st);
    return movement_load_single(ps, 0, (uint8_t *)st, &sz) && sz == sizeof(*st);
}

/* render YYy DDDd */
static void _render(loose_countdown_state_t *st) {
    uint32_t yrs  = st->days_remaining / 365UL;
    uint32_t days = st->days_remaining % 365UL;
    char buf[12];
    snprintf(buf, sizeof(buf), "%2luy %03lud",
             (unsigned long)yrs, (unsigned long)days);
    watch_display_string(buf, 0);
}

/* face hooks */
bool loose_countdown_face_setup(movement_settings_t *settings,
                                uint8_t watch_face_index,
                                void **context_out) {
    (void)watch_face_index;
    loose_countdown_state_t *st = (loose_countdown_state_t *)malloc(sizeof(*st));
    if (!st) return false;

    persistent_storage_t *ps = movement_get_persistent_storage(settings);
    if (!_load(ps, st)) {
        st->days_remaining = TOTAL_DAYS;
        st->tick_accum     = 0;
        _save(ps, st);
    }
    *context_out = st;
    return true;
}

void loose_countdown_face_activate(movement_settings_t *settings, void *context) {
    (void)settings;
    _render((loose_countdown_state_t *)context);
}

bool loose_countdown_face_loop(movement_event_t event,
                               movement_settings_t *settings,
                               void *context) {
    (void)settings;
    loose_countdown_state_t *st = (loose_countdown_state_t *)context;

    switch (event.event_type) {
        case EVENT_ACTIVATE:
            _render(st);
            return true;

        case EVENT_TICK:
            if (st->days_remaining > 0) {
                st->tick_accum++;
                if (st->tick_accum >= TICKS_PER_DAY) {
                    st->tick_accum -= TICKS_PER_DAY;
                    st->days_remaining--;
                    /* save only when the day rolls */
                    persistent_storage_t *ps =
                        movement_get_persistent_storage(settings);
                    _save(ps, st);
                }
            }
            _render(st);
            return true;

        /* swallow MODE so you can't leave this face */
        case EVENT_MODE_BUTTON_UP:
        case EVENT_MODE_LONG_PRESS:
            return true;

        default:
            /* let defaults handle light, alarm, etc. */
            return false;
    }
}

void loose_countdown_face_resign(movement_settings_t *settings, void *context) {
    (void)settings;
    if (context) {
        persistent_storage_t *ps = movement_get_persistent_storage(settings);
        _save(ps, (loose_countdown_state_t *)context);
        free(context);
    }
}

/* exported descriptor */
watch_face_t loose_countdown_face = {
    loose_countdown_face_setup,
    loose_countdown_face_activate,
    loose_countdown_face_loop,
    loose_countdown_face_resign,
    0, /* wants_background_task */
};
C
