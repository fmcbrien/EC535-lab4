#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/gpio/driver.h>

#define RED_LED_GPIO    67  // GPIO PIN FOR RED LED

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

static void mytraffic_exit(void)
{
  printk(KERN_INFO "mytraffic module unloaded\n");
}

module_init(mytraffic_init);
module_exit(mytraffic_exit);
