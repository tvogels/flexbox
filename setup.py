import sys

try:
    from skbuild import setup
except ImportError:
    print(
        "Please update pip, you need pip 10 or greater,\n"
        " or you need to install the PEP 518 requirements in pyproject.toml yourself",
        file=sys.stderr,
    )
    raise

from setuptools import find_packages

setup(
    name="yoga",
    version="0.0.1",
    description="FlexBox layout engine for Python",
    author="Thijs Vogels",
    license="MIT",
    packages=find_packages(where="src"),
    package_dir={"": "src"},
    cmake_install_dir="src/yoga",
    include_package_data=True,
    install_requires=["domtree>=0.0.4", "typing_extensions>=4", "numpy"],
    extras_require={"test": ["pytest"]},
    python_requires=">=3.7",
)
