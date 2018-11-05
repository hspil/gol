#!/usr/bin/python3

#############################################################################################
#
# Conway's Game of Life
#   *Toroidal Universe
#   *Standard B3/S23 rule by default. Adjustable below.
#
#############################################################################################

import os.path
import sys
import time


global h
global w

def main(argv):

    #B3/S23
    B = [3]
    S = [2,3]
    
    if not len(argv) == 4:
        print("Usage: ./gol.py <file> <board width> <board height>")
        return 1

    #file from command line
    file = argv[1]

    #pull width and height for board from command line argument and store into global width and height
    global w
    global h
    w = int(argv[2])
    h = int(argv[3])

    board = [[0 for x in range(w)] for y in range(h)]


    if not os.path.isfile("./" + file):
        setup = open(file, "w")

        for y in range(h):
            for x in range(w):
                setup.write('.')
            setup.write('\n')

        #close file
        setup.close()
        print(file + " file created.")
        print("Open the file in a text editor and change full stops to octothorpes before running this program again.")

    else:
        #read file and store into board array
        setup = open(file, "r")
        for y in range(h):
            for x in range(w):
                data = setup.read(1)
                #Don't store newline chars, read off another char
                if data == '\n':
                    data = setup.read(1)
                if data == '.':
                    board[y][x] = 0
                elif data == '#':
                    board[y][x] = 1

        #close file
        setup.close()

        #primary game loop
        while True:
            try:
                draw(board)
                board = change(board,B,S)
                time.sleep(0.25)
                print('\n\n\n')
            except:
                return 0


def change(board,B,S):    
    global h
    global w
    changes = [[0 for x in range(w)] for y in range(h)]

    #find what changes need to be made
    for y in range(h):
        for x in range(w):
            #count live neighbors
            neigh = 0
            for offy in range(-1, 2):
                for offx in range(-1, 2):
                    if not (offy == 0 and offx == 0):
                        #do not count self as a neighbor
                        neigh += board[(y + offy) % h][(x + offx) % w]

            #program in rule
            #birth
            if board[y][x] == 0 and neigh in B:
                changes[y][x] = 1
            #death
            if board[y][x] == 1 and not neigh in S:
                changes[y][x] = 1

    #xor array of changes needed with board and return new board
    for y in range(h):
        for x in range(w):
            board[y][x] = int(bool(board[y][x]) ^ bool(changes[y][x]))

    return board


def draw(board):
    global h
    global w
    for y in range(h):
        for x in range(w):
            if board[y][x] == 1:
                print('#', end="")
            elif board[y][x] == 0:
                print('.', end="")
        print('')

if __name__ == "__main__":
    main(sys.argv)
