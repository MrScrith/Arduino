/* The MIT License (MIT)
 *
 * Copyright (c) 2015 Erik
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* ---------------------------- Defines ---------------------------- */
#define LED_ONE      0
#define LED_TWO      1
#define LED_THREE    2
#define LED_FOUR     3
#define SWITCH_ONE   4
#define SWITCH_TWO   5
#define SWITCH_THREE A1
#define LIGHT_SENSOR A0

#define LOOP_DELAY   10
#define PRESS_LENGTH 100

#define LOW_LIGHT    100

/* ----------------------------- Enums ----------------------------- */

enum switchStates_e {
    DONE,
    PRESS,
    FINISHED
};

/* ------------------------ Local Variables ------------------------ */


static uint8_t switchStateOne   = DONE;
static uint8_t switchStateTwo   = DONE;
static uint8_t switchStateThree = DONE;


static uint8_t count2 = 0;
static uint8_t count1 = 0;

static uint8_t count3 = 0;

/* --------------------------- Functions --------------------------- */

void setup() 
{
    // debugging
    Serial.begin(57600);
    
    // Set up inputs
    pinMode(SWITCH_ONE,INPUT_PULLUP);
    pinMode(SWITCH_TWO,INPUT_PULLUP);
    pinMode(SWITCH_THREE,INPUT_PULLUP);
 
    /* turn the center light on as a signal that everything is good. */
    digitalWrite(LED_TWO, HIGH);
    digitalWrite(LED_FOUR,HIGH);
    
    Serial.println("Starting up.");
}

void loop() {
    
    uint8_t pinstate1 = digitalRead(SWITCH_ONE);
    uint8_t pinstate2 = digitalRead(SWITCH_TWO);
    uint8_t pinstate3 = digitalRead(SWITCH_THREE);
    
    if ( !pinstate1 )
    {
        /* Button one pressed */
        if (switchStateOne == DONE )
        {
            digitalWrite(LED_ONE,!digitalRead(LED_ONE));
            switchStateOne = PRESS;
            count1 = 0;
            Serial.println("Button one pressed, light set on and state set to PRESS");
        }
        else if ( switchStateOne == PRESS )
        {
            if ( count1 < PRESS_LENGTH )
            {
                count1++;
            }
            else
            {
                switchStateOne = FINISHED;
                digitalWrite(LED_TWO, digitalRead(LED_ONE));
                digitalWrite(LED_THREE, digitalRead(LED_ONE));
                Serial.println("Button one pressed for " + String(count1) + " counts, now setting all lights.");
            }
        }
    }
    else
    {
        switchStateOne = DONE;
    }
            
    if ( !pinstate2 )
    {
        /* Button two pressed */
        if (switchStateTwo == DONE )
        {
            digitalWrite(LED_THREE,!digitalRead(LED_THREE));
            switchStateTwo = PRESS;
            count2 = 0;
            Serial.println("Button two pressed, light set on and state set to PRESS");
        }
        else if ( switchStateTwo == PRESS )
        {
            if ( count2 < PRESS_LENGTH )
            {
                count2++;
            }
            else
            {
                switchStateTwo = FINISHED;
                digitalWrite(LED_ONE, digitalRead(LED_THREE));
                digitalWrite(LED_TWO, digitalRead(LED_THREE));
                Serial.println("Button two pressed for " + String(count1) + " counts, now setting all lights.");
            }
        }
    }
    else
    {
        switchStateTwo = DONE;
    }    
    
    if ( !pinstate3 )
    {
        if( switchStateThree == DONE )
        {
            uint8_t tmpval = !digitalRead(LED_TWO);
            Serial.println("Button three pressed, light set on and state set to PRESS");
            digitalWrite(LED_TWO, tmpval); // Sets oposite of center (ie off to on)
            digitalWrite(LED_ONE, tmpval); // Sets to same state as center
            digitalWrite(LED_THREE, tmpval); // Sets to same state as center
            switchStateThree = FINISHED;
        }
    }
    else
    {
        switchStateThree = DONE;
    }
    
    if ( count3 > PRESS_LENGTH )
    {
      Serial.println("Light sensor value is " + String(analogRead(LIGHT_SENSOR)));
      if ( analogRead(LIGHT_SENSOR) < LOW_LIGHT )
      {
        digitalWrite(LED_FOUR,HIGH);
        Serial.println("Light level is low, turning on light.");
      }
      else
      {
        
        Serial.println("Light level is high, turning off light.");
        digitalWrite(LED_FOUR,LOW);
      }
      count3 = 0;
    }
    else
    {
      count3++;
    }
    
    delay(LOOP_DELAY);
}
