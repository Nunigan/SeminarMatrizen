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

def winograd_inner(a, b):
    n = np.shape(a)[0]
    if n%2 == 0:
        xi = np.sum(a[::2]*a[1::2])
        etha = np.sum(b[::2]*b[1::2])
        # print("xi = {}, etha = {}".format(xi, etha))
        ab = np.sum((a[::2]+b[1::2])*(a[1::2]+b[::2]))-xi-etha
    else:
        xi = np.sum(a[0:-1:2]*a[1::2])
        etha = np.sum(b[0:-1:2]*b[1::2])
        ab = np.sum((a[0:-1:2]+b[1::2])*(a[1::2]+b[0:-1:2]))-xi-etha+a[-1]*b[-1]
    return ab

def winograd(A, B):
    m,n = np.shape(A)
    n2,p = np.shape(B)
    C = np.zeros((m,p))
    for i in range(np.shape(A)[0]):
        for j in range(np.shape(B)[1]):
            C[i,j] = winograd_inner(A[i,:], B[:,j])
    return C

def winograd2(A, B):
    m,n = np.shape(A)
    n2,p = np.shape(B)
    C = np.zeros((m,p))
    xi = np.zeros((n))
    eta = np.zeros((n))
    ab = 0
    
    for i in range(n):
        for j in range(n//2):
           xi[i] += A[i,2*j]*A[i,2*j+1]
           eta[i] += B[2*j,i]*B[2*j+1,i]

    if n%2==0:
        for i in range(n):
            for j in range(n):
                ab = 0
                for k in range(n//2):
                    ab += (A[i,2*k]+B[2*k+1,j])*(A[i,2*k+1]+B[2*k,j])
                C[i,j] = ab-eta[j]-xi[i]
    else:
        for i in range(n):
            for j in range(n):
                ab = 0
                for k in range(n//2):
                    ab += (A[i,2*k]+B[2*k+1,j])*(A[i,2*k+1]+B[2*k,j])
                C[i,j] = ab-eta[j]-xi[i]+A[i,-1]*B[-1,j]
        
    return C
        
def test_perfomance(n):
    t_mm = []
    t_mm_dc = []
    t_mm_strassen = []
    t_wino = []
    t_np = []

    for i in n:
        A = np.random.randn(i, i)
        B = np.random.randn(i, i)
        # A = np.random.randint(-100, 100,(i, i))
        # B = np.random.randint(-100, 100,(i, i))

        # start = time.time()
        # C3 = strassen(A, B)
        # t_mm_strassen.append(time.time() - start)

        start = time.time()
        C1 = MM(A, B)
        t_mm.append(time.time() - start)

        # start = time.time()
        # C2 = MM_dc(A, B)
        # t_mm_dc.append(time.time() - start)

        start = time.time()
        C4 = winograd2(A, B)
        t_wino.append(time.time() - start)

        start = time.time()
        C = A@B
        t_np.append(time.time() - start)

    plt.figure()
    plt.plot(n, t_mm, label='Standard MM', lw=5)
    # plt.plot(n, t_mm_dc, label='Divide and conquer MM', lw=5)
    # plt.plot(n, t_mm_strassen, label='Strassen MM', lw=5)
    plt.plot(n, t_wino, label='Winograd MM', lw=5)
    plt.plot(n, t_np, label='np MM', lw=5)
    plt.legend()
    return t_np

# test%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if __name__ == '__main__':
    # n = np.logspace(1,9,9,base=2,dtype=(np.int))
    n = np.arange(1,200,2)  
    # A = np.random.randint(-10, 10, (5,5))
    # B = np.random.randint(-10, 10, (5,5))

    # C = winograd2(A, B)
    # C_test = A@B

    # print(np.equal(C, C_test))

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