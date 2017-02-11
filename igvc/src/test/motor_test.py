import time
import serial

# configure the serial connections (the parameters differs on the device you are connecting to

ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=9600,
    timeout=1,
    parity=serial.PARITY_EVEN,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.SEVENBITS,
)

def get_response():
    resp = ''
    while ser.inWaiting() > 0:
        resp += ser.read(1)
    return resp

def init_serial_mode():
    for i in range(0, 10):
        ser.write('\r')

def write_byte(string):
    ser.write((string + '\r').encode())

def main():
   
    ser.setRTS(False)
    # ser.setDTR(False)

    print(ser.getCTS())
    print(ser.getDSR())

    while True:
        
        prompt = """
        Test Commands
        -------------
        command = test both wheels
        query = test if data can be retrieved from serial port
        exit = close port and terminate program
        change_mode
        """
        print(prompt)
    
        input = raw_input(">> ")

        if input == 'exit':
            ser.close()
            exit()
        
        elif input == 'command':
            write_byte("!A10")
            print(get_response())
            time.sleep(0.1) 

            write_byte("!B10")
            print(get_response())
            time.sleep(0.10)
            # print(get_response())

            # write_byte('?K')
            # print ('Speed query %s' % get_response()) 
            # time.sleep(0.10)
            #write_byte("!A00")

            #write_byte("!B10")
                

            #write_byte("!b10")
            #print(get_response())

            #write_byte("!B00")
            #print(get_response())

        elif input == 'query':
            write_byte('?K')
            print ("Speed query %s" % get_response())

            write_byte('?E')
            print ("Battery query %s" % get_response())

        elif input == 'change_mode':
            write_byte('2C04')
            get_response()

if __name__ == '__main__':
    init_serial_mode()
    main()
