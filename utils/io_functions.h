#ifndef IO_FUNCTIONS
#define IO_FUNCTIONS

int saveData(MountPoint, const char *);
/*  打包保存方法
 *  ARGS:   数据挂载点。储存数据文件的目标文件夹
 *  RETN:   success code
 */

MountPoint loadData(const char *);
/*  打包载入数据方法
 *  ARGS:   储存数据文件的文件夹
 *  RETN:   院系链表、团队链表、项目链表的挂载点
 */

#endif
