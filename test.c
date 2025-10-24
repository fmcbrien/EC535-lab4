#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>


#define RED_LED_GPIO    67  // GPIO PIN FOR RED LED
#define YELLOW_LED_GPIO 68  // GPIO PIN FOR YELLOW LED
#define GREEN_LED_GPIO  44  // GPIO PIN FOR GREEN LED


#define BUTTON0_GPIO   26  // GPIO PIN FOR BUTTON 0
#define BUTTON1_GPIO   46  // GPIO PIN FOR BUTTON 1

static int mytraffic_init(void)
{


  // REQUEST GPIO FOR LEDs
  gpio_request(RED_LED_GPIO, "RED_LED");
  gpio_request(YELLOW_LED_GPIO, "YELLOW_LED");
  gpio_request(GREEN_LED_GPIO, "GREEN_LED");

  // SET DIRECTION FOR LEDs
  gpio_direction_output(RED_LED_GPIO, 1);
  gpio_direction_output(YELLOW_LED_GPIO, 0);
  gpio_direction_output(GREEN_LED_GPIO, 0);


  // REQUEST GPIO FOR BUTTONS
  gpio_request(BUTTON0_GPIO, "BTN0");
  gpio_request(BUTTON1_GPIO, "BTN1");

  // SET DIRECTION FOR BUTTONS
  gpio_direction_input(BUTTON0_GPIO);
  gpio_direction_input(BUTTON1_GPIO);
   
    printk(KERN_INFO "mytraffic module loaded\n");
    return 0;
}

module_init(mytraffic_init);
