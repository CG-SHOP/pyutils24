from setuptools import find_packages
from skbuild_conan import setup


def readme():
    """
    :return: Content of README.md
    """
    with open("README.md") as file:
        return file.read()


setup(  # https://scikit-build.readthedocs.io/en/latest/usage.html#setup-options
    name="cgshop2024_pyutils",
    version="1.1.1",
    author="Dominik Krupke",
    license="LICENSE",
    description="Utilities for verifying solutions of the CG:SHOP 2024 Competition.",
    long_description=readme(),
    long_description_content_type="text/markdown",
    packages=find_packages("src"),  # Include all packages in `./src`.
    package_dir={"": "src"},  # The root for our python package is in `./src`.
    python_requires=">=3.7",  # lowest python version supported.
    install_requires=[
        "matplotlib",
        "numpy",
        "chardet>=4.0.0",
        "networkx>=2.0.0",
    ],  # Python Dependencies
    conan_requirements=["fmt/[>=10.0.0]", "cgal/[>=5.6]"],  # C++ Dependencies
    cmake_minimum_required_version="3.23",
)
