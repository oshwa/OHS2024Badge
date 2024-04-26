/*
  Open Hardware Summit 2024
  Designed by Cyber City Circuits
  Twitter @MakeAugusta

  This board uses Minicore with the Arduino IDE:
  https://github.com/MCUdude/MiniCore?tab=readme-ov-file#how-to-install

  Board: ATMEGA328
  Clock: Internal 8MHz
  Variant: 328PB
  Port: Comm Port used for your programmer
  Programmer: Arduino as ISP

  Pin LEDs:
  Anode 1: 23 - Head Ring Right
  Anode 2: 04 - Head Ring Top
  Anode 3: 03 - Head Ring Left
  Anode 4: 19 - Eye Right
  Anode 5: 18 - Body Right
  Anode 6: 17 - Body Center
  Anode 7: 16 - Body Left
  Anode 8: 15 - Eye Left

  Pin RGB for LED 1-4:
  Blue:  00
  Green: 01
  Red:   02

  Button: 26 (Active Low)

  SAO GPIO:
  SAO 1: 06
  SAO 2: 05

*/

// colors
int white[]  = {255, 255, 255};
int red[]    = {255,   0,   0};
int cyan[]   = {255,  65,   0};
int orange[] = {180,  20,  20};
int yellow[] = {255,  65,   0};
int purple[] = { 20,   0, 255};
int green[]  = {  0, 255,   0};
int blue[]   = {  0,   0, 255};
int pink[]   = { 20,   0, 255};

// Pin Assignments
int LED_Count = 8;
int Anodes[8] = {23, 4, 3, 19, 18, 17, 16, 15};
int Mode_Btn = 26;
int Blue = 0;   // PWM Only
int Red = 2;    // PWM Only
int Green = 1;  // PWM Only
int RandomSeedPin = 14;

int no_1[] = {Anodes[0], Anodes[1], Anodes[2]};  // Ring
int no_2[] = {Anodes[3], Anodes[7], Anodes[7]};  // Eyes
int no_3[] = {Anodes[4], Anodes[5], Anodes[6]};  // Body

// Set Colors as Class Objects

long my_timer;
long mydelay = 100;

int Mode = 4;
int Program = 10;
int digit = 0;

int CurrentColor[3] = {0, 0, 0};
int colorwheel = 0;
int strand = 0;
int fade = 0;

void setup() {
    pinMode(Mode_Btn, INPUT_PULLUP);
    pinMode(Blue, OUTPUT);
    pinMode(Red, OUTPUT);
    pinMode(Green, OUTPUT);
    digitalWrite(Blue, HIGH);
    digitalWrite(Red, HIGH);
    digitalWrite(Green, HIGH);
    for (int i = 0; i < LED_Count; i++) {
        pinMode(Anodes[i], OUTPUT);
        digitalWrite(Anodes[i], LOW);
    }

    randomSeed(analogRead(RandomSeedPin));

    // POST
    if (digitalRead(Mode_Btn) == LOW) {
        delay(100);
        POST();
    }

    BootUp();

    for (int x = 0; x < 2; x++) {
        CurrentColor[x] = green[x];
    }
}

void loop() {
    if (digitalRead(Mode_Btn) == LOW) {
        delay(200);
        reset();
        Mode++;
        if (Mode > 8) {
            Mode = 0;
        }

        switch (Mode) {
            case 0:  // Red Cycle
                Program = 0;
                CurrentColor[0] = 255;  // red
                CurrentColor[1] = 0;    // green
                CurrentColor[2] = 0;    // blue
                break;
            case 1:  // Green Cycle
                Program = 0;
                CurrentColor[0] = 0;
                CurrentColor[1] = 255;
                CurrentColor[2] = 0;
                break;
            case 2:
                Program = 0;  // Blue Cycle
                CurrentColor[0] = 0;
                CurrentColor[1] = 0;
                CurrentColor[2] = 255;
                break;
            case 3:
                Program = 0;  // Orange Cycle
                CurrentColor[0] = 180;
                CurrentColor[1] = 20;
                CurrentColor[2] = 20;
                break;
            case 4:
                Program = 10;  // Set Random Colors
                break;
            case 5:
                Program = 20;  // Fast Fireworks
                // CurrentColor[0] = random(250);
                // CurrentColor[1] = random(250);
                // CurrentColor[2] = random(250);
                break;
            case 6:
                Program = 30;  // Fireworks
                break;
            case 7:
                Program = 40;  // Twinkle
                break;
            case 8:
                Program = 50;  // Strand Test Color Fade
                break;
        }
    }

    switch (Program) {
        case 0:  // Set up timer for case 1
            mydelay = 100;
            my_timer = millis() + mydelay;
            Program = 1;
            break;
        case 1:  // Cycle through numbers on badge with random colors
            // set first set
            // setColor(random(255), random(255), random(255));
            setColor(CurrentColor[0], CurrentColor[1], CurrentColor[2]);
            if (digit == 0) {
                digitalWrite(no_1[0], HIGH);
                digitalWrite(no_1[1], HIGH);
                digitalWrite(no_1[2], HIGH);
                if (millis() > my_timer) {
                    my_timer = millis() + mydelay;
                    digitalWrite(no_1[0], LOW);
                    digitalWrite(no_1[1], LOW);
                    digitalWrite(no_1[2], LOW);
                    digit++;
                }
            }
            if (digit == 1) {
                digitalWrite(no_2[0], HIGH);
                digitalWrite(no_2[1], HIGH);
                digitalWrite(no_2[2], HIGH);
                if (millis() > my_timer) {
                    my_timer = millis() + mydelay;
                    digitalWrite(no_2[0], LOW);
                    digitalWrite(no_2[1], LOW);
                    digitalWrite(no_2[2], LOW);
                    digit++;
                }
            }
            if (digit == 2) {
                digitalWrite(no_3[0], HIGH);
                digitalWrite(no_3[1], HIGH);
                digitalWrite(no_3[2], HIGH);
                if (millis() > my_timer) {
                    my_timer = millis() + mydelay;
                    digitalWrite(no_3[0], LOW);
                    digitalWrite(no_3[1], LOW);
                    digitalWrite(no_3[2], LOW);
                    digit = 0;
                }
            }
            break;

        case 20:  // Set up timer for case 1
            fastfirework();
            break;
        case 30:
            firework();
            break;
        case 40:
            twinkle();
            break;
        case 50:
            strandtest();
            break;
        case 10:
            // set first set
            setColor(random(5, 250), random(5, 175), random(100, 250));
            digitalWrite(no_1[0], HIGH);
            digitalWrite(no_1[1], HIGH);
            digitalWrite(no_1[2], HIGH);
            delay(150);
            digitalWrite(no_1[0], LOW);
            digitalWrite(no_1[1], LOW);
            digitalWrite(no_1[2], LOW);

            setColor(random(5, 250), random(5, 175), random(100, 250));
            digitalWrite(no_2[0], HIGH);
            digitalWrite(no_2[1], HIGH);
            digitalWrite(no_2[2], HIGH);
            delay(150);
            digitalWrite(no_2[0], LOW);
            digitalWrite(no_2[1], LOW);
            digitalWrite(no_2[2], LOW);

            setColor(random(5, 250), random(5, 175), random(100, 250));
            digitalWrite(no_3[0], HIGH);
            digitalWrite(no_3[1], HIGH);
            digitalWrite(no_3[2], HIGH);
            delay(150);
            digitalWrite(no_3[0], LOW);
            digitalWrite(no_3[1], LOW);
            digitalWrite(no_3[2], LOW);

            break;
    }
}

void BootUp() {
    for (int i = 0; i < LED_Count; i++) {
        digitalWrite(Anodes[i], HIGH);
    }

    for (int x = 0; x < 250; x++) {
        setColor(x, x, x);
        delay(20);
    }

    delay(1000);

    setColor(0, 0, 0);
    for (int i = 0; i < LED_Count; i++) {
        digitalWrite(Anodes[i], LOW);
    }
}

void strandtest() {
    for (int i = 0; i < LED_Count; i++) {
        digitalWrite(Anodes[i], HIGH);
    }

    switch (strand) {
        case 0:
            setColor(colorwheel, 0, 0);  // fade red
            break;
        case 1:
            setColor(0, colorwheel, 0);  // fade green
            break;
        case 2:
            setColor(0, 0, colorwheel);  // fade blue
            break;
    }

    if (fade == 0) {
        colorwheel++;
    } else {
        colorwheel--;
    }

    if (colorwheel > 252) {
        fade = 1;
    }

    if (colorwheel < 1) {
        fade = 0;
        strand++;
    }

    if (strand > 2) {
        strand = 0;
    }

    delay(2);
}

// Sets Colors with Analog Outputs
void setColor(int red, int green, int blue) {
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
    analogWrite(Red, red);
    analogWrite(Green, green);
    analogWrite(Blue, blue);
}

// Power On Self-Test
void POST() {
    for (int i = 0; i < LED_Count / 2; i++) {
        digitalWrite(Anodes[i], HIGH);
        digitalWrite(Anodes[i + 4], HIGH);
        digitalWrite(Red, LOW);
        delay(100);
        digitalWrite(Red, HIGH);
        digitalWrite(Green, LOW);
        delay(100);
        digitalWrite(Green, HIGH);
        digitalWrite(Blue, LOW);
        delay(100);
        digitalWrite(Blue, HIGH);
        delay(100);
        digitalWrite(Anodes[i], LOW);
        digitalWrite(Anodes[i + 4], LOW);
    }
}

// Resets all anodes low/off
void reset() {
    for (int i = 0; i < LED_Count; i++) {
        digitalWrite(Anodes[i], LOW);
    }
    analogWrite(Red, 0);
    analogWrite(Green, 0);
    analogWrite(Blue, 0);
}

void firework() {
    int mydelay = random(5);
    setColor(0, 0, 0);
    int a = random(500);

    if (a == 1) {  // Firework an LED!
        int b = random(LED_Count);
        int led = random(6);
        int myColor = random(200, 255);

        digitalWrite(Anodes[b], HIGH);

        switch (led) {
            case 0:  // Turn Red On
                for (int x = 0; x < myColor; x++) {
                    setColor(x, 0, 0);
                    delay(mydelay);
                }
                delay(100);
                for (int x = myColor; x > 0; x--) {
                    setColor(x, 0, 0);
                    delay(mydelay);
                }
                break;
            case 1:  // Turn Green On
                for (int x = 0; x < myColor; x++) {
                    setColor(0, x, 0);
                    delay(mydelay);
                }
                delay(100);
                for (int x = myColor; x > 0; x--) {
                    setColor(0, x, 0);
                    delay(mydelay);
                }
                break;
            case 2:  // Turn Blue On
                for (int x = 0; x < myColor; x++) {
                    setColor(0, 0, x);
                    delay(mydelay);
                }
                delay(100);
                for (int x = myColor; x > 0; x--) {
                    setColor(0, 0, x);
                    delay(mydelay);
                }
                break;
            case 3:  // Turn Red and Green On
                for (int x = 0; x < myColor; x++) {
                    setColor(x, x, 0);
                    delay(mydelay);
                }
                delay(100);
                for (int x = myColor; x > 0; x--) {
                    setColor(x, x, 0);
                    delay(mydelay);
                }
                break;
            case 4:  // Turn Green and Blue On
                for (int x = 0; x < myColor; x++) {
                    setColor(0, x, x);
                    delay(mydelay);
                }
                delay(100);
                for (int x = myColor; x > 0; x--) {
                    setColor(0, x, x);
                    delay(mydelay);
                }
                break;
            case 5:  // Turn Red and Blue On
                for (int x = 0; x < myColor; x++) {
                    setColor(x, 0, x);
                    delay(mydelay);
                }
                delay(100);
                for (int x = myColor; x > 0; x--) {
                    setColor(x, 0, x);
                    delay(mydelay);
                }
                break;
        }
        delay(50);
        digitalWrite(Anodes[b], LOW);
    }
}

void fastfirework() {
    int mydelay = 1;  // random(5);
    setColor(0, 0, 0);
    int a = random(100);

    if (a == 1) {  // Firework an LED!
        int b = random(LED_Count);
        int c = random(LED_Count);
        int led = random(6);
        int myColor = random(200, 255);

        digitalWrite(Anodes[b], HIGH);
        digitalWrite(Anodes[c], HIGH);

        switch (led) {
            case 0:  // Turn Red On
                for (int x = 0; x < myColor; x++) {
                    setColor(x, 0, 0);
                    delay(mydelay);
                }
                delay(100);
                for (int x = myColor; x > 0; x--) {
                    setColor(x, 0, 0);
                    delay(mydelay);
                }
                break;
            case 1:  // Turn Green On
                for (int x = 0; x < myColor; x++) {
                    setColor(0, x, 0);
                    delay(mydelay);
                }
                delay(100);
                for (int x = myColor; x > 0; x--) {
                    setColor(0, x, 0);
                    delay(mydelay);
                }
                break;
            case 2:  // Turn Blue On
                for (int x = 0; x < myColor; x++) {
                    setColor(0, 0, x);
                    delay(mydelay);
                }
                delay(100);
                for (int x = myColor; x > 0; x--) {
                    setColor(0, 0, x);
                    delay(mydelay);
                }
                break;
            case 3:  // Turn Red and Green On
                for (int x = 0; x < myColor; x++) {
                    setColor(x, x, 0);
                    delay(mydelay);
                }
                delay(100);
                for (int x = myColor; x > 0; x--) {
                    setColor(x, x, 0);
                    delay(mydelay);
                }
                break;
            case 4:  // Turn Green and Blue On
                for (int x = 0; x < myColor; x++) {
                    setColor(0, x, x);
                    delay(mydelay);
                }
                delay(100);
                for (int x = myColor; x > 0; x--) {
                    setColor(0, x, x);
                    delay(mydelay);
                }
                break;
            case 5:  // Turn Red and Blue On
                for (int x = 0; x < myColor; x++) {
                    setColor(x, 0, x);
                    delay(mydelay);
                }
                delay(100);
                for (int x = myColor; x > 0; x--) {
                    setColor(x, 0, x);
                    delay(mydelay);
                }
                break;
        }
        delay(1);
        digitalWrite(Anodes[b], LOW);
        digitalWrite(Anodes[c], LOW);
    }
}

void twinkle() {
    setColor(0, 0, 0);
    int a = random(500);

    if (a == 1) {  // Firework an LED!
        int b = random(LED_Count);

        digitalWrite(Anodes[b], HIGH);

        int myred = 120;
        int mygreen = 50;
        int myblue = 255;

        setColor(myred, mygreen, myblue);
        for (int x = 0; x < 255; x++) {
            myred--;
            mygreen--;
            myblue--;

            if (myred < 1) {
                myred = 0;
            }
            if (mygreen < 1) {
                mygreen = 0;
            }
            if (myblue < 1) {
                myblue = 0;
            }
            setColor(myred, mygreen, myblue);
            delay(1);
        }

        digitalWrite(Anodes[b], LOW);
    }
}
