#!/opt/hfs17.5/bin/hython


import json
import sys
import os
import subprocess


cwd = os.path.dirname(os.path.realpath(__file__))
scene_path = os.path.join(cwd, '../hip/dop_wrangle.hipnc')
hou.hipFile.load(scene_path)

#print hou.parm('/obj/any_test/cpp_wrangle2/code').evalAsString()
#hou.setFrame(1)
node = hou.node('/obj/any_test/cpp_wrangle2/')
geo  = node.geometry()

#print geo.points()
