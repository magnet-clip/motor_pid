# motor_pid
## Project still in development
Two motors rotation speed control using PID regulator on Arduino

    PID Settings:

1. Small overshoot over long rail-to-rail move, small oscillations in the middle positions:
    Motor is connected to E and +2 terminals. There also is a +1 terminal which seems to correspond to slower winding    

```c++    
    #define KP 1
    #define KI 0.01 
    #define KD 0.25
    
    // max value of integral part, when value is smaller integral is too weak to pick up differences in real and desired values
    #define MAX_I 16384       
    
    // update frequency in ms
    #define UPDATE_PID 10       
    
    // motor rotates forward too fast and backward too slow. Hence I slow him down on the FWD
    // and speed him up in rear direction
    #define FWD_COEFF 0.4
    #define RWD_COEFF 2  

    // minimum PWM value when motor would rotate. given current settings 
    #define MIN_PWM 15 
    
    // difference between actual and desired angle which is considered to be ok
    // 10 corresponds to 300 * (10/1024) approximately 3 degrees which is ok
    // I can increase it to 20 (6 degrees) and that
    #define ACCURACY 10 

    // i map desired angle to arbitrary interval given here. I can then adjust this interval so 
    // that to avoid overshoot which will result in motor trying to rotate further then it is possible and break steering
    #define MIN_DESIRED 0     
    #define MAX_DESIRED 1023  
```

There seems to be some non-linearity in my pots, so their relative rotation seems to be out of sync. Maybe i have to make a mapping. And then maybe FRD/RWD coeffs will be obsolete
