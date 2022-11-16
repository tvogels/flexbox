from __future__ import annotations

import base64
import pathlib
from typing import Generator, Optional, TypeVar

import domtree
import numpy as np
from domtree import svg
from typing_extensions import Self

import flexbox._core as core

from .attributes import check_attributes

T = TypeVar("T", float, np.ndarray)


class BoxSpec(domtree.Node):
    def layout(self, width: float | None = None, height: float | None = None):
        if "id" in self.attributes:
            raise ValueError(
                "The `id` of the root node in the layout is not used. The root node will always be called '/'."
            )
        check_attributes(self)
        self.make_nodes_unique(self)
        cpp_layout = core.compute_layout(self, width, height)  # type: ignore
        node_index = self.object_by_path_index(self)
        index = {key: cpp_layout[node] for key, node in node_index.items()}
        return BoxResolved(index, node_index, "")

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
    __slots__ = [
        "index",
        "node_index",
        "prefix",
        "data",
        "left",
        "right",
        "top",
        "bottom",
        "width",
        "height",
    ]

    def __init__(self, index, node_index, prefix):
        self.index = index
        self.node_index = node_index
        self.prefix = prefix
        layout = self.index[self.prefix]
        self.left = layout.left
        self.right = layout.right
        self.top = layout.top
        self.bottom = layout.bottom
        self.width = layout.width
        self.height = layout.height
        self.data = self.node_index[self.prefix].attributes.get("data", None)

    def __getitem__(self, key: str) -> Self:
        full_key = self.prefix + key
        if not full_key in self.index:
            raise ValueError(f"Could not find node {full_key}.")
        return BoxResolved(self.index, self.node_index, full_key)

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
                yield key[len(self.prefix) :]

    def items(self) -> Generator[tuple[str, Self], None, None]:
        for key in self.keys():
            yield (key, self[key])

    def glob(self, pattern: str) -> Generator[Self, None, None]:
        return (
            BoxResolved(self.index, self.node_index, key)
            for key in self.index.keys()
            if pathlib.PurePath(key).match(self.prefix + pattern)
        )

    def _repr_html_(self):
        rect = svg.rect(fill="rgba(0,0,0,0.03)", stroke="#ccc")
        figure = svg.svg(
            width=self.width,
            height=self.height,
            font_family="-apple-system, BlinkMacSystemFont, avenir next, avenir, helvetica, helvetica neue, ubuntu, roboto, noto, segoe ui, arial, sans-serif",
            stroke_linecap="round",
        )(
            svg.g(id=name)(
                rect(x=box.left, y=box.top, width=box.width, height=box.height),
                svg.text(
                    x=box.left,
                    y=box.top,
                    font_size=10,
                    dy=14,
                    dx=4,
                    fill="rgba(0,0,0,0.7)",
                )(name),
            )
            for name, box in self.items()
        )
        b64 = base64.b64encode(str(figure).encode("utf-8")).decode("ascii")
        return (
            '<img src="data:image/svg+xml;base64,'
            + b64
            + '" style="background-color: white; max-width:100%; margin: 0.5em 0;" />'
        )


def compute_layout(
    box: BoxSpec, width: float | None = None, height: float | None = None
) -> BoxResolved:
    return box.layout(width, height)


box = BoxSpec("box")
