#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Mar 19 07:31:29 2021

@author: nunigan
"""
import numpy as np
import time
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit


def MM(A, B):
    n = np.shape(A)[0]
    C = np.zeros((n, n))
    for i in range(n):
        for j in range(n):
            C[i, j] = 0
            for k in range(n):
                C[i, j] += A[i, k]*B[k, j]
    return C


def MM_dc(A, B):
    n = np.shape(A)[0]
    if(n <= 2):
        C = np.zeros((n, n))
        C[0, 0] = A[0, 0]*B[0, 0]+A[0, 1]*B[1, 0]
        C[0, 1] = A[0, 0]*B[0, 1]+A[0, 1]*B[1, 1]
        C[1, 0] = A[1, 0]*B[0, 0]+A[1, 1]*B[1, 0]
        C[1, 1] = A[1, 0]*B[0, 1]+A[1, 1]*B[1, 1]
        return C
    else:
        A11, A12, A21, A22 = A[:n//2, :n//2], A[:n//2, n//2:], A[n//2:, :n//2], A[n//2:, n//2:]
        B11, B12, B21, B22 = B[:n//2, :n//2], B[:n//2, n//2:], B[n//2:, :n//2], B[n//2:, n//2:]
        C11 = MM_dc(A11, B11) + MM_dc(A12, B21)
        C12 = MM_dc(A11, B12) + MM_dc(A12, B22)
        C21 = MM_dc(A21, B11) + MM_dc(A22, B21)
        C22 = MM_dc(A21, B12) + MM_dc(A22, B22)
        C = np.vstack((np.hstack((C11, C12)), np.hstack((C21, C22))))
    return C


def strassen(A, B):
    n = np.shape(A)[0]
    if(n <= 2):
        C = np.zeros((n, n))
        P = (A[0, 0]+A[1, 1])*(B[0, 0]+B[1, 1])
        Q = (A[1, 0]+A[1, 1])*B[0, 0]
        R = A[0, 0]*(B[0, 1]-B[1, 1])
        S = A[1, 1]*(B[1, 0]-B[0, 0])
        T = (A[0, 0]+A[0, 1])*B[1, 1]
        U = (A[1, 0]-A[0, 0])*(B[0, 0]+B[0, 1])
        V = (A[0, 1]-A[1, 1])*(B[1, 0]+B[1, 1])
        C[0, 0] = P+S-T+V
        C[0, 1] = R+T
        C[1, 0] = Q+S
        C[1, 1] = P+R-Q+U
        return C
    else:
        m = n//2
        A11, A12, A21, A22 = A[:m, :m], A[:m, m:], A[m:, :m], A[m:, m:]
        B11, B12, B21, B22 = B[:m, :m], B[:m, m:], B[m:, :m], B[m:, m:]
        C11 = strassen(A11, B11) + strassen(A12, B21)
        C12 = strassen(A11, B12) + strassen(A12, B22)
        C21 = strassen(A21, B11) + strassen(A22, B21)
        C22 = strassen(A21, B12) + strassen(A22, B22)
        C = np.vstack((np.hstack((C11, C12)), np.hstack((C21, C22))))
    return C


def test_perfomance(n):
    t_mm = []
    t_mm_dc = []
    t_mm_strassen = []
    t_np = []

    for i in n:
        A = np.random.randn(i, i)
        B = np.random.randn(i, i)

        start = time.time()
        C3 = strassen(A, B)
        t_mm_strassen.append(time.time() - start)

        start = time.time()
        C1 = MM(A, B)
        t_mm.append(time.time() - start)

        start = time.time()
        C2 = MM_dc(A, B)
        t_mm_dc.append(time.time() - start)


        start = time.time()
        C = A@B
        t_np.append(time.time() - start)

    plt.figure()
    plt.plot(n, t_mm, label='Standard MM', lw=5)
    plt.plot(n, t_mm_dc, label='Divide and conquer MM', lw=5)
    plt.plot(n, t_mm_strassen, label='Strassen MM', lw=5)
    plt.plot(n, t_np, label='np MM', lw=5)
    plt.legend()
    return t_np

# test%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if __name__ == '__main__':
    n = np.logspace(1,9,9,base=2,dtype=(np.int))
    
    # A = np.random.randint(-10, 10, (64,64))
    # B = np.random.randint(-10, 10, (64,64))

    t_np = test_perfomance(n)
    # C = strassen(A, B)
    # C_test = A@B
    
    # def func(x, a):
    #     return x**a

    # popt, pcov = curve_fit(func, n, t_np, bounds=(2, 3))


    # plt.figure()
    # plt.plot(n, t_np, 'b-', label='data')
    # plt.plot(n, func(n, *popt), 'r-', label='fit: a=%5.3f' % tuple(popt))
    # plt.xlabel('x')
    # plt.ylabel('y')
    # plt.legend()