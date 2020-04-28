import cv2

import numpy as np
import math as math


def write_img_to_file(binary_img, out):
    for i in range(len(binary_img)):
        for j in range(len(binary_img[i])):
            if binary_img[i][j] == 0:
                out.write("0" * repeats)
            else:
                out.write("1" * repeats)
        out.write("\n")


def write_string_to_file(string, out):
    for i in range(len(string)):
        if string[i] == 0:
            out.write("0")
        else:
            out.write("1")
    out.write("\n")

def duplicate_all_elements(l):
  return [val for val in l for _ in range(15)]

img1 = cv2.imread("test_image1.png", 0)
img2 = cv2.imread("test_image2.png", 0)

img1 = cv2.imread("test_image1.png", 0)
img2 = cv2.imread("test_image2.png", 0)
ret, binary_img1 = cv2.threshold(img1, 127, 255, cv2.THRESH_BINARY)
ret, binary_img2 = cv2.threshold(img2, 127, 255, cv2.THRESH_BINARY)
# binary_img1 = cv2.resize(binary_img1, (200, 100))
# binary_img2 = cv2.resize(binary_img2, (200, 100))

# binary_img1 = [elem for sublist in binary_img1 for elem in sublist]
# binary_img2 = [elem for sublist in binary_img2 for elem in sublist]
# binary_img1 = binary_img1[10000:30000]
# binary_img2 = binary_img2[10000:30000]
# binary_img1 = duplicate_all_elements(binary_img1)
# binary_img2 = duplicate_all_elements(binary_img2)

# fixed_size = 2000
# fixed_string = binary_img2[:fixed_size]
# for size in range(50, 15001, 50):
#     print(size)
#     string1 = binary_img1[:size]
#     string2 = binary_img2[:size]
#     filename = "inputs/input{}.in".format(size)
#     out = open(filename, "w")
#     out.write("{} {}\n".format(size, size))
#     write_string_to_file(string1, out)
#     out.write("\n")
#     write_string_to_file(string2, out)
repeats = 1
for size in range(1,100):
  height = size
  width = size * 2
  dim = (width, height)
  res_img1 = cv2.resize(binary_img1,dim)
  res_img2 = cv2.resize(binary_img2,dim)
  filename = "inputs/input{}x{}.in".format(height, width)
  out = open(filename, "w")
  out.write("{} {}\n".format(width * height * repeats, width * height *repeats))
  write_img_to_file(res_img1, out)
  out.write("\n")
  write_img_to_file(res_img2, out)

