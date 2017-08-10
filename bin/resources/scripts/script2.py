from Vulkan0Script import *

class VScript(ScriptInstance):
    def __init__(self, script):
        ScriptInstance.__init__(self, script)

    def init(self):
        self.log("init2")

    def update(self, o):
        1+1#self.log("update2")
