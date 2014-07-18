import os
import buildUtil as util

srcDir = './src'
thirdPartyDir = './src/third_party'
testDir = './tests'

dynamicLibs = Split('pthread boost_program_options')
staticLibs = [File('./libbase.a'), File('./libmuduo.a')]
runlibs = list()
runlibs.extend(staticLibs)
runlibs.extend(dynamicLibs)

env = Environment(CCFLAGS='-std=c++0x -g -p -Wall -O0')

env.StaticLibrary(target = 'base',
        source = Glob(srcDir +'/lightbench/base/*.cpp'),
        CPPPATH = [srcDir, thirdPartyDir])

env.StaticLibrary(target = 'muduo',
        source = Glob(thirdPartyDir +'/muduo/base/*.cpp'),
        CPPPATH = thirdPartyDir)

libpath = './'

env.Program(target = 'lightbench',
        source = Glob(srcDir+'/lightbench/*.cpp'),
        LIBS=runlibs,
        LIBPATH=libpath,
        CPPPATH=[srcDir, thirdPartyDir])

#build and run the unittest cases
testSrcList = os.listdir(testDir)
testList = list()
for fileName in testSrcList:
    if util.isCppFile(fileName) is not True:
        continue
    
    testTarget = env.Program(target = testDir + os.path.sep + "_TEST__" + fileName[: fileName.index(".cpp")],
            source = testDir + os.path.sep + fileName,
            LIBS=runlibs,
            LIBPATH=libpath,
            CPPPATH=[srcDir, thirdPartyDir])
    testList.append(testTarget)

def runTest(target, source, env):
    testList = os.listdir(testDir)
    for fileName in testList:
        if util.isTestFile(fileName) == True:
            os.system(testDir + os.path.sep + fileName)

test = Command('test', [], runTest)
Depends(test, testList)
