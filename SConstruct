import os
import buildUtil as util

srcDir = './src'
testDir = './tests'

env = Environment(CCFLAGS='-std=c++0x -g')

env.StaticLibrary(target = 'base',
        source = Glob(srcDir +'/lightbench/base/*.cpp'),
        CPPPATH = srcDir)

libs = Split('pthread base boost_program_options')
libpath = './'

env.Program(target = 'lightbench',
        source = Glob(srcDir+'/lightbench/*.cpp'),
        LIBS=libs,
        LIBPATH=libpath,
        CPPPATH=srcDir)

#build and run the unittest cases
testSrcList = os.listdir(testDir)
testList = list()
for fileName in testSrcList:
    if util.isCppFile(fileName) is not True:
        continue
    
    testTarget = env.Program(target = testDir + os.path.sep + "_TEST__" + fileName[: fileName.index(".cpp")],
            source = testDir + os.path.sep + fileName,
            LIBS=libs,
            LIBPATH=libpath,
            CPPPATH=srcDir)
    testList.append(testTarget)

def runTest(target, source, env):
    testList = os.listdir(testDir)
    for fileName in testList:
        if util.isTestFile(fileName) == True:
            os.system(testDir + os.path.sep + fileName)

test = Command('test', [], runTest)
Depends(test, testList)
