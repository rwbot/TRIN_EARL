#import the necessary packages
import cv2
import numpy as np
import sys
import os, shutil

def find_if_close(cnt1,cnt2):
    row1,row2 = cnt1.shape[0],cnt2.shape[0]
    for i in xrange(row1):
        for j in xrange(row2):
            dist = np.linalg.norm(cnt1[i]-cnt2[j])
            if abs(dist) < 50 :
                return True
            elif i==row1-1 and j==row2-1:
                return False

def filter_image(frame):
    blurred = cv2.GaussianBlur(frame,(9,9),0)

    hsv= cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)

    lower_orange=np.array([0,170,0])
    upper_orange=np.array([15,255,255])
    mask = cv2.inRange(hsv,lower_orange,upper_orange)

    lower_white = np.array([0, 0, 210])
    upper_white = np.array([255, 210, 255])
    white_mask = cv2.inRange(hsv, lower_white, upper_white)
    white_edges = cv2.Canny(white_mask, 10, 30, 3)

    hsvMask = mask.copy()
 
    # white_points = cv2.findNonZero(mask)
    # edges = cv2.Canny(mask, 100, 200, 3)
    # contours,hierarchy = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    rectangle_list = []
    rectangle_map = {}

    # for cnt in contours:
    #     x,y,w,h = cv2.boundingRect(cnt)
    #     cv2.rectangle(res,(x,y),(x+w,y+h),(0,0,0),-1)

    # mask = cv2.cvtColor(res, cv2.COLOR_RGB2GRAY)

    # res[mask = 255] = [0, 255, 0]
    
    structuringElement = cv2.getStructuringElement(cv2.MORPH_RECT, (70, 70))
    structuringElement2 = cv2.getStructuringElement(cv2.MORPH_RECT, (50, 50))

    lineElement = cv2.getStructuringElement(cv2.MORPH_RECT, (1, 90))
    mask = cv2.morphologyEx( mask, cv2.MORPH_CLOSE, lineElement)
    white_edges = cv2.bitwise_and(white_edges, white_edges, mask=~mask)
    
    laneLines= cv2.HoughLinesP(white_edges, rho=1, theta=np.pi/360, threshold=100, minLineLength=60, maxLineGap=25)

    # lines = cv2.HoughLinesP(mask, rho=1, theta=np.pi / 180, threshold=100, minLineLength=150, maxLineGap=100)

    # mask = cv2.morphologyEx( mask, cv2.MORPH_CLOSE, structuringElement )

    res = cv2.bitwise_and(frame, frame, mask=~mask)
    
    if laneLines is not None:
        for l in laneLines[0]:
            cv2.line(mask, (l[0], l[1]), (l[2], l[3]), (255, 255, 255), 4)

    
	# NEARBY_RANGE = 100
    # LENGTH = len(contours)
    # status = np.zeros((LENGTH,1))

    # for i,cnt1 in enumerate(contours):
    #     x = i    
    #     if i != LENGTH-1:
    #         for j,cnt2 in enumerate(contours[i+1:]):
    #             x = x+1
    #             dist = find_if_close(cnt1,cnt2)
    #             if dist == True:
    #                 val = min(status[i],status[x])
    #                 status[x] = status[i] = val
    #             else:
    #                 if status[x]==status[i]:
    #                     status[x] = i+1

    # unified = []
    # maximum = int(status.max())+1
    # for i in xrange(maximum):
    #     pos = np.where(status==i)[0]
    #     if pos.size != 0:
    #         cont = np.vstack(contours[i] for i in pos)
    #         hull = cv2.convexHull(cont)
    #         unified.append(hull)

    # cv2.drawContours(img,unified,-1,(0,255,0),2)
    # cv2.drawContours(thresh,unified,-1,255,-1)


        # for key, val in rectangle_map:
        #     if (abs((x + w) - (key[0] + key[2])) < NEARBY_RANGE and abs((y+h) - (key[1] + key[3])) < NEARBY_RANGE):
        #         smallest_x = x if x < key[0] else key[0]
        #         smallest_y = y if y < key[1] else key[1]
        #         largest_x = (x + w) if (x + w) > (key[0] + key[2]) else (key[0] + key[2])
        #         largest_y = (y + h) if (y + h) > (key[1] + key[3]) else (key[1] + key[3])
        #         key = (smallest_x, smallest_y, largest_x, largest_y)

        #         key = (x < key[0] ? x : key[0], y < key[1] ? y : key[1]) 
        #     break
        #rectangle_list.append((x,y,w,h))

   # for r in rectangle_list:


    # if white_points is not None:
        # x,y,w,h = cv2.boundingRect(white_points)
        # rect = cv2.minAreaRect(white_points)
        # box = cv2.cv.BoxPoints(rect)
        # box = np.int0(box)
        # (x,y),radius = cv2.minEnclosingCircle(white_points)
        # center = (int(x),int(y))
        # radius = int(radius)
        #cv2.circle(res,center,radius,(0,255,0),5)
        # cv2.drawContours(res,[box],0,(0,0,255),2)
        # cv2.rectangle(res,(x,y),(x+w,y+h),(255,255,255),5)
        # cv2.rectangle(mask,(x,y),(x+w,y+h),(255,255,255),5)
    mask = cv2.cvtColor(mask, cv2.COLOR_GRAY2BGR)
    hsvMask = cv2.cvtColor(white_edges, cv2.COLOR_GRAY2BGR)

    side_comp = cv2.vconcat([res, mask, hsvMask, frame])
    height, width, _ = side_comp.shape
    side_comp_res = cv2.resize(side_comp, (height / 2, width / 2))
 

    # res2, contours, hierarchy = cv2.findContours()

    return side_comp_res

if (len(sys.argv) < 2):
    print('Provide video file')
    sys.exit()

cap = cv2.VideoCapture(sys.argv[1])

if (cap.isOpened()== False): 
  print("Error opening video stream or file")
 
# Read until video is completed
every_other = False

while(cap.isOpened()):
  # Capture frame-by-frame
  ret, frame = cap.read()
  if ret == True:

    if cv2.waitKey(20) & 0xFF == ord('w'):
      continue

    if every_other:
        every_other = not every_other
        continue

    # Display the resulting frame
    # cv2.imshow('Frame',frame)
    res = filter_image(frame)
    cv2.imshow('frame', res)

    # Press Q on keyboard to  exit
    if cv2.waitKey(20) & 0xFF == ord('q'):
      break
 
  # Break the loop
  else: 
    break
 
# When everything done, release the video capture object
cap.release()
 
# Closes all the frames
cv2.destroyAllWindows()



    #res = cv2.bitwise_and(frame,frame, mask =mask)
 
#     #cv2.imshow(wnd, res)

#     res[mask == 255] = [0, 255, 0]
#     cv2.imshow(wnd, res)


 
