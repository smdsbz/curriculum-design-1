/*  $PROGRAM_ROOT/views.h
 *  所有视图函数与用户指针结构体声明
 */

#ifndef _VIEWS_H
#define _VIEWS_H

/** 顶层 **/
/* 列出所有节点 */
extern void listAllNodes(void);
/* 列出院系搜索结果中所有内容 */
extern void listDepartWrapper(DepartWrapper *);
/* 列出团队搜索结果中所有内容 */
extern void listTeamWrapper(TeamWrapper *);
/* 列出项目搜索结果的全部内容 */
extern void listProjectWrapper(ProjectWrapper *);
/* 将数据中存储的项目类型代码，转换为对应类别的名称 */
extern char *parseTypeCodeToString(char *, const char);

/**** 查询 ****/
/* 查询对象选择器 */
extern void selectQueryObjects(void);

/****** 查询方法选择 ******/
/* 查询院系方法选择器 */
extern void selectQueryDepartMethod(void);
/* 查询团队方法选择器 */
extern void selectQueryTeamMethod(void);

/******** 查询过程 ********/
/* 按名称查找院系 */
extern void queryDepartByName(void);
/* 按负责人查找院系 */
extern void queryDepartByManager(void);
/* 按名称查找团队 */
extern void queryTeamByName(void);
/* 按教师人数查找团队 */
extern void queryTeamByTeacherNum(void);
/* 按编号查找项目 */
extern void queryProjectById(void);

/**** 添加 ****/
/* 添加记录类别选择器 */
extern void selectAddObjectType(void);

/**** 统计 ****/
/* 数据统计功能选择器 */
extern void selectStatObject(void);
/* 列出院系人数统计结果 */
extern void listDepartHRStat(void);
/* 列出院系项目统计结果 */
extern void listDepartProjectStat(void);
/* 列出NSFC项目最多的10个团队 */
extern void listTeamNSFCProjectStat(void);
/* 列出项目数/教师比值前5团队 */
extern void listTeamProjectStat(void);
/* 列出院系项目平均经费统计结果 */
extern void listDepartFundingStat(void);

/******************************************************/

/** 院系层 **/
/* 院系操作选择器 */
extern void selectDepartOperation(void);
/* 列出当前指向院系的属性 */
extern void listDepartAttr(void);
/* 修改院系属性 */
extern void selectDepartModifyAttr(void);
extern void listDepartChildTeam(void);

/** 团队层 **/
/* 团队操作选择器 */
extern void selectTeamOperation(void);
/* 列出当前指向团队的属性 */
extern void listTeamAttr(void);
/* 修改团队属性 */
extern void selectTeamModifyAttr(void);
/* 列出团队开设的项目 */
extern void listTeamChildProject(void);

/** 项目层 **/
/* 项目操作选择器 */
extern void selectProjectOperation(void);
/* 列出当前指向项目的属性 */
extern void listProjectAttr(void);
/* 修改项目属性 */
extern void selectProjectModifyAttr(void);

/******************************************************/

/** 用户指针结构体定义 **/
typedef struct _Cursor {
    int     type;   // 指向对象的类型
                    // 0-置空 1-院系 2-团队 3-项目
    void    *val;   // 指针值
} Cursor;

#endif
