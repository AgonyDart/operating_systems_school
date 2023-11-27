import matplotlib.pyplot as plt
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk
from matplotlib.backends.backend_gtk3agg import FigureCanvasGTK3Agg as FigureCanvas

# List of data
data = [[100, 1900], [400, 1500], [300, 1800], [200, 1700]]

# Current index
index = 0

# Function to draw a bar chart
def draw_chart(data):
    fig, ax = plt.subplots(figsize=(2.5,10))
    ax.set_xlim([0, 0.1])
    ax.set_xticks([])
    ax.set_ylim([0, 2000])
    ax.set_title('Mejor ajuste')
    ax.bar(0, data[0], 0.8 , data[1], color='w', edgecolor='black', label='os')
    ax.legend()
    return fig

# Function to handle key press events
def on_key_press_event(widget, event):
    global index
    keyname = Gdk.keyval_name(event.keyval)
    if keyname == 'Left':
        index = (index - 1) % len(data)
    elif keyname == 'Right':
        index = (index + 1) % len(data)
    fig = draw_chart(data[index])
    canvas = FigureCanvas(fig)
    window.add(canvas)
    window.show_all()

# Initialize the GTK window
window = Gtk.Window()
window.connect("destroy", Gtk.main_quit)
window.connect("key-press-event", on_key_press_event)

# Draw the initial bar chart
fig = draw_chart(data[index])
canvas = FigureCanvas(fig)
window.add(canvas)
window.show_all()

# Start the GTK main loop
Gtk.main()