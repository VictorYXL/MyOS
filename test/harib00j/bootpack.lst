     1 00000000                                 [FORMAT "WCOFF"]
     2 00000000                                 [INSTRSET "i486p"]
     3 00000000                                 [OPTIMIZE 1]
     4 00000000                                 [OPTION 1]
     5 00000000                                 [BITS 32]
     6 00000000                                 	EXTERN	_io_hlt
     7 00000000                                 [FILE "bootpack.c"]
     8                                          [SECTION .text]
     9 00000000                                 	GLOBAL	_HariMain
    10 00000000                                 _HariMain:
    11 00000000 55                              	PUSH	EBP
    12 00000001 89 E5                           	MOV	EBP,ESP
    13 00000003                                 L2:
    14 00000003 E8 [00000000]                   	CALL	_io_hlt
    15 00000008 EB F9                           	JMP	L2
