import cv2
import numpy as np
import hid
import time
import serial

ser = serial.Serial('/dev/cu.usbserial-1130', 9600)
gamepad = hid.device()
gamepad.open(0x054c, 0x05c4)
gamepad.set_nonblocking(True)

# default mode
def freeMotion():  # hit select to activate
    ser.write(b'I')
    cont = True
    while cont:
        snes = gamepad.read(64)
        if snes:
            if snes[5] == 72:
                ser.write(b'A')
                print('A')
            elif snes[5] == 40:
                ser.write(b'B')
                print('B')
            if snes[8] == 255:
                ser.write(b'Z')
                print('ZL')
            elif snes[9] == 255:
                ser.write(b'X')
                print('ZR')
            if snes[6] == 1:
                ser.write(b'L')
                print('L')
            elif snes[6] == 2:
                ser.write(b'R')
                print('R')
            if snes[2] <= 120:
                ser.write(b'U')
                print('UL')
            elif snes[2] >= 135:
                ser.write(b'S')
                print('DL')
            if snes[4] <= 120:
                ser.write(b'C')
                print('UR')
            elif snes[4] >= 135:
                ser.write(b'D')
                print('DR')
            if snes[6] == 32:  # start
                print('start')
                cont = False
                return True
            if snes[6] == 16:  # start
                ser.write(b'I')
            if snes[7] == 1:  # home -> exit
                cont = False
                return False


def ballByColor():  # hit start to activate
    ser.write(b'J')
    cont = True
    while cont:
        snes = gamepad.read(64)
        if snes:
            # red
            if snes[5] == 72:
                print('Grabbing Red...')
                capturePos(1)
                time.sleep(10)
            # yellow
            elif snes[5] == 40:
                print('Grabbing Yellow...')
                capturePos(2)
                time.sleep(10)
            # blue
            elif snes[5] == 136:
                print('Grabbing Blue...')
                capturePos(3)
                time.sleep(10)
            elif snes[6] == 1:
                ser.write(b'T')
                print('L Pressed: Testing Grabbing Motion...')
                time.sleep(10)
            # free motion
            if snes[6] == 16:  # select
                print('select')
                cont = False
                return True
            if snes[6] == 32:  # start
                ser.write(b'J')
            if snes[7] == 1: # home -> exit
                cont = False
                return False

# create capture object and pass camera feed
vid = cv2.VideoCapture(0, apiPreference=cv2.CAP_ANY, params=[
    cv2.CAP_PROP_FRAME_WIDTH, 640,
    cv2.CAP_PROP_FRAME_HEIGHT, 480])
# color in position booleans

# validate camera is connected and feeeding input.
if not vid.isOpened():
    print("Cannot open camera")
    exit()


def capturePos(color):
    # Open while loop captures frame by frame. boolean variable ret indicates if there was a return
    ret, frame = vid.read()
    # if frame is read correctly ret is True
    if not ret:
        print("No Video Frames Received- Exiting now..")
    print('frames read color is ' + str(color))
    # HSV format. define upper and lower ranges of desired color
    # array values are HSV respectively
    redLower = np.array([0, 60, 60])
    redUpper = np.array([20, 255, 255])
    blueLower = np.array([99, 100, 15])
    blueUpper = np.array([140, 255, 255])
    yellowLower = np.array([20, 20, 45])
    yellowUpper = np.array([40, 250, 255])

    # convert video to HSV format
    hsvCap = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    # create separate regions from initial frame
    # dimensions = start row:end row, starting col: end col, end pixel row:column for each region
    position1 = hsvCap[125:300, 0:212]
    position2 = hsvCap[125:300, 213:424]
    position3 = hsvCap[125:300, 425:640]

    # position 1 masks
    pos1RMask = cv2.inRange(position1, redLower, redUpper)
    pos1YMask = cv2.inRange(position1, yellowLower, yellowUpper)
    pos1BMask = cv2.inRange(position1, blueLower, blueUpper)

    # position 2 masks
    pos2RMask = cv2.inRange(position2, redLower, redUpper)
    pos2YMask = cv2.inRange(position2, yellowLower, yellowUpper)
    pos2BMask = cv2.inRange(position2, blueLower, blueUpper)

    # position 3 masks
    pos3RMask = cv2.inRange(position3, redLower, redUpper)
    pos3YMask = cv2.inRange(position3, yellowLower, yellowUpper)
    pos3BMask = cv2.inRange(position3, blueLower, blueUpper)

    # thresholding, weights and color booleans here
    # check thresholded video feeds for number of pixels that are white (aka desired color)- store count
    # if count reaches certain weight- tbd- set boolean to true
    # count variables - color by position
    rP1 = (int)(np.sum(np.nonzero(pos1RMask)))
    bP1 = (int)(np.sum(np.nonzero(pos1BMask)))
    yP1 = (int)(np.sum(np.nonzero(pos1YMask)))
    rP2 = (int)(np.sum(np.nonzero(pos2RMask)))
    bP2 = (int)(np.sum(np.nonzero(pos2BMask)))
    yP2 = (int)(np.sum(np.nonzero(pos2YMask)))
    rP3 = (int)(np.sum(np.nonzero(pos3RMask)))
    bP3 = (int)(np.sum(np.nonzero(pos3BMask)))
    yP3 = (int)(np.sum(np.nonzero(pos3YMask)))

    # O = Pos1 T = Pos2 E = Pos3
    # red
    print('if statements reached')
    if (color == 1):
        print('red')
        if (rP1 > 750000 and rP1 > rP2 and rP1 > rP3):
            ser.write(b'O')
            print('red in pos 1')
            print(rP1)
        if (rP2 > 750000 and rP2 > rP1 and rP2 > rP3):
            ser.write(b'T')
            print('red in pos 2')
            print(rP2)
        if (rP3 > 750000 and rP3 > rP1 and rP3 > rP2):
            ser.write(b'E')
            print('red in pos 3')
            print(rP3)
    # blue
    elif (color == 3):
        print('blue')
        if (bP1 > 750000) and bP1 > bP2 and bP1 > bP3:
            ser.write(b'W')
            print('blue in pos 1')
            print(bP1)
        if (bP2 > 750000 and bP2 > bP1 and bP2 > bP3):
            ser.write(b'V')
            print(bP2)
            print('blue in pos 2')
        if (bP3 > 750000 and bP3 > bP1 and bP3 > bP2):
            ser.write(b'C')
            print('blue in pos 3')
            print(bP3)
    # yellow
    elif (color == 2):
        print('yellow')
        if (yP1 > 750000 and yP1 > yP2 and yP1 > yP3):
            ser.write(b'H')
            print('yellow in pos 1')
            print(yP1)
        if (yP2 > 750000 and yP2 > yP1 and yP2 > yP3):
            ser.write(b'G')
            print('yellow in pos 2')
            [print(yP2)]
        if (yP3 > 500000 and yP3 > yP1 and yP3 > yP2):
            ser.write(b'S')
            print('yellow in pos 3')
            print(yP3)


def main():  # flips between free motion and ball by color
    cont = True
    while cont:
        cont = freeMotion()
        cont = ballByColor()


# When everything done, release the capture
main()
vid.release()
cv2.destroyAllWindows()