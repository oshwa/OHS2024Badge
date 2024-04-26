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
  Anode 3: 03 - Head Ring L eft
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

// Pin Definitions
// ===============

#define MODE_BUTTON_PIN 26

#define LED_PIN_HEAD_RIGHT 23
#define LED_PIN_HEAD_TOP 4
#define LED_PIN_HEAD_LEFT 3

#define LED_PIN_EYE_RIGHT 19
#define LED_PIN_EYE_LEFT 15

#define LED_PIN_BODY_RIGHT 18
#define LED_PIN_BODY_CENTER 17
#define LED_PIN_BODY_LEFT 16

#define COLOR_PIN_RED 2
#define COLOR_PIN_GREEN 1
#define COLOR_PIN_BLUE 0

#define SAO_PIN_1 6
#define SAO_PIN_2 5

// LED Animation settings
//   - Continuous transitions between low and high brightness
//   - Pressing the button cycles between modes:
//     - Mode 0: Green eyes
//     - Mode 1: Blueish-white body
//     - Mode 2: Red halo above head
// ==================================================

// Used to ensure button is triggered only once when pressed
#define DEBOUNCE_MAX_STEPS 2

// Cycle through this many modes with a button press
#define NUM_MODES 3

// This integer changes how fast the LEDs change brightness
// slow: 1, fast: 40
#define ANIMATION_SPEED 2
#define ANIMATION_MIN_BRIGHTNESS 10
#define ANIMATION_MAX_BRIGHTNESS 180

// polling timer at 50 milliseconds (20 times per second)
#define TIMER_STEP_MS 50
uint32_t timer_prev = 0;

void setup() {
    // Setup pins
    ConfigurePins();

    // Start with eyes on
    TurnOffBodyLEDs();
    TurnOffHeadLEDs();
    TurnOnEyeLEDs();

    // Start with green
    SetColor(0, 200, 50);

    // timer begin
    timer_prev = millis();
}

void loop() {
    // run timer
    uint32_t now = millis();
    uint32_t elapsed = now - timer_prev;
    if (elapsed >= TIMER_STEP_MS) {
        // advance timer
        timer_prev = timer_prev + TIMER_STEP_MS;

        // get button input
        bool button_pressed = ButtonDebounce(digitalRead(MODE_BUTTON_PIN));

        // Return mode value 0, 1, or 2, cycled by button press
        // LEDs are turned on and off depending on which mode is activated
        int mode = HandleButtonPress(button_pressed);

        // Animate color based on current mode
        AnimateFadeSequence(mode);
    }
}

void ConfigurePins() {
    // PWM LED cathodes to HIGH (no color)
    pinMode(COLOR_PIN_RED, OUTPUT);
    pinMode(COLOR_PIN_GREEN, OUTPUT);
    pinMode(COLOR_PIN_BLUE, OUTPUT);
    digitalWrite(COLOR_PIN_RED, HIGH);
    digitalWrite(COLOR_PIN_GREEN, HIGH);
    digitalWrite(COLOR_PIN_BLUE, HIGH);

    // LED Anodes to LOW
    pinMode(LED_PIN_HEAD_RIGHT, OUTPUT);
    pinMode(LED_PIN_HEAD_TOP, OUTPUT);
    pinMode(LED_PIN_HEAD_LEFT, OUTPUT);
    digitalWrite(LED_PIN_HEAD_RIGHT, LOW);
    digitalWrite(LED_PIN_HEAD_TOP, LOW);
    digitalWrite(LED_PIN_HEAD_LEFT, LOW);

    pinMode(LED_PIN_EYE_RIGHT, OUTPUT);
    pinMode(LED_PIN_EYE_LEFT, OUTPUT);
    digitalWrite(LED_PIN_EYE_RIGHT, LOW);
    digitalWrite(LED_PIN_EYE_LEFT, LOW);

    pinMode(LED_PIN_BODY_RIGHT, OUTPUT);
    pinMode(LED_PIN_BODY_CENTER, OUTPUT);
    pinMode(LED_PIN_BODY_LEFT, OUTPUT);
    digitalWrite(LED_PIN_BODY_RIGHT, LOW);
    digitalWrite(LED_PIN_BODY_CENTER, LOW);
    digitalWrite(LED_PIN_BODY_LEFT, LOW);

    // Button mode
    pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
}

void AnimateFadeSequence(int mode) {
    // animation state
    static int brightness = 255;
    static int anim_state = -1;

    // set color based on mode
    int color_red = 0;
    int color_green = 0;
    int color_blue = 0;
    switch (mode) {
        case 0:
            // Eye color
            color_red = 0;
            color_green = 255;
            color_blue = 0;
            break;

        case 1:
            // Body color
            color_red = 0;
            color_green = 0;
            color_blue = 255;
            break;

        case 2:
            // Head color
            color_red = 255;
            color_green = 0;
            color_blue = 0;
            break;

        default:
            break;
    }

    // update brightness
    brightness += anim_state * ANIMATION_SPEED;
    // reverse direction
    if (brightness > ANIMATION_MAX_BRIGHTNESS) {
        anim_state = -1;
        brightness = ANIMATION_MAX_BRIGHTNESS;
    } else if (brightness < ANIMATION_MIN_BRIGHTNESS) {
        anim_state = 1;
        brightness = ANIMATION_MIN_BRIGHTNESS;
    }

    // set color with brightness
    SetColorBrightness(color_red, color_green, color_blue, brightness);
}

int HandleButtonPress(bool button_pressed) {
    static int current_mode = 0;

    if (button_pressed) {
        // button pressed: go to next mode
        current_mode++;
        if (current_mode >= NUM_MODES) {
            current_mode = 0;
        }
        // transition state
        switch (current_mode) {
            case 0:
                // Transition to Eyes
                TurnOffBodyLEDs();
                TurnOffHeadLEDs();
                TurnOnEyeLEDs();
                break;

            case 1:
                // Transition to Body
                TurnOffEyeLEDs();
                TurnOffHeadLEDs();
                TurnOnBodyLEDs();
                break;

            case 2:
                // Transition to Head
                TurnOffBodyLEDs();
                TurnOffEyeLEDs();
                TurnOnHeadLEDs();
                break;

            default:
                break;
        }
    }

    return current_mode;
}

void SetColorBrightness(int32_t red, int32_t green, int32_t blue, int32_t brightness) {
    red = 255 - (constrain(red, 0, 255) * constrain(brightness, 0, 255)) / 255;
    green = 255 - (constrain(green, 0, 255) * constrain(brightness, 0, 255)) / 255;
    blue = 255 - (constrain(blue, 0, 255) * constrain(brightness, 0, 255)) / 255;
    analogWrite(COLOR_PIN_RED, red);
    analogWrite(COLOR_PIN_GREEN, green);
    analogWrite(COLOR_PIN_BLUE, blue);
}

void SetColor(int red, int green, int blue) {
    red = 255 - constrain(red, 0, 255);
    green = 255 - constrain(green, 0, 255);
    blue = 255 - constrain(blue, 0, 255);
    analogWrite(COLOR_PIN_RED, red);
    analogWrite(COLOR_PIN_GREEN, green);
    analogWrite(COLOR_PIN_BLUE, blue);
}

void TurnOnHeadLEDs() {
    digitalWrite(LED_PIN_HEAD_RIGHT, HIGH);
    digitalWrite(LED_PIN_HEAD_TOP, HIGH);
    digitalWrite(LED_PIN_HEAD_LEFT, HIGH);
}

void TurnOffHeadLEDs() {
    digitalWrite(LED_PIN_HEAD_RIGHT, LOW);
    digitalWrite(LED_PIN_HEAD_TOP, LOW);
    digitalWrite(LED_PIN_HEAD_LEFT, LOW);
}

void TurnOnEyeLEDs() {
    digitalWrite(LED_PIN_EYE_RIGHT, HIGH);
    digitalWrite(LED_PIN_EYE_LEFT, HIGH);
}
void TurnOffEyeLEDs() {
    digitalWrite(LED_PIN_EYE_RIGHT, LOW);
    digitalWrite(LED_PIN_EYE_LEFT, LOW);
}

void TurnOnBodyLEDs() {
    digitalWrite(LED_PIN_BODY_RIGHT, HIGH);
    digitalWrite(LED_PIN_BODY_CENTER, HIGH);
    digitalWrite(LED_PIN_BODY_LEFT, HIGH);
}
void TurnOffBodyLEDs() {
    digitalWrite(LED_PIN_BODY_RIGHT, LOW);
    digitalWrite(LED_PIN_BODY_CENTER, LOW);
    digitalWrite(LED_PIN_BODY_LEFT, LOW);
}

bool ButtonDebounce(int button_input) {
    static int button_prev = HIGH;
    static int button_steps = 0;

    bool button_pressed = false;
    if (button_prev == HIGH) {
        if (button_input == LOW) {
            button_steps++;
            if (button_steps > DEBOUNCE_MAX_STEPS) {
                button_pressed = true;
                button_prev = LOW;
                button_steps = 0;
            }
        } else if (button_steps > 0) {
            button_steps--;
        }
    } else {
        if (button_input == HIGH) {
            button_steps++;
            if (button_steps > DEBOUNCE_MAX_STEPS) {
                button_prev = HIGH;
                button_steps = 0;
            }
        } else if (button_steps > 0) {
            button_steps--;
        }
    }

    return button_pressed;
}