import time
import serial

# configure the serial connections (the parameters differs on the device you are connecting to


def get_response():
    resp = ''
    while ser.inWaiting() > 0:
        resp += ser.read(1)
    return resp


def main():

    ser = serial.Serial(
        port='/dev/ttyUSB1',
        baudrate=9600,
        parity=serial.PARITY_EVEN,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.SEVENBITS
    )

    #  10 Enter key press (new line) is needed for RS-232 Mode
    ser.write("\n\n\n\n\n\n\n\n\n\n")
    if (get_response() != 'OK'):
        print ("Did not get OK message")
        ser.close()
        exit()

    while True:
        prompt = """
        Test Commands
        -------------
        command = test both wheels
        query = test if data can be retrieved from serial port
        exit = close port and terminate program
        """
        print(prompt)
        
        input = raw_input(">> ")

        if input == 'exit':
            ser.close()
            exit()
        
        elif input == 'command':
            ser.write("!A10")
            print(get_response())
            time.sleep(1)

            ser.write("!a10")
            print(get_response())
            time.sleep(1)

            ser.write("!A00")

            ser.write("!B10")
            print(get_response())
            time.sleep(1)

            ser.write("!b10")
            print(get_response())
            time.sleep(1)

            ser.write("!B00")

        elif input == 'query':
            ser.write('?K')
            print ("Speed query %s" % get_response())

            ser.write('?E')
            print ("Battery query %s" % get_response())

if __name__ == '__main__':
    main()