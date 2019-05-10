from tkinter import *
import asyncio
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

# <c0000FF>

Serial = serial.Serial("/dev/ttyACM0")

oldItem = 0
oldColor = 0

class colorGUI:
    def __init__(self, master):
        self.master = master
        self.frame = Frame(self.master)

        self.rInt = IntVar()
        self.gInt = IntVar()
        self.bInt = IntVar()
        self.lengthInt = IntVar()

        self.lengthInt.set(1)
        self.rInt.set(255)
        self.gInt.set(255)
        self.bInt.set(255)

        self.lengthOld = self.lengthInt.get()
        self.rOld = self.rInt.get()
        self.gOld = self.gInt.get()
        self.bOld = self.gInt.get()

        self.rEntry = Entry(self.frame, textvariable=self.rInt)
        self.gEntry = Entry(self.frame, textvariable=self.gInt)
        self.bEntry = Entry(self.frame, textvariable=self.bInt)
        self.lengthEntry = Entry(self.frame, textvariable=self.lengthInt)

        self.apply = Button(self.frame, text="Apply", command=self.send)

        self.rLabel = Label(self.frame, text="R")
        self.gLabel = Label(self.frame, text="G")
        self.bLabel = Label(self.frame, text="B")
        self.lengthLabel = Label(self.frame, text="Len")

        self.rLabel.grid(row=0, column=0)
        self.gLabel.grid(row=1, column=0)
        self.bLabel.grid(row=2, column=0)
        self.lengthLabel.grid(row=3, column=0)

        self.rEntry.grid(row=0, column=1)
        self.gEntry.grid(row=1, column=1)
        self.bEntry.grid(row=2, column=1)
        self.lengthEntry.grid(row=3, column=1)
        self.apply.grid(row=4, column=1)
    
    def send(self):
        rCommand = "<" + "c" + str(selectedItem.get()) + str(selectedColor.get()) + "0" + str(hex(self.rInt.get())[2:].zfill(3)) + ">"
        Serial.write(rCommand.encode())
        self.rOld = self.rInt.get()

        gCommand = "<" + "c" + str(selectedItem.get()) + str(selectedColor.get()) + "1" + str(hex(self.gInt.get())[2:].zfill(3)) + ">"
        Serial.write(gCommand.encode())
        self.gOld = self.gInt.get()

        bCommand = "<" + "c" + str(selectedItem.get()) + str(selectedColor.get()) + "2" + str(hex(self.bInt.get())[2:].zfill(3)) + ">"
        Serial.write(bCommand.encode())
        self.bOld = self.bInt.get()

        lCommand = "<" + "l" + str(selectedItem.get()) + str(selectedColor.get()) + "0" + str(hex(self.lengthInt.get())[2:].zfill(3)) + ">"
        Serial.write(lCommand.encode())
        self.lengthOld = self.lengthInt.get()
        
        return

class selectionGUI:

    def __init__(self, master):
        self.master = master

        self.frame = Frame(master)

        self.itemSelect = Entry(self.frame, textvariable=selectedItem)
        self.colorSelect = Entry(self.frame, textvariable=selectedColor)

        self.colorLabel = Label(self.frame, text="Color #")
        self.itemLabel = Label(self.frame, text="Item #")

        self.itemLabel.grid(row=0, column=0)
        self.colorLabel.grid(row=1, column=0)
        self.itemSelect.grid(row=0, column=1)
        self.colorSelect.grid(row=1, column=1)

class itemGUI:
    def __init__(self, master):
        self.master = master

        self.frame = Frame(self.master)

        self.timeInt = IntVar()
        self.colorCountInt = IntVar()
        self.positionInt = IntVar()

        self.directionOld = 1

        self.timeInt.set(10)
        self.colorCountInt.set(1)
        self.positionInt.set(0)

        self.timeOld = self.timeInt.get()
        self.colorCountOld = self.colorCountInt.get()
        self.positionOld = self.positionInt.get()

        self.timeLabel = Label(self.frame, text="Delay")
        self.colorCountLabel = Label(self.frame, text="Color Count")
        self.positionLabel = Label(self.frame, text="Set Position")

        self.timeEntry = Entry(self.frame, textvariable=self.timeInt)
        self.colorCountEntry = Entry(self.frame, textvariable=self.colorCountInt)
        self.positionEntry = Entry(self.frame, textvariable=self.positionInt)

        self.apply = Button(self.frame, text="Apply", command= lambda: self.send("a"))
        self.directionChange = Button(self.frame, text="Change Dir", command= lambda: self.send("d"))
        self.positionApply = Button(self.frame, text="Apply Position", command= lambda: self.send("p"))


        self.timeLabel.grid(row=0, column=0)
        self.colorCountLabel.grid(row=1, column=0)
        self.positionLabel.grid(row=2, column=0)

        self.timeEntry.grid(row=0, column=1)
        self.colorCountEntry.grid(row=1, column=1)
        self.positionEntry.grid(row=2, column=1)

        self.apply.grid(row=3, column=0)
        self.directionChange.grid(row=3, column=1)
        self.positionApply.grid(row=4, column=1)

    def send(self, type):
        if(type == "a"):
            tCommand = "<" + "t" + str(selectedItem.get()) + str(selectedColor.get()) + "0" + str(hex(self.timeInt.get())[2:].zfill(3)) + ">"
            Serial.write(tCommand.encode())
            self.timeOld = self.timeInt.get()
            print("New Value" + tCommand)

            cCommand = "<" + "a" + str(selectedItem.get()) + str(selectedColor.get()) + "0" + str(hex(self.colorCountInt.get())[2:].zfill(3)) + ">"
            Serial.write(cCommand.encode())
            self.colorCountOld = self.colorCountInt.get()
            print("New Value" + cCommand)
        if(type == "d"):
            newDir = 0
            if(self.directionOld != 1):
                newDir = 1
            dCommand = "<" + "d" + str(selectedItem.get()) + str(selectedColor.get()) + "000" + str(newDir) + ">"
            Serial.write(dCommand.encode())
            self.directionOld = newDir
            print("New Value" + dCommand)
        
        if(type == "p"):
            pCommand = "<" + "p" + str(selectedItem.get()) + str(selectedColor.get()) + "0" + str(hex(self.positionInt.get())[2:].zfill(3)) + ">"
            Serial.write(pCommand.encode())
            self.positionOld = self.positionInt.get()
            print("New Value" + pCommand)
        return

class stripGUI:
    def __init__(self, master):
        self.master = master

        self.frame = Frame(self.master)

        self.itemCountInt = IntVar()
        self.itemCountInt.set(1)

        self.activeOld = 1
        
        self.itemCountOld = self.itemCountInt.get()

        self.itemCountLabel = Label(self.frame, text="Item Count")
        self.itemCountEntry = Entry(self.frame, textvariable=self.itemCountInt)
        
        self.activeButton = Button(self.frame, text="Start/Stop")
        self.apply = Button(self.frame, text="Apply", command= lambda: self.send())


        self.itemCountLabel.grid(row=0, column=0)
        self.itemCountEntry.grid(row=0, column=1)
        self.apply.grid(row=1, column=1)



    def send(self):
        if(self.itemCountInt.get() != self.itemCountOld):
            iCommand = "<" + "i" + str(selectedItem.get()) + str(selectedColor.get()) + "0" + str(hex(self.itemCountInt.get())[2:].zfill(3)) + ">"
            Serial.write(iCommand.encode())
            self.itemCountOld = self.itemCountInt.get()
            print("New Value" + iCommand)



class mainGUI:
    def __init__(self, master):
        self.master = master

        self.frame = Frame(master)
        self.frame.pack()

        self.cGUI = colorGUI(self.frame)
        self.selGUI = selectionGUI(self.frame)
        self.iGUI = itemGUI(self.frame)
        self.sGUI = stripGUI(self.frame)


        self.selGUI.frame.grid(row=1, column=0)
        self.cGUI.frame.grid(row=0, column=0)
        self.iGUI.frame.grid(row=0, column=1)
        self.sGUI.frame.grid(row=1, column=1)




root = Tk()

selectedItem = IntVar()
selectedColor = IntVar()

selectedColor.set(0)
selectedItem.set(0)

gui = mainGUI(root)


root.mainloop()
