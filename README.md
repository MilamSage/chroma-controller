# chroma-controller
Video color classification and robotic arm controller

In order to run this program you will need the Adeept 5-DOF Robot Arm Kit and the Arduino IDE which can be found at:
https://www.arduino.cc/en/software

There is a requirements.txt file that contains all necessary dependencies, however, additional libraries will need to be included through the Arduino IDE interface.

To do this open the Arduino IDE, navigate to "Sketch" -> "Include Library" -> "Manage Libraries".

    -Please search for the following libraries and click "install" once you have included them:
        -Adafruit GFX 1.11.5
        -U8glib 1.19.1
        -Servo 1.2.1
        -Wire 1.0

--------------------------------------------------------------------------------------------------------------------------------------------------------------------

Robotic Arm  capable of two primary modes:

Ball By Color:

    General function:

        Video input is analyzed to determine the presence of colors in three predefined positions. User determines which color ball to select, and 
        the arm locates and grabs the proper ball.

    Methodology:

        Using the video capturing capabilities of the Open-cv python library and the matrix manipulation capabilities of Numpy python library, a
        webcam was mounted to an assembled base underneath the robotic arm. Video feed was split into three sections representing the three 
        predefined positions the balls could be placed in. The feed was converted into HSV values and I established the upper and lower bounds of
        the desired colors. Using these limits, masks in each position for each color were created. I then used opencv's thresholding function
        to set the value of pixels within proper color ranges to 255 (or white). The present pixel counts are then tallied, and conditions are 
        set to log/output "x color is present in x position". This allows for adjustment to account for noise as the setup is unfortunately
        conducive to background interference.
        The user then presses a button representing the color they want selected and if the color is logged as present, the Arm grabs the ball 
        from the proper position, displays it, and places it back down.


Free Motion:

    General Function:

    A controller is polled for current state of buttons and translates that data into servo movement. This allows for specialized mapping of servo
    movement to whatever button desired.

    Methodology:

        Using pySerial and HIDAPI libraries, controller state data is strolled into an array. This data is then output to the microcontroller as
        bytes and those bytes are decoded into controller input. Desired signal is then sent to the microcontroller as ASCII character bytes via
        serial output. The serial input is read and the servos are rotated based on the number of occurences received. Said servo angle rotation 
        are predetermined and the continuous polling allows for a dynamic where the arm will continously move as long as the button is held. It is 
        essentially taking multiple small steps, at predetermined angles, based on whatever button is pressed. This allowed for full manipulation
        of every joint of the arm. The only limitation in movement is that it cannot be continuously rotated at the base or the servo cords will
        tangle. 


Code Dependencies:
    -Located in Requirements.txt
    
Hardware Requirements:
    -Adeept 5-DOF Robot Arm kit:
        -1 Set Acrylic Plates; 1x Bearing
        Adeept Robotic Arm Driver Board
        0.96'' OLED Display; 4x Suction Cup
        6 Servo(MG90S)
        Battery Holder
        Cross Socket Wrench
        Cross Screwdriver
        Winding Pipe; 1x Micro USB Cable
        Other Necessary Accessories(Wires, Nuts, Screws, Copper Standoffs, etc.)
