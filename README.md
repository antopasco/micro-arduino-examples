# iProtoXi Micro Arduino examples
These are Arduino examples for iProtoXi Micro and iProtoXi sensor boards. Arduino IDE Classic (1.0.x) required.
## How to use
1. Download the repository extract to Arduino Sketchbook folder.
2. If using windows. Install driver at windows-driver -folder.
3. Connect sensor board(s) to Micro.
4. Connect board to computer. 
5. Press power button to power on the device.
5. Launch Arduino IDE and open one of the examples.
6. Select iProtoXi Micro at Tools - Board.
7. Upload code.

#FAQ
##My Micro is not recognized by Windows.
Make sure you have installed the Windows drivers.
##My windows says drivers are not signed.
Windows 8 and newer require you to enable installing unsigned drivers. Look link below for how-to-tutorial:
http://www.addictivetips.com/windows-tips/how-to-install-any-digitally-unsigned-drivers-on-windows-8/
##Problems with uploading code:
After pressing upload at Arduino IDE red led should start blinking in few seconds.
When led stops blinking immediately force Micro to bootloader mode by launching avrdude with following settings:
avrdude -C"[avrdude.conf location]" -patmega32u4 -cavr109 -P[communication port] -b1200
Replace the [avrdude.conf location] with actual location of the file (default: [Arduino-installation-folder]\hardware\tools\avr\etc\avrdude.conf).
Replace the [communication port] with the port of the iProtoXi Micro (eg. Windows: COM4, Linux: /dev/ttyACM0, Mac: /dev/tty.usbmodem262471).
##iProtoXi Micro is not showing in my Arduino IDE board menu.
If you are using Windows make sure you have installed the drivers.
Make sure you have Classic Arduino IDE (versions 1.0.4 to 1.0.6 should work).
Merge the included hardware -folder with the Arduino's hardware folder (\Arduino-installation-folder\hardware\).