// Include the library InverseK.h

#include <Braccio.h>
#include <Servo.h>
#include <InverseK.h>

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

  #define L1 50
  #define L2 80
  #define L3 120

  #define TOWER_A 350
  #define TOWER_B 0
  #define TOWER_C -350

  #define X 550

void setup() {
    // Setup the lengths and rotation limits for each link
    Link base, shoulder, elbow, hand;

    Serial.begin(9600);

    base.init(0, b2a(0.0), b2a(180.0));
    shoulder.init(200, b2a(15.0), b2a(165.0));
    elbow.init(200, b2a(0.0), b2a(180.0));
    hand.init(270, b2a(0.0), b2a(180.0));

    // Attach the links to the inverse kinematic model
    InverseK.attach(base, shoulder, elbow, hand);

    float a0, a1, a2, a3;

    // InverseK.solve() return true if it could find a solution and false if not.

    // Calculates the angles without considering a specific approach angle
    // InverseK.solve(x, y, z, a0, a1, a2, a3)
    // if (InverseK.solve(550, 0, 50, a0, a1, a2, a3)) {
    //     Serial.print(a2b(a0));
    //     Serial.print(',');
    //     Serial.print(a2b(a1));
    //     Serial.print(',');
    //     Serial.print(a2b(a2));
    //     Serial.print(',');
    //     Serial.println(a2b(a3));
    // } else {
    //     Serial.println("No solution found");
    // }

    // // Calculates the angles considering a specific approach angle
    // // InverseK.solve(x, y, z, a0, a1, a2, a3, phi)
    // if (InverseK.solve(550, 0, 50, a0, a1, a2, a3, b2a(90.0))) {
    //     Serial.print(a2b(a0));
    //     Serial.print(',');
    //     Serial.print(a2b(a1));
    //     Serial.print(',');
    //     Serial.print(a2b(a2));
    //     Serial.print(',');
    //     Serial.println(a2b(a3));
        
    // } else {
    //     Serial.println("No solution found!");
    // }

    Braccio.begin();

    Serial.println("MOving to B");


    if (InverseK.solve(550, 0, 50, a0, a1, a2, a3, b2a(90.0))) {
        // Serial.print(a2b(a0));
        // Serial.print(',');
        // Serial.print(a2b(a1));
        // Serial.print(',');
        // Serial.print(a2b(a2));
        // Serial.print(',');
        // Serial.println(a2b(a3));
        Braccio.ServoMovement(10, a2b(a0), a2b(a1), a2b(a2), a2b(a3), 0, 10);
    } else {
        Serial.println("No solution found!");
    }

    // delay(3000);

    // Serial.println("MOving to A");

    // if (InverseK.solve(X, TOWER_A, L1, a0, a1, a2, a3)) {
    //     // Serial.print(a2b(a0));
    //     // Serial.print(',');
    //     // Serial.print(a2b(a1));
    //     // Serial.print(',');
    //     // Serial.print(a2b(a2));
    //     // Serial.print(',');
    //     // Serial.println(a2b(a3));
    //     Braccio.ServoMovement(10, a2b(a0), a2b(a1), a2b(a2), a2b(a3), 0, 10);
    // } else {
    //     Serial.println("No solution found!");
    // }

    // delay(3000);

    // Serial.println("MOving to C");


    // if (InverseK.solve(X, TOWER_C, L1, a0, a1, a2, a3)) {
    //     // Serial.print(a2b(a0));
    //     // Serial.print(',');
    //     // Serial.print(a2b(a1));
    //     // Serial.print(',');
    //     // Serial.print(a2b(a2));
    //     // Serial.print(',');
    //     // Serial.println(a2b(a3));
    //     Braccio.ServoMovement(10, a2b(a0), a2b(a1), a2b(a2), a2b(a3), 0, 10);
    // } else {
    //     Serial.println("No solution found!");
    // }

    // Braccio.ServoMovement(SPEED, BASE_A, 90, 90, 90, 90, GRIPPER_OPEN);
}

void loop() {
}

// Quick conversion from the Braccio angle system to radians
float b2a(float b) {
    return b / 180.0 * PI - HALF_PI;
}

// Quick conversion from radians to the Braccio angle system
float a2b(float a) {
    return (a + HALF_PI) * 180 / PI;
}