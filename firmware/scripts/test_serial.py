import serial

cmds = [
    {"name": "*IDN",
     "range": b"dispstepper-0.1.0"},
    {"name": "SETup:MODE",
     "range": [0, 2],
     "type": "i"},
    {"name": "SETup:SINGle:INDex",
     "range": [0, 3],
     "type": "i"},
    {"name": "SETup:SINGle:VALue#",
     "range": [0, 1.0],
     "index": [0, 3],
     "type": "f"},
    {"name": "SETup:REPeat:INDex",
     "range": [0, 3],
     "type": "i"},
    {"name": "SETup:REPeat:VALue#",
     "range": [0, 1.0],
     "index": [0, 3],
     "type": "f"},
    {"name": "SETup:MANual:INDex",
     "range": [0, 3],
     "type": "i"},
    {"name": "SETup:MANual:VALue#",
     "range": [0, 4096],
     "index": [0, 3],
     "type": "f"},
]

class CheckCmd:

    def __init__(self, cmd, serial):
        self._cmd = cmd
        self._lineans = ""
        self._linecmd = ""
        try:
            self._type = self._cmd["type"]
            self._isRW=True
        except KeyError:
            self._isRW = False
        self._serial = serial

    def isIndexed(self):
        return self._cmd["name"].endswith("#")

    def checkRange(self):
        # check range
        try:
            _f = float(self._lineans)
            if not (_f >= self._cmd["range"][0] and _f <= self._cmd["range"][1]):
                print("error: ", _f)
        except ValueError:
            if not (self._lineans in self._cmd["range"]):
                print("error: ", self._lineans,
                      " excpect ", self._cmd["range"])
    
    def setIndexed(self, index, value):
        self._serial.write(
            bytes((self._cmd["name"].replace("#", index))+" " + value + "\r\n", encoding="utf8"))

    def setValue(self, value):
       self._serial.write(
           bytes(self._cmd["name"]+" " + value + "\r\n", encoding="utf8"))

    def getIndexed(self, index):
        self._serial.write(
            bytes((self._cmd["name"].replace("#", index))+"?\r\n", encoding="utf8"))

    def getValue(self):
        self._serial.write(
            bytes(self._cmd["name"]+"?\r\n", encoding="utf8"))

    def read(self):
        # read value
        if self.isIndexed():
            self.getIndexed("0")
        else:
            self.getValue()
        
        self._linecmd = self._serial.readline().rstrip()   # read a '\n' terminated line
        self._lineans = self._serial.readline().rstrip()
        print("comand {}, result {}".format(self._linecmd, self._lineans))
    
    def checkans(self):
        self._linecmd = self._serial.readline().rstrip()   # read a '\n' terminated line
        self._lineans = self._serial.readline().rstrip()
        print("checkans {}, result {}".format(self._linecmd, self._lineans))

    def check(self):
        self.read()
        self.checkRange()
        if self.isIndexed():
            self.setIndexed("0", str(self._cmd["range"][0]))
        else:
            self.setValue(str(self._cmd["range"][0]))
        self.checkans()
        self.read()
        print("-----")



with serial.Serial('/dev/ttyACM0', 2000000, timeout=1) as ser:
    for c in cmds:
        cmd = CheckCmd(c, ser)
        cmd.check()
