# Leave this line at the top of the file; it has all the watch library sources and includes.
TOP = ../..
include $(TOP)/make.mk

# ---- Includes (safe to keep broad; costs nothing) ----
INCLUDES += \
  -I../ \
  -I../watch_faces/ \
  -I../watch_faces/clock/ \
  -I../watch_faces/settings/ \
  -I../watch_faces/complication/ \
  -I../watch_faces/sensor/ \
  -I../watch_faces/demo/ \
  -I../../littlefs/

# ---- Sources (minimal, single-face firmware) ----
# Core firmware + filesystem + shell
SRCS += \
  ../../littlefs/lfs.c \
  ../../littlefs/lfs_util.c \
  ../movement.c \
  ../filesystem.c \
  ../shell.c \
  ../shell_cmd_list.c

# Our ONLY watch face
SRCS += \
  ../watch_faces/demo/loose_countdown_face.c

# Leave this line at the bottom of the file; it has all the targets for making your project.
include $(TOP)/rules.mk
