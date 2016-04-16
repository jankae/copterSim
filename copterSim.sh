#!/bin/bash

Release/copterSim &
python simuGUI.py
killall copterSim
