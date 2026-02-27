import serial
import serial.threaded
import time

PORT = "COM9"
BAUD = 9600
RETRY_SEC = 2

def parse_payload(line: str):
    """
    Parse a +RCV line and return a dictionary with named fields.
    Returns None if parsing fails.
    """
    try:
        # Remove prefix and trailing metadata
        payload = line.split("=", 1)[1].split(",", 2)[2].rsplit(",", 2)[0]

        # Split into values
        values = payload.split()
        if len(values) != 10:
            raise ValueError(f"Expected 10 fields, got {len(values)}")

        # Map to variables
        n, hour, minute, second = map(int, values[:4])
        altitude, lng, lat = map(float, values[4:7])
        roll, pitch, yaw = map(int, values[7:])

        return {
            "n": n,
            "hour": hour,
            "minute": minute,
            "second": second,
            "altitude": altitude,
            "lng": lng,
            "lat": lat,
            "roll": roll,
            "pitch": pitch,
            "yaw": yaw
        }

    except (IndexError, ValueError) as e:
        print("Parse error:", e, "| Line:", line)
        return None

def handle_line(line):
        
    if (line == "+OK"):
        return
    
    line = parse_payload(line)
    if (line):
        currentData = line
        print(f"n {currentData["n"]}\tTime {currentData["hour"]}:{currentData["minute"]}:{currentData["second"]}\t Alt {currentData["altitude"]} ft\tLng {currentData["lng"]}\tlat {currentData["lat"]}")
    else:
        print("Packet Loss!")
        
        

class SerialManager:
    def __init__(self, port, baud):
        self.port = port
        self.baud = baud
        self.ser = None
        self.reader = None

    def connect(self):
        try:
            print("Connecting...")
            self.ser = serial.Serial(self.port, self.baud, timeout=1)
            print("Connected")
            return True

        except serial.SerialException as e:
            print("Connect failed:", e)
            self.cleanup()
            return False

    def cleanup(self):
        try:
            if self.reader:
                self.reader.close()
        except Exception:
            pass

        try:
            if self.ser and self.ser.is_open:
                self.ser.close()
        except Exception:
            pass

        self.reader = None
        self.ser = None

    def send(self, msg):
        if not self.ser or not self.ser.is_open:
            print("TX failed: not connected")
            return

        try:
            if not msg.endswith("\n"):
                msg += "\n"
            self.ser.write(msg.encode())
        except serial.SerialException:
            print("TX error — lost connection")
            self.cleanup()
    
    def read(self):
        try:
            if self.ser.in_waiting > 0:
                data = self.ser.readline().decode('utf-8', errors="ignore").strip()
                return data
            else:
                return None
        except:
            print("Could not read COM Port")
            self.ser = None
            return None
            

    def run(self):
        if not self.ser:
            if not self.connect():
                pass
        else:
            return

def main():
    sm = SerialManager(PORT, BAUD)

    try:
        while True:
            sm.run()
            data = sm.read()
            if (data):
                print(parse_payload(data))
                #print(data)
            #haha = input("Enter Command: ")
            #sm.send(haha)
            time.sleep(0.1)
    except KeyboardInterrupt:
        print("Exiting")
        sm.cleanup()


if __name__ == "__main__":
    main()