import sys
from pathlib import Path

this_directory = Path(__file__).parent
readme = (this_directory / "README.md").read_text()

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
    name="flexbox",
    version="0.0.2",
    description="FlexBox layout engine for Python",
    long_description=readme,
    long_description_content_type="text/markdown",
    author="Thijs Vogels",
    url="https://github.com/tvogels/flexbox",
    license="MIT",
    packages=find_packages(where="src"),
    package_dir={"": "src"},
    cmake_install_dir="src/flexbox",
    include_package_data=True,
    install_requires=["domtree>=0.0.4", "typing_extensions>=4", "numpy"],
    extras_require={"test": ["pytest"]},
    python_requires=">=3.7",
)
