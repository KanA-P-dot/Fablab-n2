import serial

# Initialiser la communication série (adapter le port selon votre configuration)
ser = serial.Serial('COM3', 9600)

avis_list = []

while True:
    if ser.in_waiting > 0:
        # Lire les données envoyées par l'Arduino
        avis = ser.readline().decode('utf-8').rstrip()
        avis_list.append(avis)
