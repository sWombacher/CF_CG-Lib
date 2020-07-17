import numpy as np

from libcfcg import cf
from libcfcg import helper


gen = cf.GEN()
gen.read("chaos_files/BUSCH_01.GEN")

print( "\nName: ", gen.getName(),
        "\nClear window each time: ", gen.clearWindowEachTime(),
        "\n",
        "\nRange X min: ", gen.getRangeX().min,
        "\nRange X max: ", gen.getRangeX().max,
        "\nRange Y min: ", gen.getRangeY().min,
        "\nRange Y max: ", gen.getRangeY().max,
        "\n",
        "\nNumber of initiator points: ", gen.getInitiatorPoints().size(),
        "\nNumber of generator points: ", gen.getGeneratorPoints().size()
        )

# iterate above all initiator points and print them
print("\n\nInitiator points:")
for ip in gen.getInitiatorPoints():
    point = helper.convertVec3ToArray(ip)
    print("Initiator point: ", point)

# iterate above all generator points and print them
print("\n\nGenerator points:")
for gp in gen.getGeneratorPoints():
    point = helper.convertVec3ToArray(gp)
    print("Generator point: ", point)

cf.Console.waitKey()

