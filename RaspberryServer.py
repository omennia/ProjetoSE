import flask
from flask_cors import CORS
import time
app = flask.Flask(__name__)
CORS(app)

N_TOWERS = 3
N_DISKS = 3
GAME_STATE = []

ILLEGAL_MOVE = "Illegal move"
ROBOT_FAILURE = "Robot failed to move"
OUT_OF_BOUNDS = "Out of bounds"
NO_DISKS = "No disks to move"
UNKOWN_ERROR = "Unknown error"
NO_ERROR = ""

GAME_WON = "GAME WON"
MOVE_SUCESSFULL = "Move successful"

@app.route('/AppMovePlayer', methods=['POST'])
def GetRequest():
    """
    Handles the GET request for moving a player. The request should include 'original' and 'destination' parameters
    representing the tower we're moving from and to, respectively.

    Returns:
        JSON: A JSON response indicating the result of the move. This could be a success message, an error message, or a game won message.
    """

    global GAME_STATE
    original = int(flask.request.json.get('original')) # Represents the tower we're moving from
    destination = int(flask.request.json.get('destination')) # Represents the tower we're moving to
    
    madeMove, error = TryMakeMove(original, destination)
    if madeMove:
        print(GAME_STATE)
    
        # TODO :This is temporary
        if(MoveWon()):
            return flask.jsonify(GAME_WON)    #
        return flask.jsonify(MOVE_SUCESSFULL) # in these ones, we can just simply send that no error has occured, but for now we're doing this to print something to the screen
                                              # later on, we will have the app printing this message to the screen
    return flask.jsonify(error)

@app.route("/GetGameState", methods=['POST'])
def GetGameState():
    """
    Handles the GET request for retrieving the current game state.

    Returns:
        JSON: A JSON response containing the current game state.
    """
    return flask.jsonify(GAME_STATE)

def MoveWon():
    """
    Checks if the game has been won.

    Returns:
        bool: True if the game has been won, False otherwise.
    """
    if len(GAME_STATE[-1]) == N_DISKS:
        return True
    return False

def TryMakeMove(original, destination):
    """
    Tries to make a move from the original tower to the destination tower.

    Args:
        original (int): The tower index we're moving from.
        destination (int): The tower index we're moving to.

    Returns:
        tuple: A tuple containing a boolean indicating if the move was successful and an error code.
    """
    legal, error = CheckLegality(original, destination)
    if not legal:
        return (False, error)
    
    UpdateGameState(original, destination)
    
    if MoveRobot():
        return (True, NO_ERROR)
    else:
        return (False, ROBOT_FAILURE)


def UpdateGameState(original, destination):
    """
    Updates the game state after a move.

    Args:
        original (int): The tower index we're moving from.
        destination (int): The tower index we're moving to.
    """

    print("Updating game state: " + str(original) + " -> " + str(destination))
    GAME_STATE[destination] = GAME_STATE[destination] + [GAME_STATE[original][-1]]
    GAME_STATE[original] = GAME_STATE[original][:-1]

def MoveRobot():
    """
    Simulates the movement of the robot.

    Returns:
        tuple: A tuple containing a boolean indicating if the move is legal and an error code.
    """

    print("Moving robot")
    # time.sleep(1) # this time is supposed to simulate the time it takes for the robot to move
    print("Robot moved")
    return (True, NO_ERROR);

def CheckLegality(original, destination):
    """
    Checks the legality of the player move.

    Args:
        original (int): The tower index we're moving from.
        destination (int): The tower index we're moving to.

    Returns:
        tuple: A tuple containing a boolean indicating if the move is legal and an error code.
    """
    if(original < 0 or original >= N_TOWERS or destination < 0 or destination >= N_TOWERS): #Sanity check
        return (False, OUT_OF_BOUNDS)
    
    if not GAME_STATE[original]: # No disks to move
        return (False, NO_DISKS)
    if not GAME_STATE[destination]: # If the destination tower is empty, move the disk
        return (True, NO_ERROR)
    if GAME_STATE[destination][-1] > GAME_STATE[original][-1]: # If the top disk of the destination tower is larger than the top disk of the original tower, move the disk
        return (True, NO_ERROR)
    
    return (False, ILLEGAL_MOVE) # If none of the above conditions are met (the top disk of the destination tower is smaller than the top disk of the original tower), the move is illegal

if __name__ == '__main__':
    GAME_STATE = [[]]*N_TOWERS
    GAME_STATE[0] = list(range(N_DISKS, 0, -1))
    print(GAME_STATE)
    app.run(host='0.0.0.0', port=8080)
