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


def parse_solution(data: typing.Dict):
    """
    Parse the instance and return a perfectly formatted and typed solution
    or raise an exception (BadSolutionFile) if the solution is invalid.
    """
    if not isinstance(data, dict):
        msg = "Solution must be a dictionary. If your JSON-file is a list of solutions, please split it into multiple files."
        raise BadSolutionFile(msg)
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
        if not isinstance(x_translations, list):
            msg += f" Found {type(x_translations)!s} for x-translations."
        if not isinstance(y_translations, list):
            msg += f" Found {type(y_translations)!s} for y-translations."
        if not isinstance(item_indices, list):
            msg += f" Found {type(item_indices)!s} for item indices."
        raise BadSolutionFile(msg)
    if len(x_translations) != len(y_translations) or len(x_translations) != len(
        item_indices
    ):
        msg = "Translations and item indices must have the same length."
        msg += f" Found {len(x_translations)} x-translations,"
        msg += f"{len(y_translations)} y-translations, and {len(item_indices)} item indices."
        raise BadSolutionFile(msg)

    for list_pos, (item_idx, x_tl, y_tl) in enumerate(
        zip(item_indices, x_translations, y_translations)
    ):
        if not isinstance(item_idx, int):
            msg = "Item indices must be integers."
            msg += f" Found {type(item_idx)!s} at position {list_pos}."
            raise BadSolutionFile(msg)
        if int(x_tl) != x_tl or int(y_tl) != y_tl:
            msg = "Translations must be integers."
            msg += f" Found {type(x_tl)!s} for x-translation and {type(y_tl)!s} for y-translation at position {list_pos}."
            raise BadSolutionFile(msg)
        item_indices[list_pos] = int(item_idx)
        x_translations[list_pos] = int(x_tl)
        y_translations[list_pos] = int(y_tl)
    return {
        "instance_name": str(data["instance_name"]),
        "item_indices": item_indices,
        "x_translations": x_translations,
        "y_translations": y_translations,
    }
