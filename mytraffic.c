#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/gpio/driver.h>

#define RED_LED_GPIO 67  // GPIO PIN FOR RED LED

static int __init mytraffic_init(void)
{
  int ret;
  
  // REQUEST GPIO FOR LEDs
  ret = gpio_request(RED_LED_GPIO, "GPIO_67");

  // SET DIRECTION FOR LEDs
  ret = gpio_direction_output(RED_LED_GPIO, 0);

  // SET VALUE FOR LEDs
  gpio_set_value(RED_LED_GPIO, 1);
   
  printk(KERN_INFO "mytraffic module loaded\n");
  return 0;
}

static void __exit mytraffic_exit(void)
{
  gpio_free(RED_LED_GPIO);
  printk(KERN_INFO "mytraffic module unloaded\n");
}

module_init(mytraffic_init);
module_exit(mytraffic_exit);
