#include "../utils/__init__.h"
#include "../views.h"
#include "../doc_strings.h"
#include "/usr/include/python3.5/Python.h"

MountPoint mp;              // 头节点挂载点组
char *TGT_PATH;      // 数据文件夹路径buffer
Cursor cursor = {0, NULL};  // 用户指针

/**** File Functions ****/

static PyObject *
curr_loadData(PyObject *self, PyObject *args) {
    if (!PyArg_ParseTuple(args, "s", &TGT_PATH)) { return NULL; }
    // Reading data
    puts(TGT_PATH);
    mp = loadData(TGT_PATH);
    if (mp.depart_head == NULL
            || mp.team_head == NULL
            || mp.project_head == NULL) {
        return NULL;
    }
    // list out for debugging
    if (mp.depart_head == NULL) {
        puts("No data!");
        return PyLong_FromLong(1);
    }
    Depart *d; Team *t; Project *p;
    for (d = mp.depart_head; d != NULL; d = d->next) {
        printf("%s\n", d->data->name);
        if (d->child_team_head) {
            for (t = d->child_team_head;
                    t != d->child_team_tail->next;
                    t = t->next) {
                printf("    %s\n", t->data->name);
                if (t->child_project_head) {
                    for (p = t->child_project_head;
                            p != t->child_project_tail->next;
                            p = p->next) {
                        printf("        %s\n", p->data->id);
                    }
                }
            }
        }
    }
    putchar('\n');
    // list finished

    return PyLong_FromLong(0);
}

static PyObject *
curr_cleanupAll(PyObject *self, PyObject *args) {
    // cleanups
    cleanupDepart(mp.depart_head);
    cleanupTeam(mp.team_head);
    cleanupProject(mp.project_head);
    return PyLong_FromLong(0);
}

/**** Focus on specific object ****/

static PyObject *
curr_getDepartAttr(PyObject *self, PyObject *args) {
    const int idx;
    if (!PyArg_ParseTuple(args, "i", &idx)) { return NULL; }
    printf("idx == %d\n", idx);
    if (!mp.depart_head) { puts("Please load data before you query!"); return NULL; }
    // getting object
    cursor.type = 1; cursor.val = mp.depart_head;
    for (int cur = 1;
         cursor.val && cur < idx;
         cur++, cursor.val = ((Depart *)cursor.val)->next) ;
    if (!cursor.val) { puts("Out of Range!"); return NULL; }
    // listDepartAttr();
    Depart *tgt = (Depart *)cursor.val;
    puts("\
        ATTR     |    VALUE\n\
     ------------|-------------------");
    printf("\
      Name       |  %s\n", tgt->data->name);
    printf("\
      Manager    |  %s\n", tgt->data->manager);
    printf("\
      Telephone  |  %s\n", tgt->data->mobile);
    putchar('\n');
    // end listDepartAttr();
    // now return a dict!
    PyObject *Computer = PyDict_New();
    PyDict_SetItemString(Computer, "name",
                         Py_BuildValue("s", ((Depart *)cursor.val)->data->name));
    PyDict_SetItemString(Computer, "manager",
                         Py_BuildValue("s", ((Depart *)cursor.val)->data->manager));
    PyDict_SetItemString(Computer, "mobile",
                         Py_BuildValue("s", ((Depart *)cursor.val)->data->mobile));
    return Computer;
}

static PyObject *
curr_getTeamtAttr(PyObject *self, PyObject *args) {
    const int idx;
    if (!PyArg_ParseTuple(args, "i", &idx)) { return NULL; }
    printf("idx == %d\n", idx);
    if (!mp.team_head) { puts("Please load data before you query!"); return NULL; }
    // getting object
    cursor.type = 2; cursor.val = mp.team_head;
    for (int cur = 1;
         cursor.val && cur < idx;
         cur++, cursor.val = ((Team *)cursor.val)->next) ;
    if (!cursor.val) { puts("Out of Range!"); return NULL; }
    // listTeamAttr();
    Team *tgt = (Team *)cursor.val;
    puts("\
        ATTR     |    VALUE\n\
     ------------|-------------------");
    printf("\
      Name       |  %s\n", tgt->data->name);
    printf("\
      Manager    |  %s\n", tgt->data->manager);
    printf("\
      Faculty    |  %s\n", tgt->data->faculty);
    printf("\
      TeacherNum |  %d\n", tgt->data->teacher_num);
    printf("\
      StudentNum |  %d\n", tgt->data->student_num);
    putchar('\n');
    // end listTeamAttr();
    // now return a dict!
    PyObject *Rocket = PyDict_New();
    PyDict_SetItemString(Rocket, "name",
                         Py_BuildValue("s", ((Team *)cursor.val)->data->name));
    PyDict_SetItemString(Rocket, "manager",
                         Py_BuildValue("s", ((Team *)cursor.val)->data->manager));
    PyDict_SetItemString(Rocket, "faculty",
                         Py_BuildValue("s", ((Team *)cursor.val)->data->faculty));
    PyDict_SetItemString(Rocket, "teacher_num",
                         Py_BuildValue("i", ((Team *)cursor.val)->data->teacher_num));
    PyDict_SetItemString(Rocket, "student_num",
                         Py_BuildValue("i", ((Team *)cursor.val)->data->student_num));
    return Rocket;
}

static PyObject *
curr_getProjectAttr(PyObject *self, PyObject *args) {
    const int idx;
    if (!PyArg_ParseTuple(args, "i", &idx)) { return NULL; }
    printf("idx == %d\n", idx);
    if (!mp.project_head) { puts("Please load data before you query!"); return NULL; }
    // getting object
    cursor.type = 2; cursor.val = mp.project_head;
    for (int cur = 1;
         cursor.val && cur < idx;
         cur++, cursor.val = ((Project *)cursor.val)->next) ;
    if (!cursor.val) { puts("Out of Range!"); return NULL; }
    // listProjectAttr();
    Project *tgt = (Project *)cursor.val;
    puts("\
        ATTR     |    VALUE\n\
     ------------|-------------------");
    printf("\
      ID         |  %s\n", tgt->data->id);
    printf("\
      TypeCode   |  %c\n", tgt->data->type);
    printf("\
      StartDate  |  %s\n", tgt->data->start_date);
    printf("\
      Funding    |  %f *10000 CNY\n", tgt->data->funding);
    printf("\
      Manager    |  %s\n", tgt->data->manager);
    printf("\
      Team       |  %s\n", tgt->data->team);
    putchar('\n');
    // end listProjectAttr();
    // now return a dict!
    PyObject *Manhatan = PyDict_New();
    PyDict_SetItemString(Manhatan, "id",
                         Py_BuildValue("s", ((Project *)cursor.val)->data->id));
    PyDict_SetItemString(Manhatan, "type",
                         Py_BuildValue("c", ((Project *)cursor.val)->data->type));
    PyDict_SetItemString(Manhatan, "start_date",
                         Py_BuildValue("s", ((Project *)cursor.val)->data->start_date));
    PyDict_SetItemString(Manhatan, "funding",
                         Py_BuildValue("f", ((Project *)cursor.val)->data->funding));
    PyDict_SetItemString(Manhatan, "team",
                         Py_BuildValue("s", ((Project *)cursor.val)->data->team));
    PyDict_SetItemString(Manhatan, "manager",
                         Py_BuildValue("s", ((Project *)cursor.val)->data->manager));

    return Manhatan;
}

/**** get list of records ****/

/**** Mellxos ****/

static PyObject *
curr_parseTypeCodeToString(PyObject *self, PyObject *args) {
    const char from;
    if (!PyArg_ParseTuple(args, "c", &from)) { return NULL; }
    char to[50];
    switch (from) {
        case '1': { return Py_BuildValue("s", "973 Program"); }
        case '2': { return Py_BuildValue("s", "NSFC"); }
        case '3': { return Py_BuildValue("s", "863 Program"); }
        case '4': { return Py_BuildValue("s", "International Cooperation"); }
        case '5': { return Py_BuildValue("s", "Transverse"); }
        default: { return Py_BuildValue("s", "---"); }
    }
    return NULL;
}


/**** Py Packaging ****/

static PyMethodDef CurrMethods[] = {
    // FILE
    { "loadData", curr_loadData, METH_VARARGS,
        "load and print all data" },
    { "cleanupAll", curr_cleanupAll, METH_VARARGS,
        "cleanup everything"},
    // TEST
    { "getDepartByIndex", curr_getDepartAttr, METH_VARARGS,
        "get department attribute, and store them in a python dict!" },
    { "getTeamByIndex", curr_getTeamtAttr, METH_VARARGS,
        "as above, get team data this time" },
    { "getProjectByIndex", curr_getProjectAttr, METH_VARARGS,
        "..." },
    ///////////////////////////////
    // { "getAllRecords", curr_getAllRecords, METH_VARARGS,
    //     "return a list containing all records" },
    // M
    { "parseTypeCodeToString", curr_parseTypeCodeToString, METH_VARARGS,
        "convert project type code to according type string" },
    { NULL, NULL, 0, NULL }
};

static struct PyModuleDef currmodule = {
    PyModuleDef_HEAD_INIT,
    "curr",
    NULL,
    -1,
    CurrMethods
};

PyMODINIT_FUNC
PyInit_curr(void) {
    PyObject *m;
    m = PyModule_Create(&currmodule);
    return m;
}
