#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Mar 12 09:02:48 2021

@author: nunigan
"""

from datetime import datetime
import numpy as np

class Helper():
    def __init__(self):
        pass

    def write_c_matrix(self, n):
        A = np.random.randint(-10,10,(n,n))
        B = np.random.randint(-10,10,(n,n))
        C = A@B
        with open('c_matrix.h', 'w') as file:
            file.writelines('/* Seminar Matrizen, autogenerated File, Michael Schmid, {} */ \n \n'.format(datetime.now().strftime("%d/%m/%Y, %H:%M:%S")))

            file.writelines('#include <stdint.h> \n')
            file.writelines('const int n = {}; \n'.format(n))

            file.writelines('const int A[][{}] = \n'.format(n))
            file.writelines('  {\n')
            for i in range(n):
                file.writelines('    {')
                for j in range(n):
                    if j == n-1:
                        file.writelines('{}'.format(A[i,j]))
                    else:
                        file.writelines('{},'.format(A[i,j]))
                if i == n-1:
                    file.writelines('}\n')
                else:
                    file.writelines('},\n')

            file.writelines('  };\n')

            file.writelines('const int B[][{}] = \n'.format(n))
            file.writelines('  {\n')
            for i in range(n):
                file.writelines('    {')
                for j in range(n):
                    if j == n-1:
                        file.writelines('{}'.format(B[i,j]))
                    else:
                        file.writelines('{},'.format(B[i,j]))
                if i == n-1:
                    file.writelines('}\n')
                else:
                    file.writelines('},\n')

            file.writelines('  };\n')

        return C


# test%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if __name__ == '__main__':

    helper = Helper()
    C = helper.write_c_matrix(64)
