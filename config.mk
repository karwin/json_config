ifneq ($(CONFIG_INCLUDED), yes)
CONFIG_INCLUDED=yes

WORKSPACE_DIR=/home/karwin/share/test/Medusa

CC=gcc
CXX=-g++
AR=ar
RANLIB=ranlib
NM=nm

CFLAGS += -I$(WORKSPACE_DIR)/pc/libchunfeng-master/include
LDFLAGS += -L/home/karwin/test/audio_service/lib -lchunfeng
LDFLAGS += -lcrypt -lpthread -lm

CFLAGS += -fgnu89-inline





endif
