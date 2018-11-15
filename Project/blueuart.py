#!/usr/bin/env python3
from PIL import Image
import serial, time, argparse, os, random, PIL

size = 480, 272

direction = {
    'u': 1,
    'up': 1,
    'd': 2,
    'down': 2,
    'r': 3,
    'right': 3,
    'l': 4,
    'left': 4
}

def send_picture(image_loc):
    print('sending', image_loc)
    im = Image.open(image_loc)
    im = im.resize(size, PIL.Image.BICUBIC)
    
    elapsed = -time.perf_counter()
    for i, px in enumerate(im.getdata()):
#        time.sleep(0.0002)
#        if i % (480*20) == 0:
#            read = ser.read(4)
#            print(read, i//480)
#            if read != b'line':
#                print('line failure')
#                exit(1)
        rg =   px[0] & 0b11111000 
        rg |= (px[1] & 0b11100000) >> 5
        gb =  (px[1] & 0b00011100) << 3
        gb |= (px[2] & 0b11111000) >> 3
    #    print('%02x%02x' % (rg, gb), end=' ')
        ser.write(bytes([rg, gb]))
    elapsed += time.perf_counter()
    print('time elapsed', elapsed)

parser = argparse.ArgumentParser()
parser.add_argument('-c', '--count', type=int, default=1, help='number of pictures to randomly select')
parser.add_argument('-f', '--folder', help='folder to pick random images from')
parser.add_argument('-i', '--image', default='framed.bmp', help='single image to send')
parser.add_argument('-d', '--direction', default='u', help='sets the orientation mode, up down.')
args = parser.parse_args()

ser = serial.Serial('/dev/cu.Embedded2-DevB', 115200)  # open serial port
print(ser.name)         # check which port was really used
os.system('stty -f /dev/cu.Embedded2-DevB 115200')
print('set stty speed')

print('waiting for greet')
greet = ser.read(len(b'hello world!\n\r'))
print(greet)
if greet != b'Hello World!\n\r':
    print('Wrong handshake')
    exit(1)
print('greeted')

print('responding')
ser.write(b'hello device')
time.sleep(1)
ser.write(b'go to next 1')


if args.count and args.folder:
#    ser.write(bytes([direction[args.direction]]))
    for i in range(args.count):
        send_picture(args.folder + '/' + random.choice(os.listdir(args.folder)))
else:
#    ser.write(bytes([direction[args.direction]]))
    send_picture(args.image)

time.sleep(3)
ser.close()
    
print('finished')
