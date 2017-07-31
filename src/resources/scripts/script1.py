from Vulkan0Script import *

class VScript(ScriptInstance):
    def __init__(self, script):
        ScriptInstance.__init__(self, script)

    def init(self):
        print "init1"

    def update(self, o):
        print "update1"
        #self.log("hi from 1")
