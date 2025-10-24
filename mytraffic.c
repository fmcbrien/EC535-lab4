#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>

#define RED_LED_GPIO 67     // GPIO PIN FOR RED LED
#define YELLOW_LED_GPIO 68  // GPIO PIN FOR YELLOW LED
#define GREEN_LED_GPIO 44   // GPIO PIN FOR GREEN LED

#define BTN0_GPIO 26    // GPIO PIN FOR BUTTON 0
#define BTN1_GPIO 46    // GPIO PIN FOR BUTTON 1

#define CYCLE_TIME 1    // CYCLE TIME FOR TRAFFIC CYCLES

static int __init mytraffic_init(void)
{
  int ret;
  
  // REQUEST GPIO FOR LEDs
  ret = gpio_request(RED_LED_GPIO, "RED_GPIO");
  if (ret) {
    printk(KERN_ERR "Failed to request RED LED GPIO\n");
    return ret;
  }
  ret = gpio_request(YELLOW_LED_GPIO, "YELLOW_GPIO");
  if (ret) {
    printk(KERN_ERR "Failed to request YELLOW LED GPIO\n");
    gpio_free(RED_LED_GPIO);
    return ret;
  }
  ret = gpio_request(GREEN_LED_GPIO, "GREEN_GPIO");
  if (ret) {
    printk(KERN_ERR "Failed to request GREEN LED GPIO\n");
    gpio_free(RED_LED_GPIO);
    gpio_free(YELLOW_LED_GPIO);
    return ret;
  }

  // SET DIRECTION FOR LEDs
  ret = gpio_direction_output(RED_LED_GPIO, 0);
  ret = gpio_direction_output(YELLOW_LED_GPIO, 0);
  ret = gpio_direction_output(GREEN_LED_GPIO, 0);

  // SET VALUE FOR LEDs
  gpio_set_value(RED_LED_GPIO, 1);
  gpio_set_value(YELLOW_LED_GPIO, 1);
  gpio_set_value(GREEN_LED_GPIO, 1);
   
  printk(KERN_INFO "mytraffic module loaded\n");
  return 0;
}

static void __exit mytraffic_exit(void)
{
  gpio_set_value(RED_LED_GPIO, 0);
  gpio_set_value(YELLOW_LED_GPIO, 0);
  gpio_set_value(GREEN_LED_GPIO, 0);

  gpio_free(RED_LED_GPIO);
  gpio_free(YELLOW_LED_GPIO);
  gpio_free(GREEN_LED_GPIO);

  gpio_free(BTN0_GPIO);
  gpio_free(BTN1_GPIO);

  printk(KERN_INFO "mytraffic module unloaded\n");
}

module_init(mytraffic_init);
module_exit(mytraffic_exit);

MODULE_LICENSE("GPL");