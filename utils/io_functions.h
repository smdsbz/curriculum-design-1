/*  $PROGRAM_ROOT/utils/io_functions.h
 *  数据加载保存方法头文件
 */

#ifndef IO_FUNCTIONS
#define IO_FUNCTIONS

extern int saveData(MountPoint, const char *);
/*  保存方法（打包）
 *  ARGS:   数据挂载点。储存数据文件的目标文件夹
 *  RETN:   success code
 */

extern int backupData(MountPoint, const char *);

extern MountPoint loadData(const char *);
/*  载入数据方法（打包）
 *  ARGS:   储存数据文件的目标文件夹
 *  RETN:   院系链表、团队链表、项目链表的挂载点
 */

#endif
