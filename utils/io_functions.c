
/********** Idea **********

${PROGRAMME_ROOT}
    +-- the_programme.exe
    +-- data/
        +-- DEPART.DAT
        +-- TEAM.DAT
        +-- PROJECT.DAT

*/

/********** Import **********/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>     // char *getcwd(char *buf, int len)

#ifndef DATA_STRUCTURE
#include "data_structure.h"
#endif
#ifndef FACULTY_FUNCTIONS
#include "faculty_functions.h"
#endif
#ifndef TEAM_FUNCTIONS
#include "team_functions.h"
#endif
#ifndef PROJECT_FUNCTIONS
#include "project_functions.h"
#endif

/********** Controlers **********/

#ifdef BUILDING
#undef BUILDING
#endif
#define BUILDING

#ifdef DEBUG
#undef DEBUG
#endif
// #define DEBUG


/********** Declaration **********/

int _saveDepartData(Depart *, const char *);
int _saveTeamData(Team *, const char *);
int _saveProjectData(Project *, const char *);
/*  分别保存方法
 *  ARGS:   要保存的数据链表，路径（endpoint形式，无最后的'\'）
 *  RETN:   success code
 */

int saveData(MountPoint , const char *);
/*  打包保存方法
 *  ARGS:   数据挂载点。储存数据文件的目标文件夹
 *  RETN:   success code
 */

MountPoint loadData(const char *);
/*  打包载入数据方法
 *  ARGS:   储存数据文件的文件夹
 *  RETN:   院系链表、团队链表、项目链表的挂载点
 */


/********** Realization **********/

int _saveDepartData(Depart *depart, const char *CUR_DIR) {
    // get target end-point
    char filename[100];
    sprintf(filename, "%s\\%s", CUR_DIR, "DEPART.DAT");
    #if defined(BUILDING)
    printf("[LOG] _saveDepartData(): target URI is \"%s\"\n", filename);
    #endif

    // touch file
    FILE *fp;
    fp = fopen(filename, "wb");
    if (fp == NULL) {
        #if defined(DEBUG)
        printf("[LOG] Error in _saveDepartData():\n\tfailed to open file");
        #endif
        return 0;
    }
    // write to file
    for (; depart && depart->data; depart = depart->next) {
        if (fwrite(depart->data, sizeof(DepartData), 1, fp) == 0) {
            #if defined(DEBUG)
            puts("[LOG] Error in _saveDepartData():\n\tfailed to write ");
            #endif
            return 0;
        }
        #if defined(DEBUG)
        printf("[LOG] _saveDepartData(): saved %s to file\n",
               depart->data->name);
        #endif
    }

    // cleanup
    fclose(fp);
    return 1;
}


int _saveTeamData(Team *depart, const char *CUR_DIR) {
    // get target end-point
    char filename[100];
    sprintf(filename, "%s\\%s", CUR_DIR, "TEAM.DAT");
    #if defined(BUILDING)
    printf("[LOG] _saveTeamData(): target URI is \"%s\"\n", filename);
    #endif

    // touch file
    FILE *fp;
    fp = fopen(filename, "wb");
    if (fp == NULL) {
        #if defined(DEBUG)
        printf("[LOG] Error in _saveTeamData():\n\tfailed to open file");
        #endif
        return 0;
    }

    // write to file
    for (; depart && depart->data; depart = depart->next) {
        if (fwrite(depart->data, sizeof(TeamData), 1, fp) == 0) {
            #if defined(DEBUG)
            puts("[LOG] Error in _saveTeamData():\n\tfailed to write ");
            #endif
            return 0;
        }
        #if defined(DEBUG)
        printf("[LOG] _saveTeamData(): saved %s to file\n",
               depart->data->name);
        #endif
    }

    // cleanup
    fclose(fp);
    return 1;
}


int _saveProjectData(Project *depart, const char *CUR_DIR) {
    // get target end-point
    char filename[100];
    sprintf(filename, "%s\\%s", CUR_DIR, "PROJECT.DAT");
    #if defined(BUILDING)
    printf("[LOG] _saveProjectData(): target URI is \"%s\"\n", filename);
    #endif

    // touch file
    FILE *fp;
    fp = fopen(filename, "wb");
    if (fp == NULL) {
        #if defined(DEBUG)
        printf("[LOG] Error in _saveProjectData():\n\tfailed to open file");
        #endif
        return 0;
    }

    // write to file
    for (; depart && depart->data; depart = depart->next) {
        if (fwrite(depart->data, sizeof(ProjectData), 1, fp) == 0) {
            #if defined(DEBUG)
            puts("[LOG] Error in _saveProjectData():\n\tfailed to write ");
            #endif
            return 0;
        }
        #if defined(DEBUG)
        printf("[LOG] _saveProjectData(): saved %s to file\n",
               depart->data->id);
        #endif
    }

    // cleanup
    fclose(fp);
    return 1;
}


int saveData(MountPoint mp, const char *TGT_PATH) {
    if (_saveDepartData(mp.depart_head, TGT_PATH)
            && _saveTeamData(mp.team_head, TGT_PATH)
            && _saveProjectData(mp.project_head, TGT_PATH)) {
        // indent-fixer
        return 1;
    }
    #if defined(DEBUG)
    puts("[LOG] Error in saveData():\n\tfailed to save");
    #endif
    return 0;
}



MountPoint loadData(const char *TGT_PATH) {
    // create mounting point
    MountPoint mp;
    mp.depart_head = createDepartHead();
    mp.team_head = createTeamHead();
    mp.project_head = createProjectHead();
    if (!(mp.depart_head && mp.team_head && mp.project_head)) {
        #if defined(DEBUG)
        puts("[LOG] Error in loadData():\n\tfailed to malloc for mounting points");
        #endif
        return mp;
    }

    // declare var
    char filename[100];
    FILE *fp;

    DepartData depart_data_buf;
    TeamData team_data_buf;
    ProjectData project_data_buf;

    size_t counter = 0;
    // ptrdiff_t file_size = 0;

    // loadDepartData

    sprintf(filename, "%s\\%s", TGT_PATH, "DEPART.DAT");
    #if defined(BUILDING)
    printf("[LOG] loadData()::loadDepartData: target URI is \"%s\"\n", filename);
    #endif
    fp = fopen(filename, "rb");
    if (fp == NULL) {       // 读取文件错误
        #if defined(DEBUG)
        puts("[LOG] Error in loadData()::loadDepartData:\n\tfailed to open file");
        #endif
        free(mp.depart_head);
        mp.depart_head = NULL;
        return mp;
    }
    // loadDepartData - 读取数据
    counter = 0;
    while (fread(&depart_data_buf, sizeof(DepartData), 1, fp) == 1) {
        #if defined(DEBUG)
        printf("[LOG] loadData()::loadDepartData: reading %s to memory\n",
               depart_data_buf.name);
        #endif
        appendDepart(mp.depart_head, depart_data_buf);
        ++counter;
    }
    // loadDepartData - 验证数据是否全部读入
    fseek(fp, 0, SEEK_END);
    if (counter != ftell(fp) / sizeof(DepartData)) {
        #if defined(DEBUG)
        puts("[LOG] Error in loadData()::loadDepartData:\n\tfailed to read al data");
        #endif
        cleanupDepart(mp.depart_head);
        mp.depart_head = NULL;
        return mp;
    }
    // free(&depart_data_buf);
    fclose(fp);

    // loadTeamData

    sprintf(filename, "%s\\%s", TGT_PATH, "TEAM.DAT");
    #if defined(BUILDING)
    printf("[LOG] loadData()::loadTeamData: target URI is \"%s\"\n", filename);
    #endif
    fp = fopen(filename, "rb");
    if (fp == NULL) {       // 读取文件错误
        #if defined(DEBUG)
        puts("[LOG] Error in loadData()::loadTeamData:\n\tfailed to open file");
        #endif
        free(mp.team_head);
        mp.team_head = NULL;
        return mp;
    }
    // loadTeamData - 读取数据
    counter = 0;
    while (fread(&team_data_buf, sizeof(TeamData), 1, fp) == 1) {
        #if defined(DEBUG)
        printf("[LOG] loadData()::loadTeamData: reading %s to memory\n",
               team_data_buf.name);
        #endif
        appendTeam(mp.team_head, team_data_buf, mp.depart_head);
        ++counter;
    }
    // loadTeamData - 验证数据是否全部读入
    fseek(fp, 0, SEEK_END);
    if (counter != ftell(fp) / sizeof(TeamData)) {
        #if defined(DEBUG)
        puts("[LOG] Error in loadData()::loadTeamData:\n\tfailed to read al data");
        #endif
        cleanupTeam(mp.team_head);
        mp.team_head = NULL;
        return mp;
    }
    // free(&team_data_buf);
    fclose(fp);


    // loadProjectData

    sprintf(filename, "%s\\%s", TGT_PATH, "PROJECT.DAT");
    #if defined(BUILDING)
    printf("[LOG] loadData()::loadProjectData: target URI is \"%s\"\n", filename);
    #endif
    fp = fopen(filename, "rb");
    if (fp == NULL) {       // 读取文件错误
        #if defined(DEBUG)
        puts("[LOG] Error in loadData()::loadProjectData:\n\tfailed to open file");
        #endif
        free(mp.project_head);
        mp.project_head = NULL;
        return mp;
    }
    // loadProjectData - 读取数据
    counter = 0;
    while (fread(&project_data_buf, sizeof(ProjectData), 1, fp) == 1) {
        #if defined(DEBUG)
        printf("[LOG] loadData()::loadProjectData: reading %s to memory\n",
               project_data_buf.id);
        #endif
        appendProject(mp.project_head, project_data_buf, mp.team_head);
        ++counter;
    }
    // loadProjectData - 验证数据是否全部读入
    fseek(fp, 0, SEEK_END);
    if (counter != ftell(fp) / sizeof(ProjectData)) {
        #if defined(DEBUG)
        puts("[LOG] Error in loadData()::loadProjectData:\n\tfailed to read al data");
        #endif
        cleanupProject(mp.project_head);
        mp.project_head = NULL;
        return mp;
    }
    // free(&project_data_buf);
    fclose(fp);


    return mp;
}




/********** Unit Test **********/
#if defined(BUILDING)

void main(void) {
    // // building test env
    // DepartData depart_data_1 = {
    //     "计算机", "张三", "13344445555"
    // };
    // #if defined(CHILD_COUNTER)
    // depart_data_1.team_num = 0;
    // #endif
    // TeamData team_data_1 = {
    //     "火箭队", "武藏", 2, 3, "计算机"
    // };
    // TeamData team_data_2 = {
    //     "银河队", "小次郎", 3, 4, "计算机"
    // };
    // ProjectData project_data_1 = {
    //     "123456", '1', "1970/01", 1.2, "王五", "火箭队"
    // };
    // ProjectData project_data_2 = {
    //     "123345", '2', "1980/01", 2.3, "赵六", "银河队"
    // };
    // ProjectData project_data_3 = {
    //     "123234", '3', "1990/01", 3.4, "李四", "火箭队"
    // };
    // Depart *Depart_HEAD = createDepartHead();
    // Team *Team_HEAD = createTeamHead();
    // Project *Project_HEAD = createProjectHead();
    // appendDepart(Depart_HEAD, depart_data_1);
    // appendTeam(Team_HEAD, team_data_1, Depart_HEAD);
    // appendTeam(Team_HEAD, team_data_2, Depart_HEAD);
    // appendProject(Project_HEAD, project_data_1, Team_HEAD);
    // appendProject(Project_HEAD, project_data_2, Team_HEAD);
    // appendProject(Project_HEAD, project_data_3, Team_HEAD);

    MountPoint mp;
    // mp.depart_head = Depart_HEAD;
    // mp.team_head = Team_HEAD;
    // mp.project_head = Project_HEAD;

    // specifying path
    char *TGT_PATH = ".\\data";

    // _saveDepartData()
    // _saveDepartData(Depart_HEAD, TGT_PATH);
    // _saveTeamData(Team_HEAD, TGT_PATH);
    // _saveProjectData(Project_HEAD, TGT_PATH);
    // saveData(mp, TGT_PATH);


    // MountPoint loadData(const char *TGT_PATH) {
    mp = loadData(TGT_PATH);
    Depart *d; Team *t; Project *p;
    for (d = mp.depart_head; d != NULL; d = d->next) {
        printf("<Depart %s @ 0x%p>\n", d->data->name, d);
        for (t = d->child_team_head; t != d->child_team_tail->next; t = t->next) {
            printf("    <Team %s @ 0x%p>\n", t->data->name, t);
            for (p = t->child_project_head; p != t->child_project_tail->next; p = p->next) {
                printf("        <Project %s @ 0x%p>\n", p->data->id, p);
            }
        }
    }


    // cleanup
    // cleanupProject(Project_HEAD);
    // cleanupTeam(Team_HEAD);
    // cleanupDepart(Depart_HEAD);
    return;

}


#endif
