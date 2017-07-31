from Vulkan0Script import *

class VScript(ScriptInstance):
    def __init__(self, script):
        ScriptInstance.__init__(self, script)

    def init(self):
        print "init2"

    def update(self, o):
        print "update2"
        self.log("hi from 2")
