from tkinter import *
from PIL import ImageTk, Image
import serial

# Serial conncetion to talk to STM
ser = serial.Serial("/dev/tty.usbserial-DN03UD56", 115200, timeout=1)

# arrays with on or off state of the buttons
# and the slider values
on_Or_Off_Array = [0, 0, 0, 0, 0, 0, 0, 0]
slider_Values = [0, 0, 0, 0, 0, 0, 0, 0]

# these arrays are where the buttons and slider objects are stored
buttons = [0, 0, 0, 0, 0, 0, 0, 0]
sliders = [0, 0, 0, 0, 0, 0, 0, 0]

number_Of_Notes = 8
number_Of_Rows = 1

# function to change the button state and update the sliders


def Button_Is_Pressed(button_Number, row_Number):
    # if the button is pressed we need to check if the button is on or off
    # we also update the sliders everytime the button is pressed.
    # this makes a slight issue where if no button is pressed no slider will update.
    if on_Or_Off_Array[button_Number] == 0:
        on_Or_Off_Array[button_Number] = 1
        buttons[button_Number] = Button(
            interface_Window,
            image=on_Button,
            command=lambda z=button_Number, r=row_Number: Button_Is_Pressed(
                z, row_Number
            ),
        )
        buttons[button_Number].grid(column=button_Number, row=row_Number)
    elif on_Or_Off_Array[button_Number] == 1:
        on_Or_Off_Array[button_Number] = 0
        buttons[button_Number] = Button(
            interface_Window,
            image=off_Button,
            command=lambda z=button_Number, r=row_Number: Button_Is_Pressed(
                z, row_Number
            ),
        )
        buttons[button_Number].grid(column=button_Number, row=row_Number)
    for i in range(number_Of_Notes):
        slider_Values[i] = sliders[i].get()
    # this is where w'll send the data to the stm32
    print(on_Or_Off_Array + slider_Values)
    ser.write(on_Or_Off_Array + slider_Values)


# function that makes the array to start


def Make_Button_Array(number_Of_Rows, number_Of_Notes):
    for x in range(number_Of_Rows):
        for i in range(number_Of_Notes):
            sliders[i] = Scale(interface_Window, from_=12, to=0)
            buttons[i] = Button(
                interface_Window,
                image=off_Button,
                command=lambda z=i, r=x: Button_Is_Pressed(z, r),
            )
            buttons[i].grid(column=i, row=x)
            sliders[i].grid(column=i, row=x + 1)


# serial_Handle = serial.Serial('/dev/tty.usbserial-DN03UD56', 115200, timeout=1)
interface_Window = Tk()

# makes every visual thing ready to be used by the function
interface_Window.title("SUPER STEP SEQUENCER")  # gives the window a title

button_Off_Image = Image.open("Images/RED.png")  # opens image
button_On_Image = Image.open("Images/GREEN.png")

resized_Off_Button = button_Off_Image.resize((20, 20), Image.BILINEAR)
resized_On_Button = button_On_Image.resize((20, 20), Image.BILINEAR)

off_Button = ImageTk.PhotoImage(resized_Off_Button)  # makes the image usable
on_Button = ImageTk.PhotoImage(resized_On_Button)

# canvas.create_image(20, 20, anchor=NW, image=resized_On_Button)

Make_Button_Array(number_Of_Rows, number_Of_Notes)

interface_Window.mainloop()
