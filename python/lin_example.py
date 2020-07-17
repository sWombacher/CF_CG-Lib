from libcfcg import cf

lin = cf.LindenmayerSystem()
lin.read("chaos_files/Koch_kurve.lin")

print(  "\nName: ", lin.getName(),
        "\nAxiom: ", lin.getAxiom(),
        "\nNumber of production: ", lin.getNumProductions(),
        "\nClear window each time?: ", lin.clearWindowEachTime(),
        "\nStart angle: ", lin.getStartAngle(),
        "\nAdjustment angle: ", lin.getAdjustmentAngle(),
        "\nScale: ", lin.getScale(),
        "\nRange X: ", [ lin.getRangeX().getMin(), lin.getRangeX().getMax() ],
        "\nRange Y: ", [ lin.getRangeY().getMin(), lin.getRangeY().getMax() ],
        "\n\n")

controller = cf.LSystem_Controller(2, lin)
print("String to iterate:\n", controller.getCompleteString(), "\n")


production = lin.getProduction('F')
if production:
    print("Prdouction for 'F' found:\n", cf.Dereference(production))
else:
    print("Prdouction for 'F' not found")

print("\n")
cf.Console.waitKey()

