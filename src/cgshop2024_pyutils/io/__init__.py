"""
Just simple parsing of json files, including some checking and cleaning.
"""
# flake8: noqa F401
from .read import (
    BadSolutionFile,
    NoSolution,
    parse_solution,
    read_instance,
    read_solution,
)

__all__ = [
    "read_instance",
    "read_solution",
    "NoSolution",
    "BadSolutionFile",
    "parse_solution",
]
