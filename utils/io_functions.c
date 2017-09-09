/*  $PROGRAM_ROOT/utils/io_functions.c
 *  数据保存与载入方法定义与实现
 */


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
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

struct stat st = {0};

#include "data_structure.h"
#include "faculty_functions.h"
#include "team_functions.h"
#include "project_functions.h"

/********** Controlers **********/

#ifdef BUILDING
#undef BUILDING
#endif
// #define BUILDING

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
int backupData(MountPoint, const char *);
MountPoint loadData(const char *);

/********** Function Realization **********/

/* 内部函数 - 保存院系数据 */
int _saveDepartData(Depart *depart, const char *CUR_DIR) {
    // 生成目标地址
    char filename[100];
    sprintf(filename, "%s/%s", CUR_DIR, "DEPART.DAT");
    #if defined(BUILDING)
    printf("[LOG] _saveDepartData(): target URI is \"%s\"\n", filename);
    #endif
    // 尝试创建该文件
    FILE *fp;
    fp = fopen(filename, "wb");
    if (fp == NULL) {
        // NOTE: 若用户指定的文件夹不存在，也会抛出错误
        #if defined(DEBUG)
        printf("[LOG] Error in _saveDepartData():\n\tfailed to open file %s\n",
               filename);
        #endif
        return 0;
    }
    // 写入数据
    for (; depart && depart->data; depart = depart->next) {
        // 直接整块写入结构体，读取也方便
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
    fclose(fp);
    return 1;
}

/* 内部函数 - 保存团队数据 */
int _saveTeamData(Team *team, const char *CUR_DIR) {
    char filename[100];
    sprintf(filename, "%s/%s", CUR_DIR, "TEAM.DAT");
    #if defined(BUILDING)
    printf("[LOG] _saveTeamData(): target URI is \"%s\"\n", filename);
    #endif
    FILE *fp;
    fp = fopen(filename, "wb");
    if (fp == NULL) {
        #if defined(DEBUG)
        printf("[LOG] Error in _saveTeamData():\n\tfailed to open file %s\n",
               filename);
        #endif
        return 0;
    }
    for (; team && team->data; team = team->next) {
        if (fwrite(team->data, sizeof(TeamData), 1, fp) == 0) {
            #if defined(DEBUG)
            puts("[LOG] Error in _saveTeamData():\n\tfailed to write ");
            #endif
            return 0;
        }
        #if defined(DEBUG)
        printf("[LOG] _saveTeamData(): saved %s to file\n",
               team->data->name);
        #endif
    }
    fclose(fp);
    return 1;
}

/* 内部函数 - 保存项目数据 */
int _saveProjectData(Project *project, const char *CUR_DIR) {
    char filename[100];
    sprintf(filename, "%s/%s", CUR_DIR, "PROJECT.DAT");
    #if defined(BUILDING)
    printf("[LOG] _saveProjectData(): target URI is \"%s\"\n", filename);
    #endif
    FILE *fp;
    fp = fopen(filename, "wb");
    if (fp == NULL) {
        #if defined(DEBUG)
        printf("[LOG] Error in _saveProjectData():\n\tfailed to open file %s\n",
               filename);
        #endif
        return 0;
    }
    for (; project && project->data; project = project->next) {
        if (fwrite(project->data, sizeof(ProjectData), 1, fp) == 0) {
            #if defined(DEBUG)
            puts("[LOG] Error in _saveProjectData():\n\tfailed to write ");
            #endif
            return 0;
        }
        #if defined(DEBUG)
        printf("[LOG] _saveProjectData(): saved %s to file\n",
               project->data->id);
        #endif
    }
    fclose(fp);
    return 1;
}

/* 数据保存方法 */
int saveData(MountPoint mp, const char *TGT_PATH) {
    if (_saveDepartData(mp.depart_head, TGT_PATH)
            && _saveTeamData(mp.team_head, TGT_PATH)
            && _saveProjectData(mp.project_head, TGT_PATH)) {
        return 1;
    }
    #if defined(DEBUG)
    puts("[LOG] Error in saveData():\n\tfailed to save");
    #endif
    return 0;
}

/* HACK: bakup data */
int backupData(MountPoint mp, const char *TGT_PATH) {
    // generate target path for backup data
    char BAK_PATH[100];
    sprintf(BAK_PATH, "%s/%s", TGT_PATH, "bak");
    // check if target backup directory already exsists
    // (NOTE: and create one if necessary)
    if (stat(BAK_PATH, &st) == -1) {
        mkdir(BAK_PATH, 0755);
    }
    if (saveData(mp, BAK_PATH)) { return 1; }
    return 0;
}

/* 数据加载方法 */
MountPoint loadData(const char *TGT_PATH) {
    // 创建挂载点副本
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
    char filename[100];
    FILE *fp;
    DepartData depart_data_buf;
    TeamData team_data_buf;
    ProjectData project_data_buf;
    size_t counter = 0;     // 用于核实读取记录条数与文件大小是否相符
    // 加载院系数据
    // 打开数据文件
    sprintf(filename, "%s/%s", TGT_PATH, "DEPART.DAT");
    #if defined(DEBUG)
    printf("[LOG] loadData()::loadDepartData: target URI is \"%s\"\n", filename);
    #endif
    fp = fopen(filename, "rb");
    if (fp == NULL) {       // 读取文件错误
        #if defined(DEBUG)
        puts("[LOG] Error in loadData()::loadDepartData:\n\tfailed to open file");
        #endif
        free(mp.depart_head);
        // 找不到数据文件，不保留挂载点
        mp.depart_head = NULL;
        return mp;
    }
    // 开始读取
    counter = 0;
    while (fread(&depart_data_buf, sizeof(DepartData), 1, fp) == 1) {
        // 按结构体整块读入
        #if defined(DEBUG)
        printf("[LOG] loadData()::loadDepartData: reading %s to memory\n",
               depart_data_buf.name);
        #endif
        appendDepart(mp.depart_head, depart_data_buf);
        ++counter;
        // fp自己跳不用管
    }
    // 验证数据是否全部读入
    fseek(fp, 0, SEEK_END);     // fp挪到文件尾
    if (counter != ftell(fp) / sizeof(DepartData)) {    // 大小不一致
        #if defined(DEBUG)
        puts("[LOG] Error in loadData()::loadDepartData:\n\tfailed to read al data");
        #endif
        // 仅保留挂载点
        cleanupDepart(mp.depart_head);
        mp.depart_head = createDepartHead();
        return mp;
    }
    fclose(fp);

    // 加载团队数据
    sprintf(filename, "%s/%s", TGT_PATH, "TEAM.DAT");
    #if defined(DEBUG)
    printf("[LOG] loadData()::loadTeamData: target URI is \"%s\"\n", filename);
    #endif
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in loadData()::loadTeamData:\n\tfailed to open file");
        #endif
        free(mp.team_head);
        mp.team_head = NULL;
        return mp;
    }
    counter = 0;
    while (fread(&team_data_buf, sizeof(TeamData), 1, fp) == 1) {
        #if defined(DEBUG)
        printf("[LOG] loadData()::loadTeamData: reading %s to memory\n",
               team_data_buf.name);
        #endif
        appendTeam(mp.team_head, team_data_buf, mp.depart_head);
        ++counter;
    }
    fseek(fp, 0, SEEK_END);
    if (counter != ftell(fp) / sizeof(TeamData)) {
        #if defined(DEBUG)
        puts("[LOG] Error in loadData()::loadTeamData:\n\tfailed to read al data");
        #endif
        cleanupTeam(mp.team_head);
        mp.team_head = createTeamHead();
        return mp;
    }
    fclose(fp);

    // 加载项目数据
    sprintf(filename, "%s/%s", TGT_PATH, "PROJECT.DAT");
    #if defined(BUILDING)
    printf("[LOG] loadData()::loadProjectData: target URI is \"%s\"\n", filename);
    #endif
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in loadData()::loadProjectData:\n\tfailed to open file");
        #endif
        free(mp.project_head);
        mp.project_head = NULL;
        return mp;
    }
    counter = 0;
    while (fread(&project_data_buf, sizeof(ProjectData), 1, fp) == 1) {
        #if defined(DEBUG)
        printf("[LOG] loadData()::loadProjectData: reading %s to memory\n",
               project_data_buf.id);
        #endif
        appendProject(mp.project_head, project_data_buf, mp.team_head);
        ++counter;
    }
    fseek(fp, 0, SEEK_END);
    if (counter != ftell(fp) / sizeof(ProjectData)) {
        #if defined(DEBUG)
        puts("[LOG] Error in loadData()::loadProjectData:\n\tfailed to read al data");
        #endif
        cleanupProject(mp.project_head);
        mp.project_head = createProjectHead();
        return mp;
    }
    fclose(fp);

    return mp;
}

/********** Unit Test **********/
#if defined(BUILDING)

void main(void) {
    // building test env
    DepartData depart_data_1 = {
        "Computer", "Zhang3", "13344445555"
    };
    #if defined(CHILD_COUNTER)
    depart_data_1.team_num = 0;
    #endif
    TeamData team_data_1 = {
        "Rocket", "Hanzo", 2, 3, "Computer"
    };
    TeamData team_data_2 = {
        "MilkyWay", "Genji", 3, 4, "Computer"
    };
    ProjectData project_data_1 = {
        "123456", '1', "1970/01", 1.2, "Wang5", "Rocket"
    };
    ProjectData project_data_2 = {
        "123345", '2', "1980/01", 2.3, "Zhao6", "MilkyWay"
    };
    ProjectData project_data_3 = {
        "123234", '3', "1990/01", 3.4, "Lee4", "Rocket"
    };
    Depart *Depart_HEAD = createDepartHead();
    Team *Team_HEAD = createTeamHead();
    Project *Project_HEAD = createProjectHead();
    appendDepart(Depart_HEAD, depart_data_1);
    appendTeam(Team_HEAD, team_data_1, Depart_HEAD);
    appendTeam(Team_HEAD, team_data_2, Depart_HEAD);
    appendProject(Project_HEAD, project_data_1, Team_HEAD);
    appendProject(Project_HEAD, project_data_2, Team_HEAD);
    appendProject(Project_HEAD, project_data_3, Team_HEAD);
    MountPoint mp;
    mp.depart_head = Depart_HEAD;
    mp.team_head = Team_HEAD;
    mp.project_head = Project_HEAD;
    // specifying path
    char *TGT_PATH = "./data";
    // _saveDepartData(Depart_HEAD, TGT_PATH);
    // _saveTeamData(Team_HEAD, TGT_PATH);
    // _saveProjectData(Project_HEAD, TGT_PATH);
    saveData(mp, TGT_PATH);
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
