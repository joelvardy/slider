# Slider

This repo contains the source code which is compiled and run on the Arduino `slider.ino` as well as the HTML, CSS, JS used to build the interface (which is stored on the SD card.)

I have written up a port detailing [how I motorised the slider][post].

## Update Yun

Go to: http://arduino.cc/en/Tutorial/YunSysupgrade

Install open SSH (for sFTP)

```
opkg update
opkg install openssh-sftp-server
```

## Install

I have an **Arduino Yun** with an **Adafruit Motor Shield V2** connected to a **stepper motor (200 steps/rev, 12V 350mA)** which moves the slider dolly via a belt drive.

I have a wire connected to the dSLR remote trigger port, between ground and pin 7.

### REST Endpoints

The following endpoints are available:

 * `/arduino/run/1` start the slider
 * `/arduino/run/0` stop the slider
 * `/arduino/reset/1` reset the slider position
 * `/arduino/distance/400` set the distance to move the slider in mm
 * `/arduino/delay/400` set the shutter delay in ms
 * `/arduino/steps/3` set the number of stepper steps per move

### JavaScript

There is a `sendRequest` method in the JavaScript which allows you to send a command such as:

```
sendRequest('run/1', function (response) {
    console.log(response); // This is the raw text output from the Yun
});
```

The raw text output from the Yun is parsed in the JavaScript using the `value` method, this simply uses regex and formats the value appropriately.

## Workflow

The workflow to start a timelapse is:

 * Connect to the Arduino access point
 * Browse to http://arduino.local/sd/slider
 * Configure the slider options
 * Press start

Developed by [Joel Vardy][joelvardy].

  [joelvardy]: https://joelvardy.com
  [post]: https://joelvardy.com/writing/motorised-timelapse-slider
