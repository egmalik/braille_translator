
#define CHAR_ARRAY_SIZE 72
#define PIN_COUNT 6
#include <Servo.h>


byte byteRead;


//servos

Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_4;
Servo servo_5;
Servo servo_6;
Servo servos [6] ;

int potpin = 2;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

// angles

int angle_1 = 0;
int angle_2 = 90 ;
int angle_3 = 0;
int angle_4 = 45;
int angle_5 = 90;
int angle_6 = 0;
//int braillePins[] = { 2, 3, 4, 5, 6, 7 };
int starterAngles[] = {0, 90, 0, 45, 90, 90};
int finishaAngles[] = {19, 147, 22, 35, 215, 170};
int refreshSpeed = 250;

String inData;
String caps = "000001";
String number = "001111";
String space = "000000";
String allUpp = "asdfghj";

// Array of braille binary representations mapped to their equivalent letter
String hashTable[] = {"a", "100000",
                      "b", "110000",
                      "c", "100100",
                      "d", "100110",
                      "e", "100010",
                      "f", "110100",
                      "g", "110110",
                      "h", "110010",
                      "i", "010100",
                      "j", "010110",
                      "k", "101000",
                      "l", "111000",
                      "m", "101100",
                      "n", "101110",
                      "o", "101010",
                      "p", "111100",
                      "q", "111110",
                      "r", "111010",
                      "s", "011100",
                      "t", "011110",
                      "u", "101001",
                      "v", "111001",
                      "w", "010111",
                      "x", "101101",
                      "y", "101111",
                      "z", "101011",
                      ".", "010011",
                      ",", "010000",
                      "!", "011010",
                      ";", "011000",
                      ":", "100011",
                      "@", "000100",
                      "&amp;", "111101",
                      "+", "001101",
                      "-", "001001",
                      "=", "111111"
                     };

void setup() {
  Serial.begin(4800);

  servo_1.attach(2);
  servo_2.attach(3);
  servo_3.attach(9);
  servo_4.attach(5);
  servo_5.attach(6);
  servo_6.attach(7);

  servos[0] = servo_1;
  servos[1] = servo_2;
  servos[2] = servo_3;
  servos[3] = servo_4;
  servos[4] = servo_5;
  servos[5] = servo_6;

  for (int i = 0; i < PIN_COUNT; i++) {
    servos[i].write(starterAngles[i]);
  }
}

void loop() {
  inData = "";
  //Serial.flush();
  int h = Serial.available() - 1;
  if ( h > 0 ) {
    char temp;
    for (int i = 0; i < h; i++) {
      temp = (char)Serial.read();
      inData += temp;
    }
    if (inData.length() > 0) {
      convertStringToBraille(inData);
      sendBinaryToArduino(space);
      //  delay(refreshSpeed);
      //  offAll();
    }

  }

}

void offAll() {
  for (int i = 0; i < PIN_COUNT; i++) {
    servos[i].write(starterAngles[i]);

  }

}

void convertStringToBraille( String text ) {

  int stringLength = text.length();

  for ( int i = 0; i < stringLength; i++ ) {
    char currentChar = text.charAt(i);
    if (currentChar >= 'A' && currentChar <= 'Z') {
      Serial.println("CAPS");
      sendBinaryToArduino(caps);
      delay(refreshSpeed);
      String braille = charToBraille(currentChar + 32);
      sendBinaryToArduino(braille);
    } else if (currentChar >= '0' && currentChar <= '9') {
      Serial.print("NUMB: ");
      Serial.println(currentChar);
      sendBinaryToArduino(number);
      delay(refreshSpeed);
      String braille = charToBraille(isANumber(currentChar));
      sendBinaryToArduino(braille);
    } else if ( currentChar == ' ' ) {
      Serial.println("SPACE");
      sendBinaryToArduino(space);
    } else {
      Serial.println("CHAR");
      String braille = charToBraille(currentChar);
      sendBinaryToArduino(braille);
    }
    delay(refreshSpeed);
    Serial.flush();
  }

}

char isANumber( char c ) {
  if (c == '0') {
    return 'j';
  } else if (c == '1') {
    return 'a';
  } else if (c == '2') {
    return 'b';
  } else if (c == '3') {
    return 'c';
  } else if (c == '4') {
    return 'd';
  } else if (c == '5') {
    return 'e';
  } else if (c == '6') {
    return 'f';
  } else if (c == '7') {
    return 'g';
  } else if (c == '8') {
    return 'h';
  } else if (c == '9') {
    return 'i';
  }
}

void sendBinaryToArduino(String binary) {
  for ( int i = 0 ; i < binary.length(); i++ ) {
    char currentChar = binary.charAt(i);
    if (i == 0 || i == 1 || i == 2)  {
      if ( currentChar == '1') {
        servos[i].write(finishaAngles[i] - starterAngles[i]);
      } else if ( currentChar == '0') {
        servos[i].write(starterAngles[i]);
      }

    }
    delay(150);
    if (i == 3 || i == 4 || i == 5) {
      if ( currentChar == '1') {
        servos[i].write(finishaAngles[i] - starterAngles[i]);
      } else if ( currentChar == '0') {
        servos[i].write(starterAngles[i]);
      }
    }
    //delay(refreshSpeed);

    //delay(refreshSpeed);
  }
  offAll();
}

String charToBraille( char c ) {
  Serial.println(String(c));
  for (int i = 0; i < CHAR_ARRAY_SIZE; i += 2) {
    if (String(c) == hashTable[i]) {
      return hashTable[i + 1];
      break;
    }
  }

}
