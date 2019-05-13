from random import randint

class colorID:
    def __init__(self):
        self.rgb = [255, 255, 255]
        self.len = 1

    def randomColor(self):
        for value in self.rgb:
            value = randint(0, 255)


class Item:
    def __init__(self):
        self.colors = [colorID()]

        self.colorCount = 1
        self.maxt = 10
        self.currentIndex = 0
        self.direction = 1 
    def addColor(self):
        self.colors.append(colorID)

class Strip:
    def __init__(self):
        self.itemCount = 0
        self.items = []
        self.active = 1

    def addItem(self):
        self.items.append(Item)
