#	$PROGRAM_ROOT/makefile
#	编译所有文件
#	USAGE:
#		Win (with MinGW):	PS > mingw32-make
#							PS > mingw32-make run
# 		*nix:	$ make && ./a.exe ./utils/data

main:
	gcc -o a.exe ./utils/*?.c ./*?.c

# `chcp 65001`为Win命令，调整控制台活动页为UTF-8
run:
	./a.exe ./utils/data

clean:
	rm a.exe
