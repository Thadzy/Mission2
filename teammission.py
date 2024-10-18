import serial
import time 

arduino = serial.Serial(port="COM5", baudrate=9600, timeout=0.1)

def write_read(x): 
    arduino.write(bytes(x, "utf-8")) 
    time.sleep(0.05) 
    return arduino.readline() 

if __name__ == "__main__":
    while True: 
        num = input("Enter a number: ")
        value = write_read( num ) 
        print( value )  