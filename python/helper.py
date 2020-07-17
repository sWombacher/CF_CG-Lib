from libcfcg import cf

def convertVec3ToArray(vec3):
    cpy = cf.GlmVec3(vec3)
    return [cpy.at(0), cpy.at(1), cpy.at(2)]


def convertArrayToVec3(array):
    if (len(array) != 3):
        raise Exception("Unable to convert array to vec3")

    vec3 = cf.GlmVec3()
    for i in range(3):
        vec3.setVal(i, array[i])

    # TODO: save the mat in some container and return 'internalFormat' instead
    return vec3


def convertArrayToMat3x3(array):
    if (len(array) != 3):
        raise Exception("Unable to convert array to mat3x3")

    for i in range(3):
        if (len(array) != 3):
            raise Exception("Unable to convert array to mat3x3")

    mat3 = cf.GlmMat3x3()
    for row in range(3):
        for column in range(3):
            mat3.setVal(column, row, array[row][column])

    # TODO: save the mat in some container and return 'internalFormat' instead
    return mat3


def convertMat3x3ToArray(mat3x3):
    cpy = cf.GlmMat3x3(mat3x3)
    return [ [cpy.at(0, 0), cpy.at(1, 0), cpy.at(2, 0) ],
             [cpy.at(0, 1), cpy.at(1, 1), cpy.at(2, 1) ],
             [cpy.at(0, 2), cpy.at(1, 2), cpy.at(2, 2) ] ]

