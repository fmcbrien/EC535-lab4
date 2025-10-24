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
  gpio_request(RED_LED_GPIO, "GPIO_67");

  // SET DIRECTION FOR LEDs
  gpio_direction_output(RED_LED_GPIO, 1);

  // SET VALUE FOR LEDs
  gpio_set_value(RED_LED_GPIO, 1);
   
  printk(KERN_INFO "mytraffic module loaded\n");
  return 0;
}

module_init(mytraffic_init);
