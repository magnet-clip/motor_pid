# motor_pid
## Project still in development
Two motors rotation speed control using PID regulator on Arduino

PID Settings:
1. Small overshoot over long rail-to-rail move, small oscillations in the middle positions:


    #define KP 1
    #define KI 0.01 
    #define KD 0.25
    #define MAX_I 16384       // max value of integral part, when value is smaller integral is too weak to pick up differences in real and desired values
    #define UPDATE_PID 10       // update frequency in ms
    
    #define FWD_COEFF 0.4 // motor rotates forward too fast and backward too slow. Hence I slow him down on the FWD
    #define RWD_COEFF 2   // and speed him up in rear direction

    #define MIN_PWM 15  // minimum PWM value when motor would rotate
    #define ACCURACY 10 // difference between actual and desired angle which is considered to be ok
    
    #define MIN_DESIRED 0     // i map desired angle to arbitrary interval given here. I can then adjust this interval so 
    #define MAX_DESIRED 1023  // that to avoid overshoot which will result in motor trying to rotate further then it is possible and break steering
