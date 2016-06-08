rem set oldpath=%path
rem set path=..;%path
zcc -O -v -b_CODE=0x8000 -b_BSS=0x9000 -b_STACK=0x9f00 hello.c
rem set path=%oldpath
