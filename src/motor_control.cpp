#include "motor_control.h"

void motor_setup(){
    // Initialize GPIOs for PWM signals
    // PWM 1 on GPIO 10 and 11
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, 10);  // PWM A on GPIO 10
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, 11);  // Complementary PWM B on GPIO 11
    
    // PWM 2 on GPIO 21 and 47
    mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM0A, 21);  // PWM A on GPIO 21
    mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM0B, 47);  // Complementary PWM B on GPIO 47

    // Configure MCPWM for unit 0 (pins 10, 11)
    mcpwm_config_t pwm_config_1;
    pwm_config_1.frequency = 440;               // 1kHz frequency
    pwm_config_1.cmpr_a = 50.0;                   // Initial duty cycle of 50% for PWM A
    pwm_config_1.cmpr_b = 50.0;                   // Initial duty cycle of 50% for PWM B
    pwm_config_1.counter_mode = MCPWM_UP_COUNTER; // Count up mode
    pwm_config_1.duty_mode = MCPWM_DUTY_MODE_0;   // Active high duty

    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config_1); // Init MCPWM for unit 0

    // Configure MCPWM for unit 1 (pins 21, 47)
    mcpwm_config_t pwm_config_2;
    pwm_config_2.frequency = 440;               // 1kHz frequency
    pwm_config_2.cmpr_a = 50.0;                   // Initial duty cycle of 50% for PWM A
    pwm_config_2.cmpr_b = 50.0;                   // Initial duty cycle of 50% for PWM B
    pwm_config_2.counter_mode = MCPWM_UP_COUNTER; // Count up mode
    pwm_config_2.duty_mode = MCPWM_DUTY_MODE_0;   // Active high duty

    mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_0, &pwm_config_2); // Init MCPWM for unit 1
}

void motor_write(int left, int right){

    int duty_L = left;// map(left, 0, 100, 100, 0);
    int duty_R = right;

        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty_L);       // Set duty for PWM A on unit 0
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, 100 - duty_L); // Complementary duty for PWM B on unit 0

        mcpwm_set_duty(MCPWM_UNIT_1, MCPWM_TIMER_0, MCPWM_OPR_A, duty_R);       // Set duty for PWM A on unit 1
        mcpwm_set_duty(MCPWM_UNIT_1, MCPWM_TIMER_0, MCPWM_OPR_B, 100 - duty_R); // Complementary duty for PWM B on unit 1

}

void motors_set_pwm_frequency(uint32_t frequency) {
    // Configure MCPWM for unit 0 (pins 10 and 11)
    mcpwm_config_t pwm_config_1;
    pwm_config_1.frequency = frequency;           // Set the frequency dynamically
    pwm_config_1.counter_mode = MCPWM_UP_COUNTER; // Count up mode
    pwm_config_1.duty_mode = MCPWM_DUTY_MODE_0;   // Active high duty
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config_1);  // Init MCPWM for unit 0 (pins 10, 11)

    // Configure MCPWM for unit 1 (pins 21 and 47)
    mcpwm_config_t pwm_config_2;
    pwm_config_2.frequency = frequency;           // Set the frequency dynamically
    pwm_config_2.counter_mode = MCPWM_UP_COUNTER; // Count up mode
    pwm_config_2.duty_mode = MCPWM_DUTY_MODE_0;   // Active high duty
    mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_0, &pwm_config_2);  // Init MCPWM for unit 1 (pins 21, 47)
}