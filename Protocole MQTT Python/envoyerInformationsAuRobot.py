## ce code permet d' envoyer et recevoir des informations


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

time.sleep(5)

while isEnd:
    print("CHOIX : 1(distance) / 2(tourner degré) : ")
    choix = input()
    choix = int(choix)
    if choix == 1:
        print("Vitesse ? ")
        vitesse = input()
        vitesse = int(vitesse)
        client.publish("0x17/robotX", "null")
        client.publish("0x17/robotX", vitesse)
        print("Message published")
    elif choix == 2:
        print("Degre ? ")
        degre = input()
        degre = int(degre)
        client.publish("0x17/tourner", "null")
        client.publish("0x17/tourner", degre)
        print("Message published")

while connected != True:
    time.sleep(0.2)
while Messagereceived != True:
    time.sleep(0.2)



##client.publish("0x17", "essai python OK")
##print("Message published")
