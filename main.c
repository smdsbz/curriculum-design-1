#include <stdio.h>
#include <stdlib.h>

#include "./utils/__init__.h"
#include "./views.h"
#include "./doc_strings.h"

// setting env
MountPoint mp;
char TGT_PATH[100] = "./";
Cursor cursor = {0, NULL};


/********** Data **********/

/**** Listing ****/

void listAllNodes(void) {
    if (mp.depart_head == NULL) {
        puts("No data!");
        return;
    }
    Depart *d; Team *t; Project *p;
    for (d = mp.depart_head; d != NULL; d = d->next) {
        printf("%s\n", d->data->name);
        if (d->child_team_head) {
            for (t = d->child_team_head; t != d->child_team_tail->next; t = t->next) {
                printf("    %s\n", t->data->name);
                if (t->child_project_head) {
                    for (p = t->child_project_head; p != t->child_project_tail->next; p = p->next) {
                        printf("        %s\n", p->data->id);
                    }
                }
            }
        }
    }
    putchar('\n');
}

void listDepartWrapper(DepartWrapper *head) {
    if (head == NULL) {
        puts("RUNTIME ERROR!");
        exit(-1);
    }
    if (head->depart == NULL) {
        puts("No record found!");
        return;
    }
    puts("\
        Name            |    Manager    |    Telephone\n\
     -------------------|---------------|---------------");
    int num = 1;
    for (; head; head = head->next, ++num) {
        printf("%4d  %-18s|  %-13s|  %s\n",
               num, head->depart->data->name,
               head->depart->data->manager,
               head->depart->data->mobile);
        // indent-fixer
    }
    putchar('\n');
}

void listDepartHRStat(void) {
    DepartStatWrapper *rst = buildDepartStatChainUnordered(mp.depart_head, NULL);
    if (rst == NULL) {
        puts("RUNTIME ERROR!");
        exit(-1);
    }
    orderDepartStatWrapperBySTRatio(rst);
    puts("\
        Name            |    Teachers   |    Students   |    S/T Ratio\n\
     -------------------|---------------|---------------|---------------");
    DepartStatWrapper *head = rst; int counter = 1;
    for (; head; head = head->next, ++counter) {
        printf("%4d  %-18s|  %-13d|  %-13d|  %-.2f\n",
               counter, head->depart->data->name, head->stat.teacher_num,
               head->stat.student_num, head->stat.st_ratio);
        // indent-fixer
    }
    cleanupDepartStatWrapper(rst);
    putchar('\n');
}

void listDepartProjectStat(void) {
    DepartStatWrapper *rst = buildDepartStatChainUnordered(mp.depart_head, NULL);
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
               counter, head->depart->data->name, head->stat.project_total,
               head->stat.project_973, head->stat.project_863, head->stat.funding);
        // indent-fixer
    }
    cleanupDepartStatWrapper(rst);
    putchar('\n');
}

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
        // indent-fixer
    }
    putchar('\n');
}

void listTeamNSFCProjectStat(void) {    // Top 10
    TeamStatWrapper *rst = buildTeamStatChainUnordered(mp.team_head, NULL);
    if (rst == NULL) {
        puts("RUNTIME ERROR!");
        exit(-1);
    }
    orderTeamStatWrapperByNSFCProject(rst);
    puts("\
        Name            |    NSFC Proj. |    Funding\n\
     -------------------|---------------|---------------");
    TeamStatWrapper *head = rst; int counter = 1;
    for (; head && counter <= 10; head = head->next, ++counter) {
        printf("%4d  %-18s|  %-13d|  %.2f\n",
               counter, head->team->data->name,
               head->stat.project_NSFC,
               head->stat.funding);
        // indent-fixer
    }
    cleanupTeamStatWrapper(rst);
    putchar('\n');
}

void listTeamProjectStat(void) {
    TeamStatWrapper *rst = buildTeamStatChainUnordered(mp.team_head, NULL);
    if (rst == NULL) {
        puts("RUNTIME ERROR!");
        exit(-1);
    }
    orderTeamStatWrapperByProjectTotal(rst);
    puts("\
        Name            |    Teachers   |    Projects   |    P/T Ratio\n\
     -------------------|---------------|---------------|---------------");
    TeamStatWrapper *head = rst; int counter = 1;
    for (; head && counter <= 5; head = head->next, ++counter) {
        printf("%4d  %-18s|  %-13d|  %-13d|  %.2f\n",
               counter, head->team->data->name,
               head->team->data->teacher_num,
               head->stat.project_total,
               (float)head->team->data->teacher_num / head->stat.project_total);
        // indent-fixer
    }
    cleanupTeamStatWrapper(rst);
    putchar('\n');
}

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
               parseTypeCodeToString(type_str, head->project->data->type));
        // indent-fixer
    }
    putchar('\n');
}

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

void selectQueryObjects(void) {
    while (1) {
        puts(DOC_QUERY_OBJ);
        int oper_code = 0;
        printf("query > "); scanf("%d", &oper_code);
        switch (oper_code) {
            case 1: {
                selectQueryDepartMethod();
                return;
            }
            case 2: {
                selectQueryTeamMethod();
                return;
            }
            case 3: {
                // NOTE: 根据团队查找项目整合到团队节点操作中去
                queryProjectById();
                return;
            }
            case 0: { return; }
            default: { break; }
        }
    }   // input loop
}

void selectQueryDepartMethod(void) {
    while (1) {
        puts(DOC_QUERY_DEPART_METHOD);
        int oper_code = 0;
        printf("query/depart > "); scanf("%d", &oper_code);
        switch(oper_code) {
            case 1: {
                queryDepartByName();
                return;
            }
            case 2: {
                queryDepartByManager();
                return;
            }
            case 0: { return; }
            default: { break; }
        }
    }   // input loop
}

void selectQueryTeamMethod(void) {
    while (1) {
        puts(DOC_QUERY_TEAM_METHOD);
        int oper_code = 0;
        printf("query/team > "); scanf("%d", &oper_code);
        switch(oper_code) {
            case 1: {
                queryTeamByName();
                return;
            }
            case 2: {
                queryTeamByTeacherNum();
                return;
            }
            case 0: { return; }
            default: { break; }
        }
    }   // input loop
}


void queryDepartByName(void) {
    char depart_name[20];
    printf("query/depart::name > "); scanf("%s", depart_name);
    DepartWrapper *depart_wrapper = getDepartByName(mp.depart_head, NULL, depart_name);
    listDepartWrapper(depart_wrapper);
    if (depart_wrapper->depart != NULL) {
        int oper_code;
        printf("Set cursor to department: "); scanf("%d", &oper_code);
        DepartWrapper *cur = depart_wrapper;
        for (; oper_code > 1 && cur; cur = cur->next, --oper_code) ;
        if (cur == NULL || oper_code == 0) {
            puts("Out of range!");
            cleanupDepartWrapper(depart_wrapper);
            return;
        }
        cursor.type = 1; cursor.val = (void *)cur->depart;
    }
    cleanupDepartWrapper(depart_wrapper);
}

void queryDepartByManager(void) {
    char depart_manager[20];
    printf("query/depart::manager > "); scanf("%s", depart_manager);
    DepartWrapper *depart_wrapper = getDepartByManager(mp.depart_head, NULL, depart_manager);
    listDepartWrapper(depart_wrapper);
    if (depart_wrapper->depart != NULL) {
        int oper_code;
        printf("Set cursor to department: "); scanf("%d", &oper_code);
        DepartWrapper *cur = depart_wrapper;
        for (; oper_code > 1 && cur; cur = cur->next, --oper_code) ;
        if (cur == NULL || oper_code == 0) {
            puts("Out of range!");
            cleanupDepartWrapper(depart_wrapper);
            return;
        }
        cursor.type = 1; cursor.val = (void *)cur->depart;
    }
    cleanupDepartWrapper(depart_wrapper);
}

void queryTeamByName(void) {
    char team_name[30];
    printf("query/team::name > "); scanf("%s", team_name);
    TeamWrapper *team_wrapper = getTeamByName(mp.team_head, NULL, team_name);
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

void queryTeamByTeacherNum(void) {
    Where cond;
    printf("query/team::condition > "); scanf("%s %d",
           &(cond.direction), &(cond.value));
    // indent-fixer
    TeamWrapper *team_wrapper = getTeamByTeacherNum(mp.team_head, NULL, cond);
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

void queryProjectById(void) {
    char project_id[15];
    printf("query/project::ID > "); scanf("%s", project_id);
    ProjectWrapper *project_wrapper = getProjectById(mp.project_head, NULL, project_id);
    // listProjectWrapper(project_wrapper);
    if (project_wrapper->project != NULL) {
        // int oper_code;
        // printf("Set cursor to project: "); scanf("%d", &oper_code);
        // ProjectWrapper *cur = project_wrapper;
        // for (; oper_code > 1 && cur; cur = cur->next, --oper_code) ;
        // if (cur == NULL || oper_code == 0) {
        //     puts("Out of range!");
        //     cleanupProjectWrapper(project_wrapper);
        //     return;
        // }
        // cursor.type = 3; cursor.val = (void *)cur->project;
        cursor.type = 3; cursor.val = (void *)project_wrapper->project;
    }
    cleanupProjectWrapper(project_wrapper);
}

/**** Adding ****/

void selectAddObjectType(void) {
    while (1) {
        puts(DOC_ADD_TYPE);
        int oper_code = 0;
        printf("add > "); scanf("%d", &oper_code);
        switch (oper_code) {
            case 1: {
                appendDepart(mp.depart_head, initDepartData());
                return;
            }
            case 2: {
                appendTeam(mp.team_head, initTeamData(), mp.depart_head);
                return;
            }
            case 3: {
                appendProject(mp.project_head, initProjectData(), mp.team_head);
                return;
            }
            case 0: { return; }
            default: { break; }
        }
    }
}

/**** Endpoint ****/

/** Routing **/

void selectDepartOperation(void) {
    while (1) {
        puts(DOC_DEPART);
        int oper_code = 0;
        printf("depart/%s > ", ((Depart *)cursor.val)->data->name);
        scanf("%d", &oper_code);
        switch (oper_code) {
            case 1: { listDepartAttr(); break; }
            case 2: { selectDepartModifyAttr(); break; }
            case 3: {
                removeDepart(&(mp.depart_head), (Depart *)cursor.val);
                cursor.type = 0; cursor.val = NULL;
                return;
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
                removeTeam(&(mp.team_head), (Team *)cursor.val);
                cursor.type = 0; cursor.val = NULL;
                return;
            }
            case 4: { listTeamChildProject(); return; }
            case 0: {
                cursor.type = 0; cursor.val = NULL;
                return;
            }
            default: { break; }
        }
    }
}

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
                removeProject(&(mp.project_head), (Project *)cursor.val);
                cursor.type = 0; cursor.val = NULL;
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

void listProjectAttr(void) {
    Project *tgt = (Project *)cursor.val;
    puts("\
        ATTR     |    VALUE\n\
     ------------|-------------------");
    printf("\
      ID         |  %s\n", tgt->data->id);
    char type_str[20] = {'\0'};
    printf("\
      Type       |  %s\n", parseTypeCodeToString(type_str, tgt->data->type));
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

void selectDepartModifyAttr(void) {
    Depart *tgt = (Depart *)cursor.val;
    while (1) {
        puts(DOC_DEPART_MODIFY);
        int oper_code = 0;
        printf("depart/%s > ", tgt->data->name); scanf("%d", &oper_code);
        switch (oper_code) {
            case 1: {
                printf("depart/%s::manager > ", tgt->data->name);
                scanf("%s", tgt->data->manager);
                return;
            }
            case 2: {
                printf("depart/%s::tele > ", tgt->data->name);
                scanf("%s", tgt->data->mobile);
                return;
            }
            case 0: { return; }
            default: { break; }
        }
    }
}

void selectTeamModifyAttr(void) {
    Team *tgt = (Team *)cursor.val;
    while (1) {
        puts(DOC_TEAM_MODIFY);
        int oper_code = 0;
        printf("team/%s > ", tgt->data->name); scanf("%d", &oper_code);
        switch (oper_code) {
            case 1: {
                printf("team/%s::manager > ", tgt->data->name);
                scanf("%s", tgt->data->manager);
                return;
            }
            case 2: {
                printf("team/%s::teacher_num > ", tgt->data->name);
                scanf("%d", &(tgt->data->teacher_num));
                return;
            }
            case 3: {
                printf("team/%s::student_num > ", tgt->data->name);
                scanf("%d", &(tgt->data->student_num));
                return;
            }
            case 0: { return; }
            default: { break; }
        }
    }
}

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

void listDepartChildTeam(void) {
    Depart *tgt = (Depart *)cursor.val;
    TeamWrapper *child_team = getTeamByDepart(tgt);
    listTeamWrapper(child_team);
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

void selectStatItem(void) {
    while (1) {
        puts(DOC_STAT);
        int oper_code = 0;
        printf("stat > "); scanf("%d", &oper_code);
        switch (oper_code) {
            case 1: { listDepartHRStat(); return; }
            case 2: { listDepartProjectStat(); return; }
            case 3: { listTeamNSFCProjectStat(); return; }
            case 4: { listTeamProjectStat(); return; }
            case 0: { return; }
            default: { break; }
        }
    }
}

/*********  Main Bone **********/

int main(int argc, char const *argv[]) {

    // welcome screen
    puts(START);

    // setting up TGT_PATH
    if (argc == 1) {
        puts("Data folder unspecified!");
        puts("Finding data files in program root...");
    } else if (argc == 2) {
        strcpy(TGT_PATH, argv[1]);
        printf("Loading data from folder \"%s\"...\n", TGT_PATH);
    }

    // loading data
    mp = loadData(TGT_PATH);
    // checking data viability
    if (mp.depart_head == NULL
            || mp.team_head == NULL
            || mp.project_head == NULL) {
        // indent-fixer
        puts("Data file not found!");
        // cleanups
        cleanupDepart(mp.depart_head);
        cleanupTeam(mp.team_head);
        cleanupProject(mp.project_head);
        // ask if init data files under current folder
        printf("Create data file in program root? [Y/N]: ");
        char option = 0;
        scanf("%c", &option);
        switch (option) {
            case 'Y':   case 'y': {
                // TODO: init data files
                // NOTE: or just auto-init while saving...
                mp.depart_head = createDepartHead();
                mp.team_head = createTeamHead();
                mp.project_head = createProjectHead();
                puts("Data will be saved to program root!");
                break;
            }
            case 'N':   case 'n':   default: {
                puts("No data available!");
                puts("exit");
                return 0;
            }
        }
    }

    // loaded successfully
    puts("Successfullly linked all data!");
    putchar('\n');

    // Operations Available
    while (1) {
        switch (cursor.type) {
            case 0: {
                puts(DOC_ROOT);
                int oper_code = 0;
                printf("> "); scanf("%d", &oper_code);
                switch (oper_code) {
                    case 1: {
                        listAllNodes();
                        break;
                    }
                    case 2: {
                        selectQueryObjects();
                        break;
                    }
                    case 3: {
                        selectStatItem();
                        break;
                    }
                    case 4: {
                        selectAddObjectType();
                        break;
                    }
                    case 0: {
                        if (saveData(mp, TGT_PATH) == 0) {
                            puts("Seems like some unexpected happened while saving...");
                            puts("GG! MY FRIEND!!!");
                        }
                        puts("exit");
                        return 0;
                    }
                    default: { break; }
                }   // inner switch
                break;
            }
            case 1: {
                selectDepartOperation();
                break;
            }
            case 2: {
                selectTeamOperation();
                break;
            }
            case 3: {
                selectProjectOperation();
                break;
            }
            default: {
                printf("ERR > ");
                break;
            }
        }
    }
}
