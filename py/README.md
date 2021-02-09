## Python中的原子变量
- L.append(x)
- L1.extend(L2)
- x = L[i]
- x = L.pop()
- L1[i:j] = L2
- L.sort()
- x = y
- x.field = y
- D[x] = y
- D1.update(D2)
- D.keys()

## Python中的非原子变量
- i = i+1
- L.append(L[-1])
- L[i] = L[j]
- D[x] = D[x] + 1