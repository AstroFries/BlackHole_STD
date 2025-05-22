import sympy as sp

# 定义符号变量
t, r, theta, phi = sp.symbols('t r theta phi', real=True)
M = sp.symbols('M', real=True, positive=True)  # 质量参数 M > 0

# 定义施瓦西度规 g_{\mu\nu}
g = sp.Matrix([
    [-(1 - 2*M/r), 0, 0, 0],
    [0, 1/(1 - 2*M/r), 0, 0],
    [0, 0, r**2, 0],
    [0, 0, 0, r**2 * sp.sin(theta)**2]
])

# 计算逆度规 g^{\mu\nu}
g_inv = g.inv()
print(g_inv)

# 定义坐标变量
coords = [t, r, theta, phi]

# 初始化 Christoffel 符号（4x4x4 张量）
Gamma = [[[0 for _ in range(4)] for _ in range(4)] for _ in range(4)]

# 计算 \Gamma^\mu_{\nu\rho}
for mu in range(4):
    for nu in range(4):
        for rho in range(4):
            term = 0
            for sigma in range(4):
                # 计算三个偏导数
                d1 = sp.diff(g[sigma, rho], coords[nu])  # \partial_\nu g_{\sigma\rho}
                d2 = sp.diff(g[sigma, nu], coords[rho])  # \partial_\rho g_{\sigma\nu}
                d3 = sp.diff(g[nu, rho], coords[sigma])  # \partial_\sigma g_{\nu\rho}
                
                # 组合成联络项
                term += 0.5 * g_inv[mu, sigma] * (d1 + d2 - d3)
            
            Gamma[mu][nu][rho] = sp.simplify(term)

# 打印非零联络系数
print("非零的 Christoffel 符号：")
for mu in range(4):
    for nu in range(4):
        for rho in range(4):
            if Gamma[mu][nu][rho] != 0:
                print(f"Gamma^{mu}_{nu, rho} = {Gamma[mu][nu][rho]}")