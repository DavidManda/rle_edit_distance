import cv2

import numpy as np
import math as math


def write_img_to_file(binary_img, out):
    for i in range(len(binary_img)):
        for j in range(len(binary_img[i])):
            if binary_img[i][j] == 0:
                out.write("0")
            else:
                out.write("1")
        out.write("\n")


def write_string_to_file(string, out):
    for i in range(len(string)):
        if string[i] == 0:
            out.write("0")
        else:
            out.write("1")
    out.write("\n")


img1 = cv2.imread("test_image1.png", 0)
img2 = cv2.imread("test_image2.png", 0)

img1 = cv2.imread("test_image1.png", 0)
img2 = cv2.imread("test_image2.png", 0)
ret, binary_img1 = cv2.threshold(img1, 127, 255, cv2.THRESH_BINARY)
ret, binary_img2 = cv2.threshold(img2, 127, 255, cv2.THRESH_BINARY)
binary_img1 = cv2.resize(binary_img1, (200, 100))
binary_img2 = cv2.resize(binary_img2, (200, 100))

binary_img1 = [elem for sublist in binary_img1 for elem in sublist]
fixed_size = 2000

for size in range(50, 15001, 50):
    print(size)
    string = binary_img1[:size]
    filename = "inputs/input{}.in".format(size)
    out = open(filename, "w")
    out.write("{} {}\n".format(size, fixed_size))
    write_string_to_file(string, out)
    out.write("\n")
    write_img_to_file(binary_img2, out)
