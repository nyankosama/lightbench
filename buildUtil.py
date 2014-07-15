
def isCppFile(fileName):
    return fileName.endswith(".cpp")

def isTestFile(fileName):
    return fileName.startswith("_TEST__")
