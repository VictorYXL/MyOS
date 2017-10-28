rd /s /q obj
md obj

makefont source\fontlib.txt obj\fontlib.bin
bin2obj obj\fontlib.bin obj\fontlib.obj _fontlib

nasm IPL.nsm -o obj\IPL.bin
nasm entry.nsm -o obj\entry.bin
nasm nasmfunc.nsm -f coff -o obj\nasmfunc.obj

gcc main.c -c
gcc graphic.c -c 
gcc dsctbl.c -c 
gcc int.c -c
gcc buffer.c -c
gcc keyboard.c -c
gcc mouse.c -c
gcc memory.c -c
gcc sheet.c -c 
gcc timer.c -c
gcc mtask.c -c
gcc console.c -c
gcc calculator.c -c
gcc memorylist.c -c 
gcc tasklist.c -c 
gcc open.c -c
gcc textviewer.c -c 
gcc imageviewer.c -c 
gcc player.c -c 

move main.o obj\main.obj
move graphic.o obj\graphic.obj
move dsctbl.o obj\dsctbl.obj
move int.o obj\int.obj
move buffer.o obj\buffer.obj
move keyboard.o obj\keyboard.obj
move mouse.o obj\mouse.obj
move memory.o obj\memory.obj
move sheet.o obj\sheet.obj
move timer.o obj\timer.obj
move mtask.o obj\mtask.obj
move console.o obj\console.obj
move calculator.o obj\calculator.obj
move memorylist.o obj\memorylist.obj
move tasklist.o obj\tasklist.obj
move open.o obj\open.obj
move textviewer.o obj\textviewer.obj
move imageviewer.o obj\imageviewer.obj
move player.o obj\player.obj

obj2bim @source\obj2bim.rul out:obj\main.bim stack:3136k map:obj\main.map obj\main.obj obj\graphic.obj obj\nasmfunc.obj obj\fontlib.obj obj\dsctbl.obj obj\int.obj obj\buffer.obj obj\keyboard.obj obj\mouse.obj obj\memory.obj obj\sheet.obj obj\timer.obj obj\mtask.obj obj\console.obj obj\calculator.obj obj\memoryList.obj obj\tasklist.obj obj\open.obj obj\textviewer.obj obj\imageviewer.obj obj\player.obj
bim2hrb obj\main.bim obj\main.hrb 0
copy /B obj\entry.bin+obj\main.hrb obj\MyOS.sys

edimg.exe   imgin:source\fdimg0at.tek   wbinimg src:obj\IPL.bin len:512 from:0 to:0 copy from:obj\MyOS.sys   to:@: imgout:MyOS.img
copy MyOS.img ..\..\MyOS.img

@pause

