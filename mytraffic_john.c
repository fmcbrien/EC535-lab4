#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>

#define DEVICE_NAME "mytraffic"
#define DEVICE_MAJOR 61
#define DEVICE_MINOR 0

#define RED_LED_GPIO 67  // GPIO PIN FOR RED LED
#define YELLOW_LED_GPIO 68 // GPIO PIN FOR YELLOW LED
#define GREEN_LED_GPIO 44 // GPIO PIN FOR GREEN LED

#define BTN0_GPIO 26    // GPIO PIN FOR BUTTON 0
#define BTN1_GPIO 46    // GPIO PIN FOR BUTTON 1

#define CYCLE_TIME 1    // CYCLE TIME FOR TRAFFIC CYCLES

#define BTN0_IRQ gpio_to_irq(BTN0_GPIO)
#define BTN1_IRQ gpio_to_irq(BTN1_GPIO)

// FLAGS
static int mode = 0; // 0 = normal, 1 = red-flashing, 2 = yellow-flashing 
static int pedestrian_waiting = 0; // 0 = no pedestrian, 1 = pedestrian waiting

// CHECK IF BUTTON 0 HAS BEEN PRESSED
static irqreturn_t btn0_isr(int irq, void *dev_id){
  printk(KERN_INFO "Button 0 pressed\n");
  mode = (mode + 1) % 3;
  return IRQ_HANDLED;
}

// CHECK IF BUTTON 1 HAS BEEN PRESSED
static irqreturn_t btn1_isr(int irq, void *dev_id){
  printk(KERN_INFO "Button 1 pressed\n");
  pedestrian_waiting = 1;
  return IRQ_HANDLED;
}

int set_output(unsigned int pin){
  int ret;
  ret = gpio_direction_output(pin, 0);
  if (ret) {
     printk(KERN_ERR "Failed to set GPIO %d direction to output\n", pin);
     gpio_free(pin); // Free the GPIO on error
  }
  return ret;
}

int set_input(unsigned int pin){
  int ret;
  ret = gpio_direction_input(pin);
  if (ret) {
     printk(KERN_ERR "Failed to set GPIO %d direction to input\n", pin);
     gpio_free(pin); // Free the GPIO on error
  }
  return ret;
}

// RESET ALL LEDS TO OFF
void reset_leds(void){
  gpio_set_value(RED_LED_GPIO, 0);
  gpio_set_value(YELLOW_LED_GPIO, 0);
  gpio_set_value(GREEN_LED_GPIO, 0);
}

// PEDESTRIAN STOP PHASE
void pedestrian_stop_phase(void){
  int i;
  for (i = 0; i < 5; i++) {
    reset_leds();
    ssleep(CYCLE_TIME);
    gpio_set_value(RED_LED_GPIO, 1);
    gpio_set_value(YELLOW_LED_GPIO, 1);
    ssleep(CYCLE_TIME);
  }
  pedestrian_waiting = 0; // Reset pedestrian waiting flag
}

static int __init mytraffic_init(void){
  int ret;

  printk(KERN_INFO "mytraffic module loaded\n");

  // FLAGS
  //int mode = 0; // 0 = normal, 1 = red-flashing, 2 = yellow-flashing
  
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
  ret = set_input(BTN0_GPIO);
  ret = set_input(BTN1_GPIO);
  
  // SET UP INTERRUPTS
  if (request_irq(BTN0_IRQ,
                  (irq_handler_t) btn0_isr,
                  IRQF_TRIGGER_RISING,
                  "btn0_handler",
                  NULL) != 0) {
    printk(KERN_ERR "mytraffic: cannot register IRQ %d\n", BTN0_IRQ);
    gpio_free(BTN0_GPIO);
  }

  if (request_irq(BTN1_IRQ,
                  (irq_handler_t) btn1_isr,
                  IRQF_TRIGGER_RISING,
                  "btn1_handler",
                  NULL) != 0) {
    printk(KERN_ERR "mytraffic: cannot register IRQ %d\n", BTN1_IRQ);
    gpio_free(BTN1_GPIO);
  }

  //int i;
  // int count = 0; // count for which cycle we are on
  while (1){
  // NORMAL LOOP DEFAULT
  int count = 0;
    while (mode == 0){
    //for(i = 0; i < 10; i++) {
      if (count == 0) {
        //gpio_set_value(RED_LED_GPIO, 0);
        reset_leds();
        if (pedestrian_waiting) {pedestrian_stop_phase();}
        gpio_set_value(GREEN_LED_GPIO, 1);
        count ++;
      } else if (count == 3) {
        //gpio_set_value(GREEN_LED_GPIO, 0);
        reset_leds();
        if (pedestrian_waiting) {pedestrian_stop_phase();}
        gpio_set_value(YELLOW_LED_GPIO, 1);
        count ++;
      } else if (count == 4) {
        //gpio_set_value(YELLOW_LED_GPIO, 0);
        reset_leds();
        if (pedestrian_waiting) {pedestrian_stop_phase();}
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
    while (mode == 1){
      //for(i = 0; i < 10; i++) {
      // gpio_set_value(GREEN_LED_GPIO, 0);
      // gpio_set_value(YELLOW_LED_GPIO, 0);
      // gpio_set_value(RED_LED_GPIO, 0);
      reset_leds();
      ssleep(CYCLE_TIME);
      gpio_set_value(RED_LED_GPIO, 1);
      ssleep(CYCLE_TIME);
    }

    // FLASHING-YELLOW
    while (mode == 2){
      // gpio_set_value(GREEN_LED_GPIO, 0);
      // gpio_set_value(YELLOW_LED_GPIO, 0);
      // gpio_set_value(RED_LED_GPIO, 0);
      reset_leds();
      ssleep(CYCLE_TIME);
      gpio_set_value(YELLOW_LED_GPIO, 1);
      ssleep(CYCLE_TIME);
    }
  }
  return 0;
}

static void __exit mytraffic_exit(void) {
  // gpio_set_value(RED_LED_GPIO, 0);
  // gpio_set_value(YELLOW_LED_GPIO, 0);
  // gpio_set_value(GREEN_LED_GPIO, 0);
  reset_leds();
  
  gpio_free(RED_LED_GPIO);
  gpio_free(YELLOW_LED_GPIO);
  gpio_free(GREEN_LED_GPIO);

  free_irq(BTN0_IRQ, NULL);
  free_irq(BTN1_IRQ, NULL);

  gpio_free(BTN0_GPIO);
  gpio_free(BTN1_GPIO);
  
  printk(KERN_INFO "mytraffic module unloaded\n");
}

module_init(mytraffic_init);
module_exit(mytraffic_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple kernel module to control GPIO using legacy functions.");

/*
Specifications
After following the instructions above and confirming that your BeagleBone and traffic light circuit work properly, write a self-contained kernel module called mytraffic that continuously operates your traffic lights according to the specifications below as long as the module is loaded.

Basic Features
Operational Modes
Your traffic light should support 3 operational modes, described below. Your traffic light should start in normal mode upon module load, and the user should be able to switch modes by pressing BTN0. Light patterns are described in time units of “cycles.” The length of a cycle is configurable (see “Additional Features” below), but should default to 1 second (i.e., 1 Hz cycle rate).

Normal: your traffic light should display the following sequence: green for 3 cycles, yellow for 1 cycle, red for 2 cycles, then repeat in that order.
Flashing-Red: your traffic light should continuously flash the red light (1 cycle on, 1 cycle off)
Flashing-Yellow: your traffic light should continuously flash the yellow light (1 cycle on, 1 cycle off)
Readable Character Device
Your module should set up a character device13 at /dev/mytraffic that returns the following information when read (e.g., using cat /dev/mytraffic):

Current operational mode (i.e., “normal”, “flashing-red”, or “flashing-yellow”)
Current cycle rate (e.g., “1 Hz”, “2 Hz”, etc.)
Current status of each light (e.g., “red off, yellow off, green on”)
Whether or not a pedestrian is “present” (i.e., currently crossing or waiting to cross after pressing the call button)
This field is only required if your traffic light supports a pedestrian call button (see “Additional Features” below)
Additional Features
Pedestrian Call Button
BTN1 should act as your traffic light’s pedestrian call button. Following Massachusetts local custom, when this button is pressed, your traffic light’s next “stop” phase14 should illuminate both the red and yellow lights for 5 cycles, instead of just showing the red light for 2 cycles. After the stop phase is complete, your traffic light should return to the normal operation light pattern given above. This button only needs to be active in the normal operation mode; pressing it while your light is in “flashing-red” or “flashing-yellow” mode may have no effect.

Writable Character Device
In addition to the read functionality described above, writing an integer between 1 and 9 (inclusive) to the /dev/mytraffic character device should alter the cycle rate (speed) of the traffic light. For example, running echo 2 > /dev/mytraffic should set the cycle rate to 2 Hz (causing each cycle to last 0.5 seconds). Any other data written to the character device should be silently ignored. As always, your kernel module should never crash.

Requirements
To receive 80% of the grade, your module must implement the three operational modes, mode switching button, and readable character device functionality described in “Basic Features.” To receive the remaining 20%, your module must also implement the pedestrian call button (10%) and the writable character device functionality (10%) described in “Additional Features.” As always, you must follow all submission requirements (including the demo video, see below) to receive full credit.

Extra Credit
For 5 extra points, add a “lightbulb check” feature that solidly lights up all three traffic lights as long as both BTN0 and BTN1 are being held down simultaneously. Upon releasing both buttons, your traffic light should “reset” back to its initial state (normal mode at 1 Hz cycle rate, starting with 3 cycles of green, no pedestrians present) regardless of which state it was in previously.

Demonstration Video
After you’ve finished your implementation according to the specifications above, please record a 90-second-or-shorter video demonstrating the operation of your traffic light. Please ensure your video demonstrates the following:

Switching between all 3 operational modes using BTN0
Operating the pedestrian call button (BTN1) while in the normal operational mode
The output of /dev/mytraffic matching what the traffic light is actually doing
Hint: try placing your running traffic light right in front/next to your monitor while running watch cat /dev/mytraffic
Changing the cycle rate by writing to /dev/mytraffic a few different times with a few different values
Hint: using the same setup from the previous hint, try running for f in 1 3 5 7 9 1; do echo $f | tee /dev/mytraffic; sleep 3; done
If attempting the extra credit, triggering the “lightbulb check” mode by holding down both BTN0 and BTN1 for a few seconds
Please don’t stress over the image quality/lighting/narration/videography/etc. of your demo video. As long as your video clearly shows what features of your lab are working properly in less than 90 seconds, you’ll be fine. Please also ensure that your video is playable using the VLC media player before submitting.
*/