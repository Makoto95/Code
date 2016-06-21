from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import matplotlib.pyplot as plt
import numpy as np

# f1(x,y) = x^4 + y^4 - 2* x^2 * y
def f1(x, y):
    return x**4 + y**3 - 2*(x**2)*(y)

# ヘッセ行列の逆行列
def inverse_hesse_f1(xy):
    x = xy[0]
    y = xy[1]

    # ヘッセ行列を作る
    H = [[12*(x**2) - 4*y, -4*x],
         [-4*x, 12*(y**2)]]
    # その逆行列を返す
    # 大きな値が出ると例外で死ぬ対策
    try:
        return np.linalg.inv(H)
    except:
        return np.array([[None, None], [None, None]])

# ∇f = [∂f/∂x, ∂f/∂y]^T
def gradient_f1(xy):
    x = xy[0]
    y = xy[1]

    # [xの偏微分, yの偏微分]
    return np.array([4 * x**3 - 4*x*y, 4 * y**3 - 2 * x**2]);

# ニュートン法
# init_pos = 初期位置. e.g. (x, y)
# 近くにあるStationary pointに収束する（多分）。よって極値だけでなく鞍点に収束する可能性もある
def gradient_descent_method(gradient_f, inverse_hesse_f, init_pos, learning_rate):
    eps = 1e-10

    # 計算しやすいようnumpyのarrayとする e.g. 1x2行列
    init_pos = np.array(init_pos)
    pos = init_pos
    pos_history = [init_pos]
    iteration_max = 1000


    # 収束するか最大試行回数に達するまで
    for i in range(iteration_max):
        print(i+1, ":", pos)

        # ニュートン法
        # 教科書の初版で'+'となっているのは誤り。正しくは'-'
        # http://www.lr.pi.titech.ac.jp/~takamura/pubs/errata01satsu.pdf
        # 2x2の行列 と 1x2の行列の積をとると、numpyでは1x2の行列が返るようだ
        # 1x2行列を転置させた後、2x2の行列との積をとって、また転置をとれば、元の数式に従った計算になる
        pos_new = pos - learning_rate * inverse_hesse_f(pos).dot(gradient_f(pos))

        # 収束条件を満たせば終了
        # np.linalg.norm(): ユークリッド距離を計算する関数
        if abs(np.linalg.norm(pos - pos_new)) < eps:
            break

        pos = pos_new
        pos_history.append(pos)

    return (pos, np.array(pos_history))


def draw_contour(ax):
    # 等高線を描く
    n = 256
    x = np.linspace(0.6, 1.0, n)
    y = np.linspace(0.4, 1.0, n)
    X,Y = np.meshgrid(x, y)

    level_num = 20
    # 等高線で同じ高さとなるエリアを色分け
    ax.contourf(X, Y, f1(X, Y), level_num, alpha=.75, cmap=plt.cm.hot)
    # 等高線を引く
    C = ax.contour(X, Y, f1(X, Y), level_num, colors='black', linewidth=.5)
    ax.clabel(C, inline=1, fontsize=10)
    ax.set_title('contour')


def is_valid(num):
    return -5 < num < 5;

def main():
    learning_rates = [ 0.1, 0.2, 0.6, 1.0 ]

    # 収束する様子を表示するためのグラフ
    fig = plt.figure(3)

    for i, learning_rate in enumerate(learning_rates):
        ans, pos_history = gradient_descent_method(gradient_f1, inverse_hesse_f1, (0.7, 0.8), learning_rate)

        # subplotの場所を指定
        ax = plt.subplot(2, 2, (i+1)) # 2行2列の意味

        # 等高線を描画
        draw_contour(ax)

        # グラフのタイトル
        ax.set_title("learning rate: " + str(learning_rate) + ", iteration: " + str(len(pos_history)))

        # 移動した点を表示
        for pos in pos_history:
            if is_valid(pos[0]) and is_valid(pos[1]):
                ax.plot(pos[0], pos[1], 'o')

        # 点同士を線で結ぶ
        for i in range(len(pos_history)-1):
            x1 = pos_history[i][0]
            y1 = pos_history[i][1]
            x2 = pos_history[i+1][0]
            y2 = pos_history[i+1][1]
            if all([is_valid(v) for v in [x1, y1, x2, y2]]):
                ax.plot([x1, x2], [y1, y2], linestyle='-', linewidth=2)

    # タイトルが重ならないようにする
    fig.tight_layout()

    # 画像を表示
    plt.show()

    # 画像を保存
    fig.savefig('2d-newton-result.png')

main()
