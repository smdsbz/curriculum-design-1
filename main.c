#include <stdio.h>
#include <stdlib.h>

#include "./utils/__init__.h"
#include "./views.h"
#include "./doc_strings.h"

// setting env
MountPoint mp;
char TGT_PATH[100] = "./";
Cursor cursor = {0, NULL};


/********** Views **********/

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
    if (head->depart == NULL) {
        puts("No record found!");
        return;
    }
    puts("        Name            |   Manager     |   Telephone\n\
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
                queryDepartByName();
                return;
            }
            case 2: {
                queryDepartByManager();
                return;
            }
            case 0: default: { break; }
        }
        if (oper_code == 0) { break; }
    }   // input loop
}

// void selectQueryTeamMethod(void) {
//     while (1) {
//         puts(DOC_QUERY_TEAM_METHOD);
//         int oper_code = 0;
//         printf("query/team > "); scanf("%d", &oper_code);
//         switch(oper_code) {
//             case 1: {
//                 queryTeamByName();
//                 return;
//             }
//             case 2: {
//                 // TODO
//             }
//             case 0: default: { break; }
//         }
//         if (oper_code == 0) { break; }
//     }   // input loop
// }


void queryDepartByName(void) {
    char depart_name[20];
    printf("query/depart::name > "); scanf("%s", depart_name);
    DepartWrapper *depart_wrapper = getDepartByName(mp.depart_head, NULL, depart_name);
    listDepartWrapper(depart_wrapper);
    if (depart_wrapper->depart != NULL) {
        int oper_code;
        printf("Set cursor to department: "); scanf("%d", &oper_code);
        DepartWrapper *cur = depart_wrapper;
        for (; oper_code != 1 && cur; cur = cur->next, --oper_code) ;
        if (cur == NULL) {
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
        for (; oper_code != 1 && cur; cur = cur->next, --oper_code) ;
        if (cur == NULL) {
            puts("Out of range!");
            cleanupDepartWrapper(depart_wrapper);
            return;
        }
        cursor.type = 1; cursor.val = (void *)cur->depart;
    }
    cleanupDepartWrapper(depart_wrapper);
}

void selectAddObjectType(void) {
    while (1) {
        puts(DOC_ADD_TYPE);
        int oper_code = 0;
        printf("> "); scanf("%d", &oper_code);
        switch (oper_code) {
            case 1: {
                appendDepart(mp.depart_head, initDepartData());
                return;
            }
            case 0: default: { break; }
        }
        if (oper_code == 0) { break; }
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
        puts(DOC_ROOT);
        int oper_code = 0;
        switch (cursor.type) {
            case 0: { printf("> "); break; }
            case 1: {
                printf("depart/%s >",
                       ((Depart *)cursor.val)->data->name);
                // indent-fixer
                break; }
        }
        scanf("%d", &oper_code);
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
                // selectStatObjects();
                break;
            }
            case 4: {
                selectAddObjectType();
            }
            case 0: default: {
                break;
            }
        }
        if (oper_code == 0) {
            if (saveData(mp, TGT_PATH) == 0) {
                puts("Seems like some unexpected happened while saving...");
                puts("GG! MY FRIEND!!!");
            }
            break;
        }
    }

    puts("exit");
    return 0;
}
