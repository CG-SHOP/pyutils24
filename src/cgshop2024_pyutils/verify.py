import typing

from .common import Coordinate, PackingError, translate
from .knapsack_verifier import Knapsack
from .intersection_verifier import IntersectionChecker


def _to_coords(xs: typing.List[int], ys: typing.List[int]):
    return [Coordinate(x, y) for x, y in zip(xs, ys)]


class InvalidSolution(Exception):
    def __init__(self, msg: str):
        self.msg = msg

    def __str__(self):
        return self.msg


def verify(instance: typing.Dict, solution: typing.Dict, slow: bool = False) -> int:
    # instance
    container = _to_coords(instance["container"]["x"], instance["container"]["y"])
    item_polys = [_to_coords(item["x"], item["y"]) for item in instance["items"]]
    item_values = [item["value"] for item in instance["items"]]
    item_quantities = [item["quantity"] for item in instance["items"]]


    if slow:
        # really simple and unoptimized verifier. should however yield the same results.
        packing_checker = IntersectionChecker(container)
    else:
        packing_checker = Knapsack(container)

    packed_value = 0
    # solution
    for i, x_translation, y_translation in zip(
        solution["item_indices"],
        solution["x_translations"],
        solution["y_translations"],
    ):
        if not (0 <= i < len(item_polys)):
            msg = f"Item index {i} is out of bounds."
            raise InvalidSolution(msg)
        if item_quantities[i] <= 0:
            msg = f"Item {i} is used too often."
            raise InvalidSolution(msg)
        item_quantities[i] -= 1
        poly = translate(item_polys[i], Coordinate(x_translation, y_translation))
        try:
            packing_checker.add_item(poly)
            # will throw an exception if the item does not fit
        except PackingError as pe:
            msg = f"Item {i} does not fit: '{pe!s}'"
            raise InvalidSolution(msg) from pe
        packed_value += item_values[i]
    return packed_value
