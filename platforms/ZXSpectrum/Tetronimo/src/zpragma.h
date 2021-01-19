// https://github.com/z88dk/z88dk/blob/master/doc/ZXSpectrumZSDCCnewlib_06_SomeDetails.md


#pragma output REGISTER_SP = 0x5FFF // place stack 

#pragma output CRT_ORG_CODE = 0x6164 // move org just above the im2 table and jump

#pragma output CRT_ENABLE_RESTART = 1 // not returning to basic

#pragma output CRT_ENABLE_CLOSE = 0   // do not close files on exit

#pragma output CLIB_EXIT_STACK_SIZE = 0 // no exit stack

#pragma output CLIB_MALLOC_HEAP_SIZE = 4500 // malloc heap size  (300 bytes per sprite?)

#pragma output CLIB_STDIO_HEAP_SIZE = 0     // no memory needed to create file descriptors

#pragma output CLIB_FOPEN_MAX = -1 // no allocated FILE structures, -1 for no file lists too

#pragma output CLIB_OPEN_MAX = 0   // no fd table


//#pragma output REGISTER_SP           = -1  // stack below code

