import matplotlib.colors as mpc
import matplotlib.pyplot as plt
from matplotlib.patches import PathPatch
from matplotlib.path import Path


def patchify(poly, **kwargs):
    """from: https://stackoverflow.com/questions/8919719/how-to-plot-a-complex-polygon"""

    def ring_coding(n):
        """Returns a list of len(n) of this format:
        [MOVETO, LINETO, LINETO, ..., LINETO, LINETO, CLOSEPOLY]
        """
        codes = [Path.LINETO] * n
        codes[0] = Path.MOVETO
        codes[-1] = Path.CLOSEPOLY
        return codes

    codes = list(ring_coding(len(poly)))
    return PathPatch(Path(poly, codes), **kwargs)


def autocolor(i, n):
    return mpc.hsv_to_rgb((i / n, 1, 1))


def plot_poly(ax: plt.Axes, poly, **kwargs):
    patch = patchify(poly, **kwargs)
    ax.add_patch(patch)


def translate_format(poly, xo, yo):
    return [(int(x) + xo, int(y) + yo) for x, y in zip(poly[0], poly[1])]


def plot_solution(ax: plt.Axes, instance, solution):
    for idx, xo, yo in zip(
        solution["item_indices"], solution["x_translations"], solution["y_translations"]
    ):
        fpoly = instance["items"][idx]
        num_instance_polys = instance["num_items"]
        translated_poly = translate_format((fpoly["x"], fpoly["y"]), xo, yo)
        color = autocolor(idx, num_instance_polys)
        plot_poly(
            ax,
            translated_poly,
            lw=1,
            ec=mpc.to_rgba(color, 0.8),
            fc=mpc.to_rgba(color, 0.3),
            zorder=0,
        )
    container = instance["container"]
    container["x"].append(container["x"][0])
    container["y"].append(container["y"][0])
    ax.plot(container["x"], container["y"], color="black", lw=1)
    ax.set_aspect("equal", "box")
