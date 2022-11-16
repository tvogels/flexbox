import numpy as np

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


def check_attributes(tree, recursive=True):
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

    if recursive:
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
