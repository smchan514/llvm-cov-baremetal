# llvm-cov 14 on Enumlated ARM 32-bit Bare Metal Platform

Initial prototype to run C code instrumented with llvm-cov version 14.0.0 on an enumlated ARM 32-bit bare metal platform.

## Setup

1. WSL 2.6.1.0
2. "Ubuntu-22.04" disrtibution in WSL
3. Clang version 14.0.0-1ubuntu1.1
    1. Installed with `sudo apt install clang`
4. QEMU version 6.2.0 (Debian 1:6.2+dfsg-2ubuntu6.27)
    1. Installed with `sudo apt install qemu-system-aarch64`
5. Arm GNU Toolchain:
    1. Download `arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi.tar.xz` under *x86_64 Linux hosted cross toolchains*, *AArch32 bare-metal target (arm-none-eabi)* from https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads
    2. Extract archive and install simlinks:
        ```
        sudo tar xJf /mnt/c/Users/smchan/Downloads/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi.tar.xz -C /opt/arm
        cd /usr/bin
        sudo ln -s /opt/arm/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-as
        sudo ln -s /opt/arm/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-ld
        sudo ln -s /opt/arm/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-gcc
        sudo ln -s /opt/arm/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-g++
        sudo ln -s /opt/arm/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-objdump
        sudo ln -s /opt/arm/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-objcopy
        ```

## Results

1. Build sample and generate `test.bin`:
    ```shell
    make clean
    make    
    ```
2. Run `test.bin` in QEMU:
    ```shell
    qemu-system-arm -M versatilepb -m 128M -nographic -audiodev none,id=none -kernel test.bin
    ```
3. Exit QEMU with `CTRL+A X`.

### Sample Output

```
[llvm_gcov_init]
[llvm_gcov_init] wfn not null
[__llvmcov_add]
[llvm_gcov_init] rfn not null
[__llvmcov_add]
[llvm_gcov_init]
[llvm_gcov_init] wfn not null
[__llvmcov_add]
[llvm_gcov_init] rfn not null
[__llvmcov_add]
[llvm_gcov_init]
[llvm_gcov_init] wfn not null
[__llvmcov_add]
[llvm_gcov_init] rfn not null
[__llvmcov_add]
Hello world!
func1
func2
func1
func2
func1
func2
func1
func2
func1
func2
[llvmcov_writeout]
[__llvmcov_write_all]
[llvm_gcda_start_file]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_summary_info]
[llvm_gcda_end_file]
[llvm_gcda_start_file]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_summary_info]
[llvm_gcda_end_file]
[llvm_gcda_start_file]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_emit_function]
[llvm_gcda_emit_arcs]
[llvm_gcda_summary_info]
[llvm_gcda_end_file]
2F 68 6F 6D 65 2F 73 6D 63 68 61 6E 2F 67 69 74  | /home/smchan/git
2F 6C 6C 76 6D 2D 63 6F 76 2D 62 61 72 65 6D 65  | /llvm-cov-bareme
74 61 6C 2F 73 61 6D 70 6C 65 73 2F 6C 6C 76 6D  | tal/samples/llvm
2D 63 6F 76 2D 31 34 2D 61 72 6D 33 32 2F 65 6E  | -cov-14-arm32/en
74 72 79 2E 67 63 64 61 00 A4 00 00 00 61 64 63  | try.gcda.....adc
67 2A 38 30 34 45 4E 8A FA 00 00 00 01 03 00 00  | g*804EN.........
00 00 00 00 00 A9 68 CD BD 45 4E 8A FA 00 00 A1  | ......h..EN.....
01 04 00 00 00 1F 00 00 00 00 00 00 00 00 02 00  | ................
00 00 00 00 00 00 00 00 01 03 00 00 00 01 00 00  | ................
00 A0 3D E7 D3 45 4E 8A FA 00 00 A1 01 02 00 00  | ..=..EN.........
00 01 00 00 00 00 00 00 00 00 00 00 01 03 00 00  | ................
00 02 00 00 00 09 18 B7 E3 45 4E 8A FA 00 00 A1  | .........EN.....
01 04 00 00 00 01 00 00 00 00 00 00 00 03 00 00  | ................
00 00 00 00 00 00 00 00 A3 03 00 00 00 00 00 00  | ................
00 00 00 00 00 01 00 00 00 00 00 00 00 00 00 00  | ................
00 2F 68 6F 6D 65 2F 73 6D 63 68 61 6E 2F 67 69  | ./home/smchan/gi
74 2F 6C 6C 76 6D 2D 63 6F 76 2D 62 61 72 65 6D  | t/llvm-cov-barem
65 74 61 6C 2F 73 61 6D 70 6C 65 73 2F 6C 6C 76  | etal/samples/llv
6D 2D 63 6F 76 2D 31 34 2D 61 72 6D 33 32 2F 74  | m-cov-14-arm32/t
65 73 74 2E 67 63 64 61 00 A4 00 00 00 61 64 63  | est.gcda.....adc
67 2A 38 30 34 3F 45 3B 11 00 00 00 01 03 00 00  | g*804?E;........
00 00 00 00 00 70 53 74 8D 3F 45 3B 11 00 00 A1  | .....pSt.?E;....
01 06 00 00 00 01 00 00 00 00 00 00 00 05 00 00  | ................
00 00 00 00 00 05 00 00 00 00 00 00 00 00 00 00  | ................
01 03 00 00 00 01 00 00 00 49 DA EE AD 3F 45 3B  | .........I...?E;
11 00 00 A1 01 02 00 00 00 05 00 00 00 00 00 00  | ................
00 00 00 00 01 03 00 00 00 02 00 00 00 10 62 8C  | ..............b.
62 3F 45 3B 11 00 00 A1 01 02 00 00 00 05 00 00  | b?E;............
00 00 00 00 00 00 00 00 A3 03 00 00 00 00 00 00  | ................
00 00 00 00 00 01 00 00 00 00 00 00 00 00 00 00  | ................
00 2F 68 6F 6D 65 2F 73 6D 63 68 61 6E 2F 67 69  | ./home/smchan/gi
74 2F 6C 6C 76 6D 2D 63 6F 76 2D 62 61 72 65 6D  | t/llvm-cov-barem
65 74 61 6C 2F 73 61 6D 70 6C 65 73 2F 6C 6C 76  | etal/samples/llv
6D 2D 63 6F 76 2D 31 34 2D 61 72 6D 33 32 2F 6C  | m-cov-14-arm32/l
6C 76 6D 2D 63 6F 76 2E 67 63 64 61 00 14 04 00  | lvm-cov.gcda....
00 61 64 63 67 2A 38 30 34 64 08 82 DB 00 00 00  | .adcg*804d......
01 03 00 00 00 00 00 00 00 1D 66 CD D6 64 08 82  | ..........f..d..
DB 00 00 A1 01 06 00 00 00 03 00 00 00 00 00 00  | ................
00 03 00 00 00 00 00 00 00 03 00 00 00 00 00 00  | ................
00 00 00 00 01 03 00 00 00 01 00 00 00 B2 17 99  | ................
74 64 08 82 DB 00 00 A1 01 08 00 00 00 06 00 00  | td..............
00 00 00 00 00 06 00 00 00 00 00 00 00 06 00 00  | ................
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  | ................
01 03 00 00 00 02 00 00 00 62 92 D9 41 64 08 82  | .........b..Ad..
DB 00 00 A1 01 04 00 00 00 00 00 00 00 00 00 00  | ................
00 03 00 00 00 00 00 00 00 00 00 00 01 03 00 00  | ................
00 03 00 00 00 DA 71 ED 57 64 08 82 DB 00 00 A1  | ......q.Wd......
01 02 00 00 00 03 00 00 00 00 00 00 00 00 00 00  | ................
01 03 00 00 00 04 00 00 00 77 36 74 7A 64 08 82  | .........w6tzd..
DB 00 00 A1 01 02 00 00 00 63 00 00 00 00 00 00  | .........c......
00 00 00 00 01 03 00 00 00 05 00 00 00 A6 E1 01  | ................
3F 64 08 82 DB 00 00 A1 01 02 00 00 00 02 00 00  | ?d..............
00 00 00 00 00 00 00 00 01 03 00 00 00 06 00 00  | ................
00 5E 21 D4 67 64 08 82 DB 00 00 A1 01 02 00 00  | .^!.gd..........
00 18 00 00 00 00 00 00 00 00 00 00 01 03 00 00  | ................
00 07 00 00 00 24 DA F6 6F 64 08 82 DB 00 00 A1  | .....$..od......
01 02 00 00 00 02 00 00 00 00 00 00 00 00 00 00  | ................
01 03 00 00 00 08 00 00 00 71 F9 DF E9 64 08 82  | .........q...d..
DB 00 00 A1 01 06 00 00 00 0F 00 00 00 00 00 00  | ................
00 0F 00 00 00 00 00 00 00 0F 00 00 00 00 00 00  | ................
00 00 00 00 01 03 00 00 00 09 00 00 00 AC 06 08  | ................
D0 64 08 82 DB 00 00 A1 01 04 00 00 00 10 00 00  | .d..............
00 00 00 00 00 1C 00 00 00 00 00 00 00 00 00 00  | ................
01 03 00 00 00 0A 00 00 00 03 D0 F1 35 64 08 82  | ............5d..
DB 00 00 A1 01 04 00 00 00 00 00 00 00 00 00 00  | ................
00 02 00 00 00 00 00 00 00 00 00 00 01 03 00 00  | ................
00 0B 00 00 00 66 05 EA 9C 64 08 82 DB 00 00 A1  | .....f...d......
01 04 00 00 00 00 00 00 00 00 00 00 00 00 00 00  | ................
00 00 00 00 00 00 00 00 01 03 00 00 00 0C 00 00  | ................
00 F7 B8 33 43 64 08 82 DB 00 00 A1 01 02 00 00  | ...3Cd..........
00 01 00 00 00 00 00 00 00 00 00 00 01 03 00 00  | ................
00 0D 00 00 00 97 26 32 E3 64 08 82 DB 00 00 A1  | ......&2.d......
01 06 00 00 00 01 00 00 00 00 00 00 00 00 00 00  | ................
00 00 00 00 00 02 00 00 00 00 00 00 00 00 00 00  | ................
01 03 00 00 00 0E 00 00 00 FC 59 A1 C5 64 08 82  | ..........Y..d..
DB 00 00 A1 01 18 00 00 00 00 00 00 00 00 00 00  | ................
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  | ................
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  | ................
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  | ................
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  | ................
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  | ................
00 00 00 00 00 00 00 00 00 00 00 00 01 03 00 00  | ................
00 0F 00 00 00 C6 4C B5 B3 64 08 82 DB 00 00 A1  | ......L..d......
01 06 00 00 00 00 00 00 00 00 00 00 00 00 00 00  | ................
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  | ................
01 03 00 00 00 10 00 00 00 B6 91 42 46 64 08 82  | ...........BFd..
DB 00 00 A1 01 04 00 00 00 03 00 00 00 00 00 00  | ................
00 DA 00 00 00 00 00 00 00 00 00 00 01 03 00 00  | ................
00 11 00 00 00 9A 5F EE AF 64 08 82 DB 00 00 A1  | ......_..d......
01 02 00 00 00 31 01 00 00 00 00 00 00 00 00 00  | .....1..........
01 03 00 00 00 12 00 00 00 11 1C 64 DF 64 08 82  | ...........d.d..
DB 00 00 A1 01 06 00 00 00 00 00 00 00 00 00 00  | ................
00 3E 01 00 00 00 00 00 00 D1 05 00 00 00 00 00  | .>..............
00 00 00 00 01 03 00 00 00 13 00 00 00 C2 0B 6C  | ...............l
47 64 08 82 DB 00 00 A1 01 02 00 00 00 00 00 00  | Gd..............
00 00 00 00 00 00 00 00 01 03 00 00 00 14 00 00  | ................
00 1A 0E 2C 2F 64 08 82 DB 00 00 A1 01 06 00 00  | ...,/d..........
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  | ................
00 00 00 00 00 00 00 00 00 00 00 00 A3 03 00 00  | ................
00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00  | ................
00 00 00 00 00                                   | .....
[__llvmcov_reset_all]
[__aeabi_memclr]
[__aeabi_memclr]
[__aeabi_memclr]
[__aeabi_memclr]
[__aeabi_memclr]
[__aeabi_memclr]
[__aeabi_memclr]
[__aeabi_memclr]
[__aeabi_memclr]
[__aeabi_memclr]
[__aeabi_memclr]
[__aeabi_memclr]
[__aeabi_memclr]
[__aeabi_memclr]
[__aeabi_memclr]
[__aeabi_memclr]
QEMU: Terminated
```

## References

1. (Hello world for bare metal ARM using QEMU)[https://balau82.wordpress.com/2010/02/28/hello-world-for-bare-metal-arm-using-qemu/]
2. (GNU linker ld (GNU Binutils) doc from Sourceware)[https://www.sourceware.org/binutils/docs/ld/index.html#SEC_Contents]
