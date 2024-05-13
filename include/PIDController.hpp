#ifndef PIDCONTROLLER
#define PIDCONTROLLER

#include <Arduino.h>

class PIDController {
private:
    float kp;
    float ki;
    float kd;

    float integral;
    float prevError;
    float setpoint;
    float output;

    float maxOutput;

public:
    PIDController(float kp, float ki, float kd, float setpoint, float maxOutput=255) {
        if (maxOutput < 0) {
            maxOutput = 0;
        }
        
        this->kp = kp;
        this->ki = ki;
        this->kd = kd;
        this->setpoint = setpoint;
        this->maxOutput = maxOutput;
        this->integral = 0;
        this->prevError = 0;
        this->output = 0;
    }

    /**
     * @brief Set the setpoint object. The setpoint is the desired value that the PID controller will try to reach.
    */
    void setSetpoint(float setpoint) {
        this->setpoint = setpoint;
    }

    /**
     * @brief Get the output object. The output is the value that the PID controller will output based on the input and setpoint.
    */
    float getOutput(float input) {
        float error = setpoint - input;

        integral += error;
        float derivative = error - prevError;
        output = kp * error + ki * integral + kd * derivative;
        prevError = error;


        return constrain(output, -maxOutput, maxOutput);
    }

    void reset() {
        integral = 0;
        prevError = 0;
        output = 0;
    }

    void setKp(float kp) {
        this->kp = kp;
    }

    void setKi(float ki) {
        this->ki = ki;
    }

    void setKd(float kd) {
        this->kd = kd;
    }
    
    float getKp() {
        return kp;
    }

    float getKi() {
        return ki;
    }

    float getKd() {
        return kd;
    }

    void setMaxOutput(float maxOutput) {
        this->maxOutput = maxOutput;
    }


};

#endif // PIDCONTROLLER