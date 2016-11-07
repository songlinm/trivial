import unittest
import copy


class Path:
    def __init__(self, words=[]):
        self.words=words

    def appendWord(self, word):
        self.words += [word]
        return self

    def insertWord(self, word):
        self.words = [word] + self.words
        return self

    def insert(self, path):
        self.words = path.words + self.words
        return self

    def append(self, path):
        self.words += path.words
        return self

    def getWordList(self):
        return self.words

    def __str__(self):
        ret = '[ '
        for word in self.words:
            ret += word
            ret += ' '
        ret += ' ]'
        return ret;

    def __eq__(self, other):
        return self.words == other.words

    def __hash__(self):
        return hash(str(self.words))
        
    def __lt__(self, other):
        return self.words < other.words


class PathCollection:
    def __init__(self):
        self.paths=[]

    def appendPath(self, path):
        self.paths.append(path)
        return self

    def __str__(self):
        ret = '[ '
        for path in self.paths:
            ret += path.__str__()
            ret += ' '
        ret += ' ]'
        return ret;

    def allPaths(self):
        return sorted(list(set(self.paths)))


def checkRange(rec, i, j):
    if rec[i][j-1].allPaths(): # (i, i+inc-1) is constructable
        if rec[j][j].allPaths():
            # for each constructable path between [i, j-1], add [j] to make it a path
            # for [i, j]
            for path in rec[i][j-1].allPaths():
                rec[i][j].appendPath(
                    copy.deepcopy(path).append(rec[j][j].allPaths()[0]))

    if rec[i][i].allPaths():
        if rec[i+1][j].allPaths():
            # for each constructable path between [i+1, j], add [i] to make it a path
            # for [i, j]
            for path in rec[i+1][j].allPaths():
                rec[i][j].appendPath(
                    copy.deepcopy(path).insert(rec[i][i].allPaths()[0]))

    for k in range(1, j-i-1):
        if rec[i][i+k].allPaths() and rec[i+k+1][j].allPaths():
            # both [i, i+k] and [i+k+1, j] are constructable. So [i, j] are 
            # constructable with combinations from each sub list
            for path1 in rec[i][i+k].allPaths():
                for path2 in rec[i+k+1][j].allPaths():
                    rec[i][j].appendPath(copy.deepcopy(path1).append(path2))


def wordBreaking(word, dictionary):
    rec = [[ PathCollection() for c in word] for c in word]
    for i in range(len(word)):
        for j in range(i, len(word)):
            if word[i:j+1] in dictionary:
                # Each element is a list of list
                rec[i][j].appendPath(Path([word[i:j+1]]))

    for increment in range(1, len(word)):
        for i in range(len(word)):
            if i + increment < len(word):
                checkRange(rec, i, i+increment);
    return rec[0][len(word)-1]


def printRecord(record):
    [[print('({0}, {1}) = {2}'.format(rn, ln, c.__str__())) 
        for ln, c in enumerate(row)] for rn, row in enumerate(record)];
    print('-------------------')

def printAllPath(pathCollection):
    print('====================')
    for path in pathCollection.allPaths():
        print(path.__str__())
    print('====================')

class TestKite(unittest.TestCase):

    def test1(self):
        self.dictionary = {'a' : 1, 'abcd' : 1, 'ef' : 1}
        self.teststring = 'abcda'

        self.paths = wordBreaking(self.teststring, self.dictionary);
        self.assertEqual(len(self.paths.allPaths()), 1);
        self.assertEqual(self.paths.allPaths()[0].getWordList(), ['abcd', 'a']);

        self.paths = wordBreaking('aabcd', self.dictionary);
        self.assertEqual(len(self.paths.allPaths()), 1);
        self.assertEqual(self.paths.allPaths()[0].getWordList(), ['a', 'abcd']);

        self.paths = wordBreaking('aabcdef', self.dictionary);
        self.assertEqual(len(self.paths.allPaths()), 1);
        self.assertEqual(self.paths.allPaths()[0].getWordList(), ['a', 'abcd', 'ef'])

        self.dictionary = {'a': 1, 'ab' : 1, 'cda' : 1, 'abcd' : 1, 'bcda': 1, 'ef' : 1}
        self.paths = wordBreaking('abcda', self.dictionary);
        self.assertEqual(len(self.paths.allPaths()), 3);
        self.assertEqual(self.paths.allPaths()[0].getWordList(), ['a', 'bcda'])
        self.assertEqual(self.paths.allPaths()[1].getWordList(), ['ab', 'cda'])
        self.assertEqual(self.paths.allPaths()[2].getWordList(), ['abcd', 'a'])

        self.dictionary = {'bed':1, 'bath':1, 'and':1, 'beyond':1, 'bat':1, 'hand':1}
        self.paths = wordBreaking('bedbathandbeyond', self.dictionary);
        self.assertEqual(len(self.paths.allPaths()), 2);
        self.assertEqual(self.paths.allPaths()[0].getWordList(), ['bed', 'bat', 'hand', 'beyond'])
        self.assertEqual(self.paths.allPaths()[1].getWordList(), ['bed', 'bath', 'and', 'beyond'])


if __name__ == "__main__":
    unittest.main()
