import random
from string import ascii_uppercase as alpha
from itertools import product

# A SOLUTION FOR EIGHT QUEENS PUZZLE
SIZE = 8
hash = list(map(lambda pair: "".join(pair), list(product(alpha, alpha))))


class Queen:
    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y

    def move(self, move: int = 1):
        if self.y + move == SIZE:
            self.y = (self.y + move) % (SIZE + 1)
        elif self.y + move == 0:
            self.y = SIZE
        else:
            self.y = (self.y + move) % (SIZE)


class Board:
    def __init__(self) -> None:
        self.pieces = []

    def add_pieces(self, piece: Queen):
        self.pieces.append(piece)

    def pieces_coor(self):
        return [[piece.x, piece.y] for piece in self.pieces]

    def __repr__(self):  # board representation with ASCII art
        board = (" " * 5 + "_" * (23 + 3 * (SIZE - 8)))
        for i in list(range(SIZE, 0, -1)):
            board += ("\n" + " " + str(i).center(3) + "|" + "".join("_Q|" if [j, i]
                                                                    in self.pieces_coor() else "__|" for j in list(range(1, SIZE + 1))))
        return board + "\n" + " " * 3 + "".join([letter.rjust(3) if order != 26 else " "
                                                 + letter.rjust(3) for order, letter in enumerate((list(alpha) + hash)[:SIZE])])


def main():
    global SIZE
    
    print("\nWELCOME,\nthis is about solution of 'eight queens puzzle' but also in any SIZE.\n")
    n = input("Enter the SIZE of the chess board: ")
    while not n.isdecimal() or int(n) < 4:
        n = input("Please enter a number bigger than 3: ")
    SIZE = int(n)
    while True:
        board = place_pieces()
        solve_puzzle(board)
        print(board)
        input("\n press enter for another solution")


def place_pieces() -> Board:
    board = Board()
    for column in range(1, SIZE + 1):
        board.add_pieces(Queen(column, random.randint(1, SIZE)))
    return board


def left_conflict(the_piece: Queen, board: Board) -> bool:
    pieces = board.pieces_coor()
    order = pieces.index([the_piece.x, the_piece.y])
    for piece in pieces[0:order]:
        if (piece[1] == the_piece.y or
            piece[0] + piece[1] == the_piece.x + the_piece.y or
                piece[0] - piece[1] == the_piece.x - the_piece.y):
            return True
    return False


def solve_puzzle(board: Board) -> None:
    while True:
        for piece in board.pieces:
            for counter in range(SIZE + 1):
                if left_conflict(piece, board):
                    piece.move()
                else:
                    break
            if counter == SIZE:
                board.pieces[0].move(-1)
                break
        if counter < SIZE:
            break


if __name__ == "__main__":
    main()
