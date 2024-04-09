import flask
import time
app = flask.Flask(__name__)

N_TOWERS = 3
N_DISKS = 3
GAME_STATE = []

ILLEGAL_MOVE = "Illegal move"
ROBOT_FAILURE = "Robot failed to move"

@app.route('/')
def GetRequest():
    global GAME_STATE
    req = flask.request.args.get('req')
    print("Request: " + req)
    if req == "PLAYER_MOVE":
        original = flask.request.args.get('original', type=int) # Represents the tower we're moving from
        destination = flask.request.args.get('destination', type=int) # Represents the tower we're moving to

        if TryMakeMove(original, destination):
            print(GAME_STATE)
            if(MoveWon()):
                print("GAME WON")

    return 'Hello World'

@app.route("/GetGameState")
def GetGameState():
    global GAME_STATE
    return flask.jsonify(GAME_STATE)

def MoveWon():
    if len(GAME_STATE[-1]) == N_DISKS:
        return True
    return False

def TryMakeMove(original, destination):
    if not CheckLegality(original, destination):
        return (False, ILLEGAL_MOVE)
    
    UpdateGameState(original, destination)
    
    if MoveRobot():
        return True
    else:
        return (False, ROBOT_FAILURE)


def UpdateGameState(original, destination):
    print("Updating game state: " + str(original) + " -> " + str(destination))
    GAME_STATE[destination] = GAME_STATE[destination] + [GAME_STATE[original][-1]]
    GAME_STATE[original] = GAME_STATE[original][:-1]

def MoveRobot():
    print("Moving robot")
    print("Robot moved")
    return True;

def CheckLegality(original, destination):
    """
    Checks the legality of the player move.

    Args:
        original (int): The tower index we're moving from.
        destination (int): The tower index we're moving to.

    Returns:
        bool: True if the move is legal, False otherwise.
    """
    if(original < 0 or original >= N_TOWERS or destination < 0 or destination >= N_TOWERS): #Sanity check
        return False
    
    if not GAME_STATE[original]:
        return False
    if not GAME_STATE[destination]: # If the destination tower is empty, move the disk
        return True
    if GAME_STATE[destination][-1] > GAME_STATE[original][-1]: # If the top disk of the destination tower is larger than the top disk of the original tower, move the disk
        return True
    return False

if __name__ == '__main__':
    GAME_STATE = [[]]*N_TOWERS
    GAME_STATE[0] = list(range(N_DISKS, 0, -1))
    print(GAME_STATE)
    app.run(host='0.0.0.0', port=8080)
