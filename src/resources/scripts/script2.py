from Quark2 import *

class Script():
    def __init__(self, script):
        self.script = script

    def init(self):
        self.script.log("init2")

    def update(self, o):
        1+1#self.log("update2")
