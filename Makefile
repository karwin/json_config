include config.mk
#include helper.mk

C_SRC_DIR = src
PROJEC_NAME = $(notdir $(PWD))

PKG_NAME = $(PROJEC_NAME).pkg.tar.bz2

C_SRC_FILES = $(shell find  $(C_SRC_DIR) -name '*.c')
C_INCLUDE_DIRS = $(shell find  $(C_SRC_DIR) -name include -type d)
C_INCLUDE_FILE_PATTERN = $(patsubst %/include,%/include/%.h, $(C_INCLUDE_DIRS) )
ifeq ($(C_INCLUDE_DIRS),)
C_INCLUDE_FILES = 
else
C_INCLUDE_FILES = $(shell find $(C_INCLUDE_DIRS) -name '*.h')
endif


INCLUDE_DIR = ./include

#C_SRC_FILE = $(shell find  Robot/ Matg/ -name '*.c')
CFLAGS +=  -I$(INCLUDE_DIR) -std=gnu99 -Wall -Wno-unused-label # -fexceptions  -fPIC -Wno-unused-but-set-variable -Wno-unused-function -Wno-unused-variable
        
#LDFLAGS +=  -Wl,--gc-sections -Wl,--as-needed  -Wl,--start-group -Wl,--end-group -lm

UNEED_FILES_C = 

TARGET_C_SRC = $(filter-out $(UNEED_FILES_C),$(C_SRC_FILES))

LIB_H     = $(patsubst %.h,$(INCLUDE_DIR)/%.h, $(notdir $(C_INCLUDE_FILES)))

OBJS_DIR = ./obj

LIB_OBJS = ${TARGET_C_SRC:src/%.c=$(OBJS_DIR)/%.o}
#LIB_OBJS_2 = ${C_SRC_FILES:%.c=$(OBJS_DIR)/%.o}


LIB = $(PROJEC_NAME)

TARGET = $(LIB) 
all: $(TARGET)

$(LIB_H):$(C_INCLUDE_FILES)
	@mkdir -p $(INCLUDE_DIR)
	cp $(shell find $^ -name $(notdir $@)) $@

$(OBJS_DIR)/%.o:$(C_SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<
	
$(OBJS_DIR)/%.o:$(C_SRC_DIR)/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJS_DIR)/%.o:$(C_SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c -o $@ $<
	
#$(TARGET):$(LIB_H) $(LIB_OBJS) 
#	$(CC) -shared $(CFLAGS)  $(LDFLAGS) -o  $@ $^
$(TARGET):$(LIB_H) $(LIB_OBJS)
	$(CC) -o  $@ $^ $(CFLAGS) $(LDFLAGS) 
	
clean:
	rm -fr $(TARGET)  $(LIB) $(OBJS_DIR)

$(info $(prefix))

INSTALL_PATH = $(PWD)/install/root/usr/

install:
	#mkdir -p $(INSTALL_PATH)/{lib/$(PROJEC_NAME),include/$(PROJEC_NAME)}
	mkdir -p $(INSTALL_PATH)/lib/$(PROJEC_NAME)
	mkdir -p $(INSTALL_PATH)/include/$(PROJEC_NAME)
	cp -af $(TARGET)  $(INSTALL_PATH)/lib/$(PROJEC_NAME)
	cp -af $(LIB_H)   $(INSTALL_PATH)/include/$(PROJEC_NAME)

uninstall:
	rm -rf $(INSTALL_PATH)/*
	
distclean:
	rm config.mk
	rm -rf  $(TARGET) *.o

PKG:
	-rm $(PKG_NAME)
	cd install/root && tar jcf ../../$(PKG_NAME) *

upload: PKG
	nc -l -p 10000 < $(PKG_NAME)

trans:
	adb push $(PROJEC_NAME).so /data/medusa
	
.PHONY : all clean install uninstall distclean $(LIB)
