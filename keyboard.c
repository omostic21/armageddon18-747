#include <linux/uinput.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void emit(int fd, int type, int code, int val)
{
   struct input_event ie;

   ie.type = type;
   ie.code = code;
   ie.value = val;
   /* timestamp values below are ignored */
   ie.time.tv_sec = 0;
   ie.time.tv_usec = 0;

   write(fd, &ie, sizeof(ie));
}

int main(void)
{
   struct uinput_setup usetup;

   int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);


   /*
    * The ioctls below will enable the device that is about to be
    * created, to pass key events, in this case the space key.
    */
   ioctl(fd, UI_SET_EVBIT, EV_KEY);
   ioctl(fd, UI_SET_KEYBIT, KEY_SPACE);
   ioctl(fd, UI_SET_KEYBIT, KEY_BACKSPACE);
   ioctl(fd, UI_SET_KEYBIT, KEY_ENTER);
   ioctl(fd, UI_SET_KEYBIT, KEY_A);
   ioctl(fd, UI_SET_KEYBIT, KEY_B);
   ioctl(fd, UI_SET_KEYBIT, KEY_C);
   ioctl(fd, UI_SET_KEYBIT, KEY_D);
   ioctl(fd, UI_SET_KEYBIT, KEY_E);
   ioctl(fd, UI_SET_KEYBIT, KEY_F);
   ioctl(fd, UI_SET_KEYBIT, KEY_G);

   memset(&usetup, 0, sizeof(usetup));
   usetup.id.bustype = BUS_USB;
   usetup.id.vendor = 0x1234; /* sample vendor */
   usetup.id.product = 0x5678; /* sample product */
   strcpy(usetup.name, "Example device");

   ioctl(fd, UI_DEV_SETUP, &usetup);
   ioctl(fd, UI_DEV_CREATE);

   /*
    * On UI_DEV_CREATE the kernel will create the device node for this
    * device. We are inserting a pause here so that userspace has time
    * to detect, initialize the new device, and can start listening to
    * the event, otherwise it will not notice the event we are about
    * to send. This pause is only needed in our example code!
    */
   while(1){
    sleep(5);

    /* Key press, report the event, send key release, and report again */
   

    /*
        * Give userspace some time to read the events before we destroy the
        * device with UI_DEV_DESTOY.
        */
    sleep(5);
    }

   ioctl(fd, UI_DEV_DESTROY);
   close(fd);

   return 0;
}