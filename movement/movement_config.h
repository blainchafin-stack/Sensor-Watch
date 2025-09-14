# backup
cp movement/movement_config.h movement/movement_config.h.bak

# ensure the faces header is included (safe if duplicated)
grep -q 'movement_faces.h' movement/movement_config.h || \
sed -i '1i #include "movement_faces.h"' movement/movement_config.h

# replace the faces array with just our face
perl -0777 -i -pe 's/const\s+watch_face_t\s+watch_faces\[\]\s*=\s*\{[^}]*\};/const watch_face_t watch_faces[] = {\n    loose_countdown_face,\n};/s' movement/movement_config.h

# ensure the secondary index is 0 (create if missing)
if grep -q "MOVEMENT_SECONDARY_FACE_INDEX" movement/movement_config.h; then
  sed -i 's/#define\s\+MOVEMENT_SECONDARY_FACE_INDEX\s\+.*/#define MOVEMENT_SECONDARY_FACE_INDEX 0/' movement/movement_config.h
else
  printf '\n#define MOVEMENT_SECONDARY_FACE_INDEX 0\n' >> movement/movement_config.h
fi
