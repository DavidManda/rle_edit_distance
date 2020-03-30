import cv2

import numpy as np

def write_img_to_file(binary_img, out):
  for i in range(len(binary_img)):
    for j in range(len(binary_img[i])):
      if binary_img1[i][j] == 0:
        out.write('0')
      else:
        out.write('1')
    out.write('\n')

img1 = cv2.imread('test_image1.png',0)
img2 = cv2.imread('test_image2.png',0)
 
# scale_percent = 20 # percent of original size
# width = int(img.shape[1] * scale_percent / 100)
# height = int(img.shape[0] * scale_percent / 100)
for size in range(1,200):
  height = size
  width = size*2
  dim = (width, height)
  # resize image
  resize1 = cv2.resize(img1, dim)
  resize2 = cv2.resize(img2, dim)

  ret,binary_img1 = cv2.threshold(resize1,127,255,cv2.THRESH_BINARY)
  ret,binary_img2 = cv2.threshold(resize2,127,255,cv2.THRESH_BINARY)
  filename = 'inputs/input{}x{}.in'.format(height, width)
  out = open(filename, 'w')
  out.write('{} {}\n'.format(height*width, height*width))
  write_img_to_file(binary_img1, out)
  out.write('\n')
  write_img_to_file(binary_img2, out)
  