"""
Just simple parsing of json files, including some checking and cleaning.
"""
# flake8: noqa F401
from .read import read_instance, read_solution, NoSolution, BadSolutionFile, parse_solution

__all__ = ["read_instance", "read_solution", "NoSolution", "BadSolutionFile", "parse_solution"]