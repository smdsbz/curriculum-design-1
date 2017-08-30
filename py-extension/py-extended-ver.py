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
    curr.loadData(DATA_FOLDER)
    print('[LOG] successfully loaded from %s' % DATA_FOLDER)
except SystemError:
    print('[LOG] load failed!')
    exit(-1)

app = Flask(__name__)
app.secret_key = 'DogLeeNation(2B||!2B)-->|'




'''
def check(func):
    @wraps(func)
    def checker(*args, **kwargs):
        if not DATA_FOLDER:
            flash('Please specify data folder before you do anything!',
                 category='error')
            return redirect(url_for('index'))
        return func(*args, **kwargs)
    return checker
'''


#### Views ####

    ## Top ##

@app.route('/')
def index():
    return render_template('index.html')


    ## Overview ##

@app.route('/overview/<object_type>')
def overview(object_type):
    if object_type == 'depart':
        data = ['Computer', 'Physics', 'Chemistry'] or None
    else:
        flash('undefined behaviour!', 'error')
        data = None
    return render_template('overview.html',
                           object_type=object_type, data=data)


    ## Focused ##

@app.route('/depart/<int:idx>', methods=['GET', 'PUT', 'DELETE'])
def depart_view(idx):
    Computer = curr.getDepartByIndex(idx)
    return render_template('depart.html', data=Computer)


#### MAIN ####

if __name__ == '__main__':
    app.run(debug=True)
