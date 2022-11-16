import unittest

from flexbox import box, compute_layout


class TestLayout(unittest.TestCase):
    def test_simple_layout(self):
        layout = compute_layout(box(width=300, height=300))

        self.assertEqual(layout.width, 300)
        self.assertEqual(layout.height, 300)
        self.assertEqual(layout.top, 0)
        self.assertEqual(layout.left, 0)
        self.assertEqual(layout.bottom, 300)
        self.assertEqual(layout.right, 300)

    def test_row_layout(self):
        layout = compute_layout(
            box(box(id="a", width=100, height=100), box(id="b", width=200, height=100))
        )

        self.assertEqual(layout.width, 300)
        self.assertEqual(layout.height, 100)
        self.assertEqual(layout["/a"].left, 0)
        self.assertEqual(layout["/b"].left, 100)

    def test_column_layout(self):
        layout = compute_layout(
            box(flex_direction="column")(
                box(id="a", width=100, height=100), box(id="b", width=200, height=100)
            )
        )

        self.assertEqual(layout.width, 200)
        self.assertEqual(layout.height, 200)
        self.assertEqual(layout["/a"].top, 0)
        self.assertEqual(layout["/b"].top, 100)

    def test_glob(self):
        layout = compute_layout(box(box(width=10, height=10) for _ in range(10)))

        child_layouts = list(layout.glob("/*"))

        self.assertEqual(len(child_layouts), 10)
