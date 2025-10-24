#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/interrupt.h>

#define DEVICE_NAME "mytraffic"
#define DEVICE_MAJOR 61
#define DEVICE_MINOR 0

#define RED_GPIO 67     // GPIO PIN FOR RED LED
#define YELLOW_GPIO 68  // GPIO PIN FOR YELLOW LED
#define GREEN_GPIO 44   // GPIO PIN FOR GREEN LED

#define BTN0_GPIO 26    // GPIO PIN FOR BUTTON 0
#define BTN1_GPIO 46    // GPIO PIN FOR BUTTON 1

#define CYCLE_TIME 1    // CYCLE TIME FOR TRAFFIC CYCLES

static struct gpio_desc *red_led;
static struct gpio_desc *yellow_led;
static struct gpio_desc *green_led;

static struct gpio_desc *button0;
static struct gpio_desc *button1;

static void GPIO_SET_1(unsigned int pin) {
  
}

static int mytraffic_init(void) {
  int mode = 0; // normal mode default (0 = normal, 1 = flashing-red, 2 = flashing-yellow)
  
  // REQUEST GPIO FOR LEDS
  red_led = gpio_to_desc(RED_GPIO);
  yellow_led = gpio_to_desc(YELLOW_GPIO);
  green_led = gpio_to_desc(GREEN_GPIO);

  gpiod_direction_output(red_led, 0);
  gpiod_direction_output(yellow_led, 0);
  gpiod_direction_output(green_led, 0);

  // REQUEST GPIO FOR BUTTONS
  button0 = gpio_to_desc(BUTTON0_GPIO);
  button1 = gpio_to_desc(BUTTON1_GPIO);

  gpiod_direction_input(button0);
  gpiod_direction_input(button1);
  
  while(1){
    if (mode == 0) {
      // NORMAL LOOP
      int count = 0;
      if (count == 0) {
	gpio_direction_output(red_led, 0);
	gpio_direction_output(green_led, 0);
      
	count ++;
      } else if (count == 3) {
	gpio_direction_output(green_led, 0);
	gpio_direction_output(yellow_led, 1);
      
	count ++;
      } else if (count == 4) {
	gpio_direction_output(yellow_led, 0);
	gpio_direction_output(red_led, 1);
      
	count ++;
      } else {   // light unchanged, just increment count for 1, 2, and 5
	count ++;
      }
    } else if (mode == 1) {
      // FLASHING-RED
    } else if (mode == 2) {
      // FLASHING-YELLOW
    }
    
    sleep(CYCLE_TIME);  // WAIT FOR CYCLE TIME
  }
  
  printk(KERN_INFO "mytraffic module loaded\n")
  return 0;
}

static void mytraffic_exit(void) {
  printk(KERN_ALERT "mytraffic module unloaded\n");
}

module_init(mytraffic_init);
module_exit(mytraffic_exit);
