#ifndef DATA_STRUCTURE
#include "data_structure.c"
#endif


#ifndef FACULTY_FUNCTIONS
#include "faculty_functions.h"
#endif

#ifndef TEAM_FUNCTIONS
#include "team_functions.h"
#endif


#ifdef BUILDING
#undef BUILDING
#endif
#define BUILDING

#ifdef DEBUG
#undef DEBUG
#endif
#define DEBUG



/********** Declaration **********/

    /**** POST | DELETE | PUT ****/

ProjectData initProjectData(void);
/*  创建一个项目数据的原型
 *  ARGS:   void
 *  RETN:   根据在该函数执行过程中输入的数据所创建出来的原型
 *  NOTE:   will trigger input action
 */

Project *appendProject(Project *head, ProjectData new_one, Team *team_chain);
/*  录入项目
 *  ARGS:   项目链表头，新增节点数据域模板，母结点链（团队链）
 *  RETN:   新增节点的地址
 */

int modifyProject(Project *tgt, ProjectData new_one);
/*  修改项目信息
 *  ARGS:   目标地址，已经修改的数据域
 *  RETN:   success code
 */

int removeProject(Project **phead, Project *tgt);
/*  删除项目节点
 *  ARGS:   指向团队链表头节点的指针，目标地址 | NULL
 *  RETN:   success code
 */


    /**** SELECT ****/

ProjectWrapper *getProjectById(Project *head, const char *id);
/*  通过id查找项目
 *  ARGS:   项目链表，目标id
 *  RETN:   搜索结果挂载点 | NULL （没有搜索结果时也返回挂载点地址）
 */

// ProjectWrapper *getProjectByTeam(Project *head, const char *team_name);
/*  通过团队查找项目
 *  NOTE:   由于通过团队节点可以很方便地取到相应的项目链，故先不构建该函数
 */


    /**** CLEANUPs ****/

void cleanupProjectWrapper(TeamWrapper *start);
/*  清空搜索结果序列
 *  ARGS:   头节点地址
 *  RETN:   void
 */

void cleanupProject(Project *start);
/*  释放Project链所占用的空间
 *  ARGS:   头节点地址
 *  RETN:   void
 */



/********** Realizations **********/

    /**** POST | DELETE | PUT ****/













#ifdef BUILDING
#undef BUILDING
#endif

#ifdef DEBUG
#undef DEBUG
#endif
