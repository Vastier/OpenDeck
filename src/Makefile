TARGETNAME := fw_opendeck

ifneq ($(findstring $(TARGETNAME), $(shell cat targets.txt)), $(TARGETNAME))
    $(error Target doesn't exist)
endif

TARGET := build/$(TARGETNAME)
.DEFAULT_GOAL := $(TARGET).elf

#includes
#important - do not change the order of inclusion!
include Defines.mk
include Sources.mk

#specify c/c++ compilers
CPP := avr-gcc
CXX := avr-g++

#common compiler flags
COMMON_FLAGS := \
-funsigned-char \
-funsigned-bitfields \
-fdata-sections \
-ffunction-sections \
-fshort-enums \
-mrelax \
-Wall \
-c \
-fno-jump-tables \
-fno-strict-aliasing

#c only
CFLAGS := \
-fpack-struct

#cpp only
CPPFLAGS := \
-std=c++11 \
-fno-exceptions

#common linker flags
LDFLAGS := -Wl,--gc-sections -mrelax -Wl,--start-group -Wl,-lm -Wl,--end-group

#optimisation level
OPT := -Os

ifneq ($(filter boot_%, $(TARGETNAME)), )
    #make sure to link .text at correct address in bootloader
    LDFLAGS += -Wl,--section-start=.text=$(BOOT_START_ADDR)
endif

#use link time optimisations only on opendeck board due to the heavy memory constrains
ifeq ($(TARGETNAME), fw_opendeck)
    COMMON_FLAGS += -flto
    LDFLAGS += -flto
endif

LEN_APPEND := 0

#custom linker script
LDFLAGS += -T application/board/avr/linker/$(MCU).x

#append length only in firmware
ifeq ($(findstring fw,$(TARGETNAME)), fw)
    LEN_APPEND := 1
endif

#use windows binary on wsl since HID access isn't possible in wsl
ifeq ($(findstring Microsoft,$(shell uname -r)), Microsoft)
    UPLOAD_BIN := @cmd.exe /c "..\bin\dfu\hid_bootloader_loader_win.exe"
else ifeq ($(shell uname), Linux)
    UPLOAD_BIN := @ ../bin/dfu/hid_bootloader_loader_linux
else ifeq ($(shell uname), Darwin)
    UPLOAD_BIN := @ ../bin/dfu/hid_bootloader_loader_mac
else
    $(error Unsupported platform)
endif

build/%.o: %.c
	@mkdir -p $(@D)
	@$(CPP) $(COMMON_FLAGS) $(CFLAGS) $(addprefix -D,$(DEFINES)) $(OPT) -mmcu=$(MCU) $(INCLUDE_DIRS) -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -c "$<" -o "$@"
	@echo Finished building: $<

build/%.o: %.cpp
	@mkdir -p $(@D)
	@$(CXX) $(COMMON_FLAGS) $(CPPFLAGS) $(addprefix -D,$(DEFINES)) $(OPT) -mmcu=$(MCU) $(INCLUDE_FILES) $(INCLUDE_DIRS) -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -c "$<" -o "$@"
	@echo Finished building: $<

$(TARGET).elf: $(OBJECTS)
	@#create a file with contents being mcu name (for upload process)
	@echo $(MCU) > build/MCU
	@#create a file with contents being target name (for upload process)
	@echo $(TARGETNAME) > build/TARGET
	@#link
	@$(CXX) -o$(TARGET).elf $(OBJECTS) -mmcu=$(MCU) $(LDFLAGS) -Wl,-Map,$(TARGET).map
	@echo Finished building target: $@
	@#convert elf to hex
	@avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "$(TARGET).elf" "$(TARGET).hex"
	@#write firmware length at specified location and crc at the end of compiled binary if supported for target
	@if [ "$(LEN_APPEND)" = "1" ]; then\
		srec_cat $(TARGET).hex -Intel -exclude $(FLASH_SIZE_START_ADDR) $(FLASH_SIZE_END_ADDR) -Little_Endian_Maximum $(FLASH_SIZE_START_ADDR) -fill 0xff -over $(TARGET).hex -I -Output $(TARGET).hex -Intel;\
		srec_cat $(TARGET).hex -Intel -Little_Endian_CRC16 -max-address $(TARGET).hex -Intel -Cyclic_Redundancy_Check_16_XMODEM -Output $(TARGET).hex -Intel;\
	fi
	@#convert hex to bin
	@avr-objcopy -I ihex "$(TARGET).hex" -O binary "$(TARGET).bin"
	@#display memory usage
	@avr-size -C --mcu=$(MCU) "$(TARGET).elf"

#firmware upload with avrdude
upload:
	@avrdude -p $(shell cat build/MCU) -P /dev/$(PORT) -b 19200 -c avrisp -e -V -U lock:w:$(FUSE_UNLOCK):m -U efuse:w:$(FUSE_EXT):m -U hfuse:w:$(FUSE_HIGH):m -U lfuse:w:$(FUSE_LOW):m
	@avrdude -p $(shell cat build/MCU) -P /dev/$(PORT) -b 19200 -c avrisp -U flash:w:build/$(shell cat build/TARGET).hex
	@avrdude -p $(shell cat build/MCU) -P /dev/$(PORT) -b 19200 -c avrisp -V -U lock:w:$(FUSE_LOCK):m

#firmware update with bootloader
uploadboot:
	$(UPLOAD_BIN) $(shell cat build/MCU) build/$(shell cat build/TARGET).hex

#lists all serial ports - used to find out where the programmer is connected
ser_list:
	dmesg | grep tty

#other targets
clean:
	@echo Cleaning up.
	@rm -rf build/ ../Documentation/

#debugging
print-%:
	@echo '$*=$($*)'
