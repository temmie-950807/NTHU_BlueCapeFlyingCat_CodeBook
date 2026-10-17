from fractions import Fraction as F
# s = 1 1 3 7 17 回傳 [2, 1]，代表 s[i] = 2*s[i-1] + 1*s[i-2]（跟 OEIS.cpp 順序相反）
def BM(s):
    n = len(s); L=m=0; C=[F(0)]*n; B=[F(0)]*n; C[0]=B[0]=F(1); b=F(1)
    for i in range(n):
        m+=1; d=sum(C[j]*s[i-j] for j in range(L+1))
        if d==0: continue
        T = C[:]; coef = F(d/b)
        for j in range(m, n): C[j] -= coef*B[j-m]
        if 2*L<=i: L, B, b, m = i+1-L, T, d, 0
    if len(C)<L+1: C += [F(0)] * (L+1-len(C))
    return [-x for x in C[1:L+1]]