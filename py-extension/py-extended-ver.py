#!/usr/bin/env python3


from flask import Flask, request, session
from flask import url_for, redirect, render_template, flash

from functools import wraps
import os, sys

import curr # written in Pure C

#### Initializations ####

if len(sys.argv) == 1:
    # DATA_FOLDER = os.path.abspath('../utils/data')
    DATA_FOLDER = os.path.abspath('../utils/data')
elif len(sys.argv) == 2:
    DATA_FOLDER = os.path.abspath(sys.argv[1])
else:
    puts('Too many arguments!')
    exit(-1)


print(DATA_FOLDER)
if curr.loadData(DATA_FOLDER):      # ram for data is shared by all users
    print('[LOG] successfully loaded from %s' % DATA_FOLDER)
else:
    print('[LOG] load failed!')
    exit(-1)

app = Flask(__name__)
app.secret_key = 'DogLeeNation(2B||!2B)-->|'




# '''
def check(func):
    @wraps(func)
    def checker(*args, **kwargs):
        if 'DATA_FOLDER' not in session:
            flash('Please load data before you do anything!',
                 category='error')
            return redirect(url_for('quit'))
        return func(*args, **kwargs)
    return checker
# '''


#### Views ####

    ## Top ##

@app.route('/')
def index():
    if 'DATA_FOLDER' not in session:    # NOTE: single user allowed!
        curr.loadData(DATA_FOLDER)
        session['DATA_FOLDER'] = DATA_FOLDER
    return render_template('index.html')

@app.route('/quit')
def quit():
    if 'DATA_FOLDER' in session:
        curr.cleanupAll()
        session.pop('DATA_FOLDER')
    return render_template('quit.html')

@app.route('/saving')
def saving():
    # if curr.saveData():
    if True:    # NOTE: use this for BUILDING stage
        flash('Changes saved!', category='success')
    else:
        flash('Error while saving!', category='error')
    return redirect(url_for('index'))

    ## Overview ##

@app.route('/overview/<object_type>')
@check
def overview(object_type):
    if object_type == 'depart':
        data = curr.getAllDepart()
        print(data)
    elif object_type == 'team':
        data = curr.getAllTeam()
    elif object_type == 'project':
        data = curr.getAllProject()
    else:
        flash('Unknown data type "%s"!' % object_type, category='warning')
        data = None
    return render_template('overview.html',
                           object_type=object_type, data=data)


    ## Focused ##

@app.route('/depart/<int:idx>', methods=['GET', 'PUT', 'DELETE'])   # REST not supported
@check
def depart_view(idx):
    Computer = curr.getDepartByIndex(idx)   # set cursor.val (in C ext. - curr)

    if request.method == 'PUT' or request.args.get('action', None) == 'modify':
        # get modify info
        partial = request.args.get('modify-partial', None)
        content = request.args.get('modify-content', None)
        print(partial, content)
        if partial and content: # double check
            if curr.modifyFocusedDepart(partial, content):
                flash('Changed successfully!', category='success')
            else:
                flash('Error while modifying!', category='error')
            # Computer = curr.getDepartByIndex(idx)   # refresh data
        return redirect(url_for('depart_view', idx=idx))

    if request.method == 'DELETE' or request.args.get('action', None) == 'delete':
        # print(Computer)
        ret_code = curr.removeFocusedDepart()
        if ret_code == 1:
            flash("Department successfully deleted!", category='success')
        elif ret_code == 0:
            flash("The department you are deleting has affiliated teams!", category='warning')
        else:
            flash("Error while removing '%s'" % Computer['Name'], category='error')
        return redirect(url_for('overview', object_type='depart'))

    if request.method == 'GET':
        return render_template('depart.html', data=Computer)


@app.route('/team/<int:idx>', methods=['GET', 'PUT', 'DELETE'])
@check
def team_view(idx):
    Rocket = curr.getTeamByIndex(idx)

    if request.method == 'PUT' or request.args.get('action', None) == 'modify':
        partial = request.args.get('modify-partial', None)
        content = request.args.get('modify-content', None)
        if partial and content:
            # input check
            if partial is 'teacher_num' or 'student_num':
                try:
                    int(content)    # content should be a integer
                except ValueError:
                    flash('Try fvck my code :-)', category='error')
                    return redirect(url_for('quit'))    # ask hackers to leave
            # everything's fine
            if curr.modifyFocusedTeam(partial, content):
                flash('Changed successfully!', category='success')
            else:
                flash('Error while modifying!', category='error')
        return redirect(url_for('team_view', idx=idx))

    if request.method == 'DELETE' or request.args.get('action', None) == 'delete':
        ret_code = curr.removeFocusedTeam()
        if ret_code == 1:
            flash("Team successfully deleted!", category='success')
        elif ret_code == 0:
            flash("The team you are deleting has affiliated projects!", category='warning')
        else:
            flash("Error while removing '%s'" % Rocket['Name'], category='error')
        return redirect(url_for('overview', object_type='team'))

    if request.method == 'GET':
        return render_template('team.html', data=Rocket)


@app.route('/project/<int:idx>', methods=['GET', 'PUT', 'DELETE'])
@check
def project_view(idx):
    Manhatan = curr.getProjectByIndex(idx)
    Manhatan['Type'] = curr.parseTypeCodeToString(Manhatan['Type']) # little trick

    if request.method == 'PUT' or request.args.get('action', None) == 'modify':
        partial = request.args.get('modify-partial', None)
        content = request.args.get('modify-content', None)
        if partial and content:
            if curr.modifyFocusedProject(partial, content):
                flash('Changed successfully!', category='success')
            else:
                flash('Error while modifying!', category='error')
        return redirect(url_for('project_view', idx=idx))

    if request.method == 'DELETE' or request.args.get('action', None) == 'delete':
        ret_code = curr.removeFocusedProject()
        if ret_code == 1:
            flash('Project successfully deleted!', category='success')
        else:
            flash("Error while removing '%s'" % Manhatan['ID'], category='error')
        return redirect(url_for('overview', object_type='project'))

    if request.method == 'GET':
        return render_template('project.html', data=Manhatan)

#### MAIN ####

if __name__ == '__main__':
    app.run(debug=True)
