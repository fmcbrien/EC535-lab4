#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/gpio/consumer.h>


#define RED_LED_GPIO    67  // GPIO PIN FOR RED LED
#define YELLOW_LED_GPIO 68  // GPIO PIN FOR YELLOW LED
#define GREEN_LED_GPIO  44  // GPIO PIN FOR GREEN LED


#define BUTTON0_GPIO   26  // GPIO PIN FOR BUTTON 0
#define BUTTON1_GPIO   46  // GPIO PIN FOR BUTTON 1


static struct gpio_desc *red_led;
static struct gpio_desc *yellow_led;
static struct gpio_desc *green_led;


static struct gpio_desc *button0;
static struct gpio_desc *button1;


static int mytraffic_init(void)
{


    // REQUEST GPIO FOR LEDS
    red_led = gpio_to_desc(RED_LED_GPIO);
    yellow_led = gpio_to_desc(YELLOW_LED_GPIO);
    green_led = gpio_to_desc(GREEN_LED_GPIO);

    gpiod_direction_output(red_led, 1);
    gpiod_direction_output(yellow_led, 0);
    gpiod_direction_output(green_led, 0);


    // REQUEST GPIO FOR BUTTONS
    button0 = gpio_to_desc(BUTTON0_GPIO);
    button1 = gpio_to_desc(BUTTON1_GPIO);

    gpiod_direction_input(button0);
    gpiod_direction_input(button1);
   
    printk(KERN_INFO "mytraffic module loaded\n");
    return 0;
}

module_init(mytraffic_init);
