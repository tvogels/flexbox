import pathlib
from typing import Any, Callable, NamedTuple, Optional, TypeVar

import domtree
import numpy as np

from ._core import *



def build_index(layout: "Layout", tree: domtree.Node, layouts: dict, prefix=[], index={}):
    name = "/" + "/".join(prefix)
    index[name] = Box.wrap(layout, name, layouts[tree], data=tree.attributes.get("data", None))
    for i, child in enumerate(tree.children):  # type: ignore
        child: domtree.Node = child  # type: ignore
        ii = child.attributes["id"] if "id" in child.attributes else str(i)
        build_index(layout, child, layouts, prefix + [ii], index)
    return index


class Layout:
    def __init__(
        self,
        tree: domtree.Node,
        width: Optional[float] = None,
        height: Optional[float] = None,
    ):
        check_attributes(tree)
        out = compute_layout(tree, width, height)  # type: ignore
        self.index: dict[str, Box] = build_index(self, tree, out, index={})

        root = self.index["/"]
        self.width = root.width
        self.height = root.height
        self.x = root.x
        self.y = root.y

    def __getitem__(self, key: str):
        return self.index[key]

    def keys(self):
        return self.index.keys()

    def items(self):
        return self.index.items()

    def glob(self, pattern: str):
        return (
            self.index[key]
            for key in self.index.keys()
            if pathlib.PurePath(key).match(pattern)
        )



class Box(NamedTuple):
    layout: "Layout"
    name: str
    width: float
    height: float
    top: float
    bottom: float
    left: float
    right: float
    data: Optional[Any]

    def __call__(self, x, y):
        return np.array([self.x(x), self.y(y)])

    def x(self, a):
        return a * self.right + (1 - a) * self.left

    def y(self, a):
        return a * self.top + (1 - a) * self.bottom

    def glob(self, pattern):
        return self.layout.glob(self.name + pattern)

    def __getitem__(self, name):
        return self.layout[self.name + name]

    @classmethod
    def wrap(cls, layout, name, boxdata, data=None):
        return cls(
            layout,
            name,
            boxdata.width,
            boxdata.height,
            boxdata.top,
            boxdata.bottom,
            boxdata.left,
            boxdata.right,
            data,
        )


def layout(**attributes):
    def inner(*args, **kwargs):
        tree = domtree.Node("div")(**attributes)(*args, **kwargs)
        return Layout(tree=tree)

    return inner


allowed_attributes = {
    "align_content",
    "align_items",
    "align_self",
    "aspect_ratio",
    "bottom",
    "direction",
    "flex_basis",
    "flex_direction",
    "flex_grow",
    "flex_shrink",
    "flex_wrap",
    "height",
    "justify_content",
    "left",
    "margin",
    "margin_bottom",
    "margin_left",
    "margin_right",
    "margin_top",
    "max_height",
    "max_width",
    "min_height",
    "min_width",
    "padding",
    "padding_bottom",
    "padding_left",
    "padding_right",
    "padding_top",
    "position",
    "right",
    "top",
    "width",
    "id",
    "data",
}


def check_attributes(tree):
    for attribute in tree.attributes.keys():
        if not attribute in allowed_attributes:
            allowed_list = list(allowed_attributes)
            rel_distances = [
                _levenshtein_edit_distance(attribute, key) / len(key)
                for key in allowed_list
            ]
            indices = np.argsort(rel_distances)[:3]
            options = ", ".join([str(allowed_list[i]) for i in indices])
            raise ValueError(
                f"Attribute '{attribute}' is invalid. Did you mean " + options + "?"
            )
    for child in tree.children:
        check_attributes(child)


def _levenshtein_edit_distance(seq1, seq2):
    """
    Copied from Frank Hofmann, https://stackabuse.com/levenshtein-distance-and-text-similarity-in-python/
    """
    size_x = len(seq1) + 1
    size_y = len(seq2) + 1
    matrix = np.zeros((size_x, size_y))
    for x in range(size_x):
        matrix[x, 0] = x
    for y in range(size_y):
        matrix[0, y] = y

    for x in range(1, size_x):
        for y in range(1, size_y):
            if seq1[x - 1] == seq2[y - 1]:
                matrix[x, y] = min(
                    matrix[x - 1, y] + 1, matrix[x - 1, y - 1], matrix[x, y - 1] + 1
                )
            else:
                matrix[x, y] = min(
                    matrix[x - 1, y] + 1, matrix[x - 1, y - 1] + 1, matrix[x, y - 1] + 1
                )
    return matrix[size_x - 1, size_y - 1]


div = domtree.Node("div")
