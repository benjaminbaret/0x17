## ce code permet d' envoyer et recevoir des informations


import paho.mqtt.client as mqttClient
import time

def on_publish(mosq, userdata, mid):
    mosq.disconnect()


connected=False
Messagereceived=False

brokerAdress = "35.180.248.89"
port = 1883
user = "0x17"
password = "WP06qS"

isEnd=1

client = mqttClient.Client("Client")
client.username_pw_set(user, password=password)

client.connect(brokerAdress, port=port)
client.loop_start()

time.sleep(2)

f=open("trouNoir.jpg", "rb") #3.7kiB in same folder
fileContent = f.read()
byteArr = bytearray(fileContent)
client.publish("0x17/image",byteArr,0)


client.loop_forever()



##client.publish("0x17", "essai python OK")
##print("Message published")
