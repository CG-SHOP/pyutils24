import json
import typing

from networkx.utils import open_file


@open_file(0, mode="r")
def read_instance(path) -> typing.Dict:
    data = json.load(path)
    if data["type"] != "cgshop2024_instance":
        msg = "Not a CGSHOP2024 instance file"
        raise ValueError(msg)
    if not data["instance_name"] or not isinstance(data["instance_name"], str):
        msg = "Missing instance name"
        raise ValueError(msg)
    return data


@open_file(0, mode="r")
def read_solution(path) -> typing.Dict:
    data = json.load(path)
    return parse_solution(data)


class NoSolution(Exception):
    def __init__(self, msg):
        self.msg = msg

    def __str__(self):
        return self.msg


class BadSolutionFile(Exception):
    def __init__(self, msg):
        self.msg = msg

    def __str__(self):
        return self.msg


def parse_solution(data):
    """
    Parse the instance and return a perfectly formatted and typed solution
    or raise an exception (BadSolutionFile) if the solution is invalid.
    """
    # make sure this is a proper solution file
    if data["type"].lower() != "cgshop2024_solution":
        msg = "Not a CGSHOP2024 solution file"
        raise NoSolution(msg)
    if "id" in data and "instance" not in data:
        data["instance_name"] = data["id"]
    if "name" in data and "instance" not in data:
        data["instance_name"] = data["name"]
    if not data["instance_name"] or not isinstance(data["instance_name"], str):
        msg = "Missing instance name"
        raise BadSolutionFile(msg)
    # in case someone used the path to the instance instead of the name
    data["instance_name"] = data["instance_name"].split("/")[-1].split(".")[0]

    item_indices = data.get("item_indices", [])
    x_translations = data.get("x_translations", [])
    y_translations = data.get("y_translations", [])
    if (
        not isinstance(x_translations, list)
        or not isinstance(y_translations, list)
        or not isinstance(item_indices, list)
    ):
        msg = "Translations and item indices must be lists."
        raise BadSolutionFile(msg)
    if len(x_translations) != len(y_translations) or len(x_translations) != len(
        item_indices
    ):
        msg = "Translations and item indices must have the same length."
        raise BadSolutionFile(
            msg
        )

    for i, x, y in zip(item_indices, x_translations, y_translations, strict=True):
        if not isinstance(i, int):
            msg = "Item indices must be integers."
            raise BadSolutionFile(msg)
        if int(x) != x or int(y) != y:
            msg = "Translations must be integers."
            raise BadSolutionFile(msg)
        item_indices[i] = int(i)
        x_translations[i] = int(x)
        y_translations[i] = int(y)
    return {
        "instance_name": str(data["instance_name"]),
        "item_indices": item_indices,
        "x_translations": x_translations,
        "y_translations": y_translations,
    }
