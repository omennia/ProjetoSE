#include <Braccio.h>
#include <Servo.h>

#define GRIPPER_OPEN 10
#define GRIPPER_CLOSED 73

#define BASE_A 135
#define BASE_B 90
#define BASE_C 45

#define L1_SHOULDER 30
#define L1_WRIST 150

#define L2_SHOULDER 45
#define L2_WRIST 135

#define L3_SHOULDER 60
#define L3_WRIST 120

#define SPEED 20
#define OFFSET 10
#define SHOULDER_OFFSET 7

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

void setup() {
    Serial.begin(9600);
    while (!Serial) {
        ;  // Wait for the serial port to connect
    }

    Braccio.begin();
    reset(0);
}

void reset(int offset) {
    Braccio.ServoMovement(SPEED, 180, 90 + SHOULDER_OFFSET, 90, 90, 90, GRIPPER_CLOSED);
    curBase = BASE_B;
    curShoulder = 90 + SHOULDER_OFFSET;
    curElbow = 90;
    curWristVertical = 90;
    curWristRotation = 90;
    curGripper = GRIPPER_CLOSED;
    delay(500);
}

bool fromAtoB = false;
void MoveAndPickup(int base, int shoulder, int elbow, int wristVertical, int wristRotation) {
    Braccio.ServoMovement(SPEED, base, 90 + SHOULDER_OFFSET, 90, 90, 90, GRIPPER_CLOSED);
    delay(500);
    Braccio.ServoMovement(SPEED, base, shoulder + SHOULDER_OFFSET, elbow, wristVertical, wristRotation, GRIPPER_OPEN);
    delay(500);
    Braccio.ServoMovement(SPEED, base, shoulder + SHOULDER_OFFSET, elbow, wristVertical, wristRotation, GRIPPER_CLOSED);
    delay(500);
    int offset = 0;
    if (base == BASE_A) {
        offset = OFFSET;
    } else if (base == BASE_C)
        offset = -OFFSET;

    reset(offset);
}

void MoveAndDrop(int base, int shoulder, int elbow, int wristVertical, int wristRotation) {
    Braccio.ServoMovement(SPEED, base, 90 + SHOULDER_OFFSET, 90, 90, 90, GRIPPER_CLOSED);
    delay(500);
    Braccio.ServoMovement(SPEED, base, shoulder + SHOULDER_OFFSET, elbow, wristVertical, wristRotation, GRIPPER_CLOSED);
    delay(500);
    Braccio.ServoMovement(SPEED, base, shoulder + SHOULDER_OFFSET, elbow, wristVertical, wristRotation, GRIPPER_OPEN);
    int offset = 0;
    if (base == BASE_A)
        offset = OFFSET;
    else if (base == BASE_C)
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