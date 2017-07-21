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
#define BUILDING

#ifdef DEBUG
#undef DEBUG
#endif
#define DEBUG



/********** Declaration **********/

    /**** POST | DELETE | PUT ****/

ProjectData initProjectData(void);
/*  创建一个项目数据的原型
 *  ARGS:   void
 *  RETN:   根据在该函数执行过程中输入的数据所创建出来的原型
 *  NOTE:   will trigger input action
 */

Project *appendProject(Project *head, ProjectData new_one, Team *team_chain);
/*  录入项目
 *  ARGS:   项目链表头，新增节点数据域模板，母结点链（团队链）
 *  RETN:   新增节点的地址
 */

int modifyProject(Project *tgt, ProjectData new_one);
/*  修改项目信息
 *  ARGS:   目标地址，已经修改的数据域
 *  RETN:   success code
 */

int removeProject(Project **phead, Project *tgt);
/*  删除项目节点
 *  ARGS:   指向团队链表头节点的指针，目标地址 | NULL
 *  RETN:   success code
 */


    /**** SELECT ****/

ProjectWrapper *getProjectById(Project *head, const char *id);
/*  通过id查找项目
 *  ARGS:   项目链表，目标id
 *  RETN:   搜索结果挂载点 | NULL （没有搜索结果时也返回挂载点地址）
 */

// ProjectWrapper *getProjectByTeam(Project *head, const char *team_name);
/*  通过团队查找项目
 *  NOTE:   由于通过团队节点可以很方便地取到相应的项目链，故先不构建该函数
 */


    /**** CLEANUPs ****/

void cleanupProjectWrapper(ProjectWrapper *start);
/*  清空搜索结果序列
 *  ARGS:   头节点地址
 *  RETN:   void
 */

void cleanupProject(Project *start);
/*  释放Project链所占用的空间
 *  ARGS:   头节点地址
 *  RETN:   void
 */



/********** Realizations **********/

    /**** POST | DELETE | PUT ****/

ProjectData initProjectData(void) {
    ProjectData Manhatan;
    printf("project.id = "); scanf("%s", Manhatan.id);
    printf("project.type = "); scanf("%c", &(Manhatan.type));
    printf("project.start_date = "); scanf("%s", Manhatan.start_date);
    printf("project.funding = "); scanf("%f", &(Manhatan.funding));
    printf("project.manager = "); scanf("%s", Manhatan.manager);
    printf("project.team = "); scanf("%s", Manhatan.team);
    return Manhatan;
}



Project *appendProject(Project *head, ProjectData new_one, Team *team_chain) {
    // get tail node
    Project *tail = head;
    for (; tail->next; tail = tail->next) ;
    // tail->next == NULL

    // get parent_team
    TeamWrapper *parent_team_wrapper = getTeamByName(team_chain, new_one.team);
    if (parent_team_wrapper == NULL) {
        #if defined(DEBUG)
        puts("[LOG] appendProject():\n\tgetTeamByName() failed");
        #endif
        return NULL;
    }
    if (parent_team_wrapper->team == NULL) {
        puts("target parent team not found\n");
        return NULL;
    }
    if (parent_team_wrapper->next != NULL) {
        puts("multiple parent teams found\n");
        return NULL;
    }

    Team *parent_team = parent_team_wrapper->team;
    cleanupTeamWrapper(parent_team_wrapper);
    #if defined(DEBUG)
    printf("[LOG] appendProject(): parent_team is %s @ 0x%p\n",
           parent_team->data->name, parent_team);
    #endif

    // I/O - case 1
    if (tail == head
            && tail->data == NULL) {
        // indent-fixer
        #if defined(DEBUG)
        puts("[LOG] appendTeam(): case 1");
        #endif
        // malloc for tail->data
        tail->data = (ProjectData *)malloc(sizeof(ProjectData));
        if (tail->data == NULL) {
            #if defined(DEBUG)
            puts("[LOG] Error in appendProject():\n\tfailed to malloc for data (head)");
            #endif
            return NULL;
        }
        // writing in
        *(tail->data) = new_one; tail->next = NULL;
        tail->parent_team = parent_team;
        parent_team->data->project_num += 1;
        parent_team->child_project_head = tail;
        parent_team->child_project_tail = tail;
        return tail;
    }
    // I/O - case 2 & 3
    if (parent_team->child_project_tail == NULL
            || parent_team->child_project_tail->next == NULL) {
        // indent-fixer
        #if defined(BUILDING)
        puts("[LOG] appendProject(): case 2");
        #endif
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
        #if defined(BUILDING)
        puts("[LOG] appendProject(): case 3");
        #endif
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

    // I/O - case 2 & 3
    *(tail->next->data) = new_one;
    parent_team->data->project_num += 1;
    if (parent_team->data->project_num == 1) {
        parent_team->child_project_head = tail->next;
    }
    parent_team->child_project_tail = tail->next;
    tail->next->parent_team = parent_team;

    return tail->next;
}



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



int removeProject(Project **phead, Project *tgt) {
    if (tgt == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in removeProject():\n\ttarget is NULL");
        #endif
        return 0;
    }

    Project *prev = *phead;
    for (; prev->next != NULL && prev->next != tgt; prev->next) ;

    if (*phead == tgt) {
        *phead = tgt->next;
    } else {
        prev->next = tgt->next;
    }

    tgt->parent_team->data->project_num -= 1;
    if (tgt->parent_team->data->project_num == 0) {
        tgt->parent_team->child_project_head = NULL;
        tgt->parent_team->child_project_tail = NULL;
    } else if (tgt->parent_team->child_project_head == tgt) {
        tgt->parent_team->child_project_head = tgt->next;
    } else if (tgt->parent_team->child_project_tail == tgt) {
        tgt->parent_team->child_project_tail = prev;
    }

    #if defined(BUILDING)
    printf("[LOG] removeProject(): freed %s @ 0x%p\n",
           tgt->data->id, tgt);
    #endif
    free(tgt->data);
    free(tgt);

    return 1;
}


    /**** SELECT ****/

ProjectWrapper *getProjectById(Project *start, const char *id) {
    ProjectWrapper *rtn = (ProjectWrapper *)malloc(sizeof(ProjectWrapper));
    if (rtn == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in getProjectById():\n\tfailed to malloc for result mounting point");
        #endif
        return NULL;
    }

    ProjectWrapper *rtn_head = rtn;
    rtn_head->project = NULL; rtn_head->next = NULL;

    for (; start; start = start->next) {
        if (strstr(start->data->id, id) != NULL) {
            #if defined(BUILDING)
            printf("[LOG] getProjectById(): found %s @ 0x%p",
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
                }
                cleanupProjectWrapper(rtn_head);
                return NULL;
            }
            rtn = rtn->next; rtn->next = NULL;
            rtn->project = start;
        }
    }
    return rtn_head;
}



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
    return rtn_head;
}



    /**** CLEANUPs ****/

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
        // indent-fixer
    }
    putchar('\n');
    return;
}


void main(void) {
    // building test env
    DepartData depart_data_1 = {
        "计算机", "张三", "13344445555", 0
    };

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


    Depart *Depart_HEAD = (Depart *)malloc(sizeof(Depart));
    Depart_HEAD->data = NULL; Depart_HEAD->next = NULL;
    Depart_HEAD->child_team_head = NULL;
    Depart_HEAD->child_team_tail = NULL;

    Team *Team_HEAD = (Team *)malloc(sizeof(Team));
    Team_HEAD->data = NULL; Team_HEAD->next = NULL;
    Team_HEAD->parent_depart = NULL;
    Team_HEAD->child_project_head = NULL;
    Team_HEAD->child_project_tail = NULL;

    Project *Project_HEAD = (Project *)malloc(sizeof(Project));
    Project_HEAD->data = NULL; Project_HEAD->next = NULL;
    Project_HEAD->parent_team = NULL;

    appendDepart(Depart_HEAD, depart_data_1);
    appendTeam(Team_HEAD, team_data_1, Depart_HEAD);
    appendTeam(Team_HEAD, team_data_2, Depart_HEAD);


    // printTeamChainToConsole(Team_HEAD);

    // appendProject()
    puts("[LOG] adding project \"123456\"");
    appendProject(Project_HEAD, project_data_1, Team_HEAD);
    puts("[LOG] adding project \"123345\"");
    appendProject(Project_HEAD, project_data_2, Team_HEAD);
    puts("[LOG] adding project \"123234\"");
    appendProject(Project_HEAD, project_data_3, Team_HEAD);
    puts("Expecting sequence:\n\t123456 --> 123234 --> 123345");
    printProjectChainToConsole(Project_HEAD);

    cleanupProject(Project_HEAD);
    cleanupTeam(Team_HEAD);
    cleanupDepart(Depart_HEAD);
    return;
}

#endif



#ifdef BUILDING
#undef BUILDING
#endif

#ifdef DEBUG
#undef DEBUG
#endif
