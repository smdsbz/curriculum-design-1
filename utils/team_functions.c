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



/**** Additional typdef: Search Condition ****/

typedef struct _Where {
    char    direction[3];   // 查找条件 - ^[\>\<]=?|=$
    int     value;
} Where;


int (*setJudger(const char *direction))(int, int) {
/*  返回匹配规则函数
 *  ARGS:   代表搜索规则的字符串(eg "<=")
 *  RETN:   int judger(int valve, int data)
 *               实际为上面申明的函数中的一种
 */
    int isSmaller(int valve, int data) { return (data < valve); }
    int isLarger(int valve, int data) { return (data > valve); }
    int isEqualTo(int valve, int data) { return (data == valve); }
    int noLargerThan(int valve, int data) { return (data <= valve); }
    int noSmallerThan(int valve, int data) { return (data >= valve); }

    if (!strcmp("<", direction)) { return isSmaller; }
    if (!strcmp(">", direction)) { return isLarger; }
    if (!strcmp("=", direction)) { return isEqualTo; }
    if (!strcmp("<=", direction)) { return noLargerThan; }
    if (!strcmp(">=", direction)) { return noSmallerThan; }
    else { return NULL; }
}




/*********** Declaration ***********/

    /**** POST | DELETE | PUT ****/

Team *appendTeam(Team *head, Team *tail, TeamData new_one,
        Depart *depart_chain);
/*  录入团队
 *  ARGS:   链表头，链表尾，已有数据的作为buffer的TeamData实例，母结点链（院系链）
 *  RETN:   新增节点的地址
 */

int modifyTeam(Team *target, TeamData new_one);
/*  修改团队信息
 *  ARGS:   目标地址，已经修改数据的buffer
 *  RETN:   success code
 */

int removeTeam(Team **phead, Team *tgt, Depart *depart_chain);
/*  删除团队节点
 *  ARGS:   指向团队链表头节点地址的指针，目标地址 | NULL
 *  RETN:   success code
 */


    /**** SELECT ****/

TeamWrapper *getTeamByTeacherNum(Team *, const Where cond,
                                 Depart *depart_chain);
/*  通过教师数量查找团队
 *  ARGS:   团队链表，查找条件，院系链表头
 *  RETN:   搜索结果挂载点 | NULL （没有结果时也返回挂载点地址）
 *  NOTE:   调用过程中会为TeamWrapper申请内存空间，使用完搜索结果后记得cleanup
 */

TeamWrapper *getTeamByName(Team *, const char *, Depart *depart_chain);
/*  通过团队名称查找团队
 *  ARGS:   团队链表，团队名称线索（不一定是全称），院系链表头
 *  RETN:   搜索结果挂载点 | NULL
 *  NOTE:   调用过程中会为TeamWrapper申请内存空间，使用完搜索结果后记得cleanup
 */

TeamData initTeamData(void);
/*  创建一个团队数据的原型
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


Team *appendTeam(Team *head, Team *tail,
        TeamData new_one, Depart *depart_chain) {

    // NOTE: 相比appendDepart()多了寻找并挂载到院系的操作
    DepartWrapper *parent_depart_wrapper = getDepartByName(depart_chain, new_one.faculty);
    if (parent_depart_wrapper->depart == NULL) {
        puts("target department not found\n");
        return NULL;
    }
    if (parent_depart_wrapper->next != NULL) {
        puts("multiple departments found\n");
        return NULL;
    }

    // 此时，已经验证了parent_depart_wrapper中只有一个院系
    Depart *parent_depart = parent_depart_wrapper->depart;

    // NOTE: 立即清理搜索结果所占用的空间
    cleanupDepartWrapper(parent_depart_wrapper);

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
    // 在母结点中注册
    parent_depart->data->team_num += 1;
    // 子节点链接母结点
    tail->next->parent_depart = parent_depart;

    tail->next->next = NULL;
    tail->next->child_project_head = NULL;
    tail->next->child_project_tail = NULL;


    return tail->next;
}


int modifyTeam(Team *tgt, TeamData new_one) {
    if (tgt == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in modifyTeam():\n\ttarget is NULL");
        #endif
        return 0;
    }
    // HACK: 易主 - 改变团队所属院系
    // 目前解决方案：删掉重来
    if (tgt->data->faculty != new_one.faculty) {
        #if defined(DEBUG)
        puts("[LOG] Error in modifyTeam():\n\tchanging team owner");
        #endif
        return 0;
    }
    *(tgt->data) = new_one;
    return 1;
}


int removeTeam(Team **phead, Team *tgt, Depart *depart_chain) {
    if (tgt == NULL) {   // 无效参数
        #if defined(DEBUG)
        puts("[LOG] Error in removeTeam():\n\ttarget is NULL");
        #endif
        return 0;
    }
    // HACK: 注销该组的时候，同时注销该组运营的所有项目
    if (tgt->child_project_head
            || tgt->child_project_tail) {  // 该组有正在运行的项目
        #if defined(DEBUG)
        puts("[LOG] Error in removeTeam():\n\tteam has working porjects, can't move");
        #endif
        return 0;
    }

    if (*phead == tgt) {    // removing the first node
        *phead = tgt->next;  // shift pointer
    }
    else {
        Team *phead_safe = *phead;
        for (; phead_safe->next != tgt; phead_safe = phead_safe->next) ;
        phead_safe->next = phead_safe->next->next;  // route around
    }

    // NOTE: register changes in parent node (before calling free())
    tgt->parent_depart->data->team_num -= 1;

    free(tgt->data);
    free(tgt);

    return 1;
}


    /**** SELECT ****/
TeamWrapper *getTeamByTeacherNum(Team *start, const Where cond,
                                 Depart *depart_chain) {
    // (auto-indent fixer)
    // NOTE: TeamWrapper中会有多个结果

    TeamWrapper *rtn = (TeamWrapper *)malloc(sizeof(TeamWrapper));
    if (rtn == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in getTeamByTeacherNum():\n\tfailed to malloc for result mounting point");
        #endif
        return NULL;
    }

    TeamWrapper *rtn_head = rtn;
    rtn_head->team = NULL; rtn_head->next = NULL;

    // set judger
    int (*judger)(int, int);
    judger = setJudger(cond.direction);
    if (judger == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in getTeamByTeacherNum():\n\tunrecoginized condition");
        #endif
        return NULL;
    }

    // start finding matching nodes
    while (1) {
        if (judger(cond.value, start->data->teacher_num)) {

            #if defined(DEBUG)
            printf("[LOG] getTeamByTeacherNum(): found %s @ 0x%p\n",
                   start->data->name, start);
            #endif

            if (rtn_head->team == NULL) {   // head node
                rtn->team = start;
            }
            else {  // non-head node - previous record already exists
                rtn->next = (TeamWrapper *)malloc(sizeof(TeamWrapper));
                if (rtn->next == NULL) {
                    #if defined(DEBUG)
                    puts("[LOG] Error in getTeamByTeacherNum():\n\tfailed to malloc for result container");
                    #endif
                    // HACK
                    cleanupTeamWrapper(rtn_head);
                    return NULL;
                }   // finished verifying malloc-ed space
                rtn = rtn->next; rtn->next = NULL;
                rtn->team = start;
            }   // end of non-head node processing

        }   // finished judging current node

        // move to next node
        start = start->next;
        // al done?
        if (start == NULL) { break; }
    }   // al done!
    // return result container
    return rtn_head;
}



    /**** CLEANUPs ****/
