/*  $PROGRAM_ROOT/utils/team_functions.c
 *  团队方法定义与实现
 */

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

/* 教师人数条件判断器 */
int isSmaller(int valve, int data) { return (data < valve); }
int isLarger(int valve, int data) { return (data > valve); }
int isEqualTo(int valve, int data) { return (data == valve); }
int noLargerThan(int valve, int data) { return (data <= valve); }
int noSmallerThan(int valve, int data) { return (data >= valve); }

int (*setJudger(const char *direction))(int, int) {
/*  教师人数条件判断选择器
 *  ARGS:   代表搜索规则的字符串(eg "<=")
 *  RETN:   int judger(int valve, int data)
 *          实际为上面申明的函数中的一种
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
Team *appendTeam(Team *, TeamData, Depart *);
int modifyTeam(Team *, TeamData);
int removeTeam(Team **, Team *);
Team *createTeamHead(void);
    /**** SELECT ****/
TeamWrapper *getTeamByTeacherNum(Team *, Team *, const Where);
TeamWrapper *getTeamByName(Team *, Team *, const char *);
TeamStatWrapper *buildTeamStatChainUnordered(Team *, Team *, char);
TeamStatWrapper *orderTeamStatWrapperByNSFCProject(TeamStatWrapper *);
TeamStatWrapper *orderTeamStatWrapperByPTRatio(TeamStatWrapper *);
    /**** CLEANUPs ****/
void cleanupTeamWrapper(TeamWrapper *);
void cleanupTeam(Team *);
void cleanupTeamStatWrapper(TeamStatWrapper *);

/************ Function Realizations *************/
// NOTE: 大部分实现与院系方法实现相同，下面只在必要处给出注释

    /**** POST | DELETE | PUT ****/

/* 创建并初始化头节点 */
Team *createTeamHead(void) {
    Team *Team_HEAD = (Team *)malloc(sizeof(Team));
    if (Team_HEAD == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in createTeamHead():\n\tfailed to malloc for team (head)");
        #endif
        return NULL;
    }
    Team_HEAD->data = NULL; Team_HEAD->next = NULL;
    Team_HEAD->parent_depart = NULL;    // 母节点
    Team_HEAD->child_project_head = NULL;
    Team_HEAD->child_project_tail = NULL;
    return Team_HEAD;
}

/* 创建团队数据模板 */
TeamData initTeamData(void) {
    TeamData VirtusPro;
    printf("add/team::name > "); scanf("%s", VirtusPro.name);
    printf("add/team::manager > "); scanf("%s", VirtusPro.manager);
    printf("add/team::teacher_num > "); scanf("%d", &(VirtusPro.teacher_num));
    printf("add/team::student_num > "); scanf("%d", &(VirtusPro.student_num));
    printf("add/team::faculty > "); scanf("%s", VirtusPro.faculty);
    return VirtusPro;
}

/* 添加团队数据 */
Team *appendTeam(Team *head, TeamData new_one, Depart *depart_chain) {
    TeamWrapper *team_wrapper = getTeamByName(head, NULL, new_one.name);
    if (team_wrapper->team != NULL) {
        printf("Record for %s team already exists!\n", new_one.name);
        cleanupTeamWrapper(team_wrapper);
        return NULL;
    }
    cleanupTeamWrapper(team_wrapper);
    Team *tail = head;
    for (; tail->next; tail = tail->next) ;
    DepartWrapper *parent_depart_wrapper = \
        getDepartByName(depart_chain, NULL, new_one.faculty);
    if (parent_depart_wrapper == NULL) {    // 查找失败
        #if defined(DEBUG)
        puts("[LOG] appendTeam():\n\tgetDepartByName() failed");
        #endif
        return NULL;
    }
    if (parent_depart_wrapper->depart == NULL) {    // 没有符合要求的结果
        puts("Target parent department not found!\n");
        return NULL;
    }
    if (parent_depart_wrapper->next != NULL) {  // 找到了多个符合要求的结果
        puts("Multiple parent departments found!\n");
        return NULL;
    }
    if (strcmp(parent_depart_wrapper->depart->data->name,
               new_one.faculty)) {
        // 由于根据名称查找院系是模糊查询，这里需要做一次校验
        printf("Department named %s not found, do you mean %s instead?\n",
               new_one.faculty, parent_depart_wrapper->depart->data->name);
        return NULL;
    }
    // 此时，已经验证了 parent_depart_wrapper 中只有一个院系
    Depart *parent_depart = parent_depart_wrapper->depart;
    cleanupDepartWrapper(parent_depart_wrapper);
    #if defined(BUILDING)
    printf("[LOG] appendTeam(): parent_depart is %s @ 0x%p\n",
           parent_depart->data->name, parent_depart);
    #endif
    // 添加节点
    //   case 1: 团队链表中目前还没有数据
    if (tail == head && tail->data == NULL) {
        #if defined(DEBUG)
        puts("[LOG] appendTeam(): case 1");
        #endif
        tail->data = (TeamData *)malloc(sizeof(TeamData));
        if (tail->data == NULL) {
            #if defined(DEBUG)
            puts("[LOG] Error in appendTeam():\n\tfailed to alloc memory for data (head)");
            #endif
            return NULL;
        }
        *(tail->data) = new_one;
        tail->next = NULL;
        tail->child_project_head = NULL; tail->child_project_tail = NULL;
        // 子节点链接母节点
        tail->parent_depart = parent_depart;
        // 在母节点中注册子节点
        parent_depart->child_team_head = tail;
        parent_depart->child_team_tail = tail;
        return tail;
    }
    //   case 2 & 3: 需要添加节点的操作
    if (parent_depart->child_team_tail == NULL
            || parent_depart->child_team_tail->next == NULL) {
        #if defined(DEBUG)
        puts("[LOG] appendTeam(): case 2");
        #endif
        // case 2: 母结点在当前还没有子节点
        //         或者 母结点的团队链的尾就是整个团队链的尾节点
        //         --> 向尾节点 tail 后添加节点
        tail->next = (Team *)malloc(sizeof(Team));
        if (tail->next == NULL) {
            #if defined(DEBUG)
            puts("[LOG] Error in appendTeam():\n\tfailed to alloc memory for container");
            #endif
            return NULL;
        }
        // 给新节点的数据域申请空间
        tail->next->data = (TeamData *)malloc(sizeof(TeamData));
        if (tail->next->data == NULL) {
            #if defined(DEBUG)
            puts("[LOG] Error in appendTeam():\n\tfailed to alloc memory for data");
            #endif
            return NULL;
        }
        // 下一个节点的指向需要在这里设置
        tail->next->next = NULL;
    } else {    // case 3: 母结点已经有子节点了
        #if defined(DEBUG)
        puts("[LOG] appendTeam(): case 3");
        #endif
        // 将tail指向现有子节点链的尾部
        tail = parent_depart->child_team_tail;
        Team *after = tail->next;   // something or NULL
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
        // 下一个节点指向
        tail->next->next = after;
    }
    // 数据域复制
    *(tail->next->data) = new_one;
    // 在母节点中注册
    //   注册指针指向
    //     该团队为母节点的第一个子节点
    if (parent_depart->child_team_head == NULL) {
        parent_depart->child_team_head = tail->next;
    }
    parent_depart->child_team_tail = tail->next;
    // 子节点链接母结点
    tail->next->parent_depart = parent_depart;
    // 下一级节点指向初始化
    tail->next->child_project_head = NULL;
    tail->next->child_project_tail = NULL;
    return tail->next;
}

/* 覆盖团队信息 */
int modifyTeam(Team *tgt, TeamData new_one) {
    if (tgt == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in modifyTeam():\n\ttarget is NULL");
        #endif
        return 0;
    }
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

/* 删除团队节点 */
int removeTeam(Team **phead, Team *tgt) {
    if (tgt == NULL) {   // 无效参数
        #if defined(DEBUG)
        puts("[LOG] Error in removeTeam():\n\ttarget is NULL");
        #endif
        return 0;
    }
    if (tgt->child_project_head) {
        // 该组有正在运行的项目，删除会导致项目信息失效，并留下垃圾数据
        #if defined(DEBUG)
        puts("[LOG] Error in removeTeam():\n\tteam has working porjects, can't move");
        #endif
        puts("The team you are deleting has affiliated projects!");
        return 0;
    }
    Team *prev = *phead;
    for (; prev->next != NULL && prev->next != tgt; prev = prev->next) ;
    // 删除节点
    if (*phead == tgt && tgt->next) {
        *phead = tgt->next;
    } else {
        prev->next = tgt->next;
    }
    // 在母结点数据域注册删除操作
    // 在母结点的指针域注册
    if (tgt->parent_depart->child_team_head == \
            tgt->parent_depart->child_team_tail) {
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
    if (tgt->next == NULL && *phead == tgt) { tgt->data = NULL; }
    else { free(tgt); }
    return 1;
}

    /**** SELECT ****/

/* 通过教师数量查找团队 */
TeamWrapper *getTeamByTeacherNum(Team *start, Team *end, const Where cond) {
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
    // 设定条件判断器
    int (*judger)(int, int);
    judger = setJudger(cond.direction);
    if (judger == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in getTeamByTeacherNum():\n\tunrecoginized condition");
        #endif
        printf("Undefined query direction %s!\n", cond.direction);
        return NULL;
    }
    // 开始查询满足条件的节点
    for (; start != end; start = start->next) {
        if (judger(cond.value, start->data->teacher_num)) {
            #if defined(DEBUG)
            printf("[LOG] getTeamByTeacherNum(): found %s @ 0x%p\n",
                   start->data->name, start);
            #endif
            if (rtn_head->team == NULL) {
                rtn->team = start;
            }
            else {
                rtn->next = (TeamWrapper *)malloc(sizeof(TeamWrapper));
                if (rtn->next == NULL) {
                    #if defined(DEBUG)
                    puts("[LOG] Error in getTeamByTeacherNum():\n\tfailed to malloc for result container");
                    #endif
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

/* 通过团队名称查找团队 */
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

/* 通过院系查找团队 */
TeamWrapper *getTeamByDepart(Depart *parent_depart) {
    TeamWrapper *rtn_head = (TeamWrapper *)malloc(sizeof(TeamWrapper));
    if (rtn_head == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in getTeamByDepart():\n\tfailed to malloc for result maunting point");
        #endif
        return NULL;
    }
    rtn_head->team = NULL; rtn_head->next = NULL;
    if (parent_depart->child_team_head == NULL) {
        return rtn_head;
    }
    Team *cur = parent_depart->child_team_head;
    TeamWrapper *rtn = rtn_head;
    // 遍历获得院系下所有团队
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

/* 统计所有团队数据 */
TeamStatWrapper *
buildTeamStatChainUnordered(Team *start, Team *end, char NSFC_flag) {
    TeamStatWrapper *unordered = \
        (TeamStatWrapper *)malloc(sizeof(TeamStatWrapper));
    if (unordered == NULL) { return NULL; }
    if (start->data == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in getTeamOrderedByMasterTeacherRatio(): building with an empty chain");
        #endif
        unordered->team = NULL; unordered->next = NULL;
        return unordered;
    }
    Team *start_bak = start; TeamStatWrapper *unordered_bak = unordered;
    // <do-while>
    unordered_bak->team = start_bak; unordered_bak->next = NULL;
    unordered_bak->stat.project_total = 0;
    unordered_bak->stat.project_NSFC = 0;
    unordered_bak->stat.funding = 0;
    // 统计项目
    Project *child_project = start_bak->child_project_head;
    if (child_project != NULL) {
        for (; child_project != start_bak->child_project_tail->next;
               child_project = child_project->next) {
            unordered_bak->stat.project_total += 1;
            // TODO
            // UGLY: 若NSFC_flag为真，则funding项里只有NSFC经费
            if ((!NSFC_flag) || (NSFC_flag && child_project->data->type=='2'))
                { unordered_bak->stat.funding += child_project->data->funding; }
            if (child_project->data->type == '2')
                { unordered_bak->stat.project_NSFC += 1; }
        }
    }
    if (unordered_bak->team->data->teacher_num) {
        unordered_bak->stat.pt_ratio = \
            (float)unordered_bak->stat.project_total \
            / unordered_bak->team->data->teacher_num;
    } else { unordered_bak->stat.pt_ratio = -1; }
    // </do-while>
    for (start_bak = start_bak->next;
            start_bak != end; start_bak = start_bak->next) {
        unordered_bak->next = (TeamStatWrapper *)malloc(sizeof(TeamStatWrapper));
        if (unordered_bak->next == NULL) { return NULL; }
        unordered_bak = unordered_bak->next;
        // <do-while>
        unordered_bak->team = start_bak; unordered_bak->next = NULL;
        unordered_bak->stat.project_total = 0;
        unordered_bak->stat.project_NSFC = 0;
        unordered_bak->stat.funding = 0;
        Project *child_project = start_bak->child_project_head;
        if (child_project != NULL) {
            for (; child_project != start_bak->child_project_tail->next;
                   child_project = child_project->next) {
                unordered_bak->stat.project_total += 1;
                if ((!NSFC_flag) || (NSFC_flag && child_project->data->type=='2'))
                    { unordered_bak->stat.funding += child_project->data->funding; }
                if (child_project->data->type == '2')
                    { unordered_bak->stat.project_NSFC += 1; }
            }
        }
        if (unordered_bak->team->data->teacher_num) {
            unordered_bak->stat.pt_ratio = \
                (float)unordered_bak->stat.project_total \
                / unordered_bak->team->data->teacher_num;
        } else { unordered_bak->stat.pt_ratio = -1; }
        // </do-while>
    }
    return unordered;
}

/* 将统计结果按NSFC项目数降序排序 */
TeamStatWrapper *orderTeamStatWrapperByNSFCProject(TeamStatWrapper *start) {
    if (start == NULL) { return NULL; }
    TeamStatWrapper *start_bak = start;
    TeamStatWrapper *cur = start;
    Team *Team_tmp; TeamStatData TeamStatData_tmp;
    for (; start_bak->next; start_bak = start_bak->next) {
        for (cur = start; cur->next; cur = cur->next) {
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

/* 将统计结果按项目/教师比降序排序 */
TeamStatWrapper *orderTeamStatWrapperByPTRatio(TeamStatWrapper *start) {
    if (start == NULL) { return NULL; }
    TeamStatWrapper *start_bak = start;
    TeamStatWrapper *cur = start;
    Team *Team_tmp; TeamStatData TeamStatData_tmp;
    for (; start_bak->next; start_bak = start_bak->next) {
        for (cur = start; cur->next; cur = cur->next) {
            if (cur->stat.pt_ratio < cur->next->stat.pt_ratio) {
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

/* 清空搜索结果序列 */
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

/* 清除统计结果链表 */
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

/* 释放Team链所占用的内存空间 */
void cleanupTeam(Team *prev) {
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
