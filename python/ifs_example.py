import numpy as np
from libcfcg import cf
from libcfcg import helper

# read ifs file
ifs = cf.IteratedFunctionSystem()
ifs.read("chaos_files/Sierpinski.ifs")

print(  "\nName: ", ifs.getName(),
        "\nNumber of transformations: ", ifs.getNumTransformations(),
        "\nInterval x-min: ", ifs.getRangeX().getMin(),
        "\nInterval x-max: ", ifs.getRangeX().getMax(),
        "\nInterval y-min: ", ifs.getRangeY().getMin(),
        "\nInterval y-max: ", ifs.getRangeY().getMax(),
        "\n")

# access to elements by hand ...
# ... or use provided helper functions (recommended):
print("Transf. 1 (second transf.!!!)\n   at position [column=0, line=1]: ", 
      cf.GlmMat3x3(ifs.getTransformation(1)).at(0, 1))

print("\nPrint transformations in native python format: ") 
      # (line 0, line 1, line 2 ... of the matrix)
for i in range(ifs.getNumTransformations()):
    t = ifs.getTransformation(i)
    array = helper.convertMat3x3ToArray(t)
    print(array)
    mat3x3 = helper.convertArrayToMat3x3(array)
    print("value_0_1 =", mat3x3.at(0,1)) # column 0, line 1

# multiplication:  matrix * matrix
matrix0 = helper.convertMat3x3ToArray(ifs.getTransformation(0))
matrix1 = helper.convertMat3x3ToArray(ifs.getTransformation(1))
result_matrix = np.matmul(matrix0, matrix1)
print("\nMatrix multiplication of:\n", matrix0, "\nand:\n", matrix1,
        "\nequals:\n", result_matrix)
      # (line 0, line 1, line 2 ... of the matrix)

# define a vector with 3 components 
# (e.g. a 2D-point in homogeneous coordinates:  last component = 1 !!!)
vector = [3, 2, 1]

# multiplication:  matrix * vector
result_vector = np.matmul(matrix0, vector)
print("\nResult of multiplication: matrix * vector:\n", result_vector)

window = None

