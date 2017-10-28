md obj

makefont source\fontlib.txt obj\fontlib.bin
bin2obj obj\fontlib.bin obj\fontlib.obj _fontlib

nasm IPL.nsm -o obj\IPL.bin
nasm easyOS.nsm -o obj\easyOS.bin
nasm nasmfunc.nsm -f coff -o obj\nasmfunc.obj
rem nask nasmfunc.nsm obj\nasmfunc.obj obj\nasmfunc.lst

gcc graphic.c -c 
gcc bootpack.c -c 
move bootpack.o obj\bootpack.obj
move graphic.o obj\graphic.obj

obj2bim @source\obj2bim.rul out:obj\bootpack.bim stack:3136k map:obj\bootpack.map obj\bootpack.obj obj\graphic.obj obj\nasmfunc.obj obj\fontlib.obj
bim2hrb obj\bootpack.bim obj\bootpack.hrb 0
copy /B obj\easyOS.bin+obj\bootpack.hrb obj\easyOS.sys

edimg.exe   imgin:source\fdimg0at.tek   wbinimg src:obj\IPL.bin len:512 from:0 to:0 copy from:obj\easyOS.sys   to:@: imgout:MyOS.img
copy MyOS.img ..\..\MyOS.img

@pause