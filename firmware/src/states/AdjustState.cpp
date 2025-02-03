#include "StateMachine.h"
#include "Controllers.h"

void AdjustState::enter()
{
    Serial.println("Entering Adjust State");

    lastActivity = millis();
    ledController.setSolid(AMBER);

    // Register state-specific handlers
    inputController.onPressHandler([this]()
                                   {
        Serial.println("Adjust State: Button pressed"); 
        
        StateMachine::idleState.setTimer(this->adjustDuration);
        displayController.showConfirmation();
        stateMachine.changeState(&StateMachine::idleState); });

    inputController.onEncoderRotateHandler([this](int delta)
                                           {
        Serial.println("Adjust State: Encoder turned");
        Serial.println(delta);
        
        // Update duration based on orientation configuration
#if ENCODER_REVERSE_VALUE
        this->adjustDuration -= (delta * 5);  // Reversed
#else
        this->adjustDuration += (delta * 5);  // Normal
#endif
        
        if (this->adjustDuration < MIN_TIMER) {
            this->adjustDuration = MIN_TIMER;
        } else if (this->adjustDuration > MAX_TIMER) {
            this->adjustDuration = MAX_TIMER;
        }

        this->lastActivity = millis(); });
}

void AdjustState::update()
{
    inputController.update();
    displayController.drawAdjustScreen(adjustDuration);

    if (millis() - lastActivity >= (CHANGE_TIMEOUT * 1000))
    {
        // Transition to Idle
        stateMachine.changeState(&StateMachine::idleState);
    }
}

void AdjustState::exit()
{
    Serial.println("Exiting Adjust State");
    inputController.releaseHandlers();
}

void AdjustState::adjustTimer(int duration)
{
    adjustDuration = duration;
}
