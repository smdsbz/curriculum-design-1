#include "./utils/__init__.h"


/********** Views **********/

void listAllNodes(void) {
    Depart *d; Team *t; Project *p;
    for (d = mp.depart_head; d != NULL; d = d->next) {
        printf("%s\n", d->data->name);
        for (t = d->child_team_head; t != d->child_team_tail->next; t = t->next) {
            printf("    %s\n", t->data->name);
            for (p = t->child_project_head; p != t->child_project_tail->next; p = p->next) {
                printf("        %s\n", p->data->id);
            }
        }
    }
    putchar('\n');
}

void selectQueryObjects(void) {
    while (1) {
        puts(DOC_QUERY_OBJ);
        int oper_code = 0;
        printf("query > "); scanf("%d", &oper_code);
        switch (oper_code) {
            case 1: {
                selectQueryDepartMethod();
                break;
            }
            case 2: {
                // selectQueryTeamMethod();
                break;
            }
            case 3: {
                // selectQueryProjectMethod();
                break;
            }
            case 0: default: { break; }
        }
        if (oper_code == 0) {
            break;
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
                // TODO
            }
            case 2: {
                // TODO
            }
            case 0: default: { break; }
        }
        if (oper_code == 0) { break; }
    }   // input loop
}

void selectStatObjects(void) {
    int oper_code = 0;
    // TODO
}
