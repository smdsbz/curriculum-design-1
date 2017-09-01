# curriculum-design-1

## branch/py-extended

### Overview

This branch (used to be a branch, at least...) added a WebApp-like User Interface to the original (ugly) Command-Line Interface.  

You can see from the `.py` and `.html` files that everything related to data operation is done by the `curr` module, which is merely `.c`s under `/utils/` wrapped up (look into `./curr.c` if you're intrested in how to *wrap up*) and compiled into *Shared Objects* (a.k.a. `.so` files under *nix)

### How to Run?
*It's really SIMPLE!*
Under *nix:
```
# Switch to this folder
curriculum-design-1 $ cd ./py-extended

# build C source into a single .so file,
#   which will be stored under
#       `./build/lib.linux-$SYS_ARCH-$PY_VER/`
#   named
#       `curr.$PY_INTERPRETER.so`
py-extended $ ./setup.py build

# link the file to $curdir
#   (so you don't have to copy it every time you compile)
# my command is as follow...
py-extended $ ln -s ./build/lib<TAB>/curr<TAB> ./

# Now run it!
# NOTE: if you don't specify the data folder,
#       it will load from `../utils/data/`
#       where stores legacy data for debuggin the CLI ver.
py-extended $ python3 ./py-extended-ver.py
```
- - - - - - - - - - - - - - - - - - - - - - - - - - - -

### Requirements

Since this is a curriculum-design proj. for our most beloved **C** programming language (clearly I'm not stepping over the line:joy:),
Python only requires my favourite Web microframework [`Flask`](flask.pocoo.org) (click to visit their website), which is powering the navigation system of this *Offline WebApp*.  
You can install it by `pip3 install flask`.


- - - - - - - - - - - - - - - - - - - - - - - - - - - -

### A Few Things You May Concern...

#### Safety Issue

See that `@checker` and `try-except` all piling up?
Try **REAL HARD** and leave a comment in the issue column :-)  

#### the "Save" button won't save?

Yes. But you can re-enable it by changing the first `if` statement in the `saving()` process-func.
(It's highlighted by a `NOTE` flag)  

#### Can I specify another data folder?

Yes. By typing `python3 ./py-extended-ver.py $PATH_TO_DATA`,
and follow the instructions printed on your screen.

- - - - - - - - - - - - - - - - - - - - - - - - - - - -

### Special Thanks to Others' Work

- [MaterializeCSS](materilizecss.com): easy to use, hackable
- [Flask](flask.pocoo.com): no need to explain why :-)
- Toastr: flash() with style! (works with MaterializeCSS)
