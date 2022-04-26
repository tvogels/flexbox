import yoga

tree = {
    "children": [
        {
            "width": 50,
            "height": 50.,
        },
        {
            "width": 100,
            "height": 25,
        },
        {
            "width": 50,
            "height": 30,
        }
    ]
}
out = yoga.compute_layout(tree)

def print_tree(tree, indent=0):
    tree = {**tree}
    children = tree.pop("children", [])
    print(" " * indent + str(tree))
    for c in children:
        print_tree(c, indent=indent+4)

print_tree(tree)