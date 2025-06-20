import serial
import threading
import time

def read_from_port(ser):
    while True:
        bytes_waiting = ser.in_waiting
        if bytes_waiting:
            data = ser.read(bytes_waiting)
            print(data.decode(errors='ignore'), end='')
        else:
            time.sleep(0.01)

def main():
    port = '/dev/tty.usbmodem1234561'  # Cambia según tu puerto real, ejemplo /dev/ttyUSB0 o /dev/ttyACM0 o /dev/cu.wchusbserial...
    baud = 115200

    try:
        with serial.Serial(port, baud, timeout=1) as ser:
            print(f"Conectado a {port} a {baud} baudios.")

            thread = threading.Thread(target=read_from_port, args=(ser,), daemon=True)
            thread.start()

            print("Escribe líneas JSON y presiona ENTER para enviar. Ctrl+C para salir.")
            while True:
                line = input()
                if line:
                    ser.write((line + '\n').encode())
    except serial.SerialException as e:
        print(f"Error al abrir el puerto serial: {e}")
    except KeyboardInterrupt:
        print("\nSaliendo...")

if __name__ == "__main__":
    main()