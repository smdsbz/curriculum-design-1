/*  $PROGRAM_ROOT/utils/team_functions.h
 *  团队方法头文件
 */

#ifndef TEAM_FUNCTIONS
#define TEAM_FUNCTIONS

    /**** POST | DELETE | PUT ****/

extern TeamData initTeamData(void);
/*  创建团队数据模板
 *  ARGS:   void
 *  RETN:   根据在该函数执行过程中输入的数据所创建出来的原型
 *  NOTE:   用户输入前端
 */

extern Team *appendTeam(Team *head, TeamData new_one,
                        Depart *depart_chain);
/*  添加团队数据
 *  ARGS:   链表头，团队数据模板，母结点链（院系链）
 *  RETN:   新增节点的地址
 */

extern int modifyTeam(Team *target, TeamData new_one);
/*  覆盖团队信息
 *  ARGS:   目标地址，已经修改数据的buffer
 *  RETN:   success code
 */

extern int removeTeam(Team **phead, Team *tgt);
/*  删除团队节点
 *  ARGS:   指向团队链表头节点地址的指针，目标地址 | NULL
 *  RETN:   success code
 */

extern Team *createTeamHead(void);
/*  创建并初始化头节点
 *  ARGS:   void
 *  RETN:   头节点地址 || NULL
 */

    /**** SELECT ****/

extern TeamWrapper *
getTeamByTeacherNum(Team *, Team *, const Where cond);
/*  通过教师数量查找团队
 *  ARGS:   团队链表，搜索结束点，查找条件
 *  RETN:   搜索结果挂载点 | NULL
 */

extern TeamWrapper *getTeamByName(Team *, Team *, const char *);
/*  通过团队名称查找团队
 *  ARGS:   团队链表，搜索结束点，团队名称线索（不一定是全称）
 *  RETN:   搜索结果挂载点 | NULL
 */

extern TeamWrapper *getTeamByDepart(Depart *);
/*  通过院系查找团队
 *  ARGS:   目标院系节点
 *  RETN:   搜索结果（该院系下所有团队）挂载点 || NULL
 */

    /**** STAT ****/

extern TeamStatWrapper *
buildTeamStatChainUnordered(Team *, Team *, char);
/*  统计所有团队数据
 *  ARGS:   团队链表头，遍历区域边界，查询NSFC项目flag
 *  RETN:   统计结果挂载点
 */

extern TeamStatWrapper *
orderTeamStatWrapperByNSFCProject(TeamStatWrapper *);
/*  将统计结果按NSFC项目数降序排序
 *  ARGS:   统计结果挂载点
 *  RETN:   排序后统计结果链的挂载点
 */

extern TeamStatWrapper *
orderTeamStatWrapperByPTRatio(TeamStatWrapper *);
/*  将统计结果按项目/教师比降序排序
 */

    /**** CLEANUPs ****/

extern void cleanupTeamWrapper(TeamWrapper *start);
/*  清空搜索结果序列
 *  ARGS:   头节点地址
 *  RETN:   void
 */

extern void cleanupTeamStatWrapper(TeamStatWrapper *);
/*  清除统计结果链表
 *  ARGS:   统计结果链表头节点地址
 *  RETN:   void
 */

extern void cleanupTeam(Team *start);
/*  释放Team链所占用的内存空间
 *  ARGS:   头节点地址
 *  RETN:   void
 */

#endif
