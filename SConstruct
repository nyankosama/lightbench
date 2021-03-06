import os
import buildUtil as util

srcDir = './src'
thirdPartyDir = './src/third_party'
testDir = './tests'

dynamicLibs = Split('pthread boost_program_options')
staticLibs = [File('./libmuduo.a')]
runlibs = list()
runlibs.extend(staticLibs)
runlibs.extend(dynamicLibs)

env = Environment(CCFLAGS='-std=c++0x -g -p -Wall')

env.StaticLibrary(target = 'muduo',
        source = Glob(thirdPartyDir +'/muduo/base/*.cpp'),
        CPPPATH = thirdPartyDir)

libpath = './'

binCom = env.Program(target = 'lightbench',
        source = Glob(srcDir+'/lightbench/*.cpp'),
        LIBS=runlibs,
        LIBPATH=libpath,
        CPPPATH=[srcDir, thirdPartyDir])

env.Program(target = 'server', source = srcDir + '/lightbench/testserver/server.cpp',CPPPATH=[srcDir, thirdPartyDir])

testCom = env.Program(
    target = 'client', source = [srcDir + '/lightbench/testserver/testClient.cpp', srcDir + '/lightbench/bench_utils.cpp'],
    LIBS=runlibs,
    LIBPATH=libpath,
    CPPPATH=[srcDir, thirdPartyDir])

Depends(testCom, binCom)

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
Depends(test, testCom)
