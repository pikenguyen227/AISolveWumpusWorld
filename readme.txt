Wumpus Simulator v2.9 (released 09/15/2017)

Copyright (c) 2017. Washington State University.

Written by Larry Holder (holder@wsu.edu).

The Wumpus Simulator is a simple framework for simulating the Wumpus World
describing in Russell and Norvig's "Artificial Intelligence: A Modern
Approach". The simulator is written in C++, but you can design your agent in
either C++ or Python. The idea is for you to modify the Agent.h and Agent.cc
(or PyAgent.py) files to implement your super-smart agent. The C++ agent
provided simply accepts commands from the keyboard and executes them in the
simulator.  The keyboard commands are 'f' for forward, 'l' for turnleft, 'r'
for turnright, 'g' for grab, 's' for shoot, and 'c' for climb. The Python agent
provided simply goes forward everytime.

Quick Start
-----------

To try out the simulator, install the code on a UNIX system (or a system that
has the 'make' program installed and a C++ compiler). Type 'make' to build the
'wumpsim' executable. Then, type './wumpsim'. You should see a
randomly-generated 4x4 world, information about the game state, and a prompt
for the next action.When the game is over, scoring information is provided.

If you want to code your agent in Python, see the "Python Agent" section below.

Simulator Options
-----------------

The wumpus simulator takes a few options, as described below.

-size <N> lets you to set the size of the world to NxN (N>1). Default is 4.

-trials <N> runs the simulator for N trials, where each trial generates a new
wumpus world. Default is 1.

-tries <N> runs each trial of the simulator N times, giving your agent multiple
tries at the same world. Default is 1.

-seed <N> lets you set the random number seed to some positive integer so that
the simulator generates the same sequence of worlds each run. By default, the
random number seed is set semi-randomly based on the clock.

-world <file> lets you play a specific world as specified in the given file.
The -size option is ignored, and each try and trial uses the same world. The
format of the world file is as follows (all lowercase, must appear in this
order):

size N
wumpus N N
gold N N
pit N N
pit N N
...

where N is a positive integer. Some error checking is performed. A sample
world file is provided in testworld.txt.

Simulator Details
-----------------

The simulator works by generating a new world and a new agent for each trial.
Before each try on this world, the agent's Initialize() method is called, which
you can use to perform any pre-game preparation. Then, the game starts.  The
agent's Process() method is called with the current Percept, and the agent
should return an action, which is performed in the simulator. This continues
until the game is over (agent dies or leaves cave) or the maximum number of
moves (1000) is exceeded. When the game is over, the Agent's GameOver() method
is called. If additional tries are left for this world, then the world is
re-initialized, and the agent's Initialize() method is called again, and play
proceeds on another instance of the same game.

After the number of tries is completed, the agent is deleted. So, you may want
to store some information in the agent's destructor method to be reloaded
during the agent's constructor method when reborn for the next trial. If
additional trials have been requested, then a new wumpus world is generated,
and the process continues as described above.

Scoring information is output at the end of each try, each trial and at the end
of the simulation.

Agent Details
-------------

Your agent must include at least five methods: constructor, destructor,
Initialize, Process, and GameOver. You may change any or all of these methods
to implement your agent. And you may include additional methods as you see fit.
You may also take advantage of the other classes defined for the simulator
(e.g., Percept, Action, Orientation, Location, WorldState, WumpusWorld) by
including them directly, subclassing them, or borrowing code for your agent.
Once your agent is complete, simply type 'make' to remake the simulator with
your C++ agent. Then run 'wumpsim' to try it out. Or, for a Python agent,
simply run 'pywumpsim'; no need to recompile the simulator.

Python Agent
------------

You can also implement your agent in Python. First, you will need to compile a
different version of the simulator that supports calling external Python agent
functions. Instead of 'make' or 'make wumpsim', you should instead compile the
simulator using 'make pywumpsim'. But before that, edit the Makefile to make
sure the PYTHON_INC variable is set to the path where your system keeps the
python header files (specifically, Python.h), and the PYTHON_LIB variable is
set to the path where your system keeps the python library files. The values in
this distribution are for a MacOS platform. This version assumes you are using
Python 2.7. On some platforms you need to set the PYTHONPATH environment
variable to your working directory, where pywumpsim and PyAgent.py reside.

Next, you will make all your changes to the PyAgent.py file. You will see five
functions in the file: PyAgent_Constructor, PyAgent_Destructor,
PyAgent_Initialize, PyAgent_Process, and PyAgent_GameOver. These five functions
are called by their counterparts in the Agent.h and Agent.cc files.  You can
see how this is done in PyAgent.h and PyAgent.cc files, BUT DO NOT MODIFY THESE
FILES. The only file you should change is PyAgent.py. Note that the
PyAgent_Process function takes the five separate percepts, rather than a
Percept class instance, and the PyAgent_Process function should return one of
the six actions defined in the Action.py file.

Once you've finished your PyAgent.py file, simply run the 'pywumpsim' program
to test your agent. The PyAgent.py file and the 'pywumpsim' executable must be
in the same directory. The 'pywumpsim' program accepts all the options
described above for 'wumpsim'. And there is no need to recompile 'pywumpsim'
after changes to PyAgent.py; this file is reloaded everytime 'pywumpsim' is
executed.

Happy hunting!

History
-------
Version 2.9 (released 09/15/2017)
- Added Action.py and Orientation.py files, which are imported into PyAgent.py
Version 2.8 (released 09/06/2017)
- Added ability to implement agent in Python
Version 2.7 (released 10/27/2015)
- Changes Location::operator== to const
- Initialize uninitialized pointers to satisfy some compilers
Version 2.6 (released 7/22/2015)
- Changed order of Orientation enum to {R,U,L,D} to support rotation by
  adding one.
- Changed "FORWARD" to "GOFORWARD" to be better aligned with text.
Version 2.5 (released 9/13/2012)
- Change world generation and world input file error checking so that gold
  cannot be in location (1,1) to be consistent with Russell and Norvig text
- Prints 'x' in location of dead wumpus; stench perceived in in wumpus
  location and adjacent locations, whether dead or alive
Version 2.4 (released 11/2/2011)
- Added GameOver() method to Agent class, which is called after the end of
  each game with the game's final score
Version 2.3 (released 10/26/2011)
- Added Write method to WumpusWorld so you can write the current world to a
  file
- Removed leading underscore from WumpusWorld variables, since they are now
  public
Version 2.2 (released 10/4/2011)
- Reorganized header file inclusion to avoid errors using the WorldState
  class in the Agent.
Version 2.1 (released 9/20/2011)
- Fixed some #include issues to work with other platforms
- Changed random/srandom to rand/srand for Windows compatibility
- Added "-world <file>" option to allow playing a specific world
Version 2.0 (released 9/19/2011)
- Initial release
