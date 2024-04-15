import 'dart:convert';
import 'dart:ffi';

import 'package:flutter/material.dart';
import 'disk.dart'; // Assuming Disk widget is defined in disk.dart
import 'rod.dart'; // Assuming Rod widget is defined in rod.dart
import 'package:http/http.dart' as http;
import 'package:pair/pair.dart'; // for using Pair
// import 'dart:convert'; // for using jsonDecode

import 'dart:developer';

const String IP = '192.168.221.57';
const String PORT = '8080';
const String HOST = 'http://$IP:$PORT';

void main() {
  runApp(const MaterialApp(
    home: TowersOfHanoi(),
  ));
}

class TowersOfHanoi extends StatefulWidget {
  const TowersOfHanoi({super.key});

  @override
  _TowersOfHanoiState createState() => _TowersOfHanoiState();
}

class _TowersOfHanoiState extends State<TowersOfHanoi> {
  List<List<int>> rods = [[], [], []];
  final Map<int, Color> diskColors = {
    1: Colors.red,
    2: Colors.green,
    3: Colors.blue,
    4: Colors.yellow,
    5: Colors.purple
  };
  bool isLoading = false; // New state to indicate loading status

  _TowersOfHanoiState() {
    _resetGame();
  }

  bool isGameActive = true;
  String waitingMessage = "Waiting for the robot to finish playing";
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Towers of Hanoi'),
        actions: <Widget>[
          IconButton(
            icon: const Icon(Icons.refresh),
            tooltip: 'Reset Game',
            onPressed: _resetGame, // Calls the reset game function
          ),
        ],
      ),
      body: Stack(
        children: [
          Column(
            children: [
              Expanded(
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                  children:
                      List.generate(3, (index) => buildRod(context, index)),
                ),
              ),
              // Additional UI components can go here if needed
            ],
          ),
          Padding(
            padding: const EdgeInsets.all(8.0),
            child: ElevatedButton(
              onPressed: _getNextMove,
              child: Text("Generate Next Move"),
            ),
          ),
          if (isLoading)
            Positioned(
              top: 100, // Adjust based on your actual layout
              left: 0,
              right: 0,
              child: Container(
                alignment: Alignment.center,
                child: const CircularProgressIndicator(),
              ),
            ),
        ],
      ),
    );
  }

  void updateState(int source, int sink, int disk) async {
    setState(() {
      rods[sink].add(disk);
      rods[source].remove(disk);
    });
    bool isGameWon = await checkIsGameWon();
    if (isGameWon) {
      _showWinningMessage();
    }
  }

  Widget buildRod(BuildContext context, int rodIndex) {
    return DragTarget<int>(
      onWillAcceptWithDetails: (DragTargetDetails<int> details) {
        // here we implement when to accept a player move
        return isGameActive;
      },
      onAcceptWithDetails: (DragTargetDetails<int> details) async {
        if (!isGameActive) return; // Do nothing if the game is not active
        int sourceIndex = rods.indexWhere(
            (rod) => rod.contains(details.data) && rod.last == details.data);

        // check with server:
        Pair<bool, String> res = await makeMove(sourceIndex, rodIndex);
        if (res.key == true) {
          // setState(() {
          //   rods[rodIndex].add(details.data);
          //   rods[sourceIndex].remove(details.data);
          // });
          // Check if the player has won after the move
          updateState(sourceIndex, rodIndex, details.data);

          return;
        } else {
          log("Error: ${res.value}");
        }
      },
      builder: (context, candidateData, rejectedData) {
        return Rod(
          children: rods[rodIndex].isEmpty
              ? [Container(height: 20)]
              : rods[rodIndex]
                  .map((disk) => buildDisk(context, disk, rodIndex))
                  .toList()
                  .reversed
                  .toList(),
        );
      },
    );
  }

  Widget buildDisk(BuildContext context, int disk, int rodIndex) {
    bool isTopDisk = rods[rodIndex].last == disk;
    Color baseColor = diskColors[disk]!;
    return isTopDisk
        ? Draggable<int>(
            data: disk,
            feedback: Material(
              type: MaterialType.transparency,
              child: Disk(size: disk, color: baseColor.withOpacity(1)),
            ),
            childWhenDragging: Opacity(
              opacity: 0.5,
              child: Disk(size: disk, color: baseColor),
            ),
            child: Disk(size: disk, color: baseColor.withOpacity(1)),
          )
        : Disk(size: disk, color: baseColor.withOpacity(0.5));
  }

  void _showWinningMessage() {
    showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: const Text("Congratulations!"),
          content:
              const Text("You have successfully solved the Towers of Hanoi."),
          actions: <Widget>[
            TextButton(
              onPressed: () {
                Navigator.of(context).pop(); // Close the dialog
                _resetGame(); // Optionally reset the game
              },
              child: const Text("OK"),
            ),
          ],
        );
      },
    );
  }

  void _resetGame() async {
    await resetGameState();
    List<dynamic> cur = await getGameState();
    cur.map((dynamic list) {
      // Ensure each element is a list and convert elements to integers
      return list.map<int>((item) => item as int).toList();
    }).toList();

    // print(cur);
    setState(() {
      rods = cur as List<List<int>>;
      // isGameActive = false; // Optionally reset the game to inactive state
    });
  }

  void _getNextMove() async {
    await getNextMove();

    await getGameState();
  }

  Future<void> getNextMove() async {
    try {
      final response = await http.post(
        Uri.parse('$HOST/NextMove'),
        headers: {'Content-Type': 'application/json'},
      );

      // Check if the response is successful
      if (response.statusCode == 200) {
        // If server returns OK response, parse the JSON
        List<dynamic> jsonData = jsonDecode(response.body);
        int source = jsonData[0];
        int sink = jsonData[1];
        int disk = jsonData[2];
        await makeMove(source, sink);
        updateState(source, sink, disk);

        return;
      } else {
        // If the server did not return a 200 OK response,
        // then throw an exception.
        throw Exception('Failed to load new game state');
      }
    } catch (e) {
      print('Error when sending state: $e');
      if (e is http.ClientException) {
        print('HTTP error: ${e.message}, URI: ${e.uri}');
      }
      throw Exception('Failed to send state: $e');
    }
  }

  Future<void> resetGameState() async {
    // URL of your endpoint

    try {
      final response = await http.post(
        Uri.parse('$HOST/ResetGameState'),
        headers: {'Content-Type': 'application/json'},
      );

      // Check if the response is successful
      if (response.statusCode == 200) {
        // If server returns OK response, parse the JSON
        return;
      } else {
        // If the server did not return a 200 OK response,
        // then throw an exception.
        throw Exception('Failed to load new game state');
      }
    } catch (e) {
      print('Error when sending state: $e');
      if (e is http.ClientException) {
        print('HTTP error: ${e.message}, URI: ${e.uri}');
      }
      throw Exception('Failed to send state: $e');
    }
  }

  Future<List<List<int>>> getGameState() async {
    // URL of your endpoint

    try {
      final response = await http.post(
        Uri.parse('$HOST/GetGameState'),
        headers: {'Content-Type': 'application/json'},
      );

      // Check if the response is successful
      if (response.statusCode == 200) {
        // If server returns OK response, parse the JSON

        List<dynamic> jsonData = jsonDecode(response.body);

        // Convert the List<dynamic> to a List<List<int>>
        List<List<int>> listOfIntLists =
            jsonData.map<List<int>>((dynamic list) {
          // Cast each element to a List<dynamic> and then map each element to int
          return (list as List<dynamic>)
              .map<int>((item) => item as int)
              .toList();
        }).toList();

        return listOfIntLists;
      } else {
        // If the server did not return a 200 OK response,
        // then throw an exception.
        throw Exception('Failed to load new game state');
      }
    } catch (e) {
      print('Error when sending state: $e');
      if (e is http.ClientException) {
        print('HTTP error: ${e.message}, URI: ${e.uri}');
      }
      throw Exception('Failed to send state: $e');
    }
  }

  Future<Pair<bool, String>> makeMove(int source, int sink) async {
    setState(() {
      isLoading = true; // Start loading
    });

    try {
      final response = await http.post(Uri.parse('$HOST/AppMovePlayer'),
          headers: {'Content-Type': 'application/json'},
          body: jsonEncode({
            'original': source.toString(),
            'destination': sink.toString(),
          }));

      if (response.statusCode == 200) {
        List<dynamic> jsonData = jsonDecode(response.body);
        return Pair(jsonData[0], jsonData[1]);
      } else {
        throw Exception('Failed to load new game state');
      }
    } catch (e) {
      print('Error when sending state: $e');
      if (e is http.ClientException) {
        print('HTTP error: ${e.message}, URI: ${e.uri}');
      }
      throw Exception('Failed to send state: $e');
    } finally {
      setState(() {
        isLoading = false; // Stop loading
      });
    }
  }

  Future<bool> checkIsGameWon() async {
    try {
      final response = await http.post(Uri.parse('$HOST/IsGameWon'),
          headers: {'Content-Type': 'application/json'});

      // Check if the response is successful
      if (response.statusCode == 200) {
        // If server returns OK response, parse the JSON

        bool jsonData = jsonDecode(response.body);
        return jsonData;
      } else {
        // If the server did not return a 200 OK response,
        // then throw an exception.
        throw Exception('Failed to load new game state');
      }
    } catch (e) {
      print('Error when sending state: $e');
      if (e is http.ClientException) {
        print('HTTP error: ${e.message}, URI: ${e.uri}');
      }
      throw Exception('Failed to send state: $e');
    }
  }
}
