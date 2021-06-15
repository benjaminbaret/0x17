#OpenCV Team 0x17

#Importation des packages
import cv2
import numpy as np

#Importation de l'image d'origine
img = cv2.imread("data\warp1.png")

#Définition des dimensions et points des images
width,height = 1200,800
pt1 = np.float32([[239,17],[1160,220],[37,540],[1022,778]])
pt2 = np.float32([[0,0],[width,0],[0,height],[width,height]])

#Matrice de conversion
matrix = cv2.getPerspectiveTransform(pt1,pt2)

#Utilisation de la fonction OpenCV de redressement d'image
warp = cv2.warpPerspective(img,matrix,(width,height))

#Output
cv2.imshow("Output",warp)
cv2.waitKey(0)
