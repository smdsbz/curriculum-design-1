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
        return PyLong_FromLong(0);
    }
    // list out for debugging
    if (mp.depart_head == NULL) {
        puts("No data!");
        return PyLong_FromLong(0);
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

    return PyLong_FromLong(1);
}

static PyObject *
curr_saveData(PyObject *self) {
    if (saveData(mp, TGT_PATH)) {
        return PyLong_FromLong(1);
    }
    return PyLong_FromLong(0);
}

static PyObject *
curr_cleanupAll(PyObject *self, PyObject *args) {
    // cleanups
    cleanupDepart(mp.depart_head);
    cleanupTeam(mp.team_head);
    cleanupProject(mp.project_head);
    return PyLong_FromLong(1);
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
    for (int cur = 0;
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
    PyDict_SetItemString(Computer, "Name",
                         Py_BuildValue("s", ((Depart *)cursor.val)->data->name));
    PyDict_SetItemString(Computer, "Manager",
                         Py_BuildValue("s", ((Depart *)cursor.val)->data->manager));
    PyDict_SetItemString(Computer, "Telephone",
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
    for (int cur = 0;
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
    PyDict_SetItemString(Rocket, "Name",
                         Py_BuildValue("s", ((Team *)cursor.val)->data->name));
    PyDict_SetItemString(Rocket, "Manager",
                         Py_BuildValue("s", ((Team *)cursor.val)->data->manager));
    PyDict_SetItemString(Rocket, "Department",
                         Py_BuildValue("s", ((Team *)cursor.val)->data->faculty));
    PyDict_SetItemString(Rocket, "Teacher Num.",
                         Py_BuildValue("i", ((Team *)cursor.val)->data->teacher_num));
    PyDict_SetItemString(Rocket, "Student Num.",
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
    cursor.type = 3; cursor.val = mp.project_head;
    for (int cur = 0;
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
    PyDict_SetItemString(Manhatan, "ID",
                         Py_BuildValue("s", ((Project *)cursor.val)->data->id));
    PyDict_SetItemString(Manhatan, "Manager",
                         Py_BuildValue("s", ((Project *)cursor.val)->data->manager));
    PyDict_SetItemString(Manhatan, "Type",
                         Py_BuildValue("c", ((Project *)cursor.val)->data->type));
    PyDict_SetItemString(Manhatan, "Start Date",
                         Py_BuildValue("s", ((Project *)cursor.val)->data->start_date));
    PyDict_SetItemString(Manhatan, "Funding",
                         Py_BuildValue("f", ((Project *)cursor.val)->data->funding));
    PyDict_SetItemString(Manhatan, "Team",
                         Py_BuildValue("s", ((Project *)cursor.val)->data->team));

    return Manhatan;
}

/**** get list of records ****/

static PyObject *
curr_getAllDepart(PyObject *self) {
    PyObject *departs = PyList_New(0);
    if (!mp.depart_head) { puts("no data!"); return departs; }
    for (Depart *cur = mp.depart_head; cur; cur = cur->next) {
        PyList_Append(departs, Py_BuildValue("s", cur->data->name));
    }
    return departs;
}

static PyObject *
curr_getAllTeam(PyObject *self) {
    PyObject *teams = PyList_New(0);
    if (!mp.team_head) { puts("no data!"); return teams; }
    for (Team *cur = mp.team_head; cur; cur = cur->next) {
        PyList_Append(teams, Py_BuildValue("s", cur->data->name));
    }
    return teams;
}

static PyObject *
curr_getAllProject(PyObject *self) {
    PyObject *projects = PyList_New(0);
    if (!mp.project_head) { puts("no data!"); return projects; }
    for (Project *cur = mp.project_head; cur; cur = cur->next) {
        PyList_Append(projects, Py_BuildValue("s", cur->data->id));
    }
    return projects;
}

/**** modify ****/

static PyObject *
curr_modifyFocusedDepart(PyObject *self, PyObject *args) {
    if (cursor.type != 1) { return NULL; }  // type mis-match
    const char *buf_str; const char *pto;
    if (!PyArg_ParseTuple(args, "ss", &pto, &buf_str)) { return NULL; }
    printf("%s::'%s'-->'%s'\n", ((Depart *)cursor.val)->data->name,
           pto, buf_str);
    if (strcmp("manager", pto) == 0) {
        strcpy(((Depart *)cursor.val)->data->manager, buf_str);
    }
    else if (strcmp("mobile", pto) == 0) {
        strcpy(((Depart *)cursor.val)->data->mobile, buf_str);
    } else { return PyLong_FromLong(0); }
    return PyLong_FromLong(1);
}

static PyObject *
curr_modifyFocusedTeam(PyObject *self, PyObject *args) {
    if (cursor.type != 2) { return NULL; }
    const char *buf_str; const char *pto;
    if (!PyArg_ParseTuple(args, "ss", &pto, &buf_str)) { return NULL; }
    printf("%s::'%s'-->%s\n", ((Team *)cursor.val)->data->name,
           pto, buf_str);
    if (strcmp("manager", pto) == 0) {
        strcpy(((Team *)cursor.val)->data->manager, buf_str);
    }
    else if (strcmp("teacher_num", pto) == 0) {
        ((Team *)cursor.val)->data->teacher_num = atoi(buf_str);
    }
    else if (strcmp("student_num", pto) == 0) {
        ((Team *)cursor.val)->data->student_num = atoi(buf_str);
    } else { return PyLong_FromLong(0); }
    return PyLong_FromLong(1);
}

static PyObject *
curr_modifyFocusedProject(PyObject *self, PyObject *args) {
    if (cursor.type != 3) { return NULL; }
    const char *buf_str; const char *pto;
    if (!PyArg_ParseTuple(args, "ss", &pto, &buf_str)) { return NULL; }
    printf("%s::'%s'-->%s\n", ((Project *)cursor.val)->data->id,
           pto, buf_str);
    if (strcmp("type", pto) == 0) {
        ((Project *)cursor.val)->data->type = *buf_str;
    }
    else if (strcmp("funding", pto) == 0) {
        ((Project *)cursor.val)->data->funding = (float)atof(buf_str);
    }
    else if (strcmp("manager", pto) == 0) {
        strcpy(((Project *)cursor.val)->data->manager, buf_str);
    } else { return NULL; }
    return PyLong_FromLong(1);
}

/**** delete ****/

static PyObject *
curr_removeFocusedDepart(PyObject *self) {
    // printf("[C] entered curr_removeFocusedDepart(), cursor.type is %d\n", cursor.type);
    if (cursor.type != 1) { puts("[C] type mis-match"); return NULL; }
    printf("deleting depart/%s\n", ((Depart *)cursor.val)->data->name);
    if (removeDepart(&(mp.depart_head), (Depart *)cursor.val)) {
        cursor.type = 0; cursor.val = NULL;
        return PyLong_FromLong(1);
    } else { return PyLong_FromLong(0); }
}

static PyObject *
curr_removeFocusedTeam(PyObject *self) {
    if (cursor.type != 2) { return NULL; }
    if (removeTeam(&(mp.team_head), (Team *)cursor.val)) {
        cursor.type = 0; cursor.val = NULL;
        return PyLong_FromLong(1);
    } else { return PyLong_FromLong(0); }
}

static PyObject *
curr_removeFocusedProject(PyObject *self) {
    if (cursor.type != 3) { return NULL; }
    if (removeProject(&(mp.project_head), (Project *)cursor.val)) {
        cursor.type = 0; cursor.val = NULL;
        return PyLong_FromLong(1);
    } else { return PyLong_FromLong(0); }
}

/**** query ****/

static PyObject *
_getDepartIdxPyString(Depart *tgt) {
    int idx = 0; Depart *cur = mp.depart_head;
    for (; cur && cur != tgt; cur=cur->next, idx++) ;
    if (cur == NULL) { return NULL; } // no match - to NULL
    else {
        return Py_BuildValue("i", idx);
    }
}

static PyObject *
_convertDepartWrapperToPyDict(DepartWrapper *wpr_head) {
    if (wpr_head == NULL) { return NULL; }
    PyObject *rtn = PyDict_New();
    for (; wpr_head; wpr_head = wpr_head->next) {
        PyDict_SetItemString(rtn, wpr_head->depart->data->name,
                             _getDepartIdxPyString(wpr_head->depart));
    }
    return rtn;
}

static PyObject *
curr_queryDepart(PyObject *self, PyObject *args) {
    const char *pto; const char *buf_str;
    DepartWrapper *C_rst; PyObject *Py_rst;
    if (!PyArg_ParseTuple(args, "ss", &pto, &buf_str)) { return NULL; }
    if (strcmp("name", pto) == 0) {
        puts("querying depart by name");
        C_rst = getDepartByName(mp.depart_head, NULL, buf_str);
    }
    else if (strcmp("manager", pto) == 0) {
        C_rst = getDepartByManager(mp.depart_head, NULL, buf_str);
    } else { return NULL; }
    Py_rst = _convertDepartWrapperToPyDict(C_rst);
    cleanupDepartWrapper(C_rst);
    return Py_rst;
}

/**** Mellxos ****/

static PyObject *
curr_parseTypeCodeToString(PyObject *self, PyObject *args) {
    const char from;
    if (!PyArg_ParseTuple(args, "c", &from)) { return NULL; }
    switch (from) {
        case '1': { return Py_BuildValue("s", "973 Program"); }
        case '2': { return Py_BuildValue("s", "NSFC"); }
        case '3': { return Py_BuildValue("s", "863 Program"); }
        case '4': { return Py_BuildValue("s", "International Cooperation"); }
        case '5': { return Py_BuildValue("s", "Transverse"); }
        default: { return Py_BuildValue("s", "---"); }
    }
}



/**** Py Packaging ****/

static PyMethodDef CurrMethods[] = {
    // FILE
    { "loadData", curr_loadData, METH_VARARGS,
        "load and print all data" },
    { "cleanupAll", curr_cleanupAll, METH_VARARGS,
        "cleanup everything"},
    // FOCUSING
    { "getDepartByIndex", curr_getDepartAttr, METH_VARARGS,
        "get department attribute, and store them in a python dict!" },
    { "getTeamByIndex", curr_getTeamtAttr, METH_VARARGS,
        "as above, get team data this time" },
    { "getProjectByIndex", curr_getProjectAttr, METH_VARARGS,
        "..." },
    // LISTING
    { "getAllDepart", curr_getAllDepart, METH_VARARGS,
        "get a list of depart names" },
    { "getAllTeam", curr_getAllTeam, METH_VARARGS,
        "get a list of team names" },
    { "getAllProject", curr_getAllProject, METH_VARARGS,
        "get a list of project IDs" },
    // QUERYING
    { "queryDepart", curr_queryDepart, METH_VARARGS,
        "query department" },
    // MODIFYING
    { "modifyFocusedDepart", curr_modifyFocusedDepart, METH_VARARGS,
        "modify depart info" },
    { "modifyFocusedTeam", curr_modifyFocusedTeam, METH_VARARGS,
        "modify team info" },
    { "modifyFocusedProject", curr_modifyFocusedProject, METH_VARARGS,
        "modify project info" },
    // DELETING
    { "removeFocusedDepart", curr_removeFocusedDepart, METH_VARARGS,
        "delete depart record" },
    { "removeFocusedTeam", curr_removeFocusedTeam, METH_VARARGS,
        "delete team record" },
    { "removeFocusedProject", curr_removeFocusedProject, METH_VARARGS,
        "delete project record" },
    // M
    { "parseTypeCodeToString", curr_parseTypeCodeToString, METH_VARARGS,
        "convert project type code to according type string" },
    { "saveData", curr_saveData, METH_VARARGS,
        "save(cover) original data" },
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
