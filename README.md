
## Overview

This project aims to create a hardware solution capable of solving the Towers of Hanoi puzzle automatically and interactively. It serves as a demonstration tool for high school students, showcasing the integration of mathematics and computer science through an embedded system. 


## Project Description

The project involves creating an interactive system that solves the Towers of Hanoi puzzle using a robotic arm, controlled by an Arduino and a Raspberry Pi. The system features a mobile application for user interaction and is designed to be portable and visually appealing for educational demonstrations.

  

## Requirements

### Functional Requirements

- Automatic solving of the Hanoi Towers puzzle.

- Determining the shortest solution and supporting step-by-step executions with possible user interaction.

- A mobile application as the user interface, displaying the game board, starting the solving process, registering moves, and ensuring legal moves.

  

### Non-Functional Requirements

- The system must include a board designed to support three rods and at least three disks, adhering to the traditional game rules.

- The design must prioritise portability, durability, and visual appeal.

- The architecture must support future enhancements, allowing for extensibility to add more disks or rods, increasing the puzzle’s complexity, or integrating new technologies.

  

### Alternative Requirements

- Using cubes instead of disks for easier manipulation by the robot.

- Fixing the starting position of the disks on the left-most rod, ordered by size, to simplify initial state detection.

- Considering an isomorphic game, such as the lamp game, to simplify the movements required by the robot.

  

## Materials Required

- **Arduino Braccio Robot Arm**

- **Raspberry Pi 4 Model B**

- **Tower of Hanoi Disks and Board** (potentially 3D printed)

- **Arduino Board (for Arduino Uno)**

- **Cables for Arduino and Connections**

- **LiDAR Sensor**

  

## System Architecture

  

### Actor Model

The project involves three main actors:

1. **User**: Interacts with the system via a mobile application.

2. **Puzzle Controller**: Handles computation and communication between components and the user.

3. **Robotic Arm**: Executes moves and communicates the game state to the Puzzle Controller.

  

### Software Architecture

- **User Interface**: Mobile app connecting to the Puzzle Controller via Wi-Fi.

- **Microprocessor (Raspberry Pi)**: Manages multiple concurrent instructions and handles game server tasks.

- **Microcontroller (Arduino)**: Controls the robotic arm and LiDAR sensor, managing the hardware and communicating with the microprocessor via UART.

  

## Flowcharts and State Machines

  

### Arduino Flowchart and State Machine

Detailed sequence of operations controlling the robotic arm's movements, including system power-up, command processing, and error handling mechanisms.

  

### Raspberry Pi Flowchart and State Machine

System initialization, user input processing, and communication with Arduino.

  

### User Flowchart and State Machine

Mobile application logic, including app initialization, connection setup, user options for puzzle-solving, and feedback mechanisms.

  

## Development Summary

  

### Goals Achieved

- Automated solving of the Towers of Hanoi puzzle.

- Fully functional mobile application for user interaction.

- Effective system integration with Raspberry Pi and Arduino communication.

- Robust design suitable for demonstration purposes.

  

### Goals Not Achieved / ToDo

- The system only solved the puzzle using three blocks instead of the full complexity with three rods due to time constraints.
- The LiDAR sensor was not yet integrated into the project.

  

### Challenges and Solutions

- **Physical Challenges**: Replaced a worn-out plastic piece and soldered a loose connection in one of the servos.

  

### Relaxation of Requirements

- Simplified puzzle using only three blocks due to time constraints.

  

## Installation and Usage

To set up and use the system, follow these steps:

  

1. **Hardware Setup**:

   - Assemble the Arduino Braccio Robot Arm.

   - Connect the Raspberry Pi and Arduino according to the system architecture.

   - Connect the LiDAR sensor and other necessary components.

  

2. **Software Installation**:

   - Clone the repository: `git clone https://github.com/omennia/ProjetoSE-HanoiTowers`

   - Install necessary dependencies on the Raspberry Pi.

   - Upload the Arduino code to the Arduino board.

  

3. **Running the System**:

   - Power up the Raspberry Pi and Arduino.

   - Start the mobile application and the server.

   - Use the mobile application to control the puzzle-solving process.

  

## Contributors

- Marco Gonçalves (202007512)

- Duarte Nóbrega (202005727)

- Hugo Cardante (202002762)


