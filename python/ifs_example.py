import numpy as np

from libcfcg import cf
from libcfcg import helper

# read ifs file
ifs = cf.IteratedFunctionSystem()
ifs.read("chaos_files/Sierpinski.ifs")

print(  "\nName: ", ifs.getName(),
        "\nNumber of transformation: ", ifs.getNumTransformations(),
        "\nInterval x min: ", ifs.getRangeX().getMin(),
        "\nInterval x max: ", ifs.getRangeX().getMax(),
        "\nInterval y min: ", ifs.getRangeY().getMin(),
        "\nInterval y max: ", ifs.getRangeY().getMax(),
        "\n\n\n")


# access elements by hand...
# prefer conversion function and arrays, much easier =)
print("Transformation 0, at pos 0,0: ", cf.GlmMat3x3(ifs.getTransformation(0)).at(0, 0))

# use provided helper funtions (recomended)
print("print transformations in native python format: ")
for i in range(ifs.getNumTransformations()):
    t = ifs.getTransformation(i)
    array = helper.convertMat3x3ToArray(t)
    print(array)
    mat3x3 = helper.convertArrayToMat3x3(array)
    cf.printMat3x3(mat3x3.internalFormat()) # same as 'cf.printMat3x3(t)'


# simple matrix multiplication
matrix = helper.convertMat3x3ToArray(ifs.getTransformation(0))
squared_matrix = np.matmul(matrix, matrix)
converted_result = helper.convertArrayToMat3x3(squared_matrix)

print("\n\n\nMatrix multiplication of:\n", matrix, "\nand:\n", matrix,
        "\n\nequals:\n", squared_matrix)

print("\n\nConverted to internal format result: ")
cf.printMat3x3(converted_result.internalFormat())


# simple multiplication of matrix and vector
vec = [1, 2, 3]

print("Result from Matrix & vector multiplication:\n", np.matmul(vec, matrix))

converted_result = helper.convertArrayToVec3(np.matmul(vec, matrix))
print("\nConverted to internal format result: ")
cf.printVec3(converted_result.internalFormat())


cf.Console.waitKey()

