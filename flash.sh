#bin/bash

/home/MrTan/Documents/ch32_toolchain/OpenOCD/OpenOCD/bin/openocd -f build/ch32v307-ninja.bin -c init -c halt -c "program os.bin"  -c exit