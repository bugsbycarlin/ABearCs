import os

honey_version = "0_09"

env=Environment()
env.Append(CXXFLAGS = "-std=c++11 -mmacosx-version-min=10.10")

libraries = ["libHoney.a"]

env.AppendUnique(FRAMEWORKS = Split("CoreFoundation OpenGL SDL2 SDL2_image SDL2_mixer"))

ABearCs = env.Program(target = "ABearCs",
  source = Glob("./Source/*.cpp"),
  LIBS = libraries,
  LIBPATH="./Honey/%s/Library" % honey_version,
  CPPPATH="./Honey/%s/Includes" % honey_version)

Default()

def cleanObjects(target, source, env):
  os.system("rm ./Source/*.o")

cleanup = Command("cleanObjects", [], cleanObjects)
Depends(cleanup, BUILD_TARGETS)
BUILD_TARGETS.append("cleanObjects")