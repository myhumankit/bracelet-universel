#!/usr/bin/env python
# -*- coding: utf-8 -*-

# librairies
import sys
import time
import serial

# connexion au bracelet
bracelet = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

# nombre de données enregistrées
data_number = 1000

# ouverture du fichier en écriture
file_name = str(int(time.time()))
if len(sys.argv) == 2:
    file_name = file_name + '_' + sys.argv[1]
file_name = file_name + ".txt"
data = open('samples/' + file_name, "w")

# on lance le chrono
start_time = time.time()

# réception des données
for i in range(data_number):
    signal = bracelet.readline().decode('ascii')
    data.write(signal)
    #print(signal)

# on arrête le chrono
stop_time = time.time()

# on écrit quelques infos à la fin du fichier
data.write("# duration: " + str((stop_time - start_time)) + 's\n')
data.write("# step duration: " + str((stop_time - start_time) / (data_number - 1)) + 's\n')

# fermeture du fichier
data.close()

# fermeture de la connexion
bracelet.close()
