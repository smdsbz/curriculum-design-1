#ifndef DATA_STRUCTURE
#include "data_structure.c"
#endif

#ifndef FACULTY_FUNCTIONS
#include "faculty_functions.h"
#endif


#ifdef BUILDING
#undef BUILDING
#endif
#define BUILDING

#ifdef DEBUG
#undef DEBUG
#endif
#define DEBUG

// #if defined(BUILDING)
// #include <stdio.h>
// #include <stdlib.h>
// #endif





/*********** Declaration ***********/

    /**** POST | DELETE | PUT ****/

Team *appendTeam(Team *head, Team *tail, TeamData new_one);
/*  录入院系
 *  ARGS:   链表头，链表尾，已有数据的作为buffer的TeamData实例
 *  RETN:   新增节点的地址
 */

int modifyTeam(Team *target, TeamData new_one);
/*  修改院系信息
 *  ARGS:   目标地址，已经修改数据的buffer
 *  RETN:   success code
 */

int removeTeam(Team **phead, Team *target);
/*  删除院系节点（也可以用作free方法？？）
 *  ARGS:   指向院系链表头节点地址的指针，目标地址 | NULL
 *  RETN:   success code
 */

    /**** SELECT ****/

TeamWrapper *getTeamByManager(Team *, const char *, TeamWrapper *);
/*  通过负责人姓名查找院系
 *  ARGS:   院系链表，院系负责人 char[12]，搜索结果挂载点
 *  RETN:   搜索结果挂载点 | NULL （没有结果时也返回挂载点地址）
 *  NOTE:   院系负责人不同名，返回的是一个院系的数据，或者也可以像下面的这个函数一样，返回结果链
 *  NOTE:   只能在院系链有数据的情况下调用该函数！
 */

TeamWrapper *getTeamByName(Team *, const char *, TeamWrapper *);
/*  通过院系名称查找院系
 *  ARGS:   名称线索（不一定是全称）
 *  RETN:   搜索结果挂载点 | NULL
 *  NOTE:   由于查找结果可能不只有一个，该操作会创建一个用于储存查询结果的新链表，返回链表的头节点地址
 */

TeamData initTeamData(void);
/*  创建一个院系数据的原型
 *  ARGS:   void
 *  RETN:   根据在该函数执行过程中输入的数据所创建出来的原型
 *  NOTE:   will trigger input action
 */

    /**** CLEANUPs ****/

void cleanupTeamWrapper(TeamWrapper *start);
/*  清空搜索结果序列
 *  ARGS:   头节点地址
 *  RETN:   void
 *  NOTE:   每次搜完了记得调一次啊。。。
 *  NOTE:   调用后传进来的那个节点也没了！
 */

void cleanupTeam(Team *start);
/*  释放Team链所占用的内存空间
 *  ARGS:   头节点地址
 *  RETN:   void
 */



/************ Function Realizations *************/

    /**** POST | DELETE | PUT ****/

TeamData initTeamData(void) {
    TeamData VirtusPro;
    printf("team.name = "); scanf("%s", VirtusPro.name);
    printf("team.manager = "); scanf("%s", VirtusPro.manager);
    printf("team.teacher_num = "); scanf("%s", VirtusPro.teacher_num);
    printf("team.student_num = "); scanf("%s", VirtusPro.student_num);
    printf("team.faculty = "); scanf("%s", VirtusPro.faculty);
    VirtusPro.project_num = 0;
    return VirtusPro;
}


Team *appendTeam(Team *head, Team *tail, TeamData new_one) {

    Depart *parent_depart = 

    // 错误处理
    if (tail->next != NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in appendTeam():\n\ttail->next not pointing to NULL");
        #endif
        return NULL;
    }
    // 向刚创建的链表的第一个节点写入数据
    if (tail == head
            && tail->data == NULL) {
        tail->data = (TeamData *)malloc(sizeof(TeamData));
        if (tail->data == NULL) {
            #if defined(DEBUG)
            puts("[LOG] Error in appendTeam():\n\tfailed to alloc memory for data (head)");
            #endif
            return NULL;
        }
        *(tail->data) = new_one; tail->next = NULL;
        tail->child_project_head = NULL; tail->child_project_tail = NULL;
        return tail;
    }
    // 需要添加节点的添加操作
    tail->next = (Team *)malloc(sizeof(Team));
    if (tail->next == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in appendTeam():\n\tfailed to alloc memory for container");
        #endif
        return NULL;
    }
    tail->next->data = (TeamData *)malloc(sizeof(TeamData));
    if (tail->next->data == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in appendTeam():\n\tfailed to alloc memory for data");
        #endif
        return NULL;
    }

    *(tail->next->data) = new_one;

    tail->next->next = NULL;
    tail->next->child_project_head = NULL;
    tail->next->child_project_tail = NULL;

    return tail->next;
}



// int modifyTeam(Team *)
