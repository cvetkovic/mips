PREFIX=C:\Users\cl160127d\Desktop\GNU MCU Eclipse\ARM Embedded GCC\8.3.1-1.1\bin

AS=$(PREFIX)/arm-none-eabi-as.exe
LD=$(PREFIX)/arm-none-eabi-ld.exe
CC=$(PREFIX)/arm-none-eabi-gcc.exe -c
OBJCOPY=$(PREFIX)/arm-none-eabi-objcopy.exe

TARGET=program
BUILD_DIR=BUILD

.PHONY: all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex

SOURCES_C=main.c
SOURCES_S=startup.s

OBJECT_FILES = $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES_S:.s=.o)))
vpath %.s $(sort $(dir $(SOURCES_S)))	

OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES_C:.c=.o)))
vpath %.c $(sort $(dir $(SOURCES_C)))	
											   
LINK_SCRIPT = linker_script.ld

$(BUILD_DIR):
	mkdir $(@)
	
$(BUILD_DIR)/$(TARGET).elf: $(OBJECT_FILES) makefile |  $(BUILD_DIR) 	
	$(LD) -T $(LINK_SCRIPT) -o $(@) $(OBJECT_FILES)  
				
MCU = -mcpu=cortex-m3 -mthumb 

$(BUILD_DIR)/%.o: %.s makefile | $(BUILD_DIR) 		
	$(AS) $(MCU) -o $(@) $(<) 
	
$(BUILD_DIR)/%.o: %.c makefile | $(BUILD_DIR) 		
	$(CC) $(MCU) -o $(@) $(<)
	
$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf makefile |  $(BUILD_DIR) 	
	$(OBJCOPY) --output-target ihex $(<) $(@) 