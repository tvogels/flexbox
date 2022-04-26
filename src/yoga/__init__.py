import domtree
from typing import Optional
from ._core import *

def build_index(tree: domtree.Node, layouts: dict, prefix=[], idx={}):
    name = "/" + "/".join(prefix)
    idx[name] = layouts[tree]
    for i, child in enumerate(tree.children):  # type: ignore
        child: domtree.Node = child  # type: ignore
        ii = child.attributes["id"] if "id" in child.attributes else str(i)
        build_index(child, layouts, prefix + [ii], idx)
    return idx
    

def layout(tree: domtree.Node, width: Optional[float] = None, height: Optional[float] = None):
    out = compute_layout(tree, width, height)  # type: ignore
    index = build_index(tree, out)
    return index

div = domtree.Node("div")
