#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

#define RED_LED_GPIO 67  // GPIO PIN FOR RED LED
#define YELLOW_LED_GPIO 68 // GPIO PIN FOR YELLOW LED
#define GREEN_LED_GPIO 44 // GPIO PIN FOR GREEN LED

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
  
  // REQUEST GPIO FOR LEDs
  ret = gpio_request(RED_LED_GPIO, "GPIO_67");
  if (ret) {
     printk(KERN_ERR "Failed to request GPIO %d\n", RED_LED_GPIO);
     return ret;
   }
  ret = gpio_request(YELLOW_LED_GPIO, "GPIO_68");
  if (ret) {
     printk(KERN_ERR "Failed to request GPIO %d\n", YELLOW_LED_GPIO);
     return ret;
   }
  ret = gpio_request(GREEN_LED_GPIO, "GPIO_44");
  if (ret) {
     printk(KERN_ERR "Failed to request GPIO %d\n", GREEN_LED_GPIO);
     return ret;
   }

  // SET DIRECTION FOR LEDs
  ret = set_output(RED_LED_GPIO);
  ret = set_output(YELLOW_LED_GPIO);
  ret = set_output(GREEN_LED_GPIO);

  // SET VALUE FOR LEDs
  gpio_set_value(RED_LED_GPIO, 1);
  gpio_set_value(YELLOW_LED_GPIO, 1);
  gpio_set_value(GREEN_LED_GPIO, 1);
  
   
  return 0;
}

static void __exit mytraffic_exit(void)
{
  gpio_free(RED_LED_GPIO);
  printk(KERN_INFO "mytraffic module unloaded\n");
}

module_init(mytraffic_init);
module_exit(mytraffic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple kernel module to control GPIO using legacy functions.");
