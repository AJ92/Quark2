from Quark2 import *

class Script():
    def __init__(self, script):
        self.script = script

    def init(self):
        self.script.log("init")

    def update(self, o):
        self.primes(5)

    def primes(self, n):
        if n==2: return [2]
        elif n<2: return []
        s=range(3,n+1,2)
        mroot = n ** 0.5
        half=(n+1)/2-1
        i=0
        m=3
        while m <= mroot:
            if s[i]:
                j=(m*m-3)/2
                s[j]=0
                while j<half:
                    s[j]=0
                    j+=m
            i=i+1
            m=2*i+3
        return [2]+[x for x in s if x]