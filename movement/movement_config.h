/*
 * Minimal movement_config.h for a single-face firmware
 * (Loose 50-year countdown face only)
 */

#ifndef MOVEMENT_CONFIG_H_
#define MOVEMENT_CONFIG_H_

#include "movement_faces.h"

/* The only face in the firmware */
static const watch_face_t watch_faces[] = {
    loose_countdown_face,
};

/* Required indices/macros */
#define MOVEMENT_NUM_FACES            (sizeof(watch_faces) / sizeof(watch_face_t))
#define MOVEMENT_SECONDARY_FACE_INDEX 0

#endif /* MOVEMENT_CONFIG_H_ */
