
void setupSerial()
{
    Serial.begin(115200);
    Serial.println("Serial OK...");
}

void setupLED()
{
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("LED OK...");
}

ICACHE_RAM_ATTR void interrupt_handler();

void setupButton()
{
    const int interrupt_pin = 0; // D3
    pinMode(interrupt_pin, INPUT_PULLUP);
    attachInterrupt(interrupt_pin, interrupt_handler, FALLING);
}

void interrupt_handler()
{
    Serial.println("INTERRUPT CATCH!!!");
}

void setup()
{
    setupSerial();
    setupLED();
    setupButton();
}

// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(1000);                     // wait for a second
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(1000);                     // wait for a second
}
