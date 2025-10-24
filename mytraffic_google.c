#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h> // Required for GPIO functions

#define MY_GPIO_PIN 67 // Replace with your actual GPIO number

static int __init mytraffic_init(void)
{
    int ret;

    printk(KERN_INFO "My GPIO module loaded\n");

    // Request the GPIO
    ret = gpio_request(MY_GPIO_PIN, "my_gpio_driver");
    if (ret) {
        printk(KERN_ERR "Failed to request GPIO %d\n", MY_GPIO_PIN);
        return ret;
    }

    // Set GPIO as output and set initial value to low
    ret = gpio_direction_output(MY_GPIO_PIN, 0);
    if (ret) {
        printk(KERN_ERR "Failed to set GPIO %d direction to output\n", MY_GPIO_PIN);
        gpio_free(MY_GPIO_PIN); // Free the GPIO on error
        return ret;
    }

    // Set GPIO value to high
    gpio_set_value(MY_GPIO_PIN, 1);
    printk(KERN_INFO "GPIO %d set to high\n", MY_GPIO_PIN);

    // You can also read the value (e.g., if it's an input or for verification)
    // int value = gpio_get_value(MY_GPIO_PIN);
    // printk(KERN_INFO "GPIO %d value: %d\n", MY_GPIO_PIN, value);

    return 0;
}

static void __exit mytraffic_exit(void)
{
    // Set GPIO value back to low before freeing (optional, good practice)
    gpio_set_value(MY_GPIO_PIN, 0);

    // Free the GPIO
    gpio_free(MY_GPIO_PIN);
    printk(KERN_INFO "My GPIO module unloaded and GPIO %d freed\n", MY_GPIO_PIN);
}

module_init(mytraffic_init);
module_exit(mytraffic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple kernel module to control GPIO using legacy functions.");
