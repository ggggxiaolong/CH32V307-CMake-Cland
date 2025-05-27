#bin/bash

/home/MrTan/Documents/ch32_toolchain/MRS_Toolchain_Linux_x64_V210/OpenOCD/OpenOCD/bin -f build/ch32v307-ninja.bin -c init -c halt -c "program os.bin"  -c exit