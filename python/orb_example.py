import helper
import cf

orb = cf.Orbit()
orb.read("chaos_files/Brezel_1.orb")

print(  "\nName: ", orb.getName(),
        "\nNumber of factors: ", orb.getNumFactors(),
        "\nNumber of starting points: ", orb.getNumStartingPoints(),
        "\nInterval x min: ", orb.getRangeX().getMin(),
        "\nInterval x max: ", orb.getRangeX().getMax(),
        "\nInterval y min: ", orb.getRangeY().getMin(),
        "\nInterval y max: ", orb.getRangeY().getMax(),
        "\n\n")

print("Starting points:")
startingPoints = orb.getAllStartingPoints()
for sp in startingPoints:
    vec = helper.convertVec3ToArray(sp)
    print(vec)

print("\n\nFactors:")
factors = orb.getAllFactors()
for i in range(factors.size()):
    factor = factors[i]
    print(factor)

print("\n\n")


