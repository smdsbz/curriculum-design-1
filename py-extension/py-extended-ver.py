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
    # if curr.saveData():     # NOTE: use this one for production
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


    ## Query ##

@app.route('/query/<object_type>', methods = ['GET', 'POST'])
@check
def query(object_type):
    if request.method == 'GET':
        if object_type == 'depart':
            data = curr.queryDepart('name', '$ VV ^ 6') # get u nothing
            print(data)
        elif object_type == 'team':
            data = curr.queryTeam('name', '$ VV ^ 6')
        elif object_type == 'project':
            data = curr.queryProject('id', '$VV^6')
        return render_template('query.html', object_type=object_type, data=data)


    if request.method == 'POST':

        if object_type == 'depart':
            partial = request.form.get('query-partial', None)
            content = request.form.get('query-content-2', None)
            if partial and content:
                data = curr.queryDepart(partial, content)
                return render_template('query.html',
                                       object_type=object_type, data=data)
            else:
                flash('Error while querying!', category='warning')
                return redirect(url_for('query'))

        if object_type == 'team':
            partial = request.form.get('query-partial', None)
            content = request.form.get('query-content-2', None)

            if partial == 'teacher_num':
                content_1 = request.form.get('query-content-1', None)
                content_2 = content
                # validation
                if content_1 is not ('<' or '>' or '='):
                    flash('Try fvck my code :-)', category='error')
                    return redirect(url_for('quit'))
                try:
                    int(content_2)
                except ValueError:
                    flash('Try fvck my code :-)', category='error')
                    return redirect(url_for('quit'))
                # OK
                content = content_1 + ' ' + str(content_2)
                # print(content)

            if partial and content:
                data = curr.queryTeam(partial, content)
                return render_template('query.html',
                                       object_type=object_type, data=data)

            # HACK: ask hackers to leave
            flash('Try fvck my code :-)', category='error')
            return redirect(url_for('quit'))


        if object_type == 'project':
            partial = 'id' or request.form.get('query-partial', None) # always 'id'
            content = request.form.get('query-content-2', None)
            if partial and content:
                data = curr.queryProject(partial, content)
                return render_template('query.html',
                                       object_type=object_type, data=data)
            else:
                flash('Error while querying!', category='warning')
                return redirect(url_for('query'))


@app.route('/query/<object_type>/<int:idx>/subnodes/')
@check
def subnodes(object_type, idx):
    if object_type == 'depart':
        Computer = curr.getDepartByIndex(idx)
        data = curr.getTeamByDepart()
        return render_template('subnodes.html', object_type='team', data=data)
    if object_type == 'team':
        Rocket = curr.getTeamByIndex(idx)
        data = curr.getProjectByTeam()
        return render_template('subnodes.html', object_type='project', data=data)
    else:
        flash('Undefined behaviour!', category='warning')
        return redirect(url_for('index'))


@app.route('/redirecting/<object_type>/parent')
@check
def redirecting(object_type):
    if object_type == 'team':
        return redirect(url_for('depart_view',
                                idx=curr.getDepartByTeam()))
    if object_type == 'project':
        return redirect(url_for('team_view',
                                idx=curr.getTeamByProject()))
    else:
        flash('Undefined behaviour!', category='warning')
        return redirect(url_for('index'))

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
            # input check
            if partial is 'funding':
                try:
                    float(content)
                except ValueError:
                    flash('Try fvck my code :-)', category='error')
                    return redirect(url_for('quit'))
            # OK
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


    ## STAT ##

@app.route('/stat/<int:nu>')
@check
def stat_view(nu):
    if nu == 1:
        data = curr.stat_1()
    elif nu == 2:   # individual view return
        year = request.args.get('year', '0')
        try:
            year = int(year)
        except ValueError:
            year = 0
        data = curr.stat_2(year)
        return render_template('stat.html', nu=nu, year=year, data=data)
    elif nu == 3:
        data = curr.stat_3()
    elif nu == 4:
        data = curr.stat_4()
    elif nu == 5:
        data = curr.stat_5()

    return render_template('stat.html', nu=nu, data=data)

#### MAIN ####

if __name__ == '__main__':
    app.run(debug=True)
