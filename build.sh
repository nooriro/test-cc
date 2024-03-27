#!/bin/sh
cd "${0%/*}"

case "$(cat /etc/alpine-release 2>/dev/null)" in
    3.19*) echo "Alpine Linux $(cat /etc/alpine-release)" ;;
    *)     echo "This script requires Alpine Linux 3.19" 1>&2; exit 1 ;;
esac

OPTS1="-fuse-ld=lld -rtlib=compiler-rt"
OPTS2="-ffunction-sections -fdata-sections -Wl,--gc-sections -O2 -DNDEBUG -static -s"

TARGETS="
    armv7|armv7-alpine-linux-musleabihf
    aarch64|aarch64-alpine-linux-musl
    x86|i586-alpine-linux-musl
    x86_64|x86_64-alpine-linux-musl
"

PROGS="edivbug1 edivbug2 edivbug3"

# How do directory permissions in Linux work?
# https://unix.stackexchange.com/questions/21251/execute-vs-read-bit-how-do-directory-permissions-in-linux-work/317446#317446

# 리눅스 chmod 명령어 사용법 (Linux chmod command)
# https://recipes4dev.tistory.com/175

DIR1="out"
[ -d "$DIR1" ] || { mkdir "$DIR1" || exit 1; }
[ -r "$DIR1" ] && [ -w "$DIR1" ] && [ -x "$DIR1" ] || { chmod u+rwx || exit 1; }

DIR2="out-cross"
[ -d "$DIR2" ] || { mkdir "$DIR2" || exit 1; }
[ -r "$DIR2" ] && [ -w "$DIR2" ] && [ -x "$DIR2" ] || { chmod u+rwx || exit 1; }


for PROG in $PROGS; do
    clang $OPTS1 $OPTS2 -o $DIR1/$PROG $PROG.c
    sha1sum $DIR1/$PROG

    for TARGET in $TARGETS; do
        TARGET_LONG="${TARGET#*|}"
        TARGET_SHORT="${TARGET%|*}"
        # echo $TARGET_SHORT$'\t'$TARGET_LONG
        clang --target=$TARGET_LONG --sysroot=$HOME/cross/$TARGET_SHORT $OPTS1 -resource-dir=$HOME/cross/$TARGET_SHORT/usr/lib/llvm17/lib/clang/17 $OPTS2 -o $DIR2/$PROG'_'$TARGET_SHORT $PROG.c
        sha1sum $DIR2/$PROG'_'$TARGET_SHORT
    done
done

exit 0

clang -fuse-ld=lld -rtlib=compiler-rt -ffunction-sections -fdata-sections -Wl,--gc-sections -O2 -DNDEBUG -static -s -o out/edivbug3 edivbug3.c
clang --target=aarch64-alpine-linux-musl --sysroot=$HOME/cross-240317/aarch64 -fuse-ld=lld -rtlib=compiler-rt -resource-dir=$HOME/cross-240317/aarch64/usr/lib/llvm17/lib/clang/17 -ffunction-sections -fdata-sections -Wl,--gc-sections -O2 -DNDEBUG -static -s -o out-cross/edivbug3_aarch64 edivbug3.c

# xps:~/cross# ./download-and-extract.sh
# NAME:          Alpine Linux
# VERSION_ID:    3.19.1
# PRETTY_NAME:   Alpine Linux v3.19
# Repository:    https://dl-cdn.alpinelinux.org/alpine/v3.19/main
# Architecture:  armv7 aarch64 x86 x86_64
# Packages:
#          (1/2) musl-dev
#          (2/2) compiler-rt
# APK filenames:
#          (1/2) musl-dev-1.2.4_git20230717-r4.apk
#          (2/2) compiler-rt-17.0.5-r0.apk
# Testing downloaded 0 APK file(s)... 0 file(s) are OK
# Downloading 8 file(s)
# (1/8) https://dl-cdn.alpinelinux.org/alpine/v3.19/main/armv7/musl-dev-1.2.4_git20230717-r4.apk
# (2/8) https://dl-cdn.alpinelinux.org/alpine/v3.19/main/armv7/compiler-rt-17.0.5-r0.apk
# (3/8) https://dl-cdn.alpinelinux.org/alpine/v3.19/main/aarch64/musl-dev-1.2.4_git20230717-r4.apk
# (4/8) https://dl-cdn.alpinelinux.org/alpine/v3.19/main/aarch64/compiler-rt-17.0.5-r0.apk
# (5/8) https://dl-cdn.alpinelinux.org/alpine/v3.19/main/x86/musl-dev-1.2.4_git20230717-r4.apk
# (6/8) https://dl-cdn.alpinelinux.org/alpine/v3.19/main/x86/compiler-rt-17.0.5-r0.apk
# (7/8) https://dl-cdn.alpinelinux.org/alpine/v3.19/main/x86_64/musl-dev-1.2.4_git20230717-r4.apk
# (8/8) https://dl-cdn.alpinelinux.org/alpine/v3.19/main/x86_64/compiler-rt-17.0.5-r0.apk
# Extracting armv7/musl-dev-1.2.4_git20230717-r4.apk
# Extracting armv7/compiler-rt-17.0.5-r0.apk
# Extracting aarch64/musl-dev-1.2.4_git20230717-r4.apk
# Extracting aarch64/compiler-rt-17.0.5-r0.apk
# Extracting x86/musl-dev-1.2.4_git20230717-r4.apk
# Extracting x86/compiler-rt-17.0.5-r0.apk
# Extracting x86_64/musl-dev-1.2.4_git20230717-r4.apk
# Extracting x86_64/compiler-rt-17.0.5-r0.apk
# xps:~/cross#
