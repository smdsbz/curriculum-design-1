/*  $PROGRAM_ROOT/utils/project_functions.h
 *  项目方法定义与实现
 */

#ifndef PROJECT_FUNCTIONS
#define PROJECT_FUNCTIONS

    /**** POST | DELETE | PUT ****/

extern ProjectData initProjectData(void);
/*  创建项目数据模板
 *  ARGS:   void
 *  RETN:   根据在该函数执行过程中输入的数据所创建出来的原型
 *  NOTE:   用户输入前端
 */

extern Project *appendProject(Project *, ProjectData, Team *);
/*  添加项目数据
 *  ARGS:   项目链表头，新增节点数据域模板，母结点链（团队链）
 *  RETN:   新增节点的地址
 */

extern int modifyProject(Project *, ProjectData);
/*  覆盖项目信息
 *  ARGS:   目标地址，已经修改数据域buffer
 *  RETN:   success code
 */

extern int removeProject(Project **, Project *);
/*  删除项目节点
 *  ARGS:   指向团队链表头节点的指针，目标地址 | NULL
 *  RETN:   success code
 */

extern Project *createProjectHead(void);
/*  创建并初始化头节点
 *  ARGS:   void
 *  RETN:   头节点地址 || NULL
 */

    /**** SELECT ****/

extern ProjectWrapper *
getProjectById(Project *, Project *, const char *);
/*  通过id查找项目
 *  ARGS:   项目链表，搜索结束点，目标id
 *  RETN:   搜索结果挂载点 | NULL （没有搜索结果时也返回挂载点地址）
 */

extern ProjectWrapper *getProjectByTeam(Team *);
/*  通过团队查找项目
 *  ARGS:   目标团队节点
 *  RETN:   搜索结果挂载点 || NULL （没有搜索结果也返回挂载点地址）
 */

    /**** CLEANUPs ****/

extern void cleanupProjectWrapper(ProjectWrapper *);
/*  清空搜索结果序列
 *  ARGS:   头节点地址
 *  RETN:   void
 */

extern void cleanupProject(Project *);
/*  释放Project链所占用的空间
 *  ARGS:   头节点地址
 *  RETN:   void
 */

#endif
