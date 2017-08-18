#ifndef DATA_STRUCTURE
#include "data_structure.h"
#endif

#ifndef FACULTY_FUNCTIONS
#include "faculty_functions.h"
#endif


#ifdef BUILDING
#undef BUILDING
#endif
// #define BUILDING

#ifdef DEBUG
#undef DEBUG
#endif
// #define DEBUG


// /**** Additional typdef: Search Condition ****/
//
// typedef struct _Where {
// char    direction[3];   // 查找条件 - ^[\>\<]=?|=$
// int     value;
// } Where;

// 实际执行的函数应在外部申明，否则在第一轮执行后，judger指向的函数内存空间会被收回
int isSmaller(int valve, int data) { return (data < valve); }
int isLarger(int valve, int data) { return (data > valve); }
int isEqualTo(int valve, int data) { return (data == valve); }
int noLargerThan(int valve, int data) { return (data <= valve); }
int noSmallerThan(int valve, int data) { return (data >= valve); }

int (*setJudger(const char *direction))(int, int) {
/*  返回匹配规则函数
*  ARGS:   代表搜索规则的字符串(eg "<=")
*  RETN:   int judger(int valve, int data)
*               实际为上面申明的函数中的一种
*/

if (!strcmp("<", direction)) { return isSmaller; }
if (!strcmp(">", direction)) { return isLarger; }
if (!strcmp("=", direction)) { return isEqualTo; }
if (!strcmp("<=", direction)) { return noLargerThan; }
if (!strcmp(">=", direction)) { return noSmallerThan; }
else { return NULL; }
}







/*********** Declaration ***********/

    /**** POST | DELETE | PUT ****/

TeamData initTeamData(void);
/*  创建一个团队数据的原型
 *  ARGS:   void
 *  RETN:   根据在该函数执行过程中输入的数据所创建出来的原型
 *  NOTE:   will trigger input action
 */


Team *appendTeam(Team *head, TeamData new_one, Depart *depart_chain);
/*  录入团队
 *  ARGS:   链表头，已有数据的作为buffer的TeamData实例，母结点链（院系链）
 *  RETN:   新增节点的地址
 */

int modifyTeam(Team *target, TeamData new_one);
/*  修改团队信息
 *  ARGS:   目标地址，已经修改数据的buffer
 *  RETN:   success code
 */

int removeTeam(Team **phead, Team *tgt);
/*  删除团队节点
 *  ARGS:   指向团队链表头节点地址的指针，目标地址 | NULL
 *  RETN:   success code
 */

Team *createTeamHead(void);
/*  创建并初始化头节点
 *  ARGS:   void
 *  RETN:   头节点地址 || NULL
 */


    /**** SELECT ****/

TeamWrapper *getTeamByTeacherNum(Team *, Team *, const Where cond);
/*  通过教师数量查找团队
 *  ARGS:   团队链表，搜索结束点，查找条件
 *  RETN:   搜索结果挂载点 | NULL （没有结果时也返回挂载点地址）
 *  NOTE:   调用过程中会为TeamWrapper申请内存空间，使用完搜索结果后记得cleanup
 */

TeamWrapper *getTeamByName(Team *, Team *, const char *);
/*  通过团队名称查找团队
 *  ARGS:   团队链表，搜索结束点，团队名称线索（不一定是全称）
 *  RETN:   搜索结果挂载点 | NULL
 *  NOTE:   调用过程中会为TeamWrapper申请内存空间，使用完搜索结果后记得cleanup
 */

TeamStatWrapper *buildTeamStatChainUnordered(Team *, Team *);
TeamStatWrapper *orderTeamStatWrapperByNSFCProject(TeamStatWrapper *);
TeamStatWrapper *orderTeamStatWrapperByProjectTotal(TeamStatWrapper *);

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

Team *createTeamHead(void) {
    Team *Team_HEAD = (Team *)malloc(sizeof(Team));
    if (Team_HEAD == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in createTeamHead():\n\tfailed to malloc for team (head)");
        #endif
        return NULL;
    }
    Team_HEAD->data = NULL; Team_HEAD->next = NULL;
    Team_HEAD->parent_depart = NULL;
    Team_HEAD->child_project_head = NULL;
    Team_HEAD->child_project_tail = NULL;
    return Team_HEAD;
}


TeamData initTeamData(void) {
    TeamData VirtusPro;
    printf("add/team::name > "); scanf("%s", VirtusPro.name);
    printf("add/team::manager > "); scanf("%s", VirtusPro.manager);
    printf("add/team::teacher_num > "); scanf("%d", &(VirtusPro.teacher_num));
    printf("add/team::student_num > "); scanf("%d", &(VirtusPro.student_num));
    printf("add/team::faculty > "); scanf("%s", VirtusPro.faculty);
    #if defined(CHILD_COUNTER)
    VirtusPro.project_num = 0;
    #endif
    return VirtusPro;
}



Team *appendTeam(Team *head, TeamData new_one, Depart *depart_chain) {
    TeamWrapper *team_wrapper = getTeamByName(head, NULL, new_one.name);
    if (team_wrapper->team != NULL) {
        printf("Record for %s team already exists!\n", new_one.name);
        cleanupTeamWrapper(team_wrapper);
        return NULL;
    }
    cleanupTeamWrapper(team_wrapper);

    // NOTE: 该函数遵循FILO顺序，因此最后生成的链表中节点的顺序与院系链表节点顺序不相关
    //       （但还是有按照院系分块的）
    //       该链表节点的顺序将在负责存储的函数中进行整理

    // NOTE: 数据域需要深复制，因为出函数体后传进来的形参 new_one 会被销毁

    Team *tail = head;
    for (; tail->next; tail = tail->next) ;
    // tail->next == NULL

    // 获取院系母结点
    DepartWrapper *parent_depart_wrapper = getDepartByName(depart_chain, NULL,  new_one.faculty);
    if (parent_depart_wrapper == NULL) {
        #if defined(DEBUG)
        puts("[LOG] appendTeam():\n\tgetDepartByName() failed");
        #endif
        return NULL;
    }
    if (parent_depart_wrapper->depart == NULL) {
        puts("Target parent department not found!\n");
        return NULL;
    }
    if (parent_depart_wrapper->next != NULL) {
        puts("Multiple parent departments found!\n");
        return NULL;
    }
    if (strcmp(parent_depart_wrapper->depart->data->name, new_one.faculty)) {
        printf("Department named %s not found, do you mean %s instead?\n",
               new_one.faculty, parent_depart_wrapper->depart->data->name);
        return NULL;
    }

    // 此时，已经验证了 parent_depart_wrapper 中只有一个院系
    Depart *parent_depart = parent_depart_wrapper->depart;

    // 立即清理搜索结果所占用的空间
    cleanupDepartWrapper(parent_depart_wrapper);

    #if defined(BUILDING)
    printf("[LOG] appendTeam(): parent_depart is %s @ 0x%p\n",
           parent_depart->data->name, parent_depart);
    #endif

    // got parent_depart

    // 添加节点

    //   case 1: 团队链表中目前还没有数据，此次操作将写入链表的第一个数据
    if (tail == head
            && tail->data == NULL) {
        #if defined(DEBUG)
        puts("[LOG] appendTeam(): case 1");
        #endif
        // 数据域深复制内存空间准备
        tail->data = (TeamData *)malloc(sizeof(TeamData));
        if (tail->data == NULL) {
            #if defined(DEBUG)
            puts("[LOG] Error in appendTeam():\n\tfailed to alloc memory for data (head)");
            #endif
            return NULL;
        }
        // 子节点注册母结点
        // 数据域深复制
        *(tail->data) = new_one; tail->next = NULL;
        tail->child_project_head = NULL; tail->child_project_tail = NULL;
        tail->parent_depart = parent_depart;
        // 母结点注册子节点
        #if defined(CHILD_COUNTER)
        parent_depart->data->team_num += 1;
        #endif
        parent_depart->child_team_head = tail;
        parent_depart->child_team_tail = tail;
        // 退出函数
        return tail;
    }
    //   case 2 & 3: 需要添加节点的操作
    if (parent_depart->child_team_tail == NULL
            || parent_depart->child_team_tail->next == NULL) {
        #if defined(DEBUG)
        puts("[LOG] appendTeam(): case 2");
        #endif
        // case 2: 母结点在当前还没有子节点
        //         或者 母结点的团队链的尾就是团队链表的尾节点
        //         --> 向尾节点 tail 后添加节点
        //         append
        tail->next = (Team *)malloc(sizeof(Team));
        if (tail->next == NULL) {
            #if defined(DEBUG)
            puts("[LOG] Error in appendTeam():\n\tfailed to alloc memory for container");
            #endif
            return NULL;
        }
        // 由于传进来的参数是形参，必须另外为数据域分配储存空间
        tail->next->data = (TeamData *)malloc(sizeof(TeamData));
        if (tail->next->data == NULL) {
            #if defined(DEBUG)
            puts("[LOG] Error in appendTeam():\n\tfailed to alloc memory for data");
            #endif
            return NULL;
        }
        // 下一个节点的指向需要在这里设置
        // 因为在下一个条件块中，该处指向不同
        tail->next->next = NULL;
    } else {    // case 3: 母结点已经有子节点了
        #if defined(DEBUG)
        puts("[LOG] appendTeam(): case 3");
        #endif
        // 为保证后续代码兼容性，退出此区块时 tail->next 指向新添加的节点
        // 将tail指向现有子节点链的尾部
        tail = parent_depart->child_team_tail;
        // 将原链表尾的下一个节点进行备份
        Team *after = tail->next;   // something or NULL
        // 为新节点开辟空间
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
        // 下一个节点指向 - 重新链接链表
        tail->next->next = after;
    }

    // 数据域深复制
    *(tail->next->data) = new_one;
    // 在母结点中注册
    //   注册团队数量
    #if defined(CHILD_COUNTER)
    parent_depart->data->team_num += 1;
    #endif
    //   注册指针指向
    if (parent_depart->child_team_head == NULL) {
    //     该团队为母结点的第一个子节点
        parent_depart->child_team_head = tail->next;
    }
    //     该团队一定是母结点最后一个注册的子节点
    parent_depart->child_team_tail = tail->next;
    // 子节点链接母结点
    tail->next->parent_depart = parent_depart;
    // 子节点的子节点指向初始化
    tail->next->child_project_head = NULL;
    tail->next->child_project_tail = NULL;
    // 返回新添加的节点
    return tail->next;
}


int modifyTeam(Team *tgt, TeamData new_one) {
    if (tgt == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in modifyTeam():\n\ttarget is NULL");
        #endif
        return 0;
    }
    // TODO: 易主 - 改变团队所属院系
    // 目前解决方案：删掉重来
    if (strcmp(tgt->data->faculty, new_one.faculty) != 0) {
        #if defined(DEBUG)
        printf("[LOG] Error in modifyTeam():\n\tchanging team faculty,");
        printf(" from %s to %s, strcmp() got %d\n",
               tgt->data->faculty, new_one.faculty,
               strcmp(tgt->data->faculty, new_one.faculty));
        #endif
        return 0;
    }
    *(tgt->data) = new_one;
    return 1;
}


int removeTeam(Team **phead, Team *tgt) {
    if (tgt == NULL) {   // 无效参数
        #if defined(DEBUG)
        puts("[LOG] Error in removeTeam():\n\ttarget is NULL");
        #endif
        return 0;
    }
    // TODO: 注销该组的时候，同时注销该组运营的所有项目
    if (tgt->child_project_head
            || tgt->child_project_tail) {
        // 该组有正在运行的项目，删除会导致项目信息失效
        #if defined(DEBUG)
        puts("[LOG] Error in removeTeam():\n\tteam has working porjects, can't move");
        #endif
        return 0;
    }

    // prev 为 tgt 之前的节点
    // NOTE: 若链表只有一个节点，prev == phead，prev->next == NULL，prev无效
    Team *prev = *phead;
    for (; prev->next != NULL && prev->next != tgt; prev = prev->next) ;
    // prev->next == tgt || NULL
    // if (prev->next == NULL) { prev = NULL; }

    // 删除节点

    // case 1 - 要删除的节点是头节点
    if (*phead == tgt) {
        *phead = tgt->next;     // 只需将头指针指向下一个节点
        // NOTE: 若tgt是最后一个节点，此时phead会指向NULL
    } else {    // case 2 - 要删除的不是头节点
        // 使链表指向绕开 tgt
        prev->next = tgt->next;  // route around
    }

    // 在母结点数据域注册删除操作
    #if defined(CHILD_COUNTER)
    tgt->parent_depart->data->team_num -= 1;
    #endif
    // 在母结点的指针域注册
    if (tgt->parent_depart->child_team_head == tgt->parent_depart->child_team_tail) {
        // case 1: tgt 是母结点下最后一个子节点，现在母结点没有子节点了
        tgt->parent_depart->child_team_head = NULL;
        tgt->parent_depart->child_team_tail = NULL;
    } else if (tgt->parent_depart->child_team_head == tgt) {
        // case 2: tgt 是母结点子链头节点
        tgt->parent_depart->child_team_head = tgt->next;
    } else if (tgt->parent_depart->child_team_tail == tgt) {
        // case 3: tgt 是母结点子链尾节点
        tgt->parent_depart->child_team_tail = prev;
    }

    #if defined(BUILDING)
    printf("[LOG] removeTeam(): freed %s @ 0x%p\n",
           tgt->data->name, tgt);
    #endif
    free(tgt->data);
    free(tgt);

    return 1;
}


    /**** SELECT ****/

TeamWrapper *getTeamByTeacherNum(Team *start, Team *end, const Where cond) {
    // (auto-indent fixer)
    // TeamWrapper中会有多个结果

    // NOTE: 传进来的 start 只是别名，不要求有指向
    TeamWrapper *rtn = (TeamWrapper *)malloc(sizeof(TeamWrapper));
    if (rtn == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in getTeamByTeacherNum():\n\tfailed to malloc for result mounting point");
        #endif
        return NULL;
    }

    TeamWrapper *rtn_head = rtn;
    rtn_head->team = NULL; rtn_head->next = NULL;
    if (start->data == NULL) {
        #if defined(DEBUG)
        puts("[LOG] getTeamByTeacherNum(): searching an empty chain");
        #endif
        return rtn_head;
    }

    // set judger
    int (*judger)(int, int);
    judger = setJudger(cond.direction);
    if (judger == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in getTeamByTeacherNum():\n\tunrecoginized condition");
        #endif
        return NULL;
    }
    // #if defined(BUILDING)
    // puts("[LOG] getTeamByTeacherNum(): judger() set");
    // #endif
    // start finding matching nodes
    for (; start != end; start = start->next) {
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
                    // cleanup before leaving
                    cleanupTeamWrapper(rtn_head);
                    return NULL;
                }   // finished verifying malloc-ed space
                rtn = rtn->next; rtn->next = NULL;
                rtn->team = start;
            }   // end of non-head node processing

        }   // finished judging current node
        #if defined(BUILDING)
        puts("[LOG] getTeamByTeacherNum(): moving to next node");
        #endif
    }   // al done!
    // return result container
    return rtn_head;
}



TeamWrapper *getTeamByName(Team *start, Team *end, const char *hint) {
    TeamWrapper *rtn = (TeamWrapper *)malloc(sizeof(TeamWrapper));
    if (rtn == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in getTeamByName():\n\tfailed to malloc for result mounting point");
        #endif
        return NULL;
    }

    TeamWrapper *rtn_head = rtn;
    rtn_head->team = NULL; rtn_head->next = NULL;
    if (start->data == NULL) {
        #if defined(DEBUG)
        puts("[LOG] getTeamByName(): searching an empty chain");
        #endif
        return rtn_head;
    }

    for (; start != end; start = start->next) {
        if (strstr(start->data->name, hint) != NULL) {
            #if defined(BUILDING)
            printf("[LOG] getTeamByName(): found %s @ 0x%p\n",
                   start->data->name, start);
            #endif
            if (rtn_head->team == NULL) {
                rtn->team = start;
            } else {
                rtn->next = (TeamWrapper *)malloc(sizeof(TeamWrapper));
                if (rtn->next == NULL) {
                    #if defined(DEBUG)
                    puts("[LOG] Error in getTeamByName():\n\tfailed to malloc for result container");
                    #endif
                    // cleanup before leaving
                    cleanupTeamWrapper(rtn_head);
                    return NULL;
                }
                rtn = rtn->next; rtn->next = NULL;
                rtn->team = start;
            }
        }
    }
    return rtn_head;
}

TeamWrapper *getTeamByDepart(Depart *parent_depart) {
    // 创建结果挂载点
    TeamWrapper *rtn_head = (TeamWrapper *)malloc(sizeof(TeamWrapper));
    if (rtn_head == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in getTeamByDepart():\n\tfailed to malloc for result maunting point");
        #endif
        return NULL;
    }
    rtn_head->team = NULL; rtn_head->next = NULL;
    // 母结点没有子节点
    if (parent_depart->child_team_head == NULL) {
        return rtn_head;    // 返回空结果
    }
    // 母结点下有子节点
    Team *cur = parent_depart->child_team_head;
    TeamWrapper *rtn = rtn_head;
    for (; cur != parent_depart->child_team_tail->next; cur = cur->next) {
        #if defined(DEBUG)
        printf("[LOG] getTeamByDepart(): reached %s @ 0x%p\n",
               cur->data->name, cur);
        #endif
        if (rtn_head->team == NULL) {
            rtn->team = cur;
        } else {
            rtn->next = (TeamWrapper *)malloc(sizeof(TeamWrapper));
            if (rtn->next == NULL) {
                #if defined(DEBUG)
                puts("[LOG] Error in getTeamByDepart():\n\tfailed to malloc for result container");
                #endif
                cleanupTeamWrapper(rtn_head);
                return NULL;
            }
            rtn = rtn->next;
            rtn->team = cur; rtn->next = NULL;
        }
    }
    return rtn_head;
}


    /**** Stat ****/

TeamStatWrapper *buildTeamStatChainUnordered(Team *start, Team *end) {
    TeamStatWrapper *unordered = (TeamStatWrapper *)malloc(sizeof(TeamStatWrapper));
    if (unordered == NULL) { return NULL; }
    if (start->data == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in getTeamOrderedByMasterTeacherRatio(): building with an empty chain");
        #endif
        unordered->team = NULL; unordered->next = NULL;
        return unordered;
    }
    // constructing unordered chain
    Team *start_bak = start; TeamStatWrapper *unordered_bak = unordered;
    {
        // TODO
        unordered_bak->team = start_bak; unordered_bak->next = NULL;
        unordered_bak->stat.project_total = 0; unordered_bak->stat.project_NSFC = 0;
        unordered_bak->stat.funding = 0;
        // Project
        Project *child_project = start_bak->child_project_head;
        if (child_project != NULL) {
            for (; child_project != start_bak->child_project_tail->next; child_project = child_project->next) {
                unordered_bak->stat.project_total += 1;
                unordered_bak->stat.funding += child_project->data->funding;
                if (child_project->data->type == '2') { unordered_bak->stat.project_NSFC += 1; }
            }
        }
    }
    for (start_bak = start_bak->next; start_bak != end; start_bak = start_bak->next) {
        unordered_bak->next = (TeamStatWrapper *)malloc(sizeof(TeamStatWrapper));
        if (unordered_bak->next == NULL) { return NULL; }
        unordered_bak = unordered_bak->next;
        {
            unordered_bak->team = start_bak; unordered_bak->next = NULL;
            unordered_bak->stat.project_total = 0; unordered_bak->stat.project_NSFC = 0;
            unordered_bak->stat.funding = 0;
            // Project
            Project *child_project = start_bak->child_project_head;
            if (child_project != NULL) {
                for (; child_project != start_bak->child_project_tail->next; child_project = child_project->next) {
                    unordered_bak->stat.project_total += 1;
                    unordered_bak->stat.funding += child_project->data->funding;
                    if (child_project->data->type == '2') { unordered_bak->stat.project_NSFC += 1; }
                }
            }
        }
    }
    // start_bak = start; unordered_bak = unordered;
    return unordered;
}

// DepartStatWrapper *orderDepartStatWrapperBySTRatio(DepartStatWrapper *start) {
//     if (start == NULL) { return NULL; }
//     DepartStatWrapper *start_bak = start;
//     DepartStatWrapper *cur = start;
//     Depart *Depart_tmp; DepartStatData DepartStatData_tmp;
//     for (; start_bak->next; start_bak = start_bak->next) {
//         for (cur = start; cur->next; cur = cur->next) {     // 少写点代码，这里就牺牲点效率了
//             if (cur->stat.st_ratio < cur->next->stat.st_ratio) {
//                 // swap depart
//                 Depart_tmp = cur->next->depart;
//                 cur->next->depart = cur->depart;
//                 cur->depart = Depart_tmp;
//                 // swap stat
//                 DepartStatData_tmp = cur->next->stat;
//                 cur->next->stat = cur->stat;
//                 cur->stat = DepartStatData_tmp;
//             }
//         }
//     }
//     return start;
// }

TeamStatWrapper *orderTeamStatWrapperByNSFCProject(TeamStatWrapper *start) {
    if (start == NULL) { return NULL; }
    TeamStatWrapper *start_bak = start;
    TeamStatWrapper *cur = start;
    Team *Team_tmp; TeamStatData TeamStatData_tmp;
    for (; start_bak->next; start_bak = start_bak->next) {
        for (cur = start; cur->next; cur = cur->next) {     // 少写点代码，这里就牺牲点效率了
            if (cur->stat.project_NSFC < cur->next->stat.project_NSFC) {
                // swap team
                Team_tmp = cur->next->team;
                cur->next->team = cur->team;
                cur->team = Team_tmp;
                // swap stat
                TeamStatData_tmp = cur->next->stat;
                cur->next->stat = cur->stat;
                cur->stat = TeamStatData_tmp;
            }
        }
    }
    return start;
}

TeamStatWrapper *orderTeamStatWrapperByProjectTotal(TeamStatWrapper *start) {
    if (start == NULL) { return NULL; }
    TeamStatWrapper *start_bak = start;
    TeamStatWrapper *cur = start;
    Team *Team_tmp; TeamStatData TeamStatData_tmp;
    for (; start_bak->next; start_bak = start_bak->next) {
        for (cur = start; cur->next; cur = cur->next) {     // 少写点代码，这里就牺牲点效率了
            if (cur->stat.project_total < cur->next->stat.project_total) {
                // swap team
                Team_tmp = cur->next->team;
                cur->next->team = cur->team;
                cur->team = Team_tmp;
                // swap stat
                TeamStatData_tmp = cur->next->stat;
                cur->next->stat = cur->stat;
                cur->stat = TeamStatData_tmp;
            }
        }
    }
    return start;
}

    /**** CLEANUPs ****/

void cleanupTeamWrapper(TeamWrapper *prev) {
    if (prev == NULL) {
        #if defined(DEBUG)
        puts("Nothing left to be cleaned");
        #endif
        return;
    }
    TeamWrapper *after = prev;
    while (1) {
        after = prev->next;
        free(prev);
        #if defined(BUILDING)
        printf("[LOG] cleanupTeamWrapper(): freed 0x%p\n", prev);
        #endif
        prev = after;
        if (prev == NULL) { break; }
    }
    return;
}

void cleanupTeamStatWrapper(TeamStatWrapper *prev) {
    if (prev == NULL) {
        #if defined(DEBUG)
        puts("Nothing left to be cleaned");
        #endif
        return;
    }
    TeamStatWrapper *after = prev;
    while (1) {
        after = prev->next;
        free(prev);
        #if defined(BUILDING)
        printf("[LOG] cleanupTeamWrapper(): freed 0x%p\n", prev);
        #endif
        prev = after;
        if (prev == NULL) { break; }
    }
    return;
}

void cleanupTeam(Team *prev) {
    // puts("[LOG] cleanupTeam(): entered");
    if (prev == NULL) {
        #if defined(DEBUG)
        puts("Nothing left to be cleaned");
        #endif
        return;
    }
    Team *after;
    while (1) {
        after = prev->next;
        #if defined(DEBUG)
        printf("[LOG] cleanupTeam(): freed 0x%p\n", prev);
        #endif
        free(prev->data);
        free(prev);
        prev = after;
        if (prev == NULL) { break; }
    }
    return;
}




/******** Unit Test ********/

#if defined(BUILDING)


void printTeamToConsole(Team *VirtusPro) {
    printf("<Team @ 0x%p>\n", VirtusPro);
    printf("\tthis.name = %s\n", VirtusPro->data->name);
    printf("\tthis.manager = %s\n", VirtusPro->data->manager);
    printf("\tthis.teacher_num = %d\n", VirtusPro->data->teacher_num);
    printf("\tthis.student_num = %d\n", VirtusPro->data->student_num);
    printf("\tthis.faculty = %s\n", VirtusPro->data->faculty);
}

void printTeamChainToConsole(Team *head) {
    if (head == NULL) {
        puts("no data!");
        return;
    }
    for (; head; head = head->next) {
        printTeamToConsole(head);
    }
    putchar('\n');
}


void printTeamWrapperToConsole(TeamWrapper *head) {
    printf("<TeamWrapper @ 0x%p>\n", head);
    if (head->team == NULL) {
        puts("\tnot match!");
    } else {
        for (; head; head = head->next) {
            printf("\tfound %s @ 0x%p\n",
                   head->team->data->name, head->team);
            // indent-fixer
        }
    }
    putchar('\n');
}


void main(void) {
    // building test env
    DepartData depart_data_1 = {
        "计算机", "张三", "13322224444"
    };
    Depart depart_1 = {
        &depart_data_1, NULL, NULL, NULL
    };

    DepartData depart_data_2 = {
        "物理", "李四", "13344445555"
    };
    Depart depart_2 = {
        &depart_data_2, NULL, NULL, NULL
    };
    #if defined(CHILD_COUNTER)
    depart_data_1.team_num = 0;
    depart_data_2.team_num = 0;
    #endif

    depart_1.next = &depart_2;
    Depart *Depart_HEAD = &depart_1;

    Team *Team_HEAD = createTeamHead();

    TeamData team_data_1 = {
        "火箭队", "武藏", 1, 2, "计算机"
    };
    TeamData team_data_2 = {
        "银河队", "小次郎", 2, 3, "物理"
    };
    TeamData team_data_3 = {
        "电子队", "洛伦兹", 3, 4, "计算机"
    };


    // initTeamData()
    // pass

    // appendTeam()
    puts("[LOG] adding team \"火箭队\"");
    appendTeam(Team_HEAD, team_data_1, Depart_HEAD);
    puts("[LOG] adding team \"银河队\"");
    appendTeam(Team_HEAD, team_data_2, Depart_HEAD);
    puts("[LOG] adding team \"电子队\"");
    appendTeam(Team_HEAD, team_data_3, Depart_HEAD);

    puts("Expecting sequence:\n\t火箭队 --> 电子队 --> 银河队");
    printTeamChainToConsole(Team_HEAD);

    // modifyTeam()
    TeamData team_data_4 = {
        "电子队", "库仑", 3, 4, "计算机"
    };
    modifyTeam(Team_HEAD->next, team_data_4);
    printTeamChainToConsole(Team_HEAD);

    // getTeamByTeacherNum()
    TeamWrapper *Team_Wrapper_HEAD;
    Where cond = {">=", 2};
    Team_Wrapper_HEAD = getTeamByTeacherNum(Team_HEAD, NULL, cond);
    printTeamWrapperToConsole(Team_Wrapper_HEAD);
    cleanupTeamWrapper(Team_Wrapper_HEAD);

    // getTeamByName()
    Team_Wrapper_HEAD = getTeamByName(Team_HEAD, NULL, "火箭");
    printTeamWrapperToConsole(Team_Wrapper_HEAD);
    cleanupTeamWrapper(Team_Wrapper_HEAD);

    // removeTeam()
    puts("[LOG] removing team No.1");
    puts("Expecting sequence:\n\t电子队 --> 银河队");
    removeTeam(&Team_HEAD, Team_HEAD);
    printTeamChainToConsole(Team_HEAD);
    puts("[LOG] removing team No.3");
    puts("Expecting sequence:\n\t电子队");
    removeTeam(&Team_HEAD, Team_HEAD->next);
    printTeamChainToConsole(Team_HEAD);
    puts("[LOG] removing the last node");
    puts("Expecting literally NOTHING");
    removeTeam(&Team_HEAD, Team_HEAD);
    printTeamChainToConsole(Team_HEAD);

    cleanupTeam(Team_HEAD);

}


#endif

#ifdef BUILDING
#undef BUILDING
#endif

#ifdef DEBUG
#undef DEBUG
#endif
