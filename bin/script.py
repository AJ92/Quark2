from Vulkan0Script import *

class VScript(ScriptInstance):
    def __init__(self, script):
        ScriptInstance.__init__(self, script)

    def init(self):
        print "init"

    def update(self):
        print "update"
