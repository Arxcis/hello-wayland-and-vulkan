#!/usr/bin/python3

WIDTH=2560
HEIGHT=1440

bgra = []

for w in range(WIDTH):
    for h in range(HEIGHT):
                #0    0    0    a
        bgra += [255, 255, 255, 255]

buffer = b''.join([byte.to_bytes(1, byteorder='little') for byte in bgra])

with open('./bin/pool.bin', 'wb') as bufferFile:
    bufferFile.write(buffer)
