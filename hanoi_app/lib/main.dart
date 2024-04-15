import 'dart:convert';

import 'package:flutter/material.dart';
import 'disk.dart'; // Assuming Disk widget is defined in disk.dart
import 'rod.dart'; // Assuming Rod widget is defined in rod.dart
import 'package:http/http.dart' as http;
// import 'dart:convert'; // for using jsonDecode

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
  List<List<int>> rods = [
    [3, 2, 1],
    [],
    []
  ];
  final Map<int, Color> diskColors = {
    1: Colors.red,
    2: Colors.green,
    3: Colors.blue
  };
  bool isGameActive = false;
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
      body: Column(
        children: [
          Expanded(
            child: Row(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: List.generate(3, (index) => buildRod(context, index)),
            ),
          ),
          SwitchListTile(
            title: Text(isGameActive ? "Game Active" : "Game Waiting"),
            value: isGameActive,
            onChanged: (bool value) {
              setState(() {
                isGameActive = value;
              });
            },
          ),
          if (!isGameActive) // Display waiting message if the game is not active
            Padding(
              padding: const EdgeInsets.all(8.0),
              child: Text(
                waitingMessage,
                style: const TextStyle(
                    fontSize: 20,
                    fontWeight: FontWeight.bold,
                    color: Colors.grey),
              ),
            ),
        ],
      ),
    );
  }

  Widget buildRod(BuildContext context, int rodIndex) {
    return DragTarget<int>(
      onWillAcceptWithDetails: (DragTargetDetails<int> details) {
        // here we implement when to accept a player move
        return isGameActive && rods[rodIndex].isEmpty
            ? true
            : rods[rodIndex].isEmpty ||
                rods[rodIndex].last >
                    details.data; // Only accept if the disk is smaller
        // return isGameActive; // Only accept if the game is active
      },
      onAcceptWithDetails: (DragTargetDetails<int> details) {
        if (!isGameActive) return; // Do nothing if the game is not active
        int sourceIndex = rods.indexWhere(
            (rod) => rod.contains(details.data) && rod.last == details.data);

        setState(() {
          rods[rodIndex].add(details.data);
          rods[sourceIndex].remove(details.data);
        });

        // Check if the player has won after the move
        if (rods[2].length == 3 &&
            rods[2][0] == 3 &&
            rods[2][1] == 2 &&
            rods[2][2] == 1) {
          _showWinningMessage();
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
    // setState(() {
    //   rods = [
    //     [3, 2, 1],
    //     [],
    //     []
    //   ];
    //   isGameActive = false; // Optionally reset the game to inactive state
    // });
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
}
