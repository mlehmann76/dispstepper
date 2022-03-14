# Dispense steppermotor controller
control a steppermotor to dispense fluids from syringes

![3d view](/pics/3d_view.png)


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
| Command | Range | Description |
| :-- | :-- | :-- |
| *IDN | | read device version |
| SETup:FACtory:DEFault| 1 |set config to factory defaults |
| SETup:MODE | (0 .. 3) | get / set Mode / Single=0/Repeat=1/Manual=2/External=3 |
| SETup:SINGle:INDex |(0..3) | get / set single mode index |
| SETup:SINGle:VALue# |(0..4096)| get / set single mode value (# is 0..3)  steps |
| SETup:REPeat:INDex |(0..3)| get / set repeat mode index  |
| SETup:REPeat:VALue# |(0..4096)|get / set repeat mode value (# is 0..3) steps |
| SETup:MANual:INDex |(0..3)| get / set manual mode index  |
| SETup:MANual:VALue# |(0..1.0)| get / set manual mode value (# is 0..3) speed |
| SETup:EXTernal:SPeed |(0..1.0)| get / set speed value  for control over serial |
| SETup:EXTernal:STeps |(0..4096)| get / set steps value for control over serial |
| SETup:EXTernal:DIR |(-1, 0, 1)| get / set direction  for control over serial, this will start move if direction not 0. set to 0 after steps |
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

# License
See Microchip / Atmel / Arm source code files for license.

The new code is licensed under MIT.
