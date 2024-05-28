#include <Braccio.h>
#include <Servo.h>

#define GRIPPER_OPEN 10
#define GRIPPER_CLOSED 73

#define BASE_A 45
#define BASE_B 90
#define BASE_C 135

#define L1_SHOULDER 30
#define L1_WRIST 150

#define L2_SHOULDER 45
#define L2_WRIST 135

#define SPEED 20
#define OFFSET 10

void MoveAndPickup(int base, int shoulder, int elbow, int wristVertical, int wristRotation);
void MoveAndDrop(int base, int shoulder, int elbow, int wristVertical, int wristRotation);
void Dance();
bool DecodeAndMove(String data);

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

int curBase, curShoulder, curElbow, curWristVertical, curWristRotation, curGripper;
// Braccio.ServoMovement(10,           BASE_B,  90, 0, 150,  0,   GRIPPER_OPEN);
//   delay(1000);

//   Braccio.ServoMovement(10,           BASE_A,  30, 0, 150,  0,   GRIPPER_OPEN);
//   delay(1000);
//   reset();

//   Braccio.ServoMovement(10,           BASE_C,  30, 0, 150,  0,   GRIPPER_OPEN);
//   delay(1000);

void setup() {
    // Initialization functions and set up the initial position for Braccio
    // All the servo motors will be positioned in the "safety" position:
    // Base (M1):90 degrees
    // Shoulder (M2): 45 degrees
    // Elbow (M3): 180 degrees
    // Wrist vertical (M4): 180 degrees
    // Wrist rotation (M5): 90 degrees
    // gripper (M6): 10 degrees

    // Base             - Allowed values from 0 to 180 degrees
    // Shoulder         - Allowed values from 15 to 165 degrees
    // Elbow            - Allowed values from 0 to 180 degrees
    // Wrist Vertical   - Allowed values from 0 to 180 degrees
    // Wrist Rotation   - Allowed values from 0 to 180 degrees
    // Gripper          - Allowed values from 10 to 73 degrees. 10: the toungue is open, 73: the gripper is closed.

    // pinMode(12, OUTPUT);  //   you need to set HIGH the pin 12
    // digitalWrite(12, HIGH);

    // Braccio.begin();  // and set a proper parameter to disable the soft start
    Serial.begin(9600);

    while (!Serial) {
        ;  // Wait for the serial port to connect
    }

    Braccio.begin();
    reset(0);
    // MoveAndPickup(BASE_A, L2_SHOULDER, 0, L2_WRIST, 0);
    // MoveAndDrop(BASE_C, L1_SHOULDER, 0, L1_WRIST, 0);
    // MoveAndPickup(BASE_A, L1_SHOULDER, 0, L1_WRIST, 0);
    // MoveAndDrop(BASE_C, L2_SHOULDER, 0, L2_WRIST, 0);

    // MoveAndPickup(BASE_C, L2_SHOULDER, 0, L2_WRIST, 0);
    // MoveAndDrop(BASE_A, L1_SHOULDER, 0, L1_WRIST, 0);
    // MoveAndPickup(BASE_C, L1_SHOULDER, 0, L1_WRIST, 0);
    // MoveAndDrop(BASE_A, L2_SHOULDER, 0, L2_WRIST, 0);
    // Dance();
}

void reset(int offset) {
    Braccio.ServoMovement(SPEED, BASE_B + offset, 90, 90, 90, 90, GRIPPER_CLOSED);
    curBase = BASE_B;
    curShoulder = 90;
    curElbow = 90;
    curWristVertical = 90;
    curWristRotation = 90;
    curGripper = GRIPPER_CLOSED;
    delay(500);
}

void MoveAndPickup(int base, int shoulder, int elbow, int wristVertical, int wristRotation) {
    Braccio.ServoMovement(SPEED, base, 90, 90, 90, 90, GRIPPER_CLOSED);
    delay(500);
    Braccio.ServoMovement(SPEED, base, shoulder, elbow, wristVertical, wristRotation, GRIPPER_OPEN);
    delay(500);
    Braccio.ServoMovement(SPEED, base, shoulder, elbow, wristVertical, wristRotation, GRIPPER_CLOSED);
    delay(500);
    int offset = 0;
    if(base == BASE_A)
      offset = OFFSET;
    else if(base == BASE_C)
      offset = -OFFSET;
    reset(offset);
}

void MoveAndDrop(int base, int shoulder, int elbow, int wristVertical, int wristRotation) {
    Braccio.ServoMovement(SPEED, base, 90, 90, 90, 90, GRIPPER_CLOSED);
    delay(500);
    Braccio.ServoMovement(SPEED, base, shoulder, elbow, wristVertical, wristRotation, GRIPPER_CLOSED);
    delay(500);
    Braccio.ServoMovement(SPEED, base, shoulder, elbow, wristVertical, wristRotation, GRIPPER_OPEN);
    int offset = 0;
    if(base == BASE_A)
      offset = OFFSET;
    else if(base == BASE_C)
      offset = -OFFSET;
    reset(offset);
    delay(500);
}

void Dance() {
    while (true) {
        Braccio.ServoMovement(SPEED, BASE_A, 90, 90, 90, 90, GRIPPER_OPEN);
        Braccio.ServoMovement(SPEED, BASE_B, 70, 70, 70, 70, GRIPPER_CLOSED);
        Braccio.ServoMovement(SPEED, BASE_C, 90, 90, 90, 90, GRIPPER_OPEN);
        Braccio.ServoMovement(SPEED, BASE_B, 70, 70, 70, 70, GRIPPER_CLOSED);
    }
}

void loop() {
    /*
    Step Delay: a milliseconds delay between the movement of each servo.  Allowed values from 10 to 30 msec.
    M1=base degrees. Allowed values from 0 to 180 degrees
    M2=shoulder degrees. Allowed values from 15 to 165 degrees
    M3=elbow degrees. Allowed values from 0 to 180 degrees
    M4=wrist vertical degrees. Allowed values from 0 to 180 degrees
    M5=wrist rotation degrees. Allowed values from 0 to 180 degrees
    M6=gripper degrees. Allowed values from 10 to 73 degrees. 10: the toungue is open, 73: the gripper is closed.
    */
    // Return to the start position.
    // Braccio.ServoMovement(20,         0,   90, 180,  180,  90, 73);

    // //Open the gripper
    // Braccio.ServoMovement(20,         0,   90, 180,  180,  90, 10 );

    if (Serial.available() > 0) {
        String data = Serial.readString();  // Read the incoming data
        DecodeAndMove(data);                // Data is in the formate of "MOVE ORIGINAL_BASE ORIGINAL_SHOULDER ORIGINAL_WRIST DESTINATION_BASE DESTINATION_SHOULDER DESTINATION_WRIST"
        Serial.println("ACK");
    }
}

void splitString(String message, char delimiter, String& first, String& rest) {
    int delimiterIndex = message.indexOf(delimiter);  // get the index of the first occurrence of delimiter
    first = message.substring(0, delimiterIndex);     // get the part of the string before the delimiter
    rest = message.substring(delimiterIndex + 1);     // get the part of the string after the delimiter
}

bool DecodeAndMove(String data) {
    String move, originalBaseStr, originalShoulderStr, originalWristStr, destinationBaseStr, destinationShoulderStr, destinationWristStr;
    splitString(data, ' ', move, data);
    splitString(data, ' ', originalBaseStr, data);
    splitString(data, ' ', originalShoulderStr, data);
    splitString(data, ' ', originalWristStr, data);
    splitString(data, ' ', destinationBaseStr, data);
    splitString(data, ' ', destinationShoulderStr, data);
    splitString(data, ' ', destinationWristStr, data);

    int originalBase = originalBaseStr.toInt();
    int originalShoulder = originalShoulderStr.toInt();
    int originalWrist = originalWristStr.toInt();
    int destinationBase = destinationBaseStr.toInt();
    int destinationShoulder = destinationShoulderStr.toInt();
    int destinationWrist = destinationWristStr.toInt();

    MoveAndPickup(originalBase, originalShoulder, 0, originalWrist, 0);
    MoveAndDrop(destinationBase, destinationShoulder, 0, destinationWrist, 0);
}