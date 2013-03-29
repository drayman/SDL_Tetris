LOCAL_PATH := $(call my-dir)
CUR_PATH := $(LOCAL_PATH)
LIB_PATH := $(LOCAL_PATH)/../lib

include $(CLEAR_VARS)
LOCAL_MODULE:= SDL2
LOCAL_PATH = $(LIB_PATH)
LOCAL_SRC_FILES := libSDL2.so
include $(PREBUILT_SHARED_LIBRARY)

LOCAL_PATH := $(CUR_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE := main
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Source \
                    $(LOCAL_PATH)/../include

LOCAL_CPPFLAGS   := -std=c++0x -fexceptions -DDEBUG_MODE

LOCAL_SRC_FILES := SDL_android_main.cpp                            \
                   ../../Source/texture/texture.cpp                \
                   ../../Source/texture/texture_manager.cpp        \
                   ../../Source/texture/BMP_texture.cpp            \
                   ../../Source/texture/TGA_texture.cpp            \
                   ../../Source/texture/font.cpp                   \
                   ../../Source/system/display.cpp                 \
		           ../../Source/system/projection.cpp              \
			       ../../Source/animation/animation.cpp            \
				   ../../Source/animation/alpha_animation.cpp      \
			       ../../Source/animation/rotate_animation.cpp     \
				   ../../Source/animation/translate_animation.cpp  \
		           ../../Source/game/key_map.cpp                   \
		           ../../Source/game/table.cpp                     \
				   ../../Source/game/tetromino.cpp                 \
				   ../../Source/game/tetris.cpp                    \
		           ../../Source/mesh/mesh.cpp                      \
		           ../../Source/main.cpp

LOCAL_SHARED_LIBRARIES = SDL2

LOCAL_LDLIBS := -lGLESv1_CM -llog

include $(BUILD_SHARED_LIBRARY)
