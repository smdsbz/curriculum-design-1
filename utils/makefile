#	$PROGRAM_ROOT/utils/makefile
#	单元测试用临时编译批处理
#	USAGE:
#		Win (with MinGW):	PS > mingw32-make
#							PS > mingw32-make run
# 		*nix:	$ make && ./a.exe
#   NOTE: 编译前解注释待测试模块的DEBUG宏，然后按需编译
#         如现在要测试团队模块功能，则编译命令应为:
		  	gcc -o a.exe ./faculty_functions.c ./team_functions.c

main:
	gcc -o a.exe ./faculty_functions.c ./team_functions.c ./project_functions.c ./io_functions.c

run:
	chcp 65001
	./a.exe
