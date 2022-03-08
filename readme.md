# Dispense steppermotor controller
control a steppermotor to dispense fluids from syringes

4 modes

* Single Mode (dispensing on button, stop + retract after steps, selectable number of steps)
* Repeat Mode (continue dispense on pressed button, retract after button release, selectable number of steps)
* Manual Mode (control direction of stepper motor, selectable speed)
* External Mode (contro over usb serial device)

## Configuration
### By Button
push mode button, selected mode led will lit and settings can be changed by up/down button and led will switch to bar mode showning actual setting (0..3)
### By serial commands
all commands can change to read mode by adding a ?
* *IDN 
> read device version
* SETup:FACtory:DEFault
> set config to factory defaults
* SETup:MODE
> get / set Mode (0 .. 2)
* SETup:SINGle:INDex
> get / set single mode index (0..3)
* SETup:SINGle:VALue#
> get / set single mode value (# is 0..3) from 0 to 4096 steps
* SETup:REPeat:INDex
> get / set repeat mode index (0..3)
* SETup:REPeat:VALue#
> get / set repeat mode value (# is 0..3) from 0 to 4096 steps
* SETup:MANual:INDex
> get / set manual mode index (0..3)
* SETup:MANual:VALue#
> get / set manual mode value (# is 0..3) from 0 to 1.0 speed
* SETup:EXTernal:SPeed
> get / set speed value (0..1.0) for control over serial
* SETup:EXTernal:STeps
> get / set steps value (0..4096) for control over serial
* SETup:EXTernal:DIR
> get / set direction (-1, 0, 1) for control over serial, this will start move if direction not 0. set to 0 after steps
# Programming
## BootLoader
change into uf2-samdx1
> make BOARD=dispstepper jlink-flash
* now LED D2 should blink
## Application
### via uf2 bootloader
* double tap reset to enter boot loader
* led2 will start blinking
* the board will register as a mass storage device
* put the uf2 update file onto the mass storage and firware update will start
