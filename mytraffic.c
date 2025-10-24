#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

#define DEVICE_NAME "mytraffic"
#define DEVICE_MAJOR 61
#define DEVICE_MINOR 0

#define RED_LED_GPIO 67  // GPIO PIN FOR RED LED
#define YELLOW_LED_GPIO 68 // GPIO PIN FOR YELLOW LED
#define GREEN_LED_GPIO 44 // GPIO PIN FOR GREEN LED

#define BTN0_GPIO 26    // GPIO PIN FOR BUTTON 0
#define BTN1_GPIO 46    // GPIO PIN FOR BUTTON 1

#define CYCLE_TIME 1    // CYCLE TIME FOR TRAFFIC CYCLES

int set_output(unsigned int pin){
  int ret;
  ret = gpio_direction_output(pin, 0);
  if (ret) {
     printk(KERN_ERR "Failed to set GPIO %d direction to output\n", pin);
     gpio_free(pin); // Free the GPIO on error
  }
  return ret;
}

static int __init mytraffic_init(void)
{
  int ret;

  printk(KERN_INFO "mytraffic module loaded\n");

  // FLAGS
  int mode = 0; // 0 = normal, 1 = red-flashing, 2 = yellow-flashing
  
  // REQUEST GPIO FOR LEDs
  ret = gpio_request(RED_LED_GPIO, "GPIO_67");
  ret = gpio_request(YELLOW_LED_GPIO, "GPIO_68");
  ret = gpio_request(GREEN_LED_GPIO, "GPIO_44");
  ret = gpio_request(BTN0_GPIO, "GPIO_26");
  ret = gpio_request(BTN1_GPIO, "GPIO_46");

  // SET DIRECTION FOR LEDs AND BUTTONS
  ret = set_output(RED_LED_GPIO);
  ret = set_output(YELLOW_LED_GPIO);
  ret = set_output(GREEN_LED_GPIO);
  // TODO set buttons up
    int i;

  
  // Normal loop default
  int count = 0; // count for which cycle we are on
  for(i = 0; i < 21; i++) {
    if (count == 0) {
      gpio_set_value(RED_LED_GPIO, 0);
      gpio_set_value(GREEN_LED_GPIO, 1);
      count ++;
    } else if (count == 3) {
      gpio_set_value(GREEN_LED_GPIO, 0);
      gpio_set_value(YELLOW_LED_GPIO, 1);
      count ++;
    } else if (count == 4) {
      gpio_set_value(YELLOW_LED_GPIO, 0);
      gpio_set_value(RED_LED_GPIO, 1);
      count ++;
    } else if (count == 5) {
      count = 0;
    } else {
      count ++;
    }
    ssleep(CYCLE_TIME);
  }

  // Flashing-red
  for(i = 0; i < 15; i++) {
    gpio_set_value(RED_LED_GPIO, 1);
    ssleep(CYCLE_TIME);
    gpio_set_value(RED_LED_GPIO, 0);
    ssleep(CYCLE_TIME);
  }

  // Flashing-yellow
  for(i = 0; i < 15; i++) {
    gpio_set_value(YELLOW_LED_GPIO, 1);
    ssleep(CYCLE_TIME);
    gpio_set_value(YELLOW_LED_GPIO, 0);
    ssleep(CYCLE_TIME);
  }
  
  return 0;
}

static void __exit mytraffic_exit(void) {
  gpio_set_value(RED_LED_GPIO, 0);
  gpio_set_value(YELLOW_LED_GPIO, 0);
  gpio_set_value(GREEN_LED_GPIO, 0);
  
  gpio_free(RED_LED_GPIO);
  gpio_free(YELLOW_LED_GPIO);
  gpio_free(GREEN_LED_GPIO);
  printk(KERN_INFO "mytraffic module unloaded\n");
}

module_init(mytraffic_init);
module_exit(mytraffic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple kernel module to control GPIO using legacy functions.");
