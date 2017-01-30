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
    
    resp = get_response() 
    if (resp != 'OK'):
        print (resp)
        print ("Did not get OK message")
    
def write_byte(string):
    ser.write((string + '\r').encode())

def main():
    #if not ser.is_open:
    #    print ("Port is not accessible")
    #    eixt()
    
    ser.setRTS(False)
    # ser.setDTR(False)

    print(ser.getCTS())
    print(ser.getDSR())

    #  10 Enter key press (new line) is needed for RS-232 Mode
    # for i in range(0, 10):
    #     ser.write("\r")
    
   #    ser.close()
    #    exit()

    while True:
        prompt = """
        Test Commands
        -------------
        command = test both wheels
        query = test if data can be retrieved from serial port
        exit = close port and terminate program
        init = start rs232 mode for motor controller
        """
        print(prompt)
        
        input = raw_input(">> ")

        if input == 'exit':
            ser.close()
            exit()
        
        elif input == 'command':
            write_byte("!A10")
            print(get_response())
            time.sleep(1)

            write_byte("!a10")
            print(get_response())
            time.sleep(1)

            write_byte("!A00")

            write_byte("!B10")
            print(get_response())
            time.sleep(1)

            write_byte("!b10")
            print(get_response())
            time.sleep(1)

            write_byte("!B00")
            print(get_response())
            time.sleep(1)

        elif input == 'query':
            write_byte('?K')
            print ("Speed query %s" % get_response())

            write_byte('?E')
            print ("Battery query %s" % get_response())

        elif input == 'init':
            init_serial_mode()
if __name__ == '__main__':
    main()
