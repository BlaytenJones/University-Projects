import collections
from functools import cmp_to_key
# A-Priori Algorithm to find frequent pairs and triples (support s = 100)
# threshold to be a frequent itemset
supportThreshold = 100
# maps an item to an integer number in the numbering system
itemTable = {}
# the entry for i will be either -1 if item i is not frequent, or the item index if the item is frequent 
frequentItemTable = []
# First Pass of A-Priori
with open("./browsing.txt") as f:
    # items will be labeled from 0
    itemIndex = 0
    # the ith element of this list will have the count of the ith item
    itemCounts = []
    for line in f:
        basket = line.split()
        for item in basket:
            if item not in itemTable:
                itemTable[item] = itemIndex
                itemIndex += 1
                itemCounts.append(1)
            else:
                itemCounts[itemTable[item]] += 1
    # Between Passes of A-Priori, find frequent items
    itemIndex = 0
    for i in range(len(itemCounts)):
        if itemCounts[i] >= supportThreshold:
            frequentItemTable.append(itemIndex)
            itemIndex += 1
        else:
            frequentItemTable.append(-1)

frequentPairs = {}
pairsCounts = {}
# Second Pass of A-Priori
with open("./browsing.txt") as f:
    for line in f:
        basket = line.split()
        basketFreqItems = [item for item in basket if frequentItemTable[itemTable[item]] != -1]
        # make sure in items lexicographic order so that pairs counting doesn't vary between baskets
        basketFreqItems.sort()
        # counts the occurrences of pairs of frequent items (uses the actual item names in the keys)
        for i in range(len(basketFreqItems)):
            item1 = basketFreqItems[i]
            for j in range(i + 1, len(basketFreqItems)):
                item2 = basketFreqItems[j]
                if (item1, item2) not in pairsCounts:
                    pairsCounts[(item1, item2)] = 1
                else:
                    pairsCounts[(item1, item2)] += 1
    # End of the Second Pass of A-Priori
    frequentPairs = [pair for pair in pairsCounts.keys() if pairsCounts[pair] >= supportThreshold] 
    # Computing Confidence Scores
    assoRulesConfScores = []
    for pair in frequentPairs:
        item1 = pair[0]
        item2 = pair[1]
        supportPair = pairsCounts[pair]
        supportItem1 = itemCounts[itemTable[item1]]
        supportItem2 = itemCounts[itemTable[item2]]
        #conf(item1 -> item2)
        assoRulesConfScores.append((item1, item2, float(supportPair)/supportItem1))
        #conf(item2 -> item1)
        assoRulesConfScores.append((item2, item1, float(supportPair)/supportItem2))
    # Compares rules to sort them by descending order of confidence scores, with ties broken
    # by lexicographically increasing order on the left hand side of the rule
    def compareScoresPairRules(ruleOne, ruleTwo):
        if ruleOne[2] > ruleTwo[2]:
            return -1
        elif ruleOne[2] < ruleTwo[2]:
            return 1
        # breaking ties
        elif ruleOne[0] < ruleTwo[0]:
            return -1
        else:
            return 1
    assoRulesConfScores.sort(key=cmp_to_key(compareScoresPairRules))
    # Output The Rules and their Scores
    outputFile = open("RulesPairs.txt", "w")
    for rule in assoRulesConfScores:
        outputFile.write("%s => %s : %f\n" % rule)
    outputFile.close()

triplesCounts = {}
# Third Pass of A-Priori (to find frequent triples)
with open("./browsing.txt") as f:
    for line in f:
        basket = line.split()
        basketFreqItems = [item for item in basket if frequentItemTable[itemTable[item]] != -1]
        # make sure in items lexicographic order so that pairs counting doesn't vary between baskets
        basketFreqItems.sort()
        # counts the occurrences of triples of items such that any two items of which are a frequent pair
        # (uses the actual item names in the keys)
        for i in range(len(basketFreqItems)):
            item1 = basketFreqItems[i]
            for j in range(i + 1, len(basketFreqItems)):
                item2 = basketFreqItems[j]
                for k in range(j + 1, len(basketFreqItems)):
                    item3 = basketFreqItems[k]
                    # test if the 3 items are possible by examining their pairs
                    if all(pair in frequentPairs for pair in [(item1, item2), (item1, item3), (item2, item3)]):
                        if (item1, item2, item3) not in triplesCounts:
                            triplesCounts[(item1, item2, item3)] = 1
                        else:
                            triplesCounts[(item1, item2, item3)] += 1
    # End of the Third Pass of A-Priori
    frequentTriples = [triple for triple in triplesCounts.keys() if triplesCounts[triple] >= supportThreshold]
    # Computing Confidence Scores
    assoRulesConfScores = []
    for triple in frequentTriples:
        item1 = triple[0]
        item2 = triple[1]
        item3 = triple[2]
        supportTriple = triplesCounts[triple]
        supportItem1and2 = pairsCounts[(item1, item2)]
        supportItem1and3 = pairsCounts[(item1, item3)]
        supportItem2and3 = pairsCounts[(item2, item3)]
        #conf(item1, item2 -> item3)
        assoRulesConfScores.append((item1, item2, item3, float(supportTriple)/supportItem1and2))
        #conf(item1, item3 -> item2)
        assoRulesConfScores.append((item1, item3, item2, float(supportTriple)/supportItem1and3))
        #conf(item2, item3 -> item1)
        assoRulesConfScores.append((item2, item3, item1, float(supportTriple)/supportItem2and3))
    # Compares rules to sort them by descending order of confidence scores, with ties broken
    # by lexicographically increasing order of the first and then second item in the pair
    def compareScoresTripleRules(ruleOne, ruleTwo):
        if ruleOne[3] > ruleTwo[3]:
            return -1
        elif ruleOne[3] < ruleTwo[3]:
            return 1
        # breaking ties. First try the first item in the pair
        elif ruleOne[0] < ruleTwo[0]:
            return -1
        elif ruleOne[0] > ruleTwo[0]:
            return 1
        # breaking ties.  Then try the second item in the pair
        elif ruleOne[1] < ruleTwo[1]:
            return -1
        else:
            return 1
    assoRulesConfScores.sort(key=cmp_to_key(compareScoresTripleRules))
    # Output the rules and their scores
    outputFile = open("RulesTriples.txt", "w")
    for rule in assoRulesConfScores:
        outputFile.write("(%s, %s) => %s : %f\n" % rule)
    outputFile.close()