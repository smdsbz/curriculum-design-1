/*  $PROGRAM_ROOT/utils/project_functions.c
 *  项目方法定义与实现
 */

#ifndef DATA_STRUCTURE
#include "data_structure.h"
#endif

#ifndef FACULTY_FUNCTIONS
#include "faculty_functions.h"
#endif

#ifndef TEAM_FUNCTIONS
#include "team_functions.h"
#endif

#ifdef BUILDING
#undef BUILDING
#endif
// #define BUILDING

#ifdef DEBUG
#undef DEBUG
#endif
// #define DEBUG

/********** Declaration **********/
    /**** POST | DELETE | PUT ****/
ProjectData initProjectData(void);
Project *appendProject(Project *head, ProjectData new_one, Team *team_chain);
int modifyProject(Project *tgt, ProjectData new_one);
int removeProject(Project **phead, Project *tgt);
Project *createProjectHead(void);
    /**** SELECT ****/
ProjectWrapper *getProjectById(Project *head, Project *, const char *id);
ProjectWrapper *getProjectByTeam(Team *parent_team);
    /**** CLEANUPs ****/
void cleanupProjectWrapper(ProjectWrapper *start);
void cleanupProject(Project *start);

/********** Function Realizations **********/

    /**** POST | DELETE | PUT ****/

/* 创建并初始化头节点 */
Project *createProjectHead(void) {
    Project *Project_HEAD = (Project *)malloc(sizeof(Project));
    if (Project_HEAD == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in createProjectHead():\n\tfailed to malloc for project (head)");
        #endif
        return NULL;
    }
    Project_HEAD->data = NULL; Project_HEAD->next = NULL;
    Project_HEAD->parent_team = NULL;
    return Project_HEAD;
}

/* 创建项目数据模板 */
ProjectData initProjectData(void) {
    ProjectData Manhatan;
    Manhatan.type = '\0';
    printf("add/project::id > "); scanf("%s", Manhatan.id);
    printf("\
add/project::type \n\
    1 - 973\n\
    2 - NSFC\n\
    3 - 863\n\
    4 - International\n\
    5 - Transverse\n\
> ");
    int type_dec;
    // 类型代码值域检查
    do { scanf("%d", &type_dec); }
    while (type_dec < 1 || type_dec > 5);
    Manhatan.type = type_dec + '0';
    printf("add/project::start_date > "); scanf("%s", Manhatan.start_date);
    printf("add/project::funding > "); scanf("%f", &(Manhatan.funding));
    printf("add/project::manager > "); scanf("%s", Manhatan.manager);
    printf("add/project::team > "); scanf("%s", Manhatan.team);
    return Manhatan;
}

/* 添加项目数据 */
Project *appendProject(Project *head, ProjectData new_one,
                       Team *team_chain) {
    ProjectWrapper *project_wrapper = \
        getProjectById(head, NULL, new_one.id);
    if (project_wrapper->project != NULL) {
        printf("Record for project %s already exists!\n", new_one.id);
        cleanupProjectWrapper(project_wrapper);
        return NULL;
    }
    cleanupProjectWrapper(project_wrapper);
    Project *tail = head;
    for (; tail->next; tail = tail->next) ;
    // 获取所属团队节点
    TeamWrapper *parent_team_wrapper = \
        getTeamByName(team_chain, NULL, new_one.team);
    if (parent_team_wrapper == NULL) {
        #if defined(DEBUG)
        puts("[LOG] appendProject():\n\tgetTeamByName() failed");
        #endif
        return NULL;
    }
    if (parent_team_wrapper->team == NULL) {
        puts("Target parent team not found!\n");
        return NULL;
    }
    if (parent_team_wrapper->next != NULL) {
        puts("Multiple parent teams found!\n");
        return NULL;
    }
    if (strcmp(parent_team_wrapper->team->data->name, new_one.team)) {
        // 模糊查找，需要二次检查
        printf("Team named %s not found, do you mean %s instead?\n",
               new_one.team, parent_team_wrapper->team->data->name);
        return NULL;
    }
    Team *parent_team = parent_team_wrapper->team;
    cleanupTeamWrapper(parent_team_wrapper);
    #if defined(DEBUG)
    printf("[LOG] appendProject(): parent_team is %s @ 0x%p\n",
           parent_team->data->name, parent_team);
    #endif
    // 添加项目
    if (tail == head && tail->data == NULL) {
        tail->data = (ProjectData *)malloc(sizeof(ProjectData));
        if (tail->data == NULL) {
            #if defined(DEBUG)
            puts("[LOG] Error in appendProject():\n\tfailed to malloc for data (head)");
            #endif
            return NULL;
        }
        *(tail->data) = new_one; tail->next = NULL;
        tail->parent_team = parent_team;
        #if defined(CHILD_COUNTER)
        parent_team->data->project_num += 1;
        #endif
        parent_team->child_project_head = tail;
        parent_team->child_project_tail = tail;
        return tail;
    }
    if (parent_team->child_project_tail == NULL
            || parent_team->child_project_tail->next == NULL) {
        tail->next = (Project *)malloc(sizeof(Project));
        if (tail->next == NULL) {
            #if defined(DEBUG)
            puts("[LOG] Error in appendProject():\n\tfailed to malloc for data");
            #endif
            return NULL;
        }
        tail->next->data = (ProjectData *)malloc(sizeof(ProjectData));
        if (tail->next->data == NULL) {
            #if defined(DEBUG)
            puts("[LOG] Error in appendProject():\n\tfailed to malloc for data");
            #endif
            return NULL;
        }
        tail->next->next = NULL;
    } else {
        tail = parent_team->child_project_tail;
        Project *after = tail->next;
        tail->next = (Project *)malloc(sizeof(Project));
        if (tail->next == NULL) {
            #if defined(DEBUG)
            puts("[LOG] Error in appendProject():\n\tfailed to malloc for container");
            #endif
            return NULL;
        }
        tail->next->data = (ProjectData *)malloc(sizeof(ProjectData));
        if (tail->next->data == NULL) {
            #if defined(DEBUG)
            puts("[LOG] Error in appendProject():\n\tfailed to malloc for data");
            #endif
            return NULL;
        }
        tail->next->next = after;
    }
    *(tail->next->data) = new_one;
    if (parent_team->child_project_head == NULL) {
        parent_team->child_project_head = tail->next;
    }
    parent_team->child_project_tail = tail->next;
    tail->next->parent_team = parent_team;
    return tail->next;
}

/* 覆盖项目信息 */
int modifyProject(Project *tgt, ProjectData new_one) {
    if (tgt == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in modifyProject():\n\ttarget is NULL");
        #endif
        return 0;
    }
    if (strcmp(tgt->data->team, new_one.team) != 0) {
        #if defined(DEBUG)
        printf("[LOG] Error in modifyProject():\n\tchanging project team,");
        printf(" from %s to %s, strcmp() got %d\n",
               tgt->data->team, new_one.team,
               strcmp(tgt->data->team, new_one.team));
        #endif
        return 0;
    }
    *(tgt->data) = new_one;
    return 1;
}

/* 删除项目节点 */
int removeProject(Project **phead, Project *tgt) {
    if (tgt == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in removeProject():\n\ttarget is NULL");
        #endif
        return 0;
    }
    Project *prev = *phead;
    for (; prev->next != NULL && prev->next != tgt; prev = prev->next) ;
    if (*phead == tgt && tgt->next) {
        *phead = tgt->next;
    } else {
        prev->next = tgt->next;
    }
    if (tgt->parent_team->child_project_head == tgt->parent_team->child_project_tail) {
        tgt->parent_team->child_project_head = NULL;
        tgt->parent_team->child_project_tail = NULL;
    } else if (tgt->parent_team->child_project_head == tgt) {
        tgt->parent_team->child_project_head = tgt->next;
    } else if (tgt->parent_team->child_project_tail == tgt) {
        tgt->parent_team->child_project_tail = prev;
    }
    #if defined(DEBUG)
    printf("[LOG] removeProject(): freed %s @ 0x%p\n",
           tgt->data->id, tgt);
    #endif
    free(tgt->data);
    if (tgt->next == NULL && *phead == tgt) { tgt->data = NULL; }
    else { free(tgt); }
    return 1;
}

    /**** SELECT ****/

/* 通过id查找项目 */
ProjectWrapper *
getProjectById(Project *start, Project *end, const char *id) {
    ProjectWrapper *rtn = (ProjectWrapper *)malloc(sizeof(ProjectWrapper));
    if (rtn == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in getProjectById():\n\tfailed to malloc for result mounting point");
        #endif
        return NULL;
    }
    ProjectWrapper *rtn_head = rtn;
    rtn_head->project = NULL; rtn_head->next = NULL;
    if (start->data == NULL) {
        #if defined(DEBUG)
        puts("[LOG] getProjectById(): searching an empty chain");
        #endif
        return rtn_head;
    }
    for (; start != end; start = start->next) {
        if (strcmp(start->data->id, id) == 0) {
            #if defined(BUILDING)
            printf("[LOG] getProjectById(): found %s @ 0x%p\n",
                   start->data->id, start);
            #endif
            if (rtn_head->project == NULL) {
                rtn->project = start;
            } else {
                rtn->next = (ProjectWrapper *)malloc(sizeof(ProjectWrapper));
                if (rtn->next == NULL) {
                    #if defined(DEBUG)
                    puts("[LOG] Error in getProjectById():\n\tfailed to malloc for result container");
                    #endif
                    cleanupProjectWrapper(rtn_head);
                    return NULL;
                }
                rtn = rtn->next; rtn->next = NULL;
                rtn->project = start;
            }
        }
    }
    return rtn_head;
}

/* 通过团队查找项目 */
ProjectWrapper *getProjectByTeam(Team *parent_team) {
    // 创建结果挂载点
    ProjectWrapper *rtn_head = (ProjectWrapper *)malloc(sizeof(ProjectWrapper));
    if (rtn_head == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in getProjectByTeam():\n\tfailed to malloc for result maunting point");
        #endif
        return NULL;
    }
    rtn_head->project = NULL; rtn_head->next = NULL;
    // 母结点没有子节点
    if (parent_team->child_project_head == NULL) {
        return rtn_head;    // 返回空结果
    }
    // 母结点下有子节点
    Project *cur = parent_team->child_project_head;
    ProjectWrapper *rtn = rtn_head;
    for (; cur != parent_team->child_project_tail->next; cur = cur->next) {
        #if defined(DEBUG)
        printf("[LOG] getProjectByTeam(): reached %s @ 0x%p\n",
               cur->data->id, cur);
        #endif
        if (rtn_head->project == NULL) {
            rtn->project = cur;
        } else {
            rtn->next = (ProjectWrapper *)malloc(sizeof(ProjectWrapper));
            if (rtn->next == NULL) {
                #if defined(DEBUG)
                puts("[LOG] Error in getProjectByTeam():\n\tfailed to malloc for result container");
                #endif
                cleanupProjectWrapper(rtn_head);
                return NULL;
            }
            rtn = rtn->next;
            rtn->project = cur; rtn->next = NULL;
        }
    }
    return rtn_head;
}

    /**** CLEANUPs ****/

/* 清空搜索结果序列 */
void cleanupProjectWrapper(ProjectWrapper *prev) {
    if (prev == NULL) {
        #if defined(DEBUG)
        puts("Nothing left to be cleaned");
        #endif
        return;
    }
    ProjectWrapper *after;
    for (; prev; prev = after) {
        after = prev->next;
        #if defined(BUILDING)
        printf("[LOG] cleanupProjectWrapper(): freed 0x%p\n", prev);
        #endif
        free(prev);
    }
    return;
}

/* 释放Project链所占用的空间 */
void cleanupProject(Project *prev) {
    if (prev == NULL) {
        #if defined(DEBUG)
        puts("Nothing left to be cleaned");
        #endif
        return;
    }
    Project *after;
    for (; prev; prev = after) {
        after = prev->next;
        #if defined(DEBUG)
        printf("[LOG] cleanupProject(): freed 0x%p\n", prev);
        #endif
        free(prev->data);
        free(prev);
    }
    #if defined(DEBUG)
    puts("[LOG] cleanupProject(): exit");
    #endif
    return;
}

/********** Unit Test **********/

#if defined(BUILDING)

void printProjectToConsole(Project *Manhatan) {
    printf("<Project @ 0x%p>\n", Manhatan);
    printf("\tthis.id = %s\n", Manhatan->data->id);
    printf("\tthis.type = %c\n", Manhatan->data->type);
    printf("\tthis.start_date = %s\n", Manhatan->data->start_date);
    printf("\tthis.funding = %.2f\n", Manhatan->data->funding);
    printf("\tthis.manager = %s\n", Manhatan->data->manager);
    printf("\tthis.team = %s\n", Manhatan->data->team);
    printf("\tthis.next = 0x%p\n", Manhatan->next);
}

void printProjectChainToConsole(Project *start) {
    if (start == NULL) {
        puts("no data!");
        return;
    }
    for (; start; start = start->next) {
        printProjectToConsole(start);
    }
    putchar('\n');
}

void printProjectWrapperToConsole(ProjectWrapper *head) {
    printf("<ProjectWrapper @ 0x%p>\n", head);
    if (head->project == NULL) {
        puts("\tno data!");
        return;
    }
    for (; head; head = head->next) {
        printf("\tfound %s @ 0x%p\n",
               head->project->data->id, head->project);
    }
    putchar('\n');
    return;
}

void main(void) {
    // building test env
    DepartData depart_data_1 = {
        "计算机", "张三", "13344445555"
    };
    #if defined(CHILD_COUNTER)
    depart_data_1.team_num = 0;
    #endif
    TeamData team_data_1 = {
        "火箭队", "武藏", 2, 3, "计算机"
    };
    TeamData team_data_2 = {
        "银河队", "小次郎", 3, 4, "计算机"
    };
    ProjectData project_data_1 = {
        "123456", '1', "1970/01", 1.2, "王五", "火箭队"
    };
    ProjectData project_data_2 = {
        "123345", '2', "1980/01", 2.3, "赵六", "银河队"
    };
    ProjectData project_data_3 = {
        "123234", '3', "1990/01", 3.4, "李四", "火箭队"
    };
    Depart *Depart_HEAD = createDepartHead();
    Team *Team_HEAD = createTeamHead();
    Project *Project_HEAD = createProjectHead();
    appendDepart(Depart_HEAD, depart_data_1);
    appendTeam(Team_HEAD, team_data_1, Depart_HEAD);
    appendTeam(Team_HEAD, team_data_2, Depart_HEAD);
    // appendProject()
    puts("[LOG] adding project \"123456\"");
    appendProject(Project_HEAD, project_data_1, Team_HEAD);
    puts("[LOG] adding project \"123345\"");
    appendProject(Project_HEAD, project_data_2, Team_HEAD);
    puts("[LOG] adding project \"123234\"");
    appendProject(Project_HEAD, project_data_3, Team_HEAD);
    puts("Expecting sequence:\n\t123456 --> 123234 --> 123345");
    printProjectChainToConsole(Project_HEAD);
    // modifyProject()
    puts("[LOG] modifying project \"123234\"");
    ProjectData project_data_4 = {
        "123234", '4', "1990/01", 3.5, "李斯特", "火箭队"
    };
    modifyProject(Project_HEAD->next, project_data_4);
    printProjectChainToConsole(Project_HEAD);
    // SELECT
    ProjectWrapper *project_wrapper;
    puts("[LOG] getting project \"123234\" via id");
    project_wrapper = getProjectById(Project_HEAD, NULL, "123234");
    printProjectWrapperToConsole(project_wrapper);
    cleanupProjectWrapper(project_wrapper);
    // removeProject()
    puts("[LOG] removing project \"123456\"");
    removeProject(&Project_HEAD, Project_HEAD);
    printProjectChainToConsole(Project_HEAD);
    puts("[LOG] getting al project under \"火箭队\"");
    project_wrapper = getProjectByTeam(Team_HEAD);
    printProjectWrapperToConsole(project_wrapper);
    cleanupProjectWrapper(project_wrapper);
    puts("[LOG] removing project \"123345\"");
    removeProject(&Project_HEAD, Project_HEAD->next);
    printProjectChainToConsole(Project_HEAD);
    cleanupProject(Project_HEAD);
    cleanupTeam(Team_HEAD);
    cleanupDepart(Depart_HEAD);
    puts("Al Success!");
    return;
}

#endif

#ifdef BUILDING
#undef BUILDING
#endif

#ifdef DEBUG
#undef DEBUG
#endif
