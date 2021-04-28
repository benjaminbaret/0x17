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

client = mqttClient.Client("Client")
client.username_pw_set(user, password=password)
client.on_connect = on_connect
client.connect(brokerAdress, port=port)
client.loop_start()
client.subscribe("0x17")

time.sleep(5)

while isEnd:
    print("Distance souhaitée : ")
    vitesse = input()
    if vitesse == -1:
        isEnd = 0
    client.publish("0x17/distanceDemandee", vitesse)
    print("Message published")



while connected != True:
    time.sleep(0.2)
while Messagereceived != True:
    time.sleep(0.2)

##client.publish("0x17", "essai python OK")
##print("Message published")
