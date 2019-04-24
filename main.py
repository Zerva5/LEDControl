from tkinter import *
import serial
import string

borderCol = "grey"
errorCol = "red"
successCol = "green"

timeEntryDefault = "0"

rEntryDefault = "0"
gEntryDefault = "0"
bEntryDefault = "0"

editorPaddingx = 10
editorPaddingy = 5

colorDict = {
    0: "r",
    1: "g",
    2: "b"
}

Serial = serial.Serial("/dev/ttyACM0")

class mainGUI:
    def __init__(self, master):
        self.master = master

        self.time = 0

        self.frame = Frame(master)
        self.frame.pack(side=TOP)

        self.header = Label(self.frame, text="Color Editor", font=("Arial", 44))

        self.controlFrame = Frame(self.frame, highlightbackground=borderCol, highlightcolor=borderCol, highlightthickness=1)
        self.controlLabel = Label(self.controlFrame, text="LED Control  ")
        self.startButton = Button(self.controlFrame, text="Start", command=lambda: self.Start_Stop(0))
        self.stopButton = Button(self.controlFrame, text="Stop", command=lambda: self.Start_Stop(1))

        self.timeFrame = Frame(self.frame)

        self.timeLabel = Label(self.timeFrame, text="Time")
        self.timeEntry = Entry(self.timeFrame)
        self.timeEntry.insert(END, timeEntryDefault)
        self.timeApply = Button(self.timeFrame, text="Apply", command=self.Get_Time)

        self.colorGUIS = [colorEditor(self.frame, 1, self), colorEditor(self.frame, 2, self), colorEditor(self.frame, 3, self)]

        self.statusMessage = Label(self.frame, text="Please Do Something", fg=successCol)

        self.header.grid(row=0, column=1)

        self.controlFrame.grid(row=1, column=0, padx=10, pady=10)
        self.controlLabel.grid(row=0, column=0)
        self.startButton.grid(row=0, column=1)
        self.stopButton.grid(row=0, column=2)

        self.timeFrame.grid(row=1, column=1)

        self.timeLabel.grid(row=0, column=0)
        self.timeEntry.grid(row=0, column=1)
        self.timeApply.grid(row=0, column=2)

        self.statusMessage.grid(row=3, column=1)

    def Send_Time(self):
        timeString = "<1t" + "{0:0{1}x}".format(self.time, 3) + ">"

        print(timeString)
        Serial.write(timeString.encode())

    def Get_Time(self):
        timeString = self.timeEntry.get()
        try:
            self.time = int(timeString)
        except ValueError:
            pass

        self.Send_Time()

    def Start_Stop(self, action):
        actionString = ""
        if(action == 0):
            actionString = "1ss"
        else:
            actionString = "1st"

        sendString = "<" + actionString + ">"

        print(sendString)
#         Send The String as serial


class colorEditor:
    def __init__(self, master, item, parent):
        self.master = master
        self.parent = parent

        self.frame = Frame(master, highlightbackground=borderCol, highlightcolor=borderCol, highlightthickness=1, padx=editorPaddingx, pady=editorPaddingy)

        self.item = item

        self.header = Label(self.frame, text="Color " + str(item))


        self.rEntry = Entry(self.frame)
        self.rEntry.insert(END, rEntryDefault)

        self.rLabel = Label(self.frame, text="Red")


        self.gEntry = Entry(self.frame)
        self.gEntry.insert(END, gEntryDefault)

        self.gLabel = Label(self.frame, text="Green")


        self.bEntry = Entry(self.frame)
        self.bEntry.insert(END, bEntryDefault)

        self.bLabel = Label(self.frame, text="Blue")


        self.apply_button = Button(self.frame, text="Apply", command=self.Get_Data)

        self.error_label = Label(self.frame, text="", fg=errorCol)

        self.frame.grid(row=2, column=self.item-1)

        self.header.grid(row=0, column=1)

        self.rLabel.grid(row=1, column=0)
        self.rEntry.grid(row=1, column=1)

        self.gLabel.grid(row=2, column=0)
        self.gEntry.grid(row=2, column=1)

        self.bLabel.grid(row=3, column=0)
        self.bEntry.grid(row=3, column=1)

        self.apply_button.grid(row=4, column=0)

        self.error_label.grid(row=4, column=1)

        self.color = [0, 0, 0]

    def Get_Data(self):
        r = self.rEntry.get()
        g = self.gEntry.get()
        b = self.bEntry.get()

        rint = 0
        gint = 0
        bint = 0

        # Try and convert the values to ints
        try:
            rint = int(r)
            gint = int(g)
            bint = int(b)
            self.error_label['text'] = ":)"

        # If its not an int its bad
        except ValueError:
            self.error_label['text'] = "BAD INPUT"
            return

        newColor = [rint, gint, bint]

        for index in range(0, 3):
            if(newColor[index] > 255):
                newColor[index] = 255
            elif(newColor[index] < 0):
                newColor[index] = 0

        self.color = newColor

        for val in range(0, 3):
            if(self.Send_Data(val) == 0):
                (self.parent.statusMessage.config(text="AYYY", fg=successCol))
            else:
                self.parent.statusMessage.config(text="SOMETHING WENT WRONG", fg=errorCol)

    def Send_Data(self, color):
        #Serial Stuff in here
        # Serial command syntax
        # (value) is in hex, 2 chars for a color, 3 for a time
        # 1(colorNum)(color)(value)
        # 1(time)(value)

        serialData = "1" + str(self.item) + colorDict[color] + "{0:0{1}x}".format(self.color[color], 2)
        # print(serialData)

        serialString = "<" + serialData + ">"


        print(serialString)

        Serial.write(serialString.encode())

        return 0


root = Tk()
gui = mainGUI(root)

root.mainloop()