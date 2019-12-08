C:\Users\cl160127d\Desktop\"GNU MCU Eclipse"\"ARM Embedded GCC"\8.3.1-1.1\bin\arm-none-eabi-as.exe -mcpu=cortex-m3 -mthumb -o startup.o startup.s
C:\Users\cl160127d\Desktop\"GNU MCU Eclipse"\"ARM Embedded GCC"\8.3.1-1.1\bin\arm-none-eabi-gcc.exe -c -mcpu=cortex-m3 -mthumb -o main.o main.c
C:\Users\cl160127d\Desktop\"GNU MCU Eclipse"\"ARM Embedded GCC"\8.3.1-1.1\bin\arm-none-eabi-ld.exe -T linker_script.ld -o program.elf main.o startup.o
C:\Users\cl160127d\Desktop\"GNU MCU Eclipse"\"ARM Embedded GCC"\8.3.1-1.1\bin\arm-none-eabi-objcopy.exe --output-target ihex program.elf program.hex 