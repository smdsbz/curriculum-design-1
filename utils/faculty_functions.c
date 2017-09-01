/*  $PROGRAM_ROOT/utils/faculty_functions.c
 *  院系方法定义与实现
 */

#ifndef DATA_STRUCTURE
#include "data_structure.h"
#endif

#ifdef BUILDING
#undef BUILDING
#endif
// #define BUILDING

#ifdef DEBUG
#undef DEBUG
#endif
// 解注释以查看DEBUG信息
// #define DEBUG

/************ Declaration ************/
// 函数调用说明见头文件
    /**** POST | DELETE | PUT ****/
Depart *appendDepart(Depart *head, DepartData new_one);
int modifyDepart(Depart *target, DepartData new_one);
int removeDepart(Depart **phead, Depart *target);
DepartData initDepartData(void);
Depart *createDepartHead(void);
    /**** SELECT ****/
DepartWrapper *getDepartByManager(Depart *, Depart *, const char *);
DepartWrapper *getDepartByName(Depart *, Depart *, const char *);
DepartStatWrapper *buildDepartStatChainUnordered(Depart *start, Depart *end, int);
DepartStatWrapper *orderDepartStatWrapperBySTRatio(DepartStatWrapper *start);
DepartStatWrapper *orderDepartStatWrapperByProjectTotal(DepartStatWrapper *start);
DepartStatWrapper *orderDepartStatWrapperByAvgFunding(DepartStatWrapper *start);
Depart *getPrevDepart(Depart *cur, Depart *head);
    /**** CLEANUPs ****/
void cleanupDepartWrapper(DepartWrapper *start);
void cleanupDepart(Depart *start);

/*********** Unit Test **************/
#if defined(BUILDING)

void printDepartToConsole(Depart *Comp) {
/*  向终端打印院系信息
 *  ARGS:   院系链表头
 *  RETN:   void
 */
    printf("<Department @ 0x%p>\n", Comp);
    printf("\tthis.name = %s\n", Comp->data->name);
    printf("\tthis.manager = %s\n", Comp->data->manager);
    printf("\t\tthis.mobile = %s\n", Comp->data->mobile);
    #if defined(CHILD_COUNTER)
    printf("\tthis.team_num = %d\n", Comp->data->team_num);
    #endif
    printf("\n\tnext_node @ 0x%p\n", Comp->next);
    printf("\tteam from 0x%p to 0x%p\n", Comp->child_team_head, Comp->child_team_tail);
    putchar('\n'); putchar('\n');
}

void printDepartWrapperToConsole(DepartWrapper *head) {
/*  向终端打印院系搜索结果
 *  ARGS:   院系搜索结果链表头
 *  RETN:   void
 */
    printf("<DepartWrapper @ 0x%p>\n", head);
    int counter = 0;
    while (head->depart != NULL) {
        ++counter;
        printf("\tfound %s @ 0x%p\n", head->depart->data->name, head->depart);
        head = head->next;
        if (head == NULL) { break; }
    }
    if (!counter) { puts("no match!"); }
    putchar('\n'); putchar('\n');
}

// 单元测试
void main(void) {
    Depart *HEAD = createDepartHead();
    // first node creation demo
    appendDepart(HEAD, initDepartData());
    printDepartToConsole(HEAD);
    // second node creation demo
    appendDepart(HEAD, initDepartData());
    printDepartToConsole(HEAD->next);
    // query demo
    auto char buf[40] = {'\0'};
    printf("search by manager: "); scanf("%s", buf);
    DepartWrapper *RST_LIST = getDepartByManager(HEAD, NULL, buf);
    printDepartWrapperToConsole(RST_LIST);
    cleanupDepartWrapper(RST_LIST);
    // search demo
    printf("search by name: "); scanf("%s", buf);
    RST_LIST = getDepartByName(HEAD, NULL, buf);
    printDepartWrapperToConsole(RST_LIST);
    cleanupDepartWrapper(RST_LIST);
    // remove demo
    puts("removing HEAD->next");
    removeDepart(&HEAD, HEAD->next);
    printDepartToConsole(HEAD);
    // end of test
    free(HEAD->next);
    free(HEAD);
}
#endif

/*************** Function Realizations ***************/

    /**** POST | DELETE | PUT ****/

/* 创建并初始化头节点 */
Depart *createDepartHead(void) {
    Depart *HEAD = (Depart *)malloc(sizeof(Depart));
    if (HEAD == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in createDepartHead():\n\tfailed to malloc for depart (head)");
        #endif
        return NULL;
    }
    HEAD->data = NULL; HEAD->next = NULL;
    HEAD->child_team_head = NULL; HEAD->child_team_tail = NULL;
    return HEAD;
}

/* 创建院系数据模板 */
DepartData initDepartData(void) {
    DepartData Computer;
    printf("add/depart::name > "); scanf("%s", Computer.name);
    printf("add/depart::manager > "); scanf("%s", Computer.manager);
    printf("add/depart::mobile > "); scanf("%s", Computer.mobile);
    #if defined(CHILD_COUNTER)
    Computer.team_num = 0;
    #endif
    return Computer;
}

/* 添加院系数据 */
Depart *appendDepart(Depart *head, DepartData new_one) {
    // 检查是否已经有同名的院系存在
    DepartWrapper *depart_wrapper = \
        getDepartByName(head, NULL, new_one.name);
    if (depart_wrapper->depart != NULL) {
        // NOTE: 这里实际上不是同名
        // 若已经有Computer，则Comp也会在这里被拦截
        printf("Record for %s department already exists!\n", new_one.name);
        cleanupDepartWrapper(depart_wrapper);
        return NULL;
    }
    cleanupDepartWrapper(depart_wrapper);
    // 获取院系链表尾节点
    Depart *tail = head;
    for (; tail->next; tail = tail->next) ;
    // 开始添加数据
    // case 1 - 院系链表此时还没有任何记录
    if (head == tail && tail->data == NULL) {
        // 只需要修改头节点的数据域
        // NOTE: 此时头节点的数据域还没有内存空间
        tail->data = (DepartData *)malloc(sizeof(DepartData));
        if (tail->data == NULL) {
            #if defined(DEBUG)
            puts("[LOG] Error in appendDepart():\n\tfailed to alloc memory for data (head)");
            #endif
            return NULL;
        }
        // 院系数据赋值
        *(tail->data) = new_one; tail->next = NULL;
        tail->child_team_head = NULL; tail->child_team_tail = NULL;
        return tail;    // 返回新添加的节点
    }
    // else { case 2 - 院系链表非空 }
    // 在尾部链接新节点
    // 为新节点申请内存空间
    tail->next = (Depart *)malloc(sizeof(Depart));
    if (tail->next == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in appendDepart():\n\tfailed to alloc memory for container");
        #endif
        return NULL;
    }
    // 为新节点的数据域申请内存空间
    tail->next->data = (DepartData *)malloc(sizeof(DepartData));
    if (tail->next->data == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in appendDepart():\n\tfailed to alloc memory for data");
        #endif
        return NULL;
    }
    *(tail->next->data) = new_one; tail->next->next = NULL;
    tail->next->child_team_head = NULL;
    tail->next->child_team_tail = NULL;
    return tail->next;   // 返回新节点地址
}

/* 覆盖院系信息 - 未调用 */
int modifyDepart(Depart *target, DepartData new_one) {
    if (target == NULL) {   // 输入检查
        #if defined(DEBUG)
        puts("[LOG] Error in modifyDepart():\n\ttarget is NULL");
        #endif
        return 0;   // error code
    }
    // 数据覆盖
    *(target->data) = new_one;
    return 1;
}

/* 删除院系节点 */
int removeDepart(Depart **phead, Depart *tgt) {
    if (tgt == NULL) {   // 输入检查
        #if defined(DEBUG)
        puts("[LOG] Error in removeDepart():\n\ttgt is NULL");
        #endif
        return 0;
    }
    // 检查院系是否有团队，防止团队信息丢失
    if (tgt->child_team_head) {
        puts("The department you are deleting has affiliated teams!");
        return 0;
    }
    // 使链表绕开要删除的节点
    // case 1 - 要删除的节点是头节点且不是唯一结点
    if (*phead == tgt && tgt->next) {
        *phead = tgt->next; // 头节点重新赋值
    } else { // case 2 - 要删除的节点不是头节点（故不可能为唯一节点）
        // NOTE: 若tgt是唯一的节点，phead不能指向NULL
        Depart *phead_safe = *phead;
        // 遍历找到目标节点之前的节点
        for (; phead_safe->next != tgt; phead_safe = phead_safe->next) ;
        phead_safe->next = tgt->next;
    }
    // 删除节点
    free(tgt->data); // 释放数据域
    // case 3 - 唯一结点，需要保留链表挂载点
    if (tgt->next == NULL && *phead == tgt) { tgt->data = NULL; }
    else { free(tgt); }    // 释放节点本身占用的空间
    return 1;
}

    /**** CLEANUPs ****/

/* 清除搜索结果链表 */
void cleanupDepartWrapper(DepartWrapper *prev) {
    if (prev == NULL) {     // 输入检查
        #if defined(DEBUG)
        puts("[LOG] cleanupDepartWrapper(): got NULL");
        #endif
        return;
    }
    DepartWrapper *after = prev;
    while (1) {
        after = prev->next; // 备份下一个节点位置
        free(prev);         // 释放当前节点
        #if defined(DEBUG)
        printf("[LOG] cleanupDepartWrapper(): freed 0x%p\n", prev);
        #endif
        prev = after;       // 准备删除下一个节点
        if (prev == NULL) { break; }   // 已经全部清空
    }
    return;
    // NOTE: 此时，传进来的头节点也没了
}

/* 清除统计结果链表 */
void cleanupDepartStatWrapper(DepartStatWrapper *prev) {
    if (prev == NULL) {
        #if defined(DEBUG)
        puts("[LOG] cleanupDepartStatWrapper(): got NULL");
        #endif
        return;
    }
    DepartStatWrapper *after = prev;
    while (1) {
        after = prev->next;
        free(prev);
        #if defined(DEBUG)
        printf("[LOG] cleanupDepartStatWrapper(): freed 0x%p\n", prev);
        #endif
        prev = after;
        if (prev == NULL) { break; }
    }
    return;
}

/* 释放院系链所占用的内存空间 */
void cleanupDepart(Depart *prev) {
    if (prev == NULL) {
        #if defined(DEBUG)
        puts("[LOG] cleanupDepart(): got NULL");
        #endif
        return;
    }
    Depart *after = prev;
    while (1) {
        after = prev->next;
        // 这里不光要释放节点本身的内存，还要释放数据域的空间
        free(prev->data); free(prev);
        #if defined(DEBUG)
        printf("[LOG] cleanupDepart(): freed 0x%p\n", prev);
        #endif
        prev = after;
        if (prev == NULL) { break; }
    }
    return;
}

    /**** SELECT ****/

/* 通过负责人姓名查找院系 */
DepartWrapper *
getDepartByManager(Depart *start, Depart *end, const char *manager) {
    // 创建搜索结果挂载点
    DepartWrapper *rtn = (DepartWrapper *)malloc(sizeof(DepartWrapper));
    if (rtn == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in getDepartByManager():\n\tfailed to malloc for result mounting point");
        #endif
        return NULL;
    }
    DepartWrapper *rtn_head = rtn;  // 保存搜索结果链的头指针
    rtn_head->depart = NULL; rtn_head->next = NULL;   // 初始化
    if (start->data == NULL) {  // 检查院系链是否有数据
        #if defined(DEBUG)
        puts("[LOG] getDepartByManager(): searching an empty chain");
        #endif
        // 若无数据，返回没有任何数据的挂载点
        return rtn_head;
    }
    // 遍历查找
    for (; start != end; start = start->next) {
        if (!strcmp(start->data->manager, manager)) {  // BINGO!
            #if defined(BUILDING)
            printf("[LOG] getDepartByManager(): found %s @ 0x%p\n", start->data->name, start);
            #endif
            // 将当前记录添加至搜索结果链表
            // case 1 - 此前没有找到任何满足要求的链表
            if (rtn_head->depart == NULL) {
                // 向挂载点写入数据
                rtn->depart = start;
            } else {    // case 2 - 已经有结果了
                // 为新结果申请空间
                rtn->next = (DepartWrapper *)malloc(sizeof(DepartWrapper));
                if (rtn->next == NULL) {
                    #if defined(DEBUG)
                    puts("[LOG] Error in getDepartByManager():\n\tfailed to malloc for result container");
                    #endif
                    // 申请空间失败则中断操作，并清空搜索结果，返回NULL
                    cleanupDepartWrapper(rtn_head);
                    return NULL;
                }
                // 申请成功，注册搜索结果
                rtn = rtn->next; rtn->next = NULL;
                rtn->depart = start;
            }

        }
    }
    return rtn_head;    // 不管有没有找到，都返回搜索结果链表头
}

/* 通过院系名称查找院系 */
// 查询函数实现都极为相似，此后只在必要时给出注释
DepartWrapper *
getDepartByName(Depart *start, Depart *end, const char *name) {
    DepartWrapper *rtn = (DepartWrapper *)malloc(sizeof(DepartWrapper));
    if (rtn == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in getDepartByName():\n\tfailed to malloc for result mounting point");
        #endif
        return NULL;
    }
    DepartWrapper *rtn_head = rtn;
    rtn_head->depart = NULL; rtn_head->next = NULL;
    if (start->data == NULL) {
        #if defined(DEBUG)
        puts("[LOG] getDepartByName(): searching an empty chain");
        #endif
        return rtn_head;
    }
    for (; start != end; start = start->next) {
        // 和上一个函数唯一的不同
        if (strstr(start->data->name, name)) {  // 搜索内容是已保存内容的子串
            #if defined(DEBUG)
            printf("[LOG] getDepartByName(): found %s @ 0x%p\n", start->data->name, start);
            #endif
            if (rtn_head->depart == NULL) {
                rtn->depart = start;
            } else {
                rtn->next = (DepartWrapper *)malloc(sizeof(DepartWrapper));
                if (rtn->next == NULL) {
                    #if defined(DEBUG)
                    puts("[LOG] Error in getDepartByName():\n\tfailed to malloc for result container");
                    #endif
                    cleanupDepartWrapper(rtn_head);
                    return NULL;
                }
                rtn = rtn->next; rtn->next = NULL;
                rtn->depart = start;
            }

        }
    }
    return rtn_head;
}

    /**** Stat ****/

/* 统计所有院系数据 */
DepartStatWrapper *
buildDepartStatChainUnordered(Depart *start, Depart *end, int year) {
    // 创建统计结果链挂载点
    DepartStatWrapper *unordered = \
        (DepartStatWrapper *)malloc(sizeof(DepartStatWrapper));
    if (unordered == NULL) { return NULL; }
    if (start->data == NULL) {  // 检查传入链表是否有数据
        #if defined(DEBUG)
        puts("[LOG] Error in getDepartOrderedByMasterTeacherRatio(): building with an empty chain");
        #endif
        unordered->depart = NULL; unordered->next = NULL;
        return unordered;
    }
    // 开始统计
    Depart *start_bak = start; DepartStatWrapper *unordered_bak = unordered;
    char year_str[10];  // 由于项目创建时间以字符串形式存储，在此设立buffer
    // 创建第一条统计数据
    // NOTE: <do-while>标识的区域内的内容完全相同
    // <do-while>
    // 节点统计数据初始化
    unordered_bak->depart = start_bak; unordered_bak->next = NULL;
    unordered_bak->stat.student_num = 0; unordered_bak->stat.teacher_num = 0;
    unordered_bak->stat.project_total = 0; unordered_bak->stat.project_973 = 0;
    unordered_bak->stat.project_863 = 0; unordered_bak->stat.funding = 0;
    // 遍历团队
    Team *child_team = start_bak->child_team_head;
    if (child_team != NULL) {   // 检查是否有下属团队，防止跨界访问
        for (; child_team != start_bak->child_team_tail->next;
               child_team = child_team->next) {
            // 统计学生、教师人数
            unordered_bak->stat.student_num += child_team->data->student_num;
            unordered_bak->stat.teacher_num += child_team->data->teacher_num;
            // 遍历项目
            Project *child_project = child_team->child_project_head;
            if (child_project != NULL) {    // 同上，防止跨界访问
                for (; child_project != child_team->child_project_tail->next;
                       child_project = child_project->next) {
                    sprintf(year_str, "%d", year);
                    if (year == 0   // 当用户输入的年份为0时，统计所有数据
                            || strstr(child_project->data->start_date,
                                      year_str) != NULL) {
                        // NOTE: 由于记录时间的字符串位数不多，直接判断输入年份是否为字串即可
                        unordered_bak->stat.project_total += 1;
                        if (child_project->data->type == '1')
                            { unordered_bak->stat.project_973 += 1; }
                        if (child_project->data->type == '3')
                            { unordered_bak->stat.project_973 += 1; }
                        unordered_bak->stat.funding += child_project->data->funding;
                    }
                }
            }
        }
    }
    // 计算学生/教师人数比
    if (unordered_bak->stat.teacher_num == 0)   // 防止零除数
        { unordered_bak->stat.st_ratio = -1; }
    else {
        unordered_bak->stat.st_ratio = \
            (float)unordered_bak->stat.student_num \
            / unordered_bak->stat.teacher_num;
    }
    // 计算项目平均经费
    if (unordered_bak->stat.project_total == 0) // 同上，防止零除数
        { unordered_bak->stat.avg_funding = -1; }
    else {
        unordered_bak->stat.avg_funding = \
            unordered_bak->stat.funding \
            / unordered_bak->stat.project_total;
    }
    // </do-while>
    for (start_bak = start_bak->next; start_bak != end; start_bak = start_bak->next) {
        unordered_bak->next = (DepartStatWrapper *)malloc(sizeof(DepartStatWrapper));
        if (unordered_bak->next == NULL) { return NULL; }
        unordered_bak = unordered_bak->next;
        // <do-while>
        unordered_bak->depart = start_bak; unordered_bak->next = NULL;
        unordered_bak->stat.student_num = 0; unordered_bak->stat.teacher_num = 0;
        unordered_bak->stat.project_total = 0; unordered_bak->stat.project_973 = 0;
        unordered_bak->stat.project_863 = 0; unordered_bak->stat.funding = 0;
        Team *child_team = start_bak->child_team_head;
        if (child_team != NULL) {
            for (; child_team != start_bak->child_team_tail->next;
                   child_team = child_team->next) {
                unordered_bak->stat.student_num += child_team->data->student_num;
                unordered_bak->stat.teacher_num += child_team->data->teacher_num;
                Project *child_project = child_team->child_project_head;
                if (child_project != NULL) {
                    for (; child_project != child_team->child_project_tail->next;
                           child_project = child_project->next) {
                        sprintf(year_str, "%d", year);
                        if (year == 0   // 当用户输入的年份为0时，统计所有数据
                                || strstr(child_project->data->start_date,
                                          year_str) != NULL) {
                            unordered_bak->stat.project_total += 1;
                            if (child_project->data->type == '1')
                                { unordered_bak->stat.project_973 += 1; }
                            if (child_project->data->type == '3')
                                { unordered_bak->stat.project_973 += 1; }
                            unordered_bak->stat.funding += child_project->data->funding;
                        }
                    }
                }
            }
        }
        if (unordered_bak->stat.teacher_num == 0)
            { unordered_bak->stat.st_ratio = -1; }
        else {
            unordered_bak->stat.st_ratio = \
                (float)unordered_bak->stat.student_num \
                / unordered_bak->stat.teacher_num;
        }
        if (unordered_bak->stat.project_total == 0)
            { unordered_bak->stat.avg_funding = -1; }
        else {
            unordered_bak->stat.avg_funding = \
                unordered_bak->stat.funding \
                / unordered_bak->stat.project_total;
        }
        // </do-while>
    }
    return unordered;
}

/* 将统计结果按学生/教师人数比排序 */
DepartStatWrapper *
orderDepartStatWrapperBySTRatio(DepartStatWrapper *start) {
    if (start == NULL) { return NULL; } // 输入检查
    DepartStatWrapper *start_bak = start;
    DepartStatWrapper *cur = start;
    Depart *Depart_tmp; DepartStatData DepartStatData_tmp;
    // 冒泡排序
    for (; start_bak->next; start_bak = start_bak->next) {
        for (cur = start; cur->next; cur = cur->next) {
            // ☝ 图个方便，这里就牺牲点效率了
            if (cur->stat.st_ratio < cur->next->stat.st_ratio) {
                // 交换院系
                Depart_tmp = cur->next->depart;
                cur->next->depart = cur->depart;
                cur->depart = Depart_tmp;
                // 交换统计信息
                DepartStatData_tmp = cur->next->stat;
                cur->next->stat = cur->stat;
                cur->stat = DepartStatData_tmp;
            }
        }
    }
    return start;
}

/* 将统计结果按项目总数降序排序 */
DepartStatWrapper *
orderDepartStatWrapperByProjectTotal(DepartStatWrapper *start) {
    if (start == NULL) { return NULL; }
    DepartStatWrapper *start_bak = start;
    DepartStatWrapper *cur = start;
    Depart *Depart_tmp; DepartStatData DepartStatData_tmp;
    for (; start_bak->next; start_bak = start_bak->next) {
        for (cur = start; cur->next; cur = cur->next) {
            if (cur->stat.project_total < cur->next->stat.project_total) {
                // swap depart
                Depart_tmp = cur->next->depart;
                cur->next->depart = cur->depart;
                cur->depart = Depart_tmp;
                // swap stat
                DepartStatData_tmp = cur->next->stat;
                cur->next->stat = cur->stat;
                cur->stat = DepartStatData_tmp;
            }
        }
    }
    return start;
}

/* 将统计结果按项目平均经费降序排序 */
DepartStatWrapper *
orderDepartStatWrapperByAvgFunding(DepartStatWrapper *start) {
    if (start == NULL) { return NULL; }
    DepartStatWrapper *start_bak = start;
    DepartStatWrapper *cur = start;
    Depart *Depart_tmp; DepartStatData DepartStatData_tmp;
    for (; start_bak->next; start_bak = start_bak->next) {
        for (cur = start; cur->next; cur = cur->next) {
            if (((float)cur->stat.funding / cur->stat.project_total) < ((float)cur->next->stat.funding / cur->next->stat.project_total)) {
                // swap depart
                Depart_tmp = cur->next->depart;
                cur->next->depart = cur->depart;
                cur->depart = Depart_tmp;
                // swap stat
                DepartStatData_tmp = cur->next->stat;
                cur->next->stat = cur->stat;
                cur->stat = DepartStatData_tmp;
            }
        }
    }
    return start;
}

/* 获得当前院系节点的前一个节点 - 未调用*/
Depart *getPrevDepart(Depart *cur, Depart *head) {
    for (; head->next != cur && head != NULL;
           head = head->next) ;
    return head;
}

#ifdef BUILDING
#undef BUILDING
#endif

#ifdef DEBUG
#undef DEBUG
#endif
