#include <stdio.h>
#include <stdlib.h>

#include "./utils/__init__.h"
#include "./views.h"
#include "./doc_strings.h"

// setting data mounting points
MountPoint mp;
char TGT_PATH[100] = ".\\";


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

/*********  Main Bone **********/

int main(int argc, char const *argv[]) {

    // welcome screen
    puts(START);

    // setting up TGT_PATH
    if (argc == 1) {
        puts("Data folder unspecified!");
        puts("Finding data files in program root...");
    } else if (argc == 2) {
        printf("Loading data from folder \"%s\"...\n", TGT_PATH);
        strcpy(TGT_PATH, argv[1]);
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
                selectStatObjects();
                break;
            }
            case 0: default: {
                break;
            }
        }
        if (oper_code == 0) {
            break;
        }
    }

    puts("exit");
    return 0;
}
