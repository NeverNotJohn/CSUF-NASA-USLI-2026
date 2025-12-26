import tkinter as tk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import random
import input

# ===== Colors / Fonts =====
BG = "#000000"
FG = "#33ff33"
FONT_TITLE = ("Courier New", 24, "bold")
FONT_TEXT = ("Courier New", 14)

root = tk.Tk()
root.title("BIG DAWG TERMINAL")
root.configure(bg=BG)

# Fullscreen
root.attributes("-fullscreen", True)
root.bind("<Escape>", lambda e: root.attributes("-fullscreen", False))

# ===== Title =====
title = tk.Label(
    root,
    text="> BIG DAWG TERMINAL",
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
    "> DATA\n"
    "TIME\t00:00:00\n"
    "ALT\t0 ft\n"
    "LAT/LNG\t0°N, 0°W\n"
    "ROLL\t0°\n"
    "PITCH\t0°\n"
    "YAW\t0°\n"
)
top_left.config(state="disabled")

# ===== Top Right (Buttons) =====
top_right = tk.Frame(main, bg=BG)
top_right.grid(row=0, column=1, sticky="nsew", padx=10, pady=10)
#top_right.config(highlightthickness=2, highlightbackground="blue")

# Allow buttons to expand inside frame
top_right.rowconfigure((0, 1, 2), weight=1)
top_right.columnconfigure(0, weight=1)

# ===== Console log function =====
def log(msg):
    console.config(state="normal")
    console.insert("end", f"> {msg}\n")
    console.see("end")
    console.config(state="disabled")

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

tk.Button(top_right, text="[ START ]", command=lambda: log("START"), **btn_style).grid(row=0, column=0, sticky="ew", pady=5)
tk.Button(top_right, text="[ STATUS ]", command=lambda: log("STATUS"), **btn_style).grid(row=1, column=0, sticky="ew", pady=5)
tk.Button(top_right, text="[ SHUTDOWN ]", command=lambda: log("SHUTDOWN"), **btn_style).grid(row=2, column=0, sticky="ew", pady=5)

# ===== Bottom Left (Console) =====
console = tk.Text(main, bg=BG, fg=FG, font=FONT_TEXT,
                  insertbackground=FG, borderwidth=0, width=0)
console.grid(row=1, column=0, sticky="nsew", padx=10, pady=10)
console.insert("end", "> LOG OUTPUT\nSystem initialized...\n")
console.config(state="disabled")
#console.config(highlightthickness=2, highlightbackground="yellow")

# ===== Bottom Right (Two Graphs Side-by-Side) =====
graph_frame = tk.Frame(main, bg=BG)
graph_frame.grid(row=1, column=1, sticky="nsew", padx=10, pady=10)
#graph_frame.config(highlightthickness=2, highlightbackground="magenta")
graph_frame.rowconfigure(0, weight=1)
graph_frame.columnconfigure((0, 1), weight=1)

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

line1, = ax1.plot(data1, color=FG)
line2, = ax2.plot(data2, color=FG)

ax1.set_title("SIGNAL A", color=FG, fontsize=14)
ax2.set_title("SIGNAL B", color=FG, fontsize=14)

canvas = FigureCanvasTkAgg(fig, master=graph_frame)
canvas.draw()
canvas.get_tk_widget().pack(fill="both", expand=True)

# ===== Streaming Data =====
def update_plot():
    data1.pop(0)
    data2.pop(0)
    data1.append(random.uniform(-8, 8))
    data2.append(random.uniform(-5, 5))
    line1.set_ydata(data1)
    line2.set_ydata(data2)
    canvas.draw()
    root.after(200, update_plot)

update_plot()

root.mainloop()
