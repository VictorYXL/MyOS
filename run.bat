nasm IPL.nsm -o IPL.bin
nasm easyOS.nsm -o easyOS.sys
edimg.exe   imgin:I:\Undergraduate\MyOS\tolset\z_tools\fdimg0at.tek   wbinimg src:IPL.bin len:512 from:0 to:0 copy from:easyOS.sys   to:@: imgout:MyOS.img
copy MyOS.img I:\Undergraduate\MyOS
@pause