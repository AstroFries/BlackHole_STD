import sympy as sp

x, y, z, t = sp.symbols('x y z t', real = True)
r, theta, phi = sp.symbols('r theta phi', real=True)
x_ = r * sp.sin(theta) * sp.cos(phi)
y_ = r * sp.sin(theta) * sp.sin(phi)
z_ = r * sp.cos(theta)
t_ = t

X = sp.Matrix([x_, y_, z_,t_])
vars = (r, theta, phi,t_)
J = X.jacobian(vars)  # 4x4

r_expr = sp.sqrt(x**2 + y**2 + z**2)

def trig_to_xyz(expr):
    expr = expr.replace(sp.sin(theta), sp.sqrt(x**2 + y**2) / sp.sqrt(x**2 + y**2 + z**2))
    expr = expr.replace(sp.cos(theta), z / sp.sqrt(x**2 + y**2 + z**2))
    expr = expr.replace(sp.sin(phi), y / sp.sqrt(x**2 + y**2))
    expr = expr.replace(sp.cos(phi), x / sp.sqrt(x**2 + y**2))
    expr = expr.replace(r, sp.sqrt(x**2 + y**2 + z**2))
    return expr

J_sp = J.applyfunc(trig_to_xyz)
#print(J)
#print(J_sp)
g_spherical = sp.Matrix([
    [1/sp.sqrt(1-2/r), 0, 0, 0],
    [0, r**2, 0, 0],
    [0, 0, r**2 * sp.sin(theta)**2, 0],
    [0, 0, 0, -sp.sqrt(1-2/r)]
])
print(g_spherical.applyfunc(trig_to_xyz))
g_cartesian = J_sp.inv().T * g_spherical.applyfunc(trig_to_xyz) * J_sp.inv()
g_cartesian = sp.simplify(g_cartesian)

# 合并时间分量


print("笛卡尔坐标系下的度规矩阵")
with open("python/output.tex", "w", encoding="utf-8") as f:
    f.write("\\begin{equation}")
    f.write(sp.latex(g_cartesian.applyfunc(trig_to_xyz)))
    f.write("\\end{equation}")
#print(sp.pretty(g_cartesian))