# Mugen Classes

# Exception handler
class MugenException(Exception):
    pass

# Command class
class Command():
    def __init__(self):
        self.state = None
        self.keys = []
        self.time = None
        self.bufferTime = None

# StateDef holds it's parameters declared in statedef and it's relative procedure states
class StateDef():
    def __init__(self, player, world):
        self.stateNumber = None
        self.player = player
        self.world = world
        self.stateType = None
        self.moveType = None
        self.physics = None
        self.anim = None
        self.velset = None
        self.ctrl = None
        self.poweradd = None
        self.juggle = None
        self.facep2 = None
        self.hitdefpersist = None
        self.movehitpersist = None
        self.hitcountpersist = None
        self.sprpriority = None
    
    def getNumber(self):
        return self.stateNumber
    
    def evaluate(self, world):
        pass

# TODO Controller classes
class Controller():
    def __init__(self):
        pass

# World class
class World():
    def __init__(self):
        self.currentTime = 0
        self.endTime = None
        self.currentRound = 1
        self.rounds = None
        self.player1 = None
        self.player2 = None
        self.player3 = None
        self.player4 = None
    
    def getTime(self):
        return self.currentTime
    
    # TODO
    def getCurrentRound(self):
        return self.currentRound
    def isMatchOver(self):
        return 0
    def getPlayerBodyXDistance(self):
        return 0
    def getPlayerBodyYDistance(self):
        return 0
    def getPlayerXDistance(self):
        return 0
    def getPlayerYDistance(self):
        return 0

# Character Class
class Character():
    def __init__(self):
        # From character definition file
        self.name = None
        self.displayName = None
        self.versionDate = []
        self.mugenVersion = []
        self.author = None
        self.paletteDefaults = []
        self.cmdFile = None
        self.stateFiles = []
        self.spriteFile = None
        self.animFile = None
        self.soundFile = None
        self.paletteFiles = {}
        self.introStoryboard = None
        self.endingStoryboard = None
        
        # Defaults (cmd file)
        self.commandTime = None;
        self.commandBufferTime = None;
        
        # Data (constants)
        self.life = None
        self.attack = None
        self.defence = None
        self.fallDefenceUp = None
        self.liedownTime = None
        self.airjuggle = None
        self.sparkno = None
        self.guardSparkno = None
        self.koEcho = None
        self.volume = None
        self.intPersistIndex = None
        self.floatPersistIndex = None

        # Size (constants)
        self.xscale = None
        self.yscale = None
        self.groundBack = None
        self.groundFront = None
        self.airBack = None
        self.airFront = None
        self.height = None
        self.attackDist = None
        self.projAttackDist = None
        self.projDoscale = None
        self.headPos = []
        self.midPos = []
        self.shadowoffset = None
        self.drawOffset = []

        # Velocity (constants)
        self.walkFwd = []
        self.walkBack = []
        self.runFwd = []
        self.runBack = []
        self.jumpNeu = []
        self.jumpBack = []
        self.jumpFwd = []
        self.runjumpBack = []
        self.runjumpFwd = []
        self.airjumpNeu = []
        self.airjumpBack = []
        self.airjumpFwd = []

        # Movement (constants)
        self.airjumpNum = None
        self.airjumpHeight = None
        self.yaccel = None
        self.standFriction = None
        self.crouchFriction = None
        
        # Command Holders
        self.commands = {}
        
        # Current Command
        self.currentCommand = None
        
        # State holders
        self.states = {}
        
        # Previous State number
        self.previousStateNumber = None
        
        # Current State
        self.currentState = None
        
        # States -3, -2, -1
        self.neg3State = None
        self.neg2State = None
        self.neg1State = None

    def setName(self, name):
        self.name = name
        
    def addStateFile(self, stateFile):
        for f in self.stateFiles:
            if f == stateFile:
                return
        self.stateFiles.append(stateFile)

    def addCommand(self, state, keys, time=None, bufferTime=None):
        cmd = Command()
        cmd.state = state
        cmd.keys = keys
        if time:
            cmd.time = time
        if bufferTime:
            cmd.bufferTime = bufferTime
        
        self.commands[state] = cmd
        
    def addState(self, number, state):
        self.states[number] = state
        
    def getState(self, number):
        try:
            return self.states[number]
        except KeyError:
            raise MugenException()
        
    # Change state to number and pass world
    def changeState(self, number, world):
        try:
            prev = self.currentState
            self.currentState = self.getState(number)(self, world)
            if prev:
                self.previousStateNumber = prev.getNumber()
            print 'Changed to state ' + str(number) + '' 
        except MugenException:
            # TODO remove later
            # self.addState(number, StateDef)
            # self.currentState = self.states[number](self, world)
            print 'State (' + str(number) + ') does not exist' 

    # List states
    def listStates(self):
        for key in self.states:
            print str(key) + ', ',
        print ''

    # Set command
    def setCurrentCommand(self, command):
        self.currentCommand = command
        
    # Evaluate a command
    def evaluateCommand(self):
        return self.currentCommand
    
    # Run through -3, -2, -1 and current states
    def act(self, world):
        # State -3, -2, -1
        self.neg3State.evaluate(world)
        self.neg2State.evaluate(world)
        self.neg1State.evaluate(world)
        
        # Current state
        self.currentState.evaluate(world)
    
    # Get previous state number
    def getPreviousStateNumber(self):
        return self.previousStateNumber
    
    # Get current state number
    def getStateNumber(self):
        return self.currentState.getNumber()
    
    # TODO
    def animationElementTime(self, time):
        return 1
    def canRecover(self):
        return 1
    def getAnimationTime(self):
        return 0
    def getBackEdgeBodyDistance(self):
        return 0
    def getBackEdgeDistance(self):
        return 0
    def getFrontEdgeBodyDistance(self):
        return 0
    def getFrontEdgeDistance(self):
        return 0
    def getHitFall(self):
        return 0
    def getHitOver(self):
        return 0
    def getHitShakeOver(self):
        return 0
    def getHitVariable(self, value):
        return 0
    def getMoveContact(self):
        return 0
    def getMoveGuarded(self):
        return 0
    def getMoveHit(self):
        return 0
    def getMoveType(self):
        return 0
    def getStateType(self):
        return 0
    def hasControl(self):
        return 0
    def roundsExisted(self):
        return 0
    def selfAnimExist(self, number):
        return 1
    def setAnimation(self, number, element):
        pass
    def currentAnimation(self):
        return 0
    def getPositionX(self):
        return 0
    def getPositionY(self):
        return 0
    def getSystemFloatVariable(self, number):
        return 0
    def getSystemVariable(self, number):
        return 0
    def getVariable(self, number):
        return 0
    def getVelocityX(self):
        return 0
    def getVelocityY(self):
        return 0
    def isAlive(self):
        return 1
    def turn(self):
        pass
    def setControl(self, trigger):
        pass

# LN definition
def ln(number):
    return math.log(number, math.e)
