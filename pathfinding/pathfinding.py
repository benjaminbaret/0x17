"On admet pour l'instant la position de départ du robot pour avancer dans le programme"
"On va mettre une lettre sur le robot, qu'on repera puis on l'utilisera pour identifier sa position (par un 3)"

" 0 : Impassable"
"1 : impassable" \
"2 : Npurriture bloc"
" 3 : Robot"

##Surface de jeu = 3m * 2m

## 1 tick = 1 cm

## 1 chiffre de notre matrice = 10 ticks




import numpy
from heapq import *
from PIL import Image

import paho.mqtt.client as mqttClient
import time



def on_connect(client, userdata, flags, rc):
    if rc==0:
        print("client is connected")
        global connected
        connected = True
    else:
        print("client is not connected")

def on_message(client, userdata, message):
    print("Message recived"+str(message.payload.decode("utf-8")))
    print("Topic"+str(message.topic))
def heuristic(a, b):
    return (b[0] - a[0]) ** 2 + (b[1] - a[1]) ** 2


def astar(array, start, goal):
    neighbors = [(0, 1), (0, -1), (1, 0), (-1, 0)]

    close_set = set()
    came_from = {}
    gscore = {start: 0}
    fscore = {start: heuristic(start, goal)}
    oheap = []

    heappush(oheap, (fscore[start], start))

    while oheap:

        current = heappop(oheap)[1]

        if current == goal:
            data = []
            while current in came_from:
                data.append(current)
                current = came_from[current]
            return data

        close_set.add(current)
        for i, j in neighbors:
            neighbor = current[0] + i, current[1] + j
            tentative_g_score = gscore[current] + heuristic(current, neighbor)
            if 0 <= neighbor[0] < array.shape[0]:
                if 0 <= neighbor[1] < array.shape[1]:
                    if array[neighbor[0]][neighbor[1]] == 1:
                        continue
                else:
                    # array bound y walls
                    continue
            else:
                # array bound x walls
                continue

            if neighbor in close_set and tentative_g_score >= gscore.get(neighbor, 0):
                continue

            if tentative_g_score < gscore.get(neighbor, 0) or neighbor not in [i[1] for i in oheap]:
                came_from[neighbor] = current
                gscore[neighbor] = tentative_g_score
                fscore[neighbor] = tentative_g_score + heuristic(neighbor, goal)
                heappush(oheap, (fscore[neighbor], neighbor))

    return False


def getGrid(path, food): #renvoi array avec les blocs (de nourriture ou non)

    img = Image.open(path).convert('RGB')

    numpyData = numpy.asanyarray(img)
    numpyDataList = numpyData.tolist()

    ligne = [0] * 30
    grid = [ligne] * 20


    impassable = 0
    nourriture = 0
    k = 0
    l = 0

    # 2000/3000
    #150/64/64

    for k in range(20):
        for l in range(30):
            for i in range(k * 100, k * 100 + 100):
                for j in range(l * 100, l * 100 + 100):
                    if numpyDataList[i][j] == list([103, 103, 103]):
                        impassable += 1
                    if numpyDataList[i][j] == list([150, 64, 64]):
                        nourriture += 1
            if food == 1:
                if nourriture >= 1:
                    ligne[l] = 2
            if impassable >= 1:
                ligne[l] = 1
            impassable = 0
            nourriture = 0
        grid[k] = ligne
        ligne = [0] * 30

    newgrid = numpy.asarray(grid)

    return newgrid


def findPosition(myGrid, numeroCherche): # Donne les coordonnées de ce que l'on cherche (nourriture ou robot par exemple)

    position = []

    for i in range(20):
        for j in range(30):
            if myGrid[i][j] == numeroCherche:
                position.append((i, j))
    return position



def findOrder(grid, robotPosition, positionFood):

    for food in positionFood:
        astar(grid, robotPosition, positionFood[0])



connected=False
Messagereceived=False

brokerAdress = "35.180.248.89"
port = 1883
user = "0x17"
password = "WP06qS"

isEnd=1
choix = 0

client = mqttClient.Client("Client")
client.username_pw_set(user, password=password)

client.connect(brokerAdress, port=port)
client.loop_start()

time.sleep(1)



path = "carte.png"

grid = getGrid(path, 0)

gridWith = getGrid(path, 1)

print(gridWith)


robotPosition = (10, 10)





positionRobot = findPosition(gridWith, 3)
positionFood = findPosition(gridWith, 2)

#print(robotPosition, positionFood)

deplacement = astar(grid, positionFood[0], robotPosition)

x = ""
y = ""


print(deplacement)



for k in range(0, len(deplacement)-1):
    x += str(deplacement[k][0]-deplacement[k+1][0])
for k in range(0, len(deplacement)-1):
    y += str(deplacement[k][1] - deplacement[k + 1][1])






client.publish("0x17/robotX", "null")
client.publish("0x17/robotY", "null")
client.publish("0x17/robotX", x)
client.publish("0x17/robotY", y)




time.sleep(2)







#client.publish("0x17/robotY", y)