import pathlib
from typing import Optional, TypeVar, Generator
from typing_extensions import Self

import domtree
import numpy as np

from ._core import *
from .attributes import check_attributes

T = TypeVar("T", float, np.ndarray)


class BoxSpec(domtree.Node):
    def layout(
        self, width: Optional[float] = None, height: Optional[float] = None
    ):
        if "id" in self.attributes:
            raise ValueError(
                "The `id` of the root node in the layout is not used. The root node will always be called '/'."
            )
        check_attributes(self)
        self.make_nodes_unique(self)
        cpp_layout = compute_layout(self, width, height)  # type: ignore
        node_index = self.object_by_path_index(self)
        index = {key: cpp_layout[node] for key, node in node_index.items()}
        return BoxResolved(index, "")

    @classmethod
    def object_by_path_index(
        cls, tree: Self, prefix: list[str] = [""]
    ) -> dict[str, Self]:
        name = "/".join(prefix)
        index = {name: tree}
        for i, child in enumerate(tree.children):  # type: ignore
            child: BoxSpec = child  # type: ignore
            ii = child.attributes["id"] if "id" in child.attributes else str(i)
            for key, value in cls.object_by_path_index(child, prefix + [ii]).items():
                if key in index:
                    raise ValueError(f"Duplicate layout id {key}")
                index[key] = value

        return index

    @classmethod
    def make_nodes_unique(cls, tree: Self, used=set()):
        used.add(tree)
        for i, child in enumerate(tree.children):
            if not isinstance(child, BoxSpec):
                raise ValueError("All nodes in the layout tree must be boxes.")
            if child in used:
                child = child()
                tree.children[i] = child
            cls.make_nodes_unique(child, used)


class BoxResolved:
    __slots__ = ["index", "prefix", "left", "right", "top", "bottom", "width", "height"]

    def __init__(self, index, prefix):
        self.index = index
        self.prefix = prefix
        layout = self.index[self.prefix]
        self.left = layout.left
        self.right = layout.right
        self.top = layout.top
        self.bottom = layout.bottom
        self.width = layout.width
        self.height = layout.height

    def __getitem__(self, key: str) -> Self:
        full_key = self.prefix + key
        if not full_key in self.index:
            raise ValueError(f"Could not find node {full_key}.")
        return BoxResolved(self.index, full_key)

    def __call__(self, x, y) -> np.ndarray:
        return np.array([self.x(x), self.y(y)])

    def x(self, a: T) -> T:
        return a * self.right + (1 - a) * self.left

    def y(self, a: T) -> T:
        return a * self.top + (1 - a) * self.bottom

    def __truediv__(self, key) -> Self:
        return self["/" + str(key)]

    def keys(self) -> Generator[str, None, None]:
        for key in self.index.keys():
            if key.startswith(self.prefix):
                yield key[len(self.prefix):]

    def items(self) -> Generator[tuple[str, Self], None, None]:
        for key in self.keys():
            yield (key, self[key])

    def glob(self, pattern: str) -> Generator[Self, None, None]:
        return (
            BoxResolved(self.index, key)
            for key in self.index.keys()
            if pathlib.PurePath(key).match(self.prefix + pattern)
        )

box = BoxSpec("box")
