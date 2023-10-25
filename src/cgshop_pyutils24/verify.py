import typing
from .knapsack import Coordinate, Knapsack, QuadTree, Rectangle, translate, PackingError

def _to_coords(xs: typing.List[int], ys: typing.List[int]):
    return [Coordinate(x, y) for x, y in zip(xs, ys, strict=True)]

class InvalidSolution(Exception):
    def __init__(self, msg: str):
        self.msg = msg

    def __str__(self):
        return self.msg

def verify(instance: dict, solution: dict) -> int:
    # instance
    container = _to_coords(instance["container"]["x"], instance["container"]["y"])
    item_polys = [_to_coords(item["x"], item["y"]) for item in instance["items"]]
    item_values = [item["value"] for item in instance["items"]]
    item_quantities = [item["quantity"] for item in instance["items"]]

    knapsack = Knapsack(container)
    packed_value = 0
    # solution
    for i, x_translation, y_translation in zip(
        solution["item_indices"],
        solution["x_translations"],
        solution["y_translations"],
        strict=True,
    ):
        if item_quantities[i]<=0:
            raise InvalidSolution(f"Item {i} is used too often.")
        item_quantities[i] -= 1
        poly = translate(item_polys[i], Coordinate(x_translation, y_translation))
        try:
            knapsack.add_item(poly)  # will throw an exception if the item does not fit
        except PackingError as pe:
            raise InvalidSolution(f"Item {i} does not fit into the container") from pe
        packed_value += item_values[i]
    return packed_value

