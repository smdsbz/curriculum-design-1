#ifndef TEAM_FUNCTIONS
#define TEAM_FUNCTIONS

    /**** POST | DELETE | PUT ****/

extern TeamData initTeamData(void);
/*  创建一个团队数据的原型
 *  ARGS:   void
 *  RETN:   根据在该函数执行过程中输入的数据所创建出来的原型
 *  NOTE:   will trigger input action
 */


extern Team *appendTeam(Team *head, TeamData new_one, Depart *depart_chain);
/*  录入团队
 *  ARGS:   链表头，已有数据的作为buffer的TeamData实例，母结点链（院系链）
 *  RETN:   新增节点的地址
 */

extern int modifyTeam(Team *target, TeamData new_one);
/*  修改团队信息
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

extern TeamWrapper *getTeamByTeacherNum(Team *, const Where cond);
/*  通过教师数量查找团队
 *  ARGS:   团队链表，查找条件
 *  RETN:   搜索结果挂载点 | NULL （没有结果时也返回挂载点地址）
 *  NOTE:   调用过程中会为TeamWrapper申请内存空间，使用完搜索结果后记得cleanup
 */

extern TeamWrapper *getTeamByName(Team *, const char *);
/*  通过团队名称查找团队
 *  ARGS:   团队链表，团队名称线索（不一定是全称）
 *  RETN:   搜索结果挂载点 | NULL
 *  NOTE:   调用过程中会为TeamWrapper申请内存空间，使用完搜索结果后记得cleanup
 */


    /**** CLEANUPs ****/

extern void cleanupTeamWrapper(TeamWrapper *start);
/*  清空搜索结果序列
 *  ARGS:   头节点地址
 *  RETN:   void
 *  NOTE:   每次搜完了记得调一次啊。。。
 *  NOTE:   调用后传进来的那个节点也没了！
 */

extern void cleanupTeam(Team *start);
/*  释放Team链所占用的内存空间
 *  ARGS:   头节点地址
 *  RETN:   void
 */







#endif
