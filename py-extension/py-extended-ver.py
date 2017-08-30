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

try:
    print(DATA_FOLDER)
    curr.loadData(DATA_FOLDER)      # ram for data is shared by all users
    print('[LOG] successfully loaded from %s' % DATA_FOLDER)
except SystemError:
    print('[LOG] load failed!')
    exit(-1)


app = Flask(__name__)
app.secret_key = 'DogLeeNation(2B||!2B)-->|'




# '''
def check(func):
    @wraps(func)
    def checker(*args, **kwargs):
        if 'DATA_FOLDER' not in session:
            flash('Please specify data folder before you do anything!',
                 category='error')
            return redirect(url_for('quit'))
        return func(*args, **kwargs)
    return checker
# '''


#### Views ####

    ## Top ##

@app.route('/')
def index():
    if 'DATA_FOLDER' not in session:
        curr.loadData(DATA_FOLDER)
        session['DATA_FOLDER'] = DATA_FOLDER
    return render_template('index.html')

@app.route('/quit')
def quit():
    if 'DATA_FOLDER' in session:
        curr.cleanupAll()
        session.pop('DATA_FOLDER')
    return render_template('quit.html')


    ## Overview ##

@app.route('/overview/<object_type>')
@check
def overview(object_type):
    if object_type == 'depart':
        data = curr.getAllDepart()
    elif object_type == 'team':
        data = curr.getAllTeam()
    elif object_type == 'project':
        data = curr.getAllProject()
    else:
        flash('Unknown data type "%s"!' % object_type, 'error')
        data = None
    return render_template('overview.html',
                           object_type=object_type, data=data)


    ## Focused ##

@app.route('/depart/<int:idx>', methods=['GET', 'PUT', 'DELETE'])
@check
def depart_view(idx):
    Computer = curr.getDepartByIndex(idx)
    return render_template('depart.html', data=Computer)

@app.route('/team/<int:idx>', methods=['GET', 'PUT', 'DELETE'])
@check
def team_view(idx):
    Rocket = curr.getTeamByIndex(idx)
    return render_template('team.html', data=Rocket)

@app.route('/project/<int:idx>', methods=['GET', 'PUT', 'DELETE'])
@check
def project_view(idx):
    Manhatan = curr.getProjectByIndex(idx)
    Manhatan['Type'] = curr.parseTypeCodeToString(Manhatan['Type'])
    return render_template('project.html', data=Manhatan)


#### MAIN ####

if __name__ == '__main__':
    app.run(debug=True)
