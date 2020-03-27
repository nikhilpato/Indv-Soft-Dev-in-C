import random
import sys

class Othello:
    @staticmethod
    def getRandomMove():
        return "[" + str(random.randint(-1, 8)) + ',' + str(random.randint(-1, 8)) + "]"

    @staticmethod
    def moveCheck():
        return ("enterMove " + Othello.getRandomMove() +
                '\nsaveMove move1' +
                "\ncompareMove " + Othello.getRandomMove() +
                "\ndoMove " + Othello.getRandomMove() +
                "\napplyMove" +
                '\nenterMove [0,2]' +
                '\nloadMove move1' +
                "\napplyMove" +
                "\nsetOptions\ny\n" + str(random.randint(1, 100)) + "\n" +
                str(random.randint(1, 100)) + "\n" +
                str(random.randint(1, 100)) + "\n" +
                str(random.randint(1, 100)) + '\n')

class Checkers:
    @staticmethod
    def getRandomMove():
        letters = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'B', 'C', 'D', 'E', 'F', 'G', 'C', 'D', 'E', 'F', 'D', 'E',
                   '1', 'X']
        numbers = ['1', '2', '3', '4', '5', '6', '7', '8', '2', '3', '4', '5', '6', '7', '3', '4', '5', '6', '4', '5',
                   '12', '-1', 'A']
        return random.choice(letters) + random.choice(numbers) + " -> " + random.choice(letters) + random.choice(
            numbers)

    @staticmethod
    def moveCheck():
        return ("enterMove " + Checkers.getRandomMove() +
                '\nsaveMove move1' +
                "\ncompareMove " + Checkers.getRandomMove() +
                "\ndoMove " + Checkers.getRandomMove() +
                "\napplyMove" +
                '\nenterMove C1 -> D2' +
                '\nloadMove move1' +
                "\napplyMove" +
                "\nsetOptions\ny\n" + str(random.randint(1, 1000)) + "\n" +
                str(random.randint(1, 1000)) + "\n" +
                str(random.randint(1, 100)) + "\n")

class Mancala:
    @staticmethod
    def getRandomMove(count):
        if count % 2 == 1:
            return str(random.randint(-1, 6))
        else:
            return str(random.randint(6, 13))

    @staticmethod
    def moveCheck(count):
        return ("enterMove " + Mancala.getRandomMove(count) +
                '\nsaveMove move1' +
                "\ncompareMove " + Mancala.getRandomMove(count) +
                "\ndoMove " + Mancala.getRandomMove(count) +
                "\napplyMove" +
                "\napplyMove" +
                '\nenterMove 2' +
                '\nloadMove move1' +
                "\nsetOptions\ny\n" + str(random.randint(1, 1000)) + "\n" +
                str(random.randint(1, 500)) + "\n" +
                str(random.randint(1, 500)) + "\n")


def createFile(filename):
    seed = random.randrange(sys.maxsize)
    random.seed(seed)
    file = open(filename, "w+")
    standard_check = "showVal\nkeyMoveCount\nshowMoveHist\nshowBoard\n"
    num = 1000
    count = 1

    while num > 15:
        rand1 = random.randint(1, 9)
        rand2 = random.randint(1, 15)
        if filename[0] == 'O':
            file.write(Othello.moveCheck())
        elif filename[0] == 'C':
            file.write(Checkers.moveCheck())
        else:
            file.write(Mancala.moveCheck(count))

        file.write(
        standard_check +
        "saveBoard currentBoard\n" +
        "testRun " + str(rand1) + ' ' + str(rand2) + '\n' +
        "currentBoard\n" +
        standard_check  +
        "undoLastMove " + str(rand2) + '\n' +
        standard_check +
        'compareKeys currentBoard\n' +
        "loadBoard currentBoard\n" +
        standard_check +
        "testRun " + str(rand1) + ' ' + str(rand2) + '\n')

        num -= rand2
        count += 1

    file.write("quit")

    return seed


def format_seed_file(filename):
    return filename + " seed: " + str(createFile(filename)) + "\n"


if __name__ == "__main__":
    seed_file = open("seedFile.txt", "w")

    for i in range(1,4):
        seed_file.write(format_seed_file('Checkers.' + str(i)))
        seed_file.write(format_seed_file('Othello.' + str(i)))
        seed_file.write(format_seed_file('Mancala.' + str(i)))



