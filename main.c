/*  $PROG_ROOT/main.c
 *  主函数及编译起始点，这里定义了所有的视图函数
 *  NOTE: 所有视图函数均不接受参数，也没有返回值
 */

#include <stdio.h>
#include <stdlib.h>

#include "./utils/__init__.h"   // 数据处理相关
#include "./views.h"            // 视图函数的申明
#include "./doc_strings.h"      // 用户导航帮助说明

// 初始化环境全局变量
MountPoint mp;              // 头节点挂载点组
char TGT_PATH[100] = "./";  // 数据文件夹路径buffer
Cursor cursor = {0, NULL};  // 用户指针


/********** Data **********/

    /**** Listing ****/

/* 列出所有节点 */
void listAllNodes(void) {
    // 判断是否有数据，防止跨界访问
    if (mp.depart_head == NULL) {
        puts("No data!");
        return;
    }
    Depart *d; Team *t; Project *p;
    // 遍历院系
    for (d = mp.depart_head; d != NULL; d = d->next) {
        printf("%s\n", d->data->name);
        if (d->child_team_head) {   // 判断该院系是否有团队
            // 遍历团队
            for (t = d->child_team_head;
                    t != d->child_team_tail->next;
                    t = t->next) {
                printf("    %s\n", t->data->name);
                if (t->child_project_head) {    // 同上
                    // 遍历项目
                    for (p = t->child_project_head;
                            p != t->child_project_tail->next;
                            p = p->next) {
                        printf("        %s\n", p->data->id);
                    }
                }
            }
        }
    }
    putchar('\n');
}

/* 列出院系搜索结果中所有内容 */
void listDepartWrapper(DepartWrapper *head) {
    // 传入参数检查
    if (head == NULL) {
        puts("RUNTIME ERROR!");
        exit(-1);
    }
    if (head->depart == NULL) {
        puts("No record found!");
        return;
    }
    // 输出表头
    puts("\
        Name            |    Manager    |    Telephone\n\
     -------------------|---------------|---------------");
    int num = 1;    // 计数器
    // 遍历搜索结果
    for (; head; head = head->next, ++num) {
        printf("%4d  %-18s|  %-13s|  %s\n",
               num, head->depart->data->name,
               head->depart->data->manager,
               head->depart->data->mobile);
    }
    putchar('\n');
}

/* 列出院系人数统计结果 */
void listDepartHRStat(void) {
    // 获取所有院系的统计结果
    DepartStatWrapper *rst = \
        buildDepartStatChainUnordered(mp.depart_head, NULL, 0);
    if (rst == NULL) {
        puts("RUNTIME ERROR!");
        exit(-1);
    }
    // 将统计结果按学生/教师人数比排序
    orderDepartStatWrapperBySTRatio(rst);
    puts("\
        Name            |    Teachers   |    Students   |    S/T Ratio\n\
     -------------------|---------------|---------------|---------------");
    DepartStatWrapper *head = rst; int counter = 1;
    for (; head; head = head->next, ++counter) {
        printf("%4d  %-18s|  %-13d|  %-13d|  ",
               counter, head->depart->data->name, head->stat.teacher_num,
               head->stat.student_num);
        if (head->stat.teacher_num) {   // 除数可能是零
            printf("%.2f\n", head->stat.st_ratio);
        } else { puts("---"); }
    }
    // 释放统计结果所占用的内存空间
    cleanupDepartStatWrapper(rst);
    putchar('\n');
}

/* 列出院系项目统计结果 */
void listDepartProjectStat(void) {
    // 获取目标查询年份
    printf("stat/project_overview::year (0 for all) > ");
    int year; scanf("%d", &year);
    // 下面的步骤于上一个函数大致相同，略去说明
    DepartStatWrapper *rst = \
        buildDepartStatChainUnordered(mp.depart_head, NULL, year);
    if (rst == NULL) {
        puts("RUNTIME ERROR!");
        exit(-1);
    }
    orderDepartStatWrapperByProjectTotal(rst);
    puts("\
        Name            |    Projects   |    973 Proj.  |    863 Proj.  |    Funding\n\
     -------------------|---------------|---------------|---------------|---------------");
    DepartStatWrapper *head = rst; int counter = 1;
    for (; head; head = head->next, ++counter) {
        printf("%4d  %-18s|  %-13d|  %-13d|  %-13d|  %.2f\n",
               counter, head->depart->data->name,
               head->stat.project_total, head->stat.project_973,
               head->stat.project_863, head->stat.funding);
    }
    cleanupDepartStatWrapper(rst);
    putchar('\n');
}

/* 列出院系项目平均经费统计结果 */
void listDepartFundingStat(void) {
    DepartStatWrapper *rst = \
        buildDepartStatChainUnordered(mp.depart_head, NULL, 0);
    if (rst == NULL) {
        puts("RUNTIME ERROR!");
        exit(-1);
    }
    orderDepartStatWrapperByAvgFunding(rst);
    puts("\
        Name            |    Projects   |    Funding    |    Fund/Proj.\n\
     -------------------|---------------|---------------|---------------");
    DepartStatWrapper *head = rst; int counter = 1;
    for (; head && counter <= 5; head = head->next, ++counter) {
        printf("%4d  %-18s|  %-13d|  %-13.2f|  ",
               counter, head->depart->data->name,
               head->stat.project_total,
               head->stat.funding);
        if (head->stat.project_total) {     // 除数可能为零
            printf("%.2f\n", head->stat.avg_funding);
        } else {
            puts("---");
        }
    }
    cleanupDepartStatWrapper(rst);
    putchar('\n');
}

/* 列出团队搜索结果中所有内容 */
void listTeamWrapper(TeamWrapper *head) {
    if (head == NULL) {
        puts("RUNTIME ERROR!");
        exit(-1);
    }
    if (head->team == NULL) {
        puts("No record found!");
        return;
    }
    puts("\
        Name            |    Manager    |    Faculty\n\
     -------------------|---------------|---------------");
    int num = 1;
    for (; head; head = head->next, ++num) {
        printf("%4d  %-18s|  %-13s|  %s\n",
               num, head->team->data->name,
               head->team->data->manager,
               head->team->data->faculty);
    }
    putchar('\n');
}

/* 列出NSFC项目最多的10个团队 */
void listTeamNSFCProjectStat(void) {
    TeamStatWrapper *rst = \
        buildTeamStatChainUnordered(mp.team_head, NULL, 1);
    if (rst == NULL) {
        puts("RUNTIME ERROR!");
        exit(-1);
    }
    orderTeamStatWrapperByNSFCProject(rst);
    puts("\
        Name            |    NSFC Proj. |    Funding\n\
     -------------------|---------------|---------------");
    TeamStatWrapper *head = rst; int counter = 1;
    // 较之前的遍历过程，多了列举项目数量的限制
    for (; head && counter <= 10; head = head->next, ++counter) {
        printf("%4d  %-18s|  %-13d|  %.2f\n",
               counter, head->team->data->name,
               head->stat.project_NSFC,
               head->stat.funding);
    }
    cleanupTeamStatWrapper(rst);
    putchar('\n');
}

/* 列出项目数/教师比值前5团队 */
void listTeamProjectStat(void) {
    TeamStatWrapper *rst = \
        buildTeamStatChainUnordered(mp.team_head, NULL, 0);
    if (rst == NULL) {
        puts("RUNTIME ERROR!");
        exit(-1);
    }
    orderTeamStatWrapperByPTRatio(rst);
    puts("\
        Name            |    Teachers   |    Projects   |    P/T Ratio\n\
     -------------------|---------------|---------------|---------------");
    TeamStatWrapper *head = rst; int counter = 1;
    for (; head && counter <= 5; head = head->next, ++counter) {
        printf("%4d  %-18s|  %-13d|  %-13d|  ",
               counter, head->team->data->name,
               head->team->data->teacher_num,
               head->stat.project_total);
        if (head->team->data->teacher_num) {
            printf("%.2f\n", head->stat.pt_ratio);
        } else { puts("---"); }
    }
    cleanupTeamStatWrapper(rst);
    putchar('\n');
}

/* 列出项目搜索结果的全部内容 */
void listProjectWrapper(ProjectWrapper *head) {
    if (head == NULL) {
        puts("RUNTIME ERROR!");
        exit(-1);
    }
    if (head->project == NULL) {
        puts("No record found!");
        return;
    }
    puts("\
        ID              |    Manager    |    Type\n\
     -------------------|---------------|---------------");
    int num = 1;
    char type_str[40];
    for (; head; head = head->next, ++num) {
        printf("%4d  %-18s|  %-13s|  %s\n",
               num, head->project->data->id,
               head->project->data->manager,
               parseTypeCodeToString(type_str,
                                     head->project->data->type));
    }
    putchar('\n');
}

/* 将数据中存储的项目类型代码，转换为对应类别的名称 */
char *parseTypeCodeToString(char *to, const char from) {
    switch (from) {
        case '1': { strcpy(to, "973 Program"); break; }
        case '2': { strcpy(to, "NSFC"); break; }
        case '3': { strcpy(to, "863 Program"); break; }
        case '4': { strcpy(to, "International Cooperation"); break; }
        case '5': { strcpy(to, "Transverse"); break; }
        default: { strcpy(to, "---"); break; }
    }
    return to;
}

    /**** Querying ****/

        /** Selectors **/

/* 查询对象选择器 */
void selectQueryObjects(void) {
    while (1) {
        // 显示帮助说明
        puts(DOC_QUERY_OBJ);
        int oper_code = 0;  // 记录用户选择的操作
        printf("query > "); scanf("%d", &oper_code);
        switch (oper_code) {
            case 1: {   // 查询院系
                selectQueryDepartMethod();
                return;
            }
            case 2: {   // 查询团队
                selectQueryTeamMethod();
                return;
            }
            case 3: {   // 查询项目
                // NOTE: 根据团队查找项目整合到团队节点操作中去
                //       故此处直接进入按ID查找项目的函数
                queryProjectById();
                return;
            }
            case 0: { return; }
            default: { break; } // 输入无效代码则要求用户重新输入
        }
    }   // input loop
}

/* 查询院系方法选择器 */
void selectQueryDepartMethod(void) {
    while (1) {
        puts(DOC_QUERY_DEPART_METHOD);
        int oper_code = 0;
        printf("query/depart > "); scanf("%d", &oper_code);
        switch(oper_code) {
            case 1: {   // 按名称
                queryDepartByName();
                return;
            }
            case 2: {   // 按负责人
                queryDepartByManager();
                return;
            }
            case 0: { return; }
            default: { break; }
        }
    }   // input loop
}

/* 查询团队方法选择器 */
void selectQueryTeamMethod(void) {
    while (1) {
        puts(DOC_QUERY_TEAM_METHOD);
        int oper_code = 0;
        printf("query/team > "); scanf("%d", &oper_code);
        switch(oper_code) {
            case 1: {   // 名称
                queryTeamByName();
                return;
            }
            case 2: {   // 教师数量
                queryTeamByTeacherNum();
                return;
            }
            case 0: { return; }
            default: { break; }
        }
    }   // input loop
}

        /** Process **/

/* 按名称查找院系 */
void queryDepartByName(void) {
    // 用户输入目标院系名称
    char depart_name[20];
    printf("query/depart::name > "); scanf("%s", depart_name);
    // 获取查询结果
    DepartWrapper *depart_wrapper = \
        getDepartByName(mp.depart_head, NULL, depart_name);
    // 列出查询结果
    listDepartWrapper(depart_wrapper);
    // ADD: 用户可以从搜索结果中选择院系，从而进行下一步针对某个院系的操作
    if (depart_wrapper->depart != NULL) {
        int oper_code;
        printf("Set cursor to department: "); scanf("%d", &oper_code);
        DepartWrapper *cur = depart_wrapper;
        for (; oper_code > 1 && cur; cur = cur->next, --oper_code) ;
        // 用户可以通过输入无效值来退出该步骤
        // NOTE: 输入负数仍会将用户指针指向第一个搜索结果
        //       （反正不会溢出，懒得改了。。
        if (cur == NULL || oper_code == 0) {
            if (cur == NULL) { puts("Out of range!"); }
            cleanupDepartWrapper(depart_wrapper);
            return;
        }
        // 给用户指针赋值
        cursor.type = 1; cursor.val = (void *)cur->depart;
    }
    cleanupDepartWrapper(depart_wrapper);
}

/* 按负责人查找院系 */
void queryDepartByManager(void) {
    char depart_manager[20];
    printf("query/depart::manager > "); scanf("%s", depart_manager);
    DepartWrapper *depart_wrapper = \
        getDepartByManager(mp.depart_head, NULL, depart_manager);
    listDepartWrapper(depart_wrapper);
    if (depart_wrapper->depart != NULL) {
        int oper_code;
        printf("Set cursor to department: "); scanf("%d", &oper_code);
        DepartWrapper *cur = depart_wrapper;
        for (; oper_code > 1 && cur; cur = cur->next, --oper_code) ;
        if (cur == NULL || oper_code == 0) {
            if (cur == NULL) { puts("Out of range!"); }
            cleanupDepartWrapper(depart_wrapper);
            return;
        }
        cursor.type = 1; cursor.val = (void *)cur->depart;
    }
    cleanupDepartWrapper(depart_wrapper);
}

/* 按名称查找团队 */
void queryTeamByName(void) {
    char team_name[30];
    printf("query/team::name > "); scanf("%s", team_name);
    TeamWrapper *team_wrapper = \
        getTeamByName(mp.team_head, NULL, team_name);
    listTeamWrapper(team_wrapper);
    if (team_wrapper->team != NULL) {
        int oper_code;
        printf("Set cursor to team: "); scanf("%d", &oper_code);
        TeamWrapper *cur = team_wrapper;
        for (; oper_code > 1 && cur; cur = cur->next, --oper_code) ;
        if (cur == NULL || oper_code == 0) {
            if (cur == NULL) { puts("Out of range!"); }
            cleanupTeamWrapper(team_wrapper);
            return;
        }
        cursor.type = 2; cursor.val = (void *)cur->team;
    }
    cleanupTeamWrapper(team_wrapper);
}

/* 按教师人数查找团队 */
void queryTeamByTeacherNum(void) {
    Where cond;
    printf("query/team::condition > "); scanf("%s %d",
           &(cond.direction), &(cond.value));
    // NOTE: 查询方向 direction 的错误输入处理交给查询函数处理
    TeamWrapper *team_wrapper = \
        getTeamByTeacherNum(mp.team_head, NULL, cond);
    listTeamWrapper(team_wrapper);
    if (team_wrapper->team != NULL) {
        int oper_code;
        printf("Set cursor to team: "); scanf("%d", &oper_code);
        TeamWrapper *cur = team_wrapper;
        for (; oper_code > 1 && cur; cur = cur->next, --oper_code) ;
        if (cur == NULL || oper_code == 0) {
            puts("Out of range!");
            cleanupTeamWrapper(team_wrapper);
            return;
        }
        cursor.type = 2; cursor.val = (void *)cur->team;
    }
    cleanupTeamWrapper(team_wrapper);
}

/* 按编号查找项目 */
void queryProjectById(void) {
    char project_id[15];
    printf("query/project::ID > "); scanf("%s", project_id);
    ProjectWrapper *project_wrapper = \
        getProjectById(mp.project_head, NULL, project_id);
    // NOTE: 该过程为精确查找，只可能有一个结果
    //       故直接定向用户指针
    if (project_wrapper->project != NULL) {
        cursor.type = 3; cursor.val = (void *)project_wrapper->project;
    }
    cleanupProjectWrapper(project_wrapper);
}

    /**** Adding ****/

/* 添加记录类别选择器 */
void selectAddObjectType(void) {
    while (1) {
        puts(DOC_ADD_TYPE);
        int oper_code = 0;
        printf("add > "); scanf("%d", &oper_code);
        switch (oper_code) {
            case 1: {   // 院系
                appendDepart(mp.depart_head,
                             initDepartData());
                return;
            }
            case 2: {   // 团队
                appendTeam(mp.team_head,
                           initTeamData(),
                           mp.depart_head);
                return;
            }
            case 3: {   // 项目
                appendProject(mp.project_head,
                              initProjectData(),
                              mp.team_head);
                return;
            }
            case 0: { return; }
            default: { break; }
        }
    }
}

/**** Endpoint ****/

    /** Routing **/

/* 院系操作选择器 */
void selectDepartOperation(void) {
    while (1) {
        puts(DOC_DEPART);
        int oper_code = 0;
        printf("depart/%s > ", ((Depart *)cursor.val)->data->name);
        scanf("%d", &oper_code);
        switch (oper_code) {
            // 列出所有属性
            case 1: { listDepartAttr(); break; }
            // 选择要修改的属性
            case 2: { selectDepartModifyAttr(); break; }
            case 3: {   // 删除节点
                if (removeDepart(&(mp.depart_head),
                                 (Depart *)cursor.val)) {   // 成功删除
                    // 用户指针置空
                    cursor.type = 0; cursor.val = NULL;
                    return;
                  // 错误提示
              } else { puts("Failed to remove record!"); break; }
            }
            case 4: { listDepartChildTeam(); return; }
            case 0: {
                cursor.type = 0; cursor.val = NULL;
                return;
            }
            default: { break; }
        }
    }
}

/* 团队操作选择器 */
void selectTeamOperation(void) {
    while (1) {
        puts(DOC_TEAM);
        int oper_code = 0;
        printf("team/%s > ", ((Team *)cursor.val)->data->name);
        scanf("%d", &oper_code);
        switch (oper_code) {
            case 1: { listTeamAttr(); break; }
            case 2: { selectTeamModifyAttr(); break; }
            case 3: {
                if (removeTeam(&(mp.team_head),
                               (Team *)cursor.val)) {
                    cursor.type = 0; cursor.val = NULL;
                    return;
                } else { puts("Failed to remove record!"); break; }
            }
            case 4: { listTeamChildProject(); return; }
            case 5: {   // 重定向至所属院系
                cursor.type = 1;
                cursor.val = ((Team *)cursor.val)->parent_depart;
                return;
            }
            case 0: {
                cursor.type = 0; cursor.val = NULL;
                return;
            }
            default: { break; }
        }
    }
}

/* 项目操作选择器 */
void selectProjectOperation(void) {
    while (1) {
        puts(DOC_PROJECT);
        int oper_code = 0;
        printf("project/%s > ", ((Project *)cursor.val)->data->id);
        scanf("%d", &oper_code);
        switch (oper_code) {
            case 1: { listProjectAttr(); break; }
            case 2: { selectProjectModifyAttr(); break; }
            case 3: {
                if (removeProject(&(mp.project_head),
                               (Project *)cursor.val)) {
                    cursor.type = 0; cursor.val = NULL;
                    return;
                } else { puts("Failed to remove record!"); break; }
            }
            case 4: {   // 重定向至所属团队
                cursor.type = 2;
                cursor.val = ((Project *)cursor.val)->parent_team;
                return;
            }
            case 0: {
                cursor.type = 0; cursor.val = NULL;
                return;
            }
            default: { break; }
        }
    }
}

    /** Listing info **/

/* 列出当前指向院系的属性 */
void listDepartAttr(void) {
    Depart *tgt = (Depart *)cursor.val;
    puts("\
        ATTR     |    VALUE\n\
     ------------|-------------------");
    printf("\
      Name       |  %s\n", tgt->data->name);
    printf("\
      Manager    |  %s\n", tgt->data->manager);
    printf("\
      Telephone  |  %s\n", tgt->data->mobile);
    putchar('\n');
}

/* 列出当前指向团队的属性 */
void listTeamAttr(void) {
    Team *tgt = (Team *)cursor.val;
    puts("\
        ATTR     |    VALUE\n\
     ------------|-------------------");
    printf("\
      Name       |  %s\n", tgt->data->name);
    printf("\
      Manager    |  %s\n", tgt->data->manager);
    printf("\
      Faculty    |  %s\n", tgt->data->faculty);
    printf("\
      TeacherNum |  %d\n", tgt->data->teacher_num);
    printf("\
      StudentNum |  %d\n", tgt->data->student_num);
    putchar('\n');
}

/* 列出当前指向项目的属性 */
void listProjectAttr(void) {
    Project *tgt = (Project *)cursor.val;
    puts("\
        ATTR     |    VALUE\n\
     ------------|-------------------");
    printf("\
      ID         |  %s\n", tgt->data->id);
    char type_str[30] = {'\0'};
    printf("\
      Type       |  %s\n",
           parseTypeCodeToString(type_str, tgt->data->type));
    printf("\
      StartDate  |  %s\n", tgt->data->start_date);
    printf("\
      Funding    |  %f *10000 CNY\n", tgt->data->funding);
    printf("\
      Manager    |  %s\n", tgt->data->manager);
    printf("\
      Team       |  %s\n", tgt->data->team);
    putchar('\n');
}

    /** Modifying **/

/* 修改院系属性 */
void selectDepartModifyAttr(void) {
    Depart *tgt = (Depart *)cursor.val;
    while (1) {
        puts(DOC_DEPART_MODIFY);
        int oper_code = 0;
        printf("depart/%s > ", tgt->data->name); scanf("%d", &oper_code);
        // NOTE: 属性修改函数限定了用户能够修改的内容
        switch (oper_code) {
            case 1: {   // 负责人
                printf("depart/%s::manager > ", tgt->data->name);
                scanf("%s", tgt->data->manager);
                return;
            }
            case 2: {   // 联系电话
                printf("depart/%s::telephone > ", tgt->data->name);
                scanf("%s", tgt->data->mobile);
                return;
            }
            case 0: { return; }
            default: { break; }
        }
    }
}

/* 修改团队属性 */
void selectTeamModifyAttr(void) {
    Team *tgt = (Team *)cursor.val;
    while (1) {
        puts(DOC_TEAM_MODIFY);
        int oper_code = 0;
        printf("team/%s > ", tgt->data->name); scanf("%d", &oper_code);
        switch (oper_code) {
            case 1: {   // 负责人
                printf("team/%s::manager > ", tgt->data->name);
                scanf("%s", tgt->data->manager);
                return;
            }
            case 2: {   // 教师人数
                printf("team/%s::teacher_num > ", tgt->data->name);
                scanf("%d", &(tgt->data->teacher_num));
                return;
            }
            case 3: {   // 学生人数
                printf("team/%s::student_num > ", tgt->data->name);
                scanf("%d", &(tgt->data->student_num));
                return;
            }
            case 0: { return; }
            default: { break; }
        }
    }
}

/* 修改项目属性 */
void selectProjectModifyAttr(void) {
    Project *tgt = (Project *)cursor.val;
    while (1) {
        puts(DOC_PROJECT_MODIFY);
        int oper_code = 0;
        printf("project/%s > ", tgt->data->id); scanf("%d", &oper_code);
        switch (oper_code) {
            case 1: {
                printf("project/%s::type > ", tgt->data->id);
                do { scanf("%c", &(tgt->data->type)); }
                while (tgt->data->type < '1' || tgt->data->type > '5');
                return;
            }
            case 2: {
                printf("project/%s::funding > ", tgt->data->id);
                scanf("%f", &(tgt->data->funding));
                return;
            }
            case 3: {
                printf("project/%s::manager > ", tgt->data->id);
                scanf("%s", tgt->data->manager);
                return;
            }
            case 0: { return; }
            default: { break; }
        }
    }
}

    /** Listing Child **/

/* 列出院系下属团队 */
void listDepartChildTeam(void) {
    Depart *tgt = (Depart *)cursor.val;
    TeamWrapper *child_team = getTeamByDepart(tgt);
    listTeamWrapper(child_team);
    // ADD: 用户可以直接导航到某一个下属团队
    if (child_team->team != NULL) {
        int oper_code;
        printf("Set cursor to team: "); scanf("%d", &oper_code);
        TeamWrapper *cur = child_team;
        for (; oper_code > 1 && cur; cur = cur->next, --oper_code) ;
        if (cur == NULL || oper_code == 0) {
            puts("Out of range!");
            cleanupTeamWrapper(child_team);
            return;
        }
        cursor.type = 2; cursor.val = (void *)cur->team;
    }
    cleanupTeamWrapper(child_team);
}

/* 列出团队开设的项目 */
void listTeamChildProject(void) {
    Team *tgt = (Team *)cursor.val;
    ProjectWrapper *child_project = getProjectByTeam(tgt);
    listProjectWrapper(child_project);
    if (child_project->project != NULL) {
        int oper_code;
        printf("Set cursor to project: "); scanf("%d", &oper_code);
        ProjectWrapper *cur = child_project;
        for (; oper_code > 1 && cur; cur = cur->next, --oper_code) ;
        if (cur == NULL || oper_code == 0) {
            puts("Out of range!");
            cleanupProjectWrapper(child_project);
            return;
        }
        cursor.type = 3; cursor.val = (void *)cur->project;
    }
    cleanupProjectWrapper(child_project);
}

/******** Stat ********/

/* 数据统计功能选择器 */
void selectStatItem(void) {
    while (1) {
        puts(DOC_STAT);
        int oper_code = 0;
        printf("stat > "); scanf("%d", &oper_code);
        switch (oper_code) {
            // 院系人数统计
            case 1: { listDepartHRStat(); return; }
            // 院系项目统计
            case 2: { listDepartProjectStat(); return; }
            // 团队国家自然科学基金项目统计
            case 3: { listTeamNSFCProjectStat(); return; }
            // 团队项目/教师比统计
            case 4: { listTeamProjectStat(); return; }
            // 院系项目平均经费统计
            case 5: { listDepartFundingStat(); return; }
            case 0: { return; }
            default: { break; }
        }
    }
}

/*********  Main Bone **********/

/* 顶层（用户指针置空时）用户导航逻辑以及系统初始化与退出 */
int main(int argc, char const *argv[]) {

    // 欢迎屏幕
    puts(START);

    // 从命令行参数读取目标数据文件夹路径
    if (argc == 1) {
        // 用户未指定路径
        // 使用默认值，即程序根目录
        puts("Data folder unspecified!");
        puts("Finding data files in program root...");
    } else if (argc == 2) {
        // 用户指定了路径
        strcpy(TGT_PATH, argv[1]);
        printf("Loading data from folder \"%s\"...\n", TGT_PATH);
    } else { puts("Too many arguments!"); exit(0); }

    // 加载数据
    mp = loadData(TGT_PATH);
    // 检查数据完整性
    if (mp.depart_head == NULL
            || mp.team_head == NULL
            || mp.project_head == NULL) {
        // 数据加载时出现错误
        //     无数据文件，数据损坏等等
        puts("Data file not found!");
        // 清理现有的含有错误数据的链表
        cleanupDepart(mp.depart_head);
        cleanupTeam(mp.team_head);
        cleanupProject(mp.project_head);
        // 询问用户是否在当前文件夹创建数据文件
        printf("Create data file in program root? [Y/N]: ");
        char option = 0;
        scanf("%c", &option);
        switch (option) {
            case 'Y':   case 'y': {
                // NOTE: 此时数据文件并不会立即创建
                //       由于fwrite()函数的特性，数据文件在保存时自动创建
                mp.depart_head = createDepartHead();
                mp.team_head = createTeamHead();
                mp.project_head = createProjectHead();
                puts("Data will be saved to program root!");
                break;
            }
            case 'N':   case 'n':   default: {
                // 没有数据文件，退出程序
                puts("No data available!");
                puts("exit");
                return 0;
            }
        }
    }

    // 成功载入数据
    puts("Successfullly linked all data!"); putchar('\n');

    // 用户导航
    while (1) {
        switch (cursor.type) {  // 根据用户指针指向判断用户所在层级
            case 0: {   // 顶层（用户指针置空）
                puts(DOC_ROOT);
                int oper_code = 0;
                printf("> "); scanf("%d", &oper_code);
                switch (oper_code) {
                    // 列出所有记录
                    case 1: { listAllNodes(); break; }
                    // 数据查询
                    case 2: { selectQueryObjects(); break; }
                    // 数据统计
                    case 3: { selectStatItem(); break; }
                    // 新增记录
                    case 4: { selectAddObjectType(); break; }
                    case 0: {
                        // 退出程序时自动保存数据
                        // NOTE: 没有单独的数据保存选项
                        if (saveData(mp, TGT_PATH) == 0) {  // 存盘操作失败
                            puts("Error while saving...");
                            puts("GG! MY FRIEND! (￣y▽,￣)╭ ");
                            // HACK: 不知道什么情况会导致这种情况的发生
                            //       因此目前没想到什么解决方案
                        }
                        // 释放内存
                        cleanupDepart(mp.depart_head);
                        cleanupTeam(mp.team_head);
                        cleanupProject(mp.project_head);
                        // 正确退出
                        puts("exit");
                        return 0;
                    }
                    default: { break; }
                }   // inner switch
                break;
            }
            case 1: { selectDepartOperation(); break; }     // 院系层
            case 2: { selectTeamOperation(); break; }       // 团队层
            case 3: { selectProjectOperation(); break; }    // 项目层
            // 指向错误的层级 - 直接退出，不做内存清理
            default: { printf("ERR > exit"); return -1; }
        }
    }
}
