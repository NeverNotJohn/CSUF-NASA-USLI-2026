import tkinter as tk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import random
import input
import time
from datetime import datetime, timezone
import csv
import os
# ===== Globals =====
PORT = "/dev/ttyACM2"
BAUD = 9600
RETRY_SEC = 2
sm = input.SerialManager(PORT, BAUD)
CSV_FILE = "flight_data.csv"
CSV_HEADERS = ["n", "hour", "minute", "second", "altitude", "lng", "lat", "roll", "pitch", "yaw", "mission_state"]
# ===== Funcs =====
def log(msg):
    console.config(state="normal")
    console.insert("end", f"> {msg}\n")
    console.see("end")
    console.config(state="disabled")

# ===== Colors / Fonts =====
BG = "#000000"
FG = "#33ff33"
FONT_TITLE = ("Courier New", 24, "bold")
FONT_TEXT = ("Courier New", 14)

root = tk.Tk()
root.title("FS2 HELLHOUND")
root.configure(bg=BG)

# Fullscreen
root.attributes("-fullscreen", True)
root.bind("<Escape>", lambda e: root.attributes("-fullscreen", False))

# ===== Title =====
title = tk.Label(
    root,
    text="> FS2 HELLHOUND",
    font=FONT_TITLE,
    fg=FG,
    bg=BG
)
title.pack(pady=10)

# ===== Main Grid =====
main = tk.Frame(root, bg=BG)
main.pack(fill="both", expand=True)

# Rows: top smaller, bottom taller
main.rowconfigure(0, weight=5)
main.rowconfigure(1, weight=1)

# Columns: left smaller, right wider
main.columnconfigure(0, weight=5)
main.columnconfigure(1, weight=1)

# ===== Top Left (Read-only Data) =====
top_left = tk.Text(main, bg=BG, fg=FG, font=FONT_TEXT,
                   insertbackground=FG, borderwidth=0, width=0)
top_left.grid(row=0, column=0, sticky="nsew", padx=10, pady=10)
#top_left.config(highlightthickness=2, highlightbackground="red")

top_left.insert(
    "end",
    "> STATUS\n"
    "n\t0\n"
    "TIME\t00:00:00\n"
    "ALT\t0 ft\n"
    "LAT/LNG\t0°N, 0°W\n"
    "ROLL\t0°\n"
    "PITCH\t0°\n"
    "YAW\t0°\n"
    "STATE\tNA\n"
)
top_left.config(state="disabled")


def initialize_csv(file_path: str = CSV_FILE, headers: list = CSV_HEADERS) -> None:
    """Create the CSV file and write the header row."""
    with open(file_path, mode="w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(headers)
    print(f"Initialized '{file_path}' with headers: {headers}")

def update_csv(data: dict, file_path: str = CSV_FILE) -> None:
    """Append a new row of data to the CSV file."""
    with open(file_path, mode="a", newline="") as f:
        writer = csv.writer(f)
        row = [data.get(header, "") for header in CSV_HEADERS]
        writer.writerow(row)
    print(f"Appended data to '{file_path}': {row}")


def updateStatus(data):
    # Enable editing
    top_left.config(state="normal")
    
    # Clear current text
    top_left.delete("1.0", tk.END)
    
    # Insert new data
    try:
        top_left.insert(
            tk.END,
            f"> STATUS\n"
            f"n\t{data["n"]}\n"
            f"TIME\t{data["hour"]}:{data["minute"]}:{data["second"]}\n"
            f"ALT\t{data["altitude"]} ft\n"
            f"LAT/LNG\t{data["lat"]}°N, {data["lng"]}°W\n"
            f"ROLL\t{data["roll"]}°\n"
            f"PITCH\t{data["pitch"]}°\n"
            f"YAW\t{data["yaw"]}°\n"
            f"STATE\t{data["mission_state"]}\n"
        )
    except:
        log("Error Updating Status")
    
    # Make read-only again
    top_left.config(state="disabled")
    

# ===== Top Right (Buttons) =====
top_right = tk.Frame(main, bg=BG)
top_right.grid(row=0, column=1, sticky="nsew", padx=10, pady=10)
#top_right.config(highlightthickness=2, highlightbackground="blue")

# Allow buttons to expand inside frame
top_right.rowconfigure((0, 1, 2), weight=1)
top_right.columnconfigure(0, weight=1)

# ===== Console log function =====

# ===== Buttons =====
btn_style = {
    "font": ("Courier New", 24),
    "fg": FG,
    "bg": BG,
    "activeforeground": BG,
    "activebackground": FG,
    "borderwidth": 0,
    "highlightthickness": 0,
}

def command(cmd):
    # Literally sphagetti code
    
    logText = ""
    rylrCmd = ""
    if (cmd == "ARM"):
        logText = "ARM"
        rylrCmd = "A"
    if (cmd == "CALIBRATE"):
        logText = "CALIBRATE"
        rylrCmd = "C"

    log(logText)        # Log on Console
    sm.send(rylrCmd)    # Send to RYLR896

tk.Button(top_right, text="[ ARM ]", command=lambda: command("ARM"), **btn_style).grid(row=0, column=0, sticky="ew", pady=5)
tk.Button(top_right, text="[ CALIBRATE ]", command=lambda: command("CALIBRATE"), **btn_style).grid(row=1, column=0, sticky="ew", pady=5)
tk.Button(top_right, text="[ HEHE ]", command=lambda: command("SHUTDOWN"), **btn_style).grid(row=2, column=0, sticky="ew", pady=5)

# ===== Bottom Left (Console) =====
console = tk.Text(main, bg=BG, fg=FG, font=FONT_TEXT,
                  insertbackground=FG, borderwidth=0, width=0)
console.grid(row=1, column=0, sticky="nsew", padx=10, pady=10)
console.insert("end", "> LOG OUTPUT\nSystem initialized...\n")
console.config(state="disabled")

def consoleAppend(data):
    console.config(state="normal")
    now_utc = datetime.now(timezone.utc)
    console.insert("end", f"[{now_utc.hour}:{now_utc.minute}:{now_utc.second}] {data}\n")
    console.config(state="disabled")

#console.config(highlightthickness=2, highlightbackground="yellow")

# ===== Bottom Right (Two Graphs Side-by-Side) =====

# Settings
WINDOW = 300        # 5 mins

graph_frame = tk.Frame(main, bg=BG)
graph_frame.grid(row=1, column=1, sticky="nsew", padx=10, pady=10)
#graph_frame.config(highlightthickness=2, highlightbackground="magenta")
graph_frame.rowconfigure(0, weight=1)
graph_frame.columnconfigure((0, 1), weight=1)

fig = Figure(figsize=(5, 4), dpi=100)
ax = fig.add_subplot(111)

ax.set_facecolor(BG)
fig.patch.set_facecolor(BG)
ax.tick_params(colors=FG)
for spine in ax.spines.values():
    spine.set_color(FG)

times = []
values = []

line, = ax.plot([], [], color=FG)
ax.set_xlim(0, WINDOW)
ax.set_xlabel("Time (s)", color=FG)
ax.set_ylabel("Altitude (ft)", color=FG)

canvas = FigureCanvasTkAgg(fig, master=graph_frame)
canvas.draw()
canvas.get_tk_widget().pack(fill="both", expand=True)

start_time = time.time()

def updateAlt(alt, n):
    # Adjust for Time
    n = n/2
    
    values.append(alt)
    times.append(n)        

    if (n < times[0]):
        line.set_data([],[])
        values.clear()
        times.clear()
        values.append(alt)
        times.append(n)       

    while times and (n - times[0] > WINDOW):
        times.pop(0)
        values.pop(0)

    line.set_data(times, values)
    ax.set_xlim(max(0, n - WINDOW), n)

    if len(values) > 1:
        ymin, ymax = min(values), max(values)
        pad = (ymax - ymin) * 0.1 or 1
        ax.set_ylim(ymin - pad, ymax + pad)

    canvas.draw_idle()

"""
fig = Figure(figsize=(6, 4), dpi=100)
fig.patch.set_facecolor(BG)

ax1 = fig.add_subplot(121)
ax2 = fig.add_subplot(122)

for ax in (ax1, ax2):
    ax.set_facecolor(BG)
    ax.tick_params(colors=FG)
    for spine in ax.spines.values():
        spine.set_color(FG)
    ax.set_ylim(-10, 10)

data1 = [0] * 50
data2 = [0] * 50

line1, = ax1.plot(alt_x, alt_y, color=FG)
line2, = ax2.plot(data2, color=FG)

# Plot 1 setup
ax1.set_title("Altitude vs n", color=FG, fontsize=14)
ax1.set_xlabel("n", color=FG)
ax1.set_ylabel("Altitude (ft)", color=FG)

ax2.set_title("SIGNAL B", color=FG, fontsize=14)

canvas = FigureCanvasTkAgg(fig, master=graph_frame)
canvas.draw()
canvas.get_tk_widget().pack(fill="both", expand=True)

def add_altitude_point(altitude_ft):
    global n_counter

    alt_y.pop(0)
    alt_y.append(altitude_ft)

    n_counter += 1
    alt_x.pop(0)
    alt_x.append(n_counter)

    line1.set_data(alt_x, alt_y)

    ax1.set_xlim(alt_x[-MAX_POINTS], alt_x[-1])
    ax1.relim()
    ax1.autoscale_view(scalex=False, scaley=True)

    canvas.draw_idle()
"""
# ===== Streaming Data =====

"""
def update_plot():
    data1.pop(0)
    data2.pop(0)
    data1.append(random.uniform(-8, 8))
    data2.append(random.uniform(-5, 5))
    line1.set_ydata(data1)
    line2.set_ydata(data2)
    canvas.draw()
    root.after(200, update_plot)
"""

def update_data():
    sm.run()
    data = sm.read()
    
    try:
        if (data and data[:4] == "+RCV"):
            rcvFlag = True
        else:
            rcvFlag = False
    except:
        rcvFlag = False
    
    # Add to console
    if (data and rcvFlag):
        consoleAppend("Packet Received")
    if (data and rcvFlag):
        update_csv(input.parse_payload(data))
    # Update Status
    if (data and rcvFlag):
        updateStatus(input.parse_payload(data))
    
    # Update Plots
    if (data and rcvFlag):
        temp = input.parse_payload(data)
        if (temp):
            updateAlt(temp["altitude"], temp["n"])
    
    # Debug
    if (data and rcvFlag):
        print(input.parse_payload(data))
    
    root.after(450, update_data)

initialize_csv(CSV_FILE, CSV_HEADERS)

update_data()

root.mainloop()
