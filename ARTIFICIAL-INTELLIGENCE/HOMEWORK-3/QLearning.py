class Node:
    def __init__(self, startVal=0, north=None, east=None, south=None, west=None, special=False, QVals=[0, 0, 0, 0]):
        self.north = north
        self.east = east
        self.south = south
        self.west = west
        self.pol = startVal
        self.QVals = QVals
        self.special = special

    def defineDir(self, north=None, east=None, south=None, west=None):
        self.north = north
        self.east = east
        self.south = south
        self.west = west

    def newQVals(self):
        self.QVals = [round(0.7 * self.north.pol + 0.15 * self.east.pol + 0.15 * self.west.pol, 2),
                      round(0.7 * self.east.pol + 0.15 * self.south.pol + 0.15 * self.north.pol, 2),
                      round(0.7 * self.south.pol + 0.15 * self.east.pol + 0.15 * self.west.pol, 2),
                      round(0.7 * self.west.pol + 0.15 * self.north.pol + 0.15 * self.south.pol, 2)]
        return self.QVals

    def getQVals(self):
        return self.QVals

    def newPol(self, vals):
        return max(vals)


wall1 = Node(-1, special=True)
wall2 = Node(-1, special=True)
wall3 = Node(-1, special=True)
wall4 = Node(-1, special=True)
wall5 = Node(-1, special=True)
gridworld = [
    [wall1, Node(startVal=1, special=True), Node(0), wall2, wall3, wall4],
    [Node(0), Node(0), Node(0), Node(0), Node(startVal=1, special=True), Node(0)],
    [Node(startVal=-2, special=True), Node(0), wall5, Node(0), Node(startVal=-2, special=True), Node(0)],
    [Node(0), Node(0), Node(0), Node(0), Node(0), Node(0)]
]

for i in range(len(gridworld)):
    for j in range(len(gridworld[i])):
        if not gridworld[i][j].special:
            north = gridworld[i - 1][j] if i > 0 else wall1
            east = gridworld[i][j + 1] if j < len(gridworld[i]) - 1 else wall2
            south = gridworld[i + 1][j] if i < len(gridworld) - 1 else wall5
            west = gridworld[i][j - 1] if j > 0 else wall4
            gridworld[i][j].defineDir(north, east, south, west)


def Print():
    for i in gridworld:
        for j in i:
            print("{:.2f}".format(j.pol).rjust(5), end=" ")
        print("\n")

    print("\n\n")

    for i in gridworld:
        for j in i:
            vals = j.getQVals()
            print("{:.2f}".format(vals[0]).rjust(5), end=" ")
            print("{:.2f}".format(vals[1]).rjust(5), end=" ")
            print("{:.2f}".format(vals[2]).rjust(5), end=" ")
            print("{:.2f}".format(vals[3]).rjust(5), end=" ")
            print("|", end=" ")
        print("\n")

    print("\n\n\n\n")


def passThrough():
    global gridworld
    newQValues = [[node.QVals[:] for node in row] for row in gridworld]
    
    for i in range(len(gridworld)):
        for j in range(len(gridworld[i])):
            if not gridworld[i][j].special:
                newGrid = gridworld[i][j].newQVals()
                newQValues[i][j] = newGrid
                
    # Update the nodes with the new Q-values
    for i in range(len(gridworld)):
        for j in range(len(gridworld[i])):
            gridworld[i][j].QVals = newQValues[i][j]
            if not gridworld[i][j].special:
                newPol = gridworld[i][j].newPol(newQValues[i][j])
                gridworld[i][j].pol = newPol


if __name__ == "__main__":
    Print()
    passThrough()
    Print()
    passThrough()
    Print()
    for i in range(100):
        passThrough()
    Print()