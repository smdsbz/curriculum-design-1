#ifndef _VIEWS_H
#define _VIEWS_H

/** __ **/
extern void listAllNodes(void);
extern void listDepartWrapper(DepartWrapper *);
extern void listTeamWrapper(TeamWrapper *);
extern void listProjectWrapper(ProjectWrapper *);
extern char *parseTypeCodeToString(char *, const char);


/**** query ****/
extern void selectQueryObjects(void);

/****** query method ******/
extern void selectQueryDepartMethod(void);
extern void selectQueryTeamMethod(void);
// extern void selectQueryProjectMethod(void);

/******** query final ********/
extern void queryDepartByName(void);
extern void queryDepartByManager(void);
extern void queryTeamByName(void);
extern void queryTeamByTeacherNum(void);
extern void queryProjectById(void);


/**** add ****/
extern void selectAddObjectType(void);


/**** statistic ****/
extern void selectStatObject(void);
extern void listDepartHRStat(void);
extern void listDepartProjectStat(void);
extern void listTeamNSFCProjectStat(void);
extern void listTeamProjectStat(void);

/******************************************************/

/** endpoint - depart **/
extern void selectDepartOperation(void);
extern void listDepartAttr(void);
extern void selectDepartModifyAttr(void);
extern void listDepartChildTeam(void);

/** endpoint - team **/
extern void selectTeamOperation(void);
extern void listTeamAttr(void);
extern void selectTeamModifyAttr(void);
extern void listTeamChildProject(void);

/** endpoint - project **/
extern void selectProjectOperation(void);
extern void listProjectAttr(void);
extern void selectProjectModifyAttr(void);

/******************************************************/

/** Additional typedef: Cursor **/

typedef struct _Cursor {
    int     type;   // 指向对象的类型(0-void|1-depart|2-team|3-project)
    void    *val;   // 指针值
} Cursor;

#endif
