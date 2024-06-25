#!/bin/bash

set -e # Exit on error

export PICO_SDK_PATH="$(cd ../../pico-sdk/; pwd)"
export PATH="$(cd ../../system/arm-none-eabi/bin; pwd):$PATH"

rm -rf build
mkdir build
cd build
cmake .. -DPICO_PLATFORM=rp2350
make -j V=1

# Put everything in its place
mv generated/pico_base/pico/version.h ../../../include/pico_base/pico/.
cp ../lwipopts.h ../../../include/.
cp ../tusb_config.h ../../../include/.
cp ../btstack_config.h ../../../include/.

rm -rf boot
mkdir boot
cd boot
mkdir -p pico 
touch pico/config.h
for type in boot2_generic_03h boot2_w25q080; do
    for div in 2 4; do
        arm-none-eabi-gcc -mcpu=cortex-m33 -mthumb -march=armv8-m.main+fp+dsp -mfloat-abi=softfp -O3 \
                         -DNDEBUG -DPICO_FLASH_SPI_CLKDIV=$div \
                         -c "$PICO_SDK_PATH/src/rp2350/boot_stage2/$type.S" \
                         -I "$PICO_SDK_PATH/src/boards/include/boards/" \
                         -I "$PICO_SDK_PATH/src/rp2350/hardware_regs/include/" \
                         -I "$PICO_SDK_PATH/src/rp2350/pico_platform/include/" \
                         -I "$PICO_SDK_PATH/src/rp2350/boot_stage2/asminclude/" \
                         -I .

        arm-none-eabi-gcc -mcpu=cortex-m33 -mthumb -march=armv8-m.main+fp+dsp -mfloat-abi=softfp -O3 \
                          -DNDEBUG -Wl,--build-id=none --specs=nosys.specs -nostartfiles \
                          -Wl,--script="$PICO_SDK_PATH/src/rp2350/boot_stage2/boot_stage2.ld" \
                          -Wl,-Map=$type.$div.elf.map $type.o -o $type.$div.elf 

        arm-none-eabi-objdump -h $type.$div.elf >  $type.$div.dis
        arm-none-eabi-objdump -d $type.$div.elf >> $type.$div.dis

        arm-none-eabi-objcopy -Obinary $type.$div.elf $type.$div.bin

        python3 "$PICO_SDK_PATH/src/rp2350/boot_stage2/pad_checksum" \
                   -s 0xffffffff $type.$div.bin ${type}_${div}_padded_checksum.S
    done
done
mv *.S ../../../../boot2/rp2350/.

# Build for RP2040
cd ../..
rm -rf build
mkdir build
cd build
cmake .. -DPICO_PLATFORM=rp2040
make -j V=1

rm -rf boot
mkdir boot
cd boot
mkdir -p pico 
touch pico/config.h
for type in boot2_generic_03h boot2_is25lp080 boot2_w25q080 boot2_w25x10cl; do
    for div in 2 4; do
        arm-none-eabi-gcc -march=armv6-m -mcpu=cortex-m0plus -mthumb -O3 \
                         -DNDEBUG -DPICO_FLASH_SPI_CLKDIV=$div \
                         -c "$PICO_SDK_PATH/src/rp2040/boot_stage2/$type.S" \
                         -I "$PICO_SDK_PATH/src/boards/include/boards/" \
                         -I "$PICO_SDK_PATH/src/rp2040/hardware_regs/include/" \
                         -I "$PICO_SDK_PATH/src/rp2040/pico_platform/include/" \
                         -I "$PICO_SDK_PATH/src/rp2040/boot_stage2/asminclude/" \
                         -I .

        arm-none-eabi-gcc -march=armv6-m -mcpu=cortex-m0plus -mthumb -O3 \
                          -DNDEBUG -Wl,--build-id=none --specs=nosys.specs -nostartfiles \
                          -Wl,--script="$PICO_SDK_PATH/src/rp2040/boot_stage2/boot_stage2.ld" \
                          -Wl,-Map=$type.$div.elf.map $type.o -o $type.$div.elf 

        arm-none-eabi-objdump -h $type.$div.elf >  $type.$div.dis
        arm-none-eabi-objdump -d $type.$div.elf >> $type.$div.dis

        arm-none-eabi-objcopy -Obinary $type.$div.elf $type.$div.bin

        python3 "$PICO_SDK_PATH/src/rp2040/boot_stage2/pad_checksum" \
                   -s 0xffffffff $type.$div.bin ${type}_${div}_padded_checksum.S
    done
done
mv *.S ../../../../boot2/rp2040/.
