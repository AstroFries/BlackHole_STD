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

a = sp.Symbol('a')
#a = sp.Rational(1,3)
rho2 = r**2 + a**2 * sp.cos(theta)**2
Delta = r**2 - 2*r + a**2




g_spherical_st = sp.Matrix([
    [1, 0, 0, 0],
    [0, r**2, 0, 0],
    [0, 0, r**2 * sp.sin(theta)**2, 0],
    [0, 0, 0, -1]
])
g_spherical = sp.Matrix([
    [1, 0, 0, 0],
    [0, r**2, 0, 0],
    [0, 0, r**2 * sp.sin(theta)**2, 0],
    [0, 0, 0, -1]
])

g_spherical = sp.Matrix([
    [1/(1-2/r), 0, 0, 0],
    [0, r**2, 0, 0],
    [0, 0, r**2 * sp.sin(theta)**2, 0],
    [0, 0, 0, -(1-2/r)]
])
g_spherical = sp.Matrix([
    [rho2 / Delta,                        0,                                    0,                               0],
    [0,                              rho2,                                    0,                               0],
    [0,                                  0,  (r**2 + a**2 + 2*a**2*r*sp.sin(theta)**2 / rho2) * sp.sin(theta)**2,   -2*a*r*sp.sin(theta)**2 / rho2],
    [0,                                  0,                                    -2*a*r*sp.sin(theta)**2 / rho2,      -(1 - 2*r/rho2)]
])
#print(g_spherical.applyfunc(trig_to_xyz))
#g_cartesian = J_sp.inv().T * g_spherical.applyfunc(trig_to_xyz) * J_sp.inv()
#g_cartesian = sp.simplify(g_cartesian)

# 1. 球坐标下求�?
g_spherical_inv = g_spherical.inv()

# 2. 变换到笛卡尔坐标�?
#g_cartesian_inv = J_sp * g_spherical_inv.applyfunc(trig_to_xyz) * J_sp.T
#g_cartesian_inv = sp.simplify(g_cartesian_inv)

# 合并时间分量


#print("笛卡尔坐标系下的度规矩阵")
#with open("python/output.tex", "w", encoding="utf-8") as f:
#    f.write("\\begin{equation}")
#    f.write(sp.latex(g_cartesian.applyfunc(trig_to_xyz)))
#    f.write("\\end{equation}")
#print(sp.pretty(g_cartesian))

#print("C语言函数输出至output_cfuncs.h")
#with open("python/output_cfuncs.h", "w", encoding="utf-8") as f:
#    for i in range(4):
#        for j in range(4):
#            func_name = f"g_cartesian_{i}{j}"
#            args = "double x, double y, double z, double t"
#            expr_c = sp.ccode(g_cartesian[i, j])
#            f.write(f"inline double {func_name}({args})"+" { return "+expr_c+"; }\n")
#            #print(f"inline double {func_name}({args})"+" { return "+expr_c+"; }")

coords_sph = [r, theta, phi, t]
g_spherical_inv = g_spherical.inv()
g_spherical_st_inv = g_spherical_st.inv()
Gamma_sph = {}

print("C语言函数输出至output_gauge_sph_inv.h")
with open("python/output_gauge_sph_inv.h", "w", encoding="utf-8") as f:
    f.write(f"#define a 0.9\n")
    f.write(f"#include <cmath>\n")
    f.write(f"using namespace std;\n")
    f.write(f"template <int Index> double g_sph(double r, double theta, double phi, double t);\n")
    for i in range(4):
        for j in range(4):
            func_name = f"template <> inline double g_sph<{4 * i + j}>"
            args = "double r, double theta, double phi, double t"
            expr_c = sp.ccode(g_spherical[i, j])
            f.write(f"{func_name}({args})"+" { return "+expr_c+"; }\n")
            #print(f"inline double {func_name}({args})"+" { return "+expr_c+"; }")

print("计算球坐标下的Christoffel符号...")



for i in range(4):
    for j in range(4):
        for k in range(4):
            s = 0
            for l in range(4):
                term = (
                    sp.diff(g_spherical[l, j], coords_sph[k]) +
                    sp.diff(g_spherical[l, k], coords_sph[j]) -
                    sp.diff(g_spherical[j, k], coords_sph[l])
                )
                s += g_spherical_inv[i, l] * term
                term = (
                    sp.diff(g_spherical_st[l, j], coords_sph[k]) +
                    sp.diff(g_spherical_st[l, k], coords_sph[j]) -
                    sp.diff(g_spherical_st[j, k], coords_sph[l])
                )
                s -= g_spherical_st_inv[i, l] * term
            Gamma_sph[(i, j, k)] = sp.simplify(0.5 * s)

print("C语言球坐标Christoffel函数输出至output_christoffel_sph.h")
with open("python/output_christoffel_sph.h", "w", encoding="utf-8") as f:
    f.write(f"#include <cmath>\n")
    f.write(f"#define a 0.9\n")
    f.write(f"using namespace std;\n")
    f.write(f"template <int Index> double gamma_sph(double r, double theta, double phi, double t);\n")
    for i in range(4):
        for j in range(4):
            for k in range(4):
                func_name = f"template <> inline double gamma_sph<{16 * i + 4 * j + k}>"
                args = "double r, double theta, double phi, double t"
                simpified_expr = sp.simplify(Gamma_sph[(i, j, k)])
                #expr_c = sp.ccode(Gamma_sph[(i, j, k)])
                expr_c = sp.ccode(simpified_expr)
                f.write(f"{func_name}({args})"+" { return "+expr_c+"; }\n")
                #print(f"inline double {func_name}({args})"+" { return "+expr_c+"; }")
            
print("C语言球坐标Christoffel函数输出至output_christoffel_sph.tex")
with open("python/output_christoffel_sph.tex", "w", encoding="utf-8") as f:
    f.write(r"\documentclass[a4paper, 12pt]{article}"+"\n")
    f.write(r"\usepackage{ctex}"+"\n")
    f.write(r"\usepackage{amsfonts}"+"\n")
    f.write(r"\usepackage{graphicx}"+"\n")
    f.write(r"\usepackage{amsmath,amssymb,amsfonts}"+"\n")
    f.write(r"\begin{document}"+"\n")
    for i in range(4):
        for j in range(4):
            for k in range(4):
                func_name = f"template <> inline double gamma_sph<{16 * i + 4 * j + k}>"
                args = "double r, double theta, double phi, double t"
                simpified_expr = sp.simplify(Gamma_sph[(i, j, k)])
                #expr_c = sp.ccode(Gamma_sph[(i, j, k)])
                expr_tex = sp.latex(simpified_expr)
                f.write(r"\[\Gamma"+r"^{" + f"{i}" +r"}_{\ " + f"{j}{k}" +r"}=" +expr_tex+r"\]"+"\n")
    
    f.write(r"\end{document}"+"\n")