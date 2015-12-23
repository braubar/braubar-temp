import serial
import time

Dauer = 10000  # in Minuten
Jump = 0 # Für Wiedereinstieg Zeit in Minuten
ZeitStart = time.time() - 60 * Jump
ZeitAktuell = time.time()


# pfad = "/Users/Dandelo/Dropbox/Brau/Arduino/Runde5/Temperatur_Gas/TempSerial"  # anpassen!!
pfad = "/home/oli/Dokumente/Studium/Braubar/code/Temperatur_Gas/TempSerial"  # anpassen!!
dateiname = "Temperaturverlauf.txt"
datei = pfad + '/' + dateiname
# seriellen Port angeben
# 641: noch funktionierender USB Ausgang
# ser = serial.Serial('/dev/tty.usbmodem641', 9600)
ser = serial.Serial('/dev/ttyACM0', 9600)
print (ser.name)

# leere Datei erstellen
out_file = open(datei, "w")
out_file.close()

i = time.time() - ZeitStart
while i < Dauer * 60:
    # Werte aus serieller Uebertragung

    #line = ser.readline()
    #ist = (line[0] | line[1] << 8) / 100
    #soll = (line[2] | line[3] << 8) / 100
    # print("ist: %f, soll: %f" % (ist, soll))

    line = ser.read(6)
    ist = (line[0] | line[1] << 8) / 100
    soll = (line[2] | line[3] << 8) / 100
    print("ist: %f, soll: %f" % (ist, soll))
    print(line)

    #ist = ser.readline()
    #soll = ser.readline()
    #print(float(ist))
    #print(float(soll))



    # aktuelle Zeit & Ausgabe im Terminal
    ZeitAktuell = (time.time() - ZeitStart)
    print("Laufzeit: %f Minuten" % (ZeitAktuell/60))
    print(' ')

    # append!
    # vorherige Werte zwischenspeichern
    out_file = open(datei,  'r')
    save = out_file.read()
    out_file.close()

    in_file = open(datei, "w")
    in_file.write(save)
    # neue Werte anhaengen
    in_file.write("%.2f\n%.2f\n%.2f\n" % (float(ist), float(soll), float(ZeitAktuell)))
    # Datei schliessen und Zaehler erhoehen
    in_file.close()
    i = time.time() - ZeitStart


# Aufzeichnung beenden
ser.close()
quit()
