import flask
from flask_cors import CORS
import time
from collections import deque

app = flask.Flask(__name__)
CORS(app)

N_TOWERS = 3
N_DISKS = 2
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
    
    (madeMove, error) = TryMakeMove(original, destination)
    if madeMove:
        # TODO :This is temporary
        if(MoveWon()):
            return flask.jsonify((madeMove, GAME_WON))    #
        return flask.jsonify((madeMove, MOVE_SUCESSFULL)) # in these ones, we can just simply send that no error has occured, but for now we're doing this to print something to the screen
                                              # later on, we will have the app printing this message to the screen
    return flask.jsonify((madeMove, error))

@app.route("/IsGameWon", methods=['POST'])
def IsGameWon():
    """
    Handles the GET request for checking if the game has been won.

    Returns:
        JSON: A JSON response indicating if the game has been won.
    """
    return flask.jsonify(MoveWon())

@app.route("/GetGameState", methods=['POST'])
def GetGameState():
    """
    Handles the GET request for retrieving the current game state.

    Returns:
        JSON: A JSON response containing the current game state.
    """
    return flask.jsonify(GAME_STATE)

@app.route("/GetGameInfo", methods=['POST'])
def GetGameInfo():
    """
    Handles the GET request for retrieving the game information.
    Used when a player joins the game

    Returns:
        JSON: A JSON response containing the number of towers, disks in the game and the current game state.
    """
    return flask.jsonify((N_TOWERS, N_DISKS, GAME_STATE))

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
    time.sleep(1) # this time is supposed to simulate the time it takes for the robot to move
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


@app.route('/ResetGameState', methods=['POST'])
def ResetGame():
    """
    Resets the game state.
    """
    global GAME_STATE
    GAME_STATE = [[]]*N_TOWERS
    GAME_STATE[0] = list(range(N_DISKS, 0, -1))
    return flask.jsonify(success=True)

@app.route('/NextMove', methods=['POST'])
def NextMove():
    (source, destination) = hanoi_next_move()
    return flask.jsonify((source, destination, GAME_STATE[source][-1]))


# sim duarte isto vem straight to chatgpt que eu sou preguiçosos e nao quero estar a implementar este trash
# este codigo tb e merda anyway, vamos te4r de fazer melhgor eventualmente
def hanoi_next_move():
    # Define the target configuration (e.g., all disks in the last tower)
    target = tuple(tuple() for _ in range(N_TOWERS - 1)) + (tuple(range(N_DISKS, 0, -1)),)

    # Helper function to generate valid moves from current state
    def generate_moves(state):
        moves = []
        for i, src in enumerate(state):
            if src:  # There is something to move
                for j in range(N_TOWERS):
                    if i != j:  # Can't move within the same tower
                        if not state[j] or state[j][-1] > src[-1]:  # Valid move
                            new_state = list(map(list, state))
                            disk = new_state[i].pop()
                            new_state[j].append(disk)
                            moves.append((tuple(map(tuple, new_state)), (i, j)))
        return moves

    # BFS to find the optimal move sequence
    queue = deque([(tuple(map(tuple, GAME_STATE)), [])])
    visited = set()
    visited.add(tuple(map(tuple, GAME_STATE)))

    while queue:
        current_state, path = queue.popleft()

        if current_state == target:
            if path:
                return path[0]  # Return the first move that leads to the solution
            break

        for next_state, move in generate_moves(current_state):
            if next_state not in visited:
                visited.add(next_state)
                queue.append((next_state, path + [move]))

    return None  # In case there is no move needed (already solved)




if __name__ == '__main__':
    GAME_STATE = [[] for _ in range(N_TOWERS)]
    GAME_STATE[0] = list(range(N_DISKS, 0, -1))
    print(GAME_STATE)
    app.run(host='0.0.0.0', port=8080, debug=True)
