from tkinter import *
import serial
import string

borderCol = "grey"
errorCol = "red"
successCol = "green"

timeEntryDefault = "1000"

rEntryDefault = "255"
gEntryDefault = "255"
bEntryDefault = "255"

lEntryDefault = "1"

editorPaddingx = 10
editorPaddingy = 5

colorDict = {
    0: "r",
    1: "g",
    2: "b"
}

# Serial = serial.Serial("/dev/ttyACM0")

class mainGUI:
    def __init__(self, master):
        self.master = master

        self.frame = Frame(master)
        self.frame.pack(side=TOP)

        self.selectedItem = IntVar()
        self.selectedItem.set(0)
        self.selectedColor = IntVar()
        self.selectedColor.set(0)

        self.rInt = IntVar()
        self.gInt = IntVar()
        self.bInt = IntVar()

        self.rInt.set(255)
        self.gInt.set(255)
        self.bInt.set(255)

        self.colorLengthString = IntVar()
        self.timeString = IntVar()
        self.itemCountString = IntVar()
        self.colorCountString = IntVar()
        self.positionString = IntVar()

        self.colorLengthString.set(1)
        self.timeString.set(10)
        self.itemCountString.set(1)
        self.colorCountString.set(1)
        self.positionString.set(0)

        self.selectionFrame = Frame(self.frame)
        self.itemSelect = Entry(self.selectionFrame, textvariable=self.selectedItem)
        self.colorSelect = Entry(self.selectionFrame, textvariable=self.selectedColor)
        self.selectionFrame.grid(row=0, column=0)
        self.itemSelect.grid(row=0, column=0)
        self.colorSelect.grid(row=1, column=0)

        self.colorFrame = Frame(self.frame)
        self.rEntry = Entry(self.colorFrame, textvariable=self.rInt)
        self.gEntry = Entry(self.colorFrame, textvariable=self.gInt)
        self.bEntry = Entry(self.colorFrame, textvariable=self.bInt)
        self.colorLength = Entry(self.colorFrame, textvariable=self.colorLengthString)
        self.colorApply = Button(self.colorFrame, text="Apply", command= lambda: self.sendCommand("c"))

        self.colorFrame.grid(row=1, column=0)
        self.rEntry.grid(row=0, column=0)
        self.gEntry.grid(row=1, column=0)
        self.bEntry.grid(row=2, column=0)
        self.colorApply.grid(row=3, column=0)

        self.toggleFrame = Frame(self.frame)
        self.start = Button(self.toggleFrame, text="Start")
        self.stop = Button(self.toggleFrame, text="Stop")
        self.forward = Button(self.toggleFrame, text="Forward")
        self.backward = Button(self.toggleFrame, text="Backward")

        self.toggleFrame.grid(row=2, column=0)
        self.start.grid(row=0, column=0)
        self.stop.grid(row=0, column=1)
        self.forward.grid(row=1, column=0)
        self.backward.grid(row=1, column=1)


        self.countFrame = Frame(self.frame)
        self.itemCountEntry = Entry(self.countFrame, textvariable=self.itemCountString)
        self.colorCountEntry = Entry(self.countFrame, textvariable=self.colorCountString)
        self.itemCountButton = Button(self.countFrame, text="Apply")
        self.colorCountButton = Button(self.countFrame, text="Apply")
        self.colorCountLabel = Label(self.countFrame, text="Color Count")
        self.itemCountLabel = Label(self.countFrame, text="Item Count")

        self.countFrame.grid(row=3, column=0)
        self.itemCountLabel.grid(row=0, column=0)
        self.itemCountEntry.grid(row=0, column=1)
        self.itemCountButton.grid(row=0, column=2)

        self.colorCountLabel.grid(row=1, column=0)
        self.colorCountEntry.grid(row=1, column=1)
        self.colorCountButton.grid(row=1, column=2)

        self.miscFrame = Frame(self.frame)
        self.timeEntry = Entry(self.miscFrame, textvariable=self.timeString)
        self.positionEntry = Entry(self.miscFrame, textvariable=self.positionString)
        self.timeButton = Button(self.miscFrame, text="Set Time")
        self.positionButton = Button(self.miscFrame, text="Set Position")


    def sendCommand(self, command):
        if(command == 'c'):
            rCommand = "<" + "c" + str(self.selectedItem.get()) + str(self.selectedColor.get()) + "0" + str(hex(self.rInt.get())[2:].zfill(3)) + ">"
            gCommand = "<" + "c" + str(self.selectedItem.get()) + str(self.selectedColor.get()) + "1" + str(hex(self.gInt.get())[2:].zfill(3)) + ">"
            bCommand = "<" + "c" + str(self.selectedItem.get()) + str(self.selectedColor.get()) + "2" + str(hex(self.bInt.get())[2:].zfill(3)) + ">"

            Serial.write(rCommand.encode())
            Serial.write(gCommand.encode())
            Serial.write((bCommand.encode()))


            # hex((int)self.rString)




root = Tk()
gui = mainGUI(root)

root.mainloop()
