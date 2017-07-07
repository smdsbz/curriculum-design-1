#include <data_structure.h>

#ifdef BUILDING
#undef BUILDING
#endif
#define BUILDING

#ifdef DEBUG
#undef DEBUG
#endif
#define DEBUG

#if defined(BUILDING)
#include <stdio.h>
#include <stdlib.h>
#endif


#include <string.h>




/*********** Declaration ***********/


int modifyTeam(Team *tgt, Team new_one);
/*  修改团队信息
 *  ARGS:   目标地址，已经修改数据的buffer
 *  RETN:   success code
 */

Team *appendTeam(Team *tail, Team new_one);
/*  录入团队信息
 *  ARGS:   链表尾地址，已有数据的buffer
 *  RETN:   新增节点的地址 | NULL
 */

int removeTeam(Team *tgt);
/*  删除团队节点
 *  ARGS:   目标地址 | NULL
 *  RETN:   success code
 */

TeamWrapper *getTeamByTeacherNumber(Team *, const char oper, int val, TeamWrapper *);
/*  通过教师数量要求查找团队
 *  ARGS:   团队链表头地址，搜索方向[\<\>\=]，条件阈值，搜索结果链表挂载点
 *  RETN:   搜索结果挂载点 | NULL
 *  NOTE:   只有在内存申请失败时才返回NULL，没有匹配的结果时也返回挂载点地址
 */

TeamWrapper *getTeamByName(Team *, const char *, TeamWrapper *);
/*  通过团队名字（或名字的一部分）查找团队
 *  ARGS:   团队链表头地址，名称线索（不一定是全称）
 *  RETN:   搜索结果挂载点 | NULL
 */

Team initTeam(void);
/*  创建团队数据原型
 *  ARGS:   void
 *  RETN:   根据在该函数执行过程中输入的数据所创建出的原型
 */

void cleanupTeamWrapper(TeamWrapper *);
/*  清空索索结果序列
 *  ARGS:   头节点地址
 *  RETN:   void
 */



/************ Function Realizations *************/





Team *appendTeam(Team *tail, Team new_one) {
    if (tail->next != NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in appendTeam():\n\ttail.next not pointing to NULL");
        #endif
        return NULL;
    }

    tail->next = (Team *)malloc(sizeof(Team));
    if (tail->next == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in appendTeam():\n\tfailed to alloc memory");
        #endif
        return NULL;
    }

    *(tail->next) = new_one;

    return tail->next;
}


int modifyTeam(Team *tgt, Team new_one) {
    if (tgt == NULL) {
        #if defined(DEBUG)
        puts("[LOG] modifyTeam():\n\ttarget is NULL");
        #endif
        return 0;
    }

    *tgt = new_one;
}
