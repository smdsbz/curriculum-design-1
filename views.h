#ifndef _VIEWS_H
#define _VIEWS_H

/** __ **/

extern void listAllNodes(void);
extern void listDepartWrapper(DepartWrapper *);

/**** query ****/

extern void selectQueryObjects(void);


/****** query method ******/

extern void selectQueryDepartMethod(void);
// extern void selectQueryTeamMethod(void);
// extern void selectQueryProjectMethod(void);

/****** query deoart ******/

extern void queryDepartByName(void);
extern void queryDepartByManager(void);

/**** stat ****/

// extern void selectStatObjects(void);

/**** add ****/

extern void selectAddObjectType(void);

/** Additional typedef: Cursor **/

typedef struct _Cursor {
    int     type;   // 指向对象的类型(0-void|1-depart|2-team|3-project)
    void    *val;   // 指针值
} Cursor;

#endif
