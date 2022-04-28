import domtree
from typing import Any, Callable, Optional, NamedTuple, TypeVar
from ._core import *
import pathlib
import numpy as np


class Box(NamedTuple):
    width: float
    height: float
    top: float
    bottom: float
    left: float
    right: float
    x: Callable[[float], float]
    y: Callable[[float], float]

class BoxInfo(NamedTuple):
    box: Box
    data: Optional[Any]




def build_index(tree: domtree.Node, layouts: dict, prefix=[], index={}):
    name = "/" + "/".join(prefix)
    index[name] = BoxInfo(wrap(layouts[tree]), tree.attributes.get("data", None))
    for i, child in enumerate(tree.children):  # type: ignore
        child: domtree.Node = child  # type: ignore
        ii = child.attributes["id"] if "id" in child.attributes else str(i)
        build_index(child, layouts, prefix + [ii], index)
    return index



def wrap(layout) -> Box:
    x = lambda a: a * layout.right + (1-a) * layout.left
    y = lambda a: a * layout.top + (1-a) * layout.bottom
    return Box(layout.width, layout.height, layout.top, layout.bottom, layout.left, layout.right, x, y)

class Layout:
    def __init__(
        self,
        tree: domtree.Node,
        width: Optional[float] = None,
        height: Optional[float] = None,
    ):
        out = compute_layout(tree, width, height)  # type: ignore
        self.index: dict[str, BoxInfo] = build_index(tree, out, index={})

        root = self.index["/"].box
        self.width = root.width
        self.height = root.height
        self.x = root.x
        self.y = root.y

    def __getitem__(self, key: str):
        return self.index[key]

    def glob(self, pattern: str):
        return (
            self.index[key]
            for key in self.index.keys()
            if pathlib.PurePath(key).match(pattern)
        )


def layout(**attributes):
    def inner(*args, **kwargs):
        tree = domtree.Node("div")(**attributes)(
            *args, **kwargs
        )
        return Layout(tree=tree
        )
    return inner


div = domtree.Node("div")
