import cv2
import numpy as np

def empty(a):
    pass

#Ne pas oublier convention XY (origine en haut à gauche)
# print(img.shape) -> Hauteur x Largeur x BGR
#
# 1 - Importation des images
# 2 - Quelques fonctions de coloration et de transformation + numpy
# 3 - Redimensionner et rogner une image
# 4 - Formes coloration de forme & texte
# 5 - Vue d'oiseau (Wrap perspective)
#
#
#
#
#
#
#
#
#
#
#
# {} []


print("CV2 importé")

#importation de l'image
img = cv2.imread("data/I2FDT-PLG-10.png")
imgResized = cv2.resize(img,(750,500))
horizontal = np.hstack((imgResized,imgResized))
vertical = np.vstack((imgResized,imgResized))

#Création des Sliders
cv2.namedWindow("Trackbars")
cv2.resizeWindow("Trackbars",640,240)
cv2.createTrackbar("Hue Min","Trackbars",0,179,empty)
cv2.createTrackbar("Hue Max","Trackbars",179,179,empty)
cv2.createTrackbar("Saturation Min","Trackbars",0,255,empty)
cv2.createTrackbar("Saturation Max","Trackbars",255,255,empty)
cv2.createTrackbar("Value Min","Trackbars",0,255,empty)
cv2.createTrackbar("Value Max","Trackbars",255,255,empty)


#forme = np.zeros((512,512,3),np.uint8)
while True:
    imgHSV = cv2.cvtColor(imgResized,cv2.COLOR_BGR2HSV)

    h_min = cv2.getTrackbarPos("Hue Min","Trackbars")
    h_max = cv2.getTrackbarPos("Hue Max","Trackbars")
    s_min = cv2.getTrackbarPos("Saturation Min","Trackbars")
    s_max = cv2.getTrackbarPos("Saturation Max","Trackbars")
    v_min = cv2.getTrackbarPos("Value Min","Trackbars")
    v_max = cv2.getTrackbarPos("Value Max","Trackbars")


    print(h_min)
    print(h_max)
    print(s_min)
    print(s_max)
    print(v_min)
    print(v_max)





#width,height = 250,350
#pts1 = np.float32([[111,219],[287,188],[154,482],[352,440]])
#pts2 = np.float32([[0,0],[width,0],[0,height],[width,height]])
#matrix = cv2.getPerspectiveTransform(pts1,pts2)
#imgOutpout = cv2.warpPerspective(imgResized,matrix,(width,height))

#cv2.line(forme,(0,0),(forme.shape[1],forme.shape[0]),(0,255,255),3)
#cv2.rectangle(forme,(0,0),(300,230),(0,255,0),cv2.FILLED)
#cv2.circle(forme,(250,250),30,(255,255,100),5)
#cv2.putText(forme,"GREG",(300,200),cv2.FONT_HERSHEY_COMPLEX,0.5,(0,150,0),2)

#forme[200:300]=255,0,0

#imgCropped = imgResized[0:200,200:750]


#print(img.shape)

#kernel = np.ones((5,5),np.uint8)

#imgCanny = cv2.Canny(img,100,100)
#imgDialation = cv2.dilate(imgCanny,kernel,iterations=1)
#imgEroded = cv2.erode(imgDialation,kernel,iterations=1)

#print(forme.shape)

    cv2.imshow("Image",imgResized)
    cv2.imshow("ImageHSV",imgHSV)
#cv2.imshow("Imagestack",horizontal)
#cv2.imshow("im",vertical)

    cv2.waitKey(1)