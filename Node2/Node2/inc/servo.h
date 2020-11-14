
#ifndef SERVO_H_
#define SERVO_H_

// Configures timer0 to provide PWM signal suitable for a servo on PWM2
void servo_init_pwm();

// Set servo to position from 0 to 255 (defined as leftbound to rightbound)
void servo_set_position(uint8_t postition);

#endif /* SERVO_H_ */