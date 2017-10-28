rd /s /q obj
md obj

makefont source\fontlib.txt obj\fontlib.bin
bin2obj obj\fontlib.bin obj\fontlib.obj _fontlib

nasm IPL.nsm -o obj\IPL.bin
nasm easyOS.nsm -o obj\easyOS.bin
nasm nasmfunc.nsm -f coff -o obj\nasmfunc.obj

gcc graphic.c -c 
gcc bootpack.c -c
gcc dsctbl.c -c 
gcc int.c -c
gcc buffer.c -c
gcc keyboard.c -c
gcc mouse.c -c
move bootpack.o obj\bootpack.obj
move graphic.o obj\graphic.obj
move dsctbl.o obj\dsctbl.obj
move int.o obj\int.obj
move buffer.o obj\buffer.obj
move keyboard.o obj\keyboard.obj
move mouse.o obj\mouse.obj

obj2bim @source\obj2bim.rul out:obj\bootpack.bim stack:3136k map:obj\bootpack.map obj\bootpack.obj obj\graphic.obj obj\nasmfunc.obj obj\fontlib.obj obj\dsctbl.obj obj\int.obj obj\buffer.obj obj\keyboard.obj obj\mouse.obj
bim2hrb obj\bootpack.bim obj\bootpack.hrb 0
copy /B obj\easyOS.bin+obj\bootpack.hrb obj\easyOS.sys

edimg.exe   imgin:source\fdimg0at.tek   wbinimg src:obj\IPL.bin len:512 from:0 to:0 copy from:obj\easyOS.sys   to:@: imgout:MyOS.img
copy MyOS.img ..\..\MyOS.img

@pause

