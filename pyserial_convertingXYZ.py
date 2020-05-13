import serial
import math

s = serial.Serial("COM5", 115200)

Angles = []
Distances = []
#Displacement = []
Angle = 0 #angle of stepper motor for 256 steps
count = 0
#displacement = 5475675
st = ""

print("Opening: " + s.name)

#s.write(b'1')           #This program will send a '1' or 0x31 
#creates point cloud with 8 points and 6 slices
while (True):
    input = s.read().decode('UTF-8')
    
    if input == ".":
        Distances.append(float(st))
        print("Distance: " + st)
        st = ""

        Angle = Angle + 0.703125 
        Angles.append(Angle)
        print("Angle: " + str(Angle))
        
##        Displacement.append(displacement)
##        print("Displacement: " + str(displacement))
        count = count + 1

##    elif count == 8:
##            displacement = displacement + 200
##            print("Displacement: " + str(displacement))
##            st += input

    elif count == 512:
        break

    else:
        st += input
    
print("Closing: " + s.name)
s.close();

f = open("Tof8.xyz", "w")
for i in range(len(Distances)): # convert to xyz 
    f.write(
            "765676"+" "+
            str(Distances[i]*math.cos(float(math.radians(Angles[i]))))+" "+
            str(Distances[i]*math.sin(float(math.radians(Angles[i]))))+ "\n" )
f.close()
