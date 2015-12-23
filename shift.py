import sys
import time

ZeitAktuell = time.time()
a = b'\x98\t\xe8\x1c\n'
b = b'\xbe\x0b\xe8\x1c\n'

d =b'l\n'
e = b'\xe8\x1c\n'
f = b'\x7f\n'

def shift(c):
    ist = (c[0]|c[1] << 8) /100
    soll = (c[2]|c[3] << 8) /100
    #print(ist)
    #print(soll)
    #print("ist: %f, soll: %f" % (ist, soll))
    in_file = open("lala2", "w")
    # neue Werte anhaengen
    in_file.write("%.2f\n%.2f\n%.2f\n" % (float(ist), float(soll), float(ZeitAktuell)))
    # Datei schliessen und Zaehler erhoehen
    in_file.close()

    print("%.2f\n%.2f\n%.2f\n" % (float(ist), float(soll), float(ZeitAktuell)))


shift(a)