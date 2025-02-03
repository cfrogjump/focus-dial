#include "controllers/InputController.h"
#include <Arduino.h>

static InputController *instancePtr = nullptr; // Global pointer for the ISR

void InputController::handleEncoderInterrupt()
{
    if (instancePtr)
    {
        instancePtr->encoder.tick();
    }
}

void InputController::handleButtonInterrupt()
{
    if (instancePtr)
    {
        bool buttonState = digitalRead(instancePtr->buttonPin);
        Serial.printf("Button state: %d\n", buttonState);
        instancePtr->button.tick();
    }
}

InputController::InputController(uint8_t buttonPin, uint8_t encoderPinA, uint8_t encoderPinB)
    : button(buttonPin, true),
      encoder(encoderPinA, encoderPinB, RotaryEncoder::LatchMode::TWO03),
      lastPosition(0),
      buttonPin(buttonPin),
      encoderPinA(encoderPinA),
      encoderPinB(encoderPinB)
{

    // Attach click, double-click, and long-press handlers using OneButton library
    button.attachClick([](void *scope)
                       { static_cast<InputController *>(scope)->onButtonClick(); }, this);
    button.attachDoubleClick([](void *scope)
                             { static_cast<InputController *>(scope)->onButtonDoubleClick(); }, this);
    button.attachLongPressStart([](void *scope)
                                { static_cast<InputController *>(scope)->onButtonLongPress(); }, this);

    instancePtr = this; // Set the global instance pointer to this instance
}

void InputController::begin()
{
    Serial.printf("Initializing button on pin %d\n", buttonPin);
    button.setDebounceMs(20);
    button.setClickMs(150);
    button.setPressMs(400);
    lastPosition = encoder.getPosition();

    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(encoderPinA, INPUT_PULLUP);
    pinMode(encoderPinB, INPUT_PULLUP);

    Serial.printf("Initial button state: %d\n", digitalRead(buttonPin));

    // Set up interrupts for encoder handling
    attachInterrupt(digitalPinToInterrupt(encoderPinA), handleEncoderInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(encoderPinB), handleEncoderInterrupt, CHANGE);
    
    // Set up interrupt for button
    attachInterrupt(digitalPinToInterrupt(buttonPin), handleButtonInterrupt, CHANGE);

    // Set up interrupt for button handling
    attachInterrupt(digitalPinToInterrupt(buttonPin), handleButtonInterrupt, CHANGE); // Interrupt on button state change
}

void InputController::update()
{
    // Check button state
    button.tick();

    // Check encoder position
    long newPosition = encoder.getPosition();
    if (newPosition != lastPosition)
    {
        int delta = newPosition - lastPosition;
        lastPosition = newPosition;
        onEncoderRotate(delta);
    }
}

// Register state-specific handlers
void InputController::onPressHandler(std::function<void()> handler)
{
    pressHandler = handler;
}

void InputController::onDoublePressHandler(std::function<void()> handler)
{
    doublePressHandler = handler;
}

void InputController::onLongPressHandler(std::function<void()> handler)
{
    longPressHandler = handler;
}

void InputController::onEncoderRotateHandler(std::function<void(int delta)> handler)
{
    encoderRotateHandler = handler;
}

// Method to release all handlers
void InputController::releaseHandlers()
{
    pressHandler = nullptr;
    doublePressHandler = nullptr;
    longPressHandler = nullptr;
    encoderRotateHandler = nullptr;

    button.reset();                       // Reset button state machine
    lastPosition = encoder.getPosition(); // Reset encoder position tracking
}

// Internal event handlers that call the registered state handlers
void InputController::onButtonClick()
{
    Serial.println("Button clicked!");
    if (pressHandler != nullptr)
    {
        pressHandler();
    }
}

void InputController::onButtonDoubleClick()
{
    Serial.println("Button double clicked!");
    if (doublePressHandler != nullptr)
    {
        doublePressHandler();
    }
}

void InputController::onButtonLongPress()
{
    Serial.println("Button long pressed!");
    if (longPressHandler != nullptr)
    {
        longPressHandler();
    }
}

void InputController::onEncoderRotate(int delta)
{
    if (encoderRotateHandler != nullptr)
    {
        encoderRotateHandler(delta); // Pass delta to the handler
    }
}
